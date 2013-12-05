/**
	This class contains a mesh suitable for loaded information which is stored per triangle (and not per vertex as will be required to render
	This class stores the root of the mesh node hierarchy. The mesh hierarchy a flat linked list of parent->child nodes.
*/
#pragma once

#include "../Container/LinkedList .h"
#include "MeshNode.h"
#include "BoneNode.h"
#include "../Batch/BatchFwdDecl.h"

namespace mesh
{

class MeshNode;
class BoneNode;

class Mesh
{
public:

	Mesh::Mesh();
	Mesh::~Mesh();

	MeshNode* GetMeshNodeHierarchy()
	{
		return m_meshNodes.m_root;
	}

	BoneNode* GetBoneNodeHierarchy()
	{
		return m_boneNodes.m_root;
	}

	void AddChildMeshNode(
		MeshNode &node
		)
	{
		m_meshNodes.Add(node);
	}

	void AddChildBoneNode(
		BoneNode &node
		)
	{
		m_boneNodes.Add(node);
	}

	int GetNumVerticesWithMaterialId(
		int materialId
	)
	{
		return m_numVerticesPerMaterial[materialId];
	}

	render::AppearanceTable &GetAppearances()
	{
		return m_appearanceTable;
	}


private:
	friend import::FBXImport; // Friend as the import class needs direct access to these arrays. All other classes accessing a mesh node should use the access function provided.
	render::AppearanceTable m_appearanceTable; // Mapping of material id's to material names
	std::vector<unsigned int> m_numVerticesPerMaterial; // A count of the number of vertex indices per material batch. Necessary for creating batches later
	container::LinkedList<MeshNode> m_meshNodes;
	container::LinkedList<BoneNode> m_boneNodes;

};
}