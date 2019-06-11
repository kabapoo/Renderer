#version 430

in vec3 vNormal;
in vec2 uv;
in vec3 vertex_pos_ws;
in vec3 color;

out vec4 FragColor;

struct Light
{
	int isEnabled;
	int type;
	vec3 pos;
	vec3 color;
	float intensity;
};

// Material types
uniform int MaterialType;

// Light
const int MaxLights = 1;
uniform Light Lights[MaxLights];

uniform vec3 view_pos;
uniform float gamma;

uniform vec3 rough;
uniform vec3 fresnel;
uniform vec3 spec_factor;
uniform vec3 diff_factor;

vec3 DirectionalLight(Light light, vec3 NormalVector, vec3 ViewDirection)
{
	// Directional light has no attenuation
	vec3 LightDirection = normalize(light.pos);
	vec3 HalfVector = normalize(LightDirection + ViewDirection);
	vec3 LightPower = light.color * light.intensity;

	float NdotL = max(dot(NormalVector, LightDirection), 0.00001);
	float NdotH = max(dot(NormalVector, HalfVector), 0.00001);
	float NdotV = max(dot(NormalVector, ViewDirection), 0.00001);
	float VdotH = max(dot(ViewDirection, HalfVector), 0.00001);

	// ambient light
	vec3 ambient = vec3(0.01, 0.01, 0.01);
	// diffuse light
	vec3 diffuse = diff_factor * max(dot(NormalVector, LightDirection), 0.0);
	// specular light(cook torrance)
	// Fresnel(Schlick approximation)
	vec3 F = fresnel + ((1.0 - fresnel) * pow(1.0 - VdotH, 5.0));

	// Geometric attenuation
	float NH2 = 2.0 * NdotH;
	float g1 = (NH2 * NdotV) / VdotH;
	float g2 = (NH2 * NdotL) / VdotH;
	float G = min(1.0, min(g1, g2));

	// microfacet distribution function
	vec3 r1 = 1.0 / (3.14 * rough * rough * pow(NdotH, 4.0));
	vec3 r2 = (NdotH * NdotH - 1.0) / (rough * rough * NdotH * NdotH);
	vec3 D = r1 * exp(r2);
	
	vec3 specular = (F * G * D) / (3.14 * NdotV) * spec_factor;
	vec3 result = ambient + (diffuse + specular) * LightPower;

	return result;
}

vec3 PointLight(Light light, vec3 NormalVector, vec3 ViewDirection)
{
	vec3 LightDirection = normalize(light.pos - vertex_pos_ws);
	vec3 HalfVector = normalize(LightDirection + ViewDirection);
	vec3 LightPower = light.color * light.intensity;

	float DistanceToLight = length(light.pos - vertex_pos_ws);
	float Attenuation = 1.0 / DistanceToLight;
	
	float NdotL = max(dot(NormalVector, LightDirection), 0.00001);
	float NdotH = max(dot(NormalVector, HalfVector), 0.00001);
	float NdotV = max(dot(NormalVector, ViewDirection), 0.00001);
	float VdotH = max(dot(ViewDirection, HalfVector), 0.00001);

	// ambient light
	vec3 ambient = vec3(0.01, 0.01, 0.01);
	// diffuse light
	vec3 diffuse = diff_factor * max(dot(NormalVector, LightDirection), 0.0);
	// specular light(cook torrance)
	// Fresnel(Schlick approximation)
	vec3 F = fresnel + ((1.0 - fresnel) * pow(1.0 - VdotH, 5.0));

	// Geometric attenuation
	float NH2 = 2.0 * NdotH;
	float g1 = (NH2 * NdotV) / VdotH;
	float g2 = (NH2 * NdotL) / VdotH;
	float G = min(1.0, min(g1, g2));

	// microfacet distribution function
	vec3 r1 = 1.0 / (3.14 * rough * rough * pow(NdotH, 4.0));
	vec3 r2 = (NdotH * NdotH - 1.0) / (rough * rough * NdotH * NdotH);
	vec3 D = r1 * exp(r2);
	
	vec3 specular = (F * G * D) / (3.14 * NdotV) * spec_factor;
	vec3 result = ambient + (diffuse + specular) * LightPower * Attenuation;
	return result;
}

void main()
{
	vec3 result = vec3(0.0);
	vec3 NormalVector = normalize(vNormal);
	vec3 ViewDirection = normalize(view_pos - vertex_pos_ws);

	for (int light = 0; light < MaxLights; ++light)
	{
		float DistanceToLight = length(Lights[light].pos - vertex_pos_ws);
		float Attenuation = 1.0 / DistanceToLight;
		if (MaterialType == 1)  // Cook Torrance Material
		{
			if (Lights[light].isEnabled != 1)
				continue;
			if (Lights[light].type == 0)	// point light source
				result += PointLight(Lights[light], NormalVector, ViewDirection);
			if (Lights[light].type == 1)	// directional light source
				result += DirectionalLight(Lights[light], NormalVector, ViewDirection);
		}
		if (MaterialType == 2)   // MERL Data-driven Material
		{
			vec3 LightPower = Lights[light].color * Lights[light].intensity * Attenuation;
			result += color * LightPower;
		}
	}
	result = pow(result, vec3(1.0/gamma));
	FragColor = vec4(result, 1.0);	
}

