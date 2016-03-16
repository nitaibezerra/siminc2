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
 * The MySQL Communication Class
 * it extends the ComBasic Class and Implement it's Abstract Methods
 * one thing specific to this Communication Scheme is that it doesn't use neither Sync class
 * nor Writting Methodology Class , as it depends on the support of this in MySQL itself
 * it's problem is that even it uses Heap tables it's not as fast as Shared Memory (when using ie WELL)
 * nor it's as Available as Flat Files
 * @package Communication_Handlers 
 * @todo Write Classes for more DBMS and an abstract one to wrap them
 */
class MyCom extends ComBasic
{
	/**
	 * MySQL Link to be used all around
	 *
	 * @var resource
	 */
	private  $Mylnk;
	/**
	 * Table name to be used for Communication
	 *
	 * @var string
	 */
	public   $Table;
	/**
	 * The MySQL Communication Class Constructor
	 * 
	 * This class is called in THE SAME way for two purposes  :-
	 * 1) From the Mother Thread and then it's Inited for creating the table
	 * 
	 * 2) From a child thread that wants to Communicate
	 *
	 * @param string $hostname The MySQL host
	 * @param string $username MySQL Authorized username
	 * @param string $pass user pass
	 * @param stringe $DB MySQL DB to be used
	 * @param string $Tablename The Table name to be used
	 * @return MyCom
	 */
	function MyCom($hostname,$username,$pass,$DB,$Tablename="")
	{
		$this->Mylnk=mysql_connect($hostname,$username,$pass);
		mysql_select_db($DB,$this->Mylnk);
		$this->Table=$Tablename;
	}
	/**
	 * MySQL Communication Specific Init Function
	 * 
	 * (For pupose of this function review ComBasic abstract class documentation)
	 * 
	 * For the MyCom class the role of this function is to create the table	 
	 * used for communication , it starts with name "process" and then continues till find 
	 * an idle name 
	 *
	 */
	function Init()
	{
		$i=-1;
		do {
			$i++;
			$n="Process".$i;
			//Create a table with simple Schema just a place for $name and $value with $name the Primaru Key
			$res=mysql_query("CREATE TABLE $n(Prob char(255) PRIMARY KEY,Val char(255))ENGINE=HEAP",$this->Mylnk);
		}while ($res==false);
		$this->Table="Process".$i;
	}
	/**
	 * MySQL Communication Specific Setting Function
	 * 
	 * Here the function tries to insert a new row containing new $name and $value
	 *
	 * @param string $name
	 * @param string $value
	 * @return Boolean
	 */
	function Set($name,$value)
	{
		$res=mysql_query("INSERT INTO $this->Table VALUES ('$name','$value')",$this->Mylnk);
		if ($res==false)
		{
			$res=mysql_query("UPDATE $this->Table SET Val='$value' WHERE Prob='$name'",$this->Mylnk);
		}
		return $res;
	}
	/**
	 * MySQL Communication Specific Getting Function
	 *
	 * Select the $value of $name from the Communication Table
	 * 
	 * @param string $name
	 * @return string the value Associated with name
	 */
	function Get($name)
	{
		$res=mysql_query("SELECT Val FROM $this->Table WHERE Prob='$name'",$this->Mylnk);
		if ($res!=false)
		{
			$row = mysql_fetch_array($res);
			return  $row['Val'];
		}
		return $res;
	}
	/**
	 * MySQL Communication Specific Existance Checking Function
	 * 
	 * Check if the result of SELECTing the Prob $name has any affected rows
	 *
	 * @param string $name
	 * @return Boolean
	 */
	function Exist($name)
	{
		$res=mysql_query("SELECT Val FROM $this->Table WHERE Prob='$name'",$this->Mylnk);
		return mysql_affected_rows($this->Mylnk);
	}
	/**
	 * MySQL Communication Specific Clearing Function
	 * 
	 * Just Drop the table
	 *
	 */
	function Clear()
	{
		mysql_query("DROP TABLE $this->Table");
	}
}
?>