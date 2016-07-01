<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class DataAccessPostgreSQL implements DataAccessInterface {
    private $db;
    private $connected;
    private $query;

    public function __construct($host,$user,$pass,$db,$schema=null) {
    	$this->connected=TRUE;
        $this->db = @pg_pconnect("host=".$host." dbname=".$db." user=".$user." password=".$pass);
        if (!$this->db) {$this->connected=FALSE;}
        $sql ='SET search_path TO '.$schema;
        if (!$this->fetch($sql)) {$this->connected=FALSE;}
    }

    public function fetch($sql) {
        $this->query = @pg_query($this->db, $sql);
        if ($this->query===FALSE) return FALSE; else return TRUE;
    }

    public function isConnected() {
        return $this->connected;
    }

    public function getError () {
        return @pg_last_error($this->db);
    }

    public function rowCount () {
        return @pg_num_rows($this->query);
    }

    public function getRow () {
        if ($row=@pg_fetch_array($this->query,null,PGSQL_ASSOC))
            return $row;
        else
            return false;
    }

    public function escape_string($string) {
    	return pg_escape_string($string);
    }
}
?>
