#pragma once

#include "../Mesh/MeshFwdDecl.h"
#include "../Batch/BatchFwdDecl.h"
#include <vector>

namespace render
{
class Batch;

class BatchProcessor
{
public:
	BatchProcessor(
		const mesh::MeshPtr &mesh
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

	void AddVertexToBatch(
		const int oldVertexIndex,
		const render::TexturedSkinnedVertex &vertex,
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
		render::PerNodeBatchList &m_perNodeRenderBatches // Map of Batch vector lists to fill in - one for each mesh node (ie one per unique model matrix)
	);

	const mesh::MeshPtr m_mesh;

};

}

