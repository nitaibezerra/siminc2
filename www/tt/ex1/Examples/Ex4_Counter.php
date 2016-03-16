<?php
/**
 * The Thread that Counts
 * @package Examples
 * @subpackage Example4
 */
require_once("../Handlers/Loader.php");
require_once("../Com/MyCom.php");
require_once("../Thread/Caller.php");
$sh=new MyCom("","root","","webct",$_POST["tb"]);
$c=$_POST["ID"];
$v=new TClient($c,$sh);
for($i=1;$i<=4;$i++)
{
	$sh->Set("CNNT",$i);
	$v->TSleep(1);
}
$v->TFinalize();
?>