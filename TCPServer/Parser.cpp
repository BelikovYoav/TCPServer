#include "Parser.h"


		Parser::Parser(string str) {
			try {// Constructor with parameters
				Parser::m_method = Parser::getWord(str, " ");
				Parser::getPath(str);
				Parser::m_version = getWord(str, "\r\n");
				Parser::getHeaders(str);
				Parser::m_body = str;
			}
			catch (exception) {
				Parser::m_syntax = false;
				// Block of code to handle errors
			}

		}

		void Parser::getVersion(const string& str)
		{
			if ("HTTP/1.1" == str)
				Parser::m_version = "HTTP/1.1";
			else if ("HTTP/1.0" == str)
				Parser::m_version = "HTTP/1.0";
			else
				throw exception("HTTP version not supported");


		}

		void Parser::getPath(string& str)
		{
			Parser::getWord(str, "/");
			string path = Parser::getWord(str, " ");
			int find = path.find("?lang=");
			if (find == -1)
				Parser::m_path = path;
			else {
				Parser::m_path = Parser::getWord(path, "?lang=");
				Parser::m_language = path;
			}


		}

		void Parser::getHeaders(string& str)
		{
			string header = Parser::getWord(str, "\r\n");
			while (!header.empty())
			{
				string fieldName = Parser::getWord(header, ": ");
				string value = header;
				this->m_headers.insert(std::pair<string, string>(fieldName, value));
				header = Parser::getWord(str, "\r\n");
			}

		}

		void Parser::getMethod(const string& str)
		{
			if ("GET" == str)
				Parser::m_method = Method::GET;
			else if ("POST" == str)
				Parser::m_method = Method::POST;
			else
				throw exception("HTTP Method not supported");
		}

		string Parser::getWord(string& str, string param)
		{	
			int find = str.find(param);
			if (find == -1)
				throw exception("syntax wrong");
			string word = str.substr(0, find);
			str.erase(0, word.size() + param.size());
			return word;
		}




