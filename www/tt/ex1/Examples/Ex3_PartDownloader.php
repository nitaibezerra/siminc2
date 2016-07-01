<?php  
/**
 * The Thread Downloads Parts of the file
 * @package Examples
 * @subpackage Example3
 */
require_once("../Handlers/Loader.php");
require_once("../Com/ShmopCom.php");
require_once("../Thread/Caller.php");

$fcom=new ShmopCom($_POST["UN"]);
$c=$_POST['Id'];
$v=new TClient($c,$fcom);
$url=trim($_POST["Fname"]);
$range=$_POST["Range"];
$wh = fopen($url, 'wb');
$ch = curl_init();
curl_setopt($ch, CURLOPT_URL,$url);
curl_setopt($ch, CURLOPT_VERBOSE, 1);
curl_setopt($ch, CURLOPT_POST, 0);
curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);
curl_setopt($ch,CURLOPT_RANGE,$range);
$file_target=$_POST["FFname"];
$wh = fopen($file_target, 'wb');
curl_setopt($ch, CURLOPT_FILE, $wh);
$dataq = curl_exec($ch);
curl_close ($ch);
fclose($wh);
?>