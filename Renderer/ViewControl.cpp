#include "ViewControl.h"

ViewControl::ViewControl()
{
	FoV = 45.0;
	WindowRatio = 16.0 / 9.0;
	CameraPosition = glm::vec3(0.0, 0.0, 5.0);
	CameraTarget = glm::vec3(0.0, 0.0, 0.0);
	UpVector = glm::vec3(0.0, 1.0, 0.0);
}

ViewControl::~ViewControl()
{

}

void ViewControl::SetMVP()
{
	// FoV(45.0), WindowRatio(16 : 9)
	ProjectionMatrix = glm::perspective(glm::radians(FoV), WindowRatio, 0.1f, 100.0f);
	ViewMatrix = glm::lookAt(
		CameraPosition,
		CameraTarget,
		UpVector
	);
	ModelMatrix = glm::mat4(1.0f);
	MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
}

glm::mat4 ViewControl::GetMVP()
{
	MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
	return MVP;
}