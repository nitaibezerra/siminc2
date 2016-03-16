<?php
/**
 * File containing the DatabaseMirror Interface.
 *
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version SVN: $Id: interface.DatabaseMirror.php 4 2005-12-13 01:47:15Z nicolas $
 * @since File available since Release 1.1.1.1
 * \\||
 */

/**
* This interface defines whether a class is a mirror to a DB table,
* and forces that class to implement the following methods.
*
* @package PHPSupportTickets
*/

interface PHPST_DatabaseMirror {
    public function getID();
    public function addToDB();
    public function removeFromDB($int);
    public function updateDB();
    public static function getFromDB($array);
    public static function validate($array);
}

?>