// Texture.h - headerphile.blogspot.no
// A bare-bones texture class.
// Contains texture, rect for texture, angle of rotation and center of rotation

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <string>
#include <iostream>
#include <sstream>

#include "Texture.h"

struct Speed
{
	double x;
	double y;
};

struct Texture
{
	Texture();

	// Initializes a texture, including size and position
	void LoadTexture( SDL_Renderer *renderer, const std::string &str );

	void Render( SDL_Renderer* renderer );
	void Update( double delta );

	// Just a wrapper to make our lives easier 
	bool CheckCollision( const Texture &other ) const;

	// This method is the same as in part 7
	bool CheckCollision( const SDL_Rect &other ) const;

	bool IsOutOfBounds( const SDL_Rect &windowSize ) const;

	void SetRect( SDL_Rect r );
	void SetPos( SDL_Point p );
	void SetSpeed( Speed s );
	void SetTexture( SDL_Texture *text )
	{
		texture = text;
		CalculateSize( );
	}
	void CenterAtPoint( const SDL_Point &p )
	{
		rect.x = p.x - ( rect.w * 0.5 );
		rect.y = p.y - ( rect.h * 0.5 );
	}

	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();

	SDL_Rect GetRect() const;
	Speed GetSpeed( ) const;

	private:
	void CalculateSize( );
	void CalculateCenter( );

	// Position
	double x;
	double y;
	SDL_Rect rect;

	SDL_Texture* texture;

	// Movement
	Speed speed;

	// Rotation
	double angle;
	SDL_Point center;
};

struct FontTexture : Texture
{
	FontTexture()
	{
	}
	FontTexture( TTF_Font *font, const SDL_Color &bgColor, const SDL_Color &fgColor )
		:	Texture()
		,	textFont( font )
		,	textColor( { fgColor } )
		,	backgroundColor( { bgColor } )
	{
	}
	void Init( TTF_Font *font, const SDL_Color &bgColor, const SDL_Color &fgColor )
	{
		textFont = font;
		textColor = bgColor;
		backgroundColor = fgColor;
	}

	void RenderText_Solid( SDL_Renderer* renderer, const std::string &str )
	{
		// Just render the text ( background will be transparent )
		SDL_Surface* surface = TTF_RenderText_Solid( textFont, str.c_str(), textColor );

		ConvertSuface( renderer, surface );
	}
	void RenderText_Shaded( SDL_Renderer* renderer, const std::string &str )
	{
		// Render with a text color
		SDL_Surface* surface = TTF_RenderText_Shaded( textFont, str.c_str(), textColor, backgroundColor );

		ConvertSuface( renderer, surface );
	}
	void RenderText_Blended( SDL_Renderer* renderer, const std::string &str )
	{
		// Render with a text color
		SDL_Surface* surface = TTF_RenderText_Blended( textFont, str.c_str(), textColor );

		ConvertSuface( renderer, surface );
	}
	void RenderValue( SDL_Renderer* renderer, const std::string &str, int32_t value )
	{
		std::stringstream  ss;
		ss << str << " : " << value;
		RenderText_Solid( renderer, ss.str() );
	}
	private:
	void ConvertSuface( SDL_Renderer* renderer, SDL_Surface* surface )
	{
		if ( surface == nullptr )
		{
			std::cout << "Text rendering failed!\n";
			std::cout << "\tFont is  : " << textFont << std::endl;
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, surface );

		SetTexture( texture );
		SDL_FreeSurface( surface );
	}

	private:
	TTF_Font* textFont;
	SDL_Color textColor;
	SDL_Color backgroundColor;
};
