<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class DataAccessMySQL implements DataAccessInterface {
    private $db;
    private $connected;
    private $query;

    public function __construct($host,$user,$pass,$db,$schema=null) {
        $this->connected=TRUE;
        $this->db=@mysql_pconnect($host,$user,$pass);
        if (!$this->db) {$this->connected=FALSE;}
        if (!@mysql_select_db($db,$this->db)) {$this->connected=FALSE;}
    }

    public function fetch($sql) {
        $this->query=@mysql_query($sql,$this->db);
        if ($this->query===FALSE) return FALSE; else return TRUE;
    }

    public function isConnected() {
        return $this->connected;
    }

    public function getError () {
        return @mysql_error($this->db);
    }

	public function rowCount () {
        return @mysql_num_rows($this->query);
    }

    public function getRow () {
        if ( $row=@mysql_fetch_array($this->query,MYSQL_ASSOC) )
            return $row;
        else
            return false;
    }

    public function escape_string($string) {
    	return mysql_escape_string($string);
    }

}
?>
