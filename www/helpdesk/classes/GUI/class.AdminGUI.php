<?php
/**
 * File containing the AdminGUI class.
 *
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version SVN: $Id: class.AdminGUI.php 4 2005-12-13 01:47:15Z nicolas $
 * @since File available since Release 1.1.1.1
 * \\||
 */

/**
* user defined includes.
* @include
*/
require_once_check(PHPST_PATH . 'classes/GUI/abstract.GUI.php');

/**
 * AdminGUI is the Admin's User Interface class.
 *
 * @access public
 * @package PHPSupportTicket
 */
class PHPST_AdminGUI extends PHPST_GUI{
    /// --- FIELDS ---

    // --- METHODS ---
    /**
    * Constructor method.
    *
    * @access public
    * @param string $absoluteURL
    * @param array $request
    */
    public function __construct($absoluteURL, $request) {

        parent::__construct($absoluteURL, $request);

        $this->buildHeader();
        $this->buildBody($request);
        $this->buildFooter();
    }

    /**
    * Updates this GUI using the $_REQUEST data sent from the previous page.
    *
    * @access public
    * @param array $request
    * @param array $files (optional)
    * @return void
    */
    public function update($request, $files = null) {
        parent::update($request, $files);
    }

    /**
    * Builds the body of the GUI based on its current state.
    *
    * @access public
    * @return void
    * @param array $request
    */
    public function buildBody($request) {
        if ($this->getPage() == '') {
            $this->setPage("home");
        }

        // Dynamically prepare the build function matching the requested page.
        $method = "build" . ucfirst($this->getPage());
        if (in_array($method, get_class_methods($this))) {
            $this->addHTML($this->$method($request));
        } else {
            $this->addHTML($this->buildHome($request));
        }
    }

    /**
    * Builds the createuser page.
    *
    * @access public
    * @return string
    * @param array $request
    */
    public function buildCreateuser($request) {
        return parent::buildCreateuser($request, 'Admin');
    }

    /**
    * Builds the createdepartment page.
    *
    * @access public
    * @return string
    * @param array $request
    */
    public function buildCreatedepartment($request) {
        $error_fields = array();
        $url = $this->getAbsoluteUrl() ;
        $string = $this->buildNavigation($request);

        if (is_array($this->getMsg())) {
            $msg = $this->buildErrorList();
            $this->setMsg($msg);
        } elseif ($this->getMsg() === true) {
            // If the message equals true, it means the department was successfully added
            $this->setMsg(PHPST_DEPARTMENT_INSERT_SUCCESS);
            $string .= '
                  <div class="gap"></div>
                    <table class="tbl">
                      <tr><td colspan="2">' . $this->getMsg() . '</td></tr>
                    </table>';
            return $string;
        } elseif (is_int($this->getMsg())) {
            $int = $this->getMsg();
            $this->setMsg(null);
            return $this->buildViewdepartment(array('id' => $int));
        }

        $string .= '
            <div class="gap"></div>
            <table class="tbl">
              <tr>
                <td class="title">New Department</td>
                <td width="70">';

            $help = "Use this form to create a new Department.";
            $string .= PHPST_IconGUI::getIcon('help', '16', 'images/icons/', 0, 0, 'style="cursor:pointer"'
                . 'onmouseover="return escape(\'' . $help . '\')"');
            $string .= PHPST_IconGUI::getIcon('arrow-down', '16', 'images/icons/', 0, 0, 'onClick="blocking(\'showNewDepartment\', \'block\'); return false;" style="cursor:pointer"');
            $string .= '
                  </td>
                </tr>
              </table>
            <div id="showNewDepartment">
            <form action="' . $url . '" method="post" name="newdepartment">
              <input type="hidden" name="formdata" value="newdepartment" />
              <input type="hidden" name="page" value="createdepartment" />
            <table class="tbl">';
            if (strlen($this->getMsg()) > 5) {
                $string .= '<tr><td colspan="2">' . var_dump($this->getMsg()) . '</td></tr>';
            }
            $string .='
              <tr>
                <th>' . PHPST_NAME . '</th>
                <td><input class="' . eval('if (in_array("name", $error_fields)) return "error";') . '" name="name" value="' . @$request['name'] . '" size="35" /></td>
              </tr>
              <tr>
                <th>' . PHPST_DESCRIPTION . '</th>
                <td><textarea class="' . eval('if (in_array("description", $error_fields)) return "error";') . '" name="description" cols="40" rows="4">' . @$request['username'] . '</textarea></td>
              </tr>
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
    * Builds the home page (browse tickets).
    *
    * @access public
    * @return string
    * @param array $request
    */
    public function buildBrowsetickets($request) {
        // Retrieve list of tickets from DB
        $tickets = PHPST_FormHandler::get_tickets('Admin', $request , null);
        return parent::buildBrowsetickets($request, $tickets);
    }

    /**
    * Redirects to this GUI's home page.
    *
    * @access public
    * @return void
    * @param array $request
    */
    public function buildHome($request) {
        return $this->buildBrowsetickets($request);
    }

    /**
    * Builds the viewticket page (view tickets).
    *
    * @access public
    * @return void
    * @param array $request
    */
    public function buildViewticket($request) {
        // Retrieve ticket info from DB
        // Note this is not a Ticket object, but an array containing a Ticket, a Department
        // and a User objects associated with that Ticket.
        $ticket = PHPST_FormHandler::get_ticket($request);
        return parent::buildViewticket($request, $ticket);
    }

    /**
    * Builds the navigation HTML.
    *
    * @access public
    * @param array $request
    * @param string $searchterm
    * @param string $page
    * @return string
    */
    public function buildNavigation($request, $searchterm = 'tickets.subject', $page = 'home') {
        $selection_title = parent::buildSelectionTitle($request);
        $url = $this-> getAbsoluteUrl() ;
        $string = '
                <table class="tbl">
                  <tr>
                    <td class="title"><a href="' . $url . 'page=browsetickets">Support Tickets Manager</a></td>
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
                    <th>
                      <a href="' . $url . 'page=browsetickets">' . PHPST_TICKETS . '</a>
                    </th>
                    <th>
                      <a href="' . $url . 'page=browsedepartments">' . PHPST_DEPARTMENTS . '</a>
                    </th>
                    <th>
                      <a href="' . $url . 'page=browseusers">' . PHPST_USERS . '</a>
                    </th>
                    <th>
                      <a href="' . $url . 'page=options">' . PHPST_OPTIONS . '</a>
                    </th>
                    <th>
                        <a href="' . $url . 'logout" title="' . PHPST_TITLELOG . '">' . PHPST_TITLELOG . '</a>
                    </th>
                  </tr>
                  <tr>
                    <td>
                      <a href="' . $url . 'page=browsetickets&amp;field=tickets_tickets.status&amp;search=open" title="' . PHPST_TITLEOPE . '">' . PHPST_TITLEOPE .'</a> |
                      <a href="' . $url . 'page=browsetickets&amp;field=tickets_tickets.status&amp;search=closed" title="' . PHPST_TITLECLO . '">' . PHPST_TITLECLO .'</a> |
                      <a href="' . $url . 'page=newticket" title="' . PHPST_TITLEREQ . '">' . PHPST_TITLEREQ .'</a>
                    </td>
                    <td>
                      <a href="' . $url . 'page=browsedepartments" title="' . PHPST_BROWSE . '">' . PHPST_BROWSE .'</a> |
                      <a href="' . $url . 'page=createdepartment" title="' . PHPST_CREATE_DEPARTMENT . '">' . PHPST_CREATE_DEPARTMENT .'</a>
                    </td>
                    <td>
                      <a href="' . $url . 'page=browseusers" title="' . PHPST_BROWSE . '">' . PHPST_BROWSE .'</a> |
                      <a href="' . $url . 'page=createuser" title="' . PHPST_CREATE_USER . '">' . PHPST_CREATE_USER .'</a>
                    </td>
                    <td>

                      <a href="' . $url . 'page=options" title="' . PHPST_EMAILOPT . '">' . PHPST_EMAILOPT .'</a>
                      <!--<a href="' . $url . 'page=privileges" title="' . PHPST_PRIVILEGES . '">' . PHPST_PRIVILEGES .'</a>
                      <a href="' . $url . 'page=ratings" title="' . PHPST_RATINGS . '">' . PHPST_RATINGS .'</a>
                      <a href="' . $url . 'page=files" title="' . PHPST_FILES . '">' . PHPST_FILES .'</a>
                      <a href="' . $url . 'page=privacy" title="' . PHPST_PRIVACY . '">' . PHPST_PRIVACY .'</a>
                      -->
                    </td>
                  </tr>
                </table>';
        return $string;
    }

    /**
    * Builds the options page
    *
    * @access public
    * @return string HTML
    * @param array $request
    */
    public function buildOptions($request) {
        $url = $this-> getAbsoluteUrl() ;

        // Load options from DB
        $options = PHPST_FormHandler::get_options();

        // Build table and form headers
        $string = $this->buildNavigation($request);
        $string .= '
            <div class="gap"></div>
            <table class="tbl">
              <tr>
                <td class="title">Email Options</td>
                <td width="70">';        
        $help = 'This table shows the email options.';
        $string .= PHPST_IconGUI::getIcon('help', '16', 'images/icons/', 0, 0, 'style="cursor:pointer"'
            . 'onmouseover="return escape(\'' . $help . '\')"');
        $string .= PHPST_IconGUI::getIcon('arrow-down', '16', 'images/icons/', 0, 0, 'onClick="blocking(\'showOptions\', \'block\'); return false;" style="cursor:pointer"');
        // $string .= PHPST_IconGUI::getIcon('save', '16', 'images/icons/', 'include/csv_export.php');
        $string .= '
                </td>
              </tr>
            </table>
            <div id="showOptions" style="display: block">
                <form name="editoption" action="' . $url . '" method="post">
                    <input type="hidden" name="formdata" value="edit" />
                    <input type="hidden" name="page" value="options" />
                    <table class="tbl">
                    ';
        // Display options dynamically in rows
        foreach ($options as $option) {
            // Prepare input field
            $input = '';
            switch ($option['type']) {
                case 'boolean':
                    $selected_0 = '';
                    $selected_1 = '';
                    if ($option['value'] == 0) {
                        $selected_0 = ' checked="checked" ';
                    } else {
                        $selected_1 = ' checked="checked" ';
                    }
                    $input = PHPST_YES . '<input type="radio"' . $selected_1 . ' name="' . $option['name'] . '" value="1" /> '
                            . PHPST_NO . '<input type="radio"' . $selected_0 . ' name="' . $option['name'] . '" value="0" />';
                    break;
                case 'integer':
                    $input = '<input type="text" name="' . $option['name'] . '" value="' . $option['value'] . '" />';
                    break;
                default:
                    break;
            }

            // Add row
            $string .= '
                  <tr align="left">
                    <th>' . eval('return PHPST_OPTION_' . $option['id'] . ';') . '</th>
                    <td>' . $input . '</td>
                  </tr>';
        }

        // Close table and form
        $string .= '
                  <tr align="right">
                    <td colspan="2">
                      <input type="submit" value="' . PHPST_SUBMIT .'" />
                    </td>
                  </tr>
            </table>
            </form>
          </div>
        </div>';

        return $string;
    }

    /**
    * Builds the browseusers page.
    *
    * @access public
    * @return string
    * @param array $request
    */
    public function buildBrowseusers($request) {

        // Retrieve list of users from DB
        $users = PHPST_FormHandler::get_users($request);
        unset($_SESSION['csv']);
        $_SESSION['csv'] = $users['csv'];

        $url = $this-> getAbsoluteUrl() ;
        $numberOfPages = ceil($users['count'] / PHPST_MAX_RECORDS);
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

        // If search or sort is performed, display tickets
        $display = "block";
        if (isset($request['search']) || isset($request['sort'])) {
            $display = "block";
        }

        if (isset($request['search'])) {
            $search = $request['search'];
        }

        $string = $this->buildNavigation($request, DB_PREFIX_USER . '.' . DB_PREFIX_USER_NAME, 'browseusers');

        if ($users != false) {
            // $string .= $this->buildPagination($users['count']);
            $string .= '
              <div class="gap"></div>
                <table class="tbl">
                  <tr>
                    <td colspan="7" class="title">Users</td>';
            $string .= parent::buildSearchBox(DB_PREFIX_USER . '.' . DB_PREFIX_USER_NAME, 'browseusers');
            $string .= '<td width="70">';
            
            $help = 'Select the CLIs on which you need to perform global actions. '
                . 'Filter the list by CLI status or network or select All/No CLIs or invert the selection.';

            $string .= PHPST_IconGUI::getIcon('help', '16', 'images/icons/', 0, 0, 'style="cursor:pointer"'
                . 'onmouseover="return escape(\'' . $help . '\')"');
            $string .= PHPST_IconGUI::getIcon('arrow-down', '16', 'images/icons/', 0, 0, 'onClick="blocking(\'showUsers\', \'block\'); return false;" style="cursor:pointer"');
            $string .= PHPST_IconGUI::getIcon('save', '16', 'images/icons/', 'include/csv_export.php');
            $string .= '
                    </td>
                  </tr>
                </table>

                <div id="showUsers" style="display: ' . $display . '">
                <table class="tbl">
                  <tr>
                    <td>' . PHPST_CLICK_USER . '.</td>
                  </tr>
                </table>
                <table class="tbl" id="table">
                  <tr>
                    <!-- nolink -->
                    <th>
                      <a href="' . $url . 'page=browseusers&amp;field=name&amp;search=' . $search . '&amp;sort=user_id&amp;order=' . $new_order . '" title="' . PHPST_ORDERBY . ' ' . PHPST_USERID . '">
                        <b>' . PHPST_USERID . '</b>
                      </a>
                    </th>
                    <th>
                      <a href="' . $url . 'page=browseusers&amp;field=name&amp;search=' . $search . '&amp;sort=admin&amp;order=' . $new_order . '" title="' . PHPST_ORDERBY . ' ' . PHPST_ADMIN_STATUS . '">
                        <b>' . PHPST_ADMIN_STATUS . '</b>
                      </a>
                    </th>
                    <th>
                      <a href="' . $url . 'page=browseusers&amp;field=name&amp;search=' . $search . '&amp;sort=timestamp&amp;order=' . $new_order . '" title="' . PHPST_ORDERBY . ' ' . PHPST_DATE_JOINED . '">
                        <b>' . PHPST_DATE_JOINED . '</b>
                      </a>
                    </th>
                    <th>
                      <a href="' . $url . 'page=browseusers&amp;field=name&amp;search=' . $search . '&amp;sort=name&amp;order=' . $new_order . '" title="' . PHPST_ORDERBY . ' ' . PHPST_NAME . '">
                        <b>' . PHPST_NAME . '</b>
                      </a>
                    </th>
                    <th>
                      <a href="' . $url . 'page=browseusers&amp;field=name&amp;search=' . $search . '&amp;sort=username&amp;order=' . $new_order . '" title="' . PHPST_ORDERBY . ' ' . PHPST_INDEX_USERNAME . '">
                        <b>' . PHPST_INDEX_USERNAME . '</b>
                      </a>
                    </th>
                    <th>
                      <a href="' . $url . 'page=browseusers&amp;field=name&amp;search=' . $search . '&amp;sort=department_name&amp;order=' . $new_order . '" title="' . PHPST_ORDERBY . ' ' . PHPST_DEPARTMENT . '">
                        <b>' . PHPST_DEPARTMENT . '</b>
                      </a>
                    </th>
                    <th>
                      <a href="' . $url . 'page=browseusers&amp;field=name&amp;search=' . $search . '&amp;sort=email&amp;order=' . $new_order . '" title="' . PHPST_ORDERBY . ' ' . PHPST_EMAIL . '">
                        <b>' . PHPST_EMAIL . '</b>
                      </a>
                    </th>
                    <th>
                      <a href="' . $url . 'page=browseusers&amp;field=name&amp;search=' . $search . '&amp;sort=ticket_timestamp&amp;order=' . $new_order . '" title="' . PHPST_ORDERBY . ' ' . PHPST_TICKETS . '">
                        <b>' . PHPST_TICKETS_TITLE . '</b>
                      </a>
                    </th>
                  </tr>';

              foreach ($users as $key => $v) {
                  if ($key != 'count' && $key != 'csv') {
                      $admin = $v['user']['admin'];
                      $date_created = gmdate('d/m/Y H:i:s',$v['user']['timestamp']);
                      $name = $v['user']['name'];
                      $username = $v['user']['username'];
                      $id = $v['user']['user_id'];
                      $email = $v['user']['email'];
                      $department_id = @$v['department']['id'];
                      $department_name = @$v['department']['name'];

                      $string .= "
                          <tr onclick=\"invertCheckBox(event, this)\" bgcolor=\"" . UseColor() . "\">
                            <!-- nolink -->
                            <td>
                              <a href=\"" . $url . "page=viewuser&amp;user_id=$id\">
                                $id
                              </a>
                            </td>
                            <td>$admin</td>
                            <td>$date_created</td>
                            <td>$name</td>
                            <td>$username</td>
                            <td>
                              <a href=\"" . $url . "page=viewdepartment&amp;field=" . DB_PREFIX_DEPARTMENTS . ".id&amp;search=$department_id&amp;id=$department_id\">
                                $department_name
                              </a>
                            </td>
                            <td>
                              <a href=\"mailto:$email\">
                                $email
                              </a>
                            </td>
                            <td>";
                      $tickets_count = 0;

                      // Only proceed if there are tickets
                      if (isset($v['tickets']) && is_array($v['tickets'])) {
                          foreach ($v['tickets'] as $ticket) {
                              if ($ticket['status'] == 'Open') {
                                  $tickets_count ++;
                              }
                          }
                      }

                      if (isset($v['tickets'])) {
                          $string .= "<a href=\"" . $url . "page=browsetickets&amp;field=" . DB_PREFIX_USER . "." . DB_PREFIX_USER_ID . "&amp;search=$id\">
                                " . $tickets_count . "/" . @count($v['tickets']) ."
                              </a>";
                      } else {
                          $string .= "0";
                      }

                      $string .= "</td>
                          </tr>";

                  }
              }
              $string .= '</table>';
              $string .= $this->buildPagination($users['count']);
        } else {
            $string .= '
                <div class="gap"></div>
                <table class="tbl">
                  <tr>
                    <td>' . PHPST_NO_RESULTS . '</td>
                  </tr>
                </table>
              </div>
            </div>';
        }
        return $string;
    }

    /**
    * Builds the browsedepartments page.
    *
    * @access public
    * @return string
    * @param array $request
    */
    public function buildBrowsedepartments($request) {
        // Retrieve list of departments from DB
        $departments = PHPST_FormHandler::get_departments($request);
        $url = $this-> getAbsoluteUrl() ;
        $search = 'all';
        unset($_SESSION['csv']);
        $_SESSION['csv'] = $departments['csv'];
        // Calculate the number of pages required for pagination
        $numberOfPages = ceil($departments['count'] / PHPST_MAX_RECORDS);
        $new_order = 'ASC';

        // Prepare the sorting order
        if (isset($request['sort']) && isset($request['order'])) {
            $sort = $request['sort'];
            $order = $request['order'];
            if ($order == 'ASC') {
                $new_order = 'DESC';
            } else {
                $new_order = 'ASC';
            }
        }

        // Carry forward the search criteria
        if (isset($request['search'])) {
            $search = $request['search'];
        }

        // If search or sort is performed, display tickets
        $display = "block";
        if (isset($request['search']) || isset($request['sort'])) {
            $display = "block";
        }

        $string = $this->buildNavigation($request, 'departments.name');

        if ($departments != false) {
            // $string .= $this->buildPagination($departments['count']);
            $string .= '
              <div class="gap"></div>
                <table class="tbl">
                  <tr>
                    <td class="title">Departments</td>';
            $string .= parent::buildSearchBox(DB_PREFIX_DEPARTMENTS . '.name', 'browsedepartments');
            $string .= '<td width="70">';
            
            $help = 'Select the CLIs on which you need to perform global actions. '
                . 'Filter the list by CLI status or network or select All/No CLIs or invert the selection.';

            $string .= PHPST_IconGUI::getIcon('help', '16', 'images/icons/', 0, 0, 'style="cursor:pointer"'
                . 'onmouseover="return escape(\'' . $help . '\')"');
            $string .= PHPST_IconGUI::getIcon('arrow-down', '16', 'images/icons/', 0, 0, 'onClick="blocking(\'showDepartments\', \'block\'); return false;" style="cursor:pointer"');
            $string .= PHPST_IconGUI::getIcon('save', '16', 'images/icons/', 'include/csv_export.php');
            $string .= '
                    </td>
                  </tr>
                </table>
                <div id="showDepartments" style="display: ' . $display . '">
                <table class="tbl">
                  <tr>
                    <td>' . PHPST_CLICK_DEPARTMENT . '.</td>
                  </tr>
                </table>

                <table class="tbl" id="table">
                  <tr>
                    <!-- nolink -->
                    <th>
                      <a href="' . $url . 'page=browsedepartments&amp;field=name&amp;search=' . $search . '&amp;sort=user_id&amp;order=' . $new_order . '" title="' . PHPST_ORDERBY . ' ' . PHPST_USERID . '">
                        <b>' . PHPST_USERID . '</b>
                      </a>
                    </th>
                    <th>
                      <a href="' . $url . 'page=browsedepartments&amp;field=name&amp;search=' . $search . '&amp;sort=name&amp;order=' . $new_order . '" title="' . PHPST_ORDERBY . ' ' . PHPST_NAME . '">
                        <b>' . PHPST_NAME . '</b>
                      </a>
                    </th>
                    <th>
                        <b>' . PHPST_TICKETS_TITLE . '</b>
                    </th>
                    <th>
                        <b>' . PHPST_MODS . '</b>
                    </th>
                    <th>
                        <b>' . PHPST_STATUS . '</b>
                    </th>
                    <th>
                        <b>' . PHPST_SUSPEND . ' / ' . PHPST_ACTIVATE . '</b>
                    </th>
                  </tr>';

              foreach ($departments as $key => $v) {
                  if ($key != 'count' && $key != 'csv') {
                      $name = $v['department']['name'];
                      $id = $v['department']['id'];

                      $tickets_count = 0;
                      // Only proceed if there are tickets
                      if (isset($v['tickets']) && is_array($v['tickets'])) {
                          foreach ($v['tickets'] as $ticket) {
                              if ($ticket['status'] == 'Open') {
                                  $tickets_count ++;
                              }
                          }
                      }

                      $users_count = @count($v['users']);

                      if ($v['department']['status'] == 'Suspended') {
                          $button = "<button onclick=\"window.location='" . $url . "page=browsedepartments&amp;id=$id&amp;formdata=update&amp;status=Active';\">" . PHPST_ACTIVATE . "</button>";
                      } else {
                          $button = "<button onclick=\"window.location='" . $url . "page=browsedepartments&amp;id=$id&amp;formdata=update&amp;status=Suspended';\">" . PHPST_SUSPEND . "</button>";
                      }
                      $string .= "
                          <tr onclick=\"invertCheckBox(event, this)\" bgcolor=\"" . UseColor() . "\">
                            <!-- nolink -->
                            <td>
                              <a href=\"" . $url . "page=viewdepartment&amp;field=" . DB_PREFIX_DEPARTMENTS . ".id&amp;search=$id&amp;id=$id\">
                                $id
                              </a>
                            </td>
                            <td>$name</td>
                            <td>
                              <a href=\"" . $url . "page=browsetickets&amp;field=" . DB_PREFIX_DEPARTMENTS . ".id&amp;search=$id\">
                                $tickets_count/" . @count($v['tickets']) . "
                              </a>
                            </td>
                            <td>
                              <a href=\"" . $url . "page=browseusers&amp;field=" . DB_PREFIX_DEPARTMENTS . ".id&amp;search=$id\">
                                $users_count
                              </a>
                            </td>
                            <td>
                              {$v['department']['status']}
                            </td>
                            <td>
                              $button
                            </td>
                          </tr>";
                  }
              }

              $string .= '</table>';
              $string .= $this->buildPagination($departments['count']);
              $string .= '</div>';
        } else {
            $string .= '
                <div class="gap"></div>
                <table class="tbl">
                  <tr>
                    <td class="text">' . PHPST_NO_RESULTS . '</td>
                  </tr>
                </table>';
        }
        $string .= '</div>';
        return $string;
    }

    /**
    * Builds the deleteDepartment page.
    *
    * @access public
    * @return string
    * @param array $request
    */
    public function buildDeletedepartment($request) {
        $url = $this-> getAbsoluteUrl() ;
        $result = PHPST_FormHandler::delete_department($request);
        $string = $this->buildNavigation($request);
        $string .= '<table class="tbl">
              <tr>
                <td>
                    ' . PHPST_DELETE_DEPARTMENT_SUCCESS . '
                </td>
              </tr>
            </table>';
        return $string;
    }

    /**
    * Builds the viewuser page.
    *
    * @access public
    * @return string
    * @param array $request
    */
    public function buildViewuser($request) {
        $url = $this-> getAbsoluteUrl() ;
        $user = PHPST_FormHandler::get_user($request);

        $field = '';
        if (is_array($this->getMsg())) {
            $msg = $this->getMsg();
            $error = $msg[0]['field'];
            $this->setMsg($msg[0]['message']);
        }

        // If there are no users for this user, just create an empty array
        if (!isset($user['tickets'])) {
            $user['tickets'] = array();
        }

        $dep_dropdown = 'N/A';

        // Only show departments drop-down if user is a mod
        if ($user['user']['admin'] == 'Mod') {
            $dep_dropdown = parent::getDepartmentsDropDown($request, @$user['department']['name']);
            $dep_dropdown .= "<a href=\"" . $url . "page=viewdepartment&amp;field=" . DB_PREFIX_DEPARTMENTS . ".id&amp;search={$user['department']['id']}&amp;id={$user['department']['id']}\">
                        " . PHPST_VIEW . '</a>';
        }

        $string = $this->buildNavigation($request);
        $string .= '
              <div class="gap"></div>
                <table class="tbl">
                  <tr>
                    <td colspan="7" class="title">User Details</td>';
            $string .= parent::buildSearchBox(DB_PREFIX_USER . '.' . DB_PREFIX_USER_NAME, 'browseusers');
            $string .= '
                    <td width="70">';
            
            $help = 'This table shows you this user&rsquo;s details and lets you change some of them';

            $string .= PHPST_IconGUI::getIcon('help', '16', 'images/icons/', 0, 0, 'style="cursor:pointer"'
                . 'onmouseover="return escape(\'' . $help . '\')"');
            $string .= PHPST_IconGUI::getIcon('arrow-down', '16', 'images/icons/', 0, 0, 'onClick="blocking(\'showDetails\', \'block\'); return false;" style="cursor:pointer"');
            $string .= '
                    </td>
                  </tr>
                </table>
            <form enctype="multipart/form-data" action="' . $url . '" method="post">
                <input type="hidden" name="formdata" value="edituser" />
                <input type="hidden" name="page" value="viewuser" />
                <input type="hidden" name="field" value="' . DB_PREFIX_USER . '.' . DB_PREFIX_USER_ID . '" />
                <input type="hidden" name="search" value="' . $user['user']['user_id'] . '" />
                <input type="hidden" name="username" value="' . $user['user']['username'] . '" />
                <input type="hidden" name="user_id" value="' . $user['user']['user_id'] . '" />
                <input type="hidden" name="email" value="' . $user['user']['email'] . '" />
                <input type="hidden" name="password" value="' . $user['user']['password'] . '" />
                <input type="hidden" name="timestamp" value="' . $user['user']['timestamp'] . '" />
                <input type="hidden" name="admin" value="' . $user['user']['admin'] . '" />
                <!--<input type="hidden" name="last_login" value="' . @$user['user']['last_login'] . '" />-->
                <div id="showDetails" style="display:block">
                  <table class="tbl">
                    <tr>
                      <th>' . PHPST_NAME . '</th>
                      <td>
                        <input type="text" name="name" value="' . $user['user']['name'] . '" />
                      </td>
                    </tr>
                    <tr>
                      <th>' . PHPST_USERNAME . '</th>
                      <td>' . $user['user']['username'] . '</td>
                    </tr>
                    <tr>
                      <th>' . PHPST_PASSWORD . '</th>
                      <td>' . $user['user']['password'] . '</td>
                    </tr>
                    <tr>
                      <th>' . PHPST_DEPARTMENT . '</th>
                      <td>' . $dep_dropdown . '</td>
                    </tr>

                    <tr>
                      <th>' . PHPST_EMAIL . '</th>
                      <td><a href="mailto:' . $user['user']['email'] . '">' . $user['user']['email'] . '</a></td>
                    </tr>
                    <tr>
                      <td colspan="2">
                        <input type="submit" value="' . PHPST_SUBMIT .'" />
                      </td>
                    </tr>
                  </table>
              </div>

              <div class="gap"></div>

                <table class="tbl">
                  <tr>
                    <td colspan="7" class="title">User\'s Tickets</td>
                    <td width="70">';
            
            $help = 'This table shows you which tickets this user has created.';

            $string .= PHPST_IconGUI::getIcon('help', '16', 'images/icons/', 0, 0, 'style="cursor:pointer"'
                . 'onmouseover="return escape(\'' . $help . '\')"');
            $string .= PHPST_IconGUI::getIcon('arrow-down', '16', 'images/icons/', 0, 0, 'onClick="blocking(\'showTickets\', \'block\'); return false;" style="cursor:pointer"');
            $string .= '
                    </td>
                  </tr>
                </table>

                <div id="showTickets" style="display:none">
                <table class="tbl" id="table">';
            if (isset($user['tickets'])) {
                // Remove closed tickets
                foreach ($user['tickets'] as $id => $ticket) {
                    if ($ticket['status'] != 'Open') {
                        unset($user['tickets'][$id]);
                    }
                }
            }

            // If any tickets are left, display them
            if (count($user['tickets']) > 0) {
                  $string .= '
                      <tr>
                        <!-- nohighlight -->
                        <th>' . PHPST_TICKETID . '</th>
                        <th>' . PHPST_REPLIES . '</th>
                        <th>' . PHPST_SUBJECT . '</th>
                        <th>' . PHPST_TIME . '</th>
                        <th>' . PHPST_URGENCY . '</th>
                        <th>' . PHPST_STATUS . '</th>
                      </tr>';

                foreach ($user['tickets'] as $id => $ticket) {
                    $myTicket = PHPST_FormHandler::get_ticket($ticket);
                    $answers = @count($myTicket['answers']);
                    $string .= '
                          <tr bgcolor="' . UseColor() . '">
                            <td><a href="' . $url . 'page=viewticket&amp;id=' . $ticket['id'] . '">' . $ticket['id'] . '</a></td>
                            <td>[' . $answers . ']</td>
                            <td>' . $ticket['subject'] . '</td>
                            <td>' . gmdate('d/m/Y H:i:s',$ticket['timestamp']) . '</td>
                            <td bgcolor="#' . eval('return PHPST_URGENCY_COLOUR_' . $ticket['urgency'] . ';') . '">' . eval('return PHPST_URGENCY_LABEL_' . $ticket['urgency'] . ';') . '</td>
                            <td>
                            <span style="color:#000000">' . $ticket['status'] . '</span></td>
                          </tr>';
                } // end foreach
            } else {
                $string .= '
                      <tr bgcolor="#AABBDD">
                        <td colspan="4"><b>' . PHPST_NO_TICKETS . '</b></td>
                      </tr>';
            }
            $string .= '
                    </table>
                  </div>
            </form>';
        return $string;
    }
}
?>