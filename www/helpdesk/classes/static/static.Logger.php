<?php
/**
 * File containing the Logger class.
 *
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version SVN: $Id: static.Logger.php 4 2005-12-13 01:47:15Z nicolas $
 * @since File available since Release 1.1.1.1
 * \\||
 */
/**
 * Short description of class Logger
 *
 * @access public
 * @package PHPSupportTickets
 */
class PHPST_Logger {
    // --- ATTRIBUTES ---
    /**
     * DB table
     *
     * @access public
     * @static
     * @var String
     */
    public static $table = DB_PREFIX_HISTORYLOG;

    /**
     * Valid types of events
     *
     * @access public
     * @static
     * @var array
     */
    public static $types = array(
        'Logging', 'Ticket Operation', 'Answer Operation',
        'Department Operation', 'User Operation', 'GUI Operation', 'DB Error', );

    // --- OPERATIONS ---
    /**
     * Logs an event into the DB log table
     *
     * @access public
     * @static
     * @return boolean true if successful
     *
     * @param int $priority 1 of 5 levels of priority, 5 being lowest importance
     * @param string $type Type of event (logging, create, delete, error etc...)
     * @param int $user_ID The ID of the user creating the event
     * @param string $message The event message, optional
     * @param string $file The file where the event occurred
     * @param int $line The line number at which the logging occurred
     */
    public static function logEvent($priority, $type,
            $user_id, $message, $file, $line) {
        $time = time();

        $ip = @$_SERVER['REMOTE_ADDR'];
        $referer = @$_SERVER['HTTP_REFERER'];
        // Validate fields first
        if (!is_numeric($priority) || $priority < 1 || $priority > 5) {
            return false;
        }
        if (!is_string($type) || $type == '' || !in_array($type, PHPST_Logger::$types)) {
            return false;
        }
        if (!is_numeric($user_id)) {
            return false;
        }
        if (!is_string($file) || $file == '') {
            return false;
        }

        $file = addslashes($file);

        // Log event in the Database
        $conn = &ADONewConnection(DSN);
        $sql = "INSERT INTO " . PHPST_Logger::$table . " SET
                priority = $priority,
                type = '$type',
                user_id = $user_id,
                message = '$message',
                file = '$file',
                timestamp = $time,
                ip = '$ip',
                referer = '$referer',
                line = '$line'";
        $rs = &$conn->Execute($sql);

        if ($rs != false) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * Returns an array of events from the DB based on a filter
     * The $filter must be in the format "AND field = value"
     *
     * @access public
     * @static
     * @return array of events
     *
     * @param string $filter An SQL filter to apply to the search
     * @param int $start A timestamp start to apply to the search
     * @param int $end A timestamp end to apply to the search
     */
    public static function getEvents($filter = '', $start, $end) {
         $events = array();

         $conn = &ADONewConnection(DSN);
         $sql = "SELECT * FROM " . PHPST_Logger::$table . " WHERE
                timestamp > $start AND timestamp < $end $filter";
         $rs = &$conn->Execute($sql);
         if ($rs != false && $rs->RecordCount() > 0) {
            for ($i = 0; $i < $rs->RecordCount(); $i++) {
                $events[] = $rs->fields;
                $rs->MoveNext();
            }
         } else {
            return false;
         }

         return $events;
    }

    /**
     * Deletes all events matching the search string
     *
     * @access public
     * @static
     * @return boolean true if successful
     *
     * @param string $filter An SQL filter to apply to the search
     * @param int $start A timestamp start to apply to the search
     * @param int $end A timestamp end to apply to the search
     */
    public static function cleanEvents($filter = '', $start, $end) {
         $conn = &ADONewConnection(DSN);
         $sql = "DELETE FROM " . PHPST_Logger::$table . " WHERE
                timestamp > $start AND timestamp < $end $filter";
         $rs = &$conn->Execute($sql);
         if ($rs != false) {
            return true;
         } elseif ($conn->Affected_Rows() == 0) {
            return "No logs to delete";
         } else {
            return false;
         }
    }

    public static function printEvent($filter, $start, $end) {

    }

    public static function getStats() {

    }
}
?>