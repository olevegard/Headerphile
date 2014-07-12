#include "Texture.h"

#include <iostream>


Texture::Texture()
	:	x( 0.0 )
	,	y( 0.0 )
		,	rect( { 0, 0, 0, 0 } )
,	angle( 0.0 )
{
}
Texture::~Texture()
{
	//SDL_DestroyTexture( texture );
	//
}
// Initializes a texture, including size and position
void Texture::LoadTexture( SDL_Renderer *renderer, const std::string &str )
{
	SDL_Surface* surface = IMG_Load( str.c_str() );

	texture = SDL_CreateTextureFromSurface( renderer, surface );

	SDL_FreeSurface( surface );

	CalculateSize( );
}
void Texture::Render( SDL_Renderer* renderer )
{
	SDL_RenderCopyEx( renderer, texture, nullptr, &rect,  angle, &center, SDL_FLIP_NONE );
}
void Texture::Update( double delta )
{
	// Delta is a floating point number, usually very small like 0.001
	// This means the result of speed.x * delta will be a decimal number
	// Since it will have decimals, we need to use double ( or float ) to let the decimals add up
	// Had we used just ints, we'd loose the decimal part.
	x += speed.x * delta * 100;
	y += speed.y * delta * 100;

	rect.x = x;
	rect.y = y;

}
SDL_Rect Texture::GetRect() const
{
	return rect;
}
Speed Texture::GetSpeed( ) const
{
	return speed;
}
void Texture::SetRect( SDL_Rect r )
{
	x = r.x;
	y = r.y;
	rect.x = r.x;
	rect.y = r.y;

	rect.w = r.w;
	rect.h = r.h;
}
void Texture::SetPos( SDL_Point p )
{
	rect.x = p.x;
	x = p.x;

	rect.y = p.y;
	y = p.y;
}
void Texture::SetSpeed( Speed s )
{
	speed.x = s.x;
	speed.y = s.y;
}
void Texture::SetTexture( SDL_Texture *text )
{
	SDL_DestroyTexture( texture );

	texture = text;
	CalculateSize( );
}
void Texture::SetTexture( SDL_Renderer* renderer, SDL_Surface* surface )
{
	ConvertSuface( renderer, surface );
}
void Texture::CenterAtPoint( const SDL_Point &p )
{
	rect.x = p.x - ( rect.w * 0.5 );
	rect.y = p.y - ( rect.h * 0.5 );
}
// Just a wrapper to make our lives easier 
bool Texture::CheckCollision( const Texture &other ) const
{
	return CheckCollision( other.GetRect() );
}
// This method is the same as in part 7
bool Texture::CheckCollision( const SDL_Rect &other ) const
{
	// Find edges of rect1
	int left1 = rect.x;
	int right1 = rect.x + rect.w;
	int top1 = rect.y;
	int bottom1 = rect.y + rect.h;

	// Find edges of rect2
	int left2 = other.x;
	int right2 = other.x + other.w;
	int top2 = other.y;
	int bottom2 = other.y + other.h;

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
bool Texture::IsOutOfBounds( const SDL_Rect &windowSize ) const
{
	if ( rect.x > windowSize.w )
		return true;

	if ( ( rect.x + rect.w ) < 0 )
		return true;

	if (  rect.y > windowSize.h )
		return true;

	if ( ( rect.y + rect.h ) < 0 )
		return true;

	// Not out fo bounds
	return false;
}
void Texture::MoveLeft()
{
	rect.x -= 20;
}
void Texture::MoveRight()
{
	rect.x += 20;
}
void Texture::MoveUp()
{
	rect.y -= 20;
}
void Texture::MoveDown()
{
	rect.y += 20;
}
void Texture::CalculateSize( )
{
	// Find width and height of texture
	int32_t width = 0;
	int32_t height = 0;
	SDL_QueryTexture( texture, nullptr, nullptr, &width, &height );

	// Set size
	rect.w = width;
	rect.h = height;

	CalculateCenter( );
}
void Texture::CalculateCenter( )
{
	center.x = rect.x + ( rect.w * 0.5 );
	center.y = rect.y + ( rect.h * 0.5 );
}
void Texture::ConvertSuface( SDL_Renderer* renderer, SDL_Surface* surface )
{
	if ( surface == nullptr )
	{
		std::cout << "Texture creation from surface failed|\n";
		std::cout << "\tERROR: " << SDL_GetError() << std::endl;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, surface );

	SetTexture( texture );
	SDL_FreeSurface( surface );
}
