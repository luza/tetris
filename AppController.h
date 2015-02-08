#pragma once

#include "MenuScreen.h"
#include "Screen.h"

#define WINDOW_TITLE		"Tetris"
#define WINDOW_WIDTH_PX		640
#define WINDOW_HEIGHT_PX	480

#define ANIMATION_INTERVAL	50		// 50 ms
#define DEMO_INTERVAL		20*1000	// 20 sec

class AppController {
public:
	AppController();
	~AppController();
    int start();

private:
	void handleEvent(SDL_Event& e);
    void onKeyPressMenu(SDL_KeyboardEvent& keyEvent);
	void onKeyPressGame(SDL_KeyboardEvent& keyEvent);
	void pause();
	void startNewGame();
	void onTimer();

	enum {
		STATE_INCEPTION,
		STATE_DEMO,
		STATE_GAME,
		STATE_PAUSE
	};

	Uint32 m_tickInterval;
	Uint32 m_lastTick;
	Uint32 m_lastAnimationTick;
	Uint32 m_lastDemoWaitingTick;
	Uint32 m_lastDemoTick;

	int m_state;
	bool m_quit;
	int m_score;

	Singer m_singer;

	SDL_Window *m_window;
	SDL_Renderer *m_renderer;
	TTF_Font *m_font;
	Painter *m_painter;

	Screen *m_screen;
	MenuScreen *m_menuScreen;
};