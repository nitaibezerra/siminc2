<?php

if( $_SESSION['evento']['eveid'] ){
    unset( $_SESSION['evento'] );
}

include_once 'cadEvento.inc';

?>