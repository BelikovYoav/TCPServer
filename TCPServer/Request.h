#pragma once
#include <string>
#include <map>
#include <algorithm>
using namespace std;
#define CRLF "\r\n"

//enum Method { OPTIONS, GET, HEAD, POST, PUT, DELETE, TRACE };
enum Lang { en, he, fr };


class Request {
public:
	string m_method;
	string m_path;
	string m_version;
	string m_language;
	bool m_syntax = true;
	map<string, string> m_headers;
	string m_body;

	Request(string str);

	void getVersion(string& str);
	void getPath(string& str);
	void getHeaders(string& str);

	void getRequestLine(string& str);

	string getToken(string& str, string param);

};
