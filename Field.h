#pragma once

#include "Painter.h"
#include "Piece.h"

#define FIELD_WIDTH			10 // blocks
#define FIELD_HEIGHT		20 // blocks

#define FIELD_X_PX			10
#define FIELD_Y_PX			10
#define FIELD_WIDTH_PX		(BLOCK_SIZE_PX * FIELD_WIDTH)
#define FIELD_HEIGHT_PX		(BLOCK_SIZE_PX * FIELD_HEIGHT)

#define BLANK				-1

class Field
{
public:
	Field(Painter *painter);
	void draw();
	void reset();
	void drawPiece(Piece *piece, int posX, int posY);
	Uint8 checkCollisions(Piece *piece, int posX, int posY, Uint8 what);
	int clearRows();
	void copyPieceToHeap(Piece *piece, int posX, int posY);

	enum {
		COLLIDED_LEFT  = 1,
		COLLIDED_RIGHT = 2,
		COLLIDED_HEAP  = 4,
		COLLIDED_TOP   = 8
	};

private:
	void drawHeap();

	Painter *m_painter;
	int m_field[FIELD_WIDTH][FIELD_HEIGHT];
};
