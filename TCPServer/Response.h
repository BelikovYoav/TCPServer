#pragma once
#include "Request.h"

class StatusCode {
private:
	static map<int, string> StatusCodes;
public:
	StatusCode();
	static string getStatusCode(int code);
};


class Response {
private:
	float httpVersion;
	pair<int, string> StatusCode;
	map<string, string> m_headers;
	string m_body;
	string language;

	void checkMethod(Request& req);
	void checkVersion();
	// to_string
public:
	Response(Request req);
	
	


};