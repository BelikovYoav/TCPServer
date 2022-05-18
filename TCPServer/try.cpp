#include "Request.h"
#include "Server.h"

#define PORT 27015
int main()
{
	int errorCode = 0;
	WSAData wsaData;

	try {

		if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData))
		{
			throw exception("Server : Error at WSAStartup()");
		}

		Server server;
		server.runServer(PORT);
	}
	catch(exception ex){
		cout << ex.what() << endl;
		errorCode = 1;
	}

	WSACleanup();
	return errorCode;
	/*
	//Parser check("GET\r\n");
	//Parser p= Parser("GET /contact_form.php?lang=en HTTP/1.1\r\nHost: developer.mozilla.org\r\nAccept - Language: fr\r\n\r\nthis is the body");
	Request p = Request("GET /contact_form.php?lang=en HTTP/1.1\r\nHost: developer.mozilla.org\r\nAccept - Language: fr\r\n\r\nthis is the body");
	//cout << check.m_version;*/
}