// TCP Tutorial - Ole Vegard
//
// This is just a simple "driver" for NetManager
//
// Compile ( Linux / Mac or Windows using gcc / clang )
// 
//	clang :
//		clang++ main.cpp -std=c++11 -lSDL2 -lSDL2_net -o TCPTest
//
//	gcc
//		g++ main.cpp -std=c++11 -lSDL2 -lSDL2_net -o TCPTest

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "NetManager.h"

bool portSet = false;

// Variables for our connection ( set from command line )
unsigned short port = 12312;
std::string ip = "127.0.0.1";
bool isServer = false;

NetManager netMan;

void Start();
void PrintStats();
void SendMessage();
void StartUpdate();
void GetSettingsFromUser();
void PrintAllMessages( const std::vector<std::string> &v);

int main( )
{
	GetSettingsFromUser();

	PrintStats();

	Start();

	return 0;
}
void GetSettingsFromUser()
{
	std::cout 
		<< "\n==========================================================================================================\n"
		<< "TCP connection - A simple test for TCP connections using SDL_Net!"
		<< "\n==========================================================================================================\n"
		<< "You'll be asked to enter the following :"
		<< "\n\tServer   :  Is the connection we are setting up a server connection? (y/n)"
		<< "\n\tIP   : The IP of the client ( not for servers )"
		<< "\n\tPort : The port you want to listen or connect to"
		<< "\n==========================================================================================================\n\n";

	std::cout << "Is this a server? ( Type \'y\' for server, otherwise \'n\' and hit enter) : \n";
	char isServerInput = 'n';
	std::cin >> isServerInput;
	isServer = (isServerInput == 'y');

	// Servers don't need IP
	if (!isServer)
	{
		std::cout << "Enter remote IP ( 127.0.0.1  for local connections ) : \n";
		std::cin >> ip;
	}

	std::cout << "Enter the port number ( this needs to be the same for server and client ) : \n";
	std::cin >> port;
}

void Start()
{
	if (!netMan.Init())
		return;

	if (isServer)
		// Server just listens for connection
		// A ClientTCPConnection will be created when a client tries to connect
		netMan.AddServer( port );
	else
		// Clients just try to connect to the server and hope for the best
		netMan.AddClient( ip, port);

	StartUpdate();
}

void StartUpdate()
{
	bool quit = false;

	std::cout << "=====================================================\n";

	while (!quit)
	{
		std::cout << "Choose one and hit enter : "
			<< "\n\ts -> send message"
			<< "\n\tr -> read message(s)"
			<< "\n\tu -> plain update ( do this to check for new connections"
			<< "\n\tq -> quit"
			"\nYou choice : ";

		char choice = '-';
		std::cin >> choice;

		switch (choice)
		{
			case 's':
				SendMessage();
				break;
			case 'r':
				netMan.DoUpdate();
				PrintAllMessages(netMan.GetAllMessages());
				break;
			case 'u':
				netMan.DoUpdate();
				break;
			case 'q':
				quit = true;
				break;
			default:
				std::cout << "Doing nothing...\n";
				break;
		}

	}

	std::cout << "Connection terminated\n";
}
void SendMessage()
{
	std::cout << "Type your message and hit enter : " << std::endl;
	std::string msg = "[EMPTY MESSAGE]";
	std::cin.ignore();
	std::getline(std::cin, msg);

	netMan.SendToAll(msg);
}
void PrintAllMessages( const std::vector<std::string> &v)
{
	std::cout << "=================== Received Messages : " << v.size() << " ===================\n";

	for (auto p = std::begin(v) ; p != std::end(v) ; ++p)
		std::cout << "Message : " << *p << std::endl;

	std::cout << "=========================================================\n";
}

void PrintStats()
{
	std::cout << "\n\n";
	std::cout << "========== CONFIG ==========\n";
	std::cout << "Is server        : " << std::boolalpha << isServer << std::endl;
	std::cout << "IP               : " << ip << std::endl;
	std::cout << "Port             : " << port << std::endl;
	std::cout << "============================\n";
}

