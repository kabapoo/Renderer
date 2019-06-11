#version 430

in vec3 normal;
in vec2 uv;
in vec3 vertex_pos_ws;

out vec3 FragColor;

uniform vec3 light_pos;
uniform vec3 light_color;
uniform float light_intensity;
uniform vec3 view_pos;

uniform vec3 rough;
uniform vec3 fresnel;
uniform vec3 spec_factor;
uniform vec3 diff_factor;

void main()
{
	// normal, light, view, half vector
	vec3 norm = normalize(normal);	
	vec3 light_dir = normalize(light_pos - vertex_pos_ws);
	vec3 view_dir = normalize(view_pos - vertex_pos_ws);
	vec3 half_vec = normalize(light_dir + view_dir);
	vec3 light_power = light_color * light_intensity;

	// Distance to the light
	float dist = length(light_pos - vertex_pos_ws);
	float NdotL = max(dot(norm, light_dir), 0.00001);
	float NdotH = max(dot(norm, half_vec), 0.00001);
	float NdotV = max(dot(norm, view_dir), 0.00001);
	float VdotH = max(dot(view_dir, half_vec), 0.00001);

	// ambient light
	vec3 ambient = vec3(0.01, 0.01, 0.01);

	// diffuse light
	float dif = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = diff_factor * dif * light_power;

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
	
	vec3 spec = (F * G * D) / (3.14 * NdotV) * spec_factor;
	vec3 specular = spec * light_power;
	
	vec3 result = ambient + diffuse + specular;
	FragColor = result;	
	//FragColor = vec3(1.0, 1.0, 1.0);
}

