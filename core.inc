<?php

include_once("config.inc");
include_once("authentication.inc");
include_once("email.inc");

$ok = false;
if (($config["referauth"]) || ($config["pwauth"])) {
	if (($_SERVER["REMOTE_ADDR"]!="") && !$noauth) {
		if ($config["referauth"]) {
			if (eregi($config["referer"], $_SERVER["HTTP_REFERER"]) && isset($_SERVER["HTTP_REFERER"])) {
				$ok = true;
			}
		}
		if ($config["pwauth"]) {
			if (isset($_COOKIE["domain.com"]) && validateCookie(parseCookie($_COOKIE["domain.com"]))) {
				$ok = true;
			}
		}
		if ($config["ipauth"]) {
			if (in_array($_SERVER["REMOTE_ADDR"], $config["ipallowed"])) {
				$ok = true;
			}
		}
		if (!$ok) {
			if ($config["pwauth"]) {
				header("Location: ".$config["notauthenticatedlogin"]);
			} else {
				header("Location: ".$config["notauthenticated"]);
			}
			die();
		}
	} else {
		$ok = true;
	}
} else {
	$ok = true;
}

function printTop() {
	print("<html><meta charset='utf-8'><head><title>Donnees Ingress techno-mages</title></head><link href=\"menu_assets/styles.css\" rel=\"stylesheet\" type=\"text/css\"><body>\n");

print("
<div id='cssmenu'>
<ul>
   <li class='active'><a href='index.php'><span>Home</span></a></li>
   <li class='has-sub'><a href='playerstats.php?type=team'><span>Equipes</span></a>
      <ul>
         <li><a href='playerstats.php?type=team'><span>Tous les joueurs</span></a></li>
         <li class='last'><a href='playerlocations.php'><span>Emplacement des joueurs</span></a></li>
      </ul>
   </li>
   <li class='has-sub last'><a href='portals.php'><span>Portails</span></a>
      <ul>
         <li><a href='portals.php'><span>Tous les portails</span></a></li>
         <li><a href='portallocation.php'><span>Emplacement des portails</span></a></li>
         <li><a href='capturedportals.php'><span>Portails captures</span></a></li>
         <li><a href='portals.php?level=7'><span>Portails hauts</span></a></li>
         <li><a href='portals.php?energy=20'><span>Portails faibles</span></a></li>
         <li class='last'><a href='addportalmonitor.php'><span>Surveiller</span></a></li>
      </ul>
   </li>
</ul>
</div>");
}
function printBottom() {
	print("</body></html>");
}



?>
