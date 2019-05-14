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

void Quad::setOriginTopLeft()
{
	originOffset = glm::vec2(scale.x / 2, scale.y / 2);
}

void Quad::setOriginTopRight()
{
	originOffset = glm::vec2(-scale.x / 2, scale.y / 2);
}

bool Quad::checkCollision(glm::vec2 point)
{
	if (pos.x <= point.x && point.x <= pos.x + scale.x
		&& pos.y <= point.y && point.y <= pos.y + scale.y)
	{
		return true;
	}
	return false;
}

bool Quad::checkCollision(const Quad& other) {
	bool collisionX = originOffset.x + scale.x >= other.originOffset.x &&
		other.originOffset.x + other.scale.x >= originOffset.x;

	bool collisionY = originOffset.y + scale.y >= other.originOffset.y &&
		other.originOffset.y + other.scale.y >= originOffset.y;

	return collisionX && collisionY;
}

Quad::~Quad()
{
}

glm::vec2 GamePiece::refPoint = glm::vec2(0);
int GamePiece::gridSize = 0;

GamePiece::GamePiece(int t, glm::vec2 coordinate) {
	type = t;
	boardCoord = coordinate;
}

GamePiece::~GamePiece() {}

void GamePiece::setRefPoint(const glm::vec2& corner) {
	refPoint = corner;
}

void GamePiece::setGridSize(int size) {
	gridSize = size;
}

void GamePiece::setType(int t) {
	type = t;
}

void GamePiece::setBoardPosition() {
	pos = glm::vec2(refPoint.x + (boardCoord.x * gridSize), 
		refPoint.y + (boardCoord.y * gridSize));
}

// Both of these are horrible looking but they work.
bool GamePiece::isMovableIn(char direction, glm::vec2 coordData[], int filledData[]) 
{
	using namespace std;
	using namespace glm;

	int targetIndex1 = -1;
	int targetIndex2 = -1;
	vec2 targetCoord1 = vec2(-1, -1);
	vec2 targetCoord2 = vec2(-1, -1);
	bool isMovable = false;

	// Finds out what coordinates to check depending on the piece
	if (type == 1) {
		if (direction == 'U') {
			targetCoord1 = boardCoord + vec2(0, -1);
		}
		else if (direction == 'D') {
			targetCoord1 = boardCoord + vec2(0, 1);
		}
		else if (direction == 'R') {
			targetCoord1 = boardCoord + vec2(1, 0);
		} 
		else if (direction == 'L') {
			targetCoord1 = boardCoord + vec2(-1, 0);
		}
	}
	else if (type == 2) {
		if (direction == 'U') {
			targetCoord1 = boardCoord + vec2(0, -1);
			targetCoord2 = boardCoord + vec2(1, -1);
		}
		else if (direction == 'D') {
			targetCoord1 = boardCoord + vec2(0, 2);
			targetCoord2 = boardCoord + vec2(1, 2);
		}
		else if (direction == 'R') {
			targetCoord1 = boardCoord + vec2(2, 0);
			targetCoord2 = boardCoord + vec2(2, 1);
		}
		else if (direction == 'L') {
			targetCoord1 = boardCoord + vec2(-1, 0);
			targetCoord2 = boardCoord + vec2(-1, 1);
		}
	}
	else if (type == 3) {
		if (direction == 'U') {
			targetCoord1 = boardCoord + vec2(0, -1);
		}
		else if (direction == 'D') {
			targetCoord1 = boardCoord + vec2(0, 2);
		}
		else if (direction == 'R') {
			targetCoord1 = boardCoord + vec2(1, 0);
			targetCoord2 = boardCoord + vec2(1, 1);
		}
		else if (direction == 'L') {
			targetCoord1 = boardCoord + vec2(-1, 0);
			targetCoord2 = boardCoord + vec2(-1, 1);
		}
	}
	else if (type == 4) {
		if (direction == 'U') {
			targetCoord1 = boardCoord + vec2(0, -1);
			targetCoord2 = boardCoord + vec2(1, -1);
		}
		else if (direction == 'D') {
			targetCoord1 = boardCoord + vec2(0, 1);
			targetCoord2 = boardCoord + vec2(1, 1);
		}
		else if (direction == 'R') {
			targetCoord1 = boardCoord + vec2(2, 0);
		}
		else if (direction == 'L') {
			targetCoord1 = boardCoord + vec2(-1, 0);
		}
	}

	// Finds the corresponding index of the corrdinate in the filled array which
	// has info on if the coordinate is filled or not
	for (int i = 0; i < 20; i++) {
		if (coordData[i] == targetCoord1) {
			targetIndex1 = i;
		}
		else if (coordData[i] == targetCoord2) {
			targetIndex2 = i;
		}
	}

	// Checks if the coordinate is filled and determines if piece is movable
	if (targetIndex2 != -1) {
		if (filledData[targetIndex1] == 0 &&
			filledData[targetIndex2] == 0) {
			isMovable = true;
		}
	}
	else {
		if (filledData[targetIndex1] == 0) {
			isMovable = true;
		}
	}
	return isMovable;
}

void GamePiece::moveIn(char direction, glm::vec2 coordData[], int filledData[]) {
	using namespace glm;
	using namespace std;

	vec2 empty1 = vec2(-1, -1);
	vec2 empty2 = vec2(-1, -1);
	vec2 fill1 = vec2(-1, -1);
	vec2 fill2 = vec2(-1, -1);

	// Finds out which coordinates need to be filled or emptied in the move
	if (type == 1) {
		if (direction == 'U') {
			empty1 = boardCoord;
			fill1 = boardCoord + vec2(0, -1);
		}
		else if (direction == 'D') {
			empty1 = boardCoord;
			fill1 = boardCoord + vec2(0, 1);
		}
		else if (direction == 'R') {
			empty1 = boardCoord;
			fill1 = boardCoord + vec2(1, 0);
		}
		else if (direction == 'L') {
			empty1 = boardCoord;
			fill1 = boardCoord + vec2(-1, 0);
		}
	}
	else if (type == 2) {
		if (direction == 'U') {
			empty1 = boardCoord + vec2(0, 1);
			empty2 = boardCoord + vec2(1, 1);
			fill1 = boardCoord + vec2(0, -1);
			fill2 = boardCoord + vec2(1, -1);
		}
		else if (direction == 'D') {
			empty1 = boardCoord + vec2(0, 0);
			empty2 = boardCoord + vec2(1, 0);
			fill1 = boardCoord + vec2(0, 2);
			fill2 = boardCoord + vec2(1, 2);
		}
		else if (direction == 'R') {
			empty1 = boardCoord + vec2(0, 0);
			empty2 = boardCoord + vec2(0, 1);
			fill1 = boardCoord + vec2(2, 0);
			fill2 = boardCoord + vec2(2, 1);
		}
		else if (direction == 'L') {
			empty1 = boardCoord + vec2(1, 0);
			empty2 = boardCoord + vec2(1, 1);
			fill1 = boardCoord + vec2(-1, 0);
			fill2 = boardCoord + vec2(-1, 1);
		}
	}
	else if (type == 3) {
		if (direction == 'U') {
			empty1 = boardCoord + vec2(0, 1);
			fill1 = boardCoord + vec2(0, -1);
		}
		else if (direction == 'D') {
			empty1 = boardCoord + vec2(0, 0);
			fill1 = boardCoord + vec2(0, 2);
		}
		else if (direction == 'R') {
			empty1 = boardCoord + vec2(0, 0);
			empty2 = boardCoord + vec2(0, 1);
			fill1 = boardCoord + vec2(1, 0);
			fill2 = boardCoord + vec2(1, 1);
		}
		else if (direction == 'L') {
			empty1 = boardCoord + vec2(0, 0);
			empty2 = boardCoord + vec2(0, 1);
			fill1 = boardCoord + vec2(-1, 0);
			fill2 = boardCoord + vec2(-1, 1);
		}
	}
	else if (type == 4) {
		if (direction == 'U') {
			empty1 = boardCoord + vec2(0, 0);
			empty2 = boardCoord + vec2(1, 0);
			fill1 = boardCoord + vec2(0, -1);
			fill2 = boardCoord + vec2(1, -1);
		}
		else if (direction == 'D') {
			empty1 = boardCoord + vec2(0, 0);
			empty2 = boardCoord + vec2(1, 0);
			fill1 = boardCoord + vec2(0, 1);
			fill2 = boardCoord + vec2(1, 1);
		}
		else if (direction == 'R') {
			empty1 = boardCoord + vec2(0, 0);
			fill1 = boardCoord + vec2(2, 0);
		}
		else if (direction == 'L') {
			empty1 = boardCoord + vec2(1, 0);
			fill1 = boardCoord + vec2(-1, 0);
		}
	}

	// Updates the filled array accordingly
	for (int i = 0; i < 20; i++) {
		if (coordData[i] == empty1) {
			filledData[i] = 0;
		}
		else if (coordData[i] == empty2) {
			filledData[i] = 0;
		}
		else if (coordData[i] == fill1) {
			filledData[i] = 1;
		}
		else if (coordData[i] == fill2) {
			filledData[i] = 1;
		}
	}

	// Moving the piece to its new coordinate position in the grid.
	if (direction == 'U') {
		boardCoord += vec2(0, -1);
	}
	else if (direction == 'D') {
		boardCoord += vec2(0, 1);
	}
	else if (direction == 'R') {
		boardCoord += vec2(1, 0);
	}
	else if (direction == 'L') {
		boardCoord += vec2(-1, 0);
	}
}