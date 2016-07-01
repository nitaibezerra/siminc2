<?
monta_titulo( "Planejamento Pedagógico - Ensino e aprendizagem", "&nbsp;");

function calculaQuantidadePerguntasRespostas($dados) {
	global $db;
	
	if($dados['sqls']) {
		$sql_total = "select count(*) from ((".implode(") union all (",$dados['sqls']).")) foo";
		$tperguntas = $db->pegaUm($sql_total);
		
		foreach($dados['sqls'] as $sqlp) {
			$dados['sqlsn'][] = str_replace("pdeinterativo2013.pergunta", "pdeinterativo2013.pergunta p inner join pdeinterativo2013.respostapergunta rp on rp.prgid=p.prgid and pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' and oppid in(".OPP_SEMPRE.",".OPP_MAIORIA_DAS_VEZES.")",$sqlp);
		}
		
		$sql_total = "select count(*) from ((".implode(") union all (",$dados['sqlsn']).")) foo";
		$trespostas = $db->pegaUm($sql_total);
		
	}
	
	return array('tperguntas' => $tperguntas, 'trespostas' => $trespostas);
	
}

?>
<script>

function diagnostico_3_1_planejamentopedagogico() {
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
	<p>Neste momento, a escola deverá fazer uma reflexão sobre o Projeto Pedagógico, o Currículo e as Avaliações. Estes temas, embora pareçam corriqueiros ou já superados pela escola, muitas vezes representam os pontos mais frágeis da escola, pois tendem apenas a reproduzir modelos e paradigmas. Sabemos que, em muitos casos, a equipe gestora aproveita o que já vem sendo feito, sem questionar a eficácia e a efetividade dessas práticas e instrumentos.</p>
	<p>Assim, antes de assinalar uma resposta, é preciso que o Grupo de Trabalho reflita e debata sobre cada sentença. Não existe resposta "certa" ou "errada", apenas respostas verdadeiras e que correspondem à situação que mais se aproxima da realidade da escola.</p>
	<p>Nesta tela, se o total de respostas “sempre” e “na maioria das vezes” for superior à 50% do total de perguntas, é necessário que a escola apresente, no final da tela, justificativas ou evidências sobre esses aspectos, às luz dos resultados apresentados nas dimensões 1 e 2. Ou seja, se os desafios do planejamento pedagógico foram superados, porque os resultados da escola não são melhores?</p>
	<p>No caso das respostas “raramente” ou “nunca” a questão é apresentada na síntese da dimensão como um possível problema, cabendo à escola decidir se ele é crítico ou não.</p>
	</td>
</tr>
</table>

<form method="post" id="formulario">
<input type="hidden" name="togo" id="togo" value="/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_2_tempodeaprendizagem">
<input type="hidden" name="requisicao" value="diagnostico_3_1_planejamentopedagogico">
<?
$existe_infantil = false;
$existe_fundamental = false;
if($_SESSION['pdeinterativo2013_vars']['pdicodinep']){
	$arrNiveis = recuperaNiveisEnsinoPorCodigoINEP($_SESSION['pdeinterativo2013_vars']['pdicodinep']);
	if($arrNiveis){
		foreach($arrNiveis as $nivel){
			if(strstr($nivel['nivel'],"Educação Infantil")){
				$existe_infantil = true;
			}
		}
		foreach($arrNiveis as $nivel){
			if(strstr($nivel['nivel'],"Ensino Fundamental")){
				$existe_fundamental = true;
			}
		}
	}
}
if($existe_infantil && $existe_fundamental){
	$sql1 = "select * from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Projeto Pedagógico' union all select * from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Projeto Pedagógico - Educação Infantil'";
	$sql2 = "select * from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Currículo' union all select * from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Currículo - Educação Infantil'";
	$sql3 = "select * from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Avaliações' union all select * from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Avaliações - Educação Infantil'";
}elseif(!$existe_infantil && $existe_fundamental){
	$sql1 = "select * from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Projeto Pedagógico'";
	$sql2 = "select * from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Currículo'";
	$sql3 = "select * from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Avaliações'";	
}elseif($existe_infantil && !$existe_fundamental){
	$sql1 = "select * from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Projeto Pedagógico - Educação Infantil'";
	$sql2 = "select * from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Currículo - Educação Infantil'";
	$sql3 = "select * from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Avaliações - Educação Infantil'";	
}

if(!$sql1 || !$sql2 || !$sql3){
	$sql1 = "select * from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Projeto Pedagógico' union all select * from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Projeto Pedagógico - Educação Infantil'";
	$sql2 = "select * from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Currículo' union all select * from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Currículo - Educação Infantil'";
	$sql3 = "select * from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Avaliações' union all select * from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Avaliações - Educação Infantil'";
}

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%">Projeto Pedagógico</td>
	<td>
	<?php quadroPerguntas($sql1) ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="10%">Currículo</td>
	<td>
	<?php quadroPerguntas($sql2) ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="10%">Avaliações</td>
	<td>
	<?php quadroPerguntas($sql3) ?>
	</td>
</tr>
<? $juedescricao = $db->pegaUm("select juedescricao from pdeinterativo2013.justificativaevidencias where abacod='diagnostico_3_1_planejamentopedagogico' AND pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'"); ?>
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
	<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_0_orientacoes';" >
	<input type="button" name="btn_continuar" value="Salvar" onclick="document.getElementById('togo').value='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_1_planejamentopedagogico';diagnostico_3_1_planejamentopedagogico();">
	<input type="button" name="btn_continuar" value="Salvar e Continuar" onclick="document.getElementById('togo').value='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_2_tempodeaprendizagem';diagnostico_3_1_planejamentopedagogico();">
	<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_2_tempodeaprendizagem';" >
	</td>
</tr>
</table>
</form>