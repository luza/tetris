#pragma once

#include <vector>
#include "SDL.h"
#include "Painter.h"
#include "Text.h"
#include "Singer.h"

class MenuScreen {
public:
    MenuScreen(SDL_Renderer *renderer, Painter *painter, Singer *singer);
    ~MenuScreen();
    void draw();
    void animate();
    void moveUp();
    void moveDown();
    int enter();
    void setTitle(const char *title);
    void setMenuItemEnabled(int action, bool active);
    void setMenuItemActive(int action);
    void setScore(int score);

    enum {
        ACTION_START,
        ACTION_CONTINUE,
        ACTION_QUIT
    };

private:
    void generateBackground();
    void drawBackground();
    void prepareMenu();

    SDL_Renderer *m_renderer;
    SDL_Texture *m_background;
    Painter *m_painter;
    Singer *m_singer;

    Text m_txTitle, m_txScore;

    struct t_menuItem {
        int action;
        Text text;
        bool enabled;
    };

    std::vector<t_menuItem> m_menuList;

    int m_slidedX;
    Uint8 m_activeMenuItem;
};