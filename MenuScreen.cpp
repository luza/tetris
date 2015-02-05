#include <stdlib.h>     /* srand, rand */
#include <list>
#include "AppController.h"
#include "MenuScreen.h"
#include "pieces.h"

MenuScreen::MenuScreen(SDL_Window *window, SDL_Renderer *renderer, Painter *painter)
	: m_txTitle("Tetris", 50, 0x00, 0x00, 0x00)
{
	m_window = window;
	m_renderer = renderer;
	m_painter = painter;

	m_background = SDL_CreateTexture(
		m_renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		WINDOW_WIDTH_PX,
		WINDOW_HEIGHT_PX
	);

	m_slidedX = 0;
	m_slidedY = 0;
	m_activeMenuItem = 0;

	generateBackground();

	prepareMenu();
}

MenuScreen::~MenuScreen()
{
}

void MenuScreen::prepareMenu()
{
	t_menuItem item1 = {
		ACTION_START,
		Text("Start New Game", 30, 0x00, 0x00, 0x00),
		true /* enabled */
	};
	m_menuList.push_back(item1);

	t_menuItem item2 = {
		ACTION_CONTINUE,
		Text("Continue", 30, 0x00, 0x00, 0x00),
		false /* disabled */
	};
	m_menuList.push_back(item2);

	t_menuItem item3 = {
		ACTION_QUIT,
		Text("Quit", 30, 0x00, 0x00, 0x00),
		true /* enabled */
	};
	m_menuList.push_back(item3);
}

void MenuScreen::generateBackground()
{
	SDL_SetRenderTarget(m_renderer, m_background);

	for (int y=0; y<4; y++) {
		for (int x=0; x<6; x++) {
			Piece piece(rand() % MAX_PIECE, Piece::ROTATION_0);
			m_painter->drawPieceAt(&piece, 30 + x * 105, 20 + y * 115);
		}
	}

	SDL_SetRenderTarget(m_renderer, NULL);
}

void MenuScreen::draw()
{
	drawBackground();

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 50;
	rect.w = WINDOW_WIDTH_PX;
	rect.h = 100;

	SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xC0);
	SDL_RenderFillRect(m_renderer, &rect);

	m_painter->drawText(&m_txTitle, WINDOW_WIDTH_PX/2, 70, Painter::TEXT_ALIGN_CENTER);

	for (Uint8 i=0,n=0; i<m_menuList.size(); i++) {
		// skip not enabled items
		if (!m_menuList[i].enabled) {
			continue;
		}

		rect.x = 0;
		rect.y = 200 + 70 * n;
		rect.w = WINDOW_WIDTH_PX;
		rect.h = 60;

		if (m_activeMenuItem == i) {
			SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0x88, 0xC0);
		} else {
			SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xC0);
		}

		SDL_RenderFillRect(m_renderer, &rect);

		m_painter->drawText(
			&m_menuList[i].text,
			WINDOW_WIDTH_PX/2,
			211 + 70 * n,
			Painter::TEXT_ALIGN_CENTER
		);

		n++;
	}

	SDL_RenderPresent(m_renderer);
}

void MenuScreen::drawBackground()
{
	SDL_Rect srect, trect;

	srect.x = 0;
	srect.y = 0;
	srect.w = WINDOW_WIDTH_PX - m_slidedX;
	srect.h = WINDOW_HEIGHT_PX;

	trect.x = m_slidedX;
	trect.y = 0;
	trect.w = WINDOW_WIDTH_PX - m_slidedX;
	trect.h = WINDOW_HEIGHT_PX;

	SDL_RenderCopy(m_renderer, m_background, &srect, &trect);

	srect.x = WINDOW_WIDTH_PX - m_slidedX;
	srect.y = 0;
	srect.w = m_slidedX;
	srect.h = WINDOW_HEIGHT_PX;

	trect.x = 0;
	trect.y = 0;
	trect.w = m_slidedX;
	trect.h = WINDOW_HEIGHT_PX;

	SDL_RenderCopy(m_renderer, m_background, &srect, &trect);
}

void MenuScreen::animate()
{
	m_slidedX++;
	if (m_slidedX >= WINDOW_WIDTH_PX) {
		m_slidedX = 0;
	}
	draw();
}

void MenuScreen::moveUp()
{
	for (int n=m_activeMenuItem-1; n>=0; n--) {
		if (m_menuList[n].enabled) {
			m_activeMenuItem = n;
			draw();
			break;
		}
	}
}

void MenuScreen::moveDown()
{
	for (Uint8 n=m_activeMenuItem+1; n<m_menuList.size(); n++) {
		if (m_menuList[n].enabled) {
			m_activeMenuItem = n;
			draw();
			break;
		}
	}
}

int MenuScreen::enter()
{
	return m_menuList[m_activeMenuItem].action;
}

void MenuScreen::setTitle(const char *title)
{
	m_txTitle.setText(title);
}

void MenuScreen::setMenuItemEnabled(int action, bool enabled) {
	for (Uint8 i=0; i<m_menuList.size(); i++) {
		if (m_menuList[i].action == action) {
			m_menuList[i].enabled = enabled;
			break;
		}
	}
}

void MenuScreen::setMenuItemActive(int action)
{
	for (Uint8 i=0; i<m_menuList.size(); i++) {
		if (m_menuList[i].action == action) {
			m_activeMenuItem = i;
			break;
		}
	}
}