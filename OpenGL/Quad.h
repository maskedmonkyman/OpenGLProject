#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "GLM/gtx/transform.hpp"

const static int vertSize = 2;

class Quad
{
private:
	const static float halfSize;
	const static float baseSquare[vertSize * 4];
	const static GLuint squareIndecies[6];
	static GLuint vertexBufferID;
	static GLuint indexBufferID;
	static GLuint shaderID;
	static unsigned int totatlSquares;

public:
	unsigned int id;
	glm::vec2 pos;
	glm::vec2 originOffset = glm::vec2(0);
	glm::vec2 scale;
	float rotation;
	glm::vec3 color;
	glm::mat4 modelMatrix;
	bool movable = true;


	static void initBuffers(GLuint shader);
	static void destroyBuffers();

	Quad(
		glm::vec2 position = glm::vec2(0.0f),
		float rotation = 0,
		glm::vec2 scale = glm::vec2(1.0f),
		glm::vec3 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	);
	void draw();
	bool checkCollision(Quad other);
	bool checkCollision(glm::vec2 point);
	bool translate(glm::vec2 offSet);
	~Quad();
};

