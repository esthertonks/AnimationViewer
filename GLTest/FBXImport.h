#pragma once

#include <fbxsdk.h> //TODO global header file?
#include <string>

namespace mesh
{
	class Mesh;
	class MeshNode;
}

namespace Import
{

class FBXImport
{
public:
	FBXImport();
	~FBXImport();

	bool Import(
		std::string &fbxFilename
	);

private:

	void LoadMeshNodes(
		FbxNode &fbxNode,
		mesh::MeshNode &node
		);

	void LoadNormals(
		FbxMesh& fbxMesh,
		int vertexId,
		int controlPointIndex
		);

	void LoadBinormals(
		FbxMesh& fbxMesh,
		int vertexId,
		int controlPointIndex
		);

	void LoadTangents(
		FbxMesh& fbxMesh,
		int vertexId,
		int controlPointIndex
		);

	void LoadUVs(
		FbxMesh& fbxMesh,
		int polygonIndex, 
		int triangleVertIndex,
		int controlPointIndex
		);

	void LoadVertexColours(
		FbxMesh& fbxMesh,
		int vertexId,
		int controlPointIndex
		);

	FbxManager* m_fbxManager;
	FbxScene* m_fbxScene;

	mesh::Mesh &m_mesh;
};

}