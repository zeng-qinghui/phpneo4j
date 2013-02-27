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
#include "neo4jPath.h"
#include "neo4jNode.h"
#include "neo4jIndex.h"
#include "neo4jRelationship.h"
#include "php_neo4j.h"

#include "neo4j_curl.h"
#include "../json/JSON_parser.h"
#include "../json/php_json.h"


/* {{{ neo4j_functions[]
 *
 * Every user visible function must have an entry in neo4j_functions[].
 */
const zend_function_entry neo4j_methods[] = {
	PHP_ME(neo4j, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(neo4j, __destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	PHP_ME(neo4j, restAPI, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4j, getNode, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4j, deleteNode, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4j, createNode, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4j, createRelationship, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4j, deleteRelationship, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4j, getRelationship, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4j, getPath, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4j, getPaths, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4j, cypher, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4j, query, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4j, getRelationshipIndex, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4j, getNodeIndex, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4j, deleteRelationshipIndex, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4j, deleteNodeIndex, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(neo4j, _arrayToNode, NULL, ZEND_ACC_PRIVATE)
	PHP_FE_END	/* Must be the last line in neo4j_functions[] */
};
/* }}} */


extern zend_class_entry* neo4jPath_ce;
extern zend_class_entry* neo4jNode_ce;
extern zend_class_entry* neo4jRelationship_ce;
extern zend_class_entry* neo4jIndex_ce;

void noe4j_init_neo4j(TSRMLS_D)
{
	zend_class_entry neo4j_class_entry;
	INIT_CLASS_ENTRY(neo4j_class_entry,"neo4j",neo4j_methods);
	neo4j_ce = zend_register_internal_class(&neo4j_class_entry TSRMLS_CC);
	zend_declare_property_stringl(neo4j_ce, "_host", sizeof("_host"), "", 0, ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_stringl(neo4j_ce, "_username", sizeof("_username"), "", 0, ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_stringl(neo4j_ce, "_password", sizeof("_password"), "", 0, ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_stringl(neo4j_ce, "_userpwd", sizeof("_userpwd"), "", 0, ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(neo4j_ce, "_tcp", sizeof("_tcp"), ZEND_ACC_PRIVATE TSRMLS_CC);
}


PHP_METHOD(neo4j, __construct) {
	char *host = "localhost";
	int host_len = 15;
	int port = 7474;
	char *userpsw = "";
	int userpsw_len = 0;
	zval *tmpTcp;
	int i;
	neo4j_curl* tcp;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|sls", &host, &host_len, &port, &userpsw, &userpsw_len) == FAILURE) {
    	return;
  	}
	zend_update_property_stringl(neo4j_ce, getThis(), "_host", sizeof("_host"), "", 0 TSRMLS_CC);
  	zend_update_property_stringl(neo4j_ce, getThis(), "_userpwd", sizeof("_userpwd"), userpsw, userpsw_len TSRMLS_CC);
  	tcp = socketInit(host,port,userpsw);
  	ALLOC_ZVAL(tmpTcp);
  	ZVAL_RESOURCE(tmpTcp,tcp);
  	zend_update_property(neo4j_ce, getThis(), "_tcp", sizeof("_tcp"), tmpTcp TSRMLS_CC);
}

PHP_METHOD(neo4j,__destruct){
	zval* tcp;
	tcp = NEO4J_THIS_PROPERTY(neo4j_ce,"_tcp");
	if(Z_RESVAL_P(tcp)>0){
		neo4j_curl_free(Z_RESVAL_P(tcp));
	}
}

PHP_METHOD(neo4j,restAPI){
	char* path;
	char* buffer;
	int pathLen, bufferLen;
	zval* data = NULL;
	zval* host;
	zval* tcp;
	REST_METHOD method;
	char* url;
	smart_str sendData = {0};

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls|a", &method, &path, &pathLen , &data) == FAILURE) {
		RETURN_NULL();
	}
	host = zend_read_property(neo4j_ce,getThis(), "_host", sizeof("_host"), 0 TSRMLS_DC);
	tcp = NEO4J_THIS_PROPERTY(neo4j_ce,"_tcp");
	if(Z_RESVAL_P(tcp)<=0){
		RETURN_NULL();
	}
	url = emalloc(pathLen+10);
	sprintf(url,"/db/data/%s",path);
	switch(method){
	case REST_METHOD_GET:
		bufferLen = neo4j_curl_get(Z_RESVAL_P(tcp), url,return_value);
		break;
	case REST_METHOD_POST:
		if(data){
			php_json_encode(&sendData, data, 0 TSRMLS_DC);
			smart_str_0(&sendData);
			bufferLen = neo4j_curl_post(Z_RESVAL_P(tcp), url, sendData.c ,return_value);
		}else{
			bufferLen = neo4j_curl_post(Z_RESVAL_P(tcp), url, "" ,return_value);
		}
		break;
	case REST_METHOD_DELETE:
		bufferLen = neo4j_curl_delete(Z_RESVAL_P(tcp), url,return_value);
		break;
	case REST_METHOD_PUT:
		if(data){
			php_json_encode(&sendData, data, 0 TSRMLS_DC);
			smart_str_0(&sendData);
			bufferLen = neo4j_curl_put(Z_RESVAL_P(tcp), url, sendData.c , return_value);
		}else{
			bufferLen = neo4j_curl_put(Z_RESVAL_P(tcp), url, "" ,return_value);
		}
		break;
	default:
		;
	}
	smart_str_free(&sendData);
	efree(url);
}

PHP_METHOD(neo4j,_arrayToNode){
	zval* crashData;
	zval temp;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &crashData) == FAILURE) {
		RETURN_NULL();
	}
	object_init_ex(return_value, neo4jNode_ce);
	NEO4J_METHOD2(neo4jNode, __construct, &temp, return_value, crashData, getThis());
}

PHP_METHOD(neo4j,getNode){
	char *nodeId;
	int nodeIdLen;
	smart_str queryString = {0};
	zval path, method;
	zval *response;
	zval **test;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &nodeId, &nodeIdLen) == FAILURE) {
		RETURN_NULL();
	}

	if(nodeIdLen>0){
		smart_str_appendl(&queryString, "node/", strlen("node/"));
		smart_str_appendl(&queryString, nodeId, nodeIdLen);
		smart_str_0(&queryString);
		ZVAL_STRING(&path,queryString.c,1);
		ZVAL_LONG(&method,REST_METHOD_GET);
		ALLOC_ZVAL(response);
		NEO4J_METHOD2(neo4j, restAPI, response, getThis(), &method, &path);
		if(ZVAL_IS_NULL(response)){
			RETURN_NULL();
		}
		if(zend_hash_find(HASH_OF(response),"exception", sizeof("exception"), (void**)&test) == SUCCESS){
			RETURN_NULL();
		}
		NEO4J_METHOD1(neo4j, _arrayToNode, return_value, getThis(), response);
		smart_str_free(&queryString);
	}else{
		RETURN_NULL();
	}
}

PHP_METHOD(neo4j,deleteNode){
	char *nodeId;
	int nodeIdLen;
	smart_str queryString = {0};
	zval path, method;
	zval *response;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &nodeId, &nodeIdLen) == FAILURE) {
		RETURN_NULL();
	}

	if(nodeIdLen>0){
		smart_str_appendl(&queryString, "node/", strlen("node/"));
		smart_str_appendl(&queryString, nodeId, nodeIdLen);
		smart_str_0(&queryString);

		ZVAL_STRING(&path,queryString.c,1);
		ZVAL_LONG(&method,REST_METHOD_DELETE);
		ALLOC_ZVAL(response);
		NEO4J_METHOD2(neo4j, restAPI, return_value, getThis(), &method, &path);
		smart_str_free(&queryString);
	}else{
		RETURN_NULL();
	}
}

PHP_METHOD(neo4j,createNode){
	zval *nodeData = NULL;
	zval method,path;
	zval *response;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|a", &nodeData) == FAILURE) {
		RETURN_NULL();
	}

	ZVAL_STRING(&path,"node",1);
	ZVAL_LONG(&method,REST_METHOD_POST);
	ALLOC_ZVAL(response);
	if(nodeData){
		NEO4J_METHOD3(neo4j, restAPI, response, getThis(), &method, &path, nodeData);
	}else{
		NEO4J_METHOD2(neo4j, restAPI, response, getThis(), &method, &path);
	}
	NEO4J_METHOD1(neo4j, _arrayToNode, return_value, getThis(), response);
}

PHP_METHOD(neo4j,createRelationship){
	zval *start, *to, *host;
	zval *properties = NULL;
	char *typeStr;
	int typeStrLen;
	smart_str queryString = {0};
	smart_str toString = {0};
	zval *startIdStr, *toIdStr;
	zval *sendData;

	zval method,path,temp;
	zval *response;


	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz|sa", &start, &to, &typeStr, &typeStrLen, &properties) == FAILURE) {
		RETURN_NULL();
	}

	host = zend_read_property(neo4j_ce,getThis(), "_host", sizeof("_host"), 0 TSRMLS_DC);

	smart_str_appendl(&queryString, "node/", strlen("node/"));
	if(Z_TYPE_P(start) == IS_OBJECT){
		ALLOC_ZVAL(startIdStr);
		NEO4J_METHOD(neo4jNode, getId, startIdStr, start);
	}else{
		convert_to_string(start);
		startIdStr = start;
	}
	smart_str_appendl(&queryString, Z_STRVAL_P(startIdStr), Z_STRLEN_P(startIdStr));
	smart_str_appendl(&queryString, "/relationships", strlen("/relationships"));
	smart_str_0(&queryString);

	smart_str_appendl(&toString, Z_STRVAL_P(host), Z_STRLEN_P(host));
	smart_str_appendl(&toString, "/db/data/node/", strlen("/db/data/node/"));
	if(Z_TYPE_P(to) == IS_OBJECT){
		ALLOC_ZVAL(toIdStr);
		NEO4J_METHOD(neo4jNode, getId, toIdStr, to);
	}else{
		convert_to_string(to);
		toIdStr = to;
	}
	smart_str_appendl(&toString, Z_STRVAL_P(toIdStr), Z_STRLEN_P(toIdStr));
	smart_str_0(&toString);

	ALLOC_ZVAL(sendData);
	array_init(sendData);
	add_assoc_string(sendData, "to", toString.c, 0);
	add_assoc_string(sendData, "type", typeStr, 0);
	if(properties){
		add_assoc_zval(sendData, "data", properties);
	}

	ZVAL_STRING(&path,queryString.c,1);
	ZVAL_LONG(&method,REST_METHOD_POST);
	ALLOC_ZVAL(response);
	NEO4J_METHOD3(neo4j, restAPI, response, getThis(), &method, &path, sendData);

	object_init_ex(return_value, neo4jRelationship_ce);
	NEO4J_METHOD2(neo4jRelationship, __construct, &temp, return_value, response, getThis());
	smart_str_free(&toString);
	smart_str_free(&queryString);
}

PHP_METHOD(neo4j,getRelationship){
	char *relationshipId;
	int relationshipIdLen;
	smart_str queryString = {0};
	zval path, method, temp;
	zval *response;
	zval **test;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &relationshipId, &relationshipIdLen) == FAILURE) {
		RETURN_NULL();
	}

	if(relationshipIdLen>0){
		smart_str_appendl(&queryString, "relationship/", strlen("relationship/"));
		smart_str_appendl(&queryString, relationshipId, relationshipIdLen);
		smart_str_0(&queryString);

		ZVAL_STRING(&path,queryString.c,1);
		ZVAL_LONG(&method,REST_METHOD_GET);
		ALLOC_ZVAL(response);
		NEO4J_METHOD2(neo4j, restAPI, response, getThis(), &method, &path);
		if(ZVAL_IS_NULL(response)){
			RETURN_NULL();
		}
		if(zend_hash_find(HASH_OF(response),"exception", sizeof("exception"), (void**)&test) == SUCCESS){
			RETURN_NULL();
		}
		object_init_ex(return_value, neo4jRelationship_ce);
		NEO4J_METHOD2(neo4jRelationship, __construct, &temp, return_value, response, getThis());
		smart_str_free(&queryString);
	}else{
		RETURN_NULL();
	}
}

PHP_METHOD(neo4j,deleteRelationship){
	char *relationshipId;
	int relationshipIdLen;
	smart_str queryString = {0};
	zval path, method;
	zval *response;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &relationshipId, &relationshipIdLen) == FAILURE) {
		RETURN_NULL();
	}

	if(relationshipIdLen>0){
		smart_str_appendl(&queryString, "relationship/", strlen("relationship/"));
		smart_str_appendl(&queryString, relationshipId, relationshipIdLen);
		smart_str_0(&queryString);

		ZVAL_STRING(&path,queryString.c,1);
		ZVAL_LONG(&method,REST_METHOD_DELETE);
		ALLOC_ZVAL(response);
		NEO4J_METHOD2(neo4j, restAPI, return_value, getThis(), &method, &path);
		smart_str_free(&queryString);
	}else{
		RETURN_NULL();
	}
}

PHP_METHOD(neo4j,cypher){
	  zval *params = NULL;
	  char *query;
	  int queryLen;
	  zval *sendData, *response;
	  zval method, path;
	  zval         **tmp, **subtmp;
	  HashPosition   pos, subpos;
	  zval 		*tmp_val_p, *subtmp_val_p;
	  zval *result, *row;
	  char** keyList;
	  int keyListLen;
	  zval **columns;
	  zval **data;
	  zval **testObj;
	  zval *buildedObj;
	  zval temp;
	  int i;

	  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|a", &query, &queryLen,&params) == FAILURE) {
		  RETURN_NULL();
	  }

	  ALLOC_ZVAL(sendData);
	  array_init(sendData);
	  add_assoc_string(sendData, "query", query, 0);
	  if(params){
		  add_assoc_zval(sendData, "params", params);
	  }

	  ZVAL_STRING(&path,"cypher",1);
	  ZVAL_LONG(&method,REST_METHOD_POST);
	  ALLOC_ZVAL(response);
	  NEO4J_METHOD3(neo4j, restAPI, response, getThis(), &method, &path,sendData);

	  if(zend_hash_find(HASH_OF(response),"columns", sizeof("columns"), (void**)&columns) != SUCCESS){
		  RETURN_NULL();
	  }

	  if(zend_hash_find(HASH_OF(response),"data", sizeof("data"), (void**)&data) != SUCCESS){
	  	  RETURN_NULL();
	  }

	  ALLOC_ZVAL(result);
	  array_init(result);

	  i = 0;
	  keyListLen = zend_hash_num_elements(Z_ARRVAL_PP(columns));
	  keyList = emalloc(keyListLen*sizeof(char*));
	  zend_hash_internal_pointer_reset_ex(Z_ARRVAL_PP(columns), &pos);
	  while (zend_hash_get_current_data_ex(Z_ARRVAL_PP(columns), (void **) &tmp, &pos) == SUCCESS) {
		  tmp_val_p = *tmp;
		  zval_copy_ctor(tmp_val_p);
		  keyList[i] = Z_STRVAL_P(tmp_val_p);
		  i++;
		  zend_hash_move_forward_ex(Z_ARRVAL_PP(columns), &pos);
	  }

	  zend_hash_internal_pointer_reset_ex(Z_ARRVAL_PP(data), &pos);
	  while (zend_hash_get_current_data_ex(Z_ARRVAL_PP(data), (void **) &tmp, &pos) == SUCCESS) {
		  tmp_val_p = *tmp;
		  //zval_copy_ctor(row);
		  i=0;
		  zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(tmp_val_p), &subpos);
		  ALLOC_ZVAL(row);
		  array_init(row);
		  while (zend_hash_get_current_data_ex(Z_ARRVAL_P(tmp_val_p), (void **) &subtmp, &subpos) == SUCCESS) {
			  subtmp_val_p = *subtmp;
			  zval_copy_ctor(subtmp_val_p);
			  if(zend_hash_find(HASH_OF(subtmp_val_p),"start", sizeof("start"), (void**)&testObj) == SUCCESS &&
					 zend_hash_find(HASH_OF(subtmp_val_p),"end", sizeof("end"), (void**)&testObj) == SUCCESS ){
				  ALLOC_ZVAL(buildedObj);
				  object_init_ex(buildedObj, neo4jRelationship_ce);
				  NEO4J_METHOD2(neo4jRelationship, __construct, &temp, buildedObj, subtmp_val_p, getThis());
				  add_assoc_zval(row, keyList[i], buildedObj);
			  }else if(zend_hash_find(HASH_OF(subtmp_val_p),"all_relationships", sizeof("all_relationships"), (void**)&testObj) == SUCCESS){
				  ALLOC_ZVAL(buildedObj);
				  object_init_ex(buildedObj, neo4jNode_ce);
				  NEO4J_METHOD2(neo4jNode, __construct, &temp, buildedObj, subtmp_val_p, getThis());
				  add_assoc_zval(row, keyList[i], buildedObj);
			  }else if(zend_hash_find(HASH_OF(subtmp_val_p),"nodes", sizeof("nodes"), (void**)&testObj) == SUCCESS &&
				  zend_hash_find(HASH_OF(subtmp_val_p),"relationships", sizeof("relationships"), (void**)&testObj) == SUCCESS ){
				  ALLOC_ZVAL(buildedObj);
				  object_init_ex(buildedObj, neo4jPath_ce);
				  NEO4J_METHOD2(neo4jPath, __construct, &temp, buildedObj, subtmp_val_p, getThis());
				  add_assoc_zval(row, keyList[i], buildedObj);
			  }else{
				  add_assoc_zval(row, keyList[i], subtmp_val_p);
			  }
			  i++;
			  zend_hash_move_forward_ex(Z_ARRVAL_P(row), &subpos);
		  }
		  add_next_index_zval(result,row);
		  zend_hash_move_forward_ex(Z_ARRVAL_PP(data), &pos);
	  }
	  efree(keyList);
	  RETURN_ZVAL(result,1,0);
}

PHP_METHOD(neo4j,getPath){
	zval *start, *to, *option = NULL;
	zval *sendData;
	char *typeStr;
	int typeStrLen;
	long algorithm;
	zval method,path,temp;
	zval *response;
	char *relationshipsJson;
	zval **testVal;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzsl|z", &start, &to, &typeStr, &typeStrLen, &algorithm, &option) == FAILURE) {
		RETURN_NULL();
	}
	ALLOC_ZVAL(sendData);
	array_init(sendData);
	add_assoc_string(sendData, "to", neo4j_obj_to_url("node", to, NEO4J_THIS_PROPERTY(neo4j_ce,"_host")),0);
	relationshipsJson = (char*)emalloc(30+typeStrLen);
	sprintf(relationshipsJson,"{\"type\":\"%s\",\"direction\":\"out\"}",typeStr);
	add_assoc_zval(sendData, "relationships", neo4j_json_decode(relationshipsJson));
	efree(relationshipsJson);
	switch(algorithm){
	case NEO4J_PATH_SHORTESTPATH:
		add_assoc_string(sendData, "algorithm", "shortestPath",1);
		if(option)add_assoc_zval(sendData, "max_depth", option);
		break;
	case NEO4J_PATH_ALLSIMPLEPATHS:
		add_assoc_string(sendData, "algorithm", "allsimplepaths",1);
		break;
	case NEO4J_PATH_ALLPATHS:
		add_assoc_string(sendData, "algorithm", "allpaths",1);
		if(option)add_assoc_zval(sendData, "max_depth", option);
		break;
	case NEO4J_PATH_DIJKSTRA:
		add_assoc_string(sendData, "algorithm", "dijkstra",1);
		if(option)add_assoc_zval(sendData, "cost_property", option);
		break;
	default:
		add_assoc_string(sendData, "algorithm", "shortestPath",1);
		if(option){
			add_assoc_zval(sendData, "max_depth", option);
		}
		break;
	}

	ZVAL_STRING(&path,neo4j_make_request("node",start,"path"),0);
	ZVAL_LONG(&method,REST_METHOD_POST);
	ALLOC_ZVAL(response);
	NEO4J_METHOD3(neo4j, restAPI, response, getThis(), &method, &path, sendData);

	if(zend_hash_find(HASH_OF(response),"exception",sizeof("exception"),(void**)&testVal)==SUCCESS){
		RETURN_NULL();
	}else{
		object_init_ex(return_value, neo4jPath_ce);
		NEO4J_METHOD2(neo4jPath, __construct, &temp, return_value, response, getThis());
	}
}

PHP_METHOD(neo4j,getPaths){
	zval *start, *to, *option = NULL;
	zval *sendData;
	char *typeStr;
	int typeStrLen;
	long algorithm;
	zval method,path,temp;
	zval *response;
	char *relationshipsJson;

	zval *returnList;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzsl|z", &start, &to, &typeStr, &typeStrLen, &algorithm, &option) == FAILURE) {
		RETURN_NULL();
	}
	ALLOC_ZVAL(sendData);
	array_init(sendData);
	add_assoc_string(sendData, "to", neo4j_obj_to_url("node", to, NEO4J_THIS_PROPERTY(neo4j_ce,"_host")),0);
	relationshipsJson = (char*)emalloc(30+typeStrLen);
	sprintf(relationshipsJson,"{\"type\":\"%s\",\"direction\":\"out\"}",typeStr);
	add_assoc_zval(sendData, "relationships", neo4j_json_decode(relationshipsJson));
	efree(relationshipsJson);
	switch(algorithm){
	case NEO4J_PATH_SHORTESTPATH:
		add_assoc_string(sendData, "algorithm", "shortestPath",1);
		if(option)add_assoc_zval(sendData, "max_depth", option);
		break;
	case NEO4J_PATH_ALLSIMPLEPATHS:
		add_assoc_string(sendData, "algorithm", "allsimplepaths",1);
		break;
	case NEO4J_PATH_ALLPATHS:
		add_assoc_string(sendData, "algorithm", "allpaths",1);
		if(option)add_assoc_zval(sendData, "max_depth", option);
		break;
	case NEO4J_PATH_DIJKSTRA:
		add_assoc_string(sendData, "algorithm", "dijkstra",1);
		if(option)add_assoc_zval(sendData, "cost_property", option);
		break;
	default:
		add_assoc_string(sendData, "algorithm", "shortestPath",1);
		if(option)add_assoc_zval(sendData, "max_depth", option);
	}

	ZVAL_STRING(&path,neo4j_make_request("node",start,"paths"),0);
	ZVAL_LONG(&method,REST_METHOD_POST);
	ALLOC_ZVAL(response);
	NEO4J_METHOD3(neo4j, restAPI, response, getThis(), &method, &path, sendData);


	returnList = neo4j_array_data_to_path(response,getThis());
	RETURN_ZVAL(returnList,1,0);
}

PHP_METHOD(neo4j,getRelationshipIndex){
	zval *key, *type, temp;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &key) == FAILURE) {
		RETURN_NULL();
	}
	ALLOC_ZVAL(type);
	ZVAL_LONG(type,NEO4J_INDEX_RELATIONSHIP);
	object_init_ex(return_value, neo4jIndex_ce);
	NEO4J_METHOD3(neo4jIndex, __construct, &temp, return_value, type, key, getThis());
}

PHP_METHOD(neo4j,getNodeIndex){
	zval *key, *type, temp;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &key) == FAILURE) {
		RETURN_NULL();
	}
	ALLOC_ZVAL(type);
	ZVAL_LONG(type,NEO4J_INDEX_NODE);
	object_init_ex(return_value, neo4jIndex_ce);
	NEO4J_METHOD3(neo4jIndex, __construct, &temp, return_value, type, key, getThis());
}

PHP_METHOD(neo4j,deleteRelationshipIndex){
	zval *key;
	zval index,temp;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &key) == FAILURE) {
		RETURN_NULL();
	}
	NEO4J_METHOD1(neo4j, getRelationshipIndex, &index, getThis(), key);
	NEO4J_METHOD(neo4jIndex, delete, &temp, &index);
	RETURN_NULL();
}

PHP_METHOD(neo4j,deleteNodeIndex){
	zval *key;
	zval index,temp;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &key) == FAILURE) {
		RETURN_NULL();
	}
	NEO4J_METHOD1(neo4j, getNodeIndex, &index, getThis(), key);
	NEO4J_METHOD(neo4jIndex, delete, &temp, &index);
	RETURN_NULL();
}

PHP_METHOD(neo4j,query){
	char *str;
	char *queryString;
	zval method,path,temp;
	zval *response;
	zval *rVal;
	int len;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &len) == FAILURE) {
		RETURN_NULL();
	}
	//index/auto/node/?query=
	queryString = emalloc(26+len);
	sprintf(queryString,"index/auto/node/?query=%s",str);
	ZVAL_STRING(&path,queryString,0);
	ZVAL_LONG(&method,REST_METHOD_GET);
	ALLOC_ZVAL(response);
	NEO4J_METHOD2(neo4j, restAPI, response, getThis(), &method, &path);
	rVal = neo4j_array_data_to_node(response,getThis());
	RETURN_ZVAL(rVal,1,0);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
