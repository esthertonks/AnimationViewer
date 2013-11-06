#pragma once

#include <glm\glm.hpp>
#include <string>
#include <map>
#include <boost\shared_array.hpp>
#include "Vertex.h"
#include "Triangle.h"
#include "../Import/FBXImport.h"
#include "../BatchMesh/RenderVertex.h" //TODO yeah - shouldn't really live here heh

namespace mesh
{

class MeshNode
{
public:

	MeshNode();
	~MeshNode();

	void SetName(
		const std::string& name
		)
	{
		m_name = name;
	};

	const std::string&	GetName()
	{		
		return m_name;
	}

	int GetNumTriangles()
	{
		return m_numTriangles;
	}

	int GetNumVertices()
	{
		return m_numVertices;
	}

	int GetNumRenderIndeces()
	{
		return m_numRenderIndices;
	}

	int GetNumRenderVerteces()
	{
		return m_numRenderVertices;
	}

	boost::shared_array<Vertex> &GetVertices()
	{
		return m_vertexArray;
	}

	boost::shared_array<Triangle> &GetTriangles()
	{
		return m_triangleArray;
	}

	boost::shared_array<RenderVertex> &GetRenderVertices()
	{
		return m_renderVertexArray;
	}

	boost::shared_array<unsigned int> &GetRenderIndices()
	{
		return m_renderIndexArray;
	}

	void							AllocateVertices(
										const int numVertices
										);

	void							AllocateTriangles(
										const int numTriangles
										);

	void							AllocateRenderVertices(
										const int numRenderVertices
										);

	void							AllocateRenderIndices(
										const int numRenderIndices
										);

	MeshNode *child;
	MeshNode *parent;

private:
	friend Import::FBXImport; // Friend as the import class needs direct access to these arrays. All other classes accessing a mesh node should use the access function provided.

	std::string m_name;

	glm::mat4x4 localTransform;			//Transform from this node to the parent node
	glm::mat4x4 globalTransform;		// Transform from this node to the world //TODO to the world or to the mesh?

	// uvsets?

	boost::shared_array<Vertex> m_vertexArray;
	boost::shared_array<Triangle> m_triangleArray;
	int m_numTriangles;
	int m_numVertices;

	boost::shared_array<RenderVertex> m_renderVertexArray;
	boost::shared_array<unsigned int> m_renderIndexArray;
	int m_numRenderIndices;
	int m_numRenderVertices;
	
	// We store here a list of material names and a list of texture names. We only actually currently need
	// the texture names however future work will expand this information to a material class storing shader parameters and multiple textures per material.
	std::map<unsigned int, std::string> m_materialTable; // Mapping of material id's to material names
	std::map<unsigned int, std::string> m_textureTable; //Mapping of material id's to texture names
};


}