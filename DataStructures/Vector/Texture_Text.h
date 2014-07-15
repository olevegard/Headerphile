// Texture_Text.h - headerphile.blogspot.no
// An overloaded Texture class that's used for rendeing text
// Stores fonts and colors for rendering
// Also stores previous rendered value which means we won't re-render the same value
#pragma once

#include <SDL2/SDL_ttf.h>

#include "Texture.h"

struct Texture_Text : Texture
{
	Texture_Text();
	Texture_Text( TTF_Font *font, const SDL_Color &bgColor, const SDL_Color &fgColor );

	void Init( TTF_Font *font, const SDL_Color &bgColor, const SDL_Color &fgColor );

	void RenderText_Solid( SDL_Renderer* renderer, const std::string &str );
	void RenderText_Shaded( SDL_Renderer* renderer, const std::string &str );
	void RenderText_Blended( SDL_Renderer* renderer, const std::string &str );

	// The two RenderValue uses RenderText_Solid() as this is more efficient than RenderText_Blanded.
	//	Feel free to change to Blanded if you want nicer text
	void RenderValue( SDL_Renderer* renderer, const std::string &str, int32_t value );
	void RenderValue( SDL_Renderer* renderer, const std::string &str, double value );

	virtual void RenderValue( SDL_Renderer* renderer, int32_t value );
	//void RenderValue( SDL_Renderer* renderer, double value );

	private:
	void ConvertSuface( SDL_Renderer* renderer, SDL_Surface* surface );

	private:
	TTF_Font* textFont;
	SDL_Color textColor;
	SDL_Color backgroundColor;
	double prevValue_d;
	int32_t prevValue_i;
};
