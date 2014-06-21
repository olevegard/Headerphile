#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "Texture.h"

bool InitEverything();
bool InitSDL();
bool CreateWindow();
bool CreateRenderer();
void SetupRenderer();

void Render();
void RunGame();

void UpdateHandPositions();

SDL_Rect windowRect = { 0, 0, 1920, 1080 };

Texture hour;
Texture minute;
Texture second;

SDL_Window* window;
SDL_Renderer* renderer;

int main( int argc, char* args[] )
{
	if ( !InitEverything() )
		return -1;

	hour.LoadTexture( renderer, "hour_hand.png", windowRect, HandType::Hour );
	minute.LoadTexture( renderer, "min_hand.png", windowRect, HandType::Minute  );
	second.LoadTexture( renderer, "sec_hand.png", windowRect, HandType::Second  );

	RunGame();
}
void RunGame()
{
	bool loop = true;

	while ( loop )
	{
		SDL_Event event;
		while ( SDL_PollEvent( &event ) )
		{
			if ( event.type == SDL_QUIT )
				loop = false;
		}

		UpdateHandPositions();
		Render();

		SDL_Delay( 16 );
	}
}
void UpdateHandPositions()
{
	hour.UpdateHandPositions();
	minute.UpdateHandPositions();
	second.UpdateHandPositions();
}
void Render()
{
	// Clear the window and make it all red
	SDL_RenderClear( renderer );

	hour.Render( renderer );
	minute.Render( renderer );
	second.Render( renderer );

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
