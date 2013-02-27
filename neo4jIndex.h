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

#ifndef NEO4JINDEX_H
#define NEO4JINDEX_H

#define NEO4J_INDEX_NODE 0
#define NEO4J_INDEX_RELATIONSHIP 1

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_METHOD(neo4jIndex,__construct);
PHP_METHOD(neo4jIndex,get);
PHP_METHOD(neo4jIndex,add);
PHP_METHOD(neo4jIndex,remove);
PHP_METHOD(neo4jIndex,delete);
PHP_METHOD(neo4jIndex,getOrCreate);
PHP_METHOD(neo4jIndex,getId);


void noe4j_init_neo4jIndex(TSRMLS_D);

#endif	/* NEO4JINDEX_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
