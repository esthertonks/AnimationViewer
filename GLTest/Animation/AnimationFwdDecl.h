#pragma once

#include <boost/shared_array.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp> 
#include <vector>

namespace animation
{
	class RotationTrack;
	class ScaleTrack;
	class PositionTrack;
	class AnimationTrack;

	typedef boost::shared_ptr<animation::AnimationTrack> AnimationTrackPtr;
	typedef boost::shared_ptr<animation::PositionTrack> PositionTrackPtr;
	typedef boost::shared_ptr<animation::RotationTrack> RotationTrackPtr;
	typedef boost::shared_ptr<animation::ScaleTrack> ScaleTrackPtr;

	typedef std::vector<glm::vec3> Vector3Array;
	typedef std::vector<glm::quat> QuaternionArray;
}