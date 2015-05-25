// Client TCP Connection - headerphile.com
//
// This is just as simple wrappaer around TCPsocket,it is meant to be used with NetManager
//
// It only handles cliwnt connections
// ( which means it can only be used to communicate with a server, but not accept incomming connections)
//
// It can be created by opening a connection to a remote server ( trying to connect to it )
// Or by a server when accepting a new connection. This is the result on the server side after a client tries to connect to it
//

#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <SDL2/SDL_net.h>

class ClientTCPConnection
{
	public:

	// This is used on the client side. It tries to connect to the server with the ip and port below
	ClientTCPConnection( const std::string &ip_, int32_t port_ )
	{
		ip = ip_;
		port = port_;
	}

	// This is used on the server / receiving side. When we accept a connection, we get a TCPsocket in return
	// We use the below constructor to create a connection from that new TCPsocket
	ClientTCPConnection( const TCPsocket &conn)
	{
		tcpSocket = conn;
		isConnected = true;
	}

	// Initialization
	// ================================================================
	bool SetupIPAddress( )
	{
		std::cout << "[CLIENT] Resolving host " << ip << " : " << port << std::endl;

		// Set up the IPAddress struct so that it has the correct IP and port to the server
		// As stated before, this ONLU sets up the IPaddress. It doesn't try to connect to a server
		int success = SDLNet_ResolveHost( &ipAddress, ip.c_str(), port );

		if ( success == -1 )
		{
			std::cout << "[CLIENT] Failed to resolve host " << ip << " : " << port << std::endl;
			return false;
		}

		std::cout	<< "[CLIENT] Host resolved" << std::endl;
		return true;
	}

	bool OpenConnectionToServer( )
	{
		std::cout << "[CLIENT] Opening connection "
			<< "\n\tHost name : " << ip << " : " << port
			<< std::endl;

		// Try to open the connection to the server. This means the server has to accept the connection
		// ( Which we do using SDLNet_TCP_Accept)
		tcpSocket = SDLNet_TCP_Open( &ipAddress );

		if ( tcpSocket == nullptr )
		{
			std::cout << "[CLIENT] Failed to open port " << ip << " : " << port
				<<" \n\tError : " << SDLNet_GetError()
				<< std::endl;

			return false;
		}

		isConnected = true;
		std::cout << "[CLIENT] Success!\n";

		return true;
	}

	void Send( std::string str )
	{
		// Cast our std::string to void* so that SDL_net can understand it properly
		void* messageData = ConvertStringToVoidPtr(str);
		int messageSize = static_cast< int > ( str.length() );

		int bytesSent = bytesSent = SDLNet_TCP_Send( tcpSocket,  messageData,  messageSize);

		std::cout << "Trying to send " << str << "\tsent : " << bytesSent << std::endl;
		if ( bytesSent < messageSize )
		{
			std::cout << "\tSend failed : " << SDLNet_GetError() << std::endl;
		}
	}

	std::string ReadMessage()
	{
		// Allocate a char buffer to hold the message for us
		char buffer[bufferSize ];
		memset( buffer, 0, bufferSize );

		// Try to receive data
		// NOTE: Receiving data involves a little C style pointer manipulation.
		// I'll try to find a cleaner, less C-like way to do it.
		int byteCount = SDLNet_TCP_Recv( tcpSocket, buffer, bufferSize );
		std::string received("");

		std::cout << "[CLIENT] Received : " << byteCount << " bytes\n";

		// Success! We received something ...
		if ( byteCount > 0 )
		{
			// Set the last character to '\0' which means "end of stirng"
			buffer[byteCount] = '\0';
			received = buffer;

			// If we received more data than our buffer can hold, we will only return the bufferSize first bytes.
			if ( byteCount >= bufferSize )
				std::cout << "[CLIENT] Too much data received : " << byteCount
					<< "\tbuffer size : " << bufferSize
					<< "\n\tMaybe increase buffer size?" << std::endl;
		}
		// A bytecount of 0 means the connection has been terminated
		else if ( byteCount == 0 )
		{
			isConnected = false;
			std::cout << "[CLIENT] Connection terminated : " << std::endl;
		}
		// A bytecount of < 0 means an error occured
		else if ( byteCount < 0 )
		{
			isConnected = false;
			std::cout << "[CLIENT] Read Failed, terminating connection : " << SDLNet_GetError()  << std::endl;
		}

		return received;
	}

	// Helper functions
	// ================================================================

	// This makes it easy for us to listen to the socket for activity
	bool CheckForActivity()
	{
		int i = SDLNet_SocketReady( tcpSocket );
		std::cout << "Activity : " << i << std::endl;
		return  i != 0;
	}

	// Add the private tcpSocket to a SDLNet_SocketSet
	void AddToSocketSet( SDLNet_SocketSet* socketSet )
	{
		SDLNet_TCP_AddSocket( *socketSet, tcpSocket );
	}

	void Close()
	{
		isConnected = false;
		std::cout << "[CLIENT] Closing connection.." << std::endl;
		SDLNet_TCP_Close( tcpSocket );
	}

	bool IsConnected()
	{
		return isConnected;
	}
	private:

	// C-style pointer manipulation thingy. Will try to improve this
	void* ConvertStringToVoidPtr( const std::string &str )
	{
		const char* charPtr = str.c_str();

		return const_cast< char* > ( charPtr );
	}

	bool isConnected;

	std::string ip;
	int32_t port;

	int32_t waitTime = 60000;

	// This is really small to show the messages being split up or joined together
	// This should really be larger, but feel free to experiment with it
	const int bufferSize = 10;

	// Holds the adress and port to the server
	// NOTE: If this object was created via a server connection, it will be null
	// (which is okay since we just use it to try to connect to the server anyways )
	IPaddress ipAddress;

	TCPsocket tcpSocket;
};


