#pragma once

#include <string>
#include <glm\glm.hpp>
#include "../Container/LinkedList .h"
#include "../Import/FBXImport.h"
#include "../Batch/BatchFwdDecl.h"

namespace mesh
{

class BoneNode : public container::LinkedListItem<BoneNode>
{
public:

BoneNode();
~BoneNode();

void SetName(
	const std::string& name
	)
{
	m_name = name;
};

const std::string&	GetName()
{		
	return m_name;
}

bool isLeaf()
{
	return m_isLeaf;
}

void SetLeaf(
	bool isLeaf
	)
{
	m_isLeaf = isLeaf;
}

void SetGlobalTransform(
	glm::mat4x4 &globalTransform
	)
{
	m_globalTransform = globalTransform;			
};

const glm::mat4x4 &GetGlobalTransform()
{
	return m_globalTransform;
};

void SetLocalKeyTransform(
	glm::mat4x4 &localTransform
	)
{
	m_localTransform = localTransform;
};

glm::mat4x4 &GetLocalKeyTransform(
	int key
	)
{
	return m_localTransform;
};

private:

	friend import::FBXImport; // Friend as the import class needs direct access to these arrays. All other classes accessing a mesh node should use the access function provided.

	std::string m_name;

	glm::mat4x4 m_localTransform;		//Transform from this node to the parent node
	glm::mat4x4 m_globalTransform;		// Transform from this node to the global model transform

	glm::mat4x4 m_inverseReferenceMatrix;	// The inverse reference matrix for this bone

	unsigned int m_boneHierarchyDepth;	// The depth of this bone in the hierarchy
	bool m_isLeaf; // Is this node the end of this chain of bones? ie is it only connected to one other bone and not two
};

}