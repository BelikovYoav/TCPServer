#pragma once
#include <string>
#include <map>
#include <algorithm>
using namespace std;
#define CRLF "\r\n"

enum Lang { en, he, fr };

class Request {
private:
	string rawRequest;
	string method;
	string path;
	string version;
	string language;
	bool validSyntax = true;
	map<string, string> headers;
	string body;

	void getRequestLine(string& str);
	void getVersion(string& str);
	void getPath(string& str);
	void getHeaders(string& str);
	string getToken(string& str, string param);

public:
	Request(string str);
	
	string getRawRequest() const;
	string getMethod() const;
	string getPath() const;
	string getVersion() const;
	string getLanguage() const;
	bool getValidSyntax() const;
	map<string, string> getHeaders() const;
	string getBody() const;
};
