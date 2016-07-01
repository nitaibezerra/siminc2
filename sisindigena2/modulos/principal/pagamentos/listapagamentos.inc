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

<script language="javascript" type="text/javascript" src="./js/sisindigena2.js"></script>
<script>

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
	jQuery("[name^='pboid[']:enabled").attr('checked',false);
	
	document.getElementById('formulario').submit();
	
}

function cancelarPagamento(pboid) {
	var justificativa = prompt('Digite a justificativa do cancelamento:','');

	if(justificativa) {
		ajaxatualizar('requisicao=cancelarPagamento&pboid='+pboid+'&justificativa='+justificativa,'');
	}
	
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
<input type="hidden" name="uniid" value="<?=$_REQUEST['uniid'] ?>">
<?

if(!$_REQUEST['pflcod']) {
	$al = array("alert"=>"Perfil não encontrado","javascript"=>"window.close();");
	alertlocation($al);
}

$perfil  	  = $db->pegaUm("SELECT pfldsc FROM seguranca.perfil WHERE pflcod='".$_REQUEST['pflcod']."'");

$periodo 	  =	$db->pegaUm("SELECT m.mesdsc||'/'||f.fpbanoreferencia as periodo FROM sisindigena2.folhapagamento f  
							INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia 
							WHERE fpbid='".$_REQUEST['fpbid']."'");

$universidade = $db->pegaUm("SELECT uninome FROM sisindigena2.universidade WHERE uniid='".$_REQUEST['uniid']."'");

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
					<td class=\"SubTituloDireita\">Universidade</td>
					<td>".$universidade."</td>
				</tr>
		   </table>";

monta_titulo( "Lista de Pagamentos", $titulo);


if($_REQUEST['muncod']) $filtro_municipio = " AND (m1.muncod='".$_REQUEST['muncod']."' OR m2.muncod='".$_REQUEST['muncod']."')";

$sql = "SELECT '<center style=\"white-space:nowrap\"><input type=\"checkbox\" name=\"pboid[]\" value=\"'||p.pboid||'\" '||CASE WHEN i.cadastradosgb=true THEN '' ELSE 'disabled' END||'> <img src=\"../imagens/valida3.gif\" style=\"cursor:pointer;\" onclick=\"cancelarPagamento('||p.pboid||');\"></center>' as chk,
			   i.iuscpf,
			   i.iusnome, 
			   nu.unisigla||' - '||nu.uninome as universidade,
			   p.pbovlrpagamento,
			   e.esddsc,
			   CASE WHEN i.cadastradosgb=true THEN '<span style=color:blue;>Sim</font>' ELSE '<span style=\"color:red;cursor:pointer;\"><img src=../imagens/atencao.png align=absmiddle onclick=\"carregarLogCadastroSGB(\''||i.iuscpf||'\');\"> Não</font>' END as cadastrosgb, 
			   '<center><img src=\"../imagens/fluxodoc.gif\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"wf_exibirHistorico( '||p.docid||' );\"></center>' as historico
		FROM sisindigena2.pagamentobolsista p 
		INNER JOIN sisindigena2.identificacaousuario i ON i.iusd = p.iusd 
		INNER JOIN workflow.documento d ON d.docid = p.docid  AND d.tpdid=".TPD_PAGAMENTOBOLSA."
		INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
		INNER JOIN sisindigena2.folhapagamento f ON f.fpbid = p.fpbid 
		LEFT JOIN sisindigena2.nucleouniversidade pic ON pic.picid = i.picid 
		LEFT JOIN sisindigena2.universidade nu ON nu.uniid = pic.uniid 
		WHERE p.uniid = '".$_REQUEST['uniid']."' AND p.fpbid='".$_REQUEST['fpbid']."' AND p.pflcod='".$_REQUEST['pflcod']."' AND e.esdid IN('".ESD_PAGAMENTO_APTO."','".ESD_PAGAMENTO_RECUSADO."') {$filtro_municipio} ORDER BY i.iusnome";

echo "<table class=tabela align=center><tr><td><input type=\"checkbox\" name=\"marcartodos[]\" onclick=\"marcarTodos(this);\"> Marcar todos</td></tr></table>";
$cabecalho = array("&nbsp","CPF","Nome","Núcleo","Valor(R$)","Situação","Cadastro SGB","Histórico");
$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','95%','',true,false,false,true);


?>
<p align="center"><input type="button" name="autorizarpagamento" value="Autorizar Pagamentos" onclick="autorizarPagamentos();"></p>
</form>