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
 * Instances of this class are used as Containers for threads (hence name Process :) )
 * upon creating of a new Process with given Communication Class it Calls it's Init Method
 * @package Thread_Calling_Handlers
 *
 */
class Process
{
	/**
	 * Communication Class Provided for threads of this Process
	 *
	 * @var ComBasic
	 */
	public $ComHelper;
	/**
	 * Class Constuctor 
	 * just assign the $ComH varaible and Init it
	 *
	 * @param Process $ComH
	 */
	function __construct($ComH)
	{
		$this->ComHelper=$ComH;
		$this->ComHelper->Init();
	}
	/**
	 * Clear the Any Equipation of the Communication Class 
	 *
	 */
	function Clear()
	{
		$this->ComHelper->Clear();
	}
}
/**
 * This is an Abstract class representing Methods all Thread-Calling classes must implement
 * it also has some Concrete methods
 * @abstract 
 * @package Thread_Calling_Handlers
 */
abstract class CallerBasic
{
	/**
	 * used for Calculating Thread id
	 *
	 * @var int
	 */
	protected  $pos;
	/**
	 * An Instance of Process class which is the Container of this Thread
	 *
	 * @var Process
	 */
	private  $MotherProcess;
	/**
	 * CallerBasic Constructor , it must be explicily called from clasese extending it (PHP OOP issue)
	 * 
	 * Preparing Counter for generating ID's , and Setting Thread state
	 *
	 * @param string $url Location of Thread to be called , it's format is Threading-class dependent
	 * @param string $params Parameters to be passed to the called thread
	 * @param Process $MothProcess Process to which thread will belong
	 */
	function __construct($url,$params,$MothProcess)
	{
		static $num=-1;
		$this->MotherProcess=$MothProcess;
		//Generating unique id
		$num++;
		$this->pos=$num;
		//Set the State of thread to be working
		
		//print Template::StateName($this->MotherProcess->ComHelper,$this->GetID()); 
		$this->MotherProcess->ComHelper->Set(Template::StateName($this->GetID()),"Working");
	}
	/**
	 * Associates / Sets Name $name with the Value $value
	 * it can be used to create a Name or reeassign it (change it's value )
	 * 
	 * This function Just wrapps the Communication Class Setting function
	 *
	 * @param string $name
	 * @param string $value
	 */
	function Set($name,$value)
	{
		$this->MotherProcess->ComHelper->Set($name,$value);
	}
	/**
	 * takes the name of a Communication Varaible and returns it's value
	 *
	 * @param string $name
	 * @return string
	 */
	function Get($name)
	{
		return  $this->MotherProcess->ComHelper->Get($name);
	}
	/**
	 * Return the thread ID 
	 *
	 * @return int
	 */
	function GetID()
	{
		return $this->pos;
	}
	/**
	 * return Thread State as read from Communication Class Varaible
	 *
	 * @return string
	 */
	function GetState()
	{
		return trim($this->MotherProcess->ComHelper->Get(Template::StateName($this->GetID())));	
	}
	/**
	 * This is a void function with no arguments
	 * it's used to Launch / Call the Thread
	 * @abstract 
	 *
	 */
	abstract public function Go();
	/**
	 * A void function with one argument
	 * 
	 * This Function is used to Change the Thread Arguments before Calling/Launching it
	 *
	 * @param string $Npr
	 */
	abstract public function SetParams($Npr);
	
	/**
	 * This is a Static Function  used to Join Another Thread/s (Wait for them to finish)
	 * These Threads MUST give a sign they finished (using TClien Class)
	 * 
	 *
	 * @param Array $ArrIds The Array of Thread ID's to be joined
	 * @param ComBasic $ComH
	 */
	static function Join($ArrIds,$ComH)
	{
		while(1)
		{
			$b=true;
			for ($i=0;$i<count($ArrIds);$i++)
			{
				// Make Sure all threads finished
				$b=$b && (trim($ComH->Get(Template::StateName($i)))=="Dead");
			}
			//Done..
			if ($b==true)return;
			//Wait for 1 Sec and Check Again
			sleep(1);
		}
	}
}
/**
 * This Class is Instaitained in Called Threads 
 * To provide Easy State Expressing / Watching Functionality
 * @package Thread_Calling_Handlers
 *
 */
class TClient
{
	/**
	 * Communication Class used by thread
	 *
	 * @var ComBasic
	 */
	private $ComHelper;
	/**
	 * Thread ID
	 *
	 * @var int
	 */
	public  $id;
	/**
	 * Class Constuctor
	 * Just Assign Instance attributes to this Thread Specific ones
	 *
	 * @param int $Id
	 * @param ComBasic $ComH
	 */
	function __construct($Id,$ComH)
	{
		$this->id=$Id;
		$this->ComHelper=$ComH;
	}
	/**
	 * Puts Thread in Sleep State for $sec Fixed Seconds
	 *
	 * @param int $sec
	 */
	function TSleep($sec)
	{
		//Express The Sleep State of Thread
		$this->ComHelper->Set(Template::StateName($this->id),"Sleep");
		//Really Sleep
		sleep($sec);
		//Express you Awake
		$this->ComHelper->Set(Template::StateName($this->id),"Working");
	}
	/**
	 * Express Thread has finished and it's "Dead"
	 *
	 */
	function TFinalize()
	{
		$this->__destruct();
	}
	/**
	 * Return This Thread state
	 *
	 * @return unknown
	 */
	function TGetState()
	{
		return $this->ComHelper->Get(Template::StateName($this->id));
	}
	/**
	 * Destructor
	 * This Thread has Died
	 *
	 */
	function __destruct()
	{
		$this->ComHelper->Set(Template::StateName($this->id),"Dead");
	}
}
/**
 * This class is used for Templating static names used by other classes
 * @todo add States "Sleep" , "Dead" , "Working" to class to unify them
 * @package Thread_Calling_Handlers
 */
class Template
{
	/**
	 * Return String Representing Thread State Attribute
	 *
	 * @param int $id Thread ID
	 * @return string
	 */
	static function StateName($id)
	{ 
			return "Thread_".$id."_State";
	}
}
?>