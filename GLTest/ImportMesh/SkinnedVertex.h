#pragma once

#include "Vertex.h"

namespace mesh
{

class SkinnedVertex : public Vertex
{
public:

	unsigned int m_jointIndex[4]; //Max 4 joints per vertex (could use uint8)
	float m_jointWeight[4];
};

}