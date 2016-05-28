#pragma once

#include <boost\shared_ptr.hpp>
#include <vector>
#include <map>
#include "../Batch/VertexFormat.h"

namespace render
{
	class Renderable;
	class VertexListProcessor;
	class RenderableMesh;
	class RenderableVertexList;
	class RenderableNormalsList;
	class BoneVertexListProcessor;
	class NormalsVertexListProcessor;
	class BatchProcessor;
	class SkinningMatrixProcessor;

	typedef std::vector<glm::mat4x4> BoneMatrixPalette;
	typedef std::vector<ColourVertex> ColourVertexArray;

	typedef boost::shared_ptr<Renderable> RenderablePtr;
	typedef boost::shared_ptr<render::VertexListProcessor> VertexListProcessorPtr;

	typedef boost::shared_ptr<render::RenderableMesh> RenderableMeshPtr;
	typedef boost::shared_ptr<render::BatchProcessor> BatchProcessorPtr;
	typedef boost::shared_ptr<render::RenderableVertexList> RenderableVertexListPtr;
	typedef boost::shared_ptr<render::RenderableNormalsList> RenderableNormalsListPtr;
	typedef boost::shared_ptr<render::BoneVertexListProcessor> BoneVertexListProcessorPtr;
	typedef boost::shared_ptr<render::NormalsVertexListProcessor> NormalsVertexListProcessorPtr;
	typedef boost::shared_ptr<render::SkinningMatrixProcessor> SkinningMatrixProcessorPtr;

}