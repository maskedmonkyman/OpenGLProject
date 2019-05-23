#include "MoveStack.h"
#include <iostream>

MoveStack::MoveStack() : movesList(nullptr) {}

MoveStack::~MoveStack() {
	this->empty();
}

void MoveStack::push(char direction, GamePiece* piece) {
	TSMoveNodePtr newNode = new TSMoveNode;

	newNode->data.direction = direction;
	newNode->data.piecePtr = piece;
	newNode->next = nullptr;

	newNode->next = movesList;
	movesList = newNode;
}

TSMoveNodePtr MoveStack::pop() {
	TSMoveNodePtr popped = nullptr;

	if (movesList) {
		popped = movesList;
		movesList = movesList->next;
	}
	popped->next = nullptr;

	return popped;
}

bool MoveStack::isEmpty() {
	bool isEmpty;

	if (movesList) {
		isEmpty = false;
	}
	else {
		isEmpty = true;
	}
	return isEmpty;
}

void MoveStack::empty() {
	TSMoveNodePtr nodeAddr = nullptr;

	while (!isEmpty()) {
		nodeAddr = this->pop();
		delete nodeAddr;
		nodeAddr = nullptr;
	}
}