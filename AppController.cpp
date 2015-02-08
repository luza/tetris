#include "SDL.h"
#include "SDL_ttf.h"
#include "AppController.h"
#include "Singer.h"

AppController::AppController()
{
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

	m_painter = new Painter(m_renderer);
	m_screen = new Screen(m_renderer, m_painter, &m_singer);

	SDL_ShowWindow(m_window);

	m_menuScreen = new MenuScreen(m_renderer, m_painter, &m_singer);
	m_menuScreen->draw();

	m_state = STATE_INCEPTION;

	m_tickInterval = m_screen->getTimerInterval();

	m_lastAnimationTick = 0;
	m_lastTick = 0;
	m_lastDemoWaitingTick = 0;
	m_lastDemoTick = 0;

	m_quit = false;
	m_score = 0;
}

AppController::~AppController()
{
	delete m_menuScreen;
	delete m_screen;
	delete m_painter;

	SDL_DestroyRenderer(m_renderer);

	TTF_Quit();

	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

/**
 * Main loop
 */
int
AppController::start()
{
	SDL_Event e;
	while (!m_quit) {
		int rv = SDL_WaitEventTimeout(&e, ANIMATION_INTERVAL);
		Uint32 ticks = SDL_GetTicks();
		if (
			m_state == STATE_INCEPTION
			|| m_state == STATE_PAUSE
		) {
			if (SDL_TICKS_PASSED(ticks, m_lastAnimationTick + ANIMATION_INTERVAL)) {
				m_lastAnimationTick = ticks;
				m_menuScreen->animate();
			}
			if (
				m_state == STATE_INCEPTION
				&& SDL_TICKS_PASSED(ticks, m_lastDemoWaitingTick + DEMO_INTERVAL)
			) {
				m_singer.setMute(true);
				m_state = STATE_DEMO;
				m_screen->resetGame();
				m_screen->draw();
			}
		}
		if (
			(m_state == STATE_GAME || m_state == STATE_DEMO)
			&& SDL_TICKS_PASSED(ticks, m_lastTick + m_tickInterval)
		) {
			m_lastTick = ticks;
			onTimer();
		}
		if (
			m_state == STATE_DEMO
			&& SDL_TICKS_PASSED(ticks, m_lastDemoTick + m_tickInterval/2)
		) {
			m_lastDemoTick = ticks;
			m_screen->moveWisely();
		}
		if (rv) {
			handleEvent(e);
		}
	}
	return 0;
}

void
AppController::handleEvent(SDL_Event& e)
{
	switch (e.type) {
		case SDL_KEYDOWN:
			// reset demo waiting time on any key press
			m_lastDemoWaitingTick = SDL_GetTicks();
			if (m_state == STATE_DEMO) {
				m_singer.setMute(false);
				m_state = STATE_INCEPTION;
				m_menuScreen->draw();
			} else if (m_state == STATE_GAME) {
				onKeyPressGame(e.key);
			} else {
				onKeyPressMenu(e.key);
			}
			break;

		case SDL_KEYUP:
			break;

		case SDL_QUIT:
			if (m_state == STATE_GAME) {
				pause();
			} else {
				m_quit = true;
			}
			break;
	}
}

void
AppController::onKeyPressMenu(SDL_KeyboardEvent& keyEvent)
{
	switch (keyEvent.keysym.sym) {
		case SDLK_ESCAPE:
			pause();
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
					startNewGame();
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

void
AppController::onKeyPressGame(SDL_KeyboardEvent& keyEvent)
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

		//case SDLK_F1:
		//	m_screen->moveWisely();
		//	break;
	}
}

void
AppController::onTimer()
{
	bool gameOver = false;
	m_screen->onTimer(&m_score, &gameOver);
	if (gameOver) {
		gameOver();
	}
	m_tickInterval = m_screen->getTimerInterval();
}

void
AppController::startNewGame()
{
	m_state = STATE_GAME;
	m_screen->resetGame();
	m_score = 0;
	m_screen->draw();
}

void
AppController::pause()
{
	if (m_state == STATE_GAME) {
		m_state = STATE_PAUSE;
		m_menuScreen->setTitle("Paused");
		m_menuScreen->setScore(m_score);
		m_menuScreen->setMenuItemEnabled(MenuScreen::ACTION_CONTINUE, true);
		m_menuScreen->setMenuItemActive(MenuScreen::ACTION_CONTINUE);
		m_menuScreen->draw();
	} else if (m_state == STATE_PAUSE) {
		m_state = STATE_GAME;
		m_screen->draw();
	}
}

void
AppController::gameOver()
{
	if (m_state != STATE_DEMO) {
		m_menuScreen->setTitle("Game Over");
		m_menuScreen->setScore(m_score);
		m_menuScreen->setMenuItemEnabled(MenuScreen::ACTION_CONTINUE, false);
		m_menuScreen->setMenuItemActive(MenuScreen::ACTION_START);
	}

	m_singer.setMute(false);
	m_lastDemoWaitingTick = SDL_GetTicks();
	m_state = STATE_INCEPTION;
}