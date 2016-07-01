<?
$coordestadual = carregarDadosIdentificacaoUsuario(array("pflcod" => PFL_COORDENADORESTADUAL,
														 "iusid"  => $_SESSION['escolaterra']['coordenadorestadual']['iusid']));

$mensagemalert	= "Dados do Cooordenador Estadual gravado com sucesso";
$sis	  		= "coordenadorestadual";

$goto_ant 		= "escolaterra.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "escolaterra.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "escolaterra.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=cadastro";



if($coordestadual) {
	$coordestadual = current($coordestadual);
	extract($coordestadual);
}



if($iusid) include_once APPRAIZ_ESCOLATERRA."dadosidentificacaousuario.inc";
else echo "<p align=center>Coordenador Estadual não esta cadastrado.</p>";

?>