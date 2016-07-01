<?php
 /**
 * File containing the EmailTemplateParser class.
 * This class depends on the Mail PEAR package, usually part of the
 * basic installation of PHP. There may be a need to configure php.ini so that
 * it knows where to find the PEAR packages.
 *
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version SVN: $Id: static.EmailTemplateParser.php 4 2005-12-13 01:47:15Z nicolas $
 * @since File available since Release 1.1.1.1
 * \\||
 */

/**
 * This static function class serves the purpose of gathering addresses and
 * creating bodies for the different emails to be sent by the host application.
 * Hence its methods will vary according to the needs of the application.
 *
 * @static
 * @access public
 * @package PHPSupportTicket
 */
class PHPST_EmailTemplateParser{

    /**
    * Returns a simpleXML object based on an email template in XML format, from a file.
    *
    * @static
    * @access public
    * @param string $file
    * @return array simpleXML object with parsed template XML data
    */
    public static function getEmailTemplate($file) {
        $string = '';
        if (!($fp = fopen($file, "r"))) {
           die("could not open XML input");
        }

        while ($data = fread($fp, 4096)) {
           $string .= $data;
        }

        return simplexml_load_string($string);
    }

    /**
    * Opens the requested file and replace the existing body by the
    * provided body. In effect, this updates the 'template' part of the
    * xml document.
    *
    */
}
?>