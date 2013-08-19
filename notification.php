<?php
include_once("config.inc");
include_once("debug.inc");
include_once("myDB.class");
include_once("email.inc");
include_once("notification.inc");

$lastnotification=getLastNotification();
recordLastNotification();

$data = getPortalMonitors($lastnotification);
if ($data!==false) {
	print_r($data);
	foreach($data as $player) {
		print("Emailing ".stripslashes($player["playeremail"])." ".stripslashes($player["actiontext"])."\n");
		email(stripslashes($player["playeremail"]), "Evenement portail:\n(".stripslashes($player["playerteam"]).") ".stripslashes($player["actiontext"]));
	}
}

$data = getPlayerMonitors($lastnotification);
if ($data!==false) {
	print_r($data);
	foreach($data as $player) {
		print("Emailing ".stripslashes($player["playeremail"])." ".stripslashes($player["actiontext"])."\n");
		email(stripslashes($player["playeremail"]), "Evenement joueur:\n(".stripslashes($player["playerteam"]).") ".stripslashes($player["actiontext"]));
	}
}

?>
