#include "FBXImport.h"
#include "Mesh.h"
#include "MeshNode.h"
#include "Triangle.h"

#include <assert.h>

namespace Import
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

bool FBXImport::Import(
	std::string &fbxFilename
	)
{

	m_fbxManager = FbxManager::Create();
	if(!m_fbxManager)
	{
		FBXSDK_printf("Can't create fbx manager.\n");
		return false;
	}

	// Set up the import settings
	FbxIOSettings* ioSettings = FbxIOSettings::Create(m_fbxManager, IOSROOT);
	m_fbxManager->SetIOSettings(ioSettings);

	//Create an FBX scene which holds the imported objects
	m_fbxScene = FbxScene::Create(m_fbxManager, "My Scene");
	if(!m_fbxScene)
	{
		FBXSDK_printf("Can't create fbx scene.\n");
		return false;
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
			return false;
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
		return false;
	}

	// Import the scene.
	if(!fbxImporter->Import(m_fbxScene))
	{
		FBXSDK_printf("Import failed for file: %s \n", fbxFilename);
		fbxImporter->Destroy();
		m_fbxScene->Destroy();
		ioSettings->Destroy();
		m_fbxManager->Destroy();
		return false;
	}

	// Fill the mesh with the imported data
	FbxNode &fbxRootNode = *m_fbxScene->GetRootNode();
	mesh::MeshNode &meshNode = *new mesh::MeshNode;
	LoadMeshNodes(fbxRootNode, meshNode);



	fbxImporter->Destroy();
	m_fbxScene->Destroy();
	ioSettings->Destroy();
	m_fbxManager->Destroy();

	return false;
}

void FBXImport::LoadMeshNodes(
	FbxNode& fbxNode,
	mesh::MeshNode &meshNode
	)
{
	m_mesh->AddNode(meshNode);

	std::string name = fbxNode.GetName();
	meshNode.SetName(name);

	FbxNodeAttribute* nodeAttribute = fbxNode.GetNodeAttribute();
	// Make sure the mesh is triangulated
	FbxGeometryConverter fbxGeometryConverter(m_fbxManager);	
	nodeAttribute = fbxGeometryConverter.Triangulate(nodeAttribute, true);

	FbxMesh& fbxMesh = *(FbxMesh *)nodeAttribute;

	// Extract and store vertices
	const unsigned int numVertices = fbxMesh.GetControlPointsCount();
	meshNode.AllocateVertices(numVertices);
	boost::shared_array<mesh::Vertex> vertexArray = meshNode.GetVertices();

	const FbxVector4* const fbxVertices = fbxMesh.GetControlPoints();
	for(unsigned int vertexIndex = 0; vertexIndex < numVertices; vertexIndex++)
	{
		fbxVertices[vertexIndex];

		vertexArray[vertexIndex].m_position.x = static_cast<float>(fbxVertices[vertexIndex][0]);
		vertexArray[vertexIndex].m_position.y = static_cast<float>(fbxVertices[vertexIndex][1]);
		vertexArray[vertexIndex].m_position.z = static_cast<float>(fbxVertices[vertexIndex][2]);
	}

	// Extract and store triangles
	int numTriangles = fbxMesh.GetPolygonCount();

	meshNode.AllocateTriangles(numTriangles);
	boost::shared_array<mesh::Triangle> triangleArray = meshNode.GetTriangles();

	for(int triangleIndex = 0; triangleIndex < numTriangles; triangleIndex++)
	{
		int triangleSize = fbxMesh.GetPolygonSize(triangleIndex);

		assert(triangleSize == 3); // The mesh was triangulated above.

		for (int triangleCornerId = 0; triangleCornerId < triangleSize; triangleCornerId++)
		{
			// Get the index into the vertex array for this vertex on this polygon
			int vertexIndex = fbxMesh.GetPolygonVertex(triangleIndex, triangleCornerId);
			triangleArray[triangleIndex].SetVertexIndex(triangleCornerId, vertexIndex);

			for (int vertexColourIndex = 0; vertexColourIndex < fbxMesh.GetElementVertexColorCount(); vertexColourIndex++)
			{
				FbxGeometryElementVertexColor* vertexColourElement = fbxMesh.GetElementVertexColor(vertexColourIndex);
				if(vertexColourElement)
				{
					LoadColourVertexElement(*vertexColourElement, vertexArray[vertexIndex].m_colour, triangleCornerId, vertexIndex);
				}
			}

			for (int uvIndex = 0; uvIndex < fbxMesh.GetElementUVCount(); uvIndex++)
			{
				FbxGeometryElementUV* uvElement = fbxMesh.GetElementUV(uvIndex);
				if(uvElement)
				{
					LoadVector2VertexElement(*uvElement, vertexArray[vertexIndex].m_uvs, triangleIndex, triangleCornerId, vertexIndex);
				}
			}

			for(int normalIndex = 0; normalIndex < fbxMesh.GetElementNormalCount(); normalIndex++)
			{
				FbxGeometryElementNormal* normalElement = fbxMesh.GetElementNormal(normalIndex);
				if(normalElement)
				{
					LoadVector4VertexElement(*normalElement, vertexArray[vertexIndex].m_normal, triangleCornerId, vertexIndex);
				}
			} 

			for(int binormalIndex = 0; binormalIndex < fbxMesh.GetElementNormalCount(); binormalIndex++)
			{
				FbxGeometryElementBinormal* binormalElement = fbxMesh.GetElementBinormal(binormalIndex);
				if(binormalElement)
				{
					LoadVector4VertexElement(*binormalElement, vertexArray[vertexIndex].m_normal, triangleCornerId, vertexIndex);
				}
			} 

			for(int tangentIndex = 0; tangentIndex < fbxMesh.GetElementNormalCount(); tangentIndex++)
			{
				FbxGeometryElementTangent* tangentElement = fbxMesh.GetElementTangent(tangentIndex);
				if(tangentElement)
				{
					LoadVector4VertexElement(*tangentElement, vertexArray[vertexIndex].m_normal, triangleCornerId, vertexIndex);
				}
			} 
		}
	}	

	for(int childIndex = 0; childIndex < fbxNode.GetChildCount(); childIndex++)
	{
		FbxNode &fbxChildNode = *fbxNode.GetChild(childIndex);
		mesh::MeshNode &meshChildNode = *new mesh::MeshNode;
		LoadMeshNodes(fbxChildNode, meshChildNode);
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
