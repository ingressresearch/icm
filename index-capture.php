<?php // content="text/plain; charset=utf-8"
include_once("config.inc");
include_once("debug.inc");
include_once("myDB.class");
include_once("ingress.inc");
require_once("jpgraph/jpgraph.php");
require_once("jpgraph/jpgraph_line.php");

DEFINE("USE_CACHE",true);
DEFINE("READ_CACHE",true);
DEFINE("CACHE_DIR","./jpgraph_cache/");;

function fill0($string) {
	$temp = $string;
	if (strlen($temp)<2) {
		$temp = "0".$temp;
	}
	return $temp;
}

function cmp($a, $b) {
    if (strtotime($a) == strtotime($b)) {
        return 0;
    }
    return (strtotime($a) < strtotime($b)) ? -1 : 1;
}

$data_r = array();
$data_a = array();
$data_x = array();
//print(strtotime($_REQUEST["start"])."\n");
//print(strtotime($_REQUEST["end"])."\n"); die();
$i=strtotime($_REQUEST["start"]);
while($i<=strtotime($_REQUEST["end"])) {
	$data_x[date("Y-m-d H:00", $i)] = 0;
	$i=$i+(60*60);
}
$data_x = array_keys($data_x);

$data = findActionsByPeriod($_REQUEST["start"], $_REQUEST["end"], "CAPTURE");
foreach($data as $action) {
	if (stripslashes($action["team"])=="RESISTANCE") {
		$data_r[stripslashes($action["date"])." ".fill0(stripslashes($action["hour"])).":00"]++;
	} else if (stripslashes($action["team"])=="ALIENS") {
		$data_a[stripslashes($action["date"])." ".fill0(stripslashes($action["hour"])).":00"]++;
	}
}

foreach($data_x as $i) {
	if (!isset($data_r[$i])) {
		$data_r[$i] = 0;
	}
	if (!isset($data_a[$i])) {
		$data_a[$i] = 0;
	}
}

//$data_x = array_keys(array_merge($data_r, $data_a)); 
//usort($data_x, "cmp");

uksort($data_r, "cmp");
uksort($data_a, "cmp");

//print_r($data_r);
//print_r($data_a);
//die();

// Setup the graph
$graph = new Graph(800,450);
$graph->SetScale("textlin");
$graph->SetMargin(30,100,30,100);

//$theme_class=new UniversalTheme;

//$graph->SetTheme($theme_class);
$graph->img->SetAntiAliasing(false);
$graph->title->Set('Portails captures');
$graph->SetBox(false);
$graph->ygrid->SetFill(false);
$graph->SetBackgroundImage("technomage-white.jpg",BGIMG_FILLFRAME);

$graph->img->SetAntiAliasing();

$graph->yaxis->HideZeroLabel();
$graph->yaxis->HideLine(false);
$graph->yaxis->HideTicks(false,false);

$graph->xgrid->Show();
$graph->xgrid->SetLineStyle("solid");
$graph->xaxis->SetTickLabels($data_x);
$graph->xgrid->SetColor('#E3E3E3');
$graph->xaxis->SetLabelAngle(90);

// Create the first line
$p1 = new LinePlot(array_values($data_r));
$graph->Add($p1);
$p1->SetColor($config["resistancecolour"]);
$p1->SetLegend('Resistance');

// Create the second line
$p2 = new LinePlot(array_values($data_a));
$graph->Add($p2);
$p2->SetColor($config["alienscolour"]);
$p2->SetLegend('Illumines');

$graph->legend->SetFrameWeight(1);
$graph->legend->SetPos(0.01,0.5);
$graph->legend->SetColumns(1);

// Output line
$graph->Stroke();

?>