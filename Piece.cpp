#include "Piece.h"
#include "pieces.h"

Piece::Piece(int piece, int rotation)
{
	m_piece = piece;
	m_rotation = rotation;
}

void Piece::rotate()
{
	m_rotation++;
	if (m_rotation > ROTATION_270) {
		m_rotation = ROTATION_0;
	}
}

void Piece::resetBlocksEnum()
{
	m_blockEnumX = 0;
	m_blockEnumY = 0;
}

/**
 * Finds next piece's block and return its position
 */
bool Piece::iterateBlocksEnum(int *x, int *y)
{
	bool hasResult = false;
	while (
		!hasResult
		&& m_blockEnumX < PIECE_SIZE
		&& m_blockEnumY < PIECE_SIZE
	) {
		if (m_rotation == ROTATION_0) {
			hasResult = (pieceBlocks[m_piece][m_blockEnumY][m_blockEnumX] > 0);
		} else if (m_rotation == ROTATION_90) {
			hasResult = (pieceBlocks[m_piece][PIECE_SIZE-m_blockEnumX-1][m_blockEnumY] > 0);
		} else if (m_rotation == ROTATION_180) {
			hasResult = (pieceBlocks[m_piece][PIECE_SIZE-m_blockEnumY-1][PIECE_SIZE-m_blockEnumX-1] > 0);
		} else {
			hasResult = (pieceBlocks[m_piece][m_blockEnumX][PIECE_SIZE-m_blockEnumY-1] > 0);
		}

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

Uint8 Piece::getColorRed()
{
	return pieceColor[m_piece][0];
}

Uint8 Piece::getColorGreen()
{
	return pieceColor[m_piece][1];
}

Uint8 Piece::getColorBlue()
{
	return pieceColor[m_piece][2];
}

int Piece::getPiece()
{
	return m_piece;
}
