phpneo4j
========
<html>
	<head>
		<title>phpNeo4j</title>
		<style type="text/css">
			.index_list dt{margin-top:20px;font-size:18px;}
		</style>
		<style type="text/css">
			.method_description{border:1px solid #555;margin-top:20px;}
		</style>
	</head>
	<body>
		<h2>Index</h2>
		<dl class="index_list">
			<dt><a href="#neo4j">neo4j</a></dt>
			<dd><a href="#neo4j___construct">neo4j::__construct</a></dd>
			<dd><a href="#neo4j_createNode">neo4j::createNode</a></dd>
			<dd><a href="#neo4j_getNode">neo4j::getNode</a></dd>
			<dd><a href="#neo4j_deleteNode">neo4j::deleteNode</a></dd>
			<dd><a href="#neo4j_createRelationship">neo4j::createRelationship</a></dd>
			<dd><a href="#neo4j_getRelationship">neo4j::getRelationship</a></dd>
			<dd><a href="#neo4j_deleteRelationship">neo4j::deleteRelationship</a></dd>
			<dd><a href="#neo4j_getPath">neo4j::getPath</a></dd>
			<dd><a href="#neo4j_getPaths">neo4j::getPaths</a></dd>
			<dd><a href="#neo4j_getNodeIndex">neo4j::getNodeIndex</a></dd>
			<dd><a href="#neo4j_deleteNodeIndex">neo4j::deleteNodeIndex</a></dd>
			<dd><a href="#neo4j_getRelationshipIndex">neo4j::getRelationshipIndex</a></dd>
			<dd><a href="#neo4j_deleteRelationshipIndex">neo4j::deleteRelationshipIndex</a></dd>
			<dd><a href="#neo4j_cypher">neo4j::cypher</a></dd>
			<dd><a href="#neo4j_query">neo4j::query</a></dd>
			<dd><a href="#neo4j_restAPI">neo4j::restAPI</a></dd>
		</dl>
		<dl class="index_list">
			<dt><a href="#neo4jNode">neo4jNode</a></dt>
			<dd><a href="#neo4jNode_getId">neo4jNode::getId</a></dd>
			<dd><a href="#neo4jNode_getUri">neo4jNode::getUri</a></dd>
			<dd><a href="#neo4jNode_save">neo4jNode::save</a></dd>
			<dd><a href="#neo4jNode_delete">neo4jNode::delete</a></dd>
			<dd><a href="#neo4jNode_getProperty">neo4jNode::getProperty</a></dd>
			<dd><a href="#neo4jNode_getAllProperties">neo4jNode::getAllProperties</a></dd>
			<dd><a href="#neo4jNode_setProperty">neo4jNode::setProperty</a></dd>
			<dd><a href="#neo4jNode_setAllProperties">neo4jNode::setAllProperties</a></dd>
			<dd><a href="#neo4jNode_deleteProperty">neo4jNode::deleteProperty</a></dd>
			<dd><a href="#neo4jNode_getRelationships">neo4jNode::getRelationships</a></dd>
			<dd><a href="#neo4jNode_getPathTo">neo4jNode::getPathTo</a></dd>
			<dd><a href="#neo4jNode_getPathFrom">neo4jNode::getPathFrom</a></dd>
			<dd><a href="#neo4jNode_getPathsTo">neo4jNode::getPathsTo</a></dd>
			<dd><a href="#neo4jNode_getPathsFrom">neo4jNode::getPathsFrom</a></dd>
			<dd><a href="#neo4jNode_traverseNodes">neo4jNode::traverseNodes</a></dd>
			<dd><a href="#neo4jNode_traversePaths">neo4jNode::traversePaths</a></dd>
			<dd><a href="#neo4jNode_traverseRelationships">neo4jNode::traverseRelationships</a></dd>
		</dl>
		<dl class="index_list">
			<dt><a href="#neo4jRelationship">neo4jRelationship</a></dt>
			<dd><a href="#neo4jRelationship_getId">neo4jRelationship::getId</a></dd>
			<dd><a href="#neo4jRelationship_getUri">neo4jRelationship::getUri</a></dd>
			<dd><a href="#neo4jRelationship_save">neo4jRelationship::save</a></dd>
			<dd><a href="#neo4jRelationship_delete">neo4jRelationship::delete</a></dd>
			<dd><a href="#neo4jRelationship_getProperty">neo4jRelationship::getProperty</a></dd>
			<dd><a href="#neo4jRelationship_getAllProperties">neo4jRelationship::getAllProperties</a></dd>
			<dd><a href="#neo4jRelationship_setProperty">neo4jRelationship::setProperty</a></dd>
			<dd><a href="#neo4jRelationship_setAllProperties">neo4jRelationship::setAllProperties</a></dd>
			<dd><a href="#neo4jRelationship_deleteProperty">neo4jRelationship::deleteProperty</a></dd>
			<dd><a href="#neo4jRelationship_startNode">neo4jRelationship::startNode</a></dd>
			<dd><a href="#neo4jRelationship_endNode">neo4jRelationship::endNode</a></dd>
			<dd><a href="#neo4jRelationship_getType">neo4jRelationship::getType</a></dd>
		</dl>
		<dl class="index_list">
			<dt><a href="#neo4jIndex">neo4jIndex</a></dt>
			<dd><a href="#neo4jIndex_getId">neo4jIndex::getId</a></dd>
			<dd><a href="#neo4jIndex_get">neo4jIndex::get</a></dd>
			<dd><a href="#neo4jIndex_getOrCreate">neo4jIndex::getOrCreate</a></dd>
			<dd><a href="#neo4jIndex_add">neo4jIndex::add</a></dd>
			<dd><a href="#neo4jIndex_remove">neo4jIndex::remove</a></dd>
			<dd><a href="#neo4jIndex_delete">neo4jIndex::delete</a></dd>
		</dl>
		<dl class="index_list">
			<dt><a href="#neo4jPath">neo4jPath</a></dt>
			<dd><a href="#neo4jPath_startNode">neo4jPath::startNode</a></dd>
			<dd><a href="#neo4jPath_endNode">neo4jPath::endNode</a></dd>
			<dd><a href="#neo4jPath_getLength">neo4jPath::getLength</a></dd>
			<dd><a href="#neo4jPath_getNodes">neo4jPath::getNodes</a></dd>
			<dd><a href="#neo4jPath_getRelationships">neo4jPath::getRelationships</a></dd>
		</dl>
		<h3 id="neo4j">neo4j</h3>
		<dl>
			<dd><a href="#neo4j___construct">neo4j::__construct</a></dd>
			<dd><a href="#neo4j_createNode">neo4j::createNode</a></dd>
			<dd><a href="#neo4j_getNode">neo4j::getNode</a></dd>
			<dd><a href="#neo4j_deleteNode">neo4j::deleteNode</a></dd>
			<dd><a href="#neo4j_createRelationship">neo4j::createRelationship</a></dd>
			<dd><a href="#neo4j_getRelationship">neo4j::getRelationship</a></dd>
			<dd><a href="#neo4j_deleteRelationship">neo4j::deleteRelationship</a></dd>
			<dd><a href="#neo4j_getPath">neo4j::getPath</a></dd>
			<dd><a href="#neo4j_getPaths">neo4j::getPaths</a></dd>
			<dd><a href="#neo4j_getNodeIndex">neo4j::getNodeIndex</a></dd>
			<dd><a href="#neo4j_deleteNodeIndex">neo4j::deleteNodeIndex</a></dd>
			<dd><a href="#neo4j_getRelationshipIndex">neo4j::getRelationshipIndex</a></dd>
			<dd><a href="#neo4j_deleteRelationshipIndex">neo4j::deleteRelationshipIndex</a></dd>
			<dd><a href="#neo4j_cypher">neo4j::cypher</a></dd>
			<dd><a href="#neo4j_query">neo4j::query</a></dd>
			<dd><a href="#neo4j_restAPI">neo4j::restAPI</a></dd>
		</dl>
		<div class="method_description" id="neo4j___construct">
			<h4>public neo4j::__construct([string $host="localhost" [,int $port=7474 [,string $userpsw=""]]])</h4>
			<p>Create a new neo4j connection object</p>
			<dl>
				<dt>userpsw</dt>
				<dd>username:password</dd>
			</dl>
		</div>
		<div class="method_description" id="neo4j_createNode">
			<h4>public neo4jNode neo4j::createNode([array $properties=array()])</h4>
			<p>Create a neo4jNode</p>
		</div>
		<div class="method_description" id="neo4j_getNode">
			<h4>public neo4jNode neo4j::getNode(string $id)</h4>
			<p>Get a neo4jNode by nodeId</p>
		</div>
		<div class="method_description" id="neo4j_deleteNode">
			<h4>public neo4j::deleteNode(string $id)</h4>
			<p>Delete a neo4jNode by nodeId</p>
		</div>
		<div class="method_description" id="neo4j_createRelationship">
			<h4>public neo4jRelationship neo4j::createRelationship(mixed $start, mixed $to [,string $type="" [,array $properties=array()]])</h4>
			<p>Create a neo4jRelationship</p>
			<p>$start and $to can be nodeId or neo4jNode object.</p>
		</div>
		<div class="method_description" id="neo4j_getRelationship">
			<h4>public neo4jRelationship neo4j::getRelationship(string $id)</h4>
			<p>Get a neo4jRelationship by relationshipId</p>
		</div>
		<div class="method_description" id="neo4j_deleteRelationship">
			<h4>public neo4j::deleteRelationship(string $id)</h4>
			<p>Delete a neo4jRelationship by relationshipId</p>
		</div>
		<div class="method_description" id="neo4j_getPath">
			<h4>public neo4jPath neo4j::getPath(mixed $start, mixed $to, string $type, int $algorithm [, array $option])</h4>
			<p>Get a neo4jPath by conditions</p>
			<dl>
				<dt>$algorithm</dt>
				<dd>NEO4J_PATH_SHORTESTPATH</dd>
				<dd>NEO4J_PATH_ALLSIMPLEPATHS</dd>
				<dd>NEO4J_PATH_ALLPATHS</dd>
				<dd>NEO4J_PATH_DIJKSTRA</dd>
			</dl>
			<dl>
				<dt>$option - An array of options to applay</dt>
				<dd>max_depth</dd>
				<dd>cost_property - only use for NEO4J_PATH_DIJKSTRA</dd>
			</dl>
		</div>
		<div class="method_description" id="neo4j_getPaths">
			<h4>public neo4jPath neo4j::getPaths(mixed $start, mixed $to, string $type, int $algorithm [, array $option])</h4>
			<p>Get all neo4jPathes by conditions</p>
			<dl>
				<dt>$algorithm</dt>
				<dd>NEO4J_PATH_SHORTESTPATH</dd>
				<dd>NEO4J_PATH_ALLSIMPLEPATHS</dd>
				<dd>NEO4J_PATH_ALLPATHS</dd>
				<dd>NEO4J_PATH_DIJKSTRA</dd>
			</dl>
			<dl>
				<dt>$option - An array of options to applay</dt>
				<dd>max_depth</dd>
				<dd>cost_property - only use for NEO4J_PATH_DIJKSTRA</dd>
			</dl>
		</div>
		<div class="method_description" id="neo4j_getNodeIndex">
			<h4>public neo4jIndex neo4j::getNodeIndex(string $id)</h4>
			<p>Get a neo4jIndex of neo4jNode by indexId</p>
		</div>
		<div class="method_description" id="neo4j_deleteNodeIndex">
			<h4>public neo4j::deleteNodeIndex(string $id)</h4>
			<p>Delete a neo4jIndex of neo4jNode by indexId</p>
		</div>
		<div class="method_description" id="neo4j_getRelationshipIndex">
			<h4>public neo4jIndex neo4j::getRelationshipIndex(string $id)</h4>
			<p>Get a neo4jIndex of neo4jRelationship by indexId</p>
		</div>
		<div class="method_description" id="neo4j_deleteRelationshipIndex">
			<h4>public neo4j::deleteRelationshipIndex(string $id)</h4>
			<p>Delete a neo4jIndex of neo4jRelationship by indexId</p>
		</div>
		<div class="method_description" id="neo4j_cypher">
			<h4>public array neo4j::cypher(string $query [, array $params = array()])</h4>
			<p>Query with cypher</p>
		</div>
		<div class="method_description" id="neo4j_query">
			<h4>public array neo4j::query(string $query)</h4>
			<p>Find node by query from an automatic index</p>
		</div>
		<div class="method_description" id="neo4j_restAPI">
			<h4>public array neo4j::restAPI(int $method, string $path [,array $data])</h4>
			<p>Send a request by restAPI</p>
			<dl>
				<dt>$method</dt>
				<dd>REST_METHOD_GET</dd>
				<dd>REST_METHOD_POST</dd>
				<dd>REST_METHOD_DELETE</dd>
				<dd>REST_METHOD_PUT</dd>
			</dl>
		</div>
		<h3 id="neo4jNode">neo4jNode</h3>
		<dl>
			<dd><a href="#neo4jNode_getId">neo4jNode::getId</a></dd>
			<dd><a href="#neo4jNode_getUri">neo4jNode::getUri</a></dd>
			<dd><a href="#neo4jNode_save">neo4jNode::save</a></dd>
			<dd><a href="#neo4jNode_delete">neo4jNode::delete</a></dd>
			<dd><a href="#neo4jNode_getProperty">neo4jNode::getProperty</a></dd>
			<dd><a href="#neo4jNode_getAllProperties">neo4jNode::getAllProperties</a></dd>
			<dd><a href="#neo4jNode_setProperty">neo4jNode::setProperty</a></dd>
			<dd><a href="#neo4jNode_setAllProperties">neo4jNode::setAllProperties</a></dd>
			<dd><a href="#neo4jNode_deleteProperty">neo4jNode::deleteProperty</a></dd>
			<dd><a href="#neo4jNode_getRelationships">neo4jNode::getRelationships</a></dd>
			<dd><a href="#neo4jNode_getPathTo">neo4jNode::getPathTo</a></dd>
			<dd><a href="#neo4jNode_getPathFrom">neo4jNode::getPathFrom</a></dd>
			<dd><a href="#neo4jNode_getPathsTo">neo4jNode::getPathsTo</a></dd>
			<dd><a href="#neo4jNode_getPathsFrom">neo4jNode::getPathsFrom</a></dd>
			<dd><a href="#neo4jNode_traverseNodes">neo4jNode::traverseNodes</a></dd>
			<dd><a href="#neo4jNode_traversePaths">neo4jNode::traversePaths</a></dd>
			<dd><a href="#neo4jNode_traverseRelationships">neo4jNode::traverseRelationships</a></dd>
		</dl>
		<div class="method_description" id="neo4jNode_getId">
			<h4>public string neo4jNode::getId()</h4>
			<p>Get nodeId</p>
		</div>
		<div class="method_description" id="neo4jNode_getUri">
			<h4>public string neo4jNode::getUri()</h4>
			<p>Get uri</p>
		</div>
		<div class="method_description" id="neo4jNode_save">
			<h4>public neo4jNode neo4jNode::save()</h4>
			<p>Save self</p>
		</div>
		<div class="method_description" id="neo4jNode_delete">
			<h4>public neo4jNode::delete()</h4>
			<p>Delete self</p>
		</div>
		<div class="method_description" id="neo4jNode_getProperty">
			<h4>public mix neo4jNode::getProperty(string $key)</h4>
			<p>Get a property by key</p>
		</div>
		<div class="method_description" id="neo4jNode_getAllProperties">
			<h4>public array neo4jNode::getAllProperties()</h4>
			<p>Get all properties</p>
		</div>
		<div class="method_description" id="neo4jNode_setProperty">
			<h4>public neo4jNode neo4jNode::setProperty(string $key,mix $value)</h4>
			<p>Set a property</p>
		</div>
		<div class="method_description" id="neo4jNode_setAllProperties">
			<h4>public neo4jNode neo4jNode::setAllProperties(array $properties)</h4>
			<p>Set all properties</p>
		</div>
		<div class="method_description" id="neo4jNode_deleteProperty">
			<h4>public neo4jNode neo4jNode::deleteProperty(string $key)</h4>
			<p>Delete a property</p>
		</div>
		<div class="method_description" id="neo4jNode_getRelationships">
			<h4>public array neo4jNode::getRelationships([int $direct = NEO4J_RELATIONSHIP_ALL [, array $typeList = array()]])</h4>
			<p>Get all relationships</p>
			<dl>
				<dt>$direct</dt>
				<dd>NEO4J_RELATIONSHIP_IN</dd>
				<dd>NEO4J_RELATIONSHIP_OUT</dd>
				<dd>NEO4J_RELATIONSHIP_ALL</dd>
			</dl>
		</div>
		<div class="method_description" id="neo4jNode_getPathTo">
			<h4>public neo4jPath neo4jNode::getPathTo(mixed $to, string $type, int $algorithm [, array $option])</h4>
			<p>Get a neo4jPath by conditions</p>
			<dl>
				<dt>$algorithm</dt>
				<dd>NEO4J_PATH_SHORTESTPATH</dd>
				<dd>NEO4J_PATH_ALLSIMPLEPATHS</dd>
				<dd>NEO4J_PATH_ALLPATHS</dd>
				<dd>NEO4J_PATH_DIJKSTRA</dd>
			</dl>
			<dl>
				<dt>$option - An array of options to applay</dt>
				<dd>max_depth</dd>
				<dd>cost_property - only use for NEO4J_PATH_DIJKSTRA</dd>
			</dl>
		</div>
		<div class="method_description" id="neo4jNode_getPathTo">
			<h4>public neo4jPath neo4jNode::getPathFrom(mixed $start, string $type, int $algorithm [, array $option])</h4>
			<p>Get a neo4jPath by conditions</p>
			<dl>
				<dt>$algorithm</dt>
				<dd>NEO4J_PATH_SHORTESTPATH</dd>
				<dd>NEO4J_PATH_ALLSIMPLEPATHS</dd>
				<dd>NEO4J_PATH_ALLPATHS</dd>
				<dd>NEO4J_PATH_DIJKSTRA</dd>
			</dl>
			<dl>
				<dt>$option - An array of options to applay</dt>
				<dd>max_depth</dd>
				<dd>cost_property - only use for NEO4J_PATH_DIJKSTRA</dd>
			</dl>
		</div>
		<div class="method_description" id="neo4jNode_getPathsTo">
			<h4>public neo4jPath neo4jNode::getPathsTo(mixed $to, string $type, int $algorithm [, array $option])</h4>
			<p>Get all neo4jPathes by conditions</p>
			<dl>
				<dt>$algorithm</dt>
				<dd>NEO4J_PATH_SHORTESTPATH</dd>
				<dd>NEO4J_PATH_ALLSIMPLEPATHS</dd>
				<dd>NEO4J_PATH_ALLPATHS</dd>
				<dd>NEO4J_PATH_DIJKSTRA</dd>
			</dl>
			<dl>
				<dt>$option - An array of options to applay</dt>
				<dd>max_depth</dd>
				<dd>cost_property - only use for NEO4J_PATH_DIJKSTRA</dd>
			</dl>
		</div>
		<div class="method_description" id="neo4jNode_getPathsFrom">
			<h4>public neo4jPath neo4jNode::getPathsFrom(mixed $start, string $type, int $algorithm [, array $option])</h4>
			<p>Get all neo4jPathes by conditions</p>
			<dl>
				<dt>$algorithm</dt>
				<dd>NEO4J_PATH_SHORTESTPATH</dd>
				<dd>NEO4J_PATH_ALLSIMPLEPATHS</dd>
				<dd>NEO4J_PATH_ALLPATHS</dd>
				<dd>NEO4J_PATH_DIJKSTRA</dd>
			</dl>
			<dl>
				<dt>$option - An array of options to applay</dt>
				<dd>max_depth</dd>
				<dd>cost_property - only use for NEO4J_PATH_DIJKSTRA</dd>
			</dl>
		</div>
		<div class="method_description" id="neo4jNode_traverseNodes">
			<h4>public array neo4jNode::traverseNodes(array $option)</h4>
			<p>Traverse nodes</p>
			<dl>
				<dt>$option</dt>
				<dd>order - [ breadth_first | depth_first ]</dd>
				<dd>relationships - [ all | in | out ]</dd>
				<dd>uniqueness - [ node_global | none | relationship_global | node_path | relationship_path ]</dd>
				<dd>return_filter - [ all | all_but_start_node ]</dd>
				<dd>prune_evaluator</dd>
				<dd>max_depth</dd>
			</dl>
		</div>
		<div class="method_description" id="neo4jNode_traversePaths">
			<h4>public array neo4jNode::traversePaths(array $option)</h4>
			<p>Traverse paths</p>
			<dl>
				<dt>$option</dt>
				<dd>order - [ breadth_first | depth_first ]</dd>
				<dd>relationships - [ all | in | out ]</dd>
				<dd>uniqueness - [ node_global | none | relationship_global | node_path | relationship_path ]</dd>
				<dd>return_filter - [ all | all_but_start_node ]</dd>
				<dd>prune_evaluator</dd>
				<dd>max_depth</dd>
			</dl>
		</div>
		<div class="method_description" id="neo4jNode_traverseRelationships">
			<h4>public array neo4jNode::traverseRelationships(array $option)</h4>
			<p>Traverse relationships</p>
			<dl>
				<dt>$option</dt>
				<dd>order - [ breadth_first | depth_first ]</dd>
				<dd>relationships - [ all | in | out ]</dd>
				<dd>uniqueness - [ node_global | none | relationship_global | node_path | relationship_path ]</dd>
				<dd>return_filter - [ all | all_but_start_node ]</dd>
				<dd>prune_evaluator</dd>
				<dd>max_depth</dd>
			</dl>
		</div>
		<h3 id="neo4jRelationship">neo4jRelationship</h3>
		<dl>
			<dd><a href="#neo4jRelationship_getId">neo4jRelationship::getId</a></dd>
			<dd><a href="#neo4jRelationship_getUri">neo4jRelationship::getUri</a></dd>
			<dd><a href="#neo4jRelationship_save">neo4jRelationship::save</a></dd>
			<dd><a href="#neo4jRelationship_delete">neo4jRelationship::delete</a></dd>
			<dd><a href="#neo4jRelationship_getProperty">neo4jRelationship::getProperty</a></dd>
			<dd><a href="#neo4jRelationship_getAllProperties">neo4jRelationship::getAllProperties</a></dd>
			<dd><a href="#neo4jRelationship_setProperty">neo4jRelationship::setProperty</a></dd>
			<dd><a href="#neo4jRelationship_setAllProperties">neo4jRelationship::setAllProperties</a></dd>
			<dd><a href="#neo4jRelationship_deleteProperty">neo4jRelationship::deleteProperty</a></dd>
			<dd><a href="#neo4jRelationship_startNode">neo4jRelationship::startNode</a></dd>
			<dd><a href="#neo4jRelationship_endNode">neo4jRelationship::endNode</a></dd>
			<dd><a href="#neo4jRelationship_getType">neo4jRelationship::getType</a></dd>
		</dl>
		<div class="method_description" id="neo4jRelationship_getId">
			<h4>public string neo4jRelationship::getId()</h4>
			<p>Get relationshipId</p>
		</div>
		<div class="method_description" id="neo4jRelationship_getUri">
			<h4>public string neo4jRelationship::getUri()</h4>
			<p>Get uri</p>
		</div>
		<div class="method_description" id="neo4jRelationship_save">
			<h4>public neo4jRelationship neo4jRelationship::save()</h4>
			<p>Save self</p>
		</div>
		<div class="method_description" id="neo4jRelationship_delete">
			<h4>public neo4jRelationship::delete()</h4>
			<p>Delete self</p>
		</div>
		<div class="method_description" id="neo4jRelationship_getProperty">
			<h4>public mix neo4jRelationship::getProperty(string $key)</h4>
			<p>Get a property by key</p>
		</div>
		<div class="method_description" id="neo4jRelationship_getAllProperties">
			<h4>public array neo4jRelationship::getAllProperties()</h4>
			<p>Get all properties</p>
		</div>
		<div class="method_description" id="neo4jRelationship_setProperty">
			<h4>public neo4jRelationship neo4jRelationship::setProperty(string $key,mix $value)</h4>
			<p>Set a property</p>
		</div>
		<div class="method_description" id="neo4jRelationship_setAllProperties">
			<h4>public neo4jRelationship neo4jRelationship::setAllProperties(array $properties)</h4>
			<p>Set all properties</p>
		</div>
		<div class="method_description" id="neo4jRelationship_deleteProperty">
			<h4>public neo4jRelationship neo4jRelationship::deleteProperty(string $key)</h4>
			<p>Delete a property</p>
		</div>
		<div class="method_description" id="neo4jRelationship_startNode">
			<h4>public neo4jNode neo4jRelationship::startNode()</h4>
			<p>Get start node</p>
		</div>
		<div class="method_description" id="neo4jRelationship_endNode">
			<h4>public neo4jNode neo4jRelationship::endNode()</h4>
			<p>Get end node</p>
		</div>
		<div class="method_description" id="neo4jRelationship_getType">
			<h4>public string neo4jRelationship::getType()</h4>
			<p>Get type</p>
		</div>
		<h3 id="neo4jIndex">neo4jIndex</h3>
		<dl>
			<dd><a href="#neo4jIndex_getId">neo4jIndex::getId</a></dd>
			<dd><a href="#neo4jIndex_get">neo4jIndex::get</a></dd>
			<dd><a href="#neo4jIndex_getOrCreate">neo4jIndex::getOrCreate</a></dd>
			<dd><a href="#neo4jIndex_add">neo4jIndex::add</a></dd>
			<dd><a href="#neo4jIndex_remove">neo4jIndex::remove</a></dd>
			<dd><a href="#neo4jIndex_delete">neo4jIndex::delete</a></dd>
		</dl>
		<div class="method_description" id="neo4jIndex_getId">
			<h4>public string neo4jIndex::getId()</h4>
			<p>Get indexId</p>
		</div>
		<div class="method_description" id="neo4jIndex_get">
			<h4>public mix neo4jIndex::get(string $key, string $value)</h4>
			<p>Get objects by $key and $value</p>
		</div>
		<div class="method_description" id="neo4jIndex_getOrCreate">
			<h4>public mix neo4jIndex::getOrCreate(mixed $obj, string $key, string $value)</h4>
			<p>Get or create an object by $key and $value</p>
		</div>
		<div class="method_description" id="neo4jIndex_add">
			<h4>public neo4jIndex neo4jIndex::add(mixed $obj, string $key, string $value)</h4>
			<p>Add an object to the index</p>
		</div>
		<div class="method_description" id="neo4jIndex_remove">
			<h4>public neo4jIndex neo4jIndex::remove(mixed $obj, string $key, string $value)</h4>
			<p>Remove an object from the index</p>
		</div>
		<div class="method_description" id="neo4jIndex_delete">
			<h4>public neo4jIndex::delete()</h4>
			<p>Delete self</p>
		</div>
		<h3 id="neo4jPath">neo4jPath</h3>
		<dl>
			<dd><a href="#neo4jPath_startNode">neo4jPath::startNode</a></dd>
			<dd><a href="#neo4jPath_endNode">neo4jPath::endNode</a></dd>
			<dd><a href="#neo4jPath_getLength">neo4jPath::getLength</a></dd>
			<dd><a href="#neo4jPath_getNodes">neo4jPath::getNodes</a></dd>
			<dd><a href="#neo4jPath_getRelationships">neo4jPath::getRelationships</a></dd>
		</dl>
		<div class="method_description" id="neo4jPath_startNode">
			<h4>public neo4jNode neo4jPath::startNode()</h4>
			<p>Get start node</p>
		</div>
		<div class="method_description" id="neo4jPath_endNode">
			<h4>public neo4jNode neo4jPath::endNode()</h4>
			<p>Get end node</p>
		</div>
		<div class="method_description" id="neo4jPath_getLength">
			<h4>public int neo4jPath::getLength()</h4>
			<p>Get length</p>
		</div>
		<div class="method_description" id="neo4jPath_getNodes">
			<h4>public array neo4jPath::getNodes()</h4>
			<p>Get all nodes</p>
		</div>
		<div class="method_description" id="neo4jPath_getRelationships">
			<h4>public array neo4jPath::getRelationships()</h4>
			<p>Get all relationships</p>
		</div>
	</body>
</html>
