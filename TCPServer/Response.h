#pragma once
#include "Request.h"
#include "Utils.h"

class StatusCode {
private:
	static map<int, string> StatusCodes;
public:
	StatusCode();
	static string getStatusCode(int code);
};


class Response {
private:
	double httpVersion;
	pair<int, string> StatusCode;
	map<string, string> m_headers;
	string m_body;

	void badRequest(int errorCode);
	bool checkValid(bool isValid);
	bool checkVersion(string& version);
	void checkMethod(Request& req);
	string getPath(string& reqPath, string& language);

	void get(Request& req);
	void head(Request& req);
	void post(Request& req);
	void put(Request& req);
	void trace(Request& req);
	void options(Request& req);

public:
	Response(Request req);
	string toString();

};