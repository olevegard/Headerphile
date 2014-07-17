#include "Container.h"

#include <iostream>
#include <sstream>

#include <chrono>
#include <thread>

using namespace std::chrono;


Container::Container()
	:	size( 0 )
	,	capacity( 0 )
	,	sleepTime( 500 )
	,	colorSizeBar( { 0, 255, 0, 255 } )
	,	colorCapacity( { 0, 0, 255, 255 } )
{
}
Container::~Container()
{
	// Free our fonts
	TTF_CloseFont( font );
}
void Container::Render( bool sleep )
{
	SDL_RenderClear( renderer );

	SDL_Rect rect = originalRect;
	SDL_Point sizePoint = { originalRect.x, originalRect.y };
	SDL_Point capacityPoint = sizePoint;
	capacityPoint.y += itemSize.y + ( marginSize.y * 2 );

	for ( auto i = 0 ; i < data.size() ; ++i )
	{
		data[i].SetOuterRect( rect );
		data[i].Render( renderer );

		rect.x += itemSize.x + marginSize.x;

		if ( i < size )
			sizePoint.x = rect.x;
		if ( i == ( data.size() - 2 ) )
			capacityPoint.x = rect.x;
	}

	if ( capacity > 0 )
	{
		legendSizeBar.SetPos( { sizePoint.x, sizePoint.y - 30 } );
		legendSizeBar.Render( renderer );

		legendCapacityBar.SetPos( { capacityPoint.x, capacityPoint.y - 30 } ); 
		legendCapacityBar.Render( renderer );
	}

	text.Render( renderer );

	SDL_RenderPresent( renderer);

	if ( sleep )
		std::this_thread::sleep_for( milliseconds( sleepTime ) );
}
bool Container::Init( SDL_Renderer* renderer_, const std::string &fontName, int32_t fontSize )
{
	renderer = renderer_;
	if ( !InitFonts( fontName, fontSize ) )
		return false;

	InitSize();
	InitText();

	Reserve( 5 );

	return true;
}
void Container::AddObject( int32_t value )
{
	AddObject( value, size  );
}
void Container::CheckCapacity( )
{
	// Chech if we need to expand vector
	if ( size >= capacity )
	{
		text.RenderText_Solid( renderer, "Size is equal to or larger than capacity -> increase capacity" );
		Render( );
		Reserve( capacity + 5 );
	}
}
// Adds object in a given position
void Container::AddObject( int32_t value, int32_t index )
{
	RenderInsertionText( value, index );

	CheckCapacity( );

	// Start in the back and move every item one space back
	// Stops when 'index' has been moved
	for ( auto i = size - 1; i >= index ; --i )
	{
		text.RenderText_Solid( renderer, "Moving elements back" );
		std::swap( data[i], data[ i + 1 ] );
		Render();
	}

	++size;
	text.RenderText_Solid( renderer, "Inserting..." );
	Render( );
	data[ index ].RenderValue( renderer, value );
	text.RenderText_Solid( renderer, "Done!" );
	Render();
}
void Container::RenderInsertionText( int32_t value, int32_t index )
{
	if ( index != size )
	{
		std::stringstream ss;
		ss << "Inserting : " << value << " into index " << index;
		text.RenderText_Solid( renderer, ss.str() );
	}
	else
		text.RenderValue( renderer, "Pushing back : ", value );

	Render();
}
void Container::Reserve( int32_t newCapacity )
{
	if ( capacity >= newCapacity )
	{
		std::cout << "New capacity is lower than current capacity\n";
		return;
	}

	// Reserve capacity
	// Documentation : http://en.cppreference.com/w/cpp/container/vector/reserve
	data.reserve( newCapacity );

	int32_t capacityIncrease = newCapacity - capacity;
	int32_t addedItems = 0;

	capacity = newCapacity;

	// Now we have reserved the capacity, but we need to add the empty elements
	while ( addedItems < capacityIncrease )
	{
		data.push_back( CreateEmptyItem() );
		++addedItems;
	}

	Render();
}
bool Container::InitFonts( const std::string &fontName, int32_t fontSize )
{
	font = TTF_OpenFont( fontName.c_str(), fontSize );
	smallFont = TTF_OpenFont( fontName.c_str(), 25 ); 

	if ( font == nullptr )
	{
		std::cout << " Failed to load font : " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}
void Container::InitText()
{
	text.Init( smallFont, { 255, 255, 0, 255 }, { 0, 0, 0, 255 } );
	text.SetRect( { container.x + 20, container.y + itemSize.y * 2, 100, 80 } );
	text.RenderText_Solid( renderer, "Text...." );

	legendSizeBar.Init( smallFont, colorSizeBar, { 0, 0, 0, 255 } );
	legendSizeBar.SetRect( { 20, container.y + container.h + 40, 0,0 } );
	legendSizeBar.RenderText_Solid( renderer, "Size" );

	legendCapacityBar.Init( smallFont, colorCapacity, { 0, 0, 0, 255 } );
	legendCapacityBar.SetRect( { 20, container.y + container.h + 40, 0,0 } );
	legendCapacityBar.RenderText_Solid( renderer, "Capacity" );
}
void Container::InitSize()
{
	itemSize.x = TTF_FontHeight( font ) + 10;
	itemSize.y = itemSize.x;

	marginSize = { 15, 15 };

	container.w = 5 * itemSize.x + 5 * marginSize.x;
	container.h = itemSize.y + ( marginSize.y * 2 ) + 50;

	originalRect = { container.x + marginSize.x, container.y + marginSize.y, itemSize.x, itemSize.y };
}
TextRect Container::CreateItem( int32_t value )
{
	TextRect item = CreateEmptyItem();
	item.RenderValue( renderer, value );

	return item;
}
TextRect Container::CreateEmptyItem( )
{
	TextRect item;

	item.Init( font, { 255, 255, 0, 255 }, { 0, 0, 0, 255 } ); 
	item.SetOuterRect( originalRect );

	container.w += marginSize.x + itemSize.x;

	return item;
}
