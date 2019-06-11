#pragma once
#include "Light.h"
#include <vector>
#define MAX_LIGHT 1

class ILight
{
public:
	ILight(GLuint id);
	void AddLight(int idx, int type, glm::vec3 pos, glm::vec3 color, float intensity);
	glm::vec3 GetPosition(int idx);
	glm::vec3 GetColor(int idx);
	float GetIntensity(int idx);
	void SetUniforms();
	void Update();

private:
	Light lights[MAX_LIGHT];
	int cnt;
	float gamma;

	GLuint ShaderID;

	// uniform ID
	GLuint TypeID[MAX_LIGHT];
	GLuint PosID[MAX_LIGHT];
	GLuint ColorID[MAX_LIGHT];
	GLuint IntensityID[MAX_LIGHT];
	GLuint isEnabledID[MAX_LIGHT];
	GLuint gammaID;
};