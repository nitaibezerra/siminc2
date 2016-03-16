<?php
/**
 * File containing the Answer class.
 *
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version SVN: $Id: class.Answer.php 4 2005-12-13 01:47:15Z nicolas $
 * @since File available since Release 1.1.1.1
 * \\||
 */

/**
* user defined includes
* @include
*/
require_once_check(PHPST_PATH . 'classes/class.Note.php');
require_once_check(PHPST_PATH . 'classes/SQL/class.AnswerSQLBuilder.php');
require_once_check(PHPST_PATH . 'classes/static/static.Logger.php');
require_once_check(PHPST_PATH . 'classes/class.Answer.php');
/**
* user defined constants
* @ignore
*/

/**
* Answer class.
*
* @access public
* @package PHPSupportTickets
*/
class PHPST_Answer {
    // --- ATTRIBUTES ---

    /**
    * The database table to mirror this object
    *
    * @access public
    * @static
    * @var string
    */
    public static $table = DB_PREFIX_ANSWERS;

    /**
    * This answer's ID in the DB
    *
    * @access private
    * @var int $ID
    */
     private $id = null;

    /**
    * The ID of the user who created this answer.
    *
    * @access private
    * @var int $user_id
    */
     private $user_id = null;

    /**
    * The body of the Answer.
    *
    * @access private
    * @var string $body
    */
     private $body;

    /**
    * Answer's subject.
    *
    * @access private
    * @var String $subject
    */
     private $subject = null;

     /**
     * Timestamp of creation of this answer.
     *
     * @access private
     * @var int $timestamp
     */
     private $timestamp;

     /**
     * Ticket_id of Answer.
     *
     * @access private
     * @var int $ticket_id
     */
     private $ticket_id;

     /**
     * User's Rating of Answer.
     *
     * @access private
     * @var int $rating
     */
     private $rating;

     /**
     * Companion object used for building SQL queries. Separates
     * class responsibilities from SQL logic.
     *
     * @access public
     * @var object $SQLBuilder
     */
     public $SQLBuilder;

    // --- METHODS ---

    /**
    * Constructor: first call on parent class constructor.
    *
    * @access public
    * @param array $fields From a Posted form or a database row
    * @return void
    */
    public function __construct($fields) {
        $this->SQLBuilder = new PHPST_AnswerSQLBuilder();
        if (is_array($fields)) {
            try {
                extract($fields);
                $this->setUser_id($user_id);
                $this->setTimestamp($timestamp);
                $this->setBody($body);
                $this->setRating($rating);
                $this->setTicket_id($ticket_id);

                if (isset($id)) {
                    $this->setId($id);
                }
            } catch (Exception $e) {
                die($e->getTrace());
            }
        } else {
            PHPST_Logger::logEvent(3, "Answer Operation", null,
                    "Wrong argument type given to Answer::__construct", __FILE__, __LINE__);
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

            if (!isset($body) || trim($body) == '') {
                $errors[] = (array('message' => VALIDATE_EMPTY_BODY, 'field' => 'body'));
            }
            // Verify that required fields are not empty
            // @todo Implement validation

            if (count($errors) == 0) {
                PHPST_Logger::logEvent(3, "Answer Operation", null,
                        "Answer object created, passed validation", __FILE__, __LINE__);
                return true;
            } else {
                PHPST_Logger::logEvent(3, "Answer Operation", null,
                        "Answer object not created, failed validation", __FILE__, __LINE__);
                return $errors;
            }
        }
    }

    /**
    * Retrieves this Answer's id number based on a unique variable.
    * Also sets this object's id number to the retrieved int
    *
    * @return int Answer's id number
    */
    public function getId() {
        if (!isset($this->id)) {
            $conn = &ADONewConnection(DSN);
            $sql = $this->SQLBuilder->search(PHPST_Answer::$table, array("id"),
                array(DB_PREFIX_ANSWERS . ".timestamp" => $this->getTimestamp()));

            $rs = &$conn->Execute($sql);

            if ($rs != false && $rs->RecordCount() > 0) {
                $this->setId($rs->fields['id']);
                return $rs->fields['id'];
            } else {
                PHPST_Logger::logEvent(3, "DB Error", null,
                        "Could not retrieve Answer id: " . $this->getId(), __FILE__, __LINE__);
            }
        } else {
            return $this->id;
        }
    }

    /**
    * Short description of method add
    *
    * @access public
    * @return boolean
    */
    public function addToDB() {
        // Initialise Database object
        $conn = &ADONewConnection(DSN);

        // First check that Answer doesn't already exist.
        $sql = $this->SQLBuilder->search(PHPST_Answer::$table, array("id"),
                array(DB_PREFIX_ANSWERS . ".timestamp" => $this->getTimestamp(), "ticket_id" => $this->getTicket_Id()));
        $rs = &$conn->Execute($sql);

        if ($rs->RecordCount() == 0) {
            // Retrieve array of table's column names (always UPPERCASE)
            $columns = $conn->MetaColumnNames(PHPST_Answer::$table);

            $sql = $this->SQLBuilder->insert($this, $columns);
            $rs = &$conn->Execute($sql);

            // Verify previous query
            if ($rs != false) {
                $id = $conn->Insert_ID();
                PHPST_Logger::logEvent(3, "Answer Operation", null,
                        "Answer object added to DB", __FILE__, __LINE__);
                return $id;
            } else {
                PHPST_Logger::logEvent(3, "DB Error", null,
                        "Could not Insert Answer object.", __FILE__, __LINE__);
                return false;
            }
        } else {
            PHPST_Logger::logEvent(3, "Answer Operation", null,
                        "Answer object not added to DB, username already exists", __FILE__, __LINE__);
            return false;
        }
    }

    /**
    * Removes a Answer from the Database
    *
    * @access public
    * @param int $ID
    * @return boolean true if successful
    */
    public function removeFromDB($id) {
        $conn = &ADONewConnection(DSN);
        $sql = $this->SQLBuilder->delete($id);
        $conn->Execute($sql);

        if ($conn->Affected_Rows() > 0) {
            PHPST_Logger::logEvent(3, "Answer Operation", null,
                    "Answer object deleted from DB.", __FILE__, __LINE__);
            return true;
        } else {
            PHPST_Logger::logEvent(3, "DB Error", null,
                    "Answer object could not be deleted", __FILE__, __LINE__);
            return false;
        }
    }

    /**
     * Implementation of the DatabaseMirror interface function getFromDB().
     * Returns a single Answer object from the DB based on input in $fields array
     *
     * @static
     * @access public
     * @return Object Answer if successful, false otherwise
     */
    public static function getFromDB($fields) {
        if (is_array($fields)) {
            $myAnswer = null;

            $conn = &ADONewConnection(DSN);
            $conn->SetFetchMode(ADODB_FETCH_ASSOC);
            $SQLBuilder = new PHPST_AnswerSQLBuilder();
            $sql = $SQLBuilder->get($fields);

            if (!$sql) {
                PHPST_Logger::logEvent(3, "Answer Operation", null,
                    "Wrong arguments passed to Answer::getFromDB()", __FILE__, __LINE__);
            }

            $rs = &$conn->Execute($sql);

            if ($rs != false && $rs->RecordCount() > 0) {
                PHPST_Logger::logEvent(3, "Answer Operation", null,
                        "Answer object retrieved from DB", __FILE__, __LINE__);
                $myAnswer = new PHPST_Answer($rs->fields);
            } else {
                PHPST_Logger::logEvent(3, "DB Error", null,
                        "Answer object could not be retrieved from DB", __FILE__, __LINE__);
                return false;
            }
        } else {
            Logger::logEvent(3, "Answer Operation", null,
                    "Wrong argument type passed to Answer::getFromDB()", __FILE__, __LINE__);
            return false;
        }
        return $myAnswer;
    }

    /**
    * Returns an array of Answer objects
    *
    * @static
    * @access public
    *
    * @param string $user_type Admin or Mod
    * @param string $search The search term
    * @param string $field The field being searched
    * @param string $sort The field by which to sort the results
    * @param string $order The order in which to sort the results
    *
    * @return array of Answers
    */
    public static function getArrayFromDB($user_type = "Mod", $search = "", $field = ""
            , $sort = "", $order = "") {
        $conn = &ADONewConnection(DSN);
        $conn->SetFetchMode(ADODB_FETCH_ASSOC);
        $answers = array();
        $ids = array();

        $SQLBuilder = new PHPST_AnswerSQLBuilder();

        // Get query from SQLBuilder object
        $sql = $SQLBuilder->getFullAnswersArray($user_type,
                $search, $field, $sort, $order);

        // Perform the query
        $rs = &$conn->Execute($sql);

        // Build answers array from query result (use pagination)
        if ($rs != false && $rs->RecordCount() > 0) {
            $answer_count = 0;
            while (!$rs->EOF) {
                $row = $rs->fields;
                // Increment answer count if this answer is new
                if (!isset($ids[$row['answer_id']])) {
                    $answer_count++;
                }

                // prepare arrays
                $answer_fields = array();
                $user_fields = array();
                $ticket_fields = array();
                $department_fields = array();

                // Increment 'recent' index count if answer within 1 week
                if ($row['answer_timestamp'] >= (time() - 604860)) {
                    $answers[$row['answer_id']]['recent'] = true;
                } else {
                    $answers[$row['answer_id']]['recent'] = false;
                }

                foreach ($row as $field => $value) {
                    if (preg_match('/^user_/', $field) > 0) {
                        $user_fields[substr($field, 5)] = $value;
                    } elseif (preg_match('/^ticket_/', $field) > 0 && $value != null) {
                        $ticket_fields[substr($field, 7)] = $value;
                    } elseif (preg_match('/^department_/', $field) > 0) {
                        $department_fields[substr($field, 11)] = $value;
                    } elseif (preg_match('/^answer_/', $field) > 0) {
                        $answer_fields[substr($field, 7)] = $value;
                    }
                }

                $answers[$row['answer_id']]['ticket'] = $ticket_fields;
                $answers[$row['answer_id']]['answer'] = $answer_fields;
                $answers[$row['answer_id']]['user'] = $user_fields;
                $answers[$row['answer_id']]['department'] = $department_fields;
                $ids[$row['answer_id']] = 1;
                $answers['count'] = $answer_count;
                $rs->MoveNext();
            }
            return $answers;
        } else {
            PHPST_Logger::logEvent(3, "DB Error", null,
                    "SQL error trying to get Array of users: " . addslashes($sql), __FILE__, __LINE__);
            return false;
        }
    }

    /* ---- GETTERS ---- */

    /**
    * Returns the Answer's user id.
    *
    * @return int user_id
    * @access public
    */
    public function getUser_id() {
        return $this->user_id;
    }

    /**
    * Returns the Answer's subject.
    *
    * @return string Subject
    * @access public
    */
    public function getSubject() {
        return $this->subject;
    }

    /**
    * Returns the Answer's creation timestamp.
    *
    * @return int Timestamp of creation
    * @access public
    */
    public function getTimestamp() {
        return $this->timestamp;
    }

    /**
    * Returns the Answer's ticket_id.
    *
    * @return int ticket_id
    * @access public
    */
    public function getTicket_id() {
        return $this->ticket_id;
    }

    /**
    * Returns the Answer's rating (open or closed).
    *
    * @return string Rating
    * @access public
    */
    public function getRating() {
        return $this->rating;
    }

    /**
    * Returns the Answer's body.
    *
    * @return string Body
    * @access public
    */
    public function getBody() {
        return $this->body;
    }

    /* ---- SETTERS ---- */

    /**
    * Sets the Answer's id.
    *
    * @param int $id
    * @access public
    */
    public function setId($id) {
        $this->id = $id;
    }

    /**
    * Sets the Answer's user id.
    *
    * @param int $user_id
    * @access public
    */
    public function setUser_id($user_id) {
        $this->user_id = $user_id;
    }

    /**
    * Sets the Answer's subject.
    *
    * @param string $subject
    * @access public
    */
    public function setSubject($subject) {
        $this->subject = $subject;
    }

    /**
    * Sets the Answer's creation timestamp.
    *
    * @param int $timestamp
    * @access public
    */
    public function setTimestamp($timestamp) {
        $this->timestamp = $timestamp;
    }

    /**
    * Sets the Answer's ticket_id.
    *
    * @param int $ticket_id
    * @access public
    */
    public function setTicket_id($ticket_id) {
        $this->ticket_id = $ticket_id;
    }

    /**
    * Sets the Answer's rating (open or closed).
    *
    * @param string $rating
    * @access public
    */
    public function setRating($rating) {
        $this->rating = $rating;
    }

    /**
    * Sets the Answer's body.
    *
    * @param string $body
    * @access public
    */
    public function setBody($body) {
        $this->body = $body;
    }

    /**
     * Returns info about this answer's attachment, or false if none exists.
     *
     * @static
     * @access public
     * @param string $username The username of the answer's owner.
     * @param int $answer_id The answer's ID
     * @return mixed Array with attachment info if such exists, false otherwise.
     *
     */
    public static function getAttachmentInfo($username, $answer_id) {
        $attachment_info = array();
        $answer_attachment_path = PHPST_UPLOAD_PATH . $username
                . '/phpst_answer_' . $answer_id;

        if (is_dir($answer_attachment_path)) {
            $attachment_info['path'] = $answer_attachment_path;
            $contents = scandir($attachment_info['path']);
            $attachment_info['filename'] = $contents[2];
            $attachment_info['url'] = PHPST_UPLOAD_RELATIVE_PATH . $username
                    . '/phpst_answer_' . $answer_id . '/' . $attachment_info['filename'];
            $attachment_info['size'] =
                PHPST_StringFormat::filesize_format(
                    filesize($attachment_info['path'] . '/' . $attachment_info['filename']));
            $attachment_info['image'] = PHPST_IconGUI::getIcon('save', '16', 'images/icons/', $attachment_info['url']);

            return $attachment_info;
        } else {
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
        $string = "Answer object:
            <br />___________________";
        $string .= "<br />id: " . $this->getId();
        $string .= "<br />Subject: " . $this->getSubject();
        $string .= "<br />Body: " . $this->getBody();
        $string .= "<br />User_id: " . $this->getUser_id();
        $string .= "<br />Ticket_id: " . $this->getTicket_id();
        $string .= "<br />Date Created: " . gmdate('d/m/Y H:i:s', $this->getTimestamp());
        $string .= "<br />Rating: " . $this->getRating();
        return $string;
    }
}

?>