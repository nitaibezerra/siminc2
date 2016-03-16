<?php
/**
 *
 * @category Procedural File
 * @package PHPSupportTickets
 * @author Ian Warner <iwarner@triangle-solutions.com>
 * @copyright (C) 2005 Triangle Solutions Ltd
 * @version SVN: $Id: config.php 3 2005-12-13 01:34:21Z nicolas $
 * @link http://www.phpsupporttickets.com/
 * @since File available since Release 1.1.1.1
 * \\||
 */
$_GET['tri_debug'] = true;

if (DB_HOST == 'host') {
    die("You haven't changed the Database settings in config.inc.php!!!
    Please edit these settings
    before attempting to use PHP Support Tickets.");
}


function require_once_check($path) {
    if (file_exists($path)) {
        require_once $path;
    } else {
        die("Could not find file $path from " . __FILE__ . " line " . __LINE__ . "\n<br />");
    }
}

/**
 * Include required libraries
 */

require_once_check(PHPST_PATH . 'classes/class.blowfish.php');
require_once_check(PHPST_PATH . 'lang/eng.php');
require_once_check(PHPST_PATH . 'include/urgency.php');
require_once_check(PHPST_PATH . 'classes/GUI/factory.GUI.php');
require_once_check(PHPST_PATH . 'classes/class.iconGui.php');
require_once_check(PHPST_PATH . 'classes/class.imageGui.php');
require_once_check(PHPST_PATH . 'classes/class.paging.php');
require_once_check(PHPST_PATH . 'classes/static/static.datetime.php');
require_once_check(PHPST_PATH . 'classes/static/static.stringformat.php');


/**
 * Beginning of site-specific configuration: start editing
 */
$mysql_version = 0;
if (DB_TYPE == 'mysqli') {
    $mysqli = new mysqli(DB_HOST, DB_USER, DB_PASS);

    if (mysqli_connect_errno()) {
       printf("Connect failed: %s\n", mysqli_connect_error());
       exit();
    }

    $mysql_version = $mysqli->server_info;
    $mysqli->close();

} elseif (DB_TYPE == 'mysql') {

    $link = mysql_connect(DB_HOST, DB_USER, DB_PASS);
    $mysql_version = mysql_get_server_info($link);
    mysql_close($link);
} else {
    die ('Your selected DB type (' . DB_TYPE . ', see config.inc.php) is incompatible with this application.
          It should be either mysqli or mysql');
}

// Exit if the mysql version is too old (under 4.0)
if (substr($mysql_version, 0, 3) < 4.1) {
    print "Your version of MySQL server, " . $mysql_version . ", is too old for
           PHP Support Tickets 2.2, which requires at least MySQL server 4.1.1";
}

define('ADODB_DIR', PHPST_PATH . 'adodb/');
define('DSN', DB_TYPE . '://' . DB_USER . ':' . DB_PASS . '@' . DB_HOST . '/' . DB_DATA . '?persistent');


/**
 * End of site-specific configuration, stop editing
 */

require_once ADODB_DIR . 'adodb.inc.php';
require_once ADODB_DIR . 'adodb-errorhandler.inc.php';
require_once ADODB_DIR . 'tohtml.inc.php';
require_once ADODB_DIR . 'toexport.inc.php';
//define('ADODB_ERROR_LOG_TYPE', '3');
//define('ADODB_ERROR_LOG_DEST', 'error.log');


// SET UP ADODB
$conn = &ADONewConnection(DSN);
if (!$conn) {
    print "Unable to connect to the database with these settings: <br />
           Username: " . DB_USER . "<br />
           Password: " . DB_PASS . "<br />
           Host: " . DB_HOST;
    exit();
}

$conn->SetFetchMode(ADODB_FETCH_BOTH);

// Conditional sql debugging. This will not work on submitted Forms!
if (isset($_GET['db_debug'])) {
    $conn->debug = true;
} else {
    $conn->debug = false;
}

// Conditional Error Reporting
if (isset($_GET['tri_debug'])) {
    $set_ini = ini_set('display_errors', 1);
    error_reporting(E_ALL);
} else {
    $set_ini = ini_set('display_errors', 0);
    error_reporting();
}


// Uncomment this if you want unconditional sql debugging
// $conn->debug = true;
$ADODB_COUNTRECS = true;
// $ADODB_CACHE_DIR = $common_path . 'cache/';

// If createtables.php page is being executed, do not search for options
if (empty($create_tables)) {
    // Pull out option constants
    $sql = "SELECT name, value FROM " . DB_PREFIX_OPTIONS;

    $rs =& $conn->Execute($sql);
    if ($rs != false && $rs->RecordCount() > 0) {
        while (!$rs->EOF) {
            define($rs->fields['name'], $rs->fields['value']);
            $rs->MoveNext();
        }
    }
}
?>