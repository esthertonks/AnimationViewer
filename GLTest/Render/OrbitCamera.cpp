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
	m_forward(0.0f,0.0f,-1.0f),	//Z axis
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
	float amount,
	float aspectRatio
	)
{
	float zoomCorrection = CalculateZoomCorrection(aspectRatio);

	m_position += (m_right * amount) / zoomCorrection;
	m_pivot += (m_right * amount) / zoomCorrection;

	CalculateViewMatrix();
}

void OrbitCamera::PanY(
	float amount,
	float aspectRatio
	)
{
	float zoomCorrection = CalculateZoomCorrection(aspectRatio);

	m_position += (m_up * amount)/ zoomCorrection;
	m_pivot += (m_up * amount) / zoomCorrection;

	CalculateViewMatrix();
}

float OrbitCamera::CalculateZoomCorrection(
	float aspectRatio
	)
{
	glm::vec3 actualPosition = m_position + (m_forward * m_zoom);
	float distance = glm::length(actualPosition);
	return (aspectRatio * 1000)/distance;
}

void OrbitCamera::RotateCamera()
{
	// Translate to the pivot point
	m_position -= m_pivot;

	// Yaw - rotate around y
	glm::mat4x4 yawMatrix;
	yawMatrix = glm::rotate(yawMatrix, m_yaw, m_up);

	//Pitch - rotate around the x (right) axis
	glm::mat4x4 pitchMatrix;
	pitchMatrix = glm::rotate(yawMatrix, m_pitch, m_right);

	// Apply rotation to translated position
	m_position = glm::vec3(glm::vec4(m_position, 1) * pitchMatrix);

	// Translate back
	m_position += m_pivot;

	// Recalculate camera axes
	m_forward = glm::normalize(m_pivot - m_position);
	m_right = glm::normalize(glm::cross(m_forward, m_up));
	m_up = glm::normalize(glm::cross(m_right, m_forward));

}

void OrbitCamera::CalculateViewMatrix()
{
	m_viewMatrix = glm::mat4(1.0); // Set as identity

	m_viewMatrix[0][0] = m_right[0]; m_viewMatrix[0][1] = m_up[0]; m_viewMatrix[0][2] = -m_forward[0];
	m_viewMatrix[1][0] = m_right[1]; m_viewMatrix[1][1] = m_up[1]; m_viewMatrix[1][2] = -m_forward[1];
	m_viewMatrix[2][0] = m_right[2]; m_viewMatrix[2][1] = m_up[2]; m_viewMatrix[2][2] = -m_forward[2];

	m_viewMatrix[3][0] = -glm::dot(m_position + (m_forward * m_zoom), m_right); 
	m_viewMatrix[3][1] = -glm::dot(m_position + (m_forward * m_zoom), m_up);
	m_viewMatrix[3][2] = glm::dot(m_position + (m_forward * m_zoom), m_forward);

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