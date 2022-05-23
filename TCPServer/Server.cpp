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
		checkTimeouts();
		nfd = getNFD(&waitRecv, &waitSend);
		handleReceiveSockets(&nfd, &waitRecv);
		handleSendSockets(&nfd, &waitSend);
	}
}

bool Server::addSocket(SOCKET socket, receiveStatus recv)
{
	bool added = false;

	// Set the socket to be in non-blocking mode.
	unsigned long flag = 1;
	if (ioctlsocket(socket, FIONBIO, &flag) != 0)
	{
		cout << "Server: Error at ioctlsocket(): " << WSAGetLastError() << endl;
	}
	else
	{
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

		if (!added)
			cout << "\t\tToo many connections, dropped!\n";
	}

	return added;
}

void Server::removeSocket(int index)
{
	sockets[index].recv = receiveStatus::EMPTY;
	sockets[index].send = sendStatus::EMPTY;
	sockets[index].timer = 0;
	socketsCount--;
	cout << socketsCount;
}

void Server::checkTimeouts()
{
	time_t timeNow;
	time(&timeNow);

	for (int i = 1; i < MAX_SOCKETS; i++)
	{
		if (timeNow - sockets[i].timer >= 120 && sockets[i].timer != 0)
		{
			closesocket(sockets[i].socket);
			removeSocket(i);
		}
	}
}

int Server::getNFD(fd_set* waitRecv, fd_set* waitSend)
{
	int nfd = 0;
	timeval timeout = { TIMEOUT, 0 };

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
			(*nfd)--;
			switch (sockets[i].recv)
			{
			case receiveStatus::LISTEN:
				acceptConnection(i);
				break;

			case receiveStatus::RECEIVE:
				receiveMessage(i);
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
			(*nfd)--;
			switch (sockets[i].send)
			{
			case sendStatus::SEND:
				sendMessage(i);
				break;
			}
		}
	}
}

void Server::acceptConnection(int index)
{
	SOCKET socket = sockets[index].socket;
	struct sockaddr_in from;
	int fromLen = sizeof(from);

	SOCKET msgSocket = accept(socket, (struct sockaddr*)&from, &fromLen);
	if (INVALID_SOCKET == msgSocket)
	{
		cout << "Server: Error at accept(): " << WSAGetLastError() << endl;
		throw exception();
	}
	cout << "Server: Client " << inet_ntoa(from.sin_addr) << ":" << ntohs(from.sin_port) << " is connected." << endl;

	if (addSocket(msgSocket, receiveStatus::RECEIVE) == false)
	{
		closesocket(socket);
	}
}

void Server::receiveMessage(int index)
{
	SOCKET msgSocket = sockets[index].socket;
	int len;
	int bytesRecv;

	len = sockets[index].len;
	bytesRecv = recv(msgSocket, &sockets[index].buffer[len], sizeof(sockets[index].buffer) - len, 0);
	time(&sockets[index].timer);
	if (SOCKET_ERROR == bytesRecv)
	{
		cout << "Server: Error at recv(): " << WSAGetLastError() << endl;
		closesocket(msgSocket);
		removeSocket(index);
	}
	else
	{
		if (bytesRecv == 0)
		{
			closesocket(msgSocket);
			removeSocket(index);
		}
		else
		{
			sockets[index].buffer[len + bytesRecv] = '\0'; //add the null-terminating to make it a string
			cout << "Server: Recieved: " << bytesRecv << " bytes of \"" << &sockets[index].buffer[len] << "\" message.\n\n";
			sockets[index].len += bytesRecv;

			string msg = sockets[index].buffer;
			if (sockets[index].len > 0 && msg.find("\r\n\r\n"))
			{
				sockets[index].len++;
				sockets[index].send = sendStatus::SEND;
			}	
		}
	}
}

void Server::sendMessage(int index)
{
	int bytesSent = 0;
	char sendBuff[1024] = "";

	SOCKET msgSocket = sockets[index].socket;
	string msg = sockets[index].buffer;
	int msgLen = msg.length() + 1;
	Request req(msg);
	Response res(req);

	strcpy(sendBuff, res.toString().c_str());
	memcpy(sockets[index].buffer, &sockets[index].buffer[msgLen], sockets[index].len - msgLen);
	sockets[index].len -= msgLen;
	/*
	//debug
	string data_str;
	ifstream file("C:\\Users\\user\\Desktop\\index.html");
	if (file.is_open()) {
		stringstream ss;
		ss << file.rdbuf();
		data_str = ss.str();
	}
	file.close();

	string msg = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\Content-Length: ";
	msg += to_string(data_str.length());
	msg += "\r\n\r\n";
	msg += data_str;
	strcpy(sendBuff, msg.c_str());

	cout << msg << endl;
	//dewbug
	*/

	// enter message to buffer here
	/*char data[] = "<!DOCTYPE html><html>\n<body>\n<h1>Hello, World!</h1>\n</body>\n</html>";
	char data2[] = "<!DOCTYPE html>\
		< html >\
		<body>\
		<h1>Hello World!< / h1>\
		< / body>\
		< / html>";
	strcpy(sendBuff, "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\Content-Length: 81\r\n\r\n<html>\n<body>\n<h1>Hello, World!</h1>\n</body>\n</html>");
	*/
	bytesSent = send(msgSocket, sendBuff, strlen(sendBuff), 0);
	if (SOCKET_ERROR == bytesSent)
	{
		cout << "Server: Error at send(): " << WSAGetLastError() << endl;
		return;
	}
	else
	{
		cout << "Server: Sent: " << bytesSent << "\\" << strlen(sendBuff) << " bytes of \"" << sendBuff << "\" message.\n\n";

		if(sockets[index].len == 0)
			sockets[index].send = sendStatus::IDLE;
	}
}
