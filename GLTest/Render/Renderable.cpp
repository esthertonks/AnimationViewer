#pragma once

#include "Renderable.h"

namespace render
{

Renderable::Renderable()
: m_rotAroundY(0),
m_rotAroundX(0),
m_modelMatrix(1.0f)
{

}

Renderable::~Renderable()
{

}

}