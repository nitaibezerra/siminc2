<?php
/**
 * This is the Part taking responsibility of Communication between threads
 * it has an abstreact class (must be extended) and some concrete classes 
 * implementing it to provide communication in several ways (Shared Memory , Mysql , flat files) now
 * @package Communication_Handlers
 * @author Mohammed Yousef Bassyouni <harrrrpo@gmail.com>
 * @license http://opensource.org/licenses/gpl-license.php GNU Public License
 */
/**
 * require the Abstract Communication Class
 */
require(Up(__FILE__,1)."Com.php");
/**
 * require the Choosen Sync. class
 */
require(Up(__FILE__,2)."Sync/SimpleMutex.php");
/**
 * require the Writting Methodology Class
 */
require(Up(__FILE__,2)."Handlers/PlainArray.php");
/**
 * The Shared Memory Communication Class
 * it extends the ComBasic Class and Implement it's Abstract Methods
 * @package Communication_Handlers
 */
class ShmopCom extends ComBasic
{
	/**
	 * Shared Memory Block Permissions
	 *
	 * @var int
	 */
	public  $perms=0777;
	/**
	 * Shared Memory block id (shmid)
	 *
	 * @var int
	 */
	public  $id;
	/**
	 * Shared Memory block Key
	 *
	 * @var int
	 */
	public  $MemKey;
	/**
	 * an Instance of the Sync Class
	 *
	 * @var SimpleMutex
	 * @todo Add an abstract Sync Class of which type the var will be
	 */
	public  $MuxH;
	/**
	 * an Instance of the Writting Methodology Class
	 *
	 * @var PlainArrayHandler
	 */
	public  $RecHandler;
	/**
	 * The Default size of the Shared memory block
	 *
	 * @var int
	 */
	public  $size=1000;
	/**
	 * The Shared Memory Communication Class Constructor
	 * 
	 * This class is called in two manners :-
	 * 1) Intializing Communication from parent thread here it's called like : $asd=new ShmopCom()
	 * 	  and Block size can be changed latter BUT before Init is called by Process (Which is very 
	 *    important according to your needs as it's Constant ) 
	 * 
	 * 2) From a called thread to start Communication with other threads ( in same process ) :$asd=new ShmopCom($Shn)
	 * 	  where $Shn is the Shared Memory block id which must be passed from mother thread to called threads
	 *
	 * @param string $Shn
	 * @return ShmopCom
	 */
	function ShmopCom($Shn=0)
	{
		// if this is called from mother thread leave
		if ($Shn==0)return;
		/*
		 here we are called from another thread that wants to communicate
		 hence memory block already exists and perms and size HAS NO affect
		*/
		$this->id=shmop_open($Shn, "c", $this->perms, 100);
		// Assign a Writting Methodolgy Class For Called Threads
		$this->RecHandler=new PlainArrayHandler($this);
		// Assign a Sync. Control Class For Called Threads
		$this->MuxH=new SimpleMutex($this);
	}
	/**
	 * Shared Memory Communication Specific Init Function
	 * 
	 * (For pupose of this function review ComBasic abstract class documentation)
	 * 
	 * For the ShmopCom class the role of this function is to create the shared memory block
	 * used for communication , it just picks a random key and keep iterating to find an idle one 
	 *
	 */
	function Init()
	{
		$x=rand(10,99999999);
		$iD=10;
		while(1)
		{
			$iD=shmop_open($x, "c", $this->perms, $this->size);
			// if $iD doesn't equal Zero then we have found a valid key
			if ($iD!=0) break;
			$x++;
		}
		$this->id=$iD;
		$this->MemKey=$x;
		$this->RecHandler=new PlainArrayHandler($this);
		$this->MuxH=new SimpleMutex($this);
	}
	/**
	 * Shared Memory Communication Specific Setting Function
	 * 
	 * This function Encapsulates Setting values through Chosen Sync Class
	 *
	 * @param string $name
	 * @param string $value
	 * */
	function Set($name,$value)
	{
		$this->MuxH->Set($name,$value);
	}
	/**
	 * Shared Memory Communication Specific Getting Function
	 * 
	 * This function Encapsulates Getting values through Chosen Sync Class
	 *
	 * @param string $name
	 * @return string the value of Communication Varaible $name (if it exists) or null if not
	 */
	function Get($name)
	{
		return $this->MuxH->Get($name);
	}
	/**
	 * This is the Class Specific Setting function
	 * it writes to the Shared Memory Block the Values returned from Writting Methodolgy class Setting function
	 *
	 * @param string $name
	 * @param string $value
	 */
	function Gset($name,$value)
	{
		shmop_write($this->id,$this->RecHandler->PAHSet($name,$value) , 0);
	}
	/**
	 * This is the Value Getting function
	 * it wrraps the Writting Methodolgy Class Getting function
	 *
	 * @param string $name
	 * @return string the value of Communication Varaible $name (if it exists) or null if not
	 */
	function Gget($name)
	{
		return $this->RecHandler->PAHGet($name);
	}
	/**
	 * Shared Memory Communication Specific Existance Checking Function
	 * 
	 *  This function uses the Writting Methodolgy Class to do the Job directly (NO Sync. bounds)
	 *
	 * @param string $name
	 * @return boolean
	 */
	function Exist($name)
	{
		return $this->RecHandler->Exist($name);
	}
	function GetSize()
	{
		return shmop_size($this->id);
	}
	/**
	 * This is a class Spacefic Deleting function
	 * it writes to the Shared Memory Block the Values returned from Writting Methodolgy class Deleting function
	 *
	 * @param string $name
	 */
	function Delete($name)
	{
		shmop_write($this->id,$this->RecHandler->Delete($name) , 0);
	}
	/**
	 * Shared Memory Communication Specific Clearing Function
	 * 
	 * it removes Communication Shared Memory Block to clear any used space
	 *
	 */
	function Clear()
	{
		shmop_delete($this->id);
		shmop_close($this->id); 
	}
	/**
	 * This function is specific to PlainArray Writting class
	 * it returns all contents , here it reads the Shared Memory Block and returns it
	 *
	 * @return unknown
	 */
	function GetAll()
	{
		return shmop_read($this->id,0,$this->GetSize());
	}
}
?>