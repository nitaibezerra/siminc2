<?php
/**
 * File containing the Department class.
 *
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version SVN: $Id: class.Department.php 4 2005-12-13 01:47:15Z nicolas $
 * @since File available since Release 1.1.1.1
 * \\||
 */

/**
* user defined includes.
* @include
*/
require_once_check(PHPST_PATH . 'classes/abstract.HierarchyLevel.php');
require_once_check(PHPST_PATH . 'classes/class.User.php');
require_once_check(PHPST_PATH . 'classes/SQL/class.DepartmentSQLBuilder.php');

// user defined constants
$crypt = new Crypt_Blowfish(DB_PASS);
if (isset($_SESSION['user'])) {
    define('DEPARTMENT_USER_ID', unserialize($crypt->decrypt($_SESSION['user']))->getId());
} else {
    define('DEPARTMENT_USER_ID', 0);
}
/**
 * A Department represents a division of
 * roles (management, accounting, sales). It contains Users.
 *
 * @access public
 * @package PHPSupportTicket
 */
class PHPST_Department extends PHPST_HierarchyLevel {
    /// --- FIELDS ---

    /**
    * The database table to mirror this object
    *
    * @access public
    * @static
    * @var string
    */
    public static $table = DB_PREFIX_DEPARTMENTS;

    /**
     * Array of users.
     *
     * @access private
     * @var User
     */
    private $users = array();

    /**
     * Companion object used for building SQL queries. Separates
     * class responsibilities from SQL logic.
     *
     * @access private
     * @var object $SQLBuilder
     */
     private $SQLBuilder;

    // --- METHODS ---

    /**
     * Short description of method __construct
     *
     * @access public
     * @param array $fields
     * @return void
     */
    public function __construct($fields) {
        parent::__construct($fields);
        if (is_array($fields)) {
            if (empty($status)) {
                $this->setStatus('Active');
            }
            // No other fields to set at this time
            $this->SQLBuilder = new PHPST_DepartmentSQLBuilder();
        } else {
            PHPST_Logger::logEvent(3, "Department Operation", DEPARTMENT_USER_ID,
                    "Wrong argument type given to Deparment::__construct", __FILE__, __LINE__);
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
            // First retrieve parent class validate() errors, if any
            if (parent::validate($fields) !== true) {
                $errors = parent::validate($fields);
            }

            if (count($errors) == 0) {
                // If no errors, return true
                return true;
            } else {
                PHPST_Logger::logEvent(3, "Department Operation", DEPARTMENT_USER_ID,
                    "Fields failed Department validation", __FILE__, __LINE__);
                return $errors;
            }
        }
    }

    /**
     * Adds a User to this Department
     *
     * @access public
     * @param User Object
     * @return boolean
     */
    public function addUser($user) {
        // Perform validation first
        $this->users[$user->getId()] = $user;
    }

    /**
     * Returns the array of users in this department.
     *
     * @access public
     * @return array
     */
    public function getUsers() {
        return $this->users;
    }

    /**
     * Removes a User from this Department
     *
     * @access public
     * @param int
     * @return boolean
     */
    public function removeUser($index) {
        // Perform validation first
        unset($this->users[$index]);
    }

    /**
     * Implementation of the DatabaseMirror interface function getID()
     *
     * @access public
     * @return int
     */
    public function getId() {
        if (isset($this->id)) {
            return $this->id;
        } else {
            $conn = &ADONewConnection(DSN);
            $sql = $this->SQLBuilder->search(DB_PREFIX_DEPARTMENTS, array("id"),
                    array("name" => $this->getName()));

            $rs = &$conn->Execute($sql);

            if ($rs && $rs->RecordCount() > 0) {
                $this->id = $rs->fields['id'];
                return (int) $this->id;
            } else {
                PHPST_Logger::logEvent(3, "DB Error", DEPARTMENT_USER_ID,
                        "Could not retrieve department ID", __FILE__, __LINE__);
                return null;
            }
        }
    }

    /**
     * Implementation of the DatabaseMirror interface function addToDB().
     * Inserts the object into the DB
     *
     * @access public
     * @return boolean True if successful
     */
    public function addToDB() {
        // Adds this Address to the DB
        $conn = &ADONewConnection(DSN);

        // First check that the department doesn't already exist
        $sql = $this->SQLBuilder->search(DB_PREFIX_DEPARTMENTS, array("id"),
                    array("name" => $this->getName()),
                    null, 'ASC', 0, 1);

        $rs = &$conn->Execute($sql);
        if ($rs != false && $rs->RecordCount() == 0) {


            // Retrieve array of table's column names (always UPPERCASE)
            $columns = $conn->MetaColumnNames(PHPST_Department::$table);
            $sql = $this->SQLBuilder->insert($this, $columns);
            $rs = &$conn->Execute($sql);

            if ($rs != false) {
                PHPST_Logger::logEvent(3, "Department Operation", DEPARTMENT_USER_ID,
                        "Department " . $this->getName() . " added", __FILE__, __LINE__);
                $id = $conn->Insert_ID();
                // Insert users entries in department_users table is such users exist
                if (count($this->getUsers()) > 0 ) {
                    $sql = $this->SQLBuilder->updateUsersList($this->getID(), $this->getUsers());
                    $rs = &$conn->Execute($sql);
                }
                return $id;
            } else {
                PHPST_Logger::logEvent(3, "DB Error", DEPARTMENT_USER_ID,
                        "Could not add Department " . $this->getName(), __FILE__, __LINE__);
                return false;
            }
        } else {
            PHPST_Logger::logEvent(3, "Department Operation", DEPARTMENT_USER_ID,
                    "Department " . $this->getName() . " already exists, none added",
                     __FILE__, __LINE__);
        }
    }

    /**
     * Implementation of the DatabaseMirror interface function removeFromDB().
     * Deletes the object from the DB
     *
     * @access public
     * @return boolean True if successful
     */
    public function removeFromDB($id) {
        $conn = &ADONewConnection(DSN);
        $sql = $this->SQLBuilder->delete($id);
        if (count($this->getUsers()) > 0) {
            $sql = $this->SQLBuilder->deleteUsers($id);
            $conn->Execute($sql);
        }

        $rs = &$conn->Execute($sql);
        if ($rs != false && $conn->Affected_Rows() == 0) {
            PHPST_Logger::logEvent(3, "Department Operation", DEPARTMENT_USER_ID,
                    "Department Deleted: ID $id", __FILE__, __LINE__);
            return true;
        } else {
            PHPST_Logger::logEvent(3, "DB Error", DEPARTMENT_USER_ID,
                    "Could not delete Department: ID $id", __FILE__, __LINE__);
            return false;
        }
    }

    /**
     * Implementation of the DatabaseMirror interface function updateDB().
     * Updates the object in the DB
     *
     * @access public
     * @return boolean True if successful
     */
    public function updateDB() {
        $conn = &ADONewConnection(DSN);
        $this->getId();

        // Do not enter row into intersection table if record already exists
        // $sql = $this->SQLBuilder->emptyDepartmentUsers($this->getId());
        // $conn->Execute($sql);

        // Update deparments table
        $sql = $this->SQLBuilder->updateUsersList($this->getId(), $this->getUsers());
        if (strlen($sql) > 0 ) {
            $conn->Execute($sql);
        }

        $columns = $conn->MetaColumnNames(PHPST_Department::$table);
        $sql = $this->SQLBuilder->update($this, $columns);
        $rs = &$conn->Execute($sql);

        if ($rs && $conn->Affected_Rows() > 0) {
            PHPST_Logger::logEvent(3, "Department Operation", DEPARTMENT_USER_ID,
                    "Updated Department " . $this->getName(), __FILE__, __LINE__);
            return true;
        } elseif($conn->Affected_Rows() == 0) {
            PHPST_Logger::logEvent(3, "Department Operation", DEPARTMENT_USER_ID,
                    "Did not update Department " . $this->getName() . ", no changes"
                    , __FILE__, __LINE__);
            return true;
        } else {
            PHPST_Logger::logEvent(3, "DB Error", DEPARTMENT_USER_ID,
                    "Could not update Department " . $this->getName(), __FILE__, __LINE__);
            return false;
        }
    }

    /**
     * Implementation of the DatabaseMirror interface function getFromDB().
     * Returns a Department object from the DB based on input in $fields array
     *
     * @static
     * @access public
     * @return Object Department if successful, false otherwise
     */
    public static function getFromDB($fields) {
        $myDepartment = null;
        $SQLBuilder = new PHPST_DepartmentSQLBuilder();
        $conn = &ADONewConnection(DSN);
        $conn->SetFetchMode(ADODB_FETCH_ASSOC);
        $sql = $SQLBuilder->get($fields);

        $rs = &$conn->Execute($sql);
        if ($rs != false && $rs->RecordCount() > 0) {
            $myDepartment = new PHPST_Department($rs->fields);

            // Get users attached to this department
            $sql = $SQLBuilder->getUsers($myDepartment->getId());
            $rs = &$conn->Execute($sql);
            if ($rs && $rs->RecordCount() > 0) {
                while (!$rs->EOF) {
                    $user = new PHPST_User($rs->fields);
                    $myDepartment->addUser($user);
                    $rs->MoveNext();
                }
            }

        } else {
            PHPST_Logger::logEvent(3, "DB Error", DEPARTMENT_USER_ID,
                    "Department retrieval: MYSQL error: " . addslashes($sql), __FILE__, __LINE__);
            return false;
        }

        return $myDepartment;
    }

    /**
     * Returns an array of Department objects from the DB based
     * on input in $fields array
     *
     * @static
     * @access public
     * @param string $user_type Admin or Mod
     * @param string $search The search term
     * @param string $field The field being searched
     * @param string $sort The field by which to sort the results
     * @param string $order The order in which to sort the results
     * @return array Departments if successful, false otherwise
     */
    public static function getArrayFromDB($user_type = "Mod", $search = null, $field = null
            , $sort = null, $order = null, $max_records, $rs_page) {
        $conn = &ADONewConnection(DSN);
        $SQLBuilder = new PHPST_DepartmentSQLBuilder();

        // Perform a query that returns every department, its users
        $sql = $SQLBuilder->getFullDepartmentsArray(
                $user_type, $search, $field, $sort, $order);

        $ids = array();
        if (!isset($rs_page)) {
            $rs_page = 1;
        }
        $departments = array();
        $departments['count'] = 0;

        // Perform SQL for creating csv file
        $csv_sql = $SQLBuilder->getCSV();
        $csv_rs = $conn->Execute($csv_sql);
        $csv = rs2csv($csv_rs);
        $departments['csv'] = $csv;

        $rs = &$conn->Execute($sql);

        // If page requested is 'all', show all records on one page
        if ($rs_page == 'all') {
            $max_records = 999999;
            $rs_page = 1;
        }

        if ($rs != false && $rs->RecordCount() > 0) {
            $department_count = 0;
            while (!$rs->EOF) {
                $row = $rs->fields;

                // Increment department count if this department is new
                if (!isset($ids[$row['id']])) {
                   $department_count++;
                }
                // Only add info to array if department is within pagination limits
                if ($department_count > (($rs_page - 1) * $max_records)
                        && $department_count <= ($rs_page * $max_records)) {
                    // prepare arrays
                    $user_fields = array();
                    $department_fields = array();
                    $ticket_fields = array();

                    foreach ($row as $field => $value) {
                        if (eregi('^ticket_', $field) && $value != null) {
                            $ticket_fields[substr($field, 7)] = $value;
                        } elseif (eregi('^user_', $field) && $value != null) {
                            $user_fields[substr($field, 5)] = $value;
                        } else {
                            $department_fields[$field] = $value;
                        }
                    }

                    if (count($ticket_fields) > 1) {
                       $departments[$row['id']]['tickets'][$ticket_fields['id']] = $ticket_fields;
                    }
                    if (count($user_fields) > 1) {
                       $departments[$row['id']]['users'][$user_fields['id']] = $user_fields;
                    }

                    $departments[$row['id']]['department'] = $department_fields;
                }
                $ids[$row['id']] = 1;
                $rs->MoveNext();
                $departments['count'] = $department_count;
            }

            return $departments;
        } else {
            PHPST_Logger::logEvent(3, "DB Error", DEPARTMENT_USER_ID,
                    "SQL error trying to get Array of departments: " . addslashes($sql),
                    __FILE__, __LINE__);
            return false;
        }
    }

    /**
    * Tostring function: returns a string representation of this object
    *
    * @access public
    * @return string
    */
    public function __toString() {
        $string = "Department object:
            <br />___________________";
        $string .= "<br />id: " . $this->getID();
        $string .= "<br />Name: " . $this->getName();
        $string .= "<br />Description: " . $this->getDescription();
        $string .= "<br />Users: <ul>";
        foreach ($this->getUsers() as $user) {
            $string .= "<li>" . $user->getName() . "</li>";
        }
        $string .= "\n</ul>";
        return $string;
    }
}
?>