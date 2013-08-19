<?php
include_once("config.inc");
include_once("core.inc");
include_once("debug.inc");
include_once("myDB.class");
include_once("ingress.inc");

printTop();

$start = date("Y-m-d H:i:s", strtotime("-1 day"));
if (isset($_REQUEST["start"])) {
	$start = $_REQUEST["start"];
}
$end = "";
if (isset($_REQUEST["end"])) {
	$end = $_REQUEST["end"];
}
$howfarback = 50;
if (isset($_REQUEST["howfarback"])) {
	$howfarback = $_REQUEST["howfarback"];
}
$level = -1;
if (isset($_REQUEST["level"])) {
	$level = $_REQUEST["level"];
}

if($_REQUEST["type"]=="player") { $config["debugdisplay"] = false;
	$data = getPlayerStats($playerid, "", $start, $end);
	$lastActions = lastActions("", $playerid, $howfarback);
	$portals = getPortalsForPlayer($playerid);
} else {
	$resistanceData = getPlayerStats("", "resistance", $start, $end);
	$aliensData = getPlayerStats("", "aliens", $start, $end);
}

$header = "<tr><td>Nom</td><td>Niveau</td><td>Portails cap</td><td>Resonators dep</td><td>Resonators det</td><td>Links crees</td><td>Links det</td></tr>\n";

print("Statistiques joueurs r&eacute;centes (commencant ".$start."):<br>\n");

if($_REQUEST["type"]=="player") {
	$output = "";
	foreach($data as $player) {
		$output .= "<tr bgcolor=\"".$config[strtolower(stripslashes($player["team"]))."colour"]."\"><td>".stripslashes($player["name"])."</td><td>".stripslashes($player["level"])."</td><td>".stripslashes($player["portalscaptured"])."</td><td>".stripslashes($player["resonatorsdeployed"])."</td><td>".stripslashes($player["resonatorsdestroyed"])."</td><td>".stripslashes($player["linkscreated"])."</td><td>".stripslashes($player["linksdestroyed"])."</td></tr>";
	}
	print("<table border=\"0\">\n");
	print("<tr ><td valign=\"top\"  width=\"60%\">\n");
		print("<table border=\"1\">\n");
		print($header);
		print($output);
		print("</table>\n");
		
		$output = "";
		foreach($portals as $portal) {
			$output .= "<tr><td><a href=\"portaldetail.php?guid=".stripslashes($portal["guid"])."\">".stripslashes($portal["plain"])."</a></td><td>".stripslashes($portal["level"])."</td></tr>\n";
		}
		print("<br><br><table border=\"1\">\n");
		print("<tr><td>Portails</td><td>Niveau</td></tr>\n");
		print($output);
		print("</table>\n");
	print("</td><td valign=\"top\">\n");
		print("<table border=\"0\">\n");
		foreach($lastActions as $action) {
			print(printAction($action));
		}
		print("</table>\n");
	print("</td></tr>\n</table>\n");
} else {
	print("<table border=\"0\">\n");
	$output = "";
	$i=0;
	foreach($resistanceData as $player) {
		if ($player["level"]>=$level) {
			$output .= "<tr bgcolor=\"".$config["resistancecolour"]."\"><td><a href=\"playerstats.php?type=player&playerid=".stripslashes($player["guid"])."\">".stripslashes($player["name"])."</a></td><td>".stripslashes($player["level"])."</td><td>".stripslashes($player["portalscaptured"])."</td><td>".stripslashes($player["resonatorsdeployed"])."</td><td>".stripslashes($player["resonatorsdestroyed"])."</td><td>".stripslashes($player["linkscreated"])."</td><td>".stripslashes($player["linksdestroyed"])."</td></tr>";
			$i++;
			if ($i % 25 == 0) {
				$output .= $header;
			}
		}
	}
	print("<tr><td valign=\"top\">");
		print("<table border=\"1\">\n");
		print($header);
		print($output);
		print("</table>\n");
	print("</td><td valign=\"top\">");
		
	$output = "";                   
	$i=0;
	foreach($aliensData as $player) {
		if ($player["level"]>=$level) {
			$output .= "<tr bgcolor=\"".$config["alienscolour"]."\"><td><a href=\"playerstats.php?type=player&playerid=".stripslashes($player["guid"])."\">".stripslashes($player["name"])."</a></td><td>".stripslashes($player["level"])."</td><td>".stripslashes($player["portalscaptured"])."</td><td>".stripslashes($player["resonatorsdeployed"])."</td><td>".stripslashes($player["resonatorsdestroyed"])."</td><td>".stripslashes($player["linkscreated"])."</td><td>".stripslashes($player["linksdestroyed"])."</td></tr>";
			$i++;
			if ($i % 25 == 0) {
				$output .= $header;
			}
		}
	}
		print("<table border=\"1\">\n");
		print($header);
		print($output);
		print("</table>\n");
	print("</td></tr></table>\n");
}

printBottom();

?>
