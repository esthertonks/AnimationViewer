#pragma once

#include <fbxsdk.h>
#include <map>
#include <boost\shared_array.hpp>
#include "../Container/LinkedTree.h"

namespace import
{
	class FBXImport;
}

namespace mesh
{
	class Vertex;
	class Triangle;

	typedef boost::shared_array<Vertex> MeshVertexArray; //TODO any better container?
	typedef boost::shared_array<Triangle> MeshTriangleArray;

class MeshNode : public container::LinkedTreeItem<MeshNode>
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

	unsigned int GetId()
	{
		return m_id;
	}

	int GetNumTriangles()
	{
		return m_numTriangles;
	}

	int GetNumVertices()
	{
		return m_numVertices;
	}

	MeshVertexArray GetVertices()
	{
		return m_vertexArray;
	}

	MeshTriangleArray GetTriangles()
	{
		return m_triangleArray;
	}

	void							AllocateVertices(
										const int numVertices
										);

	void							AllocateTriangles(
										const int numTriangles
										);

	inline void FlagAsSkinned(
		bool isSkinned
		)
	{
		m_isSkinned = isSkinned;
	}

	inline bool IsSkinned()
	{
		return m_isSkinned;
	}

	FbxAMatrix &GetGlobalTransform()
	{
		return m_globalTransform;
	};

private:
	// uvsets?
	friend class import::FBXImport; // Friend as the import class needs direct access to these arrays. All other classes accessing a mesh node should use the access function provided.
	MeshVertexArray m_vertexArray;
	MeshTriangleArray m_triangleArray;
	int m_numTriangles;
	int m_numVertices;

	bool m_isSkinned;

	std::string m_name;

	FbxAMatrix m_globalTransform;		// Transform from this node to the global model transform. This is updated each tick during animation

	unsigned int m_id;
	static unsigned int m_counter;
};


}