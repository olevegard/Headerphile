#pragma once

#include "Texture.h"

struct Enemy : Texture
{
	Enemy( SDL_Rect pos_ )
		:	Texture( ) // Delegating construcor ( C++11 ), calls Texture::constructor
	{
		SetRect( pos_ );
	}
	void ResetPosition( const SDL_Rect &windowRect )
	{
		// Set pos to either left or right edge depending on speed
		if ( GetSpeed().x > 0.0 )
			SetPos( { 0, GetRect().y } );
		else
			SetPos( { windowRect.w, GetRect().y } );
	}
	void CheckBounds( const SDL_Rect &windowRect )
	{
		if ( IsOutOfBounds( windowRect ) )
		{
			ResetPosition( windowRect );
		}
	}
	
};
