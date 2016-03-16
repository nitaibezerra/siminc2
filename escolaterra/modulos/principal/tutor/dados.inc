<?
$tutor = carregarDadosIdentificacaoUsuario(array("pflcod" => PFL_TUTOR,
												 "iusid"  => $_SESSION['escolaterra']['tutor']['iusid']));

$mensagemalert	= "Dados do Tutor gravado com sucesso";
$sis	  		= "tutor";

$goto_ant 		= "escolaterra.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "escolaterra.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "escolaterra.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=cadastro";



if($tutor) {
	$tutor = current($tutor);
	extract($tutor);
}



if($iusid) include_once APPRAIZ_ESCOLATERRA."dadosidentificacaousuario.inc";
else echo "<p align=center>Tutor não esta cadastrado.</p>";

?>