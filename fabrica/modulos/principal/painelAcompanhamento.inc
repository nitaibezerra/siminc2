<?php
$clsFuncoes = new funcoesProjetoFabrica();

if($_REQUEST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicaoAjax']();
	exit;
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$titulo = "Painel de Acompanhamento";
monta_titulo( $titulo, '&nbsp;' );

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../../includes/remedial.js"></script>
<script type="text/javascript" src="../../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>

<script>
	function exibirSolicitacao(scsid,ansid)
	{
		window.location.href = 'fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid=' + scsid + "&ansid=" + ansid;
	}
	
	function atestarSolicitacao(scsid)
	{
		
		var valor = $("[name=rdb_atestar_" + scsid + "]:radio:checked").val();
		if(valor == "sim"){
			valor = "true";
		}else{
			valor = "false";
		}
		
		$.ajax({
		   type: "POST",
		   url: window.location,
		   data: "&requisicaoAjax=atestarSolicitacao&scsid=" + scsid + "&scsatesto=" + valor,
		   success: function(msg){
		   }
		 });
	}
	
	function abrirOS(scsid,ansid,odsid)
	{
		window.open('fabrica.php?modulo=principal/cadOSExecucao&acao=A&scsid=' + scsid + '&ansid=' + ansid + '&odsid=' + odsid,'Observações','scrollbars=yes,height=700,width=800,status=no,toolbar=no,menubar=no,location=no');
	}
</script>

<style>
	.TituloTabela{background-color: #C5C5C5;font-weight:bold}
	.center{text-align:center}
	.link{cursor: pointer;}
	.middle{vertical-align: middle;}
</style>

<form name="form_cad_contrato" id="form_cad_contrato" method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="50%" class="TituloTabela center">Demandas em Execução</td>
			<td class="TituloTabela center">Detalhar Solicitações de Serviço</td>
		</tr>
		<tr>
			<td valign="top" class="center"><div style="height: 230px; overflow-y: auto; overflow-x: hidden;"><?php $clsFuncoes->exibirDemandasExecucao()?><div></td>
			<td valign="top" class="center"><div style="height: 230px; overflow-y: auto; overflow-x: hidden;"><?php $clsFuncoes->exibirSolicitacoesServico()?><div></td>
		</tr>
		<tr>
			<td valign="top" class="TituloTabela center">Demandas Aprovadas (Alocar Recurso)</td>
			<td valign="top" class="TituloTabela center">Atestar Solicitações de Serviço</td>
		</tr>
		<tr>
			<td valign="top" class="center"><div style="height: 230px; overflow-y: auto; overflow-x: hidden;"><?php $clsFuncoes->exibirDemandasAprovadas()?><div></td>
			<td valign="top" class="center"><div style="height: 230px; overflow-y: auto; overflow-x: hidden;"><?php $clsFuncoes->exibirAtestarDemandas()?><div></td>
		</tr>
	</table>
</form>