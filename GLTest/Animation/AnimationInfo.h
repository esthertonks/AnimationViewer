#pragma once

#include <string>
/*
	Information about an animation.
*/

namespace animation
{

class AnimationInfo
{
public :

	AnimationInfo();
	~AnimationInfo(){};

	void SetFrameRate(double fps)
	{
		m_fps = fps;
	}

	void SetNumFrames(float numFrames)
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

	const double GetFrameRate()
	{
		return m_fps;
	}

	const int GetNumFrames()
	{
		return m_numFrames;
	}

	bool GetLoop()
	{
		return m_loop;
	}
				
	const std::string &GetName()
	{
		return m_name;
	}

	double ConvertFrameToMilliseconds(
		int frame
		)
	{
		return floor(frame * m_fps) * 1000;
	}

	int ConvertMillisecondsToFrame(
		double milliseconds
		)
	{
		return milliseconds * m_fps;
	}

private:
	double m_fps;
	int m_numFrames;
	bool m_loop;
	std::string m_name;
};

}