#include "CookTorrance.h"

CookTorrance::CookTorrance()
{
	isUpdatable = 0;
}

CookTorrance::~CookTorrance()
{

}

void CookTorrance::Init()
{
	this->SetDiff(0.05f, 0.05f, 0.0007f);
	this->SetSpec(0.017f, 0.018f, 0.019f);
	this->SetFresnel(0.168f, 0.174f, 0.162f);
	this->SetRough(0.03f, 0.03f, 0.033f);
}

void CookTorrance::Start()
{
	std::cout << "CookTorrance start\n";
	SetUniform();
	isUpdatable = 1;
}

void CookTorrance::SetDiff(float r, float g, float b)
{
	diff = glm::vec3(r, g, b);
}

void CookTorrance::SetSpec(float r, float g, float b)
{
	spec = glm::vec3(r, g, b);
}

glm::vec3 CookTorrance::GetDiff()
{
	return diff;
}

glm::vec3 CookTorrance::GetSpec()
{
	return spec;
}

void CookTorrance::SetRough(float r, float g, float b)
{
	rough = glm::vec3(r, g, b);
}

void CookTorrance::SetFresnel(float r, float g, float b)
{
	fresnel = glm::vec3(r, g, b);
}

glm::vec3 CookTorrance::GetRough()
{
	return rough;
}

glm::vec3 CookTorrance::GetFresnel()
{
	return fresnel;
}

void CookTorrance::SetUniform()
{
	//std::cout << "CookTorrance::SetUniform() " << ShaderID << "\n";
	DiffuseID = glGetUniformLocation(ShaderID, "diff_factor");
	SpecularID = glGetUniformLocation(ShaderID, "spec_factor");
	FresnelID = glGetUniformLocation(ShaderID, "fresnel");
	RoughnessID = glGetUniformLocation(ShaderID, "rough");
	MaterialTypeID = glGetUniformLocation(ShaderID, "MaterialType");
}

void CookTorrance::Update()
{
	if (isUpdatable == 1)
	{
		glUniform3fv(DiffuseID, 1, value_ptr(GetDiff()));
		glUniform3fv(SpecularID, 1, value_ptr(GetSpec()));
		glUniform3fv(FresnelID, 1, value_ptr(GetFresnel()));
		glUniform3fv(RoughnessID, 1, value_ptr(GetRough()));
		glUniform1i(MaterialTypeID, type);
	}
}