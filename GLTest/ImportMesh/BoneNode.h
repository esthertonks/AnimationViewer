#pragma once

#include <string>
#include <glm\glm.hpp>
#include "../Container/LinkedList .h"
#include "../Import/FBXImport.h"
#include "../Batch/BatchFwdDecl.h"

namespace mesh
{

	class BoneNode
	{
		friend import::FBXImport; // Friend as the import class needs direct access to these arrays. All other classes accessing a mesh node should use the access function provided.

		std::string m_name;

		glm::mat4x4 m_localTransform;		//Transform from this node to the parent node
		glm::mat4x4 m_globalTransform;		// Transform from this node to the global model transform

		glm::mat4x4 m_inverseReferenceMatrix;	//!< The inverse reference matrix for this bone

		unsigned int m_hierarchyOrdinal;			//!< Index of this node in parent-first hierarchy traversal order

	};

}