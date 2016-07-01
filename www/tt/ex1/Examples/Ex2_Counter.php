<?php
/**
 * The Thread that Counts
 * @package Examples
 * @subpackage Example2
 */
require_once("../Handlers/Loader.php");
require_once("../Com/FileCom.php");
require_once("../Thread/Caller.php");
$sh=new FileCom($argv[2]);
$c=$argv[1];
$v=new TClient($c,$sh);
for($i=1;$i<=4;$i++)
{
	$sh->Set("CNNT",$i);
	$v->TSleep(1);
}
$v->TFinalize();
?>