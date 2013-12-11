/**
	This class contains a mesh suitable for loaded information which is stored per triangle (and not per vertex as will be required to render
	This class stores the root of the mesh node hierarchy. The mesh hierarchy a flat linked list of parent->child nodes.
*/
#pragma once

#include "Node.h"
#include "../Batch/BatchFwdDecl.h"
#include "../Animation/AnimationInfo.h"

namespace mesh
{

class Node;

class Mesh
{
public:

	Mesh::Mesh();
	Mesh::~Mesh();

	Node* GetNodeHierarchy()
	{
		return m_nodes.m_root;
	}

	void AddChildNode(
		Node *parent,	// Parent node or NULL if no parent
		Node *node		// Child node to add
		)
	{
		m_nodes.AddAsChild(parent, node);
	}

	int GetNumVerticesWithMaterialId(
		int materialId
	)
	{
		return m_numVerticesPerMaterial[materialId];
	}

	render::AppearanceTable &GetAppearanceTable()
	{
		return m_appearanceTable;
	}


	std::vector<unsigned int> &GetNumVerticesPerMaterialArray()
	{
		return m_numVerticesPerMaterial;
	}

	animation::AnimationInfo &GetAnimationInfo()
	{
		return m_animationInfo;
	}

private:
	render::AppearanceTable m_appearanceTable; // Mapping of material id's to material names
	std::vector<unsigned int> m_numVerticesPerMaterial; // A count of the number of vertex indices per material batch. Necessary for creating batches later
	container::LinkedTree<Node> m_nodes;
	animation::AnimationInfo m_animationInfo; // Extra info about all animation tracks
};
}