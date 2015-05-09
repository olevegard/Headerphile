// Texture.h - headerphile.blogspot.no
// A bare-bones texture class.
// Contains texture, rect for texture, angle of rotation and center of rotation

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>

#include "Texture.h"

struct Speed
{
	double x;
	double y;
};

struct Texture
{
	Texture();

	// Initializes a texture, including size and position
	void LoadTexture( SDL_Renderer *renderer, const std::string &str );
	void Render( SDL_Renderer* renderer );
	void Update( double delta );

	// Just a wrapper to make our lives easier 
	bool CheckCollision( const Texture &other ) const;
	// This method is the same as in part 7
	bool CheckCollision( const SDL_Rect &other ) const;

	bool IsOutOfBounds( const SDL_Rect &windowSize ) const;

	void SetRect( SDL_Rect r );
	void SetPos( SDL_Point p );
	void SetSpeed( Speed s );

	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();

	SDL_Rect GetRect() const;
	Speed GetSpeed( ) const;

	private:
	void CalculateSize( );
	void CalculateCenter( );

	// Position
	double x;
	double y;
	SDL_Rect rect;

	SDL_Texture* texture;

	// Movement
	Speed speed;

	// Rotation
	double angle;
	SDL_Point center;
};
