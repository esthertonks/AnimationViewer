#pragma once

#include "BatchFwdDecl.h"
#include "../Mesh/MeshFwdDecl.h"
#include <vector>

namespace mesh
{
	class Mesh;
	class MeshNode;
}

namespace render //TODO render or batch namespace?
{
	class Batch;
}

namespace batch
{

class BatchCreator
{
public:
	BatchCreator();
	~BatchCreator();

	void CreateBatches(
		mesh::MeshPtr &mesh,
		render::PerNodeBatchList &renderBatches // Batch vector to fill in
		);

	void AddDuplicateVertex(
		const int oldVertexIndex,
		const render::TexturedSkinnedVertex &currentVertex,
		render::Batch &batch,
		std::vector<int> &perMaterialOldToNewVertexIndexMap
		);

	void BatchCreator::PrepareBatches(
		render::PerNodeBatchList &perNodeRenderBatches
		);

	void SortBatches();//TODO

private:
	void BatchCreator::CreateBatchesInternal(
		mesh::MeshPtr &mesh,
		mesh::MeshNode* meshNode,
		render::PerNodeBatchList &perNodeRenderBatches // Map of Batch vector lists to fill in - one for each mesh node (ie one per unique model matrix)
	);

};

}

