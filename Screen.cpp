#include <stdlib.h>     /* srand, rand */
#include <stdio.h>
#include "SDL.h"
#include "Screen.h"
#include "pieces.h"

Screen::Screen(
	SDL_Window *window,
	SDL_Renderer *renderer,
	Painter *painter
) {
	m_window = window;
	m_renderer = renderer;
	m_painter = painter;

	m_piece = NULL;
	m_nextPiece = NULL;

	SDL_Color grey = {0xA0, 0xA0, 0xA0, 0xFF};

	m_txScore.setSize(30);
	m_txLines.setSize(30);
	m_txLevel.setSize(30);
	m_txScore.setColor(grey);
	m_txLines.setColor(grey);
	m_txLevel.setColor(grey);

	m_field = new Field(m_painter);
}

Screen::~Screen()
{
	delete m_field;
}

void Screen::draw()
{
	//Clear screen
	SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(m_renderer);

	m_field->draw();
	m_field->drawPiece(m_piece, piecePosX, piecePosY);

	// draw next piece
	m_painter->drawPieceAt(m_nextPiece, 280, 200);

	drawTexts();	

	SDL_RenderPresent(m_renderer);
}

void Screen::drawTexts()
{
	m_painter->drawText(&m_txScore, 280, 15);
	m_painter->drawText(&m_txLines, 280, 65);
	m_painter->drawText(&m_txLevel, 280, 115);
}

void Screen::redrawTexts()
{
	char text[32];
	sprintf_s(text, sizeof(text), "Score        %06d", score);
	m_txScore.setText(text);

	sprintf_s(text, sizeof(text), "Lines        %06d", linesCleared);
	m_txLines.setText(text);

	sprintf_s(text, sizeof(text), "Level        %06d", level);
	m_txLevel.setText(text);
}

void Screen::resetGame()
{
	if (m_nextPiece) {
		delete m_nextPiece;
	}
	m_nextPiece = new Piece(rand() % MAX_PIECE, Piece::ROTATION_0);

	score = 0;
	linesCleared = 0;
	level = 1;
	redrawTexts();

	m_field->reset();

	createPiece();
}

void Screen::createPiece()
{
	if (m_piece) {
		delete m_piece;
	}
	m_piece = m_nextPiece;
	m_nextPiece = new Piece(rand() % MAX_PIECE, Piece::ROTATION_0);
	piecePosX = FIELD_WIDTH / 2;
	piecePosY = 0;
}

bool Screen::onTimer()
{
	Uint8 collisions = m_field->checkCollisions(
		m_piece,
		piecePosX,
		piecePosY+1,
		Field::COLLIDED_HEAP
	);
	if (collisions&Field::COLLIDED_HEAP) {
		// game over
		if (piecePosY <= 1) {
			return true;
		}

		m_field->copyPieceToHeap(m_piece, piecePosX, piecePosY);

		int clearedRows = m_field->clearRows();
		if (clearedRows > 0) {
			for (int i=1; i<=clearedRows; i++) {
				score += i * CLEARING_SCORE;
			}
			linesCleared += clearedRows;
			level = (score / SCORE_PER_LEVEL) + 1;
			redrawTexts();
		}

		createPiece();
	} else {
		piecePosY++;
	}
	draw();
	return false;
}

void Screen::movePieceLeft()
{
	Uint8 collisions = m_field->checkCollisions(
		m_piece,
		piecePosX-1,
		piecePosY,
		Field::COLLIDED_LEFT|Field::COLLIDED_HEAP
	);
	if (!collisions) {
		piecePosX--;
		draw();
	}
}

void Screen::movePieceRight()
{
	Uint8 collisions = m_field->checkCollisions(
		m_piece,
		piecePosX+1,
		piecePosY,
		Field::COLLIDED_RIGHT|Field::COLLIDED_HEAP
	);
	if (!collisions) {
		piecePosX++;
		draw();
	}
}

void Screen::rotatePiece()
{
	m_piece->rotate();

	// while rotating, piece may overlap something
	// if so, return it back to field
	Uint8 collisions = 0;
	do {
		collisions = m_field->checkCollisions(
			m_piece,
			piecePosX,
			piecePosY,
			Field::COLLIDED_LEFT|Field::COLLIDED_RIGHT|Field::COLLIDED_HEAP
		);

		if (collisions&Field::COLLIDED_LEFT) {
			piecePosX++;
		}

		if (collisions&Field::COLLIDED_RIGHT) {
			piecePosX--;
		}

		if (collisions&Field::COLLIDED_HEAP) {
			piecePosY--;
		}
	} while (collisions);

	draw();
}

bool Screen::dropPiece()
{
	Uint8 collisions = 0;
	bool dropped = false;
	for (;;) {
		collisions = m_field->checkCollisions(
			m_piece,
			piecePosX,
			piecePosY+1,
			Field::COLLIDED_HEAP
		);
		if (collisions) {
			break;
		}
		piecePosY++;
		dropped = true;
	}
	draw();
	return dropped;
}

Uint32 Screen::getTimerInterval()
{
	return LEVEL1_INTERVAL / (1 + (level - 1) * SPEED_INCREASE_RATE);
}
