<?

monta_titulo( "IDEB - Indicadores e Taxas", "&nbsp;");
$sql = "SELECT * FROM pdeinterativo2013.respostaideb WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND ridsstatus='A'";
$dadosideb = $db->pegaLinha($sql);
if($dadosideb) {
	extract($dadosideb);
	$style_display = '';
} else {
	$style_display = 'style="display:none"';
}

?>
<script>

function diagnostico_1_1_ideb() {

	var checked = false;
	
	if(document.getElementById('ridinicialum_S')) {
	
		for(var i=0;i<document.getElementsByName('ridinicialum').length;i++) {
			if(document.getElementsByName('ridinicialum')[i].checked) {
				checked = true;
			}
		}
		
		if(checked == false) {
			alert('Marque a opção: O IDEB da escola vem melhorando nas duas últimas medições?(Anos iniciais do Ensino Fundamental)');
			return false;		
		}
		
	}
	
	var checked = false;
	
	if(document.getElementById('ridinicialdois_S')) {
	
		for(var i=0;i<document.getElementsByName('ridinicialdois').length;i++) {
			if(document.getElementsByName('ridinicialdois')[i].checked) {
				checked = true;
			}
		}
		
		if(document.getElementById('ridinicialdois_S').checked) {
			if(document.getElementById('riddinicialdesc').value=='') {
				alert('Preencha as evidências');
				return false;
			}
		}
		
		if(checked == false) {
			alert('Marque a opção: Há evidências de que a meta do IDEB será alcançado pelo IDEB?(Anos iniciais do Ensino Fundamental)');
			return false;		
		}
		
	}
	
	
	var checked = false;
	
	if(document.getElementById('ridfinalum_S')) {
	
		for(var i=0;i<document.getElementsByName('ridfinalum').length;i++) {
			if(document.getElementsByName('ridfinalum')[i].checked) {
				checked = true;
			}
		}
		
		if(checked == false) {
			alert('Marque a opção: O IDEB da escola vem melhorando nas duas últimas medições?(Anos finais do Ensino Fundamental)');
			return false;		
		}
		
	}
	
	var checked = false;
	
	if(document.getElementById('ridfinaldois_S')) {
	
		for(var i=0;i<document.getElementsByName('ridfinaldois').length;i++) {
			if(document.getElementsByName('ridfinaldois')[i].checked) {
				checked = true;
			}
		}
		if(document.getElementById('ridfinaldois_S').checked) {
			if(document.getElementById('ridfinaldesc').value=='') {
				alert('Preencha as evidências');
				return false;
			}
		}
		
		
		if(checked == false) {
			alert('Marque a opção: Há evidências de que a meta do IDEB será alcançado pelo IDEB?(Anos finais do Ensino Fundamental)');
			return false;		
		}
		
	}

	divCarregando();
	document.getElementById('formulario').submit();
}

function mostrarEvidencias(campo,valor) {
	document.getElementById(campo).value = '';
	document.getElementById(campo+'_tr').style.display = valor;
}

</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%"><font class="blue">Orientações</font></td>
	<td class="blue">
	<p>O Índice de Desenvolvimento da Educação Básica (IDEB) foi criado para medir a qualidade de cada escola e de cada rede de ensino. O indicador é calculado com base no desempenho do estudante e nas taxas de aprovação. Assim, para que o IDEB de uma escola ou rede cresça é preciso que o aluno aprenda, não repita o ano e frequente a sala de aula. O índice é apresentado numa escala de 0 (zero) a 10 (dez) e é medido a cada dois anos. O objetivo é que o Brasil tenha nota 6 em 2022 - correspondente à qualidade do ensino em países desenvolvidos.</p>
	<p>Clique no botão abaixo para visualizar a(s) tabela(s) e o(s) gráfico(s) que mostram a série histórica do IDEB no Brasil, Estado, Município e o IDEB da própria escola, em cada etapa oferecida no ano do Censo.</p>
	<p>Analise os gráficos e, em seguida, responda às perguntas. Caso a escola não possua pelo menos duas medições do IDEB, assinale a opção “Não se aplica”, mas não deixe de comentar os resultados do seu município, estado e do Brasil.</p>
	<? if($style_display) : ?>
	<p><input type="button" name="verideb" value="CLIQUE AQUI para visualizar os dados do IDEB" onclick="document.getElementById('div_ideb').style.display='';"></p>
	<? endif; ?> 
	</td>
</tr>
</table>

<div id="div_ideb" <? echo $style_display; ?> >
<form method="post" id="formulario">
<input type="hidden" name="togo" id="togo" value="/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_2_taxasderendimento">
<input type="hidden" name="requisicao" value="diagnostico_1_1_ideb">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<?
$possui_ensino_i = possuiEnsino($ensino='I', $submodulo='I'); 
$possui_ensino_f = possuiEnsino($ensino='F', $submodulo='I');
?>
<? if($possui_ensino_i) : ?>
<tr>
	<td class="SubTituloDireita" width="10%">Anos iniciais do Ensino Fundamental</td>
	<td width="30%" align="center" valign="top"><? $dados_ai = montaTabelaIDEB($ensino='I'); echo $dados_ai['html']; ?></td>
	<td width="30%" align="center" valign="top"><img src="pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&requisicao=montaGraficoIDEB&ensino=I"></td>
	<td width="30%" align="center" valign="top">
	<table width="100%">
		<tr>
		<td class="SubTituloCentro">Perguntas</td>
		<td class="SubTituloCentro">Sim</td>
		<td class="SubTituloCentro">Não</td>
		<td class="SubTituloCentro">Não se aplica</td>
		</tr>
		<tr>
		<td class="SubTituloDireita">Observando os dados, responda: o IDEB da escola vem melhorando nas duas últimas medições (desconsidere a meta)?</td>
		<td align="center"><input type="radio" name="ridinicialum" id="ridinicialum_S" value="S" <? echo (($ridinicialum=="S")?"checked ":""); echo $dados_ai['onclickperg']['S']; ?> ></td>
		<td align="center"><input type="radio" name="ridinicialum" id="ridinicialum_N" value="N" <? echo (($ridinicialum=="N")?"checked ":""); echo $dados_ai['onclickperg']['N']; ?> ></td>
		<td align="center"><input type="radio" name="ridinicialum" id="ridinicialum_A" value="A" <? echo (($ridinicialum=="A")?"checked ":""); echo $dados_ai['onclickperg']['A']; ?> ></td>
		</tr>
		<tr id="riddinicialdesc_tr" <? echo (($ridinicialdois=="N" || $ridinicialdois=="A" || $ridinicialdois=="")?"style=display:none;":""); ?> >
		<td colspan="4">Qual(is) é(são) a(s) evidência(s)?<br/>
		<? echo campo_textarea( 'riddinicialdesc', 'S', 'S', '', '70', '4', '200'); ?>
		</td>
		</tr>

	</table>
	</td>
</tr>
<? endif; ?>

<? if($possui_ensino_f) : ?>
<tr>
	<td class="SubTituloDireita" width="10%">Anos finais do Ensino Fundamental</td>
	<td width="30%" align="center" valign="top"><? $dados_af = montaTabelaIDEB($ensino='F'); echo $dados_af['html']; ?></td>
	<td width="30%" align="center" valign="top"><img src="pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&requisicao=montaGraficoIDEB&ensino=F"></td>
	<td width="30%" valign="top">
	<table width="100%">
		<tr>
		<td class="SubTituloCentro">Perguntas</td>
		<td class="SubTituloCentro">Sim</td>
		<td class="SubTituloCentro">Não</td>
		<td class="SubTituloCentro">Não se aplica</td>
		</tr>
		<tr>
		<td class="SubTituloDireita">Observando os dados, responda: o IDEB da escola vem melhorando nas duas últimas medições (desconsidere a meta)?</td>
		<td align="center"><input type="radio" name="ridfinalum" id="ridfinalum_S" value="S" <? echo (($ridfinalum=="S")?"checked ":""); echo $dados_af['onclickperg']['S']; ?> ></td>
		<td align="center"><input type="radio" name="ridfinalum" id="ridfinalum_N" value="N" <? echo (($ridfinalum=="N")?"checked ":""); echo $dados_af['onclickperg']['N']; ?> ></td>
		<td align="center"><input type="radio" name="ridfinalum" id="ridfinalum_A" value="A" <? echo (($ridfinalum=="A")?"checked ":""); echo $dados_af['onclickperg']['A']; ?> ></td>
		</tr>
		<tr id="ridfinaldesc_tr" <? echo (($ridfinaldois=="N" || $ridinicialdois=="A" || $ridfinaldois=="")?"style=display:none;":""); ?>>
		<td colspan="4">Qual(is) é(são) a(s) evidência(s)?<br/>
		<? echo campo_textarea( 'ridfinaldesc', 'S', 'S', '', '70', '4', '200'); ?>
		</td>
		</tr>

	</table>
	</td>
</tr>
<? endif; ?>

<? if(!$possui_ensino_i && !$possui_ensino_f) : ?>
<tr>
	<td class="SubTituloCentro" colspan="4">Não existem informações do IDEB referente a sua escola</td>
</tr>
<? endif; ?>

</table>

<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
<tr>
	<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
	<td>
		<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_0_orientacoes';" >
		<input type="button" name="btn_continuar" value="Salvar" onclick="document.getElementById('togo').value='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_1_ideb';diagnostico_1_1_ideb();">
		<input type="button" name="btn_continuar" value="Salvar e Continuar" onclick="document.getElementById('togo').value='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_2_taxasderendimento';diagnostico_1_1_ideb();">
		<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_2_taxasderendimento';" >
	</td>
</tr>
</table>
</form>
</div>
