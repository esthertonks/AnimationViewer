#pragma once

#include <string>
#include <glm\glm.hpp>

namespace mesh
{

	class BoneNode
	{
		std::string name;

		glm::mat4x4 localTransform;			//Transform from this node to the parent node
		glm::mat4x4 globalTransform;		// Transform from this node to the world //TODO to the world or to the mesh?

		// InverseReferenceMatrix - better name?
		//Vertex influences

	};

}