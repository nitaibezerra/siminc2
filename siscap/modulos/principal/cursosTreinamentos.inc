<?php
$alucpf = $_SESSION['usucpf'];

include APPRAIZ . 'includes/cabecalho.inc';
print "<br>";

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Minhas Capacitações', '' );

$sql = "SELECT cu.curdsc, to_char(tu.turdtini, 'DD/MM/YYYY')||' a '||to_char(tu.turdtfim, 'DD/MM/YYYY') as periodo,
		    tu.turcarga, tu.turhrini, tu.turhrfim,
		    case when tu.turmodalidade = 'PR' then 'Presencial'
		         when tu.turmodalidade = 'SP' then 'Semipresencial'
		         when tu.turmodalidade = 'AD' then 'A distância'
		    end as modalidade, tu.turid, cu.curid
		FROM
		    siscap.curso cu
		    inner join siscap.turma tu on tu.curid = cu.curid
		    inner join siscap.aluno_turma atu on atu.turid = tu.turid
		WHERE atu.alucpf = '".$alucpf."'";
$arDados = $db->carregar( $sql );
$arDados = $arDados ? $arDados : array();

foreach ($arDados as $key => $v) {

	$sql = "SELECT fa.facnome
			FROM siscap.facilitador fa
				inner join siscap.turma_facilitador tf on tf.facid = fa.facid
			WHERE tf.turid = ".$v['turid'];
	$arFacnome = $db->carregarColuna( $sql );
	$arFacnome = $arFacnome ? $arFacnome : array();

	$arHoraIni = explode( ':', $v['turhrini'] );
	$arHoraFim = explode( ':', $v['turhrfim'] );

	$horaini = $arHoraIni[0].'h'.$arHoraIni[1];
	$horafim = $arHoraFim[0].'h'.$arHoraFim[1];

	$dados_array[$key] = array("Acao" => '<center><a href="" onclick="informacaoCursoTurma('.$v['turid'].','.$v['curid'].');"><img src="/imagens/report.gif" height="15" border=0 alt="Ir" title="Visualizar a ementa do curso"></a></center>',
								"curso" => $v['curdsc'],
								"periodo" => $v['periodo'],
								"turcarga" => '<center>'.$v['turcarga'].'h/a'.'</center>',
								"horario" => $horaini.' a '.$horafim,
								"modalidade" => $v['modalidade'],
								"facnome" => implode( ',<br>', $arFacnome )
						);
}

$cabecalho = array("Ação", "Curso", "Período", "Carga Horária", "Horário", "Modalidade", "Facilitador");
$db->monta_lista_array($dados_array, $cabecalho, 25, 10, '', 'center', '');
?>
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<tr bgcolor="#D0D0D0">
		<td colspan="6" style="text-align: center">
			<input type="button" id="btImprimir" value="Visualizar Impressão" onclick="imprimir();" />
			<input type="button" id="btVoltar" value="Voltar" onclick="javascript: window.location.href = 'siscap.php?modulo=principal/cronogramaCurso&acao=A'" />
		</td>
	</tr>
</table>
<script>
function imprimir(alucpf){
	window.open('siscap.php?modulo=principal/popupImprimirCurTreina&acao=A','page','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no, height=450, width=1010');
}
function informacaoCursoTurma(turid, curid){
	window.open('siscap.php?modulo=principal/informacaoTurma&acao=A&turid='+turid+'&curid='+curid+'&popup=popup','page','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no, height=650, width=900');
}
</script>