<script>
function gerenciarAcompanhamento(iusidprofessor,tipoacompanhamento) {

	if('<?=$_REQUEST['fpbid'] ?>'=='') {
		alert('Nenhum período de referência encontrado');
		return false;
	}
	
	jQuery("form[name=formulario_"+tipoacompanhamento+"] input[id=iusidprofessor]").val(iusidprofessor);
	
	jQuery.ajax({
   		type: "POST",
   		url: 'escolaterra.php?modulo=principal/tutor/tutorexecucao&acao=A&aba=acompanharprofessores',
   		data: 'requisicao=carregarAcompanhamentoProfessores&fpbid=<?=$_REQUEST['fpbid'] ?>&iusidprofessor='+iusidprofessor,
   		async: false,
   		success: function(json){
   			if(json) {
   			
   				var aco = jQuery.parseJSON(json);
   				
   				if(aco.acoexistetempouniversidade=='t') {
   					jQuery("[name^='acoexistetempouniversidade'][value^='TRUE']").attr('checked',true);
   					existeTempoUniversidade('TRUE');
   				}
   				if(aco.acoexistetempouniversidade=='f') {
   					jQuery("[name^='acoexistetempouniversidade'][value^='FALSE']").attr('checked',true);
   					existeTempoUniversidade('FALSE');
   				}
   				
   				if(aco.acorecebeuformacao=='t') {
   					jQuery("[name^='acorecebeuformacao'][value^='TRUE']").attr('checked',true);
   					recebeuFormacao('TRUE');
   				}
   				if(aco.acorecebeuformacao=='f') {
   					jQuery("[name^='acorecebeuformacao'][value^='FALSE']").attr('checked',true);
   					recebeuFormacao('FALSE');
   				}
				jQuery("#td_professoravaliado1").html(aco.iusnome);   				
				jQuery("#td_professoravaliado2").html(aco.iusnome);
				jQuery("#acorecebeuformacaojustificativa").val(aco.acorecebeuformacaojustificativa);
				jQuery("#acocargahorariauniversidade").val(aco.acocargahorariauniversidade);
				jQuery("#acoconteudosdesenvolvidosformacao").val(aco.acoconteudosdesenvolvidosformacao);
				jQuery("#acoconteudosdesenvolvidosformacao").keyup();
				
				jQuery("[name^='acotrabalhandoefetivamente'][value^='"+aco.acotrabalhandoefetivamente+"']").attr('checked',true);
				jQuery("[name^='acoevolucaoaprendizagem'][value^='"+aco.acoevolucaoaprendizagem+"']").attr('checked',true);
				jQuery("[name^='acousodosmateriais'][value^='"+aco.acousodosmateriais+"']").attr('checked',true);

				jQuery("#acotrabalhandoefetivamenteobservacao").val(aco.acotrabalhandoefetivamenteobservacao);
				jQuery("#acoconteudosdesenvolvidosformacao").keyup();
				
				jQuery("#acoevolucaoaprendizagemobservacao").val(aco.acoevolucaoaprendizagemobservacao);
				jQuery("#acoevolucaoaprendizagemobservacao").keyup();
				
				jQuery("#acousodosmateriaisobservacao").val(aco.acousodosmateriaisobservacao);
				jQuery("#acousodosmateriaisobservacao").keyup();
				
   				
   			}
   		}
	});
	

	jQuery("#modalFormulario_"+tipoacompanhamento).dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });


}

function existeTempoUniversidade(value) {
	if(value=='TRUE') {
		document.getElementById('td_acoexistetempouniversidade').style.display = '';
	} else if(value=='FALSE') {
		document.getElementById('td_acoexistetempouniversidade').style.display = 'none';
		jQuery("[name^='acorecebeuformacao']").attr('checked',false);
		jQuery("#tr_acorecebeuformacaojustificativa").css('display','none');
		jQuery("#acorecebeuformacaojustificativa").val('');
		jQuery("#acocargahorariauniversidade").val('');
		jQuery("#acoconteudosdesenvolvidosformacao").val('');
	}
}

function recebeuFormacao(value) {
	if(value=='TRUE') {
		document.getElementById('tr_acorecebeuformacaojustificativa').style.display = 'none';
		jQuery("#acorecebeuformacaojustificativa").val('');
	} else if(value=='FALSE') {
		document.getElementById('tr_acorecebeuformacaojustificativa').style.display = '';
	}
}

function enviarTempoUniversidade() {

	var chk_acoexistetempouniversidade = parseInt(jQuery("[name^='acoexistetempouniversidade']:enabled:checked").length);
	
	if(chk_acoexistetempouniversidade==0) {
		alert('Informe se neste período ocorreu atividades na Universidade');
		return false;
	}

	if(jQuery('#acoexistetempouniversidade_TRUE').attr('checked')) {
	
		var chk_acorecebeuformacao = parseInt(jQuery("[name^='acorecebeuformacao']:enabled:checked").length);
		
		if(chk_acorecebeuformacao==0) {
			alert('Informe se o professor recebeu formação');
			return false;
		}
		
		if(jQuery('#acorecebeuformacao_FALSE').attr('checked')) {
		
			if(jQuery('#acorecebeuformacaojustificativa').val()=='') {
				alert('Informe uma justificativa');
				return false;
			}
			
		}
		
		if(jQuery('#acocargahorariauniversidade').val()=='') {
			alert('Informe a Carga Horária Trabalhada na Universidade');
			return false;
		}
		
		if(jQuery('#acoconteudosdesenvolvidosformacao').val()=='') {
			alert('Informe os Conteúdos desenvolvidos na Formação');
			return false;
		}
	
	}
	
	jQuery('#formulario_acompanharTempoUniversidade').submit();

}

function enviarTempoEscolaComunidade() {

	var chk_acotrabalhandoefetivamente = parseInt(jQuery("[name^='acotrabalhandoefetivamente']:enabled:checked").length);
	
	if(chk_acotrabalhandoefetivamente==0) {
		alert('Informe se o professor trabalhou efetivamente com as sugestões e orientações recebidas na formação');
		return false;
	}
	
	if(jQuery('#acotrabalhandoefetivamenteobservacao').val()=='') {
		alert('Preencha as observações');
		return false;	
	}
	
	var chk_acoevolucaoaprendizagem = parseInt(jQuery("[name^='acoevolucaoaprendizagem']:enabled:checked").length);
	
	if(chk_acoevolucaoaprendizagem==0) {
		alert('Informe se evolução da aprendizagem dos estudantes');
		return false;
	}
	
	if(jQuery('#acoevolucaoaprendizagemobservacao').val()=='') {
		alert('Preencha as observações');
		return false;	
	}
	
	var chk_acousodosmateriais = parseInt(jQuery("[name^='acousodosmateriais']:enabled:checked").length);
	
	if(chk_acousodosmateriais==0) {
		alert('Informe se uso dos materiais (Kits)');
		return false;
	}
	
	if(jQuery('#acousodosmateriaisobservacao').val()=='') {
		alert('Preencha as observações');
		return false;	
	}

	jQuery('#formulario_acompanharTempoEscolaComunidade').submit();

}

</script>


<div id="modalFormulario_acompanharTempoEscolaComunidade" style="display:none;">
<form method="post" name="formulario_acompanharTempoEscolaComunidade" id="formulario_acompanharTempoEscolaComunidade">
<input type="hidden" name="requisicao" value="gravarAcompanhamentoProfessor">
<input type="hidden" name="fpbid" value="<?=$_REQUEST['fpbid'] ?>">
<input type="hidden" name="iusidprofessor" id="iusidprofessor" value="">
	
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
<td class="SubTituloCentro" colspan="2" style="font-size:large;">Tempo Escola Comunidade</td>
</tr>
<tr>
<td class="SubTituloCentro" colspan="2">
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloDireita">Professor:</td>
	<td id="td_professoravaliado1"></td>
</tr>
</table>
</td>
</tr>
<tr>
<td class="SubTituloDireita" width="40%">O professor trabalhou adequadamente com as sugestões e orientações recebidas na formação?</td>
<td><input type="radio" name="acotrabalhandoefetivamente" value="S"> Sim <input type="radio" name="acotrabalhandoefetivamente" value="N"> Não <input type="radio" name="acotrabalhandoefetivamente" value="E"> Em parte</td>
</tr>

<tr>
<td class="SubTituloDireita">Observações:</td>
<td>
<? echo campo_textarea( 'acotrabalhandoefetivamenteobservacao', 'S', 'S', '', '70', '4', '1000'); ?>
</td>
</tr>


<tr>
<td class="SubTituloDireita">Uso dos materiais (Kits)?</td>
<td><input type="radio" name="acousodosmateriais" value="S"> Sim <input type="radio" name="acousodosmateriais" value="N"> Não <input type="radio" name="acousodosmateriais" value="E"> Em parte</td>
</tr>

<tr>
<td class="SubTituloDireita">Observações:</td>
<td>
<? echo campo_textarea( 'acousodosmateriaisobservacao', 'S', 'S', '', '70', '4', '1000'); ?>
</td>
</tr>

<tr>
<td class="SubTituloDireita">Há perspectiva de evolução da aprendizagem dos estudantes?</td>
<td><input type="radio" name="acoevolucaoaprendizagem" value="S"> Sim <input type="radio" name="acoevolucaoaprendizagem" value="N"> Não <input type="radio" name="acoevolucaoaprendizagem" value="E"> Em parte</td>
</tr>

<tr>
<td class="SubTituloDireita">Observações:</td>
<td>
<? echo campo_textarea( 'acoevolucaoaprendizagemobservacao', 'S', 'S', '', '70', '4', '1000'); ?>
</td>
</tr>

<tr>
<td class="SubTituloCentro" colspan="2">
<input type="button" name="salvar" value="Salvar" onclick="enviarTempoEscolaComunidade();">
</td>
</tr>
</table>
</form>

</div>


<div id="modalFormulario_acompanharTempoUniversidade" style="display:none;">
<form method="post" name="formulario_acompanharTempoUniversidade" id="formulario_acompanharTempoUniversidade">
<input type="hidden" name="requisicao" value="gravarAcompanhamentoProfessor">
<input type="hidden" name="fpbid" value="<?=$_REQUEST['fpbid'] ?>">
<input type="hidden" name="iusidprofessor" id="iusidprofessor" value="">
	
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
<td class="SubTituloCentro" colspan="2" style="font-size:large;">Tempo Universidade</td>
</tr>
<tr>
<td class="SubTituloCentro" colspan="2">
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloDireita">Professor:</td>
	<td id="td_professoravaliado2"></td>
</tr>
</table>
</td>
</tr>
<tr>
<td class="SubTituloDireita">Neste período ocorreu atividades na Universidade?</td>
<td><input type="radio" name="acoexistetempouniversidade" id="acoexistetempouniversidade_TRUE" value="TRUE" onclick="existeTempoUniversidade(this.value);"> Sim <input type="radio" name="acoexistetempouniversidade" id="acoexistetempouniversidade_FALSE" value="FALSE" onclick="existeTempoUniversidade(this.value);"> Não</td>
</tr>
	
<tr>
<td colspan="2" style="display:none;" id="td_acoexistetempouniversidade">
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	
<tr>
<td class="SubTituloDireita">Esse Professor recebeu formação do Escola da Terra na Universidade?</td>
<td><input type="radio" name="acorecebeuformacao" value="TRUE" onclick="recebeuFormacao(this.value);"> Sim <input type="radio" name="acorecebeuformacao" id="acorecebeuformacao_FALSE" value="FALSE" onclick="recebeuFormacao(this.value);"> Não</td>
</tr>
	
<tr id="tr_acorecebeuformacaojustificativa" style="display:none;">
<td class="SubTituloDireita">Justifique:</td>
<td>
<? echo campo_textarea( 'acorecebeuformacaojustificativa', 'S', 'S', '', '70', '4', '700'); ?>
</td>
</tr>
	
	
<tr>
<td class="SubTituloDireita">Carga Horária Trabalhada na Universidade:</td>
<td>
<? echo campo_texto('acocargahorariauniversidade', "S", "S", "Carga Horária Trabalhada na Universidade", 10, 10, "#########", "", '', '', 0, 'id="acocargahorariauniversidade"', ''); ?>
</td>
</tr>
	
<tr>
<td class="SubTituloDireita">Conteúdos desenvolvidos na Formação:</td>
<td>
<? echo campo_textarea( 'acoconteudosdesenvolvidosformacao', 'S', 'S', '', '70', '4', '1000'); ?>
</td>
</tr>
	
</table>
</td>
</tr>

<tr>
<td class="SubTituloCentro" colspan="2">
<input type="button" name="salvar" value="Salvar" onclick="enviarTempoUniversidade();">
</td>
</tr>
</table>
</form>

</div>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Acompanhar Professores</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao("/escolaterra/escolaterra.php?modulo=principal/tutor/tutorexecucao&acao=A&aba=acompanharprofessores"); ?></td>
</tr>
<tr>
	<td colspan="2" class="SubTituloCentro">
	<?
	carregarPeriodoReferencia(array('ufpid'=>$_SESSION['escolaterra']['tutor']['ufpid'],'fpbid'=>$_REQUEST['fpbid']));
	?>
	</td>
</tr>
<? if($_REQUEST['fpbid']) : ?>
<tr>
	<td colspan="2">
	<?
	
	$turma = carregarDadosTurma(array('perfil' => 'tutor'));
	
	$sql = "SELECT replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') as cpf,
				   i.iusnome as nome,
				   i.iusemailprincipal as email,
				   CASE WHEN a.acoexistetempouniversidade IS NULL THEN '<center><span style=\"cursor:pointer;font-variant:small-caps;\" onclick=\"gerenciarAcompanhamento('||i.iusid||',\'acompanharTempoUniversidade\');\"><img src=../imagens/valida2.gif align=\"absmiddle\"> clique aqui para acompanhar</span></center>'
				   		WHEN a.acoexistetempouniversidade=TRUE THEN '<center><span style=\"cursor:pointer;font-variant:small-caps;\" onclick=\"gerenciarAcompanhamento('||i.iusid||',\'acompanharTempoUniversidade\');\"><img src=../imagens/valida1.gif align=\"absmiddle\"> clique aqui para acompanhar</span></center>'
				   		WHEN a.acoexistetempouniversidade=FALSE THEN '<center><span style=\"cursor:pointer;font-variant:small-caps;\" onclick=\"gerenciarAcompanhamento('||i.iusid||',\'acompanharTempoUniversidade\');\"><img src=../imagens/valida6.gif align=\"absmiddle\"> clique aqui para acompanhar</span></center>'
				   END as tempouniversidade,
				   CASE WHEN a.acotrabalhandoefetivamente IS NULL THEN '<center><span style=\"cursor:pointer;font-variant:small-caps;\" onclick=\"gerenciarAcompanhamento('||i.iusid||',\'acompanharTempoEscolaComunidade\');\"><img src=../imagens/valida2.gif align=\"absmiddle\"> clique aqui para acompanhar</span></center>'
				   		ELSE '<center><span style=\"cursor:pointer;font-variant:small-caps;\" onclick=\"gerenciarAcompanhamento('||i.iusid||',\'acompanharTempoEscolaComunidade\');\"><img src=../imagens/valida1.gif align=\"absmiddle\"> clique aqui para acompanhar</span></center>'
				   END as tempoescolacomunidade
			FROM escolaterra.identificacaousuario i 
			INNER JOIN escolaterra.tipoperfil t ON t.iusid = i.iusid AND t.pflcod=".PFL_PROFESSOR." 
			INNER JOIN escolaterra.turmaidusuario u ON u.iusid = i.iusid 
			LEFT JOIN escolaterra.acompanhamentoprofessores a ON a.iusidprofessor = i.iusid AND a.fpbid='".$_REQUEST['fpbid']."'
			WHERE u.turid='".$turma['turid']."'";
	
	$cabecalho = array("CPF","Nome","Email","Tempo Universidade","Tempo Escola Comunidade");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);
	

	?>
	</td>
</tr>
<? endif; ?>
<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td><input type="button" value="Próximo" onclick="divCarregando();window.location='<?=$goto_pro ?>';"></td>
</tr>
</table>