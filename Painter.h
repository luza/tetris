#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
#include "Piece.h"
#include "Text.h"

#define BLOCK_SIZE_PX        23 // pixels

class Painter
{
public:
    Painter(SDL_Renderer *renderer);
    ~Painter(void);
    void drawPieceAt(Piece *piece, int posX, int posY);
    void drawBlock(int x, int y, int width, int height, int red, int green, int blue);
    void drawField(int x, int y, int width, int height);
    void prepareText(SDL_Texture **texture, const char *text, int size, Uint8 red, Uint8 green, Uint8 blue);
    void drawText(Text *text, int x, int y, int align = TEXT_ALIGN_LEFT);

    enum {
        TEXT_ALIGN_LEFT,
        TEXT_ALIGN_CENTER,
        TEXT_ALIGN_RIGHT
    };

private:
    SDL_Renderer *m_renderer;
};
