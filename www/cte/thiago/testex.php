<?php

xdebug_start_trace();
$tes = array(1,2,3,4,5,6,7,8,9,0);
foreach($tes as $a){
	echo $a;	
}
xdebug_stop_trace();
