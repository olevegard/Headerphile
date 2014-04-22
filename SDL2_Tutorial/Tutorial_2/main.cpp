// Tutorial 2
// Basic SDL2 application used to create a window. 
// Will simply spawn a window with a single background color
// Check out headerphile.blogspot.no for more info
// ==================================================================

#include <SDL2/SDL.h>
#include <iostream>

int main( int argc, char* args[] )
{
	int posX = 100;
	int posY = 200;
	int sizeX = 300;
	int sizeY = 400;

	SDL_Window* window;
	SDL_Renderer* renderer;

	// Initialize SDL
	// ==========================================================
	if ( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
	{
		// Something failed, print error and exit.
		std::cout << " Failed to initialize SDL : " << SDL_GetError() << std::endl;
		return -1;
	}

	// Create and init the window
	// ==========================================================
	window = SDL_CreateWindow( "Server", posX, posY, sizeX, sizeY, 0 );

	if ( window == nullptr )
	{
		std::cout << "Failed to create window : " << SDL_GetError();
		return -1;
	}

	// Create and init the renderer
	// ==========================================================
	renderer = SDL_CreateRenderer( window, -1, 0 );

	if ( renderer == nullptr )
	{
		std::cout << "Failed to create renderer : " << SDL_GetError();
		return -1;
	}

	// Render something
	// ==========================================================

	// Set size of renderer to the same as window
	SDL_RenderSetLogicalSize( renderer, sizeX, sizeY );

	// Set color of renderer to red
	SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );

	// Clear the window and make it all red
	SDL_RenderClear( renderer );

	// Render the changes above ( which up until now had just happened behind the scenes )
	SDL_RenderPresent( renderer);

	// Pause program so that the window doesn't disappear at once.
	// This willpause for 4000 milliseconds which is the same as 4 seconds
	SDL_Delay( 4000 );
}
