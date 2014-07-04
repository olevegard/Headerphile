#pragma once

#include "Texture.h"

struct Player : Texture
{
	Player( )
		:Texture() // Delegating construcor ( C++11 ), calls Texture::constructor
	{
	}
	void ResetPosition( const SDL_Rect &windowRect )
	{
		SDL_Rect playerRect = GetRect();

		playerRect.x = ( windowRect.w * 0.5 ) - ( playerRect.w * 0.5 );
		playerRect.y = windowRect.h - playerRect.w;

		SetRect( playerRect );
	}
	void CheckFinishLine( int32_t finishLineBottom, const SDL_Rect &windowRect )
	{
		SDL_Rect r = GetRect();

		if ( r.y < finishLineBottom )
			ResetPosition( windowRect );
	}
};
