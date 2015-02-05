#pragma once

#include "Painter.h"
#include "Field.h"
#include "Text.h"

#define LEVEL1_INTERVAL		750
#define SPEED_INCREASE_RATE	8 / 10
// 100, 300, 600, 1000
#define CLEARING_SCORE		200
#define SCORE_PER_LEVEL		10000

class Screen {
public:
	Screen(SDL_Window *window, SDL_Renderer *renderer, Painter *painter);
	~Screen();
	void draw();
	bool onTimer();
	void movePieceLeft();
	void movePieceRight();
	void rotatePiece();
	bool dropPiece();
	Uint32 getTimerInterval();
	void resetGame();

private:
	void createPiece();
	void drawTexts();
	void redrawTexts();

	SDL_Window *m_window;
	SDL_Renderer *m_renderer;

	Text m_txScore, m_txLines, m_txLevel;

	Painter *m_painter;
	Piece *m_piece, *m_nextPiece;
	Field *m_field;

	int piecePosX;
	int piecePosY;

	int score;
	int linesCleared;
	int level;
};
