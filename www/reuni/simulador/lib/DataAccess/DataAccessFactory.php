<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */
class DataAccessFactory {

	public static function factory($banco,$host,$user,$pass,$db,$schema=null) {
		switch ($banco) {

		case 'MySQL':
			return new DataAccessMySQL($host,$user,$pass,$db,$schema);
		break;

		case 'PostgreSQL':
			return new DataAccessPostgreSQL($host,$user,$pass,$db,$schema);
		break;
		}
	}

}
?>
