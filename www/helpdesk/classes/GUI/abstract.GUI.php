<?php
/**
 * File containing the abstract class GUI.
 *
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * $Id: abstract.GUI.php 7 2005-12-13 03:36:51Z nicolas $
 * @since File available since Release 1.1.1.1
 * \\||
 */

/**
* user defined includes.
* @include
*/
require_once_check(PHPST_PATH . 'classes/static/static.FormHandler.php');
define('PHPST_MAX_RECORDS', 10);

/**
 * GUI is an abstraction of a User Interface.
 *
 * @abstract
 * @access public
 * @package PHPSupportTicket
 */
abstract class PHPST_GUI {
    /// --- FIELDS ---

    /**
    * The User object using this GUI.
    * @access public
    * @var object $user
    */
    public $user;

    /**
    * The absolute URL for this GUI.
    * @access private
    * @var string $page
    */
    private $absoluteURL;

    /**
    * The current page for this GUI.
    * @access private
    * @var string $page
    */
    private $page;

    /**
    * A message about the last action performed, usually success or failure.
    *
    * @access private
    * @var string $msg
    */
    private $msg;

    /**
    * The XHTML code designed to be output to the browser.
    *
    * @access private
    * @var string $html
    */
    private $html;

    // --- METHODS ---

    /**
    * Constructor.
    *
    * @access public
    * @param string $absoluteURL
    * @param array $request
    */
    public function __construct($absoluteURL, $request) {
        $this->setAbsoluteURL($absoluteURL);

        // Do not retrieve user if logout or new page
        if (!isset($request['logout']) && count($request) > 1) {
            $this->user = PHPST_User::getFromDB($request);
            $this->user->setTimestamp(time());
            $this->user->updateDB();
        }
    }

    /**
    * Used to output the HTML code to the browser.
    *
    * @access public
    * @return void
    */
    public function output() {
        print $this->getHTML();
    }

    /**
     * GUI::addHTML()
     *
     * @access public
     * @param string $string
     * @return
     **/
    public function addHTML($string) {
        $this->html .= $string;
    }

    /**
     * GUI::getHTML()
     *
     * @access public
     * @return string
     **/
    public function getHTML() {
        return $this->html;
    }

    /**
     * Returns this GUI's absolute URL
     *
     * @access public
     * @param $absoluteURL
     * @return string
     **/
    public function getAbsoluteURL() {
        return $this->absoluteURL;
    }

    /**
     * Sets this GUI's absolute URL
     *
     * @access public
     * @param $absoluteURL
     * @return void
     **/
    public function setAbsoluteURL($absoluteURL) {
        $this->absoluteURL = $absoluteURL;
    }

    /**
     * Adds the header to this GUI's HTML.
     *
     * @param string $title
     * @param string $base
     * @return void
     **/
    public function buildHeader($title = '') {
        $base = $this->getAbsoluteURL();
        $title = PHPST_PHPST . ' :: ' . $title;
        $string = '
            <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

            <html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
            <head>
            <title>' . $title . '</title>
            <base href="' . $base . '" />
            <meta http-equiv="content-type" content="text/xml; charset=iso-8859-1" />

            <meta name="description" content="" />
            <meta name="keywords"    content="" />
            <meta name="author"      content="Ian Warner" />
            <meta name="copyright"   content="Triangle Solutions Ltd" />
            <meta name="rating"      content="General" />
            <meta name="robots"      content="All" />

            <link rel="stylesheet"    href="css/style.css" type="text/css" />
            <link rel="stylesheet"    href="css/style_global.css" type="text/css" />
            <link rel="shortcut icon" href="favicon.ico" />
            <script language="javascript" type="text/javascript" src="include/admin.js">
            </script>
            <script language="javascript" type="text/javascript">
            window.onload = function() {
                var tables = document.getElementsByTagName("table");
                if (in_array("table", tables, "id")) {
                    ConvertRowsToLinks("table");
                }
                if (in_array("table2", tables, "id")) {
                    ConvertRowsToLinks("table2");
                }
            }
        	</script>
            </head>
            <body>';
        $this->addHTML($string);
    }

   /**
    * Builds the viewticket page.
    *
    * @access public
    * @return string
    * @param array $request
    * @param object $ticket
    */
    public function buildViewticket($request, $ticket) {
        $url = $this-> getAbsoluteUrl() ;
        $field = '';
        if (is_array($this->getMsg())) {
            $msg = $this->getMsg();
            $error = $msg[0]['field'];
            $this->setMsg($msg[0]['message']);
        }
        $admin_type = $this->user->getAdmin();

        // Get attachment info about this ticket. False if no attachment
        $ticket_attachment_info = PHPST_Ticket::getAttachmentInfo($ticket['user']['username'], $ticket['ticket']['id']);

        // If there are no answers for this ticket, just create an empty array
        if (!isset($ticket['answers'])) {
            $ticket['answers'] = array();
        }
        // print_r($tickets);
        $string = $this->buildNavigation($request);
        $string .= '
            <div class="gap"></div>
              <table class="tbl">
                <tr>
                  <td class="title">' . PHPST_DETAILS . '</td>';
            $string .= $this->buildSearchBox(DB_PREFIX_TICKETS . '.subject', 'browsetickets');
        if ($ticket['ticket']['status'] == 'Open') {
            $string .= '<td width="80"><a href="' . $url . 'page=viewticket&amp;action=closeticket&amp;id=' . $ticket['ticket']['id'] . '">' . PHPST_CLOSETICKET . '</a></td>';
        } else {
            $string .= '<td width="80"><a href="' . $url . 'page=viewticket&amp;action=openticket&amp;id=' . $ticket['ticket']['id'] . '">' . PHPST_REOPENTICKET . '</a></td>';
        }
        $string .=   '<td width="70">';
        $help = 'This table shows you the Ticket&rsquo;s details and its dialog.';

        $string .= PHPST_IconGUI::getIcon('help', '16', 'images/icons/', 0, 0, 'style="cursor:pointer"'
            . 'onmouseover="return escape(\'' . $help . '\')"');
        $string .= PHPST_IconGUI::getIcon('arrow-down', '16', 'images/icons/', 0, 0, 'onClick="blocking(\'showTicket\', \'block\'); return false;" style="cursor:pointer"');
        
        $string .= '
                  </td>
                </tr>
              </table>
          <div id="showTicket">
          <table cellpadding="0" cellspacing="0" width="100%">';
        if (strlen($this->getMsg()) > 5) {
            $string .= '<tr><td colspan="2">' . $this->getMsg() . '</td></tr>';
        }

        $string .= '<tr>
              <td valign="top">
              <form enctype="multipart/form-data" action="' . $url . '" method="post">
                <input type="hidden" name="formdata" value="addanswer" />
                <input type="hidden" name="page" value="viewticket" />
                <input type="hidden" name="field" value="' . DB_PREFIX_TICKETS . '".id" />
                <input type="hidden" name="search" value="' . $ticket['ticket']['id'] . '" />
                <input type="hidden" name="user_id" value="' . $this->user->getId() . '" />
                <input type="hidden" name="ticket_id" value="' . $ticket['ticket']['id'] . '" />
                <input type="hidden" name="subject" value="' . $ticket['ticket']['subject'] . '" />
                <input type="hidden" name="timestamp" value="' . time() . '" />
                <input type="hidden" name="rating" value="1" />

              <table class="tbl" style="margin-right:5px">
                <tr>
                  <th>' . PHPST_NAME . '</th>';
        if ($admin_type == 'Admin') {
            $string .= "<td>
                    <a href=\"" . $url . "page=viewuser&amp;field=" . DB_PREFIX_USER . "." . DB_PREFIX_USER_ID . "&amp;search={$ticket['user']['id']}&amp;id={$ticket['user']['id']}\">
                    " . $ticket['user']['name'] . '</a></td>';
        } else {
            $string .= '<td>' . $ticket['department']['name'] . '</td>';
        }
        $string .= '
                    </tr>
                    <tr>
                      <th>' . PHPST_EMAIL . '</th>
                      <td><a href="mailto:' . $ticket['user']['email'] . '">' . $ticket['user']['email'] . '</a></td>
                    </tr>
                    <tr>
                      <th>' . PHPST_SUBJECT . '</th>
                      <td>' . $ticket['ticket']['subject'] . '</td>
                    </tr>
                    <tr>
                      <th>' . PHPST_DEPARTMENT . '</th>';
        if ($admin_type == 'Admin') {
            $string .= "<td>
                    <a href=\"" . $url . "page=viewdepartment&amp;field=" . DB_PREFIX_TICKETS . ".id&amp;search={$ticket['department']['id']}&amp;id={$ticket['department']['id']}\">
                    " . $ticket['department']['name'] . '</a></td>';
        } else {
            $string .= '<td>' . $ticket['department']['name'] . '</td>';
        }
        $string .= '
                    </tr>
                    <tr>
                      <th>' . PHPST_URGENCY . '</th>
                      <td style="background-color:#' . eval('return PHPST_URGENCY_COLOUR_' . $ticket['ticket']['urgency'] . ';') . '">
                          ' . eval('return PHPST_URGENCY_LABEL_' . $ticket['ticket']['urgency'] . ';') . '
                      </td>
                    </tr>
                    <tr>
                      <th>' . PHPST_STATUS . '</th>
                      <td>' . $ticket['ticket']['status'] . '</td>
                    </tr>';
            if ($ticket['ticket']['status'] == 'Open') {
                $string .='
                    <tr>
                      <th>' . PHPST_RESPOND . '</th>
                      <td><textarea name="body" cols="30" rows="7"></textarea></td>
                    </tr>
                    <tr>
                      <th>' . PHPST_ATTACHMENT . '</th>
                      <td>
                        <input type="hidden" name="MAX_FILE_SIZE" value="2000000" />
                        <input type="file"   name="userfile" size="30" />
                      </td>
                    </tr>
                    <tr>
                      <td colspan="2">
                        <input type="submit" value="' . PHPST_SUBMIT .'" />
                      </td>
                    </tr>
                  ';
            } // end if

            $string .= '
                  </table>
                </form>
                </td>
                <td width="500" valign="top">
                  <table class="tbl">
                    <tr>
                      <td class="title">' . PHPST_DIALOG_QUESTION . '</td>
                      <td width="70">';

            $help = 'This shows you the question initially asked when the Ticket was created.';

            $string .= PHPST_IconGUI::getIcon('help', '16', 'images/icons/', 0, 0, 'style="cursor:pointer"'
                . 'onmouseover="return escape(\'' . $help . '\')"');
            $string .= PHPST_IconGUI::getIcon('arrow-down', '16', 'images/icons/', 0, 0, 'onClick="blocking(\'showQuestion\', \'block\'); return false;" style="cursor:pointer"');
            $string .= '
                  </td>
                    </tr>
                  </table>
                  <div id="showQuestion">
                  <table class="tbl">
                    <tr>
                      <th width="100">' . PHPST_DATE . '</th>
                      <td>' . gmdate('d/m/Y H:i:s', $ticket['ticket']['timestamp']) . '</td>
                    </tr>
                    <tr>
                      <th>' . PHPST_QUESTION . '</th>
                      <td>' . $ticket['ticket']['body'] . '</td>
                    </tr>';
            if ($ticket_attachment_info) {
                $string .= '
                        <tr>
                          <th>' . PHPST_POSTEDBY . ': ' . $ticket['user']['name'] . '</th>
                          <td>' . @$ticket_attachment_info['filename'] . $ticket_attachment_info['image']
                            . ' [' . $ticket_attachment_info['size'] . ']</td>
                        </tr>';
            }
            $string .= '
                  </table>
                </div>

                <div class="gap"></div>
                  <table class="tbl">
                    <tr>
                      <td class="title">' . PHPST_RESPONSE . 's</td>
                      <td width="70">';
            
            $help = 'This shows you the answers to this Ticket&rsquo;s question.';

            $string .= PHPST_IconGUI::getIcon('help', '16', 'images/icons/', 0, 0, 'style="cursor:pointer"'
                . 'onmouseover="return escape(\'' . $help . '\')"');
            $string .= PHPST_IconGUI::getIcon('arrow-down', '16', 'images/icons/', 0, 0, 'onClick="blocking(\'showResponses\', \'block\'); return false;" style="cursor:pointer"');
            
            $string .= '
                      </td>
                    </tr>
                  </table>
                <div id="showResponses">';

            foreach ($ticket['answers'] as $id => $answer) {
                $phpst_user = PHPST_User::getFromDB(array('user_id' => $answer['user_id']));

                // Get attachment info about this answer, false if none exist.
                unset($answer_attachment_info);
                $answer_attachment_info = PHPST_Answer::getAttachmentInfo($phpst_user->getUsername(), $answer['id']);

                $string .= '
                      <table class="tbl">
                        <tr bgcolor="' . UseColor() . '">
                          <th width="100">' . PHPST_DATE . '</th>
                          <td>' . gmdate('d/m/Y H:i:s', $answer['timestamp']) . '</td>
                        </tr>
                        <tr bgcolor="' . UseColor(true) . '">
                          <th>' . PHPST_RESPONSE . '</th>
                          <td>' . $answer['body'] . '</td>
                        </tr>';
                if ($answer_attachment_info) {
                    $string .= '
                        <tr bgcolor="' . UseColor(true) . '">
                          <th>' . PHPST_ATTACHMENT . '</th>
                          <td>' . @$answer_attachment_info['filename'] . ' &nbsp;' .
                                @$answer_attachment_info['image'] . ' &nbsp;
                                [' . $answer_attachment_info['size'] . ']
                          </td>
                        </tr>';
                }
                $string .= '<tr bgcolor="' . UseColor(true) . '">
                          <th>' . PHPST_POSTEDBY . '</th>
                          <td>
                            <a href="mailto:' . $phpst_user->getEmail() . '">' . $phpst_user->getName() . '</a>
                          </td>
                        </tr>
                      </table>
                    <div class="gap"></div> ';
            } // end foreach

            $string .= '
                  </div>
                </td>
              </tr>
            </table>
          </div>
          </div>';
        return $string;
    }

    /**
    * Builds the newticket page .
    *
    * @access public
    * @return string
    * @param array $request
    */
    public function buildNewticket() {
        $url = $this-> getAbsoluteUrl() ;
        $string = $this->buildNavigation(null);
        if (is_array($this->getMsg())) {
            $msg = $this->buildErrorList();
            $this->setMsg(PHPST_TICKET_INSERT_FAILURE . $msg);
        } elseif (is_int($this->getMsg())) {
            $int = $this->getMsg();
            $this->setMsg('');
            $ticket = PHPST_ticket::getFromDB(array('id' => $int));
            return $this->buildViewticket(array('id' => $int), $ticket);
        } else {
            $this->setMsg(null);
        }

        if (is_array($this->getMsg())) {
            $msg = $this->buildErrorList();
            $this->setMsg($msg);
        } elseif ($this->getMsg() === true) {
            // If the message equals true, it means the user was successfully added
            $this->setMsg(PHPST_TICKET_INSERT_SUCCESS);
            $string .= '
                  <div class="gap"></div>
                    <table class="tbl">
                      <tr><td colspan="2">' . $this->getMsg() . '</td></tr>
                    </table>';
            return $string;
        }

        $string .= '
            <script type="text/javascript" language="javascript">
            <!--
            urgency_colours = new Array(4);
            urgency_colours[0] = "#' . PHPST_URGENCY_COLOUR_1 . '";
            urgency_colours[1] = "#' . PHPST_URGENCY_COLOUR_2 . '";
            urgency_colours[2] = "#' . PHPST_URGENCY_COLOUR_3 . '";
            urgency_colours[3] = "#' . PHPST_URGENCY_COLOUR_4 . '";

            function updateUrgency(value) {
                document.getElementById("urgency").style.backgroundColor = urgency_colours[value - 1];
            }

            // -->
            </script>
            <form enctype="multipart/form-data" action="' . $url . '" method="post">
                <input type="hidden" name="formdata" value="newticket" />
                <input type="hidden" name="page" value="newticket" />
                <input type="hidden" name="user_id" value="' . $this->user->getId() . '" />
                <input type="hidden" name="timestamp" value="' . time() . '" />
                <input type="hidden" name="status" value="Open" />
            <div class="gap"></div>
            <table class="tbl">
              <tr>
                <td class="title">' . PHPST_NEW . '</td>
                <td width="70">';

            $help = "Use this form to create a new Support Ticket. "
                  . "Please read the instructions carefully.";
            $string .= PHPST_IconGUI::getIcon('help', '16', 'images/icons/', 0, 0, 'style="cursor:pointer"'
                . 'onmouseover="return escape(\'' . $help . '\')"');
            $string .= PHPST_IconGUI::getIcon('arrow-down', '16', 'images/icons/', 0, 0, 'onClick="blocking(\'showNewTicket\', \'block\'); return false;" style="cursor:pointer"');
            $string .= '
                  </td>
                </tr>
              </table>
              <div id="showNewTicket">
              <table cellpadding="0" cellspacing="0" width="100%">';
            if (strlen($this->getMsg()) > 0) {
                $string .= '<tr><td colspan="2">' . $this->getMsg() . '</td></tr>';
            }
            $string .='
              <tr>
                  <td valign="top">
                      <table class="tbl" style="margin-right:5px">
                        <tr>
                          <th><b>' . PHPST_NAME . '</b></th>
                          <td>
                            ' . $this->user->getName() . '
                          </td>
                        </tr>
                        <tr>
                          <th><b>' . PHPST_EMAIL . '</b></th>
                          <td>
                            ' . $this->user->getEmail() . '
                          </td>
                        </tr>
                        <tr>
                          <th><b>' . PHPST_SUBJECT . '</b></th>
                          <td>
                            <input type="text" name="subject" value="' . @$request['subject'] . '" />
                          </td>
                        </tr>
                        <tr>
                          <th><b>' . PHPST_DEPARTMENT . '</b></th>
                          <td>' . $this->getDepartmentsDropDown() . '</td>
                        </tr>
                        <tr>
                          <th><b>' . PHPST_URGENCY . '</b></th>
                          <td>
                            <select onchange="updateUrgency(this.value);" id="urgency" style="background-color:#' . PHPST_URGENCY_COLOUR_1 . '" name="urgency">
                                <option style="background-color:#' . PHPST_URGENCY_COLOUR_1 . '" selected="selected" value="1">' . PHPST_URGENCY_LABEL_1 . '</option>
                                <option style="background-color:#' . PHPST_URGENCY_COLOUR_2 . '" value="2">' . PHPST_URGENCY_LABEL_2 . '</option>
                                <option style="background-color:#' . PHPST_URGENCY_COLOUR_3 . '" value="3">' . PHPST_URGENCY_LABEL_3 . '</option>
                                <option style="background-color:#' . PHPST_URGENCY_COLOUR_4 . '" value="4">' . PHPST_URGENCY_LABEL_4 . '</option>
                            </select>
                          </td>
                        </tr>
                      </table>

                      <div class="gap"></div>

                      <table class="tbl" style="margin-right:5px">
                        <tr>
                          <th><b>' . PHPST_QUESTION . '</b></th>
                        </tr>
                        <tr>
                          <td><textarea name="body" cols="50" rows="10"></textarea></td>
                        </tr>
                        <tr>
                          <td align="right">
                            <input type="submit" value="' . PHPST_SUBMIT .'" />
                          </td>
                        </tr>
                      </table>

                      <div class="gap"></div>

                      <table class="tbl" style="margin-right:5px">
                        <tr>
                          <th><b>' . PHPST_ATTACHMENT . '</b></th>
                        </tr>
                        <tr>
                          <td>
                            <input type="hidden" name="MAX_FILE_SIZE" value="10240000" />
                            <input type="file"   name="userfile" size="54" />
                          </td>
                        </tr>
                      </table>

                  </td>
                  <td valign="top">
                    <table class="tbl">
                      <tr>
                        <td>
                          ' . PHPST_NEWTICKET_INSTRUCTIONS . '
                        </td>
                      </tr>
                    </table>
                  </td>
                </tr>
              </table>
            </div>
            <input type="hidden" name="tri_debug" />
            </form>
          </div>
          ';

        return $string;
    }

    /**
    * Builds the createuser page.
    *
    * @access public
    * @return string
    * @param array $request
    * @param string $admin_type
    */
    public function buildCreateuser($request, $admin_type) {
        $error_fields = array();
        $url = $this-> getAbsoluteUrl() ;

        $string = $this->buildNavigation($request);
        if (is_array($this->getMsg())) {
            $msg = $this->buildErrorList();
            $this->setMsg($msg);
        } elseif ($this->getMsg() === true) {
            // If the message equals true, it means the user was successfully added
            $this->setMsg(PHPST_USER_INSERT_SUCCESS);
            $string .= '
                  <div class="gap"></div>
                    <table class="tbl">
                      <tr><td colspan="2">' . $this->getMsg() . '</td></tr>
                    </table>';
            return $string;
        }

        $string .= '
            <div class="gap"></div>
            <form action="' . $url . '" method="post" name="newuser">';

        if ($admin_type != 'Admin') {
            $string .= '<input type="hidden" name="register" value="register" />';
        }

        $string .= '
              <input type="hidden" name="formdata" value="register" />
              <input type="hidden" name="page" value="createuser" />
            <table class="tbl">
              <tr>
                <td class="title">New User</td>
                <td width="70">';

        $help = "Use this form to create a new User.";
        $string .= PHPST_IconGUI::getIcon('help', '16', 'images/icons/', 0, 0, 'style="cursor:pointer"'
            . 'onmouseover="return escape(\'' . $help . '\')"');
        $string .= PHPST_IconGUI::getIcon('arrow-down', '16', 'images/icons/', 0, 0, 'onClick="blocking(\'showNewUser\', \'block\'); return false;" style="cursor:pointer"');
        $string .= '
                  </td>
                </tr>
              </table>
              <div id="showNewUser">
            <table class="tbl"
              <tr><td colspan="2">' . $this->getMsg() . '</td></tr>
              <tr>
                <th>' . PHPST_REGNAME . '</th>
                <td><input class="' . eval('if (in_array("name", $error_fields)) return "error";') . '" name="name" value="' . @$request['name'] . '" size="35" /></td>
              </tr>
              <tr>
                <th>' . PHPST_USERNAME . '</th>
                <td><input class="' . eval('if (in_array("username", $error_fields)) return "error";') . '" name="username" value="' . @$request['username'] . '" size="35" /></td>
              </tr>
              <tr>
                <th>' . PHPST_PASSWORD . '</th>
                <td><input class="' . eval('if (in_array("password", $error_fields)) return "error";') . '" type="password" value="' . @$request['password'] . '" name="password" size="35" /></td>
              </tr>
              <tr>
                <th>' . PHPST_EMAIL . '</th>
                <td><input  class="' . eval('if (in_array("email", $error_fields)) return "error";') . '" value="' . @$request['email'] . '" name="email" size="35" /></td>
              </tr>';

        if ($admin_type == 'Admin') {
            $string .= '
                  <tr>
                    <th>' . PHPST_DEPARTMENT . '</th>
                    <td>
                        ' . $this->getDepartmentsDropDown($request) . '
                    </td>
                  </tr>
                  <tr>
                      <th>' . PHPST_ADMIN_STATUS . '</th>
                      <td>
                        ' . PHPST_CLIENT . ': <input type="radio" ' . eval('if (@$request["admin"] == "Client") return \'checked="checked"\';') . ' name="admin" value="Client" />
                        ' . PHPST_MOD . ': <input type="radio" ' . eval('if (@$request["admin"] == "Mod") return \'checked="checked"\';') . ' name="admin" value="Mod" />
                    	' . PHPST_ADMIN . ': <input type="radio" ' . eval('if (@$request["admin"] == "Admin") return \'checked="checked"\';') . ' name="admin" value="Admin" />
                      </td>
                  </tr>';
        } else {
            $string .= '<input type="hidden" name="admin" value="Client" />
                    <input type="hidden" name="department_id" value="0" />';
        }

        $string .= '
            </table>

            <table class="tbl">
              <tr>
                <td><input type="submit" value="Submit" /></td>
              </tr>
            </table>
            </form>
          </div>
        </div>';
			return $string;
    }

    /**
    * Builds a list of errors.
    *
    * @access public
    * @return string
    */
    public function buildErrorList() {
        $errors = $this->getMsg();
        $msg = "<ul>";

        if (is_array($errors)) {
            foreach ($errors as $error) {

                $msg .= "<li>{$error['message']}</li>";
                $error_fields[] = $error['field'];
            }
        }
        $msg .= "</ul>";
        return $msg;
    }

    /**
    * Builds the search box HTML.
    *
    * @access public
    * @param string $admin_type
    * @param string $searchterm
    * @return string
    */
    public function buildSearchBox($searchterm = null, $page = 'home') {
        if(empty($searchterm)) {
            $searchterm = DB_PREFIX_TICKETS . ".subject";
        }
        $terms = explode('.', $searchterm);
        $field = eval('return ' . 'PHPST_' . strtoupper($terms[0]) . ';');
        $url = $this->getAbsoluteUrl() ;
        $string = '<td width="280" align="right">' . PHPST_SEARCH . ' ' . $field .':
                      <form action="' . $url . 'page=' . $page . '" method="post">
                      <input type="hidden" name="field" value="' . $searchterm . '" />
                      <input type="text" name="search" size="15" />
                      <input type="submit" value="' . PHPST_GO . '" />
                      </form>
                   </td>';
        return $string;
    }

    /**
    * Builds the navigation HTML.
    *
    * @access public
    * @param array $request
    * @return string
    */
    public function buildNavigation($request) {
        $url = $this-> getAbsoluteUrl() ;
        $string = '
                <table class="tbl">
                  <tr>
                    <td class="title">Support Tickets Manager</td>
                    <td align="center" width="250">' . PHPST_LOGGEDAS . $this->user->getName() . '</td>
                    <td width="70">';

        $help = "Submit and answer support tickets with the help of this interface.";
        $string .= PHPST_IconGUI::getIcon('help', '16', 'images/icons/', 0, 0, 'style="cursor:pointer"'
            . 'onmouseover="return escape(\'' . $help . '\')"');
        $string .= PHPST_IconGUI::getIcon('arrow-down', '16', 'images/icons/', 0, 0, 'onClick="blocking(\'showTicketsPage\', \'block\'); return false;" style="cursor:pointer"');
        $string .= '
                    </td>
                  </tr>
                </table>
                <div id="showTicketsPage">
                <table class="tbl">
                  <tr>
                    <td><a href="' . $url . 'page=newticket" title="' . PHPST_TITLEREQ . '">' . PHPST_TITLEREQ . '</a></td>
                    <td><a href="' . $url . 'page=browsetickets&amp;field=' .DB_PREFIX_TICKETS . '.status&amp;search=open" title="' . PHPST_TITLEOPE . '">' . PHPST_TITLEOPE . '</a></td>
                    <td><a href="' . $url . 'page=browsetickets&amp;field=' . DB_PREFIX_TICKETS . '.status&amp;search=closed" title="' . PHPST_TITLECLO . '">' . PHPST_TITLECLO . '</a></td>
                    <th>
                        <a href="' . $url . 'logout" title="' . PHPST_TITLELOG . '">' . PHPST_TITLELOG . '</a>
                    </th>
                  </tr>
                </table>
                ';
        return $string;
    }

    /**
     * Adds the footer to the GUI's HTML.
     *
     * @access public
     * @return void
     **/
    public function buildFooter() {
        $string = '
            <table width="100%">
        	  <tr>
        		<td align="center" class="text">
        		</td>
        	  </tr>
        	  <tr>
        		<td align="center"><br />
        		<a href="http://www.triangle-solutions.com" target="_blank" title="triangle solutions web development">Triangle Solutions Ltd</a> |
        		<a href="http://www.phpsupporttickets.com" target="_blank" title="php support tickets">PHP Support Tickets v2.2</a><br /><br />
        		</td>
        	  </tr>
        	</table>

        	</body>
        	</html>';
        $this->addHTML($string);
    }

    /**
    * Builds the browseticket page.
    *
    * @access public
    * @return string
    * @param array $request
    */
    public function buildBrowsetickets($request, $tickets) {
        $url = $this-> getAbsoluteUrl() ;
        unset($_SESSION['csv']);
        $_SESSION['csv'] = $tickets['csv'];
        $tickets = PHPST_Ticket::getActiveTickets($tickets);
        $search = 'all';
        $new_order = 'ASC';

        if (isset($request['sort']) && isset($request['order'])) {
            $sort = $request['sort'];
            $order = $request['order'];
            if ($order == 'ASC') {
                $new_order = 'DESC';
            } else {
                $new_order = 'ASC';
            }
        }

        if (isset($request['search'])) {
            $search = $request['search'];
        }

        $string = $this->buildNavigation($request);

        // If search or sort is performed, display tickets
        $display = "block";
        if (isset($request['search']) || isset($request['sort'])) {
            $display = "block";
        }

        $recent_tickets = 0;
        if ($tickets != false && $tickets['count'] > 0) {
            foreach ($tickets as $t) {
                if ($t['recent']) {
                    $recent_tickets++;
                }
            }
            $string .= '
              <div class="gap"></div>
                <table class="tbl">
                  <tr>
                    <td class="title">Support Tickets</td>';
            $string .= $this->buildSearchBox();
            $string .=   '<td width="70">';

            $help = 'This table shows the support tickets.';

            $string .= PHPST_IconGUI::getIcon('help', '16', 'images/icons/', 0, 0, 'style="cursor:pointer"'
                . 'onmouseover="return escape(\'' . $help . '\')"');
            $string .= PHPST_IconGUI::getIcon('arrow-down', '16', 'images/icons/', 0, 0, 'onClick="blocking(\'showTickets\', \'block\'); return false;" style="cursor:pointer"');
            // $string .= PHPST_IconGUI::getIcon('save', '16', 'images/icons/', 'include/csv_export.php');
            $string .= '
                    </td>
                  </tr>
                </table>
                <script language="javascript" type="text/javascript">
                <!--
                function check_all() {
                  for (var c = 0; c < document.myform.elements.length; c++) {
                    if (document.myform.elements[c].type == \'checkbox\') {
                      if(document.myform.elements[c].checked == true) {
                        document.myform.elements[c].checked = false;
                      } else {
                        document.myform.elements[c].checked = true;
                      }
                    }
                  }
                }
                // -->
                </script>

                <div id="showTickets" style="display: ' . $display . '">
                <form name="myform" action="' . $url . '" method="post">
                <input type="hidden" name="formdata" value="changetickets" />
                <input type="hidden" name="page" value="browsetickets" />
                <table class="tbl">
                  <tr>
                    <td>' . PHPST_RECENT . ': ' . $recent_tickets . ' - ' . PHPST_CLICK . '.</td>
                  </tr>
                </table>';

            $string .= $this->buildPagination($tickets['count']);

            $string .= '
                <table class="tbl" id="table">
                  <tr>
                    <!-- nolink -->
                    <th onclick="check_all();" style="cursor:pointer">
                      <b><u>' . PHPST_ALL . '</u></b>
                    </th>
                    <th>
                      <a href="' . $url . 'page=browsetickets&amp;field=' . DB_PREFIX_TICKETS . '.status&amp;ticket_type=' . @$request['ticket_type'] . '&amp;search=' . $search . '&amp;sort=id&amp;order=' . $new_order . '" title="' . PHPST_ORDERBY . ' ' . PHPST_TICKETID . '">
                        <b>' . PHPST_TICKETID . '</b>
                      </a>
                    </th>
                    <th>
                      <!--<a href="' . $url . 'page=browsetickets" title="' . PHPST_ORDERBY . ' ' . PHPST_REPLIES . '">-->
                        <b>' . PHPST_REPLIES . '</b>
                      <!--</a>-->
                    </th>
                    <th>
                      <a href="' . $url . 'page=browsetickets&amp;field=' . DB_PREFIX_TICKETS . '.status&amp;ticket_type=' . @$request['ticket_type'] . '&amp;search=' . $search . '&amp;sort=subject&amp;order=' . $new_order . '" title="' . PHPST_ORDERBY . ' ' . PHPST_SUBJECT . '">
                        <b>' . PHPST_SUBJECT . '</b>
                      </a>
                    </th>
                    <th>
                      <a href="' . $url . 'page=browsetickets&amp;field=' . DB_PREFIX_TICKETS . '.status&amp;ticket_type=' . @$request['ticket_type'] . '&amp;search=' . $search . '&amp;sort=user_name&amp;order=' . $new_order . '" title="' . PHPST_ORDERBY . ' ' . PHPST_POSTEDBY . '">
                        <b>' . PHPST_POSTEDBY . '</b>
                      </a>
                    </th>
                    <th>
                      <a href="' . $url . 'page=browsetickets&amp;field=' . DB_PREFIX_TICKETS . '.status&amp;ticket_type=' . @$request['ticket_type'] . '&amp;search=' . $search . '&amp;sort=timestamp&amp;order=' . $new_order . '" title="' . PHPST_ORDERBY . ' ' . PHPST_TIME . '">
                        <b>' . PHPST_TIME . '</b>
                      </a>
                    </th>
                    <th>
                      <a href="' . $url . 'page=browsetickets&amp;field=' . DB_PREFIX_TICKETS . '.status&amp;ticket_type=' . @$request['ticket_type'] . '&amp;search=' . $search . '&amp;sort=urgency&amp;order=' . $new_order . '" title="' . PHPST_ORDERBY . ' ' . PHPST_URGENCY . '">
                        <b>' . PHPST_URGENCY . '</b>
                      </a>
                    </th>
                    <th>
                      <a href="' . $url . 'page=browsetickets&amp;field=' . DB_PREFIX_TICKETS . '.status&amp;ticket_type=' . @$request['ticket_type'] . '&amp;search=' . $search . '&amp;sort=department_name&amp;order=' . $new_order . '" title="' . PHPST_ORDERBY . ' ' . PHPST_DEPARTMENT . '">
                        <b>' . PHPST_DEPARTMENT . '</b>
                      </a>
                    </th>
                    <th>
                      <a href="' . $url . 'page=browsetickets&amp;field=' . DB_PREFIX_TICKETS . '.status&amp;ticket_type=' . @$request['ticket_type'] . '&amp;search=' . $search . '&amp;sort=status&amp;order=' . $new_order . '" title="' . PHPST_ORDERBY . ' ' . PHPST_STATUS . '">
                        <b>' . PHPST_STATUS . '</b>
                      </a>
                    </th>
                  </tr>';

              foreach ($tickets as $key => $v) {
                  if ($key != 'count' && $key != 'csv') {
                      $ticket = ($v['ticket']);

                      // Prepare class for highlighting ticket if recent
                      $recent = '';
                      if ($v['recent']) {
                          $recent = 'error';
                      }

                      $string .= '
                      <tr onclick="invertCheckBox(event, this)" bgcolor="' . UseColor() . '">
                        <td><input type="checkbox" name="ticket[]" value="' . $ticket['id'] . '" /></td>
                        <td>
                          <a href="' . $url . 'page=viewticket&amp;id=' . $ticket['id'] . '">' . $ticket['id'] . '</a>
                        </td>
                        <td>[' . @count($v['answers']) . ']</td>
                        <td><a href="' . $url . 'page=viewticket&amp;id=' . $ticket['id'] . '">' . $ticket['subject'] . '</a></td>
                        <td><!--<a href="' . $url . 'page=viewuser&amp;user_id=' . $v['user']['id'] . '">-->' . $v['user']['name'] . '<!--</a>--></td>
                        <td class="' . @$recent . '">' . PHPST_DateTime::unix2mysql(($ticket['timestamp'] + 0.0), 'UK') . '</td>
                        <td style="background-color:#' . eval('return PHPST_URGENCY_COLOUR_' . $ticket['urgency'] . ';') . '">
                          ' . eval('return PHPST_URGENCY_LABEL_' . $ticket['urgency'] . ';') . '
                        </td>
                        <td>' . $v['department']['name'] . '</td>
                        <td> '.
                       // '<span>' . $ticket['status'] . '</span></td>' .
                       '<span>' . ( $ticket['status'] == 'Open' ? PHPST_OPEN : PHPST_CLOSED ) . '</span></td>' .
                      '</tr>';
                  }
              }

                  $string .= '
                          <tr>
                          <!-- nohighlight -->
                            <td colspan="8">
                              <select name="status">
                              <option value="open">' . PHPST_OPEN . '</option>
                              <option value="closed">' . PHPST_CLOSED . '</option>
                              </select>
                              <input type="submit" name="sub" value="' . PHPST_GO . '" />
                            </td>
                          </tr>
                        </table>
                        </form>
                        ';
                  $string .= $this->buildPagination($tickets['count']);
        } elseif ($search != 'all') {
            $string .= '
                <div class="gap"></div>
                <table class="tbl">
                  <tr>
                    <td class="text">' . PHPST_NO_RESULTS . '</td>
                  </tr>
                </table>';
        } else {
            $string .= '
                <div class="gap"></div>
                <table class="tbl">
                  <tr>
                    <td class="text">' . PHPST_NO_TICKETS . '</td>
                  </tr>
                </table>';
        }
        $string .= '</div>
            </div>';
        return $string;
    }

    /**
    * Builds the HTML selection title.
    *
    * @access public
    * @param array $request
    * @return string html
    */
    public function buildSelectionTitle($request) {
        $retString = '';
        if (isset($request['field'])) {
            $retString = PHPST_SELECTION . ucfirst($request['field']) . ' = ' . $request['search'];
        }

        return $retString;
    }

    /**
    * Builds HTML pagination system based on page number and record Count.
    *
    * @access public
    * @param int $count The total number or records
    * @return string HTML
    */
    public function buildPagination($count = 0) {
        $conn = &ADONewConnection(DSN);
        $pager = new PHPST_Paging($conn, 10, 1);
        $pager->calcNum_pages($count);
        $string = $pager->showPaging($_SERVER['PHP_SELF'] . '?' . htmlentities($_SERVER['QUERY_STRING']));

        return $string;

    }

    /**
    * Reset's this GUI's HTML to an empty string.
    *
    * @access public
    * @return void
    */
    public function resetHTML() {
        $this->html = '';
    }

    /**
    * Returns this GUI's page variable.
    *
    * @access public
    * @return string
    */
    public function getPage() {
        return $this->page;
    }

    /**
    * Returns this GUI's page variable, formatted according to the language file definition.
    *
    * @access public
    * @return string
    */
    public function getPageName() {
        return eval('return PHPST_PAGENAME_' . strtoupper($this->page) . ';');
    }

    /**
    * Sets this GUI's page variable.
    *
    * @access public
    * @param string $string
    * @return void
    */
    public function setPage($string) {
        $this->page = $string;
    }

    /**
    * Returns this GUI's msg variable.
    *
    * @access public
    * @return string
    */
    public function getMsg() {
        return $this->msg;
    }

    /**
    * Sets this GUI's msg variable.
    *
    * @access public
    * @param string $string
    * @return void
    */
    public function setMsg($string) {
        $this->msg = $string;
    }

    /**
    * Builds a drop-down menu of all available departments
    *
    * @access public
    * @param array $request
    * @param string $selected The name of the selected department
    * @return string
    */
    public function getDepartmentsDropDown($request = null, $selected = null) {
        $departments = PHPST_FormHandler::get_departments(array('1_cur_page' => 'all'));
        $string = '<select id="departments" name="department_id">';

        // Do not include suspended departments
        foreach ($departments as $key => $dep) {
            if ($key != 'count' && $key != 'csv' && $dep != '' && !is_null($dep)
                    && $dep['department']['name'] != ''
                    && $dep['department']['status'] != 'Suspended') {
                $string .= '<option';
                if (isset($selected) && $selected == $dep['department']['name']) {
                    $string .= ' selected="selected" ';
                }
                $string .= ' value="' . $dep['department']['id'] . '">' . $dep['department']['name'] . '</option>';
            }
        }
        $string .= '</select>';
        return $string;
    }

    /**
    * Takes the REQUEST variables in order to change the GUI's state.
    *
    * @access public
    * @param array $request
    * @param array $files (optional)
    * @return void
    */
    public function update($request, $files = null) {
        $this->setMsg('');
        $this->resetHTML();

        // Handle request data and update object

        // Set the page name
        if (isset($request['page']) && $request['page'] != 'login') {
            $this->setPage($request['page']);
        } else {
            $this->setPage('home');
        }

        // Process user data entry
        if (isset($request['formdata'])) {
            $this->setMsg(PHPST_FormHandler::submitForm($this->getPage(), $request['formdata'],
                    $request, $files));
        }

        // If form was submitted and passed validation, redirect user
        if (isset($request['formdata']) && !is_array($this->getMsg())) {
            switch ($request['page']) {
                case 'newticket':
                    // @todo redirect to the ticket just created, viewticket page
                    $this->setPage('newticket');
                    break;
                case 'newuser':
                    // @todo redirect to the user just created, viewuser page
                    $this->setPage('newuser');
                    break;
                default:
                    break;
            }
        }
        $this->buildHeader($this->getPageName());
        $this->buildBody($request);
        $this->buildFooter();

    }

    /**
    * Builds the viewdepartment page.
    *
    * @access public
    * @return string
    * @param array $request
    */
    public function buildViewdepartment($request) {
        $url = $this-> getAbsoluteUrl() ;
        $department = PHPST_FormHandler::get_department($request);

        $admin_type = $this->user->getAdmin();
        $field = '';
        if (is_array($this->getMsg())) {
            $msg = $this->getMsg();
            $error = $msg[0]['field'];
            $this->setMsg($msg[0]['message']);
        }

        $string = $this->buildNavigation($request);
        $string .= '<div class="gap"></div>
            <table class="tbl">
              <tr>
                <td class="title">Department Details</td>';

        // START ADMIN ONLY
        if ($admin_type == 'Admin') {
            $string .= $this->buildSearchBox(DB_PREFIX_DEPARTMENTS . '.name', 'browsedepartments');
        }
        // END ADMIN ONLY

        $string .= '<td width="70">';
        
        $help = "Use this form to view and edit this department&rsquo;s details.";
        $string .= PHPST_IconGUI::getIcon('help', '16', 'images/icons/', 0, 0, 'style="cursor:pointer"'
            . 'onmouseover="return escape(\'' . $help . '\')"');
        $string .= PHPST_IconGUI::getIcon('arrow-down', '16', 'images/icons/', 0, 0, 'onClick="blocking(\'showNewDepartment\', \'block\'); return false;" style="cursor:pointer"');
        $string .= '
                </td>
              </tr>
            </table>
          <div id="showNewDepartment">';

        // START ADMIN ONLY
        if ($admin_type == 'Admin') {
            $string .= '
            <form enctype="multipart/form-data" action="' . $url . '" method="post">
                <input type="hidden" name="formdata" value="editdepartment" />
                <input type="hidden" name="page" value="viewdepartment" />
                <input type="hidden" name="field" value="department.id" />
                <input type="hidden" name="id" value="' . $department['department']['id'] . '" />
                <input type="hidden" name="search" value="' . $department['department']['id'] . '" />';
        }
        // END ADMIN ONLY

        $string .= '

              <table class="tbl">
                <tr>
                  <th>' . PHPST_NAME . '</th>
                  <td>';

        // START ADMIN ONLY
        if ($admin_type == 'Admin') {
            $string .= '<input type="text" name="name" size="50" value="' . $department['department']['name'] . '" />';
        }
        // END ADMIN ONLY

        // START MOD ONLY
        elseif ($admin_type == 'Mod') {
            $string .= $department['department']['name'];
        }
        // END MOD ONLY
        $string .= '
                  </td>
                </tr>
                <tr>
                  <th>' . PHPST_STATUS . '</th>
                  <td>';
        if ($admin_type == 'Admin') {
            $string .= '<input type="radio" name="status" value="Active" ';
            if ($department['department']['status'] == "Active") {
                $string .= 'checked="checked" ';
            }
            $string .= ' />Active ';
            $string .= '<br /><input type="radio" name="status" value="Suspended" ';
            if ($department['department']['status'] == "Suspended") {
                $string .= 'checked="checked" ';
            }
            $string .= ' />Suspended ';
        } elseif ($admin_type == 'Mod') {
            $string .= $department['department']['status'];
        }

        $string .= '
                  </td>
                </tr>
                <tr>
                  <th>' . PHPST_DESCRIPTION . '</th>
                  <td>';

        // START ADMIN ONLY
        if ($admin_type == 'Admin') {
            $string .= '<textarea name="description" cols="45" rows="6">'
                           . $department['department']['description'] .
                        '</textarea>';
        }
        // END ADMIN ONLY

        // START MOD ONLY
        elseif ($admin_type == 'Mod') {
            $string .= $department['department']['description'];
        }
        // END MOD ONLY

        $string .= '
                  </td>
                </tr>';

        // START ADMIN ONLY
        if ($admin_type == 'Admin') {
            $deleteUrl = $url . 'page=deletedepartment&amp;id=' . $department['department']['id'];
            $string .= '
                    <tr>
                      <td colspan="2" align="left">
                        <input type="submit" value="' . PHPST_SUBMIT .'" />
                        <!-- <button onclick="window.location=\'' . $deleteUrl . '\'">' . PHPST_DELETE . '</button> -->
                      </td>
                    </tr>
                  </table>
                </form>';
        }
        // END ADMIN ONLY

        else {
            $string .= '
                    </tr>
                  </table>';
        }

        $string .= '
                  </div>

            <div class="gap"></div>

            <table class="tbl">
              <tr>
                <td class="title">Tickets</td>
                <td width="70">';

        $help = "This table shows the tickets assigned to this department.";
        $string .= PHPST_IconGUI::getIcon('help', '16', 'images/icons/', 0, 0, 'style="cursor:pointer"'
            . 'onmouseover="return escape(\'' . $help . '\')"');
        $string .= PHPST_IconGUI::getIcon('arrow-down', '16', 'images/icons/', 0, 0, 'onClick="blocking(\'showTickets\', \'block\'); return false;" style="cursor:pointer"');
        $string .= '
                </td>
              </tr>
            </table>
                  <div id="showTickets">
                    <table class="tbl" id="table">';
        if (isset($department['tickets'])) {
            // Remove closed tickets
            foreach ($department['tickets'] as $id => $ticket) {
                if ($ticket['status'] != 'Open') {
                    unset($department['tickets'][$id]);
                }
            }
        }

        $ticket_count = @count($department['tickets']);

        // $string .= $this->buildPagination($ticket_count);

        // If any tickets left open, display them
        if ($ticket_count > 0) {

            $string .= '
                  <tr>
                    <!-- nohighlight nolink -->
                    <th>' . PHPST_TICKETID . '</th>
                    <th>' . PHPST_REPLIES . '</th>
                    <th>' . PHPST_SUBJECT . '</th>
                    <th>' . PHPST_TIME . '</th>
                    <th>' . PHPST_URGENCY . '</th>
                    <th>' . PHPST_STATUS . '</th>
                  </tr>';

            foreach ($department['tickets'] as $id => $ticket) {
                $myTicket = PHPST_FormHandler::get_ticket($ticket);
                $answers = @count($myTicket['answers']);
                $string .= '
                      <tr bgcolor="' . UseColor() . '">
                        <td><a href="' . $url . 'page=viewticket&amp;id=' . $ticket['id'] . '">' . $ticket['id'] . '</a></td>
                        <td>[' . $answers . ']</td>
                        <td>' . $ticket['subject'] . '</td>
                        <td>' . gmdate('d/m/Y H:i:s',$ticket['timestamp']) . '</td>
                        <td bgcolor="#' . eval('return PHPST_URGENCY_COLOUR_' . $ticket['urgency'] . ';') . '">' . eval('return PHPST_URGENCY_LABEL_' . $ticket['urgency'] . ';') . '</td>
                        <td>' . $ticket['status'] . '</td>
                      </tr>';
            } // end foreach
        } else {
            $string .= '
                  <tr>
                    <th colspan="4">' . PHPST_NO_TICKETS . '</th>
                  </tr>';
        }

        $string .= '
                </table>
              </div>

              <div class="gap"></div>
            <table class="tbl">
              <tr>
                <td class="title">Moderators</td>
                <td width="70">';

        $help = "This table shows the moderators and admins assigned to this department.";
        $string .= PHPST_IconGUI::getIcon('help', '16', 'images/icons/', 0, 0, 'style="cursor:pointer"'
            . 'onmouseover="return escape(\'' . $help . '\')"');
        $string .= PHPST_IconGUI::getIcon('arrow-down', '16', 'images/icons/', 0, 0, 'onClick="blocking(\'showUsers\', \'block\'); return false;" style="cursor:pointer"');
        $string .= '
                </td>
              </tr>
            </table>
              <div id="showUsers">

              <table class="tbl" id="table2">';
        if (isset($department['users'])) {
              $string .= '
                  <tr>
                    <th>' . PHPST_USERID . '</th>
                    <th>' . PHPST_NAME . '</th>
                    <th>' . PHPST_USERNAME . '</th>
                    <th>' . PHPST_EMAIL . '</th>
                  </tr>';
            foreach ($department['users'] as $id => $user) {
                $myUser = PHPST_FormHandler::get_user($user);
                $string .= '<tr bgcolor="' . UseColor() . '">';
                if ($admin_type == 'Admin') {
                    $string .= '
                            <td><a href="' . $url . 'page=viewuser&amp;user_id=' . $user['id'] . '">' . $user['id'] . '</a></td>';
                } elseif ($admin_type == 'Mod') {
                    $string .= '
                            <td>' . $user['user_id'] . '</td>';
                }
                $string .= '<td>' . $user['name'] . '</td>
                            <td>' . $user['username'] . '</td>
                            <td><a href="mailto:' . $user['email'] . '">' . $user['email'] . '</a></td>
                          </tr>';
            }
        } else {
            $string .= '
                  <tr>
                    <th colspan="4">' . PHPST_NO_MODS . '</td>
                  </tr>';
        }
        $string .= '
                </table>
            </div>
          </div>';
        return $string;
    }

    /**
    * Abstract method for all GUI implementations.
    * Builds the body HTML based on the GUI's current state.
    *
    * @abstract
    * @access public
    * @return string
    * @param array $request
    */
    public abstract function buildBody($request);

    /**
    * Abstract method for all GUI implementations.
    * Redirects the GUI to the page set as the home page.
    *
    * @abstract
    * @access public
    * @return string
    * @param array $request
    */
    public abstract function buildHome($request);
}
?>