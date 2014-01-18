#include "Mesh.h"
#include "BoneNode.h"

namespace mesh
{

Mesh::Mesh()
{
}

BoneNode* Mesh::GetBoneByName(
	std::string name
)
{
	for(Node *node = m_nodes.m_root; node != NULL; node = node->GetNextChildFirst())
	{
		if(node->GetType() == NodeType::BoneType)
		{
			if(node->GetName() == name)
			{
				return static_cast<BoneNode*>(node);
			}
		}
	}

	return NULL;
}

Mesh::~Mesh()
{
}

}