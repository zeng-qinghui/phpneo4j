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

#ifndef NEO4JPATH_H
#define NEO4JPATH_H

#define NEO4J_PATH_SHORTESTPATH 0
#define NEO4J_PATH_ALLSIMPLEPATHS 1
#define NEO4J_PATH_ALLPATHS 2
#define NEO4J_PATH_DIJKSTRA 3

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_METHOD(neo4jPath,__construct);
PHP_METHOD(neo4jPath,startNode);
PHP_METHOD(neo4jPath,endNode);
PHP_METHOD(neo4jPath,getLength);
PHP_METHOD(neo4jPath,getNodes);
PHP_METHOD(neo4jPath,getRelationships);
void noe4j_init_neo4jPath(TSRMLS_D);

#endif	/* NEO4JNODE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
