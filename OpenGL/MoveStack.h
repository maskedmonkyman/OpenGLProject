#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "GLM/gtx/transform.hpp"

#include "Quad.h"
#include "GamePiece.h"

struct Move {
	char direction;
	GamePiece* piecePtr;
};

typedef struct Move TSMove;
typedef struct Move* TSMovePtr;

struct MoveNode {
	TSMove data;
	struct MoveNode* next;
};

typedef struct MoveNode TSMoveNode;
typedef struct MoveNode* TSMoveNodePtr;

class MoveStack {
private:
	TSMoveNodePtr moveList;

public:
	MoveStack();
	~MoveStack();

	void push(char, GamePiece*);
	TSMoveNodePtr pop();

	bool isEmpty(void);

	void empty(void);
};
