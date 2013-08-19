<?php

include_once("core.inc");

function getLastNotification() {
	global $config;
	
	$temp = '2013-01-11 00:00:00';
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select timestamp from lastnotification;";
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

function recordLastNotification() {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "delete from lastnotification;";
		$db->setQuery($query);
		if ($db->executeQuery()) {
		} else {
			debug("Couldn't execute query");
		}
		$query = "insert into lastnotification(timestamp) values ('".date("Y-m-d H:i:s", time())."');";
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

function getPortalMonitors($timestamp) {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select player.email as playeremail, player.name as playername, actionplayer.team as playerteam, portal.name as portalname, portal.address as portaladdress, action.text as actiontext, action.timestamp
				from action 
				join portalmonitors on action.portalid=portalmonitors.portalid
				join player on portalmonitors.playerid=player.guid 
				join player actionplayer on action.playerid=actionplayer.guid 
				join portal on portalmonitors.portalid=portal.guid 
				where portalmonitors.active='Y'
				and player.email!=''
				and (action.action='DESTROY' or action.action='CAPTURE' or action.action='DESTROYMOD')
				
				and action.timestamp>'".$timestamp."';";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			if($db->hasMoreElements()) {
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

function getPlayerPortalMonitors($playerid) {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select portalmonitors.guid, portalmonitors.portalid, portalmonitors.playerid, portal.name as portalname, portal.address as portaladdress, portal.plain as portalplain, portal.team as portalteam 
				from portalmonitors 
				join player on portalmonitors.playerid=player.guid 
				join portal on portalmonitors.portalid=portal.guid 
				where portalmonitors.active='Y'
				and player.email!=''
				and player.guid='".addslashes($playerid)."';";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			while ($db->hasMoreElements()) {
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

function getPlayerMonitors($timestamp) {
        global $config;

        $temp = false;
        $db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
        if ($db->dbID) {
                $query = "select player.email as playeremail, player.name as playername, actionplayer.team as playerteam, p2.name as monitoredname, action.text as actiontext, action.timestamp
                                from action
                                join playermonitors on action.playerid=playermonitors.monitoredplayerid
                                join player on playermonitors.playerid=player.guid
				join player actionplayer on action.playerid=actionplayer.guid 
                                join player p2 on playermonitors.monitoredplayerid=p2.guid
                                where playermonitors.active='Y'
                                and action.timestamp>'".$timestamp."';";
                $db->setQuery($query);
                if ($db->executeQuery()) {
                        if($db->hasMoreElements()) {
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

function getPortalMonitorKey($playerid, $portalid) {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select guid from portalmonitors where playerid='".addslashes($playerid)."' and portalid='".addslashes($portalid)."';";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			if($db->hasMoreElements()) {
				$temp = $db->nextElement();
				$temp = $temp["guid"];
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

function addPortalMonitor($playerid, $portalid, $email="") {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		if ($email!="") {
			$query = "update player set email='".addslashes($email)."' where guid='".addslashes($playerid)."';";
			$db->setQuery($query);
			if ($db->executeQuery()) {
			} else {
				debug("Couldn't execute query");
			}
		}
		
		$query = "insert into portalmonitors(playerid, portalid, active) values ('".addslashes($playerid)."', '".addslashes($portalid)."', 'Y');";
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

function removePortalMonitor($playerid, $portalid, $key) {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "delete from portalmonitors where playerid='".addslashes($playerid)."' and portalid='".addslashes($portalid)."' and guid='".($key/$config["keyfactor"])."';";
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

function getPlayerMonitorKey($playerid, $monitoredplayerid) {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "select guid from playermonitors where playerid='".addslashes($playerid)."' and monitoredplayerid='".addslashes($monitoredplayerid)."';";
		$db->setQuery($query);
		if ($db->executeQuery()) {
			if($db->hasMoreElements()) {
				$temp = $db->nextElement();
				$temp = $temp["guid"];
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

function addPlayerMonitor($playerid, $monitoredplayerid, $email="") {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		if ($email!="") {
			$query = "update player set email='".addslashes($email)."' where guid='".addslashes($playerid)."';";
			$db->setQuery($query);
			if ($db->executeQuery()) {
			} else {
				debug("Couldn't execute query");
			}
		}
		
		$query = "insert into playermonitors(playerid, monitoredplayerid, active) values ('".addslashes($playerid)."', '".addslashes($monitoredplayerid)."', 'Y');";
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

function removePlayerMonitor($playerid, $monitoredplayerid, $key) {
	global $config;
	
	$temp = false;
	$db =& new myDB($config["dbserver"], $config["dbuser"], $config["dbpassword"], $config["dbdatabase"]);
	if ($db->dbID) {
		$query = "delete from playermonitors where playerid='".addslashes($playerid)."' and monitoredplayerid='".addslashes($monitoredplayer)."' and key='".($key/$config["keyfactor"])."';";
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
?>
