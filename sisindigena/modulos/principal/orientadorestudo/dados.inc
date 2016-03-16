<?
$oriest = carregarDadosIdentificacaoUsuario(array("pflcod"=>PFL_ORIENTADORESTUDO,"iusd" => $_SESSION['sisindigena']['orientadorestudo']['iusd']));

$mensagemalert	= "Dados do Orientador de Estudo salvos com sucesso";
$sis 	  		= "orientadorestudo";
$goto_ant 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= false;

if($oriest) {
	$oriest = current($oriest);
	extract($oriest);
} else {
	$consulta = true;
}

include_once APPRAIZ_SISINDIGENA."dadosidentificacaousuario.inc";

?>