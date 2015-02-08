#pragma once

#include <string>
#include "SDL.h"
#include "SDL_ttf.h"

class Text
{
public:
	Text();
	Text(const char *text, int size, SDL_Color color);
	Text(const char *text, int size, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 0xFF);
	~Text();
	void setSize(int size);
	void setColor(SDL_Color color);
	void setColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 0xFF);
	void setText(const char *text);
	SDL_Texture *getTexture(SDL_Renderer *renderer);

private:
	void init();
	void reset();

	std::string m_text;
	int m_size;
	SDL_Color m_color;
	SDL_Texture *m_texture;

	static SDL_Surface *m_font;
};
