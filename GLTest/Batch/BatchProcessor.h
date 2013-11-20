#pragma once

namespace mesh
{
	class Mesh;
}

namespace render //TODO render or batch namespace?
{
	class BatchList;
}

namespace batch
{

class BatchProcessor
{
public:
	BatchProcessor();
	~BatchProcessor();

	render::BatchList *CreateBatches(
		mesh::Mesh &importMesh
		);

	void SortBatches();//TODO

};

}

