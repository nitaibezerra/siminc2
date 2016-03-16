<?php
// Gantt example
include( 'config.inc' );
include (APPRAIZ."/includes/jpgraph/jpgraph.php");
include (APPRAIZ."/includes/jpgraph/jpgraph_gantt.php");

// 
// The data for the graphs
//
$data = array(
	array( 0, ACTYPE_GROUP,  "Phase 1",        "2001-10-26","2001-11-23",''),
	array( 1, ACTYPE_NORMAL, "  Label 2",      "2001-10-26","2001-11-16",''),
	array( 2, ACTYPE_NORMAL, "  Label 3",      "2001-11-20","2001-11-22",''),
	array( 3, ACTYPE_GROUP,   "  testando",      "2001-11-20","2001-11-31",''),
	array( 4, ACTYPE_MILESTONE,"  Phase 1 Done", "2001-11-23",'M2', '' ),
	array( 5, ACTYPE_MILESTONE,"  Phase 2 Done", "2002-01-01",'M2', '' )
);

// The constrains between the activities
//$constrains = array(array(1,2,CONSTRAIN_ENDSTART),
//		    array(2,3,CONSTRAIN_STARTSTART));
$constrains = array();


// Create the basic graph
$graph = new GanttGraph();
$graph->SetDateRange( '2001-10-26', '2001-12-31' );

$graph->ShowHeaders( GANTT_HYEAR | GANTT_HMONTH | GANTT_HDAY | GANTT_HWEEK );
$graph->scale->week->SetStyle( WEEKSTYLE_FIRSTDAY );
$graph->CreateSimple( $data, $constrains, $progress );
$graph->Stroke();

?>


