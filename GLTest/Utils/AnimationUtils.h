#pragma once

#include <glm\glm.hpp>

namespace utils
{

class AnimationUtils
{
public:

static double ConvertFrameToMilliseconds(
	const int frame,
	const float fps
	)
{
	return floor(frame * fps) * 1000;
}

static int ConvertMillisecondsToFrame(
	const double milliseconds,
	const float fps
	)
{
	return floor(milliseconds * fps) / 1000;
}
};

}
