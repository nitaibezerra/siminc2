<?php
/**
 * File containing the TicketSQLBuilder class.
 *
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version CVS: $id: class.TicketSQLBuilder.php,v 1.1.1.1 2005/10/19 23:24:10 nicolas Exp $
 * @since File available since Release 1.1.1.1
 * \\||
 */

/**
* ticket defined includes
*/
require_once_check(PHPST_PATH . 'classes/SQL/abstract.SQLBuilder.php');

/**
* ticket defined constants
* @ignore
*/

/**
* This is a class with static functions used for creating SQL queries
* for the tickets table
*
* @package PHPSupportTickets
*/
class PHPST_TicketSQLBuilder extends PHPST_SQLBuilder{

    /**
    * Constructor.
    */
    public function __construct() {
        parent::__construct();
    }

    /**
    * Creates a query for Inserting a new Ticket into the DB.
    *
    * @access public
    * @param object $ticket The Ticket to insert
    * @param array $columns The columns of the table in which to insert the Ticket
    * @return string $sql
    */
    public function insert($ticket, $columns) {
        $sql = "INSERT INTO `" . DB_PREFIX_TICKETS . "` SET ";

        // We use the DB columns as a basis for this loop, to avoid DB errors
        foreach ($columns as $k => $v) {
            if ($v != "id") {
                $value = eval('return nl2br(htmlentities($ticket->get' . ucfirst($v) . "(), ENT_QUOTES));");
                $sql .= "`$v` = '$value',";
            }
        }

        // Trim last comma
        $sql = rtrim($sql,",");
        return $sql;
    }

    /**
    * Creates a query for deleting Ticket from the DB
    *
    * @access public
    * @param int $ticket_id The ticket's ID
    * @return string $sql
    */
    public function delete($ticket_id) {
        return "DELETE FROM `" . DB_PREFIX_TICKETS . "` WHERE `id` = '" . $ticket_id . "'";
    }

    /**
    * Creates a query for updating a Ticket record in the DB from
    * the given Ticket PHP object's data.
    *
    * @access public
    * @param object $ticket
    * @param array $columns The table's columns
    * @return string $sql
    */
    public function update($ticket, $columns) {
        $sql = "UPDATE `" . DB_PREFIX_TICKETS . "` SET ";

        // We use the DB columns as a basis for this loop, to avoid DB errors
        foreach ($columns as $k => $v) {
            if ($v != "id") {
                $value = eval('return nl2br($ticket->get' . ucfirst($v) . "());");
                $sql .= "`$v` = '" . addslashes($value) . "',";
            }
        }
        // Trim last comma
        $sql = rtrim($sql, ",");
        $sql .= " WHERE `id` = '" . $ticket->getId(). "'";
        return $sql;
    }

    /**
    * Creates a query for retrieving all data about this Ticket from the DB,
    * including all data from other tables linked through foreign keys.
    *
    * @access public
    * @param array An array of fields used to customise the query
    * @return string $sql
    */
    public function get($fields) {
        $where = "";
        $myTicket = null;
        $filter = "    A.`user_id` = B.`" . DB_PREFIX_USER_ID . "`
                   AND A.`id` >= 0
                   AND A.`department_id` = C.`id` ";

        if (array_key_exists('timestamp', $fields)) {
            $timestamp = $fields['timestamp'];
            $filter .= " AND A.`timestamp` = '" . $timestamp . "' ";
        } elseif (array_key_exists('id', $fields)) {
            $id = $fields['id'];
            $filter .= " AND A.`id` = '" . $id . "' ";
        } else {
            return false;
        }

        $sql = "
             SELECT A.`id`,
                    A.`user_id`,
                    A.`subject`,
                    A.`timestamp`,
                    A.`status`,
                    A.`urgency`,
                    A.`body`,
                    A.`department_id`,

                    B.`" . DB_PREFIX_USER_ID . "` AS 'user_id',
                    B.`" . DB_PREFIX_USER_NAME . "` AS 'user_name',
                    B.`" . DB_PREFIX_USER_USERNAME . "` AS 'user_username',
                    B.`" . DB_PREFIX_USER_PASSWORD . "` AS 'user_password',
                    B.`" . DB_PREFIX_USER_TIMESTAMP . "` AS 'user_timestamp',
                    B.`" . DB_PREFIX_USER_ADMIN . "` AS 'user_admin',
                    B.`" . DB_PREFIX_USER_EMAIL . "` AS 'user_email',

                    C.`id` as 'department_id',
                    C.`name` as 'department_name',
                    C.`description` as 'department_description',
                    C.`status` as 'department_status',
                    
                    D.`id` as 'answer_id',
                    D.`user_id` as 'answer_user_id',
                    D.`ticket_id` as 'answer_ticket_id',
                    D.`body` as 'answer_body',
                    D.`subject` as 'answer_subject',
                    D.`timestamp` as 'answer_timestamp',
                    D.`rating` as 'answer_rating'

               FROM `" . DB_PREFIX_TICKETS . "` AS A,
                    `" . DB_PREFIX_USER . "` AS B,
                    `" . DB_PREFIX_DEPARTMENTS . "` AS C,
                    `" . DB_PREFIX_ANSWERS . "` AS D

              WHERE $filter
                AND D.`ticket_id` = A.`id`

              UNION
             SELECT A.`id`,
                    A.`user_id`,
                    A.`subject`,
                    A.`timestamp`,
                    A.`status`,
                    A.`urgency`,
                    A.`body`,
                    A.`department_id`,

                    B.`" . DB_PREFIX_USER_ID . "` AS 'user_id',
                    B.`" . DB_PREFIX_USER_NAME . "` AS 'user_name',
                    B.`" . DB_PREFIX_USER_USERNAME . "` AS 'user_username',
                    B.`" . DB_PREFIX_USER_PASSWORD . "` AS 'user_password',
                    B.`" . DB_PREFIX_USER_TIMESTAMP . "` AS 'user_timestamp',
                    B.`" . DB_PREFIX_USER_ADMIN . "` AS 'user_admin',
                    B.`" . DB_PREFIX_USER_EMAIL . "` AS 'user_email',

                    C.`id` as 'department_id',
                    C.`name` as 'department_name',
                    C.`description` as 'department_description',
                    C.`status` as 'department_status',

                    null as 'answer_id',
                    null as 'answer_user_id',
                    null as 'answer_ticket_id',
                    null as 'answer_body',
                    null as 'answer_subject',
                    null as 'answer_timestamp',
                    null as 'answer_rating'


               FROM `" . DB_PREFIX_TICKETS . "` AS A,
                    `" . DB_PREFIX_USER . "` AS B,
                    `" . DB_PREFIX_DEPARTMENTS . "` AS C

              WHERE $filter
                AND A.`id` NOT IN (SELECT `ticket_id` FROM `" . DB_PREFIX_ANSWERS . "`) ";

        return $sql;
    }

    /**
    * Creates a query for counting all tickets, using search criteria
    *
    * @access public
    * @param string $admin_type Admin or Mod
    * @param string $search The search term
    * @param string $field The field being searched
    * @param string $sort The field by which to sort the results
    * @param string $order The order in which to sort the results
    * @param int $department_id
    * @return string $sql
    */
    public function getCount($admin_type = "Mod", $search = "", $field = ""
            , $sort = "", $order, $id) {
        $filter = "    A.`id` >= 0
                   AND A.`user_id` = B.`" . DB_PREFIX_USER_ID . "`,
                   AND A.`department_id` = C.`id` ";

        if ($admin_type == 'Mod') {
            $filter .= " AND C.`id` = '" . $id . "' ";
        } elseif ($admin_type == 'Client') {
            $filter .= " AND B.`" . DB_PREFIX_USER_ID . "` = '" . $id . "' ";
        }

        if ($search != "") {
            // If the search is for "All" records, prepare query
            if ($search != "all") {
                $filter .= "AND `" . $field . "` LIKE '%" . $search . "%' ";
            }
        }

        if ($order != '') {
            $order = "ORDER BY `" . $sort . "` " . $order;
        }

        $sql = "SELECT COUNT(*)
                   FROM `" . DB_PREFIX_TICKETS . "` AS A,
                        `" . DB_PREFIX_USER . "` AS B,
                        `" . DB_PREFIX_DEPARTMENTS . "` AS C
                  WHERE " . $filter . " " . $order;

        return $sql;
    }

    /**
    * Creates a query for retrieving all data about a number of tickets from the DB,
    * including all data from other tables linked through foreign keys.
    *
    * @param string $admin_type Admin or Mod
    * @param string $search The search term
    * @param string $field The field being searched
    * @param string $sort The field by which to sort the results
    * @param string $order The order in which to sort the results
    * @param int $department_id or $user_id
    * @param string $ticket_type 'department' tickets or 'my' tickets
    * @return string $sql
    */
    public function getFullTicketsArray($admin_type = "Mod", $search = "", $field = ""
            , $sort = "", $order, $id, $ticket_type = 'department') {

        $filter = "    A.`id` >= 0
                   AND A.`user_id` = B.`" . DB_PREFIX_USER_ID . "` ";

        if (strlen($ticket_type) < 1) {
            $ticket_type = 'department';
        }

        // Adjust filter according to admin
        if ($admin_type == 'Mod' && $ticket_type != 'my') {
            $filter .= " AND C.`id` = '" . $id . "' ";
        } elseif ($admin_type == 'Client' || $ticket_type == 'my') {
            $filter .= " AND B.`" . DB_PREFIX_USER_ID . "` = '" . $id . "' ";
        }

        // Prepare array of table aliases
        $aliases = array(DB_PREFIX_TICKETS => 'A',
                          DB_PREFIX_USER => 'B',
                          DB_PREFIX_DEPARTMENTS => "C",
                          DB_PREFIX_ANSWERS => "D");
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

        if ($order != '') {
            $sort_split = explode(".", $sort);
            $sort_table = $sort_split[0];
            if (isset($sort_split[1])) {
                $sort_field = $sort_split[1];
            }
            if (array_key_exists($sort_table, $aliases)) {
                $order = " ORDER BY " . $aliases[$sort_table] . "." . $sort_field . " " . $order;
            } else {
                $order = " ORDER BY " . $sort . " " . $order;
            }
        }

        $answers = array();

        // Perform a query that returns every ticket and associated info
        $sql = "
             /* tickets with answers AND a Department */
             SELECT A.`id`,
                    A.`user_id`,
                    A.`subject`,
                    A.`timestamp`,
                    A.`status`,
                    A.`urgency`,
                    A.`body`,
                    A.`department_id`,

                    B.`" . DB_PREFIX_USER_ID . "` AS 'user_id',
                    B.`" . DB_PREFIX_USER_NAME . "` AS 'user_name',
                    B.`" . DB_PREFIX_USER_USERNAME . "` AS 'user_username',
                    B.`" . DB_PREFIX_USER_PASSWORD . "` AS 'user_password',
                    B.`" . DB_PREFIX_USER_TIMESTAMP . "` AS 'user_timestamp',
                    B.`" . DB_PREFIX_USER_ADMIN . "` AS 'user_admin',
                    B.`" . DB_PREFIX_USER_EMAIL . "` AS 'user_email',

                    C.`id` as 'department_id',
                    C.`name` as 'department_name',
                    C.`description` as 'department_description',
                    C.`status` as 'department_status',

                    D.`id` as 'answer_id',
                    D.`user_id` as 'answer_user_id',
                    D.`ticket_id` as 'answer_ticket_id',
                    D.`body` as 'answer_body',
                    D.`subject` as 'answer_subject',
                    D.`timestamp` as 'answer_timestamp',
                    D.`rating` as 'answer_rating'

               FROM `" . DB_PREFIX_TICKETS . "` AS A,
                    `" . DB_PREFIX_USER . "` AS B,
                    `" . DB_PREFIX_DEPARTMENTS . "` AS C,
                    `" . DB_PREFIX_ANSWERS . "` AS D

              WHERE $filter
                AND D.`ticket_id` = A.`id`
                AND A.`department_id` = C.`id`

              UNION

                /* tickets with NO answers AND a Department*/
             SELECT A.`id`,
                    A.`user_id`,
                    A.`subject`,
                    A.`timestamp`,
                    A.`status`,
                    A.`urgency`,
                    A.`body`,
                    A.`department_id`,

                    B.`" . DB_PREFIX_USER_ID . "` AS 'user_id',
                    B.`" . DB_PREFIX_USER_NAME . "` AS 'user_name',
                    B.`" . DB_PREFIX_USER_USERNAME . "` AS 'user_username',
                    B.`" . DB_PREFIX_USER_PASSWORD . "` AS 'user_password',
                    B.`" . DB_PREFIX_USER_TIMESTAMP . "` AS 'user_timestamp',
                    B.`" . DB_PREFIX_USER_ADMIN . "` AS 'user_admin',
                    B.`" . DB_PREFIX_USER_EMAIL . "` AS 'user_email',

                    C.`id` as 'department_id',
                    C.`name` as 'department_name',
                    C.`description` as 'department_description',
                    C.`status` as 'department_status',

                    null as 'answer_id',
                    null as 'answer_user_id',
                    null as 'answer_ticket_id',
                    null as 'answer_body',
                    null as 'answer_subject',
                    null as 'answer_timestamp',
                    null as 'answer_rating'

               FROM `" . DB_PREFIX_TICKETS . "` AS A,
                    `" . DB_PREFIX_USER . "` AS B,
                    `" . DB_PREFIX_DEPARTMENTS . "` AS C

              WHERE $filter
                AND A.`id` NOT IN (SELECT `ticket_id` FROM `" . DB_PREFIX_ANSWERS . "`)
                AND A.`department_id` = C.`id`";

        if ($admin_type != 'Mod' && !eregi('departments', $field)) {
            $sql .= "
              UNION
                    /* tickets with answers but NO Department*/
             SELECT A.`id`,
                    A.`user_id`,
                    A.`subject`,
                    A.`timestamp`,
                    A.`status`,
                    A.`urgency`,
                    A.`body`,
                    A.`department_id`,

                    B.`" . DB_PREFIX_USER_ID . "` AS 'user_id',
                    B.`" . DB_PREFIX_USER_NAME . "` AS 'user_name',
                    B.`" . DB_PREFIX_USER_USERNAME . "` AS 'user_username',
                    B.`" . DB_PREFIX_USER_PASSWORD . "` AS 'user_password',
                    B.`" . DB_PREFIX_USER_TIMESTAMP . "` AS 'user_timestamp',
                    B.`" . DB_PREFIX_USER_ADMIN . "` AS 'user_admin',
                    B.`" . DB_PREFIX_USER_EMAIL . "` AS 'user_email',

                    null as 'department_id',
                    null as 'department_name',
                    null as 'department_description',
                    null as 'department_status',

                    D.`id` as 'answer_id',
                    D.`user_id` as 'answer_user_id',
                    D.`ticket_id` as 'answer_ticket_id',
                    D.`body` as 'answer_body',
                    D.`subject` as 'answer_subject',
                    D.`timestamp` as 'answer_timestamp',
                    D.`rating` as 'answer_rating'

               FROM `" . DB_PREFIX_TICKETS . "` AS A,
                    `" . DB_PREFIX_USER . "` AS B,
                    `" . DB_PREFIX_ANSWERS . "` AS D

              WHERE $filter
                AND D.`ticket_id` = A.`id`
                AND A.`department_id` = ''";

            if (!eregi('departments', $field)) {
                $sql .= "
              UNION

                /* tickets with NO answers and NO Department */
             SELECT A.`id`,
                    A.`user_id`,
                    A.`subject`,
                    A.`timestamp`,
                    A.`status`,
                    A.`urgency`,
                    A.`body`,
                    A.`department_id`,

                    B.`" . DB_PREFIX_USER_ID . "` AS 'user_id',
                    B.`" . DB_PREFIX_USER_NAME . "` AS 'user_name',
                    B.`" . DB_PREFIX_USER_USERNAME . "` AS 'user_username',
                    B.`" . DB_PREFIX_USER_PASSWORD . "` AS 'user_password',
                    B.`" . DB_PREFIX_USER_TIMESTAMP . "` AS 'user_timestamp',
                    B.`" . DB_PREFIX_USER_ADMIN . "` AS 'user_admin',
                    B.`" . DB_PREFIX_USER_EMAIL . "` AS 'user_email',

                    null as 'department_id',
                    null as 'department_name',
                    null as 'department_description',
                    null as 'department_status',

                    null as 'answer_id',
                    null as 'answer_user_id',
                    null as 'answer_ticket_id',
                    null as 'answer_body',
                    null as 'answer_subject',
                    null as 'answer_timestamp',
                    null as 'answer_rating'

               FROM `" . DB_PREFIX_TICKETS . "` AS A,
                    `" . DB_PREFIX_USER . "` AS B

              WHERE $filter
                AND A.`id` NOT IN (SELECT `ticket_id` FROM `" . DB_PREFIX_ANSWERS . "`)
                AND `department_id` = ''";
             }
        }

        return $sql . $order;
    }

    public function getCSV() {
        $sql = "SELECT A.`id`,
                   B.`" . DB_PREFIX_USER_NAME . "` AS 'user',
                   A.`subject`,
                   A.`timestamp`,
                   A.`status`,
                   A.`urgency`,
                   A.`body`,
                   C.`name` AS 'department',
                   COUNT(DISTINCT D.`id`) AS 'answers'
             FROM `" . DB_PREFIX_TICKETS . "` AS A,
                  `" . DB_PREFIX_USER . "` AS B,
                  `" . DB_PREFIX_DEPARTMENTS . "` AS C,
                  `" . DB_PREFIX_ANSWERS . "` AS D
            WHERE A.`user_id` = B.`" . DB_PREFIX_USER_ID . "`
              AND A.`department_id` = C.`id`
              AND D.`ticket_id` = A.`id`
            GROUP BY A.`id`

            UNION

            SELECT A.`id`,
                   B.`" . DB_PREFIX_USER_NAME . "` AS 'user',
                   A.`subject`,
                   A.`timestamp`,
                   A.`status`,
                   A.`urgency`,
                   A.`body`,
                   C.`name` AS 'department',
                   0 AS 'answers'
             FROM `" . DB_PREFIX_TICKETS . "` AS A,
                  `" . DB_PREFIX_USER . "` AS B,
                  `" . DB_PREFIX_DEPARTMENTS . "` AS C,
                  `" . DB_PREFIX_ANSWERS . "` AS D
            WHERE A.`user_id` = B.`" . DB_PREFIX_USER_ID . "`
              AND A.`department_id` = C.`id`
              AND A.`id` NOT IN (SELECT `ticket_id` FROM `" . DB_PREFIX_ANSWERS . "`)
            GROUP BY A.`id`";
        return $sql;
    }

}
?>