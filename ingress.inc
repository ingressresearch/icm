<?php

include_once("ingressencoding.inc");

$resonatorEnergy = Array(0, 1000, 1500, 2000, 2500, 3000, 4000, 5000, 6000);

function curl_download($Url, $mintimestamp=-1, $maxtimestamp=-1, $minlatoverride="", $maxlatoverride="", $minlngoverride="", $maxlngoverride=""){
	global $config;
	global $encoding0, $encoding1;

    if (!function_exists('curl_init')){
       die('Sorry cURL is not installed!');
    }

    $minlat = $config["minlatE6"];
    if ($minlatoverride!="") { $minlat = $minlatoverride; }
    $maxlat = $config["maxlatE6"];
    if ($maxlatoverride!="") { $maxlat = $maxlatoverride; }
    $minlng = $config["minlngE6"];
    if ($minlngoverride!="") { $minlng = $minlngoverride; }
    $maxlng = $config["maxlngE6"];
    if ($maxlngoverride!="") { $maxlng = $maxlngoverride; }
    if ($mintimestamp==-1 && $maxtimestamp ==-1) { $mintimestamp = strtotime("-6 hours")*1000; }
    //$data = array("desiredNumItems" => "50","maxLatE6" => "50975033","maxLngE6"=>"8294678","maxTimestampMs" => "-1","minLatE6" => "42491544","minLngE6"=>"-12205811","minTimestampMs" => "-1","method"=>"dashboard.getPaginatedPlextsV2");
    //$data = array("desiredNumItems" => "1000","maxLatE6" => $maxlat,"maxLngE6"=>$maxlng,"maxTimestampMs" => $maxtimestamp,"minLatE6" => $minlat,"minLngE6"=>$minlng,"minTimestampMs" => $mintimestamp,"maxTimestampMs"=>-1,"factionOnly"=>true,"ascendingTimestampOrder"=>true, "method"=>"dashboard.getPaginatedPlextsV2");
    //$data_string = json_encode($data);
    //$data_string = "{\"desiredNumItems\":50,\"minLatE6\":-52593704,\"minLngE6\":-170507812,\"maxLatE6\":74353051,\"maxLngE6\":164531250,\"minTimestampMs\":1367998422504,\"maxTimestampMs\":-1,\"factionOnly\":true,\"ascendingTimestampOrder\":true,\"method\":\"dashboard.getPaginatedPlextsV2\"}";
    //$data_string = "{\"desiredNumItems\":1000,\"minLatE6\":$minlat,\"minLngE6\":$minlng,\"maxLatE6\":$maxlat,\"maxLngE6\":$maxlng,\"minTimestampMs\":$mintimestamp,\"maxTimestampMs\":$maxtimestamp,\"factionOnly\":false,\"ascendingTimestampOrder\":false,\"method\":\"dashboard.getPaginatedPlextsV2\"}";
    //$data_string = "{\"4kr3ofeptwgary2j\":\"dashboard.getPaginatedPlextsV2\",\"tmb0vgxgp5grsnhp\":100,\"pg98bwox95ly0ouu\":45595812,\"eib1bkq8znpwr0g7\":80009,\"ilfap961rwdybv63\":45703610,\"lpf7m1ifx0ieouzq\":234217,\"hljqffkpwlx0vtjt\":1378107100257,\"sw317giy6x2xj9zm\":-1,\"0dvtbatgzcfccchh\":true}";
    $data_string = "{\"".$encoding0["desiredNumItems"]."\":1000,\"".$encoding0["minLatE6"]."\":$minlat,\"".$encoding0["minLngE6"]."\":$minlng,\"".$encoding0["maxLatE6"]."\":$maxlat,\"".$encoding0["maxLngE6"]."\":$maxlng,\"".$encoding0["minTimestampMs"]."\":$mintimestamp,\"".$encoding0["maxTimestampMs"]."\":$maxtimestamp,\"".$encoding0["factionOnly"]."\":false,\"".$encoding0["method"]."\":\"dashboard.getPaginatedPlextsV2\"}";
    print($data_string);                                                                                                                                                              

    $curl = curl_init();
    curl_setopt($curl, CURLOPT_URL, $Url);
    curl_setopt($curl, CURLOPT_REFERER, "https://www.ingress.com/intel");
    curl_setopt($curl, CURLOPT_USERAGENT, "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:18.0) Gecko/20100101 Firefox/18.0");
    curl_setopt($curl, CURLOPT_POST, 1);
    curl_setopt($curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_setopt($curl, CURLOPT_POSTFIELDS, $data_string);
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($curl, CURLOPT_BINARYTRANSFER, 0);
    curl_setopt($curl, CURLOPT_SSL_VERIFYPEER,false);
    curl_setopt($curl, CURLOPT_CAINFO,'cacert.pem');
    curl_setopt($curl, CURLOPT_HTTPHEADER, array('X-Requested-With: XMLHttpRequest', 'X-CSRFToken: '.$config["X-CSRFToken"], $config["cookiestring"],'Content-Type: application/json; charset=utf-8', 'Content-Length: ' . strlen($data_string)));
    $result = curl_exec($curl);
    if(!curl_errno($curl)){
      //$info = curl_getinfo($curl);
      $output = json_decode($result, true);
    } else {
      echo 'Curl error: ' . curl_error($curl);
    } 
    curl_close($curl);

    return $output;
}

function saveIntelData($var) {
	global $config;

	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		if (isset($var["result"]) && count($var["result"])>1) {
			foreach($var["result"] as $event) {
				//print_r($event);
				$eventid = $event[0];
				$eventtimestamp = $event[1];
				$description = $event[2]["plext"];
				if (trim($description["plextType"])=="SYSTEM_BROADCAST") {
					$eventtext = trim($description["text"]);
					$markup = $description["markup"];
				
					if (trim($markup[0][0])=="PLAYER") {
						$playername=trim($markup[0][1]["plain"]);
						$playerid=trim($markup[0][1]["guid"]);
						$playerteam=trim($markup[0][1]["team"]);
						if ($playerteam=="ENLIGHTENED") { $playerteam="ALIENS"; } 
					}
					
					if (trim($markup[1][0])=="TEXT") {
						// Description of the action
						if (trim($markup[1][1]["plain"])=="destroyed an") {
							$actiontype="DESTROY";
						} else if (trim($markup[1][1]["plain"])=="destroyed a") {
							$actiontype="DESTROYMOD";
						} else if (trim($markup[1][1]["plain"])=="deployed an") {
							$actiontype="DEPLOY";
						} else if (trim($markup[1][1]["plain"])=="linked") {
							$actiontype="LINK";
						} else if (trim($markup[1][1]["plain"])=="destroyed the Link") {
							$actiontype="DESTROYLINK";
						} else if (trim($markup[1][1]["plain"])=="created a Control Field @") {
							$actiontype="FIELD";
						} else if (trim($markup[1][1]["plain"])=="destroyed a Control Field @") {
							$actiontype="DESTROYFIELD";
						} else if (trim($markup[1][1]["plain"])=="captured") {
							$actiontype="CAPTURE";
						} else {
							$actiontype="UNINTERESTING";
						}
				
						if ($actiontype=="DESTROY" || $actiontype=="DEPLOY") {
							$actionlevel = trim($markup[2][1]["plain"]);
							if (trim($markup[4][0])=="PORTAL") {
								$portalname=trim($markup[4][1]["name"]);
								$portalplain=trim($markup[4][1]["plain"]);
								$portalteam=trim($markup[4][1]["team"]);
								if ($portalteam=="ENLIGHTENED") { $portalteam="ALIENS"; } 
								$portaladdress=trim($markup[4][1]["address"]);
								$portallatE6=trim($markup[4][1]["latE6"]);
								$portallngE6=trim($markup[4][1]["lngE6"]);
								$portalid=trim($markup[4][1]["guid"]);
							}
						} else if ($actiontype=="DESTROYMOD") {
							$actionlevel = "";
							if (trim($markup[4][0])=="PORTAL") {
								$portalname=trim($markup[4][1]["name"]);
								$portalplain=trim($markup[4][1]["plain"]);
								$portalteam=trim($markup[4][1]["team"]);
								if ($portalteam=="ENLIGHTENED") { $portalteam="ALIENS"; } 
								$portaladdress=trim($markup[4][1]["address"]);
								$portallatE6=trim($markup[4][1]["latE6"]);
								$portallngE6=trim($markup[4][1]["lngE6"]);
								$portalid=trim($markup[4][1]["guid"]);
							}
						} else if ($actiontype=="CAPTURE") {
							if (trim($markup[2][0])=="PORTAL") {
								$portalname=trim($markup[2][1]["name"]);
								$portalplain=trim($markup[2][1]["plain"]);
								$portalteam=trim($markup[2][1]["team"]);
								if ($portalteam=="ENLIGHTENED") { $portalteam="ALIENS"; } 
								$portaladdress=trim($markup[2][1]["address"]);
								$portallatE6=trim($markup[2][1]["latE6"]);
								$portallngE6=trim($markup[2][1]["lngE6"]);
								$portalid=trim($markup[2][1]["guid"]);
							}
						} else if (($actiontype=="FIELD") || ($actiontype=="DESTROYFIELD")) {
							if (trim($markup[2][0])=="PORTAL") {
								$portalname=trim($markup[2][1]["name"]);
								$portalplain=trim($markup[2][1]["plain"]);
								$portalteam=trim($markup[2][1]["team"]);
								if ($portalteam=="ENLIGHTENED") { $portalteam="ALIENS"; } 
								$portaladdress=trim($markup[2][1]["address"]);
								$portallatE6=trim($markup[2][1]["latE6"]);
								$portallngE6=trim($markup[2][1]["lngE6"]);
								$portalid=trim($markup[2][1]["guid"]);
							}
						} else if ($actiontype=="LINK") {
							if (trim($markup[2][0])=="PORTAL") {
								$portalname=trim($markup[2][1]["name"]);
								$portalplain=trim($markup[2][1]["plain"]);
								$portalteam=trim($markup[2][1]["team"]);
								if ($portalteam=="ENLIGHTENED") { $portalteam="ALIENS"; } 
								$portaladdress=trim($markup[2][1]["address"]);
								$portallatE6=trim($markup[2][1]["latE6"]);
								$portallngE6=trim($markup[2][1]["lngE6"]);
								$portalid=trim($markup[2][1]["guid"]);
							}
				
							if (trim($markup[4][0])=="PORTAL") {
								$toportalname=trim($markup[4][1]["name"]);
								$toportalplain=trim($markup[4][1]["plain"]);
								$toportalteam=trim($markup[4][1]["team"]);
								if ($portalteam=="ENLIGHTENED") { $portalteam="ALIENS"; } 
								$toportaladdress=trim($markup[4][1]["address"]);
								$toportallatE6=trim($markup[4][1]["latE6"]);
								$toportallngE6=trim($markup[4][1]["lngE6"]);
								$toportalid=trim($markup[4][1]["guid"]);
							}
						} else if ($actiontype=="DESTROYLINK") {
							if (trim($markup[2][0])=="PORTAL") {
								$portalname=trim($markup[2][1]["name"]);
								$portalplain=trim($markup[2][1]["plain"]);
								$portalteam=trim($markup[2][1]["team"]);
								if ($portalteam=="ENLIGHTENED") { $portalteam="ALIENS"; } 
								$portaladdress=trim($markup[2][1]["address"]);
								$portallatE6=trim($markup[2][1]["latE6"]);
								$portallngE6=trim($markup[2][1]["lngE6"]);
								$portalid=trim($markup[2][1]["guid"]);
							}
				
							if (trim($markup[4][0])=="PORTAL") {
								$toportalname=trim($markup[4][1]["name"]);
								$toportalplain=trim($markup[4][1]["plain"]);
								$toportalteam=trim($markup[4][1]["team"]);
								if ($portalteam=="ENLIGHTENED") { $portalteam="ALIENS"; } 
								$toportaladdress=trim($markup[4][1]["address"]);
								$toportallatE6=trim($markup[4][1]["latE6"]);
								$toportallngE6=trim($markup[4][1]["lngE6"]);
								$toportalid=trim($markup[4][1]["guid"]);
							}
						}
					}
				
					//print("Event ".$eventid.": ".$eventtext.".\n");
					print("Player ".date("d-M-Y H:i:s", substr($eventtimestamp, 0, strlen($eventtimestamp)-3))."/".$playerid."/".$playername."/".$playerteam);
					if ($portallngE6<$confix["minlngE6"] || $portallngE6>$config["maxlngE6"] || $portallatE6<$config["minlatE6"] || $portallatE6>$config["maxlatE6"]) {
						print("\nPortal out of bounds\n");
					} else {
						updatePlayer($playerid, $playername, $playerteam);
						if ($actiontype=="DEPLOY") {
							print(" deployed ".$actionlevel." resonator on ".$portalname."/".$portalplain."/".$portalteam."/".$portaladdress."/".$portalid);
							$action = findAction($eventid);
							if ($action===false) {
								if (updatePortal($portalid, $portalname, $portalplain, $portalteam, $portaladdress, $portallatE6, $portallngE6)===true) {
									addAction($eventid, $playerid, $portalid, $actiontype, $actionlevel, $eventtext, $eventtimestamp);
								} else {
									print("\nPortal out of bounds\n");
								}
							}
						} else if ($actiontype=="DESTROY") {
							print(" destroyed ".$actionlevel." resonator on ".$portalname."/".$portalplain."/".$portalteam."/".$portaladdress."/".$portalid);
							$action = findAction($eventid);
							if ($action===false) {
								if (updatePortal($portalid, $portalname, $portalplain, $portalteam, $portaladdress, $portallatE6, $portallngE6)===true) {
									addAction($eventid, $playerid, $portalid, $actiontype, $actionlevel, $eventtext, $eventtimestamp);
								} else {
									print("\nPortal out of bounds\n");
								}
							}
						} else if ($actiontype=="DESTROYMOD") {
							print(" destroyed mod on ".$portalname."/".$portalplain."/".$portalteam."/".$portaladdress."/".$portalid);
							$action = findAction($eventid);
							if ($action===false) {
								if (updatePortal($portalid, $portalname, $portalplain, $portalteam, $portaladdress, $portallatE6, $portallngE6)===true) {
									addAction($eventid, $playerid, $portalid, $actiontype, $actionlevel, $eventtext, $eventtimestamp);
								} else {
									print("\nPortal out of bounds\n");
								}
							}
						} else if ($actiontype=="CAPTURE") {
							print(" captured ".$actionlevel." resonator on ".$portalname."/".$portalplain."/".$portalteam."/".$portaladdress."/".$portalid);
							$capturetime = date("Y-m-d H:i:s", substr($eventtimestamp, 0, strlen($eventtimestamp)-3));
							$action = findAction($eventid);
							if ($action===false) {
								if (updatePortal($portalid, $portalname, $portalplain, $portalteam, $portaladdress, $portallatE6, $portallngE6, "", $playerid, $capturetime)===true) {
									deletePortalResonators($portalid);
									addAction($eventid, $playerid, $portalid, $actiontype, "", $eventtext, $eventtimestamp);
								} else {
									print("\nPortal out of bounds\n");
								}
							}
						} else if ($actiontype=="LINK") {
							print(" linked ".$portalname."/".$portalplain."/".$portalteam."/".$portaladdress."/".$portalid);
							$action = findAction($eventid);
							if ($action===false) {
								if (updatePortal($portalid, $portalname, $portalplain, $portalteam, $portaladdress, $portallatE6, $portallngE6)===true) {
									updatePortal($toportalid, $toportalname, $toportalplain, $toportalteam, $toportaladdress, $toportallatE6, $toportallngE6);
									addAction($eventid, $playerid, $portalid, $actiontype, "", $eventtext, $eventtimestamp, $toportalid);
								} else {
									print("\nPortal out of bounds\n");
								}
							}
						} else if ($actiontype=="DESTROYLINK") {
							print(" linked ".$portalname."/".$portalplain."/".$portalteam."/".$portaladdress."/".$portalid);
							$action = findAction($eventid);
							if ($action===false) {
								if (updatePortal($portalid, $portalname, $portalplain, $portalteam, $portaladdress, $portallatE6, $portallngE6)===true &&
									updatePortal($toportalid, $toportalname, $toportalplain, $toportalteam, $toportaladdress, $toportallatE6, $toportallngE6)===true) {
										addAction($eventid, $playerid, $portalid, $actiontype, "", $eventtext, $eventtimestamp, $toportalid);
								} else {
									print("\nPortal out of bounds\n");
								}
							}
						} else if ($actiontype=="FIELD") {
							print(" created a field on ".$portalname."/".$portalplain."/".$portalteam."/".$portaladdress."/".$portalid);
							$action = findAction($eventid);
							if ($action===false) {
								if (updatePortal($portalid, $portalname, $portalplain, $portalteam, $portaladdress, $portallatE6, $portallngE6)===true) {
									addAction($eventid, $playerid, $portalid, $actiontype, "", $eventtext, $eventtimestamp);
								} else {
									print("\nPortal out of bounds\n");
								}
							}
						} else if ($actiontype=="DESTROYFIELD") {
							print(" destroyed a field on ".$portalname."/".$portalplain."/".$portalteam."/".$portaladdress."/".$portalid);
							$action = findAction($eventid);
							if ($action===false) {
								if (updatePortal($portalid, $portalname, $portalplain, $portalteam, $portaladdress, $portallatE6, $portallngE6)===true) {
									addAction($eventid, $playerid, $portalid, $actiontype, "", $eventtext, $eventtimestamp);
								} else {
									print("\nPortal out of bounds\n");
								}
							}
						} else {
							print("Unknown frame:\n");
							print_r($event);
						}
					}
					print("\n\n");
				}
			}
	
			recordLastUpdate();
		} else {
			email(stripslashes($config["mailfrom"]), "Ingress ".getcwd()."/".$_SERVER["PHP_SELF"]." didn't load.");
		}
	} else {
		debug("Unable to connect to DB server");
		print("Unable to connect to DB server");
	}
}

function getLastUpdate() {
	global $config;
	
	$temp = '2013-01-11 00:00:00';
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select timestamp from lastupdate;";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			if($db->hasMoreElements()) {
				$temp=$db->nextElement();
				$temp = $temp["timestamp"];
			}
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function recordLastUpdate() {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "delete from lastupdate;";
		$db->setQuery($query);
		if ($db->executeQuery()) {
		} else {
			debug("Couldn't execute query");
		}
		$query = "insert into lastupdate(timestamp) values ('".date("Y-m-d H:i:s", time())."');";
		$db->setQuery($query);
		if ($db->executeQuery()) {
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function lastActions($team="", $player="", $howfarback=50, $portal="") {
	global $config;
	
	$temp = Array();
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$maxactions = 50000;
		if ($howfarback>$maxactions) {
			$maxactions = $howfarback*2;
		}
		$query = "select action.text, action.unixtimestamp, action.timestamp, player.name, player.team, player.level as playerlevel, player.guid as playerid, action.portalid as portalid, portal.name as portalname 
			from (select * from action order by timestamp desc limit ".$maxactions.") as action join player on action.playerid=player.guid join portal on action.portalid=portal.guid ";
		$s="";
		if ($team!="") {
			if ($s!="") {
				$s .= " and ";
			}
			$s .= " player.team='".addslashes($team)."'";
		}
		if ($player!="") {
			if ($s!="") {
				$s .= " and ";
			}
			$s .= " player.guid='".addslashes($player)."'";
		}
		if ($portal!="") {
			if ($s!="") {
				$s .= " and ";
			}
			$s .= " portal.guid='".addslashes($portal)."'";
		}
		if ($s!="") {
			$query .= " where ".$s;
		}
		$query .= "order by unixtimestamp desc limit ".$howfarback.";";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			while($db->hasMoreElements()) {
				$temp[]=$db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function lastCapturedPortal($team="") {
	global $config;

	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select 'portal' as type, portal.*, action.timestamp, action.action, player.guid as playerid, player.team from action join portal on action.portalid=portal.guid join player on action.playerid=player.guid where action.action='CAPTURE'";
		if ($team!="") {
			$query .= " and player.team='".addslashes($team)."'";
		}
		$query .= " order by timestamp desc limit 1;";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			while ($db->hasMoreElements()) {
				$temp=$db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}

		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function lastCapturedPortals($start, $end, $team="") {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select portal.*, (select sum(level/8)
				from portalresonator
				where portalresonator.portalid=portal.guid
				) as level, action.timestamp, action.unixtimestamp, action.action from action join portal on action.portalid=portal.guid where action.action='CAPTURE' and action.timestamp>='".addslashes($start)."' and action.timestamp<='".addslashes($end)."'";
		if ($team!="") {
			$query .= " and team='".addslashes($team)."'";
		}
		$query .= " order by unixtimestamp asc;";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			while ($db->hasMoreElements()) {
				$a = $db->nextElement();
				$temp[$a["guid"]]=$a;
			}
		} else {
			debug("Couldn't execute query");
		}

		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function getAllPortals($team="", $level="", $energy="", $postcodes=Array()) {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select portal.*, 'portal' as type"; 
		$query .= ", (select sum(level/8)
				from portalresonator
				where portalresonator.portalid=portal.guid
				) as level";
		$query .= ",  (select avg(case when portalresonator.level=1 then (portalresonator.evergytotal/1000*100) 
			when portalresonator.level=2 then (portalresonator.evergytotal/1500*100)  
			when portalresonator.level=3 then (portalresonator.evergytotal/2000*100)  
			when portalresonator.level=4 then (portalresonator.evergytotal/2500*100)  
			when portalresonator.level=5 then (portalresonator.evergytotal/3000*100)  
			when portalresonator.level=6 then (portalresonator.evergytotal/4000*100)  
			when portalresonator.level=7 then (portalresonator.evergytotal/5000*100)  
			when portalresonator.level=8 then (portalresonator.evergytotal/6000*100)  
			end)
			from portalresonator
			where portalresonator.portalid=portal.guid
			) as energy";
		$query .= " from portal where name!=''";
		$s = "";
		if ($team!="") {
			if ($s!="") {
				$s .= " and ";
			}
			$s .= " team='".addslashes($team)."'";
		}
		if ($level!="") {
			if ($s!="") {
				$s .= " and ";
			}
			$s .= " (select sum(level/8)
					from portalresonator
					where portalresonator.portalid=portal.guid
					)>=".$level;
		}
		if (is_array($postcodes) && count($postcodes)>0) {
			if ($s!="") {
				$s .= " and ";
			}
			$p = "";
			foreach($postcodes as $postcode) {
				if ($p!="") {
					$p .= " or ";
				}
				$p .= "portal.address like '%".$postcode."%'";
			}
			$s .= "(".$p.")" ;
		}
		if ($energy!="") {
			if ($s!="") {
				$s .= " and ";
			}
			$s .= " (select avg(case when portalresonator.level=1 then (portalresonator.evergytotal/1000*100) 
					when portalresonator.level=2 then (portalresonator.evergytotal/1500*100)  
					when portalresonator.level=3 then (portalresonator.evergytotal/2000*100)  
					when portalresonator.level=4 then (portalresonator.evergytotal/2500*100)  
					when portalresonator.level=5 then (portalresonator.evergytotal/3000*100)  
					when portalresonator.level=6 then (portalresonator.evergytotal/4000*100)  
					when portalresonator.level=7 then (portalresonator.evergytotal/5000*100)  
					when portalresonator.level=8 then (portalresonator.evergytotal/6000*100)  
					end)
				from portalresonator
				where portalresonator.portalid=portal.guid
				)<=".$energy;
		}
		if ($s!="") {
			$query .= " and ".$s;
		}
		$query .= ";";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			while ($db->hasMoreElements()) {
				$temp[]=$db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}

		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function deletePortals($portalids=array()) {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$s="";
		foreach($portalids as $portalid) {
			if ($s!="") {
				$s .= ", ";
			}
			$s .= "'".stripslashes($portalid)."'";
		}
		$query = "delete from portalresonator where portalid in (".$s.");";
		$db->setQuery($query);
		if ($db->executeQuery()) {
		} else {
			debug("Couldn't execute query");
		}
		$query = "delete from portalmod where portalid in (".$s.");";
		$db->setQuery($query);
		if ($db->executeQuery()) {
		} else {
			debug("Couldn't execute query");
		}
		$query = "delete from portal where guid in (".$s.");";
		$db->setQuery($query);
		if ($db->executeQuery()) {
		} else {
			debug("Couldn't execute query");
		}

		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function getAllPlayers($team="") {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select player.* from player";
		if ($team!="") {
			$query .= " where team='".addslashes($team)."'";
		}
		$query .= ";";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			while ($db->hasMoreElements()) {
				$temp[]=$db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}

		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function getPlayer($guid) {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select guid, case when name is null then guid else name end as name, team, email from player where guid='".addslashes($guid)."'";

		$query .= ";";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			while ($db->hasMoreElements()) {
				$temp=$db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}

		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function getPortal($guid) {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select portal.*, (select sum(level/8)
				from portalresonator
				where portalresonator.portalid=portal.guid
				) as level from portal where guid='".addslashes($guid)."'";
		$query .= ";";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			if ($db->hasMoreElements()) {
				$temp=$db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}

		$query = "select portalresonator.* from portalresonator where portalid='".addslashes($guid)."'";
		$query .= ";";
		$a = Array();
		$db->setQuery($query);
		if ($db->executeQuery()) {
			while ($db->hasMoreElements()) {
				$a[]=$db->nextElement();
			}
			$temp["resonators"] = $a;
		} else {
			debug("Couldn't execute query");
		}

		$query = "select portaledge.* from portaledge where startportalid='".addslashes($guid)."' or endportalid='".addslashes($guid)."'";
		$query .= ";";
		$a = Array();
		$db->setQuery($query);
		if ($db->executeQuery()) {
			while ($db->hasMoreElements()) {
				$a[]=$db->nextElement();
			}
			$temp["edges"] = $a;
		} else {
			debug("Couldn't execute query");
		}

		$query = "select portalmod.* from portalmod where portalid='".addslashes($guid)."'";
		$query .= ";";
		$a = Array();
		$db->setQuery($query);
		if ($db->executeQuery()) {
			while ($db->hasMoreElements()) {
				$a[]=$db->nextElement();
			}
			$temp["mods"] = $a;
		} else {
			debug("Couldn't execute query");
		}

		$query = "select portalfield.* from portalfield where portalAid='".addslashes($guid)."' or portalBid='".addslashes($guid)."' or portalCid='".addslashes($guid)."'";
		$query .= ";";
		$a = Array();
		$db->setQuery($query);
		if ($db->executeQuery()) {
			while ($db->hasMoreElements()) {
				$a[]=$db->nextElement();
			}
			$temp["fields"] = $a;
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function getPortalsForPlayer($playerid) {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select portal.*, (select sum(level/8)
				from portalresonator
				where portalresonator.portalid=portal.guid
				) as level from portal where playerid='".addslashes($playerid)."'";
		$query .= ";";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			while ($db->hasMoreElements()) {
				$temp[]=$db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}

		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function findPlayer($playerid) {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select * from player where guid='".addslashes($playerid)."';";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			if($db->hasMoreElements()) {
				$temp = $db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function findPlayerByName($playername) {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select * from player where name='".addslashes($playername)."';";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			if($db->hasMoreElements()) {
				$temp = $db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function updatePlayer($playerid, $playername, $playerteam="") {
	global $config;
	
	$player = findPlayer($playerid);
	if ($player["guid"]!="") {
		$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
		if ($db->dbID) {
			$query = "update player set name='".addslashes($playername)."' ";
			if ($team!="") {
				$query .= ", team='".addslashes($playerteam)."' ";
			}
			$query .= " where guid='".addslashes($playerid)."';";
			$db->setQuery($query);
			if ($db->executeQuery()) {
			} else {
				debug("Couldn't execute query");
			}
			$db->close();
		} else {
			debug("Unable to connect to DB server");
		
		}
	} else {
		addPlayer($playerid, $playername, $playerteam);
	}
}

function addPlayer($playerid, $playername, $playerteam) {
	global $config;
	
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "insert into player(guid, name, team) values ('".addslashes($playerid)."', '".addslashes($playername)."', '".addslashes($playerteam)."');";
		$db->setQuery($query);
		if ($db->executeQuery()) {
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
}

function findPortal($portalid) {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select * from portal where guid='".addslashes($portalid)."';";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			if($db->hasMoreElements()) {
				$temp = $db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function findPortalsOutside($minlatE6, $maxlatE6, $minlngE6, $maxlngE6) {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select * from portal where latE6<".$minlatE6." or latE6>".$maxlatE6." or lngE6<".$minlngE6." or lngE6>".$maxlngE6.";";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			while($db->hasMoreElements()) {
				$temp[] = $db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function updatePortal($portalid, $portalname, $portalplain, $portalteam, $portaladdress, $portallatE6, $portallngE6, $portalimage="", $playerid="", $capturedtime="") {
	global $config;
	
	$temp = false;
	if ($portallngE6>=$config["minlngE6"] && $portallngE6<=$config["maxlngE6"] && $portallatE6>=$config["minlatE6"] && $portallatE6<=$config["maxlatE6"]) {
		$portal = findPortal($portalid);
		if ($portal["guid"]!="") {
			$temp = true;
			$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
			if ($db->dbID) {
				$query = "update portal set lastupdate='".addslashes(date("Y-m-d H:i:s", time()))."', name='".addslashes($portalname)."', plain='".addslashes($portalplain)."', team='".addslashes($portalteam)."', address='".addslashes($portaladdress)."', latE6='".addslashes($portallatE6)."', lngE6='".addslashes($portallngE6)."'";
				if ($playerid!="") {
					$query .= ", playerid='".addslashes($playerid)."'";
				}
				if ($capturedtime!="") {
					$query .= ", capturedtime='".addslashes($capturedtime)."'";
				}
				if ($portalimage!="") {
					$query .= ", imageurl='".addslashes($portalimage)."'";
				}
				$query .= " where guid='".addslashes($portalid)."';";
				$db->setQuery($query);
				if ($db->executeQuery()) {
				} else {
					debug("Couldn't execute query");
				}
				$db->close();
			} else {
				debug("Unable to connect to DB server");
			
			}
		} else {
			$temp = addPortal($portalid, $portalname, $portalplain, $portalteam, $portaladdress, $portallatE6, $portallngE6, $portalimage, $playerid, $capturedtime);
		}
	} else {
		$temp = false;
	}
	return $temp;
}

function addPortal($portalid, $portalname, $portalplain, $portalteam, $portaladdress, $portallatE6, $portallngE6, $portalimage="", $playerid="", $capturedtime="") {
	global $config;
	
	$temp = false;
	if ($portallngE6>=$config["minlngE6"] && $portallngE6<=$config["maxlngE6"] && $portallatE6>=$config["minlatE6"] && $portallatE6<=$config["maxlatE6"]) {
		$temp = true;
		$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
		if ($db->dbID) {
			$query = "insert into portal(guid, lastupdate, name, plain, team, address, latE6, lngE6, imageurl, playerid, capturedtime) values ('".addslashes($portalid)."', '".addslashes(date("Y-m-d H:i:s", time()))."', '".addslashes($portalname)."', '".addslashes($portalplain)."', '".addslashes($portalteam)."', '".addslashes($portaladdress)."', '".addslashes($portallatE6)."', '".addslashes($portallngE6)."', '".addslashes($portalimage)."', '".addslashes($playerid)."', '".addslashes($capturedtime)."');";
			$db->setQuery($query);
			if ($db->executeQuery()) {
			} else {
				debug("Couldn't execute query");
			}
			$db->close();
		} else {
			debug("Unable to connect to DB server");
		
		}
	} else {
		$temp = false;
	}
	return $temp;
}

function findAction($eventid) {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select * from action where guid='".addslashes($eventid)."';";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			if($db->hasMoreElements()) {
				$temp = $db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function addAction($eventid, $playerid, $portalid, $actiontype, $actionlevel, $eventtext, $unixtimestamp, $toportalid="") {
	global $config;
	
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "insert into action(guid, timestamp, playerid, portalid, action, actionlevel, text, unixtimestamp, toportalid) values ('".addslashes($eventid)."', '".date("Y-m-d H:i:s", substr($unixtimestamp, 0, strlen($unixtimestamp)-3))."', '".addslashes($playerid)."', '".addslashes($portalid)."', '".addslashes($actiontype)."', '".addslashes($actionlevel)."', '".addslashes($eventtext)."', ".$unixtimestamp.",'".addslashes($toportalid)."');";
		$db->setQuery($query);
		if ($db->executeQuery()) {
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
}

function findActionsByPeriod($start, $end, $type) {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select player.team as team, player.name, player.guid as playerid, player.level as playerlevel, action.action, action.unixtimestamp, date(timestamp) as date, hour(timestamp) as hour, portal.name as portalname, portal.guid as portalid, action.text from action join player on action.playerid=player.guid join portal on action.portalid=portal.guid where timestamp>='".addslashes($start)."' and timestamp<='".addslashes($end)."'";
		$query .= " and action.action='".addslashes($type)."' order by unixtimestamp asc";
		$query .= ";";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			while($db->hasMoreElements()) {
				$temp[] = $db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function getNumberOfPortals($faction="") {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select guid from portal";
		if ($faction!="") {
			$query .= " where team='".$faction."'";
		}
		$query .= ";";
		$db->setQuery($query);
		if ($db->executeQuery()) {     
			while ($db->hasMoreElements()) {
				$temp[]=$db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function getNumberOfPlayers($faction="") {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select guid from player";
		if ($faction!="") {
			$query .= " where team='".$faction."'";
		}
		$query .= ";";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			while ($db->hasMoreElements()) {
				$temp[]=$db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function drawMap($data, $zoom="", $centerlat="", $centerlng="", $type="portal", $edges=Array()) {
	global $config;

	if ($centerlat=="") { $centerlat=$config["centerlat"]; }
	if ($centerlng=="") { $centerlng=$config["centerlng"]; }
	if ($zoom=="") { $zoom=$config["defaultzoom"]; }
	$output = file_get_contents("map.html");
	$output = str_replace("#ZOOM#", $zoom, $output);
	$output = str_replace("#CENTERLAT#", $centerlat, $output);
	$output = str_replace("#CENTERLNG#", $centerlng, $output);
	$output = str_replace("#TYPE#", $type, $output);
	$element = file_get_contents("element.html");
	
	$i = 0;
	$s = "";
	foreach($data as $row) {
		if (($row["latE6"]!="") && ($row["lngE6"]!="")) {
			if ($s!="") {
				$s .= ",";
			}
			$el = $element;
			$el = str_replace("#LAT#", ($row["latE6"]/1000000), $el);
			$el = str_replace("#LON#", ($row["lngE6"]/1000000), $el);
			$el = str_replace("#PORTALTEAM#", strtolower(str_replace("'", "\'", stripslashes($row["team"]))), $el);
			$plain = str_replace("'", "\'", stripslashes($row["plain"])." L".(stripslashes($row["level"])==""?"<=3":stripslashes($row["level"])));
			$plain = str_replace("\n", "", $plain);
			$plain = str_replace("\r", "", $plain);
			$el = str_replace("#PORTALPLAIN#", $plain, $el);
			$el = str_replace("#I#", $i, $el);
			$el = str_replace("#TYPE#", (stripslashes($row["type"])!=""? stripslashes($row["type"]) : $type), $el);
			$s .= $el;
			$i++;
		}
	}
	$output = str_replace("#ELEMENTS#", $s, $output);
	
	$element = file_get_contents("edgeelement.html");
	
	$i = 0;
	$s = "";
	foreach($edges as $row) {
		if (($row["p1latE6"]!="") && ($row["p1lngE6"]!="")) {
			if ($s!="") {
				$s .= ",";
			}
			$el = $element;
			$el = str_replace("#LAT1#", ($row["p1latE6"]/1000000), $el);
			$el = str_replace("#LON1#", ($row["p1lngE6"]/1000000), $el);
			$el = str_replace("#LAT2#", ($row["p2latE6"]/1000000), $el);
			$el = str_replace("#LON2#", ($row["p2lngE6"]/1000000), $el);
			$colour = "grey";
			if (strtolower($row["team"])=="resistance") {
				$colour = $config["resistancecolour"];
			} else if (strtolower($row["team"])=="aliens") {
				$colour = $config["alienscolour"];
			}
			$el = str_replace("#COLOUR#", $colour, $el);
			$s .= $el;
			$i++;
		}
	}
	$output = str_replace("#EDGEELEMENTS#", $s, $output);

	return $output;
}

function findResonator($resonatorid) {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select * from portalresonator where guid='".addslashes($resonatorid)."';";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			if($db->hasMoreElements()) {
				$temp = $db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function updateResonator($portalid, $resonatorid, $resonatorslot, $resonatorlevel, $resonatorenergytotal, $resonatordistancetoportal, $resonatorplayerid) {
	global $config;

	$portal = findResonator($resonatorid);
	if ($portal["guid"]!="") {
		$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
		if ($db->dbID) {
			$query = "update portalresonator set slot=".addslashes($resonatorslot).", 
					level=".addslashes($resonatorlevel).", 
					evergytotal=".addslashes($resonatorenergytotal).", 
					distancetoportal=".addslashes($resonatordistancetoportal).", 
					portalid='".addslashes($portalid)."',
					playerid='".addslashes($resonatorplayerid)."'
					where portalresonator.guid='".addslashes($portal["guid"])."';";
			$db->setQuery($query);
			if ($db->executeQuery()) {
			} else {
				debug("Couldn't execute query");
			}
			$db->close();
		} else {
			debug("Unable to connect to DB server");
		
		}
	} else {
		addResonator($portalid, $resonatorid, $resonatorslot, $resonatorlevel, $resonatorenergytotal, $resonatordistancetoportal, $resonatorplayerid);
	}
}

function addResonator($portalid, $resonatorid, $resonatorslot, $resonatorlevel, $resonatorenergytotal, $resonatordistancetoportal, $resonatorplayerid) {
	global $config;
	
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "insert into portalresonator(portalid, guid, slot, level, evergytotal, distancetoportal, playerid) 
			values ('".addslashes($portalid)."', '".addslashes($resonatorid)."', ".addslashes($resonatorslot).", ".addslashes($resonatorlevel).", ".addslashes($resonatorenergytotal).",
			".addslashes($resonatordistancetoportal).", '".addslashes($resonatorplayerid)."');";
		$db->setQuery($query);
		if ($db->executeQuery()) {
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
}

function deletePortalResonators($portalid) {
	global $config;
	
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "delete from portalresonator where portalid='".addslashes($portalid)."';";
		$db->setQuery($query);
		if ($db->executeQuery()) {
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
}

function findEdge($edgeid) {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select * from portaledge where guid='".addslashes($edgeid)."';";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			if($db->hasMoreElements()) {
				$temp = $db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function updateEdge($edgeid, $startportalid, $endportalid) {
	global $config;
	
	$edge = findEdge($edgeid);
	if ($edge["guid"]!="") {
		$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
		if ($db->dbID) {
			$query = "update portaledge set startportalid='".addslashes($startportalid)."', 
					endportalid='".addslashes($endportalid)."', 
					where portaledge.guid='".addslashes($edgeid)."';";
			$db->setQuery($query);
			if ($db->executeQuery()) {
			} else {
				debug("Couldn't execute query");
			}
			$db->close();
		} else {
			debug("Unable to connect to DB server");
		
		}
	} else {
		addEdge($edgeid, $startportalid, $endportalid);
	}
}

function addEdge($edgeid, $startportalid, $endportalid) {
	global $config;
	
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "insert into portaledge(guid, startportalid, endportalid) 
			values ('".addslashes($edgeid)."', '".addslashes($startportalid)."', '".addslashes($endportalid)."');";
		$db->setQuery($query);
		if ($db->executeQuery()) {
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
}

function deletePortalEdges($portalid) {
	global $config;
	
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "delete from portaledge where startportalid='".addslashes($portalid)."';";
		$db->setQuery($query);
		if ($db->executeQuery()) {
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
}

function getEdges($portalids) {
	global $config;
	
	$temp = Array();
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$ids = "";
		foreach($portalids as $id) {
			if ($ids!="") {
				$ids .= ",";
			}
			$ids .= "'".$id."'";
		}
		$query = "select *, p1.latE6 as p1latE6, p1.lngE6 as p1lngE6, p2.latE6 as p2latE6, p2.lngE6 as p2lngE6, p1.team as team 
			from portaledge
			join portal p1 on portaledge.startportalid=p1.guid
			join portal p2 on portaledge.endportalid=p2.guid
			where startportalid in (".$ids.") or endportalid in (".$ids.");";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			while($db->hasMoreElements()) {
				$temp[] = $db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}


function findPortalMod($portalmodid) {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select * from portalmod where guid='".addslashes($portalmodid)."';";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			if($db->hasMoreElements()) {
				$temp = $db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function updatePortalMod($portalmodid, $portalid, $type, $playerid, $display, $rarity, $stats) {
	global $config;
	
	$edge = findPortalMod($portalmodid);
	if ($edge["guid"]!="") {
		$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
		if ($db->dbID) {
			$query = "update portalmod set portalid='".addslashes($portalid)."', 
					type='".addslashes($type)."', 
					playerid='".addslashes($playerid)."', 
					display='".addslashes($display)."', 
					rarity='".addslashes($rarity)."', 
					stats='".addslashes($stats)."' 
					where portalmod.guid='".addslashes($portalmodid)."';";
			$db->setQuery($query);
			if ($db->executeQuery()) {
			} else {
				debug("Couldn't execute query");
			}
			$db->close();
		} else {
			debug("Unable to connect to DB server");
		
		}
	} else {
		addPortalMod($portalid, $type, $playerid, $display, $rarity, $stats);
	}
}

function addPortalMod($portalid, $type, $playerid, $display, $rarity, $stats) {
	global $config;
	
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "insert into portalmod(portalid, type, playerid, display, rarity, stats) 
			values ('".addslashes($portalid)."', '".addslashes($type)."', '".addslashes($playerid)."', '".addslashes($display)."', '".addslashes($rarity)."', '".addslashes($stats)."');";
		$db->setQuery($query);
		if ($db->executeQuery()) {
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
}

function deletePortalMods($portalid) {
	global $config;
	
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "delete from portalmod where portalid='".addslashes($portalid)."';";
		$db->setQuery($query);
		if ($db->executeQuery()) {
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
}

function getPlayerLevel($playerid="", $team="") {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select guid, case when name is null then guid else name end as name, team, email, level
			from player ";
		$s = "";
		if ($playerid!="") {
			if ($s!="") {
				$s .= " and ";
			}
			$s .= " player.guid='".addslashes($playerid)."'";
		}
		if ($team!="") {
			if ($s!="") {
				$s .= " and ";
			}
			$s .= " team='".addslashes($team)."'";
		}
		if ($s!="") {
			$query .= " and ".$s;
		}
		$query .= " order by player.name desc;";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			while ($db->hasMoreElements()) {
				$temp[]=$db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}

		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function updatePlayerLevels() {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "update player set level =
				GREATEST(COALESCE(player.level, 0), COALESCE((select max(right(actionlevel, 1)) from action where action.playerid=player.guid and action.actionlevel is not null and action.action='deploy'), 0), COALESCE((select max(level) from portalresonator where portalresonator.playerid=player.guid), 0))";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			while ($db->hasMoreElements()) {
				$temp[]=$db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}

		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function getPlayerStats($playerid="", $team="", $start="", $end="", $postcodes=Array()) {
	global $config;
	
	if ($start=="" ) {
		$start = "2012-01-01 00:00:00";
	}
	if ($end=="" ) {
		$end = "2030-01-01 00:00:00";
	}
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select guid, case when name is null then guid else name end as name, team, email, level,
			(select count(*) from action where action.playerid=player.guid and action.action='deploy' and timestamp>='".addslashes($start)."' and timestamp<='".addslashes($end)."') as resonatorsdeployed,
			(select count(*) from action where action.playerid=player.guid and action.action='destroy' and timestamp>='".addslashes($start)."' and timestamp<='".addslashes($end)."') as resonatorsdestroyed,
			(select count(*) from action where action.playerid=player.guid and action.action='capture' and timestamp>='".addslashes($start)."' and timestamp<='".addslashes($end)."') as portalscaptured,
			(select count(*) from action where action.playerid=player.guid and action.action='link' and timestamp>='".addslashes($start)."' and timestamp<='".addslashes($end)."') as linkscreated,
			(select count(*) from action where action.playerid=player.guid and action.action='destroylink' and timestamp>='".addslashes($start)."' and timestamp<='".addslashes($end)."') as linksdestroyed
			from player";
		$s = "";
		if ($playerid!="") {
			if ($s!="") {
				$s .= " and ";
			}
			$s .= " player.guid='".addslashes($playerid)."'";
		}
		if ($team!="") {
			if ($s!="") {
				$s .= " and ";
			}
			$s .= " team='".addslashes($team)."'";
		}
		if ($s!="") {
			$query .= " where ".$s;
		}
		$s = "";
		if (is_array($postcodes) && count($postcodes)>0) {
			$p = "";
			foreach($postcodes as $postcode) {
				if (trim($postcode)!="") {
					if ($p!="") {
						$p .= " or ";
					}
					$p .= "portal.address like '%".$postcode."%'";
				}
			}
			if ($p!="") {
				$s .= " and ";
				$s .= " (exists(select * from action join portal on action.portalid=portal.guid where action.playerid=player.guid and (".$p."))" ;
				$s .= " or exists(select * from portal join portalresonator on portal.guid=portalresonator.portalid where (portalresonator.playerid=player.guid) and (".$p.")))" ;
			}
		}
		if ($s!="") {
			$query .= $s;
		}
		$query .= " order by player.name asc;";
		$db->setQuery($query);
//		print($query);
		if ($db->executeQuery()) {
			while ($db->hasMoreElements()) {
				$temp[]=$db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}

		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function lastPlayersLocations($team="", $player="", $start="", $end="", $historical="") {
	global $config;
	
	if ($start=="") {
		$start = date("Y-m-d H:i:s", strtotime("-45 minutes"));
	}
	$temp = Array();
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select 'player' as type, portal.latE6, portal.lngE6, player.name as plain, player.team, player.level as level, action.unixtimestamp from action join portal on action.portalid=portal.guid join player on action.playerid=player.guid 
		left join action a on action.playerid=a.playerid and a.timestamp>='".$start."' ".($end!=""? "and a.timestamp<='".addslashes($end)."'" : "")." and strcmp(action.guid, a.guid)=-1 and a.action in ('DEPLOY', 'DESTROY', 'CAPTURE') 
		where action.action in ('DEPLOY', 'DESTROY', 'CAPTURE') and action.timestamp>='".$start."'";
		if ($end!="") {
			$query .= " and action.timestamp<='".addslashes($end)."'";
		}
		if ($team!="") {
			$query .= " and player.team='".addslashes($team)."'";
		}
		if ($player!="") {
			$query .= " and player.guid='".addslashes($player)."'";
		}
		if ($historical=="") {
			$query .= " and a.guid is null";
		}
		$query .= " order by unixtimestamp desc;"; 
		//print($query);
		$db->setQuery($query);
		if ($db->executeQuery()) {
			while($db->hasMoreElements()) {
				$temp[]=$db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function printAction($action) {
	global $config;
	
	$temp = "";
	
	$timestamp = stripslashes($action["unixtimestamp"]);
	$timestamp = substr($timestamp, 0, strlen($timestamp)-3);
	$text = stripslashes($action["text"]);
	$text = str_replace(stripslashes($action["name"]), "<a href=\"playerstats.php?type=player&playerid=".stripslashes($action["playerid"])."\">".stripslashes($action["name"])."</a> (".substr(stripslashes($action["team"]), 0, 1)." L".stripslashes($action["playerlevel"]).")", $text);
	$text = str_replace(stripslashes($action["portalname"]), "<a href=\"portaldetail.php?guid=".stripslashes($action["portalid"])."\">".stripslashes($action["portalname"])."</a>", $text);
	$colour = "black";
	if (ereg("destroy", $text)) {
		$colour = "#b30022";
	} else if (ereg("deploy", $text) || ereg("linked", $text) || ereg("create", $text)) {
		$colour = "#005f09";
	}
	$temp = "<tr><td bgcolor=\"".$config[strtolower(stripslashes($action["team"]))."colour"]."\"><font color=\"".$colour."\">".$text."<br>".date("d/M/Y H:i:s", $timestamp)."</font></td></tr>\n";
	return $temp;
}

function portalGeoMapping($slot) {
	$geo = Array("E", "NE", "N", "NW", "W", "SW", "S", "SE");
	return $geo[$slot];
}

function getPlayersbyGuids($guids = Array()) {
	global $config;
//{"guids":["6f0f277649404b40bcae1418a1c53703.c","e680ee85d56b465e91161bf4880dfcfa.c","d6760fb381af40ce80f105ab6519139d.c","d7de0034e899473394a54c45c7295c6f.c","70853d468a844b77928f9e0a0d0856b0.c","fbeda74dbad949e99e67a47d247163b2.c","23733ace9199434ca7377fcd82196553.c","cf6488c620ff414baaaf8a9a34c59567.c","33312884a586412b955a07450491b91a.c","7275792a478d4915b140878be964ec37.c"],"method":"dashboard.getPlayersByGuids"}

    $Url = "https://www.ingress.com/rpc/dashboard.getPlayersByGuids";
    if (!function_exists('curl_init')){
        die('Sorry cURL is not installed!');
    }
    
    $s = "";
    foreach($guids as $guid) {
    	    if ($s != "") {
    	    	    $s .= ",";
    	    }
    	    $s .= "\"".$guid."\"";
    }
    $data_string=" {\"guids\":[".$s."],\"method\":\"dashboard.getPlayersByGuids\"}";
    //print_r($data_string);
    
    $curl = curl_init();
    curl_setopt($curl, CURLOPT_URL, $Url);
    curl_setopt($curl, CURLOPT_REFERER, "https://www.ingress.com/intel");
    curl_setopt($curl, CURLOPT_USERAGENT, "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:17.0) Gecko/20100101 Firefox/17.0");
    curl_setopt($curl, CURLOPT_POST, 1);
    curl_setopt($curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_setopt($curl, CURLOPT_POSTFIELDS, $data_string);
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($curl, CURLOPT_BINARYTRANSFER, 0);
    curl_setopt($curl, CURLOPT_SSL_VERIFYPEER,false);
    curl_setopt($curl, CURLOPT_CAINFO,'cacert.pem');
    curl_setopt($curl, CURLOPT_HTTPHEADER, array('X-Requested-With: XMLHttpRequest', 'X-CSRFToken: '.$config["X-CSRFToken"], $config["cookiestring"],'Content-Type: application/json; charset=utf-8', 'Content-Length: ' . strlen($data_string)));
//print($config["cookiestring"]);
    $result = curl_exec($curl);
    $output = json_decode($result, true);
    curl_close($curl);
    
    return $output;	
}

function findField($fieldid) {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select * from portalfield where guid='".addslashes($fieldid)."';";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			if($db->hasMoreElements()) {
				$temp = $db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function updateField($fieldid, $portalAid, $portalBid, $portalCid, $score, $team, $playerid, $creationtime) {
	global $config;
	
	$temp = false;
	$field = findField($fieldid);
	if ($field["guid"]!="") {
		$temp = true;
		$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
		if ($db->dbID) {
			$query = "update portalfield set creationtime='".addslashes(date("Y-m-d H:i:s", $creationtime))."', portalAid='".addslashes($portalAid)."', portalBid='".addslashes($portalBid)."', portalCid='".addslashes($portalCid)."',
				score=".addslashes($score).",
				team='".addslashes($team)."', playerid='".addslashes($playerid)."'";
			$query .= " where guid='".addslashes($fieldid)."';";
			$db->setQuery($query);
			if ($db->executeQuery()) {
			} else {
				debug("Couldn't execute query");
			}
			$db->close();
		} else {
			debug("Unable to connect to DB server");
		
		}
	} else {
		$temp = addField($fieldid, $portalAid, $portalBid, $portalCid, $score, $team, $playerid, $creationtime);
	}
	return $temp;
}

function addField($fieldid, $portalAid, $portalBid, $portalCid, $score, $team, $playerid, $creationtime) {
	global $config;
	
	$temp = true;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "insert into portalfield(guid, portalAid, portalBid, portalCid, score, team, playerid, creationtime) values ('".addslashes($fieldid)."', '".addslashes($portalAid)."', '".addslashes($portalBid)."', '".addslashes($portalCid)."', ".addslashes($score).", '".addslashes($team)."', '".addslashes($playerid)."', '".addslashes(date("Y-m-d H:i:s", $creationtime))."');";
		$db->setQuery($query);
		if ($db->executeQuery()) {
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function getAllFields($team="") {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select portalfield.*";
		$query .= " from portalfield";
		$s = "";
		if ($team!="") {
			if ($s!="") {
				$s .= " and ";
			}
			$s .= " team='".addslashes($team)."'";
		}
		if ($s!="") {
			$query .= " where ".$s;
		}
		$query .= ";";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			while ($db->hasMoreElements()) {
				$temp[]=$db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}

		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function deleteFields($fieldids=array()) {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$s="";
		foreach($fieldids as $fieldid) {
			if ($s!="") {
				$s .= ", ";
			}
			$s .= "'".stripslashes($fieldid)."'";
		}
		$query = "delete from portalfield where guid in (".$s.");";
		$db->setQuery($query);
		if ($db->executeQuery()) {
		} else {
			debug("Couldn't execute query");
		}

		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function getFields($portalid) {
	global $config;
	
	$temp = Array();
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select * from portalfield where portalAid='".addslashes($portalid)."' or portalBid='".addslashes($portalid)."' portalCid='".addslashes($portalid)."';";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			while($db->hasMoreElements()) {
				$temp[] = $db->nextElement();
			}
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
	return $temp;
}

function deletePortalFields($portalid) {
	global $config;
	
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "delete from portalfield where portalAid='".addslashes($portalid)."' or portalBid='".addslashes($portalid)."' or portalCid='".addslashes($portalid)."';";
		$db->setQuery($query);
		if ($db->executeQuery()) {
		} else {
			debug("Couldn't execute query");
		}
		$db->close();
	} else {
		debug("Unable to connect to DB server");
	
	}
}

function getPlayerInventory() {
	global $config;

    $Url = "https://m-dot-betaspike.appspot.com/rpc/playerUndecorated/getInventory";
    if (!function_exists('curl_init')){
        die('Sorry cURL is not installed!');
    }
    //lastQueryTimestamp
    $s = "";
    foreach($guids as $guid) {
    	    if ($s != "") {
    	    	    $s .= ",";
    	    }
    	    $s .= "\"".$guid."\"";
    }
    $data_string="{\"params\":{\"lastQueryTimestamp\":0}}";
    //print_r($data_string);
    
    $curl = curl_init();
    curl_setopt($curl, CURLOPT_URL, $Url);
    curl_setopt($curl, CURLOPT_REFERER, "http://m-dot-betaspike.appspot.com");
    curl_setopt($curl, CURLOPT_USERAGENT, "Nemesis (gzip)");
    curl_setopt($curl, CURLOPT_POST, 1);
    curl_setopt($curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_setopt($curl, CURLOPT_POSTFIELDS, $data_string);
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($curl, CURLOPT_BINARYTRANSFER, 0);
    curl_setopt($curl, CURLOPT_SSL_VERIFYPEER,false);
    curl_setopt($curl, CURLOPT_CAINFO,'cacert.pem');
    curl_setopt($curl, CURLOPT_HTTPHEADER, array('X-XsrfToken: '.$config["X-CSRFToken"], 'Cookie: SACSID='.$config["SACSID"].';','Content-Type: application/json; charset=utf-8', 'Content-Length: ' . strlen($data_string)));

    $result = curl_exec($curl);
    $output = json_decode($result, true);
    curl_close($curl);
    
    return $output;	
}
?>
