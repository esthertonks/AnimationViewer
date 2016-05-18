#include "NormalsVertexListCreator.h"
#include "../../Mesh/BoneNode.h"
#include "../../Mesh/MeshNode.h"
#include "../RenderableMesh.h"
#include "../../Utils/MathsUtils.h"
#include "../../Mesh/Mesh.h"
#include "../../Mesh/Triangle.h"
#include "../../Mesh/Vertex.h"
#include "../../Render/SkinningMatrixCreator.h"

namespace render
{

	NormalsVertexListCreator::NormalsVertexListCreator(
		mesh::MeshPtr mesh
	)
		: m_numVerts(0),
		m_mesh(mesh),
		m_skinningMatrixCreator(new SkinningMatrixCreator)
	{
	}

	int NormalsVertexListCreator::GetNumVertsInList()
	{
		return m_numVerts; // Updated in CreateVertexListFromBonePositions
	}

	ColourVertexArray &NormalsVertexListCreator::GetVertexList()
	{
		return m_normalsVertexArray;
	}

	// First go through bones to create matrix palette.
	// Each vert has 4 bone ids and 4 weights (MAX_INFLUENCES). Multiply for the weightedBoneMatrix
	// Multiply this by the normal position - we are using model space here.
	// Extrude the normal position for the extra vert to show the normals
	// Store in vertex list
	void NormalsVertexListCreator::CreateVertexListFromNormals(
		mesh::BoneNode *boneHierarchyRoot
	)
	{
		m_skinningMatrixCreator->CreateBoneMatrix(boneHierarchyRoot);

		m_normalsVertexArray.clear();

		mesh::MeshNode* rootMeshNode = m_mesh->GetMeshNodeHierarchy();

		CreateVertexListFromNormalsInternal(rootMeshNode);

		m_numVerts = m_normalsVertexArray.size(); // Store so we dont access the size all the time
	}

	static float col = 4.0f;
	static float extrudeFactor = 2.0; //FIXME pass in as parameter
	void NormalsVertexListCreator::CreateVertexListFromNormalsInternal(
		mesh::MeshNode* meshNode
	)
	{
		for (meshNode; meshNode != NULL; meshNode = meshNode->m_next)
		{
			mesh::MeshTriangleArray triangleArray = meshNode->GetTriangles();
			int numTriangles = meshNode->GetNumTriangles();
			if (numTriangles == 0)
			{
				continue;
			}
			mesh::MeshVertexArray vertexArray = meshNode->GetVertices();
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
					BoneMatrixPalette boneMatrixPalette = m_skinningMatrixCreator->GetBoneMatrixPalette();

					assert(MAX_INFLUENCES == 4); // If it isn't 4 then this is out of sync with the  shaders which are using vec4 to pass in the parameters.

					glm::mat4 weightedBoneMatrix(0); // Make sure this is zero'd - we dont want an identity matrix.
					for (int boneInfluenceIndex = 0; boneInfluenceIndex < MAX_INFLUENCES; boneInfluenceIndex++)
					{
						weightedBoneMatrix += boneMatrixPalette[vertexArray[vertexIndex].GetBoneInfluenceId(boneInfluenceIndex)] * vertexArray[vertexIndex].GetBoneWeight(boneInfluenceIndex);
					}

					ColourVertex skinnedVertexPosition;
					skinnedVertexPosition.m_position = glm::vec3(weightedBoneMatrix * position);
					skinnedVertexPosition.m_colour = glm::vec3(col, 0.0, 0.0);

					// Extrapolate normal
					ColourVertex extrudedNormalPosition;	
					glm::vec3 normal = glm::vec3(triangleArray[triangleIndex].GetNormal(triangleCornerIndex));
					extrudedNormalPosition.m_position = skinnedVertexPosition.m_position + (glm::normalize(normal) * extrudeFactor);
					extrudedNormalPosition.m_colour = glm::vec3(col, 0.0, 0.0);

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

	NormalsVertexListCreator::~NormalsVertexListCreator()
	{
	}

}