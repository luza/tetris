#include "Piece.h"
#include "pieces.h"

Piece::Piece()
{
	m_piece = 0;
	m_rotation = ROTATION_0;
}

Piece::Piece(int piece, int rotation)
{
	m_piece = piece;
	m_rotation = rotation;
}

void
Piece::rotate()
{
	m_rotation++;
	if (m_rotation > ROTATION_270) {
		m_rotation = ROTATION_0;
	}
}

void
Piece::resetBlocksEnum()
{
	m_blockEnumX = 0;
	m_blockEnumY = 0;
}

/**
 * Finds next piece's block and return its position
 */
bool
Piece::iterateBlocksEnum(int *x, int *y)
{
	bool hasResult = false;
	while (
		!hasResult
		&& m_blockEnumX < PIECE_SIZE
		&& m_blockEnumY < PIECE_SIZE
	) {
		hasResult = checkHasBlock(m_blockEnumX, m_blockEnumY);

		if (hasResult) {
			*x = m_blockEnumX;
			*y = m_blockEnumY;
		}

		m_blockEnumY++;
		if (m_blockEnumY >= PIECE_SIZE) {
			m_blockEnumY = 0;
			m_blockEnumX++;
		}
	}
	return hasResult;
}

bool
Piece::checkHasBlock(int x, int y)
{
	if (m_rotation == ROTATION_0) {
		return (pieceBlocks[m_piece][y][x] > 0);
	} else if (m_rotation == ROTATION_90) {
		return (pieceBlocks[m_piece][PIECE_SIZE-x-1][y] > 0);
	} else if (m_rotation == ROTATION_180) {
		return (pieceBlocks[m_piece][PIECE_SIZE-y-1][PIECE_SIZE-x-1] > 0);
	} else {
		return (pieceBlocks[m_piece][x][PIECE_SIZE-y-1] > 0);
	}
}

Uint8
Piece::getColorRed()
{
	return pieceColor[m_piece][0];
}

Uint8
Piece::getColorGreen()
{
	return pieceColor[m_piece][1];
}

Uint8
Piece::getColorBlue()
{
	return pieceColor[m_piece][2];
}

void
Piece::setPiece(int piece)
{
	m_piece = piece;
}

int
Piece::getPiece()
{
	return m_piece;
}

void
Piece::setRotation(int rotation)
{
	m_rotation = rotation;
}

int
Piece::getRotation()
{
	return m_rotation;
}
