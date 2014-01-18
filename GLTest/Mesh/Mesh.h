/**
	This class contains a mesh suitable for loaded information which is stored per triangle (and not per vertex as will be required to render
	This class stores the root of the mesh node hierarchy. The mesh hierarchy a flat linked list of parent->child nodes.
*/
#pragma once

#include "Node.h"
#include "../Batch/BatchFwdDecl.h"
#include "MeshFwdDecl.h"

namespace mesh
{

class Node;
class MeshNode;
class BoneNode;

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

	BoneNode* GetBoneByName(
		std::string name
	);

	int GetNumVerticesWithMaterialId(
		int materialId
	)
	{
		return m_numVerticesPerMaterialArray[materialId];
	}

	render::AppearanceTable &GetAppearanceTable()
	{
		return m_appearanceTable;
	}


	std::vector<unsigned int> &GetNumVerticesPerMaterialArray()
	{
		return m_numVerticesPerMaterialArray;
	}

	mesh::AnimationInfoPtr GetAnimationInfo()
	{
		return m_animationInfo;
	}

	void SetAnimationInfo(
		mesh::AnimationInfoPtr animationInfo
		)
	{
		m_animationInfo = animationInfo;
	}

private:
	render::AppearanceTable m_appearanceTable; // Mapping of material id's to material names //TODO pointer and set method
	std::vector<unsigned int> m_numVerticesPerMaterialArray; // A count of the number of vertex indices per material batch. Necessary for creating batches later
	container::LinkedTree<Node> m_nodes;
	mesh::AnimationInfoPtr m_animationInfo; // Extra info about all animation tracks
};
}