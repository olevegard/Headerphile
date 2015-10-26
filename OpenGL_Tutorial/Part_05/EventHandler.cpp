#include "EventHandler.h"

void EventHandler::Update( )
{
	SDL_Event event;
	while ( SDL_PollEvent( &event ) )
	{
		switch ( event.type )
		{
			case SDL_QUIT :
				AddQuitEvent();
				break;
			case SDL_KEYDOWN : 
			case SDL_KEYUP :
				HandleKeyBoard( event );
				break;
			case SDL_MOUSEBUTTONDOWN :
			case SDL_MOUSEBUTTONUP :
				HandleMouseButton( event );
				break;
			case SDL_MOUSEMOTION :
				HandleMouseMove( event );
				break;
			default :
				break;
		}
	}
}
void EventHandler::AddQuitEvent()
{
	quit = true;
	events.emplace_back( Event( EventType::Quit ) );
}
void EventHandler::HandleKeyBoard( const SDL_Event &event )
{
	ButtonState state = ConvertButtonStateFromSDL( event.type, EventType::Keyboard );

	if ( state != keyCode[ event.key.keysym.sym ] )
	{
		// Save new state of the button
		keyCode[ event.key.keysym.sym ] = state;

		AddKeyboardEvent( event );

		if ( event.key.keysym.sym == SDLK_ESCAPE )
			AddQuitEvent();
	}
}
void EventHandler::HandleMouseButton( const SDL_Event &event )
{
	MouseButton button = ConvertMouseButtonToEnum( event.button.button );
	ButtonState state = ConvertButtonStateFromSDL( event.type, EventType::MouseButton );

	if ( state != mouseButton[ button ] )
	{
		mouseButton[ button ] = state;
		AddMouseEvent( event );
	}
}
void EventHandler::HandleMouseMove( const SDL_Event &event )
{
	Event newEvent( EventType::MouseMotion );
	newEvent.mouseMove.newPos.x = event.motion.x;
	newEvent.mouseMove.newPos.y = event.motion.y;

	newEvent.mouseMove.relativePos.x = event.motion.xrel;
	newEvent.mouseMove.relativePos.y = event.motion.yrel;

	events.push_back( newEvent );

	mousePoint = newEvent.mouseMove.newPos;
}
ButtonState EventHandler::GetKeyState( SDL_Keycode key ) const
{
	ButtonState state;

	try
	{
		state = keyCode.at( key );
	}
	catch ( std::out_of_range &e )
	{
		state = ButtonState::Up;
	}

	return state;
}
bool EventHandler::IsKeyDown( SDL_Keycode key ) const
{
	return GetKeyState( key ) == ButtonState::Down;
}
ButtonState EventHandler::GetMouseButtonState(  MouseButton button ) const
{
	ButtonState buttonState;

	try
	{
		buttonState = mouseButton.at( button );
	}
	catch ( std::out_of_range &e )
	{
		buttonState = ButtonState::Up;
	}

	return buttonState;
}
bool EventHandler::IsMouseButtonDown( MouseButton button ) const
{
	return GetMouseButtonState( button ) == ButtonState::Down;
}
SDL_Point EventHandler::GetMousePoint( ) const
{
	return mousePoint;
}
void EventHandler::AddKeyboardEvent( const SDL_Event &event )
{
	events.push_back( CreateKeyboardEvent( event ) );
}
void EventHandler::AddMouseEvent( const SDL_Event &event )
{
	events.push_back( CreateMouseEvent( event ) );
}
Event EventHandler::CreateKeyboardEvent( const SDL_Event &event ) const
{
	Event keyEvent( EventType::Keyboard );
	keyEvent.keyboard.key = event.key.keysym.sym; 

	if ( event.type == SDL_KEYUP )
		keyEvent.keyboard.eventType = ButtonEventType::Released;
	else
		keyEvent.keyboard.eventType = ButtonEventType::Pressed;

	return keyEvent;
}
Event EventHandler::CreateMouseEvent( const SDL_Event &event ) const
{
	Event mouseEvent( EventType::MouseButton );
	mouseEvent.mouseButton.button = ConvertMouseButtonToEnum( event.button.button );

	if ( event.type == SDL_MOUSEBUTTONUP )
		mouseEvent.mouseButton.eventType = ButtonEventType::Released;
	else
		mouseEvent.mouseButton.eventType = ButtonEventType::Pressed;

	return mouseEvent;
}
std::vector< Event > EventHandler::GetEvents()
{
	return events;
}
void EventHandler::ClearEvents()
{
	//keyPresses.clear();
	events.clear();
}
MouseButton EventHandler::ConvertMouseButtonToEnum( uint8_t SDLButtonID ) const
{
	switch ( SDLButtonID )
	{
		case 1 :
			return MouseButton::Left;
		case 2 :
			return MouseButton::Middle;
		case 3 :
			return MouseButton::Right;
		default:
			break;
	}

	return MouseButton::Unknown;
}
uint8_t EventHandler::ConvertMouseButtonToID( MouseButton button ) const
{
	switch ( button )
	{
		case MouseButton::Left:
			return 1;
		case MouseButton::Middle:
			return 2;
		case MouseButton::Right:
			return 3;
		default:
			break;
	}

	return -1;
}
ButtonState EventHandler::ConvertButtonStateFromSDL ( uint32_t keyEventID, EventType type ) const
{
	ButtonState state;

	if ( type == EventType::MouseButton )
	{
		if ( keyEventID == SDL_MOUSEBUTTONDOWN )
			state = ButtonState::Down;
		else
			state = ButtonState::Up;
	}
	else if ( type == EventType::Keyboard )
	{
		if ( keyEventID == SDL_KEYDOWN )
			state = ButtonState::Down;
		else
			state = ButtonState::Up;
	}

	return state;
}
uint32_t EventHandler::ConvertButtonStateToSDL ( ButtonState button, EventType type ) const
{
	if ( type == EventType::MouseButton )
	{
		if ( button == ButtonState::Down)
			return SDL_MOUSEBUTTONDOWN;
		else
			return SDL_MOUSEBUTTONUP;
	}

	return SDL_MOUSEBUTTONUP;
}
