#include "NormalsVertexListProcessor.h"
#include "../Mesh/BoneNode.h"
#include "../Mesh/MeshNode.h"
#include "../Renderable/RenderableMesh.h"
#include "../Utils/MathsUtils.h"
#include "../Mesh/Mesh.h"
#include "../Mesh/Triangle.h"
#include "../Mesh/Vertex.h"
#include "SkinningMatrixProcessor.h"

namespace render
{

	NormalsVertexListProcessor::NormalsVertexListProcessor(
		mesh::MeshPtr mesh,
		glm::vec3 colour,
		float normalLength
	)
		: m_numVerts(0),
		m_colour(colour),
		m_normalLength(normalLength),
		m_mesh(mesh),
		m_skinningMatrixProcessor(new SkinningMatrixProcessor)
	{
	}

	int NormalsVertexListProcessor::GetNumVertsInList()
	{
		return m_numVerts; // Updated in CreateVertexListFromBonePositions
	}

	ColourVertexArray &NormalsVertexListProcessor::GetVertexList()
	{
		return m_normalsVertexArray;
	}

	// First go through bones to create matrix palette.
	// Each vert has 4 bone ids and 4 weights (MAX_INFLUENCES). Multiply for the weightedBoneMatrix
	// Multiply this by the normal position - we are using model space here.
	// Extrude the normal position for the extra vert to show the normals
	// Store in vertex list
	void NormalsVertexListProcessor::CreateAnimatedVertexList(
		mesh::BoneNode *boneHierarchyRoot
	)
	{
		m_skinningMatrixProcessor->CreateBoneMatrix(boneHierarchyRoot);

		m_normalsVertexArray.clear();

		mesh::MeshNode* rootMeshNode = m_mesh->GetMeshNodeHierarchy();

		CreateVertexListFromNormalsInternal(rootMeshNode);

		m_numVerts = m_normalsVertexArray.size(); // Store so we dont access the size all the time
	}

	void NormalsVertexListProcessor::CreateVertexListFromNormalsInternal(
		mesh::MeshNode* meshNode
	)
	{
		for (meshNode; meshNode != NULL; meshNode = meshNode->m_next)
		{
			mesh::MeshTriangleArrayPtr triangleArray = meshNode->GetTriangles();
			int numTriangles = meshNode->GetNumTriangles();
			if (numTriangles == 0)
			{
				continue;
			}
			mesh::MeshVertexArrayPtr vertexArray = meshNode->GetVertices();
			int numVertices = meshNode->GetNumVertices();
			if (numVertices == 0)
			{
				continue;
			}

			for (int triangleIndex = 0; triangleIndex < numTriangles; triangleIndex++)
			{
				for (int triangleCornerIndex = 0; triangleCornerIndex < 3; triangleCornerIndex++)
				{
					unsigned int vertexIndex = triangleArray[triangleIndex].GetVertexIndex(triangleCornerIndex);

					// Calcualte the skinned position of the verts
					glm::vec4 position;
					utils::MathsUtils::ConvertFBXVector4ToGlVec4(vertexArray[vertexIndex].GetPosition(), position);
					BoneMatrixPalette boneMatrixPalette = m_skinningMatrixProcessor->GetBoneMatrixPalette();

					assert(MAX_INFLUENCES == 4); // If it isn't 4 then this is out of sync with the  shaders which are using vec4 to pass in the parameters.

					glm::mat4 weightedBoneMatrix(0); // Make sure this is zero'd - we dont want an identity matrix.
					for (int boneInfluenceIndex = 0; boneInfluenceIndex < MAX_INFLUENCES; boneInfluenceIndex++)
					{
						weightedBoneMatrix += boneMatrixPalette[vertexArray[vertexIndex].GetBoneInfluenceId(boneInfluenceIndex)] * vertexArray[vertexIndex].GetBoneWeight(boneInfluenceIndex);
					}

					ColourVertex skinnedVertexPosition;
					skinnedVertexPosition.m_position = glm::vec3(weightedBoneMatrix * position);
					skinnedVertexPosition.m_colour = m_colour;

					// Extrapolate normal
					ColourVertex extrudedNormalPosition;	
					glm::vec3 normal = glm::vec3(triangleArray[triangleIndex].GetNormal(triangleCornerIndex));
					glm::vec3 extrudedNormal = glm::normalize(normal) * m_normalLength;
					extrudedNormalPosition.m_position = skinnedVertexPosition.m_position + extrudedNormal;
					extrudedNormalPosition.m_colour = m_colour;

					m_normalsVertexArray.push_back(skinnedVertexPosition);
					m_normalsVertexArray.push_back(extrudedNormalPosition);
				}
			}

			for (mesh::MeshNode *childNode = meshNode->m_firstChild; childNode != NULL; childNode = childNode->m_next)
			{
				CreateVertexListFromNormalsInternal(childNode);
			}
		}
	}

	NormalsVertexListProcessor::~NormalsVertexListProcessor()
	{
	}

}