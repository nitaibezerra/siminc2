<?
monta_titulo( "Finanças - Gestão", "&nbsp;");

?>
<script>

function diagnostico_4_3_financas() {
	if(!verificaRespostasPerguntas()){
		alert('Favor responder todas as perguntas!');
		return false;
	}
	
	var numChk = jQuery("[name^='recebeurecursosanoanterior']:enabled:checked").length;
	
	if(numChk == 0) {
		alert('Marque se a escola recebeu recursos no ano anterior.');
		return false;
	}
	
	divCarregando();
	document.getElementById('formulario').submit();
}

function gerenciarProgramasFinancas(fprid) {
	window.open('pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&requisicao=gerenciarProgramasFinancas&fprid='+fprid,'Programas','scrollbars=no,height=400,width=600,status=no,toolbar=no,menubar=no,location=no');
}

jQuery(document).ready(function() {
	document.getElementById('div_usos').innerHTML = "Carregando...";
	carregarProgramasFinancas();
	carregarDiferencaFontesUsos();
	<? if(carregarProgramasFinancas($dados=array('existeRegistros'=>true))) : ?>
	document.getElementById('td_fontesprogramas_grafico').style.display='';
	<? endif; ?>
	carregarResumoProgramasFinancas();
	carregarUsosFinancas();
	<? if(carregarUsosFinancas($dados=array('existeRegistros'=>true))) : ?>
	document.getElementById('td_usos_grafico').style.display='';
	<? endif; ?>
});

function carregarProgramasFinancas() {

	jQuery.ajax({
   		type: "POST",
   		url: "pdeinterativo2013.php?modulo=principal/diagnostico&acao=A",
   		data: "requisicao=carregarProgramasFinancas",
   		async: false,
   		success: function(msg){jQuery('#div_fontesprogramas').html(msg);}
 		});
	 		
}

function carregarResumoProgramasFinancas() {

	jQuery.ajax({
   		type: "POST",
   		url: "pdeinterativo2013.php?modulo=principal/diagnostico&acao=A",
   		data: "requisicao=carregarResumoProgramasFinancas",
   		async: false,
   		success: function(msg){jQuery('#div_fontesprogramas_resumo').html(msg);}
 		});
	 		
}

function carregarUsosFinancas() {

	jQuery.ajax({
   		type: "POST",
   		url: "pdeinterativo2013.php?modulo=principal/diagnostico&acao=A",
   		data: "requisicao=carregarUsosFinancas",
   		async: false,
   		success: function(msg){jQuery('#div_usos').html(msg);}
 		});
	 		
}

function excluirProgramasFinancas(fprid) {
	var msgs;
	var conf = confirm('Deseja realmente excluir o programa?');
	if(conf) {
	
		jQuery.ajax({
	   		type: "POST",
	   		url: "pdeinterativo2013.php?modulo=principal/diagnostico&acao=A",
	   		data: "requisicao=excluirProgramasFinancas&fprid="+fprid,
	   		async: false,
	   		success: function(msg){
				carregarProgramasFinancas();
				carregarDiferencaFontesUsos();
				carregarResumoProgramasFinancas();
				carregarImagemGrafico(fprid);
				msgs = msg.split(":");
				if(msgs[0] == '0') {
					document.getElementById('td_fontesprogramas_grafico').style.display='none';
				}
	   			alert(msgs[1]);
			}
	 		});
	 		
 	}
}

function somarGerenciarUsos() {
	var valores_custeio = new Array();
	var valores_capital = new Array();
	var total_custeio=0;
	var total_capital=0;
	var total_geral=0;
	var tabela = document.getElementById('div_usos').childNodes[0]; 
	for(var i=1;i<document.getElementById('div_usos').childNodes[0].rows.length-1;i++) {
		if(tabela.rows[i].cells[1].childNodes[0].value) {
			valores_custeio[i] = parseFloat(replaceAll(replaceAll(tabela.rows[i].cells[1].childNodes[0].value,'.',''),',','.'));
			total_custeio += parseFloat(replaceAll(replaceAll(tabela.rows[i].cells[1].childNodes[0].value,'.',''),',','.'));
		}
		if(tabela.rows[i].cells[2].childNodes[0].value) {
			valores_capital[i] = parseFloat(replaceAll(replaceAll(tabela.rows[i].cells[2].childNodes[0].value,'.',''),',','.'));
			total_capital += parseFloat(replaceAll(replaceAll(tabela.rows[i].cells[2].childNodes[0].value,'.',''),',','.')); 
		}
	}
	
	total_geral = total_custeio+total_capital;
	
	for(var i=1;i<document.getElementById('div_usos').childNodes[0].rows.length-1;i++) {
		if(valores_custeio[i] || valores_capital[i]) {
			var tot = valores_custeio[i]+valores_capital[i];
			var porcent = (tot/total_geral)*100;
			tabela.rows[i].cells[3].innerHTML = mascaraglobal('###.###.###,##',tot.toFixed(2));
			tabela.rows[i].cells[4].innerHTML = mascaraglobal('###,##',porcent.toFixed(2));
		}
	}
	tabela.rows[document.getElementById('div_usos').childNodes[0].rows.length-1].cells[1].innerHTML = mascaraglobal('###.###.###,##',total_custeio.toFixed(2));
	tabela.rows[document.getElementById('div_usos').childNodes[0].rows.length-1].cells[2].innerHTML = mascaraglobal('###.###.###,##',total_capital.toFixed(2));
	tabela.rows[document.getElementById('div_usos').childNodes[0].rows.length-1].cells[3].innerHTML = mascaraglobal('###.###.###,##',total_geral.toFixed(2));
	tabela.rows[document.getElementById('div_usos').childNodes[0].rows.length-1].cells[4].innerHTML = '100';

}

function carregarDiferencaFontesUsos() {
	jQuery.ajax({
   		type: "POST",
   		url: "pdeinterativo2013.php?modulo=principal/diagnostico&acao=A",
   		data: "requisicao=carregarDiferencaFontesUsos",
   		success: function(msg){
			document.getElementById('div_dif_usos').innerHTML=msg;
		}
 		});
}

function receberRecursosAnoAnterior(valor) {
	
	if(valor == "FALSE") {
		var conf = confirm("Caso você tenha gravado alguma fonte ou programa, estes serão apagados. Deseja continuar?");
		if(!conf) {
			return false;
		}
	}
	
	jQuery.ajax({
   		type: "POST",
   		url: "pdeinterativo2013.php?modulo=principal/diagnostico&acao=A",
   		data: "requisicao=gravarRecebimentoRecursosAnoAnterior&recebeurecursosanoanterior="+valor,
   		success: function(msg){
   			if(valor=="TRUE") {
				carregarProgramasFinancas();
				carregarResumoProgramasFinancas();
   				document.getElementById('td_fontesprogramas_grafico').style.display='none';
   				document.getElementById('recebeu_recursos').style.display='';
   			} else if(valor == "FALSE") {
   				document.getElementById('recebeu_recursos').style.display='none';
   			}
		}
 		});
 		
	
}

function gerenciarUsos() {

	somarGerenciarUsos();
	
	jQuery.ajax({
   		type: "POST",
   		url: "pdeinterativo2013.php?modulo=principal/diagnostico&acao=A",
   		data: "requisicao=gerenciarUsos&"+jQuery("[name^='fuscusteio[']").serialize()+"&"+jQuery("[name^='fuscapital[']").serialize(),
   		success: function(msg){
			carregarDiferencaFontesUsos();
			document.getElementById('td_usos_grafico').style.display='';
   			carregarImagemGrafico2();
			carregarImagemGrafico3();
		}
 		});
 		
	
}

function carregarImagemGrafico() {
	var hash = new Date();
	document.getElementById('img_grafico').src='pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&requisicao=montaGraficoFinancas&tipo=fontesprogramas&hash='+hash;
}
function carregarImagemGrafico2() {
	var hash = new Date();
	document.getElementById('img_grafico2').src='pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&requisicao=montaGraficoFinancas&tipo=usos&hash='+hash;
}
function carregarImagemGrafico3() {
	var hash = new Date();
	document.getElementById('img_grafico3').src='pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&requisicao=montarGraficoBarraFinancas&tipo=usos&hash='+hash;
}

</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%"><font class="blue">Orientações</font></td>
	<td class="blue">
	<p>Há duas informações essenciais para o diagnóstico da gestão financeira da escola: a indicação das fontes (ou seja, de onde vem os recursos) e dos usos (ou seja, para onde vão os recursos). Inicialmente, vamos indicar as Fontes de recursos. Para tanto, clique no botão "Incluir fontes e programas", informe todos os programas que repassaram recursos para a escola e qual a esfera responsável por esses programas (se federal, estadual, municipal ou outra).</p> 
	<p>Na tabela seguinte, vamos indicar os Usos. Neste caso, preencha os campos em aberto com o valor gasto em cada item, diferenciando capital de custeio. Despesas de capital são aquelas que visam formar ou adquirir um ativo real, ou seja, um bem que pode ser incorporado ao patrimônio da Secretaria. Despesas de custeio são aquelas necessárias à prestação de serviços e à manutenção da própria escola.</p>
	<p>As despesas que não estiverem contempladas na lista deverão ser registradas no campo "Outras despesas". Caso o valor total das despesas seja superior ou inferior ao total de recursos recebidos e declarados na tabela de "Fontes", o saldo será exibido na última linha (seja ele positivo ou negativo). Discuta com o Grupo de Trabalho se a gestão financeira da escola está adequada e, em seguida, responda às perguntas.</p>
	</td>
</tr>
</table>

<form method="post" id="formulario">
<input type="hidden" name="togo" id="togo" value="/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_4_sintesedimensao4">
<input type="hidden" name="requisicao" value="diagnostico_4_3_financas">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%">Fontes e programas</td>
	<td>
	<? $recebeurecursosanoanterior = $db->pegaUm("SELECT recebeurecursosanoanterior FROM pdeinterativo2013.flag WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'"); ?>
	<p>A escola recebeu recursos no ano anterior? <input type="radio" name="recebeurecursosanoanterior" value="TRUE" onclick="if(this.checked){receberRecursosAnoAnterior(this.value)}" <?=(($recebeurecursosanoanterior=="t")?"checked":"") ?> > Sim <input type="radio" name="recebeurecursosanoanterior" value="FALSE" onclick="if(this.checked){receberRecursosAnoAnterior(this.value)}" <?=(($recebeurecursosanoanterior=="f")?"checked":"") ?> > Não</p>
	<div id="recebeu_recursos" <?=(($recebeurecursosanoanterior=="t")?"":"style=display:none") ?> >
	<p><input type="button" name="fontesprogramas" value="Incluir Fontes e Programa(s)" onclick="gerenciarProgramasFinancas('');"></p>	
	<table width="100%">
	<tr>
	<td valign="top" width="33%">
	<div id="div_fontesprogramas"></div>
	</td>
	<td valign="top" width="33%">
	<div id="div_fontesprogramas_resumo"></div>
	</div>
	</td>
	<td valign="top" align="center" width="33%" id="td_fontesprogramas_grafico" style="display:none">
	<img src="pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&requisicao=montaGraficoFinancas&tipo=fontesprogramas" id="img_grafico">
	</td>
	</tr>
	</table>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="10%">Usos</td>
	<td>
	<table width="100%">
	<tr>
		<td valign="top" colspan="2" align="center">
		<div id="div_usos"></div>
		<div id="div_dif_usos"></div>
		<div id="td_usos_grafico" style="display:none;width:100%;">
		<br>
		<img src="pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&requisicao=montarGraficoBarraFinancas&tipo=usos" id="img_grafico3">
		<img src="pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&requisicao=montaGraficoFinancas&tipo=usos" id="img_grafico2">		
		</div>
		</td>
	</tr>
	</table>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="10%">Gestão Financeira</td>
	<td>
	<?php $sql = "select * from pdeinterativo2013.pergunta where prgmodulo = 'G' and prgsubmodulo = 'F' and prgstatus = 'A' and prgdetalhe='Gestão Financeira' order by prgid"; ?>
	<?php quadroPerguntas($sql) ?>
	</td>
</tr>
</table>
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
<tr>
	<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
	<td>
	<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_2_processos';" >
	<input type="button" name="btn_continuar" value="Salvar" onclick="document.getElementById('togo').value='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_3_financas';diagnostico_4_3_financas();">
	<input type="button" name="btn_continuar" value="Salvar e Continuar" onclick="document.getElementById('togo').value='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_4_sintesedimensao4';diagnostico_4_3_financas();">
	<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_4_sintesedimensao4';" >
	</td>
</tr>
</table>
</form>