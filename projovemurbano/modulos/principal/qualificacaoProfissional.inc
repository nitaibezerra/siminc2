<?

$sql = "SELECT * FROM projovemurbano.qualificacaoprofissioanal WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'";
$qualificacaoprofissioanal = $db->pegaLinha($sql);

if(!$qualificacaoprofissioanal) {
	$sql = "INSERT INTO projovemurbano.qualificacaoprofissioanal(
            pjuid)
    		VALUES ('".$_SESSION['projovemurbano']['pjuid']."') RETURNING qprid;";
	
	$_SESSION['projovemurbano']['qprid'] = $db->pegaUm($sql);

} else {
	$_SESSION['projovemurbano']['qprid'] = $generoalimenticio['qprid'];	
}

$habilita = 'S';
$perfil = pegaPerfilGeral();
if(in_array(PFL_CONSULTA, $perfil)){
	$habilita = 'N';
}
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
function gravarQualificacaoProfissioanal() {

	jQuery('#form').submit();
}

</script>
<form id="form" name="form" method="POST">
<input type="hidden" name="requisicao" value="gravarGeneroAlimenticio">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="35%">Orientações:</td>
		<td><p>momomo momomomom omomoomommo momomomomomomo mommomomoom momomomomomo momomomo.</p>
			<p>momomo momomomom omomoomommo momomomomomomo mommomomoom momomomomomo momomomo.</p>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Percentual da Ação</td>
		<td>
		
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
		<tr>
			<td class="SubTituloCentro">Percentual Máximo previsto(%)</td>
			<td class="SubTituloCentro">Percentual utilizado(%)</td>
		</tr>
		<tr>
			<td align="center"><? echo campo_texto('qprpercmax', 'S', $habilita, 'Percentual Máximo previsto(%)', 6, 5, "###.#", "", '', '', 0, 'id="galpercmax"', '', $generoalimenticio['galpercmax'], '' ); ?></td>
			<td align="center"><? echo campo_texto('qprpercutilizado', 'S', $habilita, 'Percentual utilizado(%)', 6, 5, "###.#", "", '', '', 0, 'id="galpercutilizado"', '', $generoalimenticio['galpercutilizado'], '' ); ?></td>
		</tr>
		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo de oferta</td>
		<td>
		<p><input type="checkbox" name="" value=""> Arcos</p>
		<p><input type="button" name="" value="Inserir"></p>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
		<tr>
			<td class="SubTituloCentro">Meta</td>
			<td class="SubTituloCentro">Crianças filhas de estudantes(10% da meta)</td>
			<td class="SubTituloCentro">Meses</td>
			<td class="SubTituloCentro">Valor mensal do lanche ou Refeição por pessoa(R$)</td>
			<td class="SubTituloCentro">Valor total(R$)</td>
		</tr>
		<tr>
			<td align="center"><? echo campo_texto('lremeta', 'S', $habilita, 'Percentual Máximo previsto(%)', 6, 5, "###.#", "", '', '', 0, 'id="lremeta"', '', $lancherefeicao['lremeta'], '' ); ?></td>
			<td align="center"><? echo campo_texto('lreqtdcrianca', 'S', $habilita, 'Percentual utilizado(%)', 6, 5, "###.#", "", '', '', 0, 'id="lreqtdcrianca"', '', $lancherefeicao['lreqtdcrianca'], '' ); ?></td>
			<td align="center"><? echo campo_texto('lreqtdmeses', 'S', $habilita, 'Percentual utilizado(%)', 8, 7, "#######", "", '', '', 0, 'id="lreqtdmeses"', '', $lancherefeicao['lreqtdmeses'], '' ); ?></td>
			<td align="center"><? echo campo_texto('lrevlrreflanche', 'S', $habilita, 'Percentual utilizado(%)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="lrevlrreflanche"', '', number_format($lancherefeicao['lrevlrreflanche'],2,",","."), '' ); ?></td>
			<td align="center"><? echo campo_texto('lrevlrtotal', 'S', $habilita, 'Percentual Máximo previsto(%)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="lrevlrtotal"', '', number_format($lancherefeicao['lrevlrtotal'],2,",","."), '' ); ?></td>
		</tr>
		</table>
		<p><input type="checkbox" name="" value=""> Programas Nacionais de Educação Técnica em Formação Inicila e Continuada</p>
		<p><input type="button" name="" value="Inserir"></p>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
		<tr>
			<td class="SubTituloCentro">Meta</td>
			<td class="SubTituloCentro">Crianças filhas de estudantes(10% da meta)</td>
			<td class="SubTituloCentro">Meses</td>
			<td class="SubTituloCentro">Valor mensal do lanche ou Refeição por pessoa(R$)</td>
			<td class="SubTituloCentro">Valor total(R$)</td>
		</tr>
		<tr>
			<td align="center"><? echo campo_texto('lremeta', 'S', $habilita, 'Percentual Máximo previsto(%)', 6, 5, "###.#", "", '', '', 0, 'id="lremeta"', '', $lancherefeicao['lremeta'], '' ); ?></td>
			<td align="center"><? echo campo_texto('lreqtdcrianca', 'S', $habilita, 'Percentual utilizado(%)', 6, 5, "###.#", "", '', '', 0, 'id="lreqtdcrianca"', '', $lancherefeicao['lreqtdcrianca'], '' ); ?></td>
			<td align="center"><? echo campo_texto('lreqtdmeses', 'S', $habilita, 'Percentual utilizado(%)', 8, 7, "#######", "", '', '', 0, 'id="lreqtdmeses"', '', $lancherefeicao['lreqtdmeses'], '' ); ?></td>
			<td align="center"><? echo campo_texto('lrevlrreflanche', 'S', $habilita, 'Percentual utilizado(%)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="lrevlrreflanche"', '', number_format($lancherefeicao['lrevlrreflanche'],2,",","."), '' ); ?></td>
			<td align="center"><? echo campo_texto('lrevlrtotal', 'S', $habilita, 'Percentual Máximo previsto(%)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="lrevlrtotal"', '', number_format($lancherefeicao['lrevlrtotal'],2,",","."), '' ); ?></td>
		</tr>
		</table>
		
		</td>
	</tr>

	
	<tr>
		<td class="SubTituloCentro" colspan="2">
		<?php if( $habilita == 'S' ){ ?>
			<input type="button" name="salvar" value="Salvar" onclick="gravarGeneroAlimenticio();"></td>
		<?php }?>
	</tr>
</table>
</form>
<? registarUltimoAcesso(); ?>
