#ifndef NEO4J_CURL_C
#define NEO4J_CURL_C

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_smart_str.h"
#include "neo4j.h"
#include "neo4jPath.h"
#include "neo4jNode.h"
#include "neo4jIndex.h"
#include "neo4jRelationship.h"
#include "php_neo4j.h"

#include "neo4j_curl.h"
#include "../json/JSON_parser.h"
#include "../json/php_json.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

char* neo4j_base64_encode(const unsigned char *str, int length, int *ret_length)
{
	const char *current = str;
	char *p;
	char *result;
	const char neo4j_base64_table[] = {
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
		'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
		'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', '\0'
	};
	const char neo4j_base64_pad = '=';
	const short neo4j_base64_reverse_table[256] = {
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -2, -2, -1, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, 62, -2, -2, -2, 63,
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -2, -2, -2, -2, -2, -2,
		-2,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -2, -2, -2, -2, -2,
		-2, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2
	};

	if (length < 0) {
		if (ret_length != NULL) {
			*ret_length = 0;
		}
		return NULL;
	}

	result = (char *) safe_emalloc((length + 2) / 3, 4 * sizeof(char), 1);
	p = result;

	while (length > 2) { /* keep going until we have less than 24 bits */
		*p++ = neo4j_base64_table[current[0] >> 2];
		*p++ = neo4j_base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
		*p++ = neo4j_base64_table[((current[1] & 0x0f) << 2) + (current[2] >> 6)];
		*p++ = neo4j_base64_table[current[2] & 0x3f];

		current += 3;
		length -= 3; /* we just handle 3 octets of data */
	}

	/* now deal with the tail end of things */
	if (length != 0) {
		*p++ = neo4j_base64_table[current[0] >> 2];
		if (length > 1) {
			*p++ = neo4j_base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
			*p++ = neo4j_base64_table[(current[1] & 0x0f) << 2];
			*p++ = neo4j_base64_pad;
		} else {
			*p++ = neo4j_base64_table[(current[0] & 0x03) << 4];
			*p++ = neo4j_base64_pad;
			*p++ = neo4j_base64_pad;
		}
	}
	if (ret_length != NULL) {
		*ret_length = (int)(p - result);
	}
	*p = '\0';
	return result;
}

neo4j_curl* socketInit(char *host, int port, char* userpwd){
	int clientSock;
	struct sockaddr_in  sa;
	char ip[16];
	neo4j_curl* curl;
	char* encodeKey;
	int encodeKeyLen;

	clientSock = socket( AF_INET, SOCK_STREAM, 0 );
	if( clientSock < 0 )
	{
		return 0;
	}
	memset( &sa, 0, sizeof(sa) );
	sa.sin_family = AF_INET;
	sa.sin_port   = htons( port );
	hostname_to_ip(host,ip);
	sa.sin_addr.s_addr = inet_addr(ip);
	if(connect ( clientSock, ( struct sockaddr * ) & sa, sizeof ( struct sockaddr ) )==-1){
		return NULL;
	}
	curl = emalloc(sizeof(neo4j_curl));
	curl->host = host;
	curl->socket = clientSock;


	if(strlen(userpwd)){
		encodeKey = neo4j_base64_encode(userpwd, strlen(userpwd),&encodeKeyLen);
		curl->templateStr = emalloc(68+strlen(host)+encodeKeyLen);
		sprintf(curl->templateStr," HTTP/1.1\r\nAccept: application/json\r\nHost: %s\r\nAuthorization:Basic %s\r\n",host,encodeKey);
	}else{
		curl->templateStr = emalloc(68+strlen(host));
		sprintf(curl->templateStr," HTTP/1.1\r\nAccept: application/json\r\nHost: %s\r\n",host);
	}
	curl->templateStrLen = strlen(curl->templateStr);
	return curl;
}

void neo4j_curl_free(neo4j_curl *curl){
	close(curl->socket);
	efree(curl->templateStr);
	efree(curl);
}

int hostname_to_ip(char * hostname , char* ip)
{
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
    if ( (he = gethostbyname( hostname ) ) == NULL){
    	return 1;
    }
    addr_list = (struct in_addr **) he->h_addr_list;
    for(i = 0; addr_list[i] != NULL; i++){
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }
    return 1;
}

int neo4j_curl_delete(neo4j_curl *curl, const char* url, zval* return_value) {
	int len;
	char *sendData;
	sendData = (char*)emalloc(7+strlen(url)+curl->templateStrLen+3);
	sprintf(sendData,"DELETE %s%s\r\n",url,curl->templateStr);
	send(curl->socket,sendData,7+strlen(url)+curl->templateStrLen+2,0);
	efree(sendData);
	neo4j_curl_exec(curl, return_value);
	return 0;
}


int neo4j_curl_get(neo4j_curl *curl,const char* url, zval* return_value) {
	int len;
	char *sendData;
	sendData = (char*)emalloc(4+strlen(url)+curl->templateStrLen+3);
	sprintf(sendData,"GET %s%s\r\n",url,curl->templateStr);
	send(curl->socket,sendData,4+strlen(url)+curl->templateStrLen+2,0);
	efree(sendData);
	neo4j_curl_exec(curl, return_value);
	return 0;
}

int neo4j_curl_put(neo4j_curl *curl,const char* url,const char* data, zval* return_value){
	int len;
	char *sendData;
	sendData = (char*)emalloc(4+strlen(url)+curl->templateStrLen+strlen(data)+71);
	//Content-Length: 10000
	sprintf(sendData,"PUT %s%sContent-Type: application/json\r\nContent-Length: %d\r\n\r\n%s",url,curl->templateStr,strlen(data),data);
	send(curl->socket,sendData,strlen(sendData),0);
	efree(sendData);
	neo4j_curl_exec(curl, return_value);
	return 0;
}

int neo4j_curl_post(neo4j_curl *curl,const char* url,const char* data, zval* return_value){
	int len;
	char *sendData;
	sendData = (char*)emalloc(5+strlen(url)+curl->templateStrLen+strlen(data)+71);
	sprintf(sendData,"POST %s%sContent-Type: application/json\r\nContent-Length: %d\r\n\r\n%s",url,curl->templateStr,strlen(data),data);
	send(curl->socket,sendData,strlen(sendData),0);
	efree(sendData);
	neo4j_curl_exec(curl, return_value);
	return 0;
}

int neo4j_curl_exec(neo4j_curl *curl, zval* return_value) {
	int len;
	char *tmpBuffer;
	char *jsonStr;
	char httpHeadBuff[64];
	char *lengthHead;
	int bufferLen = 0;
	int i;

	len = recv(curl->socket,httpHeadBuff,63,0);
	lengthHead = strstr(httpHeadBuff,"Content-Length:") + 15;
	if(lengthHead == 15){
		ZVAL_NULL(return_value);
		return -1;
	}
	for(i=0;i<len&lengthHead[i]!='\r';i++){
		if(lengthHead[i]!=' '){
			bufferLen = bufferLen*10 + lengthHead[i] - '0';
		}
	}
	if(!bufferLen){
		return 0;
	}
//	puts(httpHeadBuff);
	tmpBuffer = (char *)emalloc(bufferLen+256);
	len = recv(curl->socket,tmpBuffer,bufferLen+255,0);
	tmpBuffer[len] = '\0';
	jsonStr = strstr(tmpBuffer,"\r\n\r\n")+4;
#ifdef PHP_JSON_OBJECT_AS_ARRAY
	php_json_decode_ex(return_value, jsonStr, strlen(jsonStr), PHP_JSON_OBJECT_AS_ARRAY, JSON_PARSER_DEFAULT_DEPTH TSRMLS_CC);
#else
	php_json_decode(return_value, jsonStr, strlen(jsonStr), 1, 512 TSRMLS_CC);
#endif
	efree(tmpBuffer);
	return 0;
}

#endif

