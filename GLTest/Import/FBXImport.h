#pragma once

#include <string>
#include <boost/shared_array.hpp>
#include "../Batch/BatchFwdDecl.h"
#include "../Mesh/MeshFwdDecl.h"

namespace mesh
{
	class Mesh;
	class Node;
	class MeshNode;
	class Triangle;
	class TexturedVertex;
}

namespace import
{

class FBXImport
{
public:
	FBXImport();
	~FBXImport();

	/**
		@brief Imports an fbx mesh into an import mesh format
		@return Returns an import mesh or NULL if import failed
	*/
	mesh::MeshPtr Import(
		const std::string &fbxFilename		// The name of the FBX file to import
	);

private:

	bool LoadNodes(
		FbxNode& fbxNode,	// The FBX mesh to extract data from and add to m_mesh
		mesh::Node *parent
	);

	mesh::Node *LoadMeshNode(
		FbxNode &fbxNode,				// The FBX mesh to extract data from and add to m_mesh mesh node list
		mesh::Node *parent
		);

	mesh::Node *LoadBoneNode(
		FbxNode& fbxNode,	// The FBX mesh to extract data from and add to m_mesh bone node list
		mesh::Node *parent
	);

	void LoadAnimationLayerInfo();

	void LoadSkin(
		FbxNode& fbxNode // The FBX mesh to extract data from
		);

	void LoadSkin(
		const FbxGeometry &fbxGeometry,// The FBX mesh geometry node to extract data from
		const FbxSkin &fbxSkin// The skin itself to extract the skin info from
		);

	/**
	@brief Bake all possible FBX transforms into the statndard rotation, trnaslate and scale variables so the can be extracted easily later
	*/
	void BakeNodeTransforms(
		FbxNode &node		//!< Node to bake transforms for
		) const;


	const unsigned int GetUVVertexIndex(
		const unsigned int triangleIndex, 
		const unsigned int triangleCornerId
		) const;

	/**
		@brief Loads a material id for the given triangle
	*/
	void LoadMaterials(
	FbxMesh &fbxMesh,			// FBX mesh to import data from
	int triangleIndex,			// Index of the current triangle being loaded
	mesh::MeshNode &meshNode,	// The mesh node which will store the imported material and texture data
	render::AppearanceTable &appearanceTable,
	std::vector<unsigned int> &numVerticesPerMaterial
		);

	/**
		@brief Loads the vertex Indices for the given triangle
	*/
	void LoadVertexIndices(
		FbxMesh &fbxMesh,			// FBX mesh to import data from
		int triangleIndex,			// Index of the current triangle being loaded
		mesh::Triangle &triangle	// The current triangle to store the imported data
		);

	/**
		@brief Loads the colours at each corner of the given triangle
	*/
	void LoadColours(
		FbxMesh &fbxMesh,			// FBX mesh to import data from
		int triangleIndex,			// Index of the current triangle being loaded
		mesh::Triangle &triangle	// The current triangle to store the imported data
		);

	/**
		@brief Loads the uvs at each corner of the given triangle
	*/
	void LoadUVs(
		FbxMesh &fbxMesh,			// FBX mesh to import data from
		int triangleIndex,			// Index of the current triangle being loaded
		mesh::Triangle &triangle	// The current triangle to store the imported data
		);

	/**
		@brief Loads the normals at each corner of the given triangle
	*/
	void LoadNormals(
		FbxMesh &fbxMesh,			// FBX mesh to import data from
		int triangleIndex,			// Index of the current triangle being loaded
		mesh::Triangle &triangle	// The current triangle to store the imported data
		);

	/**
		@brief Loads the binormals at each corner of the given triangle
	*/
	void LoadBinormals(
		FbxMesh &fbxMesh,			// FBX mesh to import data from
		int triangleIndex,			// Index of the current triangle being loaded
		mesh::Triangle &triangle	// The current triangle to store the imported data
		);

	/**
		@brief Loads the tangents at each corner of the given triangle
	*/
	void LoadTangents(
		FbxMesh &fbxMesh,			// FBX mesh to import data from
		int triangleIndex,			// Index of the current triangle being loaded
		mesh::Triangle &triangle	// The current triangle to store the imported data
		);

	void LoadVector4VertexElement(
		FbxLayerElementTemplate<FbxVector4> &element,
		glm::vec4 &data,
		int triangleCornerId,								// The corner(vertex) of this triangle being refered to
		int vertexIndex										// The index of the vertex in the vertex array
		);

	void LoadVector2VertexElement(
		FbxLayerElementTemplate<FbxVector2> &element,
		glm::vec2 &data,
		int triangleIndex,									// The index of this triangle in the triangle array
		int triangleCornerId,								// The corner(vertex) of this triangle being refered to
		int vertexIndex										// The index of the vertex in the vertex array
		);

	void LoadColourVertexElement(
		FbxLayerElementTemplate<FbxColor> &element,
		glm::vec3 &colour, // The current nodes vertex array which will store the imported data
		int triangleCornerId,								// The corner(vertex) of this triangle being refered to
		int vertexIndex										// The index of the vertex in the vertex array
		);

	FbxManager *m_fbxManager;			// FBX SDK manager
	FbxScene *m_fbxScene;				// FBX SDK scene which data will be extracted from
	FbxImporter* m_fbxImporter;			// Scene importer for the FBX file

	mesh::MeshPtr m_mesh;					// The mesh to hold the imported data

	static const std::string m_dummyTexture;
};

}