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

void makePieces() {
	using namespace glm;
	Quad squares[5];
	Quad rectangles[5];

	//how this works I don't know
	squares[0] = glm::vec2(400, 300);
	squares[1] = glm::vec2(300, 164);
	squares[2] = glm::vec2(300, 368);
	squares[3] = glm::vec2(500, 164);
	squares[4] = glm::vec2(500, 368);

	for (int i = 0; i < 5; i++) {
		if (i == 1) {
			rectangles[i].scale = glm::vec2(128, 64);
			rectangles[i].color = glm::vec3(0, 1, 0);
		}
		else {
			rectangles[i].scale = glm::vec2(64, 128);
			rectangles[i].color = glm::vec3(0, 1, 0);
		}
	}

	squares[0].pos = vec2(400, 164);
	squares[1].pos = vec2(300, 468);
	squares[2].pos = vec2(368, 368);
	squares[3].pos = vec2(432, 368);
	squares[4].pos = vec2(300, 468);

	for (int i = 0; i < 5; i++) {
		if (i == 1) {
			squares[i].scale = glm::vec2(128);
			squares[i].color = glm::vec3(1, 0, 0);
		}
		else {
			squares[i].scale = glm::vec2(64);
			squares[i].color = glm::vec3(0, 0, 1);
		}
	}

	for (int i = 0; i < 5; i++) {
		squares[i].draw();
		rectangles[i].draw();
	}
}

void gameLoop(GLFWwindow* window)
{
	using namespace glm;
	std::vector<Quad> board = makeBoard(vec2(300, 300), vec2(200,100), 46, 100);
	//kevin, make game peice vectors/arrays local to this function scope and have thier init be
	//returned by a function like above, this will make updating them later much easier since it
	//will be local to the loop

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (Quad quad : board)
			quad.draw();

		makePieces();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
}