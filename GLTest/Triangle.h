#pragma once

namespace mesh
{

class Triangle
{
public :

	void SetVertexIndex(
		int triangleCornerId,
		int vertexIndex
		)
	{
		m_vertexIndices[triangleCornerId] = vertexIndex;
	};

	void SetVertexIndeces(
		unsigned int vertexIndices[3]
		)
	{
		m_vertexIndices[0] = vertexIndices[0];
		m_vertexIndices[1] = vertexIndices[1];
		m_vertexIndices[2] = vertexIndices[2];
	};

private:
	unsigned int m_vertexIndices[3];	// Vertex indeces for the 3 triangle corners
};

}