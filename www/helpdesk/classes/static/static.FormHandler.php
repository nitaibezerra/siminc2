<?php
/**
 * File containing the FormHandler class.
 *
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version SVN: $Id: static.FormHandler.php 4 2005-12-13 01:47:15Z nicolas $
 * @since File available since Release 1.1.1.1
 * \\||
 */

/**
* user defined includes.
* @include
*/
require_once_check(PHPST_PATH . 'classes/class.User.php');
require_once_check(PHPST_PATH . 'classes/class.Answer.php');
require_once_check(PHPST_PATH . 'classes/class.Department.php');
require_once_check(PHPST_PATH . 'classes/class.Ticket.php');
require_once_check(PHPST_PATH . 'classes/class.Note.php');

/**
 * This static function class is a relay between the GUI and the DatabaseMirror classes.
 * It processes the form data and returns error messages if such errors occur.
 *
 * @access public
 * @package PHPSupportTicket
 */
class PHPST_FormHandler{
    /// --- FIELDS ---

    /**
    * Array of authorised file types.
    *
    * @static
    * @access public
    * @var array $allowed_types
    */
    public static $allowedtypes = array(
            'image/jpeg',
            'image/gif',
            'image/png',
            'application/msword',
            'application/pdf',
            'application/zip',
            'application/octet-stream',
            'text/plain',
            'text/css',
            'text/html',
            'text/x-javascript',
            'text/xml');


    /**
    * Array of file types and their extensions.
    *
    * @static
    * @access public
    * @var array $filetypes
    */
    public static $filetypes = 	array (
            'image/jpeg' => '.jpg .jpeg',
            'image/gif' => '.gif',
            'image/png' => '.png',
            'application/msword' => '.doc',
            'application/pdf' => '.pdf',
            'application/zip' => '.zip',
            'application/octet-stream' => '.csv',
            'text/plain' => '.txt',
            'text/css' => '.css',
            'text/html' => '.htm .html .xhtml',
            'text/x-javascript' => '.js',
            'text/xml' => '.xml'
            );


    // --- METHODS ---
    /**
    * Sends the request data to the proper method for processing.
    *
    * @static
    * @access public
    * @param string $page
    * @param string $action
    * @param array $request
    * @param array $files (optional)
    * @return void
    */
    public static function submitForm($page, $action, $request, $files = null) {
        return eval ('return PHPST_FormHandler::' . $action . '_' . $page . '($request, $files);');
    }

    /**
    * Changes the status of a department.
    */
    public static function update_browsedepartments($request) {
        if (isset($request['id']) && isset($request['status'])) {
            $department = PHPST_Department::getFromDB(array("id" => $request['id']));
            if ($department != null) {
                $department->setStatus($request['status']);
                $department->updateDB();
            } else {
                return "Error updating department";
            }
        }
    }


    /**
    * Processes form data for registering a User.
    *
    * @static
    * @access public
    * @param array $request
    * @return array Array with next page or array of errors with their fields
    */
    public static function register_createuser($request) {
        $msg = '';
        $error = array();
        $result = PHPST_User::validate($request);
        if ($result !== true) {
            return $result;
        } else {
            // Insert new PHPST_User
            $inserted_user = new PHPST_User($request);

            $result = $inserted_user->addToDB();

            // If option is on, send email to all admins and to new PHPST_User
            /*
            if (OPTION_EMAIL_USER_WHEN_REGISTERED == 1) {
                $users = PHPST_User::getArrayFromDB('Admin', 'Admin',
                        'admin', null, null, PHPST_MAX_RECORDS, 'all');
                $admins = array();

                if (is_array($users) && count($users) > 0) {
                    foreach ($users as $k => $user) {
                        if ($k != 'count' && $k != 'csv') {
                            // print_r($user);
                            $admins[] = $user['user']['email'];
                        }
                    }
                }

        				$userInfo = array();
        				$userInfo['email'] = $inserted_user->getEmail();
        				$userInfo['username'] = $inserted_user->getUsername();
        				$userInfo['password'] = $inserted_user->getPassword();

        				// notify new PHPST_User
                $result = PHPST_MailMan::newUserNotify($userInfo);

                if ($result !== true) {
                    // Returns array of errors with error fields and messages
                    return array(0 => array("field" => "Email",
                        "message" => $result));
                }

                // notify admins of new PHPST_User
                $result = PHPST_MailMan::newUserAdminNotify($admins, $userInfo);
                if ($result !== true) {
                    // Returns array of errors with error fields and messages
                    return array(0 => array("field" => "Email",
                        "message" => $result));
                }
            }
            */

            // Insert user ID in department_users table
            // A department_id of 0 means the user does not get added to a department
            if ($request['department_id'] > 0) {
                $department = PHPST_Department::getFromDB(array("id" => $request['department_id']));
                $department->addUser($inserted_user);
                $depresult = $department->updateDB();
            } else {
                $depresult = true;
            }
            if ($result === false || $depresult !== true) {
                $msg = PHPST_USER_INSERT_FAILURE;
            } elseif(is_null($result)) {
                $msg = PHPST_USER_ALREADY_EXISTS;
            } else {
                $msg = true;
            }
        }
        return $msg;
    }

    /**
    * Processes form data for editing a User's details.
    *
    * @static
    * @access public
    * @param array $request
    * @return string Success or Failure message
    */
    public static function edituser_viewuser($request) {
        // Perform validation on user entry
        $result = PHPST_User::validate($request);
        if ($result === true) {
            $user = new PHPST_User($request);
            $result = $user->updateDB();
            if ($result) {
                // If department_id is set, update department_users table
                if (isset($request['department_id'])) {
                    $department = PHPST_Department::getFromDB(array('id' => $request['department_id']));
                    if ($request['admin'] == 'Mod') {
                        $department->addUser($user);
                    } else {
                        $department->removeUser($request['id']);
                    }
                    $department->updateDB();
                }
                return true;
            } else {
                return false;
            }
        } else {
            return $result;
        }
    }

    /**
    * Processes form data for editing a department's details.
    *
    * @static
    * @access public
    * @param array $request
    * @return string Success or Failure message
    */
    public static function editdepartment_viewdepartment($request) {
        // Perform validation on user entry
        $result = PHPST_Department::validate($request);
        if ($result === true) {
            $department = PHPST_Department::getFromDB($request);
            $department->setName($request['name']);
            $department->setDescription($request['description']);
            $department->setStatus($request['status']);
            $result = $department->updateDB();
            if ($result) {
                return true;
            } else {
                return false;
            }
        } else {
            return $result;
        }
    }

    /**
    * Processes form data for verifying a User's login.
    *
    * @static
    * @access public
    * @param array $request
    * @return mixed Success or Failure message
    */
    public static function verify_login($request) {
        $user = PHPST_User::getFromDB($request);
        // print_r($request);
        if (is_object($user)) {
            return $user->getAdmin();
        } else {
            return false;
        }
    }

    /**
    * Processes form data for adding an Answer to a Ticket
    *
    * @static
    * @access public
    * @param array $request
    * @param array $files (optional)
    * @return mixed Success or Failure message
    */
    public static function addanswer_viewticket($request, $files = null) {

        $result = PHPST_Answer::validate($request);
        if ($result === true) {
            $answer = new PHPST_Answer($request);

            $user = PHPST_User::getFromDB(array('user_id' => $answer->getUser_id()));

            if ($answer->addToDB()) {
                // If file is being uploaded, send it to handling function
                if (isset($files) && is_array($files) && $files['userfile']['size'] > 0) {
                    $result = PHPST_FormHandler::handleFileUpload($files, $user, $answer);
                    if ($result !== true) {
                        // Returns array of errors with error fields and messages
                        return array(0 => array("field" => "File Upload",
                            "message" => $result));
                    }
                }

                if (OPTION_EMAIL_CLIENT_WHEN_NEW_ANSWER == 1) {
                    $recipients = array($user->getEmail());
                    $result = PHPST_MailMan::newAnswerNotify($recipients, $answer);
                    if ($result !== true) {
                        return $result;
                    }
                }
                return true;
            } else {
                return false;
            }
        } else {
            return $result;
        }
    }

    /**
    * Retrieves a list (array) of tickets from the DB.
    *
    * @static
    * @access public
    * @param string $admin_type Admin or Mod
    * @param string $search
    * @return array Tickets
    */
    public static function get_tickets($admin_type, $request, $department_id) {
        if (!isset($request['sort'])) {
            $request['sort'] = "timestamp";
            $request['order'] = 'DESC';
        }

        $tickets = PHPST_Ticket::getArrayFromDB($admin_type, @$request['search'],
                @$request['field'], $request['sort'], $request['order'], PHPST_MAX_RECORDS,
                @$request['1_cur_page'], $department_id, @$request['ticket_type']);
        return $tickets;
    }

    /**
    * Retrieves a single ticket Object from the DB.
    *
    * @static
    * @access public
    * @param string $admin_type Admin or Mod
    * @param string $search
    * @return array Tickets
    */
    public static function get_ticket($request) {

        $ticket = PHPST_Ticket::getFromDB($request);
        $tickets = array();
        // Update ticket status if requested
        if (isset($request['action'])) {
            if ($request['action'] == 'closeticket') {
                $ticket->setStatus('Closed');
            } elseif ($request['action'] == 'openticket') {
                $ticket->setStatus('Open');
            }
            $ticket->updateDB();
        }
        $id = null;
        if (isset($request['id'])) {
            $id = $request['id'];
        } else {
            $id = $request['ticket_id'];
        }

        $tickets = PHPST_Ticket::getArrayFromDB('Admin', $id, DB_PREFIX_TICKETS . '.id', null, null, 10, 1, null, null);

        if (is_array($tickets)) {
            next($tickets);
            $ticket = next($tickets);
        }
        return $ticket;
    }

    /**
    * Retrieves an array of options from the DB.
    *
    * @static
    * @access public
    * @return array Tickets
    */
    public static function get_options() {
        $options = array();
        $conn = &ADONewConnection(DSN);
        $query = "SELECT * FROM " . DB_PREFIX_OPTIONS;
        $rs = &$conn->Execute($query);
        if ($rs != false && $rs->RecordCount() > 0) {
            while (!$rs->EOF) {
                $options[] = $rs->fields;
                $rs->MoveNext();
            }
        }
        return $options;
    }

    /**
    * Updates options in the DB.
    *
    * @static
    * @access public
    * @param int $option_id
    * @param mixed $value
    * @return boolean result
    */
    public static function edit_options($request) {
        $conn = &ADONewConnection(DSN);
        foreach($request as $var => $value) {
            if (substr($var, 0, 6) == 'OPTION') {
                $query = "UPDATE " . DB_PREFIX_OPTIONS . " SET value = '$value' WHERE name = '$var'";
                $rs = &$conn->Execute($query);
            }
        }
    }

    /**
    * Retrieves a single user Object from the DB.
    *
    * @static
    * @access public
    * @param array $request
    * @return array User
    */
    public static function get_user($request) {
        $user = PHPST_User::getFromDB($request);

        // Update user status if requested
        if (isset($request['action'])) {
            if ($request['action'] == 'closeuser') {
                $user->setStatus('Closed');
            } elseif ($request['action'] == 'openuser') {
                $user->setStatus('Open');
            }
            $user->updateDB();
        }

        $id = null;
        if (isset($request['id'])) {
            $id = $request['id'];
        } else {
            $id = $request['user_id'];
        }
        $users = PHPST_User::getArrayFromDB('Admin', $id, DB_PREFIX_USER . '.' . DB_PREFIX_USER_ID, null, null, 10, 1, null);

        next($users);
        $user = next($users);
        return $user;
    }

    /**
    * Retrieves a list of users from the DB.
    *
    * @static
    * @access public
    * @param array $request
    * @return array Users
    */
    public static function get_users($request) {
        $users = PHPST_User::getArrayFromDB('Admin', @$request['search'],
                @$request['field'], @$request['sort'], @$request['order'], PHPST_MAX_RECORDS,
                @$request['1_cur_page']);
        return $users;
    }

    /**
    * Retrieves a single department Object from the DB.
    *
    * @static
    * @access public
    * @param array $request
    * @return array Department
    */
    public static function get_department($request) {
        $department = PHPST_Department::getFromDB($request);

        $id = null;
        if (isset($request['id'])) {
            $id = $request['id'];
        } else {
            $id = @$request['department_id'];
        }
        $departments = PHPST_Department::getArrayFromDB('Admin', $id, DB_PREFIX_DEPARTMENTS . '.id', null, null, 10, 1, null);
        // die($departments['count']);

        // The first index is the count, the second is the csv, so move it twice
        if (is_array($departments)) {
            next($departments);
            $department = next($departments);
        }
        return $department;
    }

    /**
    * Deletes a Department from the DB
    *
    * @static
    * @access public
    * @param array $request
    * @return boolean True if successful
    */
    public static function delete_department($request) {
        $department = PHPST_Department::getFromDB($request);
        return $department->removeFromDB($department->getId());
    }

    /**
    * Retrieves a list of departments from the DB.
    *
    * @static
    * @access public
    * @param array $request
    * @return array Departments
    */
    public static function get_departments($request) {
        $departments = PHPST_Department::getArrayFromDB('Admin', @$request['search'],
                @$request['field'], @$request['sort'], @$request['order'], PHPST_MAX_RECORDS,
                @$request['1_cur_page']);
        return $departments;
    }

    /**
    * Adds a new PHPST_Ticket to the DB.
    *
    * @static
    * @access public
    * @param array $request
    * @param array $files Optional upload file
    * @return mixed Inserted ticket's id if successful, false otherwise, or error message in an array
    */
    public static function newticket_newticket($request, $files = null) {
        $result = PHPST_Ticket::validate($request);

        if ($result === true) {

            $ticket = new PHPST_Ticket($request);
            $user = PHPST_User::getFromDB(array('user_id' => $request['user_id']));
            $id = $ticket->addToDB();

            if ($id !== false && $id !== null) {

                // If file is being uploaded, send it to handling function
                if (isset($files) && is_array($files) && $files['userfile']['size'] > 0) {
                    $result = PHPST_FormHandler::handleFileUpload($files, $user, $ticket);
                    if ($result !== true && $result != PHPST_UPLOAD_NO_ATTACHMENT) {
                        // Returns array of errors with error fields and messages
                        return array(0 => array("field" => "File Upload",
                            "message" => $result));
                    }
                }

                // If option is on, send email to all mods in that department
                if (OPTION_EMAIL_MODS_WHEN_NEW_TICKET == 1) {

                    $users = PHPST_User::getArrayFromDB('Mod', $request['department_id'],
                            DB_PREFIX_DEPARTMENTS . '.id', null, null, PHPST_MAX_RECORDS, 'all');

                    $admins = PHPST_User::getArrayFromDB('Admin', '1',
                            DB_PREFIX_USER . '.' . DB_PREFIX_USER_ADMIN, null, null, PHPST_MAX_RECORDS, 'all');
                    if (!empty($admins)) {
                        $users = array_merge($users, $admins);
                    }

                    if (is_array($users) && count($users) > 0) {
                        $recipients = array();
                        foreach ($users as $key => $user) {
                            if ($key != 'count' && $key != 'csv')
                            $recipients[] = $user['user']['email'];
                        }

                        $result = PHPST_MailMan::newTicketNotify($recipients, $ticket);

                        if ($result !== true) {
                            // Returns array of errors with error fields and messages
                            return array(0 => array("field" => "Email",
                                "message" => $result));
                        }
                    }
                }
                return $id;
            } else {
                // Ticket fails to be added because it has already been submitted
                return false;
            }
        } else {
            return $result;
        }
    }

    /**
    * Adds a new PHPST_Department to the DB.
    *
    * @static
    * @access public
    * @param array $request
    * @return mixed Inserted department's id if successful or error message in an array
    */
    public static function newdepartment_createdepartment($request) {
        $result = PHPST_Department::validate($request);
        if ($result === true) {
            $department = new PHPST_Department($request);
            $id = $department->addToDB();
            if ($id !== false && $id !== null) {
                return $id;
            } else {
                return array(0 => array('field' => 'name', 'message' => PHPST_DEPARTMENT_EXISTS));
            }
        } else {
            return $result;
        }
    }

    /**
    * Change tickets' status in the array to the value chosen (Open or closed).
    *
    * @static
    * @access public
    * @param array $request
    * @return boolean
    */
    public static function changetickets_browsetickets($request) {
        if (isset($request['ticket'])) {
            foreach($request['ticket'] as $ticket_id) {
                $ticket = PHPST_Ticket::getFromDB(array("id" => $ticket_id));

                $ticket->setStatus($request['status']);
                $ticket->updateDB();
            }
        }
    }

    public static function verify_home() {}

    /**
    * When a file is uploaded with a ticket or answer, this
    * function is used to verify the file, create the appropriate
    * folder structure and save the file.
    *
    * @static
    * @access public
    * @param array $files uploaded files
    * @param object $user
    * @param object $object
    */
    public static function handleFileUpload($files, $user, $object) {
        $type = strtolower(get_class($object));

        if ($files['userfile']['error'] == '4') {
            $msg = PHPST_UPLOAD_NO_ATTACHMENT;
        } elseif ($files['userfile']['error'] == '2') {
            $msg = PHPST_UPLOAD_TOO_BIG_FOR_TOOL;
        } elseif ($files['userfile']['error'] == '1') {
            $msg = PHPST_UPLOAD_TOO_BIG_FOR_PHP;
        } elseif ($files['userfile']['error'] == '3') {
            $msg = PHPST_UPLOAD_PARTIAL;
        }

        // CHECK TO MAKE SURE THE UPLOADED FILE IS OF A FILE WE ALLOW AND GET THE NEWFILE EXTENSION
        elseif (!in_array($files['userfile']['type'], PHPST_FormHandler::$allowedtypes)) {
            $msg = PHPST_UPLOAD_FILETYPE . $files['userfile']['type'] . PHPST_UPLOAD_NOT_ALLOWED;

            while ($type = current(PHPST_FormHandler::$allowedtypes)) {
                $msg .= '<br />' . PHPST_FormHandler::$filetypes[$type] . ' (' . $type . ')';
                next(PHPST_FormHandler::$allowedtypes);
            }
        }

        // IF FILE IS NOT OVER SIZE AND IS CORRECT TYPE THEN CONTINUE WITH PROCESS
        elseif ($files['userfile']['error'] == '0') {
            $newfilename = strtolower($files['userfile']['name']);
            // PRINT OUT THE RESULTS
            $msg = '<p><b>' . PHPST_UPLOAD_ATTACHMENT_SUCCESS . '</b> - ' . PHPST_UPLOAD_SUBMITTED . $files['userfile']['name'] . '
    				' . PHPST_UPLOAD_SIZE .  $files['userfile']['size'] . ' bytes -
    				' . PHPST_UPLOAD_TYPE . $files['userfile']['type'];

            // Check whether this user already has a folder in his/her name
            $user_folder = PHPST_UPLOAD_PATH . $user->getUsername();

//rmdirRecursive( $user_folder );
            
            if (!is_dir($user_folder)) {
                // Create the directory
                //mkdir($user_folder, 0700);
                mkdir($user_folder, 0777);
            }

            // Create the folder for this file (e.g. /uploads/username/ticket_43)
            $object_folder = $user_folder . '/' . $type . '_' . $object->getID();
            //mkdir($object_folder, 0700);
            mkdir($object_folder, 0777);

            $result = move_uploaded_file($files['userfile']['tmp_name'], $object_folder . '/' . $newfilename);

            if ($result === true) {
                return $result;
            } else {
                return $result;
            }
        }
        return $msg;
    }
}

/*
function rmdirRecursive($path,$followLinks=false) {
	$dir = opendir($path) ;
	while ( $entry = readdir($dir) ) {
	
		if ( is_file( "$path/$entry" ) ) {// || ((!$followLinks) && is_link("$path/$entry")) ) {
			unlink( "$path/$entry" );
			//echo ( "unlink $path/$entry;\n" );
			// Uncomment when happy!
			//unlink( "$path/$entry" );
		} elseif ( is_dir( "$path/$entry" ) && $entry!='.' && $entry!='..' ) {
			rmdirRecursive( "$path/$entry" ) ;
		}
	}
	closedir($dir) ;
	rmdir( $path );
	//echo "rmdir $path;\n";
	// Uncomment when happy!
	// return rmdir($path);
}
*/


?>