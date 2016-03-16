<?php
/**
 * This is the Part taking responsibility of Maintaing Sync. of read and 
 * write operation now it only has a simple Mutex Class
 * @package Synchronization_Handlers
 * @author Mohammed Yousef Bassyouni <harrrrpo@gmail.com>
 * @license http://opensource.org/licenses/gpl-license.php GNU Public License
 */
/**
 * Simple Mutex Class
 * A Class that Acts as a simple Mutex on an abstract manner it Takes a Communication class
 * and all Reading/Writting Process Must go through it
 * it used with uncontrolled Communications (e.g. Shmop ) and not used with alredy controlled ones(e.g. MySQL)
 * @package Synchronization_Handlers
 */
class SimpleMutex
{
	/**
	 * The Communication Helper Wrapped
	 *
	 * @var ComBasic
	 */
	public  $ComHelper;
	/**
	 * Simple Mutex Class Constructor
	 * 
	 * Reading is Allowed at it's starting
	 *
	 * @param ComBasic $ComH
	 * @return SimpleMutex
	 */
	function SimpleMutex($ComH)
	{
		$this->ComHelper=$ComH;
		$this->ComHelper->Gset("Read","ON");
	}
	/**
	 * Communication Varaibles Getter Function
	 * 
	 * it checks for the Availability of Reading , if not available it halts for 0.1 sec and tries again
	 * 
	 * @todo Make the 0.1sec value changeable by user and and set an upper waiting bound
	 * @param string $name
	 * @return string
	 */
	function Get($name)
	{
		while(1)
		{
			if ($this->MLock())
			{
				return $this->ComHelper->Gget($name);
			}
			usleep(100000);
		}
	}
	/**
	 * Communication Varaibles Setter Function
	 * 
	 * it sets a global lock and then changes the value then it releases the lock
	 *
	 * @todo Think again if the Set function should check for the Existance of lock or not (Currently it doesn't)
	 * @param string $name
	 * @param string $value
	 */
	function Set($name,$value)
	{
		$this->SetBusy();
		$this->ComHelper->Gset($name,$value);
		$this->UnSetBusy();
	}
	/**
	 * The Lock Checking Function
	 * this function checks the existance of a lock over the Communication class
	 * through checking the "Read" Communication Varaible
	 *
	 * @return Boolean Value indicating where Communication is locked or not
	 */
	function MLock()
	{
		$c=$this->ComHelper->Gget("Read");
		if ($c=="ON"){return true;}
		elseif ($c=="OFF"){return false;}
		else {$this->UnSetBusy();}
	}
	/**
	 * Setting the Lock function
	 * just set the "Read" value to "OFF"
	 *
	 */
	function SetBusy()
	{
		$this->ComHelper->Gset("Read","OFF");
	}
	/**
	 * Releasing the Lock function
	 * just set the "Read" value to "ON"
	 *
	 */
	function UnSetBusy()
	{
		$this->ComHelper->Gset("Read","ON");
	}
}
?>