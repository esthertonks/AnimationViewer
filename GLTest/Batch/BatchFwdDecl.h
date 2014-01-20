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

typedef std::vector<ColourVertex> ColourVertexArrayPtr;
typedef std::vector<TexturedVertex> TexturedVertexArrayPtr;
typedef std::vector<TexturedSkinnedVertex> TexturedSkinnedVertexArrayPtr;
typedef std::vector<unsigned short> IndexArrayPtr;
typedef boost::shared_ptr<Appearance> AppearancePtr;
typedef boost::shared_ptr<Batch> BatchPtr;
typedef std::vector<BatchPtr> BatchList;
typedef std::map<int, std::vector<BatchPtr>> PerNodeBatchList;

typedef boost::shared_ptr<Renderable> RenderablePtr;
typedef boost::shared_ptr<render::RenderableMesh> RenderableMeshPtr;

typedef boost::shared_ptr<render::RenderableBoneList> RenderableBoneListPtr;

typedef std::map<unsigned int, render::AppearancePtr> AppearanceTable;
typedef std::pair<unsigned int, render::AppearancePtr> AppearanceTableEntry;

}