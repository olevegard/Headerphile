// NetManager - Headerphile.com
//
// A simple class that handles TCP Connections
//
// Has functionality for handling both server and client connections
// Can also be used for sending a message to all client connections,
// and retrieving a vector of newly received messages
//

#pragma once

#include <SDL2/SDL_net.h>
#include <vector>

#include "ServerTCPConnection.h"
#include "ClientTCPConnection.h"

class NetManager
{
	public:
		NetManager()
		{
		}

		bool Init()
		{
			clientSocketSet= SDLNet_AllocSocketSet( 10 );
			serverSocketSet = SDLNet_AllocSocketSet( 10 );

			return true;
		}

		// Add a new client and try to connect to a server
		bool AddClient(const std::string &ip, int32_t port)
		{
			std::unique_ptr<ClientTCPConnection>
				clientCon( new ClientTCPConnection(ip, port) );

			if (!clientCon->SetupIPAddress())
				return false;

			if (!clientCon->OpenConnectionToServer())
				return false;

			clientCon->AddToSocketSet( &clientSocketSet );

			clientConnections.emplace_back(std::move(clientCon));

			return true;
		}

		// Add server for listening for new connectios
		bool AddServer(int32_t port)
		{
			std::unique_ptr<ServerTCPConnection>
				serverCon( new ServerTCPConnection(port) );

			if (!serverCon->SetupIPAddress())
				return false;

			if (!serverCon->OpenPortForListening())
				return false;

			serverCon->AddToSocketSet(&serverSocketSet);

			// This is a bit tricky move-semantcs stuff.
			// Basically ; we add serverCon to serverConnections (wihtou copying it)
			serverConnections.emplace_back(std::move(serverCon));

			return true;
		}

		// Send a message to all connected clients
		void SendToAll(const std::string &message)
		{
			for ( auto p = std::begin(clientConnections) ; p != std::end(clientConnections) ; ++p )
				(*p)->Send(message);
		}

		// Return a vector of all messages received since last time
		// NOTE : Here you'd probably want a more effective way of returning objects
		// But this is beyond the scope of this simple implmentation
		std::vector<std::string> GetAllMessages()
		{
			std::vector<std::string> messageListCopy = messageList;

			messageList.clear();
			return messageListCopy;
		}

		void DoUpdate()
		{
			TryToAcceptConnection();
			CheckClientConnections();

			SDL_Delay(50);
		}

		// Check if any clients has acitivy
		// If so ; loop through the client connections until we have checked all connections that had activity
		void CheckClientConnections()
		{
			int32_t countActiveConnections = CountClientSocketsWithActivity();

			if ( countActiveConnections == 0 )
				return;

			std::cout << "[NET MANAGER] Client activity : " << countActiveConnections << std::endl;

			int i = 0;
			while (countActiveConnections != 0)  
			{
				// Check if this connectio has activity, which means it's either disconnect or has received a message
				if (!clientConnections[i]->CheckForActivity())
				{
					++i;
					continue;
				}

				--countActiveConnections;

				if ( !clientConnections[i]->IsConnected() )
				{
					// Delete connection witht the index i
					clientConnections.erase( std::begin(clientConnections) + i );
					++i;

					std::cout << "Client has disconnected!\n";
					continue;
				}

				messageList.push_back(clientConnections[i]->ReadMessage() );

				// Even if we have checked all connections, something might have happened since last time
				if (countActiveConnections ==0)
				{
					countActiveConnections = CountClientSocketsWithActivity();
					i = 0;
				}
			}
		}
	
		int CountConnectedClients()
		{
			return clientConnections.size();
		}

		int CountServerSocketsWithActivity()
		{
			return SDLNet_CheckSockets( serverSocketSet, 5 );
		}

		int CountClientSocketsWithActivity()
		{
			return SDLNet_CheckSockets( clientSocketSet, 5 );
		}

		// (Server only)
		// Try to accept a client that's trying to connect
		// This will loop through all server connection, but not client connections
		bool TryToAcceptConnection()
		{
			int count = CountServerSocketsWithActivity();

			if ( count > 0 )
				std::cout << "[NET MANAGER] " << count << " servers connections with activity\n";

			int countFound = 0;

			for (uint32_t i = 0 ; i < serverConnections.size() ; ++i)
			{
				if ( countFound == count)
					break;

				if (!serverConnections[i]->CanAcceptConnection())
					continue;

				// Now we know that there is a connection waiting for us, so let's accept it!
				auto newConnection
					=  serverConnections[i]->TryToAcceptConnection();

				if (newConnection != nullptr)
				{
					newConnection->AddToSocketSet( &clientSocketSet );
					clientConnections.emplace_back(std::move(newConnection));
				}
			}

			return true;
		}

	private:
		// The socket sets can be used to quickly check all connections
		SDLNet_SocketSet serverSocketSet;
		SDLNet_SocketSet clientSocketSet;

		std::vector<std::string> messageList;

		// I use unique pointers here. If you want to read more, you can check them out on my blog
		// Otherwise ; just think of them as regular pointers
		std::vector<std::unique_ptr<ServerTCPConnection> > serverConnections;
		std::vector<std::unique_ptr<ClientTCPConnection> > clientConnections;
};
