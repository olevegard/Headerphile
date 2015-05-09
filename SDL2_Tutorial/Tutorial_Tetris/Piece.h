#pragma once

#include "TetrisBoard.h"

struct Piece
{
	SDL_Point pt;
	std::array< std::array< int32_t, 4 > , 4 > pieceTypes;
	SDL_Rect pieceRect;

	int32_t minX = 3;
	int32_t maxX = 0;

	int32_t minY = 3;
	int32_t maxY = 0;

	void Rotate()
	{
		std::array< std::array< int32_t, 4 > , 4 > copied;

		for ( int column = 0 ; column < 4 ; ++column )
		{
			for ( int row = 0 ; row < 4 ; ++row )
				copied[column][row] = pieceTypes[column][row];
		}

		pieceTypes[0][0] = copied[0][3];
		pieceTypes[1][0] = copied[0][2];
		pieceTypes[2][0] = copied[0][1];
		pieceTypes[3][0] = copied[0][0];

		pieceTypes[0][1] = copied[1][3];
		pieceTypes[0][2] = copied[2][3];
		pieceTypes[0][3] = copied[3][3];

		pieceTypes[1][3] = copied[3][2];
		pieceTypes[2][3] = copied[3][1];
		pieceTypes[3][3] = copied[3][0];

		pieceTypes[3][1] = copied[1][0];
		pieceTypes[3][2] = copied[2][0];
		pieceTypes[3][3] = copied[3][0];


		pieceTypes[1][1] = copied[1][2];
		pieceTypes[2][1] = copied[1][1];

		pieceTypes[1][2] = copied[2][2];

		pieceTypes[2][2] = copied[2][1];

		pieceTypes[2][1] = copied[1][1];

		AdjustRect();

		if ( minX < 0 )
		{
			for ( int column = 0 ; column < 4 ; ++column )
			{
				for ( int row = 0 ; row < 4 ; ++row )
					pieceTypes[column][row] = copied[column][row];
			}

			AdjustRect();
		}

	}
	void AdjustRect()
	{
		maxX = 0;
		minX = 4;

		maxY = 0;
		minY = 4;

		for ( int column = 0 ; column < 4 ; ++column )
		{
			for ( int row = 0 ; row < 4 ; ++row )
			{
				if ( pieceTypes[column][row] != -1 )
				{
					maxY = std::max( column, maxY );
					minY = std::min( column, minY );

					maxX = std::max( row, maxX );
					minX = std::min( row, minX );
				}
			}
		}
		maxX = pt.x + maxX;
		minX = pt.x + minX;

		maxY = pt.y + maxY;
		minY = pt.y + minY;

		std::cout << "Pos : " << minX << " - " << maxX << " : " << minY << " - " << maxY << std::endl;
	}
	void MoveLeft( int32_t minBoundX )
	{
		if ( minX <= minBoundX )
			return;

		--pt.x;
		AdjustRect();
	}
	void MoveRight( int32_t maxBoundX )
	{
		std::cout << "Pt " << pt.x << ", " << pt.y << std::endl;

		if (maxX >= ( maxBoundX - 1 ) )
			return;

		++pt.x;
		AdjustRect();
	}

};


