<?
monta_titulo( "Tempo de aprendizagem - Ensino e aprendizagem", "&nbsp;");

$sql = "SELECT * FROM pdeinterativo.respostatempoaprendizagem WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
$dadosta = $db->pegaLinha($sql);

$rtaestrategia = array();
if($dadosta['rtaestrategia']) {
	$rtaestrategia = explode(";",$dadosta['rtaestrategia']);
}
$rtaestudantepart = array();
if($dadosta['rtaestudantepart']) {
	$rtaestudantepart = explode(";",$dadosta['rtaestudantepart']);
}
$rtaatividade = array();
if($dadosta['rtaatividade']) {
	$rtaatividade = explode(";",$dadosta['rtaatividade']);
	foreach($rtaatividade as $dado) {
		$d = explode(",",$dado);
		$atv[$d[0]] = true;
		$qtd[$d[0]] = $d[1];
	}
}

$rtaporque = array();
if($dadosta['rtaporque']) {
	$rtaporque = explode(";",$dadosta['rtaporque']);
}

$rtamacrocampo = array();
if($dadosta['rtamacrocampo']) {
	$rtamacrocampo = explode(";",$dadosta['rtamacrocampo']);
}

?>
<script>

function diagnostico_3_1_planejamentopedagogico() {
	if(!verificaRespostasPerguntas()){
		alert('Favor responder todas as perguntas!');
		return false;
	}
	
	if(document.getElementById('rtacaso_S').checked) {
		var estrategias = document.getElementsByName('rtaestrategia[]');
		var estrategia_marcado=false;
		for(var i=0;i<estrategias.length;i++) {
			if(estrategias[i].checked==true) {
				estrategia_marcado=true;
			}
		}
		if(estrategia_marcado==false) {
			alert('Selecione uma estratégia');
			return false;
		}
		if(document.getElementById('qtd_propria_escola').value=="" || document.getElementById('qtd_outra_escola').value=="") {
			alert('Preencha a quantidade de estudantes/escolas participam de atividade de educação integral');
			return false;
		}
		if(document.getElementById('carga_horaria_semanal').value=="") {
			alert('Preencha carga horária das ações de educação integral, por semana');
			return false;
		}
		
		
		var macro = document.getElementsByName('rtamacrocampo[]');
		var macro_marcado=false;
		for(var i=0;i<macro.length;i++) {
			if(macro[i].checked==true) {
				macro_marcado=true;
			}
		}
		
		if(macro_marcado==false) {
			alert('Selecione macrocampos das atividades desenvolvidas');
			return false;
		}



	} else if(document.getElementById('rtacaso_N').checked) {
	
		var porque = document.getElementsByName('porque[]');
		var porque_marcado=false;
		for(var i=0;i<porque.length;i++) {
			if(porque[i].checked==true) {
				porque_marcado=true;
			}
		}
		if(porque_marcado==false) {
			alert('Selecione um porque');
			return false;
		}
		
		if(document.getElementById('tr_rtaporqueoutro').style.display=='') {
			if(document.getElementById('rtaporqueoutro').value=='') {
				alert('Preencha "Qual?"');
				return false;
			}
		}
	
	} else {

		alert('Selecione se a escola desenvolve ações de Educação Integral');
		return false;	
	
	}
	
	divCarregando();	
	document.getElementById('formulario').submit();
}

function desenvolveEducacaoIntegral(resp) {
	if(resp=='S') {
		document.getElementById('porque_espacofisico').checked=false;
		document.getElementById('porque_profissionais').checked=false;
		document.getElementById('porque_recursosmateriais').checked=false;
		document.getElementById('porque_outro').checked=false;

		document.getElementById('educ_integral_N').style.display='none';
		document.getElementById('educ_integral_S').style.display='';
	} else {
		document.getElementById('estrategia_P').checked=false;
		document.getElementById('estrategia_I').checked=false;
		document.getElementById('estrategia_O').checked=false;
		document.getElementById('qtd_propria_escola').value='';
		document.getElementById('qtd_outra_escola').value='';
		document.getElementById('carga_horaria_semanal').value='';
		
		document.getElementById('educ_integral_S').style.display='none';
		document.getElementById('educ_integral_N').style.display='';
	}
}

</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%"><font class="blue">Orientações</font></td>
	<td class="blue">
	<p>Nesta etapa, reflita sobre a importância de assegurar aos estudantes o tempo de aprendizagem, respondendo às questões relacionadas a este tema e também sobre a ampliação dos tempos, espaços e oportunidades de aprendizagem.</p>
	<p>O foco do debate, além da carga horária dedicada efetivamente às atividades educativas, deve incorporar também questões relativas ao uso qualitativo desse tempo, ou seja, se a escola adota de fato meios que estimulem o crescimento individual e coletivo.</p>
	<p>Assinale a resposta correspondente à situação que mais se aproxima da realidade da escola e em seguida responda sobre às questões sobre a “Jornada ampliada”.</p>
	</td>
</tr>
</table>

<form method="post" id="formulario">
<input type="hidden" name="togo" id="togo" value="/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_3_sintesedimensao3">
<input type="hidden" name="requisicao" value="diagnostico_3_2_tempodeaprendizagem">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%">Tempo de aprendizagem</td>
	<td>
	<?php $sql = "select * from pdeinterativo.pergunta where prgmodulo = 'E' and prgsubmodulo = 'T' and prgstatus = 'A' and prgdetalhe='Tempo de Aprendizagem'"; ?>
	<?php quadroPerguntas($sql) ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="10%">Jornada Ampliada</td>
	<td>
	<p><b>A escola desenvolve ações de Educação Integral?</b> <input type="radio" name="rtacaso" id="rtacaso_S" value="S" <?=(($dadosta['rtacaso']=="S")?"checked":"") ?> onclick="desenvolveEducacaoIntegral('S');"> Sim <input type="radio" name="rtacaso" id="rtacaso_N" value="N" <?=(($dadosta['rtacaso']=="N")?"checked":"") ?> onclick="desenvolveEducacaoIntegral('N');"> Não</p>
	<div id="educ_integral_S" <?=(($dadosta['rtacaso']=="S")?"":"style=\"display:none\"") ?> >
	<p><img src="../imagens/exclama.gif"> Qual a estratégia utilizada? <input type="checkbox" name="rtaestrategia[]" <?=((in_array("P",$rtaestrategia))?"checked":"") ?> id="estrategia_P" value="P"> Programa Mais Educação <input type="checkbox" id="estrategia_I" <?=((in_array("I",$rtaestrategia))?"checked":"") ?> name="rtaestrategia[]" value="I"> Iniciativa Estadual/Municipal <input type="checkbox" id="estrategia_O" <?=((in_array("O",$rtaestrategia))?"checked":"") ?> name="rtaestrategia[]" value="O"> Outras</p>
	<p><img src="../imagens/exclama.gif"> Quantos estudantes participam de atividade de educação integral nesta escola? 
	   Estudantes da própria escola: <input type="text" onkeyup="this.value=mascaraglobal('#####',this.value);" class="normal" size="6" value="<?=$rtaestudantepart[0] ?>" id="qtd_propria_escola" name="rtaestudantepart[]"> Estudantes de outras escolas: <input type="text" onkeyup="this.value=mascaraglobal('#####',this.value);" class="normal" size="6" value="<?=$rtaestudantepart[1] ?>" id="qtd_outra_escola" name="rtaestudantepart[]"></p>
	<p><img src="../imagens/exclama.gif"> Qual a carga horária das ações de educação integral, por semana? <input type="text" onkeyup="this.value=mascaraglobal('#####',this.value);" class="normal" size="6" value="<?=$dadosta['rtacargahoraria'] ?>" id="carga_horaria_semanal" name="rtacargahoraria"> horas por semana</p>
	<p><img src="../imagens/exclama.gif"> Assinale os macrocampos das atividades desenvolvidas:<br>
	<br>
	<table>
		<tr>
			<td><input type="checkbox" name="rtamacrocampo[]" id="mac_acompanhamentopedagogico" value="acompanhamentopedagogico" <?=((in_array("acompanhamentopedagogico",$rtamacrocampo))?"checked":"") ?> ></td>
			<td>Acompanhamento pedagógico</td>
		</tr>
		<tr>
			<td><input type="checkbox" name="rtamacrocampo[]" id="mac_educacaoambiental" value="educacaoambiental" <?=((in_array("educacaoambiental",$rtamacrocampo))?"checked":"") ?> ></td>
			<td>Educação ambiental</td>
		</tr>
		<tr>
			<td><input type="checkbox" name="rtamacrocampo[]" id="mac_esportelazer" value="esportelazer" <?=((in_array("esportelazer",$rtamacrocampo))?"checked":"") ?> ></td>
			<td>Esporte e lazer</td>
		</tr>
		
		<tr>
			<td><input type="checkbox" name="rtamacrocampo[]" id="mac_direitoshumanoseducacao" value="direitoshumanoseducacao" <?=((in_array("direitoshumanoseducacao",$rtamacrocampo))?"checked":"") ?> ></td>
			<td>Direitos humanos em educação</td>
		</tr>
		<tr>
			<td><input type="checkbox" name="rtamacrocampo[]" id="mac_culturaartes" value="culturaartes" <?=((in_array("culturaartes",$rtamacrocampo))?"checked":"") ?> ></td>
			<td>Cultura e artes</td>
		</tr>
		<tr>
			<td><input type="checkbox" name="rtamacrocampo[]" id="mac_culturadigital" value="culturadigital" <?=((in_array("culturadigital",$rtamacrocampo))?"checked":"") ?> ></td>
			<td>Cultura digital</td>
		</tr>
		<tr>
			<td><input type="checkbox" name="rtamacrocampo[]" id="mac_promocaosaude" value="promocaosaude" <?=((in_array("promocaosaude",$rtamacrocampo))?"checked":"") ?> ></td>
			<td>Promoção da saúde</td>
		</tr>
		<tr>
			<td><input type="checkbox" name="rtamacrocampo[]" id="mac_comunicacaousomidias" value="comunicacaousomidias" <?=((in_array("comunicacaousomidias",$rtamacrocampo))?"checked":"") ?> ></td>
			<td>Comunicação e uso de mídias</td>
		</tr>
		<tr>
			<td><input type="checkbox" name="rtamacrocampo[]" id="mac_educacaoeconomica" value="educacaoeconomica" <?=((in_array("educacaoeconomica",$rtamacrocampo))?"checked":"") ?> ></td>
			<td>Educação econômica</td>
		</tr>
	</table>
	</p>
	</div>
	
	<div id="educ_integral_N" <?=(($dadosta['rtacaso']=="N")?"":"style=\"display:none\"") ?> >
	<p><img src="../imagens/exclama.gif"> Por que?</p>
	<p>
	<table class="listagem" width="60%">
	<tr>
		<td class="SubTituloDireita" width="10%"><input type="checkbox" name="porque[]" id="porque_espacofisico" value="espacofisico" <?=((in_array("espacofisico",$rtaporque))?"checked":"") ?> ></td>
		<td>Não possui espaço físico</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="10%"><input type="checkbox" name="porque[]" id="porque_profissionais" value="profissionais" <?=((in_array("profissionais",$rtaporque))?"checked":"") ?> ></td>
		<td>Não dispoe de profissionais para coordenar as atividades</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="10%"><input type="checkbox" name="porque[]" id="porque_recursosmateriais" value="recursosmateriais" <?=((in_array("recursosmateriais",$rtaporque))?"checked":"") ?> ></td>
		<td>Não dispoe de recursos materiais</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="10%"><input type="checkbox" onclick="if(this.checked){document.getElementById('tr_rtaporqueoutro').style.display='';}else{document.getElementById('tr_rtaporqueoutro').style.display='none';document.getElementById('rtaporqueoutro').value='';}" name="porque[]" id="porque_outro" value="outro" <?=((in_array("outro",$rtaporque))?"checked":"") ?> ></td>
		<td>Outro</td>
	</tr>
	<tr id="tr_rtaporqueoutro" style="display:none">
		<td colspan="2">Qual?<br><? echo campo_textarea( 'rtaporqueoutro', 'S', 'S', '', '70', '4', '140'); ?></td>
	</tr>

	</table>
	</p>
	</div>
	</td>
</tr>

</table>
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
<tr>
	<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
	<td>
	<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_1_planejamentopedagogico';" >
	<input type="button" name="btn_continuar" value="Salvar" onclick="document.getElementById('togo').value='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_2_tempodeaprendizagem';diagnostico_3_1_planejamentopedagogico();">
	<input type="button" name="btn_continuar" value="Salvar e Continuar" onclick="document.getElementById('togo').value='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_3_sintesedimensao3';diagnostico_3_1_planejamentopedagogico();">
	<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_3_sintesedimensao3';" >
	</td>
</tr>
</table>
</form>