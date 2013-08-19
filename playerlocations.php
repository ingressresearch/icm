<?php
include_once("config.inc");
include_once("core.inc");
include_once("debug.inc");
include_once("myDB.class");
include_once("ingress.inc");

printTop();

$team="";
$player="";
$start="";
$end="";
$historical = "";
if ($_REQUEST["team"]!="") {
	$team = strtolower($_REQUEST["team"]); 
}
if ($_REQUEST["player"]!="") {
	$player = $_REQUEST["player"]; 
}
if ($_REQUEST["start"]!="") {
	$start = $_REQUEST["start"]; 
}
if ($_REQUEST["end"]!="") {
	$end = $_REQUEST["end"]; 
}
if ($_REQUEST["historical"]!="") {
	$historical = $_REQUEST["historical"]; 
}

print("<form action=\"playerlocations.php\">\n");
print("<table border=\"0\">\n");
print("<tr><td valign=\"top\">\n");
	print("Equipe: <select name=\"team\"><option value=\"\">Toutes</option>\n<option ".($team=="resistance"?"SELECTED":"")." value=\"resistance\">Resistance</option>\n<option ".($team=="aliens"?"SELECTED":"")." 	value=\"aliens\">Illumines</option></select><br>\n");
	$players = getAllPlayers();
	print("Joueur: <select name=\"player\">");
	print("<option ".($player==""?"SELECTED":"")." value=\"\">Tous</option>\n");
	foreach($players as $playerdata) {
		if (trim($playerdata["name"])!="") {
			print("<option ".($player==stripslashes($playerdata["guid"])?"SELECTED":"")." value=\"".stripslashes($playerdata["guid"])."\">".stripslashes($playerdata["name"])."</option>\n");
		}
	}
	print("</select><br>\n");
print("</td><td valign=\"top\">\n");
	print("Date/heure depart:");
	print("<input name=\"start\" value=\"".$start."\"> (defaut=derniere position sur les 45 dernieres minutes)<br>\n");
	print("Date/heure fin:");
	print("<input name=\"end\" value=\"".$end."\"><br>\n");
print("</td><td valign=\"top\">\n");
	print("<input type=checkbox name=\"historical\" ".($historical!=""?"CHECKED":"")." value=\"1\">Inclure toutes activit&eacute;s\n");
print("</table>\n");
print("<input type=submit>\n");
print("</form>\n");

$data = lastPlayersLocations($team, $player, $start, $end, $historical);
//$data = array_merge($data, getAllPortals());
print(drawMap($data, $config["defaultzoom"], $config["centerlat"], $config["centerlng"], "logo"));
	
printBottom();

?>
