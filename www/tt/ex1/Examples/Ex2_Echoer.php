<?php
/**
 * The Thread That Prints to file
 * @package Examples
 * @subpackage Example2
 */
require_once("../Handlers/Loader.php");
require_once("../Com/FileCom.php");
require_once("../Thread/Caller.php");
$sh=new FileCom($argv[2]);
$w=fopen("qqqC","w+");
$c=$argv[1];
$v=new TClient($c,$sh);

for($f=1;$f<=4;$f++)
{
	$v->TSleep(1);
	fwrite($w,$sh->Get("CNNT")."\n");
}
$v->TFinalize();
?>