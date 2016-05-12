#include "BoneListCreator.h"
#include "../Mesh/BoneNode.h"

namespace render
{

BoneListCreator::BoneListCreator()
{
}

static float col = 0.0f;
void BoneListCreator::AddPositionToVertexList(
	mesh::BoneNode *boneNode,
	ColourVertexArrayPtr &vertexArray
)
{
	for (boneNode; boneNode != NULL; boneNode = boneNode->m_next)
	{
		if (boneNode->m_parent) // Dont bother with the root - it will be added as the parent of it's children
		{
			// Add the parent nodes position
			ColourVertex parentVertex;
			FbxAMatrix& parentGlobalTransform = boneNode->m_parent->GetGlobalTransform();
			parentVertex.m_position.x = static_cast<float>(parentGlobalTransform[3][0]);
			parentVertex.m_position.y = static_cast<float>(parentGlobalTransform[3][1]);
			parentVertex.m_position.z = static_cast<float>(parentGlobalTransform[3][2]);
			parentVertex.m_colour = glm::vec3(col, 0.0, 0.0);

			vertexArray.push_back(parentVertex);

			// Add this nodes position
			ColourVertex vertex;
			FbxAMatrix& globalTransform = boneNode->GetGlobalTransform();
			vertex.m_position.x = static_cast<float>(globalTransform[3][0]);
			vertex.m_position.y = static_cast<float>(globalTransform[3][1]);
			vertex.m_position.z = static_cast<float>(globalTransform[3][2]);
			vertex.m_colour = glm::vec3(col, 0.0, 0.0);
			col += 0.02f;

			vertexArray.push_back(vertex);
		}

		if (boneNode->m_firstChild)
		{
			AddPositionToVertexList(boneNode->m_firstChild, vertexArray); // If this node has any children then add their info two
		}
	}
}

BoneListCreator::~BoneListCreator()
{
}

}