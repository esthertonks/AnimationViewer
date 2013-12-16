//#include "QuaternionTrack.h"
//
//namespace animation
//{
//
//QuaternionTrack::QuaternionTrack(
//	const int numFrames
//	) : Track()
//{
//	m_keys.reserve(numFrames);
//}
//
//void QuaternionTrack::AddKey(
//	const glm::quat &rotation
//	)
//{
//	m_keys.push_back(rotation);
//}
//
//const glm::quat &QuaternionTrack::GetKey(
//	int frame
//	)
//{
//
//	return m_keys[frame];
//}
//
//}