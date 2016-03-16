<?php
/**
 * Migration script: Database v1.9 -> v2.0
 *
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version CVS: $Id: index.php,v 1.1.1.1 2005/10/19 23:24:04 nicolas Exp $
 * @since File available since Release 1.1.1.1
 * \\||
 */

//  Crash and give error message if php version < 5.0
if (version_compare(phpversion(), '5.0.1') == -1) {
   die("This program is incompatible with your PHP version (" . phpversion() . "). Version 5.0.1+ needed");
}
require_once('include/config.php');

// Settings for new database are already set up in the include/config.php file. Set up the old DB settings here
$host = 'localhost';
$user = 'username';
$pass = 'password';
$data = 'old_tickets';
define('DSN2', "mysqli://$user:$pass@$host/$data");

$conn = &ADONewConnection(DSN);
$old_conn = &ADONewConnection(DSN2);

// Retrieve all data from old DB
// Categories first
$categories = array();
$query = "SELECT * FROM tickets_categories";
$rs = &$old_conn->Execute($query);
while (!$rs->EOF) {
    $row = $rs->fields;
    $categories[] = $row;
    $rs->MoveNext();
}

// Users next
$users = array();
$query = "SELECT * FROM tickets_users";
$rs = &$old_conn->Execute($query);
while (!$rs->EOF) {
    $row = $rs->fields;
    $users[] = $row;
    $rs->MoveNext();
}

// Tickets finally
$tickets = array();
$query = "SELECT * FROM tickets_tickets";
$rs = &$old_conn->Execute($query);
while (!$rs->EOF) {
    $row = $rs->fields;
    $tickets[] = $row;
    $rs->MoveNext();
}

// Target DB MUST be empty!!!!

// Process categories into departments
foreach ($categories as $category) {
    $query = "INSERT INTO departments (
            `ID`, `name`)
        VALUES (
            '{$category['tickets_categories_id']}',
            '{$category['tickets_categories_name']}')";
    $rs = &$conn->Execute($query);
}

// Process users
// Start by adding Management and Support departments for Admins and Mods
$query = "INSERT INTO departments (`name`) VALUES ('Management')";
$rs = &$conn->Execute($query);
$management_ID = $conn->Insert_ID();
$query = "INSERT INTO departments (`name`) VALUES ('Support')";
$rs = &$conn->Execute($query);
$support_ID = $conn->Insert_ID();

foreach ($users as $user) {
    // Add users
    $query = "INSERT INTO users (
            `ID`, `nickname`, `password`, `username`, `email`, `last_login`, `timestamp`, `admin_status`)
        VALUES (
            '{$user['tickets_users_id']}',
            '{$user['tickets_users_name']}',
            '{$user['tickets_users_password']}',
            '{$user['tickets_users_username']}',
            '{$user['tickets_users_email']}',
            '{$user['tickets_users_lastlogin']}',
            '{$user['tickets_users_newlogin']}',
            '{$user['tickets_users_admin']}')";
    $rs = &$conn->Execute($query);

    // Assign users to departments based on Admin status
    if ($user['tickets_users_admin'] == "Admin") {
        $query = "INSERT INTO department_users (`department_ID`, `user_ID`) VALUES ('$management_ID', '{$user['tickets_users_id']}')";
        $rs = &$conn->Execute($query);
    } elseif ($user['tickets_users_admin'] == "Mod") {
        $query = "INSERT INTO department_users (`department_ID`, `user_ID`) VALUES ('$support_ID', '{$user['tickets_users_id']}')";
        $rs = &$conn->Execute($query);
    } else {
        // Do nothing if user is neither Admin nor Mod
    }
}

// Process tickets into tickets and answers

foreach ($tickets as $ticket) {
    // First retrieve the user_ID of the users referenced by the username in the old tickets table
    $query = "SELECT `ID` FROM users WHERE `username` = '{$ticket['tickets_username']}'";
    $rs = &$conn->Execute($query);
    $user_ID = $rs->fields['ID'];

    // Retrieve the department_ID referenced by the category name in the old table
    $query = "SELECT `ID` FROM departments WHERE `name` = '{$ticket['tickets_category']}'";
    $rs = &$conn->Execute($query);
    $department_ID = $rs->fields['ID'];

    // Enter the ticket if the child field is 0, otherwise enter it as an answer
    if ($ticket['tickets_child'] == 0) {
        $query = "INSERT INTO tickets (
                `ID`, `user_ID`, `subject`, `timestamp`, status, urgency, department_ID, body)
            VALUES (
                '{$ticket['tickets_id']}',
                '$user_ID',
                '{$ticket['tickets_subject']}',
                '{$ticket['tickets_timestamp']}',
                '{$ticket['tickets_status']}',
                '{$ticket['tickets_urgency']}',
                '$department_ID',
                '{$ticket['tickets_question']}')";
    } else {
        $query = "INSERT INTO answers (
                `ID`, `user_ID`, `ticket_ID`, `subject`, `timestamp`, body)
            VALUES (
                '{$ticket['tickets_id']}',
                '$user_ID',
                '{$ticket['tickets_child']}',
                '{$ticket['tickets_subject']}',
                '{$ticket['tickets_timestamp']}',
                '{$ticket['tickets_question']}')";
    }
    $rs = &$conn->Execute($query);
}

?>