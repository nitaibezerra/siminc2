<?php
/**
* File holding the DepartmentSQLBuilder class
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version SVN: $Id:$
*/

/**
* department defined includes
*/
require_once_check(PHPST_PATH . 'classes/SQL/abstract.SQLBuilder.php');

/**
* department defined constants
* @ignore
*/

/**
* This is a class with static functions used for creating SQL queries
* for the departments table
*/
class PHPST_DepartmentSQLBuilder extends PHPST_SQLBuilder{

    /**
    * Constructor.
    */
    public function __construct() {
        parent::__construct();
    }

    /**
    * Creates a query for Inserting a new Department into the DB.
    *
    * @access public
    * @param object $department The Department to insert
    * @param array $columns The columns of the table in which to insert the Department
    * @return string $sql
    */
    public function insert($department, $columns) {
        $sql = "INSERT INTO `" . DB_PREFIX_DEPARTMENTS . "` SET ";

        // We use the DB columns as a basis for this loop, to avoid DB errors
        foreach ($columns as $k => $v) {
            if ($v != "id") {
                $value = eval('return htmlentities($department->get' . ucfirst($v) . "(), ENT_QUOTES);");
                $sql .= "`$v` = '$value',";
            }
        }

        // Trim last comma
        $sql = rtrim($sql,",");
        return $sql;
    }

    /**
    * Creates a query for retrieving an array of Users
    * attached to a Department.
    *
    * @access public
    * @param int $department_id The department's id
    * @return string $sql
    */
    public function getUsers($department_id) {
        $sql ="SELECT A.`" . DB_PREFIX_USER_ID . "` AS 'user_id',
                        A.`" . DB_PREFIX_USER_NAME . "` AS 'name',
                        A.`" . DB_PREFIX_USER_USERNAME . "` AS 'username',
                        A.`" . DB_PREFIX_USER_PASSWORD . "` AS 'password',
                        A.`" . DB_PREFIX_USER_TIMESTAMP . "` AS 'timestamp',
                        A.`" . DB_PREFIX_USER_ADMIN . "` AS 'admin',
                        A.`" . DB_PREFIX_USER_EMAIL . "` AS 'email'

                  FROM `" . DB_PREFIX_USER . "` AS A,
                       `" . DB_PREFIX_DEPARTMENTS_USERS . "` AS B
                 WHERE B.`user_id` = A.`" . DB_PREFIX_USER_ID . "`
                   AND B.`id` = '$department_id'";
        return $sql;
    }

    /**
    * Creates a query for deleting Department from the DB
    * Some users may end up with no department linking to them.
    *
    * @access public
    * @param int $department_id The department's id
    * @return string $sql
    */
    public function delete($department_id) {
        $sql = "DELETE FROM `" . DB_PREFIX_DEPARTMENTS . "` AS A
                        WHERE A.`id` = '$department_id' ";
        return $sql;
    }

    /**
    * Creates a query for deleting user->department associations from the DB.
    * Some users may end up with no department linking to them.
    *
    * @access public
    * @param int $department_id The department's id
    * @return string $sql
    */
    public function deleteUsers($department_id) {
        $sql = "DELETE FROM `" . DB_PREFIX_DEPARTMENTS_USERS . "`
                   WHERE `department_id` = '$department_id' ";
        return $sql;
    }

    /**
    * Creates a query for updating the list of Users.
    *
    * @access public
    * @param int $department_id The Department's id
    * @param array $users The department's array of users
    * @return string $sql
    */
    public function updateUsersList($department_id, $users) {
        if (is_array($users) && count($users) > 0) {
            $sql = "INSERT INTO `" . DB_PREFIX_DEPARTMENTS_USERS . "` (department_id, user_id) VALUES ";
            $changed = false;
            foreach ($users as $k => $v) {
                if ($v->getAdmin() == 'Mod') {
                    $sql .= "($department_id, " . $v->getId() . "),";
                    $changed = true;
                }
            }
            // Erase last comma
            $sql = rtrim($sql, ",");

            if (!$changed) {
                $sql = null;
            }

            return $sql;
        } else {
            return null;
        }
    }

    /**
    * Creates a query for removing user entries in department_users table for this department
    *
    * @access public
    * @param int $department_id The Department's id
    * @return string $sql
    */
    public function emptyDepartmentUsers($department_id) {
        $sql = "DELETE FROM `" . DB_PREFIX_DEPARTMENTS_USERS . "` WHERE `department_id` = $department_id; ";

        return $sql;
    }

    /**
    * Creates a query for updating a Department record in the DB from
    * the given Department PHP object's data.
    *
    * @access public
    * @param object $department
    * @param array $columns The table's columns
    * @return string $sql
    */
    public function update($department, $columns) {
        $sql = "UPDATE `" . DB_PREFIX_DEPARTMENTS . "` SET ";

        // We use the DB columns as a basis for this loop, to avoid DB errors
        foreach ($columns as $k => $v) {
            if ($v != "id" && $v != "user_id") {
                $value = eval('return $department->get' . ucfirst($v) . "();");
                $sql .= "`$v` = '$value',";
            }
        }
        // Trim last comma
        $sql = rtrim($sql, ",");
        $sql .= " WHERE `id` = " . $department->getId(). "";
        return $sql;
    }

    /**
    * Creates a query for retrieving a complete Department Object
    *
    * @access public
    * @param array An array of fields used to customise the query
    * @return string $sql
    */
    public function get($fields) {
        $filter = DB_PREFIX_DEPARTMENTS . ".id >= 0 ";
        $sql = "SELECT * FROM `" . DB_PREFIX_DEPARTMENTS . "` WHERE ";

        if (is_array($fields) && array_key_exists('name', $fields)) {
            $name = $fields['name'];
            $filter .= "AND `name` = '$name'";
        } elseif (array_key_exists('id', $fields) || array_key_exists(DB_PREFIX_DEPARTMENTS . '.id', $fields)) {
            $id = $fields['id'];
            $filter .= "AND `id` = '$id'";
        } elseif(isset($fields['field']) && $fields['field'] == DB_PREFIX_DEPARTMENTS . '.id') {
            $id = $fields['search'];
            $filter .= "AND `id` = '$id'";
        } else {
            return false;
        }
        $sql .= " $filter ";

        return $sql;
    }

    /**
    * Creates a query for retrieving all data about a number of Departments from the DB,
    * including all data from other tables linked through foreign keys.
    *
    * @param int $department_id
    * @param string $department_type Admin or Mod
    * @param string $search The search term
    * @param string $field The field being searched
    * @param string $sort The field by which to sort the results
    * @param string $order The order in which to sort the results
    * @return string $sql
    */
    public function getFullDepartmentsArray($department_type = "Mod",
            $search = null, $field = null, $sort = null, $order = null) {

        $filter = "";
        // Prepare array of table aliases
        $aliases = array(DB_PREFIX_DEPARTMENTS => 'A',
                          DB_PREFIX_DEPARTMENTS_USERS => 'B',
                          DB_PREFIX_USER => "C",
                          DB_PREFIX_TICKETS => "D");

        $field_split = explode(".", $field);
        $field_table = $field_split[0];
        if (isset($field_split[1])) {
            $field_field = $field_split[1];
        }

        // Adjust filter according to search term and value
        if ($search != "" && $search != "all") {
            // If the search is for "All" records, prepare query
            if (array_key_exists($field_table, $aliases)) {
                $filter .= " AND LCASE(" . $aliases[$field_table] . "." . $field_field . ") LIKE LCASE('%" . $search . "%') ";
            } else {
                $filter .= " AND LCASE(" . $field . ") LIKE LCASE('%" . $search . "%') ";
            }
        }

        if (!is_null($order) && !is_null($sort) && $order != '') {
            $order = " ORDER BY $sort $order";
        } else {
            $order = " ORDER BY name ASC";
        }

        // Perform a query that returns every department, its mods and tickets
        $sql = "
                /* Departments with mods AND tickets */
                SELECT DISTINCT
                    A.`id` AS 'id',
                    A.`name` AS 'name',
                    A.`description` AS 'description',
                    A.`status` AS 'status',

                    C.`" . DB_PREFIX_USER_ID . "` AS 'user_id',
                    C.`" . DB_PREFIX_USER_NAME . "` AS 'user_name',
                    C.`" . DB_PREFIX_USER_USERNAME . "` AS 'user_username',
                    C.`" . DB_PREFIX_USER_PASSWORD . "` AS 'user_password',
                    C.`" . DB_PREFIX_USER_TIMESTAMP . "` AS 'user_timestamp',
                    C.`" . DB_PREFIX_USER_ADMIN . "` AS 'user_admin',
                    C.`" . DB_PREFIX_USER_EMAIL . "` AS 'user_email',

                    D.`id` AS 'ticket_id',
                    D.`user_id` AS 'ticket_user_id',
                    D.`subject` AS 'ticket_subject',
                    D.`body` AS 'ticket_body',
                    D.`timestamp` AS 'ticket_timestamp',
                    D.`status` AS 'ticket_status',
                    D.`urgency` AS 'ticket_urgency',
                    D.`department_id` AS 'ticket_department_id'

               FROM `" . DB_PREFIX_DEPARTMENTS . "` AS A,
                    `" . DB_PREFIX_DEPARTMENTS_USERS . "` AS B,
                    `" . DB_PREFIX_USER . "` AS C,
                    `" . DB_PREFIX_TICKETS . "` AS D

              WHERE B.`department_id` = A.`id`
                AND D.`department_id` = A.`id`
                AND B.`user_id` = C.`" . DB_PREFIX_USER_ID . "`
                $filter

                UNION

                /* Departments with mods AND NO tickets */
                SELECT  DISTINCT
                    A.`id` AS 'id',
                    A.`name` AS 'name',
                    A.`description` AS 'description',
                    A.`status` AS 'status',

                    C.`" . DB_PREFIX_USER_ID . "` AS 'user_id',
                    C.`" . DB_PREFIX_USER_NAME . "` AS 'user_name',
                    C.`" . DB_PREFIX_USER_USERNAME . "` AS 'user_username',
                    C.`" . DB_PREFIX_USER_PASSWORD . "` AS 'user_password',
                    C.`" . DB_PREFIX_USER_TIMESTAMP . "` AS 'user_timestamp',
                    C.`" . DB_PREFIX_USER_ADMIN . "` AS 'user_admin',
                    C.`" . DB_PREFIX_USER_EMAIL . "` AS 'user_email',

                    null AS 'ticket_id',
                    null AS 'ticket_user_id',
                    null AS 'ticket_subject',
                    null AS 'ticket_body',
                    null AS 'ticket_timestamp',
                    null AS 'ticket_status',
                    null AS 'ticket_urgency',
                    null AS 'ticket_department_id'

               FROM `" . DB_PREFIX_DEPARTMENTS . "` AS A,
                    `" . DB_PREFIX_DEPARTMENTS_USERS . "` AS B,
                    `" . DB_PREFIX_USER . "` AS C

              WHERE B.`department_id` = A.`id`
                AND B.`user_id` = C.`" . DB_PREFIX_USER_ID . "`
                $filter

                UNION

                /* Departments with NO mods AND WITH tickets */
                SELECT DISTINCT

                    A.`id` AS 'id',
                    A.`name` AS 'name',
                    A.`description` AS 'description',
                    A.`status` AS 'status',

                    null AS 'user_id',
                    null AS 'user_name',
                    null AS 'user_username',
                    null AS 'user_password',
                    null AS 'user_email',
                    null AS 'user_timestamp',
                    null AS 'user_admin',

                    D.`id` AS 'ticket_id',
                    D.`user_id` AS 'ticket_user_id',
                    D.`subject` AS 'ticket_subject',
                    D.`body` AS 'ticket_body',
                    D.`timestamp` AS 'ticket_timestamp',
                    D.`status` AS 'ticket_status',
                    D.`urgency` AS 'ticket_urgency',
                    D.`department_id` AS 'ticket_department_id'

               FROM `" . DB_PREFIX_DEPARTMENTS . "` AS A,
                    `" . DB_PREFIX_TICKETS . "` AS D

              WHERE A.`id` NOT IN (SELECT `department_id` FROM `" . DB_PREFIX_DEPARTMENTS_USERS . "`)
                AND D.`department_id` = A.`id`
                $filter

                UNION

                /* Departments with NO Mods AND NO tickets */
                SELECT DISTINCT
                    A.`id` AS 'id',
                    A.`name` AS 'name',
                    A.`description` AS 'description',
                    A.`status` AS 'status',

                    null AS 'user_id',
                    null AS 'user_name',
                    null AS 'user_username',
                    null AS 'user_password',
                    null AS 'user_email',
                    null AS 'user_timestamp',
                    null AS 'user_admin',

                    null AS 'ticket_id',
                    null AS 'ticket_user_id',
                    null AS 'ticket_subject',
                    null AS 'ticket_body',
                    null AS 'ticket_timestamp',
                    null AS 'ticket_status',
                    null AS 'ticket_urgency',
                    null AS 'ticket_department_id'

               FROM `" . DB_PREFIX_DEPARTMENTS . "` AS A
              WHERE A.`id` NOT IN (SELECT `department_id` FROM `" . DB_PREFIX_DEPARTMENTS_USERS . "`)
                AND A.`id` NOT IN (SELECT `department_id` FROM `" . DB_PREFIX_TICKETS . "`)
                $filter $order
                ";
        // die($sql);
        return $sql;
    }

    public function getCSV() {
        $sql = "SELECT A.`id`,
                   A.`name`,
                   A.`description`,
                   A.`status`,
                   COUNT(DISTINCT B.`" . DB_PREFIX_USER_ID . "`) AS 'moderators',
                   COUNT(DISTINCT C.`id`) AS 'tickets'
             FROM `" . DB_PREFIX_DEPARTMENTS . "` AS A,
                  `" . DB_PREFIX_USER . "` AS B,
                  `" . DB_PREFIX_TICKETS . "` AS C,
                  `" . DB_PREFIX_DEPARTMENTS_USERS . "` as D
            WHERE A.`id` = D.`department_id`
              AND C.`department_id` = A.`id`
              AND D.`user_id` = B.`" . DB_PREFIX_USER_ID . "`
            GROUP BY A.`id`

            UNION

            SELECT A.`id`,
                   A.`name`,
                   A.`description`,
                   A.`status`,
                   0 AS 'moderators',
                   COUNT(DISTINCT C.`id`) AS 'tickets'
             FROM `" . DB_PREFIX_DEPARTMENTS . "` AS A,
                  `" . DB_PREFIX_TICKETS . "` AS C,
                  `" . DB_PREFIX_DEPARTMENTS_USERS . "` as D
            WHERE A.`id` = D.`department_id`
              AND C.`department_id` = A.`id`
              AND A.`id` NOT IN (SELECT `department_id` FROM `" . DB_PREFIX_DEPARTMENTS_USERS . "`)
            GROUP BY A.`id`

            UNION

            SELECT A.`id`,
                   A.`name`,
                   A.`description`,
                   A.`status`,
                   COUNT(DISTINCT B.`" . DB_PREFIX_USER_ID . "`) AS 'moderators',
                   0 AS 'tickets'
             FROM `" . DB_PREFIX_DEPARTMENTS . "` AS A,
                  `" . DB_PREFIX_USER . "` AS B,
                  `" . DB_PREFIX_TICKETS . "` AS C,
                  `" . DB_PREFIX_DEPARTMENTS_USERS . "` as D
            WHERE A.`id` = D.`department_id`
              AND A.`id` NOT IN (SELECT `department_id` FROM `" . DB_PREFIX_TICKETS . "`)
              AND D.`user_id` = B.`" . DB_PREFIX_USER_ID . "`
            GROUP BY A.`id`

            UNION

            SELECT A.`id`,
                   A.`name`,
                   A.`description`,
                   A.`status`,
                   0 AS 'moderators',
                   0 AS 'tickets'
             FROM `" . DB_PREFIX_DEPARTMENTS . "` AS A,
                  `" . DB_PREFIX_DEPARTMENTS_USERS . "` as D
            WHERE A.`id` = D.`department_id`
              AND A.`id` NOT IN (SELECT `department_id` FROM `" . DB_PREFIX_TICKETS . "`)
              AND A.`id` NOT IN (SELECT `department_id` FROM `" . DB_PREFIX_DEPARTMENTS_USERS . "`)
            GROUP BY A.`id`";
        return $sql;
    }
}
?>