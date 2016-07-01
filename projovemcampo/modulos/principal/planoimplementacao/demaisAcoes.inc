<?
// $sql = "";

// $demaisacoes = $db->pegaLinha($sql);

$meta = pegameta();

$montante = calcularMontante();

$retornarTotalMaximoDemaisAcoes=true;
$totalMax = validacaoCompletaPlanoImplementacao($retornarTotalMaximoDemaisAcoes);

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
function gravarDemaisAcoes() {
	if(parseFloat(document.getElementById('deapercutilizado').value) > parseFloat(document.getElementById('deapercmax').value)) {
		alert('Percentual Utilizado(%) maior do que Percentual Máximo previsto(%)');
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

function calcularValorTotal(tdaid) {
	var qtdmeses = parseFloat(document.getElementById('qtdmeses_'+tdaid).value);
	var valorpormes=0;
	
	if(isNaN(qtdmeses)||qtdmeses==0){
		alert('O campo meses não pede ser deixado em branco.');
		jQuery('#qtdmeses_'+tdaid).val('24');
		return false;
	}
	if(document.getElementById('valorpormes_'+tdaid).value!='') {
		valorpormes 	= parseFloat(replaceAll(replaceAll(document.getElementById('valorpormes_'+tdaid).value,".",""),",","."));
	}
	
	var total = qtdmeses*valorpormes;
	
	document.getElementById('valortotal_'+tdaid).value = mascaraglobal('###.###.###,##',total.toFixed(2));
}

function calculaPorcentTotal() {
	
	var total=0;
	var montante = parseFloat('<?=$montante ?>');
	<? 
	$sql = "SELECT tdaid FROM projovemcampo.tiposdemaisacoes --WHERE acostatus='A' AND ppuid = 3";
	$tdaids = $db->carregarColuna($sql);
	?>
	<?if($tdaids) :?>
	
		<? foreach($tdaids as $tdaid) : ?>
		
			var valortotal_<?=$tdaid ?>=0;
			if(document.getElementById('valortotal_<?=$tdaid ?>').value!='') {
				
				var idapercmaxprevisto_<?=$tdaid ?> =0;
				valortotal_<?=$tdaid ?> 	= parseFloat(replaceAll(replaceAll(document.getElementById('valortotal_<?=$tdaid ?>').value,".",""),",","."));
				
		}
		total += valortotal_<?=$tdaid ?>;
		<? endforeach; ?>
	<? endif; ?>
	
	var montante = parseFloat('<?=$montante ?>');
	
	var final = (total*100)/montante;
	
	document.getElementById('span_totalutilizado').innerHTML = mascaraglobal('###.###.###,##',total.toFixed(2));
	document.getElementById('deapercutilizado').value = final.toFixed(1);

}


/* 
 * Regra solicitada pelo Wallace
 * 08/08/12 : Somente para o Município SP - Suzano, os valores devem estar fixos com todos os campos desabilitados 
 */

<?// $desabilitado = true; ?>
jQuery(document).ready(function() {
// 	jQuery("[name^='valorpormes[']").attr("disabled","disabled");
// 	jQuery("[name^='valorpormes[']").attr("className","disabled");
	<?
		$sql = "SELECT tdaid FROM projovemcampo.tiposdemaisacoes --WHERE acostatus='A' AND ppuid = 3";
		$tdaids = $db->carregarColuna($sql);
	?>
	<?if($tdaids) :?>
		<? foreach($tdaids as $tdaid) : ?>
			calcularValorTotal(<?=$tdaid?>);
		<? endforeach; ?>
	<? endif; ?>	
	calculaPorcentTotal();
});


</script>
<form id="form" name="form" method="POST">
<input type="hidden" name="requisicao" value="gravarDemaisAcoes">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações:</td>
		<td>
			<font color=blue>
				Caso o EEx não atinja os percentuais máximos previstos na resolução CD/FNDE n° 11 de 16 de abril de 2014 
				nas ações descritas nas abas anteriores, poderá empregar o restante dos recursos transferidos para custear as ações discriminadas abaixo.
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
		<?
// 		$vlrdemaisacoes = $db->pegaUm("SELECT SUM(qtdmeses*valorpormes)FROM projovemcampo.itemdemaisacoes WHERE deaid='".$_SESSION['projovemcampo']['deaid']."' AND idastatus='A'");
// 		$montante = calcularMontante($meta);
// 		if($montante){
// 			$deaperc = $vlrdemaisacoes/$montante*100;
// 		}else{
// 			$deaperc = '';
// 		}
		
// 		if(number_format($deaperc,1,".","")!=$demaisacoes['deapercutilizado']) {
// 			$demaisacoes['deapercutilizado'] = number_format($deaperc,1,".","");
// 			$db->executar("UPDATE projovemcampo.demaisacoes SET deapercutilizado='".number_format($deaperc,1,".","")."' WHERE deaid='".$_SESSION['projovemcampo']['deaid']."'");
// 			$db->commit();
// 		}
		?>
		<tr>
			<td align="center"><? echo campo_texto('deapercmax', 'N', 'N', 'Percentual Máximo previsto(%)', 6, 5, "###.#", "", '', '', 0, 'id="deapercmax"', '', number_format(($totalMax*100)/$montante,1), '' ); ?> R$ <span id="span_totalmaximo"><?=number_format($totalMax,2,",",".") ?></span></td>
			<td align="center"><? echo campo_texto('deapercutilizado', 'N', 'N', 'Percentual utilizado(%)', 6, 5, "###.#", "", '', '', 0, 'id="deapercutilizado"', '', (($demaisacoes['deapercutilizado'])?$demaisacoes['deapercutilizado']:'0.0'), '' ); ?> R$ <span id="span_totalutilizado"><?=number_format($vlrdemaisacoes,2,",",".") ?></span></td>
		</tr>
		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Demais ações</td>
		<td>
		<?
				$sql1 = "SELECT 
							CASE WHEN valorpormes IS NULL 
							THEN '' 
							ELSE trim(to_char(valorpormes,'999g999g999d99')) END 
						FROM 
							projovemcampo.tipodemaisacoesplano tpa
						WHERE 
							tpa.tdaid=tda.tdaid 
						AND	tpa.pimid = {$_SESSION['projovemcampo']['pimid']}";
				$sql2 = "SELECT 
							CASE WHEN qtdmeses IS NULL 
							THEN '' 
							ELSE trim(to_char(qtdmeses,'999')) END 
						FROM 
							projovemcampo.tipodemaisacoesplano tpa
						WHERE 
							tpa.tdaid=tda.tdaid 
						AND	tpa.pimid = {$_SESSION['projovemcampo']['pimid']}";
				$sql3 = "SELECT 
							CASE WHEN qtdmeses IS NULL 
							THEN '' 
							ELSE trim(to_char(valortotal,'999g999g999d99')) END 
						FROM 
							projovemcampo.tipodemaisacoesplano tpa
						WHERE 
							tpa.tdaid=tda.tdaid 
						AND	tpa.pimid = {$_SESSION['projovemcampo']['pimid']}";
				
				$sql = "SELECT 
							nome,
					   		'<input type=\"text\" class=\"obrigatorio normal\" title=\"Valor\" id=\"valorpormes_'||tda.tdaid||'\" onblur=\"MouseBlur(this);calculaPorcentTotal();\" 
					   			onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" 
					   			onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);calcularValorTotal(\''||tda.tdaid||'\');calculaPorcentTotal();\" 
					   			value=\"'||COALESCE(($sql1),'')||'\" maxlength=\"14\" size=\"16\" name=\"valorpormes['||tdaid||']\" 
								style=\"text-align:;\">' as inp1, 
					   		'<input type=\"text\" class=\"obrigatorio normal\" title=\"Valor\" id=\"qtdmeses_'||tda.tdaid||'\" onblur=\"MouseBlur(this);calculaPorcentTotal();\" 
					   			onmouseout=\"MouseOut(this);\"onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" 
					   			onkeyup=\"this.value=mascaraglobal(\'########\',this.value);calcularValorTotal(\''||tda.tdaid||'\');calculaPorcentTotal();\"
								value=\"'||COALESCE(($sql2),'24')||'\" maxlength=\"7\" size=\"8\" name=\"qtdmeses['||tdaid||']\" 
								style=\"text-align:;\">' as inp2,
					   		'<input type=\"text\" class=\"disabled\" readonly title=\"Valor\" id=\"valortotal_'||tda.tdaid||'\" onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" 
								onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" 
								value=\"'||COALESCE(($sql3),'')||'\" maxlength=\"14\" size=\"16\" 
								name=\"valortotal['||tdaid||']\" style=\"text-align:;\">' as inp4
						FROM projovemcampo.tiposdemaisacoes tda 
						--WHERE 
							--ppuid = {$_SESSION['projovemcampo']['ppuid']}
						ORDER BY tda.tdaid";
// ver($sql,d);
		$cabecalho=array("Demais Ações","Valor/mês(R$)","Meses(18)","Valor total(R$)");
//                 $results = $db->carregar($sql);
//                 ver($results,$cabecalho,d);
		$db->monta_lista($sql,$cabecalho,50,5,'N','100%',$par2);
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">
		<input type="button" name="anterior" value="Anterior" onclick="window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=qualificacaoProfissional';"> 
		<? if(!$desabilitado) : ?>
			<input type="button" name="salvar" value="Salvar" onclick="gravarDemaisAcoes();"> 
		<? endif; ?>
		<input type="button" name="proximo" value="Próximo" onclick="window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=enderecoEntrega';"></td>
	</tr>
</table>
</form>
<? registarUltimoAcesso(); ?>
