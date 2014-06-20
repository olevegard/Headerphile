// Texture.h - headerphile.blogspot.no
// Contains basics of a rotable texture + extra code specifc to clock hands
// Any code marked as specific to clock hands can be removed to get a bare bones Texture class
//
#pragma once // Seme as standard inclusion guard ( #ifndef _TEXTURE_ #define _TEXTURE_ #endif )

// Specific to clock hands, can be removed
// ==================================================================
#include <ctime>

enum class HandType
{
	Hour,
	Minute,
	Second
};
// ==================================================================
//
struct Texture
{
	// Initializes a texture, including size and position
	void LoadTexture( SDL_Renderer *renderer, const std::string &str, const SDL_Rect &windowRect, HandType handType_ )
	{
		SDL_Surface* surface = IMG_Load( str.c_str() );

		texture = SDL_CreateTextureFromSurface( renderer, surface );

		SDL_FreeSurface( surface );

		handType = handType_;
		SetInitialPosition( windowRect );
	}
	// Renders the texture with rotation
	void Render( SDL_Renderer* renderer )
	{
		SDL_RenderCopyEx( renderer, texture, nullptr, &rect,  angle, &center, SDL_FLIP_NONE );
	}
	private:
	// Sets position and center
	void SetInitialPosition( const SDL_Rect &windowRect )
	{
		// Find width and height of texture
		int32_t width = 0;
		int32_t height = 0;
		SDL_QueryTexture( texture, nullptr, nullptr, &width, &height );

		// Set size
		rect.w = width;
		rect.h = height;

		// Center hands
		rect.x = windowRect.w * 0.5;
		rect.y = ( windowRect.h * 0.5 ) - rect.h;

		// Calculate center
		center.x = rect.w * 0.5;
		center.y = rect.h - ( rect.w * 0.5 );

		angle = 0;
	}

	double angle;
	SDL_Texture* texture;
	SDL_Rect rect;
	SDL_Point center;

	// Code specific to our clock hands - can be removed
	// ==================================================================
	public:
	void UpdateHandPositions()
	{
		// time( 0 ) returns current time as UNIX timestamp / epoch ( seconds since 1.1.1970 )
		auto timeEpoch = time( 0 );

		// Loceltime takes the time as epoch and returns the pointer to a tm containing the time as a tm
		// A is a struct containing variables like time in seconds, minutes andhours. 
		// Note:
		// 	Seconds are represented in the range 0 - 60 ( since C++11 )
		// 	Minutes are represented in the range 0 - 59
		// 	Hours are represented in the range 0 - 23
		tm* tmCurrent = localtime( &timeEpoch );

		if ( handType == HandType::Hour ) 
			SetAngleForTime( tmCurrent->tm_hour, 12 );
		else if ( handType == HandType::Minute ) 
			SetAngleForTime( tmCurrent->tm_min, 59 );
		else if ( handType == HandType::Second ) 
			SetAngleForTime( tmCurrent->tm_sec, 60 );
	}
	private:
	// Set rotation according to input
	void SetAngleForTime( int32_t value, double maxValue )
	{
		// Note :
		//	For hours value can be between 0 and 23, but max value is always 12
		//	This means than when hour is larger than 12, rotation will be more than 1.0 
		//	This means that angle will be larger than 360.
		//	Luckily, SDL handles this and treats it like the corresponding value less than 360
		//	So for ecample, if angle is 540 ( 360 + 180 ), SDL will treat it as 540 - 360 = 180
		double rotation = static_cast< double >( value / maxValue );

		angle = 360 * rotation; 
	}

	HandType handType;
	// ==================================================================
};

