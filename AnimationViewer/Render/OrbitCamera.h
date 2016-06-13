#pragma once

#include "../Render/RenderFwdDecl.h"

namespace render
{
class OrbitCamera
{

public:
	OrbitCamera(
		glm::vec3 &position
		);

	glm::mat4x4 &GetViewMatrix()
	{
		return m_viewMatrix;
	};

	glm::vec3 &GetPivot()
	{
		return m_pivot;
	}

	void Reset();

	void Zoom(
		float amount
		);

	void PanX(
		float amount,
		float aspectRatio
		);

	void PanY(
		float amount,
		float aspectRatio
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

private:
	void CalculateViewMatrix();
	void RotateCamera();

	float CalculateZoomCorrection(
		float aspectRatio
		);

	glm::mat4x4 m_viewMatrix;
	glm::mat4x4 m_projectionMatrix;

	glm::vec3 m_position;
	glm::vec3 m_startPosition;

	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_forward;

	glm::vec3 m_pivot;

	float m_yaw;	//Rotation around y axis
	float m_pitch; // Rotation around x axis;

	float m_zoom;	//Amount the camera is currently moved from 0

	float m_maxZoom;
	float m_minZoom;
};
}

