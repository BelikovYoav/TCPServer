#pragma once
#include <string>
using namespace std;

enum Method { OPTIONS, GET, HEAD, POST, PUT, DELETE, TRACE };
enum Lang { en, he, fr };

class Request {
public:
	Method m_method;
	string m_URL;
	string m_version;
	string m_language;

	Request(string str);

	void getMethod(const string& str);
	void getURL(string& str);
	void getVersion(const string& str);


	string getWord(string& str, string param);

};
