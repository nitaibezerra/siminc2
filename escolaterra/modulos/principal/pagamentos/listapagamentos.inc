<?
//include "_funcoes.php";
include_once APPRAIZ . "includes/workflow.php";
include_once "_funcoes.php";
include_once "_funcoes_pagamentos.php";


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>

<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script language="javascript" type="text/javascript" src="./js/escolaterra.js"></script>
<script>

function carregarRelatorio<?=PFL_TUTOR ?>(iusid,fpbid) {

	window.open(window.location+'&requisicao=carregarRelatorioTutor&iusid='+iusid+'&fpbid='+fpbid,'relatorios','scrollbars=1,width=1366,height=768,resizable=yes');


}

function carregarRelatorio<?=PFL_COORDENADORESTADUAL ?>(iusid,fpbid) {

	window.open(window.location+'&requisicao=carregarRelatorioCoordenadorEstadual&iusid='+iusid+'&fpbid='+fpbid,'relatorios','scrollbars=1,width=1366,height=768,resizable=yes');


}


function carregarLogCadastroSGB(usucpf) {

	divCarregando();
	

	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: 'requisicao=carregarLogCadastroSGB&usucpf='+usucpf,
   		async: false,
   		success: function(html){
   			jQuery("#log").html(html);
   		}
	});
	
	jQuery("#modalLog").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 400,
	                        modal: true,
	                     	close: function(){} 
	                    });
	                    
	divCarregado();
}

function sincronizarDadosUsuarioSGB() {
	divCarregando();
	ajaxatualizar('requisicao=sincronizarDadosUsuarioSGB&iusd='+jQuery('#iusd_log').val(),'');
	alert('Sincronização concluída com sucesso');
	divCarregado();
	
}

function marcarTodos(obj) {
	if(obj.checked) {
		jQuery("[name^='pboid[']:enabled").attr('checked',true);
	} else {
		jQuery("[name^='pboid[']:enabled").attr('checked',false);
	}
}

function autorizarPagamentos() {
	document.getElementById('formulario').submit();
}

function verTodos() {

	if(jQuery('#muncod_sim').length!=0) {
		jQuery('#muncod_sim').val('');
	}
	
	jQuery('#estuf_pagamento').val('');
	jQuery('#requisicao').val('');
	
	document.getElementById('formulario').submit();

}

function filtrarPagamentos() {
	if(jQuery('#muncod_sim').length==0) {
		alert('Selecione uma UF');
		return false;
	}
	
	if(jQuery('#muncod_sim').val()=='') {
		alert('Selecione uma UF');
		return false;
	}
	
	jQuery('#requisicao').val('');
	
	document.getElementById('formulario').submit();
}

function wf_exibirHistorico( docid )
		{
			var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/historico.php' +
				'?modulo=principal/tramitacao' +
				'&acao=C' +
				'&docid=' + docid;
			window.open(
				url,
				'alterarEstado',
				'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
			);
		}
</script>
<div id="modalLog" style="display:none;">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloCentro">Log</td>
</tr>
<tr>
	<td id="log"> 

	
	</td>
</tr>
<tr>	
	<td class="SubTituloCentro"><input type="button" value="Enviar novamente" onclick="sincronizarDadosUsuarioSGB();" id="sincronizarDadosUsuarioSGB"></td>
</tr>

</table>
</div>


<form method="post" name="formulario" id="formulario">
<input type="hidden" name="requisicao" id="requisicao" value="autorizarPagamentos">
<input type="hidden" name="pflcod" value="<?=$_REQUEST['pflcod'] ?>">
<input type="hidden" name="fpbid" value="<?=$_REQUEST['fpbid'] ?>">
<input type="hidden" name="ufpid" value="<?=$_REQUEST['ufpid'] ?>">
<?

if(!$_REQUEST['pflcod']) {
	$al = array("alert"=>"Perfil não encontrado","javascript"=>"window.close();");
	alertlocation($al);
}

$perfil  	  = $db->pegaUm("SELECT pfldsc FROM seguranca.perfil WHERE pflcod='".$_REQUEST['pflcod']."'");

$periodo 	  =	$db->pegaUm("SELECT m.mesdsc||'/'||f.fpbanoreferencia as periodo FROM escolaterra.periodoreferencia f  
							INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia 
							WHERE fpbid='".$_REQUEST['fpbid']."'");

$uf = $db->pegaUm("SELECT estuf FROM escolaterra.ufparticipantes WHERE ufpid='".$_REQUEST['ufpid']."'");


$titulo = "<table class=\"listagem\" align=\"center\" bgcolor=\"#f5f5f5\" cellspacing=\"1\" cellpadding=\"3\" width=\"100%\">
				<tr>
					<td class=\"SubTituloDireita\">Perfil</td>
					<td>".$perfil."</td>
				</tr>
				<tr>
					<td class=\"SubTituloDireita\">Período de referência</td>
					<td>".$periodo."</td>
				</tr>
				<tr>
					<td class=\"SubTituloDireita\">UF</td>
					<td>".$uf."</td>
				</tr>
		   </table>";

monta_titulo( "Lista de Pagamentos", $titulo);


if($_REQUEST['muncod']) $filtro_municipio = " AND (m1.muncod='".$_REQUEST['muncod']."' OR m2.muncod='".$_REQUEST['muncod']."')";

$sql = "SELECT '<center><input type=\"checkbox\" name=\"pboid[]\" value=\"'||p.pboid||'\" '||CASE WHEN i.cadastradosgb=true THEN '' ELSE 'disabled' END||'></center>' as chk,
   			   '<center><img src=\"../imagens/historico.png\" style=\"cursor:pointer;\" onclick=\"carregarRelatorio".$_REQUEST['pflcod']."('||i.iusid||',".$_REQUEST['fpbid'].");\"></center>' as projeto,
			   i.iuscpf,
			   i.iusnome, 
			   p.pbovlrpagamento,
			   e.esddsc,
			   CASE WHEN i.cadastradosgb=true THEN '<span style=color:blue;>Sim</font>' ELSE '<span style=\"color:red;cursor:pointer;\"><img src=../imagens/atencao.png align=absmiddle onclick=\"carregarLogCadastroSGB(\''||i.iuscpf||'\');\"> Não</font>' END as cadastrosgb, 
			   '<center><img src=\"../imagens/fluxodoc.gif\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"wf_exibirHistorico( '||p.docid||' );\"></center>' as historico
		FROM escolaterra.pagamentobolsista p 
		INNER JOIN escolaterra.identificacaousuario i ON i.iusid = p.iusid 
		INNER JOIN workflow.documento d ON d.docid = p.docid  AND d.tpdid=".TPD_PAGAMENTOBOLSA."
		INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
		INNER JOIN escolaterra.periodoreferencia f ON f.fpbid = p.fpbid 
		WHERE p.ufpid = '".$_REQUEST['ufpid']."' AND p.fpbid='".$_REQUEST['fpbid']."' AND p.pflcod='".$_REQUEST['pflcod']."' AND e.esdid IN('".ESD_PAGAMENTO_APTO."','".ESD_PAGAMENTO_RECUSADO."') {$filtro_municipio} ORDER BY i.iusnome";

echo "<table class=tabela align=center><tr><td><input type=\"checkbox\" name=\"marcartodos[]\" onclick=\"marcarTodos(this);\"> Marcar todos</td></tr></table>";
$cabecalho = array("&nbsp","Projeto","CPF","Nome","Valor(R$)","Situação","Cadastro SGB","Histórico");
$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','95%','',true,false,false,true);


?>
<p align="center"><input type="button" name="autorizarpagamento" value="Autorizar Pagamentos" onclick="autorizarPagamentos();"></p>
</form>