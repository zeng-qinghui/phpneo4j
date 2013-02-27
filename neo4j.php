<?php

$module = 'neo4j';
if (extension_loaded($module)) {
	echo "Module $module is compiled into PHP\n";
	//$con = new neo4j('localhost','7474','root:root');
	$con = new neo4j('localhost','7474','root:blue@shopinterest');
	$con->createNode(array('name'=>'n1','type'=>'a'));
	$con->createNode(array('name'=>'n2','type'=>'b'));
	$con->createNode(array('name'=>'n3','type'=>'a'));
	$data = $con->query('type:a');
	var_dump($data);
}
/*
	$neo4j = new neo4j('localhost','7474','root:blue@shopinterest');

	echo "GetNode ",getNode($neo4j),"ms<br>";
	echo "getRelationship ",getRelationship($neo4j),"ms<br>";

	function getNode($neo4j){
		$time = 10000;
		$start = microtime(true);
		while($time--){
			$data = $neo4j->getNode(0)->getAllProperties();
		}
		return microtime(true)-$start;
	}

	function getRelationship($neo4j){
		$time = 10000;
		$start = microtime(true);
		while($time--){
			$neo4j->getRelationship(0);
		}
		return microtime(true)-$start;
	}
*/
