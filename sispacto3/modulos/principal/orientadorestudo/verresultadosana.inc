<script>

function acessarResultadoANA(inep) {
	window.location='sispacto3.php?modulo=principal/orientadorestudo/orientadorestudo&acao=A&aba=verresultadosana&tpacodigoescola='+inep;
}

</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" width="30%">Orientações</td>
	<td><? echo carregarOrientacao("/sispacto3/sispacto3.php?modulo=".$_REQUEST['modulo']."&acao=A&aba=verresultadosana"); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Escolas:</td>
	<td>
	<?

	$sql = "SELECT DISTINCT tpacodigoescola as codigo, tpanomeescola as descricao 
			FROM sispacto3.turmasprofessoresalfabetizadores 
			WHERE iusd IN(
			SELECT ot.iusd FROM sispacto3.turmas t 
			INNER JOIN sispacto3.orientadorturma ot ON ot.turid = t.turid 
			WHERE t.iusd='".$_SESSION['sispacto3']['orientadorestudo']['iusd']."'
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