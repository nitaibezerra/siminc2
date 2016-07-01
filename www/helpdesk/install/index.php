<?php
/**
 * Installs the application.
 *
 * @package   phpsupporttickets_procedural
 * @author    Triangle Solutions Ltd
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version   SVN: $Id: $
 * @todo      Check that mysqli - mysql extensions are loaded
 * \\||
 */

// Include the needed files

// include_once '../classes/functions.php';
include_once '../adodb/adodb.inc.php';
include_once 'header.php';

$set_ini = ini_set('display_errors', 1);
error_reporting(E_ALL);

$error_msg = '';

require_once 'HTML/QuickForm.php';


/**
 * Shortcut function for showing the form.
 *
 */
function show_form($form)
{
    echo '<center>';
    echo '<h1>PHP Support Tickets 2.2 Installation</h1>';
    echo '<p>Note: for hints, leave your mouse pointer over the fields.</p>';
    $form->display();
    echo '</center>';
}

/**
 * Shortcut function for displaying error messages.
 *
 * @param object $form
 * @param string $error_msg
 */
function show_error($form, $error_msg)
{
    echo '<center><span class="redtext">' . $error_msg . '</span></center>';
    die(show_form($form));
}

$action_url = 'index.php';

// Prepare arrays
$mail_methods = array('mail' => 'mail (PHP native function)',
                      'sendmail' => 'sendmail',
                      'smtp' => 'smtp',
                      'qmail' => 'qmail');

// Prepare defaults
$defaults = array('db_host' => 'localhost',
                  'db_data' => 'phpst',
                  'prefix' => 'tickets_',
                  'users_table' => 'users',
                  'users_table_id' => 'id',
                  'users_table_user' => 'username',
                  'users_table_pass' => 'password',
                  'users_table_name' => 'name',
                  'users_table_email' => 'email',
                  'users_table_timestamp' => 'timestamp',
                  'users_table_admin' => 'admin',
                  'index_page' => 'index.php?',
                  'base_url' => rtrim('http://' . $_SERVER['HTTP_HOST'] . $_SERVER['PHP_SELF'], 'install/index.php') . '/',
                  'upload_relative_path' => rtrim('http://' . $_SERVER['HTTP_HOST'] . $_SERVER['PHP_SELF'], 'install/index.php') . '/upload/',
                  'upload_absolute_path' => rtrim($_SERVER['SCRIPT_FILENAME'], 'install/index.php') . '/upload/',
                  'mail_to' => 'support@domain.com',
                  'mail_subject' => 'Contact from Support Tickets',
                  'smtp_socket_from' => 'support@domain.com',
                  'smtp_socket_from_name' => 'Admin',
                  'smtp_socket_reply' => 'support@domain.com',
                  'smtp_socket_reply_name' => 'Admin',
                  'smtp_host' => 'support@domain.com',
                  'smtp_user' => 'support@domain.com'
                  );

$input_width = 35;

// Instantiate the HTML_QuickForm object
$form = new HTML_QuickForm('setupform', 'post', $action_url);
$form->setDefaults($defaults);

// Add elements to the form
$form->addElement('html', '<fieldset><legend>Database connection</legend><table summary="Database Connection settings">');
$form->addElement('html', '<caption>This automatically creates a config.php file</caption>');
$form->addElement('select', 'db_type', 'Type', array('mysql' => 'mysql', 'mysqli' => 'mysqli'), array('title' => 'The type of Database connection to use.'));
$form->addElement('text', 'db_host', 'Host', array('title' => 'Your MySQL server host name (normally localhost)', 'size' => $input_width));
$form->addElement('text', 'db_user', 'Username', array('title' => 'The username for your MySQL server account', 'size' => $input_width));
$form->addElement('password', 'db_pass', 'Password', array('title' => 'The password for your MySQL server account', 'size' => $input_width));
$form->addElement('text', 'db_data', 'Database Name', array('title' => 'The Database name for your suppport tickets (may be an existing table).', 'size' => $input_width));
$form->addElement('text', 'prefix', 'Table Prefix', array('title' => 'The prefix to put before all table names (to avoid table name conflicts).', 'size' => $input_width));

$form->addElement('html', '</table></fieldset><fieldset><legend>Users table merge</legend><table summary="Users table merge">');
$form->addElement('html', '<caption>Change these settings if you want to use an existing users table (enter your existing field names)</caption>');
$form->addElement('text', 'users_table', 'Users Table', array('title' => 'Existing users table name or new users table name', 'size' => $input_width));
$form->addElement('text', 'users_table_id', 'id field', array('title' => 'integer(16) unsigned auto-increment', 'size' => $input_width));
$form->addElement('text', 'users_table_user', 'username field', array('title' => 'varchar (64)', 'size' => $input_width));
$form->addElement('text', 'users_table_pass', 'password field', array('title' => 'varchar (64)', 'size' => $input_width));
$form->addElement('text', 'users_table_name', 'name field', array('title' => 'varchar (64)', 'size' => $input_width));
$form->addElement('text', 'users_table_email', 'email field', array('title' => 'varchar (128)', 'size' => $input_width));
$form->addElement('text', 'users_table_timestamp', 'timestamp field', array('title' => 'integer (10) unsigned', 'size' => $input_width));
$form->addElement('text', 'users_table_admin', 'admin status field', array('title' => 'enum(\'Admin\', \'Mod\', \'Client\'), Default Mod', 'size' => $input_width));

$form->addElement('html', '</table></fieldset><fieldset><legend>Account Settings</legend><table summary="Account Settings">');
$form->addElement('html', '<caption>Creates the Main Administrator account for this installation</caption>');
$form->addElement('text', 'admin_user', 'Username', array('size' => $input_width));
$form->addElement('password', 'admin_pass', 'Password', array('size' => $input_width));
$form->addElement('password', 'admin_pass_repeat', 'Confirm Password', array('size' => $input_width));
$form->addElement('text', 'admin_email', 'Email', array('size' => $input_width));

$form->addElement('html', '</table></fieldset><fieldset><legend>Path Settings</legend><table summary="Path Settings">');
$form->addElement('html', '<caption>You must set these correctly before the script can run.</caption>');
$form->addElement('text', 'index_page', 'Index Page', array('title' => 'The root page from which all of PHPST is run. Default is index.php?', 'size' => $input_width));
$form->addElement('text', 'phpst_path', 'Script Location', array('title' => 'Optional path to tickets script (e.g. \'pages/tickets/\')', 'size' => $input_width));
$form->addElement('text', 'base_url', 'Base URL', array('title' => 'Set this constant to the URL location of index.php', 'size' => $input_width));
$form->addElement('text', 'upload_absolute_path', 'Upload physical Path', array('title' => 'The physical path to your upload directory.', 'size' => $input_width));
$form->addElement('text', 'upload_relative_path', 'Upload URL', array('title' => 'The URL path to your upload directory.', 'size' => $input_width));

$form->addElement('html', '</table></fieldset><fieldset><legend>Email Settings</legend><table summary="Email Settings">');
$form->addElement('html', '<caption>You need to set these up if you want to receive email notifications</caption>');
$form->addElement('text', 'mail_to', 'Main Suppport Email', array('title' => 'This is the address that will receive all email notifications if such are turned on', 'size' => $input_width));
$form->addElement('text', 'mail_name', 'Email name', array('title' => 'This is the name that will appear on email notifications', 'size' => $input_width));
$form->addElement('text', 'mail_subject', 'Email subject', array('title' => 'This is the subject that will appear on email notifications', 'size' => $input_width));
$form->addElement('select', 'mail_method', 'Email method', $mail_methods, array('title' => 'Email method: mail, sendmail, smtp or qmail.'));
$form->addElement('text', 'smtp_socket_from', 'SMTP \'From\' Email', array('title' => 'Email address to appear in \'from\' field', 'size' => $input_width));
$form->addElement('text', 'smtp_socket_from_name', 'SMTP \'From\' Name', array('title' => 'Email address to appear in \'from\' field', 'size' => $input_width));
$form->addElement('text', 'smtp_socket_reply', 'SMTP \'Reply\' Email', array('title' => 'Email address to appear in \'reply\' field', 'size' => $input_width));
$form->addElement('text', 'smtp_socket_reply_name', 'SMTP \'Reply\' Name', array('title' => 'Email address to appear in \'reply\' field', 'size' => $input_width));
$form->addElement('text', 'smtp_host', 'SMTP host', array('title' => 'SMTP host to send the emails via the smtp socket', 'size' => $input_width));
$form->addElement('select', 'smtp_auth', 'SMTP Authentication', array(true => 'on', false => 'off'), array('title' => 'Set this to ON if your SMTP server requires authentication'));
$form->addElement('text', 'smtp_user', 'SMTP username', array('title' => 'SMTP username - usually the same as your mailbox', 'size' => $input_width));
$form->addElement('text', 'smtp_pass', 'SMTP password', array('title' => 'SMTP password', 'size' => $input_width));

$form->addElement('html', '</table></fieldset>');
$form->addElement('submit', 'button', 'Submit', array('class' => 'button'));

// Required field Rules
$form->addRule('db_host', 'Please enter your database Host (usually localhost)', 'required', null, 'client');
$form->addRule('db_user', 'Please enter the username for your database.', 'required', null, 'client');
$form->addRule('db_pass', 'Please enter the password for your database.', 'required', null, 'client');
$form->addRule('db_data', 'Please enter the name of your database.', 'required', null, 'client');
$form->addRule('admin_user', 'Please enter the username for the administrator account', 'required', null, 'client');
$form->addRule('admin_pass', 'Please enter the password for the administrator account', 'required', null, 'client');
$form->addRule('admin_pass_repeat', 'Please confirm the password for the administrator account', 'required', null, 'client');
$form->addRule(array('admin_pass', 'admin_pass_repeat'), 'The passwords do not match', 'compare', null, 'client');
$form->addRule('admin_email', 'Please enter the administrator\'s email address', 'required', null, 'client');
$form->addRule('index_page', 'Please enter the name of the index page (plus a question mark)', 'required', null, 'client');
$form->addRule('base_url', 'Please enter the base url (http://domain/phpst_installation/)', 'required', null, 'client');
$form->addRule('upload_absolute_path', 'Please enter the physical path to your upload directory', 'required', null, 'client');
$form->addRule('upload_relative_path', 'Please enter the URL to your upload directory', 'required', null, 'client');
$form->addRule('mail_to', 'Please enter the main support email address', 'required', null, 'client');

// String format rules
$form->addRule('admin_email', 'Please enter a valid Email Address.', 'email', null, 'client');

// Filters
$form->applyFilter('db_host', 'trim');
$form->applyFilter('db_user', 'trim');
$form->applyFilter('url', 'trim');
$form->applyFilter('db_pass', 'trim');
$form->applyFilter('db_data', 'trim');
$form->applyFilter('prefix', 'trim');
$form->applyFilter('admin_user', 'trim');
$form->applyFilter('admin_pass', 'trim');
$form->applyFilter('admin_pass_repeat', 'trim');
$form->applyFilter('admin_email', 'trim');


// Set up renderer
$renderer =& HTML_QuickForm::defaultRenderer();
$form->_requiredNote = '<span style="font-size:100%; color:#ff0000;">*</span><span style="font-size:100%;"> denotes required field</span>';

$renderer->setFormTemplate('
        <form{attributes}>
          {content}
          </table>
        </form>');

$renderer->setElementTemplate('
        <table summary="Shows the submit button">
        <tr>
          <td valign="top" colspan="2" align="center">
            {element}
          </td>
        </tr>
        </table>
        <table summary="Note about required fields">',
        'button');

$renderer->setElementTemplate('
        <tr>
          <th width="120" align="left" valign="top">

            <label>{label}<!-- BEGIN required -->
            <span style="color: #ff0000">*</span>
            <!-- END required --></label>

          </th>
          <td valign="top" align="left">
            <!-- BEGIN error -->
            <span style="color: #ff0000">{error}</span><br />
            <!-- END error -->
            {element}
          </td>
        </tr>');

$form->accept($renderer);

// Try to validate a form
if ($form->validate()) {
    // Check that the database type is acceptable

    $php_version = '5.0.1';
    $mysql_version = 0;

    if (extension_loaded('mysqli')) {
        define('DB_EXT', 'mysqli');

        $mysqli = new mysqli($_POST['db_host'], $_POST['db_user'], $_POST['db_pass']);

        if (mysqli_connect_errno()) {
           show_error($form, "Connect failed: " . mysqli_connect_error());
        } else {
            $connect = true;
        }

        $mysql_version = $mysqli->server_info;
        $mysqli->close();

    } elseif (extension_loaded('mysql')) {
        if (!$link = mysql_connect($_POST['db_host'], $_POST['db_user'], $_POST['db_pass'])) {
            show_error($form, "Connect failed: " . mysql_error($link));
        } else {
            $connect = true;
        }

        $mysql_version = mysql_get_server_info($link);
        mysql_close($link);

    } else {
        show_error($form, "You do not have the mysql or mysqli extension enabled in php.ini. Please enable either one or the other.");
    }

    $conn = ADONewConnection($_POST['db_type']);
    $conn->Connect($_POST['db_host'], $_POST['db_user'], $_POST['db_pass']);

    // Exit if the mysql version is too old (under 4.0)

    if (substr($mysql_version, 0, 3) < 4.1) {
        show_error($form, "Your version of MySQL server, " . $mysql_version . ", is too old for
               PHP Support Tickets 2.2, which requires at least MySQL server 4.1.1");
    }

    // Check that the PHP version is suitably high enough to use

    elseif (PHP_VERSION < $php_version) {
        show_error($form, '<p>Error [Your PHP version is less than the required <?php echo $php_version ?>. Please update this at <a href="http://www.php.net" target="_blank" title="PHP.net">PHP.net</a>]</p>');
    }

    // If the config file already exists then detect this and comment

    elseif (is_file('../config.inc.php')) {
        show_error($form, '<p>Previous Install Detected - Please delete the previous installation or rename to an obsolete folder.</p>');
    }

    if (isset($connect) && $connect == true) {

        $file = 'config.inc.php';
        $path = '../';

        if ($handle = fopen($path . $file, 'wb')) {

            $text = '<?php
/**
 * You may wish to edit these settings after the install script has created it
 *
 * DO NOT CHANGE ANY OF THE TEXT IN UPPER CASE ON THIS PAGE.
 * If you do, the program will not work. For example, do not
 * modify PHPST_INDEXPAGE, only change the value next to it, "index.php?"
 */

// This is the root page from which all of PHPST is run. Default it index.php?
define(\'PHPST_INDEXPAGE\', "' . $_POST['index_page'] . '");

// If your tickets installation is in a subfolder of the script from which it
// will be running, set this constant to that path (e.g. \'pages/tickets/\')
define(\'PHPST_PATH\', \'' . $_POST['phpst_path'] . '\');

// Set this constant to the URL location of index.php
define(\'BASE_URL\', \'' . $_POST['base_url'] . '\');

// The URL and the path to your upload directory
define(\'PHPST_UPLOAD_PATH\', \'' . $_POST['upload_absolute_path'] . '\');
define(\'PHPST_UPLOAD_RELATIVE_PATH\', \'' . $_POST['upload_relative_path'] . '\');

// This is the address that will receive all email notifications if such are turned on
define(\'PHPST_MAIL_TO\', \'' . $_POST['mail_to'] . '\');

// This is the name that will appear on email notifications
define(\'PHPST_MAIL_NAME\', \'' . $_POST['mail_name'] . '\');

// This is the subject that will appear on email notifications
define(\'PHPST_MAIL_SUBJECT\', \'' . $_POST['mail_subject'] . '\');

// Choose your email method
// Sets the send method for all the mailings
// coming out of this app - Following are options
// If you are getting errors then try a different option
// - smtp - sends the mail via sockets through sockethost
// - sendmail - USES SENDMAIL TO SEND MAIL
// - mail - USES PHP INBUILT MAIL FUNCTION
// - qmail - USES QMAIL TO SEND THROUGH
define(\'PHPST_MAIL_SENDMETHOD\', \'' . $_POST['mail_method'] . '\');

// If you chose SMTP, you will need to set these constants too
// email address to appear in from
define(\'PHPST_MAIL_SOCKETFROM\', \'' . $_POST['smtp_socket_from'] . '\');

// name to appear in from field
define(\'PHPST_MAIL_SOCKETFROMNAME\', \'' . $_POST['smtp_socket_from_name'] . '\');

// email address to reply to
define(\'PHPST_MAIL_SOCKETREPLY\', \'' . $_POST['smtp_socket_reply'] . '\');

// name for reply email
define(\'PHPST_MAIL_SOCKETREPLYNAME\', \'' . $_POST['smtp_socket_reply_name'] . '\');

// smtp host to send the emails via the smtp socket
// this may simply be localhost
define(\'PHPST_MAIL_SOCKETHOST\', \'' . $_POST['smtp_host'] . '\');

// If you use smtp authentication:
// set this to true if your smtp server requires authentication
define(\'PHPST_MAIL_SMTPAUTH\', ' . $_POST['smtp_auth'] . ');

// smtp username - usually the same as your mailbox
define(\'PHPST_MAIL_SMTPAUTHUSER\', \'' . $_POST['smtp_user'] . '\');

// smtp password - usually the same as your mailbox
define(\'PHPST_MAIL_SMTPAUTHPASS\', \'' . $_POST['smtp_pass'] . '\');

// Database set up
define(\'DB_HOST\', \'' . $_POST['db_host'] . '\');
define(\'DB_USER\', \'' . $_POST['db_user'] . '\');
define(\'DB_PASS\', \'' . $_POST['db_pass'] . '\');
define(\'DB_TYPE\', \'' . $_POST['db_type']. '\');
define(\'DB_DATA\', \'' . $_POST['db_data'] . '\');

// These are the names of the tables you will use for PHPST.
// If you modify these in order to merge the users tables,
// you will only need to modify the tables with phpmyadmin or a similar tool.
// PHPST doesn\'t have table names hard-coded in.

// You should always use a prefix to avoid table name conflicts
define(\'DB_PREFIX\', \'' . $_POST['prefix'] . '\');

// The users table is likely to become common with an existing one.
// Enter its name here if needed. Otherwise enter the name of the new users table.
define(\'DB_PREFIX_USER\', \'' . $_POST['users_table'] . '\');

// If you are merging users tables, you may want to enter here the names of
// your existing fields that are compatible with the fields needed by PHPST
// Here is a data dictionary of what is needed (items in [] are optional):
//    ID : integer, [16], [unsigned], unique, PK, auto-increment
//    username: varchar, [64]
//    password: varchar, [64]
//    name: varchar, [64]
//    email: varchar, [128]
//    timestamp: int, [16], [unsigned]
//    admin: enum(\'Admin\', \'Mod\', \'Client\'), Default Mod
//
// It is likely that you will need to add the Admin field to your existing users table.
// The other fields should already exist if you keep users data, so enter their names here.
// Otherwise you may leave these fields as they are, and a new table will be created.
define(\'DB_PREFIX_USER_ID\', \'' . $_POST['users_table_id'] . '\');
define(\'DB_PREFIX_USER_USERNAME\', \'' . $_POST['users_table_user'] . '\');
define(\'DB_PREFIX_USER_PASSWORD\', \'' . $_POST['users_table_pass'] . '\');
define(\'DB_PREFIX_USER_NAME\', \'' . $_POST['users_table_name'] . '\');
define(\'DB_PREFIX_USER_EMAIL\', \'' . $_POST['users_table_email'] . '\');
define(\'DB_PREFIX_USER_TIMESTAMP\', \'' . $_POST['users_table_timestamp'] . '\');
define(\'DB_PREFIX_USER_ADMIN\', \'' . $_POST['users_table_admin'] . '\');

// The following shouldn\'t need to be changed
define(\'DB_PREFIX_ANSWERS\', DB_PREFIX . \'answers\');
define(\'DB_PREFIX_TICKETS\', DB_PREFIX . \'tickets\');
define(\'DB_PREFIX_DEPARTMENTS\', DB_PREFIX . \'departments\');
define(\'DB_PREFIX_DEPARTMENTS_USERS\', DB_PREFIX . \'department_users\');
define(\'DB_PREFIX_OPTIONS\', DB_PREFIX . \'options\');
define(\'DB_PREFIX_HISTORYLOG\', DB_PREFIX . \'history_log\');
?>';
            $fp = fputs($handle, $text);

            if (file_exists($path . $file)) {
                $created = true;
            } else {
                show_error($form, '<p>Could not write to config file</p>');
            }
        } else {
            show_error($form, '<p>Could not create config file</p>');
        }
    } else {
        show_error($form, '<p>No DB connection so will not create config file</p>');
    }

    if (!isset($error) && empty($error_msg) &&
         isset($connect) && $connect == true &&
         isset($created) && $created == true) {

        // Check to see if a database is already present with the entered name

        if ($_POST['db_data']) {

            $found = false;

            $show  = "SHOW DATABASES LIKE '" . $_POST['db_data'] . "'";
            $dbs   = $conn->Execute($show) or show_error($form, $conn->ErrorMsg() . $show);
            $found = $dbs->RecordCount();

            if (!$found) {

                $conn = null;

                // Create the tickets database

                $conn = ADONewConnection($_POST['db_type']);
                $conn->Connect($_POST['db_host'], $_POST['db_user'], $_POST['db_pass']);

                $sql = 'CREATE DATABASE ' . $_POST['db_data'];

                $rs =& $conn->Execute($sql);

                if ($rs) {
                    echo '<center><p class="redtext">Database ' . $_POST['db_data'] . ' Created</p></center>';
                    $conn->Connect($_POST['db_host'], $_POST['db_user'], $_POST['db_pass'], $_POST['db_data']);
                } elseif ($_POST['db_type'] != 'sqlite') {
                    show_error($form, 'Error creating [' . $_POST['db_data'] . '] database' . '<br />Error [' . __FILE__ . '/' . __LINE__ . '] - ' . $conn->ErrorMsg());
                }
            } else {
                $conn->Connect($_POST['db_host'], $_POST['db_user'], $_POST['db_pass'], $_POST['db_data']);
            }
        }

        define('DB_PREFIX_USER', $_POST['users_table']);
        define('DB_PREFIX', $_POST['prefix']);

        // If you are merging users tables, you may want to enter here the names of
        // your existing fields that are compatible with the fields needed by PHPST
        // Here is a data dictionary of what is needed (items in [] are optional):
        //    ID : integer, [16], [unsigned], unique, PK, auto-increment
        //    username: varchar, [64]
        //    password: varchar, [64]
        //    name: varchar, [64]
        //    email: varchar, [128]
        //    timestamp: int, [16], [unsigned]
        //    admin: enum('Admin', 'Mod', 'Client'), Default Mod
        //
        // It is likely that you will need to add the Admin field to your existing users table.
        // The other fields should already exist if you keep users data, so enter their names here.
        // Otherwise you may leave these fields as they are, and a new table will be created.
        define('DB_PREFIX_USER_ID', $_POST['users_table_id']);
        define('DB_PREFIX_USER_USERNAME', $_POST['users_table_user']);
        define('DB_PREFIX_USER_PASSWORD', $_POST['users_table_pass']);
        define('DB_PREFIX_USER_NAME', $_POST['users_table_name']);
        define('DB_PREFIX_USER_EMAIL', $_POST['users_table_email']);
        define('DB_PREFIX_USER_TIMESTAMP', $_POST['users_table_timestamp']);
        define('DB_PREFIX_USER_ADMIN', $_POST['users_table_admin']);

        // The following shouldn't need to be changed
        define('DB_PREFIX_ANSWERS', DB_PREFIX . 'answers');
        define('DB_PREFIX_TICKETS', DB_PREFIX . 'tickets');
        define('DB_PREFIX_DEPARTMENTS', DB_PREFIX . 'departments');
        define('DB_PREFIX_DEPARTMENTS_USERS', DB_PREFIX . 'department_users');
        define('DB_PREFIX_OPTIONS', DB_PREFIX . 'options');
        define('DB_PREFIX_HISTORYLOG', DB_PREFIX . 'history_log');

        $sql_array = array();

        $sql_array[] = "DROP TABLE IF EXISTS `" . DB_PREFIX_ANSWERS . "`";
        $sql_array[] = "DROP TABLE IF EXISTS `" . DB_PREFIX_DEPARTMENTS . "`";
        $sql_array[] = "DROP TABLE IF EXISTS `" . DB_PREFIX_DEPARTMENTS_USERS . "`";
        $sql_array[] = "DROP TABLE IF EXISTS `" . DB_PREFIX_OPTIONS . "`";
        $sql_array[] = "DROP TABLE IF EXISTS `" . DB_PREFIX_TICKETS . "`";
        $sql_array[] = "DROP TABLE IF EXISTS `" . DB_PREFIX_HISTORYLOG . "`";
        $sql_array[] = "DROP TABLE IF EXISTS `" . DB_PREFIX_USER . "`";

        $sql_array[] = "
        --
        -- Table structure for table `" . DB_PREFIX_ANSWERS . "`
        --

        CREATE TABLE IF NOT EXISTS `" . DB_PREFIX_ANSWERS . "` (
          `id` int(10) unsigned NOT NULL auto_increment,
          `user_id` int(32) unsigned NOT NULL default '0',
          `ticket_id` int(32) unsigned NOT NULL default '0',
          `body` text NOT NULL,
          `timestamp` int(16) unsigned NOT NULL default '0',
          `rating` int(1) unsigned NOT NULL default '0',
          `subject` varchar(64) NOT NULL default '',
          UNIQUE KEY `id` (`id`),
          KEY `user_id` (`user_id`),
          KEY `ticket_id` (`ticket_id`)
        )";

        $sql_array[] = "
        -- --------------------------------------------------------

        --
        -- Table structure for table `" . DB_PREFIX_DEPARTMENTS_USERS . "`
        --

        CREATE TABLE IF NOT EXISTS `" . DB_PREFIX_DEPARTMENTS_USERS . "` (
          `id` int(16) unsigned NOT NULL auto_increment,
          `department_id` int(16) unsigned NOT NULL default '0',
          `user_id` int(16) unsigned NOT NULL default '0',
          UNIQUE KEY `id` (`id`),
          KEY `department_id` (`department_id`),
          KEY `user_id` (`user_id`)
        )";

        $sql_array[] = "

        --
        -- Dumping data for table `" . DB_PREFIX_DEPARTMENTS_USERS . "`
        --

        INSERT INTO `" . DB_PREFIX_DEPARTMENTS_USERS . "` VALUES (1, 1, 1)";

        $sql_array[] = "

        -- --------------------------------------------------------

        --
        -- Table structure for table `" . DB_PREFIX_DEPARTMENTS . "`
        --

        CREATE TABLE IF NOT EXISTS `" . DB_PREFIX_DEPARTMENTS . "` (
          `id` int(16) NOT NULL auto_increment,
          `name` varchar(64) NOT NULL default '',
          `description` varchar(255) NOT NULL default '',
          `status` enum('Active', 'Suspended') NOT NULL default 'Active',
          UNIQUE KEY `id` (`id`)
        )";

        $sql_array[] = "

        --
        -- Dumping data for table `" . DB_PREFIX_DEPARTMENTS . "`
        --

        INSERT INTO `" . DB_PREFIX_DEPARTMENTS . "` VALUES (1, 'Administration', '', 'Active')";

        $sql_array[] = "

        -- --------------------------------------------------------

        --
        -- Table structure for table `" . DB_PREFIX_HISTORYLOG . "`
        --

        CREATE TABLE IF NOT EXISTS `" . DB_PREFIX_HISTORYLOG . "` (
          `id` int(10) unsigned NOT NULL auto_increment,
          `message` text NOT NULL,
          `type` varchar(64) NOT NULL default '',
          `priority` enum('1','2','3','4','5') NOT NULL default '1',
          `user_id` int(16) unsigned NOT NULL default '0',
          `file` text NOT NULL default '',
          `line` varchar(64) NOT NULL default '',
          `ip` varchar(16) NOT NULL default '',
          `referer` varchar(255) NOT NULL default '',
          `timestamp` int(12) unsigned NOT NULL default '0',
          UNIQUE KEY `id` (`id`),
          KEY `user_id` (`user_id`)
        )";

        $sql_array[] = "
        -- --------------------------------------------------------
        --
        -- Table structure for table `" . DB_PREFIX_OPTIONS . "`
        --

        CREATE TABLE IF NOT EXISTS `" . DB_PREFIX_OPTIONS . "` (
          `id` int(10) unsigned NOT NULL auto_increment,
          `name` varchar(255) NOT NULL default '',
          `value` varchar(32) NOT NULL default '',
          `changed` int(16) unsigned NOT NULL default '0',
          `type` varchar(16) NOT NULL default '',
          PRIMARY KEY  (`id`),
          KEY `changed` (`changed`)
        )";

        $sql_array[] = "

        --
        -- Dumping data for table `" . DB_PREFIX_OPTIONS . "`
        --

        INSERT INTO `" . DB_PREFIX_OPTIONS . "` VALUES (1, 'OPTION_EMAIL_MODS_WHEN_NEW_TICKET', '1', 1123815378, 'boolean')";

        $sql_array[] = "INSERT INTO `" . DB_PREFIX_OPTIONS . "` VALUES (2, 'OPTION_EMAIL_CLIENT_WHEN_NEW_ANSWER', '1', 1123815378, 'boolean')";
        $sql_array[] = "INSERT INTO `" . DB_PREFIX_OPTIONS . "` VALUES (3, 'OPTION_RECENT_TICKETS_DAYS', '7', 1124320692, 'integer')";


        $sql_array[] = "
        -- --------------------------------------------------------

        --
        -- Table structure for table `" . DB_PREFIX_TICKETS . "`
        --

        CREATE TABLE IF NOT EXISTS `" . DB_PREFIX_TICKETS . "` (
          `id` int(5) unsigned NOT NULL auto_increment,
          `user_id` int(16) unsigned NOT NULL,
          `subject` varchar(50) NOT NULL default '',
          `timestamp` bigint(10) unsigned NOT NULL default '0',
          `status` set('Open','Closed') NOT NULL default 'Open',
          `urgency` set('1','2','3','4') NOT NULL default '1',
          `body` text NOT NULL,
          `department_id` int(16) unsigned default NULL,
          PRIMARY KEY  (`id`),
          KEY `user_id` (`user_id`),
          KEY `department_id` (`department_id`)
        )";

        $sql_array[] = "

        -- --------------------------------------------------------

        --
        -- Table structure for table `" . DB_PREFIX_USER . "`
        --

        CREATE TABLE IF NOT EXISTS `" . DB_PREFIX_USER . "` (
          `" . DB_PREFIX_USER_ID . "` int(16) unsigned NOT NULL auto_increment,
          `" . DB_PREFIX_USER_USERNAME . "` varchar(16) binary NOT NULL default '',
          `" . DB_PREFIX_USER_PASSWORD . "` varchar(32) binary NOT NULL default '',
          `" . DB_PREFIX_USER_NAME . "` varchar(30) binary NOT NULL,
          `" . DB_PREFIX_USER_EMAIL . "` varchar(150) binary NOT NULL default '',
          `" . DB_PREFIX_USER_TIMESTAMP . "` int(16) NOT NULL default '0',
          `" . DB_PREFIX_USER_ADMIN . "` enum('Admin','Mod','Client') NOT NULL default 'Mod',
          UNIQUE KEY `ID` (`" . DB_PREFIX_USER_ID . "`)
        )";

        $sql_array[] = "

        --
        -- Dumping data for table `" . DB_PREFIX_USER . "`
        --

        INSERT INTO `" . DB_PREFIX_USER . "` VALUES (1, '" . $_POST['admin_user'] . "', '" . $_POST['admin_pass'] . "', 'Administrator', '" . $_POST['admin_email'] . "', UNIX_TIMESTAMP(), 'Admin')";

        $msg = '';

        foreach($sql_array as $sql) {
            $rs  = &$conn->Execute($sql);
            if (!$rs) {
                show_error($form, '<p>Error: ' . $conn->ErrorMsg() . '</p>');
            }
            sleep(1);
        }

        if ($error_msg == '') {
            echo '<center><p class="redtext">Congratulations! PHP Support Tickets has been
                  successfully installed and configured</p>
                  <a href="../" title="User Front End">[User Front End]</a></center>';
        }
    }
} else {
    show_form($form);
}

include_once 'footer.php';
?>
