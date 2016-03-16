<?php
/**
 * File containing the GUIFactory class.
 *
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version SVN: $Id: factory.GUI.php 4 2005-12-13 01:47:15Z nicolas $
 * @since File available since Release 1.1.1.1
 * \\||
 */

/**
* user defined includes.
* @include
*/

require_once_check(PHPST_PATH . 'classes/GUI/class.AdminGUI.php');
require_once_check(PHPST_PATH . 'classes/GUI/class.ClientGUI.php');
require_once_check(PHPST_PATH . 'classes/GUI/class.ClientGUI.php');
require_once_check(PHPST_PATH . 'classes/GUI/class.ModGUI.php');
require_once_check(PHPST_PATH . 'classes/GUI/class.LoginGUI.php');
require_once_check(PHPST_PATH . 'classes/class.User.php');
require_once_check(PHPST_PATH . 'classes/static/static.MailMan.php');

/**
 * Creates and returns an instance of the appropriate GUI page, based on session and request data
 *
 * @access public
 * @package PHPSupportTicket
 */
class PHPST_GUIFactory {
    /// --- FIELDS ---

    public static $absoluteURL;

    // --- METHODS ---
    /**
    * Returns an instance of a concrete implementation of the GUI class.
    *
    * @access static
    * @param string $admin_type "Client", "Admin" or "Mod"
    * @param array $_REQUEST
    * @return object
    */
    public static function getInstance($admin_type, $request) {
        $GUI = 'PHPST_' . $admin_type . 'GUI';
        return new $GUI(PHPST_GUIFactory::$absoluteURL, $request);
    }
}
?>