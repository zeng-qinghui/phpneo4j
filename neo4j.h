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

#ifndef NEO4J_H
#define NEO4J_H

#ifdef ZTS
#include "TSRM.h"
#endif

typedef enum {REST_METHOD_GET,REST_METHOD_POST,REST_METHOD_DELETE,REST_METHOD_PUT}REST_METHOD;

PHP_MINIT_FUNCTION(neo4j);
PHP_MSHUTDOWN_FUNCTION(neo4j);
PHP_RINIT_FUNCTION(neo4j);
PHP_RSHUTDOWN_FUNCTION(neo4j);
PHP_MINFO_FUNCTION(neo4j);

PHP_METHOD(neo4j,__construct);
PHP_METHOD(neo4j,__destruct);
PHP_METHOD(neo4j,cypher);
PHP_METHOD(neo4j,query);
PHP_METHOD(neo4j,getNode);
PHP_METHOD(neo4j,createNode);
PHP_METHOD(neo4j,deleteNode);
PHP_METHOD(neo4j,createRelationship);
PHP_METHOD(neo4j,deleteRelationship);
PHP_METHOD(neo4j,getRelationship);
PHP_METHOD(neo4j,getPath);
PHP_METHOD(neo4j,getPaths);
PHP_METHOD(neo4j,_arrayToNode);
PHP_METHOD(neo4j,restAPI);
PHP_METHOD(neo4j,getRelationshipIndex);
PHP_METHOD(neo4j,getNodeIndex);
PHP_METHOD(neo4j,deleteRelationshipIndex);
PHP_METHOD(neo4j,deleteNodeIndex);


/* True global resources - no need for thread safety here */
static int le_neo4j;

static zend_class_entry *neo4j_ce;
static zend_class_entry *neo4j_exception_ce;

void noe4j_init_neo4j(TSRMLS_D);

#endif	/* PHP_NEO4J_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
