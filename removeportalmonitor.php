<?php
include_once("config.inc");
include_once("debug.inc");
include_once("myDB.class");
include_once("ingress.inc");
include_once("notification.inc");
include_once("email.inc");

function savePortalMonitor($playerid, $portalid) {
	global $config;
	
	$temp = '2013-01-11 00:00:00';
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select timestamp from lastnotification;";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			if($db->hasMoreElements()) {
				$temp=$db->nextElement();
				$temp = $temp["timestamp"];
			}
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

if ($_POST["op"]=="") {
	$portals = getAllPortals();
	$players = getAllPlayers();
	
	print("Retirer un portal monitor:<br>\n");
	print("<form METHOD=\"POST\" enctype=\"multipart/form-data\" action=\"removeportalmonitor.php\">\n<input type=hidden name=\"op\" value=\"remove\">\n");
	print("<table border=\"0\"><tr><td>Joueur</td><td>Portail</td><td>Clef secrete</td></tr>\n");
	print("<tr><td><select name=\"playerid\">\n");
	foreach ($players as $player) {
		print("<option value=\"".stripslashes($player["guid"])."\">".stripslashes($player["name"])."</option>\n");
	}
	print("</select></td><td><select name=\"portalid\">");
	foreach ($portals as $portal) {
		print("<option value=\"".stripslashes($portal["guid"])."\">".stripslashes($portal["plain"])."</option>\n");
	}
	print("</select></td><td>");
	print("<input name=\"clef\">\n</td></tr>\n");
	print("</table>\n");
	print("<input type=submit value=\"Save\">");
} else if ($_POST["op"]=="remove") {
	$player = getPlayer($_POST["playerid"]);
	if ($player===false || $player["guid"]=="") {
		print("Player not found\n");
		die();
	}
	$portal = getPortal($_POST["portalid"]);
	if ($portal===false || $portal["guid"]=="") {
		print("Portal not found\n");
		die();
	}
	removePortalMonitor($_POST["playerid"], $_POST["portalid"], $_POST["clef"]);
	email(stripslashes($player["email"]), "Vous ne receverez plus les notifications concernant ".stripslashes($portal["plain"])."\n");
	print("Player ".stripslashes($player["name"])." no longer monitors ".stripslashes($portal["plain"]));
}else {
	print("Unknown operation");
}

?>
