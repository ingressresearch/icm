<?php

function parseCookie(&$cookie) {
	$temp = Array();
	$a = explode("|", $cookie);
	$temp["email"] = $a[0];
	$temp["expiry"] = $a[1];
	$temp["ip"] = $a[2];
	$temp["md5"] = $a[3];
	return $temp;
}

function buildCookieString(&$decodedCookie) {
	global $config;

	$temp = $decodedCookie["email"]."|".$decodedCookie["expiry"]."|".$decodedCookie["ip"]."|".$config["secretpassword"];
	return $temp;
}

function buildCookie(&$decodedCookie) {
	global $config;

	$temp = $decodedCookie["email"]."|".$decodedCookie["expiry"]."|".$decodedCookie["ip"]."|".$config["secretpassword"];
	$md5 = md5($temp);
	$temp = $decodedCookie["email"]."|".$decodedCookie["expiry"]."|".$decodedCookie["ip"]."|".$md5;
	$decodedCookie["md5"]=$md5;
	return $temp;
}

function validateCookie(&$decodedCookie) {
	global $config;

	$md5 = md5(buildCookieString($decodedCookie));
	return ($md5 == $decodedCookie["md5"]);
}

?>
