#include "glRenderer.h"

glRenderer::glRenderer()
{

}

glRenderer::~glRenderer()
{

}

void glRenderer::Start()
{
	modules.Start();
	CreateVBO();
	CreateVAO();
}

void glRenderer::Options()
{
	// Gamma Correction
	//glEnable(GL_FRAMEBUFFER_SRGB);

	// Back-face Culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// Z-buffer
	glEnable(GL_DEPTH_TEST);
}

void glRenderer::Draw()
{
	glBindVertexArray(vaoID);
	
	modules.Update();
	
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)modules.simpleSphere.vertices.size());
}

void glRenderer::CreateVBO()
{
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, modules.simpleSphere.vertices.size() * sizeof(glm::vec3), &(modules.simpleSphere.vertices[0]), GL_STATIC_DRAW);

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, modules.simpleSphere.uvs.size() * sizeof(glm::vec2), &(modules.simpleSphere.uvs[0]), GL_STATIC_DRAW);

	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, modules.simpleSphere.normals.size() * sizeof(glm::vec3), &(modules.simpleSphere.normals[0]), GL_STATIC_DRAW);

	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, modules.simpleSphere.colors.size() * sizeof(glm::vec3), &(modules.simpleSphere.colors[0]), GL_STATIC_DRAW);
}

void glRenderer::CreateVAO()
{
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
}