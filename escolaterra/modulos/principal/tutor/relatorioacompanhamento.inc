<script>
function gravarRelatorioAcompanhamento() {
	if(jQuery('#ractutacoesacompanhamento').val()=='') {
		alert('Preencha as  Ações de Acompanhamento');
		return false;
	}

	if(jQuery('#ractutdificuldadesencontradas').val()=='') {
		alert('Preencha os Dificuldades enfrentadas');
		return false;
	}

	if(jQuery('#ractutobservacoes').val()=='') {
		alert('Preencha as Observações');
		return false;
	}

	if(jQuery('#ractutavancospraticasobservadas').val()=='') {
		alert('Preencha os Avanços nas Práticas Observadas');
		return false;
	}
	
	var input = document.createElement("input");
	input.setAttribute("type", "hidden");
	input.setAttribute("name", "iusid");
	input.setAttribute("value", "<?=$_SESSION['escolaterra']['tutor']['iusid'] ?>");
	document.getElementById("formulario_relatorioacompanhamento").appendChild(input);

	var input = document.createElement("input");
	input.setAttribute("type", "hidden");
	input.setAttribute("name", "fpbid");
	input.setAttribute("value", "<?=$_REQUEST['fpbid'] ?>");
	document.getElementById("formulario_relatorioacompanhamento").appendChild(input);
	
	
	document.getElementById("formulario_relatorioacompanhamento").submit();

	
}
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Principal</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao("/escolaterra/escolaterra.php?modulo=principal/tutor/tutorexecucao&acao=A&aba=relatorioacompanhamento"); ?></td>
</tr>
<tr>
	<td colspan="2" class="SubTituloCentro">
	<?
	carregarPeriodoReferencia(array('ufpid'=>$_SESSION['escolaterra']['tutor']['ufpid'],'fpbid'=>$_REQUEST['fpbid']));
	?>
	</td>
</tr>
<? if($_REQUEST['fpbid']) : ?>


<?

$sql = "SELECT * FROM escolaterra.relatorioacompanhamento WHERE iusid='".$_SESSION['escolaterra']['tutor']['iusid']."' AND fpbid='".$_REQUEST['fpbid']."'";
$relatorioacompanhamento = $db->pegaLinha($sql);

if(!$relatorioacompanhamento) {

	$docid = wf_cadastrarDocumento( TPD_RELATORIO, 'RELATÓRIO : '.$_SESSION['escolaterra']['tutor']['descricao'].' FPBID: '.$_REQUEST['fpbid'].' IUSD:'.$_SESSION['escolaterra']['tutor']['iusid'] );
	
	$relatorioacompanhamento['docid'] = $docid;

	$sql = "INSERT INTO escolaterra.relatorioacompanhamento(
            iusid, docid, fpbid)
    		VALUES ('".$_SESSION['escolaterra']['tutor']['iusid']."', {$docid}, '".$_REQUEST['fpbid']."');";
	
	$db->executar($sql);
	$db->commit();
} else {
	extract($relatorioacompanhamento);
}




?>
<tr>
	<td colspan="2">
	<form method="post" name="formulario_relatorioacompanhamento" id="formulario_relatorioacompanhamento">
	<input type="hidden" name="requisicao" value="gravarRelatorioAcompanhamento">
	
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td colspan="3" class="SubTituloCentro">Descrição das Atividades Realizadas de Acompanhamento Pedagógico</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">1. Ações de Acompanhamento:</td>
		<td><? echo campo_textarea( 'ractutacoesacompanhamento', 'S', 'S', '', '70', '4', '2000'); ?></td>
		<td rowspan="3" valign="top" width="5%">
		<?
		/* Barra de estado atual e ações e Historico */
		wf_desenhaBarraNavegacao( $relatorioacompanhamento['docid'], array('fpbid' => $_REQUEST['fpbid']) );
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">2. Dificuldades enfrentadas:</td>
		<td><? echo campo_textarea( 'ractutdificuldadesencontradas', 'S', 'S', '', '70', '4', '2000'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">3. Observações:</td>
		<td><? echo campo_textarea( 'ractutobservacoes', 'S', 'S', '', '70', '4', '2000'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">4. Avanços nas Práticas Observadas:</td>
		<td><? echo campo_textarea( 'ractutavancospraticasobservadas', 'S', 'S', '', '70', '4', '2000'); ?></td>
	</tr>
	</table>
	
	</form>
	</td>
</tr>


<? endif; ?>
<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td><input type="button" value="Anterior" onclick="divCarregando();window.location='<?=$goto_pro ?>';"> <input type="button" value="Gravar" onclick="gravarRelatorioAcompanhamento();"></td>
</tr>
</table>