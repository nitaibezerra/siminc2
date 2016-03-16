<?php

if($_REQUEST['req']=='mostraLote'){
	
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
		  <link rel="stylesheet" type="text/css" href="../includes/listagem.css" />'; 
	
	monta_titulo('Lote de Devolução','');
	
	$sql = "select 
				t.tuttipo,
				t.tutnome,
				t.tutcpf,
				r.nu_nib,
				r.dt_ini_periodo,
				r.dt_fim_periodo,
				r.cs_natur_credito,
				r.id_orgao_pagador,
				r.dt_fim_validade
			from maismedicos.remessadetalhedevolucao r
			join maismedicos.remessadetalhe d on r.rmdid = d.rmdid
			join maismedicos.tutor t on t.tutid = r.tutid
			where arqid = {$_REQUEST['arqid']}";
	
	$arCabecalho = array('Tipo','Nome','CPF', 'Benefício', 'Inicio Período', 'Fim Período', 'Nat. Crédito', 'Orgão Pagador', 'Fim Validade');
	$db->monta_lista($sql, $arCabecalho, 1000000, 50, '', '', '', '');	
	die;
}

if($_REQUEST['req']=='downloadArquivo'){
	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";	
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
	$arquivo = $file->getDownloadArquivo($arqid);
// 		echo"<script>window.location.href = 'elabrev.php?modulo=principal/anexos&acao=A';</script>";
	exit;
}
 
if($_REQUEST['requisicao'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicao']();
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
$db->cria_aba( $abacod_tela, $url, $parametros);
monta_titulo( "Devolver Remessa de Crédito", "Anexe o arquivo de retorno.");

$arrPerfil = pegaPerfilGeral();
if(in_array(PERFIL_CONSULTA,$arrPerfil)){
	$habilitado = "N";
}else{
	$habilitado = "S";
}

?>
<style>
	.link{cursor:pointer}
	.bold{font-weight:bold}
	.center{text-align:center}
</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
<form name="formulario" id="formulario" method="post" action="" enctype="multipart/form-data" >
<input type="hidden" id="requisicao" name="requisicao" value="" />
<input type="hidden" id="classe" name="classe" value="RemessaCabecalho" />
<?php if($habilitado == "S"): ?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%"  >
		<tr>
			<td width="25%" class="SubTituloDireita">Arquivo de Retorno da Remessa de Crédito</td>
		    <td><input type="file" name="arquivoremesssa" id="arquivoremesssa" /></td>
		</tr>
		<tr>
			<td colspan="2" align="center" >
				<input type="button" name="btn_validar" value="Devolver" onclick="devolverRemessaCredito()" />
			</td>
		</tr>
	</table>
<?php endif; ?>
</form>
<script type="text/javascript">
	function visualizarLote(arqid)
	{		
		var popUp = window.open('/maismedicos/maismedicos.php?modulo=principal/devolverRemessaCredito&acao=A&req=mostraLote&arqid='+arqid, 'popLote', 'height=500,width=800,scrollbars=yes,top=50,left=200');
		popUp.focus();
	}
</script>
<?php 
monta_titulo('Lotes Devolvidos', '');
$sql = "select distinct	
			'<img src=\"../imagens/lupa_grafico.gif\" onclick=\"visualizarLote(' || a.arqid || ')\" alt=\"Visualizar lote\" title=\"Visualizar lote\" style=\"cursor:pointer;\" />' as acao,
			a.arqnome || '.' || a.arqextensao as arquivo,
			u.usunome,
			to_char(dt_cadastro, 'DD/MM/YYYY')
		from maismedicos.remessadetalhedevolucao r
		join public.arquivo a on a.arqid = r.arqid
		join seguranca.usuario u on u.usucpf = a.usucpf";

$arCabecalho =  array('Ação','Arquivo','Responsável','Data de envio');
$db->monta_lista($sql, $arCabecalho, 10, 50, '', '', '', '');
?>
<script>
function devolverRemessaCredito()
{
	var erro = 0;
	if(!$("#arquivoremesssa").val()){
		erro = 1;
		alert('Favor anexar o retorno da Remessa de Crédito.');
		return false;
	}
	if(erro == 0){
		$("#classe").val("RemessaDetalheDevolucao");
		$("#requisicao").val("devolverRemessaCredito");
		$("#formulario").submit();
	}
}

$(function() {
	<?php if($_SESSION['maismedicos']['remessa']['alert']): ?>
		alert('<?php echo $_SESSION['maismedicos']['remessa']['alert'] ?>');
		<?php unset($_SESSION['maismedicos']['remessa']['alert']) ?>
	<?php endif; ?>
});
</script>