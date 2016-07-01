<?

$meta = pegameta();

$montante = calcularMontante();

$sql = "SELECT
			gaqtdcriancas,
			gaqtdmeses,
			gavalormensal
		FROM
			projovemcampo.planodeimplementacao
		WHERE
			pimid = {$_SESSION['projovemcampo']['pimid']} ";

$generoalimenticio = $db->pegaLinha($sql);

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
	
    var mes = document.getElementById('gaqtdmeses').value;
    if(mes > 24){
        alert('Por favor informe uma quantidade menor ou igual à 24 mêses');
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
	if(document.getElementById('gavalormensal').value!='') {
		var gaqtdcriancas=0;
		if(document.getElementById('gaqtdcriancas').value!='') {
			gaqtdcriancas = parseFloat(document.getElementById('gaqtdcriancas').value);
		}
		var meta=0;
		if(document.getElementById('meta').value!='') {
			meta = parseFloat(document.getElementById('meta').value);
		}
		var valor = parseFloat(replaceAll(replaceAll(document.getElementById('gavalormensal').value,".",""),",","."));
		total = valor*(gaqtdcriancas+meta)*document.getElementById('gaqtdmeses').value;

	}
	if(total>0) {
		document.getElementById('valortotal').value = mascaraglobal('###.###.###,##',total.toFixed(2));
	} else {
		document.getElementById('valortotal').value = '';
	}
	

}

function calculaPorcentTotal() {
	var valortotal=0;
	if(document.getElementById('valortotal').value!='') {
		valortotal 	= parseFloat(replaceAll(replaceAll(document.getElementById('valortotal').value,".",""),",","."));
	}
	
	var montante = parseFloat('<?=$montante?>');
	
	var total = valortotal;
	
	var final = (total*100)/montante;
	
	
	document.getElementById('span_totalutilizado').innerHTML = mascaraglobal('###.###.###,##',total.toFixed(2));
	document.getElementById('galpercutilizado').value = final.toFixed(1);

}

jQuery(document).ready(function() {
<? if($esdid!=ESD_EMELABORACAO && !$db->testa_superuser()) : ?>
<? $desabilitado = true; ?>
	jQuery("[name^='gaqtdcriancas']").attr("disabled","disabled");
	jQuery("[name^='gaqtdcriancas']").attr("className","disabled");
	jQuery("[name^='gavalormensal']").attr("disabled","disabled");
	jQuery("[name^='gavalormensal']").attr("className","disabled");
<? endif; ?>
	<?if($generoalimenticio['gavalormensal']){?>
		somarValorTotalGenAlim();	
		calculaPorcentTotal();
	<?}?>
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
				Aquisição de gêneros alimentícios destinados exclusivamente ao fornecimento de lanche ou refeição aos jovens matriculados e frequentes, no âmbito do Programa, 
				até que o ente executor passe a receber os recursos procedentes do Programa Nacional de Alimentação Escolar (PNAE), bem como aquisição de gêneros alimentícios 
				para o fornecimento de lanche ou refeição para os filhos desses jovens, de zero a oito anos de idade, atendidos nas salas de acolhimento por todo o período de 
				implementação do curso, garantindo, em ambos os casos, qualidade compatível com a exigida no PNAE.
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
			<td align="center"><? echo campo_texto('galpercmax', 'N', $habilita, 'Percentual Máximo previsto(%)', 6, 5, "###.#", "", '', '', 0, 'id="galpercmax"', '', '5.0', '' ); ?> R$ <span id="span_totalmaximo"><?=number_format(calcularMontante()*('5.0')/100,2,",",".") ?></span></td>
			<td align="center"><? echo campo_texto('galpercutilizado', $habilita, 'N', 'Percentual utilizado(%)', 6, 5, "###.#", "", '', '', 0, 'id="galpercutilizado"', '', (($generoalimenticio['gavalormensal'])?$generoalimenticio['gavalormensal']:'0.0'), '' ); ?> R$ <span id="span_totalutilizado"><?=number_format($generoalimenticio['valortotal'],2,",",".") ?></span></td>
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
			<td align="center"><? echo campo_texto('meta', 'N', 'N', 'Meta', 6, 5, "###", "", '', '', 0, 'id="meta"', '', $meta, '' ); ?></td>
			<td align="center"><? echo campo_texto('gaqtdcriancas', 'N', $habilita, 'Crianças filhas de estudantes(10% da meta)', 8, 7, "#######", "", '', '', 0, 'id="gaqtdcriancas"', 'if(this.value>'.(($generoalimenticio['meta'])?round(($generoalimenticio['meta']*0.1),2):round(($meta*0.1),0)).'){alert(\'Não pode ser maior que '.(($generoalimenticio['meta'])?round(($generoalimenticio['meta']*0.1),0):round(($meta*0.1),0)).'\');this.value=\'\';}somarValorTotalGenAlim();calculaPorcentTotal();', $generoalimenticio['gaqtdcriancas'], '' ); ?></td>
			<td align="center"><? echo campo_texto('gaqtdmeses', 'N', 'S', 'Meses', 8, 7, "##", "", '', '', 0, 'id="gaqtdmeses"', 'somarValorTotalGenAlim();calculaPorcentTotal();', (($generoalimenticio['gaqtdmeses'])?$generoalimenticio['gaqtdmeses']:'24'), '' ); ?></td>
			<td align="center"><? echo campo_texto('gavalormensal', 'N', $habilita, 'Valor mensal do lanche ou Refeição por pessoa(R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="gavalormensal"', 'somarValorTotalGenAlim();calculaPorcentTotal();', (($generoalimenticio['gavalormensal'])?number_format($generoalimenticio['gavalormensal'],2,",","."):''), '' ); ?></td>
			<td align="center"><? echo campo_texto('valortotal', 'N', 'N', 'Valor total(R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="valortotal"', '', ((($generoalimenticio['gaqtdcriancas']+$meta)*$generoalimenticio['gavalormensal']*$generoalimenticio['gaqtdmeses'])?number_format(($generoalimenticio['gaqtdcriancas']+$meta)*$generoalimenticio['gavalormensal']*$generoalimenticio['gaqtdmeses'],2,",","."):''), '' ); ?></td>
		</tr>
		</table>
		
		</td>
	</tr>

	
	<tr>
		<td class="SubTituloCentro" colspan="2">
		<input type="button" name="anterior" value="Anterior" onclick="window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=formacaoEducadores';">
		<? if(!$desabilitado) : ?> 
			<input type="button" name="salvar" value="Salvar" onclick="gravarGeneroAlimenticio();">
		<? endif; ?> 
		<input type="button" name="proximo" value="Próximo" onclick="window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=qualificacaoProfissional<?=$linkproximo ?>';"></td>
	</tr>
</table>
</form>
<? registarUltimoAcesso(); ?>