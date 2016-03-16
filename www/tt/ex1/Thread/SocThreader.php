<?php
/**
 * This is the Part taking responsibility of Calling / Launching Threads
 * it has an abstreact class (must be extended) and some concrete classes 
 * implementing it to provide Calling Threads in several ways (Ajax , Sockets , Executing as Separate PHP/Cli Process) now
 * @package Thread_Calling_Handlers
 * @author Mohammed Yousef Bassyouni <harrrrpo@gmail.com>
 * @license http://opensource.org/licenses/gpl-license.php GNU Public License
 */
/**
 * require the Abstract Thread - Calling Class
 */
require Up(__FILE__,1)."Caller.php";
/**
 * The Socket Calling Class
 * This Class uses PHP Sockets to connect to server and launch a PHP Script/Thread
 * it Extends CallerBasic and implelemts it's Abstract functions
 * 
 * @package Thread_Calling_Handlers
 *
 */
class SocThread extends CallerBasic
{
	/**
	 * url of thread to be called
	 *
	 * @var string
	 */
	private $Host;
	/**
	 * Parameters to be passed to thread
	 *
	 * @var string
	 */
	public $params;
	/**
	 * SocThread Constructor
	 *
	 * @param string $url FULL Url to the Thread to be called
	 * @param string $params Thread Parameters
	 * @param Process $MothProcess The Process that Contains Threads
	 * @return ScoThread
	 */
	function __construct($url,$params,$MothProcess)
	{
		//Call The Parent Class Constructor
		CallerBasic::__construct($url,$params,$MothProcess);
		$this->Host=$url;
		$this->params=$params;
	}
	/**
	 * Socket Thread Calling Specific Launching Function
	 * 
	 * use Proper Http Headers to use with Sockets in order to make server Execute Script
	 *
	 */
	function Go()
	{
		$ar=Parse_url($this->Host);
		$HostQ=$ar['host'];
		$URI=$ar["path"];
		$ReqBody=$this->params;
		$ContentLength=strlen($ReqBody);
		$ReqHeader =
		"POST $URI HTTP/1.1\n".
		"Host: $HostQ\n".
		"Content-Type: application/x-www-form-urlencoded\n".
		"Content-Length: $ContentLength\n\n".
		"$ReqBody\n".
		"Connection: close\n";
		$socket = fsockopen($HostQ, 80, &$errno, &$errstr);
		fputs($socket, $ReqHeader);
		fclose($socket);
	}
	/**
	 * Socket Thread Calling Specific Parameters Setting Function
	 * 
	 * Direct Setting of Thread Params 
	 * 
	 * Note : Parameters Format are Like Passed through GET in urls
	 * e.g. "asd=1&qwe=2&rty=3" will be perceived by called thread
	 * $_POST['asd']=1 , $_POST['qwe']=2 , $_POST['rty']=3 
	 *
	 * @param string $Npr
	 */
	function SetParams($Npr)
	{
		$this->params=$Npr;
	}
}
?>