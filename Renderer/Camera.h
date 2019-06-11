#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	void Start(GLuint id);

	void SetPosition(float x, float y, float z);
	void SetTarget(float x, float y, float z);
	void SetDirection();
	void SetUp(float x, float y, float z);
	void SetRight();
	void SetUniform();

	glm::vec3 GetPosition();

	void Update();

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

private:
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 direction;

	float winRatio;
	float fov;

	GLuint ShaderID;

	// Uniform IDs
	GLuint ProjectionMatrixID;
	GLuint ViewMatrixID;
	GLuint ViewID;
};