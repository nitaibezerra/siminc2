<?php


if($_REQUEST['requisicao']){
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include  APPRAIZ."includes/cabecalho.inc";

echo "<br/>";

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
function setarInuId(estuf) {
	window.location='snf.php?modulo=principal/diagnosticoPar&acao=A&requisicao=setarInuIdPorEstuf&estuf='+estuf+'&goto=ide';
}
</script>
<?

$perfis = pegaPerfilGeral();

if((in_array(PERFIL_FORUM_APROVACAO,$perfis) || in_array(PERFIL_FORUM_DISTRIBUICAO,$perfis))) {
	
	$sql = "SELECT DISTINCT estuf FROM snf.usuarioresponsabilidade WHERE pflcod IN('".PERFIL_FORUM_APROVACAO."','".PERFIL_FORUM_DISTRIBUICAO."') AND usucpf='".$_SESSION['usucpf']."' AND rpustatus='A'";
	$estuf = $db->pegaUm($sql);
	
	if($estuf) setarInuIdPorEstuf(array("estuf" => $estuf));
	
}

/* montando o menu */
echo montarAbasArray(montaMenuAtendimentoForum(), $_SERVER['REQUEST_URI']);
/* fim montando o menu */

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<thead>
		<th class="tdDegradde01" colspan="2" align="left">
			<h1 class="notprint">
				IDE do
				Estado
				:
				<?=$db->pegaUm("select estdescricao||' - '||estuf from territorios.estado where estuf='".$_SESSION['snf']['estuf']."'"); ?>
			</h1>
			<? if($db->testa_superuser()) : ?>
			<table cellspacing="0" cellpadding="3" border="0"
				style="width: 80px;">
				<tr style="text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
					<?
					$sql = "SELECT estuf as codigo, estdescricao as descricao FROM territorios.estado ORDER BY estdescricao"; 
					$db->monta_combo('estuf', $sql, 'S', 'Selecione o Estado', 'setarInuId', '', '', '200', 'N', 'estuf', '', $_SESSION['snf']['estuf']); 
					?>
					</td>
				</tr>
			</table>
			<? endif; ?>
		</th>
	</thead>

<? if($_SESSION['snf']['estcod']) : ?>
	<tr>
		<td>
		<iframe width="100%" height="500" frameborder="0" scrolling="yes" marginheight="0" marginwidth="0" src="http://ide.mec.gov.br/2011/estados/relatorio/uf/<?=$_SESSION['snf']['estcod'] ?>"></iframe>		
		</td>
	</tr>
<? else : ?>
	<tr>
		<td>
		Nenhum estado foi identificado.		
		</td>
	</tr>
<? endif; ?>
</table>

