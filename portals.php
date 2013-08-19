<?php
include_once("config.inc");
include_once("core.inc");
include_once("debug.inc");
include_once("myDB.class");
include_once("ingress.inc");

$team = "";
if (isset($_REQUEST["team"])) {
	$team = strtoupper($_REQUEST["team"]);
}
$level = "";
if (isset($_REQUEST["level"])) {
	$level = strtoupper($_REQUEST["level"]);
}
$energy = "";
if (isset($_REQUEST["energy"])) {
	$energy = strtoupper($_REQUEST["energy"]);
}
$minlat = "";
if (isset($_REQUEST["minlat"])) {
	$minlat = strtoupper($_REQUEST["minlat"]);
}
$maxlat = "";
if (isset($_REQUEST["maxlat"])) {
	$maxlat = strtoupper($_REQUEST["maxlat"]);
}
$minlng = "";
if (isset($_REQUEST["minlng"])) {
	$minlng = strtoupper($_REQUEST["minlng"]);
}
$maxlng = "";
if (isset($_REQUEST["maxlng"])) {
	$maxlng = strtoupper($_REQUEST["maxlng"]);
}
$postcodes_array = Array();
$a = explode(",", $postcodes);
foreach ($a as $p) {
	$postcodes_array[] = trim($p);
} 
$data = getAllPortals($team, $level, $energy, $postcodes_array);

printTop();

print("Liste des portails ");
if ($level!="") {
	print(" avec level>=".$level);
}
if ($team!="") {
	print(" avec team=".$team);
}
if ($energy!="") {
	print(" avec energie<=".$energy."%");
}
if ($postcodes!="") {
	print(" pour codes postaux ".$postcodes);
}
if ($minlat!="" && $maxlat!="" && $minlng!="" && $maxlng!="") {
	print(" pour coordonn&eacute;es ".$minlat.",".$maxlat.",".$minlng.",".$maxlng);
}
print(" (".count($data)."):<br>\n");

print("<form action=\"portals.php\">\n");
print("<table border=\"0\">\n<tr valign=\"top\"><td>");
print("Equipe: <select name=\"team\"><option value=\"\"></option><option ".(strtolower($team)=="resistance"?"SELECTED":"")." value=\"RESISTANCE\">RESISTANCE</option><option ".(strtolower($team)=="aliens"?"SELECTED":"")." value=\"ALIENS\">ALIENS</option><option ".(strtolower($team)=="neutral"?"SELECTED":"")." value=\"NEUTRAL\">NEUTRE</option></select><br>\n");
print("Niveau minimum: <input name=\"level\" value=\"".$level."\"><br>\n");
print("Energie maximum: <input name=\"energy\" value=\"".$energy."\"><br>\n");
print("</td>\n<td>");
print("Min latitude: <input name=\"minlat\" value=\"".$minlat."\"> (non E6)<br>\n");
print("Max latitude: <input name=\"maxlat\" value=\"".$maxlat."\"> (non E6)<br>\n");
print("Min longitude: <input name=\"minlng\" value=\"".$minlng."\"> (non E6)<br>\n");
print("Min longitude: <input name=\"maxlng\" value=\"".$maxlng."\"> (non E6)<br>\n");
print("Codes postaux: <input name=\"postcodes\" value=\"".$postcodes."\"> (s&eacute;par&eacute;s par ,)<br>\n");
print("</td></tr></table>");
print("<input type=submit><br>\n");
print("</form><br>\n");

$header = "<tr><td>Nom</td><td>Adresse</td><td>Niveau</td><td>Energie %</td><td>Coordonnees</td><td></td></tr>\n";

print("<table border=\"1\">\n".$header);
$i=0;
foreach ($data as $portal) {
	$keep = true;
	if ($minlat!="" && $maxlat!="" && $minlng!="" && $maxlng!="") {
		if (stripslashes($portal["latE6"])/1000000<$minlat || stripslashes($portal["latE6"])/1000000>$maxlat || stripslashes($portal["lngE6"])/1000000<$minlng || stripslashes($portal["lngE6"])/1000000>$maxlng) {
			$keep = false;
		}
	}
	if ($keep) {
		$colour = $config[strtolower(stripslashes($portal["team"]))."colour"];
		print("<tr bgcolor=\"".$colour."\"><td>".stripslashes($portal["name"])."</td><td>".stripslashes($portal["address"])."</td><td>".(stripslashes($portal["level"])!=""?stripslashes($portal["level"]):"<=3")."</td><td>".stripslashes($portal["energy"])."</td><td><a href=\"http://www.ingress.com/intel?latE6=".stripslashes($portal["latE6"])."&lngE6=".stripslashes($portal["lngE6"])."&z=16\">".(stripslashes($portal["latE6"])/1000000).", ".(stripslashes($portal["lngE6"])/1000000)."</a></td><td><a href=\"portaldetail.php?guid=".stripslashes($portal["guid"])."\">Details</a></td></tr>\n");
		$i++;
		if ($i % 25 == 0) {
			print($header);
		}
	}
}
print("</table>");

printBottom();

?>
