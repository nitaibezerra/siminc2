<?php
/**
 * File containing the AnswerSQLBuilder class.
 *
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version SVN: $Id: class.AnswerSQLBuilder.php 4 2005-12-13 01:47:15Z nicolas $
 * @since File available since Release 1.1.1.1
 * \\||
 */


/**
* answer defined includes
*/
require_once_check(PHPST_PATH . 'classes/SQL/abstract.SQLBuilder.php');

/**
* answer defined constants
* @ignore
*/

/**
* This is a class with static functions used for creating SQL queries
* for the answers table
*
* @package PHPSupportTickets
*/
class PHPST_AnswerSQLBuilder extends PHPST_SQLBuilder{

    /**
    * Constructor.
    */
    public function __construct() {
        parent::__construct();
    }

    /**
    * Creates a query for Inserting a new Answer into the DB.
    *
    * @access public
    * @param object $answer The Answer to insert
    * @param array $columns The columns of the table in which to insert the Answer
    * @return string $query
    */
    public function insert($answer, $columns) {
        $query = "INSERT INTO `" . DB_PREFIX_ANSWERS . "` SET ";

        // We use the DB columns as a basis for this loop, to avoid DB errors
        foreach ($columns as $k => $v) {
            if ($v != "id") {
                $value = eval('return nl2br(htmlentities($answer->get' . ucfirst($v) . "(), ENT_QUOTES));");
                $query .= "$v = '$value',";
            }
        }

        // Trim last comma
        $query = rtrim($query,",");
        return $query;
    }

    /**
    * Creates a query for deleting Answer from the DB
    *
    * @access public
    * @param int $answer_id The answer's id
    * @return string $query
    */
    public function delete($answer_id) {
        return "DELETE FROM `" . DB_PREFIX_ANSWERS . "` WHERE id = '$answer_id'";
    }

    /**
    * Creates a query for updating a Answer record in the DB from
    * the given Answer PHP object's data.
    *
    * @access public
    * @param object $answer
    * @param array $columns The table's columns
    * @return string $query
    */
    public function update($answer, $columns) {
        $query = "UPDATE `" . DB_PREFIX_ANSWERS . "` SET ";

        // We use the DB columns as a basis for this loop, to avoid DB errors
        foreach ($columns as $k => $v) {
            if ($v != "id") {
                $value = eval('return $answer->get' . ucfirst($v) . "();");
                $query .= "$v = '$value',";
            }
        }
        // Trim last comma
        $query = rtrim($query, ",");
        $query .= " WHERE `id` = " . $answer->getId(). "";
        return $query;
    }

    /**
    * Creates a query for retrieving all data about this Answer from the DB,
    * including all data from other tables linked through foreign keys.
    *
    * @access public
    * @param array An array of fields used to customise the query
    * @return string $query
    */
    public function get($fields) {
        $where = "";
        $myAnswer = null;
        $filter = "      A.`id` >= 0
                     AND A.`ticket_id` = B.id
                     AND A.`user_id` = C.user_id
                     AND B.`department_id` = D.id ";
        $query = "SELECT A.`id`,
                         A.`user_id`,
                         A.`ticket_id`,
                         A.`body`,
                         A.`timestamp`,
                         A.`rating`,
                         A.`subject`,

                         C.`" . DB_PREFIX_USER_ID . "` AS 'user_id',
                         C.`" . DB_PREFIX_USER_NAME . "` AS 'user_name',
                         C.`" . DB_PREFIX_USER_USERNAME . "` AS 'user_username',
                         C.`" . DB_PREFIX_USER_PASSWORD . "` AS 'user_password',
                         C.`" . DB_PREFIX_USER_ADMIN . "` AS 'user_admin',

                         B.`user_id` AS 'ticket_user_id',
                         B.`id` AS 'ticket_id',
                         B.`body` AS 'ticket_body',
                         B.`subject` AS 'ticket_subject',
                         B.`timestamp` AS 'ticket_timestamp',
                         B.`status` AS 'ticket_status',
                         B.`urgency` AS 'ticket_urgency',
                         B.`category` AS 'ticket_category',
                         B.`department_id` AS 'ticket_department_id',

                         D.`id` AS 'department_id',
                         D.`name` AS 'department_name',
                         D.`description` AS 'department_description',
                         D.`status` as 'department_status'

                    FROM `" . DB_PREFIX_ANSWERS . "` AS A,
                         `" . DB_PREFIX_TICKETS . "` AS B,
                         `" . DB_PREFIX_USER . "` AS C,
                         `" . DB_PREFIX_DEPARTMENTS . "` AS D
                   WHERE ";

        if (array_key_exists('timestamp', $fields)) {
            $timestamp = $fields['timestamp'];
            $filter .= " AND A.`timestamp` = '$timestamp' ";
        } elseif (array_key_exists('id', $fields)) {
            $id = $fields['id'];
            $filter .= " AND A.`id` = '$id' ";
        } else {
            return false;
        }

        $query .= $filter;
        return $query;
    }

    /**
    * Creates a query for retrieving all data about a number of Answers from the DB,
    * including all data from other tables linked through foreign keys.
    *
    * @param int $answer_id
    * @param string $answer_type Admin or Mod
    * @param string $search The search term
    * @param string $field The field being searched
    * @param string $sort The field by which to sort the results
    * @param string $order The order in which to sort the results
    * @return string $query
    */
    public function getFullAnswersArray($answer_type = "Mod", $search = "", $field = ""
            , $sort = "", $order = "") {

        $filter = " A.`id` >= 0 AND A.`ticket_id` = B.`id`
                   AND A.`user_id` = C.`" . DB_PREFIX_USER_ID . "` AND B.`department_id` = D.`id` ";

        $aliases = array(DB_PREFIX_ANSWERS => 'A',
                          DB_PREFIX_TICKETS => 'B',
                          DB_PREFIX_USER => "C",
                          DB_PREFIX_DEPARTMENTS => "D");
        $field_split = explode(".", $field);
        $field_table = $field_split[0];
        if (isset($field_split[1])) {
            $field_field = $field_split[1];
        }

        // Adjust filter according to search term and value
        if ($search != "" && $search != "all") {
            // If the search is for "All" records, prepare query
            if (array_key_exists($field_table, $aliases)) {
                $filter .= "AND LCASE(" . $aliases[$field_table] . "." . $field_field . ") LIKE LCASE('%" . $search . "%') ";
            } else {
                $filter .= "AND LCASE(" . $field . ") LIKE LCASE('%" . $search . "%') ";
            }
        }

        $order = "ORDER BY $sort $order";

        $answers = array();

        // Perform a query that returns every answer and associated info
        $query = "SELECT A.`id` AS 'answer_id',
                         A.`user_id` AS 'answer_user_id',
                         A.`ticket_id` AS 'answer_ticket_id',
                         A.`body` AS 'answer_body',
                         A.`timestamp` AS 'answer_timestamp',
                         A.`rating` AS 'answer_rating',
                         A.`subject` AS 'answer_subject',

                         C.`" . DB_PREFIX_USER_ID . "` AS 'user_user_id',
                         C.`" . DB_PREFIX_USER_NAME . "` AS 'user_name',
                         C.`" . DB_PREFIX_USER_USERNAME . "` AS 'user_username',
                         C.`" . DB_PREFIX_USER_PASSWORD . "` AS 'user_password',
                         C.`" . DB_PREFIX_USER_ADMIN . "` AS 'user_admin',

                         B.`user_id` AS  'ticket_user_id',
                         B.`id` AS  'ticket_ticket_id',
                         B.`body` AS  'ticket_body',
                         B.`subject` AS  'ticket_subject',
                         B.`timestamp` AS  'ticket_timestamp',
                         B.`status` AS  'ticket_status',
                         B.`urgency` AS  'ticket_urgency',
                         B.`department_id` AS  'ticket_department_id',

                         D.`id` AS  'department_id',
                         D.`name` AS  'department_name',
                         D.`description` AS  'department_description',
                         D.`status` AS  'department_status'

                    FROM `" . DB_PREFIX_ANSWERS . "` AS A,
                         `" . DB_PREFIX_TICKETS . "` AS B,
                         `" . DB_PREFIX_USER . "` AS C,
                         `" . DB_PREFIX_DEPARTMENTS . "` AS D
                   WHERE ";
            $query .= " $filter $order ";
        return $query;
    }
}
?>