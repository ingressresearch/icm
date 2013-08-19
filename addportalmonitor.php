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

printTop();

if ($_REQUEST["op"]=="") {
	$portals = getAllPortals();
	if (isset($_SERVER['PHP_AUTH_USER'])) {
		$players = array(findPlayerByName($_SERVER['PHP_AUTH_USER']));
	} else {
		$players = getAllPlayers();
	}
	
	print("<script lang=\"JavaScript\" src=\"js/list.js\"></script>\n");
	print("Ajouter un portal monitor:<br>\n");
	print("<form METHOD=\"POST\" enctype=\"multipart/form-data\" action=\"addportalmonitor.php\">\n<input type=hidden name=\"op\" value=\"add\">\n");
	print("<table border=\"0\"><tr><td>Joueur</td><td>Portail</td><td>Email</td></tr>\n");
	print("<tr><td valign=\"top\">");
		
	if (!isset($_SERVER['PHP_AUTH_USER'])) {
		print("Recherche: <input name=\"playersearch\" onKeyUp=\"search(playerid,playersearch)\"><br>\n");
	}
	print("<select name=\"playerid\" id=\"playerid\">\n");
	foreach ($players as $player) {
		print("<option value=\"".stripslashes($player["guid"])."\">".stripslashes($player["name"])."</option>\n");
	}
	print("</select>");
	if (isset($_SERVER['PHP_AUTH_USER'])) {
		$player = findPlayerByName($_SERVER['PHP_AUTH_USER']);
		$portalmonitors = getPlayerPortalMonitors(stripslashes($player["guid"]));
		print("<br>\n");
		print("<table border=\"1\">\n");
		print("<tr><td>Joueur</td><td>Portail</td><td>Equipe</td><td></td></tr>\n");
		foreach ($portalmonitors as $portalmonitor) {
			print("<tr><td valign=\"top\"><a href=\"playerstats.php?type=player&playerid=".stripslashes($player["guid"])."\">".stripslashes($player["name"])."</a></td><td valign=\"top\"><a href=\"portaldetail.php?guid=".stripslashes($portalmonitor["portalid"])."\">".stripslashes($portalmonitor["portalplain"])."</a></td><td valign=\"top\">".stripslashes($portalmonitor["portalteam"])."</td><td><input type=button value=\"Effacer\" onclick=\"location.href='addportalmonitor.php?op=askremove&portalid=".stripslashes($portalmonitor["portalid"])."'\"></td></tr>\n");
		}
		print("</table>\n");
	}
	print("</td><td valign=\"top\">");
	print("Recherche: <input name=\"portalsearch\" onKeyUp=\"search(portalidfrom,portalsearch)\"><br>\n");
	print("<select name=\"portalidfrom\" multiple size=\"10\" id=\"portalidfrom\" style=\"width: 600px;\"> ");
	foreach ($portals as $portal) {
		print("<option value=\"".stripslashes($portal["guid"])."\">".stripslashes($portal["plain"])."</option>\n");
	}
	print("</select><br>\n");
	print("<input type=\"button\" name=\"Ajouter  V\" value=\"Ajouter  V\" onclick=\"move('portalidfrom','portalid');\"> ");
	print("<input type=\"button\" name=\"Enlever  ^\" value=\"Enlever  ^\" onclick=\"remove('portalid','portalidfrom');\"> ");
	print("<input type=\"button\" name=\"Vider\" value=\"Vider\" onclick=\"clearList('portalid', 'portalidfrom');\"><br>\n");
	print("<select name=\"portalid[]\" id=\"portalid\" multiple size=\"10\" style=\"width: 600px;\">\n");
	print("</select></td><td valign=\"top\">");
	print("<input name=\"email\" value=\"".(isset($_SERVER['PHP_AUTH_USER'])?stripslashes($players[0]["email"]):"")."\"> (si l'addresse n'apparait pas, envoyez-la moi)\n</td></tr>\n");
	print("</table>\n");
	print("<input type=submit value=\"Save\">");
} else if ($_REQUEST["op"]=="add") {
	$player = getPlayer($_REQUEST["playerid"]);
	if ($player===false || $player["guid"]=="") {
		print("Player not found\n");
		die();
	}
	if (!is_array($_REQUEST["portalid"])) {
		$_REQUEST["portalid"] = array($_REQUEST["portalid"]);
	}
	foreach($_REQUEST["portalid"] as $portalid) {
		$portal = getPortal($portalid);
		if ($portal===false || $portal["guid"]=="") {
			print("Portal ".$portalid." not found<br>\n");
		} else {
			addPortalMonitor($_REQUEST["playerid"], $portalid, $_REQUEST["email"]);
			$key = getPortalMonitorKey(stripslashes($_REQUEST["playerid"]), stripslashes($portalid));
			email(stripslashes($player["email"]), "Vous receverez maintenant les notifications concernant ".stripslashes($portal["plain"])."\nVotre clef secrete est: ".($key*$config["keyfactor"])." (necessaire pour desactiver le suivi).");
			print("Player ".stripslashes($player["name"])." now monitors ".stripslashes($portal["plain"])."<br>\n");
		}
	}
	print("<br><br>\n<a href=\"addportalmonitor.php\">Retour</a>\n");
} else if ($_REQUEST["op"]=="askremove") {
	print("Desactivation d'un portal monitor<br><br>\n");
	print("<form action=\"addportalmonitor.php\">\n");
	print("<input type=hidden name=\"op\" value=\"remove\">\n");
	print("<input type=hidden name=\"portalid\" value=\"".stripslashes($_REQUEST["portalid"])."\">\n");
	print("Clef secrete (envoyee par mail au moment de la creation du monitor: <input name=\"key\"><br>\n");
	print("<input type=submit><br>\n");
	print("</form>\n");
	print("<br><br>\n<a href=\"addportalmonitor.php\">Retour</a>\n");
} else if ($_REQUEST["op"]=="remove") {
	if ($_REQUEST["portalid"]=="" || $_REQUEST["key"]=="") {
		print("ID ou clef secrete invalide.");
	} else {
		if (isset($_SERVER['PHP_AUTH_USER'])) {
			$player = findPlayerByName($_SERVER['PHP_AUTH_USER']);
		}
		if ($player!==false) {		
			$temp = removePortalMonitor(stripslashes($player["guid"]), $_REQUEST["portalid"], $_REQUEST["key"]);
			print("Monitor efface.");
		} else {
			print("Mauvais joueur.");
		}
	}
	print("<br><br>\n<a href=\"addportalmonitor.php\">Retour</a>\n");
} else {
	print("Unknown operation");
}

printBottom();

?>
