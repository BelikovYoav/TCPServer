#include "Request.h"
#include <iostream>
int main()
{
	Request check("GET thisisurl?lang=he HTTP/1.1\r\n");
	cout << check.m_method;
	cout << check.m_version;
}