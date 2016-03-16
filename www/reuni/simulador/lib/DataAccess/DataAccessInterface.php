<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */
Interface DataAccessInterface {

	public function __construct($host,$user,$pass,$db,$schema=null);

    public function fetch($sql);

    public function rowCount();

    public function getRow();

    public function isConnected();

    public function getError();

    public function escape_string($string);

}
?>
