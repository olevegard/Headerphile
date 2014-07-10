#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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

// Our new function for setting uo SDL_TTF
bool SetupTTF( const std::string &fontName );

// Timing
using namespace std::chrono; // Adding this means we don't have to add std::chrono in front of everything
double GetDelta();

void InitializeObjects();
void AddEnemies( int32_t count );
void AddEnemy( int32_t height );

// Update ( happens every frame )
void Render();
void RunGame();
void HandleInput();
void UpdateObjects( double delta );

SDL_Rect windowRect = { 900, 300, 600, 800 };
SDL_Point windowMiddle;

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;
TTF_Font* bigFont;

bool quit = false;

// Objects
Player player;
Texture background;
Texture topBar;
Texture bottomBar;

FontTexture points;
FontTexture backgroundText;
FontTexture start;
FontTexture finish;

std::vector< Enemy > enemies;

auto timePrev = high_resolution_clock::now();

int main( int argc, char* args[] )
{
	if ( !InitEverything() )
		return -1;

	InitializeObjects();
	
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

	// Compare the two to create time_point containing delta time in nanosecnds
	auto timeDiff = duration_cast< nanoseconds >( timeCurrent - timePrev );

	// Get the tics as a variable
	double delta = timeDiff.count();

	// Turn nanoseconds into seconds
	delta /= 1000000000;

	timePrev = timeCurrent;
	return delta;
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
	start.Init( bigFont, { 255, 255, 0, 255 }, { 0, 0, 0, 255 } ); 
	start.RenderText_Shaded( renderer, "Shaded" );
	start.CenterAtPoint( { windowMiddle.x, windowMiddle.y + 160 } );

	finish.Init( bigFont, { 255, 255, 0, 255 }, { 0, 0, 0, 255 } );
	finish.RenderText_Blended( renderer, "Blended" );
	finish.CenterAtPoint( { windowMiddle.x, windowMiddle.y - 160 } );

	points.Init( font, { 255, 255, 0, 255 }, { 0, 0, 0, 255 } ); 
	points.SetPos( { 0, 0 } );
	points.RenderValue( renderer, "Points", 0 );

	background.LoadTexture( renderer, "background_600x800.png" );

	backgroundText.Init( bigFont, { 255, 255, 255, 255 }, { 0, 0, 0, 255 } ); 
	backgroundText.RenderText_Solid( renderer, "Solid" );
	backgroundText.CenterAtPoint( windowMiddle );

	bottomBar.LoadTexture( renderer, "bar_600x40.png" );
	bottomBar.SetPos( { 0, windowRect.h - 40 } );

	topBar.LoadTexture( renderer, "bar_600x40.png" );

	AddEnemies( 15 );
}
void AddEnemies( int32_t count )
{
	int32_t lastEnemyPos = 0;

	for ( int32_t i = 0 ; i < count ; ++i )
	{
		AddEnemy( lastEnemyPos );
		lastEnemyPos += 50;
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
	backgroundText.Render( renderer );

	start.Render( renderer );
	finish.Render( renderer );

	for (  auto &p : enemies )
		p.Render( renderer );

	topBar.Render( renderer );
	bottomBar.Render( renderer );
	player.Render( renderer );
	points.Render ( renderer );

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

	if ( !SetupTTF( "sketchy.ttf" ) )
		return false;

	SetupRenderer();
	windowMiddle.x = static_cast< uint32_t > ( windowRect.w * 0.5 );
	windowMiddle.y = static_cast< uint32_t > ( windowRect.h * 0.5 );

	return true;
}
bool SetupTTF( const std::string &fontName)
{
	if ( TTF_Init() == -1 )
	{
		std::cout << " Failed to initialize TTF : " << SDL_GetError() << std::endl;
		return false;
	}

	font = TTF_OpenFont( fontName.c_str(), 38 );
	bigFont = TTF_OpenFont( fontName.c_str(), 64 );
	
	if ( font == nullptr )
	{
		std::cout << " Failed to load font : " << SDL_GetError() << std::endl;
		return false;
	}

	points.Init( font, { 255, 0, 255, 255 }, { 0, 255, 0, 255 } );

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
