<?php

$tcrid = (integer) $_REQUEST['tcrid'];
if ( $tcrid )
{
	tc_remover( $tcrid );
	$db->commit();
}

