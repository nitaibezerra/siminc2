<?php
//echo 'Produção';

class ConnectOracle{
    #CONECXÃO COMO BANCO DE PRODUÇÃO - APENAS "LEITURA"
    private $_oraHost = '10.38.0.244';        //host name or server ip address
    private $_oraDB   = 'prdods.mec.gov.br';  //database name
    private $_oraUser = 'IECENSOSUP_SIMEC';   //username
    private $_oraPass = '3nd9Fh9nN9N9vhnjs0'; //user password
    private $_oraPort = 1551;

    #Desenvolvimento    
//    private $_oraDB   = 'dsvods.mec.gov.br';//database name
//    private $_oraHost = 'dsv-oracle-scan';//host name or server ip address
//    private $_oraUser = 'IECENSOSUP_SIMEC'; //username
//    private $_oraPass = 'IECENSOSUP_SIMEC'; //user password
//    private $_oraPort = 1521;

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
        
        //$dbname = "(DESCRIPTION = (LOAD_BALANCE = YES) (ADDRESS = (PROTOCOL = TCP)(HOST = )(PORT = 1521))(CONNECT_DATA =(SERVER = DEDICATED)(SERVICE_NAME = )))";
        //$dbname = "(DESCRIPTION = (LOAD_BALANCE = YES) (ADDRESS = (PROTOCOL = TCP)(HOST = )(PORT = 1521))(CONNECT_DATA =(SERVER = DEDICATED)(SERVICE_NAME = )))";
        //$dbname = "(DESCRIPTION = (LOAD_BALANCE = YES) (ADDRESS = (PROTOCOL = TCP)(HOST = )(PORT = 1521))(CONNECT_DATA =(SERVER = DEDICATED)(SERVICE_NAME = )))";
        
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
        $_oraHost = '10.38.0.244';        //host name or server ip address
        $_oraDB   = 'prdods.mec.gov.br';  //database name
        $_oraUser = 'IECENSOSUP_SIMEC';   //username
        $_oraPass = '3nd9Fh9nN9N9vhnjs0'; //user password
        $_oraPort = 1551;

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
