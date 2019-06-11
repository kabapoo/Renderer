#include "ILight.h"

ILight::ILight(GLuint id)
{
	ShaderID = id;
	gamma = 1.5;
}

void ILight::AddLight(int idx, int type, glm::vec3 pos, glm::vec3 color, float intensity)
{
	lights[idx].Start(idx, type);
	lights[idx].SetIntensity(intensity);
	lights[idx].SetPosition(pos.x, pos.y, pos.z);
	lights[idx].SetColor(color.r, color.g, color.b);
}

float ILight::GetIntensity(int idx)
{
	return lights[idx].GetIntensity();
}

glm::vec3 ILight::GetPosition(int idx)
{
	return lights[idx].GetPosition();
}

glm::vec3 ILight::GetColor(int idx)
{
	return lights[idx].GetColor();
}

void ILight::SetUniforms()
{
	std::string str;
	for (int i = 0; i < MAX_LIGHT; i++)
	{
		str = "Lights[" + std::to_string(i) + "].type";
		TypeID[i] = glGetUniformLocation(ShaderID, str.c_str());
		str = "Lights[" + std::to_string(i) + "].pos";
		PosID[i] = glGetUniformLocation(ShaderID, str.c_str());
		str = "Lights[" + std::to_string(i) + "].color";
		ColorID[i] = glGetUniformLocation(ShaderID, str.c_str());
		str = "Lights[" + std::to_string(i) + "].intensity";
		IntensityID[i] = glGetUniformLocation(ShaderID, str.c_str());
		str = "Lights[" + std::to_string(i) + "].isEnabled";
		isEnabledID[i] = glGetUniformLocation(ShaderID, str.c_str());
	}
	gammaID = glGetUniformLocation(ShaderID, "gamma");
}

void ILight::Update()
{
	for (int i = 0; i < MAX_LIGHT; i++)
	{
		glUniform1i(TypeID[i], lights[i].type);
		glUniform3fv(PosID[i], 1, value_ptr(lights[i].GetPosition()));
		glUniform3fv(ColorID[i], 1, value_ptr(lights[i].GetColor()));
		glUniform1f(IntensityID[i], lights[i].GetIntensity());
		glUniform1i(isEnabledID[i], lights[i].isEnabled);
	}
	glUniform1f(gammaID, gamma);
}