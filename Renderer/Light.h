#pragma once
#include <string>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class Light
{
public:
	Light();
	~Light();

	int GetIDX();
	
	void Start(int _idx, int _type);

	// Set
	void SetPosition(float x, float y, float z);
	void SetColor(float r, float g, float b);
	void SetIntensity(float i);

	// Get
	float GetIntensity();
	glm::vec3 GetPosition();
	glm::vec3 GetColor();

	int isEnabled;
	int type;

private:
	glm::vec3 pos;
	glm::vec3 color;
	float intensity;
	
	int idx;
};