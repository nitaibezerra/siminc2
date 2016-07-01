<?
$coordIES = carregarDadosIdentificacaoUsuario(
											  array("uncid"=>$_SESSION['sispacto3']['universidade']['uncid'],
													"pflcod"=>PFL_COORDENADORIES)
											  
											  );

$mensagemalert	= "Dados do Coordenador da IES salvos com sucesso";
$sis 	  		= "universidade";

if($coordIES) {
	$coordIES = current($coordIES);
	extract($coordIES);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISPACTO."dadosidentificacaousuario.inc";

?>