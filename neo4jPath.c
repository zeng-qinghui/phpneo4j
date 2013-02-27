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
#include "neo4j.h"
#include "neo4jPath.h"
#include "neo4jRelationship.h"
#include "php_neo4j.h"

#include "neo4j_curl.h"

/* If you declare any globals in php_neo4j.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(neo4j)
*/
extern zend_class_entry* neo4jRelationship_ce;
extern zend_class_entry* neo4jNode_ce;

/* True global resources - no need for thread safety here */
static int le_neo4jPath;
static zend_class_entry *neo4jPath_exception_ce;
zend_class_entry *neo4jPath_ce;


/* {{{ neo4j_functions[]
 *
 * Every user visible function must have an entry in neo4j_functions[].
 */
const zend_function_entry neo4jPath_methods[] = {
	PHP_ME(neo4jPath,__construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(neo4jPath,startNode, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jPath,endNode, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jPath,getLength, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jPath,getNodes, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4jPath,getRelationships, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END	/* Must be the last line in neo4j_functions[] */
};
/* }}} */

void noe4j_init_neo4jPath(TSRMLS_D)
{

	REGISTER_MAIN_LONG_CONSTANT("NEO4J_PATH_SHORTESTPATH", NEO4J_PATH_SHORTESTPATH, CONST_CS | CONST_PERSISTENT);
	REGISTER_MAIN_LONG_CONSTANT("NEO4J_PATH_ALLSIMPLEPATHS", NEO4J_PATH_ALLSIMPLEPATHS, CONST_CS | CONST_PERSISTENT);
	REGISTER_MAIN_LONG_CONSTANT("NEO4J_PATH_ALLPATHS", NEO4J_PATH_ALLPATHS, CONST_CS | CONST_PERSISTENT);
	REGISTER_MAIN_LONG_CONSTANT("NEO4J_PATH_DIJKSTRA", NEO4J_PATH_DIJKSTRA, CONST_CS | CONST_PERSISTENT);

	zend_class_entry neo4jPath_class_entry;
	INIT_CLASS_ENTRY(neo4jPath_class_entry,"neo4jPath",neo4jPath_methods);
	neo4jPath_ce = zend_register_internal_class(&neo4jPath_class_entry TSRMLS_CC);
	zend_declare_property_long(neo4jPath_ce, "_start", sizeof("_start"), -1, ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(neo4jPath_ce, "_nodes", sizeof("_nodes"), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(neo4jPath_ce, "_length", sizeof("_length"), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(neo4jPath_ce, "_relationships", sizeof("_relationships"), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(neo4jPath_ce, "_end", sizeof("_end"), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(neo4jPath_ce, "_parent", sizeof("_parent"), ZEND_ACC_PRIVATE TSRMLS_CC);
}

PHP_METHOD(neo4jPath, __construct) {
	zval *crashData, *parent;
	zval **start, **nodes, **length, **relationships, **end;
	parent = NULL;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|aO" , &crashData, &parent, neo4j_ce) == FAILURE) {
		return;
	}

	if(zend_hash_find(HASH_OF(crashData),"start", sizeof("start"), (void**)&start) == SUCCESS){
		zend_update_property(neo4jPath_ce, getThis(), "_start", sizeof("_start"), *start TSRMLS_CC);
	}

	if(zend_hash_find(HASH_OF(crashData),"nodes", sizeof("nodes"), (void**)&nodes) == SUCCESS){
		zend_update_property(neo4jPath_ce, getThis(), "_nodes", sizeof("_nodes"), *nodes TSRMLS_CC);
	}

	if(zend_hash_find(HASH_OF(crashData),"length", sizeof("length"), (void**)&length) == SUCCESS){
		zend_update_property(neo4jPath_ce, getThis(), "_length", sizeof("_length"), *length TSRMLS_CC);
	}

	if(zend_hash_find(HASH_OF(crashData),"relationships", sizeof("relationships"), (void**)&relationships) == SUCCESS){
		zend_update_property(neo4jPath_ce, getThis(), "_relationships", sizeof("_relationships"), *relationships TSRMLS_CC);
	}

	if(zend_hash_find(HASH_OF(crashData),"end", sizeof("end"), (void**)&end) == SUCCESS){
		zend_update_property(neo4jPath_ce, getThis(), "_end", sizeof("_end"), *end TSRMLS_CC);
	}

	if(parent){
		zend_update_property(neo4jPath_ce, getThis(), "_parent", sizeof("_parent"), parent TSRMLS_CC);
	}
}

PHP_METHOD(neo4jPath,startNode){
	zval *node, *parent;
	char *idStr;
	zval *id;
	node = NEO4J_THIS_PROPERTY(neo4jPath_ce,"_start");
	if(Z_TYPE_P(node)==IS_STRING){
		parent = NEO4J_THIS_PROPERTY(neo4jPath_ce,"_parent");
		idStr = Z_STRVAL_P(node);
		idStr = strrchr(idStr,'/') + 1;
		ALLOC_ZVAL(id);
		ZVAL_STRING(id,idStr,1);
		NEO4J_METHOD1(neo4j, getNode, return_value, parent, id);
		zend_update_property(neo4jPath_ce, getThis(), "_start", sizeof("_start"), return_value TSRMLS_CC);
	}else{
		RETURN_ZVAL(node,1,0);
	}
}

PHP_METHOD(neo4jPath,endNode){
	zval *node, *parent;
	char *idStr;
	zval *id;
	node = NEO4J_THIS_PROPERTY(neo4jPath_ce,"_end");
	if(Z_TYPE_P(node)==IS_STRING){
		parent = NEO4J_THIS_PROPERTY(neo4jPath_ce,"_parent");
		idStr = Z_STRVAL_P(node);
		idStr = strrchr(idStr,'/') + 1;
		ALLOC_ZVAL(id);
		ZVAL_STRING(id,idStr,1);
		NEO4J_METHOD1(neo4j, getNode, return_value, parent, id);
		zend_update_property(neo4jPath_ce, getThis(), "_end", sizeof("_end"), return_value TSRMLS_CC);
	}else{
		RETURN_ZVAL(node,1,0);
	}
}

PHP_METHOD(neo4jPath,getLength){
	zval* length = NEO4J_THIS_PROPERTY(neo4jPath_ce,"_length");
	RETURN_ZVAL(length,1,0);
}

PHP_METHOD(neo4jPath,getRelationships){
	zval* relationships;
	zval* returnList;
	zval         **tmp;
	HashPosition   pos;
	zval *perRelationship;
	zval *tmp_val_p;
	zval *parent;
	zval id;
	char* idStr;

	parent = NEO4J_THIS_PROPERTY(neo4jPath_ce,"_parent");
	relationships = NEO4J_THIS_PROPERTY(neo4jPath_ce,"_relationships");
	ALLOC_ZVAL(returnList);
	array_init(returnList);
	if(zend_hash_num_elements(Z_ARRVAL_P(relationships))){
		zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(relationships), &pos);
		while (zend_hash_get_current_data_ex(Z_ARRVAL_P(relationships), (void **) &tmp, &pos) == SUCCESS) {
			tmp_val_p = *tmp;
			zval_copy_ctor(tmp_val_p);
			if(Z_TYPE_P(tmp_val_p)==IS_STRING){
				ALLOC_ZVAL(perRelationship);
				object_init_ex(perRelationship, neo4jRelationship_ce);
				idStr = Z_STRVAL_P(tmp_val_p);
				idStr = strrchr(idStr,'/') + 1;
				ZVAL_STRING(&id,idStr,1);
				NEO4J_METHOD1(neo4j, getRelationship, perRelationship, parent ,&id);
			}else{
				perRelationship = tmp_val_p;
			}
			add_next_index_zval(returnList,perRelationship);
			zend_hash_move_forward_ex(Z_ARRVAL_P(relationships), &pos);
		}
	}
	RETURN_ZVAL(returnList,1,0);
}

PHP_METHOD(neo4jPath,getNodes){
	zval* nodes;
	zval* returnList;
	zval         **tmp;
	HashPosition   pos;
	zval *perNode;
	zval *tmp_val_p;
	zval *parent;
	zval id;
	char* idStr;

	parent = NEO4J_THIS_PROPERTY(neo4jPath_ce,"_parent");
	nodes = NEO4J_THIS_PROPERTY(neo4jPath_ce,"_nodes");
	ALLOC_ZVAL(returnList);
	array_init(returnList);
	if(zend_hash_num_elements(Z_ARRVAL_P(nodes))){
		zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(nodes), &pos);
		while (zend_hash_get_current_data_ex(Z_ARRVAL_P(nodes), (void **) &tmp, &pos) == SUCCESS) {
			tmp_val_p = *tmp;
			zval_copy_ctor(tmp_val_p);
			if(Z_TYPE_P(tmp_val_p)==IS_STRING){
				ALLOC_ZVAL(perNode);
				object_init_ex(perNode, neo4jNode_ce);
				idStr = Z_STRVAL_P(tmp_val_p);
				idStr = strrchr(idStr,'/') + 1;
				ZVAL_STRING(&id,idStr,1);
				NEO4J_METHOD1(neo4j, getNode, perNode, parent ,&id);
			}else{
				perNode = tmp_val_p;
			}
			add_next_index_zval(returnList,perNode);
			zend_hash_move_forward_ex(Z_ARRVAL_P(nodes), &pos);
		}
	}
	RETURN_ZVAL(returnList,1,0);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
