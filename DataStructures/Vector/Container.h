// Container - headerphile.blogspot.no
// A simple class to show how a vector works
// Note : the code is messy because this is just inteded to show how a vector works
// 			Code also contains a lot of Render() calls, this is because we want to flush the status text quite often
#pragma once

#include <SDL2/SDL_ttf.h>

#include <vector>

#include "TextRect.h"		// TextTexture with a surrounding rect
#include "Texture.h"		// Regular textures ( Enemy, Player )
#include "Texture_Text.h"	// Text textures ( 'solid', 'blended', 'shaded' ), average FPS display

struct Container
{
	public:
		Container();
		~Container();
		void Render( bool sleep = true );
		bool Init( SDL_Renderer* renderer_, const std::string &fontName, int32_t fontSize );

		// Adds an object to the back
		void AddObject( int32_t value );

		// Adds object in a given position
		void AddObject( int32_t value, int32_t index );

		// Increase capacity of vector ( will add empty items )
		void Reserve( int32_t newCapacity );
	private:
		void RenderInsertionText( int32_t value, int32_t index );
		bool InitFonts( const std::string &fontName, int32_t fontSize );
		void InitText();
		void InitSize();
		TextRect CreateItem( int32_t value );
		TextRect CreateEmptyItem( );

		// Vector
		// ======================================================
		int32_t size;
		int32_t capacity;

		std::vector< TextRect > data;

		// Rendering
		// ======================================================
		SDL_Renderer* renderer;
		TTF_Font* font;
		TTF_Font* smallFont;

		SDL_Point itemSize = { 66, 66 }; 
		SDL_Point marginSize = { 5, 5 }; 
		SDL_Rect container = { 10, 10, 15, 0 };
		SDL_Rect originalRect;

		Texture_Text legendSizeBar;
		SDL_Color colorSizeBar;
		Texture_Text legendCapacityBar;
		SDL_Color colorCapacity;

		Texture_Text text;
		int32_t sleepTime; // Raise after every render
};
