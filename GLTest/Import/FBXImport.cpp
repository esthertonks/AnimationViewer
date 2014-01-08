#include "FBXImport.h"
#include "../ImportMesh/Mesh.h"
#include "../ImportMesh/MeshNode.h"
#include "../ImportMesh/BoneNode.h"
#include "../ImportMesh/Triangle.h"
#include "../ImportMesh/Vertex.h"
#include "../Batch/LambertAppearance.h"
#include "../Batch/PhongAppearance.h"
#include "../Utils/MathsUtils.h"
#include "../Animation/VectorKey.h"
#include "../Animation/QuaternionKey.h"

#include <assert.h>

#include <wx/log.h>

namespace import
{
	const std::string FBXImport::m_dummyTexture("Blank.tga");

FBXImport::FBXImport()
: m_fbxManager(NULL),
m_fbxScene(NULL),
m_mesh(NULL)
{



}

FBXImport::~FBXImport()
{


}

mesh::MeshPtr FBXImport::Import(
	const std::string &fbxFilename
	)
{

	m_fbxManager = FbxManager::Create();
	if(!m_fbxManager)
	{
		FBXSDK_printf("Can't create fbx manager.\n");
		return NULL;
	}

	// Set up the import settings
	FbxIOSettings* ioSettings = FbxIOSettings::Create(m_fbxManager, IOSROOT);
	m_fbxManager->SetIOSettings(ioSettings);

	//Create an FBX scene which holds the imported objects
	m_fbxScene = FbxScene::Create(m_fbxManager, "My Scene");
	if(!m_fbxScene)
	{
		FBXSDK_printf("Can't create fbx scene.\n");
		return NULL;
	}

	m_fbxImporter = FbxImporter::Create(m_fbxManager,"");

	if(!m_fbxImporter->Initialize(fbxFilename.c_str(), -1, ioSettings))
	{
		FbxString error = m_fbxImporter->GetStatus().GetErrorString();
		FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
		FBXSDK_printf("FBX Import failed with error: %s\n", error.Buffer());

		if (m_fbxImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
		{
			FBXSDK_printf("FBX file format version for file '%s' is not valid for this SDK version\n", fbxFilename);
			return NULL;
		}

		m_fbxImporter->Destroy();
		m_fbxScene->Destroy();
		ioSettings->Destroy();
		m_fbxManager->Destroy();
	}

	if(!m_fbxImporter->IsFBX())
	{
		FBXSDK_printf("File %s is not an FBX file. \n", fbxFilename);
		m_fbxImporter->Destroy();
		m_fbxScene->Destroy();
		ioSettings->Destroy();
		m_fbxManager->Destroy();
		return NULL;
	}

	// Import the scene.
	if(!m_fbxImporter->Import(m_fbxScene))
	{
		FBXSDK_printf("Import failed for file: %s \n", fbxFilename);
		m_fbxImporter->Destroy();
		m_fbxScene->Destroy();
		ioSettings->Destroy();
		m_fbxManager->Destroy();
		return NULL;
	}

	// Fill the mesh with the imported data
	m_mesh = mesh::MeshPtr(new mesh::Mesh());
	FbxNode &fbxRootNode = *m_fbxScene->GetRootNode();

	LoadAnimationLayerInfo();
	animation::AnimationInfo &animationInfo = m_mesh->GetAnimationInfo();

	// Bake all FBX transforms ie pivots and offsets into standard scale, rotation etc
	// See http://docs.autodesk.com/FBX/2014/ENU/FBX-SDK-Documentation/index.html?url=files/GUID-C35D98CB-5148-4B46-82D1-51077D8970EE.htm,topicNumber=d30e8813
	BakeNodeTransforms(fbxRootNode);
	fbxRootNode.ConvertPivotAnimationRecursive(NULL, FbxNode::eDestinationPivot, animationInfo.GetFPS());	// FPS fixed at 30 atm

	LoadNodes(fbxRootNode, m_mesh->GetNodeHierarchy());

	// Now the all the animation is loaded adjust the time to make sure that it starts at zero //TODO function
	animationInfo.SetEndSample(animationInfo.GetEndSample() - animationInfo.GetStartSample());
	animationInfo.SetStartSample(0);

	// TODO revome duplicate keys

	m_fbxImporter->Destroy();
	m_fbxScene->Destroy();
	ioSettings->Destroy();
	m_fbxManager->Destroy();

	return m_mesh;
}

bool FBXImport::LoadNodes(
	FbxNode& fbxNode,
	mesh::Node *parent
	)
{
	mesh::Node *newNode = NULL;

	// Find ut the type of node ie Skeleton, Mesh, Camera, Light - currently only Bones and Mesh nodes are supported
	FbxNodeAttribute* const fbxNodeAttribute = fbxNode.GetNodeAttribute();
	if(fbxNodeAttribute)
	{
		const FbxNodeAttribute::EType fbxAttributeType = fbxNodeAttribute->GetAttributeType();

		switch(fbxAttributeType)
		{
		case FbxNodeAttribute::eMesh:
			newNode = LoadMeshNode(fbxNode, parent);
			break;
		case FbxNodeAttribute::eSkeleton:
			newNode = LoadBoneNode(fbxNode, parent);
			break;
		default:
			FBXSDK_printf("Node %s type is %d. Only node of type eMesh (4) or eSkeleton (3) can be loaded\n", fbxNode.GetName(), fbxAttributeType);

			break;

		}

	}

	for(int childIndex = 0; childIndex < fbxNode.GetChildCount(); childIndex++)
	{
		FbxNode &fbxChildNode = *fbxNode.GetChild(childIndex);

		LoadNodes(fbxChildNode, newNode != NULL ? newNode : parent); // If we didnt load a node just pass through the parent. Otherwise pass the new node as the parent
	}

	return true;
}

mesh::Node *FBXImport::LoadMeshNode(
	FbxNode& fbxNode,
	mesh::Node *parent
	)
{
	FbxMesh* fbxMesh = fbxNode.GetMesh();
	mesh::MeshNode *meshNode = NULL;
	if(fbxMesh)
	{
		if(!fbxMesh->IsTriangleMesh())
		{
			// Make sure the mesh is triangulated
			FbxGeometryConverter fbxGeometryConverter(m_fbxManager);	
			FbxNodeAttribute* fbxNodeAttribute = fbxGeometryConverter.Triangulate(fbxNode.GetMesh(), true);
			if(!fbxNodeAttribute)
			{
				FBXSDK_printf("Mesh Triangulation failed. Node Import aborted.\n");
				return NULL;
			}
			fbxMesh = (FbxMesh *)fbxNodeAttribute;
		}

		meshNode = new mesh::MeshNode();
		m_mesh->AddChildNode(parent, meshNode);

		std::string name = fbxNode.GetName();
		meshNode->SetName(name);

		// Extract and store vertices
		const unsigned int numVertices = fbxMesh->GetControlPointsCount();
		meshNode->AllocateVertices(numVertices);
		mesh::MeshVertexArray vertexArray = meshNode->GetVertices();

		const FbxVector4* const fbxVertices = fbxMesh->GetControlPoints();
		for(unsigned int vertexIndex = 0; vertexIndex < numVertices; vertexIndex++)
		{
			fbxVertices[vertexIndex];

			vertexArray[vertexIndex].m_position.x = static_cast<float>(fbxVertices[vertexIndex][0]);
			vertexArray[vertexIndex].m_position.y = static_cast<float>(fbxVertices[vertexIndex][1]);
			vertexArray[vertexIndex].m_position.z = static_cast<float>(fbxVertices[vertexIndex][2]);
		}

		// Extract and store triangles
		int numTriangles = fbxMesh->GetPolygonCount();

		meshNode->AllocateTriangles(numTriangles);
		mesh::MeshTriangleArray triangleArray = meshNode->GetTriangles();

			// Get the per triangle material index
		const int materialLayerCount = fbxMesh->GetElementMaterialCount();
		if(materialLayerCount > 1)
		{
			FBXSDK_printf("Only one material layer is supported. Only the first layer will be loaded.\n");
		}

		// Load per triangle data
		for(int triangleIndex = 0; triangleIndex < numTriangles; triangleIndex++)
		{
			int triangleSize = fbxMesh->GetPolygonSize(triangleIndex);
			assert(triangleSize == 3); // The mesh was triangulated above.

			mesh::Triangle &triangle = triangleArray[triangleIndex];

			if(materialLayerCount != 0)
			{
				LoadMaterials(*fbxMesh, triangleIndex, *meshNode, m_mesh->GetAppearanceTable(), m_mesh->GetNumVerticesPerMaterialArray());
			}
			else
			{
				//TODO assign default material
			}

			LoadVertexIndices(*fbxMesh, triangleIndex, triangle);

			LoadColours(*fbxMesh, triangleIndex, triangle);
			LoadUVs(*fbxMesh, triangleIndex, triangle);

			LoadNormals(*fbxMesh, triangleIndex, triangle);
			LoadBinormals(*fbxMesh, triangleIndex, triangle);		
			LoadTangents(*fbxMesh, triangleIndex, triangle);
		}
	}

	return meshNode;
}

mesh::Node *FBXImport::LoadBoneNode(
	FbxNode& fbxNode, // The FBX mesh to extract data from and add to m_mesh bone node list
	mesh::Node *parent
	)
{
	const FbxSkeleton* const fbxSkeleton = static_cast<const FbxSkeleton*>(fbxNode.GetNodeAttribute());
	mesh::BoneNode *boneNode = NULL;
	if (!fbxSkeleton)
	{
		FBXSDK_printf("Could not extract skeleton from node");
		return NULL;
	}

	// Add the bone to the mesh as a parent->child list
	boneNode = new mesh::BoneNode;

	// Find out and record whether this is a leaf node (in case we need to know the hierarchy and not just what the parent was)
	//const FbxSkeleton::EType skeletonType = fbxSkeleton->GetSkeletonType();
	//switch(skeletonType)
	//{
	//case FbxSkeleton::eLimb: // This is a leaf node ie the node is only connected to one other node

	//	break;

	//case FbxSkeleton::eLimbNode:// This is a limb node ie the node is connected to two other nodes

	//	break;

	//case FbxSkeleton::eRoot:// This node is not usually drawn. Discarding the node.
	//case FbxSkeleton::eEffector:// This node is not usually drawn. Discarding the node.

	//	return false;
	//}

	m_mesh->AddChildNode(parent, boneNode);

	std::string name = fbxNode.GetName();
	boneNode->SetName(name);

	// Store the initial global pose values. The rest will be calulated from the local transforms //TODO do we even need to load the global transforms at all?
	const FbxAMatrix fbxGlobalTransform = fbxNode.EvaluateGlobalTransform(0, FbxNode::eDestinationPivot);
	glm::mat4x4 globalTransform;
	utils::MathsUtils::ConvertFBXToGLMatrix(fbxGlobalTransform, globalTransform);
	boneNode->m_globalTransform = globalTransform;

	//TODO temp to debug
	glm::mat4x4 initialLocalTransform;
	const FbxAMatrix fbxInitialLocalTransform = fbxNode.EvaluateLocalTransform(0, FbxNode::eDestinationPivot);
	utils::MathsUtils::ConvertFBXToGLMatrix(fbxInitialLocalTransform, initialLocalTransform);

	boneNode->m_localTransform = initialLocalTransform;

	animation::AnimationInfo &animationInfo = m_mesh->GetAnimationInfo();
	// Load in the local keys transoforms for each key
	int numSamples = animationInfo.GetNumFrameSamples();
	double frameRate = animationInfo.GetFPS();

	if(numSamples)
	{
		boneNode->AllocateAnimationTracks(numSamples);
	}

	int startFrame = animationInfo.GetStartSample();
	long startTime = animationInfo.ConvertFrameToMilliseconds(startFrame);
	FbxTime fbxTime;

	for(int sample = 0; sample <= numSamples; sample++)
	{
		long sampleTime = animationInfo.ConvertFrameToMilliseconds(sample); // Get the number of milliseconds into the animation for this sample frame (starting at 0)
		fbxTime.SetMilliSeconds(startTime + sampleTime); // Find the current time in the FBX file (starting at anim start time)

		const FbxAMatrix fbxLocalTransform = fbxNode.EvaluateLocalTransform(fbxTime, FbxNode::eDestinationPivot);

		FbxVector4 fbxScale = fbxLocalTransform.GetS();
		FbxVector4 fbxPosition = fbxLocalTransform.GetT();
		FbxQuaternion fbxRotation = fbxLocalTransform.GetQ();
		FbxVector4 rotTestTemp = fbxLocalTransform.GetR();//TODO remove

		// Store the keys with the adjusted time (ie the time starting at 0 regardless or where it started in the FBX file
		boost::shared_ptr<animation::VectorKey> scale(new animation::VectorKey(fbxScale[0], fbxScale[1], fbxScale[2], sampleTime));
		boost::shared_ptr<animation::VectorKey> position(new animation::VectorKey(fbxPosition[0], fbxPosition[1], fbxPosition[2], sampleTime));

		// glm quat constructor expects w, x, y, z. FBX is x, y, z, w. glm nontheless stores x, y, z, w internally
		boost::shared_ptr<animation::QuaternionKey> rotation(new animation::QuaternionKey(fbxRotation[0], fbxRotation[1], fbxRotation[2], fbxRotation[3], sampleTime));

		boneNode->AddLocalKeyTransform(position, rotation, scale);
	}

	// Record node scale inheritance //TODO scale inheritance
	if (parent)
	{
		FbxEnum inheritType = fbxNode.InheritType.Get();
		switch(inheritType)
		{
		case FbxTransform::eInheritRrs:
			boneNode->SetInheritScale(false);
			break;
		case FbxTransform::eInheritRrSs:
			FBXSDK_printf("Unsupported scale type used");
			break;
		case FbxTransform::eInheritRSrs:
			boneNode->SetInheritScale(true);
			break;
		}
	}
	else
	{
		boneNode->SetInheritScale(false);// No parent, cant inherit scale
	}

	return boneNode;
}

void FBXImport::LoadAnimationLayerInfo()
{
	// Get the fps and calculate the number of frames that should be loaded
	animation::AnimationInfo &animationInfo = m_mesh->GetAnimationInfo();

	const FbxTakeInfo &takeInfo = *m_fbxImporter->GetTakeInfo(0);

	// Set the frame rate as millisecond to frame conversion is based on this
	double frameRate = 30.0f;
	FbxTime::EMode fbxTimeMode;
	if (m_fbxImporter->GetFrameRate(fbxTimeMode))
	{
		frameRate = FbxTime::GetFrameRate(fbxTimeMode);
	}

	animationInfo.SetFPS(frameRate);

	// Set the start and end frame. Use frame and not time to prevent rounding errors
	const long startTime = takeInfo.mLocalTimeSpan.GetStart().GetMilliSeconds();
	const long endTime = takeInfo.mLocalTimeSpan.GetStop().GetMilliSeconds();
	animationInfo.SetStartSample(animationInfo.ConvertMillisecondsToFrame(startTime));
	animationInfo.SetEndSample(animationInfo.ConvertMillisecondsToFrame(endTime));

	int numFrames = animationInfo.GetEndSample() - animationInfo.GetStartSample() - 1; // This will return the number of frame samples ie |-|-|-|-| (where | is a sample). So -1 for the number of frames (where - is a frame)

	animationInfo.SetNumFrames(numFrames);
}

/**
	\brief Bake all possible FBX transforms into the statndard rotation, trnaslate and scale variables so the can be extracted easily later
*/

void FBXImport::BakeNodeTransforms(
	FbxNode &node		//!< Node to bake transforms for
	) const
{
	FbxVector4 zeroVector(0,0,0);
 
	// Activate pivot converting 
	node.SetPivotState(FbxNode::eSourcePivot, FbxNode::ePivotActive); 
	node.SetPivotState(FbxNode::eDestinationPivot, FbxNode::ePivotActive); 
 
	// We want to set all these to 0 and bake them into the transforms. 
	node.SetPostRotation(FbxNode::eDestinationPivot, zeroVector); 
	node.SetPreRotation(FbxNode::eDestinationPivot, zeroVector); 
	node.SetRotationOffset(FbxNode::eDestinationPivot, zeroVector); 
	node.SetScalingOffset(FbxNode::eDestinationPivot, zeroVector); 
	node.SetRotationPivot(FbxNode::eDestinationPivot, zeroVector); 
	node.SetScalingPivot(FbxNode::eDestinationPivot, zeroVector); 
 
	// This is to import in a system that supports rotation order. 
	// If rotation order is not supported, do this instead: 
	node.SetRotationOrder(FbxNode::eDestinationPivot, eEulerXYZ); 
 
	// Similarly, this is the case where geometric transforms are supported by the system. 
	// If geometric transforms are not supported, set them to zero instead of the source’s geometric transforms. 
	// Geometric transform = local transform, not inherited by children. 
	node.SetGeometricTranslation(FbxNode::eDestinationPivot, node.GetGeometricTranslation(FbxNode::eSourcePivot)); 
	node.SetGeometricRotation(FbxNode::eDestinationPivot, node.GetGeometricRotation(FbxNode::eSourcePivot)); 
	node.SetGeometricScaling(FbxNode::eDestinationPivot, node.GetGeometricScaling(FbxNode::eSourcePivot)); 
 
	// Idem for quaternions. 
	node.SetQuaternionInterpolation(FbxNode::eDestinationPivot, node.GetQuaternionInterpolation(FbxNode::eSourcePivot));

	// Recurse on children
	const int childCount = node.GetChildCount();
	for(int childNum = 0; childNum < childCount; childNum++)
		BakeNodeTransforms(*node.GetChild(childNum));
}

/**
	@brief Loads a material id for the given triangle
*/
void FBXImport::LoadMaterials(
	FbxMesh &fbxMesh,			// FBX mesh to import data from
	int triangleIndex,			// Index of the current triangle being loaded
	mesh::MeshNode &meshNode,	// The mesh node which will store the imported material and texture data
	render::AppearanceTable &appearanceTable,
	std::vector<unsigned int> &numVerticesPerMaterial

	)
{
	const FbxGeometryElementMaterial &materialElement = *fbxMesh.GetElementMaterial(0); // Get the first material layer element. Only one is supported.
	const int materialId = materialElement.GetIndexArray().GetAt(triangleIndex);
	if (materialId >= 0)
	{
		const FbxSurfaceMaterial& surfaceMaterial = *fbxMesh.GetNode()->GetMaterial(materialId);
		meshNode.m_triangleArray[triangleIndex].m_materialId = materialId;

		if(appearanceTable.count(materialId) == 0)
		{
			numVerticesPerMaterial.push_back(0);// Add another int initialised at 0 for this material
			render::AppearancePtr appearance;

			if (surfaceMaterial.GetClassId().Is(FbxSurfacePhong::ClassId))
			{
				appearance = render::AppearancePtr(new render::PhongAppearance());
					
				const FbxSurfacePhong& phongMaterial = (const FbxSurfacePhong &)surfaceMaterial;
				glm::vec3 ambientColour(phongMaterial.Ambient.Get()[0], phongMaterial.Ambient.Get()[1], phongMaterial.Ambient.Get()[3]);
				glm::vec3 diffuseColour(phongMaterial.Diffuse.Get()[0], phongMaterial.Diffuse.Get()[1], phongMaterial.Diffuse.Get()[3]);
				glm::vec3 specularColour(phongMaterial.Specular.Get()[0], phongMaterial.Specular.Get()[1], phongMaterial.Specular.Get()[3]);
				glm::vec3 emmissiveColour(phongMaterial.Emissive.Get()[0], phongMaterial.Emissive.Get()[1], phongMaterial.Emissive.Get()[3]);

				double transparency = phongMaterial.TransparencyFactor.Get();
				double shininess = phongMaterial.Shininess.Get();
				double reflectivity = phongMaterial.ReflectionFactor.Get();
				double diffuseFactor = phongMaterial.DiffuseFactor.Get();

				boost::shared_ptr<render::PhongAppearance> phongAppearancePtr = boost::static_pointer_cast<render::PhongAppearance>(appearance);

				phongAppearancePtr->SetAmbient(ambientColour);
				phongAppearancePtr->SetDiffuse(diffuseColour);
				phongAppearancePtr->SetSpecular(specularColour);
				phongAppearancePtr->SetEmissive(emmissiveColour);

				phongAppearancePtr->SetTransparency(transparency);
				phongAppearancePtr->SetShininess(shininess);
				phongAppearancePtr->SetReflectivity(reflectivity);
				phongAppearancePtr->SetDiffuseFactor(diffuseFactor);
			}
			else if(surfaceMaterial.GetClassId().Is(FbxSurfaceLambert::ClassId))
			{
				appearance = render::AppearancePtr(new render::LambertAppearance());

				const FbxSurfaceLambert& lambertMaterial = (const FbxSurfaceLambert &)surfaceMaterial;

				glm::vec3 ambientColour(lambertMaterial.Ambient.Get()[0], lambertMaterial.Ambient.Get()[1], lambertMaterial.Ambient.Get()[3]);
				glm::vec3 diffuseColour(lambertMaterial.Diffuse.Get()[0], lambertMaterial.Diffuse.Get()[1], lambertMaterial.Diffuse.Get()[3]);
				glm::vec3 emmissiveColour(lambertMaterial.Emissive.Get()[0], lambertMaterial.Emissive.Get()[1], lambertMaterial.Emissive.Get()[3]);

				double transparency = lambertMaterial.TransparencyFactor.Get();
				double diffuseFactor = lambertMaterial.DiffuseFactor.Get();
				boost::shared_ptr<render::LambertAppearance> lambertAppearancePtr = boost::static_pointer_cast<render::LambertAppearance>(appearance);

				lambertAppearancePtr->SetAmbient(ambientColour);
				lambertAppearancePtr->SetDiffuse(diffuseColour);
				lambertAppearancePtr->SetEmissive(emmissiveColour);

				lambertAppearancePtr->SetTransparency(transparency);
				lambertAppearancePtr->SetDiffuseFactor(diffuseFactor);
			}
			else
			{
				FBXSDK_printf("Material Id %d, name &s is not supported", materialId, surfaceMaterial.GetName());
				//TODO assign default material
			}

			// Just get the diffuse for now. Will load normal/bump and other textures here in future.
			FbxProperty materialProperty = surfaceMaterial.FindProperty(FbxSurfaceMaterial::sDiffuse);
			unsigned int textureCount = materialProperty.GetSrcObjectCount<FbxTexture>();
			if(textureCount > 1)
			{
				FBXSDK_printf("Currently only 1 diffuse texture is supported. Others have not been loaded", materialId, surfaceMaterial.GetName());
			}
			for(int textureIndex = 0; textureIndex < 1; textureIndex++)
			{
				FbxFileTexture* fbxFileTexture = materialProperty.GetSrcObject<FbxFileTexture>(textureIndex);
				if(fbxFileTexture != NULL)
				{
					std::string textureFilename = fbxFileTexture->GetFileName();//TODO cant currently support multiple materials!
					appearance->SetDiffuseTexturePath(textureFilename);
				}
				else
				{
					appearance->SetDiffuseTexturePath(m_dummyTexture);
				}
			}

			render::AppearanceTableEntry materialInfo;
			materialInfo.first = materialId;
			materialInfo.second = appearance;
			appearanceTable.insert(materialInfo);
		}
		numVerticesPerMaterial[materialId]+=3;
	}
	else
	{
		FBXSDK_printf("Face %d has no associated material.\n", triangleIndex);
	}
}

/**
	@brief Loads the vertex Indices for the given triangle
*/
void FBXImport::LoadVertexIndices(
	FbxMesh &fbxMesh,			// FBX mesh to import data from
	int triangleIndex,			// Index of the current triangle being loaded
	mesh::Triangle &triangle	// The current triangle to store the imported data
	)
{
	// Get the index into the vertex array for this vertex on this polygon
	for (int triangleCornerId = 0; triangleCornerId < 3; triangleCornerId++)
	{
		int vertexIndex = fbxMesh.GetPolygonVertex(triangleIndex, triangleCornerId);
		triangle.SetVertexIndex(triangleCornerId, vertexIndex);
	}
}

/**
	@brief Loads the colours at each corner of the given triangle
*/
void FBXImport::LoadColours(
	FbxMesh &fbxMesh,			// FBX mesh to import data from
	int triangleIndex,			// Index of the current triangle being loaded
	mesh::Triangle &triangle	// The current triangle to store the imported data
	)
{
	if(fbxMesh.GetElementVertexColorCount() > 1)
	{
		FBXSDK_printf("Only one set of vertex colours supported\n");
	}

	FbxGeometryElementVertexColor* vertexColourElement = fbxMesh.GetElementVertexColor();
	if(vertexColourElement)
	{
		for (int triangleCornerId = 0; triangleCornerId < 3; triangleCornerId++)
		{
			int vertexIndex = fbxMesh.GetPolygonVertex(triangleIndex, triangleCornerId);
			LoadColourVertexElement(*vertexColourElement, triangle.m_colours[triangleCornerId], triangleCornerId, vertexIndex);
		}
	}
}

/**
	@brief Loads the uvs at each corner of the given triangle
*/
void FBXImport::LoadUVs(
	FbxMesh &fbxMesh,			// FBX mesh to import data from
	int triangleIndex,			// Index of the current triangle being loaded
	mesh::Triangle &triangle	// The current triangle to store the imported data
	)
{
	if(fbxMesh.GetElementUVCount() != 1)
	{
		FBXSDK_printf("Mesh must have one sest of uvs. This mesh has %d uv sets.\n", fbxMesh.GetElementUVCount()); // May support more uv sets later
	}

	FbxGeometryElementUV* uvElement = fbxMesh.GetElementUV(/*uvIndex*/);
	if(uvElement)
	{
		for (int triangleCornerId = 0; triangleCornerId < 3; triangleCornerId++)
		{
			int vertexIndex = fbxMesh.GetPolygonVertex(triangleIndex, triangleCornerId);
			LoadVector2VertexElement(*uvElement, triangle.m_uvs[triangleCornerId], triangleIndex, triangleCornerId, vertexIndex);
		}
	}
}

/**
	@brief Loads the normals at each corner of the given triangle
*/
void FBXImport::LoadNormals(
	FbxMesh &fbxMesh,			// FBX mesh to import data from
	int triangleIndex,			// Index of the current triangle being loaded
	mesh::Triangle &triangle	// The current triangle to store the imported data
	)
{
	if(fbxMesh.GetElementNormalCount() > 1)
	{
		FBXSDK_printf("Only one set of normals currently supported\n");
	}

	FbxGeometryElementNormal* normalElement = fbxMesh.GetElementNormal();
	if(normalElement)
	{
		for (int triangleCornerId = 0; triangleCornerId < 3; triangleCornerId++)
		{
			int vertexIndex = fbxMesh.GetPolygonVertex(triangleIndex, triangleCornerId);
			LoadVector4VertexElement(*normalElement, triangle.m_normals[triangleCornerId], (triangleIndex * 3) + triangleCornerId, vertexIndex);
			//wxLogDebug("NormalA %f, %f, %f\n", triangle.m_normals[triangleCornerId].x, triangle.m_normals[triangleCornerId].y, triangle.m_normals[triangleCornerId].z);
		}
	}
}

/**
	@brief Loads the binormals at each corner of the given triangle
*/
void FBXImport::LoadBinormals(
	FbxMesh &fbxMesh,			// FBX mesh to import data from
	int triangleIndex,			// Index of the current triangle being loaded
	mesh::Triangle &triangle	// The current triangle to store the imported data
	)
{
	if(fbxMesh.GetElementBinormalCount() > 1)
	{
		FBXSDK_printf("Only one set of binormals currently supported\n");
	}

	FbxGeometryElementBinormal* binormalElement = fbxMesh.GetElementBinormal();
	if(binormalElement)
	{
		for (int triangleCornerId = 0; triangleCornerId < 3; triangleCornerId++)
		{
			int vertexIndex = fbxMesh.GetPolygonVertex(triangleIndex, triangleCornerId);
			LoadVector4VertexElement(*binormalElement, triangle.m_binormals[triangleCornerId], triangleCornerId, vertexIndex);
		}
	}	
}

/**
	@brief Loads the tangents at each corner of the given triangle
*/
void FBXImport::LoadTangents(
	FbxMesh &fbxMesh,			// FBX mesh to import data from
	int triangleIndex,			// Index of the current triangle being loaded
	mesh::Triangle &triangle	// The current triangle to store the imported data
	)
{
	if(fbxMesh.GetElementTangentCount() > 1)
	{
		FBXSDK_printf("Only one set of tangents currently supported\n");
	}

	FbxGeometryElementTangent* tangentElement = fbxMesh.GetElementTangent();
	if(tangentElement)
	{
		for (int triangleCornerId = 0; triangleCornerId < 3; triangleCornerId++)
		{
			int vertexIndex = fbxMesh.GetPolygonVertex(triangleIndex, triangleCornerId);
			LoadVector4VertexElement(*tangentElement, triangle.m_tangents[triangleCornerId], triangleCornerId, vertexIndex);
		}
	}
}


void FBXImport::LoadVector4VertexElement(
	FbxLayerElementTemplate<FbxVector4> &element,
	glm::vec4 &data, // The current nodes vertex array which will store the imported data
	int triangleCornerId,
	int vertexIndex
	)
{
	switch (element.GetMappingMode())
	{
		case FbxGeometryElement::eByControlPoint:
		{
			switch (element.GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				{
					FbxVector4 fbxData = element.GetDirectArray().GetAt(vertexIndex);
					data[0] = static_cast<float>(fbxData[0]);
					data[1] = static_cast<float>(fbxData[1]);
					data[2] = static_cast<float>(fbxData[2]);
					data[3] = static_cast<float>(fbxData[3]);
				}
				break;
				case FbxGeometryElement::eIndexToDirect:
				{
					int id = element.GetIndexArray().GetAt(vertexIndex);
					FbxVector4 fbxData = element.GetDirectArray().GetAt(id);
					data[0] = static_cast<float>(fbxData[0]);
					data[1] = static_cast<float>(fbxData[1]);
					data[2] = static_cast<float>(fbxData[2]);
					data[3] = static_cast<float>(fbxData[3]);
				}
				break;
			default:
				FBXSDK_printf("Trying to load %s with an unsupported reference mode\n", element.GetName());
			break;
			}
		}
		break;
		case FbxGeometryElement::eByPolygonVertex:
		{
			switch(element.GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				{
					FbxVector4 fbxData = element.GetDirectArray().GetAt(triangleCornerId);
					data[0] = static_cast<float>(fbxData[0]);
					data[1] = static_cast<float>(fbxData[1]);
					data[2] = static_cast<float>(fbxData[2]);
					data[3] = static_cast<float>(fbxData[3]);
				}
				break;

				case FbxGeometryElement::eIndexToDirect:
				{
					int indexId = element.GetIndexArray().GetAt(triangleCornerId);
					FbxVector4 fbxData = element.GetDirectArray().GetAt(indexId);	
					data[0] = static_cast<float>(fbxData[0]);
					data[1] = static_cast<float>(fbxData[1]);
					data[2] = static_cast<float>(fbxData[2]);
					data[3] = static_cast<float>(fbxData[3]);
				}
				break;

				default:
					FBXSDK_printf("Trying to load %s with an unsupported reference mode\n", element.GetName());
					break;
			}
		}
		default:
			FBXSDK_printf("Trying to load %s with an unsupported mapping mode\n", element.GetName());
		break;
	}
}

const unsigned int FBXImport::GetUVVertexIndex(
	const unsigned int triangleIndex, 
	const unsigned int triangleCornerId
	) const
{
	return triangleIndex * 3 + triangleCornerId; // Triangle index * num verts in a triangle + current vertex corner in the triangle
}

void FBXImport::LoadVector2VertexElement(
	FbxLayerElementTemplate<FbxVector2> &element,
	glm::vec2 &data,
	int triangleIndex,									// The index of this triangle in the triangle array
	int triangleCornerId,								// The corner(vertex) of this triangle being refered to
	int vertexIndex										// The index of the vertex in the vertex array
	)
{
	switch (element.GetMappingMode())
	{
		case FbxGeometryElement::eByControlPoint:
		{
			switch (element.GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				{
					FbxVector2 fbxData = element.GetDirectArray().GetAt(vertexIndex);
					data.x = static_cast<float>(fbxData[0]);
					data.y = static_cast<float>(fbxData[1]);
				}
				break;
				case FbxGeometryElement::eIndexToDirect:
				{
					int id = element.GetIndexArray().GetAt(vertexIndex);
					FbxVector2 fbxUvs = element.GetDirectArray().GetAt(id);
					data.x = static_cast<float>(fbxUvs[0]);
					data.y = static_cast<float>(fbxUvs[1]);
				}
				break;
				default:
					FBXSDK_printf("Trying to load %s with an unsupported reference mode\n", element.GetName());
					break;
			}
		}
		break;
		case FbxGeometryElement::eByPolygonVertex:
		{
			switch (element.GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				case FbxGeometryElement::eIndexToDirect:
				{
					unsigned int uvIndex = element.GetIndexArray().GetAt(GetUVVertexIndex(triangleIndex, triangleCornerId));
					FbxVector2 fbxData = element.GetDirectArray().GetAt(uvIndex);
					data.x = static_cast<float>(fbxData[0]);
					data.y = static_cast<float>(fbxData[1]);
				}
				break;
				default:
					FBXSDK_printf("Trying to load %s with an unsupported reference mode\n", element.GetName());
				break;
			}
		}
		break;
		default:
			FBXSDK_printf("Trying to load %s with an unsupported mapping mode\n", element.GetName());
		break;
	}
}

void FBXImport::LoadColourVertexElement(
	FbxLayerElementTemplate<FbxColor> &element,
	glm::vec3 &colour, // The colour to import data into
	int triangleCornerId,
	int vertexIndex
	)
{
	switch (element.GetMappingMode())
	{
		case FbxGeometryElement::eByControlPoint:
			switch (element.GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				{
					FbxColor fbxColour = element.GetDirectArray().GetAt(vertexIndex);
					colour[0] = static_cast<float>(fbxColour[0]);
					colour[1] = static_cast<float>(fbxColour[1]);
					colour[2] = static_cast<float>(fbxColour[2]);
				}
				break;
				case FbxGeometryElement::eIndexToDirect:
				{
					int id = element.GetIndexArray().GetAt(vertexIndex);
					FbxColor fbxColour = element.GetDirectArray().GetAt(id);
					colour[0] = static_cast<float>(fbxColour[0]);
					colour[1] = static_cast<float>(fbxColour[1]);
					colour[2] = static_cast<float>(fbxColour[2]);
				}
				break;
			default:
				FBXSDK_printf("Trying to load %s with an unsupported reference mode\n", element.GetName());
				break;
			}
			break;

		case FbxGeometryElement::eByPolygonVertex:
		{
			switch(element.GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				{
					FbxColor fbxColour = element.GetDirectArray().GetAt(triangleCornerId);				
					colour[0] = static_cast<float>(fbxColour[0]);
					colour[1] = static_cast<float>(fbxColour[1]);
					colour[2] = static_cast<float>(fbxColour[2]);
				}
					break;
				case FbxGeometryElement::eIndexToDirect:
				{
					int id = element.GetIndexArray().GetAt(triangleCornerId);
					FbxColor fbxColour = element.GetDirectArray().GetAt(id);
					colour[0] = static_cast<float>(fbxColour[0]);
					colour[1] = static_cast<float>(fbxColour[1]);
					colour[2] = static_cast<float>(fbxColour[2]);
				}
				break;
			default:
				FBXSDK_printf("Trying to load %s with an unsupported reference mode\n", element.GetName());
				break;
			}
		}
		break;
		default:
			FBXSDK_printf("Trying to load %s with an unsupported mapping mode\n", element.GetName());
			break;
	}

}

}
