#include "NormalsVertexListCreator.h"
#include "../Mesh/BoneNode.h"
#include "../Mesh/MeshNode.h"
#include "RenderableMesh.h"
#include "../Utils/MathsUtils.h"
#include "../Mesh/Mesh.h"
#include "../Mesh/Triangle.h"
#include "../Mesh/Vertex.h"

namespace render
{

	NormalsVertexListCreator::NormalsVertexListCreator(
		mesh::MeshPtr mesh
	)
		: m_numVerts(0),
		m_mesh(mesh)
	{
	}

	int NormalsVertexListCreator::GetNumVertsInList()
	{
		return m_numVerts; // Updated in CreateVertexListFromBonePositions
	}

	ColourVertexArray &NormalsVertexListCreator::GetVertexList()
	{
		return m_vertexArray;
	}

	static unsigned int boneIdCheck = 0;

	bool NormalsVertexListCreator::CreateBoneMatrix( //FIXME duplicated from renderable mesh - remove
		mesh::BoneNode *boneHierarchyRoot
	)
	{
		if (!boneHierarchyRoot)
		{
			return false;
		}
		m_matrixPalette.clear();
		boneIdCheck = 0;

		CreateBoneMatrixInternal(boneHierarchyRoot);

		return true;
	}

	bool NormalsVertexListCreator::CreateBoneMatrixInternal(
		mesh::BoneNode *node
	)
	{
		//TODO these MUST be in the sameorder they were added in import so the id's match up //TODO createPalette method in either animator or mesh class?
		assert(node->GetId() == boneIdCheck);
		glm::mat4x4 bonePaletteMatrix;
		utils::MathsUtils::ConvertFBXToGLMatrix(node->GetGlobalTransform() * node->GetInverseReferenceMatrix(), bonePaletteMatrix);

		m_matrixPalette.push_back(bonePaletteMatrix);

		boneIdCheck++;

		for (mesh::BoneNode* childNode = node->m_firstChild; childNode != NULL; childNode = childNode->m_next)
		{
			CreateBoneMatrixInternal(childNode);
		}

		return true;
	}

	// First go through bones to create matrix palettes.
	// Each vert has 4 bone ids and 4 weights. Multiply for the weightedBoneMatrix
	// Multiply this by the normal position (ie world space??? or can we use model space?)
	// Extrude the normal position for the extra vert to show the normals
	// Store in vertex list

	// FIXME we are about to duplicate the bone matrix palette!!!

	void NormalsVertexListCreator::CreateVertexListFromNormals(
		mesh::BoneNode *boneHierarchyRoot
	)
	{
		CreateBoneMatrix(boneHierarchyRoot);

		m_vertexArray.clear();

		mesh::MeshNode* rootMeshNode = m_mesh->GetMeshNodeHierarchy();

		CreateVertexListFromNormalsInternal(rootMeshNode);

		m_numVerts = m_vertexArray.size(); // Store so we dont access the size all the time
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
				// Assign the mesh information to batches. Reassigning the indices as the data is processed.
				for (int triangleCornerIndex = 0; triangleCornerIndex < 3; triangleCornerIndex++)
				{
					unsigned int vertexIndex = triangleArray[triangleIndex].GetVertexIndex(triangleCornerIndex);

					glm::vec4 position;
					utils::MathsUtils::ConvertFBXVector4ToGlVec4(vertexArray[vertexIndex].GetPosition(), position);
					glm::mat4 weightedBoneMatrix = m_matrixPalette[vertexArray[vertexIndex].GetBoneInfluenceId(0)] * vertexArray[vertexIndex].GetBoneWeight(0)
						+ m_matrixPalette[vertexArray[vertexIndex].GetBoneInfluenceId(1)] * vertexArray[vertexIndex].GetBoneWeight(1)
						+ m_matrixPalette[vertexArray[vertexIndex].GetBoneInfluenceId(2)] * vertexArray[vertexIndex].GetBoneWeight(2)
						+ m_matrixPalette[vertexArray[vertexIndex].GetBoneInfluenceId(3)] * vertexArray[vertexIndex].GetBoneWeight(3);

					ColourVertex skinnedVertexPosition;
					skinnedVertexPosition.m_position = glm::vec3(weightedBoneMatrix * position);
					skinnedVertexPosition.m_colour = glm::vec3(col, 0.0, 0.0);

					// Extrapolate normal
					ColourVertex extrudedNormalPosition;	
					glm::vec3 normal = glm::vec3(triangleArray[triangleIndex].GetNormal(triangleCornerIndex));
					extrudedNormalPosition.m_position = skinnedVertexPosition.m_position + (glm::normalize(normal) * extrudeFactor);
					extrudedNormalPosition.m_colour = glm::vec3(col, 0.0, 0.0);

					m_vertexArray.push_back(skinnedVertexPosition);
					m_vertexArray.push_back(extrudedNormalPosition);
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