//Vertex array
//Triangle Array
//MeshNodeHierarchy
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