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
 * Cli Thread Calling Class
 * This Class uses OS Specific calls to launch a Separate PHP/Cli Process to execute Thread
 * Sure it's heavier that other Ways but Supports both PHP/Cli and PHP as part of web server
 *  (in any way CGI , Module ..etc )
 * 
 * IMPORTANT NOTE : in order for this class to work :-
 *         On Windows : The *.php class must be tied to your php.exe 
 *	       On *nix    : The Script must be executable by adding Execution path on top of top
 * 
 * @package Thread_Calling_Handlers
 */
class CliThread extends CallerBasic
{
	/**
	 * Thread/Script Path
	 *
	 * @var string
	 */
	private $file;
	/**
	 * Parameters to be passed
	 *
	 * @var string
	 */
	public $params;
	/**
	 * Cli-Thread Class Constuctor
	 * Just Pick Arguments and tidy them
	 *
	 * @param string $url
	 * @param string $params
	 * @param Process $MothProcess
	 */
	function __construct($url,$params,$MothProcess)
	{
		//Call The Parent Class Constructor
        CallerBasic::__construct($url,$params,$MothProcess);
		
		$this->Host=$url;
		$this->params=$params;
		/* 
		Since Arguments allowed to be passed like urls (*nix like)
		if we are in Windows we must tidy them 
		*/
		if ($this->IsWindows())
		{
			$url=str_replace("/","\\",$url);
			$url=($url[0]=="\\"?substr($url,1):$url); 
		}
		$this->file=$url;
	}
	/**
	 * Cli-Thread Calling Specific Launching Function
	 * 
	 * Launch Threads and them thier arguments in OS dependent manner
	 *
	 * @param int $Method has effect on Windows only whether to use Shell Scirpt or Pipes
	 */
	function Go($Method=1)
	{
		if ($this->IsWindows())
		{
			if ($Method==2)
			{
				$WshShell = new COM("WScript.Shell");
				echo $this->file." ".$this->params;
				$oExec = $WshShell->Run($this->file." ".$this->params, 0, false);				
			}
			elseif ($Method==1)
			{
				pclose(popen("start \"bla\" \"" . $this->file . "\" " . $this->params, "r"));
			}
		}
		else 
		{
			exec("./" . $this->file . " " . $this->params . " > /dev/null &"); 
		}
	}
	/**
	 * Cli-Thread Calling Specific Parameters Setting Function
	 * 
	 * Direct Setting of Thread Params 
	 * 
	 * Note : The format of Parameters here is like Cli Programs arguments passing
	 * e.g. "asd qwe rty" Can be Percived at Called Threads as
	 * $argv[1]="asd" , $argv[1]="qwe" , $argv[1]="rty"
	 *
	 * @param string $Npr
	 */
	function SetParams($Npr)
	{
		$this->params=$Npr;
	}
	/**
	 * Detect Using Windows
	 *
	 * @return Boolean
	 */
	function IsWindows()
	{
		return strstr(php_uname(),"Windows")!=false;
	}
}
?>