#pragma once

#include <vector>
#include "SDL.h"
#include "Painter.h"
#include "Text.h"

class MenuScreen {
public:
	MenuScreen(SDL_Window *window, SDL_Renderer *renderer, Painter *painter);
	~MenuScreen();
	void draw();
	void animate();
	void moveUp();
	void moveDown();
	int enter();
	void setTitle(const char *title);
	void setMenuItemEnabled(int action, bool active);
	void setMenuItemActive(int action);

	enum {
		ACTION_START,
		ACTION_CONTINUE,
		ACTION_QUIT
	};

private:
	void generateBackground();
	void drawBackground();
	void prepareMenu();

	SDL_Window *m_window;
	SDL_Renderer *m_renderer;
	SDL_Texture *m_background;
	Painter *m_painter;

	Text m_txTitle;

	struct t_menuItem {
		int action;
		Text text;
		bool enabled;
	};

	std::vector<t_menuItem> m_menuList;

	int m_slidedX, m_slidedY;
	Uint8 m_activeMenuItem;
};