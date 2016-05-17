#pragma once

#include <fbxsdk.h>
#include <map>
#include "../Container/LinkedTree.h"
#include "../Batch/BatchFwdDecl.h" // TODO For appearances only - really needed here?

namespace import
{
	class FBXImport;
}

namespace mesh
{

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

	static void ResetIdCounter()
	{
		 m_counter = 0;
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

	render::AppearanceTable &GetAppearanceTable()
	{
		return m_appearanceTable;
	}

	int GetNumVerticesWithMaterialId(
		int materialId
	)
	{
		return m_numVerticesPerMaterialArray[materialId];
	}

	std::vector<unsigned int> &GetNumVerticesPerMaterialArray()
	{
		return m_numVerticesPerMaterialArray;
	}

private:
	// uvsets?
	friend class import::FBXImport; // Friend as the import class needs direct access to these arrays. All other classes accessing a mesh node should use the access function provided.
	MeshVertexArray m_vertexArray;
	MeshTriangleArray m_triangleArray;
	int m_numTriangles;
	int m_numVertices;

	render::AppearanceTable m_appearanceTable; // Mapping of material id's to material names //TODO pointer and set method
	std::vector<unsigned int> m_numVerticesPerMaterialArray; // A count of the number of vertex indices per material batch. Necessary for creating batches later

	bool m_isSkinned;

	std::string m_name;

	FbxAMatrix m_globalTransform;		// Transform from this node to the global model transform. This is updated each tick during animation

	unsigned int m_id;
	static unsigned int m_counter;
};


}