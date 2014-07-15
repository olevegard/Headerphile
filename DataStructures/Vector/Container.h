#pragma once

#include <vector>
#include <SDL2/SDL_ttf.h>

#include "TextRect.h"		// TextTexture with a surrounding rect
#include "Texture.h"		// Regular textures ( Enemy, Player )
#include "Texture_Text.h"	// Text textures ( 'solid', 'blended', 'shaded' ), average FPS display

struct Container
{
	public:
		~Container()
		{
			// Free our fonts
			TTF_CloseFont( font );
			TTF_CloseFont( bigFont );
		}
		void Render( SDL_Renderer* renderer )
		{
			SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
			SDL_RenderDrawRect( renderer, &container );
			SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );

			for ( const auto &text : items )
			{
				text.Render( renderer );
			
			}
		}
		bool Init( const std::string &fontName )
		{
			font = TTF_OpenFont( fontName.c_str(), 38 );
			bigFont = TTF_OpenFont( fontName.c_str(), 90 );

			if ( font == nullptr )
			{
				std::cout << " Failed to load font : " << SDL_GetError() << std::endl;
				return false;
			}

			return true;
		}
		void AddObject( SDL_Renderer* renderer, int32_t value )
		{
			TextRect item;
			SDL_Rect r( { container.x + container.w, container.y + 10, 20, 20 } );

			item.Init( font, { 255, 255, 0, 255 }, { 0, 0, 0, 255 } ); 
			item.RenderValue( renderer, value );
			item.SetOuterRect( r );

			items.push_back( item );
			container.w += item.GetOuterRect().w + 10;
		}
	private:
		TTF_Font* font;
		TTF_Font* bigFont;
		std::vector< TextRect > items;
		SDL_Point itemSize = { 66, 66 }; 
		SDL_Rect container = { 10, 10, 10, 85 };
};
