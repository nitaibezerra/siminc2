<?
$qualificacaoprofissional = $db->pegaLinha("SELECT * FROM projovemcampo.planodeimplementacao WHERE pimid='".$_SESSION['projovemcampo']['pimid']."'");
// ver($qualificacaoprofissional);
$percmax = '7';
$meta = pegameta();
$sql ="SELECT DISTINCT
			true
		FROM
			projovemcampo.despesaqualificacaoplano
		WHERE
			pimid = {$_SESSION['projovemcampo']['pimid']}";
$temdespesalancada = $db->pegaUm($sql);
$montante = calcularMontante();
if($temdespesalancada){
	$where = "	--WHERE 
					--dqp.pimid = {$_SESSION['projovemcampo']['pimid']}";
}
$habilita = 'S';
$perfil = pegaPerfilGeral();
if(in_array(PFL_CONSULTA, $perfil)){
	$habilita = 'N';
}
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/projovemcampo.js"></script>
<script>

function gravarQualificacaoProfissional() {
	var erro = false;
	
	if(parseFloat(document.getElementById('qprpercutilizado').value) > parseFloat(document.getElementById('qprpercmax').value)) {
		alert('Percentual Utilizado(%) maior do que Percentual Máximo previsto(%)');
		erro = true;
		return false;
	}
	
	var qualificacaoarco = jQuery('[name*="qualificacaoarco"]:checked').length;
	var qualificacaopronatec = jQuery('[name*="qualificacaopronatec"]:checked').length;
	
	if(qualificacaoarco < 1 && qualificacaopronatec < 1){
		alert('Selecionar ARCOS e/ou PRONATEC');
		erro = true;
		return false;
	}

	if(!erro){
		jQuery('#form').submit();
	}
}
function calcularValorTotal(qpdid) {
	
	var qtdmeses = parseFloat(document.getElementById('qtdmeses_'+qpdid).value);
	
	var valor;
	
	if(document.getElementById('valor_'+qpdid).value!='') {
		valor 	= parseFloat(replaceAll(replaceAll(document.getElementById('valor_'+qpdid).value,".",""),",","."));
	}
	
	var total = qtdmeses*valor;
	
	document.getElementById('valortotal_'+qpdid).value = mascaraglobal('###.###.###,##',total.toFixed(2));
}

function calculaPorcentTotal() {
	var total=0;
	
	jQuery('[id^="valortotal"]').each(function(){
		if( jQuery(this).val() != '' ){
			total += parseFloat(replaceAll(replaceAll(jQuery(this).val(),".",""),",","."));
		}
	});
	
	//total = total*qtd;
	
	var montante = parseFloat('<?=$montante ?>');
	
	var final = (total*100)/montante;
	
	document.getElementById('span_totalutilizado').innerHTML = mascaraglobal('###.###.###,##',total.toFixed(2));
	document.getElementById('qprpercutilizado').value = final.toFixed(1);
		
}
function controleDivs(obj) {

	var checked = jQuery('[name*="qualificacaoarco"]:checked').length;

	if( checked > 0 ) {
		document.getElementById('tr_dados').style.display='';
	} else {
		document.getElementById('tr_dados').style.display='none';
		jQuery('[name*="valor"]').val(0);
		jQuery('[name*="valor"]').keyup();
	}
}

jQuery(document).ready(function() {
<? if($esdid!=ESD_EMELABORACAO && !$db->testa_superuser()) : ?>
<? $desabilitado = true; ?>
	jQuery("[name^='valor[']").attr("disabled","disabled");
	jQuery("[name^='valor[']").attr("className","disabled");
<? endif; ?>
	calcularValorTotal('1');
	calcularValorTotal('2');
	calcularValorTotal('3');
	calculaPorcentTotal();
});

</script>
<form id="form" name="form" method="POST">
<input type="hidden" name="requisicao" value="gravarQualificacaoProfissional">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="35%">Orientações:</td>
		<td>
			<font color=blue>
				A Qualificação Profissional poderá ser ofertada por meio de programas nacionais de educação técnica em formação inicial e continuada - PRONATEC ou por meio de arcos ocupacionais ou por meio das duas opções(Arcos Ocupacionais e PRONATEC)
				<br>
				Para oferta por meio dos arcos acupacionsi, apresenta-se, a seguir as despesas possíveis para esta execução.
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
					<td align="center"><? echo campo_texto('qprpercmax', 'N', 'N', 'Percentual Máximo previsto(%)', 6, 5, "###.#", "", '', '', 0, 'id="qprpercmax"', '',$percmax, '' ); ?>  R$ <span id=""><?=number_format(($percmax*$montante)/100,2,",",".") ?></span></td>
					<td align="center"><? echo campo_texto('qprpercutilizado', 'N', 'N', 'Percentual utilizado(%)', 6, 5, "###.#", "", '', '', 0, 'id="qprpercutilizado"', '', ''); ?> R$ <span id="span_totalutilizado"></span></td>
				</tr>
			</table>
		</td>
	</tr>
	<tr id="tr_dados" <!--  style="<? // =($qualificacaoprofissional['qualificacaoarco']=='t'?"":"display:none;") ?>"-->>
		<td colspan="2">
		<?
		$sql1="SELECT
					valor
				FROM
					projovemcampo.despesaqualificacaoplano dqp
				WHERE
					dqp.qpdid = qpd.qpdid
				AND 	pimid = {$_SESSION['projovemcampo']['pimid']}";
		$sql2="SELECT
					qtdmeses
				FROM
					projovemcampo.despesaqualificacaoplano dqp 
				WHERE
					dqp.qpdid = qpd.qpdid
				AND 	pimid = {$_SESSION['projovemcampo']['pimid']}";
		$sql3 = "SELECT
					qtdmeses*valor as valortotal
				FROM
					projovemcampo.despesaqualificacaoplano dqp 
				WHERE
					dqp.qpdid = qpd.qpdid
				AND 	pimid = {$_SESSION['projovemcampo']['pimid']}";
		$sql = "SELECT
					nome,
					'<input type=\"text\" class=\"obrigatorio normal\" title=\"Valor\" id=\"valor_'||qpd.qpdid||'\" 
							onblur=\"MouseBlur(this);calcularValorTotal(\''||qpd.qpdid||'\');calculaPorcentTotal();\" 
							onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" 
							onmouseover=\"MouseOver(this);\" 
							onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);calcularValorTotal(\''||qpd.qpdid||'\');calculaPorcentTotal();\" 
							value=\"'||COALESCE(to_char(($sql1),'999g999g999d99'),'')||'\" maxlength=\"14\" size=\"16\" 
							name=valor['||qpd.qpdid||'] style=\"text-align:;\">' as inp1,
					'<input type=\"text\" class=\"obrigatorio normal\" readonly title=\"Valor\" id=\"qtdmeses_'||qpd.qpdid||'\" onblur=\"MouseBlur(this);calcularValorTotal(\''||qpd.qpdid||'\');calculaPorcentTotal();\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'#######\',this.value);calcularValorTotal(\''||qpd.qpdid||'\');calculaPorcentTotal();\" value=\"'||COALESCE(($sql2),24)||'\" maxlength=\"7\" size=\"8\" name=\"qtdmeses['||qpd.qpdid||']\" style=\"text-align:;\">' as inp2,
					'<input type=\"text\" class=\"disabled\" readonly title=\"Valor\" id=\"valortotal_'||qpd.qpdid||'\" onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" value=\"'||COALESCE(($sql3),'0')||'\" maxlength=\"14\" size=\"16\" style=\"text-align:;\">' as inp3 
				FROM
					projovemcampo.qualificacaoprofissionaldespesa qpd
				--LEFT JOIN projovemcampo.despesaqualificacaoplano dqp ON dqp.qpdid = qpd.qpdid
				$where
				
				";
// 		ver(htmlentities($sql));
		$cabecalho=array("Despesas","Valor/mês(R$)","Meses(10)","Valor total(R$)");
		
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
		?>
		</td>
	</tr>
	<tr>
		<td>
			<p><input type="checkbox" id="qualificacaoarco" name="qualificacaoarco" value="t" onclick="controleDivs(this.value)" <?=(($qualificacaoprofissional['qualificacaoarco']=="t")?"checked":"")  ?> > Arcos</p>
		</td>
	</tr>
	<tr>
		<td>
			<p><input type="checkbox" id="qualificacaopronatec" name="qualificacaopronatec" value="t" <?=(($qualificacaoprofissional['qualificacaopronatec']=="t")?"checked":"") ?> > Programas Nacionais de Educação Técnica em Formação Inicial e Continuada</p>		
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">
			<input type="button" name="anterior" value="Anterior" onclick="window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=generoAlimenticios';">
		<? if(!$desabilitado) : ?> 
			<input type="button" name="salvar" value="Salvar" onclick="gravarQualificacaoProfissional();">
		<? endif; ?> 
			<input type="button" name="proximo" value="Próximo" onclick="window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=demaisAcoes';">
		</td>
	</tr>
</table>
</form>
<? registarUltimoAcesso(); ?>