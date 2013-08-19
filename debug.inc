<?php

include_once("core.inc");

function debug($text) {
	global $config;

	if ($config["debugdisplay"]) {
		print($text);
	}
}

?>
