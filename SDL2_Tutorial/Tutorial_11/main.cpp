// SLD2 Tutorial Part 11 - Headerphile.blogspot.no
// Tests the Font.h class
// Compile  : clang++ main.cpp Texture.cpp Teure_Text.cpp Timer.cpp -std=c++1y -lSDL2 -lSDL2_image -lSDL2_ttf -o Game
// Run      : ./Game
// Updated  : Texture.h/.cpp, Texture_Text.h/.cpp, Timer.h/.cpp
// Added    : Font.h

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <vector>
#include <iostream>
#include <bitset>
#include <sstream>

#include "Font.h"		// Regular textures ( Enemy, Player )
#include "Texture.h"		// Regular textures ( Enemy, Player )
#include "Texture_Text.h"	// Text textures ( 'solid', 'blended', 'shaded' ), average FPS display

// Setup
bool InitEverything();
bool InitSDL();
bool CreateWindow();
bool CreateRenderer();
void SetupRenderer();

void InitializeObjects();

// Update ( happens every frame )
void Render();
void RunGame();
void HandleInput();

SDL_Rect windowRect = { 900, 300, 800, 400 };
SDL_Point windowMiddle;

SDL_Window* window;
SDL_Renderer* renderer;

Font font;

bool quit = false;

Texture_Text text;

void PrintText()
{
	std::bitset<4> x( ( font.GetMask() ) );
	std::stringstream bitmask;
	bitmask << x;

	text.RenderText_Blended( renderer, bitmask.str() );
	text.CenterAtPoint( windowMiddle );
}
int main( int argc, char* args[] )
{
	if ( !InitEverything() )
	{
		std::cout << "Failed!" << std::endl;
		std::cin.ignore();
		return -1;
	}

	InitializeObjects();

	RunGame();
}
void RunGame()
{
	while ( !quit )
	{
		HandleInput();
		PrintText();

		Render();
	}
}
void HandleInput()
{
	SDL_Event event;
	while ( SDL_PollEvent( &event ) )
	{
		if ( event.type == SDL_QUIT )
			quit = true;
		else if ( event.type == SDL_KEYDOWN )
		{
			switch ( event.key.keysym.sym )
			{
				case SDLK_ESCAPE:
					quit = true;
					break;
				case SDLK_b:
					font.AddFontStyle( FontStyle::Bold );
					break;
				case SDLK_i:
					font.AddFontStyle( FontStyle::Italic );
					break;
				case SDLK_u :
					font.AddFontStyle( FontStyle::Underline );
					break;
				case SDLK_s :
					font.AddFontStyle( FontStyle::Strikethrough );
					break;
				default :
					break;
			}
		}
		else if ( event.type == SDL_KEYUP )
		{
			switch ( event.key.keysym.sym )
			{
				case SDLK_ESCAPE:
					quit = true;
					break;
				case SDLK_b:
					font.RemFontStyle( FontStyle::Bold );
					break;
				case SDLK_i:
					font.RemFontStyle( FontStyle::Italic );
					break;
				case SDLK_u :
					font.RemFontStyle( FontStyle::Underline );
					break;
				case SDLK_s :
					font.RemFontStyle( FontStyle::Strikethrough );
					break;
				default :
					break;
			}
		}
	}
}
void InitializeObjects()
{
	text.Init( font.GetFont(), { 255, 255, 0, 255 }, { 0, 0, 255, 255 } );
	text.RenderText_Blended( renderer, "0000 0000" );
	text.CenterAtPoint( windowMiddle );
}
void Render()
{
	SDL_RenderClear( renderer );

	text.Render( renderer );

	SDL_RenderPresent( renderer);
}
// Initialization ++
// ==================================================================
bool InitEverything()
{
	if ( !InitSDL() )
		return false;

	if ( !CreateWindow() )
		return false;

	if ( !CreateRenderer() )
		return false;

	if ( !font.Init( "Anonymous.ttf", 80 ) )
		return false;


	SetupRenderer();
	windowMiddle.x = static_cast< uint32_t > ( windowRect.w * 0.5 );
	windowMiddle.y = static_cast< uint32_t > ( windowRect.h * 0.5 );

	return true;
}
bool InitSDL()
{
	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		std::cout << " Failed to initialize SDL : " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}
bool CreateWindow()
{
	window = SDL_CreateWindow( "Server", windowRect.x, windowRect.y, windowRect.w, windowRect.h, 0 );

	if ( window == nullptr )
	{
		std::cout << "Failed to create window : " << SDL_GetError();
		return false;
	}

	return true;
}
bool CreateRenderer()
{
	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );

	if ( renderer == nullptr )
	{
		std::cout << "Failed to create renderer : " << SDL_GetError();
		return false;
	}

	return true;
}
void SetupRenderer()
{
	// Set size of renderer to the same as window
	SDL_RenderSetLogicalSize( renderer, windowRect.w, windowRect.h );

	// Set color of renderer to red
	SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
}
