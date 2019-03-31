#include "Square.h"
#include <iostream>

const float Square::halfSize = 0.5f;
const float Square::baseSquare[vertSize * 4] =
{
	 halfSize,  halfSize, //top right 0
	 halfSize, -halfSize, //botom right 1
	-halfSize,  halfSize, //top left 2
	-halfSize, -halfSize  //botom left 3
};
const GLuint Square::squareIndecies[6] =
{
	0, 1, 3,
	0, 2, 3
};
GLuint Square::vertexBufferID = 0;
GLuint Square::indexBufferID = 0;
GLuint Square::shaderID = 0;
unsigned int Square::totatlSquares = 0;

void Square::initBuffers(GLuint shader)
{
	//vertex
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), &baseSquare, GL_STATIC_DRAW);

	//index
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), squareIndecies, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, vertSize, GL_FLOAT, GL_FALSE, sizeof(float) * vertSize, 0);
	shaderID = shader;
}

void Square::destroyBuffers()
{
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteBuffers(1, &indexBufferID);
	shaderID = NULL;
}

Square::Square(glm::vec2 position, float rotation, glm::vec2 scale, glm::vec3 color)
	: pos(position), scale(scale), rotation(rotation), color(color)
{
	totatlSquares++;
	id = totatlSquares;
}

void Square::draw()
{
	using namespace glm;
	modelMatrix = mat4(1.0f);
	modelMatrix = translate(modelMatrix, vec3(pos.x, pos.y, 0));
	modelMatrix = rotate(modelMatrix, radians(rotation), vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, vec3(scale.x, scale.y, 0));

	glUniformMatrix4fv(glGetUniformLocation(shaderID, "modelMatrix"), 1, false, &modelMatrix[0][0]);
	glUniform4fv(glGetUniformLocation(shaderID, "uColor"), 1, &color[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glVertexAttribPointer(0, vertSize, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

bool Square::checkCollision(glm::vec2 point)
{
	if (pos.x - scale.x / 2 <= point.x && point.x <= pos.x + scale.x / 2
		&& pos.y - scale.y / 2 <= point.y && point.y <= pos.y + scale.y / 2
		)
	{
		return true;
	}
	return false;
}

Square::~Square()
{
}

/*
	 0.5,  0.5, //top right 0
	 0.5, -0.5, //botom right 1
	-0.5,  0.5, //top left 2
	-0.5, -0.5  //botom left 3
*/