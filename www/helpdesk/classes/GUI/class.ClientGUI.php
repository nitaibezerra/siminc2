<?php
/**
 * File containing the AdminGUI class.
 *
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version SVN: $Id: class.ClientGUI.php 4 2005-12-13 01:47:15Z nicolas $
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
class PHPST_ClientGUI extends PHPST_GUI{
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
            $this->addHTML($this->buildHome());
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
        // die(print_r($this->user))
        $tickets = PHPST_FormHandler::get_tickets('Client', $request, $this->user->getId());
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
        // Note this is not a Ticket object, but an array containing a Ticket and a user_ID
        $ticket = PHPST_FormHandler::get_ticket($request);
        return parent::buildViewticket($request, $ticket);
    }
}
?>