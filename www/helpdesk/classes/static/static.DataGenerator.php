<?php

/**
 * @category Class
 * @package PHPSupportTickets
 * @author Ian Warner, <iwarner@triangle-solutions.com>
 * @author Nicolas Connault, <nick@connault.com.au>
 * @copyright (C) 2001 Triangle Solutions Ltd
 * @version SVN: $Id: class.AnswerSQLBuilder.php 4 2005-12-13 01:47:15Z nicolas $
 * @since File available since Release 1.1.1.1
 */

 /*
// Generate dummy data for users
for ($i = 1; $i < 58; $i++) {
    $fname = DataGenerator::getName("M");
    $lname = DataGenerator::getName("L");
    $name = $fname . ' ' . $lname;
    $username = strtolower(substr($fname, 0, 4) . substr($lname, 0, 4));
    $email = $username . '@hotmail.com';
    $query = "UPDATE users SET name = '$name',
            username = '$username', email = '$email' WHERE ID = $i";
    $rs = &$conn->Execute($query);
}
*/
// user defined includes

// user defined constants

/**
 * Methods for generating sample data.
 *
 * @access public
 * @package contactmanagement
 */
class PHPST_DataGenerator{
    // --- ATTRIBUTES ---

    public static $maleFirstNamesFile = 'include/males.txt';
    public static $femaleFirstNamesFile = 'include/females.txt';
    public static $lastNamesFile = 'include/lastnames.txt';

    // --- OPERATIONS ---
    /**
     * Generates a first name.
     *
     * @static
     * @access public
     * @param string $type (M)ales, (F)emales or (L)ast names
     * @param int $common The higher the number, the less common the name
     * @return string A generated first name of the given gender
     */
    public static function getName($type = "M", $common = 100) {
        $handle = null;
        // Open file

        switch ($type) {
            case "L" :
                $handle = fopen(DataGenerator::$lastNamesFile, "r");
                break;
            case "F" :
                $handle = fopen(DataGenerator::$femaleFirstNamesFile, "r");
                break;
            default :
                $handle = fopen(DataGenerator::$maleFirstNamesFile, "r");
                break;
        }

        // Read line
        $rand = rand(0, $common);
        $row = 0;
        while (!feof($handle)) {
            $buffer = fgets($handle, 4096);
            if ($rand == $row) {
                fclose($handle);
                return $buffer;
            } else {
                $row++;
            }
        }
    }

    /**
    * Generate a random, secure password.
    *
    * @static
    * @access public
    *
    * @param int $length
    * @param string $chars "Alpha", "Numeric", "Alphanumeric" or "Any"
    * @return string password
    */
    public static function getPassword($length, $chars = "Alphanumeric") {
        $rand = mt_rand();
        $pass = MD5($rand);
        switch ($chars){
        	case "Alpha":
        		$pass = ereg_replace("[0-9 \/\.]", "", crypt($pass, substr($pass, 0, 2)));
        		break;
        	case "Numeric":
        		$pass = ereg_replace("[a-zA-z]", "", $pass);
        		break;
            case "Any":
        		$pass = crypt($pass);
        		break;
        	default:
        		;
        }

        $pass = substr($pass, 4, $length);
        while (strlen($pass) < $length) {
            $pass .= substr(DataGenerator::getPassword(5, $chars), 4, 5);
        }
        return substr($pass, 0, $length);
    }

    /**
    * Generates a random date in a given format, around a certain average age.
    *
    * @static
    * @access public
    * @param string $format
    * @param int $average Average age
    * @return string date
    */
    public static function getRandDate($average, $format = "d/m/Y") {
        $rand_day = mt_rand(1, 28);
        $rand_month = mt_rand(1, 12);
        $min_year = gmdate("Y", time()) - ($average - 20);
        $max_year = gmdate("Y", time()) - ($average + 20);
        $rand_year = mt_rand($min_year, $max_year);

        return sprintf("%02d-%02d-%04d", $rand_year, $rand_month, $rand_day);
    }

    public static function getPhone() {}
    public static function getZip() {}
    public static function getURL() {}
    public static function getCountry() {}
}



?>