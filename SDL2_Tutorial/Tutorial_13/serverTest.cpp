#include <iostream>
#include <sstream>

#include "UDPConnection.h"

 
UDPConnection udpConnection;

int main(int argc, char **argv)
{
	udpConnection.Init( argv[1], atoi( argv[2] ) );
	udpConnection.CheckForData();


	/*
	// Check for parameters
	if (argc < 3)
	{
		return EXIT_FAILURE; 
	}
 
	// Initialize SDL_net
	if ( SDLNet_Init() < 0 )
	{
		std::cout << "SDLNet_Init failed : " << SDLNet_GetError() << std::endl;
		return EXIT_FAILURE; 
	}
 
	if ( !OpenPort( 0 ) )
		return EXIT_FAILURE;

	if ( !SetIPAndPort( "127.0.0.1", 2000 ) )
		return EXIT_FAILURE; 

	if ( !CreatePacket( 512 ) )
		return EXIT_FAILURE; 

	Send( "quit" );
 
	SDLNet_FreePacket(packet);
	SDLNet_Quit();
 
	return EXIT_SUCCESS;
	*/
}
