<script>

function acessarResultadoANA(inep) {
	window.location='sispacto2.php?modulo=principal/formadories/formadories&acao=A&aba=verresultadosana&tpacodigoescola='+inep;
}

</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" width="30%">Orientações</td>
	<td><? echo carregarOrientacao("/sispacto2/sispacto2.php?modulo=".$_REQUEST['modulo']."&acao=A&aba=verresultadosana"); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Escolas:</td>
	<td>
	<?

	$sql = "SELECT DISTINCT tpacodigoescola as codigo, tpanomeescola as descricao 
			FROM sispacto2.turmasprofessoresalfabetizadores 
			WHERE iusd IN(
			SELECT ot2.iusd FROM sispacto2.turmas t 
			INNER JOIN sispacto2.orientadorturma ot ON ot.turid = t.turid 
			INNER JOIN sispacto2.turmas t2 ON t2.iusd = ot.iusd 
			INNER JOIN sispacto2.orientadorturma ot2 ON ot2.turid = t2.turid  
			WHERE t.iusd='".$_SESSION['sispacto2']['formadories']['iusd']."'
			) ORDER BY 2";
	
	$db->monta_combo('tpacodigoescola', $sql, 'S', 'Selecione', 'acessarResultadoANA', '', '', '', 'S', 'tpacodigoescola', '', $_REQUEST['tpacodigoescola']);
	
	?>
	</td>
</tr>
</table>
<?

if($_REQUEST['tpacodigoescola']) :

include_once APPRAIZ_SISPACTO."resultadosana.inc";

endif;

?>