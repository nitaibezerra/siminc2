<?php
//echo 'Produção';

class ConnectOracle{
    #CONECXÃO COMO BANCO DE PRODUÇÃO - APENAS "LEITURA"
    private $_oraHost = '';        //host name or server ip address
    private $_oraDB   = '';  //database name
    private $_oraUser = '';   //username
    private $_oraPass = ''; //user password
    private $_oraPort = '';

    #Desenvolvimento    
//    private $_oraDB   = '';//database name
//    private $_oraHost = '';//host name or server ip address
//    private $_oraUser = ''; //username
//    private $_oraPass = ''; //user password
//    private $_oraPort = '';

    private $_autoConnect;
    private $_sql;

    /**
     * @name db
     * @var Banco de dados.
     */
    private $_db;

    public function __construct($autoConnect = true)
    {
        $this->_autoConnect = $autoConnect;

        if($this->_autoConnect){
            $this->connect();
        }
    }

    public function setSql($sql)
    {
        $this->_sql = $sql;
        return $this;
    }

    public function getSql()
    {
        return $this->_sql;
    }

    /**
     * Simple function to replicate PHP 5 behaviour
     */
    function microtime_float()
    {
        list($usec, $sec) = explode(" ", microtime());
        return ((float)$usec + (float)$sec);
    }

    private function connect()
    {
        $time_start = $this->microtime_float();
        
        //$dbname = "(DESCRIPTION = (LOAD_BALANCE = YES) (ADDRESS = (PROTOCOL = TCP)(HOST = )(PORT = ))(CONNECT_DATA =(SERVER = DEDICATED)(SERVICE_NAME = )))";
        //$dbname = "(DESCRIPTION = (LOAD_BALANCE = YES) (ADDRESS = (PROTOCOL = TCP)(HOST = )(PORT = ))(CONNECT_DATA =(SERVER = DEDICATED)(SERVICE_NAME = )))";
        //$dbname = "(DESCRIPTION = (LOAD_BALANCE = YES) (ADDRESS = (PROTOCOL = TCP)(HOST = )(PORT = ))(CONNECT_DATA =(SERVER = DEDICATED)(SERVICE_NAME = )))";
        
        #BANCO PRODUÇÃO APENAS LEITURA
        $dbname = "(DESCRIPTION = (LOAD_BALANCE = YES) (ADDRESS = (PROTOCOL = TCP)(HOST = )(PORT = ))(CONNECT_DATA =(SERVER = DEDICATED)(SERVICE_NAME = )))";

        $time = microtime(true);
        //$this->_bd = oci_connect($this->_oraUser,$this->_oraPass,'//'.$this->_oraHost . ':' . $this->_oraPort .'/'.$this->_oraDB,'');
        
        $this->_bd = oci_connect($this->_oraUser,$this->_oraPass, $dbname);
        
        if (!$this->_bd){
            $ora_conn_erno = oci_error();
            echo ($ora_conn_erno['message']."\n");
            oci_close($this->_bd);
        }
        $time_end = $this->microtime_float();
        $time = $time_end - $time_start;
        
    //ver($time );
    }

    private function disconnect()
    {
        oci_close($this->_bd);
    }

    public function getAll($sql = null)
    {
        if(!$sql) $sql = $this->getSql();

        //$time_start = $this->microtime_float();
        $stid = oci_parse($this->_bd, $sql);
        oci_execute($stid);
        
        $result = oci_fetch_all($stid, $res, null, null, OCI_FETCHSTATEMENT_BY_ROW);

        if($this->_autoConnect){
            //$this->disconnect();
        }
        //$time_end = $this->microtime_float();
        //$time = $time_end - $time_start;

        //ver($time , $sql , count($res) , $res);
        return $res;
    }

    public static function sGetAll($sql)
    {
        $_oraHost = '';        //host name or server ip address
        $_oraDB   = '';  //database name
        $_oraUser = '';   //username
        $_oraPass = ''; //user password
        $_oraPort = '';

        $_db = oci_connect($_oraUser,$_oraPass,'//'.$_oraHost . ':' . $_oraPort .'/'.$_oraDB,'');
        $stid = oci_parse($_db, $sql);

        oci_execute($stid);
        $result = oci_fetch_all($stid, $res, null, null, OCI_FETCHSTATEMENT_BY_ROW);

        oci_close($_db);

        return $res;
    }

    public function get($sql = null)
    {
        if(!$sql) $sql = $this->getSql();

        $stid = oci_parse($this->_bd, $sql);

        oci_execute($stid);
        $result = oci_fetch_row($stid);
        $result = oci_fetch_assoc($stid);

        if($this->_autoConnect){
            $this->disconnect();
        }
        return $result;
    }

    public function save()
    {

    }

    /**
     * Returns the static model of the specified AR class.
     * Please note that you should have this exact method in all your CActiveRecord descendants!
     * @param string $className active record class name.
     * @return User the static model class
     */
    public static function ConnectOracle($className=__CLASS__)
    {
        ver($className, d);
        //self::ConnectOracle(__construct());
        //return self::ConnectOracle($className);
    }
}
