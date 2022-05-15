#include "Parser.h"
#include <iostream>
int main()
{
	//Parser check("GET\r\n");
	Parser("GET /contact_form.php?lang=en HTTP/1.1\r\nHost: developer.mozilla.org\r\nAccept - Language: fr\r\n\r\n");
	//cout << check.m_method;
	//cout << check.m_version;
}