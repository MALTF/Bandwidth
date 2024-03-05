
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "URL.h"

int main()
{
	URL *url;

	url = new(URL);
	$(url, describe, NULL); puts("");
	if ($(url, scheme) != URLSchemeNone)
		return 1;
	release(url);

	typedef struct {
		const char *url;
		URLScheme scheme;
	} SchemeData;
	SchemeData testData [] = {
		{ "tel:123456", URLSchemeTelephone },
		{ "fiLe:///etc/hosts", URLSchemeFile},
		{ "https://xyz.be", URLSchemeHTTPS },
		{ "httP://fubar.foo?nu", URLSchemeHTTP },
		{ "FTP://fubar.foo?nu", URLSchemeFTP },
		{ "Mailto:fubar@foo.nu", URLSchemeMailto },
		{ "irc://fubar@foo.nu", URLSchemeIRC },
		{ "javascript:asldkfjlkads.nu", URLSchemeJavascript },
		{ "javascript//asldkfjlkads.nu", URLSchemeNone },
		{ "sms:171717", URLSchemeSMS },
		{ "sms:", URLSchemeNone },
		{ "file:", URLSchemeNone },
		{ "file://", URLSchemeNone },
		{ "file:///", URLSchemeFile },
		{ "file://../../etc/hosts", URLSchemeFile },
		{ "file:///etc/hosts", URLSchemeFile },
		{ "snmp://xyz.xyz", URLSchemeSNMP },
		{ "", URLSchemeNone },
		{ "://", URLSchemeNone },
		{ "abc://asdlfjalds", URLSchemeNone },
		{ "ftp:/asdlfjalds", URLSchemeNone },
		{ "ftp//asdlfjalds", URLSchemeNone },
		{ "http:asdlfjalds", URLSchemeNone },
		{ "http:/asdlfjalds", URLSchemeNone },
		{ "http//asdlfjalds", URLSchemeNone },
		{ "https:/asdlfjalds", URLSchemeNone },
	};
	for (unsigned i=0; i < sizeof(testData)/sizeof(SchemeData); i++) {
		url = _URL(testData[i].url);
		$(url, describe, NULL); puts("");
		fflush(NULL);
		if (testData[i].scheme != $(url, scheme))
			return 2;
	}

	release(url);
	release(url);
	release(url);
	release(url);
	if (url != NULL)
		return 99;

	url = _URL("http://yahoo.com");
	if ($(url, scheme) != URLSchemeHTTP)
		return 3;
	String *str = $(url, string);
	String *str2 = _String("http://yahoo.com");
	if (!$(str, equals, str2)) 
		return 4;
	if (!$(str2, equals, str)) 
		return 5;
	release(str2);

	unsigned statusCode;
	Data *data = $(url, synchronousGet, &statusCode);
	if (!data) 
		return 5;
	printf ("HTTP got %lu bytes\n", $(data, length));
	release(data);
	release(str);
	release(url);
	printf("HTTP status code %u\n", statusCode);

	url = _URL("https://yahoo.com");
	if ($(url, scheme) != URLSchemeHTTPS)
		return 6;
	data = $(url, synchronousGet, &statusCode);
	if (!data) 
		return 7;
	char *string = $(data, asCString);
	printf ("HTTPS got %lu bytes\n", strlen(string));
	printf("HTTPS status code %u\n", statusCode);
	free (string);
	release(url);
	release(data);

	deallocateClasses();
	return 0;
}

