#include "Request.h"


		Request::Request(string str) { // Constructor with parameters
			Request::getMethod(Request::getWord(str," "));
			Request::getURL(str);
			Request::getVersion(getWord(str, "\r\n"));

		}

		void Request::getVersion(const string& str)
		{
			if ("HTTP/1.1" == str)
				Request::m_version = "HTTP/1.1";
			else if ("HTTP/1.0" == str)
				Request::m_version = "HTTP/1.0";
			else
				throw exception("HTTP version not supported");


		}

		void Request::getURL(string& str)
		{
			Request::m_URL = Request::getWord(str, "?lang=");
			Request::m_language = Request::getWord(str, " ");

		}

		void Request::getMethod(const string& str)
		{
			if ("GET" == str)
				Request::m_method = Method::GET;
			else if ("POST" == str)
				Request::m_method = Method::POST;
			else
				throw exception("HTTP Method not supported");
		}

		string Request::getWord(string& str, string param)
		{
			string word = str.substr(0, str.find(param));
			str.erase(0, word.size() + param.size());
			return word;
		}




