#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <vector>
#include <iostream>

#include <chrono>

#include "Enemy.h"
#include "Player.h"
#include "Texture.h"

// Setup
bool InitEverything();
bool InitSDL();
bool CreateWindow();
bool CreateRenderer();
void SetupRenderer();

void InitializeObjects();
void AddEnemies( int32_t count );
void AddEnemy( int32_t height );

// Update ( happens every frame )
void Render();
void RunGame();
void HandleInput();
void UpdateObjects( double delta );

SDL_Rect windowRect = { 900, 300, 300, 400 };

SDL_Window* window;
SDL_Renderer* renderer;
bool quit = false;

// Objects
Player player;
Texture background;
Texture topBar;
Texture bottomBar;

std::vector< Enemy > enemies;

// Timing
using namespace std::chrono; // Adding this means we don't have to add std::chrono in front of everything
double GetDelta();

auto timePrev = high_resolution_clock::now();

int main( int argc, char* args[] )
{
	if ( !InitEverything() )
		return -1;

	player.LoadTexture( renderer, "player.png" );
	player.ResetPosition( windowRect );

	background.LoadTexture( renderer, "background.png" );

	bottomBar.LoadTexture( renderer, "bar.bmp" );
	bottomBar.SetPos( { 0, windowRect.h - 20 } );
	topBar.LoadTexture( renderer, "bar.bmp" );

	AddEnemies( 15 );

	RunGame();
}
void RunGame()
{
	while ( !quit )
	{
		HandleInput();
		UpdateObjects( GetDelta() );

		Render();
	}
}
// Returns time since last time this function was called in seconds with nanosecond precision
double GetDelta()
{
	// Gett current time as a std::chrono::time_point
	// which basically contains info about the current point in time
	auto timeCurrent = high_resolution_clock::now();

	// Create a duration that stores seconds as double 
	duration< double, std::ratio< 1, 1 > > timeDiff;

	// And assign it to the difference between timeCurrent and timePrev ( delta time )
	timeDiff = ( timeCurrent - timePrev );

	timePrev = timeCurrent;
	return timeDiff.count();
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
				case SDLK_RIGHT:
					player.MoveRight( );
					break;
				case SDLK_LEFT:
					player.MoveLeft( );
					break;
				case SDLK_DOWN:
					player.MoveDown( );
					break;
				case SDLK_UP:
					player.MoveUp( );
					break;
				default :
					break;
			}
		}
	}
}
void UpdateObjects( double delta )
{
	player.CheckFinishLine( topBar.GetRect().y + topBar.GetRect().h, windowRect );

	for (  auto &p : enemies )
	{
		p.Update( delta );

		if ( player.CheckCollision( p ) )
			player.ResetPosition( windowRect );

		p.CheckBounds( windowRect );
	}
}
void InitializeObjects()
{
}
void AddEnemies( int32_t count )
{
	int32_t lastEnemyPos = 0;

	for ( int32_t i = 0 ; i < count ; ++i )
	{
		AddEnemy( lastEnemyPos );
		lastEnemyPos += 25;
	}
}
void AddEnemy( int32_t height )
{
	Enemy enemy( { rand() % 300, height, 20, 20 } );
	enemy.LoadTexture( renderer, "enemy.png" );

	if ( ( rand() % 2 ) == 0 )
		enemy.SetSpeed( { 1, 0 } );
	else
		enemy.SetSpeed( { -1, 0 } );

	enemies.push_back( enemy );
}
void Render()
{
	// Clear the window and make it all red
	SDL_RenderClear( renderer );

	background.Render( renderer );

	for (  auto &p : enemies )
		p.Render( renderer );

	topBar.Render( renderer );
	bottomBar.Render( renderer );
	player.Render( renderer );

	// Render the changes above
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
	//window = SDL_CreateWindow( "Server", posX, posY, sizeX, sizeY, 0 );
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
	// Set size of renderer to the same as window
	SDL_RenderSetLogicalSize( renderer, windowRect.w, windowRect.h );

	// Set color of renderer to red
	SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
}
