<?php
/**
 * This is the Part taking responsibility of the way Communication Classes uses
 * given Space and Saves Varaibles
 * @package Writting_Methodology_Handlers
 * @author Mohammed Yousef Bassyouni <harrrrpo@gmail.com>
 * @license http://opensource.org/licenses/gpl-license.php GNU Public License
 */
/**
 * The Plain Array Class Implements a very simple way of Reading and writting
 * it Saves varaibles in array and Serailizes it to Media , Whenever it needs to
 * read a Varaible or write to one it takes the whole Serailized Array , Unserailize it 
 * and returns value or writes to existing one and Serailize back
 * @package Writting_Methodology_Handlers
 */
class PlainArrayHandler
{
	/**
	 * Communication class used by Writting Class
	 *
	 * @var ComBasic
	 */
	public  $ComHelper;
	/**
	 * PlainArrayHandler Class Constructor
	 * 
	 * Just assign given ComBasic to Class Varaible
	 * 
	 * @param ComBasic $ComH
	 * @return PlainArrayHandler
	 */
	function PlainArrayHandler($ComH)
	{
		$this->ComHelper=$ComH;
	}
	/**
	 * Plain Array Class Getter function
	 * 
	 * unserailaize Data from Communication Class and return Value Corresponding to name in array
	 *
	 * @param string $name
	 * @return string
	 */
	function PAHGet($name)
	{
		$data=unserialize($this->ComHelper->GetAll());
		return $data[$name];
	}
	/**
	 * Plain Array Class Setter function
	 *
	 * after Unserailizing data this function assign vakue to name and returns Serialized version of the array
	 * to be written back to Media
	 * 
	 * @param string $name
	 * @param string $value
	 * @return string
	 */
	function PAHSet($name,$value)
	{
		$data=unserialize($this->ComHelper->GetAll());
		$data[$name]=$value;
		return serialize($data);
	}
	/**
	 * Plain Array Class Existance Checking function
	 *
	 * after Unserailizing data this function Check for Exstance of a certain key using isset function
	 * 
	 * @param string $name
	 * @return Boolean
	 */
	function Exist($name)
	{
		$data=unserialize($this->ComHelper->GetAll());
		return isset($data[$name]);
	}
	/**
	 * Plain Array Class Deletion Function
	 * 
	 * after Unserailizing data this function Deletes a given key and returns Serialized version of the array
	 * to be written back to Media
	 *
	 * @param string $name
	 * @return string
	 */
	function Delete($name)
	{
		$data=unserialize($this->ComHelper->GetAll());
		unset($data[$name]);
		return serialize($data);
	}
}
?>