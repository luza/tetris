#pragma once

#include "Field.h"
#include "Piece.h"

class Player
{
public:
	Player(Field *field);
	~Player();

	enum {
		ACTION_MOVE_LEFT,
		ACTION_MOVE_RIGHT,
		ACTION_ROTATE,
		ACTION_DROP
	};

	int getNextAction(int posX, int posY, int rotation);
	void setContext(Piece *piece, Piece *nextPiece);

private:
	void calculateDest();
	int checkXR(int posX, int r);

	int m_destPosX, m_destPosY, m_destRotation;

	Field *m_field;
	Painter *m_painter;
	Piece m_piece, m_nextPiece;
};
