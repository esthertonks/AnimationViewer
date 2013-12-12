#pragma once

#include <string>
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
	mesh::Node* rootBoneNode,
	int frame
	);

void Interpolate(){};

private:

};

}