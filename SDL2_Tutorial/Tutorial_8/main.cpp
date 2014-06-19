#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <vector>

#include <time.h>

struct Texture
{
	SDL_Texture* texture;
	SDL_Rect rect;
	SDL_Point center;
	double angle;

	void LoadTexture( SDL_Renderer *renderer, const std::string &str )
	{
		SDL_Surface* surface = IMG_Load( str.c_str() );

		texture = SDL_CreateTextureFromSurface( renderer, surface );

		SDL_FreeSurface( surface );
	}
	void Update( const SDL_Rect &windowRect )
	{
		int32_t width = 0;
		int32_t height = 0;

		SDL_QueryTexture( texture, nullptr, nullptr, &width, &height );

		rect.w = width * 1.1;
		rect.h = height * 1.1;

		rect.x = windowRect.w * 0.5;
		rect.y = windowRect.h * 0.5;

		center.x = rect.h * 0.5;
		center.y = rect.h * 0.5;
	}
	void Render( SDL_Renderer* renderer )
	{
		SDL_RenderCopyEx( renderer, texture, nullptr, &rect, ( angle + 270 ), &center, SDL_FLIP_NONE );
	}
};

bool InitEverything();
bool InitSDL();
bool CreateWindow();
bool CreateRenderer();
void SetupRenderer();

Texture LoadTexture( const std::string &str );

void Render();
void RunGame();

SDL_Rect windowRect = { 0, 0, 1920, 1080 };

void UpdateHandPositions();
void SetHourPosition( int32_t time );
void SetSecondPosition( int32_t time );
void SetMinutePosition( int32_t time );

Texture hour;
Texture minute;
Texture second;

SDL_Window* window;
SDL_Renderer* renderer;

int main( int argc, char* args[] )
{
	if ( !InitEverything() )
		return -1;

	hour.LoadTexture( renderer, "hour_hand.png" );
	hour.Update( windowRect );
	hour.angle = 0;

	minute.LoadTexture( renderer, "min_hand.png" );
	minute.Update( windowRect );
	minute.angle = 180;

	second.LoadTexture( renderer, "sec_hand_2.png" );
	second.Update( windowRect );
	second.angle = 90;

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

		// Add a 16msec delay to make our game run at ~60 fps
		SDL_Delay( 16 );
	}
}
void UpdateHandPositions()
{
	auto ttimeee = time( 0 );
	tm* tmCurrent = localtime( &ttimeee );

	SetHourPosition( tmCurrent->tm_hour );
	SetSecondPosition( tmCurrent->tm_sec );
	SetMinutePosition( tmCurrent->tm_min);
}
void SetHourPosition( int32_t time )
{
	time %= 12;

	hour.angle = 360 * static_cast< float >( time / 12.0 );
}
void SetSecondPosition( int32_t time )
{
	time %= 61;
	second.angle = 360 * static_cast< float >( time / 60.0 );
}
void SetMinutePosition( int32_t time )
{
	time %= 61;

	minute.angle = 360 * static_cast< float >( time / 60.0 );
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
