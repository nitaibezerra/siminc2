<?php
header('Access-Control-Allow-Origin: *');
header("Cache-Control: no-cache, must-revalidate");
header("Expires: Sat, 26 Jul 1997 05:00:00 GMT");

// Define path to server documento root directory
defined('SERVER_PATH') || define('SERVER_PATH', realpath(dirname('../../')));

// Define path to public directory
defined('PUBLIC_PATH') || define('PUBLIC_PATH', realpath(dirname(__FILE__) . DIRECTORY_SEPARATOR . 'public'));

// Define path to APPLICATION directory
defined('APPLICATION_PATH') || define('APPLICATION_PATH', realpath(dirname(__FILE__) . DIRECTORY_SEPARATOR . 'application'));

// Define path to library directory
defined('LIBRARY_PATH') || define('LIBRARY_PATH', realpath(dirname(__FILE__) . DIRECTORY_SEPARATOR . 'library'));

// Define path to module directory
defined('MODULE_PATH') || define('MODULE_PATH', APPLICATION_PATH . DIRECTORY_SEPARATOR . 'modules' . DIRECTORY_SEPARATOR);

// Define APPLICATION environment
defined('APPLICATION_ENV') || define("APPLICATION_ENV", "staging");

defined('APP_CONFIG_FILE_ZIMEC') || define('APP_CONFIG_FILE_ZIMEC', (getenv('APP_CONFIG_FILE_ZIMEC') ? getenv('APP_CONFIG_FILE_ZIMEC') : APPLICATION_PATH . '/configs/database.ini'));

// Define path to APPLICATION/configs
defined('APPLICATION_CONFIGS') || define('APPLICATION_CONFIGS', APPLICATION_ENV . DIRECTORY_SEPARATOR . 'configs');

// Define theme front-end
defined('SCAFFOLDING_PATH') || define('SCAFFOLDING_PATH', '/tmp/');

set_include_path(
	'D:\\Workspace\\php\\simec\\zimec\\library\\Zend' . PATH_SEPARATOR .
	APPLICATION_PATH . PATH_SEPARATOR .
	LIBRARY_PATH . DIRECTORY_SEPARATOR . PATH_SEPARATOR .
	APPLICATION_PATH . DIRECTORY_SEPARATOR . 'modules' . PATH_SEPARATOR
);

// Zend_Application //
require_once 'Simec/Util.php';
require_once 'Zend/Application.php';

// Create APPLICATION, bootstrap, and run
try 
{
	$application = new Zend_Application(APPLICATION_ENV, APPLICATION_PATH . DIRECTORY_SEPARATOR . 'configs' . DIRECTORY_SEPARATOR . 'application.ini');
	$application->bootstrap()->run();	
} catch (Exception $e) {
	die($e->getMessage());
}