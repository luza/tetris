#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "SDL.h"
#include "SDL_ttf.h"
#include "AppController.h"

AppController::AppController()
{
	/* initialize random seed */
	srand((unsigned int)time(NULL));

	SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO|SDL_INIT_EVENTS);

	m_window = SDL_CreateWindow(
		WINDOW_TITLE,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH_PX,
		WINDOW_HEIGHT_PX,
		SDL_WINDOW_HIDDEN
	);

	TTF_Init();

	m_renderer = SDL_CreateRenderer(
		m_window,
		-1, /* use the first one supporting the requested flags */
		SDL_RENDERER_ACCELERATED
	);

	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

	SDL_ShowWindow(m_window);

	m_painter = new Painter(m_renderer);
	m_menuScreen = new MenuScreen(m_window, m_renderer, m_painter);
	m_screen = new Screen(m_window, m_renderer, m_painter);
	m_menuScreen->draw();

	m_paused = true;
	m_gameStarted = false;
	m_quit = false;

	m_tickInterval = m_screen->getTimerInterval();
}

AppController::~AppController()
{
	delete m_screen;
	delete m_painter;

	SDL_DestroyRenderer(m_renderer);

	TTF_Quit();

	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

int AppController::start()
{
	SDL_Event e;
	while (!m_quit) {
		int rv = SDL_WaitEventTimeout(&e, ANIMATION_INTERVAL);
		if (
			m_paused
			&& SDL_TICKS_PASSED(SDL_GetTicks(), m_lastAnimationTick + ANIMATION_INTERVAL)
		) {
			m_lastAnimationTick = SDL_GetTicks();
			m_menuScreen->animate();
		}
		if (
			!m_paused
			&& SDL_TICKS_PASSED(SDL_GetTicks(), m_lastTick + m_tickInterval)
		) {
			m_lastTick = SDL_GetTicks();
			if (m_screen->onTimer()) {
				m_paused = true;
				m_gameStarted = false;
				m_menuScreen->setTitle("Game Over");
				m_menuScreen->setMenuItemEnabled(MenuScreen::ACTION_CONTINUE, false);
				m_menuScreen->setMenuItemActive(MenuScreen::ACTION_START);
			}
			m_tickInterval = m_screen->getTimerInterval();
		}
		if (rv) {
			handleEvent(e);
		}
	}
	return 0;
}

void AppController::handleEvent(SDL_Event& e)
{
	switch (e.type) {
		case SDL_KEYDOWN:
			if (m_paused) {
				onKeyPressMenu(e.key);
			} else {
				onKeyPressGame(e.key);
			}
			break;

		case SDL_KEYUP:
			break;

		case SDL_QUIT:
			if (m_paused) {
				m_quit = true;
			} else {
				pause();
			}
			break;
	}
}

void AppController::onKeyPressMenu(SDL_KeyboardEvent& keyEvent)
{
	switch (keyEvent.keysym.sym) {
		case SDLK_ESCAPE:
			if (m_gameStarted) {
				pause();
			}
			break;

		case SDLK_UP:
			m_menuScreen->moveUp();
			break;

		case SDLK_DOWN:
			m_menuScreen->moveDown();
			break;

		case SDLK_RETURN:
			switch (m_menuScreen->enter()) {
				case MenuScreen::ACTION_START:
					m_screen->resetGame();
					m_gameStarted = true;
					pause();
					break;

				case MenuScreen::ACTION_CONTINUE:
					pause();
					break;

				case MenuScreen::ACTION_QUIT:
					m_quit = true;
					break;
			}
			break;
	}
}

void AppController::onKeyPressGame(SDL_KeyboardEvent& keyEvent)
{
	switch (keyEvent.keysym.sym) {
		case SDLK_ESCAPE:
			pause();
			break;

		case SDLK_UP:
			m_screen->rotatePiece();
			break;

		case SDLK_DOWN:
		case SDLK_SPACE:
			if (m_screen->dropPiece()) {
				// restart tick timer to give player
				// chance to move dropped piece
				m_lastTick = SDL_GetTicks();
			}
			break;

		case SDLK_LEFT:
			m_screen->movePieceLeft();
			break;

		case SDLK_RIGHT:
			m_screen->movePieceRight();
			break;
	}
}

void AppController::pause()
{
	m_paused = !m_paused;
	if (m_paused) {
		m_menuScreen->setTitle("Paused");
		m_menuScreen->setMenuItemEnabled(MenuScreen::ACTION_CONTINUE, true);
		m_menuScreen->setMenuItemActive(MenuScreen::ACTION_CONTINUE);
		m_menuScreen->draw();
	} else {
		m_screen->draw();
	}
}
