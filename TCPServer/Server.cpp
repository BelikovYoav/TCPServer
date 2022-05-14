#include "Server.h"

//CONSTRUCTOR AND DESTRUCTOR
Server::Server()
{
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == listenSocket)
	{
		closesocket(listenSocket);
		cout << "Server: Error at socket(): " << WSAGetLastError() << endl;
		throw exception();
	}

}

Server::~Server()
{
	closesocket(listenSocket);
}

//OTHER
void Server::runServer(int listenPort)
{
	init(listenPort);
	handleConnections();
	closesocket(listenSocket);
}

void Server::init(int listenPort)
{
	serverService.sin_family = AF_INET;
	serverService.sin_addr.s_addr = INADDR_ANY;
	serverService.sin_port = htons(listenPort);

	if (SOCKET_ERROR == bind(listenSocket, (SOCKADDR*)&serverService, sizeof(serverService)))
	{
		closesocket(listenSocket);
		cout << "Server: Error at bind(): " << WSAGetLastError() << endl;
		throw exception();
	}

	if (SOCKET_ERROR == listen(listenSocket, 5))
	{
		closesocket(listenSocket);
		cout << "Server: Error at listen(): " << WSAGetLastError() << endl;
		throw exception();
	}

	addSocket(listenSocket, receiveStatus::LISTEN);
}

void Server::handleConnections()
{
	int nfd = 0;

	while (true)
	{
		fd_set waitRecv, waitSend;
		nfd = getNFD(&waitRecv, &waitSend);
		cout << nfd << endl;
		handleReceiveSockets(&nfd, &waitRecv);
		handleSendSockets(&nfd, &waitSend);
	}
}

bool Server::addSocket(SOCKET socket, receiveStatus recv)
{
	bool added = false;
	for (int i = 0; i < MAX_SOCKETS; i++)
	{
		if (sockets[i].recv == receiveStatus::EMPTY)
		{
			sockets[i].socket = socket;
			sockets[i].recv = recv;
			sockets[i].send = sendStatus::IDLE;
			sockets[i].len = 0;
			socketsCount++;
			added = true;
			break;
		}
	}

	return added;
}

void Server::removeSocket(int index)
{
	sockets[index].recv = receiveStatus::EMPTY;
	sockets[index].send = sendStatus::EMPTY;
	socketsCount--;
}

int Server::getNFD(fd_set* waitRecv, fd_set* waitSend)
{
	int nfd = 0;
	timeval timeout = {TIMEOUT, 0};
	
	FD_ZERO(waitRecv);
	for (int i = 0; i < MAX_SOCKETS; i++)
	{
		if ((sockets[i].recv == receiveStatus::LISTEN) || (sockets[i].recv == receiveStatus::RECEIVE))
			FD_SET(sockets[i].socket, waitRecv);
	}

	FD_ZERO(waitSend);
	for (int i = 0; i < MAX_SOCKETS; i++)
	{
		if (sockets[i].send == sendStatus::SEND)
			FD_SET(sockets[i].socket, waitSend);
	}

	nfd = select(0, waitRecv, waitSend, NULL, &timeout);
	if (nfd == SOCKET_ERROR)
	{
		cout << "Server: Error at select(): " << WSAGetLastError() << endl;
		throw exception();
	}

	return nfd;
}

void Server::handleReceiveSockets(int* nfd, fd_set* waitRecv)
{
	for (int i = 0; i < MAX_SOCKETS && *nfd > 0; i++)
	{
		if (FD_ISSET(sockets[i].socket, waitRecv))
		{
			nfd--;
			switch (sockets[i].recv)
			{
			case receiveStatus::LISTEN:
				//acceptConnection(i);
				break;

			case receiveStatus::RECEIVE:
				//receiveMessage(i);
				break;
			}
		}
	}
}

void Server::handleSendSockets(int* nfd, fd_set* waitSend) 
{
	for (int i = 0; i < MAX_SOCKETS && *nfd > 0; i++)
	{
		if (FD_ISSET(sockets[i].socket, waitSend))
		{
			nfd--;
			switch (sockets[i].send)
			{
			case sendStatus::SEND:
				//sendMessage(i);
				break;
			}
		}
	}
}