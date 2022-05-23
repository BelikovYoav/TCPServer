#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#define BASICPATH  "C:\\temp\\"

string doubleTheChar(string str, char ch);
string getFileData(bool* valid, string path);
bool fileExist(string path);