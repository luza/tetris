#pragma once

#include "SDL.h"

class Piece {
public:
	Piece(int piece, int rotation);
	void resetBlocksEnum();
	bool iterateBlocksEnum(int *x, int *y);
	void rotate();

	int getPiece();
	Uint8 getColorRed();
	Uint8 getColorGreen();
	Uint8 getColorBlue();

	enum {
		ROTATION_0,
		ROTATION_90,
		ROTATION_180,
		ROTATION_270
	};

private:
	int m_piece;
	int m_rotation;

	int m_blockEnumX;
	int m_blockEnumY;
};