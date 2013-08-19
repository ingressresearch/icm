<?php

$noauth=1;

include_once("config.inc");
include_once("core.inc");
include_once("email.inc");
include_once("passwords.inc");

if ($op=="") {
	print("<form action=\"login.php\">\n");
	print("<input type=hidden name=\"op\" value=\"login\">\n");
	print("<table border=\"0\"><tr><td>Nom:</td><td><input name=\"userlogin\"></td></tr>\n");
	print("<tr><td>Mot de passe:</td><td><input type=\"password\" name=\"userpassword\"></td></tr>\n");
	print("<tr><td><input type=\"submit\" name=\"OK\"></td><td></td></tr>\n");
	print("</table><br>");
} else if ($op=="login") {
	$ok = false;
	if ($passwords[$_REQUEST["userlogin"]]==$_REQUEST["userpassword"]) {
		$ok = true;
		$decodedCookie = Array("email"=>$_REQUEST["userlogin"], "expiry"=>time()+60*60*24*30, "ip"=>"");
		$cookie = buildCookie($decodedCookie);
		setcookie("domain.com", $cookie, time()+60*60*24*30);
		header("Location: index.php");
		die();
	} else {
		header("Location: ".$config["notauthenticated"]);
		die();
	}
}else {
	$_SERVER["PHP_AUTH_USER"];
}

?>
