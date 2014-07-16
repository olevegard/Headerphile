// Texture.h - headerphile.blogspot.no
// A bare-bones texture class.
// Contains texture, rect for texture, angle of rotation and center of rotation

#pragma once

#include <SDL2/SDL.h>

#include <string>

struct Speed
{
	double x;
	double y;
};

struct Texture
{
	Texture();
	~Texture();

	// Initializes a texture, including size and position
	void LoadTexture( SDL_Renderer *renderer, const std::string &str );

	virtual void Render( SDL_Renderer* renderer ) const;
	void Update( double delta );

	void IncrementX( int32_t x )
	{
		rect.x += x;
	}

	// Collision detection
	bool CheckCollision( const Texture &other ) const;
	bool CheckCollision( const SDL_Rect &other ) const;
	bool IsOutOfBounds( const SDL_Rect &windowSize ) const;

	// Setters
	void SetRect( SDL_Rect r );
	void SetPos( SDL_Point p );
	void SetSpeed( Speed s );

	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
	void CenterAtPoint( const SDL_Point &p );

	// Converts surface to SDL_Texture* and call SetTextre() with that SDL_Texture*
	void SetTexture( SDL_Renderer* renderer, SDL_Surface* surface );

	// Frees previous and sets texture to text
	void SetTexture( SDL_Texture *text );

	SDL_Texture* GetTexture()
	{
		return texture;
	}

	SDL_Rect GetRect() const;
	Speed GetSpeed( ) const;
	private:
	void CalculateSize( );
	void CalculateCenter( );

	void ConvertSuface( SDL_Renderer* renderer, SDL_Surface* surface );

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
