<?php
/**
 * File containing the ModGUI class.
 *
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version SVN: $Id: class.ModGUI.php 4 2005-12-13 01:47:15Z nicolas $
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
class PHPST_ModGUI extends PHPST_GUI{
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
    * Builds the home page (browse tickets).
    *
    * @access public
    * @return string
    * @param array $request
    */
    public function buildBrowsetickets($request) {
        // Retrieve list of tickets from DB
        $id = $this->user->getInfo('department_id');

        if (isset($request['ticket_type']) && $request['ticket_type'] == 'my') {
            $id = $this->user->getId();
        }

        $tickets = PHPST_FormHandler::get_tickets('Mod', $request, $id);

        return parent::buildBrowsetickets($request, $tickets);
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
        // $string = parent::buildSearchBox($searchterm, $page);
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
                      <a href="' . $url . 'page=viewdepartment&amp;field=' . DB_PREFIX_DEPARTMENTS . '.id&amp;search=' . $this->user->getInfo('department_id') . '&amp;id=' . $this->user->getInfo('department_id') . '" title="' . PHPST_DEPARTMENT . '">' . PHPST_DEPARTMENT .'</a>
                    </th>
                    <th>
                      <a href="' . $url . 'page=browsetickets">' . PHPST_MYTICKETS . '</a>
                    </th>
                    <th>
                        <a href="' . $url . 'logout" title="' . PHPST_TITLELOG . '">' . PHPST_TITLELOG . '</a>
                    </th>
                  </tr>
                  <tr>
                    <td>
                      <a href="' . $url . 'page=viewdepartment&amp;field=' . DB_PREFIX_DEPARTMENTS . '.id&amp;search=' . $this->user->getInfo('department_id') . '&amp;id=' . $this->user->getInfo('department_id') . '" title="' . PHPST_VIEW . ' ' . PHPST_DEPARTMENT . '">' . PHPST_VIEW . ' ' . PHPST_DEPARTMENT . '</a> |
                      <a href="' . $url . 'page=browsetickets&amp;field=' . DB_PREFIX_TICKETS . '.status&amp;search=open" title="' . PHPST_TITLEOPE . '">' . PHPST_TITLEOPE .'</a> |
                      <a href="' . $url . 'page=browsetickets&amp;field=' . DB_PREFIX_TICKETS . '.status&amp;search=closed" title="' . PHPST_TITLECLO . '">' . PHPST_TITLECLO .'</a>
                    </td>
                    <td>
                      <a href="' . $url . 'page=browsetickets&amp;ticket_type=my&amp;field=' . DB_PREFIX_TICKETS . '.status&amp;search=open" title="' . PHPST_TITLEOPE . '">' . PHPST_TITLEOPE .'</a> |
                      <a href="' . $url . 'page=browsetickets&amp;ticket_type=my&amp;field=' . DB_PREFIX_TICKETS . '.status&amp;search=closed" title="' . PHPST_TITLECLO . '">' . PHPST_TITLECLO .'</a> |
                      <a href="' . $url . 'page=newticket" title="' . PHPST_TITLEREQ . '">' . PHPST_TITLEREQ .'</a>
                    </td>
                  </tr>
                </table>';
        return $string;
    }

    /**
    * Redirects to this GUI's home page.
    *
    * @access public
    * @return string
    * @param array $request
    */
    public function buildHome($request) {
        return $this->buildBrowsetickets($request);
    }

    /**
    * Builds the viewticket page (view tickets).
    *
    * @access public
    * @return string
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
    * Builds the createuser page.
    *
    * @access public
    * @return string
    * @param array $request
    */
    public function buildCreateuser($request) {
        return $this->buildHome($request);
    }
}
?>