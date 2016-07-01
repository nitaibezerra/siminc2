<?
monta_titulo( "Demais Profissionais - Comunidade Escolar", "&nbsp;");
?>
<script>

function diagnostico_5_3_demaisprofissionais() {
	if(!verificaRespostasPerguntas()){
		alert('Favor responder todas as perguntas!');
		return false;
	}

	divCarregando();
	document.getElementById('formulario').submit();
}

function gerenciarDemaisProfissionais(pesid) {
	window.open('pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&requisicao=gerenciarDemaisProfissionais&pesid='+pesid,'Profissionais','scrollbars=no,height=400,width=600,status=no,toolbar=no,menubar=no,location=no');
}

function carregarDemaisProfissionais() {

	jQuery.ajax({
   		type: "POST",
   		url: "pdeinterativo2013.php?modulo=principal/diagnostico&acao=A",
   		data: "requisicao=carregarDemaisProfissionais",
   		async: false,
   		success: function(msg){jQuery('#div_demaisprofissionais').html(msg);}
 		});
	 		
}

function excluirDemaisProfissionais(pesid) {
	var conf = confirm('Deseja realmente excluir?');
	
	if(conf) {

		jQuery.ajax({
	   		type: "POST",
	   		url: "pdeinterativo2013.php?modulo=principal/diagnostico&acao=A",
	   		data: "requisicao=excluirDemaisProfissionais&pesid="+pesid,
	   		async: false,
	   		success: function(msg){
	   			carregarDemaisProfissionais();
	   			alert(msg);
	   		}
	 		});
 		
 	}
	 		
}

jQuery(document).ready(function() {
	carregarDemaisProfissionais();
});

</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%"><font class="blue">Orientações</font></td>
	<td class="blue">
	<p>O clima escolar traduz a atmosfera geral da escola e para promover um ambiente harmonioso é fundamental que a equipe interaja positivamente. Além dos gestores, docentes e estudantes, os demais profissionais que trabalham na escola realizam atividades importantes para propiciar um ambiente favorável ao aprendizado.</p>
	<p>A seguir, vamos conhecer um pouco mais sobre esses profissionais e as relações internas de toda a equipe. Para tanto, insira os nomes dos demais profissionais que atuam na escola, preenchendo os campos solicitados. Em seguida, leia as perguntas e assinale em que medida o Grupo de Trabalho concorda com elas.</p>
	</td>
</tr>
</table>

<form method="post" id="formulario">
<input type="hidden" name="togo" id="togo" value="/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_4_paisecomunidade">
<input type="hidden" name="requisicao" value="diagnostico_5_3_demaisprofissionais">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%">Perfil</td>
	<td>
	<p><input type="button" value="Inserir Demais Profissionais" onclick="gerenciarDemaisProfissionais('');"></p>
	<div id="div_demaisprofissionais"></div>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="10%">Cooperação e respeito</td>
	<td>
	<?php $sql = "select * from pdeinterativo2013.pergunta where prgmodulo = 'C' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Cooperação e Respeito' order by prgid"; ?>
	<?php quadroPerguntas($sql) ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="10%">Motivação</td>
	<td>
	<?php $sql = "select * from pdeinterativo2013.pergunta where prgmodulo = 'C' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Motivação' order by prgid"; ?>
	<?php quadroPerguntas($sql) ?>
	</td>
</tr>
</table>

<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
<tr>
	<td class="SubTituloDireita">O que deseja fazer agora?</td>
	<td>
	<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_2_docentes';" >
	<input type="button" name="btn_continuar" value="Salvar" onclick="document.getElementById('togo').value='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_3_demaisprofissionais';diagnostico_5_3_demaisprofissionais();">
	<input type="button" name="btn_continuar" value="Salvar e Continuar" onclick="document.getElementById('togo').value='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_4_paisecomunidade';diagnostico_5_3_demaisprofissionais();">
	<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_4_paisecomunidade';" >
	</td>
</tr>
</table>
</form>