<?php
/**
 * @category  Class
 * @package   Icon GUI
 * @author    Ian Warner, <iwarner@triangle-solutions.com>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version   SVN: $Id: class.icon.gui.php 2 2005-12-13 01:12:53Z nicolas $
 * @since     File available since Release 1.1.1.1
 * \\||
 */


/**
* Abstraction for icon set within site
*
* @abstract
* @package IconGUI
*/

abstract class PHPST_IconGUI {


    /**
    * Array of accepted icon names
    *
    * @static
    * @access public
    * @var array $icon_array
    */
    public static $icon_array = array('add', 'admin', 'applications', 'arrow-back', 'arrow-down', 'arrow-forward', 'arrow-up', 'calendar',
                                      'cancel', 'chat', 'close', 'computer', 'confirm', 'contacts', 'control-forward',
                                      'control-pause', 'control-play', 'control-reverse', 'control-skip-backward',
                                      'control-skip-forward', 'control-stop', 'copy', 'cut', 'delete', 'disc-media',
                                      'documents', 'edit', 'export', 'favorites-add', 'favorites', 'finance', 'folder-closed',
                                      'folder-open', 'go', 'group', 'hard-disk', 'help', 'history', 'home', 'import', 'info',
                                      'mail', 'movie', 'music', 'network', 'notes', 'paste', 'pictures', 'print', 'redo',
                                      'refresh', 'report', 'save', 'search', 'security-locked', 'security-unlocked',
                                      'shopping-cart', 'stop', 'trash', 'undo', 'update', 'user', 'web', 'zoom-in', 'zoom-out'
                                      );


    /**
    * Array of accepted icon sizes
    *
    * @static
    * @access public
    * @var array $icon_sizes
    */
    public static $icon_sizes = array('16', '24', '32');


    /**
    * getIcon: takes in icon path, and desired icon and size
    *
    * @static
    * @access public
    * @param string $icon_name
    * @param string $icon_size
    * @param string $icon_path
    * @param string $url Empty string if no url
    * @param string $target Empty string if no target
    * @return string
    */
    public static function getIcon($icon_name,
                                   $icon_size = '16',
                                   $icon_path = 'images/icons/',
                                   $url = false,
                                   $target = false,
                                   $extra = false) {

        // Don't echo things from a static class, instead returned a buffered string

        $retstring = '';

        // You can use the 'self' keyword when calling a static class's methods (self::iconCheckPath),
        // or just use the method name

        if (PHPST_IconGUI::iconCheckPath($icon_path) && PHPST_IconGUI::iconInArray($icon_name) && PHPST_IconGUI::iconCorrectSize($icon_size)) {

            if ($url) {

                if (!$target) {
                    $target = '';
                }

                $retstring .=  '<a href="' . $url . '" title="' . $icon_name . '" ' . $target . '>';
            }

            if (!$extra) {
                $extra = '';
            }

            $retstring .=  '<img src="' . $icon_path . $icon_name . '_' . $icon_size . '.gif" title="' . $icon_name . '" alt="' . $icon_name . '" height="' . $icon_size . '" width="' . $icon_size . '" border="0" align="middle" ' . $extra . ' />&nbsp;';

            if ($url) {
                $retstring .= '</a>';
            }
        } else {
            return false;
        }

        // If we make is so far, it means no error has occurred, and we should have a html string ready to return.

        return $retstring;
    }


    /**
    * Function to check that the given path exists
    *
    * @access private
    * @return boolean
    */
    private function iconCheckPath($icon_path) {
        return file_exists($icon_path);
    }


    /**
    * Function to check that the icon exists in the icon array
    *
    * @access private
    * @return boolean
    */
    private function iconInArray($icon_name) {
        return in_array($icon_name, PHPST_IconGUI::$icon_array);
    }


    /**
    * Function to check that the icon size is correct
    *
    * @access private
    * @return boolean
    */
    private function iconCorrectSize($icon_size) {
        return in_array($icon_size, PHPST_IconGUI::$icon_sizes);
    }
}

?>