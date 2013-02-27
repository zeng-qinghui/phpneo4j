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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "neo4j.h"
#include "neo4jIndex.h"
#include "neo4jPath.h"
#include "neo4jNode.h"
#include "neo4jRelationship.h"
#include "php_neo4j.h"
#include "../json/JSON_parser.h"
#include "../json/php_json.h"

extern zend_class_entry* neo4jNode_ce;
extern zend_class_entry* neo4jPath_ce;
extern zend_class_entry* neo4jIndex_ce;
extern zend_class_entry* neo4jRelationship_ce;
/* If you declare any globals in php_neo4j.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(neo4j)
*/


/* {{{ neo4j_functions[]
 *
 * Every user visible function must have an entry in neo4j_functions[].
 */
const zend_function_entry neo4j_functions[] = {
	PHP_FE_END	/* Must be the last line in neo4j_functions[] */
};
/* }}} */

/* {{{ neo4j_module_entry
 */
zend_module_entry neo4j_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"neo4j",
	neo4j_functions,
	PHP_MINIT(neo4j),
	PHP_MSHUTDOWN(neo4j),
	PHP_RINIT(neo4j),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(neo4j),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(neo4j),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_NEO4J
ZEND_GET_MODULE(neo4j)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("neo4j.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_neo4j_globals, neo4j_globals)
    STD_PHP_INI_ENTRY("neo4j.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_neo4j_globals, neo4j_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_neo4j_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_neo4j_init_globals(zend_neo4j_globals *neo4j_globals)
{
	neo4j_globals->global_value = 0;
	neo4j_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(neo4j)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	noe4j_init_neo4jIndex(TSRMLS_C);
	noe4j_init_neo4jPath(TSRMLS_C);
	noe4j_init_neo4jRelationship(TSRMLS_C);
	noe4j_init_neo4jNode(TSRMLS_C);
	noe4j_init_neo4j(TSRMLS_C);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(neo4j)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(neo4j)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(neo4j)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(neo4j)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "neo4j support", "enabled");
	php_info_print_table_row(2, "version", "0.0.1");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */



PHPAPI char* neo4j_obj_to_url(char* type, zval* base, zval* host){
	zval* idStr;
	char* path;
	if(Z_TYPE_P(base) == IS_OBJECT){
		ALLOC_ZVAL(idStr);
		if(strcmp(type,"node")==0){
			NEO4J_METHOD(neo4jNode, getId, idStr, base);
		}else if(strcmp(type,"relationship")==0){
			NEO4J_METHOD(neo4jRelationship, getId, idStr, base);
		}
	}else{
		convert_to_string(base);
		idStr = base;
	}
	path = (char *)emalloc(Z_STRLEN_P(host) + strlen("/db/data/node/") + Z_STRLEN_P(idStr) + 1);
	sprintf(path,"%s/db/data/node/%s",Z_STRVAL_P(host),Z_STRVAL_P(idStr));
	return path;
}

PHPAPI char* neo4j_make_request(char* type,zval* base,char* attribute){
	zval* idStr;
	char* request;
	if(Z_TYPE_P(base) == IS_OBJECT){
		ALLOC_ZVAL(idStr);
		if(strcmp(type,"node")==0){
			NEO4J_METHOD(neo4jNode, getId, idStr, base);
		}else if(strcmp(type,"relationship")==0){
			NEO4J_METHOD(neo4jRelationship, getId, idStr, base);
		}else if(strcmp(type,"index")==0){
			NEO4J_METHOD(neo4jIndex, getId, idStr, base);
		}
	}else{
		convert_to_string(base);
		idStr = base;
	}
	if(strlen(attribute) && attribute[0]!='?'){
		request = (char*)emalloc(strlen(type) + 1 + Z_STRLEN_P(idStr) + 1 + strlen(attribute) + 1);
		sprintf(request, "%s/%s/%s", type, Z_STRVAL_P(idStr), attribute);
	}else{
		request = (char*)emalloc(strlen(type) + 1 + Z_STRLEN_P(idStr) + strlen(attribute) + 1);
		sprintf(request, "%s/%s%s", type, Z_STRVAL_P(idStr), attribute);
	}
	return request;
}

PHPAPI zval* neo4j_json_decode(char* json){
	zval* rVal;
	ALLOC_ZVAL(rVal);
#ifdef PHP_JSON_OBJECT_AS_ARRAY
	php_json_decode_ex(rVal, json, strlen(json), PHP_JSON_OBJECT_AS_ARRAY, JSON_PARSER_DEFAULT_DEPTH TSRMLS_CC);
#else
	php_json_decode(rVal, json, strlen(json), 1, 512 TSRMLS_CC);
#endif
	return rVal;
}

PHPAPI zval* neo4j_array_data_to_relationship(zval* data, zval* pNeo4j)
{
	zval temp, **tmp, *tmp_val_p, *li, *returnList;
	HashPosition   pos;
	ALLOC_ZVAL(returnList);
	array_init(returnList);
	int relationshipLen = zend_hash_num_elements(Z_ARRVAL_P(data));
	if(relationshipLen){
		zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(data), &pos);
		while (zend_hash_get_current_data_ex(Z_ARRVAL_P(data), (void **) &tmp, &pos) == SUCCESS) {
			tmp_val_p = *tmp;
			zval_copy_ctor(tmp_val_p);
			ALLOC_ZVAL(li);
			object_init_ex(li, neo4jRelationship_ce);
			NEO4J_METHOD2(neo4jRelationship, __construct, &temp, li, tmp_val_p, pNeo4j);
			add_next_index_zval(returnList,li);
			zend_hash_move_forward_ex(Z_ARRVAL_P(data), &pos);
		}
	}
	return returnList;
}

PHPAPI zval* neo4j_array_data_to_node(zval* data, zval* pNeo4j)
{
	zval temp, **tmp, *tmp_val_p, *li, *returnList;
	HashPosition   pos;
	ALLOC_ZVAL(returnList);
	array_init(returnList);
	int relationshipLen = zend_hash_num_elements(Z_ARRVAL_P(data));
	if(relationshipLen){
		zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(data), &pos);
		while (zend_hash_get_current_data_ex(Z_ARRVAL_P(data), (void **) &tmp, &pos) == SUCCESS) {
			tmp_val_p = *tmp;
			zval_copy_ctor(tmp_val_p);
			ALLOC_ZVAL(li);
			object_init_ex(li, neo4jNode_ce);
			NEO4J_METHOD2(neo4jNode, __construct, &temp, li, tmp_val_p, pNeo4j);
			add_next_index_zval(returnList,li);
			zend_hash_move_forward_ex(Z_ARRVAL_P(data), &pos);
		}
	}
	return returnList;
}

PHPAPI zval* neo4j_array_data_to_path(zval* data, zval* pNeo4j)
{
	zval temp, **tmp, *tmp_val_p, *li, *returnList;
	HashPosition   pos;
	ALLOC_ZVAL(returnList);
	array_init(returnList);
	int relationshipLen = zend_hash_num_elements(Z_ARRVAL_P(data));
	if(relationshipLen){
		zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(data), &pos);
		while (zend_hash_get_current_data_ex(Z_ARRVAL_P(data), (void **) &tmp, &pos) == SUCCESS) {
			tmp_val_p = *tmp;
			zval_copy_ctor(tmp_val_p);
			ALLOC_ZVAL(li);
			object_init_ex(li, neo4jPath_ce);
			NEO4J_METHOD2(neo4jPath, __construct, &temp, li, tmp_val_p, pNeo4j);
			add_next_index_zval(returnList,li);
			zend_hash_move_forward_ex(Z_ARRVAL_P(data), &pos);
		}
	}
	return returnList;
}
