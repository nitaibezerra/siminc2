<?php
/**
 * File containing the LoginGUI class.
 *
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version SVN: $Id: class.LoginGUI.php 4 2005-12-13 01:47:15Z nicolas $
 * @since File available since Release 1.1.1.1
 * \\||
 */

/**
* user defined includes.
* @include
*/
require_once_check(PHPST_PATH . 'classes/GUI/abstract.GUI.php');

/**
 * LoginGUI is the GUI that is presented to the User on first visit and
 * when he logs out. It displays either a login screen or a register screen.
 *
 * @access public
 * @package PHPSupportTicket
 */
class PHPST_LoginGUI extends PHPST_GUI{
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
        $this->buildHeader("Login");
        $this->buildBody($request);
        $this->buildFooter();
    }

    /**
    * If this GUI is updated and has either 'login' or 'register' in its request
    * variable, it means an error occurred, and we must display it. Otherwise,
    * the user is switching between login and register screens.
    *
    * @access public
    * @param array $request
    * @return void
    */
    public function update($request) {
        $this->setMsg('');
        $this->resetHTML();

        if (isset($request['login'])) {
            $this->setMsg(PHPST_LOGINPAGE);
        } elseif (isset($request['register']) && isset($request['formdata'])) {
            $errors = PHPST_FormHandler::register_createuser($request);
            $this->setMsg($errors);
        } else {
            $this->setPage(@$request['page']);
        }

        $this->buildHeader($this->getPageName());
        $this->buildBody($request);
        $this->buildFooter();
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
    * Builds a HTML user registration table.
    *
    * @access public
    * @param array $request
    * @return string
    */
    public function buildRegister($request) {
        return parent::buildCreateuser($request, 'Client');
    }

    /**
    * Builds the login page.
    *
    * @access public
    * @param string $string
    * @return void
    */
    public function buildLogin() {
        $url = $this-> getAbsoluteUrl() ;
        $string = $this->buildNavigation();
        $string .= '
            <form action="' . $url . '" method="post">
            <input type="hidden" name="login" value="login" />
            <input type="hidden" name="page" value="login" />
            <table width="75%" cellspacing="1" cellpadding="1" class="boxborder bodycolour" align="center">
              <tr><td colspan="2">' . $this->getMsg() . '</td></tr>
              <tr>
                <td class="text" align="center"><br />
                  ' . PHPST_USERNAME . ' <input name="username" size="20"
                  /> ' . PHPST_PASSWORD . ' <input type="password" name="password" size="20"
                  /> <input type="submit" name="form" value="' . PHPST_LOGIN . '" /><br /><br />
                </td>
              </tr>
            </table>
            </form>';
        $this->addHTML($string);
    }

    /**
    * Redirects to this GUI's home page.
    *
    * @access public
    * @return string
    * @param array $request
    */
    public function buildHome($request) {
        return $this->buildLogin();
    }

    /**
    * Builds the navigation HTML.
    *
    * @access public
    * @param array $request
    * @return string
    */
    public function buildNavigation() {
        $url = $this-> getAbsoluteUrl() ;
        $title = PHPST_LOGIN;
        $button = PHPST_REGISTER;
        $link = 'register';

        if ($this->getPage() == 'register') {
            $title = PHPST_REGISTER;
            $button = PHPST_LOGIN;
            $link = 'login';
        }

        $string = '
                <table class="tbl">
                  <tr>
                    <td class="title"><a href="' . $url . 'page=browsetickets">Support Tickets Manager</a></td>
                    <td width="70">';
        
        $help = "Submit and answer support tickets with the help of this interface.";
        $string .= PHPST_IconGUI::getIcon('help', '16', 'images/icons/', 0, 0, 'style="cursor:pointer"'
            . 'onmouseover="return escape(\'' . $help . '\')"');
        $string .= PHPST_IconGUI::getIcon('arrow-down', '16', 'images/icons/', 0, 0, 'onClick="blocking(\'showLoginPage\', \'block\'); return false;" style="cursor:pointer"');
        $string .= '
                    </td>
                  </tr>
                </table>
                <div id="showLoginPage">
                <table class="tbl">
                  <tr>
                    <th class="boxborder text titlebarcolour">' . $title . '</th>
                    <td class="boxborder list-menu" width="15%"><a href="' . $url . 'page=' . $link . '" title="' . $button . '">' . $button . '</a></td>
                    <td class="boxborder list-menu" width="15%"><a href="' . $url . 'page=resend" title="Resend Details">' . PHPST_RESEND . '</a></td>
                    <!--<td class="boxborder list-menu" width="10%"><a href="javascript:popwindow(\'help.php#userpage\',\'top=150,left=300,width=400,height=400,buttons=no,scrollbars=YES,location=no,menubar=no,resizable=no,status=no,directories=no,toolbar=no\')" title="Help popup">Help</a></td>-->
                  </tr>
                </table>';
        return $string;

    }
}
?>