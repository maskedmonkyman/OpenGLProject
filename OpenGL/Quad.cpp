#include "Quad.h"
#include <iostream>

const float Quad::halfSize = 0.5f;
const float Quad::baseSquare[vertSize * 4] =
{
	 halfSize,  halfSize, //top right 0
	 halfSize, -halfSize, //botom right 1
	-halfSize,  halfSize, //top left 2
	-halfSize, -halfSize  //botom left 3
};
const GLuint Quad::squareIndecies[6] =
{
	0, 1, 3,
	0, 2, 3
};
GLuint Quad::vertexBufferID = 0;
GLuint Quad::indexBufferID = 0;
GLuint Quad::shaderID = 0;
unsigned int Quad::totatlSquares = 0;

void Quad::initBuffers(GLuint shader)
{
	//vertex
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), &baseSquare, GL_STATIC_DRAW);

	//index
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), squareIndecies, GL_STATIC_DRAW);

	//atrib pointer
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, vertSize, GL_FLOAT, GL_FALSE, sizeof(float) * vertSize, 0);
	shaderID = shader;
}

void Quad::destroyBuffers()
{
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteBuffers(1, &indexBufferID);
	shaderID = NULL;
}

//constructor
Quad::Quad(glm::vec2 position, float rotation, glm::vec2 scale, glm::vec3 color)
	: pos(position), scale(scale), rotation(rotation), color(color)
{
	totatlSquares++;
	id = totatlSquares;
}

void Quad::draw()
{
	using namespace glm;
	//set up model matrix
	modelMatrix = mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, vec3(originOffset.x + pos.x, originOffset.y + pos.y, 0));
	modelMatrix = rotate(modelMatrix, radians(rotation), vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, vec3(scale.x, scale.y, 0));

	//GL calls
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "modelMatrix"), 1, false, &modelMatrix[0][0]);
	glUniform4fv(glGetUniformLocation(shaderID, "uColor"), 1, &color[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glVertexAttribPointer(0, vertSize, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

bool Quad::checkCollision(Quad other)
{
	return false;
}

bool Quad::checkCollision(glm::vec2 point)
{
	if (pos.x - scale.x / 2 <= point.x && point.x <= pos.x + scale.x / 2
		&& pos.y - scale.y / 2 <= point.y && point.y <= pos.y + scale.y / 2
		)
	{
		return true;
	}
	return false;
}

//Todo add colision detection to translate
bool Quad::translate(glm::vec2 offSet)
{
	pos += offSet;
	return true;
}

Quad::~Quad()
{
}