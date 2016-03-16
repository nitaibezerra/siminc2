<script src="<?php echo Up($_SERVER['PHP_SELF'],2);?>AjT.js"></script>
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
 * The Ajax Thread Calling Class
 * it uses Ajax in Client Side to call threads
 * it has the benefit of no dependence on PHP Sockets Support on server
 * and that threads can print to browser directly
 * Problems are when user Disables JS or Limited Number of Connections 
 * issue (as RFC sets them to 2 in Browser side)
 * 
 * @package Thread_Calling_Handlers
 *
 */
class AjThread extends CallerBasic
{
	/**
	 * Ajax Class Constructor
	 *
	 * @param string $url PATH to the thread
	 * @param string $params Thread Parameters
	 * @param string $MothProcess The Process that Contains Threads
	 */
	function __construct($url,$params,$MothProcess)
	{
		//Call The Parent Class Constructor
        CallerBasic::__construct($url,$params,$MothProcess);
		//creating a new instance of the Js class with the index of $num (Refer to Js file for arguments)
		echo "<script language=\"javascript\">";
		echo "t[$this->pos]=new Threader(\"$url\",$this->pos,\"$params\");";
		echo "</script>";
	}
	/**
	 * Ajax-Thread Calling Specific Parameters Setting
	 * 
	 * Direct Setting of Thread Params through sending JS Code to browser to change Params
	 * 
	 * Note : Parameters Format are Like Passed through GET in urls
	 * e.g. "asd=1&qwe=2&rty=3" will be perceived by called thread
	 * $_POST['asd']=1 , $_POST['qwe']=2 , $_POST['rty']=3  
	 *
	 * @param string $Npr
	 */
	function SetParams($Npr)
	{
		echo "<script language=\"javascript\">";
		echo "t[$this->pos].params=\"$Npr\";";
		echo "</script>";
	}
	/**
	 * Ajax-Thread Calling Specific Thread Launching function
	 * 
	 * sending JS Code to browser to start Thread (Fire Ajax Request) and use flush() 
	 * for instant execution (Empty buffer)
	 * 
	 * Note : Parameters Format are Like Passed through GET in urls
	 * e.g. "asd=1&qwe=2&rty=3" will be perceived by called thread
	 * $_POST['asd']=1 , $_POST['qwe']=2 , $_POST['rty']=3 
	 *
	 */
	function Go()
	{
		echo "<script language=\"javascript\">";
		echo "t[$this->pos].begin();";
		echo "</script>";
		flush();
	}
}
?>