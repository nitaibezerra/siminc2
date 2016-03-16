<?php
/**
 * File containing the HierarchyLevel abstract class.
 *
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version SVN: $Id: abstract.HierarchyLevel.php 4 2005-12-13 01:47:15Z nicolas $
 * @since File available since Release 1.1.1.1
 * \\||
 */
/**
* @package PHPSupportTickets
*/

require_once_check(PHPST_PATH . 'classes/interface.DatabaseMirror.php');


/**
* This abstract class serves as a top-level definition for all
* organisational entities: projects, companies, departments and offices.
* All these classes should overload most of the methods defined here, though
* this is not absolutely necessary.
*
* @package PHPSupportTickets
*/
abstract class PHPST_HierarchyLevel implements PHPST_DatabaseMirror{

    /**
    * id number matching the DB entry
    *
    * @access private
    * @var int
    */
    private $id;

    /**
    * Project name.
    *
    * @access private
    * @var String
    */
    private $name;

    /**
    * Projet description.
    *
    * @access private
    * @var string
    */
    private $description;

    /**
    * An Employee object, this level's manager
    *
    * @access private
    * @var object
    */
    private $manager = null;

    /**
    * This level's status, either Active or Suspended.
    *
    * @access private
    * @var string
    */
    private $status = 'Active';
    

    /**
    * HierarchyLevel::__construct()
    *
    * @param $name
    * @param $description
    * @return string
    **/
    public function __construct($fields) {
        if (is_array($fields)) extract($fields);

        // Sometimes we want to create a blank Person that gets filled
        // from the database afterwards

        if (isset($name)) {
            $this->setName($name);
            @$this->setDescription($description);
            @$this->setId($id);
            @$this->setStatus($status);
        }
    }

    /**
    * This static function must be implemented by all objects that implement
    * the DatabaseMirror interface. It enables us to validate the user input
    * fields BEFORE creating the object or entering it into the DB.
    *
    * @param array $fields
    * @return boolean True if all fields are valid, array with error
    *       message and name of field if error is encountered.
    */
    public static function validate($fields) {
        $errors = array();
        if (is_array($fields)) {
            extract($fields);
            if ($name == "" || is_null($name)) {
                $errors[] = array("field" => "name",
                        "message" => VALIDATE_EMPTY_NAME);
            }
            if (count($errors) == 0) {
                // If no errors, return true
                return true;
            } else {
                // If any error was found, an array of errors is returned
                return $errors;
            }
        }
    }

    /**
    * HierarchyLevel::getName()
    *
    * @return string
    **/
    public function getName() {
        return $this->name;
    }

    /**
    * HierarchyLevel::getDescription()
    *
    * @return string
    **/
    public function getDescription() {
        return $this->description;
    }

    /**
    * HierarchyLevel::getStatus()
    *
    * @return string
    **/
    public function getStatus() {
        return $this->status;
    }
    
    /**
    * HierarchyLevel::getManager()
    *
    * @return Employee
    **/
    public function getManager() {
        return $this->manager;
    }

    /**
    * HierarchyLevel::setId()
    *
    * @param $id
    * @return boolean True if successful
    **/
    public function setId($id) {
        // Perform validation first
        $this->id = $id;
    }

    /**
    * HierarchyLevel::setName()
    *
    * @param $name
    * @return boolean True if successful
    **/
    public function setName($name) {
        // Perform validation first
        $this->name = $name;
    }

    /**
    * HierarchyLevel::setDescription()
    *
    * @param $name
    * @return boolean True if successful
    **/
    public function setDescription($description) {
        // Perform validation first
        $this->description = $description;
    }

    /**
    * HierarchyLevel::setStatus()
    *
    * @param $name
    * @return boolean True if successful
    **/
    public function setStatus($status) {
        // Perform validation first
        $this->status = $status;
    }
    
    /**
    * HierarchyLevel::setManager()
    *
    * @param $user
    * @return boolean True if successful
    **/
    public function setManager(User $user) {
        // Perform validation first
        $this->manager = $user;
    }
}

?>