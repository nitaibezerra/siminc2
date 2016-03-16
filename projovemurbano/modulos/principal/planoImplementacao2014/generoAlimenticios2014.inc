<?

criaSessaoGeneroAlimenticios();
$sql = "SELECT * FROM projovemurbano.generoalimenticio 
		WHERE galid = '".$_SESSION['projovemurbano']['galid']."'";

$generoalimenticio = $db->pegaLinha($sql);
$sql = "SELECT * FROM projovemurbano.lancherefeicao 
		WHERE galid = '".$_SESSION['projovemurbano']['galid']."'";

$lancherefeicao = $db->pegaLinha($sql);

if($_SESSION['projovemurbano']['muncod']) {
	$linkproximo = "Municipio";
} elseif ($_SESSION['projovemurbano']['estuf']) {
	$linkproximo = "Estado";
}

$sugestaoampliacao = carregarSugestaoAmpliacao();
$meta = carregarMeta($sugestaoampliacao);

$habilita = 'S';
$perfil = pegaPerfilGeral();
if(in_array(PFL_CONSULTA, $perfil)){
	$habilita = 'N';
}

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
function gravarGeneroAlimenticio() {

	if(parseFloat(document.getElementById('galpercutilizado').value) > parseFloat(document.getElementById('galpercmax').value)) {
		alert('Percentual Utilizado(%) maior do que Percentual Máximo previsto(%)');
		return false;
	}
	
    var mes = document.getElementById('lreqtdmeses').value;
    if(mes > 18){
        alert('Por favor informe uma quantidade menor ou igual à 18 mêses');
        return false;
    }
    
    
	var vlr1 = parseFloat(replaceAll(replaceAll(document.getElementById('span_totalmaximo').innerHTML,".",""),",","."));
	var vlr2 = parseFloat(replaceAll(replaceAll(document.getElementById('span_totalutilizado').innerHTML,".",""),",","."));
	
	if(vlr2>vlr1) {
		alert('Valor Utilizado maior do que Valor Máximo previsto');
		return false;
	}


	jQuery('#form').submit();
}

function somarValorTotalGenAlim() {
	var total=0;
	if(document.getElementById('lrevlrreflanche').value!='') {
		var lreqtdcrianca=0;
		if(document.getElementById('lreqtdcrianca').value!='') {
			lreqtdcrianca = parseFloat(document.getElementById('lreqtdcrianca').value);
		}
		var lremeta=0;
		if(document.getElementById('lremeta').value!='') {
			lremeta = parseFloat(document.getElementById('lremeta').value);
		}
		var valor = parseFloat(replaceAll(replaceAll(document.getElementById('lrevlrreflanche').value,".",""),",","."));
		total = valor*(lreqtdcrianca+lremeta)*document.getElementById('lreqtdmeses').value;

	}
	if(total>0) {
		document.getElementById('lrevlrtotal').value = mascaraglobal('###.###.###,##',total.toFixed(2));
	} else {
		document.getElementById('lrevlrtotal').value = '';
	}
	

}

function calculaPorcentTotal() {
	var lrevlrtotal=0;
	if(document.getElementById('lrevlrtotal').value!='') {
		lrevlrtotal 	= parseFloat(replaceAll(replaceAll(document.getElementById('lrevlrtotal').value,".",""),",","."));
	}
	
	var montante = parseFloat('<?=calcularMontante($meta) ?>');
	
	var total = lrevlrtotal;
	
	var final = (total*100)/montante;
	
	
	document.getElementById('span_totalutilizado').innerHTML = mascaraglobal('###.###.###,##',total.toFixed(2));
	document.getElementById('galpercutilizado').value = final.toFixed(1);

}

jQuery(document).ready(function() {
<? if($esdid!=ESD_EMELABORACAO && !$db->testa_superuser()) : ?>
<? $desabilitado = true; ?>
	jQuery("[name^='lreqtdcrianca']").attr("disabled","disabled");
	jQuery("[name^='lreqtdcrianca']").attr("className","disabled");
	jQuery("[name^='lrevlrreflanche']").attr("disabled","disabled");
	jQuery("[name^='lrevlrreflanche']").attr("className","disabled");
<? endif; ?>
	calculaPorcentTotal();
	jQuery('input').blur(function(){
		calculaPorcentTotal();
	});
});
</script>
<form id="form" name="form" method="POST">
<input type="hidden" name="requisicao" value="gravarGeneroAlimenticio">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="35%">Orientações:</td>
		<td>
			<font color=blue>
				<?=$arrOrientacoes[6] ?>
			</font>
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
			<td align="center"><? echo campo_texto('galpercmax', 'N', $habilita, 'Percentual Máximo previsto(%)', 6, 5, "###.#", "", '', '', 0, 'id="galpercmax"', '', '5.0', '' ); ?> R$ <span id="span_totalmaximo"><?=number_format(calcularMontante($meta)*('5.0')/100,2,",",".") ?></span></td>
			<td align="center"><? echo campo_texto('galpercutilizado', $habilita, 'N', 'Percentual utilizado(%)', 6, 5, "###.#", "", '', '', 0, 'id="galpercutilizado"', '', (($generoalimenticio['galpercutilizado'])?$generoalimenticio['galpercutilizado']:'0.0'), '' ); ?> R$ <span id="span_totalutilizado"><?=number_format($lancherefeicao['lrevlrtotal'],2,",",".") ?></span></td>
		</tr>
		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Valor do Lanche ou Refeição</td>
		<td>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
		<tr>
			<td class="SubTituloCentro">Meta</td>
			<td class="SubTituloCentro">Crianças filhas de estudantes(10% da meta)</td>
			<td class="SubTituloCentro">Meses</td>
			<td class="SubTituloCentro">Valor mensal do lanche ou Refeição por pessoa(R$)</td>
			<td class="SubTituloCentro">Valor total(R$)</td>
		</tr>
		<tr>
			<td align="center"><? echo campo_texto('$meta', 'N', 'N', 'Meta', 6, 5, "###", "", '', '', 0, 'id="lremeta"', '', $meta, '' ); ?></td>
			<td align="center"><? echo campo_texto('lreqtdcrianca', 'N', $habilita, 'Crianças filhas de estudantes(10% da meta)', 8, 7, "#######", "", '', '', 0, 'id="lreqtdcrianca"', 'if(this.value>'.(($lancherefeicao['lremeta'])?round(($lancherefeicao['lremeta']*0.1),2):round(($meta*0.1),0)).'){alert(\'Não pode ser maior que '.(($lancherefeicao['lremeta'])?round(($lancherefeicao['lremeta']*0.1),0):round(($meta*0.1),0)).'\');this.value=\'\';}somarValorTotalGenAlim();calculaPorcentTotal();', $lancherefeicao['lreqtdcrianca'], '' ); ?></td>
			<td align="center"><? echo campo_texto('lreqtdmeses', 'N', 'S', 'Meses', 8, 7, "##", "", '', '', 0, 'id="lreqtdmeses"', 'somarValorTotalGenAlim();calculaPorcentTotal();', (($lancherefeicao['lreqtdmeses'])?$lancherefeicao['lreqtdmeses']:'18'), '' ); ?></td>
			<td align="center"><? echo campo_texto('lrevlrreflanche', 'N', $habilita, 'Valor mensal do lanche ou Refeição por pessoa(R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="lrevlrreflanche"', 'somarValorTotalGenAlim();calculaPorcentTotal();', (($lancherefeicao['lrevlrreflanche'])?number_format($lancherefeicao['lrevlrreflanche'],2,",","."):''), '' ); ?></td>
			<td align="center"><? echo campo_texto('lrevlrtotal', 'N', 'N', 'Valor total(R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="lrevlrtotal"', '', (($lancherefeicao['lrevlrtotal'])?number_format($lancherefeicao['lrevlrtotal'],2,",","."):''), '' ); ?></td>
		</tr>
		</table>
		
		</td>
	</tr>

	
	<tr>
		<td class="SubTituloCentro" colspan="2">
		<input type="button" name="anterior" value="Anterior" onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao2014&acao=A&aba=<?php echo $_GET['aba'] ?>&aba2=formacaoEducadores';">
		<? if(!$desabilitado) : ?> 
			<input type="button" name="salvar" value="Salvar" onclick="gravarGeneroAlimenticio();">
		<? endif; ?> 
		<input type="button" name="proximo" value="Próximo" onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao2014&acao=A&aba=<?php echo $_GET['aba'] ?>&aba2=qualificacaoProfissional<?=$linkproximo ?>';"></td>
	</tr>
</table>
</form>
<? registarUltimoAcesso(); ?>