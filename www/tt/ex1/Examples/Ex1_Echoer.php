<?php
/**
 * The Thread That Prints to file
 * @package Examples
 * @subpackage Example1
 */
require_once("../Handlers/Loader.php");
require_once("../Com/ShmopCom.php");
require_once("../Thread/Caller.php");
$sh=new ShmopCom($_POST['xsh']);
$w=fopen("qqqC","w+");
$c=$_POST['ID'];
$v=new TClient($c,$sh);
for($f=1;$f<=4;$f++)
{
	$v->TSleep(1);
	fwrite($w,$sh->Get("CNNT")."\n");
}
$v->TFinalize();
?>