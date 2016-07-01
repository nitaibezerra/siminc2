<?php
include ('config.inc');
include (APPRAIZ."includes/jpgraph1/jpgraph.php");
include (APPRAIZ."includes/jpgraph1/jpgraph_pie.php");

$data = array(40,60,21,33);

$graph = new PieGraph(300,200,"auto");
$graph->SetShadow();

$graph->title->Set("A simple Pie plot");

$p1 = new PiePlot($data);
$graph->Add($p1);
$graph->Stroke();

?>


