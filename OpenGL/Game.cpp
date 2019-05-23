#pragma once
#include <vector>

#include "glm/glm.hpp"
#include "Quad.h"
#include "GamePiece.h"
#include "MoveStack.h"
#include "game.h"
#include "GLFW/glfw3.h"

#include <iostream>

//makes a game board, size is the internal rectangle size, pos is top left corner
//boarderSize is how thick the boarders will be (note this will be added to size)
//gap size is how big the bottom gap is
std::vector<Quad> makeBoard(glm::vec2 size, glm::vec2 pos, int boarderSize, int gapSize)
{
	using namespace glm;
	std::vector<Quad> board(7);
	vec3 color = vec3(255.0f / 255, 144.0f / 255, 0.0f / 255); //orange

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
	board[3].scale = vec2(size.x / 2 - gapSize / 2, boarderSize);
	board[3].setOriginTopLeft();
	board[3].pos = vec2(pos.x, pos.y + size.y);

	//bot right
	board[4].scale = vec2(size.x / 2 - gapSize / 2, boarderSize);
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
	GamePiece::setCellSize(size + sepDist);

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

	return pieces;
}

// Four buttons, two on either side. refPoint is the upper left corner of 
// the first button. Size is how tall and wide. sepdist is the
// seperation between the buttons.
std::vector<Quad> makeButtons(glm::vec2 refPoint, int size, int sepDist) {
	using namespace glm;

	vec2 oppositeRefPoint = vec2((800 - refPoint.x - size), refPoint.y);

	std::vector<Quad> buttons(4);
	// 0 is UNDO
	// 1 is RESET
	// 2 is CHEAT
	// 3 is EXIT

	// Color
	buttons[0].color = vec3(1, 0.843, 0);
	buttons[1].color = vec3(0, 0.392, 0.);
	buttons[2].color = vec3(0.545, 0, 0);
	buttons[3].color = vec3(0.282, 0.239, 0.545);

	// Scale
	for (int i = 0; i < 4; i++) {
		buttons[i].scale = vec2(size);
		buttons[i].setOriginTopLeft();
	}

	// Positions
	buttons[0].pos = refPoint + vec2(0, 0 * (size + sepDist));
	buttons[1].pos = refPoint + vec2(0, 1 * (size + sepDist));
	buttons[2].pos = oppositeRefPoint + vec2(0, 0 * (size + sepDist));
	buttons[3].pos = oppositeRefPoint + vec2(0, 1 * (size + sepDist));

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

void resetBoard(std::vector<GamePiece>& pieces, int filled[]) {
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
		filled[i] = originalFilled[i];
	}
}

void undo(TSMoveNodePtr info, glm::vec2 coordinates[], int filled[]) {
	char undoDirection;
	GamePiece* targetPiece = info->data.piecePtr;

	if (info->data.direction == 'U') {
		undoDirection = 'D';
	}
	else if (info->data.direction == 'D') {
		undoDirection = 'U';
	}
	else if (info->data.direction == 'R') {
		undoDirection = 'L';
	}
	else {
		if (info->data.direction == 'L') {
			undoDirection = 'R';
		}
	}

	if (targetPiece->isMovableIn(undoDirection, coordinates, filled)) {
		targetPiece->moveIn(undoDirection, coordinates, filled);
		targetPiece->setBoardPosition();
	}
}

void solve(std::vector<GamePiece>& pieces, int filled[]) {
	using namespace glm;

	int solvedFilled[20] = {
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1,
		0, 1, 1, 1,
		0, 1, 1, 1
	};

	// Big Red Square
	pieces[0].boardCoord = vec2(1, 3);
	// Small Green Squares
	pieces[1].boardCoord = vec2(0, 2);
	pieces[2].boardCoord = vec2(1, 2);
	pieces[3].boardCoord = vec2(3, 3);
	pieces[4].boardCoord = vec2(3, 4);
	// Horizontal Rectangle
	pieces[5].boardCoord = vec2(2, 2);
	// Vertical Rectangle
	pieces[6].boardCoord = vec2(0, 0);
	pieces[7].boardCoord = vec2(1, 0);
	pieces[8].boardCoord = vec2(2, 0);
	pieces[9].boardCoord = vec2(3, 0);

	for (int i = 0; i < 10; i++) {
		pieces[i].setBoardPosition();
	}

	for (int i = 0; i < 20; i++) {
		filled[i] = solvedFilled[i];
	}
}

void gameLoop(GLFWwindow* window)
{
	using namespace std;
	using namespace glm;

	const vec2 CORNER = vec2(216, 64);
	const int PIECE_SIZE = 80;
	const int SEP_DIST = 12;
	const int BORDER_WIDTH = 44;

	int gridSize = PIECE_SIZE + SEP_DIST;
	vec2 boardSize = vec2((4 * gridSize + SEP_DIST), (5 * gridSize + SEP_DIST));
	int gapSize = 2 * gridSize + SEP_DIST;

	std::vector<Quad> board = makeBoard(boardSize, CORNER, BORDER_WIDTH, gapSize);
	std::vector<GamePiece> pieces = makePieces(CORNER, PIECE_SIZE, SEP_DIST);
	std::vector<Quad> buttons = makeButtons(vec2(36, 124), 80, 117);

	MoveStack* undoStack = new MoveStack();
	TSMoveNodePtr undoInfoPtr = nullptr;

	vec2 cellCoordinates[20] = {
		vec2(0,0), vec2(1,0), vec2(2,0), vec2(3,0),
		vec2(0,1), vec2(1,1), vec2(2,1), vec2(3,1),
		vec2(0,2), vec2(1,2), vec2(2,2), vec2(3,2),
		vec2(0,3), vec2(1,3), vec2(2,3), vec2(3,3),
		vec2(0,4), vec2(1,4), vec2(2,4), vec2(3,4)
	};

	int filledData[20] = {
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 0, 0, 1
	};

	GamePiece* heldPiece = nullptr;

	bool clicked = false;

	double xPos, yPos;
	vec2 currentCell;
	vec2 lastCell;
	char direction;

	int moveCounter = 0;

	cout << "\nSQUARES ON EACH SIDE:" <<
		"\n  UNDO  - Gold" <<
		"\n  RESET - Dark Green" <<
		"\n  CHEAT - Dark Red" <<
		"\n  EXIT  - Purple\n\n" << endl;

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

			if (!heldPiece) {
				for (int i = 0; i < 10; i++)
				{
					if (pieces[i].checkCollision(vec2(xPos, yPos)))
					{
						heldPiece = &pieces[i];
						lastCell = currentCell;
					}
				}
			}
			else {
				if (currentCell != lastCell) {
					direction = getDirection(currentCell, lastCell);

					if (heldPiece == &pieces[0] &&
						pieces[0].boardCoord == vec2(1, 3) &&
						direction == 'D')
					{
						pieces[0].boardCoord = vec2(1, 4);
						pieces[0].setBoardPosition();
						undoStack->empty();
						cout << "YOU WIN!!! Bye-Bye.\n" << endl;
						glfwSetWindowShouldClose(window, true);
					}

					if (heldPiece->isMovableIn(direction, cellCoordinates, filledData)) {
						heldPiece->moveIn(direction, cellCoordinates, filledData);
						heldPiece->setBoardPosition();
						lastCell = currentCell;

						moveCounter++;
						cout << "Total Number of Moves: " << moveCounter << "\n" << endl;
						undoStack->push(direction, heldPiece);
					}
				}
			}

			if (!clicked) {
				if (buttons[0].checkCollision(vec2(xPos, yPos))) {
					if (!undoStack->isEmpty()) {
						cout << "Undoing Last Move\n" << endl;
						undoInfoPtr = undoStack->pop();
						undo(undoInfoPtr, cellCoordinates, filledData);

						delete undoInfoPtr;
						undoInfoPtr = nullptr;

						moveCounter--;
						cout << "Total Number of Moves: " << moveCounter << "\n" << endl;
						clicked = true;
					}
				}
			}

			if (!clicked) {
				if (buttons[1].checkCollision(vec2(xPos, yPos))) {
					cout << "Reseting Board!\n" << endl;
					resetBoard(pieces, filledData);
					moveCounter = 0;
					cout << "Total Number of Moves: " << moveCounter << "\n" << endl;
					undoStack->empty();

					clicked = true;
				}
			}

			if (!clicked) {
				if (buttons[2].checkCollision(vec2(xPos, yPos))) {
					cout << "Solving Board!\n" << endl;
					solve(pieces, filledData);
					moveCounter = 0;
					undoStack->empty();

					clicked = true;
				}
			}

			if (buttons[3].checkCollision(vec2(xPos, yPos))) {
				undoStack->empty();
				cout << "Exiting! Bye-Bye!\n" << endl;
				glfwSetWindowShouldClose(window, true);
			}
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
			heldPiece = nullptr;
			clicked = false;
		}

		for (GamePiece p : pieces) {
			p.draw();
		}

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	delete undoStack;
	undoStack = nullptr;
}
