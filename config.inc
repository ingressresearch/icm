<?php

$config = Array();

$config["sitename"] = "Ingress Change Monitor (ICM)";

$config["dbserver"] = "127.0.0.1";
$config["dbdatabase"] = "icm";
$config["dbuser"] = "icm";
$config["dbpassword"] = "icm";

$config["debugdisplay"] = false;

$config["mailfrom"] = "ingress.notification@domain.com";
$config["mailfromname"] = "Notification Ingress @ domain.com";
$config["notificationsubject"] = "Notification d'activite Ingress";

$config["keyfactor"] = 6546891;
$config["resistancecolour"] = "#00adef";
$config["alienscolour"] = "#8dc641";

$config["minlatE6"] = 42747012;
$config["minlngE6"] = -1812744;
$config["maxlatE6"] = 45660127;
$config["maxlngE6"] = 1472168;
$config["queryzoom"] = 0; //17=all portals

$config["longlatslices"] = 3;

include_once("./acsid.inc");
$config["mapparameters"] = "ingress.intelmap.lat=44.488668; ingress.intelmap.lng=-0.664673; ingress.intelmap.zoom=8";
$config["cookiestring"] = "Cookie: __utma=24037858.684179029.1357746829.1361271667.1361373461.90; __utmz=24037858.1359118361.32.6.utmcsr=domain.com|utmccn=(referral)|utmcmd=referral|utmcct=/ingresspoitou/portaldetail.php; csrftoken=".$config["X-CSRFToken"]."; __utmc=24037858; ingress.intelmap.type=3; __utmb=24037858.107.9.1361375490507; ACSID=".$config["ACSID"]."; SACSID=".$config["SACSID"].";".$config["mapparameters"];

$config["centerlat"] = 44.488668;
$config["centerlng"] = -0.664673;
$config["defaultzoom"] = 8;
?>
