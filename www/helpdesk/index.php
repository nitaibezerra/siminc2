<?php
/**
 * File containing the index page.
 *
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version SVN: $Id: index.php 4 2005-12-13 01:47:15Z nicolas $
 * @since File available since Release 1.1.1.1
 * \\||
 */

session_start();

error_reporting( E_ALL );

require_once('config.inc.php');
//print 'a';
//die();
require_once(PHPST_PATH . 'include/config.php');

PHPST_GUIFactory::$absoluteURL = BASE_URL . PHPST_INDEXPAGE;

$myGUI = null;

if (!isset($_SESSION['GUI']) || isset($_REQUEST['logout'])) {
    // Upon first visit or logout, show login screen
    $myGUI = PHPST_GUIFactory::getInstance('Login', array());

} elseif (isset($_REQUEST['login'])) {
    // If login info sent, verify user and create appropriate GUI

    $admin_type = PHPST_FormHandler::verify_login($_REQUEST);

    if (!$admin_type) {
        // Validation failed. keep Login GUI and output errors
        $myGUI = unserialize($_SESSION['GUI']);

        $myGUI->update($_REQUEST, $_FILES);
    } else {
        // Validation successful, create correct GUI
        $myGUI = PHPST_GUIFactory::getInstance($admin_type, $_REQUEST);
    }
} elseif (isset($_REQUEST['register'])) {
    // If register info is sent from LoginGUI, try to add user
    $result = PHPST_FormHandler::register_createuser($_REQUEST);
    if ($result === false || is_array($result)) {
        $myGUI = unserialize($_SESSION['GUI']);
        $myGUI->update($_REQUEST, $_FILES);
    } else {
        $myGUI = PHPST_GUIFactory::getInstance('Client', $_REQUEST);
    }
} else {
    $myGUI = unserialize($_SESSION['GUI']);
    $myGUI->update($_REQUEST, $_FILES);
}

$myGUI->output();
$_SESSION['GUI'] = serialize($myGUI);
?>