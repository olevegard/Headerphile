#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include <map>
#include <chrono>

#include "TetrisPiece.h"
#include "TetrisBoard.h"

using namespace std::chrono;

bool InitEverything();
bool InitSDL();
bool CreateWindow();
bool CreateRenderer();
void SetupRenderer();

void Render();
void RunGame();

void HandleKeyboard( const SDL_Event &event );

SDL_Rect windowRect = { 0, 0, 800, 600 };

Texture tile;
TetrisBoard board;

SDL_Window* window;
SDL_Renderer* renderer;


auto start = high_resolution_clock::now();
milliseconds fallTimer( 800 ); // std::chrono::duration

bool loop = true;

std::map< SDL_Keycode, SDL_EventType > keyCode;

int main( int argc, char* args[] )
{
	if ( !InitEverything() )
		return -1;

	RunGame();
}
void RunGame()
{
	keyCode[ SDLK_UP ] = SDL_KEYUP ;
	board.Init( renderer, windowRect );

	while ( loop )
	{
		auto timeNow = high_resolution_clock::now();

		if ( ( timeNow - start ) >= fallTimer)
		{
			board.MoveDown();
			start = high_resolution_clock::now();
		}

		SDL_Event event;
		while ( SDL_PollEvent( &event ) )
		{
			if ( event.type == SDL_QUIT )
				loop = false;

			HandleKeyboard( event );
		}

		Render();
	}
}
void HandleKeyboard( const SDL_Event &event )
{
	if  ( event.key.keysym.sym == SDLK_UP )
	{
		if ( event.type == SDL_KEYDOWN )
		{
			if ( keyCode[ SDLK_UP ] == SDL_KEYUP )
				board.Rotate();

			keyCode[ SDLK_UP ] = SDL_KEYDOWN ;
		}
		else if ( event.type == SDL_KEYUP )
			keyCode[ SDLK_UP ] = SDL_KEYUP ;
	}

	if  ( event.key.keysym.sym == SDLK_LEFT )
	{
		if ( event.type == SDL_KEYDOWN )
		{
			if ( keyCode[ SDLK_LEFT ] == SDL_KEYUP )
				board.MoveLeft();

			keyCode[ SDLK_LEFT ] = SDL_KEYDOWN ;
		}
		else if ( event.type == SDL_KEYUP )
			keyCode[ SDLK_LEFT ] = SDL_KEYUP ;
	}
	//if  ( event.key.keysym.sym == SDLK_LEFT )

	if  ( event.key.keysym.sym == SDLK_RIGHT )
		board.MoveRight();
	//if  ( event.key.keysym.sym == SDLK_DOWN ) piece.MoveDown();

	if  ( event.key.keysym.sym == SDLK_ESCAPE )
		loop = false;

}
void Render()
{
	// Clear the window and make it all red
	SDL_RenderClear( renderer );

	board.Render( renderer );
	//piece.Render( renderer );

	// Render the changes above
	SDL_RenderPresent( renderer);
}
bool InitEverything()
{
	if ( !InitSDL() )
		return false;

	if ( !CreateWindow() )
		return false;

	if ( !CreateRenderer() )
		return false;

	SetupRenderer();

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
	renderer = SDL_CreateRenderer( window, -1, 0 );

	if ( renderer == nullptr )
	{
		std::cout << "Failed to create renderer : " << SDL_GetError();
		return false;
	}

	return true;
}
void SetupRenderer()
{
	SDL_RenderSetLogicalSize( renderer, windowRect.w, windowRect.h );

	SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
}
