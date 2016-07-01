<?php
/**
 * $Id: Log.php 77306 2014-03-17 17:36:12Z maykelbraz $
 */

/**
 * @see Simec_SoapClient_Log_LoggerDb
 */
require_once(dirname(__FILE__) . '/Log/LoggerDb.php');

/**
 * Classe de instanciaзгo de loggers - Factory.
 * 
 * @see Simec_SoapClient_Log_Logger
 * @see Simec_SoapClient_Log_LoggerDb
 */
class Simec_SoapClient_Log
{
    private static $instance;
    private static $availableLoggers = array('db', /*'file', 'screen', 'email'*/);

    private function __construct()
    {
    }

    /**
     * Cria o nome da classe de logger com base no tipo de log.
     *
     * @param string $type Tipo de log que serб gravado.
     * @return string Nome da classe de logger.
     * @static
     */
    static protected function getLoggerClass($type)
    {
        $loggerClassName = ucfirst(strtolower($type));
        return "Simec_SoapClient_Log_Logger{$loggerClassName}";
    }

    public static function getLogger($type)
    {
        if (!isset(self::$instance)) {
            if (!in_array($type, self::$availableLoggers)) {
                throw new Exception('Tipo invбlido de logger. Tipos disponнveis: db');
            }

            $loggerClassName = self::getLoggerClass($type);
            self::$instance = new $loggerClassName();
        }
        return self::$instance;
    }
}
