<?php
//require_once("configSSD.php" );
class SSD_Start
{
    public function __construct()
    {
        require_once("library/SSD/SSDConnector.php" );
        require_once("library/SSD/SSDWsAuth.php" );
        require_once("library/SSD/SSDWSSignDocs.php" );
        require_once("library/SSD/SSDWsUser.php" );
    }
}