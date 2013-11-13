#pragma once

#include <glm\glm.hpp>
#include <string>
#include <map>
#include <boost\shared_array.hpp>
#include "../Container/LinkedList .h"
#include "../Import/FBXImport.h"

namespace mesh
{
	class Vertex;
	class Triangle;

class MeshNode : public container::LinkedListItem<MeshNode>
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

	boost::shared_array<Vertex> &GetVertices()
	{
		return m_vertexArray;
	}

	boost::shared_array<Triangle> &GetTriangles()
	{
		return m_triangleArray;
	}

	void							AllocateVertices(
										const int numVertices
										);

	void							AllocateTriangles(
										const int numTriangles
										);

private:
	friend import::FBXImport; // Friend as the import class needs direct access to these arrays. All other classes accessing a mesh node should use the access function provided.

	std::string m_name;

	glm::mat4x4 localTransform;			//Transform from this node to the parent node
	glm::mat4x4 globalTransform;		// Transform from this node to the world //TODO to the world or to the mesh?

	// uvsets?

	boost::shared_array<Vertex> m_vertexArray;
	boost::shared_array<Triangle> m_triangleArray;
	int m_numTriangles;
	int m_numVertices;
	
	// We store here a list of material names and a list of texture names. We only actually currently need
	// the texture names however future work will expand this information to a material class storing shader parameters and multiple textures per material.
	std::map<unsigned int, std::string> m_materialTable; // Mapping of material id's to material names
	std::map<unsigned int, std::string> m_textureTable; //Mapping of material id's to texture names
};


}