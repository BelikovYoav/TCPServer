#include "Server.h"

//CONSTRUCTOR AND DESTRUCTOR
Server::Server()
{
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == listenSocket)
	{
		cout << "Time Server: Error at socket(): " << WSAGetLastError() << endl;
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
	cout << "success" << endl;
}

void Server::init(int listenPort)
{
	serverService.sin_family = AF_INET;
	serverService.sin_addr.s_addr = INADDR_ANY;
	serverService.sin_port = htons(listenPort);

	if (SOCKET_ERROR == bind(listenSocket, (SOCKADDR*)&serverService, sizeof(serverService)))
	{
		cout << "Time Server: Error at bind(): " << WSAGetLastError() << endl;
		throw exception();
	}

	if (SOCKET_ERROR == listen(listenSocket, 5))
	{
		cout << "Time Server: Error at listen(): " << WSAGetLastError() << endl;
		throw exception();
	}
}