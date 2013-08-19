<?php
include_once("config.inc");
include_once("debug.inc");
include_once("myDB.class");
include_once("ingress.inc");

printTop();

$start = date("Y-m-d H:00:00", strtotime("-1 days"));
$end = date("Y-m-d H:00:00", time());
if ($_REQUEST["start"]!="") {
	$start = $_REQUEST["start"];
}
if ($_REQUEST["end"]!="") {
	$end = $_REQUEST["end"];
}
print("<form action=\"capturedportals.php\">\n");
print("Date/heure depart: <input name=\"start\" value=\"".$start."\"><br>\n");
print("Date/heure fin: <input name=\"end\" value=\"".$end."\"><br>\n");
print("<input type=submit><br>\n");
print("</form>\n");
$portals = lastCapturedPortals($start, $end);
print(count($portals)." capture(s).<br>\n");
print(drawMap($portals, $config["defaultzoom"], $config["centerlat"], $config["centerlng"], "portal"));

printBottom();

?>
