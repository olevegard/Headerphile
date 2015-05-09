// Tutorial 4
// Basic SDL2 application used to create a window. 
// Will spawn a window and draw several shapes
// Feel free to change the shapes, change colors, create new one.
// Experiment and have fun!
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

	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		std::cout << " Failed to initialize SDL : " << SDL_GetError() << std::endl;
		return -1;
	}
	window = SDL_CreateWindow( "Server", posX, posY, sizeX, sizeY, 0 );
	if ( window == nullptr )
	{
		std::cout << "Failed to create window : " << SDL_GetError();
		return -1;
	}
	renderer = SDL_CreateRenderer( window, -1, 0 );
	if ( renderer == nullptr )
	{
		std::cout << "Failed to create renderer : " << SDL_GetError();
		return -1;
	}
	// Set size of renderer to the same as window
	SDL_RenderSetLogicalSize( renderer, sizeX, sizeY );

	// Set color of renderer to red
	SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );

	// Clear the window and make it all red
	SDL_RenderClear( renderer );

	// Create a rectangle
	SDL_Rect r;
	r.x = 50;
	r.y = 50;
	r.w = 50;
	r.h = 50;
	// Change color to blue!
	SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );

	// Render our SDL_Rect
	// The rectangle will now be blue
	SDL_RenderDrawRect( renderer, &r );

	// Shrink the width of the rectangle to 25 and the same for the height
	// The position will remain the same
	r.w = 25;
	r.h = 25;

	// Change color to green!
	SDL_SetRenderDrawColor( renderer, 0, 255, 0, 255 );

	// This will draw a green rectangle
	// The rectangle will be drawn inside the rectangle we just drew
	// The top-left position of this rectangle will be the same as the top-left position of the previous rectangle
	// It will cover half the width and height of the rectangle
	SDL_RenderFillRect( renderer, &r );

	// Move the rectangle 25 to the right and 25 down
	r.x += 25;
	r.y += 25;

	// This will draw a green rectangle
	// The size of this rectangle will be the same as the previous one
	// ( half width, half height of the first one)
	// The top-left position of this rectangle will be the same as the bottom-right of the previous rectangle
	SDL_RenderFillRect( renderer, &r );

	// Move the rectangle 25 to the left and 25 down
	r.x -= 25;
	r.y += 30;

	// Increase width by 100
	r.w += 100;

	// This will draw a wider rectangle just to show that the width and height doesn't have to be the same
	// I.E. it doesn't have to be a square
	SDL_RenderFillRect( renderer, &r );

	// Render the changes above
	SDL_RenderPresent( renderer);

	// Pause program so that the window doesn't disappear at once.
	// This will pause for 4000 milliseconds which is the same as 4 seconds
	SDL_Delay( 4000 );
}
