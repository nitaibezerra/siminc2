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
 * This is an Abstract class representing Methods all Communication classes must implement
 * @package Communication_Handlers
 * @abstract 
 */
abstract class ComBasic
{
	/**
	 * This is a void function with no arguments
	 * it's called by the Threading class to provide initialization to the communication class 
	 * @abstract 
	 */
	abstract public function Init();
	/**
	 * This is a void function with no arguments
	 * it's used to clear any space equipied by the communication class
	 * @abstract 
	 *
	 */
	abstract public function Clear();
	/**
	 * void function with arguments
	 * Associates / Sets Name $name with the Value $value
	 * it can be used to create a Name or reeassign it (change it's value )
	 * @abstract 
	 * @param string $name the Communication Varaible name
	 * @param string $value Communication Varaible value
	 * @todo Add a return type to indicate whether value properly set or not
	 */
	abstract public function Set($name,$value);
	/**
	 * Value returning function with arguments
	 * takes the name of a Communication Varaible and returns it's value
	 * All Names are allowed BUT the following :-
	 * "Read","Thread_#_State" where # is any number
	 * chosing any of these two reserved Names Could Crash the thing
	 * @abstract 
	 * @param string $name the Communication Varaible Name
	 * @return string the value of Communication Varaible $name (if it exists) or null if not
	 */
	abstract public function Get($name);
	/**
	 * Value returning function with arguments
	 * used to check if a Communication Varaible Name exists or not
	 * @abstract 
	 * @param string $name the Communication Varaible Name
	 * @return  bool A Boolean value indicating whether this Communication Varaible Name Exists or not
	 */
	abstract public function Exist($name);
}
?>