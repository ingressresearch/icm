<?php

include_once("config.inc");

$EARTH_RADIUS=6378137;
$PI = 3.1415926535;
$DEG2RAD = $PI / 180;
$ingressURL = "https://www.ingress.com/rpc/dashboard.getThinnedEntitiesV4";

function makeIngressRequest($pileoftiles) {
	global $ingressURL;
	global $config;

	$data_string = "";
	foreach($pileoftiles as $tile) {
		if ($data_string!="") {
			$data_string .= ",";
		}
		$data_string .= makeIngressString($tile);
	}
	//$data_string = "{\"boundsParamsList\":[".$data_string."],\"zoom\":".$config["queryzoom"].",\"method\":\"dashboard.getThinnedEntitiesV4\"}";
	$data_string = "{\"method\":\"dashboard.getThinnedEntitiesV4\",\"boundsParamsList\":[".$data_string."]}";
	//"{\"boundsParamsList\":[{\"id\":\"".$tile["id"]."\",\"minLatE6\":".$tile["minLatE6"].",\"minLngE6\":".$tile["minLngE6"].",\"maxLatE6\":".$tile["maxLatE6"].",\"maxLngE6\":".$tile["maxLngE6"].",\"qk\":\"".$tile["qk"]."\"}],\"zoom\":".$config["queryzoom"].",\"method\":\"dashboard.getThinnedEntitiesV2\"}";
	//$data_string = "{\"method\":\"dashboard.getThinnedEntitiesV4\",\"boundsParamsList\":[{\"id\":\"9_1030_727\",\"qk\":\"9_1030_727\",\"minLatE6\":46073231,\"minLngE6\":1054688,\"maxLatE6\":46195042,\"maxLngE6\":1230469},{\"id\":\"9_1022_727\",\"qk\":\"9_1022_727\",\"minLatE6\":46073231,\"minLngE6\":-351562,\"maxLatE6\":46195042,\"maxLngE6\":-175781},{\"id\":\"9_1026_727\",\"qk\":\"9_1026_727\",\"minLatE6\":46073231,\"minLngE6\":351563,\"maxLatE6\":46195042,\"maxLngE6\":527344},{\"id\":\"9_1018_727\",\"qk\":\"9_1018_727\",\"minLatE6\":46073231,\"minLngE6\":-1054687,\"maxLatE6\":46195042,\"maxLngE6\":-878906},{\"id\":\"9_1021_728\",\"qk\":\"9_1021_728\",\"minLatE6\":45951150,\"minLngE6\":-527344,\"maxLatE6\":46073231,\"maxLngE6\":-351562},{\"id\":\"9_1024_729\",\"qk\":\"9_1024_729\",\"minLatE6\":45828799,\"minLngE6\":0,\"maxLatE6\":45951150,\"maxLngE6\":175781},{\"id\":\"9_1028_729\",\"qk\":\"9_1028_729\",\"minLatE6\":45828799,\"minLngE6\":703125,\"maxLatE6\":45951150,\"maxLngE6\":878906},{\"id\":\"9_1020_729\",\"qk\":\"9_1020_729\",\"minLatE6\":45828799,\"minLngE6\":-703125,\"maxLatE6\":45951150,\"maxLngE6\":-527344},{\"id\":\"9_1029_728\",\"qk\":\"9_1029_728\",\"minLatE6\":45951150,\"minLngE6\":878906,\"maxLatE6\":46073231,\"maxLngE6\":1054688},{\"id\":\"9_1017_728\",\"qk\":\"9_1017_728\",\"minLatE6\":45951150,\"minLngE6\":-1230469,\"maxLatE6\":46073231,\"maxLngE6\":-1054687},{\"id\":\"9_1025_728\",\"qk\":\"9_1025_728\",\"minLatE6\":45951150,\"minLngE6\":175781,\"maxLatE6\":46073231,\"maxLngE6\":351563},{\"id\":\"9_1034_727\",\"qk\":\"9_1034_727\",\"minLatE6\":46073231,\"minLngE6\":1757813,\"maxLatE6\":46195042,\"maxLngE6\":1933594},{\"id\":\"9_1032_729\",\"qk\":\"9_1032_729\",\"minLatE6\":45828799,\"minLngE6\":1406250,\"maxLatE6\":45951150,\"maxLngE6\":1582031},{\"id\":\"9_1031_726\",\"qk\":\"9_1031_726\",\"minLatE6\":46195042,\"minLngE6\":1230469,\"maxLatE6\":46316584,\"maxLngE6\":1406250},{\"id\":\"9_1033_728\",\"qk\":\"9_1033_728\",\"minLatE6\":45951150,\"minLngE6\":1582031,\"maxLatE6\":46073231,\"maxLngE6\":1757813},{\"id\":\"9_1016_729\",\"qk\":\"9_1016_729\",\"minLatE6\":45828799,\"minLngE6\":-1406250,\"maxLatE6\":45951150,\"maxLngE6\":-1230469},{\"id\":\"9_1035_726\",\"qk\":\"9_1035_726\",\"minLatE6\":46195042,\"minLngE6\":1933594,\"maxLatE6\":46316584,\"maxLngE6\":2109375},{\"id\":\"9_1027_726\",\"qk\":\"9_1027_726\",\"minLatE6\":46195042,\"minLngE6\":527344,\"maxLatE6\":46316584,\"maxLngE6\":703125}]}";

	print($data_string."\n");
	$curl = curl_init();
	curl_setopt($curl, CURLOPT_URL, $ingressURL);
	curl_setopt($curl, CURLOPT_REFERER, "https://www.ingress.com/intel");
	curl_setopt($curl, CURLOPT_ORIGIN, "https://www.ingress.com/intel");
	curl_setopt($curl, CURLOPT_USERAGENT, "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:17.0) Gecko/20100101 Firefox/17.0");
	curl_setopt($curl, CURLOPT_POST, 1);
	curl_setopt($curl, CURLOPT_CUSTOMREQUEST, "POST");
	curl_setopt($curl, CURLOPT_POSTFIELDS, $data_string);
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
	curl_setopt($curl, CURLOPT_BINARYTRANSFER, 0);
	curl_setopt($curl, CURLOPT_SSL_VERIFYPEER,false);
	curl_setopt($curl, CURLOPT_CAINFO,'cacert.pem');
	curl_setopt($curl, CURLOPT_HTTPHEADER, array('X-Requested-With: XMLHttpRequest', 'X-CSRFToken: '.$config["X-CSRFToken"], $config["cookiestring"],'Content-Type: application/json; charset=utf-8', 'Content-Length: ' . strlen($data_string)));
	print($config["cookiestring"]);
	$result = curl_exec($curl);
	$output = json_decode($result, true);
	curl_close($curl);
	
	return $output;
}

function makeIngressString($tile) {
	global $config;
	
	return "{\"id\":\"".$tile["id"]."\",\"minLatE6\":".$tile["minLatE6"].",\"minLngE6\":".$tile["minLngE6"].",\"maxLatE6\":".$tile["maxLatE6"].",\"maxLngE6\":".$tile["maxLngE6"].",\"qk\":\"".$tile["qk"]."\"}";
}

function queryIngress($tiles) {
	global $config;
	
	foreach($tiles as $tile) {
		print_r(makeIngressRequest($tile));
	}
}

function clampLat($lat) {
	if ($lat > 90.0) {
		$lat = 90.0;
	} else if ($lat < -90.0) {
		$lat = -90.0;
	}
	return $lat;
}

function clampLng($lng) {
	if ($lng > 180.0) {
		$lng = 180.0;
	} else if ($lng < -180.0) {
		$lng = -180.0;
	}
	return $lng;
}


function clampLatLng($latlng) {
	return Array( "lat"=>clampLat($latlng["lat"]), "lng"=>clampLng($latlng["lng"]) );
}

function clampLatLngBounds($bounds) {
  return Array ( "sw" => clampLatLng($bounds["sw"]), "ne" => clampLatLng($bounds["ne"]) );
}

function lngToTile($lng, $zoom) {
	return floor(($lng + 180) / 360 * pow(2, ($zoom>12)?$zoom:($zoom+2)));
}

function latToTile($lat, $zoom) {
	global $PI;
	
	return floor((1 - log(tan($lat * $PI / 180) + 1 / cos($lat * $PI / 180)) / $PI) / 2 * pow(2, ($zoom>12)?$zoom:($zoom+2)));
}

function tileToLng($x, $zoom) {
	return $x / pow(2, ($zoom>12)?$zoom:($zoom+2)) * 360 - 180;
}

function tileToLat($y, $zoom) {
	global $PI;
	
	$n = $PI - 2 * $PI * $y / pow(2,  ($zoom>12)?$zoom:($zoom+2));
	return 180 / $PI * atan(0.5 * (exp($n) - exp(-$n)));
}

function pointToTileId($zoom, $x, $y) {
	return $zoom . "_" . $x . "_" . $y;
}

// given tile id and bounds, returns the format as required by the
// Ingress API to request map data.
function generateBoundsParams($tile_id, $minLat, $minLng, $maxLat, $maxLng) {
	return Array (
		"id"=> $tile_id,
		"qk"=> $tile_id,
		"minLatE6"=> round($minLat * 1000000),
		"minLngE6"=> round($minLng * 1000000),
		"maxLatE6"=> round($maxLat * 1000000),
		"maxLngE6"=> round($maxLng * 1000000)
		);
}

function createIngressTiles($longlatcenter, $bounds) {
	global $config;

	$bounds = clampLatLngBounds($bounds);

	//we query the server as if the zoom level was this. it may not match the actual map zoom level
	$z = $config["queryzoom"];
	//print_r($bounds);
	$x1 = lngToTile($bounds["sw"]["lng"], $z);
	$x2 = lngToTile($bounds["ne"]["lng"], $z);
	$y1 = latToTile($bounds["ne"]["lat"], $z);
	$y2 = latToTile($bounds["sw"]["lat"], $z);
	//print("12=".$x1."/".$x2."/".$y1."/".$y2."\n");
	$tiles = Array();

	// y goes from left to right
	for ($y = $y1; $y <= $y2; $y++) {
		// x goes from bottom to top(?)
		for ($x = $x1; $x <= $x2; $x++) {
			//print($x."/".$y."/".$z."\n"); 
			$tile_id = pointToTileId($z, $x, $y);
			$latNorth = tileToLat($y,$z);
			$latSouth = tileToLat($y+1,$z);
			$lngWest = tileToLng($x,$z);
			$lngEast = tileToLng($x+1,$z);

			$boundsParam = generateBoundsParams(
				$tile_id,
				$latSouth,
				$lngWest,
				$latNorth,
				$lngEast
			);

			$tiles[] = $boundsParam;
		}
	}
	return $tiles;
}

?>
