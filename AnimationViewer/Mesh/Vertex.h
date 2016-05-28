#pragma once

#include "../Mesh/MeshFwdDecl.h"

namespace mesh
{

class Vertex
{
public:
	Vertex::Vertex();

	void AddWeight(
		const unsigned int boneInfluenceid,
		const float weight
		)
	{
		assert(m_numInfluences < MAX_INFLUENCES);
		m_boneInfluenceIds[m_numInfluences] = boneInfluenceid;
		m_boneWeights[m_numInfluences] = weight;
		m_numInfluences++;
	}

	float GetBoneWeight(
		int influenceIndex
		)
	{
		return m_boneWeights[influenceIndex];
	}

	unsigned int GetBoneInfluence(
		int influenceIndex
		)
	{
		return m_boneInfluenceIds[influenceIndex];
	}

	unsigned int GetNumInfluences()
	{
		return m_numInfluences;
	}

	unsigned int GetBoneInfluenceId(
		int influenceIndex
		)
	{
		return m_boneInfluenceIds[influenceIndex];
	}

	void SetPosition(
		FbxVector4 position
	)
	{
		m_position = position;
	}

	FbxVector4 GetPosition()
	{
		return m_position;
	}

private:
	
	FbxVector4 m_position;
	unsigned int m_boneInfluenceIds[MAX_INFLUENCES]; //MAX_INFLUENCES joints per vertex (could use uint8) //TODO has to be float to pass to gl?
	float m_boneWeights[MAX_INFLUENCES];
	unsigned int m_numInfluences;
};

}