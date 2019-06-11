#include "Light.h"

Light::Light()
{
	isEnabled = 0;
}

Light::~Light()
{

}

int Light::GetIDX()
{
	return idx;
}

void Light::Start(int _idx, int _type)
{
	idx = _idx;
	type = _type;
	isEnabled = 1;		// enable
}

void Light::SetPosition(float x, float y, float z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
}

void Light::SetColor(float r, float g, float b)
{
	color.r = r;
	color.g = g;
	color.b = b;
}

void Light::SetIntensity(float i)
{
	intensity = i;
}

float Light::GetIntensity()
{
	return intensity;
}

glm::vec3 Light::GetPosition()
{
	return pos;
}

glm::vec3 Light::GetColor()
{
	return color;
}