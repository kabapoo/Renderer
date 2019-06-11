#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class ViewControl
{
public:
	ViewControl();
	~ViewControl();

	void SetMVP();
	glm::mat4 GetMVP();

private:
	float FoV;
	float WindowRatio;
	glm::mat4 ModelMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::mat4 MVP;

	glm::vec3 CameraPosition;
	glm::vec3 CameraTarget;
	glm::vec3 UpVector;
};