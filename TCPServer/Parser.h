#pragma once
#include <string>
#include <map>
using namespace std;

enum Method { OPTIONS, GET, HEAD, POST, PUT, DELETE, TRACE };
enum Lang { en, he, fr };

class Parser {
public:
	string m_method;
	string m_path;
	string m_version;
	string m_language;
	bool m_syntax = true;
	map<string, string> m_headers;
	string m_body;

	Parser(string str);

	void getMethod(const string& str);
	void getPath(string& str);
	void getVersion(const string& str);
	void getHeaders(string& str);


	string getWord(string& str, string param);

};
