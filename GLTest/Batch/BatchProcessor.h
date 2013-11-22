#pragma once

#include "BatchFwdDecl.h"
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
		mesh::Mesh &importMesh,
		render::BatchList &renderBatches // Batch vector to fill in
		);

	void SortBatches();//TODO

};

}

