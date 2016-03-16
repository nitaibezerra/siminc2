<?php
/**
 * File containing the Validate static class.
 *
 * @category Utilities
 * @package PHPShop
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version SVN: $Id: static.Validate.php 4 2005-12-13 01:47:15Z nicolas $
 * @since File available since Release 1.1.1.1
 * \\||
 */

/**
 * Validate
 *
 * Tools for validating user input
 *
 * @access public
 * @package PHPShop
 */
if (!class_exists("Validate")) {
class PHPST_Validate {
    /**
     *
     * @public string url_pattern Regular Expression matching a correctly formatted URL.
     */

    public static $url_pattern = '^(http|https|ftp)\://([a-zA-Z0-9\.\-]+(\:[a-zA-Z0-9\.&%\$\-]+)*@)*((25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9])\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[0-9])|localhost|([a-zA-Z0-9\-]+\.)*[a-zA-Z0-9\-]+\.(com|edu|gov|int|mil|net|org|biz|arpa|info|name|pro|aero|coop|museum|[a-zA-Z]{2}))(\:[0-9]+)*(/($|[a-zA-Z0-9\.\,\?\'\\\+&%\$#\=~_\-]+))*$';
    /**
     *
     * @public string email_pattern Regular Expression matching a correctly formatted email address
     */

    public static $email_pattern = '^[a-zA-Z0-9_\.\-]+@[a-zA-Z0-9\-]+\.[a-zA-Z0-9\-\.]+$';
    /**
     * Accepts phone number in both local format
     * (eg. 02 1234 5678 or 123 123 4567) or international format
     * (eg. +61 (0) 2 1234 5678 or +1 123 123 4567).
     * It also accepts an optional extention of up to five
     * digits prefixed by x or ext (eg. 123 123 4567 x89)
     *
     * @see http://javascript.about.com/library/blre.htm
     * @public string phone_pattern Regular Expression matching a correctly formatted phone number
     */
    public static $phone_pattern = '/^((\+\d{1,3}(-| )?\(?\d\)?(-| )?\d{1,3})|(\(?\d{2,3}\)?))(-| )?(\d{3,4})(-| )?(\d{4})(( x| ext)\d{1,5}){0,1}$/';
    /**
     *
     * @public string zip_pattern Regular Expression matching a correctly formatted zip number
     */
    public static $zip_pattern = '[0-9]{4,5}';
    /**
     *
     * @public string alphanumeric_pattern
     */
    public static $alphanumeric_pattern = '^[a-zA-Z0-9]*$';
    /**
     *
     * @public string name_pattern
     */
    public static $name_pattern = '^[a-zA-Z0-9\-]*$';
    /**
     *
     * @public string password_pattern Regular Expression matching a correctly formatted password
     */
    public static $password_pattern = '^[a-zA-Z0-9]{6,15}$';
    /**
     *
     * @public constant_pattern Constant message name pattern
     */
    public static $constant_pattern = '^[A-Z]+[A-Z \_ 0-9]+$';

    /**
     * in the format dd/mm/yyyy or dd-mm-yyyy
     *
     * @public date_pattern Date pattern
     */
    public static $date_pattern = '^([0-9]){2}(\/|-){1}([0-9]){2}(\/|-)([0-9]){4}$';

    /**
     *
     * @todo -o"Nicolas Connault" -c"Validate::alpha_pattern" -p3 Upgrade Regexp to take > 1 word.
     * @public alpha_pattern Alphabet only
     */
    public static $alpha_pattern = '^([a-zA-Z\-]+)*$';

    /**
     *
     * @public title_pattern Title pattern
     */
    public static $title_pattern = '^(Mr\.|Mrs\.|Ms\.|Dr\.|Prof\.){1}$';

    /**
     *
     * @public street_pattern Street pattern
     */
    public static $street_pattern = '/^[0-9a-zA-Z\-\,\.\s\/]*$/';

    /* --- NO CONSTRUCTOR: STATIC CLASS --- */

    /**
     * Checks a URL.
     *
     * @param string $string
     * @param string $field The field being validated
     * @return boolean TRUE if successful, array with error message and error field if error occurs
     */
    public static function checkUrl($url, $field = "url") {
        if (!eregi(self::$url_pattern, $url)) {
            return array("message" => VALIDATE_INVALID_URL, "field" => $field);
        } else {
            return true;
        }
    }

    /**
     * Checks an email address.
     *
     * @param string $string
     * @param string $field The field being validated
     * @return boolean TRUE if successful, array with error message and error field if error occurs
     */
    public static function checkEmail($email, $field = "email") {
        if (!eregi(self::$email_pattern, $email)) {
            return array("message" => VALIDATE_INVALID_EMAIL, "field" => $field);
        } else {
            return true;
        }
    }

    /**
     * Checks an alphanumeric string
     *
     * @param string $string
     * @param string $field The field being validated
     * @return boolean TRUE if successful, array with error message and error field if error occurs
     */
    public static function checkAlphanumeric($string, $field = "username") {
        $string = ereg_replace(" ", "", $string);
        if (!eregi(self::$alphanumeric_pattern, $string)) {
            return array("message" => VALIDATE_INVALID_ALPHANUMERIC,
                "field" => $field);
        } else {
            return true;
        }
    }

    /**
     * Checks a name string
     *
     * @param string $string
     * @param string $field The field being validated
     * @return boolean TRUE if successful, array with error message and error field if error occurs
     */
    public static function checkName($string, $field = "username") {
        $string = ereg_replace(" ", "", $string);
        if (!eregi(self::$name_pattern, $string)) {
            return array("message" => VALIDATE_INVALID_NAME,
                "field" => $field);
        } else {
            return true;
        }
    }

    /**
     * Checks a title string
     *
     * @param string $string
     * @param string $field The field being validated
     * @return boolean TRUE if successful, array with error message and error field if error occurs
     */
    public static function checkTitle($string, $field = "title") {
        if (!eregi(self::$title_pattern, $string)) {
            return array("message" => VALIDATE_INVALID_TITLE,
                "field" => $field);
        } else {
            return true;
        }
    }

    /**
     * Checks a street string
     *
     * @param string $string
     * @param string $field The field being validated
     * @return boolean TRUE if successful, array with error message and error field if error occurs
     */
    public static function checkStreet($string, $field = "street") {
        if (preg_match(self::$street_pattern, $string) == 0) {
            return array("message" => VALIDATE_INVALID_STREET,
                "field" => $field);
        } else {
            return true;
        }
    }

    /**
     * Checks a password
     *
     * @param string $string
     * @param string $field The field being validated
     * @return boolean TRUE if successful, array with error message and error field if error occurs
     */
    public static function checkPassword($password, $field = "password") {
        if (!eregi(self::$password_pattern, $password) && $password != "") {
            return array("message" => VALIDATE_INVALID_PASSWORD, "field" => $field);
        } else {
            return true;
        }
    }

    /**
     * Checks a phone number
     *
     * @param string $string
     * @param string $field The field being validated
     * @return boolean TRUE if successful, array with error message and error field if error occurs
     */
    public static function checkPhone($phone, $field = "phone") {
        if (preg_match(self::$phone_pattern, $phone) == 0) {
            return array("message" => VALIDATE_INVALID_PHONE, "field" => $field);
        } else {
            return true;
        }
    }

    /**
     * Checks a zip code
     *
     * @param string $string
     * @param string $field The field being validated
     * @return boolean TRUE if successful, array with error message and error field if error occurs
     */
    public static function checkZip($zip, $field = "zip") {
        if (!eregi(self::$zip_pattern, (int) $zip)) {
            return array("message" => VALIDATE_INVALID_ZIP, "field" => $field);
        } else {
            return true;
        }
    }

    /**
     * Checks a constant value.
     *
     * @param  $constant
     * @param string $field The field being validated
     * @return boolean TRUE if successful, array with error message and error field if error occurs
     */
    public static function checkConstant($constant, $field = "constant") {
        if (!eregi(self::$constant_pattern, $constant)) {
            return array("message" => VALIDATE_INVALID_CONSTANT, "field" => $field);
        } else {
            return true;
        }
    }

    /**
     * Checks a Date of Birth.
     *
     * @param  $constant
     * @param string $field The field being validated
     * @return boolean TRUE if successful, array with error message and error field if error occurs
     */
    public static function checkDate($date, $field = "dob") {
        if (!eregi(self::$date_pattern, $date)) {
            return array("message" => VALIDATE_INVALID_DATE, "field" => $field);
        } else {
            return true;
        }
    }

    /**
     * Checks an alphabetical-only field.
     *
     * @param  $constant
     * @param string $field The field being validated
     * @return boolean TRUE if successful, array with error message and error field if error occurs
     */
    public static function checkAlpha($string, $field = "name") {
        $string = ereg_replace(" ", "", $string);
        if (!eregi(self::$alpha_pattern, $string)) {
            return array("message" => VALIDATE_INVALID_ALPHA, "field" => $field);
        } else {
            return true;
        }
    }
}
}
?>