#include "Utils.h"

string doubleTheChar(string str, char ch)
{
	string newStr = "";
	for (string::size_type i = 0; i < str.size(); i++)
	{
		newStr.push_back(str[i]);
		if (str[i] == ch)
			newStr.push_back(str[i]);
	}

	return newStr;
}

string getFileData(bool* valid, string path)
{
	string data_str = "";
	*valid = true;
	ifstream file(path);
	if (file.is_open()) {
		stringstream ss;
		ss << file.rdbuf();
		data_str = ss.str();
		file.close();
	}
	else
	{
		*valid = false;
	}

	return data_str;
}

bool fileExist(string path) 
{
	bool exist;

	if (FILE* file = fopen(path.c_str(), "r")) {
		fclose(file);
		exist = true;
	}
	else {
		exist = false;
	}

	return exist;
}