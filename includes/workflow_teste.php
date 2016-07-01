<?php

function algumacoisa( $pri, $sec )
{
	echo $pri . ' -> ' . $sec;
}

$chamada = "algumacoisa( algo, otro )";
$param = array(
	'algo' => 44,
	'otro' => "lasanha"
);

$chamada = wf_tratarChamada( $chamada, $param );
//var_dump( $chamada );
//echo "<br/>";
call_user_func_array( $chamada['funcao'], $chamada['parametros'] );


?>