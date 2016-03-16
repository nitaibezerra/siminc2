<?php
/**
 * @category  Static class
 * @package   Common
 * @author    Ian Warner <iwarner@triangle-solutions.com>
 * @copyright (C) 2005 Triangle Solutions Ltd
 * @version   SVN: $Id: static.datetime.php 3 2005-12-13 01:34:35Z nicolas $
 * @link      http://www.wowstats.co.uk/
 * @since     File available since Release 1.1.1.1
 * \\||
 */

// TODO
//

// CHANGE LOG
//

class PHPST_DateTime {


    /**
     * Formats a ??
     *
     * @param string $time A mysql timestamp
     * @return string
     */
    public static function format_mysql_datetime($time)
    {
        $datetime = explode(' ', $time);
        $date     = explode('-', $datetime['0']);
        $time     = explode(':', $datetime['1']);

        $time = $date['2'] . '/' . $date['1'] . '/' . $date['0'] . '&nbsp;' . $time['0'] . ':' . $time['1'];
        return $time;
    }


    /**
     * Enter description here...
     *
     * @return float
     */
    public static function getmicrotime()
    {
        list($usec, $sec) = explode(' ', microtime());
        return ((float)$usec + (float)$sec);
    }


    /**
     * Returns the difference between two UNIX timestamps
     *
     * @param int $start Start timestamp
     * @param int $end End timestamp
     * @return int Difference between start and end timestamps
     */
    public static function timing($start, $end)
    {
        $time = round($end - $start, 3);
        return $time;
    }


    /**
     * Returns the current time in microseconds
     *
     * @return int
     */
    public static function start_time()
    {
        $time_start = microtime(true);
        return $time_start;
    }


    /**
     * Returns the time elapsed between a start microtime and
     * the current microtime. Used for benchmarking.
     *
     * @param int $time_start A microtime
     * @return string Formatted message showing time elapsed
     */
    public static function end_time($time_start)
    {
        $time_end = microtime(true);
        $time     = $time_end - $time_start;

        $msg = '<br /><div align="center">Produced in ' . number_format($time, '5') . ' seconds</div><br />';

        return $msg;
    }


    /**
     * Formats a mysql timestamp into a string of type dd/mm/yyyy [h:m:s].
     *
     * @param int $time MYSQL timestamp
     * @param boolean $show_time If turned on, shows time. Defaults to true.
     * @return string Formatted date
     */
    public static function format_mysql_timestamp($time, $show_time = true)
    {
        $year   = substr($time, '0', '4');
        $month  = substr($time, '4', '2');
        $day    = substr($time, '6', '2');
        $hour   = substr($time, '8', '2');
        $minute = substr($time, '10', '2');

        if (!$show_time) {
            $time = $day . '/' . $month . '/' . $year;
        } else {
            $time = $day . '/' . $month . '/' . $year . ' ' . $hour . ':' . $minute;
        }

        return $time;
    }


    /**
     * Formats a UNIX timestamp into a MySQL timestamp or a different format.
     *
     * @param timestamp $int
     * @param string $format (defaults to mysql timestamp format). [UK or US]
     * @return string MySQL timestamp (default) or given format (UK or US)
     */
    public static function unix2mysql($int, $format = 'YmdHis')
    {
        $final_format = '';

        switch ($format) {
            case 'UK' :
                $final_format = 'd/m/Y H:i:s';
                break;
            case 'US' :
                $final_format = 'm/d/Y H:i:s';
                break;
            default :
                $final_format = $format;
        }

        $timestamp = gmdate($final_format, $int);

        // If an invalid format was passed, return the current time

        if ($timestamp == -1) {
            return time();
        } else {
            return $timestamp;
        }
    }


    /**
     * Comverts a MySQL timestamp or a formatted date into a UNIX timestamp.
     *
     * @param string $string MySQL timestamp or formatted date.
     * @param string $format Defaults to MySQL timestamp format, others are UK and US
     * @return int UNIX timestamp
     */
    public static function mysql2unix($string, $format = 'YmdHis')
    {
        $string = eregi_replace(' ', '', $string);
        $string = eregi_replace('-', '', $string);
        $string = eregi_replace(':', '', $string);
        $string = eregi_replace('/', '', $string);

        switch ($format) {
            case 'UK' :
                $year  = substr($string, '4', '4');
                $month = substr($string, '2', '2');
                $day   = substr($string, '0', '2');
                break;
            case 'US' :
                $year  = substr($string, '4', '4');
                $month = substr($string, '0', '2');
                $day   = substr($string, '2', '2');
            default :
                $year  = substr($string, '0', '4');
                $month = substr($string, '4', '2');
                $day   = substr($string, '6', '2');
        }

        $hour   = substr($string, '8', '2');
        $minute = substr($string, '10', '2');
        $second = substr($string, '12', '2');

        $timestamp = gmmktime($hour, $minute, $second, $month, $day, $year);

        // If an invalid format was passed, return the current time
        if ($timestamp == -1) {
            return time();
        } else {
            return $timestamp;
        }
    }


    /**
     * Returns an array with two timestamps, one for
     * the start day of the month and one for the end day.
     *
     * @param int $month Month in the year (0-11)
     * @param int $year Year (####)
     * @return array Array with start and end timestamp for the given month and year.
     */
    public static function get_month_stamp($month, $year) {

        $end_day = date('d', gmmktime(0, 0, 0, $month + 1, 0, $year));

        // Make time stamps

        $start_stamp = gmmktime(0, 0, 0, $month, 1, $year);
        $end_stamp   = gmmktime(23, 59, 59, $month, $end_day, $year);

        // Make array for return

        $month_array = array('start' => $start_stamp, 'end' => $end_stamp);
        $return_this['range'] = $month_array;
        $return_this['days']  = $end_day;

        return $return_this;
    }


    /**
     * Formats a timestamp according to predefined format.
     * Allowed formats are :
     *  1. UK : dd/mm/yyyy hh:mm:ss
     *  2. US : mm-dd-yyyy hh:mm:ss
     *  3. international : yyyy-mm-dd
     *
     * @param int $ts
     * @param string $format
     * @return string Formatted date
     */
    public static function format_timestamp ($ts, $format = 'UK') {
        $UK = 'd/m/Y H:i:s';
        $US = 'm-d-Y H:i:s';
        $international = 'Y-m-d';

        return gmdate($$format, $ts);
    }


    /**
     * Returns an option list of days, months or years
     *
     * @param string day - month or year string to identity what to show
     * @return string Formatted option elements
     */
    public static function get_date_options($start = 1, $end = 32)
    {
        $options = '';

        for ($i = $start; $i < $end; $i++) {

            $options .= '<option value="' . $i . '"';

            if (isset($day) && $day == $i) {
                $options .= ' selected="selected" ';
            }

            $options .= '>' . $i . '</options>';
        }

        return $options;
    }
}
?>