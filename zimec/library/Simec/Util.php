<?php

defined('MSG_ORIENTATION') || define('MSG_ORIENTATION', 'orientation' );
defined('MSG_SUCCESS') || define('MSG_SUCCESS', 'success' );
defined('MSG_ERROR') || define('MSG_ERROR', 'error' );
defined('MSG_ALERT') || define('MSG_ALERT', 'alert' );

class Simec_Util
{
    static protected $instance;
    const DIA = 1;
    const HORA = 2;
    const MINUTOS = 3;
    
    static public function getInstance()
    {
        return self::$instance;
    }
    
    static public function getSession($nome, $namespace = 'zimec')
    {
        $o = new Zend_Session_Namespace($namespace);
        return $o->$nome;
    }

    static public function setSession($nome, $valor, $namespace = 'zimec')
    {
        $o = new Zend_Session_Namespace($namespace);
        $o->$nome = $valor;
        return self::getInstance();
    }

    static public function clear($nome, $namespace = 'zimec')
    {
        $o = new Zend_Session_Namespace($namespace);
        unset($o->$nome);
        return self::getInstance();
    }

    static public function utf8_decode_recursive($array)
    {
    	$utf8_array = array();
    
    	foreach ($array as $key => $val)
    	{
    		if (is_array($val))
    			$utf8_array[$key] = self::utf8_decode_recursive($val);
    		else
    			$utf8_array[$key] = utf8_decode($val);
    	}
    	return $utf8_array;
    }
    
    static public function utf8_encode_recursive($array)
    {
    	$utf8_array = array();
    
    	foreach ($array as $key => $val)
    	{
    		if (is_array($val))
    			$utf8_array[$key] = self::utf8_encode_recursive($val);
    		else
    			$utf8_array[$key] = utf8_encode($val);
    	}
    	return $utf8_array;
    }
    
    static public function import($classPath)
    {
    	$basePaths = array(APPLICATION_PATH . DIRECTORY_SEPARATOR . 'modules' . DIRECTORY_SEPARATOR, APPLICATION_PATH . DIRECTORY_SEPARATOR);
    
    	if ( preg_match( '/(\.\*)$/', $classPath ) ) {
    		$importFilePath = substr( $classPath, 0, strlen( $classPath ) - 2 );
    		$importFilePath = str_replace( ".", DIRECTORY_SEPARATOR, $importFilePath );
    		
    		foreach ($basePath as $basePath)
    		{
	    		$d = dir( $basePath . $importFilePath );
	    
	    		while ( false !== ( $file = $d->read() ) ) {
	    			/* Reject parent, current directories and sub directories */
	    			if ( ( $file == '.' ) || ( $file == '..' ) || ( is_dir( $d->path . DIRECTORY_SEPARATOR . $file ) ) ) {
	    				continue;
	    			}
	    			require_once ( $basePath . $importFilePath . DIRECTORY_SEPARATOR . $file );
	    		}
    		}
    	} else {
    		/* If a single file is specified */
    		$importFile = str_replace( ".", DIRECTORY_SEPARATOR, $classPath ) . ".php";
    		$ruleModelPart = substr($importFile, 0, strpos($importFile, 'models' . DIRECTORY_SEPARATOR));
    		$filePath = str_replace( $ruleModelPart . 'models' . DIRECTORY_SEPARATOR, 'models' . DIRECTORY_SEPARATOR . $ruleModelPart, $importFile);

    		foreach ($basePaths as $basePath)
    		{
    			if (is_readable($basePath . $filePath))
    				require_once ( $basePath . $filePath );
    		}
    	}
    }

    static public function formatarData($data, $formato = null)
    {
        if($data){
            $date = new Zend_Date($data);

            if($formato){
                return $date->get($formato);
            }

            if('/' == substr($data, 2, 1)){
                $formato = strlen($data) > 10 ? 'YYYY-MM-dd HH:mm:ss' : 'YYYY-MM-dd';
                return $date->get($formato);
            }

            $formato = strlen($data) > 10 ? 'dd/MM/YYYY HH:mm:ss' : 'dd/MM/YYYY';
            return $date->get($formato);
        }
        return null;
    }

    static public function comparaData($firstDate, $secondDate, $formato = 'dd/MM/YYYY',$tipoRetorno = self::DIA)
    {
        $first = new Zend_Date($firstDate, $formato);
        $last = new Zend_Date($secondDate, $formato);
        $diff = $last->sub($first)->toValue();
        
        switch($tipoRetorno){
            case self::DIA:
                return floor($diff/60/60/24);
            case self::HORA:
                return floor($diff/60/60);
            case self::MINUTOS:
                return floor($diff/60);
        }
    }
    
    static public function encode($string)
    {
    	return base64_encode($string);
    }
    
    static public function decode($string)
    {
    	return base64_decode($string);
    }
}

function x($vars)
{
	echo "<div id='x'>";
	foreach (func_get_args() as $idx => $arg) {
		echo "<b><u>ARG[$idx]</u></b><br><pre>";
		print_r( $arg );
		echo "</pre>";
	}
	echo "</div><br><br>";
	flush();
}

function xd($vars)
{
	x($vars);
	die;
}

function import($classPath)
{
	Simec_Util::import($classPath);
}