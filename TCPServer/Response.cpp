#include "Response.h"

StatusCode::StatusCode() 
{
	//Used

	// Not Used
	StatusCodes.insert(pair<int, string>(200, "OK"));
	StatusCodes.insert(pair<int, string>(201, "Created"));
	StatusCodes.insert(pair<int, string>(202, "Accepted"));
	StatusCodes.insert(pair<int, string>(204, "No Content"));
	StatusCodes.insert(pair<int, string>(301, "Moved Permanently"));
	StatusCodes.insert(pair<int, string>(400, "Bad Request"));
	StatusCodes.insert(pair<int, string>(404, "Not Found"));
	StatusCodes.insert(pair<int, string>(405, "Method Not Allowed"));
	StatusCodes.insert(pair<int, string>(500, "Internal Server Error"));
	StatusCodes.insert(pair<int, string>(501, "Not Implemented"));
	StatusCodes.insert(pair<int, string>(505, "HTTP Version Not Supported"));
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
	//checkvalid
	checkVersion();
	checkMethod(req);
}

void Response::checkVersion()
{

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