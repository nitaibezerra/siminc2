<?
criaSessaoQualificacaoProfissional();
$qualificacaoprofissional = $db->pegaLinha("SELECT * FROM projovemurbano.qualificacaoprofissional WHERE qprid='".$_SESSION['projovemurbano']['qprid']."'");

$sugestaoampliacao = carregarSugestaoAmpliacao();
$meta = carregarMeta($sugestaoampliacao);

$montante = calcularMontante($meta);

$habilita = 'S';
$perfil = pegaPerfilGeral();
if(in_array(PFL_CONSULTA, $perfil)){
	$habilita = 'N';
}
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/projovemurbano.js"></script>
<script>

function gravarQualificacaoProfissional() {

	if(parseFloat(document.getElementById('qprpercutilizado').value) > parseFloat(document.getElementById('qprpercmax').value)) {
		alert('Percentual Utilizado(%) maior do que Percentual Máximo previsto(%)');
		return false;
	}
	
	var nucid;
	var teste;
	var erro = false;
	
	jQuery('[name="nucid[]"]').each(function(){
		nucid=jQuery(this).val();
		teste = jQuery('[name*="'+nucid+'"]:checked').length;
		
		if( teste < 1 ){
			erro = true;
			alert('Selecione o tipo de oferta de cada núcleo.');
			return false;
		}
	});
	if(!erro){
		jQuery('#form').submit();
	}
}
function calcularValorTotal(desid,muncod) {
	
	var qtdmeses = parseFloat(document.getElementById('pgaqtdmeses_'+muncod+'_'+desid).value);
	
	var pgavlrmes;
	
	if(document.getElementById('pgavlrmes_'+muncod+'_'+desid).value!='') {
		pgavlrmes 	= parseFloat(replaceAll(replaceAll(document.getElementById('pgavlrmes_'+muncod+'_'+desid).value,".",""),",","."));
	}
	
	var total = qtdmeses*pgavlrmes;
	
	document.getElementById('pgavlrtotal_'+muncod+'_'+desid).value = mascaraglobal('###.###.###,##',total.toFixed(2));
}

function calculaPorcentTotal() {
	var total=0;
	
	var qtd = jQuery('[name*="qpnarco["]:checked').length;

	jQuery('[id^="pgavlrtotal"]').each(function(){
		if( jQuery(this).val() != '' ){
			total += parseFloat(replaceAll(replaceAll(jQuery(this).val(),".",""),",","."));
		}
	});
	
	//total = total*qtd;
	
	var montante = parseFloat('<?=calcularMontante($meta) ?>');
	
	var final = (total*100)/montante;
	
	document.getElementById('span_totalutilizado').innerHTML = mascaraglobal('###.###.###,##',total.toFixed(2));
	document.getElementById('qprpercutilizado').value = final.toFixed(1);
		
}

function controleDivs(div,obj) {

	var checked = jQuery('[name*="qpnarco["]:checked').length;

	if( checked > 0 ) {
		document.getElementById('tr_dados').style.display='';
	} else {
		document.getElementById('tr_dados').style.display='none';
		jQuery('[name*="pgavlrmes"]').val(0);
		jQuery('[name*="pgavlrmes"]').keyup();
	}
}

function selecionarInstituicaoMunicipio(muncod,instituicao,nucid) {

	if(nucid=='') {
		alert('É necessário cadastrar núcleo');
		jQuery('#instituicao_'+nucid).val('');
		return false;
	}
	
	jQuery.ajax({
   		type: "POST",
   		url: "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A",
   		data: "requisicao=buscarCursosPorInstituicoesMunicipio&nucid="+nucid+"&ccuibge="+muncod+"&ccuinstituicao="+instituicao,
   		async: false,
   		success: function(dados){
   				jQuery('#nucleo_'+nucid).html(dados);
   			}
 		});
}

function calcularQuantidadeCursosMunicpio(ccuid, muncod, obj) {
	var campos  = jQuery("[id^='cupqtdestudantes_"+ccuid+"_']");
	var total=0;
	jQuery.each(campos, function(i, v) {
	if(v.value!='') {
		total = total + parseFloat(replaceAll(replaceAll(v.value,".",""),",","."));
	} 
	});
	
	if(total > jQuery('#totalvagas_'+ccuid+'_'+muncod).val()) {
		alert('Número de vagas não pode ultrapassar o número ofertado');
		if(obj) {
			obj.value='';
		}
	} else {
		jQuery('#totalutilizado_'+ccuid+'_'+muncod).val(mascaraglobal('###.###.###,##',total.toFixed(0)));
	}
	
}

jQuery(document).ready(function() {
<? if($esdid!=ESD_EMELABORACAO && !$db->testa_superuser()) : ?>
<? $desabilitado = true; ?>
	jQuery("[name^='qprarco']").attr("disabled","disabled");
	jQuery("[name^='qpredutecnica']").attr("disabled","disabled");

	jQuery("[name^='arcidmunicipio[']").attr("disabled","disabled");
	jQuery("[name^='arcidmunicipio[']").attr("className","disabled");
	
	jQuery("[name^='pgavlrmes[']").attr("disabled","disabled");
	jQuery("[name^='pgavlrmes[']").attr("className","disabled");
<? endif; ?>
	calculaPorcentTotal();
});

</script>
<form id="form" name="form" method="POST">
<input type="hidden" name="requisicao" value="gravarQualificacaoProfissional2013">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="35%">Orientações:</td>
		<td>
			<font color=blue>
				<?=$arrOrientacoes[7] ?>
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
			<td align="center"><? echo campo_texto('qprpercmax', 'N', 'N', 'Percentual Máximo previsto(%)', 6, 5, "###.#", "", '', '', 0, 'id="qprpercmax"', '', (($qualificacaoprofissional['qprpercmax'])?$qualificacaoprofissional['qprpercmax']:'7.0'), '' ); ?>  R$ <span id=""><?=number_format((($qualificacaoprofissional['qprpercmax'])?$qualificacaoprofissional['qprpercmax']:'7.0')*$montante/100,2,",",".") ?></span></td>
			<td align="center"><? echo campo_texto('qprpercutilizado', 'N', 'N', 'Percentual utilizado(%)', 6, 5, "###.#", "", '', '', 0, 'id="qprpercutilizado"', '', '', '' ); ?> R$ <span id="span_totalutilizado"></span></td>
		</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo de oferta</td>
		<td>
		<?
		$pmupossuipolo = $db->pegaUm("SELECT pmupossuipolo FROM projovemurbano.polomunicipio pmu 
										WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND pmu.tprid = {$_SESSION['projovemurbano']['tprid']}");
		
		
		if($pmupossuipolo=="t") {
			
			$sql = "SELECT DISTINCT mun.muncod,mundescricao,mun.munid FROM projovemurbano.polomunicipio pmu 
					INNER JOIN projovemurbano.polo pol ON pol.pmuid=pmu.pmuid 
					INNER JOIN projovemurbano.associamucipiopolo amp ON amp.polid=pol.polid 
					INNER JOIN projovemurbano.municipio mun ON mun.munid=amp.munid 
					INNER JOIN territorios.municipio m ON m.muncod=mun.muncod 
					INNER JOIN projovemurbano.nucleo nuc ON nuc.munid=mun.munid
					WHERE 
						pmu.pjuid='".$_SESSION['projovemurbano']['pjuid']."' 
						--AND pmu.tprid = {$_SESSION['projovemurbano']['tprid']} 
						AND nuc.tprid = {$_SESSION['projovemurbano']['tprid']} 
						AND pol.polstatus='A' AND mun.munstatus='A' AND nuc.nucstatus='A'";
		} else {

			$sql = "SELECT DISTINCT mun.muncod,mundescricao,mun.munid FROM projovemurbano.polomunicipio pmu 
					INNER JOIN projovemurbano.municipio mun ON mun.pmuid=pmu.pmuid 
					INNER JOIN territorios.municipio m ON m.muncod=mun.muncod 
					INNER JOIN projovemurbano.nucleo nuc ON nuc.munid=mun.munid
					WHERE 
						pmu.pjuid='".$_SESSION['projovemurbano']['pjuid']."' 
						--AND pmu.tprid = {$_SESSION['projovemurbano']['tprid']} 
						AND nuc.tprid = {$_SESSION['projovemurbano']['tprid']}
						AND mun.munstatus='A' AND nuc.nucstatus='A'";
			
		}
//		ver($sql);
		$municipios = $db->carregar($sql);
		
		?>
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
				<tr>
					<td class="SubTituloCentro" colspan="2">Município</td>
				</tr>
		<? 
		if($municipios[0]) :
			$teste = false;
			foreach($municipios as $value) : 
		?>
				<tr>
					<td colspan="2">Nucleo <?=$value['mundescricao'] ?></td>
				</tr>
		<? 
				$arcid = $db->pegaUm("SELECT arcid FROM projovemurbano.arcoqualificacao WHERE muncod='".$value['muncod']."' AND qprid='".$_SESSION['projovemurbano']['qprid']."'");
				$sql = "SELECT nucid FROM projovemurbano.nucleo WHERE munid = {$value['munid']} AND nucstatus = 'A' AND tprid = '".$_SESSION['projovemurbano']['tprid']."'";
				$nucleos = $db->carregarColuna($sql);
//				ver($sql);
				foreach($nucleos as $nucid){
					$sql = "SELECT qpnarco, qpnedutecnica FROM projovemurbano.qualificacaoprofissional_nucleo WHERE qprid='".$_SESSION['projovemurbano']['qprid']."' AND nucid = $nucid";
					$quali_nuc = $db->pegaLinha($sql);
		?>
				<tr>
					<td>Nucleo <?=$nucid ?></td>
					<td>
						<input type="hidden" 	name="nucid[]" 						value="<?=$nucid ?>">
						<input type="checkbox" 	name="qpnarco[<?=$nucid ?>]" 		value="TRUE" <?=(($quali_nuc['qpnarco']=="t")?"checked":"") ?> 		onclick="controleDivs('<?=$nucid ?>',this);calculaPorcentTotal();" > Arcos
						<input type="checkbox" 	name="qpnedutecnica[<?=$nucid ?>]"	value="TRUE" <?=(($quali_nuc['qpnedutecnica']=="t")?"checked":"") ?>  > Programas Nacionais de Educação Técnica em Formação Inicial e Continuada
					</td>
				</tr>
		<?
				$teste = (($quali_nuc['qpnarco']=="t") && !$teste ? true : $teste) ;
				}
			endforeach;
			
		endif;
		?>
			</table>
		</td>
	</tr>
	<tr id="tr_dados" style="<?=($teste?"":"display:none;") ?>">
		<td colspan="2">
		<?
		
		if($pmupossuipolo=="t") {
			
			$sql1 = "SELECT DISTINCT CASE WHEN pgavlrmes IS NULL THEN '' ELSE trim(to_char(pgavlrmes,'999g999g999d99')) END 
   		   			FROM projovemurbano.previsaogasto p
   		   			INNER JOIN projovemurbano.nucleo nuc ON nuc.nucid = p.nucid 
					INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
					INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid 
					INNER JOIN projovemurbano.polo pol ON pol.polid = amp.polid 
					INNER JOIN projovemurbano.polomunicipio pmu ON pmu.pmuid = pol.pmuid 
   		   			WHERE 
   		   				pmu.pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND nuc.tprid = {$_SESSION['projovemurbano']['tprid']} AND pol.polstatus='A' AND mun.munstatus='A' 
   		   				AND pgavlrmes IS NOT NULL
						AND p.desid = des.desid
						AND p.qprid = {$_SESSION['projovemurbano']['qprid']}
						AND pgastatus = 'A'
   		   			LIMIT 1";
			
			$sql2 = "SELECT DISTINCT CASE WHEN pgaqtdmeses IS NULL THEN '10' ELSE trim(pgaqtdmeses::text) END 
   		   			FROM projovemurbano.previsaogasto p
   		   			INNER JOIN projovemurbano.nucleo nuc ON nuc.nucid = p.nucid 
					INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
					INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid 
					INNER JOIN projovemurbano.polo pol ON pol.polid = amp.polid 
					INNER JOIN projovemurbano.polomunicipio pmu ON pmu.pmuid = pol.pmuid 
   		   			WHERE 
   		   				pmu.pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND nuc.tprid = {$_SESSION['projovemurbano']['tprid']} AND pol.polstatus='A' AND mun.munstatus='A' 
   		   				AND pgaqtdmeses IS NOT NULL
						AND p.desid = des.desid
						AND p.qprid = {$_SESSION['projovemurbano']['qprid']}
						AND pgastatus = 'A'
   		   			LIMIT 1";
		
			$sql3 = "SELECT DISTINCT CASE WHEN pgaqtdmeses IS NOT NULL AND pgavlrmes IS NOT NULL THEN trim(to_char(pgaqtdmeses*pgavlrmes,'999g999g999d99')) ELSE '' END 
   		   			FROM projovemurbano.previsaogasto p
   		   			INNER JOIN projovemurbano.nucleo nuc ON nuc.nucid = p.nucid 
					INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
					INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid 
					INNER JOIN projovemurbano.polo pol ON pol.polid = amp.polid 
					INNER JOIN projovemurbano.polomunicipio pmu ON pmu.pmuid = pol.pmuid 
   		   			WHERE 
   		   				pmu.pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND nuc.tprid = {$_SESSION['projovemurbano']['tprid']} AND pol.polstatus='A' AND mun.munstatus='A' AND pgaqtdmeses IS NOT NULL
   		   				AND pgaqtdmeses IS NOT NULL AND pgavlrmes IS NOT NULL
						AND p.desid = des.desid
						AND p.qprid = {$_SESSION['projovemurbano']['qprid']}
						AND pgastatus = 'A'
   		   			LIMIT 1";
		
		} else {
		
			$sql1 = "SELECT DISTINCT CASE WHEN pgavlrmes IS NULL THEN '' ELSE trim(to_char(pgavlrmes,'999g999g999d99')) END 
   		   			FROM projovemurbano.previsaogasto p
   		   			INNER JOIN projovemurbano.nucleo nuc ON nuc.nucid = p.nucid 
					INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid
					INNER JOIN projovemurbano.polomunicipio pmu ON pmu.pmuid = mun.pmuid 
					WHERE 
						pmu.pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND nuc.tprid = {$_SESSION['projovemurbano']['tprid']} AND mun.munstatus='A' 
						AND pgavlrmes IS NOT NULL
						AND p.qprid = {$_SESSION['projovemurbano']['qprid']}
						AND p.desid = des.desid
						AND pgastatus = 'A'
					LIMIT 1";

			$sql2 = "SELECT DISTINCT CASE WHEN pgaqtdmeses IS NULL THEN '10' ELSE trim(pgaqtdmeses::text) END 
   		   			FROM projovemurbano.previsaogasto p
   		   			INNER JOIN projovemurbano.nucleo nuc ON nuc.nucid = p.nucid 
					INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid
					INNER JOIN projovemurbano.polomunicipio pmu ON pmu.pmuid = mun.pmuid 
					WHERE 
						pmu.pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND nuc.tprid = {$_SESSION['projovemurbano']['tprid']} AND mun.munstatus='A' 
						AND pgaqtdmeses IS NOT NULL
						AND p.qprid = {$_SESSION['projovemurbano']['qprid']}
						AND p.desid = des.desid
						AND pgastatus = 'A'
					LIMIT 1";
			
			$sql3 = "SELECT DISTINCT CASE WHEN pgaqtdmeses IS NOT NULL AND pgavlrmes IS NOT NULL THEN trim(to_char(pgaqtdmeses*pgavlrmes,'999g999g999d99')) ELSE '' END
   		   			FROM projovemurbano.previsaogasto p
   		   			INNER JOIN projovemurbano.nucleo nuc ON nuc.nucid = p.nucid 
					INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid
					INNER JOIN projovemurbano.polomunicipio pmu ON pmu.pmuid = mun.pmuid 
					WHERE 
						pmu.pjuid='".$_SESSION['projovemurbano']['pjuid']."' 
						AND nuc.tprid = {$_SESSION['projovemurbano']['tprid']} 
						AND mun.munstatus='A' AND pgaqtdmeses IS NOT NULL
						AND p.qprid = {$_SESSION['projovemurbano']['qprid']}
						AND pgastatus = 'A'
						AND pgaqtdmeses IS NOT NULL 
						AND pgavlrmes IS NOT NULL
						AND p.desid = des.desid
					LIMIT 1";
			
		}
		$sql = "SELECT DISTINCT
					desdesc,
				   '<input type=\"text\" class=\"obrigatorio normal\" 	title=\"Valor\" id=\"pgavlrmes_{$nucid}_'||des.desid||'\" 		onblur=\"MouseBlur(this);\" 
				   		   onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" 
				   		   onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);calcularValorTotal(\''||des.desid||'\',\'{$nucid}\');calculaPorcentTotal();\" 
				   		   value=\"'||COALESCE(($sql1),'0')||'\" 
				   		   maxlength=\"14\" size=\"16\" name=\"pgavlrmes['||desid||']\" style=\"text-align:;\">' as inp1, 
				   '<input type=\"text\" class=\"disabled\" readonly 	title=\"Valor\" id=\"pgaqtdmeses_{$nucid}_'||des.desid||'\" 	onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'#######\',this.value);\" 
				   		   value=\"'||COALESCE(($sql2),'10')||'\" 
				   		   maxlength=\"7\" size=\"8\" name=\"pgaqtdmeses['||desid||']\" style=\"text-align:;\">' as inp2,
				   '<input type=\"text\" class=\"disabled\" readonly 	title=\"Valor\" id=\"pgavlrtotal_{$nucid}_'||des.desid||'\" 	onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" 
				   		   value=\"'||COALESCE(($sql3),'0')||'\" 
				   		   maxlength=\"14\" size=\"16\" name=\"rgavalor['||desid||']\" style=\"text-align:;\">' as inp3
				FROM projovemurbano.despesas des 
				WHERE desstatus='A'
				ORDER BY
					desdesc";
		
//		ver($sql1);
		$cabecalho=array("Despesas","Valor/mês(R$)","Meses(10)","Valor total(R$)");
		
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">
			<input type="button" name="anterior" value="Anterior" onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba=<?php echo $_GET['aba'] ?>&aba2=generoAlimenticios';">
		<? if(!$desabilitado) : ?> 
			<input type="button" name="salvar" value="Salvar" onclick="gravarQualificacaoProfissional();">
		<? endif; ?> 
			<input type="button" name="proximo" value="Próximo" onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba=<?php echo $_GET['aba'] ?>&aba2=demaisAcoes';">
		</td>
	</tr>
</table>
</form>
<? registarUltimoAcesso(); ?>