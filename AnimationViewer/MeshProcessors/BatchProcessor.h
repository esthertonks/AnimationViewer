#pragma once

#include "../Mesh/MeshFwdDecl.h"
#include "../Batch/BatchFwdDecl.h"
#include <vector>

namespace mesh
{
	class Mesh;
	class MeshNode;
}

namespace render
{
class Batch;

class BatchProcessor
{
public:
	BatchProcessor(
		mesh::MeshPtr &mesh
		);
	~BatchProcessor();

	void CreateBatches(
		render::PerNodeBatchList &renderBatches // Batch vector to fill in
		);

	void AddNewBatch(
		render::BatchList &renderBatches,
		mesh::MeshNode* meshNode,
		render::AppearanceTable& appearances,
		const int materialId
	);

	void CreateRenderVertex(
		render::TexturedSkinnedVertex &vertex, // Vertex to create
		const mesh::MeshVertexArray &vertexArray, // Vertex array containing the vertices
		const int vertexIndex, // The index of a vertex to copy
		const mesh::MeshTriangleArray &triangleArray, // The traingle array containing all the triangles
		const int triangleIndex, // The index of the triangle we need to copy a vert from
		const int triangleCornerIndex // The specific vertex in the traingle that we need to copy
	);

	void AddDuplicateVertex(
		const int oldVertexIndex,
		const render::TexturedSkinnedVertex &currentVertex,
		render::Batch &batch,
		std::vector<int> &perMaterialOldToNewVertexIndexMap
		);

	void BatchProcessor::PrepareBatches(
		render::PerNodeBatchList &perNodeRenderBatches
		);

	void SortBatches();//TODO

private:
	void BatchProcessor::CreateBatchesInternal(
		mesh::MeshNode* meshNode,
		render::PerNodeBatchList &perNodeRenderBatches // Map of Batch vector lists to fill in - one for each mesh node (ie one per unique model matrix)
	);

	mesh::MeshPtr m_mesh;

};

}

