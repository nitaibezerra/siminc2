<?php 

ini_set("memory_limit","250M");
set_time_limit(0);

include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
require_once APPRAIZ . 'includes/workflow.php';

$_SESSION['par']['cod_inep'] = $_REQUEST['cod_inep'] ? $_REQUEST['cod_inep'] : $_SESSION['par']['cod_inep'];

//Chamada de programa
include_once APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";

monta_titulo('FINANCIAMENTO DE NOVAS TURMAS DE EJA', '');
echo "<br>";
$sql = "SELECT DISTINCT
			no_entidade as escola
		FROM 
			".SCHEMAEDUCACENSO.".tab_entidade
		WHERE
			cod_orgao_regional_inep = '".$_REQUEST['cod_inep']."'";
$linha1 = $db->pegaUm($sql); 
if($_SESSION['par']['muncod']){
	
	$sql = "select mundescricao, estuf from territorios.municipio where muncod = '{$_SESSION['par']['muncod']}'";
	$rsMunicipio = $db->pegaLinha($sql);
	$linha2 = $rsMunicipio['mundescricao'].' - '.$rsMunicipio['estuf'];
	
}else{
	
	$linha2 = $_SESSION['par']['estuf'];
}
monta_titulo($linha1, $linha2);

echo "<br/>"; 
$db->cria_aba($abacod_tela, $url, $parametros);
monta_titulo('Plano de Ações',''); 

$queid = 78;

$qrpid = pegaQrpidEJA( $_SESSION['par']['cod_inep'], $queid );

$sql = "SELECT
			docid
		FROM
			par.instrumentoeja 
		WHERE
			pk_cod_entidade = ".$_SESSION['par']['cod_inep'];

$docid = $db->pegaUm( $sql );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
	$(function(){
	});
</script>
<center>
	<table bgcolor="#f5f5f5" align="center" class="tabela" >
		<tr>
			<td width="90%" align="center">
				<?php
				$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'tamDivPx' => 250, 'habilitado' => $habilitado ) );
				?>
			</td>
			<td valign="top">
				<?php wf_desenhaBarraNavegacao( $docid , array( 'cod_inep' => $_SESSION['par']['cod_inep'] ),  array('historico'=>true)); ?>
			</td>
		</tr>
	</table>
	<p>
	<input type="button" value="Anterior" onclick="window.location.href = 'par.php?modulo=principal/programas/fin_nov_tur_eja/formularioAdesao&acao=A'" />
<!--	<input type="button" value="Próximo"  onclick="window.location.href = 'par.php?modulo=principal/programas/fin_nov_tur_eja/turmas&acao=A'" />-->
	<input type="button" value="Sair" 	  onclick="window.location.href = 'par.php?modulo=principal/programas/fin_nov_tur_eja/lista_escolas&acao=A'" />
</center>
