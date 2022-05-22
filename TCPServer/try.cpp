#include "Server.h"

#define PORT 27015
int main()
{
	StatusCode::StatusCode();
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
	catch (exception ex) {
		cout << ex.what() << endl;
		errorCode = 1;
	}

	WSACleanup();
	return errorCode;


	/*
	StatusCode::StatusCode();
	stringstream str;

	str << "GET /index HTTP/1.1" << CRLF\
		<< "Accept: *" << CRLF\
		<< "Cache-Control: no-cache" << CRLF\
		<< "Postman-Token: b9179086-2255-4afb-a785-dfe4000a612e" << CRLF\
		<< "Host: 127.0.0.1:27015" << CRLF\
		<< "Accept-Encoding: gzip, deflate, br" << CRLF\
		<< "Connection: keep-alive" << CRLF\
		<< CRLF;

	Request req(str.str());
	Response res(req);
	cout << res.toString();
		*/
		/*
		//Parser check("GET\r\n");
		//Parser p= Parser("GET /contact_form.php?lang=en HTTP/1.1\r\nHost: developer.mozilla.org\r\nAccept - Language: fr\r\n\r\nthis is the body");
		Request p = Request("GET /contact_form.php?lang=en HTTP/1.1\r\nHost: developer.mozilla.org\r\nAccept - Language: fr\r\n\r\nthis is the body");
		//cout << check.m_version;*/
}