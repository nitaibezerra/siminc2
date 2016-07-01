<?php
/* vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4: */
//
// +------------------------------------------------------------------------+
// | G1 - MVC Framework for PHP5                                            |
// | Copyright (c) 2005 The GCoders Group                                   |
// | All Right Reserved                                                     |
// +------------------------------------------------------------------------+
// | The contents of this file are subject to the Mozilla Public License    |
// | Version 1.1 (the "MPL"); you may not use this file except in           |
// | compliance with the License. You may obtain a copy of the License at   |
// | http://www.mozilla.org/MPL/                                            |
// |                                                                        |
// | Software distributed under the License is distributed on an "AS IS"    |
// | basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See    |
// | the License for the specific language governing rights and limitations |
// | under the License.                                                     |
// |                                                                        |
// | The Original Code is The GCoders Group.                                |
// | The Initial Developer of the Original Code is:                         |
// |     Douglas Gontijo <douglas@gcoders.net>                              |
// |                                                                        |
// | Alternatively, the contents of this file may be used under the terms   |
// | of the BSD License (BSD License), in which case the provisions of BSD  |
// | License are applicable instead of those above.                         |
// |                                                                        |
// | If you wish to allow use of your version of this file only under the   |
// | terms of the BSD License and not to allow others to use your version   |
// | of this file under the MPL, indicate your decision by deleting the     |
// | provisions above and replace them with the notice and other provisions |
// | required by the BSD License                                            |
// |                                                                        |
// | If you do not delete the provisions above, a recipient may use your    |
// | version of this file under either the MPL or the BSD License.          |
// +------------------------------------------------------------------------+
//
// $Id$
//


/**
 * @package net.gcoders.base
 * @class G1
 */

error_reporting(E_ALL);
require_once 'Object.php';
/**
 * @class G1
 *
 *
 * @version $Revision$
 * @author  Douglas Gontijo <douglas@gcoders.net>
 */
final class G1 extends Object {
    //-------------------------------------------------------------- constants


    //------------------------------------------------------------- properties
    /**
     * 
     */
    static private $_init = false;

    /**
     * 
     */
    static private $_processStart;


    //----------------------------------------------------------------- public
    /**
     * 
     */
    static public function init()
    {
        self::$_processStart  = microtime(true);

        if (!headers_sent())
            header('content-type: text/plain; charset=utf-8');

        // Lemme try to handle this shit :)
        set_exception_handler(array('G1', 'exceptionHandler'));
        set_error_handler(array('G1', 'errorHandler'));

        if (!defined('__ROOT__')) {
            //throw new Exception('nhá');
        }

        register_shutdown_function(array('G1', 'applicationShutdown2'));

        self::process();
    }


    /**
     * 
     */
    static public function exceptionHandler(Exception $e)
    {
        //if (DB::hasTransaction()) {
        //    DB::rollbackAll();
        //}

        echo $e;

        exit -1;
    }


    /**
     * 
     */
    static public function errorHandler($code, $function, $file, $line, $params)
    {
        static $messages = array(E_ERROR             => 'fatal error',
                                 E_PARSE             => 'parse',
                                 E_NOTICE            => 'notice',
                                 E_STRICT            => 'strict standards',
                                 E_WARNING           => 'warning',
                                 E_USER_ERROR        => 'user error (deprecated)',
                                 E_USER_NOTICE       => 'user notice (deprecated)',
                                 E_USER_WARNING      => 'user warning (deprecated)',
                                 E_RECOVERABLE_ERROR => 'recoverable error');

        $trace = debug_backtrace();
        array_shift($trace);

        $message  = preg_replace('/(.+)( \[\<a.+\]\: )/', '', $function);
        $function = preg_replace('/(.+)( \[\<a.+\])(.*)/', '$1', $function);

        if (array_key_exists($code, $messages))
            $errorType = 'PHP ' . $messages[$code];
        else
            $errorType = 'Unknown PHP error';

        printf("%s with message '%s' in %s:%d\nStack trace:\n", $errorType,
                                                                $message,
                                                                $file,
                                                                $line);

        while (list($i, $traceItem) = each($trace)) {
            if (array_key_exists('class', $traceItem)) {
                $class = $traceItem['class'];
                $type  = $traceItem['type'];
            } else {
                $class = '';
                $type  = '';
            }

            printf("#%d %s(%d): %s%s%s()\n" , $i,
                                              $traceItem['file'],
                                              $traceItem['line'],
                                              $class,
                                              $type,
                                              $traceItem['function']);
        }

        echo "#" , sizeof($trace) , " {main}\n";

        exit -1;
    }


    /**
     * 
     */
    static public function process()
    {
    }


    /**
     * 
     */
    static public function applicationShutdown()
    {
        self::$_processStart = microtime(true) - self::$_processStart;
    }


    //-------------------------------------------------------------- protected


    //---------------------------------------------------------------- private


}
G1::init();





