#pragma once

#include <glm\glm.hpp>
#include <string>
#include <map>
#include <boost\shared_array.hpp>
#include "../Container/LinkedList .h"
#include "../Import/FBXImport.h"
#include "../Batch/BatchFwdDecl.h"

namespace mesh
{
	class Vertex;
	class Triangle;

	typedef std::map<unsigned int, render::AppearancePtr> AppearanceTable;
	typedef std::pair<unsigned int, render::AppearancePtr> AppearanceTableEntry;
	typedef boost::shared_array<Vertex> MeshVertexArray;
	typedef boost::shared_array<Triangle> MeshTriangleArray;

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

	int GetNumVerticesWithMaterialId(
		int materialId
	)
	{
		return m_numVerticesPerMaterial[materialId];
	}

	MeshVertexArray GetVertices()
	{
		return m_vertexArray;
	}

	MeshTriangleArray GetTriangles()
	{
		return m_triangleArray;
	}

	AppearanceTable &GetAppearances()
	{
		return m_appearanceTable;
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

	MeshVertexArray m_vertexArray;
	MeshTriangleArray m_triangleArray;
	int m_numTriangles;
	int m_numVertices;
	
	// We store here a list of material names and a list of texture names. We only actually currently need
	// the texture names however future work will expand this information to a material class storing shader parameters and multiple textures per material.
	AppearanceTable m_appearanceTable; // Mapping of material id's to material names
	std::vector<unsigned int> m_numVerticesPerMaterial; // A count of the number of vertex indices per material batch. Necessary for creating batches later
};


}