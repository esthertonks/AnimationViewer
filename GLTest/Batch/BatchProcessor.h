#pragma once

#include "BatchFwdDecl.h"
#include "../Mesh/MeshFwdDecl.h"
#include <vector>

namespace mesh
{
	class Mesh;
}

namespace render //TODO render or batch namespace?
{
	class Batch;
}

namespace batch
{

class BatchProcessor
{
public:
	BatchProcessor();
	~BatchProcessor();

	void CreateBatches(
		mesh::MeshPtr &mesh,
		render::BatchList &renderBatches // Batch vector to fill in
		);

	void AddDuplicateVertex(
		const int oldVertexIndex,
		const render::TexturedVertex &currentVertex,
		render::Batch &batch,
		std::vector<int> &perMaterialOldToNewVertexIndexMap
		);

	void BatchProcessor::PrepareBatches(
		render::BatchList &renderBatches
		);

	void SortBatches();//TODO

};

}

