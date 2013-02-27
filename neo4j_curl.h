/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:  Zeng Ohm                                                    |
  | E-mail:  zengohm@gmail.com                                           |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef NEO4J_CURL_H
#define NEO4J_CURL_H


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

typedef struct{
	char* host;
	char* templateStr;
	int templateStrLen;
	int socket;
}neo4j_curl;

char* neo4j_base64_encode(const unsigned char *str, int length, int *ret_length);
int neo4j_curl_get(neo4j_curl *curl,const char* url, zval* return_value);
int neo4j_curl_delete(neo4j_curl *curl, const char* url, zval* return_value);
int neo4j_curl_post(neo4j_curl *curl,const char* url,const char* data, zval* return_value);
int neo4j_curl_put(neo4j_curl *curl,const char* url,const char* data, zval* return_value);
int neo4j_curl_exec(neo4j_curl *curl, zval* return_value);
void neo4j_curl_free(neo4j_curl *curl);

neo4j_curl* socketInit(char *host, int port,char* userpwd);
int hostname_to_ip(char * hostname , char* ip);

#endif
