#pragma once

#include <vector>
#include <SDL2/SDL_ttf.h>

#include "TextRect.h"		// TextTexture with a surrounding rect
#include "Texture.h"		// Regular textures ( Enemy, Player )
#include "Texture_Text.h"	// Text textures ( 'solid', 'blended', 'shaded' ), average FPS display

struct Container
{
	public:
		Container()
		:	size( 0 )
		,	capacity( 0 )
		{
		}
		~Container()
		{
			// Free our fonts
			TTF_CloseFont( font );
		}
		//void Render( SDL_Renderer* renderer )
		void Render( )
		{
			SDL_RenderClear( renderer );

			SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
			SDL_RenderDrawRect( renderer, &container );
			SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );

			SDL_Rect rect = data[0].GetOuterRect();

			data[0].Render( renderer );
			for ( auto i = 1 ; i < data.size() ; ++i )
			{
				rect.x += itemSize.x + marginSize.x;
				data[i].SetOuterRect( rect );
				data[i].Render( renderer );
			}

			SDL_RenderPresent( renderer);
			std::cin.ignore();
		}
		bool Init( SDL_Renderer* renderer_, const std::string &fontName, int32_t fontSize )
		{
			renderer = renderer_;
			font = TTF_OpenFont( fontName.c_str(), fontSize );

			if ( font == nullptr )
			{
				std::cout << " Failed to load font : " << SDL_GetError() << std::endl;
				return false;
			}

			itemSize.x = TTF_FontHeight( font ) + 10;
			itemSize.y = itemSize.x;

			marginSize = { 15, 15 };

			container.h = itemSize.y + ( marginSize.y * 2 );

			originalRect = { container.x + marginSize.x, container.y + marginSize.y, itemSize.x, itemSize.y };

			Reserve( 5 );
			return true;
		}
		void AddObject( int32_t value )
		{
			// Chech if we need to expand vector
			if ( size >= capacity )
				Reserve( capacity + 5 );

			// Add item to last free pos ( this is the same as push_back )
			data[ size++ ].RenderValue( renderer, value );
		}
		void AddObject( int32_t value, int32_t index )
		{
			// Chech if we need to expand vector
			if ( size >= capacity )
				Reserve( capacity + 5 );

			//data.insert( std::begin( data) + index, CreateItem( renderer, value ) );
			std::cout << "Setting item " << size << " data size " << data.size() << std::endl;
			data[size].RenderValue( renderer, value );
		}
		void Reserve( int32_t newCapacity )
		{
			data.reserve( newCapacity );

			if ( capacity >= newCapacity )
			{
				std::cout << "New capacity is lower than current capacity\n";
				return;
			}
			// Reserve capacity
			// Documentation : http://en.cppreference.com/w/cpp/container/vector/reserve
			int32_t capacityIncrease = newCapacity - capacity;
			int32_t addedItems = 0;

			capacity = newCapacity;

			// Now we have reserved the capacity, but we need to add the empty elements
			while ( addedItems < capacityIncrease )
			{
				data.push_back( CreateEmptyItem() );
				++addedItems;
			}

			Render();
		}
	private:
		TextRect CreateItem( int32_t value )
		{
			TextRect item;

			item.Init( font, { 255, 255, 0, 255 }, { 0, 0, 0, 255 } ); 
			item.RenderValue( renderer, value );
			item.SetOuterRect( originalRect );

			container.w += marginSize.x + itemSize.x;


			return item;
		}
		TextRect CreateEmptyItem( )
		{
			TextRect item;

			item.Init( font, { 255, 255, 0, 255 }, { 0, 0, 0, 255 } ); 
			item.SetOuterRect( originalRect );

			container.w += marginSize.x + itemSize.x;

			return item;
		}
		// Vector
		// ======================================================
		int32_t size;
		int32_t capacity;

		std::vector< TextRect > data;

		// Rendering
		// ======================================================
		SDL_Renderer* renderer;
		TTF_Font* font;
		SDL_Point itemSize = { 66, 66 }; 
		SDL_Point marginSize = { 5, 5 }; 
		SDL_Rect container = { 10, 10, 15, 0 };
		SDL_Rect originalRect;
};
