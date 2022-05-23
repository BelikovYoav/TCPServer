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

	void fillResponse(int statusCode);
	void fillResponse(int statusCode, string data);
	bool checkValid(bool isValid);
	bool checkVersion(string& version);
	void checkMethod(Request& req);
	string getPath(string& reqPath, string& language);
	void addHeader(const string& key, const string& value);

	void httpGet(Request& req);
	void httpHead(Request& req);
	void httpPost(Request& req);
	void httpPut(Request& req);
	void httpDelete(Request& req);
	void httpTrace(Request& req);
	void httpOptions(Request& req);

public:
	Response(Request req);
	string toString();

};