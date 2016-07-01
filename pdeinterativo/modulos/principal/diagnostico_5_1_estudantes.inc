<?
monta_titulo( "Estudantes - Comunidade Escolar", "&nbsp;");
?>
<script>

function diagnostico_5_1_estudantes() {
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
	<p>A formação cidadã é um dos principais objetivos do processo educacional. Para tanto, é fundamental que a escola ofereça oportunidades de desenvolvimento integral aos estudantes, estimulando o compromisso com a escola e com o seu auto-desenvolvimento, bem como promovendo práticas e situações que incentivem o protagonismo e a participação ativa na vida escolar e da comunidade.</p>
	<p>Leia as frases abaixo e assinale em que medida o Grupo de Trabalho concorda com cada pergunta. Quando as respostas “sempre” e “na maioria das vezes” somarem mais de 50% em relação ao total de perguntas, é necessário que a escola apresente, no final da tela, justificativas ou evidências sobre esses aspectos, à luz dos resultados apresentados nas dimensões 1 e 2. Ou seja, se os estudantes são tão comprometidos e engajados, porque os resultados da escola não são melhores?</p>
	</td>
</tr>
</table>

<form method="post" id="formulario">
<input type="hidden" name="togo" id="togo" value="/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_2_docentes">
<input type="hidden" name="requisicao" value="diagnostico_5_1_estudantes">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%">Compromisso</td>
	<td>
	<?php $sql = "select * from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'E' and prgstatus = 'A' and prgdetalhe='Compromisso' order by prgid"; ?>
	<?php quadroPerguntas($sql) ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="10%">Protagonismo e Participação</td>
	<td>
	<?php $sql = "select * from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'E' and prgstatus = 'A' and prgdetalhe='Protagonismo e Participação' order by prgid"; ?>
	<?php quadroPerguntas($sql) ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="10%">Saúde e Bem-estar</td>
	<td>
	<?php $sql = "select * from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'E' and prgstatus = 'A' and prgdetalhe='Saúde e Bem-estar' order by prgid"; ?>
	<?php quadroPerguntas($sql) ?>
	</td>
</tr>
<? $juedescricao = $db->pegaUm("select juedescricao from pdeinterativo.justificativaevidencias where abacod='diagnostico_5_1_estudantes' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'"); ?>
<tr id="tr_justificativasevidencias" <?=(($juedescricao)?'':'style="display:none"') ?>>
	<td class="SubTituloDireita" width="10%">Justificativas/ Evidências</td>
	<td>
	<? echo campo_textarea( 'juedescricao', 'S', 'S', '', '100', '5', '1000'); ?>
	</td>
</tr>
</table>

<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
<tr>
	<td class="SubTituloDireita">O que deseja fazer agora?</td>
	<td>
	<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_0_orientacoes';" >
	<input type="button" name="btn_continuar" value="Salvar" onclick="document.getElementById('togo').value='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_1_estudantes';diagnostico_5_1_estudantes();">
	<input type="button" name="btn_continuar" value="Salvar e Continuar" onclick="document.getElementById('togo').value='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_2_docentes';diagnostico_5_1_estudantes();">
	<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_2_docentes';" >
	</td>
</tr>
</table>
</form>