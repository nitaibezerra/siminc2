<?

include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sisindigena2.js"></script>';

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

function selecionarUniversidade(uncid) {
	window.location='sisindigena2.php?modulo=principal/pagamentos/pagamentos&acao=A&uncid='+uncid;
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
<form method="post" name="formulario" id="formulario" action="sisindigena2.php?modulo=principal/pagamentos/listapagamentos&acao=A">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" width="30%">Universidade</td>
	<td><?
	$perfis = pegaPerfilGeral();
	
	if(in_array(PFL_COORDENADORIES,$perfis)) {
		
		if(!$_SESSION['sisindigena2']['universidade']['uncid']) {
			$al = array("alert"=>"Universidade não identificada","location"=>"sisindigena2.php?modulo=inicio&acao=C");
			alertlocation($al);
		}
		
		$sql = "SELECT un.uncid as codigo, u.unisigla||' - '||u.uninome as descricao 
				FROM sisindigena2.universidade u 
				INNER JOIN sisindigena2.universidadecadastro un ON un.uniid = u.uniid 
				WHERE un.uncid='".$_SESSION['sisindigena2']['universidade']['uncid']."'";
		
		$arrUn = $db->pegaLinha($sql);
		
		echo $arrUn['descricao'];
		
		if(!$_REQUEST['uncid'])  {
			echo "<script>jQuery(document).ready(function() {
					selecionarUniversidade(".$arrUn['codigo'].");
				  });</script>";
		}
		
	} else {
		
		$sql = "SELECT un.uncid as codigo, u.unisigla||' - '||u.uninome as descricao 
				FROM sisindigena2.universidade u 
				INNER JOIN sisindigena2.universidadecadastro un ON un.uniid = u.uniid 
				ORDER BY uninome";
		
		$db->monta_combo('uncid', $sql, 'S', 'Selecione', 'selecionarUniversidade', '', '', '', 'S', 'uniid', '', $_REQUEST['uncid']);
				
	}
	?></td>
</tr>
<? if($_REQUEST['uncid']) : ?>
<? $uniid = $db->pegaUm("SELECT uniid FROM sisindigena2.universidadecadastro WHERE uncid='".$_REQUEST['uncid']."'"); ?>
<? echo "<input type=hidden name=\"uniid\" value=\"".$uniid."\">"; ?>
<tr>
	<td class="SubTituloDireita">Selecione período de referência:</td>
	<td><? 
	carregarPeriodoReferencia(array('uncid'=>$_REQUEST['uncid'],'somentecombo'=>true,'fpbid'=>$_REQUEST['fpbid']))
	?>
	</td>
</tr>
<? if($_REQUEST['fpbid']) : ?>
<tr>
	<td class="SubTituloDireita">Perfil</td>
	<td><?
	$sql = "SELECT p.pflcod as codigo, p.pfldsc || ' ( '||COALESCE((SELECT count(*) FROM sisindigena2.pagamentobolsista pb INNER JOIN workflow.documento d ON d.docid = pb.docid WHERE (d.esdid=".ESD_PAGAMENTO_APTO." OR d.esdid=".ESD_PAGAMENTO_RECUSADO.") AND pb.pflcod = p.pflcod AND pb.uniid='".$uniid."' AND pb.fpbid='".$_REQUEST['fpbid']."'),0)||' )' as descricao FROM seguranca.perfil p 
			INNER JOIN sisindigena2.pagamentoperfil pp ON pp.pflcod = p.pflcod 
			WHERE p.sisid='".SIS_INDIGENA."'";
	
	$db->monta_combo('pflcod', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'pflcod', '', $_REQUEST['pflcod']);
	?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2">
	<p>Acompanhamento dos pagamentos</p>
	<? exibirSituacaoPagamento(array('uncid'=>$_REQUEST['uncid'],'fpbid'=>$_REQUEST['fpbid'])); ?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" value="Visualizar lista de pagamentos" onclick="listaPagamentos();"></td>
</tr>
<? endif; ?>
<? endif; ?>
</table>
</form>