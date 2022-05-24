#include "Response.h"

map<int, string> StatusCode::StatusCodes;

StatusCode::StatusCode()
{
	//Used
	StatusCodes.insert(pair<int, string>(200, "OK"));
	StatusCodes.insert(pair<int, string>(201, "Created"));
	StatusCodes.insert(pair<int, string>(204, "No Content"));
	StatusCodes.insert(pair<int, string>(400, "Bad Request"));
	StatusCodes.insert(pair<int, string>(404, "Not Found"));
	StatusCodes.insert(pair<int, string>(405, "Method Not Allowed"));
	StatusCodes.insert(pair<int, string>(505, "HTTP Version Not Supported"));

	// Not Used
	StatusCodes.insert(pair<int, string>(202, "Accepted"));
	StatusCodes.insert(pair<int, string>(301, "Moved Permanently"));
	StatusCodes.insert(pair<int, string>(500, "Internal Server Error"));
	StatusCodes.insert(pair<int, string>(501, "Not Implemented"));
}

string StatusCode::getStatusCode(int code)
{
	map<int, string>::iterator itr = StatusCodes.find(code);
	return itr->second;
}

Response::Response(Request req)
{
	if (checkValid(req.getValidSyntax()))
		if (checkVersion(req.getVersion()))
			checkMethod(req);

}

string Response::toString()
{
	stringstream str;

	str << "HTTP/" << httpVersion << " "
		<< StatusCode.first << " " << StatusCode.second << CRLF;
	for (map<string, string>::iterator itr = headers.begin(); itr != headers.end(); itr++)
	{
		str << itr->first << ": " << itr->second << CRLF;
	}

	str << CRLF;
	if (!body.empty())
	{
		str << body;
	}

	return str.str();
}

void Response::fillResponse(int statusCode)
{
	this->httpVersion = 1.1;
	this->StatusCode = pair<int, string>(statusCode, StatusCode::getStatusCode(statusCode));
	if (statusCode != 204)
		addHeader("Content-Length", "0");
}

void Response::fillResponse(int statusCode, string data)
{
	fillResponse(statusCode);
	addHeader("Content-Type", "text/html; charset=UTF-8");
	addHeader("Content-Length", to_string(data.length()));
	this->body = data;
}

bool Response::checkValid(bool isValid)
{
	if (false == isValid)
		fillResponse(400);
	return isValid;
}

bool Response::checkVersion(const string& version)
{
	bool check = true;

	if ("HTTP/1.1" == version)
		this->httpVersion = 1.1;
	else if ("HTTP/1.0" == version)
		this->httpVersion = 1.0;
	else {
		fillResponse(505);
		check = false;
	}
	return check;
}

void Response::checkMethod(Request& req)
{
	string methodName = req.getMethod();
	if (methodName == "GET")
	{
		httpGet(req);
	}
	else if (methodName == "HEAD")
	{
		httpHead(req);
	}
	else if (methodName == "POST")
	{
		httpPost(req);
	}
	else if (methodName == "PUT")
	{
		httpPut(req);
	}
	else if (methodName == "DELETE")
	{
		httpDelete(req);
	}
	else if (methodName == "TRACE")
	{
		httpTrace(req);
	}
	else if (methodName == "OPTIONS")
	{
		httpOptions(req);
	}
	else
	{
		fillResponse(405);
	}
}

string Response::getPath(const string& reqPath, const string& language)
{
	string path = reqPath;
	if (language == "" || ((language != "fr") && (language != "en") && (language != "he")))
		path.append(".en");
	else
		path.append(".").append(language);

	path = BASICPATH + doubleTheChar(path, '\\');
	return path;
}

void Response::addHeader(const string& key, const string& value)
{
	this->headers[key] = value;
}


void Response::httpGet(Request& req)
{
	bool valid;
	string path = getPath(req.getPath(), req.getLanguage());
	string data = getFileData(&valid, path);
	if (valid)
	{
		fillResponse(200, data);
	}
	else
	{
		fillResponse(404);
	}
}

void Response::httpHead(Request& req)
{
	httpGet(req);
	if (StatusCode.first == 200)
	{
		this->body = "";
	}
}

void Response::httpPost(Request& req)
{
	if (req.getBody() == "")
	{
		fillResponse(400);
	}
	else
	{
		fillResponse(200);
		cout << req.getBody() << endl;
	}
}

void Response::httpPut(Request& req)
{
	map<string, string> headers = req.getHeaders();
	if (headers.find("Content-Length") == headers.end())
	{
		fillResponse(400);
	}
	else
	{
		string path = getPath(req.getPath(), req.getLanguage());
		ofstream file;
		if (fileExist(path))
		{
			fillResponse(200);
		}
		else
		{
			fillResponse(201);
		}

		if (path[path.length() - 4] == '/')
		{
			fillResponse(400);
		}
		else
		{
			file.open(path);
			if (!file.is_open())
			{
				fillResponse(400);
			}
			else
			{
				file.write(req.getBody().c_str(), req.getBody().length());
			}

			file.close();
		}
	}
}

void Response::httpDelete(Request& req)
{
	string path = getPath(req.getPath(), req.getLanguage());
	if (fileExist(path))
	{
		remove(path.c_str()) == 0 ? fillResponse(200) : fillResponse(400);
	}
	else
	{
		fillResponse(404);
	}
}

void Response::httpTrace(Request& req)
{
	fillResponse(200, req.getRawRequest());
	addHeader("Content-Type", "message/http");
}

void Response::httpOptions(Request& req)
{
	string path = getPath(req.getPath(), req.getLanguage());

	if ((req.getPath() != "*") && (!fileExist(path)))
		fillResponse(404);
	else
	{
		fillResponse(200);
		addHeader("Allow", "GET, HEAD, POST, PUT, DELETE, TRACE, OPTIONS");
	}
}
