<?
criaSessaoProfissionais();
criaSessaoFormacaoEducadores();

$sql = "SELECT * FROM projovemurbano.formacaoeducadores
          WHERE fedid='".$_SESSION['projovemurbano']['fedid']."'
            AND tprid = {$_SESSION['projovemurbano']['tprid']}";
$formacaoeducadores = $db->pegaLinha($sql);

if($_SESSION['projovemurbano']['muncod'])
	$refesfera="M";
else if($_SESSION['projovemurbano']['estuf']) 
	$refesfera = "E";

$sugestaoampliacao = carregarSugestaoAmpliacao();
$meta = carregarMeta($sugestaoampliacao);

$auxiliofinanceiro = $db->pegaLinha("SELECT * FROM projovemurbano.auxiliofinanceiro WHERE fedid='".$_SESSION['projovemurbano']['fedid']."'");


$coordgeral = pegarCoordenadorGeral($_SESSION['projovemurbano']['proid']);
$assistenteadministrativo_A = pegarAssistentesCoordenadorGeral($_SESSION['projovemurbano']['proid'], 'A');
$assistenteadministrativo_P = pegarAssistentesCoordenadorGeral($_SESSION['projovemurbano']['proid'], 'P');
$coordassistentes = array("coaqtd"		=> $assistenteadministrativo_A['coaqtd']+$assistenteadministrativo_P['coaqtd'],
						  "coavlrtotal" => $assistenteadministrativo_A['coavlrtotal']+$assistenteadministrativo_P['coavlrtotal']); 
$diretorpolo 	  = pegarDiretorPolo($_SESSION['projovemurbano']['proid']);
$dirassistentes_A = pegarAssistentesDiretorPolo($_SESSION['projovemurbano']['proid'], 'A');
$dirassistentes_P = pegarAssistentesDiretorPolo($_SESSION['projovemurbano']['proid'], 'P');
$dirassistentes   = array("dasqtdefetivo40hr" 	 => $dirassistentes_A['dasqtdefetivo40hr']+$dirassistentes_P['dasqtdefetivo40hr'],
						  "dasqtdrecursoproprio" => $dirassistentes_A['dasqtdrecursoproprio']+$dirassistentes_P['dasqtdrecursoproprio'],
						  "creqtd" 				 => $dirassistentes_A['creqtd']+$dirassistentes_P['creqtd'],
						  "crevlrtotal" 		 => $dirassistentes_A['crevlrtotal']+$dirassistentes_P['crevlrtotal']);
$educadores_F = pegarEducadores($_SESSION['projovemurbano']['proid'], 'F');
$educadores_Q = pegarEducadores($_SESSION['projovemurbano']['proid'], 'Q');
$educadores_P = pegarEducadores($_SESSION['projovemurbano']['proid'], 'P');
$educadores_M = pegarEducadores($_SESSION['projovemurbano']['proid'], 'M');
$educadores_T = pegarEducadores($_SESSION['projovemurbano']['proid'], 'T');

$divisor = contatipoeducadores($_SESSION['projovemurbano']['proid']);
$fedqtd = $educadores_F['eduqtd']+$educadores_Q['eduqtd']+$educadores_P['eduqtd'];
if($divisor){
	$arrT = array("vlrprofrecurso" => $coordassistentes['coavlrtotal']+$dirassistentes['crevlrtotal']+$educadores_F['crevlrtotal']+$educadores_Q['crevlrtotal']+$educadores_P['crevlrtotal']+$educadores_M['crevlrtotal']+$educadores_T['crevlrtotal'], 
				  "vlrprofcomplem" => $coordgeral['cgevlrtotal']+$diretorpolo['ccmvlrtotal']+$educadores_F['ccmvlrtotal']+$educadores_Q['ccmvlrtotal']+$educadores_P['ccmvlrtotal'],
				  "qtdeducrecursoFQP" => $educadores_F['creqtd']+$educadores_Q['creqtd']+$educadores_P['creqtd']+$educadores_F['eduefetivo30hr']+$educadores_Q['eduefetivo30hr']+$educadores_P['eduefetivo30hr'],
				  "vlreducrecursoFQP" => round(($educadores_F['crevlrbrutorem']+$educadores_Q['crevlrbrutorem']+$educadores_P['crevlrbrutorem'])/$divisor,2));
}

$habilita = 'S';
$perfil = pegaPerfilGeral();
if(in_array(PFL_CONSULTA, $perfil)){
	$habilita = 'N';
}

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
function gravarFormacaoCadastro() {

	if(parseFloat(document.getElementById('fedperutilizado').value) > parseFloat(document.getElementById('fedpercmax').value)) {
		alert('Percentual Utilizado(%) maior do que Percentual Máximo previsto(%)');
		return false;
	}
	
	if(parseFloat(document.getElementById('aufpercutilizado').value) > parseFloat(document.getElementById('aufpercmax').value)) {
		alert('Percentual Utilizado(%) maior do que Percentual Máximo previsto(%)');
		return false;
	}
	
	var vlr1 = parseFloat(replaceAll(replaceAll(document.getElementById('span_totalmaximo').innerHTML,".",""),",","."));
	var vlr2 = parseFloat(replaceAll(replaceAll(document.getElementById('span_totalutilizado').innerHTML,".",""),",","."));
	
	if(vlr2>vlr1) {
		alert('Valor Utilizado maior do que Valor Máximo previsto');
		return false;
	}

	var vlr1 = parseFloat(replaceAll(replaceAll(document.getElementById('span_totalmaximo2').innerHTML,".",""),",","."));
	var vlr2 = parseFloat(replaceAll(replaceAll(document.getElementById('span_totalutilizado2').innerHTML,".",""),",","."));
	
	if(vlr2>vlr1) {
		alert('Valor Utilizado maior do que Valor Máximo previsto');
		return false;
	}


	jQuery('#form').submit();
}

function calculaPorcentTotal1() {
	
	var total=0;

	jQuery('[id^="refid_"]').each(function(){
		if( jQuery(this).val() != '' ){
			total = total+parseFloat(replaceAll(replaceAll(jQuery(this).val(),".",""),",","."));
		}
	});
	
	var montante = parseFloat('<?=calcularMontante($meta) ?>');
	
	var final = (total*100)/montante;
	
	document.getElementById('span_totalutilizado').innerHTML = mascaraglobal('###.###.###,##',total.toFixed(2));
	document.getElementById('fedperutilizado').value = final.toFixed(1);

}


function calculaPorcentTotal2() {
	var total=0;
	var aufavlrauxilio=0;
	if(document.getElementById('aufavlrauxilio').value!='' && document.getElementById('aufqtdeducador').value!='') {
		aufavlrauxilio = parseFloat(replaceAll(replaceAll(document.getElementById('aufavlrauxilio').value,".",""),",","."));
		var aufqtdeducador = document.getElementById('aufqtdeducador').value;
		aufavlrauxilio = aufavlrauxilio*aufqtdeducador;
		
		document.getElementById('span_totalutilizado2').innerHTML =  mascaraglobal('###.###.###,##',aufavlrauxilio.toFixed(2));;
		document.getElementById('auftotal').value =  mascaraglobal('###.###.###,##',aufavlrauxilio.toFixed(2));
	}
	total += aufavlrauxilio;

	var montante = parseFloat('<?=calcularMontante($meta) ?>');
	
	var final = (total*100)/montante;
	
	document.getElementById('aufpercutilizado').value = final.toFixed(1);

}

function verListaApoioReferenciaFormacao() { 
	window.open('projovemurbano.php?modulo=principal/planoImplementacao&acao=A&requisicao=listaApoioReferenciaFormacao','Programas','scrollbars=yes,height=400,width=600,status=no,toolbar=no,menubar=no,location=no');
}

<? if($esdid!=ESD_EMELABORACAO && !$db->testa_superuser()) :  ?>
<? $desabilitado = true; ?>
jQuery(document).ready(function() {
	jQuery("[name^='rgavalor[']").attr("disabled","disabled");
	jQuery("[name^='rgavalor[']").attr("className","disabled");
	
	jQuery("[name^='aufqtdeducador']").attr("disabled","disabled");
	jQuery("[name^='aufqtdeducador']").attr("className","disabled");
	jQuery("[name^='aufavlrauxilio']").attr("disabled","disabled");
	jQuery("[name^='aufavlrauxilio']").attr("className","disabled");
	
});
<? endif; ?>

</script>
<form id="form" name="form" method="POST">
<input type="hidden" name="requisicao" value="gravarFormacaoEducadores">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="35%">Orientações:</td>
		<td>
			<font color=blue><?=$arrOrientacoes[5] ?></font>
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
			<td align="center"><? echo campo_texto('fedpercmax', 'N', 'N', 'Percentual Máximo previsto(%)', 6, 5, "###.#", "", '', '', 0, 'id="fedpercmax"', '', (($formacaoeducadores['fedpercmax'])?$formacaoeducadores['fedpercmax']:'10.0'), '' ); ?> R$ <span id="span_totalmaximo"><?=number_format(calcularMontante($meta)*(($formacaoeducadores['fedpercmax'])?$formacaoeducadores['fedpercmax']:'10.0')/100,2,",",".") ?></span></td>
			<td align="center"><? echo campo_texto('fedperutilizado', 'N', 'N', 'Percentual utilizado(%)', 6, 5, "###.#", "", '', '', 0, 'id="fedperutilizado"', '', (($formacaoeducadores['fedperutilizado'])?$formacaoeducadores['fedperutilizado']:'0.0'), '' ); ?> R$ <span id="span_totalutilizado"><?=number_format($db->pegaUm("SELECT SUM(rgavalor) FROM projovemurbano.recursosgastos WHERE fedid='".$_SESSION['projovemurbano']['fedid']."' AND rgastatus='A'"),2,",",".") ?></span></td>
		</tr>
		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Recursos gastos com a formação</td>
		<td>
		<p><b>Quantidade:</b> <? echo campo_texto('fedqtd', 'N', 'N', 'Quantidade', 8, 7, "########", "", '', '', 0, 'id="fedqtd"', '', $fedqtd, '' ); ?>  <input type="button" value="Lista Referências" onclick="verListaApoioReferenciaFormacao();"></p>
		<?
		$sql = "SELECT refdesc, 
			       '<input type=\"text\" class=\"obrigatorio normal\" title=\"Valor\" id=\"refid_'||ref.refid||'\" onblur=\"MouseBlur(this);calculaPorcentTotal1();\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);calculaPorcentTotal1();\" value=\"'|| CASE WHEN rec.rgavalor IS NULL THEN '' ELSE trim(to_char(rec.rgavalor,'999g999g999d99')) END||'\" maxlength=\"14\" size=\"16\" name=\"rgavalor['||ref.refid||']\" style=\"text-align:;\">' as inp 
			  FROM projovemurbano.referenciaformacao ref 
			    LEFT JOIN projovemurbano.recursosgastos rec
                              ON rec.refid=ref.refid AND rec.fedid='".$_SESSION['projovemurbano']['fedid']."' 
			  WHERE refstatus='A'
                            AND ppuid = {$_SESSION['projovemurbano']['ppuid']}
                            AND refesfera='$refesfera'";
		$cabecalho=array("&nbsp;","Valor(R$)");
		
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
		?>
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
			<td align="center"><? echo campo_texto('aufpercmax', 'N', 'N', 'Percentual Máximo previsto(%)', 6, 5, "###.#", "", '', '', 0, 'id="aufpercmax"', '', (($auxiliofinanceiro['aufpercmax'])?$auxiliofinanceiro['aufpercmax']:'1.0'), '' ); ?> R$ <span id="span_totalmaximo2"><?=number_format(calcularMontante($meta)*(($auxiliofinanceiro['aufpercmax'])?$auxiliofinanceiro['aufpercmax']:'1.0')/100,2,",",".") ?></span></td>
			<td align="center"><? echo campo_texto('aufpercutilizado', 'N', 'N', 'Percentual utilizado(%)', 6, 5, "###.#", "", '', '', 0, 'id="aufpercutilizado"', '', (($auxiliofinanceiro['aufpercutilizado'])?$auxiliofinanceiro['aufpercutilizado']:'0.0'), '' ); ?> R$ <span id="span_totalutilizado2"><?=number_format($auxiliofinanceiro['aufqtdeducador']*$auxiliofinanceiro['aufavlrauxilio'],2,",",".") ?></span></td>
		</tr>
		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Valor destinado ao pagamento de auxílio financeiro para a primeira etapa da formação continuada de educadores de ensino fundamental, qualificação profissional e participação cidadã</td>
		<td>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
		<tr>
			<td class="SubTituloCentro">Número de educadores previstos para primeira etapa de formação continuada</td>
			<td class="SubTituloCentro">Auxílio financeiro a ser pago(R$)</td>
		</tr>
		<tr>
			<td align="center"><?if($arrT['qtdeducrecursoFQP'] == ''){$arrT['qtdeducrecursoFQP'] = '0';}?>
                            <? echo campo_texto('aufqtdeducador', 'N', $habilita, 'Número de educadores previstos para primeira etapa de formação continuada', 8, 7, "########", "", '', '', 0, 'id="aufqtdeducador"', 'if(this.value>'.$arrT['qtdeducrecursoFQP'].'){alert(\'Não pode ser maior que '.$arrT['qtdeducrecursoFQP'].'\');this.value=\''.$arrT['qtdeducrecursoFQP'].'\';}calculaPorcentTotal2();', (($auxiliofinanceiro['aufqtdeducador'])?$auxiliofinanceiro['aufqtdeducador']:''), '' ); ?>
                        </td>
			<td align="center"><? echo campo_texto('aufavlrauxilio', 'N', $habilita, 'Auxílio financeiro a ser pago(R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="aufavlrauxilio"', 'if(parseFloat(replaceAll(replaceAll(this.value,\'.\',\'\'),\',\',\'.\'))>'.round(($arrT['vlreducrecursoFQP']*0.3),2).'){alert(\'Não pode ser maior que '.round(($arrT['vlreducrecursoFQP']*0.3),2).'\');this.value=\''.number_format(($arrT['vlreducrecursoFQP']*0.3),2,",",".").'\';}calculaPorcentTotal2();', (($auxiliofinanceiro['aufavlrauxilio'])?number_format($auxiliofinanceiro['aufavlrauxilio'],2,",","."):''), '' ); ?></td>
			<td align="center"><? echo campo_texto('auftotal', 'N', 'N', 'Total', 15, 14, "###.###.###,##", "", '', '', 0, 'id="auftotal"', '', number_format(($auxiliofinanceiro['aufqtdeducador']*$auxiliofinanceiro['aufavlrauxilio']),2,",","."), '' ); ?></td>
		</tr>
		</table>
		
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloCentro" colspan="2">
		<input type="button" name="anterior" value="Anterior" onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao2014&acao=A&aba=<?php echo $_GET['aba'] ?>&aba2=profissionais';">
		<? if(!$desabilitado) : ?> 
			<input type="button" name="salvar" value="Salvar" onclick="gravarFormacaoCadastro();">
		<? endif; ?> 
		<input type="button" name="proximo" value="Próximo" onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao2014&acao=A&aba=<?php echo $_GET['aba'] ?>&aba2=generoAlimenticios';"></td>
	</tr>
</table>
</form>
<? registarUltimoAcesso(); ?>