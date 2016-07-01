<script>

function carregarEscolasMuncod(muncod) {
	window.location='sispacto2.php?modulo=<?=$_REQUEST['modulo'] ?>&acao=A&aba=verresultadosana&muncod_escola='+muncod+'&estuf_escola='+jQuery('#estuf_escola').val();
}

function carregarMunicipiosPorUFEscola(estuf) {
	if(estuf) {
		ajaxatualizar('requisicao=carregarMunicipiosPorUFAbrangenciaIES&onclick=carregarEscolasMuncod&id=muncod_escola&name=muncod_escola&estuf='+estuf+'&uncid=<?=$_SESSION['sispacto2']['universidade']['uncid'] ?>','td_municipio_escola');
	} else {
		document.getElementById('td_municipio_escola').innerHTML = "Selecione uma UF";
	}
}


function acessarResultadoANA(inep) {
	window.location='sispacto2.php?modulo=<?=$_REQUEST['modulo'] ?>&acao=A&aba=verresultadosana&estuf_escola=<?=$_REQUEST['estuf_escola']?>&muncod_escola=<?=$_REQUEST['muncod_escola']?>&tpacodigoescola='+inep;
}

</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" width="30%">Orientações</td>
	<td><? echo carregarOrientacao("/sispacto2/sispacto2.php?modulo=".$_REQUEST['modulo']."&acao=A&aba=verresultadosana"); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="30%">UF</td>
	<td>
	<? 
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado WHERE estuf IN( SELECT DISTINCT m.estuf FROM sispacto2.abrangencia a INNER JOIN territorios.municipio m ON m.muncod = a.muncod INNER JOIN sispacto2.estruturacurso e ON e.ecuid = a.ecuid WHERE e.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."' ) ORDER BY estuf";
	$db->monta_combo('estuf_escola', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUFEscola', '', '', '', 'S', 'estuf_escola', '', $_REQUEST['estuf_escola']); 
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Município</td>
	<td id="td_municipio_escola">
	<?
	
	if($_REQUEST['estuf_escola']) {
		carregarMunicipiosPorUFAbrangenciaIES(array('onclick'=>'carregarEscolasMuncod','id'=>'muncod_escola','name'=>'muncod_escola','estuf'=>$_REQUEST['estuf_escola'],'valuecombo'=>$_REQUEST['muncod_escola'],'uncid'=>$_SESSION['sispacto2']['universidade']['uncid']));
	}
	
	?>
	</td>
</tr>
<? if($_REQUEST['muncod_escola']) : ?>
<tr>
	<td class="SubTituloDireita">Escola:</td>
	<td>
	<?

	$sql = "SELECT DISTINCT id_escola as codigo, no_entidade as descricao FROM sispacto2.ana WHERE fk_cod_municipio='".$_REQUEST['muncod_escola']."'";
	$db->monta_combo('tpacodigoescola', $sql, 'S', 'Selecione', 'acessarResultadoANA', '', '', '', 'S', 'tpacodigoescola', '', $_REQUEST['tpacodigoescola']);
	
	?>
	</td>
</tr>
<? endif; ?>
</table>
<?

if($_REQUEST['tpacodigoescola']) :

include_once APPRAIZ_SISPACTO."resultadosana.inc";

endif;

?>