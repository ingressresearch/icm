<?php
include_once("config.inc");
include_once("core.inc");
include_once("debug.inc");
include_once("myDB.class");
include_once("ingress.inc");

$portalsResistance = getNumberOfPortals("RESISTANCE");
$portalsAliens = getNumberOfPortals("ALIENS");

$playersResistance = getNumberOfPlayers("RESISTANCE");
$playersAliens = getNumberOfPlayers("ALIENS");

$lastPortalResistance = lastCapturedPortal("RESISTANCE");
$lastPortalAliens = lastCapturedPortal("ALIENS");
$lastActions = lastActions();

printTop();

print("<table border=\"0\">\n");
print("<tr><td valign=\"top\" width=\"60%\">");

	print("<table border =\"0\">\n");
	print("<tr><td bgcolor=\"".$config["resistancecolour"]."\">Portails de la resistance: ".count($portalsResistance)." (".(count($portalsResistance)/(count($portalsResistance)+count($portalsAliens))*100)."%)</td>\n");
	print("<td bgcolor=\"".$config["alienscolour"]."\">Portails des illumines: ".count($portalsAliens)." (".(count($portalsAliens)/(count($portalsResistance)+count($portalsAliens))*100)."%)</td></r>\n");
	print("<tr><td bgcolor=\"".$config["resistancecolour"]."\">Joueurs dans la resistance: ".count($playersResistance)." (".(count($playersResistance)/(count($playersResistance)+count($playersAliens))*100)."%)</td>\n");
	print("<td bgcolor=\"".$config["alienscolour"]."\">Joueurs chez les illumines: ".count($playersAliens)." (".(count($playersAliens)/(count($playersResistance)+count($playersAliens))*100)."%)</td></tr>\n");
	print("<tr><td bgcolor=\"".$config["resistancecolour"]."\">Dernier portail pris par la resistance: <a href=\"portaldetail.php?guid=".stripslashes($lastPortalResistance["guid"])."\">".stripslashes($lastPortalResistance["name"])."</a> (".stripslashes($lastPortalResistance["address"]).")<br>\n".stripslashes($lastPortalResistance["timestamp"])."</td>\n");
	print("<td bgcolor=\"".$config["alienscolour"]."\">Dernier portail pris par les illumines: <a href=\"portaldetail.php?guid=".stripslashes($lastPortalAliens["guid"])."\">".stripslashes($lastPortalAliens["name"])."</a> (".stripslashes($lastPortalAliens["address"]).")<br>\n".stripslashes($lastPortalAliens["timestamp"])."</td></tr>\n");
	print("</table>\n");
	print("Derniere mise a jour: ".getLastUpdate()."<br>\n");
	
	print("<br>\n");
	print("Nombre de portails captures dans les dernieres 24h:<br>\n");
	print("<img width=\"700\" src=\"index-capture.php?start=".date("Y-m-d H:00:00", strtotime("-1 days"))."&end=".date("Y-m-d H:00:00", time())."\"><br>\n");
	
	/* $captures = findActionsByPeriod(date("Y-m-d H:00:00", strtotime("-1 days")), date("Y-m-d H:00:00", time()), "CAPTURE");
	print("<table border=\"0\">\n");
	foreach($captures as $action) {
		print(printAction($action));
	}
	print("</table>\n"); */
	
	/* print("<br>\n");
	print("Emplacement des portails captures dans les dernieres 24h:<br>\n");
	print(drawMap(lastCapturedPortals(date("Y-m-d H:00:00", strtotime("-1 days")), date("Y-m-d H:00:00", time()))));
	*/
print("</td><td valign=\"top\">");
	print("<table border=\"0\">\n");
	foreach($lastActions as $action) {
		print(printAction($action));
	}
	print("</table>\n");
print("</td></tr>\n");
print("</table>\n");

printBottom();

?>
