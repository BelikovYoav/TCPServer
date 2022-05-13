#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <time.h>
#pragma comment(lib, "Ws2_32.lib")
using namespace std;

class Server
{
	private:
		SOCKET listenSocket;
		sockaddr_in serverService;

		void init(int _listenPort);
	public:
		//CONSTRUCTOR AND DESTRUCTOR
		Server();
		~Server();

		//OTHER
		void runServer(int listenPort);
};