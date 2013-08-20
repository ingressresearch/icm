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
	$data_string = "{\"boundsParamsList\":[".$data_string."],\"method\":\"dashboard.getThinnedEntitiesV4\"}";
	//"{\"boundsParamsList\":[{\"id\":\"".$tile["id"]."\",\"minLatE6\":".$tile["minLatE6"].",\"minLngE6\":".$tile["minLngE6"].",\"maxLatE6\":".$tile["maxLatE6"].",\"maxLngE6\":".$tile["maxLngE6"].",\"qk\":\"".$tile["qk"]."\"}],\"zoom\":".$config["queryzoom"].",\"method\":\"dashboard.getThinnedEntitiesV2\"}";
	print($data_string."\n");
	$curl = curl_init();
	curl_setopt($curl, CURLOPT_URL, $ingressURL);
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

function convertCenterLat($centerLat) {
	global $DEG2RAD;
	
	return round(256 * 0.9999 * abs(1 / cos($centerLat * $DEG2RAD)));
}

function calculateR($convCenterLat) {
	global $config;
	
	return 1 << ($config["queryzoom"] - (($convCenterLat / 256) - 1));
}

function convertLatLngToPoint($latlng, $magic, $R) {
	global $DEG2RAD;
	global $PI;
	
	$x = ($magic + $latlng["lng"] * $magic / 180)*$R;
	$l = sin($latlng["lat"] * $DEG2RAD);
	$y =  ($magic + log((1+$l)/(1-$l)) * -($magic / (2*$PI)))*$R;
	return Array("x"=> floor($x/$magic), "y"=> floor($y/$magic));
}

function convertPointToLatLng($x, $y, $magic, $R) {
	global $PI;
	
	$e = Array();
	$e["sw"] = Array(
	// orig function put together from all over the place
	// lat: (2 * Math.atan(Math.exp((((y + 1) * magic / R) - (magic/ 2)) / (-1*(magic / (2 * Math.PI))))) - Math.PI / 2) / (Math.PI / 180),
	// shortened version by your favorite algebra program.
	"lat"=> (360*atan(exp($PI - $PI*($y+1)/$R)))/$PI - 90,
	"lng"=> 180*$x/$R-180
	);
	$e["ne"] = Array(
	//lat: (2 * Math.atan(Math.exp(((y * magic / R) - (magic/ 2)) / (-1*(magic / (2 * Math.PI))))) - Math.PI / 2) / (Math.PI / 180),
	"lat"=> (360*atan(exp($PI - $PI*$y/$R)))/$PI - 90,
	"lng"=> 180*($x+1)/$R-180
	);
	return $e;
}

// calculates the quad key for a given point. The point is not(!) in
// lat/lng format.
function pointToQuadKey($x, $y) {
	global $config;
	
	return $config["queryzoom"] . "_" . $x . "_" . $y;
}

function generateBoundsParams ($quadkey, $bounds) {
  return Array(
    "id"=> $quadkey,
    "qk"=> $quadkey,
    "minLatE6"=> round($bounds["sw"]["lat"] * 1000000),
    "minLngE6"=> round($bounds["sw"]["lng"] * 1000000),
    "maxLatE6"=> round($bounds["ne"]["lat"] * 1000000),
    "maxLngE6"=> round($bounds["ne"]["lng"] * 1000000)
  );
}

function createIngressTiles($longlatcenter, $bounds) {
	global $config;

	$magic = convertCenterLat($longlatcenter["lat"]);
	$R = calculateR($magic);

	// convert to point values
	$topRight = convertLatLngToPoint($bounds["ne"], $magic, $R);
	$bottomLeft = convertLatLngToPoint($bounds["sw"] , $magic, $R);
	// how many quadrants intersect the current view?
	$quadsX = abs($bottomLeft["x"] - $topRight["x"]);
	$quadsY = abs($bottomLeft["y"] - $topRight["y"]);
	
	// will group requests by second-last quad-key quadrant
	$tiles = Array();
	
	// walk in x-direction, starts right goes left
	for($i = 0; $i <= $quadsX; $i++) {
		$x = abs($topRight["x"] - $i);
		$qk = pointToQuadKey($x, $topRight["y"]);
		$bnds = convertPointToLatLng($x, $topRight["y"], $magic, $R);
		//if(!$tiles[$qk.slice(0, -1)]) $tiles[$qk.slice(0, -1)] = [];
		//$tiles[$qk.slice(0, -1)].push(generateBoundsParams($qk, $bnds));
		$tiles[] = generateBoundsParams($qk, $bnds);
		
		// walk in y-direction, starts top, goes down
		for($j = 1; $j <= $quadsY; $j++) {
			$qk = pointToQuadKey($x, $topRight["y"] + $j);
			$bnds = convertPointToLatLng($x, $topRight["y"] + $j, $magic, $R);
			//if(!$tiles[$qk.slice(0, -1)]) $tiles[$qk.slice(0, -1)] = [];
			//$tiles[$qk.slice(0, -1)].push(generateBoundsParams($qk, $bnds));
			$tiles[] = generateBoundsParams($qk, $bnds);
		}
	}
	return $tiles;
}

?>
