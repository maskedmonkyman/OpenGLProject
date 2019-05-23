#pragma once 
#include <vector>

#include "glm/glm.hpp"
#include "Quad.h"
#include "GamePiece.h"
#include "MoveStack.h"
#include "GLFW/glfw3.h"

#include <iostream>

std::vector<Quad> makeBoard(glm::vec2 size, glm::vec2 pos, int boarderSize, int gapSize);

std::vector<GamePiece> makePieces(glm::vec2 corner, int size, int sepDist);

std::vector<Quad> makeButtons(glm::vec2 refPoint, int size, int sepDist);

char getDirection(glm::vec2& currentCell, glm::vec2& lastCell);

void resetBoard(std::vector<GamePiece>& pieces, int filledData[]);

void undo(TSMoveNodePtr info, glm::vec2 coordinates[], int filled[]);

void solve(std::vector<GamePiece>& pieces, int filled[]);

void gameLoop(GLFWwindow* window);