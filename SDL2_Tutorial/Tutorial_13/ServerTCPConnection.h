// ServerTCPConnection - headerphile.com
//
// This is just as simple wrappaer around TCPsocket, It is meant to be used with NetManager
//
// The class handles server conenctions, which are just used to listen for new connections. They are never connected to another client/computer!
// When a client connects, a new client connection will be created
// 		The connection is used for communicating
// 		The other connection is for listening for new connections ( and will always be until it is closed )
//
#pragma once

#include <string>
#include <memory>
#include <iostream>

#include <SDL2/SDL_net.h>

#include "ClientTCPConnection.h"

class ServerTCPConnection
{
	public:

	ServerTCPConnection( int32_t port_ )
	{
		port = port_;
	}

	// Initialization
	// ================================================================
	bool SetupIPAddress( )
	{
		std::cout << "[SERVER] Resolving host : " << port << std::endl;

		// Since this is the server connection, we supply nullptr instead of a valid IP
		// This sets the IP value in ipAddress to INADDR_NONE
		int success = SDLNet_ResolveHost( &ipAddress, nullptr, port );

		if ( success == -1 )
		{
			std::cout << "[SERVER] Failed to open port : " << port << std::endl;

			return false;
		}

		std::cout << "[SERVER] Host resolved" << std::endl;
		return true;
	}

	bool OpenPortForListening( )
	{
		std::cout << "[SERVER] Opening connection "
			<< "\n\tHost name : " << " : " << port
			<< std::endl;

		// Above we sat the IP of the ipAddress struct to INADDR_NONE.
		// When we supply that to SDLNet_TCP_Open,
		// it means it should listen for connections rather than trying to initiate one
		tcpSocket = SDLNet_TCP_Open( &ipAddress );

		if ( tcpSocket == nullptr )
		{
			std::cout << "[SERVER] Failed to open port : " << port
				<<" \n\tError : " << SDLNet_GetError()
				<< std::endl;

			return false;
		}

		std::cout << "[SERVER] Success! Port opened for listening!\n";

		return true;
	}

	// Accepting client connections
	// ================================================================

	// Try to accept a client that's trying to connect
	std::unique_ptr<ClientTCPConnection> TryToAcceptConnection()
	{
		if (!CanAcceptConnection())
			return nullptr;

		// Tries to accept incoming TCP connection.
		// If there are no connections incoming, we get null back
		TCPsocket newSocket = SDLNet_TCP_Accept( tcpSocket );

		// No connection was accepted. This either means none is trying to connect, or that there were an error.
		if ( newSocket == nullptr )
		{
			std::cout << "[SERVER] Cannot accept TCP connection : " << SDLNet_GetError() << std::endl;
			return nullptr;
		}

		std::unique_ptr<ClientTCPConnection> connection( new ClientTCPConnection(newSocket));

		std::cout << "Connection accepted!\n";

		return connection;
	}

	// Helper functions
	// ================================================================

	// This makes it easy for us to listen to the socket for activity
	bool CheckForActivity()
	{
		return SDLNet_SocketReady( tcpSocket ) != 0;
	}

	// Add the private tcpSocket to a SDLNet_SocketSet
	void AddToSocketSet( SDLNet_SocketSet* socketSet )
	{
		SDLNet_TCP_AddSocket( *socketSet, tcpSocket );
	}

	void Close()
	{
		std::cout << "[SERVER] Closing connection.." << std::endl;

		SDLNet_TCP_Close( tcpSocket );
	}

	// Returns true if there is any action on this connection
	// (Which means there is a client connection we can accept)
	bool CanAcceptConnection()
	{
		if ( tcpSocket == nullptr )
		{
			std::cout <<  "[SERVER] Can't accept, TCP Socket is null" << std::endl;
			return false;
		}

		bool canAccept = CheckForActivity();

		if ( canAccept )
			std::cout << "[SERVER] Server socket action!\n";
		else
			std::cout << "[SERVER] No server socket action!\n";

		return canAccept;
	}

	private:

	int32_t port;

	IPaddress ipAddress;
	TCPsocket tcpSocket;
};
