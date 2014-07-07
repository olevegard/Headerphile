#pragma once

#include "TetrisPiece.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <array>
#include <memory>

struct TetrisBoard
{
	TetrisBoard( )
	{
		currentPiece = std::make_unique< TetrisPiece >();

	}

	void Init( SDL_Renderer* renderer,  const SDL_Rect &windowRect )
	{
		for ( int column = 0 ; column < 8 ; ++column )
		{
			for ( int row = 0 ; row < 15 ; ++row )
			{
				textures[column][row].LoadTexture( renderer, "empty_tile.png", windowRect );
			}
		}

		currentPiece->Init( renderer, "red_tile.png", windowRect );
		PositionPieces();
	}
	void MoveDown()
	{
		currentPiece->MoveDown( );
	}
	void MoveRight()
	{
		if ( currentPiece->GetRightMostX( ) >= textures[ 7 ][ 14 ].rect.x )
			return;

		currentPiece->MoveRight( );
	}
	void MoveLeft()
	{
		currentPiece->MoveLeft( );
	}
	void PositionPieces( )
	{
		for ( int row = 0 ; row < 15 ; ++row )
		{
			for ( int column = 0 ; column < 8 ; ++column )
			{
				if ( column > 0 )
					textures[column][row].rect.x =
						textures[column- 1][row].rect.x + textures[column - 1][row].rect.w + 10;
				else 
					textures[column][row].rect.x = 0;

				if ( row > 0 )
					textures[column][row].rect.y
						= textures[column][row- 1].rect.y + textures[column][row - 1 ].rect.h + 10;
				else 
					textures[column][row].rect.y = 0;
			}
		}
	}
	/*
	   for ( int column = 0 ; column < 4 ; ++column )
	   for ( int row = 0 ; row < 4 ; ++row )
	   if ( column > 0 )
	   textures[column][row].rect.x
	   = textures[column- 1][row].rect.x + textures[column - 1][row].rect.w + 10;

	   if ( row > 0 )
	   textures[column][row].rect.y
	   = textures[column][row- 1].rect.y + textures[column][row - 1 ].rect.h + 10;
	   */
	void Render( SDL_Renderer* renderer )
	{
		for ( const auto &p : textures )
		{
			for ( const auto &texture : p )
				texture.Render( renderer );

		}

		currentPiece->Render( renderer );
	}

	std::array< std::array< Texture, 15 > , 8 > textures;
	std::unique_ptr< TetrisPiece > currentPiece;
};
