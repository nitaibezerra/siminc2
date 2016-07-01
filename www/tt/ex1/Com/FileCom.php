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
 * The Flat File Communication Class
 * it extends the ComBasic Class and Implement it's Abstract Methods
 * @package Communication_Handlers 
 */
class FileCom extends ComBasic
{
	/**
	 * The Flat File Name
	 *
	 * @var string
	 */
	public  $UniqueName="MyFile";
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
	 * @todo Add an abstract writting Class of which type the var will be
	 */
	public  $RecHandler;
	/**
	 * The Flat File Communication Class Constructor
	 * 
	 * This class is called in two manners :-
	 * 1) Intializing Communication from parent thread here it's called like : $asd=new FileCom()
	 * 	  and File Name can be changed latter BUT before Init is called by Process (which has no 
	 *    effect but JUST changing the name)
	 * 
	 * 2) From a called thread to start Communication with other threads ( in same process ) :$asd=new FileCom($uname)
	 * 	  where $uname is the Communication file name which must be passed from mother thread to called threads
	 *
	 * @param string $uname
	 * @return FileCom
	 */
	function FileCom($uname="")
	{
		// if this is called from mother thread leave
		if ($uname=="")return;
		$this->UniqueName=$uname;
		// Assign a Writting Methodolgy Class For Called Threads
		$this->RecHandler=new PlainArrayHandler($this);
		// Assign a Sync. Control Class For Called Threads
		$this->MuxH=new SimpleMutex($this); 
	}
	/**
	 * Flat File Communication Specific Init Function
	 * 
	 * (For pupose of this function review ComBasic abstract class documentation)
	 * 
	 * For the FileCom class the role of this function is to create the file to be ready for using
	 * it checks for the existance of a file with the same name and keeps looping to find an idle one
	 *
	 */
	function Init()
	{
		$i=0;
		while(1){
			if (!(file_exists($this->UniqueName.$i))){
				$w=fopen($this->UniqueName.$i,"wb");
				fclose($w);
				$this->UniqueName=$this->UniqueName.$i;
				break;
			}
			else
			{
				$i++;
			}
		}
		$this->RecHandler=new PlainArrayHandler($this);
		$this->MuxH=new SimpleMutex($this); 
	}
	/**
	 * Flat File Communication Specific Exisctance Checking Function
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
	/**
	 * Flat File Communication Specific Setting Function
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
	 * Flat File Communication Specific Getting Function
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
	 * it writes to the file the Values returned from Writting Methodolgy class Setting function
	 *
	 * @param string $name
	 * @param string $value
	 */
	function  Gset($name,$value)
	{
		file_put_contents($this->UniqueName,$this->RecHandler->PAHSet($name,$value));
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
	 * This is a class Spacefic Deleting function
	 * it writes to the file the Values returned from Writting Methodolgy class Deleting function
	 *
	 * @param string $name
	 */
	function Delete($name)
	{
		file_put_contents($this->$UniqueName,$this->RecHandler->PAHSet($name,$value));
	}
	/**
	 * This function is specific to PlainArray Writting class
	 * it returns all contents , here it reads all file and returns it
	 *
	 * @return unknown
	 */
	function GetAll()
	{
		return file_get_contents($this->UniqueName);
	}
	/**
	 * Flat File Communication Specific Clearing Function
	 * 
	 * it removes communication files to clear any used space
	 *
	 */
	function Clear()
	{
		unlink($this->UniqueName);
	}
}
?>