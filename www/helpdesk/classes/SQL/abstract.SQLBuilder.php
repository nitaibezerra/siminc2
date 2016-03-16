<?php
/**
 * File containing the SQLBuilder abstract class.
 *
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version SVN: $Id: abstract.SQLBuilder.php 4 2005-12-13 01:47:15Z nicolas $
 * @since File available since Release 1.1.1.1
 * \\||
 */

/**
 * user defined includes
 *
 * @include
 */

/**
 * user defined constants
 *
 * @ignore
 */

/**
 * Include in this class all the methods and variables common to
 * all SQLBuilder classes, and use it as a super-class.
 *
 * @package PHPSupportTickets
 */
abstract class PHPST_SQLBuilder {
    /**
     * Constructor.
     */
    public function __construct() {
    }

    /**
     * Standard generic search, builds a Select statement limited by the arguments given.
     *
     * @access public
     * @param string $table
     * @param int $start The row at which to start returning results
     * @param int $rows The number of rows to return
     * @return string Query
     */
    public function searchAll($table, $start = null, $rows = null) {
        $query = "SELECT * FROM '$table'";
        // Add Limit
        if (isset($start) && isset($rows)) {
            $query .= " LIMIT $start, $rows ";
        }
        return $query;
    }

    /**
     * Standard dynamic search, builds a Select statement based on the arguments given.
     *
     * @access public
     * @param string $table
     * @param array $displayFields The fields to fetch from the table
     * @param array $searchFields An associative array: ['field'] => 'value'
     * @param string $order The field by which to Order to query
     * @param string $sort ASC or DESC, the sort order
     * @param int $start The row at which to start returning results
     * @param int $rows The number of rows to return
     * @return string Query
     */
    public function search($table, $displayFields, $searchFields,
        $order = null, $sort = 'ASC', $start = null, $rows = null) {
        $query = "SELECT ";
        // Add display fields
        if ($displayFields == "All" || count($displayFields) == 0 || !is_array($displayFields)) {
            $query .= " * ";
        } else {
            foreach ($displayFields as $df) {
                $query .= " $table.$df,";
            }
            // Remove last comma
            $query = rtrim($query, ",");
        }

        $query .= " FROM $table ";
        // Add search fields
        if ($searchFields == "All" || count($searchFields) == 0 || !is_array($searchFields)) {
            // Do nothing
        } else {
            $query .= " WHERE ";
            foreach ($searchFields as $field => $value) {
                if (ctype_digit($value) === true) {
                    $query .= " $field = $value AND ";
                } else {
                    $query .= " $field = '$value' AND ";
                }
            }
            // Remove last 'AND'
            $query = substr($query, 0, -4);
        }
        // Add order
        if (isset($order) && isset($sort)) {
            $query .= " ORDER BY $order $sort ";
        }
        // Add Limit
        if (isset($start) && isset($rows)) {
            $query .= " LIMIT $start, $rows ";
        }

        return $query;
    }
    // -------------------------------------------//
    // ----------- ABSTRACT METHODS --------------//
    // THESE MUST BE IMPLEMENTED BY ALL CHILDREN  //
    // -------------------------------------------//
    /**
     * Creates the SQL to insert a new object in the DB.
     *
     * @abstract
     * @param object $object An object whose data is used to insert the new row
     * @param array $columns The fields of the table in which to insert the row
     * @return string an SQL query
     */
    abstract function insert($object, $columns);

    /**
     * Creates the SQL to update an existing object in the DB.
     *
     * @abstract
     * @param object $object An object whose data is used to update the DB
     * @param array $columns The fields of the table with which to update the DB
     * @return string an SQL query
     */
    abstract function update($object, $columns);

    /**
     * Creates the SQL to delete an existing object from the DB.
     *
     * @abstract
     * @param int $ID The unique ID of the object to delete from the DB
     * @return string an SQL query
     */
    abstract function delete($ID);

    /**
     * Creates the SQL to retrieve an existing object from the DB.
     *
     * @abstract
     * @param array $fields The fields used to retrieve a unique record (usually ID) and their values
     * @return string an SQL query
     */
    abstract function get($fields);
}

?>