<?
// criaSessaoProfissionais();
// criaSessaoFormacaoEducadores();
$percmax = '10';

$montante = calcularMontante();
$efetivorecprog = 3;
$efeticocomp = 2;

$educadores_EF_rec = pegarprofissionais('4',$efetivorecprog);
$educadores_EF_ccm = pegarprofissionais('4',$efeticocomp);

$educadores_EQ_rec = pegarprofissionais('5',$efetivorecprog);
$educadores_EQ_ccm = pegarprofissionais('5',$efeticocomp);

$educadores_EP_rec = pegarprofissionais('6',$efetivorecprog);
$educadores_EP_ccm = pegarprofissionais('6',$efeticocomp);

$sql = "SELECT
			SUM(valor)
		FROM 
			projovemcampo.tiposrecursoformacao tpf
		LEFT JOIN projovemcampo.tiporecursoplanoimplementacao trp ON trp.tpfid=tpf.tpfid
		WHERE
			pimid = {$_SESSION['projovemcampo']['pimid']}";
$totalutilizado = $db->pegaUm($sql);

$sql = "SELECT
			count(pprid)
		FROM
			projovemcampo.planoprofissional 
		WHERE
			pimid = {$_SESSION['projovemcampo']['pimid']}
		AND 	valorbrutoremuneracao is not null
		AND 	profid in(4,5)";

$tiposprof = $db->pegaUm($sql);

if(!$tiposprof){
	$tiposprof = '1';
}
$totalrec   = array(	"vlrtotal" 	 => 		$educadores_EF_rec['valorbrutoremuneracao']+
												$educadores_EQ_rec['valorbrutoremuneracao'],
						"qtdcontratado" => 		$educadores_EF_rec['qtdcontratado']+
												$educadores_EQ_rec['qtdcontratado']);

$totalccm =  array(	"vlrtotal" 	 => 			$educadores_EF_ccm['valorbrutoremuneracao']+
												$educadores_EQ_ccm['valorbrutoremuneracao'],
					"qtdcontratado" => 			$educadores_EF_ccm['qtdcontratado']+
												$educadores_EQ_ccm['qtdcontratado']);
$total = array("valor"=> $totalccm['vlrtotal']+ $totalrec['vlrtotal'],
				"qtd"=>$totalccm['qtdcontratado']+ $totalrec['qtdcontratado']);

$sql = "SELECT
			qtdeduccontinuada,
			auxfinanceiroeduc
		FROM
			projovemcampo.planodeimplementacao
		WHERE
			pimid = {$_SESSION['projovemcampo']['pimid']} ";

$auxiliofinanceiro = $db->pegaLinha($sql);
$habilita = 'S';
$perfil = pegaPerfilGeral();
if(in_array(PFL_CONSULTA, $perfil)){
	$habilita = 'N';
}

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
function gravarFormacaoCadastro() {
	
	if(jQuery('[id*=aufpercutilizado]').val()!=0.0){
		if(jQuery('[name="qtdeduccontinuada').val()==''){
			alert('Preencha o campo número de educadores.');
	        	rolaTela('qtdcontratado_CG');
			return false;
		}
		if(jQuery('[name="qtdeduccontinuada').val()==''){
			alert('Preencha o campo número de educadores.');
			return false;
		}
		if(jQuery('[name="auxfinanceiroeduc').val()==''){
			alert('Preencha o campo auxilio finaceiro.');
			return false;
		}
	}else{
		alert('Preencha os dados da tela para presseguir com o salvamento.');
		return false;
	}
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
	
	jQuery('[id^="tpfid_"]').each(function(){
		if( jQuery(this).val() != '' ){
			total = total+parseFloat(replaceAll(replaceAll(jQuery(this).val(),".",""),",","."));
		}
	});
	
	var montante = parseFloat('<?=$montante ?>');
	
	var final = (total*100)/montante;
	document.getElementById('span_totalutilizado').innerHTML = mascaraglobal('###.###.###,##',total.toFixed(2));
	document.getElementById('fedperutilizado').value = final.toFixed(1);

}


function calculaPorcentTotal2() {
	var total=0;
	var auxfinanceiroeduc=0;
	if(document.getElementById('auxfinanceiroeduc').value!='' && document.getElementById('qtdeduccontinuada').value!='') {
		auxfinanceiroeduc = parseFloat(replaceAll(replaceAll(document.getElementById('auxfinanceiroeduc').value,".",""),",","."));
		var qtdeduccontinuada = document.getElementById('qtdeduccontinuada').value;
		auxfinanceiroeduc = auxfinanceiroeduc*qtdeduccontinuada;
		
		document.getElementById('span_totalutilizado2').innerHTML =  mascaraglobal('###.###.###,##',auxfinanceiroeduc.toFixed(2));;
		document.getElementById('auxiliototal').value =  mascaraglobal('###.###.###,##',auxfinanceiroeduc.toFixed(2));
	}
	total += auxfinanceiroeduc;

	var montante = parseFloat('<?=$montante ?>');
	
	var final = (total*100)/montante;
	
	document.getElementById('aufpercutilizado').value = final.toFixed(1);

}

<? if($esdid!=ESD_EMELABORACAO && !$db->testa_superuser()) :  ?>
<? $desabilitado = true; ?>
jQuery(document).ready(function() {
	jQuery("[name^='rgavalor[']").attr("disabled","disabled");
	jQuery("[name^='rgavalor[']").attr("className","disabled");
	
	jQuery("[name^='qtdeduccontinuada']").attr("disabled","disabled");
	jQuery("[name^='qtdeduccontinuada']").attr("className","disabled");
	jQuery("[name^='auxfinanceiroeduc']").attr("disabled","disabled");
	jQuery("[name^='auxfinanceiroeduc']").attr("className","disabled");
	
});
<? endif; ?>

</script>
<form id="form" name="form" method="POST">
<input type="hidden" name="requisicao" value="gravarFormacaoEducadores">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="35%">Orientações:</td>
		<td>
			<font color=blue>
				A seguir será discriminado o custeio da formação continuada de educadores/professores, formadores e gestores, 
				bem como pagamento de auxílio financeiro aos educadores durante a primeira etapa de formação, conforme os percentuais determinados 
				na Resolução CD/FNDE n° 11 de 16 de abril de 2014.
				<br>
				ATENÇÂO: somente os educadores NÂO contratados até o período da formação poderão receber o auxílio financeiro.
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
			<td align="center"><? echo campo_texto('fedpercmax', 'N', 'N', 'Percentual Máximo previsto(%)', 6, 5, "###.#", "", '', '', 0, 'id="fedpercmax"', '',$percmax, '' ); ?> R$ <span id="span_totalmaximo"><?=number_format($montante*$percmax/100,2,",",".") ?></span></td>
			<td align="center"><? echo campo_texto('fedperutilizado', 'N', 'N', 'Percentual utilizado(%)', 6, 5, "###.#", "", '', '', 0, 'id="fedperutilizado"', '', (number_format(($totalutilizado*100)/$montante,2,",",".")?number_format(($totalutilizado*100)/$montante,2,",","."):'0.0'), '' ); ?> R$ <span id="span_totalutilizado"><?=number_format($totalutilizado,2,",",".") ?></span></td>
		</tr>
		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Recursos gastos com a formação</td>
		<td>
		<? 
		if(!$_SESSION['projovemcampo']['estuf']){
			$AND =" WHERE tpf.tpfid != 6" ;
		}
		$sql1 = "SELECT 
					CASE WHEN valor IS NULL THEN '0' ELSE valor END as inp 
				FROM 
					projovemcampo.tiporecursoplanoimplementacao trp
				WHERE
					trp.tpfid = tpf.tpfid 
				AND	pimid = {$_SESSION['projovemcampo']['pimid']}"; 
		$sql = "SELECT 
					nome, 
					'<input type=\"text\" class=\"obrigatorio normal\" title=\"Valor\" id=\"tpfid_'||tpf.tpfid||'\" onblur=\"MouseBlur(this);calculaPorcentTotal1();\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);calculaPorcentTotal1();\" value=\"'||COALESCE(to_char(($sql1),'999g999g999d99'),'')||'\" maxlength=\"14\" size=\"16\" name=\"valor['||tpf.tpfid||']\" style=\"text-align:;\">' as inp 
				FROM 
					projovemcampo.tiposrecursoformacao tpf
					$AND
                    ";
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
			<td align="center"><? echo campo_texto('aufpercutilizado', 'N', 'N', 'Percentual utilizado(%)', 6, 5, "###.#", "", '', '', 0, 'id="aufpercutilizado"', '', ((($auxiliofinanceiro['qtdeduccontinuada']*$auxiliofinanceiro['auxfinanceiroeduc']*100)/$montante)?number_format(($auxiliofinanceiro['qtdeduccontinuada']*$auxiliofinanceiro['auxfinanceiroeduc']*100)/$montante,2,",","."):'0.0'), '' ); ?> R$ <span id="span_totalutilizado2"><?=number_format($auxiliofinanceiro['qtdeduccontinuada']*$auxiliofinanceiro['auxfinanceiroeduc'],2,",",".") ?></span></td>
		</tr>
		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Valor destinado ao pagamento de auxílio financeiro para a primeira etapa da formação continuada de educadores/professores de educação básica, qualificação social e profissional
		</td>
		<td>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
		<tr>
			<td class="SubTituloCentro">Número de educadores previstos para primeira etapa de formação continuada</td>
			<td class="SubTituloCentro">Auxílio financeiro a ser pago(R$)</td>
		</tr>
		<tr>
			<td align="center"><?if($total['qtd']== ''){$total['qtd'] = '0';}?>
                            <? echo campo_texto('qtdeduccontinuada', 'N', $habilita, 'Número de educadores previstos para primeira etapa de formação continuada', 8, 7, "########", "", '', '', 0, 'id="qtdeduccontinuada"', 'if(this.value>'.$total['qtd'].'){alert(\'Não pode ser maior que '.$total['qtd'].'\');this.value=\''.$total['qtd'].'\';}calculaPorcentTotal2();', (($auxiliofinanceiro['qtdeduccontinuada'])?$auxiliofinanceiro['qtdeduccontinuada']:''), '' ); ?>
                        </td>
			<td align="center"><? echo campo_texto('auxfinanceiroeduc', 'N', $habilita, 'Auxílio financeiro a ser pago(R$)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="auxfinanceiroeduc"', 'if(parseFloat(replaceAll(replaceAll(this.value,\'.\',\'\'),\',\',\'.\'))>'.round((($total['valor']/$tiposprof)*0.3),2).'){alert(\'Não pode ser maior que '.round((($total['valor']/$tiposprof)*0.3),2).'\');this.value=\''.number_format((($total['valor']/$tiposprof)*0.3),2,",",".").'\';}calculaPorcentTotal2();', (($auxiliofinanceiro['auxfinanceiroeduc'])?number_format($auxiliofinanceiro['auxfinanceiroeduc'],2,",","."):''), '' ); ?></td>
			<td align="center"><? echo campo_texto('auxiliototal', 'N', 'N', 'Total', 15, 14, "###.###.###,##", "", '', '', 0, 'id="auxiliototal"', '', number_format(($auxiliofinanceiro['qtdeduccontinuada']*$auxiliofinanceiro['auxfinanceiroeduc']),2,",","."), '' ); ?></td>
		</tr>
		</table>
		
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloCentro" colspan="2">
		<input type="button" name="anterior" value="Anterior" onclick="window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais';">
		<? if(!$desabilitado) : ?> 
			<input type="button" name="salvar" value="Salvar" onclick="gravarFormacaoCadastro();">
		<? endif; ?> 
		<input type="button" name="proximo" value="Próximo" onclick="window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=generoAlimenticios';"></td>
	</tr>
</table>
</form>
<? registarUltimoAcesso(); ?>