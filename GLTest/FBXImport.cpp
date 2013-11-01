#include "FBXImport.h"
#include "Mesh.h"
#include "MeshNode.h"

#include <assert.h>

namespace Import
{

FBXImport::FBXImport()
: fbxManager(NULL),
fbxScene(NULL),
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
	fbxManager->SetIOSettings(ioSettings);

	//Create an FBX scene which holds the imported objects
	m_fbxScene = FbxScene::Create(fbxManager, "My Scene");
	if(!m_fbxScene)
	{
		FBXSDK_printf("Can't create fbx scene.\n");
		return false;
	}

	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager,"");

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
	fbxManager->Destroy();

	return false;
}

void FBXImport::LoadMeshNodes(
	FbxNode& fbxNode,
	mesh::MeshNode &meshNode
	)
{
	m_mesh.AddNode(meshNode);

	std::string name = fbxNode.GetName();
	meshNode.SetName(name);

	FbxMesh& fbxMesh = *(FbxMesh*)fbxNode.GetNodeAttribute();

	// Make sure the mesh is triangulated
	FbxGeometryConverter fbxGeometryConverter(m_fbxManager);
	fbxGeometryConverter.TriangulateInPlace(&fbxNode);

	// Extract and store vertices


	// Extract and store triangles
	int numPolygons = fbxMesh.GetPolygonCount();
	FbxVector4* lControlPoints = fbxMesh.GetControlPoints(); 
	int vertexId = 0;
	for(int polygonIndex = 0; polygonIndex < numPolygons; polygonIndex++)
	{
		int polygonSize = fbxMesh.GetPolygonSize(polygonIndex);

		assert(polygonSize == 3);

		for (int triangleVertIndex = 0; triangleVertIndex < polygonSize; triangleVertIndex++)
		{
			int controlPointIndex = fbxMesh.GetPolygonVertex(polygonIndex, triangleVertIndex);

				LoadVertexColours(fbxMesh, vertexId);
				LoadUVs(fbxMesh, vertexId);
				LoadNormals(fbxMesh, vertexId);
				LoadTangents(fbxMesh, vertexId);
				LoadBinormals(fbxMesh, vertexId);

				vertexId++;
		} 
	}

	//meshNode.GetTriangleArray();

	for(int childIndex = 0; childIndex < fbxNode.GetChildCount(); childIndex++)
	{
		FbxNode &fbxChildNode = *fbxNode.GetChild(childIndex);
		mesh::MeshNode &meshChildNode = *new mesh::MeshNode;
		LoadMeshNodes(fbxChildNode, meshChildNode);
	}

}

void FBXImport::LoadNormals(
	FbxMesh& fbxMesh,
	int vertexId
	)
{
	for(int normalIndex = 0; normalIndex < fbxMesh.GetElementNormalCount(); normalIndex++)
	{
		FbxGeometryElementNormal& normalElement = *fbxMesh.GetElementNormal(normalIndex);

		if(normalElement.GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			switch(normalElement.GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				{
					FbxVector4 normal = normalElement.GetDirectArray().GetAt(vertexId);
				}
				break;

				case FbxGeometryElement::eIndexToDirect:
				{
					int indexId = normalElement.GetIndexArray().GetAt(vertexId);
					FbxVector4 normal = normalElement.GetDirectArray().GetAt(indexId);
				}
				break;

				default:
				break; // other reference modes not shown here!
			}
		}
	}
}

void FBXImport::LoadBinormals(
	FbxMesh& fbxMesh,
	int vertexId
	)
{
	for(int binormalIndex = 0; binormalIndex < fbxMesh.GetElementBinormalCount(); binormalIndex++)
	{

		FbxGeometryElementBinormal& binormalElement = *fbxMesh.GetElementBinormal(binormalIndex);

		if(binormalElement.GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			switch(binormalElement.GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				{
					FbxVector4 binormal = binormalElement.GetDirectArray().GetAt(vertexId);
				}
				break;
				case FbxGeometryElement::eIndexToDirect:
				{
					int id = binormalElement.GetIndexArray().GetAt(vertexId);
					FbxVector4 binormal = binormalElement.GetDirectArray().GetAt(id);
				}
				break;
				default:
					break; // other reference modes not shown here!
			}
		}
	}
}

void FBXImport::LoadTangents(
	FbxMesh& fbxMesh,
	int vertexId
	)
{
	for(int tangentIndex = 0; tangentIndex < fbxMesh.GetElementTangentCount(); tangentIndex++)
	{
		FbxGeometryElementTangent& tangentElement = *fbxMesh.GetElementTangent(tangentIndex);

		if(tangentElement.GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			switch (tangentElement.GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				{
					FbxVector4 tangent = tangentElement.GetDirectArray().GetAt(vertexId);
				}
				break;
				case FbxGeometryElement::eIndexToDirect:
				{
					int id = tangentElement.GetIndexArray().GetAt(vertexId);
					FbxVector4 tangent = tangentElement.GetDirectArray().GetAt(id);
				}
				break;
				default:
				break;// other reference modes not shown here!
			}
		}
	}
}

void FBXImport::LoadUVs(
	FbxMesh& fbxMesh,
	int polygonIndex, 
	int triangleVertIndex,
	int controlPointIndex
	)
{
	for (int uvIndex = 0; uvIndex < fbxMesh.GetElementUVCount(); uvIndex++)
	{
		FbxGeometryElementUV& uvElement = *fbxMesh.GetElementUV(uvIndex);

		switch (uvElement.GetMappingMode())
		{
			case FbxGeometryElement::eByControlPoint:
			{
				switch (uvElement.GetReferenceMode())
				{
					case FbxGeometryElement::eDirect:
					{
						FbxVector2 uvs = uvElement.GetDirectArray().GetAt(controlPointIndex);
					}
					break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = uvElement.GetIndexArray().GetAt(controlPointIndex);
						FbxVector2 uvs = uvElement.GetDirectArray().GetAt(id);
					}
					break;
					default:
						break; // TODO other reference modes not shown here!
				}
			}
			break;
			case FbxGeometryElement::eByPolygonVertex:
			{
				int UVIndex = fbxMesh.GetTextureUVIndex(polygonIndex, triangleVertIndex);
				switch (uvElement.GetReferenceMode())
				{
					case FbxGeometryElement::eDirect:
					case FbxGeometryElement::eIndexToDirect:
					{
						FbxVector2 uvs = uvElement.GetDirectArray().GetAt(UVIndex);
					}
					break;
					default:
						break; // TODO other reference modes not shown here!
				}
			}
			case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
			case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
			case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
			break;
		}
	}
}

void FBXImport::LoadVertexColours(
	FbxMesh& fbxMesh,
	int vertexId,
	int controlPointIndex
	)
{
	for (int vertexColourIndex = 0; vertexColourIndex < fbxMesh.GetElementVertexColorCount(); vertexColourIndex++)
	{
		FbxGeometryElementVertexColor& vertexColourElement = *fbxMesh.GetElementVertexColor(vertexColourIndex);

		switch (vertexColourElement.GetMappingMode())
		{
			case FbxGeometryElement::eByControlPoint:
				switch (vertexColourElement.GetReferenceMode())
				{
					case FbxGeometryElement::eDirect:
					{
						FbxColor vertexColour = vertexColourElement.GetDirectArray().GetAt(controlPointIndex);
					}
					break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = vertexColourElement.GetIndexArray().GetAt(controlPointIndex);
						FbxColor vertexColour = vertexColourElement.GetDirectArray().GetAt(id);
					}
					break;
				default:
					break; // other reference modes not shown here!
				}
				break;

			case FbxGeometryElement::eByPolygonVertex:
			{
				switch(vertexColourElement.GetReferenceMode())
				{
					case FbxGeometryElement::eDirect:
					{
						FbxColor vertexColour = vertexColourElement.GetDirectArray().GetAt(vertexId);
					}
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = vertexColourElement.GetIndexArray().GetAt(vertexId);
						FbxColor vertexColour = vertexColourElement.GetDirectArray().GetAt(id));
					}
					break;
				default:
					break; // other reference modes not shown here!
				}
			}
			case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
			case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
			case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
			break;
		}
	}
}

}
