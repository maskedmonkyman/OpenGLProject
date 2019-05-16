#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "GLM/gtx/transform.hpp"

#include "Quad.h"

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

	// static functions
	static void setRefPoint(const glm::vec2&);
	static void setGridSize(int);
	
	// setter for private member
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