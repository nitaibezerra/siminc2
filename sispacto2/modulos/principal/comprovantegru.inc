<?

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sispacto2.js"></script>';

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<div id="modalOrientacaoAdm" style="display:none;">
<form method="post" id="formulario_orientacao" name="formulario_orientacao">
<input type="hidden" name="abaid" id="abaid">
<input type="hidden" name="requisicao" value="salvarOrientacaoAdm">
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloCentro" colspan="2">Orientação</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%"></td>
	<td><? echo campo_textarea( 'oabdesc', 'S', 'S', '', '70', '4', '5000'); ?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="salvarorientacao" value="Salvar Orientação" onclick="salvarOrientacaoAdm();"></td>
</tr>
</table>
</form>
</div>

<script>
function salvarComprovanteGRU() {

	if(jQuery("#cogcpf").val()=='') {
		alert('CPF do bolsista em branco');
		return false;
	}

	if(jQuery("#fpbid").val()=='') {
		alert('Selecione período de referência');
		return false;
	}

	if(jQuery("#pflcod").val()=='') {
		alert('Selecione o perfil');
		return false;
	}

	if(jQuery("#cogdtpagamento").val()=='') {
		alert('Data de pagamento em branco');
		return false;
	}
	
	jQuery("#formulariogru").submit();
	
}

function excluirGRU(cogid) {
	var conf = confirm('Deseja realmente excluir o comprovante GRU?');

	if(conf) {
		window.location='sispacto2.php?modulo=principal/comprovantegru&acao=A&requisicao=excluirComprovanteGRU&cogid='+cogid;
	}
	
}
</script>

<form method="post" id="formulariogru" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="inserirComprovanteGRU">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Anexar comprovante GRU</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Orientações</td>
	<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">CPF do bolsista</td>
	<td><?=campo_texto('cogcpf', "S", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="cogcpf"', '', '', ''); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Período de referência</td>
	<td>
	<?
	$sql = "SELECT f.fpbid as codigo, 'Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia as descricao
			FROM sismedio.folhapagamento f
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia";
	
	$db->monta_combo('fpbid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'fpbid','', $_REQUEST['fpbid']);
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Perfil</td>
	<td>
	<?
	
    $sql = "SELECT p.pflcod as codigo, p.pfldsc||' - R$ '||trim(coalesce(to_char(pp.plpvalor,'999g999g999d99'),'')) as descricao FROM seguranca.perfil p 
			INNER JOIN sispacto2.pagamentoperfil pp ON pp.pflcod = p.pflcod 
			ORDER BY pfldsc";
    $db->monta_combo('pflcod', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'pflcod','', $_REQUEST['pflcod']); 
	
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Data de pagamento</td>
	<td>
	<?=campo_data2('cogdtpagamento','S', 'S', 'Data de pagamento', 'S', '', '', '', '', '', 'cogdtpagamento'); ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Anexo do comprovante GRU</td>
	<td><input type="file" name="arquivo" id="arquivo"></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">&nbsp;</td>
	<td><input type="button" name="salvar" value="Salvar" onclick="salvarComprovanteGRU();"></td>
</tr>

</table>
</form>

<?php 

$sql = "SELECT '<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirGRU('||cogid||');\"> <img src=\"../imagens/anexo.gif\" style=\"cursor:pointer;\" onclick=\"window.location=\'sispacto2.php?modulo=principal/comprovantegru&acao=A&requisicao=downloadDocumentoDesignacao&arqid='||arqid||'\';\">' as acao,
			   replace(to_char(cogcpf::numeric, '000:000:000-00'), ':', '.') as cogcpf,  
			   'Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia as referencia,
			   p.pfldsc||' - R$ '||trim(coalesce(to_char(pp.plpvalor,'999g999g999d99'),'')) as perfil,
			   to_char(cogdtpagamento,'dd/mm/YYYY') as cogdtpagamento
		
		FROM sispacto2.comprovantegru c
		INNER JOIN sismedio.folhapagamento f ON f.fpbid = c.fpbid 
		INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia 
		INNER JOIN seguranca.perfil p ON p.pflcod = c.pflcod 
		INNER JOIN sispacto2.pagamentoperfil pp ON pp.pflcod = p.pflcod 
		WHERE c.cogstatus='A' AND c.cogcpfinsercao='".$_SESSION['usucpf']."'";

$cabecalho = array("&nbsp;","CPF do bolsista","Período de referência","Perfil","Data do pagamento");
$db->monta_lista($sql,$cabecalho,100,10,'N','center','N','formulario','','',null,array('ordena'=>false));


?>
