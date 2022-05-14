#include "Server.h"

//CONSTRUCTOR AND DESTRUCTOR
Server::Server()
{
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == listenSocket)
	{
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
}

void Server::init(int listenPort)
{
	serverService.sin_family = AF_INET;
	serverService.sin_addr.s_addr = INADDR_ANY;
	serverService.sin_port = htons(listenPort);

	if (SOCKET_ERROR == bind(listenSocket, (SOCKADDR*)&serverService, sizeof(serverService)))
	{
		cout << "Server: Error at bind(): " << WSAGetLastError() << endl;
		throw exception();
	}

	if (SOCKET_ERROR == listen(listenSocket, 5))
	{
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