#include "Vertex.h"
#include "BoneNode.h"

namespace mesh
{
	Vertex::Vertex()
		: m_numInfluences(0)
	{
		for(int influence = 0; influence < MAX_INFLUENCES; influence++)
		{
			m_boneInfluenceIds[influence] = 0; //Max 4 joints per vertex (could use uint8) //TODO has to be float to pass to gl?
			m_boneWeights[influence] = 0.0f;
		}

	}

}