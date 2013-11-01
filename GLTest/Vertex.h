#pragma once


namespace mesh
{

class Vertex
{
public:

	float m_position[3];
	float m_normal[3];//Vertex normal
	float m_u;
	float m_v;
	float m_colour;
};

}