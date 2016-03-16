<?php
/**
 * This Example Demostrates using Shared Memory Communication with Socket Thread Calling
 * 
 * it's a very simple Download Manager (call it PDM) what it basicly do  is getting url  from user
 * and number of connections is definde in file then it launches a number of threads of each part
 * of file and watches thier progress till they finish
 * 
 * it Consists of the Files:-
 * 
 * 1)Ex3_MainThread.php : The Main thread that Getts input , Launch other threads & watch them
 * 2)Ex3_PartDownloader.php    : The Thread Retrieves part of file
 * 
 * @package Examples
 * @subpackage Example3
 * @author Mohammed Yousef Bassyouni <harrrrpo@gmail.com>
 * @license http://opensource.org/licenses/gpl-license.php GNU Public License
 * @todo Fully Document the example
 */require_once("../Handlers/Loader.php");
require_once("../Thread/SocThreader.php");
require_once("../Com/ShmopCom.php");
ini_set('max_input_time','0');
ini_set('max_execution_time','0');
ini_set('max_input_time','0');
//the file to be downloaded url NOTE IT MUST BE RESUMABLE (i.e. allows partial GET header)
$url="http://localhost/IPB.zip";
//number of connections
$num=5;
$ss=remote_file_size($url);
$part=floor($ss/$num);
echo "<html><body><form name=\"asd\">";
for($i=0;$i<$num;$i++)
{
	echo "Part$i <input type=\"text\" name=\"a$i\"><br>";
}
echo "</form></body>";flush();
$i=0;
$scom=new ShmopCom();
$P=new Process($scom);
for($i=0;$i<$num-1;$i++)
{
	//initialize threads
	$e[$i]=new SocThread("http://localhost/Threader_Dev/Examples/Ex3_PartDownloader.php","Fname=$url&FFname=asd$i.asd&UN=$scom->MemKey&Id=$i&Range=".($i*$part)."-".(($i+1)*$part-1),$P);
	$e[$i]->Go();
}
$e[$num-1]=new SocThread("http://localhost/Threader_Dev/Examples/Ex3_PartDownloader.php","Fname=$url&FFname=asd$i.asd&UN=$scom->MemKey&Id=".($num-1)."&Range=".(($num-1)*$part)."-".(1*$ss),$P);
$e[$num-1]->Go();
//monitor the status of downloading
echo $part."<br>";
while (1)
{
	sleep(1);
	echo "<script language=\"javascript\">";
	for($i=0;$i<$num;$i++)
	{
		echo "document.asd.a$i.value="."\"".(round(remote_file_size("http://localhost/Threader_Dev/Examples/asd$i.asd")/$part*100,2))."%\";";
	}
	echo "</script>";
	if(all($e,$num,$scom)==1)break;
	flush();
}
$ww=fopen("asd.exe","wb");
fclose($ww);
$ww=fopen("asd.exe",'a');
for($i=0;$i<$num;$i++)
{
	$wh=fopen("asd$i.asd",'r');
	fwrite($ww,fread($wh,filesize("asd$i.asd")));
	fclose($wh);
	unlink("asd$i.asd");
}
$scom->Clear();
function all($e,$num,$scom)
{
	for($i=0;$i<$num;$i++)
	{
		if($scom->Get(Template::StateName($e[$i]->GetID()))!="Dead")
		{
			return 0;
		}
	}
	return 1;
}

//a big function but  yet very helpfull to get file size as filesize function doesnot work properly
function remote_file_size ($url)
{
	$head = "";
	$url_p = parse_url($url);
	$host = $url_p["host"];
	$path = $url_p["path"];

	$fp = fsockopen($host, 80, $errno, $errstr, 20);
	if(!$fp)
	{ return false; }
	else
	{
		fputs($fp, "HEAD ".$url." HTTP/1.1\r\n");
		fputs($fp, "HOST: dummy\r\n");
		fputs($fp, "Connection: close\r\n\r\n");
		$headers = "";
		while (!feof($fp)) {
			$headers .= fgets ($fp, 128);
		}
	}
	fclose ($fp);
	$return = false;
	$arr_headers = explode("\n", $headers);
	foreach($arr_headers as $header) {
		$s = "Content-Length: ";
		if(substr(strtolower ($header), 0, strlen($s)) == strtolower($s)) {
			$return = substr($header, strlen($s));
			break;
		}
	}
	return $return;
}
?>