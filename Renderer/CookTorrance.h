#pragma once
#include "Material.h"

class CookTorrance : public Material
{
public:
	CookTorrance();
	~CookTorrance();

	// Virtual methods inherited from Material
	virtual void Start();
	virtual void Init();
	virtual void Update();
	virtual void SetUniform();

	void SetRough(float r, float g, float b);
	void SetFresnel(float r, float g, float b);
	glm::vec3 GetRough();
	glm::vec3 GetFresnel();

	void SetDiff(float r, float g, float b);
	void SetSpec(float r, float g, float b);
	glm::vec3 GetDiff();
	glm::vec3 GetSpec();

private:
	glm::vec3 diff;
	glm::vec3 spec;
	glm::vec3 rough;
	glm::vec3 fresnel;

	// uniform variable handles
	GLuint FresnelID;
	GLuint RoughnessID;
	GLuint DiffuseID;
	GLuint SpecularID;
};
