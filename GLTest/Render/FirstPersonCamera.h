#pragma once

#include <glm/glm.hpp>

namespace render
{
class FirstPersonCamera
{

public:
	FirstPersonCamera();
	FirstPersonCamera(
		glm::vec3 position
		);

	glm::mat4x4 &GetViewMatrix()
	{
		return m_viewMatrix;
	};

	void SetPosition(
		glm::vec3 &position
		);

	void Move(
		float amount
		);

	void MoveLeft(
		float amount
		);

	void MoveRight(
		float amount
		);

	void LookAt(
		glm::vec3 &position
		);

	void RotateAroundX(
		float radians
		);

	void RotateAroundY(
		float radians
		);

	void CalculateViewMatrix();

private:

	void RecalculateAxes();

	float ClampTo360(
		float angle
		) const;

	glm::mat4x4 m_viewMatrix;
	glm::mat4x4 m_projectionMatrix;

	glm::vec3 m_position;

	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_forward;

	//float m_speed;
	//float m_zoom;

	float m_yaw;	//Rotation around y axis
	float m_pitch; // Rotation around x axis;

	float m_zoom;	//Amount the camera is currently moved from 0

	static float TWOPI;
};
}

