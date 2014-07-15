#include "Texture_Text.h"

class TextRect : public Texture_Text
{
	public:
	TextRect()
		:	rect( { 0,0,0,0 } )
	{
	}
	void SetOuterRect( SDL_Rect rect_ )
	{
		rect.x = rect_.x;
		rect.y = rect_.y;

		rect.w += 20;
		rect.h += 20;

		CheckRect();
	}
	void CheckRect( )
	{
		SDL_Rect innerRect = GetRect();

		rect.w = std::max( rect.w, innerRect.w );
		rect.h = std::max( rect.h, innerRect.h );

		/*jjjj
		if ( rect.w > rect.h )
			rect.h = rect.w; 
		else
			rect.w = rect.h; 
			*/

		SDL_Point p;
		p.x = rect.x + ( rect.w * 0.5 );
		p.y = rect.y + ( rect.h * 0.5 );
		CenterAtPoint( p );
	}
	void Render( SDL_Renderer* renderer ) const
	{
		SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
		SDL_RenderFillRect( renderer, &rect );

		SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
		SDL_RenderDrawRect( renderer, &rect );

		SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );

		Texture_Text::Render( renderer );
	}
	void RenderValue( SDL_Renderer* renderer, int32_t value )
	{
		Texture_Text::RenderValue( renderer, value );
		CheckRect( );
	}
	SDL_Rect GetOuterRect()
	{
		return rect;
	}

	private:
	SDL_Rect rect;
};
