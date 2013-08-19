<?php
include_once("config.inc");
include_once("debug.inc");
include_once("myDB.class");
include_once("ingress.inc");
$config["debugdisplay"] = true;
$portals = findPortalsOutside($config["minlatE6"], $config["maxlatE6"], $config["minlngE6"], $config["maxlngE6"]);
//print_r($portals);
$ids = Array();
foreach($portals as $portal) {
	$ids[] = stripslashes($portal["guid"]);
}
deletePortals($ids);

?>
