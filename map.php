<?php
include_once("config.inc");
include_once("debug.inc");
include_once("myDB.class");
include_once("ingress.inc");
include_once("email.inc");
include_once("ingresscalculations.inc");

set_time_limit(60*60*5);

$portals = Array();
$allportals = Array();
$temp = getAllPortals();
foreach($temp as $portal) {
	$allportals[] = $portal["guid"];
}

$fields = Array();
$allfields = Array();
$temp = getAllFields();
foreach($temp as $field) {
	$allfields[] = $field["guid"];
}

if ($config["longlatslices"]<1) {
	$config["longlatslices"] = 1;
}
$xdiff = round(($config["maxlngE6"]-$config["minlngE6"])/$config["longlatslices"]);
$ydiff = round(($config["maxlatE6"]-$config["minlatE6"])/$config["longlatslices"]);
print("xdiff=".$xdiff."\n");
print("ydiff=".$ydiff."\n");
$xstart = $config["minlngE6"];
$ystart = $config["minlatE6"];
for ($xcoord=0; $xcoord<$config["longlatslices"]; $xcoord++) {
	print("xcoord ".$xcoord."\n");
	$ystart = $config["minlatE6"];	
	for ($ycoord=0; $ycoord<$config["longlatslices"]; $ycoord++) {
		print("xcoord ".$xcoord." of ".$config["longlatslices"].", ycoord ".$ycoord." of ".$config["longlatslices"]."\n");
		print($xstart."/".($xstart+$xdiff)."/".$ystart."/".($ystart+$ydiff)."\n");

		$bounds = Array("ne"=>Array("lat"=>(($ystart+$ydiff)/1000000),"lng"=>(($xstart+$xdiff)/1000000)), "sw"=>Array("lat"=>($ystart/1000000),"lng"=>($xstart/1000000)));
		//$bounds = Array("ne"=>Array("lat"=>46195042/1000000,"lng"=>1230469/1000000), "sw"=>Array("lat"=>45073231/1000000,"lng"=>1054688/1000000));
		$tiles = createIngressTiles(Array("lat"=>(($ystart+($ydiff/2))/1000000), "lng"=>(($xstart+($xdiff/2))/1000000)), $bounds);
		
		//$quadkey = "013212223212";
		//$tile = Array(
		//    "id"=> $quadkey,
		//    "qk"=> $quadkey,
		//    "minLatE6"=> $ystart,
		//    "minLngE6"=> $xstart,
		//    "maxLatE6"=> $ystart+$ydiff,
		//    "maxLngE6"=> $xstart+$xdiff
		//  );
		
		//$tiles = Array($tile);
		print("Querying ".count($tiles)." tiles\n");
		$tilecount=0;
		while($tilecount<count($tiles)) {
			$pileoftiles = Array();
			for($i=0; $i<10 && $tilecount<count($tiles); $i++) {
				$tile = $tiles[$tilecount];
				$pileoftiles[] = $tile;
				$tilecount++;
			}
			print("Querying tile ".$tilecount.". ".(count($tiles)-$tilecount)." to go.\n");
			$var = makeIngressRequest($pileoftiles);
			print_r($var);
			
			foreach($var["result"]["map"] as $chunk) {
				if (count($chunk["gameEntities"])>0) {
					$config["debugdisplay"] = true;
					$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
					if ($db->dbID) {
						$query = "insert into portalarchive(jason) values ('".base64_encode(gzcompress(serialize($var)))."');";
						$db->setQuery($query);
						if ($db->executeQuery()) {
						} else {
							debug("Couldn't execute query");
						}
				
						$map = $chunk["gameEntities"];
						foreach($map as $portaldef) {
							if (!isset($portaldef[2]["edge"]) && !isset($portaldef[2]["capturedRegion"])) { //Fields seem to be described within portals
								//print_r($portaldef); print("\n");
								$portalid = $portaldef[0];
								$portallngE6 = $portaldef[2]["locationE6"]["lngE6"];
								$portallatE6 = $portaldef[2]["locationE6"]["latE6"];
								$portalimage = $portaldef[2]["imageByUrl"]["imageUrl"];
								$portalteam = $portaldef[2]["controllingTeam"]["team"];
								if ($portalteam=="ENLIGHTENED") { $portalteam="ALIENS"; } 
								$portalname = $portaldef[2]["portalV2"]["descriptiveText"]["TITLE"];
								$portaladdress = $portaldef[2]["portalV2"]["descriptiveText"]["ADDRESS"];
								$playerid = $portaldef[2]["captured"]["capturingPlayerId"];
								$capturedtime = substr($portaldef[2]["captured"]["capturedTime"], 0, strlen($portaldef[2]["captured"]["capturedTime"])-3);
								
								print($portalid."/".$portallngE6."/".$portallatE6."/".$portalteam."/".$portalname."/".$portaladdress."/".$playerid."/".$capturedtime."\n");
								if ($portallngE6<$config["minlngE6"] || $portallngE6>$config["maxlngE6"] || $portallatE6<$config["minlatE6"] || $portallatE6>$config["maxlatE6"] || !ereg("France", $portaladdress)) {
									print("Portal out of bounds\n");
								} else {
									$player = findPlayer($playerid);
									if ($player==false) {
										addPlayer($playerid, $playerid, $portalteam);
									}
									
									updatePortal($portalid, $portalname, $portalname." (".$portaladdress.")", $portalteam, $portaladdress, $portallatE6, $portallngE6, $portalimage, $playerid, date("Y-m-d H:i:s", $capturedtime));
									deletePortalResonators($portalid);
									
									foreach($portaldef[2]["resonatorArray"]["resonators"] as $resonator) {
										if (is_array($resonator)) {
											//print_r($resonator);
											if (isset($resonator["id"])) {
												updateResonator($portalid, $resonator["id"], $resonator["slot"], $resonator["level"], $resonator["energyTotal"], $resonator["distanceToPortal"], $resonator["ownerGuid"]);
												$player = findPlayer($resonator["ownerGuid"]);
												if ($player==false) {
													addPlayer($resonator["ownerGuid"], $resonator["ownerGuid"], $portalteam);
												}
											}
										}
									}
				
									deletePortalEdges($portalid);
									
									foreach($portaldef[2]["portalV2"]["linkedEdges"] as $edge) {
										//print_r($edge);
										if ($edge["isOrigin"]=="1") {
											$portaldest = $edge["otherPortalGuid"];
											updateEdge($edge["edgeGuid"], $portalid, $portaldest);
											print("Edge: ".$edge["edgeGuid"]." ".$portalid."->".$portaldest."\n");
										}
									}
				
									deletePortalMods($portalid);
									
									foreach($portaldef[2]["portalV2"]["linkedModArray"] as $mod) {
										if (is_array($mod)) {
											//print_r($mod);
											$player = $mod["installingUser"];
											$display = $mod["displayName"];
											$type = $mod["type"];
											$rarity = $mod["rarity"];
											$stats = "";
											foreach($mod["stats"] as $stat => $value) {
												if ($stats!="") {
													$stats .= ", ";
												}
												$stats .= $stat." ".$value;
											}
											addPortalMod($portalid, $type, $playerid, $display, $rarity, $stats);
											print("PortalMod: ".$player." ".$portalid." ".$type." ".$rarity." ".$stats."\n");
										}
									}
				
									$portals[] = $portalid;
									//print_r($portal); print("\n");
								}
							} else if (isset($portaldef[2]["capturedRegion"])) {
								//print_r($portaldef); print("\n");
								$fieldid = $portaldef[0];
								$team = $portaldef[2]["controllingTeam"]["team"];
								if ($team=="ENLIGHTENED") { $team="ALIENS"; } 
								$portalAid = $portaldef[2]["capturedRegion"]["vertexA"]["guid"];
								$portalBid = $portaldef[2]["capturedRegion"]["vertexB"]["guid"];
								$portalCid = $portaldef[2]["capturedRegion"]["vertexC"]["guid"];
								$score = $portaldef[2]["entityScore"]["entityScore"];
								$playerid = $portaldef[2]["creator"]["creatorGuid"];
								$creationtime = $portaldef[2]["creator"]["creationTime"];
		
								if ($fieldid!="") {
									$fields[] = $fieldid;
									print("Saving field ".$fieldid." from ".$portalAid." to ".$portalBid." to ".$portalCid." for ".$team."\n");
									updateField($fieldid, $portalAid, $portalBid, $portalCid, $score, $team, $playerid, substr($creationtime, 0, strlen($creationtime)-3));
								}
							}
						}
					} else {
						debug("Unable to connect to DB server");
						print("Unable to connect to DB server");
					}
					
				} else {
					//print_r($var);
					//email(stripslashes($config["mailfrom"]), "Map ".$_SERVER["PHP_SELF"]." didn't load.");
				}
			}
		}
		$ystart += $ydiff;
		sleep(rand(60, 180));
	}
	$xstart += $xdiff;
}

print("Pruning portals.\n");
$staleportals = array_diff($allportals, $portals);
if (count($staleportals)>0 && count($staleportals)<=5 && count($allportals)>0) {
	print_r($staleportals);
	deletePortals($staleportals);
}

print("Pruning fields.\n");
$stalefields = array_diff($allfields, $fields);
if (count($stalefields)>0  && count($allfields)>0) {
	print_r($stalefields);
	deleteFields($stalefields);
}

?>
