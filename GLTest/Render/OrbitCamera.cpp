#include "OrbitCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include <wx/log.h>

namespace render
{

float OrbitCamera::TWOPI = glm::pi<float>() * 2;
int OrbitCamera::MAXZOOM = 640;
int OrbitCamera::MINZOOM = 10;//TODO Currently this is right by the camera - isn't this backwards?????

OrbitCamera::OrbitCamera()
	: m_up(0.0f,1.0f,0.0f),		// Y axis
	m_forward(0.0f,0.0f,1.0f),	//Z axis
	m_right(1.0f,0.0f,0.0f)	,	//X axis
	m_position(0.0f, 0.0f, -20.f),
	m_pivot(0.0f, 0.0f, 0.0f),
	m_yaw(0.0f),
	m_pitch(0.0f),
	m_zoom(glm::length(m_position))
{
	RotateCamera();
	CalculateViewMatrix();
}

OrbitCamera::OrbitCamera(
	glm::vec3 position
	)
	: m_up(0.0f,1.0f,0.0f),		// Y axis
	m_forward(0.0f,0.0f,1.0f),	//Z axis
	m_right(1.0f,0.0f,0.0f),	//X axis
	m_pivot(0.0f, 0.0f, 0.0f),
	m_position(position)
{
	RotateCamera();
	CalculateViewMatrix();
}

void OrbitCamera::Zoom(
	float amount
	)
{
	glm::vec3 position = m_position + ((m_forward * amount) / (m_zoom / 2));
	float zoom = glm::length(m_pivot - m_position);

	m_position = position;
	m_zoom = zoom;

	wxLogDebug("zoom %f", m_zoom);

	RotateCamera();
	CalculateViewMatrix();
}

void OrbitCamera::Pan(
	float amount
	)
{
	m_position += (m_right * amount) / (m_zoom / 2);
	m_pivot += (m_right * amount) / (m_zoom / 2);
	//m_zoom = glm::length(m_pivot - m_position);
	RotateCamera();
	CalculateViewMatrix();
}

//void OrbitCamera::MoveRight(
//	float amount
//	)
//{
//	m_position -= (m_right * amount) / (m_zoom / 2);
//	m_pivot -= (m_right * amount) / (m_zoom / 2);
//	//m_zoom = glm::length(m_pivot - m_position);
//	RotateCamera();
//	CalculateViewMatrix();
//}

void OrbitCamera::RotateCamera()
{
	// Yaw - Rotate forward and right vectors around the up vector
	glm::mat4x4 yawMatrix;
	yawMatrix = glm::rotate(yawMatrix, m_yaw, m_up);

	m_position = glm::vec3(glm::vec4(m_position, 1) * yawMatrix);

	// Recalculate camera axes from forward direction and up (which hasn't changed)
	m_forward = glm::normalize(m_pivot - m_position);
	m_right = glm::normalize(glm::cross(m_forward, m_up));

	//Pitch - Rotate forward and up vectors around the right vector
	glm::mat4x4 pitchMatrix;
	pitchMatrix = glm::rotate(pitchMatrix, m_pitch, m_right);

	m_position = glm::vec3(glm::vec4(m_position, 1) * pitchMatrix);

	// Recalculate camera axes from forward direction and right
	m_forward = glm::normalize(m_pivot - m_position);
	m_up = glm::normalize(glm::cross(m_right, m_forward));

}

void OrbitCamera::CalculateViewMatrix()
{
	//m_viewMatrix = glm::lookAt(m_position, m_pivot, m_up);
	m_viewMatrix = glm::mat4(1.0); // Set as identity

	m_viewMatrix[0][0] = m_right[0]; m_viewMatrix[0][1] = m_up[0]; m_viewMatrix[0][2] = -m_forward[0];
	m_viewMatrix[1][0] = m_right[1]; m_viewMatrix[1][1] = m_up[1]; m_viewMatrix[1][2] = -m_forward[1];
	m_viewMatrix[2][0] = m_right[2]; m_viewMatrix[2][1] = m_up[2]; m_viewMatrix[2][2] = -m_forward[2];

	m_viewMatrix[3][0] = -glm::dot(m_position, m_right); 
	m_viewMatrix[3][1] = -glm::dot(m_position, m_up);
	m_viewMatrix[3][2] = glm::dot(m_position, m_forward);

}

void OrbitCamera::RotateAroundX(
	float angle
	)
{
	m_pitch = angle * (m_zoom / 2);
	m_pitch = Clamp(m_pitch);
	//m_pitch = ClampTo360(m_pitch);
	RotateCamera();
	CalculateViewMatrix();
}

void OrbitCamera::RotateAroundY(
	float angle
	)
{
	//wxLogDebug("Rot around Yangle : %f\n", angle);
	m_yaw = angle * (m_zoom / 2);
	m_yaw = Clamp(m_yaw);
	//wxLogDebug("Yaw1 : %f\n", m_yaw);
	//m_yaw = ClampTo360(m_yaw);
	//wxLogDebug("Yaw2 : %f\n", m_yaw);
	RotateCamera();
	CalculateViewMatrix();
}

float OrbitCamera::Clamp(
	float angle
	) const
{
	if(angle > TWOPI)
	{
		angle -= TWOPI;
	}

	if(angle < -TWOPI)
	{
		angle += TWOPI;
	}

	return angle;
}

}