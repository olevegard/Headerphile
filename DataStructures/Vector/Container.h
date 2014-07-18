// Container - headerphile.blogspot.no
// A simple class to show how a vector works
// Note : the code is messy because this is just inteded to show how a vector works
//		Code also contains a lot of Render() calls,
//		this is because we want to flush the status text quite often( like when moving every item backwards
//		like in void MoveElementsBack( int32_t indexStart, int32_t indexStop );
#pragma once

#include <SDL2/SDL_ttf.h>

#include <vector>

#include "TextRect.h"
#include "Texture_Text.h"

struct Container
{
	public:
		Container();
		~Container();

		// Note: no SDL_Renderer, uses SDL_Rendere* member
		// This is because we need to render inside loops to show step by how insertion and deletion works
		void Render( bool sleep = true );
		void RenderMarkers( const SDL_Point &sizePoint, const SDL_Point &capacityPoint, const SDL_Point &backPoint );
		void RenderStatus( );

		// Initializes everything. renderer_ is used to set the member variable.
		bool Init( SDL_Renderer* renderer_, const std::string &fontName, int32_t fontSize );

		// Adds an object to the back
		void AddObject( int32_t value );

		// Adds object in a given position
		void AddObject( int32_t value, int32_t index );

		// Increase capacity of vector ( will add empty items )
		void Reserve( int32_t newCapacity, bool renderText = true );
	private:
		// Calls CreateEmptyItem() and initializes the text before returning it
		TextRect CreateItem( int32_t value );

		// Creates a new item and sets postiion. 
		TextRect CreateEmptyItem( );

		// Move all elements back ( as when inserting an element in the iddle of the vector )
		void MoveElementsBack( int32_t indexStart, int32_t indexStop );

		// Check that there is free capacity to insert item
		void CheckCapacity( );

		// Writes either "Adding x in pos y" or "Pushing back " depending on the situatuion 
		void RenderInsertionText( int32_t value, int32_t index );

		// Loads and checks fonts
		bool InitFonts( const std::string &fontName, int32_t fontSize );
		void InitText();
		void InitSize();

		// Vector
		// ======================================================
		// Number of elements in the vecor ( = first free position )
		int32_t size;

		// Number of unused elements ( empty squares )
		int32_t capacity;

		// Our data objects
		std::vector< TextRect > data;

		// Rendering
		// ======================================================
		SDL_Renderer* renderer;
		TTF_Font* font;
		TTF_Font* smallFont;
		TTF_Font* tinyFont;

		// Positions
		// ======================================================
		// These variables contains some startd offsets and positions
		// Please not that a lot of positions are hard-coded
		// So changing any of the below might result in unexpected behaviour
		SDL_Point itemSize = { 66, 66 }; 
		SDL_Point marginSize = { 5, 5 }; 
		SDL_Rect container = { 10, 10, 15, 0 };
		SDL_Rect originalRect;

		// Status text
		// ======================================================
		// The main function, like inserting 42 into index 3
		Texture_Text caption;
		SDL_Color captionColor{ 255, 255, 0, 255 };

		// The step along the way, like pushing items back
		Texture_Text subText;
		SDL_Color subTextColor{ 0, 255, 255, 255 };


		Texture_Text backMarker;
		Texture_Text sizeMarker;
		Texture_Text capacityMarker;

		// Sleep time after every render pass( argument is true )
		int32_t sleepTime; 
};
