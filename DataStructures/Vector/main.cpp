// SLD2 Tutorial Part 10 - Headerphile.blogspot.no
// Text renderering
// Extends our game by adding text
// Compile  : clang++ main.cpp Texture.cpp Teure_Text.cpp Timer.cpp -std=c++1y -lSDL2 -lSDL2_image -lSDL2_ttf -o Game
// Run      : ./Game
// Updated : Texture.h/.cpp, Texture_Text.h/.cpp, Timer.h/.cpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>

#include "Container.h"

// Setup
bool InitEverything();
bool InitSDL();
bool CreateWindow();
bool CreateRenderer();
void SetupRenderer();
bool SetupTTF();

// Update ( happens every frame )
void Render();
void RunGame();
void HandleInput();

SDL_Rect windowRect = { 900, 300, 1200, 300 };
SDL_Point windowMiddle;

SDL_Window* window;
SDL_Renderer* renderer;

Container container;

int counter = 0;
bool quit = false;

int main( int argc, char* args[] )
{
	if ( !InitEverything() )
		return -1;

	RunGame();

}
void RunGame()
{
	while ( !quit )
	{
		HandleInput();

		if ( counter == 2 )
		{
			container.AddObject( 44,4 );
			++counter;
		}
		if ( counter == 1 )
		{
			container.AddObject( 2 );

			container.AddObject( 3 );
			container.AddObject( 4 );

			container.AddObject( 5 );
			container.AddObject( 6 );
			container.AddObject( 55, 5 );
			++counter;
		}
		if ( counter == 0 )
		{
			container.AddObject( 1 );
		}

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
				default :
					break;
			}
		}
	}
}
void Render()
{
	container.Render( );
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

	if ( !SetupTTF( ) )
		return false;

	SetupRenderer();
	windowMiddle.x = static_cast< uint32_t > ( windowRect.w * 0.5 );
	windowMiddle.y = static_cast< uint32_t > ( windowRect.h * 0.5 );

	container.Init( renderer, "sketchy.ttf", 38 );

	return true;
}
bool SetupTTF( )
{
	if ( TTF_Init() == -1 )
	{
		std::cout << " Failed to initialize TTF : " << SDL_GetError() << std::endl;
		return false;
	}

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
	SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
}
