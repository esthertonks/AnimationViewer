/**
	This class contains a mesh suitable for loaded information which is stored per triangle (and not per vertex as will be required to render
	This class stores the root of the mesh node hierarchy. The mesh hierarchy a flat linked list of parent->child nodes.
*/
#pragma once

#include "../Container/LinkedList .h"
#include "MeshNode.h"

namespace mesh
{

class MeshNode;
class BoneNode;

class Mesh : public container::LinkedList<MeshNode>
{
public:

	Mesh::Mesh();
	Mesh::~Mesh();

	MeshNode* GetNodeHierarchy()
	{
		return m_root;
	}

	void AddChildNode(
		MeshNode &node
		)
	{
		Add(node);
	}
};
}