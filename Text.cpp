#include <stdarg.h>
#include "Text.h"

Text::Text()
{
	m_texture = NULL;
}

Text::Text(const char *text, int size, SDL_Color color)
{
	m_texture = NULL;

	m_text = text;
	m_size = size;
	m_color = color;
}

Text::Text(
	const char *text,
	int size,
	Uint8 red,
	Uint8 green,
	Uint8 blue,
	Uint8 alpha
) {
	SDL_Color color = { red, green, blue, alpha };

	m_texture = NULL;

	m_text = text;
	m_size = size;
	m_color = color;
}

Text::~Text()
{
	if (m_texture) {
		SDL_DestroyTexture(m_texture);
	}
}

void Text::setSize(int size)
{
	m_size = size;
	reset();
}

void Text::setColor(SDL_Color color)
{
	m_color = color;
	reset();
}

void Text::setColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
{
	m_color.r = red;
	m_color.g = green;
	m_color.b = blue;
	m_color.a = alpha;
	reset();
}

void Text::setText(const char *text)
{
	m_text = text;
	reset();
}

void Text::reset()
{
	// reset previous calculations
	if (m_texture) {
		SDL_DestroyTexture(m_texture);
		m_texture = NULL;
	}
}

SDL_Texture *Text::getTexture(SDL_Renderer *renderer)
{
	if (m_texture != NULL) {
		return m_texture;
	}

	TTF_Font *font = TTF_OpenFont("nk57-monospace-sc-rg.ttf", m_size);
	SDL_Surface *textSurface = TTF_RenderText_Blended(font, m_text.c_str(), m_color);
	TTF_CloseFont(font);

	m_texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);

	return m_texture;
}