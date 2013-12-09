#include "BoneNode.h"
#include "Vertex.h"
#include "Triangle.h"
#include "../Batch/Appearance.h"

namespace mesh
{

BoneNode::BoneNode()
	: m_isLeaf(false),
	m_boneHierarchyDepth(0)
{


};


BoneNode::~BoneNode()
{
};


}