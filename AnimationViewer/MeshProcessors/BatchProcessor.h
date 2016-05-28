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

