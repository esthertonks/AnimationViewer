/**
	This class stores the root of the mesh node hierarchy. The mesh hierarchy a flat linked list of parent->child nodes.
*/
#pragma once

#include <map>
#include "MeshNode.h"

namespace mesh
{

class MeshNode;
class BoneNode;

class Mesh
{
public:

	MeshNode* GetNodeHierarchy()
	{
		return m_meshHierarchyRoot;
	}

	void AddNode(
		MeshNode &node
		)
	{
		m_meshHierarchyRoot->child = &node;
	}

	MeshNode *m_meshHierarchyRoot;
};
}