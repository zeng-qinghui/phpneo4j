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
#include "Zend/zend_list.h"
#include "ext/standard/info.h"
#include "ext/standard/php_smart_str.h"
#include "neo4j.h"
#include "neo4jIndex.h"
#include "neo4jRelationship.h"
#include "php_neo4j.h"
#include "neo4j_curl.h"

/* If you declare any globals in php_neo4j.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(neo4j)
*/
extern zend_class_entry* neo4jRelationship_ce;
extern zend_class_entry* neo4jNode_ce;

/* True global resources - no need for thread safety here */
static int le_neo4jIndex;
static zend_class_entry *neo4jIndex_exception_ce;
zend_class_entry *neo4jIndex_ce;


/* {{{ neo4j_functions[]
 *
 * Every user visible function must have an entry in neo4j_functions[].
 */
const zend_function_entry neo4jIndex_methods[] = {
	PHP_ME(neo4jIndex,__construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(neo4jIndex,add, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jIndex,get, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jIndex,remove, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jIndex,delete, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jIndex,getOrCreate, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jIndex,getId, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END	/* Must be the last line in neo4j_functions[] */
};
/* }}} */

void noe4j_init_neo4jIndex(TSRMLS_D)
{

	REGISTER_MAIN_LONG_CONSTANT("NEO4J_INDEX_NODE", NEO4J_INDEX_NODE, CONST_CS | CONST_PERSISTENT);
	REGISTER_MAIN_LONG_CONSTANT("NEO4J_INDEX_RELATIONSHIP", NEO4J_INDEX_RELATIONSHIP, CONST_CS | CONST_PERSISTENT);

	zend_class_entry neo4jIndex_class_entry;
	INIT_CLASS_ENTRY(neo4jIndex_class_entry,"neo4jIndex",neo4jIndex_methods);
	neo4jIndex_ce = zend_register_internal_class(&neo4jIndex_class_entry TSRMLS_CC);
	zend_declare_property_null(neo4jIndex_ce, "_name", sizeof("_name"), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(neo4jIndex_ce, "_type", sizeof("_type"), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(neo4jIndex_ce, "_parent", sizeof("_parent"), ZEND_ACC_PRIVATE TSRMLS_CC);
}

PHP_METHOD(neo4jIndex, __construct) {
	zval *parent;
	char *indexName;
	int *indexNameLen;
	long type;
	parent = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls|O" , &type, &indexName, &indexNameLen, &parent, neo4j_ce) == FAILURE) {
		return;
	}

	switch(type){
	case NEO4J_INDEX_NODE:
		zend_update_property_string(neo4jIndex_ce, getThis(), "_type", sizeof("_type"), "node" TSRMLS_CC);
		break;
	case NEO4J_INDEX_RELATIONSHIP:
		zend_update_property_string(neo4jIndex_ce, getThis(), "_type", sizeof("_type"), "relationship" TSRMLS_CC);
		break;
	default:
		zend_update_property_string(neo4jIndex_ce, getThis(), "_type", sizeof("_type"), "node" TSRMLS_CC);
	}

	zend_update_property_string(neo4jIndex_ce, getThis(), "_name", sizeof("_name"), indexName TSRMLS_CC);

	if(parent){
		zend_update_property(neo4jIndex_ce, getThis(), "_parent", sizeof("_parent"), parent TSRMLS_CC);
	}
}


PHP_METHOD(neo4jIndex,add){
	zval *type, *parent;
	char *key,*value,*queryString;
	int keyLen,valueLen;
	zval *obj;
	zval method, path, sendData;
	zval *response;
	zval uri, temp;
	zend_class_entry *typeEntry;
	type = NEO4J_THIS_PROPERTY(neo4jIndex_ce,"_type");
	typeEntry = strcmp(Z_STRVAL_P(type),"node")==0 ? neo4jNode_ce : neo4jRelationship_ce;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Oss" , &obj, typeEntry, &key, &keyLen, &value, &valueLen) == FAILURE) {
		RETURN_NULL();
	}
	queryString = neo4j_make_request("index",getThis(),"");
	ZVAL_LONG(&method,REST_METHOD_POST);
	ZVAL_STRING(&path,queryString,0);
	parent = NEO4J_THIS_PROPERTY(neo4jIndex_ce, "_parent");

	if(typeEntry == neo4jNode_ce){
		NEO4J_METHOD(neo4jNode, getUri, &uri, obj)
	}else{
		NEO4J_METHOD(neo4jRelationship, getUri, &uri, obj)
	}
	array_init(&sendData);
	add_assoc_string(&sendData, "key", key, 0);
	add_assoc_string(&sendData, "value", value, 0);
	add_assoc_zval(&sendData, "uri", &uri);
	ALLOC_ZVAL(response);
	NEO4J_METHOD3(neo4j, restAPI, response, parent, &method, &path, &sendData);
	if(typeEntry == neo4jNode_ce){
		object_init_ex(return_value, neo4jNode_ce);
		NEO4J_METHOD2(neo4jNode, __construct, &temp, return_value, response, parent);
	}else{
		object_init_ex(return_value, neo4jRelationship_ce);
		NEO4J_METHOD2(neo4jRelationship, __construct, &temp, return_value, response, parent);
	}
	RETURN_ZVAL(getThis(),1,0);
}

PHP_METHOD(neo4jIndex,remove){
	zval *type, *parent;
	char *key,*value,*queryString;
	smart_str searchString = {0};
	int keyLen = 0,valueLen = 0;
	zval *obj;
	zval objId;
	zval method, path, sendData;
	zval *response;
	zval uri, temp;
	zend_class_entry *typeEntry;
	type = NEO4J_THIS_PROPERTY(neo4jIndex_ce,"_type");
	typeEntry = strcmp(Z_STRVAL_P(type),"node")==0 ? neo4jNode_ce : neo4jRelationship_ce;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Oss" , &obj, typeEntry, &key, &keyLen, &value, &valueLen) == FAILURE) {
		RETURN_NULL();
	}

	if(keyLen){
		smart_str_appendl(&searchString, key,keyLen);
		smart_str_appendl(&searchString, "/",1);
		if(valueLen){
			smart_str_appendl(&searchString, value,valueLen);
			smart_str_appendl(&searchString, "/",1);
		}
	}
	if(typeEntry == neo4jNode_ce){
		NEO4J_METHOD(neo4jNode, getId, &objId, obj);
	}else{
		NEO4J_METHOD(neo4jRelationship, getId, &objId, obj);
	}
	smart_str_appendl(&searchString, Z_STRVAL(objId),Z_STRLEN(objId));
	smart_str_0(&searchString);
	queryString = neo4j_make_request("index",getThis(),searchString.c);

	ZVAL_LONG(&method,REST_METHOD_DELETE);
	ZVAL_STRING(&path,queryString,0);
	parent = NEO4J_THIS_PROPERTY(neo4jIndex_ce, "_parent");
	ALLOC_ZVAL(response);
	NEO4J_METHOD2(neo4j, restAPI, response, parent, &method, &path);
	smart_str_free(&searchString);
	RETURN_ZVAL(getThis(),1,0);
}

PHP_METHOD(neo4jIndex,get){
	zval *type, *parent;
	char *key,*value,*queryString;
	int keyLen,valueLen;
	char *encodeKey, *encodeValue, *encodeKeyValuePair;
	int encodeKeyLen,encodeValueLen;
	zval method, path;
	zval *response;
	zend_class_entry *typeEntry;
	zval *rVal;

	type = NEO4J_THIS_PROPERTY(neo4jIndex_ce,"_type");
	typeEntry = strcmp(Z_STRVAL_P(type),"node")==0 ? neo4jNode_ce : neo4jRelationship_ce;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss" , &key, &keyLen, &value, &valueLen) == FAILURE) {
		RETURN_NULL();
	}
	encodeKey = php_url_encode(key, keyLen, &encodeKeyLen);
	encodeValue = php_url_encode(value,valueLen, &encodeValueLen);
	encodeKeyValuePair = (char*)emalloc(encodeKeyLen+encodeValueLen+2);
	sprintf(encodeKeyValuePair,"%s/%s",encodeKey,encodeValue);
	queryString = neo4j_make_request("index", getThis(), encodeKeyValuePair);
	efree(encodeKeyValuePair);
	ZVAL_LONG(&method,REST_METHOD_GET);
	ZVAL_STRING(&path,queryString,0);
	parent = NEO4J_THIS_PROPERTY(neo4jIndex_ce, "_parent");
	ALLOC_ZVAL(response);
	NEO4J_METHOD2(neo4j, restAPI, response, parent, &method, &path);
	if(typeEntry == neo4jNode_ce){
		rVal = neo4j_array_data_to_node(response,parent);
	}else{
		rVal = neo4j_array_data_to_relationship(response,parent);
	}
	RETURN_ZVAL(rVal,1,0);
}

PHP_METHOD(neo4jIndex,getOrCreate){
	zval *type, *parent;
	char *key,*value,*queryString;
	int *keyLen,*valueLen;
	zval *obj;
	zval method, path, sendData;
	zval *response;
	zval uri, temp;
	zend_class_entry *typeEntry;
	type = NEO4J_THIS_PROPERTY(neo4jIndex_ce,"_type");
	typeEntry = strcmp(Z_STRVAL_P(type),"node")==0 ? neo4jNode_ce : neo4jRelationship_ce;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zss" ,&obj, &key, &keyLen, &value, &valueLen) == FAILURE) {
		RETURN_NULL();
	}
	queryString = neo4j_make_request("index",getThis(),"?unique");
	ZVAL_LONG(&method,REST_METHOD_POST);
	ZVAL_STRING(&path,queryString,0);
	parent = NEO4J_THIS_PROPERTY(neo4jIndex_ce, "_parent");

	array_init(&sendData);
	if(Z_TYPE_P(obj)==IS_OBJECT){
		if(typeEntry == neo4jNode_ce){
			NEO4J_METHOD(neo4jNode, getUri, &uri, obj)
		}else{
			NEO4J_METHOD(neo4jRelationship, getUri, &uri, obj)
		}
		add_assoc_zval(&sendData, "uri", &uri);
	}else{
		add_assoc_zval(&sendData, "properties", obj);
	}
	add_assoc_string(&sendData, "key", key, 0);
	add_assoc_string(&sendData, "value", value, 0);
	ALLOC_ZVAL(response);
	NEO4J_METHOD3(neo4j, restAPI, response, parent, &method, &path, &sendData);
	if(typeEntry == neo4jNode_ce){
		object_init_ex(return_value, neo4jNode_ce);
		NEO4J_METHOD2(neo4jNode, __construct, &temp, return_value, response, parent);
	}else{
		object_init_ex(return_value, neo4jRelationship_ce);
		NEO4J_METHOD2(neo4jRelationship, __construct, &temp, return_value, response, parent);
	}
}

PHP_METHOD(neo4jIndex,getId){
	zval *type,*name;
	char *idStr;
	type = NEO4J_THIS_PROPERTY(neo4jIndex_ce,"_type");
	name = NEO4J_THIS_PROPERTY(neo4jIndex_ce,"_name");
	idStr = (char*)emalloc(Z_STRLEN_P(type) + Z_STRLEN_P(name) + 2);
	sprintf(idStr,"%s/%s",Z_STRVAL_P(type),Z_STRVAL_P(name));
	RETURN_STRING(idStr,0);
}

PHP_METHOD(neo4jIndex,delete){
	zval id;
	zval *response;
	zval method, path;
	zval *parent;
	char* queryString;
	NEO4J_METHOD(neo4jIndex, getId, &id, getThis());
	queryString = neo4j_make_request("index",getThis(),"");
	ZVAL_LONG(&method,REST_METHOD_DELETE);
	ZVAL_STRING(&path,queryString,0);
	parent = NEO4J_THIS_PROPERTY(neo4jIndex_ce, "_parent");
	ALLOC_ZVAL(response);
	NEO4J_METHOD2(neo4j, restAPI, response, parent, &method, &path);
	RETURN_NULL();
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
