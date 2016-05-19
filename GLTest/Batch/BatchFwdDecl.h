#pragma once

#include <boost\shared_ptr.hpp>
#include <vector>
#include <map>
#include "VertexFormat.h"

namespace render
{

class Appearance;
class Batch;

typedef std::vector<TexturedVertex> TexturedVertexArray;
typedef std::vector<TexturedSkinnedVertex> TexturedSkinnedVertexArray;
typedef std::vector<unsigned short> IndexArray;

typedef boost::shared_ptr<Appearance> AppearancePtr;
typedef boost::shared_ptr<Batch> BatchPtr;
typedef std::vector<BatchPtr> BatchList;
typedef std::map<int, std::vector<BatchPtr>> PerNodeBatchList;

typedef std::map<unsigned int, render::AppearancePtr> AppearanceTable;
typedef std::pair<unsigned int, render::AppearancePtr> AppearanceTableEntry;

}