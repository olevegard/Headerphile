#include <iostream>
#include <sstream>

#include "UDPConnection.h"

 
UDPConnection udpConnection;

int main(int argc, char **argv)
{
	std::string IP;
	int32_t localPort = 0;
	int32_t remotePort = 0;

	std::cout 
		<< "\n==========================================================================================================\n"
		<< "UDP connection - A simple test for UDP connections using SDL_Net!"
		<< "\n==========================================================================================================\n"
		<< "You'll be asked to enter the following :"
		<< "\n\tRemote IP   : The IP you want to connect to"
		<< "\n\tRemote Port : The port you want to connect to"
		<< "\n\tLocal port  : Uour port"
		<< "\nLocal port should be the same as remote port on the other instance of the application"
		<< "\n==========================================================================================================\n\n";

	std::cout << "Enter remote IP ( 127.0.0.1  for local connections ) : ";
	std::cin >> IP;
	std::cout << "...and remote port : ";
	std::cin >> remotePort;

	std::cout << "Enter local port : ";
	std::cin >> localPort;

	udpConnection.Init( IP, remotePort, localPort );


	uint8_t command = 0;

	while ( !udpConnection.WasQuit() )
	{
		std::cout
			<< "Your command : "
			<< "\n\t0 : Send \'test\'"
			<< "\n\t1 : Quit"
			<< "\n\t2 : Check for data"
			<< std::endl;

		std::cin >> command;
		
		if ( command == '0' )
			udpConnection.Send( "test" );
		else if ( command == '1' ) 
			udpConnection.Send( "quit" );
		else if ( command == '2' ) 
			udpConnection.CheckForData();
		else
			std::cout << "Illegal command\n";
	}

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
