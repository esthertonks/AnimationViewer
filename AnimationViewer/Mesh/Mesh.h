/**
	This class contains a mesh suitable for loaded information which is stored per triangle (and not per vertex as will be required to render
	This class stores the root of the mesh node hierarchy. The mesh hierarchy is a linked tree of parent->child nodes.
*/
#pragma once

#include "../Container/LinkedTree.h"
#include "MeshFwdDecl.h"

namespace mesh
{

class Mesh
{
public:

	Mesh::Mesh();
	Mesh::~Mesh();

	BoneNode* GetBoneNodeHierarchy()
	{
		return m_boneNodes.m_root;
	}

	MeshNode* GetMeshNodeHierarchy()
	{
		return m_meshNodes.m_root;
	}

	void AddChildBoneNode(
		BoneNode *parent,	// Parent node or NULL if no parent
		BoneNode *node		// Child node to add
		)
	{
		m_boneNodes.AddAsChild(parent, node);
		m_numBoneNodes++;
	}

	void AddChildMeshNode(
		MeshNode *parent,	// Parent node or NULL if no parent
		MeshNode *node		// Child node to add
		)
	{
		m_meshNodes.AddAsChild(parent, node);
		m_numMeshNodes++;
	}

	BoneNode* GetBoneByName(
		const std::string &name
	);

	unsigned int GetNumMeshNodes()
	{
		return m_numMeshNodes;
	};

	unsigned int GetNumBoneNodes()
	{
		return m_numBoneNodes;
	};

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
	container::LinkedTree<BoneNode> m_boneNodes;
	container::LinkedTree<MeshNode> m_meshNodes;
	mesh::AnimationInfoPtr m_animationInfo; // Extra info about all animation tracks
	unsigned int m_numMeshNodes;
	unsigned int m_numBoneNodes;
};
}