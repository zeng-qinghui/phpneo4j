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
#include "ext/standard/php_smart_str.h"
#include "neo4j.h"
#include "neo4jNode.h"
#include "neo4jRelationship.h"
#include "php_neo4j.h"
#include "neo4j_curl.h"

/* If you declare any globals in php_neo4j.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(neo4j)
*/
extern zend_class_entry* neo4jNode_ce;

/* True global resources - no need for thread safety here */
static int le_neo4jRelationship;
static zend_class_entry *neo4jRelationship_exception_ce;
zend_class_entry *neo4jRelationship_ce;

/* {{{ neo4j_functions[]
 *
 * Every user visible function must have an entry in neo4j_functions[].
 */
const zend_function_entry neo4jRelationship_methods[] = {
	PHP_ME(neo4jRelationship,__construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(neo4jRelationship,getProperty, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jRelationship,setProperty, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jRelationship,deleteProperty, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jRelationship,getAllProperties, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jRelationship,setAllProperties, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jRelationship,save, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jRelationship,delete, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jRelationship,startNode, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jRelationship,endNode, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jRelationship,getType, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jRelationship,getId, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jRelationship,getUri, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END	/* Must be the last line in neo4j_functions[] */
};
/* }}} */

void noe4j_init_neo4jRelationship(TSRMLS_D)
{
	zend_class_entry neo4jRelationship_class_entry;
	INIT_CLASS_ENTRY(neo4jRelationship_class_entry,"neo4jRelationship",neo4jRelationship_methods);
	neo4jRelationship_ce = zend_register_internal_class(&neo4jRelationship_class_entry TSRMLS_CC);
	zend_declare_property_long(neo4jRelationship_ce, "_id", sizeof("_id"), -1, ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(neo4jRelationship_ce, "_crashData", sizeof("_crashData"), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(neo4jRelationship_ce, "_data", sizeof("_data"), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(neo4jRelationship_ce, "_exception", sizeof("_exception"), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(neo4jRelationship_ce, "_parent", sizeof("_parent"), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(neo4jRelationship_ce, "_start", sizeof("_start"), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(neo4jRelationship_ce, "_end", sizeof("_end"), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(neo4jRelationship_ce, "_type", sizeof("_type"), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(neo4jRelationship_ce, "_self", sizeof("_self"), ZEND_ACC_PRIVATE TSRMLS_CC);
}

PHP_METHOD(neo4jRelationship, __construct) {
	zval *crashData, *parent, *startNode;
	zval **data, **extension, **type, **self, **start, **end;
	parent = NULL;
	startNode = NULL;
	char *id, *startNodeId, *endNodeId;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|aOO" , &crashData, &parent, neo4j_ce, &startNode, neo4jNode_ce) == FAILURE) {
		return;
	}

	if(zend_hash_find(HASH_OF(crashData),"data", sizeof("data"), (void**)&data) == SUCCESS){
		zend_update_property(neo4jRelationship_ce, getThis(), "_data", sizeof("_data"), *data TSRMLS_CC);
	}

	if(zend_hash_find(HASH_OF(crashData),"exception", sizeof("exception"), (void**)&extension) == SUCCESS){
		zend_update_property(neo4jRelationship_ce, getThis(), "_exception", sizeof("_exception"), *extension TSRMLS_CC);
	}

	if(zend_hash_find(HASH_OF(crashData),"type", sizeof("type"), (void**)&type) == SUCCESS){
		zend_update_property(neo4jRelationship_ce, getThis(), "_type", sizeof("_type"), *type TSRMLS_CC);
	}

	if(zend_hash_find(HASH_OF(crashData),"self", sizeof("self"), (void**)&self) == SUCCESS){
		id = Z_STRVAL_PP(self);
		id = strrchr(id,'/')+1;
		zend_update_property_string(neo4jRelationship_ce, getThis(), "_id", sizeof("_id"), id TSRMLS_CC);
		zend_update_property(neo4jRelationship_ce, getThis(), "_self", sizeof("_self"), *self TSRMLS_CC);
	}

	if(parent){
		zend_update_property(neo4jRelationship_ce, getThis(), "_parent", sizeof("_parent"), parent TSRMLS_CC);
	}

	if(startNode){
		zend_update_property(neo4jRelationship_ce, getThis(), "_start", sizeof("_start"), startNode TSRMLS_CC);
	}else if(zend_hash_find(HASH_OF(crashData),"start", sizeof("start"), (void**)&start) == SUCCESS){
		startNodeId = Z_STRVAL_PP(start);
		startNodeId = strrchr(startNodeId,'/')+1;
		zend_update_property_string(neo4jRelationship_ce, getThis(), "_start", sizeof("_start"), startNodeId TSRMLS_CC);
	}

	if(zend_hash_find(HASH_OF(crashData),"end", sizeof("end"), (void**)&end) == SUCCESS){
		endNodeId = Z_STRVAL_PP(end);
		endNodeId = strrchr(endNodeId,'/')+1;
		zend_update_property_string(neo4jRelationship_ce, getThis(), "_end", sizeof("_end"), endNodeId TSRMLS_CC);
	}

	zend_update_property(neo4jRelationship_ce, getThis(), "_crashData", sizeof("_crashData"), crashData TSRMLS_CC);
}

PHP_METHOD(neo4jRelationship,setProperty){
	zval *value, *data;
	char *key;
	int *keyLen;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz" , &key, &keyLen, &value) == FAILURE)RETURN_NULL();
	data = zend_read_property(neo4jRelationship_ce,getThis(),"_data",sizeof("_data"), 0 TSRMLS_CC);
	if(Z_TYPE_P(data) != IS_ARRAY)RETURN_NULL();
	zval_add_ref(&value);
	add_assoc_zval(data,key,value);
	RETURN_ZVAL(getThis(),1,0);
}

PHP_METHOD(neo4jRelationship,getProperty){
	zval *data;
	zval **value;
	char *key;
	int keyLen;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s" , &key, &keyLen) == FAILURE)RETURN_NULL();
	data = zend_read_property(neo4jRelationship_ce,getThis(),"_data",sizeof("_data"),0 TSRMLS_CC);
	if(Z_TYPE_P(data) != IS_ARRAY)RETURN_NULL();
	if(zend_hash_find(HASH_OF(data),key, keyLen+1, (void**)&value) == SUCCESS){
		RETURN_ZVAL(*value,1,0);
	}
	RETURN_NULL();
}


PHP_METHOD(neo4jRelationship,deleteProperty){
	char *key;
	int keyLen;
	zval *data;
	data = zend_read_property(neo4jRelationship_ce,getThis(),"_data",sizeof("_data"),0 TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s" , &key, &keyLen) == FAILURE)RETURN_NULL();
	zend_hash_del_key_or_index(HASH_OF(data), key, keyLen+1,0,HASH_DEL_KEY);
	RETURN_ZVAL(getThis(),1,0);
}


PHP_METHOD(neo4jRelationship,getAllProperties){
	zval *data;
	data = zend_read_property(neo4jRelationship_ce,getThis(),"_data",sizeof("_data"),0 TSRMLS_CC);
	RETURN_ZVAL(data,1,0);
}

PHP_METHOD(neo4jRelationship,setAllProperties){
	zval *data;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z" , &data) == FAILURE)RETURN_NULL();
	zend_update_property(neo4jRelationship_ce, getThis(), "_data", sizeof("_data"), data TSRMLS_CC);
	RETURN_ZVAL(getThis(),1,0);
}

PHP_METHOD(neo4jRelationship,save){
	zval *data, *parent, *id, *response;
	zval path, method;
	smart_str queryString = {0};
	id = zend_read_property(neo4jRelationship_ce,getThis(),"_id",sizeof("_id"),0 TSRMLS_CC);
	data = zend_read_property(neo4jRelationship_ce,getThis(),"_data",sizeof("_data"),0 TSRMLS_CC);
	parent = zend_read_property(neo4jRelationship_ce,getThis(),"_parent",sizeof("_parent"),0 TSRMLS_CC);
	if(Z_TYPE_P(parent) != IS_OBJECT)RETURN_NULL();
	if(Z_TYPE_P(data) != IS_ARRAY)RETURN_NULL();
	smart_str_appendl(&queryString, "relationship/", strlen("relationship/"));
	smart_str_appendl(&queryString, Z_STRVAL_P(id), Z_STRLEN_P(id));
	smart_str_appendl(&queryString, "/properties", strlen("/properties"));
	smart_str_0(&queryString);
	ZVAL_STRING(&path,queryString.c,1);
	ZVAL_LONG(&method,REST_METHOD_PUT);
	ALLOC_ZVAL(response);
	NEO4J_METHOD3(neo4j, restAPI, response, parent, &method, &path, data);
	smart_str_free(&queryString);
	RETURN_ZVAL(getThis(),1,0);
}

PHP_METHOD(neo4jRelationship,startNode){
	zval *start, *parent;
	start = zend_read_property(neo4jRelationship_ce,getThis(),"_start",sizeof("_start"),0 TSRMLS_CC);
	switch(Z_TYPE_P(start)){
	case IS_OBJECT:
		RETURN_ZVAL(start,1,0);
		break;
	case IS_STRING:
		parent = zend_read_property(neo4jRelationship_ce,getThis(),"_parent",sizeof("_parent"),0 TSRMLS_CC);
		NEO4J_METHOD1(neo4j, getNode, return_value, parent, start);
		zend_update_property(neo4jRelationship_ce, getThis(), "_start", sizeof("_start"), return_value TSRMLS_CC);
		break;
	default:
		RETURN_NULL();
	}
}

PHP_METHOD(neo4jRelationship,endNode){
	zval *end, *parent;
	end = zend_read_property(neo4jRelationship_ce,getThis(),"_end",sizeof("_end"),0 TSRMLS_CC);
	switch(Z_TYPE_P(end)){
	case IS_OBJECT:
		RETURN_ZVAL(end,1,0);
		break;
	case IS_STRING:
		parent = zend_read_property(neo4jRelationship_ce,getThis(),"_parent",sizeof("_parent"),0 TSRMLS_CC);
		NEO4J_METHOD1(neo4j, getNode, return_value, parent, end);
		zend_update_property(neo4jRelationship_ce, getThis(), "_end", sizeof("_end"), return_value TSRMLS_CC);
		break;
	default:
		RETURN_NULL();
	}
}

PHP_METHOD(neo4jRelationship,getType){
	zval *type;
	type = zend_read_property(neo4jRelationship_ce,getThis(),"_type",sizeof("_type"),0 TSRMLS_CC);
	RETURN_ZVAL(type,1,0);
}

PHP_METHOD(neo4jRelationship,getId){
	zval *id;
	id = zend_read_property(neo4jRelationship_ce,getThis(),"_id",sizeof("_id"),0 TSRMLS_CC);
	RETURN_ZVAL(id,1,0);
}


PHP_METHOD(neo4jRelationship,delete){
	zval *id, *parent;
	id = zend_read_property(neo4jRelationship_ce,getThis(),"_id",sizeof("_id"),0 TSRMLS_CC);
	parent = zend_read_property(neo4jRelationship_ce,getThis(),"_parent",sizeof("_parent"),0 TSRMLS_CC);
	if(Z_TYPE_P(parent) != IS_OBJECT)RETURN_NULL();
	NEO4J_METHOD1(neo4j, deleteRelationship, return_value, parent, id);
}

PHP_METHOD(neo4jRelationship,getUri){
	zval *self;
	self = NEO4J_THIS_PROPERTY(neo4jNode_ce, "_self");
	RETURN_ZVAL(self,1,0);
}


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
