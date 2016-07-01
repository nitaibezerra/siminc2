<script>

function diagnostico_1_2_taxasderendimento() {

	var checked = false;
	
	if(document.getElementById('rtrfunaprova_S')) {
	
		for(var i=0;i<document.getElementsByName('rtrfunaprova').length;i++) {
			if(document.getElementsByName('rtrfunaprova')[i].checked) {
				checked = true;
			}
		}
		
		if(checked == false) {
			alert('Marque a opção: A taxa de aprovação da escola vem melhorando nos últimos dois anos, no ensino fundamental?(Ensino Fundamental)');
			return false;		
		}
		
	}
	
	var checked = false;
	
	if(document.getElementById('rtrfunreprova_S')) {
	
		for(var i=0;i<document.getElementsByName('rtrfunreprova').length;i++) {
			if(document.getElementsByName('rtrfunreprova')[i].checked) {
				checked = true;
			}
		}
		
		if(checked == false) {
			alert('Marque a opção: A taxa de reprovação da escola tem diminuído nos dois últimos anos, no ensino fundamental?(Ensino Fundamental)');
			return false;		
		}
		
	}
	
	var checked = false;
	
	if(document.getElementById('rtrfunabandono_S')) {
	
		for(var i=0;i<document.getElementsByName('rtrfunabandono').length;i++) {
			if(document.getElementsByName('rtrfunabandono')[i].checked) {
				checked = true;
			}
		}
		
		if(checked == false) {
			alert('Marque a opção: A taxa de abandono tem diminuído nos dois últimos anos, no ensino fundamental?(Ensino Fundamental)');
			return false;		
		}
		
	}
	
	var checked = false;
	
	if(document.getElementById('rtrmedaprova_S')) {
	
		for(var i=0;i<document.getElementsByName('rtrmedaprova').length;i++) {
			if(document.getElementsByName('rtrmedaprova')[i].checked) {
				checked = true;
			}
		}
		
		if(checked == false) {
			alert('Marque a opção: A taxa de aprovação da escola vem melhorando nos últimos dois anos, no ensino fundamental?(Ensino Médio)');
			return false;		
		}
		
	}
	
	var checked = false;
	
	if(document.getElementById('rtrmedreprova_S')) {
	
		for(var i=0;i<document.getElementsByName('rtrmedreprova').length;i++) {
			if(document.getElementsByName('rtrmedreprova')[i].checked) {
				checked = true;
			}
		}
		
		if(checked == false) {
			alert('Marque a opção: A taxa de reprovação da escola tem diminuído nos dois últimos anos, no ensino fundamental?(Ensino Médio)');
			return false;		
		}
		
	}
	
	var checked = false;
	
	if(document.getElementById('rtrmedabandono_S')) {
	
		for(var i=0;i<document.getElementsByName('rtrmedabandono').length;i++) {
			if(document.getElementsByName('rtrmedabandono')[i].checked) {
				checked = true;
			}
		}
		
		if(checked == false) {
			alert('Marque a opção: A taxa de abandono tem diminuído nos dois últimos anos, no ensino fundamental?(Ensino Médio)');
			return false;		
		}
		
	}
	
	divCarregando();
	document.getElementById('formulario').submit();
}

</script>
<?
monta_titulo( "Taxas de rendimento - Indicadores e Taxas", "&nbsp;");

$sql = "SELECT * FROM pdeinterativo2013.respostataxarendimento WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND rtrstatus='A'";
$dadostx = $db->pegaLinha($sql);
if($dadostx) {
	extract($dadostx);
	$style_display = '';
} else {
	$style_display = 'style="display:none"';
}


?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%"><font class="blue">Orientações</font></td>
	<td class="blue">
	<p>Depois de analisar o IDEB, vamos refletir sobre as taxas de rendimento. As principais taxas informadas no Censo Escolar são: Aprovação, Reprovação e Abandono.</p>
	<p>Clique no botão abaixo para visualizar a(s) tabela(s) e o(s) gráfico(s) que mostram as taxas de rendimento do Brasil, Estado, Município e da própria escola, nos três últimos anos.</p>
	<p>Analise os gráficos de cada taxa e, em seguida, responda às perguntas.</p>
	<? if($style_display) : ?>
	<p><input type="button" name="verideb" value="CLIQUE AQUI para visualizar as taxas de rendimento" onclick="document.getElementById('div_taxasderendimento').style.display='';"></p>
	<? endif; ?> 
	</td>
</tr>
</table>
<div id="div_taxasderendimento" <?=$style_display ?> >
<form method="post" id="formulario">
<input type="hidden" name="togo" id="togo" value="/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_3_provabrasil">
<input type="hidden" name="requisicao" value="diagnostico_1_2_taxasderendimento">
<?
$possui_ensino_f = possuiEnsino($ensino='U', $submodulo='T'); 
$possui_ensino_m = possuiEnsino($ensino='M', $submodulo='T');
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<? if($possui_ensino_f) : ?>
<tr>
	<td class="SubTituloDireita" width="10%">Ensino Fundamental</td>
	<td width="30%" align="center" valign="top">
	<? $dados_ef_a = montaTabelaTaxasRendimento($ensino='U',$tipo='A'); echo $dados_ef_a['html']; ?><br>
	<? $dados_ef_r = montaTabelaTaxasRendimento($ensino='U',$tipo='R'); echo $dados_ef_r['html']; ?><br>
	<? $dados_ef_b = montaTabelaTaxasRendimento($ensino='U',$tipo='B'); echo $dados_ef_b['html']; ?>
	</td>
	<td width="30%" align="center" valign="top">
	<img src="pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&requisicao=montaGraficoTaxasRendimento&ensino=U&tipo=A">
	<br><br>
	<img src="pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&requisicao=montaGraficoTaxasRendimento&ensino=U&tipo=R">
	<br><br>
	<img src="pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&requisicao=montaGraficoTaxasRendimento&ensino=U&tipo=B">
	</td>
	<td width="30%" align="center" valign="top">
	<table width="100%">
		<tr>
		<td class="SubTituloCentro">Perguntas</td>
		<td class="SubTituloCentro">Sim</td>
		<td class="SubTituloCentro">Não</td>
		<td class="SubTituloCentro">Não se aplica</td>
		</tr>
		<tr>
		<td class="SubTituloDireita">A taxa de aprovação da escola vem melhorando nas duas últimas medições, no ensino fundamental?</td>
		<td align="center"><input type="radio" id="rtrfunaprova_S" name="rtrfunaprova" value="S" <? echo (($rtrfunaprova=="S")?"checked ":""); echo $dados_ef_a['onclickperg']['S']; ?> ></td>
		<td align="center"><input type="radio" name="rtrfunaprova" value="N" <? echo (($rtrfunaprova=="N")?"checked ":""); echo $dados_ef_a['onclickperg']['N']; ?> ></td>
		<td align="center"><input type="radio" name="rtrfunaprova" value="A" <? echo (($rtrfunaprova=="A")?"checked ":""); echo $dados_ef_a['onclickperg']['A']; ?> ></td>
		</tr>
		<tr>
		<td class="SubTituloDireita">A taxa de reprovação da escola tem diminuído nas duas últimas medições, no ensino fundamental?</td>
		<td align="center"><input type="radio" id="rtrfunreprova_S" name="rtrfunreprova" value="S" <? echo (($rtrfunreprova=="S")?"checked ":""); echo $dados_ef_r['onclickperg']['S']; ?> ></td>
		<td align="center"><input type="radio" name="rtrfunreprova" value="N" <? echo (($rtrfunreprova=="N")?"checked ":""); echo $dados_ef_r['onclickperg']['N']; ?> ></td>
		<td align="center"><input type="radio" name="rtrfunreprova" value="A" <? echo (($rtrfunreprova=="A")?"checked ":""); echo $dados_ef_r['onclickperg']['A']; ?> ></td>
		</tr>
		<tr>
		<td class="SubTituloDireita">A taxa de abandono tem diminuído nas duas últimas medições, no ensino fundamental?</td>
		<td align="center"><input type="radio" id="rtrfunabandono_S" name="rtrfunabandono" value="S" <? echo (($rtrfunabandono=="S")?"checked ":""); echo $dados_ef_b['onclickperg']['S']; ?> ></td>
		<td align="center"><input type="radio" name="rtrfunabandono" value="N" <? echo (($rtrfunabandono=="N")?"checked ":""); echo $dados_ef_b['onclickperg']['N']; ?> ></td>
		<td align="center"><input type="radio" name="rtrfunabandono" value="A" <? echo (($rtrfunabandono=="A")?"checked ":""); echo $dados_ef_b['onclickperg']['A']; ?> ></td>
		</tr>

	</table>
	</td>
</tr>
<? endif; ?>

<? if($possui_ensino_m) : ?>
<tr>
	<td class="SubTituloDireita">Ensino Médio</td>
	<td width="30%" align="center">
	<? $dados_em_a = montaTabelaTaxasRendimento($ensino='M',$tipo='A'); echo $dados_em_a['html']; ?>
	<? $dados_em_r = montaTabelaTaxasRendimento($ensino='M',$tipo='R'); echo $dados_em_r['html']; ?>
	<? $dados_em_b = montaTabelaTaxasRendimento($ensino='M',$tipo='B'); echo $dados_em_b['html']; ?>
	</td>
	<td width="30%" align="center" valign="top">
	<img src="pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&requisicao=montaGraficoTaxasRendimento&ensino=M&tipo=A">
	<br>	
	<img src="pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&requisicao=montaGraficoTaxasRendimento&ensino=M&tipo=R">
	<br>
	<img src="pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&requisicao=montaGraficoTaxasRendimento&ensino=M&tipo=B">
	</td>
	<td width="30%" align="center" valign="top">
	<table width="100%">
		<tr>
		<td class="SubTituloCentro">Perguntas</td>
		<td class="SubTituloCentro">Sim</td>
		<td class="SubTituloCentro">Não</td>
		<td class="SubTituloCentro">Não se aplica</td>
		</tr>
		<tr>
		<td class="SubTituloDireita">A taxa de aprovação da escola vem melhorando nas duas últimas medições, no ensino médio?</td>
		<td align="center"><input type="radio" id="rtrmedaprova_S" name="rtrmedaprova" value="S" <? echo (($rtrmedaprova=="S")?"checked ":""); echo $dados_em_a['onclickperg']['S']; ?> ></td>
		<td align="center"><input type="radio" name="rtrmedaprova" value="N" <? echo (($rtrmedaprova=="N")?"checked ":""); echo $dados_em_a['onclickperg']['N']; ?> ></td>
		<td align="center"><input type="radio" name="rtrmedaprova" value="A" <? echo (($rtrmedaprova=="A")?"checked ":""); echo $dados_em_a['onclickperg']['A']; ?> ></td>
		</tr>
		<tr>
		<td class="SubTituloDireita">A taxa de reprovação da escola tem diminuído nas duas últimas medições, no ensino médio?</td>
		<td align="center"><input type="radio" id="rtrmedreprova_S" name="rtrmedreprova" value="S" <? echo (($rtrmedreprova=="S")?"checked ":""); echo $dados_em_r['onclickperg']['S']; ?> ></td>
		<td align="center"><input type="radio" name="rtrmedreprova" value="N" <? echo (($rtrmedreprova=="N")?"checked ":""); echo $dados_em_r['onclickperg']['N']; ?> ></td>
		<td align="center"><input type="radio" name="rtrmedreprova" value="A" <? echo (($rtrmedreprova=="A")?"checked ":""); echo $dados_em_r['onclickperg']['A']; ?> ></td>
		</tr>
		<tr>
		<td class="SubTituloDireita">A taxa de abandono tem diminuído nas duas últimas medições, no ensino médio?</td>
		<td align="center"><input type="radio" id="rtrmedabandono_S" name="rtrmedabandono" value="S" <? echo (($rtrmedabandono=="S")?"checked ":""); echo $dados_em_b['onclickperg']['S']; ?> ></td>
		<td align="center"><input type="radio" name="rtrmedabandono" value="N" <? echo (($rtrmedabandono=="N")?"checked ":""); echo $dados_em_b['onclickperg']['N']; ?> ></td>
		<td align="center"><input type="radio" name="rtrmedabandono" value="A" <? echo (($rtrmedabandono=="A")?"checked ":""); echo $dados_em_b['onclickperg']['A']; ?> ></td>
		</tr>
	</table>
	</td>
</tr>
<? endif; ?>

<? if(!$possui_ensino_i && !$possui_ensino_f) : ?>
<tr>
	<td class="SubTituloCentro" colspan="4">Não existem informações das Taxas de rendimento referente a sua escola</td>
</tr>
<? endif; ?>

</table>

<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
<tr>
	<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
	<td>
	
	<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_1_ideb';" >
	<input type="button" name="btn_continuar" value="Salvar" onclick="document.getElementById('togo').value='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_2_taxasderendimento';diagnostico_1_2_taxasderendimento();">
	<input type="button" name="btn_continuar" value="Salvar e Continuar" onclick="document.getElementById('togo').value='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_3_provabrasil';diagnostico_1_2_taxasderendimento();">
	<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_3_provabrasil';" >
	</td>
</tr>
</table>
</form>
</div>