<?php
include_once("config.inc");
include_once("debug.inc");
include_once("myDB.class");
include_once("ingress.inc");
include_once("email.inc");                   

$players = getAllPlayers();
$guids = Array();
foreach($players as $player) {
	if ($player["guid"]==$player["name"]) {
		$guids[] = $player["guid"];
	}
	if (count($guids)>5) {
		$name = Array();
		$names = getPlayersbyGuids($guids);
		print_r($names);
		
		foreach($names["result"] as $result) {
			updatePlayer($result["guid"], $result["nickname"]);
		}
		$guids = Array();
	}
}
if (count($guids)>0) {
	$name = Array();
	$names = getPlayersbyGuids($guids);
	print_r($names);
	
	foreach($names["result"] as $result) {
		updatePlayer($result["guid"], $result["nickname"]);
	}
	$guids = Array();
}

?>

