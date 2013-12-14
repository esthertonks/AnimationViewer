#pragma once

#include <string>
#include <glm/glm.hpp>
/*
	Information about an animation.
*/

namespace mesh
{
	class Node;
}

namespace animation
{

class Animator
{
public :

	Animator();
	~Animator(){};

void PrepareBoneHierarcy(
	mesh::Node *node,
	const int frame
	);

void PrepareBoneHierarcy(
	mesh::Node* node,
	const int frame,
	const glm::mat4x4 &parentGlobalScaleMatrix,
	const glm::mat4x4 &parentGlobalRotationMatrix
	);

void Interpolate(){};

private:

};

}