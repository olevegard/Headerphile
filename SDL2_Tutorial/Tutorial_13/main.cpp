#include <iostream>
#include <string.h>

#include "TCPConnection.h"

bool ipSet = false;
bool portSet = false;
bool serverSet = false;

TCPConnection tcpConnection;
unsigned short port = 22222;
bool isServer = false;
std::string ip = "127.0.0.1";

bool Init();
bool WaitForClientToConnect();

int main( int argc, char* args[] )
{

	std::cout << "Args : \n";

	for ( int i = 1; i < argc ; ++i)
	{
		if ( argc >  i )
		{
			std::string str = args[i];

			if  ( str == "-server" )
			{
				isServer = true;
				serverSet = true;
			}
			else if ( str == "-ip" && argc > ( i + 1 ) )
			{
				ipSet = true;
				ip = args[ i + 1 ];
			}
			else if ( str == "-port" && argc > ( i + 1 ) )
			{
				portSet = true;
				port = static_cast<unsigned short >( std::stoi( args[ i + 1 ] ) );
			}
		}
	}

	std::cout << "\n\n";

	std::cout << "========== CONFIG ==========\n";
	std::cout << "Is server        : " << std::boolalpha << isServer << std::endl;
	std::cout << "IP               : " << ip << std::endl;
	std::cout << "Port             : " << port << std::endl;
	std::cout << "============================\n";

	if (!Init())
		return -1;

	return 0;
}

bool Init()
{
	if ( SDLNet_Init() < 0 )
	{
		std::cout << "Failed to init SDLNet : " << SDLNet_GetError() << std::endl;
		return false;
	}

	tcpConnection.Init(ip, port, isServer);

	if (isServer)
		WaitForClientToConnect();

	int quitCountDown = 3;
	while ( tcpConnection.IsConnected() )
	{
		tcpConnection.Send("Hello");
		std::cout <<"Sending Hello\n";

		//if (tcpConnection.CheckForActivity())
		{
			std::string msg = tcpConnection.ReadMessages();
			std::cout << "Receiving : " << msg << std::endl;

			if (quitCountDown == 0 && tcpConnection.IsServer())
				tcpConnection.Close();

			--quitCountDown;
			SDL_Delay(1500);
		}
	}

	std::cout << "Connection closed\n";
	return true;
}

bool WaitForClientToConnect()
{
	while (!tcpConnection.IsConnected())
	{
		tcpConnection.TryToOpenConnectionToClient();
		SDL_Delay(100);
	}

	return true;
}
/*

   void NetManager::Init( bool server)
   {
   isServer = server;
   isReady = false;

   mainServer.Init( "127.0.0.1", 3113, false );
   }
   void NetManager::Connect( std::string ip, unsigned short port )
   {
   ipAdress = ip;
   portNr = port;
   isReady = true;

// Temporary code to give the server some time to start up...
while ( !gameClient.Init( ip, port, false ) );
}
}
void NetManager::Close()
{
isReady = false;
gameServer.Close();
}
void NetManager::Update()
{
if ( !isReady )
return;

if ( !isServer || gameServer.IsConnected()  )
{
return;
}
gameServer.Update();
}
std::string NetManager::ReadMessage( )
{
if ( isServer )
return gameServer.ReadMessages();
else
return gameClient.ReadMessages();
}
std::string NetManager::ReadMessageFromServer( )
{
return mainServer.ReadMessages();
}
void NetManager::SendMessage( std::string str )
{
if ( isServer )
{
gameServer.Send( str );
}
else
{
gameClient.Send( str );
}
}
*/

