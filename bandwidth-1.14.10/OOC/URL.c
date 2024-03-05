/*============================================================================
  URL, an object-oriented URL class.
  Copyright (C) 2015-2019, 2022 by Zack T Smith.

  Object-Oriented C is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  Object-Oriented C is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.
 
  You should have received a copy of the GNU Lesser General Public License
  along with this software.  If not, see <http://www.gnu.org/licenses/>.

  The author may be reached at 1@zsmith.co.
 *===========================================================================*/

#define _GNU_SOURCE

#include "URL.h"
#include "MutableData.h"
#include "Log.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>

// PATH_MAX
#if defined(__CYGWIN__)
  #include <limits.h> 
#elif defined(__linux__)
  #include <linux/limits.h> 
#elif defined(__darwin__)
  #include <limits.h> 
#endif

#include <time.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#ifdef HAVE_LIBCURL
#include <curl/curl.h> // HTTPS
#endif

typedef struct {
	const char *prefix;
	URLScheme scheme;
} Scheme;
static Scheme schemes [] = {
	{ "https://", URLSchemeHTTPS },
	{ "http://", URLSchemeHTTP },
	{ "tel:", URLSchemeTelephone },
	{ "file://", URLSchemeFile},
	{ "sms:", URLSchemeSMS },
	{ "mailto:", URLSchemeMailto },
	{ "javascript:", URLSchemeJavascript },
	{ "ftp://", URLSchemeFTP },
	{ "irc://", URLSchemeIRC },
	{ "git://", URLSchemeGIT },
	{ "ssh://", URLSchemeSSH },
};
#define N_SCHEMES (sizeof(schemes)/sizeof(Scheme))

URLClass *_URLClass = NULL;

void URL_destroy (Any *self_)
{
	if (!self_)
		return;
	verifyCorrectClassOrSubclass(self_,URL);

	URL *self = self_;

	if (self->url_string) {
		ooc_free (self->url_string);
	}
	if (self->read_buffer) {
		ooc_free (self->read_buffer);
	}
}

static void URL_describe (URL* self, FILE *outputFile) 
{ 
	if (!self)
		return;
	verifyCorrectClass(self,URL);

	if (!outputFile)
		outputFile = stdout;

	fprintf (outputFile, "%s(%s)", $(self, className), self->url_string);
}

String *URL_string (URL *self)
{
	if (!self)
		return NULL;
	verifyCorrectClass(self,URL);

	return String_withCString(self->url_string);
}

void URL_setCString (URL *self, const char *url)
{
	if (!self)
		return;
	verifyCorrectClass(self,URL);

	self->scheme = URLSchemeNone;

	if (!url) {
		if (self->url_string) {
			ooc_free (self->url_string);
			self->url_string = NULL;
		}
		return;
	}

	for (unsigned i = 0; i < N_SCHEMES; i++) {
		const char *scheme = schemes[i].prefix;
		size_t len = strlen(scheme);
		if (!strncasecmp (url, scheme, len)) {
			if (strlen (url + len)) {
				self->scheme = schemes[i].scheme;
			}
			break;
		}
	}

	self->url_string = strdup (url);
}

static HTTPOperation URL_httpOperation (URL *self)
{
	if (!self)
		return HTTPOperationNone;
	verifyCorrectClass(self,URL);

	return self->operation;
}

static URLScheme URL_scheme (URL *self)
{
	if (!self)
		return URLSchemeNone;
	verifyCorrectClass(self,URL);

	return self->scheme;
}

static void URL_setHTTPOperation (URL *self, HTTPOperation op)
{
	if (!self)
		return;
	verifyCorrectClass(self,URL);

	switch(op) {
	case HTTPOperationGet: 
	case HTTPOperationPost:
	case HTTPOperationPut:
	case HTTPOperationDelete: 
		self->operation = op;
		break;
	default:
		self->operation = HTTPOperationGet;
		break;
	}	
}

#ifdef HAVE_LIBCURL
static int synchronous_request_callback (uint8_t *bytes, size_t size, size_t nmemb, void *pageContents_)
{
	MutableData *pageContents = pageContents_;

	size_t length = size * nmemb;

	if (pageContents) {
		$(pageContents, appendBytes, bytes, length);
		return length;
	}
	return 0;
} 

/*---------------------------------------------------------------------------
 * Name:	https_synchronous_request
 * Purpose:	Fetches HTML at secure URL.
 *-------------------------------------------------------------------------*/
static MutableData *https_synchronous_request (char *url, HTTPOperation operation, uint8_t *post_payload, unsigned *return_statuscode)
{
	if (!url) {
		return NULL;
	}

	curl_global_init(CURL_GLOBAL_DEFAULT);
	CURL *session = curl_easy_init();
	if (!session) {
		return NULL;
	}
	
	MutableData *pageContents = NULL;

	do {
		CURLcode errorCode;
		char errorBuffer[CURL_ERROR_SIZE];

		errorCode = curl_easy_setopt(session, CURLOPT_ERRORBUFFER, errorBuffer);
		if (CURLE_OK != errorCode) {
			Log_warning_printf (__FUNCTION__, "Failed to set error buffer [%d]", errorCode);
			break;
		}

		errorCode = curl_easy_setopt(session, CURLOPT_URL, url);
		if (CURLE_OK != errorCode) {
			Log_warning_printf (__FUNCTION__, "Failed to set URL: %s", errorBuffer);
			break;
		}

		errorCode = curl_easy_setopt(session, CURLOPT_SSL_VERIFYPEER, 0L);
		if (CURLE_OK != errorCode) {
			Log_warning_printf (__FUNCTION__, "Failed to clear verify-peer: %s", errorBuffer);
			break;
		}

		errorCode = curl_easy_setopt(session, CURLOPT_SSL_VERIFYHOST, 0L);
		if (CURLE_OK != errorCode) {
			Log_warning_printf (__FUNCTION__, "Failed to clear verify-host: %s", errorBuffer);
			break;
		}

		errorCode = curl_easy_setopt(session, CURLOPT_FOLLOWLOCATION, 1L);
		if (CURLE_OK != errorCode) {
			Log_warning_printf (__FUNCTION__, "Failed to set follow-location: %s", errorBuffer);
			break;
		}

		errorCode = curl_easy_setopt(session, CURLOPT_ACCEPT_ENCODING, ""); 
		if (CURLE_OK != errorCode) {
			Log_warning_printf (__FUNCTION__, "Failed to enable decompression: %s", errorBuffer);
			break;
		}

		errorCode = curl_easy_setopt(session, CURLOPT_WRITEFUNCTION, synchronous_request_callback);
		if (CURLE_OK != errorCode) {
			Log_warning_printf (__FUNCTION__, "Failed to set writer function: %s", errorBuffer);
			break;
		}

		errorCode = curl_easy_setopt(session, CURLOPT_TIMEOUT_MS, 1750);
		if (CURLE_OK != errorCode) {
			Log_warning_printf (__FUNCTION__, "Failed to timeout: %s", errorBuffer);
			break;
		}

		// This data object will be passed to the callback.
		pageContents = new(MutableData);
		
		errorCode = curl_easy_setopt(session, CURLOPT_WRITEDATA, pageContents);
		if (CURLE_OK != errorCode) {
			Log_warning_printf (__FUNCTION__, "Failed to set writer data: %s", errorBuffer);
			release(pageContents);
			pageContents = NULL;
			break;
		}

		errorCode = curl_easy_perform(session);
		if (CURLE_OK != errorCode) {
			Log_warning_printf (__FUNCTION__, "Page fetch failed: %s", errorBuffer);
			release(pageContents);
			pageContents = NULL;
			break;
		} 

		// If no errors, fetch the HTML.
		int http_status_code = 0;
		curl_easy_getinfo (session, CURLINFO_RESPONSE_CODE, &http_status_code);
		if (return_statuscode) {
			*return_statuscode = http_status_code;
		}
	} while(false);

	curl_easy_cleanup (session);
	curl_global_cleanup ();

	if (pageContents) {
		printf ("Got page of length %lu, URL=%s\n", (unsigned long) $(pageContents, length), url);
	}

	return pageContents;
}
#endif

/*---------------------------------------------------------------------------
 * Name:	http_synchronous_request
 * Purpose:	Fetches HTML at URL and handles several kinds of redirects.
 *-------------------------------------------------------------------------*/
static MutableData *http_synchronous_request (char *url, HTTPOperation operation, uint8_t *post_payload, unsigned *return_statuscode)
{
	if (!url || !return_statuscode) {
		Log_debug (__FUNCTION__, "Bad parameter.");
		return NULL;
	}
	if (operation == HTTPOperationPost && !post_payload) {
		Log_debug (__FUNCTION__, "Missing POST payload.");
		return NULL;
	}
	if (!strstr (url, "://")) {
		Log_debug_printf (__FUNCTION__, "Bad URL %s", url);
		return NULL;
	}
	//==========

	char domain [PATH_MAX];
	char rest [PATH_MAX];

	bzero (domain, sizeof(domain));
	bzero (rest, sizeof(rest));

	if (!strncasecmp ("https://", url, 8)) {
		Log_debug (__FUNCTION__, "HTTPS not supported.");
		return NULL;
	}
	else if (!strncasecmp ("http://", url, 7)) {
		url += 7;
	}
	else {
		Log_debug (__FUNCTION__, "Incorrect scheme.");
		return NULL;
	}

	// Split URL into domain & the rest of it.
	char *slash = strstr (url, "/");
	if (!slash) {
		strcpy (domain, url);
	} else {
		unsigned long domain_length = slash - url;
		memcpy (domain, url, domain_length);
		domain[domain_length] = 0;
		strncpy (rest, slash+1, sizeof(rest));
	}

	// Look up the domain.
        char *ip_addr_string = NULL;
	struct hostent *he = gethostbyname (domain);
	if (!he) {
		Log_perror (__FUNCTION__, "gethostbyname");
		return NULL;
	}

	// Since this is non-HTTPS, assume port 80 is meant.
	const int port = 80;

	// Connect to the web server.
	ip_addr_string = inet_ntoa (*(struct in_addr *) *he->h_addr_list);
        int sockfd = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr (ip_addr_string);
        addr.sin_port = htons(port);

	if (connect (sockfd, (struct sockaddr*) &addr, sizeof (struct sockaddr))) {
		close (sockfd);
		Log_perror(__FUNCTION__, "connect");
		return NULL;
	}

	Log_debug_printf (__FUNCTION__, "DOMAIN: %s", domain);
	Log_debug_printf (__FUNCTION__, "IPv4: %s", ip_addr_string);
	Log_debug_printf (__FUNCTION__, "REST OF URL: %s", rest);

	MutableData *sendBuffer = new(MutableData);
	char *operationString = operation == HTTPOperationGet ? "GET" : "POST";
	char buffer[16384];
	snprintf (buffer, sizeof(buffer)-1, "%s /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.0; rv:51.0) Gecko/20100101 Firefox/51.0\r\nAccept: */*\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: identity\r\nReferer: %s\r\nConnection: close\r\n\n",
		operationString,
		rest,
		domain,
		url
	);

	$(sendBuffer, appendBytes, (uint8_t*) buffer, strlen(buffer));

	Log_debug_printf (__FUNCTION__, "REQUEST: %s", buffer);

	if (operation == HTTPOperationPost) {
		$(sendBuffer, appendBytes, post_payload, /* XX */ strlen((char*) post_payload));
	}

	const char *bytes = (const char*) $(sendBuffer, bytes);
	size_t nBytes = $(sendBuffer, length);
	long bytes_sent = sendto (sockfd, bytes, nBytes, 0, (struct sockaddr*)&addr, sizeof(addr));
	release (sendBuffer);

	if (bytes_sent < 0) {
		Log_perror(__FUNCTION__, "sendto");
		Log_debug_printf (__FUNCTION__, "Error sending request: %s\n", strerror(errno));
		return NULL;
	}

	if (bytes_sent != nBytes) {
		Log_debug (__FUNCTION__, "Failed to send entirety of request.");
		return NULL;
	}

	MutableData *receiveBuffer = new(MutableData);
#define RECV_BUFFER_SIZE (32768)
	uint8_t chunkBuffer [RECV_BUFFER_SIZE];

	// Fetch the response from the server, in chunks if necessary.
	long nBytesReceivedInThisChunk = 0;
	while ((nBytesReceivedInThisChunk = recv (sockfd, chunkBuffer, RECV_BUFFER_SIZE, 0))) {
		if (nBytesReceivedInThisChunk <= 0)
			break;
		$(receiveBuffer, appendBytes, chunkBuffer, nBytesReceivedInThisChunk);
	}
	close (sockfd);

	Log_debug_printf (__FUNCTION__, "TOTAL BYTES RECEIVED: %lu", $(receiveBuffer, length));

	uint8_t *receivedBytes = $(receiveBuffer, bytes);

	// Extract the response status code.
	int status_code = 0;
	char *status_line = strstr((const char*) receivedBytes, "HTTP/1");
	if (status_line) {
		char *tmp = strchr (status_line, ' ');
		if (tmp) {
			status_code = atoi (++tmp);
		}
	}

	Log_debug_printf (__FUNCTION__, "RESPONSE STATUS CODE: %d\n", status_code);

	// Handle redirections
	//
	if (status_code == 301 || status_code == 302 || status_code == 307 || status_code == 308) {

		// Case 1: Simple HTTP case
		char *newURLString = strcasestr ((char*) receivedBytes, "Location: http://");
		if (newURLString) {
			char *eolString = strchr (newURLString, '\n');
			if (eolString)
				*eolString=0;
			eolString = strchr (newURLString, '\r');
			if (eolString)
				*eolString=0;

			newURLString += strlen ("Location: "), 
			Log_debug_printf (__FUNCTION__, "FOLLOWING %i REDIRECT to %s", status_code, newURLString);

			MutableData *data = http_synchronous_request (newURLString, operation, post_payload, return_statuscode);
			release (receiveBuffer);
			return data;
		}

		// Case 2: Simple HTTPS case
		newURLString = strcasestr ((char*) receivedBytes, "Location: https://");
		if (newURLString) {
			char *eolString = strchr (newURLString, '\n');
			if (eolString)
				*eolString=0;
			eolString = strchr (newURLString, '\r');
			if (eolString)
				*eolString=0;

			newURLString += strlen ("Location: ");
#ifndef HAVE_LIBCURL
			// Cannot process it if libcurl is missing.
			Log_debug (__FUNCTION__, "Redirect contains unsupported HTTPS scheme.");
			release (receiveBuffer);
			return NULL;
#else 
			Log_debug_printf (__FUNCTION__, "FOLLOWING %i REDIRECT to %s", status_code, newURLString);
			MutableData *data = https_synchronous_request (newURLString, operation, post_payload, return_statuscode);
			release (receiveBuffer);
			return data;
#endif
		}

		// Case 3: URL in first <A> tag in HTML
		newURLString = strcasestr ((char*) receivedBytes, "<A ");
		if (newURLString) {
			newURLString += 3;

			// Case 3a: Full URL is HTTP 
			if (!strncasecmp (newURLString, "href=http://", 12)
			 || !strncasecmp (newURLString, "href='http://", 13)
			 || !strncasecmp (newURLString, "href=\"http://", 13)) {
				newURLString += 5;
				char *ptr = newURLString;
				if (*ptr == '\'' || *ptr == '"')
					ptr++;

				// Find end of URL and truncate.
				while (*ptr) {
					int ch = *ptr;
					if (isspace(ch) || ch == '>' || ch == '"' || ch == '\'') {
						*ptr = 0;
						break;
					}
					ptr++;
				}

				Log_debug_printf (__FUNCTION__, "FOLLOWING %i REDIRECT to %s", status_code, newURLString);
				MutableData *data = http_synchronous_request (newURLString, operation, post_payload, return_statuscode);
				release (receiveBuffer);
				return data;
			}

			// Case 3b: URL is HTTPS
			if (!strncasecmp (newURLString, "href=https://", 13)
			 || !strncasecmp (newURLString, "href='https://", 14)
			 || !strncasecmp (newURLString, "href=\"https://", 14)) {
#ifndef HAVE_LIBCURL
				Log_warning (__FUNCTION__, "Redirect to unsupported HTTPS.");
				release (receiveBuffer);
				return NULL;
#else
				newURLString += 5;
				char *ptr = newURLString;
				if (*ptr == '\'' || *ptr == '"')
					ptr++;

				// Find end of URL and truncate.
				while (*ptr) {
					int ch = *ptr;
					if (isspace(ch) || ch == '>' || ch == '"' || ch == '\'') {
						*ptr = 0;
						break;
					}
					ptr++;
				}

				Log_debug_printf (__FUNCTION__, "FOLLOWING %i REDIRECT to %s", status_code, newURLString);
				MutableData *data = https_synchronous_request (newURLString, operation, post_payload, return_statuscode);
				release (receiveBuffer);
				return data;
#endif
			}

			// Case 3c: URL is relative URL HTTP
			if (!strncasecmp (newURLString, "href=", 5)) {
				newURLString += 5;
				char *ptr = newURLString;
				if (*ptr == '"' || *ptr == '\'')
					ptr++;

				// Find end of URL and truncate.
				while (*ptr) {
					int ch = *ptr;
					if (isspace(ch) || ch == '>' || ch == '"' || ch == '\'') {
						*ptr = 0;
						break;
					}
					ptr++;
				}

				char stringWithDomainPrepended [2*PATH_MAX];
				snprintf (stringWithDomainPrepended, sizeof(stringWithDomainPrepended), "https://%s/%s", domain, newURLString);

				Log_debug_printf (__FUNCTION__, "FOLLOWING %i REDIRECT to %s", status_code, newURLString);
				MutableData *data = NULL;
#ifdef HAVE_LIBCURL
				data = https_synchronous_request (newURLString, operation, post_payload, return_statuscode);
#endif
				release (receiveBuffer);
				return data;
			}
		}

		release (receiveBuffer);
		return NULL;
	}

	// Case 4: Soft redirect using Javascript.
	if (200 == status_code) {
		const char *winLocationString = "window.location.href=\"";
		char *newURLString = strcasestr ((char*) receivedBytes, winLocationString);

		if (newURLString) {
			newURLString += strlen (winLocationString);
			char *eolString = strchr (newURLString, '"');
			if (eolString) {
				*eolString=0;
			} else {
				eolString = strchr (newURLString, '\r');
				if (eolString)
					*eolString=0;
			}

			// Case 4a: Non-secure
			newURLString = strcasestr ((char*) receivedBytes, "http://");
			if (newURLString) {
				Log_debug_printf (__FUNCTION__, "FOLLOWING JAVASCRIPT REDIRECT to %s", newURLString);
				MutableData *data = http_synchronous_request (newURLString, operation, post_payload, return_statuscode);
				release (receiveBuffer);
				return data;
			}
			// Case 4b: Secure
			newURLString = strcasestr ((char*) receivedBytes, "https://");
			if (newURLString) {
#ifdef HAVE_LIBCURL
				Log_debug_printf (__FUNCTION__, "FOLLOWING JAVASCRIPT REDIRECT to %s", newURLString);
				MutableData *data = https_synchronous_request (newURLString, operation, post_payload, return_statuscode);
				release (receiveBuffer);
				return data;
#else
				release (receiveBuffer);
				return NULL;
#endif
			}
			
			release (receiveBuffer);
			Log_warning (__FUNCTION__, "Unsupported scheme in redirect URL.");
			return NULL;
		}
	}
	
	*return_statuscode = status_code;
	return receiveBuffer;
}

static Data* URL_synchronousFetch (URL *self, HTTPOperation operation, uint8_t *postData, size_t postLength, unsigned *statusReturn)
{
	if (!self)
		return NULL;
	verifyCorrectClass(self,URL);

	if (!self->url_string)
		return NULL;

	unsigned status_code = 0;

	if (self->scheme == URLSchemeHTTP) {
		MutableData *data = http_synchronous_request (self->url_string, HTTPOperationGet, NULL, &status_code);
		if (data) {
			if (statusReturn)
				*statusReturn = status_code;
			
			return Data_withData (data);
		}
	}
	else if (self->scheme == URLSchemeHTTPS) {
#ifdef HAVE_LIBCURL
		MutableData *data = https_synchronous_request (self->url_string, HTTPOperationGet, NULL, &status_code);
		if (data) {
			if (statusReturn)
				*statusReturn = status_code;

			return Data_withData(data);
		}
#else
		Log_debug (__FUNCTION__, "Found HTTPS but libcurl is not installed.");
#endif
	}

	return NULL;
}

Data* URL_synchronousGet (URL *self, unsigned *statusReturn)
{
	return URL_synchronousFetch (self, HTTPOperationGet, NULL, 0, statusReturn);
}

Data* URL_synchronousPost (URL *self, uint8_t *postData, size_t postLength, unsigned *statusReturn)
{

	return URL_synchronousFetch (self, HTTPOperationPost, postData, postLength, statusReturn);
}

URLClass* URLClass_init (URLClass *class)
{
	SET_SUPERCLASS(Object);

	SET_OVERRIDDEN_METHOD_POINTER(URL,describe);
	SET_OVERRIDDEN_METHOD_POINTER(URL,destroy);

	SET_METHOD_POINTER(URL,string);
	SET_METHOD_POINTER(URL,setCString);
	SET_METHOD_POINTER(URL,scheme);
	SET_METHOD_POINTER(URL,httpOperation);
	SET_METHOD_POINTER(URL,setHTTPOperation);
	SET_METHOD_POINTER(URL,synchronousGet);
	SET_METHOD_POINTER(URL,synchronousPost);
	
        VALIDATE_CLASS_STRUCT(class);
	return class;
}

URL* URL_init (URL *self)
{
	ENSURE_CLASS_READY(URL);

	Object_init ((Object*) self);
	self->is_a = _URLClass;
	
	return self;
}

URL *URL_new () 
{
	URL *self = allocate(URL);
	URL_init (self);
	return self;
}

URL *URL_withCString (const char *url) 
{
	URL *self = new(URL);
	URL_setCString (self, url);
	return self;
}

