#version 430

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv;
layout(location = 2) in vec3 vertex_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 light_pos;

out vec3 normal_cs;
out vec2 uv;
out vec3 vertex_pos_cs;
out vec3 light_pos_cs;

void main()
{
	normal_cs = vertex_normal;
	light_pos_cs = light_pos;
	vertex_pos_cs = (model * vec4(vertex_position, 1.0)).xyz;
	
	uv = vertex_uv;

	gl_Position = projection * view * model * vec4(vertex_position, 1.0);
}