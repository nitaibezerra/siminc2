<?php

if( $_SESSION['eveid'] )
{
	unset($_SESSION['eveid']);
}

include_once 'cadEvento.inc';

?>