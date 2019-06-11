#version 430

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in vec3 vertex_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vNormal;
out vec2 uv;
out vec3 vertex_pos_ws;
out vec3 color;

void main()
{
	vNormal = vertex_normal;
	vertex_pos_ws = (model * vec4(vertex_position, 1.0)).xyz;
	
	uv = vertex_uv;
	color = vertex_color;

	gl_Position = projection * view * model * vec4(vertex_position, 1.0);
}