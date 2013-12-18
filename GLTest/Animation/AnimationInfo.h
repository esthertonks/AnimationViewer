#pragma once

#include <string>
#include "../Utils/MathsUtils.h"
/*
	Information about an animation.
*/

namespace animation
{

	//TODO better called animation details?
class AnimationInfo
{
public :

	AnimationInfo();
	~AnimationInfo(){};

	void SetFPS(double fps)
	{
		m_fps = utils::MathsUtils::Clamp(fps, 0.01f, 1000.0f);
		m_millisecondsPerFrame = 1000.0f/m_fps;
		m_millisecondsPerHalfFrame = 500.0f/m_fps;
	}

	void SetNumFrames(int numFrames)
	{
		m_numFrames = numFrames;
	}

	void SetLoop(bool loop)
	{
		m_loop = loop;
	}
				
	void SetName(const std::string &name)
	{
		m_name = name;
	}

	void SetStartTime(
		long startTime
		)
	{
		m_startTime = startTime;
	}

	void SetEndTime(
		long endTime
		)
	{
		m_endTime = endTime;
	}

	const double GetFPS() const
	{
		return m_fps;
	}

	const int GetNumFrames() const
	{
		return m_numFrames;
	}

	bool GetLoop() const
	{
		return m_loop;
	}
				
	const std::string &GetName() const
	{
		return m_name;
	}

	const long GetStartTime() const
	{
		return m_startTime;
	}

	const long GetEndTime() const
	{
		return m_endTime;
	}

	float ConvertFrameToMilliseconds(
		const int frame
	)
	{
		return floor(static_cast<float>(frame) * m_millisecondsPerFrame);
	}

 int ConvertMillisecondsToFrame(
	const double milliseconds
	)
	{
		return static_cast<int>(floor(milliseconds / m_millisecondsPerFrame));
	}

private:
	// Each frame is 1000 milliseconds apart ie one second
	// Milliseconds per frame = 1000/m_fps; ie If we used seconds it would be 1/m_fps
	// Global time = global start time + ((millisecondsPerFrame/m_fps) * localTime)
	// Local time = (globalTime - localTime) * m_fps
	int m_numFrames;
	bool m_loop;
	std::string m_name;
	long m_startTime;
	long m_endTime;
	float m_fps;
	float m_millisecondsPerFrame;	//1000.0f/m_fps;
	float m_millisecondsPerHalfFrame; //500.0f/m_fps;
};

}