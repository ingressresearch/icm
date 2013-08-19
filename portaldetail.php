<?php
include_once("config.inc");
include_once("core.inc");
include_once("debug.inc");
include_once("myDB.class");
include_once("ingress.inc");

printTop();

if (isset($_REQUEST["guid"])) { 
	$portal = getPortal($_REQUEST["guid"]);
	$howfarback = 50;
	if (isset($_REQUEST["howfarback"])) {
		$howfarback = $_REQUEST["howfarback"];
	}
	$lastActions = lastActions("", "", $howfarback, $_REQUEST["guid"]);
	print("<table border=\"0\">\n");
	print("<tr ><td valign=\"top\"  width=\"60%\">\n");
		print("Details portail:<br>\n");
		print("<table border=\"0\"><tr><td>\n");
			print("<table>\n");
			print("<tr><td>Nom:</td><td>".stripslashes($portal["name"])."</td></tr>\n");
			print("<tr><td>Adresse:</td><td>".stripslashes($portal["address"])."</td></tr>\n");
			print("<tr><td>Equipe:</td><td>".stripslashes($portal["team"])."</td></tr>\n");
			$player = findPlayer(stripslashes($portal["playerid"]));
			print("<tr><td>Proprietaire:</td><td><a href=\"playerstats.php?type=player&playerid=".stripslashes($player["guid"])."\">".stripslashes($player["name"])."</a></td></tr>\n");
			print("<tr><td>Capture:</td><td>".stripslashes($portal["capturedtime"])."</td></tr>\n");
			print("<tr><td>Latitude:</td><td>".stripslashes($portal["latE6"])."</td></tr>\n");
			print("<tr><td>Longitude:</td><td>".stripslashes($portal["lngE6"])."</td></tr>\n");
			print("<tr><td><a href=\"http://www.ingress.com/intel?latE6=".stripslashes($portal["latE6"])."&lngE6=".stripslashes($portal["lngE6"])."&z=16\">Carte</a></td></tr>\n");
			$level = 0;
			foreach($portal["resonators"] as $resonator) {
				$level += stripslashes($resonator["level"]);
			}
			//print("<tr><td>Niveau:</td><td>".($level==0?"<=3":($level/8))."</td></tr>\n");
			print("<tr><td>Niveau:</td><td>".stripslashes($portal["level"])."</td></tr>\n");
			print("<tr><td>Derni&egrave;re mise &agrave; jour:</td><td>".stripslashes($portal["lastupdate"])."</td></tr>\n");
			print("</table>\n");
		print("</td><td>\n");
			if (trim($portal["imageurl"])!="") {
				print("<img width=\"200\" src=\"".stripslashes($portal["imageurl"])."\">\n");
			}
		print("</td></tr></table>\n");
	
		print("<br>\nResonators:<br>\n");
		print("<table border=\"1\">\n");
		print("<tr><td>Slot</td>");
		print("<td>Niveau</td>");
		print("<td>Energie</td>");
		print("<td>Distance</td>");
		print("<td>Propri&eacute;taire</td></tr>");
		foreach($portal["resonators"] as $resonator) {
			print("<tr><td>".stripslashes($resonator["slot"])." (".portalGeoMapping(stripslashes($resonator["slot"])).")</td>");
			print("<td>".stripslashes($resonator["level"])."</td>");
			print("<td>".stripslashes($resonator["evergytotal"])." (".(stripslashes($resonator["evergytotal"])/$resonatorEnergy[stripslashes($resonator["level"])]*100)."%)</td>");
			print("<td>".stripslashes($resonator["distancetoportal"])."</td>");
			$player = getPlayer(stripslashes($resonator["playerid"]));
			print("<td><a href=\"playerstats.php?type=player&playerid=".stripslashes($player["guid"])."\">".stripslashes($player["name"])."</a></td></tr>");
		}
		print("</table>");
		
		print("<br>Mods:<br>\n");
		print("<table border=\"1\">\n");
		print("<tr><td>Mod</td>");
		print("<td>Rarit&eacute;</td>");
		print("<td>Stats</td></tr>");
		foreach($portal["mods"] as $mod) {
			print("<tr><td>".stripslashes($mod["display"])."</td>");
			print("<td>".stripslashes($mod["rarity"])."</td>");
			print("<td>".stripslashes($mod["stats"])."</td><tr>");
		}
		print("</table>");

		print("<br>Links:<br>\n");
		print("<table border=\"1\">\n");
		foreach($portal["edges"] as $edge) {
			$direction = "";
			if (stripslashes($portal["guid"])==stripslashes($edge["startportalid"])) {
				$direction = " -> ";
				$otherportal = getPortal(stripslashes($edge["endportalid"]));
			} else {
				$direction = " <- ";
				$otherportal = getPortal(stripslashes($edge["startportalid"]));
			}
			$portalplain = "Hors de la zone";
			if (stripslashes($otherportal["guid"])!="") {
				$portalplain = "<a href=\"portaldetail.php?guid=".stripslashes($otherportal["guid"])."\">".stripslashes($otherportal["plain"])."</a>";
			}
			print("<tr><td>Link:</td><td>".$direction." ".$portalplain."</td></tr>\n");
		}
		print("</table>");
		
		print("<br>Fields:<br>\n");
		print("<table border=\"1\">\n");
		foreach($portal["fields"] as $field) {
			$portalA = getPortal(stripslashes($field["portalAid"]));
			$portalB = getPortal(stripslashes($field["portalBid"]));
			$portalC = getPortal(stripslashes($field["portalCid"]));
			$portalAplain = "Hors de la zone";
			if (stripslashes($portalA["guid"])!="") {
				$portalAplain = "<a href=\"portaldetail.php?guid=".stripslashes($portalA["guid"])."\">".stripslashes($portalA["plain"])."</a>";
			}
			$portalBplain = "Hors de la zone";
			if (stripslashes($portalB["guid"])!="") {
				$portalBplain = "<a href=\"portaldetail.php?guid=".stripslashes($portalB["guid"])."\">".stripslashes($portalB["plain"])."</a>";
			}
			$portalCplain = "Hors de la zone";
			if (stripslashes($portalC["guid"])!="") {
				$portalCplain = "<a href=\"portaldetail.php?guid=".stripslashes($portalC["guid"])."\">".stripslashes($portalC["plain"])."</a>";
			}
			print("<tr><td>Field:</td><td>".$portalAplain."<br>\n
				".$portalBplain."<br>\n
				".$portalCplain."</a></td></tr>\n");
		}
		print("</table>");

	print("</td><td valign=\"top\">\n");
		print("<table border=\"0\">\n");
		foreach($lastActions as $action) {
			print(printAction($action));
		}
		print("</table>\n");
	print("</td></tr>\n</table>\n");
}

printBottom();

?>
