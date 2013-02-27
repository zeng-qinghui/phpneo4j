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

#ifndef NEO4JNODE_H
#define NEO4JNODE_H

#define NEO4J_RELATIONSHIP_ALL 0
#define NEO4J_RELATIONSHIP_IN 1
#define NEO4J_RELATIONSHIP_OUT 2

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_METHOD(neo4jNode,__construct);
PHP_METHOD(neo4jNode,getProperty);
PHP_METHOD(neo4jNode,setProperty);
PHP_METHOD(neo4jNode,deleteProperty);
PHP_METHOD(neo4jNode,getAllProperties);
PHP_METHOD(neo4jNode,setAllProperties);
PHP_METHOD(neo4jNode,getId);
PHP_METHOD(neo4jNode,save);
PHP_METHOD(neo4jNode,delete);
PHP_METHOD(neo4jNode,getRelationships);
PHP_METHOD(neo4jNode,getPathTo);
PHP_METHOD(neo4jNode,getPathFrom);
PHP_METHOD(neo4jNode,getPathsTo);
PHP_METHOD(neo4jNode,getPathsFrom);
PHP_METHOD(neo4jNode,traverseNodes);
PHP_METHOD(neo4jNode,traversePaths);
PHP_METHOD(neo4jNode,traverseRelationships);
PHP_METHOD(neo4jNode,getUri);
void noe4j_init_neo4jNode(TSRMLS_D);

#endif	/* NEO4JNODE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
