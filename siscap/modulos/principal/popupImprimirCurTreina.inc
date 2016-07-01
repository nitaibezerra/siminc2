<?php
$alucpf = $_SESSION['usucpf'];

monta_titulo( 'Cursos e Treinamentos', '' );

$sql = "SELECT cu.curdsc, to_char(tu.turdtini, 'DD/MM/YYYY')||' a '||to_char(tu.turdtfim, 'DD/MM/YYYY') as periodo, 
		    tu.turcarga, tu.turhrini||' às '||tu.turhrfim as horario, 
		    case when tu.turmodalidade = 'PR' then 'Presencial'
		    end as modalidade, tu.turid
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
	
	$dados_array[$key] = array("curso" => $v['curdsc'],
								 "periodo" => $v['periodo'],
								 "turcarga" => '<center>'.$v['turcarga'].' hs'.'</center>',
								 "horario" => $v['horario'],
								 "modalidade" => $v['modalidade'],
								 "facnome" => implode( ',<br>', $arFacnome )
						);
}

$cabecalho = array("Curso", "Período", "Carga Horária (horas)", "Horário", "Modalidade", "Facilitador");
$db->monta_lista_array($dados_array, $cabecalho, 25, 10, '', 'center', '');	

/*$arDados = $db->carregar( $sql );
$arDados = $arDados ? $arDados : array();*/
?>
<style type="">
	@media print {.notprint { display: none } .div_rolagem{display: none} }	
	@media screen {.notscreen { display: none; }
	
	.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
	
</style>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<?/*foreach ($arDados as $key => $v) { ?>
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<tr>
		<td class="subtitulodireita" width="15%"><b>Sequêncial:</b></td>
		<td width="35%"><?=$key+1; ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita" width="15%"><b>Curso:</b></td>
		<td width="35%"><?=$v['curdsc']; ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Período:</b></td>
		<td><?=$v['periodo']; ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Carga Horária:</b></td>
		<td><?=$v['turcarga']; ?> hora(s)</td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Horário:</b></td>
		<td><?=$v['horario']; ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Modalidade:</b></td>
		<td><?=$v['modalidade']; ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Facilitador:</b></td>
		<td><?=$v['facnome']; ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Situação:</b></td>
		<td><?=$v['']; ?></td>
	</tr>
</table>
<?}*/ ?>
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<tr bgcolor="#D0D0D0">
		<td colspan="6" style="text-align: center" class="div_rolagem">
			<input type="button" id="btImprimir" value="Imprimir" onclick="javascript: window.print();" />
			<input type="button" name="fechar" value="Fechar" onclick="javascript: window.close();">
		</td>
	</tr>
</table>