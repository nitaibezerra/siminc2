<?php
/**
 * @category  Class
 * @package   Image GUI
 * @author    Ian Warner, <iwarner@triangle-solutions.com>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version   SVN: $Id: class.image.gui.php 2 2005-12-13 01:12:53Z nicolas $
 * @since     File available since Release 1.1.1.1
 * \\||
 */


/**
* Abstraction for icon set within site
*
* @abstract
* @package PHPST_IconGUI
*/

abstract class PHPST_ImageGUI {


    /**
    * getImage
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

    function getImage($path,           // Path to the image
                      $text,           // Test to display in the ALT and TITLE tags
                      $width  = false, // Width of the image - default false
                      $height = false, // Height of the images - default false
                      $style  = false, // any style elements that need to be included
                      $hyper  = false, // Hyperlink on the image - default false
                      $target = false  // Whether the link should open in a new window or not
                     )
    {
        $return = '';

        $width  = ($width) ? 'width="' . $width . '"' : '';
        $height = ($height) ? 'height="' . $height . '"' : '';
        $target = ($target == '1') ? 'target="_blank"' : '';

        if (!$style) {
            $style = '';
        }

        if ($hyper != '0') {
            $return .= '<a href="' . $hyper . '" ' . $target . '>';
        }

        $return .= '<img src="' . $path . '" ' . $width . ' ' . $height . ' title="' . $text . '" alt="' . $text . '" ' . $style .'  />';

        if ($hyper != '0') {
            $return .= '</a>';
        }

        return $return;
    }
}

function UseColor()
{
    $trcolor1 = '#FFFFFF';
    $trcolor2 = '#EEEEEE';
    static $colorvalue;

    if ($colorvalue == $trcolor1) {
        $colorvalue = $trcolor2;
    } else {
        $colorvalue = $trcolor1;
    }

    return($colorvalue);
}


?>