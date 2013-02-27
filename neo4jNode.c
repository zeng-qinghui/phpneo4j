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
#include "neo4jNode.h"
#include "neo4jRelationship.h"
#include "php_neo4j.h"

#include "neo4j_curl.h"
#include "../json/JSON_parser.h"
#include "../json/php_json.h"

/* If you declare any globals in php_neo4j.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(neo4j)
*/
extern zend_class_entry* neo4jRelationship_ce;

/* True global resources - no need for thread safety here */
static int le_neo4jNode;
static zend_class_entry *neo4jNode_exception_ce;
zend_class_entry *neo4jNode_ce;


/* {{{ neo4j_functions[]
 *
 * Every user visible function must have an entry in neo4j_functions[].
 */
const zend_function_entry neo4jNode_methods[] = {
	PHP_ME(neo4jNode,__construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(neo4jNode,getProperty, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jNode,setProperty, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jNode,deleteProperty, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jNode,getAllProperties, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jNode,setAllProperties, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jNode,getId, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jNode,save, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jNode,delete, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jNode,getRelationships, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jNode,getPathTo, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jNode,getPathFrom, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jNode,getPathsTo, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jNode,getPathsFrom, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jNode,traverseNodes, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jNode,traversePaths, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jNode,traverseRelationships, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jNode,getUri, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END	/* Must be the last line in neo4j_functions[] */
};
/* }}} */

void noe4j_init_neo4jNode(TSRMLS_D)
{
	REGISTER_MAIN_LONG_CONSTANT("NEO4J_RELATIONSHIP_ALL", NEO4J_RELATIONSHIP_ALL, CONST_CS | CONST_PERSISTENT);
	REGISTER_MAIN_LONG_CONSTANT("NEO4J_RELATIONSHIP_IN", NEO4J_RELATIONSHIP_IN, CONST_CS | CONST_PERSISTENT);
	REGISTER_MAIN_LONG_CONSTANT("NEO4J_RELATIONSHIP_OUT", NEO4J_RELATIONSHIP_OUT, CONST_CS | CONST_PERSISTENT);

	zend_class_entry neo4jNode_class_entry;
	INIT_CLASS_ENTRY(neo4jNode_class_entry,"neo4jNode",neo4jNode_methods);
	neo4jNode_ce = zend_register_internal_class(&neo4jNode_class_entry TSRMLS_CC);
	zend_declare_property_long(neo4jNode_ce, "_id", sizeof("_id"), -1, ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(neo4jNode_ce, "_crashData", sizeof("_crashData"), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(neo4jNode_ce, "_self", sizeof("_self"), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(neo4jNode_ce, "_data", sizeof("_data"), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(neo4jNode_ce, "_exception", sizeof("_exception"), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(neo4jNode_ce, "_parent", sizeof("_parent"), ZEND_ACC_PRIVATE TSRMLS_CC);
}

PHP_METHOD(neo4jNode, __construct) {
	zval *crashData, *parent;
	zval **data, **extension, **self;
	parent = NULL;
	char *id;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|aO" , &crashData, &parent, neo4j_ce) == FAILURE) {
		return;
	}

	if(zend_hash_find(HASH_OF(crashData),"data", sizeof("data"), (void**)&data) == SUCCESS){
		zend_update_property(neo4jNode_ce, getThis(), "_data", sizeof("_data"), *data TSRMLS_CC);
	}

	if(zend_hash_find(HASH_OF(crashData),"exception", sizeof("exception"), (void**)&extension) == SUCCESS){
		zend_update_property(neo4jNode_ce, getThis(), "_exception", sizeof("_exception"), *extension TSRMLS_CC);
	}

	if(zend_hash_find(HASH_OF(crashData),"self", sizeof("self"), (void**)&self) == SUCCESS){
		id = Z_STRVAL_PP(self);
		id = strrchr(id,'/')+1;
		zend_update_property_string(neo4jNode_ce, getThis(), "_id", sizeof("_id"), id TSRMLS_CC);
		zend_update_property(neo4jNode_ce, getThis(), "_self", sizeof("_self"), *self TSRMLS_CC);
	}

	if(parent){
		zend_update_property(neo4jNode_ce, getThis(), "_parent", sizeof("_parent"), parent TSRMLS_CC);
	}


	zend_update_property(neo4jNode_ce, getThis(), "_crashData", sizeof("_crashData"), crashData TSRMLS_CC);
}

PHP_METHOD(neo4jNode,setProperty){
	zval *value, *data;
	char *key;
	int *keyLen;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz" , &key, &keyLen, &value) == FAILURE)RETURN_NULL();
	data = zend_read_property(neo4jNode_ce,getThis(),"_data",sizeof("_data"), 0 TSRMLS_CC);
	if(Z_TYPE_P(data) != IS_ARRAY)RETURN_NULL();
	zval_add_ref(&value);
	add_assoc_zval(data,key,value);
	RETURN_ZVAL(getThis(),1,0);
}

PHP_METHOD(neo4jNode,getProperty){
	zval *data;
	zval **value;
	char *key;
	int keyLen;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s" , &key, &keyLen) == FAILURE)RETURN_NULL();
	data = zend_read_property(neo4jNode_ce,getThis(),"_data",sizeof("_data"),0 TSRMLS_CC);
	if(Z_TYPE_P(data) != IS_ARRAY)RETURN_NULL();
	if(zend_hash_find(HASH_OF(data),key, keyLen+1, (void**)&value) == SUCCESS){
		RETURN_ZVAL(*value,1,0);
	}
	RETURN_NULL();
}


PHP_METHOD(neo4jNode,deleteProperty){
	char *key;
	int keyLen;
	zval *data;
	data = zend_read_property(neo4jNode_ce,getThis(),"_data",sizeof("_data"),0 TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s" , &key, &keyLen) == FAILURE)RETURN_NULL();
	zend_hash_del_key_or_index(HASH_OF(data), key, keyLen+1,0,HASH_DEL_KEY);
	RETURN_ZVAL(getThis(),1,0);
}


PHP_METHOD(neo4jNode,getAllProperties){
	zval *data;
	data = zend_read_property(neo4jNode_ce,getThis(),"_data",sizeof("_data"),0 TSRMLS_CC);
	RETURN_ZVAL(data,1,0);
}

PHP_METHOD(neo4jNode,setAllProperties){
	zval *data;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z" , &data) == FAILURE)RETURN_NULL();
	zend_update_property(neo4jNode_ce, getThis(), "_data", sizeof("_data"), data TSRMLS_CC);
	RETURN_ZVAL(getThis(),1,0);
}

PHP_METHOD(neo4jNode,save){
	zval *data, *parent, *id, *response;
	zval path, method;
	smart_str queryString = {0};
	id = zend_read_property(neo4jNode_ce,getThis(),"_id",sizeof("_id"),0 TSRMLS_CC);
	data = zend_read_property(neo4jNode_ce,getThis(),"_data",sizeof("_data"),0 TSRMLS_CC);
	parent = zend_read_property(neo4jNode_ce,getThis(),"_parent",sizeof("_parent"),0 TSRMLS_CC);
	if(Z_TYPE_P(parent) != IS_OBJECT)RETURN_NULL();
	if(Z_TYPE_P(data) != IS_ARRAY)RETURN_NULL();
	smart_str_appendl(&queryString, "node/", strlen("node/"));
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

PHP_METHOD(neo4jNode,delete){
	zval *id, *parent;
	id = zend_read_property(neo4jNode_ce,getThis(),"_id",sizeof("_id"),0 TSRMLS_CC);
	parent = zend_read_property(neo4jNode_ce,getThis(),"_parent",sizeof("_parent"),0 TSRMLS_CC);
	if(Z_TYPE_P(parent) != IS_OBJECT)RETURN_NULL();
	NEO4J_METHOD1(neo4j, deleteNode, return_value, parent, id);
}

PHP_METHOD(neo4jNode,getId){
	zval *id;
	id = zend_read_property(neo4jNode_ce,getThis(),"_id",sizeof("_id"),0 TSRMLS_CC);
	RETURN_ZVAL(id,1,0);
}

PHP_METHOD(neo4jNode,getRelationships){
	zval *id, *parent, *response;
	zval path, method;
	zval *typeList = NULL;
	int direct = NEO4J_RELATIONSHIP_ALL;
	smart_str queryString = {0};
	zval temp;

	zval         **tmp;
	HashPosition   pos;
	smart_str      implstr = {0};
	int            numelems, i = 0;
	zval tmp_val;
	zval *tmp_val_p;

	char* encodeStr;
	int encodeStrLen = 0;

	zval* returnList;
	zval* perRelationship;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|la", &direct, &typeList) == FAILURE)RETURN_NULL();

	id = zend_read_property(neo4jNode_ce,getThis(),"_id",sizeof("_id"),0 TSRMLS_CC);
	parent = zend_read_property(neo4jNode_ce,getThis(),"_parent",sizeof("_parent"),0 TSRMLS_CC);

	smart_str_appendl(&queryString, "node/", strlen("node/"));
	smart_str_appendl(&queryString, Z_STRVAL_P(id), Z_STRLEN_P(id));
	switch(direct){
	case NEO4J_RELATIONSHIP_IN:
		smart_str_appendl(&queryString, "/relationships/in/", strlen("/relationships/in/"));
		break;
	case NEO4J_RELATIONSHIP_OUT:
		smart_str_appendl(&queryString, "/relationships/out/", strlen("/relationships/out/"));
		break;
	case NEO4J_RELATIONSHIP_ALL:
		smart_str_appendl(&queryString, "/relationships/all/", strlen("/relationships/all/"));
		break;
	}

	if(typeList){
		numelems = zend_hash_num_elements(Z_ARRVAL_P(typeList));
		if(typeList){
			zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(typeList), &pos);
			while (zend_hash_get_current_data_ex(Z_ARRVAL_P(typeList), (void **) &tmp, &pos) == SUCCESS) {
				tmp_val = **tmp;
				zval_copy_ctor(&tmp_val);
				convert_to_string(&tmp_val);
				encodeStr = php_url_encode(Z_STRVAL(tmp_val), Z_STRLEN(tmp_val), &encodeStrLen);
				smart_str_appendl(&implstr,encodeStr,encodeStrLen);
				zval_dtor(&tmp_val);
				if (++i != numelems) {
					smart_str_appendl(&implstr, "&", 1);
				}
				zend_hash_move_forward_ex(Z_ARRVAL_P(typeList), &pos);
			}
			smart_str_0(&implstr);
			if (implstr.len) {
				smart_str_appendl(&queryString, implstr.c, implstr.len);
			}
			smart_str_free(&implstr);
		}
	}
	smart_str_0(&queryString);

	ZVAL_STRING(&path,queryString.c,1);
	ZVAL_LONG(&method,REST_METHOD_GET);
	ALLOC_ZVAL(response);
	NEO4J_METHOD2(neo4j, restAPI, response, parent, &method, &path);

	ALLOC_ZVAL(returnList);
	array_init(returnList);

	int relationshipLen = zend_hash_num_elements(Z_ARRVAL_P(response));
	if(relationshipLen){
		zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(response), &pos);

		while (zend_hash_get_current_data_ex(Z_ARRVAL_P(response), (void **) &tmp, &pos) == SUCCESS) {
			tmp_val_p = *tmp;
			zval_copy_ctor(tmp_val_p);
			ALLOC_ZVAL(perRelationship);
			object_init_ex(perRelationship, neo4jRelationship_ce);
			NEO4J_METHOD2(neo4jRelationship, __construct, &temp, perRelationship, tmp_val_p, parent);
			add_next_index_zval(returnList,perRelationship);
			zend_hash_move_forward_ex(Z_ARRVAL_P(response), &pos);
		}
	}
	smart_str_free(&queryString);
	RETURN_ZVAL(returnList,1,0);
}

PHP_METHOD(neo4jNode,getPathTo){
	zval *to, *type, *algorithm;
	zval *option = NULL;
	zval *parent;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz|z", &to, &type, &algorithm, &option) == FAILURE) {
		RETURN_NULL();
	}
	parent = NEO4J_THIS_PROPERTY(neo4jNode_ce,"_parent");
	if(option){
		NEO4J_METHOD5(neo4j, getPath, return_value, parent, getThis(), to, type, algorithm, option);
	}else{
		NEO4J_METHOD4(neo4j, getPath, return_value, parent, getThis(), to, type, algorithm);
	}
}

PHP_METHOD(neo4jNode,getPathFrom){
	zval *from, *type, *algorithm;
	zval *option = NULL;
	zval *parent;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz|z", &from, &type, &algorithm, &option) == FAILURE) {
		RETURN_NULL();
	}
	parent = NEO4J_THIS_PROPERTY(neo4jNode_ce,"_parent");
	if(option){
		NEO4J_METHOD5(neo4j, getPath, return_value, parent, from, getThis(), type, algorithm, option);
	}else{
		NEO4J_METHOD4(neo4j, getPath, return_value, parent, from, getThis(), type, algorithm);
	}
}

PHP_METHOD(neo4jNode,getPathsTo){
	zval *to, *type, *algorithm;
	zval *option = NULL;
	zval *parent;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz|z", &to, &type, &algorithm, &option) == FAILURE) {
		RETURN_NULL();
	}
	parent = NEO4J_THIS_PROPERTY(neo4jNode_ce,"_parent");
	if(option){
		NEO4J_METHOD5(neo4j, getPaths, return_value, parent, getThis(), to, type, algorithm, option);
	}else{
		NEO4J_METHOD4(neo4j, getPaths, return_value, parent, getThis(), to, type, algorithm);
	}
}

PHP_METHOD(neo4jNode,getPathsFrom){
	zval *from, *type, *algorithm;
	zval *option = NULL;
	zval *parent;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz|z", &from, &type, &algorithm, &option) == FAILURE) {
		RETURN_NULL();
	}
	parent = NEO4J_THIS_PROPERTY(neo4jNode_ce,"_parent");
	if(option){
		NEO4J_METHOD5(neo4j, getPaths, return_value, parent, from, getThis(), type, algorithm, option);
	}else{
		NEO4J_METHOD4(neo4j, getPaths, return_value, parent, from, getThis(), type, algorithm);
	}
}

PHP_METHOD(neo4jNode,traverseNodes){
	zval *option, *response;
	zval method,path;
	zval *parent;
	zval *result;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &option) == FAILURE) {
		RETURN_NULL();
	}
	ZVAL_STRING(&path,neo4j_make_request("node",getThis(),"traverse/node"),0);
	ZVAL_LONG(&method,REST_METHOD_POST);
	ALLOC_ZVAL(response);
	parent = NEO4J_THIS_PROPERTY(neo4jNode_ce,"_parent");
	NEO4J_METHOD3(neo4j, restAPI, response, parent, &method, &path, option);
	result = neo4j_array_data_to_node(response,parent);
	RETURN_ZVAL(result,1,0);
}

PHP_METHOD(neo4jNode,traversePaths){
	zval *option, *response;
	zval method,path;
	zval *parent;
	zval *result;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &option) == FAILURE) {
		RETURN_NULL();
	}
	ZVAL_STRING(&path,neo4j_make_request("node",getThis(),"traverse/path"),0);
	ZVAL_LONG(&method,REST_METHOD_POST);
	ALLOC_ZVAL(response);
	parent = NEO4J_THIS_PROPERTY(neo4jNode_ce,"_parent");
	NEO4J_METHOD3(neo4j, restAPI, response, parent, &method, &path, option);
	result = neo4j_array_data_to_path(response,parent);
	RETURN_ZVAL(result,1,0);
}

PHP_METHOD(neo4jNode,traverseRelationships){
	zval *option, *response;
	zval method,path;
	zval *parent;
	zval *result;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &option) == FAILURE) {
		RETURN_NULL();
	}
	ZVAL_STRING(&path,neo4j_make_request("node",getThis(),"traverse/relationship"),0);
	ZVAL_LONG(&method,REST_METHOD_POST);
	ALLOC_ZVAL(response);
	parent = NEO4J_THIS_PROPERTY(neo4jNode_ce,"_parent");
	NEO4J_METHOD3(neo4j, restAPI, response, parent, &method, &path, option);
	result = neo4j_array_data_to_relationship(response,parent);
	RETURN_ZVAL(result,1,0);
}

PHP_METHOD(neo4jNode,getUri){
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
