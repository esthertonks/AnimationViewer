#include "BoneVertexListCreator.h"
#include "../Mesh/BoneNode.h"

namespace render
{

BoneVertexListCreator::BoneVertexListCreator(
		glm::vec3 colour
	)
	: m_numVerts(0),
	m_colour(colour)
{
}

int BoneVertexListCreator::GetNumVertsInList()
{
	return m_numVerts; // Updated in CreateVertexListFromBonePositions
}

ColourVertexArray &BoneVertexListCreator::GetVertexList()
{
	return m_vertexArray;
}

void BoneVertexListCreator::CreateAnimatedVertexList(
	mesh::BoneNode *boneHierarchyRoot
)
{
	m_vertexArray.clear();

	UpdateVertexListFromBonePositions(boneHierarchyRoot);

	m_numVerts = m_vertexArray.size(); // Store so we dont access the size all the time
}

void BoneVertexListCreator::UpdateVertexListFromBonePositions(
	mesh::BoneNode *boneNode
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
			parentVertex.m_colour = m_colour;

			m_vertexArray.push_back(parentVertex);

			// Add this nodes position
			ColourVertex vertex;
			FbxAMatrix& globalTransform = boneNode->GetGlobalTransform();
			vertex.m_position.x = static_cast<float>(globalTransform[3][0]);
			vertex.m_position.y = static_cast<float>(globalTransform[3][1]);
			vertex.m_position.z = static_cast<float>(globalTransform[3][2]);
			vertex.m_colour = m_colour;

			m_vertexArray.push_back(vertex);
		}

		if (boneNode->m_firstChild)
		{
			UpdateVertexListFromBonePositions(boneNode->m_firstChild); // If this node has any children then add their info two
		}
	}
}

BoneVertexListCreator::~BoneVertexListCreator()
{
}

}