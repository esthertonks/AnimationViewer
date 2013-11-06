#pragma once

namespace mesh
{
	class Mesh;
}

namespace batch
{

class BatchProcessor
{
public:
	BatchProcessor();
	~BatchProcessor();

	mesh::Mesh &ProcessImportMesh(
		mesh::Mesh &importMesh
		);

};

}

