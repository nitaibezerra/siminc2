<?php
/**
 * File containing the User class.
 *
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version SVN: $Id: class.User.php 4 2005-12-13 01:47:15Z nicolas $
 * @since File available since Release 1.1.1.1
 * \\||
 */

/**
* user defined includes
* @include
*/
require_once_check(PHPST_PATH . 'classes/class.Note.php');
require_once_check(PHPST_PATH . 'classes/abstract.Person.php');
require_once_check(PHPST_PATH . 'classes/SQL/class.UserSQLBuilder.php');
require_once_check(PHPST_PATH . 'classes/static/static.Logger.php');
/**
* user defined constants
* @ignore
*/
$crypt = new Crypt_Blowfish(DB_PASS);
if (isset($_SESSION['user'])) {
    define('USER_USER_ID', unserialize($crypt->decrypt($_SESSION['user']))->getId());
    define('USER_ADMIN', unserialize($crypt->decrypt($_SESSION['user']))->getAdmin());
} else {
    define('USER_USER_ID', 0);
    define('USER_ADMIN', 'Mod');
}

/**
* User class.
*
* @access public
* @package PHPSupportTickets
*/
class PHPST_User extends PHPST_Person {
    // --- ATTRIBUTES ---

    /**
    * The database table to mirror this object
    *
    * @access public
    * @static
    * @var string
    */
    public static $table = DB_PREFIX_USER;

    /**
    * An array of which fields are required from user input
    *
    * @access public
    * @static
    * @var array
    */
    public static $required_fields = array(
                'username', 'password', 'admin',
                'name', 'email');

    /**
    * Short description of attribute username
    *
    * @access private
    * @var String
    */
     private $username = null;

    /**
    * Short description of attribute password
    *
    * @access private
    * @var String
    */
     private $password = null;

    /**
    * Short description of attribute email
    *
    * @access private
    * @var String
    */
     private $email = null;

    /**
    * Short description of attribute lastLogin
    *
    * @access public
    * @var String
    */
     // private $last_login = null;

    /**
    * Short description of attribute admin
    *
    * @access private
    * @var boolean
    */
     private $admin = "Mod";

     /**
     * Contains all information about this user in an associative array,
     * including all class variables.
     *
     * @access private
     * @var array info
     */
     private $info = array();

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
    * Constructor: first call on parent class constructor.
    *
    * @access public
    * @param array $fields From a Posted form or a database row
    * @return void
    */
    public function __construct($fields) {
        $this->SQLBuilder = new PHPST_UserSQLBuilder();
        if (is_array($fields) && isset($fields['username'])) {
            parent::__construct($fields);
            extract($fields);

            $this->setAdmin($admin);
            $this->setEmail($email);
            // $this->setLast_login($last_login);
            $this->setPassword($password);
            $this->setUsername($username);
            $this->setInfo($fields);

            if (isset($id)) {
                $this->setId($id);
            }
        } else {
            PHPST_Logger::logEvent(3, "User Operation", USER_USER_ID,
                    "Wrong argument type given to PHPST_User::__construct", __FILE__, __LINE__);
        }
    }

    /* --- DATABASEMIRROR METHODS --- */

    /**
    * This static function must be implemented by all objects that implement
    * the DatabaseMirror interface. It enables us to validate the user input
    * fields BEFORE creating the object or entering it into the DB.
    *
    * @param array $fields
    * @return boolean True if all fields are valid, array of arrays with error
    *       message and name of field if error is encountered.
    */
    public static function validate($fields) {
        if (is_array($fields)) {
            extract($fields);
            $errors = array();

            // Only require non-empty values for required fields for this admin type
            $admin = USER_ADMIN;

            // First retrieve parent class validate() errors, if any
            if (parent::validate($fields) !== true) {
                $errors = parent::validate($fields);
            }

            // Verify that required fields are not empty

            if (in_array("email", PHPST_User::$required_fields) &&
                    ($email == "" || is_null($email))) {
                $errors[] = array("field" => "email",
                        "message" => VALIDATE_EMPTY_EMAIL);
            } elseif (PHPST_Validate::checkEmail($email) !== true) {
                $errors[] = PHPST_Validate::checkEmail($email, "email");
            }

            if (in_array("username", PHPST_User::$required_fields) &&
                    ($username == "" || is_null($username))) {
                $errors[] = array("field" => "username",
                        "message" => VALIDATE_EMPTY_USERNAME);
            } elseif (PHPST_Validate::checkAlphanumeric($username) !== true) {
                $errors[] = PHPST_Validate::checkAlphanumeric($username, "username");
            }

            if (in_array("password", PHPST_User::$required_fields) &&
                    ($password == "" || is_null($password))) {
                $errors[] = array("field" => "password",
                        "message" => VALIDATE_EMPTY_PASSWORD);
            } elseif (PHPST_Validate::checkPassword($password) !== true) {
                $errors[] = PHPST_Validate::checkPassword($password, "password");
            }

            if (in_array("admin", PHPST_User::$required_fields)
                   && (!isset($admin)|| $admin == "")) {
                $errors[] = array("field" => "admin",
                        "message" => VALIDATE_EMPTY_ADMIN);
            } elseif ($admin != "Admin"
                    && $admin != "Mod"
                    && $admin != "Client") {
                $errors[] = array("field" => "admin",
                        "message" => VALIDATE_INVALID_ADMIN);
            }

            if (count($errors) == 0) {
                PHPST_Logger::logEvent(3, "User Operation", USER_USER_ID,
                        "User object created, passed validation", __FILE__, __LINE__);
                return true;
            } else {
                PHPST_Logger::logEvent(3, "User Operation", USER_USER_ID,
                        "User object not created, failed validation", __FILE__, __LINE__);
                return $errors;
            }
        }
    }

    /**
    * Retrieves this User's ID number based on a unique variable.
    * Also sets this object's ID number to the retrieved int
    *
    * @return int User's ID number
    */
    public function getId() {
        $conn = &ADONewConnection(DSN);

        // We might be searching for a Client with no username, so look
        // for another unique combination of fields.
        if (is_null($this->getUsername()) || $this->getUsername() == "") {
            $sql = $this->SQLBuilder->search(phpst_User::$table, array(DB_PREFIX_USER_ID),
                    array("email" => $this->getEmail(), "name" => $this->getName()));
        } else {
            $sql = $this->SQLBuilder->search(phpst_User::$table, array(DB_PREFIX_USER_ID),
                array("username" => $this->getUsername()));
        }

        $rs = &$conn->Execute($sql);

        if ($rs != false && $rs->RecordCount() > 0) {
            $this->id = $rs->fields[DB_PREFIX_USER_ID];
            return $this->id;
        } else {
            PHPST_Logger::logEvent(3, "DB Error", USER_USER_ID,
                    "Could not retrieve User id: " . $this->getUsername(), __FILE__, __LINE__);
            return false;
        }
    }

    /**
    * Short description of method add
    *
    * @access public
    * @return boolean Returns null if user already exists, true if successfully entered, false otherwise
    */
    public function addToDB() {
        // Initialise Database object
        $conn = &ADONewConnection(DSN);

        // First check that User doesn't already exist.
        if (is_null($this->getUsername()) || $this->getUsername() == "") {
            $sql = $this->SQLBuilder->search(phpst_User::$table, array(DB_PREFIX_USER_ID),
                    array("email" => $this->getEmail(), "name" => $this->getName()));
        } else {
            $sql = $this->SQLBuilder->search(phpst_User::$table, array(DB_PREFIX_USER_ID),
                array("username" => $this->getUsername()));
        }

        $rs = &$conn->Execute($sql);

        if ($rs->RecordCount() == 0) {
            // Retrieve array of table's column names (always UPPERCASE)
            $columns = $conn->MetaColumnNames(phpst_User::$table);
            $sql = $this->SQLBuilder->insert($this, $columns);

            $rs = &$conn->Execute($sql);
            $id = $conn->Insert_ID();
            // Verify previous query
            if ($rs != false) {
                PHPST_Logger::logEvent(3, "User Operation", USER_USER_ID,
                        "User object added to DB", __FILE__, __LINE__);
                return $id;
            } else {
                PHPST_Logger::logEvent(3, "DB Error", USER_USER_ID,
                        "Could not Insert User object.", __FILE__, __LINE__);
                return false;
            }
        } else {
            PHPST_Logger::logEvent(3, "User Operation", USER_USER_ID,
                        "User object not added to DB, username already exists", __FILE__, __LINE__);
            return null;
        }
    }

    /**
    * Removes a User from the Database
    *
    * @access public
    * @param int $id
    * @return boolean true if successful
    */
    public function removeFromDB($id) {
        $conn = &ADONewConnection(DSN);
        $sql = $this->SQLBuilder->delete($id);
        $rs = &$conn->Execute($sql);

        if ($conn->Affected_Rows() > 0) {
            PHPST_Logger::logEvent(3, "User Operation", USER_USER_ID,
                    "User object deleted from DB.", __FILE__, __LINE__);

            // Now clean up the associated user_contacts table
            $sql = $this->SQLBuilder->deleteContact($this->getId());
            $rs = &$conn->Execute($sql);
            return true;
        } else {
            PHPST_Logger::logEvent(3, "DB Error", USER_USER_ID,
                    "User object could not be deleted", __FILE__, __LINE__);
            return false;
        }
    }

    /**
    * Updates a User in the Database
    *
    * @access public
    * @param int
    * @return boolean true if successful
    */
    public function updateDB() {
        $conn = &ADONewConnection(DSN);

        $this->getId();
        $Contact = null;
        $columns = $conn->MetaColumnNames(PHPST_User::$table);
        $sql = $this->SQLBuilder->update($this, $columns);

        $rs = &$conn->Execute($sql);

        if ($rs != false && $conn->Affected_Rows() > 0) {
            PHPST_Logger::logEvent(3, "User Operation", USER_USER_ID,
                    "User object updated in DB", __FILE__, __LINE__);
        } elseif($conn->Affected_Rows() == 0) {
            PHPST_Logger::logEvent(3, "User Operation", USER_USER_ID,
                    "User object note updated, no changes to perform", __FILE__, __LINE__);
        } else {
            PHPST_Logger::logEvent(3, "DB Error", USER_USER_ID,
                    "User object not updated, DB error", __FILE__, __LINE__);
            return false;
        }

        // Update department_users table if the user has a department_id
        if ($this->getInfo('department_id') > 0) {
            $sql = $this->SQLBuilder->updateDepartment_users($this->getId(),
                    $this->getInfo('department_id'));
            $rs = &$conn->Execute($sql);

            if ($rs != false) {
                PHPST_Logger::logEvent(3, "Department Operation", USER_USER_ID,
                        "User reference updated in department_users table", __FILE__, __LINE__);
            } else {
                PHPST_Logger::logEvent(3, "DB Error", USER_USER_ID,
                        "Failed to update department_users table", __FILE__, __LINE__);
                return false;
            }
        }
        return true;
    }

    /**
     * Implementation of the DatabaseMirror interface function getFromDB().
     * Returns a User object from the DB based on input in $fields array
     *
     * @static
     * @access public
     * @return Object User if successful, false otherwise
     */
    public static function getFromDB($fields) {
        if (is_array($fields)) {
            $myUser = null;

            $conn = &ADONewConnection(DSN);
            $conn->SetFetchMode(ADODB_FETCH_ASSOC);
            $SQLBuilder = new PHPST_UserSQLBuilder();

            $sql = $SQLBuilder->get($fields);

            if (!$sql) {
                PHPST_Logger::logEvent(3, "User Operation", USER_USER_ID,
                    "Wrong arguments passed to PHPST_User::getFromDB()", __FILE__, __LINE__);
            }

            $rs = &$conn->Execute($sql);

            if ($rs != false && $rs->RecordCount() > 0) {
                PHPST_Logger::logEvent(3, "User Operation", USER_USER_ID,
                        "User object retrieved from DB", __FILE__, __LINE__);
                $myUser = new PHPST_User($rs->fields);
            } else {
                PHPST_Logger::logEvent(3, "DB Error", USER_USER_ID,
                        "User object could not be retrieved from DB", __FILE__, __LINE__);
                return false;
            }
        } else {
            PHPST_Logger::logEvent(3, "User Operation", USER_USER_ID,
                    "Wrong argument type passed to PHPST_User::getFromDB()", __FILE__, __LINE__);
            return false;
        }

        return $myUser;
    }

    /**
    * Returns an array of user objects, fully instantiated with
    * department info and address info, using one large SQL query.
    * To do this, we get all the info we can from the query, and use
    * only object constructors (which do not open DB connections)
    *
    * @access public
    * @static
    *
    * @param string $user_type Admin or Mod
    * @param string $search The search term
    * @param string $field The field being searched
    * @param string $sort The field by which to sort the results
    * @param string $order The order in which to sort the results
    *
    * @return array of Users
    */
    public static function getArrayFromDB($user_type = "Mod", $search = "", $field = ""
            , $sort = "", $order = "", $max_records, $rs_page) {



        $conn = &ADONewConnection(DSN);
        $SQLBuilder = new PHPST_UserSQLBuilder();
        $users = array();
        $ids = array();

        if (!isset($rs_page)) {
            $rs_page = 1;
        }
        $users['count'] = 0;

        // Perform SQL for creating csv file
        $csv_sql = $SQLBuilder->getCSV();
        $csv_rs = $conn->Execute($csv_sql);
        $csv = rs2csv($csv_rs);
        $users['csv'] = $csv;

        // Get query for retrieving all users from DB
        $sql = $SQLBuilder->getFullUsersArray($user_type, $search, $field, $sort, $order);

        // Perform the query
        $rs = &$conn->Execute($sql);

        // If page requested is 'all', show all records on one page
        if ($rs_page == 'all') {
            $max_records = 999999;
            $rs_page = 1;
        }

        // Build Users array from query result
        if ($rs != false && $rs->RecordCount() > 0) {
           $user_count = 0;
           while (!$rs->EOF) {
               $row = $rs->fields;
               // die(print $rs->RecordCount());

               // Increment user count if this ticket is new
               if (!isset($ids[$row['user_id']])) {
                   $user_count++;
               }

               // Only add info to array if user is within pagination limits
               if ($user_count > (($rs_page - 1) * $max_records)
                        && $user_count <= ($rs_page * $max_records)) {

                   $user_fields = array();
                   $ticket_fields = array();
                   $department_fields = array();

                   foreach ($row as $field => $value) {
                       if (eregi('^ticket_', $field) && $value != null) {
                           $ticket_fields[substr($field, 7)] = $value;
                       } elseif (eregi('^department_', $field) && $value != null) {
                           $department_fields[substr($field, 11)] = $value;
                       } else {
                           $user_fields[$field] = $value;
                       }
                   }

                   if (count($ticket_fields) > 1) {
                       $users[$row['user_id']]['tickets'][] = $ticket_fields;
                   }
                   $users[$row['user_id']]['user'] = $user_fields;
                   $users[$row['user_id']]['department'] = $department_fields;
               }

               $ids[$row['user_id']] = 1;
               $rs->MoveNext();

               $users['count'] = $user_count;
           }

           return $users;
        } else {
            PHPST_Logger::logEvent(3, "DB Error", USER_USER_ID,
                    "SQL error trying to get Array of users: " . addslashes($sql), __FILE__, __LINE__);
            return false;
        }
    }

    /**
    * Returns a string of this user's descriptive information.
    *
    * @return string
    */
    public function getFullDescription() {
        return $this->getFname() . " "
                . $this->getLname();
    }

    /* ---- GETTERS ---- */

    /**
    * Returns the User's info chosen field.
    *
    * @return array
    * @access public
    * @param string field
    */
    public function getInfo($field) {
        // die(print_r($this->info));
        if (array_key_exists($field, $this->info)) {
            return $this->info[$field];
        } else {
            return null;
        }
    }

    /**
    * Returns the User's username.
    *
    * @return string
    * @access public
    */
    public function getUsername() {
        return $this->username;
    }

    /**
    * Returns the User's encrypted password.
    *
    * @return string
    * @access public
    */
    public function getPassword() {
        return $this->password;
    }

    /**
    * Returns the User's email address.
    *
    * @return string
    * @access public
    */
    public function getEmail() {
        return $this->email;
    }

    /**
    * Returns the User's last login UNIX Timestamp.
    * Needs formatting by a DateHandler class
    *
    * @return string
    * @access public
    */
    /*
    public function getLast_login() {
        return $this->last_login;
    }
    */

    /**
    * Returns the User's admin status.
    * This has 3 possible settings, determining what level of
    * control the User has. They are 'Admin',
    * 'Mod'and 'Client'.
    *
    * @return string
    * @access public
    */
    public function getAdmin() {
        return $this->admin;
    }

    /* ---- SETTERS ---- */

    /**
    * Sets the User's username.
    *
    * @return boolean True if successful, false otherwise.
    * @access public
    * @param string
    */
    public function setUsername($string) {
        // @todo Do some validation first
        $this->username = $string;
    }

    /**
    * Sets the User's info array.
    *
    * @return boolean True if successful, false otherwise.
    * @access public
    * @param array
    */
    public function setInfo($array) {
        // @todo Do some validation first
        $this->info = $array;
    }

    /**
    * Sets the User's encrypted password.
    *
    * @return boolean True if successful, false otherwise.
    * @access public
    * @param string
    */
    public function setPassword($string) {
        // @todo Do some validation first
        $this->password = $string;
    }

    /**
    * Sets the User's email address.
    *
    * @return boolean True if successful, false otherwise.
    * @access public
    * @param string
    */
    public function setEmail($string) {
        // @todo Do some validation first
        $this->email = $string;
    }

    /**
    * Sets the User's last login UNIX Timestamp.
    * Needs formatting by a DateHandler class
    *
    * @return boolean True if successful, false otherwise.
    * @access public
    * @param int
    */
    /*
    public function setLast_login($timestamp) {
        // @todo Do some validation first
        $this->last_login = $timestamp;
    }
    */

    /**
    * Sets the User's admin status.
    * This has 3 possible settings, determining what level of
    * control the User has. They are 'Admin',
    * 'Mod'and 'Client'.
    *
    * @return boolean True if successful, false otherwise.
    * @access public
    * #param string
    */
    public function setAdmin($string) {
        $this->admin = $string;
    }

    /**
    * Tostring function: returns a string representation of this object
    *
    * @access public
    * @return string
    */
    public function __toString() {
        $string = "User object:
            <br />___________________";
        $string .= "<br />id: " . $this->getId();
        $string .= "<br />Name: " . $this->getName();
        $string .= "<br />username: " . $this->getUsername();
        $string .= "<br />password: " . $this->getPassword();
        $string .= "<br />email: " . $this->getEmail();
        // $string .= "<br />Last Login: " . gmdate('d/m/Y H:i:s', $this->getLast_login());
        $string .= "<br />Admin Status: " . $this->getAdmin();
        return $string;
    }

}

?>