#pragma once

#include "MenuScreen.h"
#include "Screen.h"

#define WINDOW_TITLE		"Tetris"
#define WINDOW_WIDTH_PX		640
#define WINDOW_HEIGHT_PX	480

#define ANIMATION_INTERVAL	50

class AppController {
public:
	AppController();
	~AppController();
    int start();

private:
	void handleEvent(SDL_Event& e);
	void onTick();
    void onKeyPressMenu(SDL_KeyboardEvent& keyEvent);
	void onKeyPressGame(SDL_KeyboardEvent& keyEvent);
	void pause();

	Uint32 m_tickInterval;
	Uint32 m_lastTick;
	Uint32 m_lastAnimationTick;
	bool m_paused;
	bool m_gameStarted;
	bool m_quit;

	SDL_Window *m_window;
	SDL_Renderer *m_renderer;
	TTF_Font *m_font;
	Painter *m_painter;

	Screen *m_screen;
	MenuScreen *m_menuScreen;
};