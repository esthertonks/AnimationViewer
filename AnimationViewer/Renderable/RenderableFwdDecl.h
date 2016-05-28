#pragma once

#include <boost\shared_ptr.hpp>
#include <vector>
#include <map>
#include "../Batch/VertexFormat.h"

namespace render
{
	class Renderable;
	class VertexListCreatorBase;
	class RenderableMesh;
	class RenderableVertexList;
	class RenderableNormalsList;
	class BoneVertexListCreator;
	class NormalsVertexListCreator;
	class BatchCreator;
	class SkinningMatrixCreator;

	typedef std::vector<glm::mat4x4> BoneMatrixPalette;
	typedef std::vector<ColourVertex> ColourVertexArray;

	typedef boost::shared_ptr<Renderable> RenderablePtr;
	typedef boost::shared_ptr<render::VertexListCreatorBase> VertexListCreatorBasePtr;

	typedef boost::shared_ptr<render::RenderableMesh> RenderableMeshPtr;
	typedef boost::shared_ptr<render::BatchCreator> BatchCreatorPtr;
	typedef boost::shared_ptr<render::RenderableVertexList> RenderableVertexListPtr;
	typedef boost::shared_ptr<render::RenderableNormalsList> RenderableNormalsListPtr;
	typedef boost::shared_ptr<render::BoneVertexListCreator> BoneVertexListCreatorPtr;
	typedef boost::shared_ptr<render::NormalsVertexListCreator> NormalsVertexListCreatorPtr;
	typedef boost::shared_ptr<render::SkinningMatrixCreator> SkinningMatrixCreatorPtr;

}