#pragma once

#include <boost\shared_ptr.hpp>
#include <vector>

namespace render
{

class VertexFormat;
class Appearance;

typedef std::vector<VertexFormat> BatchVertexArrayPtr;
typedef std::vector<unsigned short> IndexArrayPtr;
typedef boost::shared_ptr<Appearance> AppearancePtr;

}