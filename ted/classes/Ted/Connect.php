<?php
/**
 * Created by PhpStorm.
 * User: lucasgomes
 * Date: 01/10/15
 * Time: 16:53
 */

class Ted_Connect
{
    /**
     * @var cls_banco
     */
    private static $db = null;

    /**
     * @var DBgetInstance
     */
    public static function dbGetInstance()
    {
        if (null === self::$db) {
            self::$db = new cls_banco();
        }

        return self::$db;
    }
} 