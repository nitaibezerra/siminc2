<?php
/**
 * File containing the Ticket class.
 *
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version SVN: $Id: class.Ticket.php 4 2005-12-13 01:47:15Z nicolas $
 * @since File available since Release 1.1.1.1
 * \\||
 */

/**
 * user defined includes
 *
 * @include
 */
require_once_check(PHPST_PATH . 'classes/class.Note.php');
require_once_check(PHPST_PATH . 'classes/SQL/class.TicketSQLBuilder.php');
require_once_check(PHPST_PATH . 'classes/static/static.Logger.php');
require_once_check(PHPST_PATH . 'classes/class.Answer.php');
/**
 * user defined constants
 *
 * @ignore
 */

/**
 * Ticket class.
 *
 * @access public
 * @package PHPSupportTickets
 */
class PHPST_Ticket {
    // --- ATTRIBUTES ---
    /**
     * The database table to mirror this object
     *
     * @access public
     * @static
     * @var string
     */
    public static $table = DB_PREFIX_TICKETS;

    /**
     * This ticket's ID in the DB
     *
     * @access private
     * @var int $id
     */
    private $id = null;

    /**
     * The ID of the user who created this ticket.
     *
     * @access private
     * @var int $user_id
     */
    private $user_id = null;

    /**
     * The body of the Ticket.
     *
     * @access private
     * @var string $body
     */
    private $body;

    /**
     * Ticket's subject.
     *
     * @access private
     * @var String $subject
     */
    private $subject = null;

    /**
     * Ticket's status, open or closed
     *
     * @access private
     * @var String $status
     */
    private $status = 'open';

    /**
     * Urgency level of the Ticket, from 1 to 4
     *
     * @access public
     * @var int
     */
    private $urgency = 1;

    /**
     * Ticket's category
     *
     * @access private
     * @var string $category
     */
    private $category = null;

    /**
     * Timestamp of creation of this ticket.
     *
     * @access private
     * @var int $timestamp
     */
    private $timestamp;

    /**
     * Department_id of Ticket.
     *
     * @access private
     * @var int $department_id
     */
    private $department_id;

    /**
     * Ticket's array of Answer objects;
     *
     * @access private
     * @var array $answers
     */
    private $answers = array();

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
        $this->SQLBuilder = new PHPST_TicketSQLBuilder();
        if (is_array($fields) && count($fields) > 0) {
            extract($fields);
            $this->setUser_id($user_id);
            $this->setSubject($subject);
            $this->setTimestamp($timestamp);
            $this->setBody($body);
            $this->setStatus($status);
            $this->setUrgency($urgency);
            $this->setDepartment_id($department_id);
            if (isset($id)) {
                $this->setId($id);
            }
        } else {
            PHPST_Logger::logEvent(3, "Ticket Operation", null,
                "Wrong argument type given to Ticket::__construct", __FILE__, __LINE__);
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
     *        message and name of field if error is encountered.
     */
    public static function validate($fields) {
        if (is_array($fields)) {
            extract($fields);
            $errors = array();
            // Verify that required fields are not empty
            if ($subject == "" || is_null($subject)) {
                $errors[] = array("field" => "subject",
                    "message" => VALIDATE_EMPTY_SUBJECT);
            }

            if ($body == "" || is_null($body)) {
                $errors[] = array("field" => "body",
                    "message" => VALIDATE_EMPTY_BODY);
            }

            if (count($errors) == 0) {
                PHPST_Logger::logEvent(3, "Ticket Operation", null,
                    "Ticket object created, passed validation", __FILE__, __LINE__);
                return true;
            } else {
                $errors[] = array("field" => "body",
                    "message" => 'not good!');
                PHPST_Logger::logEvent(3, "Ticket Operation", null,
                    "Ticket object not created, failed validation", __FILE__, __LINE__);
                return $errors;
            }
        }
    }

    /**
     * Retrieves this Ticket's ID number based on a unique variable.
     * Also sets this object's ID number to the retrieved int
     *
     * @return int Ticket's ID number
     */
    public function getId() {
        if (!isset($this->id)) {
            $conn = &ADONewConnection(DSN);
            $sql = $this->SQLBuilder->search(PHPST_Ticket::$table, array('id'),
                array(DB_PREFIX_TICKETS . ".timestamp" => $this->getTimestamp()));

            $rs = &$conn->Execute($sql);

            if ($rs != false && $rs->RecordCount() > 0) {
                $this->setId($rs->fields['id']);
                return $rs->fields['id'];
            } else {
                PHPST_Logger::logEvent(3, "DB Error", null,
                    "Could not retrieve Ticket ID", __FILE__, __LINE__);
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
        // First check that Ticket doesn't already exist.
        $sql = $this->SQLBuilder->search(PHPST_Ticket::$table, array('id'),
            array(DB_PREFIX_TICKETS . ".timestamp" => $this->getTimestamp()));
        $rs = &$conn->Execute($sql);

        if ($rs->RecordCount() == 0) {
            // Retrieve array of table's column names (always UPPERCASE)
            $columns = $conn->MetaColumnNames(PHPST_Ticket::$table);

            $sql = $this->SQLBuilder->insert($this, $columns);

            $rs = &$conn->Execute($sql);
            // Verify previous query
            if ($rs != false) {
                $id = $conn->Insert_ID();
                PHPST_Logger::logEvent(3, "Ticket Operation", null,
                    "Ticket object added to DB", __FILE__, __LINE__);
                return $id;
            } else {

                PHPST_Logger::logEvent(3, "DB Error", null,
                    "Could not Insert Ticket object.", __FILE__, __LINE__);
                return false;
            }
        } else {
            PHPST_Logger::logEvent(3, "Ticket Operation", null,
                "Ticket object not added to DB, username already exists", __FILE__, __LINE__);
            return false;
        }
    }

    /**
     * Removes a Ticket from the Database
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
            PHPST_Logger::logEvent(3, "Ticket Operation", null,
                "Ticket object deleted from DB.", __FILE__, __LINE__);
            return true;
        } else {
            PHPST_Logger::logEvent(3, "DB Error", null,
                "Ticket object could not be deleted", __FILE__, __LINE__);
            return false;
        }
    }

    /**
     * Updates a Ticket in the Database
     *
     * @access public
     * @param int $
     * @return boolean true if successful
     */
    public function updateDB() {
        $conn = &ADONewConnection(DSN);
        $this->getId();

        $columns = $conn->MetaColumnNames(PHPST_Ticket::$table);
        $sql = $this->SQLBuilder->update($this, $columns);

        $rs = &$conn->Execute($sql);

        if ($rs != false && $conn->Affected_Rows() > 0) {
            PHPST_Logger::logEvent(3, "Ticket Operation", USER_USER_ID,
                "Ticket object updated in DB", __FILE__, __LINE__);
        } elseif ($conn->Affected_Rows() == 0) {
            PHPST_Logger::logEvent(3, "Ticket Operation", USER_USER_ID,
                "Ticket object note updated, no changes to perform", __FILE__, __LINE__);
        } else {
            PHPST_Logger::logEvent(3, "DB Error", USER_USER_ID,
                "Ticket object not updated, DB error", __FILE__, __LINE__);
            return false;
        }

        return true;
    }

    /**
     * Implementation of the DatabaseMirror interface function getFromDB().
     * Returns a single Ticket object from the DB based on input in $fields array
     *
     * @static
     * @access public
     * @return Object Ticket if successful, false otherwise
     */
    public static function getFromDB($fields) {

        if (is_array($fields)) {
            $myTicket = null;

            $conn = &ADONewConnection(DSN);
            $conn->SetFetchMode(ADODB_FETCH_ASSOC);
            $SQLBuilder = new PHPST_TicketSQLBuilder();
            $sql = $SQLBuilder->get($fields);

            if (!$sql) {
                PHPST_Logger::logEvent(3, "Ticket Operation", null,
                    "Wrong arguments passed to Ticket::getFromDB()", __FILE__, __LINE__);
            }

            $rs = &$conn->Execute($sql);

            if ($rs != false && $rs->RecordCount() > 0) {
                PHPST_Logger::logEvent(3, "Ticket Operation", null,
                    "Ticket object retrieved from DB", __FILE__, __LINE__);
                $myTicket = new PHPST_Ticket($rs->fields);

                // Add answers to object
                $answers = PHPST_Answer::getArrayFromDB('Admin', $myTicket->getId(),
                        DB_PREFIX_ANSWERS . '.ticket_id', 'A.timestamp', 'DESC');
                if (!empty($answers)) {
                    foreach($answers as $id => $answer_fields) {
                        if ($id != 'count' && $id != 'csv') {
                            $answer = new PHPST_Answer($answer_fields['answer']);
                            $myTicket->addAnswer($answer);
                        }
                    }
                }

            } else {
                PHPST_Logger::logEvent(3, "DB Error", null,
                    "Ticket object could not be retrieved from DB", __FILE__, __LINE__);
                return false;
            }
        } else {
            PHPST_Logger::logEvent(3, "Ticket Operation", null,
                "Wrong argument type passed to Ticket::getFromDB()", __FILE__, __LINE__);
            return false;
        }
        return $myTicket;
    }

    /**
     * Returns an array of Ticket objects
     *
     * @access public
     * @param string $user_type Admin or Mod
     * @param string $search The search term
     * @param string $field The field being searched
     * @param string $sort The field by which to sort the results
     * @param string $order The order in which to sort the results
     * @param string $ticket_type 'department' tickets or 'my' tickets
     * @return array of Tickets
     */
    public static function getArrayFromDB($user_type, $search, $field,
        $sort, $order, $max_records, $rs_page, $department_id, $ticket_type) {
        $conn = &ADONewConnection(DSN);
        $tickets = array();
        $ids = array();

        $SQLBuilder = new PHPST_TicketSQLBuilder();
        if (!isset($rs_page)) {
            $rs_page = 1;
        }
        $tickets['count'] = 0;


        // Perform SQL for creating csv file
        $csv_sql = $SQLBuilder->getCSV();
        $csv_rs = $conn->Execute($csv_sql);
        $csv = rs2csv($csv_rs);
        $tickets['csv'] = $csv;

        // If page requested is 'all', show all records on one page
        if ($rs_page == 'all') {
            $max_records = 999999;
            $rs_page = 1;
        }

        // Get query from SQLBuilder object
        $sql = $SQLBuilder->getFullTicketsArray($user_type,
            $search, $field, $sort, $order, $department_id, $ticket_type);

        // Perform the query
        $rs = &$conn->Execute($sql);

        // Build tickets array from query result (use pagination)
        if ($rs != false && $rs->RecordCount() > 0) {
            $ticket_count = 0;
            while (!$rs->EOF) {
                $row = $rs->fields;
                // Increment ticket count if this ticket is new
                if (!isset($ids[$row['id']])) {
                    $ticket_count++;
                }
                // Only add info to array if ticket is within pagination limits
                if ($ticket_count > (($rs_page - 1) * $max_records) && $ticket_count <= ($rs_page * $max_records)) {
                    // prepare arrays
                    $ticket_fields = array();
                    $user_fields = array();
                    $answer_fields = array();
                    $department_fields = array();

                    // Increment 'recent' index count if ticket within the predefined number of days
                    if ($row['timestamp'] >= (time() - OPTION_RECENT_TICKETS_DAYS * 86400)) {
                        $tickets[$row['id']]['recent'] = true;
                    } else {
                        $tickets[$row['id']]['recent'] = false;
                    }

                    foreach ($row as $field => $value) {
                        // echo "$field : $value <br />";
                        if (preg_match('/^user_/', $field) > 0) {
                            $user_fields[substr($field, 5)] = $value;
                        } elseif (preg_match('/^answer_/', $field) > 0 && $value != null) {
                            $answer_fields[substr($field, 7)] = $value;
                        } elseif (preg_match('/^department_/', $field) > 0) {
                            $department_fields[substr($field, 11)] = $value;
                        } else {
                            $ticket_fields[$field] = $value;
                        }
                    }

                    if (count($answer_fields) > 1) {
                        $tickets[$row['id']]['answers'][] = $answer_fields;
                    }

                    $tickets[$row['id']]['ticket'] = $ticket_fields;
                    $tickets[$row['id']]['user'] = $user_fields;
                    $tickets[$row['id']]['department'] = $department_fields;
                }
                $ids[$row['id']] = 1;
                $tickets['count'] = $ticket_count;
                $rs->MoveNext();
            }
            return $tickets;
        } else {
            PHPST_Logger::logEvent(3, "DB Error", null,
                "SQL error trying to get Array of users: " . addslashes($sql), __FILE__, __LINE__);
            return false;
        }
    }

    /* ---- GETTERS ---- */

    /**
     * Returns the Ticket's user ID.
     *
     * @return int user_id
     * @access public
     */
    public function getUser_id() {
        return $this->user_id;
    }

    /**
     * Returns the Ticket's subject.
     *
     * @return string Subject
     * @access public
     */
    public function getSubject() {
        return $this->subject;
    }

    /**
     * Returns the Ticket's creation timestamp.
     *
     * @return int Timestamp of creation
     * @access public
     */
    public function getTimestamp() {
        return $this->timestamp;
    }

    /**
     * Returns the Ticket's department_id.
     *
     * @return int department_id
     * @access public
     */
    public function getDepartment_id() {
        return $this->department_id;
    }

    /**
     * Returns the Ticket's status (open or closed).
     *
     * @return string Status
     * @access public
     */
    public function getStatus() {
        return $this->status;
    }

    /**
     * Returns the Ticket's urgency level (1 - 4).
     *
     * @return int Urgency level
     * @access public
     */
    public function getUrgency() {
        return $this->urgency;
    }

    /**
     * Returns the Ticket's category.
     *
     * @return string Category
     * @access public
     */
    public function getCategory() {
        return $this->category;
    }

    /**
     * Returns the Ticket's body.
     *
     * @return string Body
     * @access public
     */
    public function getBody() {
        return $this->body;
    }

    /**
     * Returns the Ticket's answers.
     *
     * @return array answers
     * @access public
     */
    public function getAnswers() {
        return $this->answers;
    }

    /* ---- SETTERS ---- */

    /**
     * Sets the Ticket's ID.
     *
     * @param int $id
     * @access public
     */
    public function setId($id) {
        $this->id = $id;
    }

    /**
     * Sets the Ticket's user ID.
     *
     * @param int $user_id
     * @access public
     */
    public function setUser_id($user_id) {
        $this->user_id = $user_id;
    }

    /**
     * Sets the Ticket's subject.
     *
     * @param string $subject
     * @access public
     */
    public function setSubject($subject) {
        $this->subject = $subject;
    }

    /**
     * Sets the Ticket's creation timestamp.
     *
     * @param int $timestamp
     * @access public
     */
    public function setTimestamp($timestamp) {
        $this->timestamp = $timestamp;
    }

    /**
     * Sets the Ticket's department_id.
     *
     * @param int $department_id
     * @access public
     */
    public function setDepartment_id($department_id) {
        $this->department_id = $department_id;
    }

    /**
     * Sets the Ticket's status (open or closed).
     *
     * @param string $status
     * @access public
     */
    public function setStatus($status) {
        $this->status = $status;
    }

    /**
     * Sets the Ticket's urgency level (1 - 4).
     *
     * @param int $urgency
     * @access public
     */
    public function setUrgency($urgency) {
        $this->urgency = $urgency;
    }

    /**
     * Sets the Ticket's category.
     *
     * @param string $category
     * @access public
     */
    public function setCategory($category) {
        $this->category = $category;
    }

    /**
     * Sets the Ticket's body.
     *
     * @param string $body
     * @access public
     */
    public function setBody($body) {
        $this->body = $body;
    }

    /**
     * Adds an answer to the Ticket.
     *
     * @param object $answers
     * @access public
     */
    public function addAnswer(PHPST_Answer $answer) {
        $this->answers[] = $answer;
    }

    /**
     * Returns a formatted thread of responses to this ticket
     * ready for inclusion in email body.
     *
     * @access public
     * @return string
     *
     */
    public function getThread() {
        $user = PHPST_User::getFromDB(array('user_id' => $this->getUser_id()));
        $ticket_attachment = PHPST_Ticket::getAttachmentInfo($user->getUsername(), $this->getId());

        $string = "\nTicket's initial message:\n";
        $string .= "\n/--------------------------------------------------";
        $body = html_entity_decode($this->getBody());
        $body = str_replace("\n", "\n| ", $body);
        $body = str_replace('<br />', "\n| ", $body);
        $body = wordwrap($body, 48, "\n| ");
        $string .= "\n| " . $body;
        $string .= "\n| -------------------------------------------------";
        $string .= "\n| By " . $user->getName() . " (" . $user->getEmail() . ").";
        $string .= "\n| On " . PHPST_DateTime::unix2mysql($this->getTimestamp(), 'UK') . ".";
        if ($ticket_attachment) {
            $string .= "\n| Attachment: " . $ticket_attachment['filename'] . " (" .
                       $ticket_attachment['size'] . ", " . $ticket_attachment['url'] . ").";
        }
        $string .= "\n\\--------------------------------------------------\n";
        $string .= "\n\nResponses to this ticket (most recent on top):\n";

        foreach ($this->getAnswers() as $id => $answer) {
            // Get attachment info
            $user = PHPST_User::getFromDB(array('user_id' => $answer->getUser_id()));
            $answer_attachment = PHPST_Answer::getAttachmentInfo($user->getUsername(), $answer->getId());

            // Break up the body of each answer into multiple lines
            $body = html_entity_decode($answer->getBody());
            $body = str_replace("\n", "\n| ", $body);
            $body = str_replace('<br />', "\n| ", $body);
            $body = wordwrap($body, 48, "\n| ");
            $string .= "\n/--------------------------------------------------";
            $string .= "\n| " . $body;
            $string .= "\n| -------------------------------------------------";
            $string .= "\n| By " . $user->getName() . " (" . $user->getEmail() . ").";
            $string .= "\n| On " . PHPST_DateTime::unix2mysql($answer->getTimestamp(), 'UK') . ".";
            if ($answer_attachment) {
                $string .= "\n| Attachment: " . $answer_attachment['filename'] . " (" .
                           $answer_attachment['size'] . ", " . $answer_attachment['url'] . ").";
            }
            $string .= "\n\\--------------------------------------------------\n";
        }
        return $string;
    }

    /**
     * Returns info about this ticket's attachment, or false if none exists.
     *
     * @static
     * @access public
     * @param string $username The username of the ticket's owner.
     * @param int $ticket_id The ticket's ID
     * @return mixed Array with attachment info if such exists, false otherwise.
     *
     */
    public static function getAttachmentInfo($username, $ticket_id) {
        $attachment_info = array();
        $ticket_attachment_path = PHPST_UPLOAD_PATH . $username
                . '/phpst_ticket_' . $ticket_id;

        if (is_dir($ticket_attachment_path)) {
            $attachment_info['path'] = $ticket_attachment_path;
            $contents = scandir($attachment_info['path']);
            $attachment_info['filename'] = $contents[2];
            $attachment_info['url'] = PHPST_UPLOAD_RELATIVE_PATH . $username
                    . '/phpst_ticket_' . $ticket_id . '/' . $attachment_info['filename'];
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
    * Reformats a full tickets array obtained through Ticket::getArrayFrom DB,
    * removes all tickets whose department is suspended, and returns that tickets array.
    *
    * @static
    * @access public
    * @param array $ticketw
    * @return int
    */
    public static function getActiveTickets($tickets) {
        $count = 0;
        $copy = $tickets;

        if (!empty($tickets)) {

            foreach ($tickets as $key => $ticket) {
                if ($key != 'count' && $key != 'csv' && $ticket['department']['status'] != 'Suspended') {
                    $count++;
                } else {
                    unset($copy[$key]);
                }
            }
            $copy['count'] = $count;
            $copy['csv'] = $tickets['csv'];
        }

        return $copy;
    }

    /**
    * Tostring function: returns a string representation of this object
    *
    * @access public
    * @return string
    */
    public function __toString() {
        $string = "Ticket object:
            <br />___________________";
        $string .= "<br />id: " . $this->getId();
        $string .= "<br />Department: " . $this->getDepartment_id();
        $string .= "<br />Body: " . $this->getBody();
        $string .= "<br />User_id: " . $this->getUser_id();
        $string .= "<br />Urgency: " . $this->getUrgency();
        $string .= "<br />Date Created: " . gmdate('d/m/Y H:i:s', $this->getTimestamp());
        $string .= "<br />Status: " . $this->getStatus();
        $string .= "<br />Answers: <ul>";
        foreach ($this->getAnswers() as $id => $answer) {
            $string .= "<li>" . $answer->getUser_id() . ": " . $answer->getBody() . "</li>";
        }
        $string .= "</ul>";
        return $string;
    }
}

?>