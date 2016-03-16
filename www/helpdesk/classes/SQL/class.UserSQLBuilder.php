<?php
/**
* File holding the UserSQLBuilder class
* @package contactmanagement
* @author Nicolas Connault, <nick@connault.com.au>
*/

/**
* user defined includes
*/
require_once_check(PHPST_PATH . 'classes/SQL/abstract.SQLBuilder.php');

/**
* user defined constants
* @ignore
*/

/**
* This is a class with static functions used for creating SQL queries
* for the users table
*/
class PHPST_UserSQLBuilder extends PHPST_SQLBuilder{

    /**
    * Constructor.
    */
    public function __construct() {
        parent::__construct();
    }

    /**
    * Creates a query for Inserting a new User into the DB.
    *
    * @access public
    * @param object $user The User to insert
    * @param array $columns The columns of the table in which to insert the User
    * @return string $sql
    */
    public function insert($user, $columns) {
        $sql = "INSERT INTO `" . DB_PREFIX_USER . "`" .
                      " SET `" . DB_PREFIX_USER_NAME . "` = '" . htmlentities($user->getName(), ENT_QUOTES) . "',
                            `" . DB_PREFIX_USER_ADMIN . "` = '" . htmlentities($user->getAdmin(), ENT_QUOTES) . "',
                            `" . DB_PREFIX_USER_EMAIL . "` = '" . htmlentities($user->getEmail(), ENT_QUOTES) . "',
                            `" . DB_PREFIX_USER_PASSWORD . "` = '" . htmlentities($user->getPassword(), ENT_QUOTES) . "',
                            `" . DB_PREFIX_USER_TIMESTAMP . "` = '" . htmlentities($user->getTimestamp(), ENT_QUOTES) . "',
                            `" . DB_PREFIX_USER_USERNAME . "` = '" . htmlentities($user->getUsername(), ENT_QUOTES) . "'";

        return $sql;
    }

    /**
    * Creates a query for deleting User from the DB
    *
    * @access public
    * @param int $user_id The user's id
    * @return string $sql
    */
    public function delete($user_id) {
        return "DELETE FROM `" . DB_PREFIX_USER . "` WHERE `" . DB_PREFIX_USER_ID . "` = '" . $user_id . "'";
    }

    /**
    * Creates a query for updating a User record in the DB from
    * the given User PHP object's data.
    *
    * @access public
    * @param object $user
    * @param array $columns The table's columns
    * @return string $sql
    */
    public function update($user, $columns) {
        $sql = "UPDATE `" . DB_PREFIX_USER . "`" .
                      " SET `" . DB_PREFIX_USER_NAME . "` = '" . $user->getName() . "',
                            `" . DB_PREFIX_USER_ADMIN . "` = '" . $user->getAdmin() . "',
                            `" . DB_PREFIX_USER_EMAIL . "` = '" . $user->getEmail() . "',
                            `" . DB_PREFIX_USER_PASSWORD . "` = '" . $user->getPassword() . "',
                            `" . DB_PREFIX_USER_TIMESTAMP . "` = '" . $user->getTimestamp() . "',
                            `" . DB_PREFIX_USER_USERNAME . "` = '" . $user->getUsername() . "'
                      WHERE `" . DB_PREFIX_USER_ID . "` = '" . $user->getId(). "'";
        return $sql;
    }

    /**
    * Creates a query for updating the Department associated with this User.
    *
    * @access public
    * @param int $user_id The user's id
    * @param int $department_id The department's id
    * @return string $sql
    */
    public function updateDepartment_users($user_id, $department_id) {
        $sql = "UPDATE `" . DB_PREFIX_DEPARTMENTS_USERS . "` SET `department_id` = '" . $department_id . "'
                   WHERE `" . DB_PREFIX_USER_ID . "` = '" . $user_id . "'";
        return $sql;
    }

    /**
    * Creates a query for retrieving all data about this User from the DB,
    * including all data from other tables linked through foreign keys.
    *
    * @access public
    * @param array An array of fields used to customise the query
    * @return string $sql
    */
    public function get($fields) {
        $where = "";
        $myUser = null;
        $filter = "";

        if (array_key_exists('username', $fields)
                && array_key_exists('password', $fields)) {
            $username = $fields['username'];
            $password = $fields['password'];
            $filter .= "AND A.`" . DB_PREFIX_USER_USERNAME . "` = '" . $username . "'
                        AND A.`" . DB_PREFIX_USER_PASSWORD . "` = '" . $password . "'";
        } elseif (array_key_exists('user_id', $fields)) {
            $id = $fields['user_id'];
            $filter .= "AND A.`" . DB_PREFIX_USER_ID . "` = '" . $id . "'";

        } else {
            return false;
        }

        $sql = "  SELECT A.`" . DB_PREFIX_USER_ID . "` AS 'user_id',
                         A.`" . DB_PREFIX_USER_NAME . "` AS 'name',
                         A.`" . DB_PREFIX_USER_USERNAME . "` AS 'username',
                         A.`" . DB_PREFIX_USER_PASSWORD . "` AS 'password',
                         A.`" . DB_PREFIX_USER_TIMESTAMP . "` AS 'timestamp',
                         A.`" . DB_PREFIX_USER_ADMIN . "` AS 'admin',
                         A.`" . DB_PREFIX_USER_EMAIL . "` AS 'email',

                         B.`id` as 'department_id',
                         B.`name` as 'department_name',
                         B.`status` as 'department_status'

                    FROM `" . DB_PREFIX_USER . "` AS A,
                         `" . DB_PREFIX_DEPARTMENTS . "` AS B,
                         `" . DB_PREFIX_DEPARTMENTS_USERS . "` AS C
                   WHERE A.`" . DB_PREFIX_USER_ID . "` >= 0
                     AND C.`user_id` = A.`" . DB_PREFIX_USER_ID . "`
                     AND C.`department_id` = B.`id`
               " . $filter . "

                   UNION

                  SELECT A.`" . DB_PREFIX_USER_ID . "` AS 'user_id',
                         A.`" . DB_PREFIX_USER_NAME . "` AS 'name',
                         A.`" . DB_PREFIX_USER_USERNAME . "` AS 'username',
                         A.`" . DB_PREFIX_USER_PASSWORD . "` AS 'password',
                         A.`" . DB_PREFIX_USER_TIMESTAMP . "` AS 'timestamp',
                         A.`" . DB_PREFIX_USER_ADMIN . "` AS 'admin',
                         A.`" . DB_PREFIX_USER_EMAIL . "` AS 'email',

                         null as 'department_id',
                         null as 'department_name',
                         null as 'department_status'

                    FROM `" . DB_PREFIX_USER . "` AS A

                   WHERE A.`" . DB_PREFIX_USER_ID . "` >= 0
                     AND A.`" . DB_PREFIX_USER_ID . "` NOT IN (SELECT `user_id` FROM `" . DB_PREFIX_DEPARTMENTS_USERS . "`)
               " . $filter . " ";
        // exit($sql);
        return $sql;
    }


    /**
    * Creates a query for retrieving all data about a number of Users from the DB,
    * including all data from other tables linked through foreign keys.
    *
    * @param string $user_type Admin or Mod
    * @param string $search The search term
    * @param string $field The field being searched
    * @param string $sort The field by which to sort the results
    * @param string $order The order in which to sort the results
    * @return string $sql
    */
    public function getFullUsersArray($user_type = "Mod", $search = "", $field = ""
            , $sort = "", $order = "") {
        $filter = "";
        // Prepare array of table aliases
        $aliases = array(DB_PREFIX_USER => 'A',
                          DB_PREFIX_DEPARTMENTS => 'B',
                          DB_PREFIX_DEPARTMENTS_USERS => "C",
                          DB_PREFIX_TICKETS => "D");

        $field_split = explode(".", $field);
        $field_table = $field_split[0];
        $field_field = 'id';
        if (isset($field_split[1])) {
            $field_field = $field_split[1];
        }

        // Adjust filter according to search term and value
        if ($search != "" && $search != "all") {
            // If the search is for "All" records, prepare query
            if (array_key_exists($field_table, $aliases)) {
                if ($field_field == 'id' || $field_field == 'user_id') {
                    $filter .= "AND " . $aliases[$field_table] . "." . $field_field . " = '" . $search . "' ";
                } else {
                    $filter .= "AND LCASE(" . $aliases[$field_table] . "." . $field_field . ") LIKE LCASE('%" . $search . "%') ";
                }
            } else {
                $filter .= "AND LCASE(" . $field . ") LIKE LCASE('%" . $search . "%') ";
            }
        }

        if ($order != "") {
            $order = "ORDER BY " . $sort . " " . $order . " ";
        }

        $users = array();

        // Perform a query that returns every user, their department and tickets
        $sql = "
                /* Users with departments AND tickets */
                SELECT B.`id` AS 'department_id',
                       B.`name` AS 'department_name',
                       B.`description` AS 'department_description',
                       B.`status` as 'department_status',

                       A.`" . DB_PREFIX_USER_ID . "` AS 'user_id',
                       A.`" . DB_PREFIX_USER_NAME . "` AS 'name',
                       A.`" . DB_PREFIX_USER_USERNAME . "` AS 'username',
                       A.`" . DB_PREFIX_USER_PASSWORD . "` AS 'password',
                       A.`" . DB_PREFIX_USER_TIMESTAMP . "` AS 'timestamp',
                       A.`" . DB_PREFIX_USER_ADMIN . "` AS 'admin',
                       A.`" . DB_PREFIX_USER_EMAIL . "` AS 'email',

                       D.`id` AS 'ticket_id',
                       D.`user_id` AS 'ticket_user_id',
                       D.`subject` AS 'ticket_subject',
                       D.`body` AS 'ticket_body',
                       D.`timestamp` AS 'ticket_timestamp',
                       D.`status` AS 'ticket_status',
                       D.`urgency` AS 'ticket_urgency',
                       D.`department_id` AS 'ticket_department_id'

                  FROM `" . DB_PREFIX_USER . "` AS A,
                       `" . DB_PREFIX_DEPARTMENTS . "` AS B,
                       `" . DB_PREFIX_DEPARTMENTS_USERS . "` AS C,
                       `" . DB_PREFIX_TICKETS . "` AS D

                 WHERE C.`department_id` = B.`id`
                   AND C.`user_id` = A.`" . DB_PREFIX_USER_ID . "`
                   AND D.`user_id` = A.`" . DB_PREFIX_USER_ID . "`
                " . $filter . " ";

        if (!eregi('tickets', $field) || eregi('departments', $field) > 0) {
            $sql .= "UNION

                /* Users with departments AND NO tickets */
                SELECT B.`id` AS 'department_id',
                       B.`name` AS 'department_name',
                       B.`description` AS 'department_description',
                       B.`status` as 'department_status',

                       A.`" . DB_PREFIX_USER_ID . "` AS 'user_id',
                       A.`" . DB_PREFIX_USER_NAME . "` AS 'name',
                       A.`" . DB_PREFIX_USER_USERNAME . "` AS 'username',
                       A.`" . DB_PREFIX_USER_PASSWORD . "` AS 'password',
                       A.`" . DB_PREFIX_USER_TIMESTAMP . "` AS 'timestamp',
                       A.`" . DB_PREFIX_USER_ADMIN . "` AS 'admin',
                       A.`" . DB_PREFIX_USER_EMAIL . "` AS 'email',

                       null AS 'ticket_id',
                       null AS 'ticket_user_id',
                       null AS 'ticket_subject',
                       null AS 'ticket_body',
                       null AS 'ticket_timestamp',
                       null AS 'ticket_status',
                       null AS 'ticket_urgency',
                       null AS 'ticket_department_id'

                  FROM `" . DB_PREFIX_USER . "` AS A,
                       `" . DB_PREFIX_DEPARTMENTS . "` AS B,
                       `" . DB_PREFIX_DEPARTMENTS_USERS . "` AS C

                 WHERE C.`department_id` = B.`id`
                   AND C.`user_id` = A.`" . DB_PREFIX_USER_ID . "`
                   AND A.`" . DB_PREFIX_USER_ID . "` NOT IN (SELECT `user_id` FROM `" . DB_PREFIX_TICKETS . "`)
                " . $filter . " ";
        }
        if (!eregi('departments', $field)) {
            $sql .= "UNION

                /* Users with NO department AND WITH tickets */
                SELECT null AS 'department_id',
                       null AS 'department_name',
                       null AS 'department_description',
                       null AS 'department_status',

                       A.`" . DB_PREFIX_USER_ID . "` AS 'user_id',
                       A.`" . DB_PREFIX_USER_NAME . "` AS 'name',
                       A.`" . DB_PREFIX_USER_USERNAME . "` AS 'username',
                       A.`" . DB_PREFIX_USER_PASSWORD . "` AS 'password',
                       A.`" . DB_PREFIX_USER_TIMESTAMP . "` AS 'timestamp',
                       A.`" . DB_PREFIX_USER_ADMIN . "` AS 'admin',
                       A.`" . DB_PREFIX_USER_EMAIL . "` AS 'email',

                       D.`id` AS 'ticket_id',
                       D.`user_id` AS 'ticket_user_id',
                       D.`subject` AS 'ticket_subject',
                       D.`body` AS 'ticket_body',
                       D.`timestamp` AS 'ticket_timestamp',
                       D.`status` AS 'ticket_status',
                       D.`urgency` AS 'ticket_urgency',
                       D.`department_id` AS 'ticket_department_id'

                  FROM `" . DB_PREFIX_USER . "` AS A,
                       `" . DB_PREFIX_TICKETS . "` AS D
                 WHERE A.`" . DB_PREFIX_USER_ID . "` NOT IN (SELECT `user_id` FROM `" . DB_PREFIX_DEPARTMENTS_USERS . "`)
                   AND D.`user_id` = A.`" . DB_PREFIX_USER_ID . "`
                " . $filter . " ";
        }
        if (!eregi('departments', $field) && !eregi('tickets', $field)) {
            $sql .= "
                UNION

                /* Users with NO department AND NO tickets */
                SELECT null AS 'department_id',
                       null AS 'department_name',
                       null AS 'department_description',
                       null AS 'department_status',

                       A.`" . DB_PREFIX_USER_ID . "` AS 'user_id',
                       A.`" . DB_PREFIX_USER_NAME . "` AS 'name',
                       A.`" . DB_PREFIX_USER_USERNAME . "` AS 'username',
                       A.`" . DB_PREFIX_USER_PASSWORD . "` AS 'password',
                       A.`" . DB_PREFIX_USER_TIMESTAMP . "` AS 'timestamp',
                       A.`" . DB_PREFIX_USER_ADMIN . "` AS 'admin',
                       A.`" . DB_PREFIX_USER_EMAIL . "` AS 'email',

                       null AS 'ticket_id',
                       null AS 'ticket_user_id',
                       null AS 'ticket_subject',
                       null AS 'ticket_body',
                       null AS 'ticket_timestamp',
                       null AS 'ticket_status',
                       null AS 'ticket_urgency',
                       null AS 'ticket_department_id'

                  FROM `" . DB_PREFIX_USER . "` AS A
                 WHERE A.`" . DB_PREFIX_USER_ID . "` NOT IN (SELECT `user_id` FROM `" . DB_PREFIX_DEPARTMENTS_USERS . "`)
                   AND A.`" . DB_PREFIX_USER_ID . "` NOT IN (SELECT `user_id` FROM `" . DB_PREFIX_TICKETS . "`)
                " . $filter . " ";
        }
        // die($sql);
        return $sql . $order;
    }

    public function getCSV() {
        $sql = "SELECT A.`" . DB_PREFIX_USER_ID . "`,
                   A.`" . DB_PREFIX_USER_NAME . "`,
                   A.`" . DB_PREFIX_USER_USERNAME . "`,
                   A.`" . DB_PREFIX_USER_EMAIL . "`,
                   A.`" . DB_PREFIX_USER_ADMIN . "`,
                   COUNT(DISTINCT B.`id`) AS 'tickets'
             FROM `" . DB_PREFIX_USER . "` AS A,
                  `" . DB_PREFIX_TICKETS . "` AS B
            WHERE A.`" . DB_PREFIX_USER_ID . "` = B.`user_id`
            GROUP BY A.`" . DB_PREFIX_USER_ID . "`

            UNION

            SELECT A.`" . DB_PREFIX_USER_ID . "`,
                   A.`" . DB_PREFIX_USER_NAME . "`,
                   A.`" . DB_PREFIX_USER_USERNAME . "`,
                   A.`" . DB_PREFIX_USER_EMAIL . "`,
                   A.`" . DB_PREFIX_USER_ADMIN . "`,
                   0 AS 'tickets'
             FROM `" . DB_PREFIX_USER . "` AS A
            WHERE A.`" . DB_PREFIX_USER_ID . "` NOT IN (SELECT `user_id` FROM `" . DB_PREFIX_TICKETS . "`)
            GROUP BY A.`" . DB_PREFIX_USER_ID . "`";
        return $sql;
    }
}
?>