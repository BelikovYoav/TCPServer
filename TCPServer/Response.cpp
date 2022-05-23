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

	str << "HTTP/" << httpVersion << " "
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
	this->httpVersion = 1.1;
	this->StatusCode = pair<int, string>(errorCode, StatusCode::getStatusCode(errorCode));
	this->m_headers.insert(pair<string, string>("Content-Length", "0"));
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
		head(req);
	}
	else if (methodName == "POST")
	{
		post(req);
	}
	else if (methodName == "PUT")
	{

	}
	else if (methodName == "DELETE")
	{

	}
	else if (methodName == "TRACE")
	{
		trace(req);
	}
	else if (methodName == "OPTIONS")
	{
		options(req);
	}
	else
	{
		badRequest(405);
	}
}

string Response::getPath(string& reqPath, string& language)
{
	string path = reqPath != "" ? reqPath : "index";
	if (language == "" || ((language != "fr") && (language != "en") && (language != "he")))
		path.append(".en");
	else
		path.append(".").append(language);

	return path;
}

void Response::get(Request& req)
{
	bool valid;
	string path = getPath(req.m_path, req.m_language);
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

void Response::head(Request& req)
{
	get(req);
	if(StatusCode.first == 200)
		this->m_body = "";
}

void Response::post(Request& req)
{
	if (req.m_body == "")
	{
		badRequest(400);
	}
	else
	{
		this->StatusCode = pair<int, string>(200, StatusCode::getStatusCode(200));
		this->m_headers.insert(pair<string, string>("Content-Length", "0"));
		cout << req.m_body << endl;
	}
}

void Response::trace(Request& req)
{
	this->StatusCode = pair<int, string>(200, StatusCode::getStatusCode(200));
	this->m_headers.insert(pair<string, string>("Content-Type", "message/http"));
	this->m_body = req.m_raw;
	this->m_headers.insert(pair<string, string>("Content-Length", to_string((this->m_body).length())));
}

void Response::options(Request& req)
{
	bool valid;
	string path = getPath(req.m_path, req.m_language);
	getFileData(&valid, path);

	if ((req.m_path != "*") && (!valid))
		badRequest(404);
	else
	{
		this->StatusCode = pair<int, string>(200, StatusCode::getStatusCode(200));
		this->m_headers.insert(pair<string, string>("Allow", "GET, HEAD, POST, PUT, DELETE, TRACE, OPTIONS"));
	}
}
