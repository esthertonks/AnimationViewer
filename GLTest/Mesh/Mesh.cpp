#include "Mesh.h"
#include "BoneNode.h"

namespace mesh
{

Mesh::Mesh()
	: m_meshNode(NULL)
{
}

BoneNode* Mesh::GetBoneByName(
	std::string name
)
{
	for(Node *node = m_nodes.m_root; node != NULL; )
	{
		if(node->GetType() == NodeType::BoneType)
		{
			if(node->GetName() == name)
			{
				return static_cast<BoneNode*>(node);
			}
		}

		node = node->GetNextChildFirst();
	}
	return NULL;
}

Mesh::~Mesh()
{
}

}