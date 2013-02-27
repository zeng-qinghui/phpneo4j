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

#ifndef PHP_NEO4J_H
#define PHP_NEO4J_H

#ifndef NULL
#define NULL   ((void *) 0)
#endif

extern zend_module_entry neo4j_module_entry;
#define phpext_neo4j_ptr &neo4j_module_entry

#ifdef PHP_WIN32
#	define PHP_NEO4J_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_NEO4J_API __attribute__ ((visibility("default")))
#else
#	define PHP_NEO4J_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#if ZEND_MODULE_API_NO >= 20060613
// normal, nice method
#define NEO4J_METHOD_BASE(classname, name) zim_##classname##_##name
#else
// gah!  wtf, php 5.1?
#define NEO4J_METHOD_BASE(classname, name) zif_##classname##_##name
#endif /* ZEND_MODULE_API_NO >= 20060613 */

#define PUSH_PARAM(arg) zend_vm_stack_push(arg TSRMLS_CC)
#define POP_PARAM() (void)zend_vm_stack_pop(TSRMLS_C)
#define PUSH_EO_PARAM()
#define POP_EO_PARAM()

#define NEO4J_METHOD_HELPER(classname, name, retval, thisptr, num, param) \
  PUSH_PARAM(param); PUSH_PARAM((void*)num);				\
  PUSH_EO_PARAM();							\
  NEO4J_METHOD_BASE(classname, name)(num, retval, NULL, thisptr, 0 TSRMLS_CC); \
  POP_EO_PARAM();			\
  POP_PARAM(); POP_PARAM();

/* push parameters, call function, pop parameters */
#define NEO4J_METHOD(classname, name, retval, thisptr)			\
  NEO4J_METHOD_BASE(classname, name)(0, retval, NULL, thisptr, 0 TSRMLS_CC);

#define NEO4J_METHOD1(classname, name, retval, thisptr, param1)		\
  NEO4J_METHOD_HELPER(classname, name, retval, thisptr, 1, param1);

#define NEO4J_METHOD2(classname, name, retval, thisptr, param1, param2)	\
  PUSH_PARAM(param1);							\
  NEO4J_METHOD_HELPER(classname, name, retval, thisptr, 2, param2);	\
  POP_PARAM();
#define NEO4J_METHOD3(classname, name, retval, thisptr, param1, param2, param3) \
  PUSH_PARAM(param1); PUSH_PARAM(param2);                               \
  NEO4J_METHOD_HELPER(classname, name, retval, thisptr, 3, param3);     \
  POP_PARAM(); POP_PARAM();

#define NEO4J_METHOD4(classname, name, retval, thisptr, param1, param2, param3, param4) \
  PUSH_PARAM(param1); PUSH_PARAM(param2); PUSH_PARAM(param3);           \
  NEO4J_METHOD_HELPER(classname, name, retval, thisptr, 4, param4);     \
  POP_PARAM(); POP_PARAM(); POP_PARAM();

#define NEO4J_METHOD5(classname, name, retval, thisptr, param1, param2, param3, param4, param5) \
  PUSH_PARAM(param1); PUSH_PARAM(param2); PUSH_PARAM(param3); PUSH_PARAM(param4); \
  NEO4J_METHOD_HELPER(classname, name, retval, thisptr, 5, param5);     \
  POP_PARAM(); POP_PARAM(); POP_PARAM(); POP_PARAM();



/*
  	Declare any global variables you may need between the BEGIN
	and END macros here:

ZEND_BEGIN_MODULE_GLOBALS(neo4j)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(neo4j)
*/

/* In every utility function you add that needs to use variables
   in php_neo4j_globals, call TSRMLS_FETCH(); after declaring other
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as NEO4J_G(variable).  You are
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define NEO4J_G(v) TSRMG(neo4j_globals_id, zend_neo4j_globals *, v)
#else
#define NEO4J_G(v) (neo4j_globals.v)
#endif

#define NEO4J_THIS_PROPERTY(class,name) zend_read_property(class, getThis(), name, sizeof(name), 0 TSRMLS_DC)

PHPAPI char* neo4j_obj_to_url(char* type, zval* base, zval* host);
PHPAPI char* neo4j_make_request(char* type,zval* base,char* attribute);
PHPAPI zval* neo4j_json_decode(char* json);
PHPAPI zval* neo4j_array_data_to_node(zval* data, zval* pNeo4j);
PHPAPI zval* neo4j_array_data_to_relationship(zval* data, zval* pNeo4j);
PHPAPI zval* neo4j_array_data_to_path(zval* data, zval* pNeo4j);

#ifndef PHP_FE_END
#define PHP_FE_END { NULL, NULL, NULL, 0, 0 }
#endif

#endif	/* PHP_NEO4J_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
