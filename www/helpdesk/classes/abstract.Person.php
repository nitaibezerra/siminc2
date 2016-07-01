<?php
/**
 * File containing the Person abstract class.
 *
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version SVN: $Id: abstract.Person.php 4 2005-12-13 01:47:15Z nicolas $
 * @since File available since Release 1.1.1.1
 * \\||
 */
/**
 * includes
 */
require_once_check(PHPST_PATH . 'classes/interface.DatabaseMirror.php');
require_once_check(PHPST_PATH . 'classes/static/static.Validate.php');

/**
 * This abstract class serves as a top-level definition for all
 * organisational entities: projects, companies, departments and offices.
 * All these classes should overload most of the methods defined here, though
 * this is not absolutely necessary.
 *
 * @package PHPSupportTickets
 */
abstract class PHPST_Person implements PHPST_DatabaseMirror {
    /**
     * ID number matching the DB entry
     *
     * @access private
     * @var int
     */
    private $id;

    /**
    * Person's Name.
    *
    * @access private
    * @var String
    */
    private $name;

    /**
     * Person's UNIX timestamp of creation in DB.
     *
     * @access private
     * @var String
     */
    private $timestamp;

    /**
     * Constructor: takes an array whose keys are attributes of the
     * Person and values are their value. Can come from either a
     * submitted entry form, or a returned row from the DB.
     *
     * @access public
     * @param array $ Fields
     */
    public function __construct($fields) {
        if (is_array($fields)) extract($fields);
        // Sometimes we want to create a blank Person that gets filled
        // from the database afterwards
        if (isset($timestamp)) {
            $this->setTimestamp($timestamp);
        } else {
            $this->setTimestamp(time());
        }

        $this->setName($name);

        if (isset($id)) {
            $this->setId($id);
        }
    }

    /**
     * This static function must be implemented by all objects that implement
     * the DatabaseMirror interface. It enables us to validate the user input
     * fields BEFORE creating the object or entering it into the DB.
     *
     * @param array $fields
     * @return boolean True if all fields are valid, array of arrays with error
     *         message and name of field if error is encountered.
     */
    public static function validate($fields) {
        if (is_array($fields)) {

            extract($fields);
            $errors = array();
            if ($name == "" || is_null($name)) {
                $errors[] = array("field" => "name",
                        "message" => VALIDATE_EMPTY_NAME);
            } elseif (PHPST_Validate::checkAlpha($name) !== true) {
                $errors[] = PHPST_Validate::checkAlpha($name, "name");
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

    /* ---- GETTERS ---- */

    /**
    * Returns the Person's name.
    *
    * @return string
    * @access public
    */
    public function getName() {
        return $this->name;
    }

    /**
     * Returns the Person's UNIX timestamp of creation in DB.
     *
     * @return int
     * @access public
     */
    public function getTimestamp() {
        return $this->timestamp;
    }

    /* ---- SETTERS ---- */

    /**
    * Sets the Person's name.
    *
    * @param string
    * @access public
    */
    public function setName($string) {
         $this->name = $string;
    }

    /**
     * Sets the Person's UNIX timestamp of creation in DB.
     *
     * @param int $
     * @access public
     */
    public function setTimestamp($int) {
        $this->timestamp = $int;
    }

    /**
     * Person::setID()
     *
     * @param  $ID
     * @access public
     * @return boolean true if successful
     */
    public function setId($id) {
        // Perform validation first
        $this->id = $id;
    }



    /**
     * Returns a formatted string made up of different fields.
     * E.g. <title> <fname> <mname> <lname>, <job_title> (job_title belongs
     * to a subclass, this why this method is only abstract).
     * THIS METHOD MUST BE IMPLEMENTED BY ALL CONCRETE SUB-CLASSES !
     *
     * @abstract
     */
    abstract function getFullDescription();
}

?>