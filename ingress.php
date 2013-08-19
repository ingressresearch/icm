<?php
include_once("config.inc");
include_once("debug.inc");
include_once("myDB.class");
include_once("ingress.inc");

$var = curl_download("https://www.ingress.com/rpc/dashboard.getPaginatedPlextsV2");

if (isset($var["result"])) {
	$var["result"] = array_reverse($var["result"]);
	//print_r($var); //die();
	saveIntelData($var);
} else {
	print_r($var);
}

?>
