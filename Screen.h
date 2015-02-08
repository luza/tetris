#pragma once

#include "Painter.h"
#include "Field.h"
#include "Text.h"
#include "Singer.h"
#include "Player.h"

#define LEVEL1_INTERVAL		750
#define SPEED_INCREASE_RATE	8 / 10
// 100, 300, 600, 1000
#define CLEARING_SCORE		200
#define SCORE_PER_LEVEL		10000

class Screen {
public:
	Screen(SDL_Renderer *renderer, Painter *painter, Singer *singer);
	~Screen();
	void draw();
	void onTimer(int *s, bool *gameOver);
	void movePieceLeft();
	void movePieceRight();
	void rotatePiece();
	void moveWisely();
	bool dropPiece();
	Uint32 getTimerInterval();
	void resetGame();

private:
	void createPiece();
	void drawTexts();
	void redrawTexts();

	SDL_Renderer *m_renderer;

	Field m_field;
	Text m_txScore, m_txLines, m_txLevel;
	Player m_player;

	Painter *m_painter;
	Singer *m_singer;
	Piece *m_piece, *m_nextPiece;

	int piecePosX;
	int piecePosY;

	int score;
	int linesCleared;
	int level;
};
