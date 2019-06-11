#version 430

in vec3 normal_cs;
in vec2 uv;
in vec3 light_pos_cs;
in vec3 vertex_pos_cs;

out vec3 FragColor;

uniform vec3 light_color;

void main()
{
	vec3 norm = normalize(normal_cs);	
	vec3 light_dir = normalize(light_pos_cs - vertex_pos_cs);
	vec3 ambient = vec3(0.2, 0.2, 0.2);

	float dif = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = dif * light_color;
	
	vec3 result = diffuse;
	FragColor = result;	
}