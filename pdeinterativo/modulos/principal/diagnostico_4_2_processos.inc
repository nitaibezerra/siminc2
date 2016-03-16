<?
monta_titulo( "Processos - Gestão", "&nbsp;");

?>
<script>

function diagnostico_4_2_processos() {
	if(!verificaRespostasPerguntas()){
		alert('Favor responder todas as perguntas!');
		return false;
	}
	
	var totalPerguntas=parseInt(jQuery("[name^='perg[']:enabled").length/4);
	var max = parseInt(Math.floor(totalPerguntas/2))+1;
	var numRespS = parseInt(jQuery("[name^='perg['][value=<?=OPP_SEMPRE ?>]:enabled:checked").length);
	var numRespM = parseInt(jQuery("[name^='perg['][value=<?=OPP_MAIORIA_DAS_VEZES ?>]:enabled:checked").length);
	var totresp = numRespS+numRespM;
	if(totresp>max) {
		if(document.getElementById('tr_justificativasevidencias').style.display=='none') {
			document.getElementById('tr_justificativasevidencias').style.display='';
		}
		if(jQuery('#juedescricao').val()=='') {
			alert('Foram assinaladas mais de 50% das respostas com as opções “Sempre” e “Na maioria das vezes”. À luz dos resultados analisados nas dimensões 1 e 2, justifique as respostas.');
			return false;
		}
	} else {
		if(document.getElementById('tr_justificativasevidencias').style.display=='') {
			alert('A justificativa não é mais necessária e será removida.');
			document.getElementById('tr_justificativasevidencias').style.display='none';
			jQuery('juedescricao').val('');
		}
	}


	divCarregando();
	document.getElementById('formulario').submit();
}

</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%"><font class="blue">Orientações</font></td>
	<td class="blue">
	<p>Os processos referem-se, quase sempre, às atividades rotineiras realizadas na escola, mas que nem por isso deixam de ser importantes, pelo contrário. Quando a equipe gestora consome mais tempo em atividades administrativas e burocráticas do que nas atividades pedagógicas, na mobilização social e na inovação, é hora de rever os processos.</p>
	<p>As questões abaixo apresentam temas importantes e que devem ser respondidos pela equipe com tranqüilidade. Observe que, mais do que questionar, as perguntas sugerem possíveis soluções para os temas abordados. Não se preocupe com a resposta “certa” e sim com a resposta verdadeira. Responda às perguntas assinalando em que medida o Grupo de Trabalho concorda com elas.</p>
	</td>
</tr>
</table>

<form method="post" id="formulario">
<input type="hidden" name="togo" id="togo" value="/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_3_financas">
<input type="hidden" name="requisicao" value="diagnostico_4_2_processos">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%">Planejamento</td>
	<td>
	<?php $sql = "select * from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Planejamento' order by prgid"; ?>
	<?php quadroPerguntas($sql) ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="10%">Rotinas</td>
	<td>
	<?php $sql = "select * from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Rotinas' order by prgid"; ?>
	<?php quadroPerguntas($sql) ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="10%">Normas e Regulamentos</td>
	<td>
	<?php $sql = "select * from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Normas e Regulamentos' order by prgid"; ?>
	<?php quadroPerguntas($sql) ?>
	</td>
</tr>
<? $juedescricao = $db->pegaUm("select juedescricao from pdeinterativo.justificativaevidencias where abacod='diagnostico_4_2_processos' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'"); ?>
<tr id="tr_justificativasevidencias" <?=(($juedescricao)?'':'style="display:none"') ?>>
	<td class="SubTituloDireita" width="10%">Justificativas/ Evidências</td>
	<td>
	<? echo campo_textarea( 'juedescricao', 'S', 'S', '', '100', '5', '1000'); ?>
	</td>
</tr>
</table>
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
<tr>
	<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
	<td>
	<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_1_direcao';" >
	<input type="button" name="btn_continuar" value="Salvar" onclick="document.getElementById('togo').value='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_2_processos';diagnostico_4_2_processos();">
	<input type="button" name="btn_continuar" value="Salvar e Continuar" onclick="document.getElementById('togo').value='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_3_financas';diagnostico_4_2_processos();">
	<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_3_financas';" >
	</td>
</tr>
</table>
</form>