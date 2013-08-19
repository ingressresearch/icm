<?php
include_once("config.inc");
include_once("debug.inc");
include_once("myDB.class");
include_once("ingress.inc");

$starttimestamp = strtotime("26-feb-2013 00:00:00");
$endtimestamp = $starttimestamp + (24*60*60);

$i=0;
while ($i<400 && $starttimestamp<time()) {

	print(date("d-M-Y H:i:s", $starttimestamp)."/".date("d-M-Y H:i:s", $endtimestamp)."\n");
	$var = curl_download("https://www.ingress.com/rpc/dashboard.getPaginatedPlextsV2", $starttimestamp*1000, $endtimestamp*1000);
	$var["result"] = array_reverse($var["result"]);
	print_r($var); //die();
	saveIntelData($var);
	
	$starttimestamp = $starttimestamp + (24*60*60);
	$endtimestamp = $starttimestamp + (24*60*60);
	$i++;
	sleep(10);
}

?>
