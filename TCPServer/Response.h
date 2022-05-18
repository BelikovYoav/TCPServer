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

	bool checkValid(bool isValid);
	void checkMethod(Request& req);
	bool checkVersion(string& version);
	void badRequest(int errorCode);
	// to_string
public:
	Response(Request req);
	
	


};