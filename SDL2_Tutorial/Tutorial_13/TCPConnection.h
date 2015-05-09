#pragma once

#include <string>
#include <iostream>

#include <SDL2/SDL_net.h>

class TCPConnection
{
public:
	TCPConnection()
	:	isConnected( false )
	,   bufferSize( 80000 )
	{
	}

	bool IsConnected() const
	{
		return isConnected;
	}

	bool IsServer() const
	{
		return isServer;
	}


	bool Init( const std::string &host, unsigned short port, bool server )
	{
		isConnected = false;

		isServer = server;
		hostName = host;
		portNr = port;
		socketSet = SDLNet_AllocSocketSet( 1 );

		if ( !ResolveHost() )
			return false;

		if ( !OpenConnectionToHost() )
		{
			std::cout << "Opening connection to host faield : " << SDLNet_GetError() << std::endl;
			return false;
		}

		if( !server)
			WaitForConnection();

		return true;
	}

	void WaitForConnection()
	{
		SDLNet_CheckSockets( socketSet, 10000 );
		std::cout << "Connected\n";
		isConnected = true;
	}

	bool ResolveHost()
	{
		int success = 0;
		std::string serverOrClient;

		if ( isServer )
		{
			// IP is null becasue this is a server
			// Port is the port we listen to for incoming connections
			success = SDLNet_ResolveHost( &ipAddress, nullptr, portNr );
			std::cout << "Waiting for client to connect on port : " << portNr << std::endl;
		}
		else
		{
			// IP is the IP of the server
			// Port is the port the serer listens on ( see above )
			success = SDLNet_ResolveHost( &ipAddress, hostName.c_str(), portNr );
			std::cout << "Resolving server : " << hostName << " : " << portNr << std::endl;
		}

		if ( success == -1 )
		{
			std::cout <<  "Failed!"
				<< "\n\tServer? " << std::boolalpha << isServer
				<< "\n\tHostname " << hostName << " : " << portNr << std::endl;

			return false;
		}

		std::cout	<< "Host resolved" << std::endl;
		return true;
	}

	bool OpenConnectionToHost( )
	{
		// Open connection
		// 	Client :
		// 		Try to connect to server
		// 		At this point, server must already be running and accept the connection
		// 	Server
		// 		We initialized ipAdress using SDLNet_ResolveHost() with nullptr as the hostname
		// 		This means it'll have host = INADDR_ANY and that means it will just open to port so it can listen to connections
		tcpSocket = SDLNet_TCP_Open( &ipAddress );

		// Add the socket to our SocketSet so that we can check it with SDLNet_CheckSockets()
		if ( !isServer )
			SDLNet_TCP_AddSocket( socketSet, tcpSocket );

		if ( tcpSocket == nullptr )
		{
			std::cout << "Failed to connect!"
				<< "\n\tServer? " << std::boolalpha << isServer
				<< "\n\tHost name : " << hostName << " : " << portNr
				<<" \n\tError : " << SDLNet_GetError()
				<< std::endl;

			return false;
		}

		if (!isServer)
			// Since nothing failed (tcpSocket is valid), we now have a valid connection to the server
			std::cout << "Connection opened : " << hostName << ":" << portNr << std::endl;
		else
			// Nothing failed so now all we can do is to wait for any clients trying to connect....
			// No actual connection has been established yet
			std::cout << "Port opened for listening : " << hostName << ":" << portNr << std::endl;

		return true;
	}

	bool CheckForActivity() const
	{
		// Verify that we are connected
		if ( !isConnected )
			return false;

		// Check if there is any new packets for us
		int countReady = SDLNet_CheckSockets( socketSet, 0 );

		if ( countReady < 0 )
		{
			std::cout << "Sockets with activity count is negative! : " << SDLNet_GetError() << std::endl;
			return false;
		}

		if ( isServer )
			return SDLNet_SocketReady( socketToClient ) != 0;
		else
			return SDLNet_SocketReady( tcpSocket ) != 0;
	}

	void Send( std::string str )
	{
		if ( !isConnected  )
			return;

		void* messageData = ConvertStringToVoidPtr(str);
		int messageSize = static_cast< int > ( str.length() );
		int bytesSent = 0;

		if ( isServer )
			bytesSent = SDLNet_TCP_Send( socketToClient,  messageData,  messageSize);
		else
			bytesSent = SDLNet_TCP_Send( tcpSocket,  messageData,  messageSize);

		if ( bytesSent < messageSize )
		{
			std::cout << "Send failed : " << SDLNet_GetError() << std::endl;
			Close();
		}
	}

	std::string ReadMessages()
	{
		if ( !isConnected || !CheckForActivity() )
			return "";

		char buffer[bufferSize ];
		memset( buffer, 0, bufferSize );

		int byteCount  = 0;
		std::string received("");

		if ( isServer )
			byteCount = SDLNet_TCP_Recv( socketToClient, buffer, bufferSize );
		else
			byteCount = SDLNet_TCP_Recv( tcpSocket, buffer, bufferSize );

		if ( byteCount > 0 )
		{
			buffer[byteCount] = '\0';
			received = buffer;

			if ( byteCount >= bufferSize )
				std::cout << " Too much data received : " << byteCount << " buffer size : " << bufferSize << std::endl;
		}
		// A bytecount of 0 means the connection has been terminated
		else if ( byteCount == 0 )
		{
			std::cout << "Connection terminated : " << std::endl;
			Close();
		}
		// A bytecount of < 0 means an error occured
		else if ( byteCount < 0 )
			std::cout << "Read Failed : " << SDLNet_GetError()  << std::endl;

		return received;
	}

	// (Server only)
	// Try to establish connection to a client
	void TryToOpenConnectionToClient()
	{
		if ( !isServer || isConnected )
			return;

		if ( TryToAcceptConnection() )
		{
			if ( SetServerSocket() )
			{
				// Add the socket to our SocketSet so that we can check it with SDLNet_CheckSockets()
				SDLNet_TCP_AddSocket( socketSet, socketToClient );
				isConnected = true;
			}
		}
	}

	// (Server only)
	// Try to accept a client that's trying to connect
	bool TryToAcceptConnection()
	{
		if ( !isServer || tcpSocket == nullptr )
		{
			std::cout <<  "Can't accept, TCP Socket is null" << std::endl;
			return false;
		}

		// Tries to accept incoming TCP connection.
		// If there is no connections incoming, we get null back
		socketToClient = SDLNet_TCP_Accept( tcpSocket );

		// No connection was accepted. This either means none is trying to connect, or that there were an error.
		if ( socketToClient  == nullptr )
		{
			std::cout << " Cannot accept TCP connection : " << SDLNet_GetError() << std::endl;
			isConnected = false;
			return false;
		}

		std::cout << "Connection accepted!\n";

		return true;
	}

	// (Server only)
	// We have accepted the connection of a client, now we need to find the IP and port of that client
	bool SetServerSocket()
	{
		// Try to find ipAdress of the client
		IPaddress* ipRemote = SDLNet_TCP_GetPeerAddress( socketToClient );

		// Unable to get the IPadress, this can happen if
		if ( ipRemote == nullptr )
		{
			std::cout << "Can't get client adress " << hostName << " : " << portNr << "\n\tServer : " << std::boolalpha << isServer;
			isConnected = false;

			return false;
		}

		// Everything went fine, and we can now communicate
		std::cout << "Host connected " << ipRemote->host << " : " << ipRemote->port << std::endl;

		isConnected = true;
		return true;
	}

	// Clean up...
	void Close()
	{
		std::cout << "Closing connection.." << std::endl;
		if ( !isConnected )
		{
			std::cout <<  "Can't close : not connected" << std::endl;
			return;
		}

		if ( isServer )
		{
			SDLNet_TCP_DelSocket( socketSet, socketToClient );
			SDLNet_TCP_Close( socketToClient );
			SDLNet_FreeSocketSet( socketSet );
		}
		else
		{
			SDLNet_TCP_Close( tcpSocket );
		}

		isConnected = false;
	}

private:
	void* ConvertStringToVoidPtr( const std::string &str )
	{
		const char* charPtr = str.c_str();

		return const_cast< char* > ( charPtr );
	}

	bool isServer;
	std::string hostName;
	unsigned short portNr;
	bool isConnected;
	IPaddress ipAddress;
	const int bufferSize;

	TCPsocket tcpSocket;
	TCPsocket socketToClient;
	SDLNet_SocketSet socketSet;
};

