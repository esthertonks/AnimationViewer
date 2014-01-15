#pragma once

#include "../Mesh/MeshFwdDecl.h"

namespace mesh
{
	class BoneNode;

#define MAX_INFLUENCES 4

class Vertex
{
public:
	Vertex::Vertex();

	void AddWeight(
		const mesh::BoneNode* boneInfluence,
		const float weight
		)
	{
		assert(m_numBoneInfluences <= MAX_INFLUENCES);
		m_boneInfluence[m_numBoneInfluences++] = boneInfluence;
		m_jointWeight[m_numBoneInfluences++] = weight;
	}

	unsigned int GetMaxBoneInfulences()
	{
		return MAX_INFLUENCES;
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

	const mesh::BoneNode* m_boneInfluence[4]; //Max 4 joints per vertex (could use uint8)
	float m_jointWeight[4];
	int m_numBoneInfluences;
};

}