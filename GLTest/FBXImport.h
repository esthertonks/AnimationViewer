#pragma once

#include <fbxsdk.h> //TODO global header file?
#include <string>
#include <boost/shared_array.hpp>
#include <glm/glm.hpp>

namespace mesh
{
	class Mesh;
	class MeshNode;
	class Triangle;
	class Vertex;
}

namespace Import
{

class FBXImport
{
public:
	FBXImport();
	~FBXImport();

	bool Import(
		std::string &fbxFilename		// The name of the FBX file to import
	);

private:

	void LoadMeshNodes(
		FbxNode &fbxNode,				// The FBX mesh to extract data from
		mesh::MeshNode &node			// The node to hold the extracted data
		);

	const unsigned int GetUVVertexIndex(
		const unsigned int triangleIndex, 
		const unsigned int triangleCornerId
		) const;

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

	mesh::Mesh *m_mesh;					// The mesh to hold the imported data
};

}