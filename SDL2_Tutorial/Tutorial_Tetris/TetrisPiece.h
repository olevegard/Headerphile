#pragma once

#include <array>
//
//#include "Texture.h"


struct Texture
{
	void LoadTexture( SDL_Renderer *renderer, const std::string &str, const SDL_Rect &windowPos )
	{
		SDL_Surface* surface = IMG_Load( str.c_str() );

		texture = SDL_CreateTextureFromSurface( renderer, surface );

		SDL_FreeSurface( surface );

		CalculateSize( );
	}
	void Render( SDL_Renderer* renderer ) const
	{
		SDL_RenderCopyEx( renderer, texture, nullptr, &rect,  angle, &center, SDL_FLIP_NONE );
	}
	void CalculateSize( )
	{
		// Find width and height of texture
		int32_t width = 0;
		int32_t height = 0;
		SDL_QueryTexture( texture, nullptr, nullptr, &width, &height );

		// Set size
		rect.w = width;
		rect.h = height;

		//CalculateCenter( );
	}
	double angle;
	SDL_Point center;
	SDL_Texture* texture;
	SDL_Rect rect;
};

struct TetrisPiece
{
	public:
		TetrisPiece()
		{
			// Hard coded for now. This will result in a L piece
			positions =
			{
				false , false, false, false,
				false , true , false, false,
				false , true, false , false,
				false , true , true , false
			};
			// This will produce the L-piece
			// |=========|
			// | - - - - |
			// | - X - - |
			// | - X - - |
			// | - X X - |
			// |=========|
		}
		void Init( SDL_Renderer* renderer, const std::string &str, const SDL_Rect &windowPos )
		{
			tile.LoadTexture( renderer, "red_tile.png", windowPos );
			noTile.LoadTexture( renderer, "empty_tile.png", windowPos );

			for ( int column = 0 ; column < 4 ; ++column )
			{
				for ( int row = 0 ; row < 4 ; ++row )
				{
					if ( positions[row][column] == true )
						textures[column][row].LoadTexture( renderer, "red_tile.png", windowPos );
					else
						textures[column][row].LoadTexture( renderer, "empty_tile.png", windowPos );
				}
			}

			PositionPieces();
			Rotate();
		}
		void Render( SDL_Renderer* renderer ) const
		{
			for ( int column = 0 ; column < 4 ; ++column )
			{
				for ( int row = 0 ; row < 4 ; ++row )
				{
					textures[column][row].Render( renderer );
				}
			}
		}
		int32_t GetLeftMostX()
		{
			for ( int column = 0 ; column < 4 ; ++column )
			{
				for ( int row = 0 ; row < 4 ; ++row )
				{
					if ( positions[row][column] )
					{
						return textures[column][row].rect.x;
					}
				}
			}

			return -1;
		}
		int32_t GetRightMostX()
		{
			for ( int column = 3 ; column != 0 ; --column )
			{
				for ( int row = 3 ; row != 0 ;  --row )
				{
					if ( positions[row][column] )
						return  textures[column][row].rect.x;
				}
			}

			return -1;
		}
		void Rotate()
		{
			// This is the tricky part. The rotation needs to work for all pieces
			// The rotation for the L piece will look like this
			// |=========|----|=========|----|=========|----|=========|----|=========|
			// | - - - - |----| - - - - |----| - X X - |----| - - - - |----| - - - - |
			// | - X - - |----| X X X - |----| - - X - |----| - - - X |----| - X - - |
			// | - X - - |----| X - - - |----| - - X - |----| - X X x |----| - X - - |
			// | - X X - |----| - - - - |----| - - - - |----| - - - - |----| - X X - |
			// |=========|----|=========|----|=========|----|=========|----|=========|

			// The position within the 4x4 grid doesn't really matter because it will be hidden by the player ( more about that in the next part. )
			// y, x


			// |=========|----|=========|----|=========|----|=========|----|=========|
			// | - - - - |----| X X X - |----| - X X - |----| - - - - |----| - - - - |
			// | X - - - |----| X - - - |----| - - X - |----| - - - X |----| - X - - |
			// | X - - - |----| - - - - |----| - - X - |----| - X X x |----| - X - - |
			// | X X - - |----| - - - - |----| - - - - |----| - - - - |----| - X X - |
			// |=========|----|=========|----|=========|----|=========|----|=========|
			//
			/*
			//
			//
			*/

			std::array< std::array< Texture, 4 > , 4 > copied;

			for ( int column = 0 ; column < 4 ; ++column )
			{
				for ( int row = 0 ; row < 4 ; ++row )
				{
					copied[column][row].texture = textures[column][row].texture;
				}
			}

			textures[0][0].texture = copied[0][3].texture;
			textures[1][0].texture = copied[0][2].texture;
			textures[2][0].texture = copied[0][1].texture;
			textures[3][0].texture = copied[0][0].texture;

			textures[0][1].texture = copied[1][3].texture;
			textures[0][2].texture = copied[2][3].texture;
			textures[0][3].texture = copied[3][3].texture;

			textures[1][3].texture = copied[3][2].texture;
			textures[2][3].texture = copied[3][1].texture;
			textures[3][3].texture = copied[3][0].texture;

			textures[3][1].texture = copied[1][0].texture;
			textures[3][2].texture = copied[2][0].texture;
			textures[3][3].texture = copied[3][0].texture;


			textures[1][1].texture = copied[1][2].texture;
			textures[2][1].texture = copied[1][1].texture;

			textures[1][2].texture = copied[2][2].texture;

			textures[2][2].texture = copied[2][1].texture;

			textures[2][1].texture = copied[1][1].texture;
		}

	private:
		void PositionPieces( )
		{
			for ( int column = 0 ; column < 4 ; ++column )
			{
				for ( int row = 0 ; row < 4 ; ++row )
				{
					if ( column > 0 )
						textures[column][row].rect.x
							= textures[column- 1][row].rect.x + textures[column - 1][row].rect.w + 10;

					if ( row > 0 )
						textures[column][row].rect.y
							= textures[column][row- 1].rect.y + textures[column][row - 1 ].rect.h + 10;
				}
			}
		}

		std::array< std::array< bool, 4 > , 4 > positions;
		std::array< std::array< Texture, 4 > , 4 > textures;

		Texture tile;
		Texture noTile;
};
