#version 430

in vec3 normal;
in vec2 uv;
in vec3 vertex_pos_ws;

out vec3 FragColor;

uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 view_pos;

void main()
{
	// normal, light, view, half vector
	vec3 norm = normalize(normal);	
	vec3 light_dir = normalize(light_pos - vertex_pos_ws);
	vec3 view_dir = normalize(view_pos - vertex_pos_ws);
	vec3 half_vec = normalize(light_dir + view_dir);

	// ambient light
	vec3 ambient = vec3(0.1, 0.1, 0.1);

	// diffuse light
	float dif = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = dif * light_color;

	// specular light
	float spec = pow(max(dot(norm, half_vec), 0.0), 64.0);
	vec3 specular = spec * light_color;
	
	vec3 result = ambient + diffuse + specular;
	FragColor = result;	
}