#include "Response.h"

map<int, string> StatusCode::StatusCodes;

StatusCode::StatusCode()
{
	//Used
	StatusCodes.insert(pair<int, string>(200, "OK"));
	StatusCodes.insert(pair<int, string>(400, "Bad Request"));
	StatusCodes.insert(pair<int, string>(404, "Not Found"));
	StatusCodes.insert(pair<int, string>(505, "HTTP Version Not Supported"));

	// Not Used
	StatusCodes.insert(pair<int, string>(201, "Created"));
	StatusCodes.insert(pair<int, string>(202, "Accepted"));
	StatusCodes.insert(pair<int, string>(204, "No Content"));
	StatusCodes.insert(pair<int, string>(301, "Moved Permanently"));
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

string Response::toString()
{
	stringstream str;

	str << "HTTP/" << httpVersion << " " \
		<< StatusCode.first << " " << StatusCode.second << CRLF;
	for (map<string, string>::iterator itr = m_headers.begin(); itr != m_headers.end(); itr++)
	{
		str << itr->first << ": " << itr->second << CRLF;
	}

	str << CRLF;
	if (!m_body.empty())
	{
		str << m_body;
	}

	return str.str();
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
	string methodName = req.m_method;
	if (methodName == "GET")
	{
		get(req);
	}
	else if (methodName == "HEAD")
	{

	}
	else if (methodName == "POST")
	{

	}
	else if (methodName == "PUT")
	{

	}
	else if (methodName == "DELETE")
	{

	}
	else if (methodName == "TRACE")
	{

	}
	else if (methodName == "OPTIONS")
	{

	}
	else
	{
		badRequest(405);
	}
}

void Response::get(Request& req)
{
	string path;
	bool valid;

	path = req.m_path != "" ? req.m_path:"index";
	if (req.m_language == "" || ((req.m_language != "fr") && (req.m_language != "en") && (req.m_language != "he")))
		path.append(".en");
	else
		path.append(".").append(req.m_language);

	string data_str = getFileData(&valid, path);
	int data_len = data_str.length();
	if (valid)
	{
		this->StatusCode = pair<int, string>(200, StatusCode::getStatusCode(200));
		this->m_headers.insert(pair<string, string>("Content-Type", "text/html; charset=UTF-8"));
		this->m_headers.insert(pair<string, string>("Content-Length", to_string(data_len)));
		this->m_body = data_str;
	}
	else
	{
		badRequest(404);
	}
}