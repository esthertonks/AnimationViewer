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

typedef std::vector<Vertex> BatchVertexArrayPtr;
typedef std::vector<unsigned short> IndexArrayPtr;
typedef boost::shared_ptr<Appearance> AppearancePtr;
typedef boost::shared_ptr<Batch> BatchPtr;
typedef std::vector<BatchPtr> BatchList;

typedef boost::shared_ptr<Renderable> RenderablePtr;
typedef boost::shared_ptr<render::RenderableMesh> RenderableMeshPtr;

typedef std::map<unsigned int, render::AppearancePtr> AppearanceTable;
typedef std::pair<unsigned int, render::AppearancePtr> AppearanceTableEntry;

}