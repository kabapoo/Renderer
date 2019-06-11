#include "Camera.h"

Camera::Camera()
{
	
}

Camera::~Camera()
{

}

void Camera::Start(GLuint id)
{
	position = glm::vec3(0.0f, 0.0f, 3.0f);
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	direction = glm::normalize(position - target);
	right = glm::normalize(glm::cross(up, direction));
	winRatio = 16.0f / 9.0f;
	fov = 90.0f;

	ShaderID = id;
	SetUniform();
}

void Camera::SetPosition(float x, float y, float z)
{
	position = glm::vec3(x, y, z);
}

void Camera::SetTarget(float x, float y, float z)
{
	target = glm::vec3(x, y, z);
}

void Camera::SetDirection()
{
	direction = glm::normalize(position - target);
}

void Camera::SetUp(float x, float y, float z)
{
	up = glm::vec3(x, y, z);
}

void Camera::SetRight()
{
	right = glm::normalize(glm::cross(up, direction));
}

glm::vec3 Camera::GetPosition()
{
	return position;
}

glm::mat4 Camera::GetViewMatrix()
{
	glm::mat4 view;
	view = glm::lookAt(
		position,
		target,
		up
	);
	return view;
}

glm::mat4 Camera::GetProjectionMatrix()
{
	glm::mat4 proj = glm::perspective(glm::radians(fov), winRatio, 0.1f, 100.0f);
	return proj;
}

void Camera::SetUniform()
{
	// Get a handle for the "MVP" uniform
	ProjectionMatrixID = glGetUniformLocation(ShaderID, "projection");
	ViewMatrixID = glGetUniformLocation(ShaderID, "view");
	ViewID = glGetUniformLocation(ShaderID, "view_pos");
}

void Camera::Update()
{
	// Send our transformation to the currently bound shader, in the "MVP" uniform
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, glm::value_ptr(GetViewMatrix()));
	glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, glm::value_ptr(GetProjectionMatrix()));
	glUniform3fv(ViewID, 1, glm::value_ptr(position));
}