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

	//static functions
	static void initBuffers(GLuint shader);
	static void destroyBuffers();

	//functions
	Quad(
		glm::vec2 position = glm::vec2(0.0f),
		float rotation = 0,
		glm::vec2 scale = glm::vec2(1.0f),
		glm::vec3 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	);
	void draw();
	void setOriginTopLeft();
	void setOriginTopRight();
	bool checkCollision(glm::vec2 point);

	bool checkCollision(const Quad& other);
	~Quad();
};

class GamePiece : public Quad
{
private:
	static glm::vec2 refPoint;
	static int gridSize;
	int type;
public:
	glm::vec2 boardCoord;

	GamePiece(
		int t = 0,
		glm::vec2 coordinate = glm::vec2(0)
	);

	~GamePiece();

	static void setRefPoint(const glm::vec2&);
	static void setGridSize(int);
	void setType(int);

	void setBoardPosition(void);

	bool isMovableIn(char direction, glm::vec2[], int[]);
	void moveIn(char direction, glm::vec2[], int[]);
};

// Type (using an int to represent each type of piece for now)
// Small Square - 1
// Big Square - 2
// Tall Rectangle - 3
// Long Rectangle - 4

