#pragma once

#include <boost\shared_ptr.hpp>
#include <vector>
#include <map>
#include "VertexFormat.h"

//TODO no longer just batch- rename
namespace render
{

class Appearance;
class Batch;
class Renderable;
class RenderableMesh;
class RenderableBoneList;
class RenderableNormalsList;
class BoneVertexListCreator;
class NormalsVertexListCreator;

typedef std::vector<ColourVertex> ColourVertexArray;
typedef std::vector<TexturedVertex> TexturedVertexArray;
typedef std::vector<TexturedSkinnedVertex> TexturedSkinnedVertexArray;
typedef std::vector<unsigned short> IndexArray;

typedef boost::shared_ptr<Appearance> AppearancePtr;
typedef boost::shared_ptr<Batch> BatchPtr;
typedef std::vector<BatchPtr> BatchList;
typedef std::map<int, std::vector<BatchPtr>> PerNodeBatchList;

typedef boost::shared_ptr<Renderable> RenderablePtr;
typedef boost::shared_ptr<render::RenderableMesh> RenderableMeshPtr;

typedef boost::shared_ptr<render::RenderableBoneList> RenderableBoneListPtr;
typedef boost::shared_ptr<render::RenderableNormalsList> RenderableNormalsListPtr;
typedef boost::shared_ptr<render::BoneVertexListCreator> VertexListCreatorPtr;
typedef boost::shared_ptr<render::NormalsVertexListCreator> NormalsVertexListCreatorPtr;

typedef std::map<unsigned int, render::AppearancePtr> AppearanceTable;
typedef std::pair<unsigned int, render::AppearancePtr> AppearanceTableEntry;

}