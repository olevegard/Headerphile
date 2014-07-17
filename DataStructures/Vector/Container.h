#pragma once

#include <vector>
#include <SDL2/SDL_ttf.h>

#include <sstream>

#include <chrono>
#include <thread>

using namespace std::chrono;


#include "TextRect.h"		// TextTexture with a surrounding rect
#include "Texture.h"		// Regular textures ( Enemy, Player )
#include "Texture_Text.h"	// Text textures ( 'solid', 'blended', 'shaded' ), average FPS display

struct Container
{
	public:
		Container()
		:	size( 0 )
		,	capacity( 0 )
		,	sleepTime( 500 )
		{
		}
		~Container()
		{
			// Free our fonts
			TTF_CloseFont( font );
		}
		void Render( bool sleep = true )
		{
			SDL_RenderClear( renderer );

			SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
			SDL_RenderDrawRect( renderer, &container );
			SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );

			SDL_Rect rect = originalRect;

			for ( auto i = 0 ; i < data.size() ; ++i )
			{
				data[i].SetOuterRect( rect );
				data[i].Render( renderer );

				rect.x += itemSize.x + marginSize.x;
			}

			text.Render( renderer );
			SDL_RenderPresent( renderer);

			if ( sleep )
				std::this_thread::sleep_for( milliseconds( sleepTime ) );
		}
		bool Init( SDL_Renderer* renderer_, const std::string &fontName, int32_t fontSize )
		{
			renderer = renderer_;
			if ( !InitFonts( fontName, fontSize ) )
				return false;

			InitText();
			InitSize();

			Reserve( 5 );

			return true;
		}
		bool InitFonts( const std::string &fontName, int32_t fontSize )
		{
			font = TTF_OpenFont( fontName.c_str(), fontSize );
			smallFont = TTF_OpenFont( fontName.c_str(), 25 ); 

			if ( font == nullptr )
			{
				std::cout << " Failed to load font : " << SDL_GetError() << std::endl;
				return false;
			}

			return true;
		}
		void InitText()
		{
			text.Init( smallFont, { 255, 255, 0, 255 }, { 0, 0, 0, 255 } );
			text.SetRect( { container.x + 20, container.y + itemSize.y + 20, 100, 80 } );
			text.RenderText_Solid( renderer, "Text...." );
		}
		void InitSize()
		{
			itemSize.x = TTF_FontHeight( font ) + 10;
			itemSize.y = itemSize.x;

			marginSize = { 15, 15 };

			container.w = 5 * itemSize.x + 5 * marginSize.x;
			container.h = itemSize.y + ( marginSize.y * 2 ) + 50;

			originalRect = { container.x + marginSize.x, container.y + marginSize.y, itemSize.x, itemSize.y };

		}
		void AddObject( int32_t value )
		{
			AddObject( value, size  );
		}
		// Adds object in a given position
		void AddObject( int32_t value, int32_t index )
		{
			RenderInsertionText( value, index );

			// Chech if we need to expand vector
			if ( size >= capacity )
				Reserve( capacity + 5 );

			// Start in the back and move every item one space back
			// Stops when 'index' has been moved
			for ( auto i = size - 1; i >= index ; --i )
			{
				std::cout << "Swapping\n";
				std::swap( data[i], data[ i + 1 ] );
				Render();
			}

			data[ index ].RenderValue( renderer, value );
			text.RenderText_Solid( renderer, "Done!" );
			Render();
			++size;
		}
		void RenderInsertionText( int32_t value, int32_t index )
		{
			if ( index != size )
			{
				std::stringstream ss;
				ss << "Inserting : " << value << " into index " << index;
				text.RenderText_Solid( renderer, ss.str() );
			}
			else
				text.RenderValue( renderer, "Pushing back : ", value );

			Render();
		}
		void Reserve( int32_t newCapacity )
		{
			if ( capacity >= newCapacity )
			{
				std::cout << "New capacity is lower than current capacity\n";
				return;
			}

			// Reserve capacity
			// Documentation : http://en.cppreference.com/w/cpp/container/vector/reserve
			data.reserve( newCapacity );
			text.RenderText_Solid( renderer, "Resserving capacity" );
			Render();

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
			TextRect item = CreateEmptyItem();
			item.RenderValue( renderer, value );

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
		TTF_Font* smallFont;
		SDL_Point itemSize = { 66, 66 }; 
		SDL_Point marginSize = { 5, 5 }; 
		SDL_Rect container = { 10, 10, 15, 0 };
		SDL_Rect originalRect;
		Texture_Text text;
		int32_t sleepTime; // Raise after every render
};
