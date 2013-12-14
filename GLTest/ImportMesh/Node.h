#pragma once

#include "../Container/LinkedTree.h"
#include <string>
#include <glm/glm.hpp>

namespace import
{
	class FBXImport;
}


namespace mesh
{

enum NodeType
{
	MeshType,
	BoneType
};

class Node : public container::LinkedTreeItem<Node>
{
public:

	Node(){};
	~Node(){};

virtual NodeType GetType() = 0;

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

void SetGlobalTransform(
	glm::mat4x4 &globalTransform
	)
{
	m_globalTransform = globalTransform;			
};

const glm::mat4x4 &GetGlobalTransform() const
{
	return m_globalTransform;
};

glm::mat4x4 &GetGlobalTransform()
{
	return m_globalTransform;
};

private:
	friend class import::FBXImport; // Friend as the import class needs direct access to these arrays. All other classes accessing a mesh node should use the access function provided.

	std::string m_name;

	glm::mat4x4 m_globalTransform;		// Transform from this node to the global model transform

	glm::mat4x4 m_inverseReferenceMatrix;	// The inverse reference matrix for this bone
};

}