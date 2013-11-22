#include "FBXImport.h"
#include "../ImportMesh/Mesh.h"
#include "../ImportMesh/MeshNode.h"
#include "../ImportMesh/Triangle.h"
#include "../ImportMesh/Vertex.h"
#include "../Batch/LambertAppearance.h"
#include "../Batch/PhongAppearance.h"
#include "../Batch/Batch.h"

#include <assert.h>

#include <wx/log.h>

namespace import
{

FBXImport::FBXImport()
: m_fbxManager(NULL),
m_fbxScene(NULL),
m_mesh(NULL)
{



}

FBXImport::~FBXImport()
{


}

mesh::Mesh* FBXImport::Import(
	std::string &fbxFilename
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

	FbxImporter* fbxImporter = FbxImporter::Create(m_fbxManager,"");

	if(!fbxImporter->Initialize(fbxFilename.c_str(), -1, ioSettings))
	{
		FbxString error = fbxImporter->GetStatus().GetErrorString();
		FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
		FBXSDK_printf("FBX Import failed with error: %s\n", error.Buffer());

		if (fbxImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
		{
			FBXSDK_printf("FBX file format version for file '%s' is not valid for this SDK version\n", fbxFilename);
			return NULL;
		}

		fbxImporter->Destroy();
		m_fbxScene->Destroy();
		ioSettings->Destroy();
		m_fbxManager->Destroy();
	}

	if(!fbxImporter->IsFBX())
	{
		FBXSDK_printf("File %s is not an FBX file. \n", fbxFilename);
		fbxImporter->Destroy();
		m_fbxScene->Destroy();
		ioSettings->Destroy();
		m_fbxManager->Destroy();
		return NULL;
	}

	// Import the scene.
	if(!fbxImporter->Import(m_fbxScene))
	{
		FBXSDK_printf("Import failed for file: %s \n", fbxFilename);
		fbxImporter->Destroy();
		m_fbxScene->Destroy();
		ioSettings->Destroy();
		m_fbxManager->Destroy();
		return NULL;
	}

	// Fill the mesh with the imported data
	m_mesh = new mesh::Mesh();
	FbxNode &fbxRootNode = *m_fbxScene->GetRootNode();
	mesh::MeshNode &meshNode = *new mesh::MeshNode;
	LoadMeshNodes(fbxRootNode, meshNode);

	fbxImporter->Destroy();
	m_fbxScene->Destroy();
	ioSettings->Destroy();
	m_fbxManager->Destroy();

	return m_mesh;
}

bool FBXImport::LoadMeshNodes(
	FbxNode& fbxNode,
	mesh::MeshNode &meshNode
	)
{
	// Make sure the mesh is triangulated
	FbxGeometryConverter fbxGeometryConverter(m_fbxManager);	

	// Check that this is a mesh
	FbxNodeAttribute* const fbxNodeAttribute = fbxNode.GetNodeAttribute();
	if(fbxNodeAttribute)
	{
		const FbxNodeAttribute::EType fbxAttributeType = fbxNodeAttribute->GetAttributeType();

		if(fbxAttributeType != FbxNodeAttribute::eMesh)
		{
			FBXSDK_printf("Node %s type is %d. Only node of type eMesh (4) can be loaded\n", fbxNode.GetName(), fbxAttributeType);
			return false;
		}
	}

	FbxMesh* fbxMesh = fbxNode.GetMesh();
	if(fbxMesh)
	{
		if(!fbxMesh->IsTriangleMesh())
		{
			FbxNodeAttribute* fbxNodeAttribute = fbxGeometryConverter.Triangulate(fbxNode.GetMesh(), true);
			if(!fbxNodeAttribute)
			{
				FBXSDK_printf("Mesh Triangulation failed. Import aborted.\n");
				return false;
			}
			fbxMesh = (FbxMesh *)fbxNodeAttribute;
		}

		m_mesh->AddChildNode(meshNode);

		std::string name = fbxNode.GetName();
		meshNode.SetName(name);

		// Extract and store vertices
		const unsigned int numVertices = fbxMesh->GetControlPointsCount();
		meshNode.AllocateVertices(numVertices);
		mesh::MeshVertexArray vertexArray = meshNode.GetVertices();

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

		meshNode.AllocateTriangles(numTriangles);
		mesh::MeshTriangleArray triangleArray = meshNode.GetTriangles();

			// Get the per triangle material index
		const int materialLayerCount = fbxMesh->GetElementMaterialCount();
		if(materialLayerCount != 1)
		{
			FBXSDK_printf("Only one material layer is supported. Only the first layer will be loaded.\n");
		}

		// Load per triangle data
		for(int triangleIndex = 0; triangleIndex < numTriangles; triangleIndex++)
		{
			int triangleSize = fbxMesh->GetPolygonSize(triangleIndex);
			assert(triangleSize == 3); // The mesh was triangulated above.

			mesh::Triangle &triangle = triangleArray[triangleIndex];
			LoadMaterials(*fbxMesh, triangleIndex, meshNode);

			LoadVertexIndices(*fbxMesh, triangleIndex, triangle);

			LoadColours(*fbxMesh, triangleIndex, triangle);
			LoadUVs(*fbxMesh, triangleIndex, triangle);

			LoadNormals(*fbxMesh, triangleIndex, triangle);
			LoadBinormals(*fbxMesh, triangleIndex, triangle);		
			LoadTangents(*fbxMesh, triangleIndex, triangle);
		}
	}

	for(int childIndex = 0; childIndex < fbxNode.GetChildCount(); childIndex++)
	{
		FbxNode &fbxChildNode = *fbxNode.GetChild(childIndex);
		mesh::MeshNode &meshChildNode = *new mesh::MeshNode;
		LoadMeshNodes(fbxChildNode, meshChildNode);
	}

	return true;
}

/**
	@brief Loads a material id for the given triangle
*/
void FBXImport::LoadMaterials(
	FbxMesh &fbxMesh,			// FBX mesh to import data from
	int triangleIndex,			// Index of the current triangle being loaded
	mesh::MeshNode &meshNode	// The mesh node which will store the imported material and texture data
	)
{
	const FbxGeometryElementMaterial &materialElement = *fbxMesh.GetElementMaterial(0); // Get the first material layer element. Only one is supported.
	const int materialId = materialElement.GetIndexArray().GetAt(triangleIndex);
	if (materialId >= 0)
	{
		const FbxSurfaceMaterial& surfaceMaterial = *fbxMesh.GetNode()->GetMaterial(materialId);
		meshNode.m_triangleArray[triangleIndex].m_materialId = materialId;

		// Just get the diffuse for now. Will load normal/bump and other textures here in future.
		FbxProperty materialProperty = surfaceMaterial.FindProperty(FbxSurfaceMaterial::sDiffuse);

		if(meshNode.m_appearanceTable.count(materialId) == 0)
		{
			meshNode.m_numVerticesPerMaterial.push_back(0);// Add another int initialised at 0 for this material
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

				boost::shared_ptr<render::PhongAppearance> phongAppearancePtr = boost::static_pointer_cast<render::PhongAppearance>(appearance);

				phongAppearancePtr->SetAmbient(ambientColour);
				phongAppearancePtr->SetDiffuse(diffuseColour);
				phongAppearancePtr->SetSpecular(specularColour);
				phongAppearancePtr->SetEmissive(emmissiveColour);

				phongAppearancePtr->SetTransparency(transparency);
				phongAppearancePtr->SetShininess(shininess);
				phongAppearancePtr->SetReflectivity(reflectivity);
			}
			else if(surfaceMaterial.GetClassId().Is(FbxSurfaceLambert::ClassId))
			{
				appearance = render::AppearancePtr(new render::LambertAppearance());

				const FbxSurfaceLambert& lambertMaterial = (const FbxSurfaceLambert &)surfaceMaterial;

				glm::vec3 ambientColour(lambertMaterial.Ambient.Get()[0], lambertMaterial.Ambient.Get()[1], lambertMaterial.Ambient.Get()[3]);
				glm::vec3 diffuseColour(lambertMaterial.Diffuse.Get()[0], lambertMaterial.Diffuse.Get()[1], lambertMaterial.Diffuse.Get()[3]);
				glm::vec3 emmissiveColour(lambertMaterial.Emissive.Get()[0], lambertMaterial.Emissive.Get()[1], lambertMaterial.Emissive.Get()[3]);

				double transparency = lambertMaterial.TransparencyFactor.Get();

				boost::shared_ptr<render::LambertAppearance> lambertAppearancePtr = boost::static_pointer_cast<render::LambertAppearance>(appearance);

				lambertAppearancePtr->SetAmbient(ambientColour);
				lambertAppearancePtr->SetDiffuse(diffuseColour);
				lambertAppearancePtr->SetEmissive(emmissiveColour);

				lambertAppearancePtr->SetTransparency(transparency);
			}
			else
			{
				FBXSDK_printf("Material Id %d, name &s is not supported", materialId, surfaceMaterial.GetName());
			}

			unsigned int textureCount = materialProperty.GetSrcObjectCount<FbxTexture>();
			for(int textureIndex = 0; textureIndex < textureCount; textureIndex++)
			{
				FbxFileTexture* fbxFileTexture = materialProperty.GetSrcObject<FbxFileTexture>(textureIndex);

				std::string textureFilename = fbxFileTexture->GetFileName();//TODO cant currently support multiple materials!
				appearance->AddTexture(textureFilename);
			}

			mesh::AppearanceTableEntry materialInfo;
			materialInfo.first = materialId;
			materialInfo.second = appearance;
			meshNode.m_appearanceTable.insert(materialInfo);
		}
		meshNode.m_numVerticesPerMaterial[materialId]++;
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
	if(fbxMesh.GetElementUVCount() > 1)
	{
		FBXSDK_printf("Only one set of uvs currently supported\n"); // May support more uv sets later
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
					data[0] = static_cast<float>(fbxData[0]);
					data[1] = static_cast<float>(fbxData[1]);
				}
				break;
				case FbxGeometryElement::eIndexToDirect:
				{
					int id = element.GetIndexArray().GetAt(vertexIndex);
					FbxVector2 fbxUvs = element.GetDirectArray().GetAt(id);
					data[0] = static_cast<float>(fbxUvs[0]);
					data[1] = static_cast<float>(fbxUvs[1]);
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
					data[0] = static_cast<float>(fbxData[0]);
					data[1] = static_cast<float>(fbxData[1]);
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
