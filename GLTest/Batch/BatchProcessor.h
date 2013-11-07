#pragma once

namespace mesh
{
	class Mesh;
	class RenderMesh;
}

namespace batch
{

class BatchProcessor
{
public:
	BatchProcessor();
	~BatchProcessor();

	mesh::RenderMesh *CreateRenderMesh(
		mesh::Mesh &importMesh
		);

};

}

