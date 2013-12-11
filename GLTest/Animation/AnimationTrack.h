#pragma once

#include "AnimationFwdDecl.h"

/*
	AnimationTrack class containing the rotate, scale and position tracks for each bone
*/

namespace import
{
	class FBXImport;
}

namespace animation
{

class AnimationTrack
{
public :

	AnimationTrack(
		const int numFrames
	);
	~AnimationTrack(){};

	RotationTrackPtr GetRotationTrack()
	{
		return m_rotationTrack;
	}

	ScaleTrackPtr GetScaleTrack()
	{
		return m_scaleTrack;
	}

	PositionTrackPtr GetPositionTrack()
	{
		return m_positionTrack;
	}

private:
	friend class import::FBXImport; // Friend as the import class needs direct access to these arrays. All other classes accessing a mesh node should use the access function provided.

	float fps;
	int numFrames;
	bool loop;

	RotationTrackPtr m_rotationTrack;
	ScaleTrackPtr m_scaleTrack;
	PositionTrackPtr m_positionTrack;
};

}