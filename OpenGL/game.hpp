#pragma once
#include <vector>

#include "glm/glm.hpp"
#include "Quad.h"
#include "GLFW/glfw3.h"

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
std::vector<Quad> makePieces(glm::vec2 corner, int size, int sepDist) {
	using namespace glm;

	std::vector<Quad> pieces(10);

	int gridSize = size + sepDist;
	vec2 refPoint = vec2(corner.x + sepDist, corner.y + sepDist);

	// square pieces (color and scale)
	for (int i = 0; i < 5; i++) {
		if (i == 0) {
			pieces[i].scale = glm::vec2((size * 2) + sepDist); // Big square
			pieces[i].color = glm::vec3(1, 0, 0); // Red
		}
		else {
			pieces[i].scale = glm::vec2(size); // Smaller Squares
			pieces[i].color = glm::vec3(0, 1, 0); // Green
		}
		pieces[i].setOriginTopLeft();
	}

	// rectangle pieces (color and scale)
	for (int i = 5; i < 10; i++) {
		if (i == 5) {
			pieces[i].scale = glm::vec2((size * 2) + sepDist, size); // Horizontal Rect
		}
		else {
			pieces[i].scale = glm::vec2(size, (size * 2) + sepDist); // Vertical Rect
		}
		pieces[i].setOriginTopLeft();
		pieces[i].color = glm::vec3(0, 0, 1); // Blue
	}

	// Pieces Positions
	// Big Red Square
	pieces[0].pos = vec2(refPoint.x + (1 * gridSize), refPoint.y + (0 * gridSize));
	// Small Green Squares
	pieces[1].pos = vec2(refPoint.x + (0 * gridSize), refPoint.y + (4 * gridSize));
	pieces[2].pos = vec2(refPoint.x + (1 * gridSize), refPoint.y + (3 * gridSize));
	pieces[3].pos = vec2(refPoint.x + (2 * gridSize), refPoint.y + (3 * gridSize));
	pieces[4].pos = vec2(refPoint.x + (3 * gridSize), refPoint.y + (4 * gridSize));
	// Horizontal Rectangle
	pieces[5].pos = vec2(refPoint.x + (1 * gridSize), refPoint.y + (2 * gridSize));
	// Vertical Rectangle
	pieces[6].pos = vec2(refPoint.x + (0 * gridSize), refPoint.y + (0 * gridSize));
	pieces[7].pos = vec2(refPoint.x + (0 * gridSize), refPoint.y + (2 * gridSize));
	pieces[8].pos = vec2(refPoint.x + (3 * gridSize), refPoint.y + (0 * gridSize));
	pieces[9].pos = vec2(refPoint.x + (3 * gridSize), refPoint.y + (2 * gridSize));

	for (int i = 0; i < 10; i++) {
		pieces[i].pos += pieces[i].originOffset;
		pieces[i].originOffset = vec2(0);
	}

	return pieces;
}

void gameLoop(GLFWwindow* window)
{
	using namespace glm;
	std::vector<Quad> board = makeBoard(vec2(296, 368), vec2(200,100), 46, 152);
	std::vector<Quad> pieces = makePieces(vec2(200, 100), 64, 8);

	Quad* heldSquare = nullptr;
	double mouseX, mouseY;

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (Quad quad : board)
			quad.draw();

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &mouseX, &mouseY);

			if (!heldSquare)
				for (int i = 0; i < 10; i++)
				{
					if (pieces[i].checkCollision(vec2(mouseX, mouseY)))
					{
						heldSquare = &pieces[i];
					}
				}
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
			heldSquare = nullptr;

		if (heldSquare) {
			heldSquare->pos = vec2(mouseX, mouseY);
		}

		for (Quad quad : pieces)
			quad.draw();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
}