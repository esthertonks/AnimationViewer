#include "RenderableVertexListFactory.h"
#include "RenderableVertexList.h"
#include "../MeshProcessors/NormalsVertexListProcessor.h"
#include "../MeshProcessors/BoneVertexListProcessor.h"

namespace render 
{
	RenderableVertexListPtr RenderableVertexListFactory::CreateBoneVertexList(
		const glm::vec3 blueColour,
		const int pointSize
		)
	{
		BoneVertexListProcessorPtr boneVertexListProcessorPtr = render::BoneVertexListProcessorPtr(new BoneVertexListProcessor(blueColour));
		VertexListProcessorPtr vertexListProcessor = boost::dynamic_pointer_cast<render::VertexListProcessor>(boneVertexListProcessorPtr);
		return render::RenderableVertexListPtr(new render::RenderableVertexList(vertexListProcessor, pointSize));

	}

	RenderableVertexListPtr RenderableVertexListFactory::CreateNormalsVertexList(
		mesh::MeshPtr mesh,
		const glm::vec3 colour,
		const float normalLength,
		const int pointSize
	)
	{
		NormalsVertexListProcessorPtr normalsVertexListProcessorPtr = render::NormalsVertexListProcessorPtr(new NormalsVertexListProcessor(mesh, colour, normalLength));
		VertexListProcessorPtr vertexListProcessor = boost::dynamic_pointer_cast<render::VertexListProcessor>(normalsVertexListProcessorPtr);
		return RenderableVertexListPtr(new RenderableVertexList(vertexListProcessor, pointSize));
	}
}
