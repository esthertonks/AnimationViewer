#pragma once

#include <boost\shared_ptr.hpp>
#include <vector>
#include "VertexFormat.h"

namespace render
{

class Appearance;
class Batch;

typedef std::vector<Vertex> BatchVertexArrayPtr;
typedef std::vector<unsigned short> IndexArrayPtr;
typedef boost::shared_ptr<Appearance> AppearancePtr;
typedef boost::shared_ptr<Batch> BatchPtr;
typedef std::vector<BatchPtr> BatchList;

}