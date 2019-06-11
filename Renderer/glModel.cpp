#include "glModel.h"

glModel::glModel()
{
	isLoaded = 0;
}

glModel::~glModel()
{

}

int glModel::LoadOBJ(const char* path)
{
	vertices.clear();
	uvs.clear();
	normals.clear();
	colors.clear();

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	FILE *fp;
	fopen_s(&fp, path, "r");
	if (fp == NULL) {
		printf("Impossible to open the file !\n");
		return 0;
	}

	while (1)
	{
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(fp, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

				   // else : parse lineHeader
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(fp, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(fp, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(fp, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return 0;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		vertices.push_back(vertex);
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		uvs.push_back(uv);
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		normals.push_back(normal);
	}
	printf("%s loaded.\n", path);
	isLoaded = 1;
	return isLoaded;
}

void glModel::Start(GLuint id)
{
	ShaderID = id;
	transformMatrix = glm::mat4(1.0f);
	SetUniform();
}

void glModel::Update()
{
	// Send our transformation to the currently bound shader, in the "MVP" uniform
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, glm::value_ptr(transformMatrix));
}

void glModel::SetUniform()
{
	ModelMatrixID = glGetUniformLocation(ShaderID, "model");
}

void glModel::SetTransform()
{
	transformMatrix = glm::mat4(1.0f);
}

glm::mat4 glModel::GetTransform()
{
	return transformMatrix;
}