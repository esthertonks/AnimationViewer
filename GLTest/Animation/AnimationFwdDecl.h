#pragma once

#include <fbxsdk.h>
#include <boost/shared_ptr.hpp>
//#include <glm/glm.hpp>
//#include <glm/gtx/quaternion.hpp> 
//#include <vector>

namespace animation
{
	class AnimationController;
	//class Key;
	//class VectorKey;
	//class QuaternionKey;

	//class Track;
	//class VectorTrack;
	//class QuaternionTrack;

	//typedef boost::shared_ptr<animation::Track> TrackPtr;
	//typedef boost::shared_ptr<animation::VectorTrack> VectorTrackPtr;
	//typedef boost::shared_ptr<animation::QuaternionTrack> QuaternionTrackPtr;

	//typedef std::vector<boost::shared_ptr<Key>> KeyArray;
	typedef boost::shared_ptr<animation::AnimationController> AnimationControllerPtr;
}