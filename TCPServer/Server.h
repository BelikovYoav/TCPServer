#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <time.h>
#pragma comment(lib, "Ws2_32.lib")
using namespace std;
#include"Response.h"

#define MAX_SOCKETS 60
#define TIMEOUT 5
class Server
{
private:
	enum class receiveStatus { EMPTY, LISTEN, RECEIVE };
	enum class sendStatus { EMPTY, IDLE, SEND };
	enum sendType {};

	struct SocketState
	{
		SOCKET socket;
		receiveStatus recv;
		sendStatus send;
		sendType sendType;
		char buffer[600];
		int len;
		time_t timer = 0;
	};

	SOCKET listenSocket;
	sockaddr_in serverService;

	// listenSocket state will be at index 0
	struct SocketState sockets[MAX_SOCKETS] = { 0 };
	int socketsCount = 0;

	void init(int listenPort);
	void handleConnections();

	bool addSocket(SOCKET socket, receiveStatus recv);
	void removeSocket(int index);

	void checkTimeouts();
	int getNFD(fd_set* waitRecv, fd_set* waitSend);
	void handleReceiveSockets(int* nfd, fd_set* waitRecv);
	void handleSendSockets(int* nfd, fd_set* waitSend);
	void acceptConnection(int index);
	void receiveMessage(int index);
	void sendMessage(int index);
public:
	//CONSTRUCTOR AND DESTRUCTOR
	Server();
	~Server();

	//OTHER
	void runServer(int listenPort);
};