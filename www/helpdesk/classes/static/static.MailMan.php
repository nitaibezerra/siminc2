<?php
 /**
 * File containing the MailMan class.
 * This class depends on the Mail PEAR package, usually part of the
 * basic installation of PHP. There may be a need to configure php.ini so that
 * it knows where to find the PEAR packages.
 *
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version SVN: $Id: static.MailMan.php 4 2005-12-13 01:47:15Z nicolas $
 * @since File available since Release 1.1.1.1
 * \\||
 */

/**
* user defined includes.
* @include
*/
require_once_check(PHPST_PATH . 'classes/class.phpmailer.php');
require_once_check(PHPST_PATH . 'classes/static/static.EmailTemplateParser.php');

/**
* Constants
* @ignore
*/
define ('PHPST_EMAIL_TEMPLATE_LOCATION', PHPST_PATH .  'email/');
define ('PHPST_EMAIL_TEMPLATE_NEW_TICKET', PHPST_EMAIL_TEMPLATE_LOCATION . 'new_ticket.xml');
define ('PHPST_EMAIL_TEMPLATE_NEW_ANSWER', PHPST_EMAIL_TEMPLATE_LOCATION . 'new_answer.xml');
define ('PHPST_EMAIL_TEMPLATE_NEW_USER', PHPST_EMAIL_TEMPLATE_LOCATION . 'new_user.xml');
define ('PHPST_EMAIL_TEMPLATE_NEW_USER_ADMIN', PHPST_EMAIL_TEMPLATE_LOCATION . 'new_user_admin.xml');
define ('PHPST_EMAIL_TEMPLATE_TICKET_STATUS', PHPST_EMAIL_TEMPLATE_LOCATION . 'ticket_status.xml');


/**
 * This static function class serves the purpose of gathering addresses and
 * creating bodies for the different emails to be sent by the host application.
 * Hence its methods will vary according to the needs of the application.
 *
 * @access public
 * @package PHPSupportTicket
 */
class PHPST_MailMan{
    /**
    * Reply address.
    *
    * @static
    * @access public
    * @var string $reply
    */
    public static $reply;

    /**
    * Sends mail to all users in a department, notifying them of a new ticket.
    * If no mods exist in this department, all Admins are notified. If no Admins
    * exist (you're in trouble!), notify the user that his request cannot be answered for lack of staff!
    *
    * @static
    * @access public
    * @param array $recipients an array of email addresses
    * @param object $ticket
    * @return boolean true if successful
    */
    public static function newTicketNotify($recipients, $ticket) {
        $template = PHPST_EmailTemplateParser::getEmailTemplate(PHPST_EMAIL_TEMPLATE_NEW_TICKET);
		$user = PHPST_User::getFromDB(array("user_id" => $ticket->getUser_id()));
		$fields = $template->fields;
		$subject = $template->subject;
		$body = $template->body;

        // Parse and replaced fields in the email's subject and body
		foreach ($fields->children() as $field) {
			$find = '[' . trim($field['name']) . ']';
			$replace = eval('return $' . trim($field['object']) . '->' . trim($field['method']) . ';');
			$body = str_replace($find, $replace, $body);
			$subject = str_replace($find, $replace, $subject);
		}

		try {
            return PHPST_MailMan::sendMail($recipients, $subject, $body);
        } catch (Exception $e) {
            print $e->getMessage();
        }
    }

    /**
    * Sends mail to a ticket's owner, notifying him/her of a new answer.
    *
    * @static
    * @access public
    * @param array $recipients an array of email addresses
    * @param object $answer
    * @return boolean true if successful
    */
    public static function newAnswerNotify($recipients, $answer) {
        $template = PHPST_EmailTemplateParser::getEmailTemplate(PHPST_EMAIL_TEMPLATE_NEW_ANSWER);
        $user = PHPST_User::getFromDB(array('user_id' => $answer->getUser_id()));
        $fields = $template->fields;
    		$subject = $template->subject;
    		$body = $template->body;

    		$ticket = PHPST_Ticket::getFromDB(array('id' => $answer->getTicket_id()));

    		// Parse and replace fields in the email's subject and body
    		foreach ($fields->children() as $field) {
    		    $find = '[' . trim($field['name']) . ']';
      			$object = trim($field['object']);
      			$method = trim($field['method']);
    		    $replace = eval('return $' . trim($field['object']) . '->' . $method . ';');
    		    $body = str_replace($find, $replace, $body);
    		    $subject = str_replace($find, $replace, $subject);
    		}

        try {
            return PHPST_MailMan::sendMail($recipients, $subject, $body);
        } catch (Exception $e) {
            print $e->getMessage();
        }
    }

    /**
    * Sends mail to a new user is also sent an email confirming his/her registration.
    *
    * @static
    * @access public
    * @param object $user
    * @return boolean true if successful
    */
    public static function newUserNotify($user) {
        $template = PHPST_EmailTemplateParser::getEmailTemplate(PHPST_EMAIL_TEMPLATE_NEW_USER);
    		$fields = $template->fields;
    		$subject = $template->subject;
    		$body = $template->body;

    		// Parse and replaced fields in the email's subject and body
    		foreach ($fields->children() as $field) {
      			$find = '[' . trim($field['name']) . ']';
      			$replace = eval('return $user["' . trim($field['name']) . '"];');
      			$body = str_replace($find, $replace, $body);
      			$subject = str_replace($find, $replace, $subject);
		    }

        try {
            return PHPST_MailMan::sendMail($user['email'], $subject, $body);
        } catch (Exception $e) {
            print $e->getMessage();
        }
    }

    /**
    * Sends mail to the Admins, notifying them that a new User has registered.
    *
    * @static
    * @access public
    * @param object $recipients
    * @return boolean true if successful
    */
    public static function newUserAdminNotify($recipients, $user) {
        $template = PHPST_EmailTemplateParser::getEmailTemplate(PHPST_EMAIL_TEMPLATE_NEW_USER_ADMIN);
		$fields = $template->fields;
		$subject = $template->subject;
		$body = $template->body;

		// Parse and replaced fields in the email's subject and body
		foreach ($fields->children() as $field) {
			$find = '[' . trim($field['name']) . ']';
			$replace = eval('return $user["' . trim($field['name']) . '"];');
			$body = str_replace($find, $replace, $body);
			$subject = str_replace($find, $replace, $subject);
		}

		try {
            return PHPST_MailMan::sendMail($recipients, $subject, $body);
        } catch (Exception $e) {
            print $e->getMessage();
        }
    }

    /**
    * Sends mail to the ticket's owner when a ticket's status is changed by someone else.
    *
    * @static
    * @access public
    * @param object $ticket
    * @return boolean true if successful
    */
    public static function ticketStatusChangeNotify($recipients, $ticket, $user) {
        $template = PHPST_EmailTemplateParser::getEmailTemplate(PHPST_EMAIL_TEMPLATE_TICKET_STATUS);

        // Parse and replaced fields in the email's body
        $template->body = ereg_replace('\[ticket_subject\]', $ticket->getSubject(), $template->body);
        $template->body = ereg_replace('\[ticket_body\]', $ticket->getBody(), $template->body);

        $subject = 'New Answer:: ' . $ticket->getSubject();

        $body = $template->body;

        try {
            return PHPST_MailMan::sendMail($recipients, $subject, $body);
        } catch (Exception $e) {
            print $e->getMessage();
        }
    }

    /**
    * Sends the prepared email.
    *
    * @access public
    * @static
    * @param mixed $recipients
    * @param string $subject
    * @param string $body
    *
    * @return boolean success or failure
    */
    public static function sendMail($recipients, $subject, $body) {
        $mail_object = new PHPST_PHPMailer();
        $mail_object->isHTML(false);

        switch (PHPST_MAIL_SENDMETHOD) {
            case 'sendmail' :
                $mail_object->IsSendmail();
                break;
            case 'smtp' :
                $mail_object->IsSMTP();
                break;
            case 'mail' :
                $mail_object->IsMail();
                break;
            case 'qmail' :
                $mail_object->IsQmail();
                break;
        }

        $mail_object->Host = PHPST_MAIL_SOCKETHOST;

        if (PHPST_MAIL_SMTPAUTH) {
            $mail_object->SMTPAuth = true;
            $mail_object->Username = PHPST_MAIL_SMTPAUTHUSER;
            $mail_object->Password = PHPST_MAIL_SMTPAUTHPASS;
        }

        if (PHPST_MAIL_SENDMETHOD == 'smtp') {
            $mail_object->From = PHPST_MAIL_SOCKETFROM;
            $mail_object->FromName = PHPST_MAIL_SOCKETFROMNAME;
            $mail_object->AddReplyTo(PHPST_MAIL_SOCKETREPLY, PHPST_MAIL_SOCKETREPLYNAME);
        } else {
            $mail_object->From = PHPST_MAIL_TO;
            $mail_object->FromName = PHPST_MAIL_NAME;
            $mail_object->AddReplyTo(PHPST_MAIL_TO, PHPST_MAIL_NAME);
        }
        if (is_array($recipients)) {
            foreach ($recipients as $to) {
                $mail_object->addAddress($to);
            }
        } else {
            throw new Exception("The recipients array was empty, no mail sent.");
        }

        $mail_object->Body = $body;
        $mail_object->Subject = $subject;
        $mail_object->From = PHPST_MailMan::$reply;
        $mail_object->Sender = PHPST_MailMan::$reply;

        $result = $mail_object->send();
        if ($result !== true) {
            return PHPST_EMAIL_SEND_FAILURE;
        } else {
            return true;
        }
    }
}
?>