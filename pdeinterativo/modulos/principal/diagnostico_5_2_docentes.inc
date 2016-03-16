<?
monta_titulo( "Docentes - Comunidade Escolar", "&nbsp;");

?>
<script>
jQuery(function() {
	jQuery("[name^='rdovinculo[']").change(function() {
	  	var id = jQuery(this).attr("name");
		id = id.replace("rdovinculo[","");
 		id = id.replace("]","");
	  	desabilitaColuna(id,jQuery(this).val());
	});
});

function verificaRespostas(names) {
	var numPerg = jQuery("[name^='"+names+"']:enabled").length;
	var numPergresp = jQuery("[name^='"+names+"']:enabled:checked").length;
	
	if(numPerg > 0){
		numPerg = numPerg/2;
		if(numPerg != numPergresp){
			erro = 1;
		}else{
			erro = 0;
		}
	}else{
		erro = 0;
	}
	
	if(erro == 0){
		return true;
	}else{
		return false;
	}

}

function diagnostico_5_2_docentes_parcial() {
	document.getElementById('requisicao').value='diagnostico_5_2_docentes_parcial';
	document.getElementById('togo').value='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_2_docentes';
	divCarregando();
	document.getElementById('formulario').submit();
	
}

function diagnostico_5_2_docentes() {
	
	if(jQuery("[name^='rdovinculo['][value!='']").length != jQuery("[name^='rdovinculo[']").length) {
		alert('Favor preencher os vínculos');
		return false;
	}

	if(!verificaRespostas('rdodeseja[')){
		alert('Favor responder todas as opções referentes: Considera sua formação apropriada para ministrar esta disciplina?');
		return false;
	}
	
	if(!verificaRespostas('rdoformapro[')){
		alert('Favor responder todas as opções referentes: Deseja participar de algum curso de formação continuada nos próximos anos?');
		return false;
	}

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
	
	function desabilitaColuna(coddocente,valor)
	{
		if(valor == "D"){
			jQuery("[name^='rdodeseja[" + coddocente + "][']").attr("disabled","disabled");
			jQuery("[name^='rdodeseja[" + coddocente + "][']").attr("checked","");
			jQuery("[name^='rdoformapro[" + coddocente + "]']").attr("disabled","disabled");
			jQuery("[name^='rdoformapro[" + coddocente + "]']").attr("checked","");
		}else{
			jQuery("[name^='rdodeseja[" + coddocente + "][']").attr("disabled","");
			jQuery("[name^='rdoformapro[" + coddocente + "]']").attr("disabled","");
		}
	}
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%"><font class="blue">Orientações</font></td>
	<td class="blue">
	<p>Agora, vamos conhecer um pouco melhor o corpo docente da escola. São eles que, além de ministrar os conteúdos formais, ajudam a formar cidadãos e fazer da trajetória escolar um período rico de experiências. Mas para realizar bem o seu trabalho é importante que os docentes estejam preparados, sintam-se motivados e sejam valorizados pela comunidade escolar.</p>
	<p>Esta etapa é muito importante para a elaboração do Plano de Formação Continuada, pois a partir das respostas assinaladas nesta tela surgirão as demandas de formação da escola. Ao responder “Sim” à pergunta “Deseja participar de algum curso de formação continuada nos próximos anos?”, o sistema registrará o nome do docente no Plano Geral, a fim de que seja indicado o curso desejado.</p>
	<p>Em seguida, vamos analisar como e em que medida a escola apóia e estimula o corpo docente. Leia as perguntas e assinale em que medida o Grupo de Trabalho concorda com elas. Quando as respostas “sempre” e “na maioria das vezes” somarem mais de 50% em relação ao total de perguntas, é necessário que a escola apresente, no final da tela, justificativas ou evidências sobre esses aspectos, à luz dos resultados apresentados nas dimensões 1 e 2. Ou seja, se os docentes são suficientemente motivados e já adotam práticas pedagógicas apropriadas, porque os resultados da escola não são melhores?</p>
	</td>
</tr>
</table>

<form method="post" id="formulario">
<input type="hidden" name="togo" id="togo" value="/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_3_demaisprofissionais">
<input type="hidden" name="requisicao" id="requisicao" value="diagnostico_5_2_docentes">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%">Perfil</td>
	<td>
	<?
	$sql = "SELECT d.pk_cod_docente, d.no_docente, d.num_cpf, dd.no_disciplina, dd.pk_cod_disciplina, e.no_escolaridade, ee.id_tipo_docente FROM educacenso_2010.tab_docente d 
       		INNER JOIN educacenso_2010.tab_dado_docencia ee on d.pk_cod_docente = ee.fk_cod_docente
			LEFT JOIN educacenso_2010.tab_docente_disc_turma t ON d.pk_cod_docente = t.fk_cod_docente AND ee.fk_cod_entidade = t.fk_cod_entidade
			LEFT JOIN educacenso_2010.tab_escolaridade e ON e.pk_cod_escolaridade = d.fk_cod_escolaridade 
			LEFT JOIN educacenso_2010.tab_disciplina dd ON dd.pk_cod_disciplina = t.fk_cod_disciplina
			WHERE ee.fk_cod_entidade='".$_SESSION['pdeinterativo_vars']['pdicodinep']."' 
			GROUP BY d.pk_cod_docente, d.no_docente, d.num_cpf, dd.no_disciplina, dd.pk_cod_disciplina, e.no_escolaridade, ee.id_tipo_docente 
			ORDER BY no_docente, no_disciplina ";
	
	$arrDocentes = $db->carregar($sql);
	
	if($arrDocentes[0]) {
		foreach($arrDocentes as $doc) {
			$docentes[$doc['pk_cod_docente']]['no_docente'] 	 = $doc['no_docente'];
			$docentes[$doc['pk_cod_docente']]['num_cpf'] 	 	 = $doc['num_cpf'];
			$docentes[$doc['pk_cod_docente']]['no_escolaridade'] = $doc['no_escolaridade'];
			if($doc['pk_cod_disciplina']) $docentes[$doc['pk_cod_docente']]['no_disciplina'][$doc['pk_cod_disciplina']] = $doc['no_disciplina'];
		}
	}
	
	$sql = "SELECT * FROM pdeinterativo.respostadocente rd 
			LEFT JOIN pdeinterativo.respostadocenteformacao rf ON rf.rdoid = rd.rdoid 
			WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND rd.rdostatus='A'";
	
	$resp = $db->carregar($sql);
	if($resp[0]) {
		foreach($resp as $r) {
			$dadosDocentes[$r['pk_cod_docente']]['vinculo'] = $r['rdovinculo'];
			$dadosDocentes[$r['pk_cod_docente']]['rdodeseja'][$r['pk_cod_disciplina']] = $r['rdodeseja'];
			$dadosDocentes[$r['pk_cod_docente']]['rdoformapro'] = $r['rdoformapro'];
		}
	}
	
	?>
	<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro" colspan="7">Perfil dos Docentes</td>
		</tr>
		<tr>
			<td class="SubTituloCentro">Nome Docentes</td>
			<td class="SubTituloCentro">CPF</td>
			<td class="SubTituloCentro">Vínculo</td>
			<td class="SubTituloCentro">Escolaridade</td>
			<td class="SubTituloCentro">Áreas de conhecimento/Disciplina(s) que ministra nesta escola</td>
			<td class="SubTituloCentro" width="12%">Considera sua formação apropriada para ministrar esta disciplina?</td>
			<td class="SubTituloCentro" width="12%">Deseja participar de algum curso de formação continuada nos próximos anos?</td>
		</tr>	
	<?
	
	$dadosvinculo=array(0 => array("codigo" => "T", "descricao" => "Temporário"),
						1 => array("codigo" => "E", "descricao" => "Efetivo"),
						2 => array("codigo" => "D", "descricao" => "Desvinculado da escola"));
						
	if($docentes) {
		foreach($docentes as $coddocente => $arrDoc) {
			if(!$arrDoc['no_disciplina']) $arrDoc['no_disciplina'] = array();
			echo "<tr>";
			echo "<td rowspan=".count($arrDoc['no_disciplina']).">".$arrDoc['no_docente']."</td>";
			echo "<td rowspan=".count($arrDoc['no_disciplina']).">".$arrDoc['num_cpf']."</td>";
			echo "<td rowspan=".count($arrDoc['no_disciplina']).">".$db->monta_combo('rdovinculo['.$coddocente.']', $dadosvinculo, 'S', 'Selecione', '', '', '', '', 'N', '', true, $dadosDocentes[$coddocente]['vinculo'])."</td>";
			echo "<td rowspan=".count($arrDoc['no_disciplina']).">".$arrDoc['no_escolaridade']."</td>";
			echo "<td>".(($arrDoc['no_disciplina'])?current($arrDoc['no_disciplina']):"<center>-</center>")."</td>";
			echo "<td align=center>".((!$arrDoc['no_disciplina'])?"<b>-</b>":"<input type=radio ".(($dadosDocentes[$coddocente]['rdodeseja'][current(array_keys($arrDoc['no_disciplina']))]=="t")?"checked":"")." name=\"rdodeseja[".$coddocente."][".current(array_keys($arrDoc['no_disciplina']))."]\" ".($dadosDocentes[$coddocente]['vinculo'] == "D" ? "disabled=\"disabled\"" : "")." value=\"TRUE\"> Sim <input type=radio ".(($dadosDocentes[$coddocente]['rdodeseja'][current(array_keys($arrDoc['no_disciplina']))]=="f")?"checked":"")." name=\"rdodeseja[".$coddocente."][".current(array_keys($arrDoc['no_disciplina']))."]\" ".($dadosDocentes[$coddocente]['vinculo'] == "D" ? "disabled=\"disabled\"" : "")." value=\"FALSE\"> Não")."</td>";
			echo "<td rowspan=".count($arrDoc['no_disciplina'])." align=center><input type=radio ".(($dadosDocentes[$coddocente]['rdoformapro']=="t")?"checked":"")." name=rdoformapro[".$coddocente."] ".($dadosDocentes[$coddocente]['vinculo'] == "D" ? "disabled='disabled'" : "")." value=\"TRUE\"> Sim <input type=radio ".(($dadosDocentes[$coddocente]['rdoformapro']=="f")?"checked":"")." name=rdoformapro[".$coddocente."] ".($dadosDocentes[$coddocente]['vinculo'] == "D" ? "disabled='disabled'" : "")." value=\"FALSE\"> Não</td>";
			if($arrDoc['no_disciplina']) {
				$indice = key($arrDoc['no_disciplina']);
				unset($arrDoc['no_disciplina'][$indice]);
			}
			echo "</tr>";

			if($arrDoc['no_disciplina']) {
				foreach($arrDoc['no_disciplina'] as $coddisciplina => $discip) {
					echo "<tr>";
					echo "<td>".$discip."</td>";
					echo "<td align=center><input type=radio ".(($dadosDocentes[$coddocente]['rdodeseja'][$coddisciplina]=="t")?"checked":"")." name=\"rdodeseja[".$coddocente."][".$coddisciplina."]\" ".($dadosDocentes[$coddocente]['vinculo'] == "D" ? "disabled=\"disabled\"" : "")." value=\"TRUE\"> Sim <input type=radio ".(($dadosDocentes[$coddocente]['rdodeseja'][$coddisciplina]=="f")?"checked":"")." name=\"rdodeseja[".$coddocente."][".$coddisciplina."]\" ".($dadosDocentes[$coddocente]['vinculo'] == "D" ? "disabled=\"disabled\"" : "")." value=\"FALSE\"> Não</td>";
					echo "</tr>";
				}
			}
		}
	}
	?>
	<tr>
	<td colspan="7" class="SubTituloEsquerda">Total: <?=count($docentes) ?> docentes</td>
	</tr>
	<tr>
	<td colspan="7" class="SubTituloCentro"><input type="button" name="gravardocentes" value="Gravar dados docentes" onclick="diagnostico_5_2_docentes_parcial();"> <font size=1>* A gravação parcial não confirma a gravação da TELA</font></td>
	</tr>
	</table>

	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="10%">Práticas</td>
	<td>
	<?php $sql = "select * from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'D' and prgstatus = 'A' and prgdetalhe='Práticas' order by prgid"; ?>
	<?php quadroPerguntas($sql) ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="10%">Experiência e Auto-confiança</td>
	<td>
	<?php $sql = "select * from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'D' and prgstatus = 'A' and prgdetalhe='Experiencia e Auto-Confiança' order by prgid"; ?>
	<?php quadroPerguntas($sql) ?>
	</td>
</tr>
<? $juedescricao = $db->pegaUm("select juedescricao from pdeinterativo.justificativaevidencias where abacod='diagnostico_5_2_docentes' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'"); ?>
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
	<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_1_estudantes';" >
	<input type="button" name="btn_continuar" value="Salvar" onclick="document.getElementById('togo').value='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_2_docentes';diagnostico_5_2_docentes();">
	<input type="button" name="btn_continuar" value="Salvar e Continuar" onclick="document.getElementById('togo').value='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_3_demaisprofissionais';diagnostico_5_2_docentes();">
	<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_3_demaisprofissionais';" >
	</td>
</tr>
</table>
</form>