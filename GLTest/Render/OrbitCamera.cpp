#include "OrbitCamera.h"

#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/constants.hpp>

#include <wx/log.h>

namespace render
{

//float OrbitCamera::TWOPI = glm::pi<float>() * 2;

OrbitCamera::OrbitCamera(
	glm::vec3 &position
	)
	: m_up(0.0f,1.0f,0.0f),		// Y axis
	m_forward(0.0f,0.0f,1.0f),	//Z axis
	m_right(1.0f,0.0f,0.0f)	,	//X axis
	m_position(position),
	m_startPosition(position),
	m_pivot(0.0f, 0.0f, 0.0f),
	m_yaw(0.0f),
	m_pitch(0.0f),
	m_maxZoom(500.0f),
	m_minZoom(10 - glm::length(m_position)),
	m_zoom(0.0f)
{
	RotateCamera();
	CalculateViewMatrix();
}

void OrbitCamera::Reset()
{
	m_up = glm::vec3(0.0f,1.0f,0.0f);
	m_forward = glm::vec3(0.0f,0.0f,1.0f);
	m_right = glm::vec3(1.0f,0.0f,0.0f);
	m_position = m_startPosition;
	m_pivot = glm::vec3(0.0f, 0.0f, 0.0f);
	m_yaw = 0.0f;
	m_pitch = 0.0f;
	m_zoom = 0.0f;

	RotateCamera();
	CalculateViewMatrix();
}

void OrbitCamera::Zoom(
	float amount
	)
{
	m_zoom += amount;

	if(m_zoom > m_maxZoom)
	{
		m_zoom = m_maxZoom;
	}
	else if(m_zoom < m_minZoom)
	{
		m_zoom = m_minZoom;
	}

	CalculateViewMatrix();
}

void OrbitCamera::PanX(
	float amount
	)
{
	if(m_zoom == 0)
	{
		m_position += m_right * amount;
		m_pivot += m_right * amount;
	}
	else
	{
		float zoomScalar = m_zoom / 20;
		m_position += (m_right * amount) * zoomScalar;
		m_pivot += (m_right * amount) * zoomScalar;
	}

	CalculateViewMatrix();
}

void OrbitCamera::PanY(
	float amount
	)
{
	if(m_zoom == 0)
	{
		m_position += m_up * amount;
		m_pivot += m_up * amount;
	}
	else
	{
		float zoomScalar = m_zoom / 20;
		m_position += (m_up * amount) * zoomScalar;
		m_pivot += (m_up * amount) * zoomScalar;
	}

	CalculateViewMatrix();
}

void OrbitCamera::RotateCamera()
{
	m_position -= m_pivot;
	// Yaw - Rotate forward and right vectors around the up vector
	glm::mat4x4 yawMatrix;
	yawMatrix = glm::rotate(yawMatrix, m_yaw, m_up);

	m_position = glm::vec3(glm::vec4(m_position, 1) * yawMatrix);

	m_position += m_pivot;

	// Recalculate camera axes from forward direction and up (which hasn't changed)
	m_forward = glm::normalize(m_pivot - m_position);
	m_right = glm::normalize(glm::cross(m_forward, m_up));

	m_position -= m_pivot;
	//Pitch - Rotate forward and up vectors around the right vector
	glm::mat4x4 pitchMatrix;
	pitchMatrix = glm::rotate(pitchMatrix, m_pitch, m_right);

	m_position = glm::vec3(glm::vec4(m_position, 1) * pitchMatrix);
	wxLogDebug("position %f, %f, %f\n", m_position.x, m_position.y, m_position.z);

	m_position += m_pivot;

	// Recalculate camera axes from forward direction and right
	m_forward = glm::normalize(m_pivot - m_position);
	m_up = glm::normalize(glm::cross(m_right, m_forward));

}

void OrbitCamera::CalculateViewMatrix()
{
	m_viewMatrix = glm::mat4(1.0); // Set as identity

	m_viewMatrix[0][0] = m_right[0]; m_viewMatrix[0][1] = m_up[0]; m_viewMatrix[0][2] = -m_forward[0];
	m_viewMatrix[1][0] = m_right[1]; m_viewMatrix[1][1] = m_up[1]; m_viewMatrix[1][2] = -m_forward[1];
	m_viewMatrix[2][0] = m_right[2]; m_viewMatrix[2][1] = m_up[2]; m_viewMatrix[2][2] = -m_forward[2];

	m_viewMatrix[3][0] = -glm::dot(m_position + (m_forward * -m_zoom), m_right); 
	m_viewMatrix[3][1] = -glm::dot(m_position + (m_forward * -m_zoom), m_up);
	m_viewMatrix[3][2] = glm::dot(m_position + (m_forward * -m_zoom), m_forward);

}

void OrbitCamera::RotateAroundX(
	float angle
	)
{
	m_pitch = angle;

	RotateCamera();
	CalculateViewMatrix();
}

void OrbitCamera::RotateAroundY(
	float angle
	)
{
	m_yaw = angle;

	RotateCamera();
	CalculateViewMatrix();
}

}