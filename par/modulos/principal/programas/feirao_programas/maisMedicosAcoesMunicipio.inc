<?php 
include_once APPRAIZ . "includes/workflow.php";
include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';
echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/maisMedicosAcoesMunicipio&acao=A');
monta_titulo( $titulo_modulo, recuperaMunicipioEstado());
$_SESSION['maismedicos'] = true; 

include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";

if($_SESSION['par']['rqmid']){
	$docid = pgMaisMedicosCriarDocumento($_SESSION['par']['rqmid']);	
}

$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);

if(in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $arrayPerfil) || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $arrayPerfil) || in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) || in_array(PAR_PERFIL_AVAL_INSTITUCIONAL_MM, $arrayPerfil) ){
    $habilitado = "N"; 
} else {
    $habilitado = "S";
}

?>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
	<tr>
		<td>
			<?php 
			header('Content-type: text/html; charset=ISO-8859-1');
			$qrpid = pegaQrpidMaisMedicos($_SESSION['par']['rqmid'], QUEST_MAIS_MEDICOS);
			$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'habilitado' => $habilitado, 'relatorio'=> 'modulo=relatorio/maisMedicosRelatorioAcoesMunicipio&acao=A')); ?>
		</td>
		<?php if($docid){ ?>
		<td align="center" valign="top" width="10%"><br><?php wf_desenhaBarraNavegacao( $docid , array('rqmid' => $_SESSION['cap']['rqmid']), ''); ?></td>
		<?php } ?>	
	</tr>
</table>
