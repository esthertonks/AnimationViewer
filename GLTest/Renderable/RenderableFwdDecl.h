#pragma once

#include <boost\shared_ptr.hpp>
#include <vector>
#include <map>
#include "../Batch/VertexFormat.h"

namespace render
{
	class Renderable;
	class RenderableVertexListCreator;
	class RenderableMesh;
	class RenderableBoneList;
	class RenderableNormalsList;
	class BoneVertexListCreator;
	class NormalsVertexListCreator;
	class BatchCreator;

	typedef std::vector<ColourVertex> ColourVertexArray; //FIMXE shouldnt be here


	typedef boost::shared_ptr<Renderable> RenderablePtr;
	typedef boost::shared_ptr<render::RenderableVertexListCreator> RenderableVertexListCreatorPtr;

	typedef boost::shared_ptr<render::RenderableMesh> RenderableMeshPtr;
	typedef boost::shared_ptr<render::BatchCreator> BatchCreatorPtr;
	typedef boost::shared_ptr<render::RenderableBoneList> RenderableBoneListPtr;
	typedef boost::shared_ptr<render::RenderableNormalsList> RenderableNormalsListPtr;
	typedef boost::shared_ptr<render::BoneVertexListCreator> BoneVertexListCreatorPtr;
	typedef boost::shared_ptr<render::NormalsVertexListCreator> NormalsVertexListCreatorPtr;

}