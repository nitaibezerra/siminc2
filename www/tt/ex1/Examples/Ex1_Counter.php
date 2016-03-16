<?php
/**
 * The Thread that Counts
 * @package Examples
 * @subpackage Example1
 */
require_once("../Handlers/Loader.php");
require_once("../Com/ShmopCom.php");
require_once("../Thread/Caller.php");
$sh=new ShmopCom($_POST['xsh']);
$c=$_POST['ID'];
$v=new TClient($c,$sh);
for($i=1;$i<=4;$i++)
{
	$sh->Set("CNNT",$i);
	$v->TSleep(1);
}
$v->TFinalize();
?>