<?
//include "_funcoes.php";
include_once APPRAIZ . "includes/workflow.php";
include_once "_funcoes.php";
include_once "_funcoes_pagamentos.php";


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	$db->close();
	exit;
}

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>

<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script language="javascript" type="text/javascript" src="./js/sismedio.js"></script>
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
	window.location.reload();
	
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

function excluirPagamento(pboid) {

	var conf = confirm('Deseja realmente excluir pagamento?');

	if(conf) {
		ajaxatualizar('requisicao=excluirPagamento&pboid='+pboid,'');
		alert('Pagamento excluído com sucesso');
		window.location.reload();
	}
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

$periodo 	  =	$db->pegaUm("SELECT m.mesdsc||'/'||f.fpbanoreferencia as periodo FROM sismedio.folhapagamento f  
							INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia 
							WHERE fpbid='".$_REQUEST['fpbid']."'");

$universidade = $db->pegaUm("SELECT uninome FROM sismedio.universidade WHERE uniid='".$_REQUEST['uniid']."'");


$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
$combo_estuf = $db->monta_combo('estuf_pagamento', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF', '', '', '', 'S', 'estuf_pagamento', true, $_REQUEST['estuf_pagamento']); 


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
				<tr>
					<td class=\"SubTituloDireita\">UF</td>
					<td>".$combo_estuf."</td>
				</tr>
				<tr>
					<td class=\"SubTituloDireita\">Município</td>
					<td id=\"td_municipio\">".(($_REQUEST['estuf_pagamento'])?carregarMunicipiosPorUF(array('id'=> 'muncod_sim','name'=> 'muncod','estuf' =>$_REQUEST['estuf_pagamento'],'valuecombo' =>$_REQUEST['muncod'],'returncombo' =>true)):"Selecione uma UF")."</td>
				</tr>
				<tr>
					<td class=\"SubTituloCentro\" colspan=2><input type=\"button\" name=\"filtrar\" value=\"Filtrar\" onclick=\"filtrarPagamentos();\"> <input type=\"button\" name=\"todos\" value=\"Ver Todos\" onclick=\"verTodos();\"></td>
				</tr>
		   </table>";

monta_titulo( "Lista de Pagamentos", $titulo);


if($_REQUEST['muncod']) $filtro_municipio = " AND i.muncodatuacao='".$_REQUEST['muncod']."'";

$sql = "SELECT '<center style=\"white-space:nowrap;\"><input type=\"checkbox\" name=\"pboid[]\" value=\"'||p.pboid||'\" '||CASE WHEN i.cadastradosgb=true THEN '' ELSE 'disabled' END||'> <img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirPagamento('||p.pboid||');\"></center>' as chk,
			   i.iuscpf,
			   i.iusnome, 
			   '<span style=\"font-size:x-small;\">'||CASE WHEN i.iuscodigoinep IS NOT NULL THEN lemnomeescola
			   ELSE 'Equipe IES' END||'</font>' as rede,
			   p.pbovlrpagamento,
			   e.esddsc,
			   CASE WHEN i.cadastradosgb=true THEN '<span style=color:blue;>Sim</font>' ELSE '<span style=\"color:red;cursor:pointer;\"><img src=../imagens/atencao.png align=absmiddle onclick=\"carregarLogCadastroSGB(\''||i.iuscpf||'\');\"> Não</font>' END as cadastrosgb, 
			   '<center><img src=\"../imagens/fluxodoc.gif\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"wf_exibirHistorico( '||p.docid||' );\"></center>' as historico
		FROM sismedio.pagamentobolsista p 
		INNER JOIN sismedio.identificacaousuario i ON i.iusd = p.iusd 
		INNER JOIN workflow.documento d ON d.docid = p.docid  AND d.tpdid=".TPD_PAGAMENTOBOLSA."
		INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
		INNER JOIN sismedio.folhapagamento f ON f.fpbid = p.fpbid 
		LEFT JOIN sismedio.listaescolasensinomedio lesc ON lesc.lemcodigoinep::integer = i.iuscodigoinep 
		WHERE p.uniid = '".$_REQUEST['uniid']."' AND p.fpbid='".$_REQUEST['fpbid']."' AND p.pflcod='".$_REQUEST['pflcod']."' AND e.esdid IN('".ESD_PAGAMENTO_APTO."','".ESD_PAGAMENTO_RECUSADO."') {$filtro_municipio} ORDER BY i.iusnome";

echo "<table class=tabela align=center><tr><td><input type=\"checkbox\" name=\"marcartodos[]\" onclick=\"marcarTodos(this);\"> Marcar todos</td></tr></table>";
$cabecalho = array("&nbsp","CPF","Nome","Escola","Valor(R$)","Situação","Cadastro SGB","Histórico");
$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','95%','',true,false,false,true);


?>
<p align="center"><input type="button" name="autorizarpagamento" value="Autorizar Pagamentos" onclick="autorizarPagamentos();"></p>
</form>