#include "Request.h"


		Request::Request(string str) {
			try {// Constructor with parameters
				string line = Request::getToken(str, CRLF);
				Request::getRequestLine(line);
				Request::getHeaders(str);
				Request::m_body = str;
			}
			catch (exception) {
				Request::m_syntax = false;
			}
		}

		void Request::getRequestLine(string& str)
		{
			if (count(str.begin(), str.end(), ' ') != 2)
			{
				throw exception("syntax wrong");
			}
			Request::m_method = Request::getToken(str, " /");
			Request::getPath(str);
			Request::getVersion(str);
		}

		void Request::getPath(string& str)
		{
			string path = Request::getToken(str, " ");
			int find = path.find("?lang=");
			if (find == -1)
				Request::m_path = path;
			else {
				Request::m_path = Request::getToken(path, "?lang=");
				Request::m_language = path;
			}


		}

		void Request::getVersion(string& str)
		{
			if(str.find("HTTP/")!=0)
				throw exception("syntax wrong");
			Request::m_version = str;
		}

		void Request::getHeaders(string& str)
		{
			string header = Request::getToken(str, CRLF);
			while (!header.empty())
			{
				string fieldName = Request::getToken(header, ": ");
				string value = header;
				this->m_headers.insert(std::pair<string, string>(fieldName, value));
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
