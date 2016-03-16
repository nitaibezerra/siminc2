<script>



function diagnostico_1_3_provabrasil() {


	if(document.getElementById('rpbinicialport_S')) {
		var checked = false;
	
		for(var i=0;i<document.getElementsByName('rpbinicialport').length;i++) {
			if(document.getElementsByName('rpbinicialport')[i].checked) {
				checked = true;
			}
		}
		
		if(checked == false) {
			alert('Marque a opção: Os resultados de Língua Portuguesa na Prova Brasil nos anos iniciais do Ensino Fundamental demonstram evolução nos últimos anos?');
			return false;		
		}
		
		var checked = false;
		
		for(var i=0;i<document.getElementsByName('rpbinicialmat').length;i++) {
			if(document.getElementsByName('rpbinicialmat')[i].checked) {
				checked = true;
			}
		}
		
		if(checked == false) {
			alert('Marque a opção: Os resultados de Matemática na Prova Brasil nos anos iniciais do Ensino Fundamental demonstram evolução nos últimos anos?');
			return false;		
		}
		
	}
	
	if(document.getElementById('rpbfinalport_S')) {
		var checked = false;
	
		for(var i=0;i<document.getElementsByName('rpbfinalport').length;i++) {
			if(document.getElementsByName('rpbfinalport')[i].checked) {
				checked = true;
			}
		}
		
		if(checked == false) {
			alert('Marque a opção: Os resultados de Língua Portuguesa na Prova Brasil nos anos finais do Ensino Fundamental demonstram evolução nos últimos anos?');
			return false;		
		}
		
		var checked = false;
		
		for(var i=0;i<document.getElementsByName('rpbfinalmat').length;i++) {
			if(document.getElementsByName('rpbfinalmat')[i].checked) {
				checked = true;
			}
		}
		
		if(checked == false) {
			alert('Marque a opção: Os resultados de Matemática na Prova Brasil nos anos finais do Ensino Fundamental demonstram evolução nos últimos anos?');
			return false;		
		}
		
	}

	divCarregando();
	document.getElementById('formulario').submit();
}

</script>
<?
monta_titulo( "Prova Brasil - Indicadores e Taxas", "&nbsp;");

$sql = "SELECT * FROM pdeinterativo.respostaprovabrasil WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND rbdstatus='A'";
$dadospb = $db->pegaLinha($sql);

if($dadospb) {
	extract($dadospb);
	$style_display = '';
} else {
	$style_display = 'style="display:none"';
}

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%"><font class="blue">Orientações</font></td>
	<td class="blue">
	<p>Agora vamos analisar o último indicador desta Dimensão. A Prova Brasil e o Sistema Nacional de Avaliação da Educação Básica (Saeb) são avaliações para diagnóstico e avaliação da qualidade do ensino oferecido pelo sistema educacional brasileiro. Nos testes aplicados na 4ª (quarta) e 8ª (oitava) séries (quinto e nono anos) do Ensino Fundamental e na 3ª (terceira) série do Ensino Médio, os estudantes respondem a questões de Língua Portuguesa (com foco em leitura) e Matemática (com foco na resolução de problemas). No questionário socioeconômico, os estudantes fornecem informações sobre fatores de contexto que podem estar associados ao desempenho.</p>
	<p>As médias de desempenho nessas avaliações ajudam a definir ações de aprimoramento da qualidade da educação e também subsidiam o cálculo do Índice de Desenvolvimento da Educação Básica (IDEB), ao lado das taxas de aprovação nessas esferas.</p>
	<p>Clique no botão abaixo para visualizar a(s) tabela(s) e o(s) gráfico(s) que mostram a série histórica da Prova Brasil no Brasil, Estado, Município e os resultados da própria escola, em cada etapa oferecida no ano do Censo.</p>
	<p>Analise os gráficos e, em seguida, responda às perguntas. Caso a escola não possua resultados em pelo menos dois anos, assinale a opção “Não se aplica”, mas não deixe de comentar os resultados do seu município, estado e do Brasil.</p>
	
	<? if($style_display) : ?>
	<p>Clique no botão abaixo e veja os resultados do Brasil, Estado, Município e da sua escola na Prova Brasil, nas três últimas medições. Analise a evolução desse indicador e, em seguida, responda às perguntas.</p>
	<p><input type="button" name="verideb" value="CLIQUE AQUI para visualizar os dados da Prova Brasil" onclick="document.getElementById('div_provabrasil').style.display='';"></p>
	<? endif; ?> 
	</td>
</tr>
</table>
<div id="div_provabrasil" <?=$style_display ?> >
<form method="post" id="formulario">
<input type="hidden" name="togo" id="togo" value="/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_4_sintesedimensao1">
<input type="hidden" name="requisicao" value="diagnostico_1_3_provabrasil">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<?
$possui_ai = possuiEnsino($ensino='I', $submodulo='P'); 
$possui_af = possuiEnsino($ensino='F', $submodulo='P');
?>
<? if($possui_ai) : ?>
<tr>
	<td class="SubTituloDireita" width="10%">Anos iniciais do Ensino Fundamental</td>
	<td width="30%" align="center">
	<? $dados_ai_p = montaTabelaProvaBrasil($ensino='I',$materia='P'); echo $dados_ai_p['html']; ?>
	<? $dados_ai_m = montaTabelaProvaBrasil($ensino='I',$materia='M'); echo $dados_ai_m['html']; ?>
	</td>
	<td width="30%" align="center">
	<img src="pdeinterativo.php?modulo=principal/diagnostico&acao=A&requisicao=montaGraficoProvaBrasil&ensino=I&materia=P">
	<img src="pdeinterativo.php?modulo=principal/diagnostico&acao=A&requisicao=montaGraficoProvaBrasil&ensino=I&materia=M">
	</td>
	<td width="30%" valign="top">
	<table width="100%">
		<tr>
		<td class="SubTituloCentro">Perguntas</td>
		<td class="SubTituloCentro">Sim</td>
		<td class="SubTituloCentro">Não</td>
		<td class="SubTituloCentro">Não se aplica</td>
		</tr>
		<tr>
		<td class="SubTituloDireita">Os resultados de Língua Portuguesa na Prova Brasil nos anos iniciais do Ensino Fundamental demonstram evolução nas duas últimas medições?</td>
		<td align="center"><input type="radio" name="rpbinicialport" id="rpbinicialport_S" value="S" <? echo (($rpbinicialport=="S")?"checked":""); echo $dados_ai_p['onclickperg']['S']; ?> ></td>
		<td align="center"><input type="radio" name="rpbinicialport" id="rpbinicialport_N" value="N" <? echo (($rpbinicialport=="N")?"checked":""); echo $dados_ai_p['onclickperg']['N']; ?> ></td>
		<td align="center"><input type="radio" name="rpbinicialport" id="rpbinicialport_A" value="A" <? echo (($rpbinicialport=="A")?"checked":""); echo $dados_ai_p['onclickperg']['A']; ?> ></td>
		</tr>
		<tr>
		<td class="SubTituloDireita">Os resultados de Matemática na Prova Brasil nos anos iniciais do Ensino Fundamental demonstram evolução nas duas últimas medições? </td>
		<td align="center"><input type="radio" name="rpbinicialmat" value="S" <? echo (($rpbinicialmat=="S")?"checked":""); echo $dados_ai_m['onclickperg']['S']; ?> ></td>
		<td align="center"><input type="radio" name="rpbinicialmat" value="N" <? echo (($rpbinicialmat=="N")?"checked":""); echo $dados_ai_m['onclickperg']['N']; ?> ></td>
		<td align="center"><input type="radio" name="rpbinicialmat" value="A" <? echo (($rpbinicialmat=="A")?"checked":""); echo $dados_ai_m['onclickperg']['A']; ?> ></td>
		</tr>

	</table>
	</td>
</tr>
<? endif; ?>
<? if($possui_af) : ?>
<tr>
	<td class="SubTituloDireita" width="10%">Anos finais do Ensino Fundamental</td>
	<td width="30%" align="center">
	<? $dados_af_p = montaTabelaProvaBrasil($ensino='F',$materia='P'); echo $dados_af_p['html']; ?>
	<? $dados_af_m = montaTabelaProvaBrasil($ensino='F',$materia='M'); echo $dados_af_m['html']; ?>
	</td>
	<td width="30%" align="center">
	<img src="pdeinterativo.php?modulo=principal/diagnostico&acao=A&requisicao=montaGraficoProvaBrasil&ensino=F&materia=P">
	<img src="pdeinterativo.php?modulo=principal/diagnostico&acao=A&requisicao=montaGraficoProvaBrasil&ensino=F&materia=M">
	</td>
	<td width="30%" valign="top">
	<table width="100%">
		<tr>
		<td class="SubTituloCentro">Perguntas</td>
		<td class="SubTituloCentro">Sim</td>
		<td class="SubTituloCentro">Não</td>
		<td class="SubTituloCentro">Não se aplica</td>
		</tr>
		<tr>
		<td class="SubTituloDireita">Os resultados de Língua Portuguesa na Prova Brasil nos anos finais do Ensino Fundamental demonstram evolução nas duas últimas medições?</td>
		<td align="center"><input type="radio" name="rpbfinalport" value="S" <? echo (($rpbfinalport=="S")?"checked":""); echo $dados_af_p['onclickperg']['S']; ?> ></td>
		<td align="center"><input type="radio" name="rpbfinalport" value="N" <? echo (($rpbfinalport=="N")?"checked":""); echo $dados_af_p['onclickperg']['N']; ?> ></td>
		<td align="center"><input type="radio" name="rpbfinalport" value="A" <? echo (($rpbfinalport=="A")?"checked":""); echo $dados_af_p['onclickperg']['A']; ?> ></td>
		</tr>
		<tr>
		<td class="SubTituloDireita">Os resultados de Matemática na Prova Brasil nos anos finais do Ensino Fundamental demonstram evolução nas duas últimas medições?</td>
		<td align="center"><input type="radio" name="rpbfinalmat" value="S" <? echo (($rpbfinalmat=="S")?"checked":""); echo $dados_af_m['onclickperg']['S']; ?> ></td>
		<td align="center"><input type="radio" name="rpbfinalmat" value="N" <? echo (($rpbfinalmat=="N")?"checked":""); echo $dados_af_m['onclickperg']['N']; ?> ></td>
		<td align="center"><input type="radio" name="rpbfinalmat" value="A" <? echo (($rpbfinalmat=="A")?"checked":""); echo $dados_af_m['onclickperg']['A']; ?> ></td>
		</tr>

	</table>
	</td>
</tr>
<? endif; ?>

<? if(!$possui_ai && !$possui_af) : ?>
<tr>
	<td class="SubTituloCentro" colspan="4">Não existem informações da Prova Brasil referente a sua escola</td>
</tr>
<? endif; ?>

</table>

<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
<tr>
	<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
	<td>
	<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_2_taxasderendimento';" >
	<input type="button" name="btn_continuar" value="Salvar" onclick="document.getElementById('togo').value='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_3_provabrasil';diagnostico_1_3_provabrasil();">
	<input type="button" name="btn_continuar" value="Salvar e Continuar" onclick="document.getElementById('togo').value='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_4_sintesedimensao1';diagnostico_1_3_provabrasil();">
	<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_4_sintesedimensao1';" >
	</td>
</tr>
</table>

</form>
</div>