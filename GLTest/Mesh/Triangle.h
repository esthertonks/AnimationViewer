#pragma once

#include "glm\glm.hpp"
/*
	Triangle class storing per triangle corner information so as not to lose data importing the mesh.
	This info can be averaged later if required to reduce it to per vertex information.
*/
namespace mesh
{

class Triangle
{
public :

	unsigned int GetMaterialId()
	{
		return m_materialId;
	}

	void SetMaterialId(
		unsigned int materialId
		)
	{
		m_materialId = materialId;
	}

	int GetVertexIndex(
		int triangleCornerId
		)
	{
		return m_vertexIndices[triangleCornerId];
	};

	void SetVertexIndex(
		int triangleCornerId,
		int vertexIndex
		)
	{
		m_vertexIndices[triangleCornerId] = vertexIndex;
	};

	void SetVertexIndices(
		unsigned int vertexIndices[3]
		)
	{
		m_vertexIndices[0] = vertexIndices[0];
		m_vertexIndices[1] = vertexIndices[1];
		m_vertexIndices[2] = vertexIndices[2];
	};

	void SetNormals(
		glm::vec4 normals[3]
		)
	{
		m_normals[0] = normals[0];
		m_normals[1] = normals[1];
		m_normals[2] = normals[2];
		m_normals[3] = normals[3];
	};

	void SetNormal(
		glm::vec4 normal,
		int triangleCornerId
		)
	{
		m_normals[triangleCornerId] = normal;

	};

	glm::vec4 GetNormal(
		int triangleCornerId
	)
	{
		return m_normals[triangleCornerId];
	}

	void SetBinormals(
		glm::vec4 binormals[3]
		)
	{
		m_binormals[0] = binormals[0];
		m_binormals[1] = binormals[1];
		m_binormals[2] = binormals[2];
		m_binormals[3] = binormals[3];
	};

	void SetBinormal(
		glm::vec4 binormal,
		int triangleCornerId
		)
	{
		m_binormals[triangleCornerId] = binormal;

	};

	glm::vec4 GetBinormal(
		int triangleCornerId
	)
	{
		return m_binormals[triangleCornerId];
	}

	void SetTangents(
		glm::vec4 tangents[3]
		)
	{
		m_tangents[0] = tangents[0];
		m_tangents[1] = tangents[1];
		m_tangents[2] = tangents[2];
		m_tangents[3] = tangents[3];
	};

	void SetTangent(
		glm::vec4 tangent,
		int triangleCornerId
		)
	{
		m_tangents[triangleCornerId] = tangent;

	};

	glm::vec4 GetTangent(
		int triangleCornerId
	)
	{
		return m_tangents[triangleCornerId];
	}

	void SetUVs(
		glm::vec2 uvs[3]
		)
	{
		m_uvs[0] = uvs[0];
		m_uvs[1] = uvs[1];
		m_uvs[2] = uvs[2];
		m_uvs[3] = uvs[3];

	};

  	void SetUV(
		glm::vec2 uv,
		int triangleCornerId
		)
	{
		m_uvs[triangleCornerId] = uv;

	};

	glm::vec2 GetUV(
		int triangleCornerId
	)
	{
		return m_uvs[triangleCornerId];
	}

	glm::vec3 GetColour(
		int triangleCornerId
	)
	{		
		return m_colours[triangleCornerId];
	};

	void SetColour(
		glm::vec3 colour,
		int triangleCornerId
		)
	{		
		m_colours[triangleCornerId] = colour;
	};

	void SetColours(
		glm::vec3 colours[3]
		)
	{		
		m_colours[0] = colours[0];
		m_colours[1] = colours[1];
		m_colours[2] = colours[2];
		m_colours[3] = colours[3];
	};

private:
		
	//TODO fbx colour etc?

	unsigned int m_vertexIndices[3];	// Vertex Indices for the 3 triangle corners
	glm::vec4 m_normals[3];				// Normals for the 3 triangle corners (thus multiple normals per vertex)
	glm::vec4 m_binormals[3];			// Binormals for the 3 triangle corners (thus multiple normals per vertex)
	glm::vec4 m_tangents[3];			// Tangents for the 3 triangle corners (thus multiple normals per vertex)
	glm::vec2 m_uvs[3];					// UVs for the 3 triangle corners (thus multiple uvs per vertex)
	glm::vec3 m_colours[3];				// Colours for the 3 triangle corners (thus multiple colours per vertex)
	unsigned int m_materialId;			// Material Id. The mesh must have a material - a default one will be assigned if it does not
};

}