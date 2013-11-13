#include "FirstPersonCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include <wx/log.h>

namespace render
{

float FirstPersonCamera::TWOPI = glm::pi<float>() * 2;

FirstPersonCamera::FirstPersonCamera()
	: m_up(0.0f,1.0f,0.0f),		// Y axis
	m_forward(0.0f,0.0f,1.0f),	//Z axis
	m_right(1.0f,0.0f,0.0f)	,	//X axis
	m_position(0.0f, 0.0f, -20.f),
	m_yaw(0.0f),
	m_pitch(0.0f),
	m_zoom(glm::length(m_position))
{
	CalculateViewMatrix();
}

FirstPersonCamera::FirstPersonCamera(
	glm::vec3 position
	)
	: m_up(0.0f,1.0f,0.0f),		// Y axis
	m_forward(0.0f,0.0f,1.0f),	//Z axis
	m_right(1.0f,0.0f,0.0f)	,	//X axis
	m_position(position)
{
}

void FirstPersonCamera::SetPosition(
	glm::vec3 &position
	)
{
	m_viewMatrix[3][0] = position[0];
	m_viewMatrix[3][1] = position[1];
	m_viewMatrix[3][2] = position[2];
	m_viewMatrix[3][3] = 1;
}

void FirstPersonCamera::Move(
	float amount
	)
{
	m_position += (m_forward* amount) / (m_zoom / 2);
	m_zoom = glm::length(m_position);
	CalculateViewMatrix();
}

void FirstPersonCamera::MoveLeft(
	float amount
	)
{
	m_position += (m_right * amount) / (m_zoom / 2);
	m_zoom = glm::length(m_position);
	CalculateViewMatrix();
}

void FirstPersonCamera::MoveRight(
	float amount
	)
{
	m_position -= (m_right * amount) / (m_zoom / 2);
	m_zoom = glm::length(m_position);
	CalculateViewMatrix();
}

void FirstPersonCamera::RecalculateAxes()
{
	// Convert to vec4 for glm::rotate's matrix 4x4
	glm::vec4 tempForward(m_forward, 1);
	glm::vec4 tempRight(m_right, 1);
	glm::vec4 tempUp(m_up, 1);

	// Yaw - Rotate forward and right vectors around the up vector
	glm::mat4x4 yawMatrix;
	yawMatrix = glm::rotate(yawMatrix, m_yaw, m_up);

	tempForward = tempForward * yawMatrix;
	m_right = glm::vec3(tempRight * yawMatrix);

	//Pitch - Rotate forward and up vectors around the right vector
	glm::mat4x4 pitchMatrix;
	pitchMatrix = glm::rotate(pitchMatrix, m_pitch, m_right);

	
	m_forward = glm::vec3(tempForward * pitchMatrix);
	m_up = glm::vec3(tempUp * pitchMatrix);

}

void FirstPersonCamera::CalculateViewMatrix()
{
	glm::vec3 target = m_position + m_forward; // Create a target in the direction the camera is facing
	m_viewMatrix = glm::lookAt(m_position, target, m_up);
}

void FirstPersonCamera::RotateAroundX(
	float angle
	)
{
	m_pitch = angle * (m_zoom / 2);
	m_pitch = ClampTo360(m_pitch);
	RecalculateAxes();
	CalculateViewMatrix();
}

void FirstPersonCamera::RotateAroundY(
	float angle
	)
{
	//wxLogDebug("Rot around Yangle : %f\n", angle);

	m_yaw = angle * (m_zoom / 2);
	//wxLogDebug("Yaw1 : %f\n", m_yaw);
	m_yaw = ClampTo360(m_yaw);
	//wxLogDebug("Yaw2 : %f\n", m_yaw);
	RecalculateAxes();
	CalculateViewMatrix();
}

float FirstPersonCamera::ClampTo360(
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