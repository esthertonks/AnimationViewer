#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

namespace render
{

Camera::Camera()
	: m_up(0.0f,1.0f,0.0f),		// Y axis
	m_forward(0.0f,0.0f,1.0f),	//Z axis
	m_right(1.0f,0.0f,0.0f)	,	//X axis
	m_position(0.0f, 0.0f, -20.f),
	m_yaw(0.0f),
	m_pitch(0.0f)
{
	CalculateViewMatrix();
}

Camera::Camera(
	glm::vec3 position
	)
	: m_up(0.0f,1.0f,0.0f),		// Y axis
	m_forward(0.0f,0.0f,1.0f),	//Z axis
	m_right(1.0f,0.0f,0.0f)	,	//X axis
	m_position(position)
{
}

void Camera::SetPosition(
	glm::vec3 &position
	)
{
	m_viewMatrix[3][0] = position[0];
	m_viewMatrix[3][1] = position[1];
	m_viewMatrix[3][2] = position[2];
	m_viewMatrix[3][3] = 1;
}

void Camera::MoveForward(
	float amount
	)
{
	m_position += m_forward * amount;
	CalculateViewMatrix();
}

void Camera::MoveLeft(
	float amount
	)
{
	m_position += m_right  * amount;
	CalculateViewMatrix();
}

void Camera::MoveRight(
	float amount
	)
{
	m_position -= m_right * amount;
	CalculateViewMatrix();
}

void Camera::MoveBack(
	float amount
	)
{
	m_position -= m_forward * amount;
	CalculateViewMatrix();
}

void Camera::RecalculateAxes()
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

void Camera::CalculateViewMatrix()
{
	glm::vec3 target = m_position + m_forward; // Create a target in the direction the camera is facing
	m_viewMatrix = glm::lookAt(m_position, target, m_up);
}

void Camera::RotateAroundX(
	float angle
	)
{
	m_pitch = angle;
	m_pitch = ClampTo360(m_pitch);
	RecalculateAxes();
	CalculateViewMatrix();
}

void Camera::RotateAroundY(
	float angle
	)
{
	m_yaw = angle;
	m_yaw = ClampTo360(m_yaw);
	RecalculateAxes();
	CalculateViewMatrix();
}

float Camera::ClampTo360(
	float angle
	) const
{
	float halfpi = glm::half_pi<float>();

	while(angle > halfpi)
	{
		angle -= halfpi;
	}

	while(angle < 0)
	{
		angle += halfpi;
	}

	return angle;
}

}