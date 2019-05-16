#pragma once
#include <vector>

#include "glm/glm.hpp"
#include "Quad.h"
#include "GamePiece.h"
#include "GLFW/glfw3.h"

#include <iostream>

//makes a game board, size is the internal rectangle size, pos is top left corner
//boarderSize is how thick the boarders will be (note this will be added to size)
//gap size is how big the bottom gap is
std::vector<Quad> makeBoard(glm::vec2 size, glm::vec2 pos, int boarderSize, int gapSize)
{
	using namespace glm;
	std::vector<Quad> board(7);
	vec3 color = vec3(255.0f/255, 144.0f/255, 0.0f/255); //orange

	for (int i = 0; i < 5; i++)
		board[i].color = color;

	//top
	board[0].scale = vec2(size.x, boarderSize);
	board[0].setOriginTopLeft();
	board[0].pos = vec2(pos.x, pos.y - boarderSize);

	//middle left
	board[1].scale = vec2(boarderSize, size.y + boarderSize * 2);
	board[1].setOriginTopLeft();
	board[1].pos = vec2(pos.x - boarderSize, pos.y - boarderSize);

	//middle right
	board[2].scale = vec2(boarderSize, size.y + boarderSize * 2);
	board[2].setOriginTopLeft();
	board[2].pos = vec2(pos.x + size.x, pos.y - boarderSize);

	//bot left
	board[3].scale = vec2(size.x/2 - gapSize/2, boarderSize);
	board[3].setOriginTopLeft();
	board[3].pos = vec2(pos.x, pos.y + size.y);

	//bot right
	board[4].scale = vec2(size.x/2 - gapSize/2, boarderSize);
	board[4].setOriginTopRight();
	board[4].pos = vec2(pos.x + size.x, pos.y + size.y);

	//background
	board[5].scale = size;
	board[5].setOriginTopLeft();
	board[5].pos = pos;

	//background
	board[6].scale = vec2(gapSize, boarderSize);
	board[6].setOriginTopLeft();
	board[6].pos = vec2(pos.x + board[4].scale.x, pos.y + size.y);

	return board;
}

// corner is the corner of the interior of the board, size is the size of single
// square piece and sepDist is the seperation between the pieces. 
std::vector<GamePiece> makePieces(glm::vec2 corner, int size, int sepDist) {
	using namespace glm;

	GamePiece::setRefPoint(vec2(corner.x + sepDist, corner.y + sepDist));
	GamePiece::setGridSize(size + sepDist);

	std::vector<GamePiece> pieces(10);

	// square pieces (color and scale)
	for (int i = 0; i < 5; i++) {
		if (i == 0) {
			pieces[i].scale = glm::vec2((size * 2) + sepDist); // Big square
			pieces[i].color = glm::vec3(1, 0, 0); // Red
			pieces[i].setType(2);
		}
		else {
			pieces[i].scale = glm::vec2(size); // Smaller Squares
			pieces[i].color = glm::vec3(0, 1, 0); // Green
			pieces[i].setType(1);
		}
		pieces[i].setOriginTopLeft();
	}

	// rectangle pieces (color and scale)
	for (int i = 5; i < 10; i++) {
		if (i == 5) {
			pieces[i].scale = glm::vec2((size * 2) + sepDist, size); // Horizontal Rect
			pieces[i].setType(4);
		}
		else {
			pieces[i].scale = glm::vec2(size, (size * 2) + sepDist); // Vertical Rect
			pieces[i].setType(3);
		}
		pieces[i].setOriginTopLeft();
		pieces[i].color = glm::vec3(0, 0, 1); // Blue
	}

	// Pieces Positions (using relative coordinates of the board)
	// Big Red Square
	pieces[0].boardCoord = vec2(1, 0);
	// Small Green Squares
	pieces[1].boardCoord = vec2(0, 4);
	pieces[2].boardCoord = vec2(1, 3);
	pieces[3].boardCoord = vec2(2, 3);
	pieces[4].boardCoord = vec2(3, 4);
	// Horizontal Rectangle
	pieces[5].boardCoord = vec2(1, 2);
	// Vertical Rectangle
	pieces[6].boardCoord = vec2(0, 0);
	pieces[7].boardCoord = vec2(0, 2);
	pieces[8].boardCoord = vec2(3, 0);
	pieces[9].boardCoord = vec2(3, 2);

	for (int i = 0; i < 10; i++) {
		pieces[i].setBoardPosition();
	}

	/*
	for (int i = 0; i < 10; i++) {
		pieces[i].pos += pieces[i].originOffset;
		pieces[i].originOffset = vec2(0);
	}
	*/
	return pieces;
}

// All buttons are rectangles in a column. refPoint is the upper left corner of 
// the first button. Size is how tall, width is double the size. sepdits is the
// seperation between the buttons.
std::vector<Quad>makeButtons(glm::vec2 refPoint, int size, int sepDist) {
	using namespace glm;

	std::vector<Quad> buttons(3);
	// 0 is UNDO
	// 1 is RESET
	// 2 is EXIT

	// Color
	buttons[0].color = vec3(1, 0.834, 0);
	buttons[1].color = vec3(0.980, 0.502, 0.447);
	buttons[2].color = vec3(0.282, 0.239, 0.545);

	// Scale
	for (int i = 0; i < 3; i++) {
		buttons[i].scale = vec2(2 * size, size);
		buttons[i].setOriginTopLeft();
	}

	// Positions
	for (int i = 0; i < 3; i++) {
		buttons[i].pos = refPoint + vec2(0, i * (size + sepDist));
	}

	return buttons;
}

// getDirection using cell coordinates
char getDirection(glm::vec2& currentCell, glm::vec2& lastCell) {
	char direction;
	glm::vec2 cardinalVec = currentCell - lastCell;

	if (cardinalVec.y) {
		if (cardinalVec.y < 0) {
			direction = 'U';
		}
		else {
			direction = 'D';
		}
	}
	else {
		if (cardinalVec.x < 0) {
			direction = 'L';
		}
		else {
			direction = 'R';
		}
	}
	return direction;
}

void displayFilled(int dataAry[]) {
	std::cout << "Current Grid Status:" << std::endl;
	for (int i = 1; i < 21; i++) {
		std::cout << dataAry[i - 1] << ", ";
		if (i % 4 == 0) {
			std::cout << "\n";
		}
	}
	std::cout << "\n";
}

void resetBoard(std::vector<GamePiece>& pieces, int filledData[]) {
	using namespace glm;

	int originalFilled[20] = {
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 0, 0, 1
	};

	// Big Red Square
	pieces[0].boardCoord = vec2(1, 0);
	// Small Green Squares
	pieces[1].boardCoord = vec2(0, 4);
	pieces[2].boardCoord = vec2(1, 3);
	pieces[3].boardCoord = vec2(2, 3);
	pieces[4].boardCoord = vec2(3, 4);
	// Horizontal Rectangle
	pieces[5].boardCoord = vec2(1, 2);
	// Vertical Rectangle
	pieces[6].boardCoord = vec2(0, 0);
	pieces[7].boardCoord = vec2(0, 2);
	pieces[8].boardCoord = vec2(3, 0);
	pieces[9].boardCoord = vec2(3, 2);

	for (int i = 0; i < 10; i++) {
		pieces[i].setBoardPosition();
	}

	for (int i = 0; i < 20; i++) {
		filledData[i] = originalFilled[i];
	}
}

void gameLoop(GLFWwindow* window)
{
	using namespace std;
	using namespace glm;

	const vec2 CORNER = vec2(200, 44); // vec2(200, 100)
	const int PIECE_SIZE = 88;          // 64
	const int SEP_DIST = 12;             // 8
	const int BORDER_WIDTH = 44;        // 46

	int gridSize = PIECE_SIZE + SEP_DIST;
	vec2 boardSize = vec2((4 * gridSize + SEP_DIST), (5 * gridSize + SEP_DIST));
	int gapSize = 2 * gridSize + SEP_DIST;

	std::vector<Quad> board = makeBoard(boardSize, CORNER, BORDER_WIDTH, gapSize);
	std::vector<GamePiece> pieces = makePieces(CORNER, PIECE_SIZE, SEP_DIST);
	std::vector<Quad> buttons = makeButtons(vec2(39, 124), 39, 117);

	vec2 coordinates[20] = {
		vec2(0,0), vec2(1,0), vec2(2,0), vec2(3,0),
		vec2(0,1), vec2(1,1), vec2(2,1), vec2(3,1),
		vec2(0,2), vec2(1,2), vec2(2,2), vec2(3,2),
		vec2(0,3), vec2(1,3), vec2(2,3), vec2(3,3),
		vec2(0,4), vec2(1,4), vec2(2,4), vec2(3,4)
	};

	int filled[20] = {
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 0, 0, 1
	};

	GamePiece* heldPiece = nullptr;

	vec2 currentCell;
	vec2 lastCell;
	double xPos, yPos;
	char direction;

	int moveCounter = 0;

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (Quad quad : board)
			quad.draw();

		for (Quad b : buttons)
			b.draw();

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &xPos, &yPos);
			currentCell.x = (int)((xPos - CORNER.x) / gridSize);
			currentCell.y = (int)((yPos - CORNER.y) / gridSize);
			//cout << "currentCell: " << currentCell.x << ", " << currentCell.y << endl;

			if (!heldPiece) {
				for (int i = 0; i < 10; i++)
				{
					if (pieces[i].checkCollision(vec2(xPos, yPos)))
					{
						std::cout << "Piece Selected\n" << std::endl;
						heldPiece = &pieces[i];
						lastCell = currentCell;
						//cout << "lastCell: " << lastCell.x << ", "
						//<< lastCell.y << endl;
					}
				}
			}
			else {
				if (currentCell != lastCell) {
					direction = getDirection(currentCell, lastCell);
					//cout << "Got " << direction << endl;

					if (heldPiece->isMovableIn(direction, coordinates, filled)) {
						heldPiece->moveIn(direction, coordinates, filled);
						heldPiece->setBoardPosition();
						lastCell = currentCell;
						//cout << "lastCell: " << lastCell.x << ", " 
						//<< lastCell.y << endl;

						moveCounter++;

						cout << "Total Number of Moves: " << moveCounter << "\n" << endl;
						displayFilled(filled);
					}
				}
			}

			if (moveCounter) {
				if (buttons[1].checkCollision(vec2(xPos, yPos))) {
					cout << "Reseting Board!\n" << endl;
					resetBoard(pieces, filled);
					moveCounter = 0;

					displayFilled(filled);
				}
			}

			if (buttons[2].checkCollision(vec2(xPos, yPos))) {
				cout << "Exiting! Bye-Bye!\n" << endl;
				glfwSetWindowShouldClose(window, true);
			}
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
			heldPiece = nullptr;
		}

		for (GamePiece p : pieces) {
			p.draw();
		}

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
}

// Best so far (Key Movement)
/*
if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
{
	glfwGetCursorPos(window, &lastX, &lastY);

	if (!heldPiece)
		for (int i = 0; i < 10; i++)
		{
			if (pieces[i].checkCollision(vec2(lastX, lastY)))
			{
				std::cout << "Selecting Piece\n" << std::endl;
				heldPiece = &pieces[i];
			}
		}
}

if (heldPiece)
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		direction = 'U';
		if (heldPiece->isMovableIn(direction, coordinates, filled)) {
			heldPiece->moveIn(direction, coordinates, filled);
			heldPiece->setBoardPosition();

			displayFilled(filled);
		}
		heldPiece = nullptr;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		direction = 'D';
		if (heldPiece->isMovableIn(direction, coordinates, filled)) {
			heldPiece->moveIn(direction, coordinates, filled);
			heldPiece->setBoardPosition();

			displayFilled(filled);
		}
		heldPiece = nullptr;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		direction = 'L';
		if (heldPiece->isMovableIn(direction, coordinates, filled)) {
			heldPiece->moveIn(direction, coordinates, filled);
			heldPiece->setBoardPosition();

			displayFilled(filled);
		}
		heldPiece = nullptr;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		direction = 'R';
		if (heldPiece->isMovableIn(direction, coordinates, filled)) {
			heldPiece->moveIn(direction, coordinates, filled);
			heldPiece->setBoardPosition();

			displayFilled(filled);
		}
		heldPiece = nullptr;
	}
}
*/