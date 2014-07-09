#pragma once

#include "TetrisPiece.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <array>
#include <memory>
#include <vector>

struct TetrisBoard
{
	TetrisBoard( )
	{
		currentPiece = std::make_unique< TetrisPiece >();

	}
	void CreatePieces()
	{
		// Hard coded for now. This will result in a L piece

		// This will produce the L-piece
		// |=========|
		// | - - - - |
		// | - X - - |
		// | - X - - |
		// | - X X - |
		// |=========|

		{
			PieceLayout piece;
			piece.layout = 
			{
				false, true , false, false,
				false, true , false, false,
				false, true , true , false,
				false, false, false, false
			};

			layouts.push_back( piece );
		}
		{
			PieceLayout piece;
			piece.layout = 
			{
				false , true, false, false,
				false , true, false, false,
				false , true, false, false,
				false , true, false, false
			};

			layouts.push_back( piece );
		}

	}

	void Init( SDL_Renderer* renderer,  const SDL_Rect &windowRect )
	{
		CreatePieces();

		for ( int column = 0 ; column < 8 ; ++column )
		{
			for ( int row = 0 ; row < 15 ; ++row )
			{
				textures[column][row].LoadTexture( renderer, "empty_tile.png", windowRect );
			}
		}
		currentPiece->Init( renderer, "red_tile.png", layouts[ 1 ], windowRect );
		PositionPieces();

		boardRect.x = 0;
		boardRect.y = 0;
		boardRect.w = textures[7][14].rect.x + textures[7][14].rect.w;
		boardRect.h = textures[7][14].rect.y + textures[7][14].rect.h;
	}
	void MoveDown()
	{
		SDL_Rect r = currentPiece->GetRect();

		if ( ( r.y + r.h ) < ( boardRect.y + boardRect.h ) )
			currentPiece->MoveDown( );
	}
	void MoveRight()
	{
		if ( currentPiece->GetRect().x  + currentPiece->GetRect().w >= boardRect.w )
			return;

		currentPiece->MoveRight( );
	}
	void MoveLeft()
	{
		if ( currentPiece->GetRect().x <= boardRect.x )
			return;

		currentPiece->MoveLeft( );
	}
	void Rotate()
	{
		SDL_Rect pieceRect = currentPiece->GetRect();
		if ( pieceRect.x < boardRect.x )
			return;

		if ( pieceRect.x + pieceRect.w > boardRect.w )
			return;

		currentPiece->Rotate( boardRect );
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
	void Render( SDL_Renderer* renderer )
	{
		for ( const auto &p : textures )
		{
			for ( const auto &texture : p ) texture.Render( renderer );
		}

		currentPiece->Render( renderer );

		SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
		SDL_RenderDrawRect( renderer, &boardRect );
		SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
	}
	void Update( )
	{
		SDL_Rect r = currentPiece->GetRect();

		if ( ( r.y + r.h ) >= ( boardRect.y + boardRect.h ) )
			std::cout << "Tile at bottom\n";

	}
	SDL_Rect boardRect;
	std::array< std::array< Texture, 15 > , 8 > textures;
	std::unique_ptr< TetrisPiece > currentPiece;
	std::vector< PieceLayout > layouts;
};
