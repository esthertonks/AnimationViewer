#pragma once

/*
	Triangle class storing per triangle corner information so as not to lose data importing the mesh.
	This info can be averaged later if required to reduce it to per vertex information.
*/
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

	void SetNormals(glm::vec4 normals[3])
	{
		m_normals[0] = normals[0];
		m_normals[1] = normals[1];
		m_normals[2] = normals[2];
		m_normals[3] = normals[3];
	};

	void SetUVs(glm::vec2 uvs[3])
	{
		m_uvs[0] = uvs[0];
		m_uvs[1] = uvs[1];
		m_uvs[2] = uvs[2];
		m_uvs[3] = uvs[3];

	};

	void SetColours(glm::vec3 colours[3])
	{		
		m_colours[0] = colours[0];
		m_colours[1] = colours[1];
		m_colours[2] = colours[2];
		m_colours[3] = colours[3];
	};

	unsigned int m_vertexIndices[3];	// Vertex indeces for the 3 triangle corners
	glm::vec4 m_normals[3];				// Normals for the 3 triangle corners (thus multiple normals per vertex)
	glm::vec4 m_binormals[3];			// Binormals for the 3 triangle corners (thus multiple normals per vertex)
	glm::vec4 m_tangents[3];			// Tangents for the 3 triangle corners (thus multiple normals per vertex)
	glm::vec2 m_uvs[3];					// UVs for the 3 triangle corners (thus multiple uvs per vertex)
	glm::vec3 m_colours[3];				// Colours for the 3 triangle corners (thus multiple colours per vertex)
	unsigned int m_materialId;
};

}