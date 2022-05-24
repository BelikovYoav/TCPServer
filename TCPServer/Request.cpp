#include "Request.h"


Request::Request(string str) {
	try {// Constructor with parameters
		Request::rawRequest = str;
		string line = Request::getToken(str, CRLF);
		Request::getRequestLine(line);
		Request::getHeaders(str);
		Request::body = str;
	}
	catch (exception) {
		Request::validSyntax = false;
	}
}

void Request::getRequestLine(string& str)
{
	if (count(str.begin(), str.end(), ' ') != 2)
	{
		throw exception("syntax wrong");
	}
	Request::method = Request::getToken(str, " /");
	Request::getPath(str);
	Request::getVersion(str);
}

void Request::getPath(string& str)
{
	string path = Request::getToken(str, " ");
	int find = path.find("?lang=");
	if (find == -1)
	{
		Request::path = path;
		Request::language = "en";
	}
	else {
		Request::path = Request::getToken(path, "?lang=");
		Request::language = path;
	}


}

void Request::getVersion(string& str)
{
	if (str.find("HTTP/") != 0)
		throw exception("syntax wrong");
	Request::version = str;
}

void Request::getHeaders(string& str)
{
	string header = Request::getToken(str, CRLF);
	while (!header.empty())
	{
		string fieldName = Request::getToken(header, ": ");
		string value = header;
		this->headers.insert(std::pair<string, string>(fieldName, value));
		header = Request::getToken(str, CRLF);
	}

}

string Request::getToken(string& str, string param)
{
	int find = str.find(param);
	if (find == -1)
		throw exception("syntax wrong");
	string word = str.substr(0, find);
	str.erase(0, word.size() + param.size());
	return word;
}

string Request::getRawRequest() const
{
	return this->rawRequest;
}

string Request::getMethod() const
{
	return this->method;
}

string Request::getPath() const
{
	return this->path;
}

string Request::getVersion() const
{
	return this->version;
}

string Request::getLanguage() const
{
	return this->language;
}

bool Request::getValidSyntax() const
{
	return this->validSyntax;
}

map<string, string> Request::getHeaders() const
{
	return this->headers;
}

string Request::getBody() const
{
	return this->body;
}
