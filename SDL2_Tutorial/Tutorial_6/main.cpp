#include <SDL2/SDL.h>

#include <iostream>
#include <vector>

bool InitEverything();
bool InitSDL();
bool CreateWindow();
bool CreateRenderer();
void SetupRenderer();

SDL_Texture* LoadTexture( const std::string &str );

void Render();
void RunGame();

void AddEnemy();
void MoveEnemies();

void ResetPlayerPos();

bool CheckCollision( const SDL_Rect &rect1, const SDL_Rect &rect2 );
bool CheckEnemyCollisions();

// Window pos
/*
int posX = 900;
int posY = 300;
int sizeX = 300;
int sizeY = 400;
*/
SDL_Rect windowRect = { 900, 300, 300, 400 };

int movementFactor = 15; // Amount of pixels the player move per key press
int lastEnemyPos = 50;

SDL_Window* window;
SDL_Renderer* renderer;

SDL_Rect playerPos;
SDL_Rect topBar;
SDL_Rect bottomBar;
SDL_Rect backgroundPos;

SDL_Texture* enemyTexture;
SDL_Texture* playerTexture;
SDL_Texture* backgroundTexture;
SDL_Texture* barTexture;

enum class Direction
{
	Left,
	Right
};
struct Enemy
{
	Enemy( SDL_Rect pos_, int speed_, Direction dir_ )
	{
		pos = pos_;
		speed = speed_;
		dir = dir_;
	}

	SDL_Rect pos;
	int speed;
	Direction dir;
};

std::vector< Enemy > enemies;

int main( int argc, char* args[] )
{
	// Background texture has the same size as window
	// But textures are drawn with 0,0 being to top-left of the window
	// 	 and not the top-left of the screen
	backgroundPos.x = 0;
	backgroundPos.y = 0;
	backgroundPos.w = windowRect.w;
	backgroundPos.h = windowRect.h;


	if ( !InitEverything() )
		return -1;

	enemyTexture      = LoadTexture( "enemy.bmp" );
	playerTexture     = LoadTexture( "player.bmp" );
	backgroundTexture = LoadTexture( "background.bmp" );
	barTexture        = LoadTexture( "bar.bmp" );

	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();

	// Init top and bottom bar
	topBar.x = 0;
	topBar.y = 0;
	topBar.w = windowRect.w;//sizeX;
	topBar.h = 20;

	bottomBar.x = 0;
	//bottomBar.y = sizeY - 20;
	bottomBar.y = windowRect.h - 20;
	bottomBar.w = windowRect.w;//sizeX;
	bottomBar.h = 20;

	// Initlaize our player
	playerPos.w = 20;
	playerPos.h = 20;
	ResetPlayerPos();

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
			else if ( event.type == SDL_KEYDOWN )
			{
				switch ( event.key.keysym.sym )
				{
					case SDLK_RIGHT:
						playerPos.x += movementFactor;
						break;
					case SDLK_LEFT:
						playerPos.x -= movementFactor;
						break;
						// Remeber 0,0 in SDL is left-top. So when the user pressus down, the y need to increase
					case SDLK_DOWN:
						playerPos.y += movementFactor;
						break;
					case SDLK_UP:
						playerPos.y -= movementFactor;
						break;
					default :
						break;
				}
			}
		}

		MoveEnemies();

		// Check collisions against enemies
		if ( CheckEnemyCollisions() )
			ResetPlayerPos();

		// Check collusion against bottom bar
		// Since top bar covers the entire width, we only need to check the y value
		// topBar.y refers to top of top bar ( top of the screen ) 
		// Since 0,0 is the top-right we need to add topBar.h to find the bottom of topBar
		if ( playerPos.y < ( topBar.y + topBar.h) )
			ResetPlayerPos();

		Render();

		// Add a 16msec delay to make our game run at ~60 fps
		SDL_Delay( 16 );
	}
}
SDL_Texture* LoadTexture( const std::string &str )
{
	// Load image as SDL_Surface
	SDL_Surface* surface = SDL_LoadBMP( str.c_str() );

	// SDL_Surface is just the raw pixels
	// Convert it to a hardware-optimzed texture so we can render it
	SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, surface );

	// Don't need the orignal texture, release the memory
	SDL_FreeSurface( surface );

	return texture;
}
void Render()
{
	// Clear the window and make it all red
	SDL_RenderClear( renderer );

	SDL_RenderCopy( renderer, backgroundTexture, NULL, &backgroundPos );

	SDL_RenderCopy( renderer, barTexture, NULL, &topBar );
	SDL_RenderCopy( renderer, barTexture, NULL, &bottomBar );

	SDL_RenderCopy( renderer, playerTexture, NULL, &playerPos );

	for ( const auto &p : enemies )
		SDL_RenderCopy( renderer, enemyTexture, NULL, &p.pos );

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
void MoveEnemies()
{
	for (  auto &p : enemies )
	{
		if ( p.dir == Direction::Right )
		{
			p.pos.x += p.speed;

			//if ( p.pos.x >= sizeX )
			if ( p.pos.x >= windowRect.w )
				p.pos.x = 0;
		}
		else
		{
			p.pos.x -= p.speed;

			if ( ( p.pos.x + p.pos.w )  <= 0 )
				p.pos.x = windowRect.w - p.pos.w;
				//p.pos.x = sizeX - p.pos.w;

		}
	}
}
bool CheckCollision( const SDL_Rect &rect1, const SDL_Rect &rect2 )
{
	// Find edges of rect1
	int left1 = rect1.x;
	int right1 = rect1.x + rect1.w;
	int top1 = rect1.y;
	int bottom1 = rect1.y + rect1.h;

	// Find edges of rect2
	int left2 = rect2.x;
	int right2 = rect2.x + rect2.w;
	int top2 = rect2.y;
	int bottom2 = rect2.y + rect2.h;

	// Check edges
	if ( left1 > right2 )// Left 1 is right of right 2
		return false; // No collision

	if ( right1 < left2 ) // Right 1 is left of left 2
		return false; // No collision

	if ( top1 > bottom2 ) // Top 1 is below bottom 2
		return false; // No collision

	if ( bottom1 < top2 ) // Bottom 1 is above top 2 
		return false; // No collision

	return true;
}
bool CheckEnemyCollisions()
{
	for ( const auto &p : enemies )
	{
		if ( CheckCollision( p.pos, playerPos) ) 
			return true;
	}

	return false;
}
void AddEnemy()
{
	if ( ( rand() % 2 ) == 0 )
	{
		enemies.push_back( Enemy( { rand() % 300, lastEnemyPos, 20, 20 }, 1, Direction::Right )  );
	}
	else
	{
		enemies.push_back( Enemy( { rand() % 300, lastEnemyPos, 20, 20 }, 1, Direction::Left )  );
	}

	lastEnemyPos += 25;
}
void ResetPlayerPos()
{
	// sizeX / 2 = middle pixel of the screen
	// playerPos.w / 2 = middle of the player 
	// So setting player x pos to [middle of screen] - [middle of player] means it will be centerd in the screen.
	//
	//playerPos.x = ( sizeX / 2 ) - ( playerPos.w / 2 );
	//playerPos.y = sizeY - bottomBar.h;
	//
	playerPos.x = ( windowRect.w / 2 ) - ( playerPos.w / 2 );
	playerPos.y = windowRect.h - bottomBar.h;
}
