<?php

function email($recipient, $body) {
	global $config;

	sendEmail($config["mailfromname"], $config["mailfrom"], $recipient, $config["notificationsubject"], $body);
}

function sendEmail($from, $fromemail, $recipient, $subject, $body) {
	global $config;
	
	$header = "From: ". $from . " <" . $fromemail . ">\r\n"; //optional headerfields

	ini_set('sendmail_from', $fromemail); //Suggested by "Some Guy"

	mail($recipient, $subject, $body, $header); //mail command :)
}

?>
