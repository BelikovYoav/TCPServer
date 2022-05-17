#include "Request.h"
#include <iostream>
int main()
{
	//Parser check("GET\r\n");
	//Parser p= Parser("GET /contact_form.php?lang=en HTTP/1.1\r\nHost: developer.mozilla.org\r\nAccept - Language: fr\r\n\r\nthis is the body");
	Request p = Request("GET /contact_form.php?lang=en HTTP/1.1\r\nHost: developer.mozilla.org\r\nAccept - Language: fr\r\n\r\nthis is the body");
	//cout << check.m_version;
}