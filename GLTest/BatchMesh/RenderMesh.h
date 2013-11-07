/**
	This class is in a format suited to rendering. It stores information per vertex and splits vertices where information differs on triangle corners.
	It will contain render batches based on material type.
	This class stores the root of the mesh node hierarchy. The mesh hierarchy a flat linked list of parent->child nodes.
*/
#pragma once

#include "../Container/LinkedList .h"
#include "RenderMeshNode.h"

namespace mesh
{

class RenderMeshNode;

class RenderMesh : public container::LinkedList<RenderMeshNode>
{
public:

	RenderMesh::RenderMesh(){};
	RenderMesh::~RenderMesh();

	RenderMeshNode* GetNodeHierarchy()
	{
		return m_root;
	}

	void AddChildNode(
		RenderMeshNode &node
		)
	{
		Add(node);
	}
};
}