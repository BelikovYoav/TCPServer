#include "Response.h"

StatusCode::StatusCode() 
{
	//Used
	StatusCodes.insert(pair<int, string>(400, "Bad Request"));
	StatusCodes.insert(pair<int, string>(505, "HTTP Version Not Supported"));

	// Not Used
	StatusCodes.insert(pair<int, string>(200, "OK"));
	StatusCodes.insert(pair<int, string>(201, "Created"));
	StatusCodes.insert(pair<int, string>(202, "Accepted"));
	StatusCodes.insert(pair<int, string>(204, "No Content"));
	StatusCodes.insert(pair<int, string>(301, "Moved Permanently"));
	StatusCodes.insert(pair<int, string>(404, "Not Found"));
	StatusCodes.insert(pair<int, string>(405, "Method Not Allowed"));
	StatusCodes.insert(pair<int, string>(500, "Internal Server Error"));
	StatusCodes.insert(pair<int, string>(501, "Not Implemented"));
}

string StatusCode::getStatusCode(int code)
{
	map<int, string>::iterator itr = StatusCodes.find(code);
	//if (itr == StatusCodes.end())
	//	return 
	return itr->second;
}

Response::Response(Request req) 
{
	if (checkValid(req.m_syntax))
		if (checkVersion(req.m_version))
			checkMethod(req);	
}

void Response::badRequest(int errorCode)
{
	this->StatusCode = pair<int, string>(errorCode, StatusCode::getStatusCode(errorCode));
	this->httpVersion = 1.1;
}

bool Response::checkValid(bool isValid)
{
	if (false == isValid)
		badRequest(400);
	return isValid;
}

bool Response::checkVersion(string& version)
{
	bool check = true;

	if ("HTTP/1.1" == version)
		this->httpVersion = 1.1;
	else if ("HTTP/1.0" == version)
		this->httpVersion = 1.0;
	else {
		badRequest(500);
		check = false;
	}
	return check;
}

void Response::checkMethod(Request& req)
{
	string name = "name";
	if (name == "GET")
	{
		//get()
	}
	else
	{

	}
}