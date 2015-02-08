#include "Field.h"
#include "pieces.h"

Field::Field(Painter *painter)
{
	m_painter = painter;
}

void
Field::reset()
{
	// clear field
	for (Uint8 x=0; x<FIELD_WIDTH; x++) {
		for (Uint8 y=0; y<FIELD_HEIGHT; y++) {
			m_field[x][y] = BLANK;
		}
	}
}

void
Field::draw()
{
	m_painter->drawField(
		FIELD_X_PX,
		FIELD_Y_PX,
		FIELD_WIDTH_PX,
		FIELD_HEIGHT_PX
	);
	drawHeap();
}

void
Field::drawHeap()
{
	// from bottom to top
	for (int y=FIELD_HEIGHT-1; y>=0; y--) {
		bool emptyRow = true;
		for (int x=0; x<FIELD_WIDTH; x++) {
			if (m_field[x][y] != BLANK) {
				m_painter->drawBlock(
					FIELD_X_PX + BLOCK_SIZE_PX * x,
					FIELD_Y_PX + BLOCK_SIZE_PX * y,
					BLOCK_SIZE_PX,
					BLOCK_SIZE_PX,
					0x66,
					0x66,
					0x66
				);
				emptyRow = false;
			}
		}
		// no reasons to continue if we met an empty row
		if (emptyRow) {
			break;
		}
	}
}

void
Field::drawPiece(Piece *piece, int posX, int posY)
{
	int x, y;
	piece->resetBlocksEnum();
	while (piece->iterateBlocksEnum(&x, &y)) {
		int blockX = posX + x - PIECE_SIZE/2;
		int blockY = posY + y - PIECE_SIZE/2;

		// siply don't draw everything behind the top edge
		if (blockY < 0) {
			continue;
		}

		m_painter->drawBlock(
			FIELD_X_PX + BLOCK_SIZE_PX * blockX,
			FIELD_Y_PX + BLOCK_SIZE_PX * blockY,
			BLOCK_SIZE_PX,
			BLOCK_SIZE_PX,
			piece->getColorRed(),
			piece->getColorGreen(),
			piece->getColorBlue()
		);
	}
}

Uint8
Field::checkCollisions(
	Piece *piece,
	int posX,
	int posY,
	Uint8 what
) {
	Uint8 result = 0;
	int x, y;

	piece->resetBlocksEnum();
	while (piece->iterateBlocksEnum(&x, &y)) {

		int blockX = posX + x - PIECE_SIZE/2;
		int blockY = posY + y - PIECE_SIZE/2;

		// check collisions

		if (
			what&COLLIDED_LEFT
			&& blockX < 0
		) {
			result |= COLLIDED_LEFT;
		}

		if (
			what&COLLIDED_RIGHT
			&& blockX >= FIELD_WIDTH
		) {
			result |= COLLIDED_RIGHT;
		}

		if (
			what&COLLIDED_TOP
			&& blockY < 0
		) {
			result |= COLLIDED_TOP;
		}

		// reached the bottom or the heap
		if (
			what&COLLIDED_HEAP
			&& (
				blockY >= FIELD_HEIGHT
				|| (
					/* keep it within the field */
					blockY >= 0 && blockX >= 0 && blockX < FIELD_WIDTH
					&& m_field[blockX][blockY] != BLANK
				)
			)
		) {
			result |= COLLIDED_HEAP;
		}
	}

	return result;
}

int
Field::clearRows()
{
	int clearedRows = 0;

	// from bottom to top
	for (int y=FIELD_HEIGHT-1; y>=0; y--) {
		bool completeRow = true;
		bool emptyRow = true;
		for (int x=0; x<FIELD_WIDTH; x++) {
			if (m_field[x][y] == BLANK) {
				completeRow = false;
			} else {
				emptyRow = false;
			}
		}
		if (completeRow) {
			clearedRows++;

			// todo: memcpy
			// shift down
			for (int j=y-1; j>=0; j--) {
				for (int x=0; x<FIELD_WIDTH; x++) {
					m_field[x][j+1] = m_field[x][j];
				}
			}
			// sempty first line
			for (int x=0; x<FIELD_WIDTH; x++) {
				m_field[x][0] = BLANK;
			}
			// check the shifted down row again
			// as it also may be complete
			y++;
		}
		// no reasons to continue if we met an empty row
		if (emptyRow) {
			break;
		}
	}

	return clearedRows;
}

void
Field::copyPieceToHeap(Piece *piece, int posX, int posY)
{
	int x, y;

	piece->resetBlocksEnum();
	while (piece->iterateBlocksEnum(&x, &y)) {

		int blockX = posX + x - PIECE_SIZE/2;
		int blockY = posY + y - PIECE_SIZE/2;

		if (
			blockX >= 0
			&& blockY >= 0
			&& blockX < FIELD_WIDTH
			&& blockY < FIELD_HEIGHT
		) {
			m_field[blockX][blockY] = piece->getPiece();
		}
	}
}

bool
Field::checkNotBlank(int x, int y)
{
	return (m_field[x][y] != BLANK);
}

