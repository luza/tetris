#include "Painter.h"
#include "pieces.h"

Painter::Painter(SDL_Renderer *renderer)
{
	m_renderer = renderer;
}

Painter::~Painter()
{
}

/**
 * Draw piece at absolute position
 */
void Painter::drawPieceAt(Piece *piece, int posX, int posY)
{
	int x, y;
	piece->resetBlocksEnum();
	while (piece->iterateBlocksEnum(&x, &y)) {
		drawBlock(
			posX + x * BLOCK_SIZE_PX,
			posY + y * BLOCK_SIZE_PX,
			BLOCK_SIZE_PX,
			BLOCK_SIZE_PX,
			piece->getColorRed(),
			piece->getColorGreen(),
			piece->getColorBlue()
		);
	}
}

void Painter::drawBlock(
	int x,
	int y,
	int width,
	int height,
	int red,
	int green,
	int blue
) {
	SDL_Rect rect;
	rect.x = x + 1;
	rect.y = y + 1;
	rect.w = width - 1;
	rect.h = height - 1;
	SDL_SetRenderDrawColor(m_renderer, red, green, blue, 0xFF);
	SDL_RenderFillRect(m_renderer, &rect);

	// brighter color
	SDL_SetRenderDrawColor(
		m_renderer,
		red * 13 / 10 < 255 ? red * 13 / 10 : 255,
		green * 13 / 10 < 255 ? green * 13 / 10 : 255,
		blue * 13 / 10 < 255 ? blue * 13 / 10 : 255,
		0xFF
	);

	// left and top edges
	SDL_RenderDrawLine(m_renderer, x, y, x, y + height - 1);
	SDL_RenderDrawLine(m_renderer, x, y, x + width - 1, y);

	// darker color
	SDL_SetRenderDrawColor(
		m_renderer,
		red * 7 / 10,
		green * 7 / 10,
		blue * 7 / 10,
		0xFF
	);

	// right and bottom edges
	SDL_RenderDrawLine(m_renderer, x + width - 1, y, x + width - 1, y + height - 1);
	SDL_RenderDrawLine(m_renderer, x, y + height - 1, x + width - 1, y + height - 1);
}

void Painter::drawField(int x, int y, int width, int height)
{
	SDL_Rect rect;
	SDL_SetRenderDrawColor(m_renderer, 0x80, 0x80, 0x80, 0xFF);
	for (int i=0; i<2; i++) {
		rect.x = x - (2 + i);
		rect.y = y - (2 + i);
		rect.w = width + (2 + i) * 2;
		rect.h = height + (2 + i) * 2;
		SDL_RenderDrawRect(m_renderer, &rect);
	}
}

void Painter::drawText(Text *text, int x, int y, int align)
{
	SDL_Rect destRect;
	SDL_Texture *texture = text->getTexture(m_renderer);

	SDL_QueryTexture(texture, NULL, NULL, &destRect.w, &destRect.h);

	if (align == TEXT_ALIGN_LEFT) {
		destRect.x = x;
	} else if (align == TEXT_ALIGN_RIGHT) {
		destRect.x = x - destRect.w;
	} else {
		destRect.x = x - destRect.w / 2;
	}

	destRect.y = y;

	SDL_RenderCopy(m_renderer, texture, NULL, &destRect);
}
