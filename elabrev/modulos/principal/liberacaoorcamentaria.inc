<?php
if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

if($_REQUEST['lbrid']) {
	session_start();
	$_SESSION['elabrev_var']['lbrid'] = $_REQUEST['lbrid'];
	header( "Location: elabrev.php?modulo=principal/dadosGeraisCDO&acao=A" );
	exit();
}

unset($_SESSION['elabrev_var']['lbrid']);



include APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";
$titulo_modulo = "Liberação Orçamentária";
monta_titulo( $titulo_modulo, 'Ferramenta de liberação orçamentária' );

$estadoAtual = pegarEstadoAtual( $_SESSION['elabrev_var']['lbrid'] );

if( !empty($_POST) ){
	extract( $_POST );
}
?>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script>
function criarliberacao() {
	var conf = confirm('Deseja realmente criar uma nova liberação?');
	if(conf) {
		window.location='?modulo=principal/liberacaoorcamentaria&acao=A&requisicao=criarNovaLiberacao';		
	}
}
function removerliberacao(lbrid) {
	var conf = confirm('Deseja realmente excluir liberação?');
	if(conf) {
		window.location='?modulo=principal/liberacaoorcamentaria&acao=A&requisicao=removerLiberacao&lbrid='+lbrid;		
	}
}
function alterarSituacaoVariosLiberacao() {
	if(document.getElementById('lbrsituacao').value == "") {
		alert("Selecione a situação");
		return false;
	}
	var params = document.getElementById('liberacoes').serialize();
	params = params+'&requisicao=alterarSituacaoLiberacao&lbrsituacao='+document.getElementById('lbrsituacao').value;
	
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: params,
			asynchronous: false,
			onComplete: function(resp) {
			window.location='elabrev.php?modulo=principal/liberacaoorcamentaria&acao=A';
			},
			onLoading: function(){}
		});
		
}

function pesquisaLiberação(){
	$('requisicao1').value = 'pesquisa';
	$('formulario').submit();
}
</script>
<form id="formulario" name="formulario" action="#" method="post" enctype="multipart/form-data" >
<input type="hidden" name="requisicao1" id="requisicao1" value="">

<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="subtitulodireita">Número liberação</td>
		<td><?=campo_texto('libnumeroliberacao', 'N', 'S', '', 15, 10, '[#]', '','','','','id="libnumeroliberacao"', '', '', "this.value=mascaraglobal('[#]',this.value);"); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Número Processo</td>
		<td><?=campo_texto('libnumprocesso', 'N', 'S', '', 30, 20, '', '','','','','id="libnumprocesso"'); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Data Liberação</td>
		<td><?=campo_data2('libdataliberacao', 'N','S','Data Liberação','','',''); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Situação</td>
		<td><?
			$sql = "SELECT distinct
					  	lbrsituacao as codigo,
					  	CASE lbrsituacao 
							WHEN 'E' THEN 'Em Elaboração'
							WHEN 'G' THEN 'Liberação Gerada'
							WHEN 'I' THEN 'Liberação Impressa'
							WHEN 'A' THEN 'Aguardando assinatura'
						END as descricao
					FROM 
  						elabrev.liberacao WHERE lbrstatus = 'A'";
			echo $db->monta_combo("lbrsituacaof", $sql, 'S','-- Selecione --','', '', '',150,'N','lbrsituacaof', '', '', 'Situação');?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Descrição</td>
		<td><?=campo_texto('libdescricao', 'N', 'S', '', 80, 250, '', '','','','','id="libdescricao"');?></td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td></td>
		<td>
			<input type="button" value="Pesquisar" name="btnPesquisar" id="btnPesquisar" onclick="pesquisaLiberação();" style="cursor: pointer;">
			<input type="button" value="Limpar Campos" name="btnLimpar" id="btnLimpar" onclick="window.location.href = 'elabrev.php?modulo=principal/liberacaoorcamentaria&acao=A';" style="cursor: pointer;">
		</td>
	</tr>
</table>
</form>
<?
monta_titulo( '', 'Lista de liberação orçamentária' );

$filtro = '';

if( !empty($_POST['libnumprocesso']) ){
	$filtro .= " and lib.libnumprocesso = '{$_POST['libnumprocesso']}'";
}
if( !empty($_POST['lbrsituacaof']) ){
	$filtro .= " and lib.lbrsituacao = '{$_POST['lbrsituacaof']}'";
}
if( !empty($_POST['libnumeroliberacao']) ){
	$filtro .= " and lib.libnumeroliberacao = '{$_POST['libnumeroliberacao']}'";
}
if( !empty($_POST['libdataliberacao']) ){
	$filtro .= " and lib.libdataliberacao = '".formata_data_sql($_POST['libdataliberacao'])."'";
}
if( !empty($_POST['libdescricao']) ){
	$filtro .= " and lib.libdescricao ilike ('%{$_POST['libdescricao']}%')";
}

if( disabled($estadoAtual) ){
	$btExcluir = " <img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer\"> ";
} else {
	$btExcluir = " <img src=\"../imagens/excluir.gif\" onclick=\"removerliberacao(\''||lib.lbrid||'\')\" style=\"cursor:pointer\"> ";	
}

$sql = "SELECT '<center><input type=\"checkbox\" ".disabled($estadoAtual)." name=\"lbrsituacao_'||lib.lbrid||'\" value=\"'||lib.lbrid||'\"></center>' as chk, 
		--'<center><img src=\"../imagens/alterar.gif\" onclick=\"window.location=\'?modulo=principal/liberacaoorcamentaria&acao=A&requisicao=alterarLiberacao&lbrid='||lbrid||'\';\" style=\"cursor:pointer\"> $btExcluir </center>' as acao, 
		'<center><img src=\"../imagens/alterar.gif\" onclick=\"window.location=\'elabrev.php?modulo=principal/liberacaoorcamentaria&acao=A&lbrid='||lbrid||'\';\" style=\"cursor:pointer\"> $btExcluir </center>' as acao,
		'<center>'||libnumeroliberacao || '/' || to_char(libdataliberacao, 'YYYY')||'</center>' as num,
		usunome, 
		to_char(libdataliberacao, 'dd/mm/YYYY') as libdataliberacao,
		CASE lbrsituacao 
			WHEN 'E' THEN 'Em Elaboração'
			WHEN 'G' THEN 'Liberação Gerada'
			WHEN 'I' THEN 'Liberação Impressa'
			WHEN 'A' THEN 'Aguardando assinatura'
		END as lbrsituacao
		FROM elabrev.liberacao lib 
		LEFT JOIN seguranca.usuario usu ON usu.usucpf=lib.usucpf 
		WHERE lbrstatus!='I' $filtro ORDER BY libnumeroliberacao";
$cabecalho = array("","","Número liberação","Usuário","Data", "Situação");
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "liberacoes");

?>
<table class="tabela" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<!--<tr>
	<td colspan="2" align="right">
	<?
	$dados = array(0 => array("codigo" => "E", "descricao" => "Em Elaboração"),
				   1 => array("codigo" => "G", "descricao" => "Liberação Gerada"),
				   2 => array("codigo" => "I", "descricao" => "Liberação Impressa"),
				   3 => array("codigo" => "A", "descricao" => "Aguardando assinatura")
				   );
				   
	$db->monta_combo("lbrsituacao", $dados, "S", "Selecione...", '', '', '', '', 'N', 'lbrsituacao');
	?>
	 <input type="button" name="subbtn" value='Alterar Situação' <?=disabled($estadoAtual); ?> onclick="alterarSituacaoVariosLiberacao();">
	</td>
</tr>-->
<tr bgcolor="#C0C0C0">
	<td colspan="2"><input type='button' <?=disabled($estadoAtual); ?> onclick="criarliberacao();" value='Nova liberação'></td>
</tr>
</table>