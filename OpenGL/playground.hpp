#pragma once
#include <iostream>
#include "glm/glm.hpp"
#include "Quad.h"
#include "GLFW/glfw3.h"

//testing area

Quad* gridResizeVec(Quad* squares, int newSize, float squareSize)
{
	using namespace glm;
	delete[] squares;
	squares = new Quad[newSize];
	for (int i = 0; i < newSize; i++)
	{
		squares[i].scale = vec2(squareSize);
	}
	return squares;
}

void gridLoop(GLFWwindow* window)
{
	using namespace glm;
	float squareSize = 60;
	int indexResetValue = 0;
	int rowIndex = indexResetValue;
	int colIndex = indexResetValue;
	int rowSize = 10;
	int colSize = 10;
	int numberOfSquares = rowSize * colSize;
	Quad* squares = new Quad[numberOfSquares];
	int sepDist = 61;
	for (int i = 0; i < numberOfSquares; i++)
	{
		squares[i].scale = vec2(squareSize);
	}

	bool pressed = false;

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		rowIndex = indexResetValue;
		colIndex = indexResetValue;

		for (int i = 0; i < numberOfSquares; i++)
		{
			if (colIndex >= colSize)
			{
				colIndex = indexResetValue;
				rowIndex++;
			}
			if (rowIndex >= rowSize)
				rowIndex = indexResetValue;

			squares[i].pos = vec2(sepDist * rowIndex, sepDist * colIndex);
			squares[i].draw();
			colIndex++;
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			if (colSize > 1)
			{
				colSize -= 1;
				numberOfSquares = rowSize * colSize;
				squares = gridResizeVec(squares, numberOfSquares, squareSize);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			colSize += 1;
			numberOfSquares = rowSize * colSize;
			squares = gridResizeVec(squares, numberOfSquares, squareSize);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			if (rowSize > 1)
			{
				rowSize -= 1;
				numberOfSquares = rowSize * colSize;
				squares = gridResizeVec(squares, numberOfSquares, squareSize);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			rowSize += 1;
			numberOfSquares = rowSize * colSize;
			squares = gridResizeVec(squares, numberOfSquares, squareSize);
		}

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
}

void printMat4(glm::mat4 mat)
{
	using namespace glm;
	int x = 0, y = 0;
	for (int i = 0; i < 16; i++)
	{
		if (x > 3)
		{
			x = 0;
			y++;
			printf("\n");
		}
		printf("%f ", mat[x][y]);
		x++;
	}
	printf("\n\n");
}

void circleLoop(GLFWwindow* window)
{
	using namespace glm;
	Quad square = Quad();
	vec2 center = vec2(400, 300);
	int numberOfSquares = 32;
	float radius = 250;
	float ang = 0;
	float angChange = 1;

	mat4 modelMatrix = mat4(1.0f);

	modelMatrix = glm::translate(modelMatrix, vec3(3, 3, 3));
	printMat4(modelMatrix);

	modelMatrix = glm::rotate(modelMatrix, (float) radians(30.0f), glm::vec3(1,1,1));
	printMat4(modelMatrix);

	modelMatrix = glm::scale(modelMatrix, vec3(1, 1, 0));
	printMat4(modelMatrix);


	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		square.scale = vec2(3 * radius / numberOfSquares);

		ang += angChange;
		if (ang >= 360)
			ang = 0;

		for (int i = 0; i < numberOfSquares; i++)
		{
			float thisAng = ang + i * 360 / numberOfSquares;
			square.rotation = thisAng;
			square.pos = vec2(
				center.x + cos(radians(thisAng)) * radius,
				center.y + sin(radians(thisAng)) * radius
			);
			square.color = vec4(cos(radians(thisAng)), sin(radians(thisAng)), 1 * (numberOfSquares / (radius / 3)), 1);
			square.draw();
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			numberOfSquares += 1;
			printf("squares is: %i\n", numberOfSquares);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			if (numberOfSquares > 0)
			{
				numberOfSquares -= 1;
				printf("squares is: %i\n", numberOfSquares);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			radius += 1;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			if (radius > 2)
				radius -= 1;
		}

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
}

void testLoop(GLFWwindow* window)
{
	using namespace glm;
	const int totalSquares = 3;
	Quad squares[totalSquares];
	for (int i = 0; i < totalSquares; i++)
		squares[i] = Quad(vec2(64 + 64*i), 0, vec2(64));
	Quad* heldSquare = nullptr;
	double mouseX, mouseY;
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &mouseX, &mouseY);

			if(!heldSquare)
				for (int i = 0; i < totalSquares; i++)
				{
					if (squares[i].checkCollision(vec2(mouseX, mouseY)))
					{
						heldSquare = &squares[i];
					}
				}
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
			heldSquare = nullptr;
		if (heldSquare)
			heldSquare->pos = vec2(mouseX, mouseY);

		for(Quad s : squares)
			s.draw();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
}

void collisionTestLoop(GLFWwindow* window)
{
	using namespace glm;
	int gamePeiceSize = 64;
	float offSet = gamePeiceSize / 2;
	Quad gamePieces[4];
	for (int i = 0; i < 4; i++)
	{
		Quad *piece = &gamePieces[i];
		piece->originOffset = vec2(offSet);
		piece->pos = vec2(gamePeiceSize * i, 0);
		piece->scale = vec2(gamePeiceSize, gamePeiceSize * 2);
		if (i % 2)
		piece->color = vec4(1, 0, 0, 1);
	}

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		for (Quad peice : gamePieces)
			peice.draw();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
}