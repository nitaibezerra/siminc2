<?

include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/escolaterra.js"></script>';

echo "<br>";


monta_titulo( "Efetuando Pagamentos", "Lista de envio bolsas para o Sistema de Gestão de Bolsas(SGB) - FNDE");

?>
<script>

function carregarResumo() {
	var filtro='';
	if(jQuery('#uniid').val()!='') {
		filtro+='&uniid='+jQuery('#uniid').val();
	}
	
	if(jQuery('#pflcod').val()!='') {
		filtro+='&pflcod='+jQuery('#pflcod').val();
	}
}

function selecionarEstado(ufpid) {
	window.location='escolaterra.php?modulo=principal/pagamentos/pagamentos&acao=A&ufpid='+ufpid;
}

function listaPagamentos() {

	if(jQuery('#pflcod').val()=='') {
		alert('Selecione um perfil');
		return false;
	}
	
	var formulario = document.formulario;
	// submete formulario
	formulario.target = 'listapagamentos';
	var janela = window.open( '', 'listapagamentos', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
	formulario.submit();
	janela.focus();
}
</script>
<form method="post" name="formulario" id="formulario" action="escolaterra.php?modulo=principal/pagamentos/listapagamentos&acao=A">
<input type="hidden" name="ufpid" id="ufpid" value="<?=$_REQUEST['ufpid'] ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td colspan="2"><? 
	$sql = "SELECT foo.estado, sum(foo.ce) as ce, sum(foo.tu) FROM (
			SELECT  est.estuf||' - '||est.estdescricao as estado, CASE WHEN p.pflcod=".PFL_COORDENADORESTADUAL." THEN 1 ELSE 0 END as ce, CASE WHEN p.pflcod=".PFL_TUTOR." THEN 1 ELSE 0 END as tu FROM escolaterra.pagamentobolsista p 
			INNER JOIN escolaterra.ufparticipantes ufp ON ufp.ufpid = p.ufpid 
			INNER JOIN territorios.estado est ON est.estuf = ufp.estuf 
			INNER JOIN workflow.documento d ON d.docid = p.docid 
			WHERE (d.esdid=".ESD_PAGAMENTO_APTO." OR d.esdid=".ESD_PAGAMENTO_RECUSADO.")
			) foo GROUP BY foo.estado 
			ORDER BY foo.estado";
	
	$cabecalho = array("UF","Coordenador Estadual","Tutor Municipal");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','',true, false, false, true);
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="30%">UF:</td>
	<td><?
		
	$sql = "SELECT u.ufpid as codigo, e.estuf||' - '||e.estdescricao as descricao 
			FROM escolaterra.ufparticipantes u 
			INNER JOIN territorios.estado e ON e.estuf = u.estuf  
			ORDER BY e.estuf";
	
	$db->monta_combo('ufpid', $sql, 'S', 'Selecione', 'selecionarEstado', '', '', '', 'S', 'ufpid', '', $_REQUEST['ufpid']);
				
	?></td>
</tr>
<? if($_REQUEST['ufpid']) : ?>
<tr>
	<td colspan="2"><? 
	$sql = "SELECT foo.periodo, sum(foo.ce) as ce, sum(foo.tu) FROM (
			SELECT to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'01')::date,'YYYYmmdd') as ref, 'Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia as periodo, CASE WHEN p.pflcod=".PFL_COORDENADORESTADUAL." THEN 1 ELSE 0 END as ce, CASE WHEN p.pflcod=".PFL_TUTOR." THEN 1 ELSE 0 END as tu FROM escolaterra.pagamentobolsista p 
			INNER JOIN escolaterra.periodoreferencia f ON f.fpbid = p.fpbid 
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia 
			INNER JOIN workflow.documento d ON d.docid = p.docid 
			WHERE ufpid='".$_REQUEST['ufpid']."' AND (d.esdid=".ESD_PAGAMENTO_APTO." OR d.esdid=".ESD_PAGAMENTO_RECUSADO.")
			) foo GROUP BY foo.periodo, foo.ref 
			ORDER BY foo.ref";
	
	$cabecalho = array("Período de referência","Coordenador Estadual","Tutor Municipal");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','',true, false, false, true);
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Selecione período de referência:</td>
	<td><? 
	carregarPeriodoReferencia(array('ufpid'=>$_REQUEST['ufpid'],'fpbid'=>$_REQUEST['fpbid'],'somentecombo'=>true));
	?>
	</td>
</tr>
<? if($_REQUEST['fpbid']) : ?>
<tr>
	<td class="SubTituloCentro" colspan="2">
	<? exibirSituacaoPagamento(array('ufpid'=>$_REQUEST['ufpid'],'fpbid'=>$_REQUEST['fpbid'])); ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Selecione um Perfil</td>
	<td><?
	$sql = "SELECT p.pflcod as codigo, p.pfldsc || ' ( '||COALESCE((SELECT count(*) FROM escolaterra.pagamentobolsista pb INNER JOIN workflow.documento d ON d.docid = pb.docid WHERE (d.esdid=".ESD_PAGAMENTO_APTO." OR d.esdid=".ESD_PAGAMENTO_RECUSADO.") AND pb.pflcod = p.pflcod AND pb.ufpid='".$_REQUEST['ufpid']."' AND pb.fpbid='".$_REQUEST['fpbid']."'),0)||' )' as descricao FROM seguranca.perfil p 
			INNER JOIN escolaterra.pagamentoperfil pp ON pp.pflcod = p.pflcod 
			WHERE p.sisid='".SIS_ESCOLATERRA."'";
	
	$db->monta_combo('pflcod', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'pflcod', '', $_REQUEST['pflcod']);
	?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" value="Visualizar lista de pagamentos" onclick="listaPagamentos();"></td>
</tr>
<? endif; ?>
<? endif; ?>
</table>
</form>