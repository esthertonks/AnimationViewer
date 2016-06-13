#pragma once

#include "../Mesh/MeshFwdDecl.h"
#include "../Batch/BatchFwdDecl.h"
#include "VertexListProcessor.h"

namespace render {

class SkinningMatrixProcessor;

class NormalsVertexListProcessor : public VertexListProcessor
{
public:
	NormalsVertexListProcessor(
		mesh::MeshPtr mesh,
		glm::vec3 colour,
		float normalLength
	);

	~NormalsVertexListProcessor();

	virtual int GetNumVertsInList();

	virtual ColourVertexArray &GetVertexList();

	void CreateAnimatedVertexList(
		mesh::BoneNode *boneHierarchyRoot
	);

private:

	void CreateVertexListFromNormalsInternal(
		mesh::MeshNode* meshNode
	);

	ColourVertexArray m_normalsVertexArray;
	int m_numVerts;
	glm::vec3 m_colour;
	float m_normalLength;
	mesh::MeshPtr m_mesh;
	render::SkinningMatrixProcessorPtr m_skinningMatrixProcessor;
};

}
