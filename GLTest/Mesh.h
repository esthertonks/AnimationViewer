//Vertex array
//Triangle Array
//MeshNodeHierarchy
#pragma once

#include "SkinnedVertex.h"
#include "Triangle.h"

class MeshNode;
class BoneNode;

namespace mesh
{

	class Mesh
	{
	public:

		MeshNode* GetNodeHierarchy()
		{
			return m_meshHierarchyRoot;
		}

		void AddNode(MeshNode &node)
		{
			m_meshHierarchyRoot->child = &node;
		}


		MeshNode* m_meshHierarchyRoot;
	};
}