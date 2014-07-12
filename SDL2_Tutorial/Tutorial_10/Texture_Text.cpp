#include "Texture_Text.h"

Texture_Text::Texture_Text()
	:	Texture_Text( nullptr, { 0, 0, 0, 255 }, { 255, 255, 255, 255 } )
{
}

Texture_Text::Texture_Text( TTF_Font *font, const SDL_Color &bgColor, const SDL_Color &fgColor )
	:	Texture()
	,	textFont( font )
	,	textColor( { fgColor } )
	,	backgroundColor( { bgColor } )
	,	prevValue_d( -1.0 )
	,	prevValue_i( -1 )
{
}
void Texture_Text::Init( TTF_Font *font, const SDL_Color &bgColor, const SDL_Color &fgColor )
{
	textFont = font;
	textColor = bgColor;
	backgroundColor = fgColor;
}
void Texture_Text::RenderText_Solid( SDL_Renderer* renderer, const std::string &str )
{
	// Just render the text ( background will be transparent )
	SDL_Surface* surface = TTF_RenderText_Solid( textFont, str.c_str(), textColor );

	SetTexture( renderer, surface );
}
void Texture_Text::RenderText_Shaded( SDL_Renderer* renderer, const std::string &str )
{
	// Render with a text color
	SDL_Surface* surface = TTF_RenderText_Shaded( textFont, str.c_str(), textColor, backgroundColor );

	SetTexture( renderer, surface );
}
void Texture_Text::RenderText_Blended( SDL_Renderer* renderer, const std::string &str )
{
	// Render with a text color, nicer than TTF_RenderText_Shaded, but slower
	SDL_Surface* surface = TTF_RenderText_Blended( textFont, str.c_str(), textColor );

	SetTexture( renderer, surface );
}
// The two RenderValue uses RenderText_Solid() as this is more efficient than RenderText_Blanded.
//	Feel free to change to Blanded if you want nicer text
void Texture_Text::RenderValue( SDL_Renderer* renderer, const std::string &str, int32_t value )
{
	// Don't re-render the same value
	if ( prevValue_i == value )
		return;

	prevValue_d = -1.0;
	prevValue_i = value;

	std::stringstream  ss;
	ss << str << " : " << value;
	RenderText_Solid( renderer, ss.str() );
}
void Texture_Text::RenderValue( SDL_Renderer* renderer, const std::string &str, double value )
{
	// Don't re-render the same value
	if ( prevValue_d == value )
		return;

	prevValue_i = -1;
	prevValue_d = value;

	std::stringstream  ss;
	ss << str << " : " << value;
	RenderText_Solid( renderer, ss.str() );
}
