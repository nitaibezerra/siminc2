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
$w=fopen("qqqC","w+");
$c=$_POST['ID'];
$v=new TClient($c,$sh);
for($f=1;$f<=4;$f++)
{
	$v->TSleep(1);
	sleep(1);
	fwrite($w,$sh->Get("CNNT")."\n");
	
}
$v->TFinalize();
?>