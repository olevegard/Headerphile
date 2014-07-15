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
		bool Init( const std::string &fontName, int32_t fontSize )
		{
			font = TTF_OpenFont( fontName.c_str(), fontSize );

			if ( font == nullptr )
			{
				std::cout << " Failed to load font : " << SDL_GetError() << std::endl;
				return false;
			}

			container.h = TTF_FontHeight( font ) + 15;

			return true;
		}
		void AddObject( SDL_Renderer* renderer, int32_t value )
		{
			TextRect item;
			SDL_Rect r( { container.x + container.w, container.y + 5, 0, 0 } );

			item.Init( font, { 255, 255, 0, 255 }, { 0, 0, 0, 255 } ); 
			item.RenderValue( renderer, value );
			item.SetOuterRect( r );

			items.push_back( item );
			container.w += item.GetOuterRect().w + 10;
		}
	private:
		TTF_Font* font;
		std::vector< TextRect > items;
		SDL_Point itemSize = { 66, 66 }; 
		SDL_Rect container = { 10, 10, 10, 85 };
};
