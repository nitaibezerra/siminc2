<?
criaSessaoDemaisAcoes();
$sql = "SELECT * FROM projovemurbano.demaisacoes 
		WHERE deaid='".$_SESSION['projovemurbano']['deaid']."' AND tprid = {$_SESSION['projovemurbano']['tprid']}";
$demaisacoes = $db->pegaLinha($sql);

$sugestaoampliacao = carregarSugestaoAmpliacao();
$meta = carregarMeta($sugestaoampliacao);
//ver($sugestaoampliacao);
$retornarTotalMaximoDemaisAcoes=true;
$totalMax = validacaoCompletaPlanoImplementacao2013();

//ver($totalMax);
$deapercmax = (($totalMax*100)/calcularMontante($meta));
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

function calcularValorTotal(acoid) {
	var qtdmeses = parseFloat(document.getElementById('idaqtdmeses_'+acoid).value);
	var idavlrmes=0;
	if(document.getElementById('idavlrmes_'+acoid).value!='') {
		idavlrmes 	= parseFloat(replaceAll(replaceAll(document.getElementById('idavlrmes_'+acoid).value,".",""),",","."));
	}
	
	var total = qtdmeses*idavlrmes;
	
	document.getElementById('idavalortotal_'+acoid).value = mascaraglobal('###.###.###,##',total.toFixed(2));
}

function calculaPorcentTotal() {
	var total=0;
	var montante = parseFloat('<?=calcularMontante($meta) ?>');
	<? 
	$sql = "SELECT acoid FROM projovemurbano.acoes WHERE acostatus='A' AND ppuid = {$_SESSION['projovemurbano']['ppuid']}";
	$acoids = $db->carregarColuna($sql);
	?>
	<? if($acoids) : ?>
		<? foreach($acoids as $acoid) : ?>
		var idavalortotal_<?=$acoid ?>=0;
		if(document.getElementById('idavalortotal_<?=$acoid ?>').value!='') {
			idavalortotal_<?=$acoid ?> 	= parseFloat(replaceAll(replaceAll(document.getElementById('idavalortotal_<?=$acoid ?>').value,".",""),",","."));
			var idapercmaxprevisto_<?=$acoid ?> = document.getElementById('idapercmaxprevisto_<?=$acoid ?>').value;
			if((idapercmaxprevisto_<?=$acoid ?>*montante/100) < idavalortotal_<?=$acoid ?>) {
				alert('Valor não ultrapassar percentual máximo previsto');
				idavalortotal_<?=$acoid ?>=0;
				document.getElementById('idavlrmes_<?=$acoid ?>').value='';
				document.getElementById('idavalortotal_<?=$acoid ?>').value='';
			}
		}
		total += idavalortotal_<?=$acoid ?>;
		<? endforeach; ?>
	<? endif; ?>
	
	var montante = parseFloat('<?=calcularMontante($meta) ?>');
	
	var final = (total*100)/montante;
	
	document.getElementById('span_totalutilizado').innerHTML = mascaraglobal('###.###.###,##',total.toFixed(2));
	document.getElementById('deapercutilizado').value = final.toFixed(1);

}


/* 
 * Regra solicitada pelo Wallace
 * 08/08/12 : Somente para o Município SP - Suzano, os valores devem estar fixos com todos os campos desabilitados 
 */
<? if(($esdid!=ESD_EMELABORACAO && !$db->testa_superuser()) || $_SESSION['projovemurbano']['muncod'] == MUNCOD_SUZANO) : ?>
<? $desabilitado = true; ?>
jQuery(document).ready(function() {
	jQuery("[name^='idavlrmes[']").attr("disabled","disabled");
	jQuery("[name^='idavlrmes[']").attr("className","disabled");

});
<? endif; ?>

</script>
<form id="form" name="form" method="POST">
<input type="hidden" name="requisicao" value="gravarDemaisAcoes">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações:</td>
		<td>
			<font color=blue>
				<?=$arrOrientacoes[9] ?>
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
		$vlrdemaisacoes = $db->pegaUm("SELECT SUM(idaqtdmeses*idavlrmes)FROM projovemurbano.itemdemaisacoes WHERE deaid='".$_SESSION['projovemurbano']['deaid']."' AND idastatus='A'");
		$montante = calcularMontante($meta);
		$deaperc = $vlrdemaisacoes/$montante*100;
		
		if(number_format($deaperc,1,".","")!=$demaisacoes['deapercutilizado']) {
			$demaisacoes['deapercutilizado'] = number_format($deaperc,1,".","");
			$db->executar("UPDATE projovemurbano.demaisacoes SET deapercutilizado='".number_format($deaperc,1,".","")."' WHERE deaid='".$_SESSION['projovemurbano']['deaid']."'");
			$db->commit();
		}
		?>
		<tr>
			<td align="center"><? echo campo_texto('deapercmax', 'N', 'N', 'Percentual Máximo previsto(%)', 6, 5, "###.#", "", '', '', 0, 'id="deapercmax"', '', number_format($deapercmax,1), '' ); ?> R$ <span id="span_totalmaximo"><?=number_format($totalMax,2,",",".") ?></span></td>
			<td align="center"><? echo campo_texto('deapercutilizado', 'N', 'N', 'Percentual utilizado(%)', 6, 5, "###.#", "", '', '', 0, 'id="deapercutilizado"', '', (($demaisacoes['deapercutilizado'])?$demaisacoes['deapercutilizado']:'0.0'), '' ); ?> R$ <span id="span_totalutilizado"><?=number_format($vlrdemaisacoes,2,",",".") ?></span></td>
		</tr>
		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Demais ações</td>
		<td>
		<?
		$sql = "SELECT acodesc,
					   '<input type=\"text\" class=\"obrigatorio normal\" title=\"Valor\" id=\"idavlrmes_'||aca.acoid||'\" onblur=\"MouseBlur(this);calculaPorcentTotal();\" 
					   			onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" 
					   			onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);calcularValorTotal(\''||aca.acoid||'\');calculaPorcentTotal();\" 
					   			value=\"'||COALESCE((SELECT CASE WHEN idavlrmes IS NULL THEN '' ELSE trim(to_char(idavlrmes,'999g999g999d99')) END 
					   								 FROM projovemurbano.itemdemaisacoes 
					   								 WHERE acoid=aca.acoid and deaid='".$_SESSION['projovemurbano']['deaid']."'),'')||'\" maxlength=\"14\" size=\"16\" name=\"idavlrmes['||acoid||']\" style=\"text-align:;\">' as inp1, 
					   '<input type=\"text\" class=\"disabled\" readonly title=\"Valor\" id=\"idaqtdmeses_'||aca.acoid||'\" onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'#######\',this.value);\" value=\"'||COALESCE((SELECT CASE WHEN idaqtdmeses IS NULL THEN '' ELSE trim(idaqtdmeses::text) END FROM projovemurbano.itemdemaisacoes WHERE acoid=aca.acoid and deaid='".$_SESSION['projovemurbano']['deaid']."'),'18')||'\" maxlength=\"7\" size=\"8\" name=\"idaqtdmeses['||acoid||']\" style=\"text-align:;\">' as inp2,
					   '<center><input type=\"text\" class=\"disabled\" readonly title=\"Valor\" id=\"idapercmaxprevisto_'||aca.acoid||'\" onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'####\',this.value);\" 
					   					value=\"'||COALESCE((SELECT idapercmaxprevisto FROM projovemurbano.itemdemaisacoes WHERE acoid=aca.acoid and deaid='".$_SESSION['projovemurbano']['deaid']."'),acomaxporcent)||'\" maxlength=\"6\" size=\"5\" name=\"idapercmaxprevisto['||acoid||']\" style=\"text-align:;\"></center>' as inp3,
					   '<input type=\"text\" class=\"disabled\" readonly title=\"Valor\" id=\"idavalortotal_'||aca.acoid||'\" onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" value=\"'||COALESCE((SELECT CASE WHEN idaqtdmeses IS NOT NULL AND idavlrmes IS NOT NULL THEN trim(to_char(idaqtdmeses*idavlrmes,'999g999g999d99')) ELSE '' END FROM projovemurbano.itemdemaisacoes WHERE acoid=aca.acoid and deaid='".$_SESSION['projovemurbano']['deaid']."'),'')||'\" maxlength=\"14\" size=\"16\" name=\"idavalortotal\" style=\"text-align:;\">' as inp4
				FROM projovemurbano.acoes aca 
				WHERE 
					acostatus='A'
					AND ppuid = {$_SESSION['projovemurbano']['ppuid']}
				ORDER BY aca.acoid";
//ver($sql);
		$cabecalho=array("Demais Ações","Valor/mês(R$)","Meses(18)","Percentual Máximo previstos(%)","Valor total(R$)");
                $results = $db->carregar($sql);
        
		$db->monta_lista_simples($results,$cabecalho,50,5,'N','100%',$par2);
		?>
		</td>
	</tr>
	<tr>
        <?php 
            if($_SESSION['projovemurbano']['muncod']) {
                $linkanterior = "Municipio";
            } else {
                $linkanterior = "Estado";
            }
            ?>
		<td class="SubTituloCentro" colspan="2">
		<input type="button" name="anterior" value="Anterior" onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba=<?php echo $_GET['aba'] ?>&aba2=qualificacaoProfissional<?php echo $linkanterior ?>';"> 
		<? if(!$desabilitado) : ?>
			<input type="button" name="salvar" value="Salvar" onclick="gravarDemaisAcoes();"> 
		<? endif; ?>
		<input type="button" name="proximo" value="Próximo" onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba=<?php echo $_GET['aba'] ?>&aba2=resumoFinanceiro';"></td>
	</tr>
</table>
</form>
<? registarUltimoAcesso(); ?>
