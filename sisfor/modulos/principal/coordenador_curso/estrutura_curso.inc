<?php

if(!$_SESSION['sisfor']['sifid'])
	die("<script>
			alert('Problemas na navegação. Você esta sendo redirecionado apra página principal. Tente novamente!');
			window.location='sisfor.php?modulo=inicio&acao=C';
		 </script>");


$estado = wf_pegarEstadoAtual( $_SESSION['sisfor']['docidprojeto'] );

if(($estado['esdid'] != ESD_PROJETO_EMCADASTRAMENTO && $estado['esdid'] != ESD_PROJETO_AJUSTES) || in_array(PFL_CONSULTAGERAL,$perfil)) {
	$consulta = true;
} ?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

<? if($consulta) : ?>
jQuery(function() {
jQuery("[id='cadastrarabrangencia']").remove();
jQuery("[type='text']").attr('class','disabled');
jQuery("[type='text']").attr('disabled','disabled');
jQuery("[type='radio']").attr('disabled','disabled');
jQuery("[name='estuf_endereco'],[name='ecuobsplanoatividades'],[name='muncod_endereco'],[name='ainseducjustificativa'],[name='ainundimejustificativa'],[name='ainuncmejustificativa']").attr('disabled','disabled');
});
<? endif; ?>

function salvarEstruturaCurso(goto) {

	var validado=true;
	
	jQuery("[name^='atidatainicio[']").each(function(key, obj) {
		if(obj.value=='') {
			alert('PERÍODO DE EXECUÇÃO - Data Início em branco');
			validado = false;
			return false;
		}
		
		if(!validaData(obj)) {
			alert('PERÍODO DE EXECUÇÃO - Data Início inválida');
			validado = false;
			return false;
		}

		
	});
	
	if(!validado) {
		return false;
	}
	
	jQuery("[name^='atidatafim[']").each(function(key, obj) {
		if(obj.value=='') {
			alert('PERÍODO DE EXECUÇÃO - Data Término em branco');
			validado = false;
			return false;
		}
	
		if(!validaData(obj)) {
			alert('PERÍODO DE EXECUÇÃO - Data Término inválida');
			validado = false;
			return false;
		}
		
	
	});
	
	if(!validado) {
		return false;
	}

	var sifforumestadualpermanente_r = jQuery("[name^='sifforumestadualpermanente']:enabled:checked").length;
	if(sifforumestadualpermanente_r==0) {
		alert('Marque se o Fórum Estadual Permanente conhece este projeto');
		return false;
	}
	
	if(jQuery('#sifforumestadualpermanentejustificativa').val()=='') {
		alert('Preencha a justificativa do Fórum Estadual Permanente');
		return false;
	}
	
	var sifseduc_r = jQuery("[name^='sifseduc']:enabled:checked").length;
	if(sifseduc_r==0) {
		alert('Marque se foi feita articulação com a SEDUC');
		return false;
	}
	
	if(jQuery('#sifseducjustificativa').val()=='') {
		alert('Preencha a justificativa da SEDUC');
		return false;
	}
	
	var sifundime_r = jQuery("[name^='sifundime']:enabled:checked").length;
	if(sifundime_r==0) {
		alert('Marque se foi feita articulação com a UNDIME');
		return false;
	}
	
	if(jQuery('#sifundimejustificativa').val()=='') {
		alert('Preencha a justificativa da UNDIME');
		return false;
	}

	var sifuncme_r = jQuery("[name^='sifuncme']:enabled:checked").length;
	if(sifuncme_r==0) {
		alert('Marque se foi feita articulação com a UNCME');
		return false;
	}
	
	if(jQuery('#sifuncmejustificativa').val()=='') {
		alert('Preencha a justificativa da UNCME');
		return false;
	}

	var sifmsoc_r = jQuery("[name^='sifmsoc']:enabled:checked").length;
	if(sifmsoc_r==0) {
		alert('Marque se foi feita articulação com movimentos sociais e outras organizações da sociedade civil');
		return false;
	}
	
	if(jQuery('#sifoutrasarticulacoes').val()=='') {
		alert('Preencha se foi feita articulação com movimentos sociais e outras organizações da sociedade civil');
		return false;
	}
	
    divCarregando();
    
    jQuery('#goto').val(goto);

	document.getElementById('formulario').submit();

}

function carregarSubAtividades(atiid) {
	ajaxatualizar('requisicao=carregarSubAtividades&atiid='+atiid,'td_subatividade');
}

function definirAbrangencia() {
	ajaxatualizar('requisicao=listarPolosCurso&sifid=<?=$_SESSION['sisfor']['sifid'] ?>','haverapolos_true');
	ajaxatualizar('requisicao=definirAbrangencia&sifid=<?=$_SESSION['sisfor']['sifid'] ?>','div_abrangencia');
}

function abrirMunicipioAbrangencia() {
	window.open('sisfor.php?modulo=principal/coordenador_curso/inserirmunicipioabrangencia&acao=A&sifid=<?=$_SESSION['sisfor']['sifid'] ?>','Documento','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}

function inserirPolos(polid) {
	var l = '';
	if(polid!='') {
		l = '&polid='+polid;
	}
	window.open('sisfor.php?modulo=principal/coordenador_curso/inserirpolocurso&acao=A&sifid=<?=$_SESSION['sisfor']['sifid'] ?>'+l,'Documento','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}

function excluirPolo(polid) {
	
	var conf = confirm('Deseja remover o polo?');
	
	if(conf) {
		ajaxatualizar('requisicao=removerPolo&polid='+polid,'');
	}


	window.location = window.location;
}

function abrirDetalhamentoAbrangencia(muncod, esfera, obj) {

	var tabela = obj.parentNode.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode.parentNode;
	
	if(obj.title=="mais") {
		obj.title    = "menos";
		obj.src      = "../imagens/menos.gif";
		var nlinha   = tabela.insertRow(linha.rowIndex);
		var ncol     = nlinha.insertCell(0);
		ncol.colSpan = 8;
		ncol.id      = 'coluna_'+nlinha.rowIndex;
		ajaxatualizar('requisicao=carregarDetalhamentoAbrangencia&ecuid=<?=$_SESSION['sispacto']['universidade']['ecuid'] ?>&muncod='+muncod+'&esfera='+esfera,'coluna_'+nlinha.rowIndex);
	} else {
		obj.title    = "mais";
		obj.src      = "../imagens/mais.gif";
		tabela.deleteRow(linha.rowIndex);
	}
	 
}

function excluirAbrangencia(abrid) {
	var conf = confirm("Deseja realmente excluir este Município?");
	if(conf) {
		divCarregando();
		ajaxatualizar('requisicao=excluirAbrangencia&abrid='+abrid,'');
		ajaxatualizar('requisicao=definirAbrangencia&sifid=<?=$_SESSION['sisfor']['sifid'] ?>','div_abrangencia');
		divCarregado();
	}
}


function haveraPolos(obj) {

	if(obj.value=='false') {
		var conf = confirm('Você selecionou que o curso não possui Polo, caso tenha algum polo cadastrado, este será excluído. Confirma?');

		if(!conf) {
			jQuery("[name^=haverapolos][value^=true]").attr('checked','checked');
			return false;
		}
	}

	if(obj.value=='true') {
		var conf = confirm('Você selecionou que o curso possui Polo, caso tenha algum município de abrangência cadastrado (na opção sem polos), estes serão removidos. Confirma?');

		if(!conf) {
			jQuery("[name^=haverapolos][value^=false]").attr('checked','checked');
			return false;
		}
	}
	
	

	ajaxatualizar('requisicao=harevaPolo&sifid=<?=$_SESSION['sisfor']['sifid'] ?>&sifpossuipolo='+obj.value,'');

	definirAbrangencia();

	if(obj.value=='true') {
		ajaxatualizar('requisicao=removerMunicipiosAbrangencia&sifid=<?=$_SESSION['sisfor']['sifid'] ?>','');
		jQuery('#haverapolos_true').css('display','');
		jQuery('#haverapolos_false').css('display','none');
	}

	if(obj.value=='false') {
		ajaxatualizar('requisicao=removerPolo&sifid=<?=$_SESSION['sisfor']['sifid'] ?>','');
		jQuery('#haverapolos_true').css('display','none');
		jQuery('#haverapolos_false').css('display','');
	}
	
}

function definirSede(obj) {

	ajaxatualizar('requisicao=definirSede&muncod='+obj.value+'&polo='+obj.name,'');

	alert('Sede definida com sucesso');
	
}

function adicionarModeloPlanoAtividade(id, atiidpai) {

	if(jQuery('#'+id).val()=='') {
		alert('Selecione um modelo para adicionar');
		return false;
	}

	ajaxatualizar('requisicao=inserirModeloPlanoAtividade&sifid=<?=$_SESSION['sisfor']['sifid'] ?>&pamid='+jQuery('#'+id).val()+'&atiidpai='+atiidpai,'');
	ajaxatualizar('requisicao=carregarPlanoAtividades&sifid=<?=$_SESSION['sisfor']['sifid'] ?>','dv_planoatividades');
	
}

function excluirPlanoAtividades(atiid) {
	
	var conf = confirm('Deseja realmente excluir? As subatividades serão excluídas também');
	
	if(conf) {
		ajaxatualizar('requisicao=excluirPlanoAtividades&atiid='+atiid,'');
	}
	
	ajaxatualizar('requisicao=carregarPlanoAtividades&sifid=<?=$_SESSION['sisfor']['sifid'] ?>','dv_planoatividades');
}

</script>
<form method="post" id="formulario">
<input type="hidden" name="requisicao" value="atualizarEstruturaCurso">
<input type="hidden" name="goto" id="goto" value="">

<?php 

$estruturacurso = $db->pegaLinha("SELECT * FROM sisfor.sisfor WHERE sifid='".$_SESSION['sisfor']['sifid']."'");
if($estruturacurso) extract($estruturacurso);

?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Estrutura do Curso</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td>Orientação</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Abrangência</td>
		<td>
		<p><b>Haverá polos? <input type="radio" name="haverapolos" value="true" onclick="haveraPolos(this);" <?=(($sifpossuipolo=='t')?"checked":"") ?>> Sim <input type="radio" name="haverapolos" value="false" onclick="haveraPolos(this);" <?=(($sifpossuipolo=='f')?"checked":"") ?>> Não</b></p>
		
		<div id="haverapolos_true" <?=(($sifpossuipolo=='t')?'':'style="display:none;"') ?>>
		<?
		
		listarPolosCurso(array('sifid' => $_SESSION['sisfor']['sifid'], 'consulta' => $consulta));
		
		?>
		</div>
		
		<div id="haverapolos_false" <?=(($estruturacurso['sifpossuipolo']=='f')?'':'style="display:none;"') ?>>
		
		<p><input type="button" name="cadastrarabrangencia" id="cadastrarabrangencia" value="Inserir Município" onclick="abrirMunicipioAbrangencia();"></p>
		<div id="div_abrangencia">
		<? definirAbrangencia(array("sifid"=>$_SESSION['sisfor']['sifid'])); ?>
		</div>
		
		</div>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Plano de Atividades</td>
		<td>
		<div id="dv_planoatividades">
		<? carregarPlanoAtividades(array("sifid" => $_SESSION['sisfor']['sifid'], 'consulta' => $consulta)); ?>
		</div>
		<br/>
		<p>Comentários sobre o cronograma (plano de atividades):</p>
		<?=campo_textarea('sifobsplanoatividades', 'N', (($consulta)?'N':'S'), '', '100', '12', '5000'); ?>
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Articulação Institucional</td>
		<td>
			<table>
				<tr>
					<td>O Fórum Estadual Permanente conhece este projeto?</td>
					<td><input type="radio" name="sifforumestadualpermanente" value="TRUE" <?=(($sifforumestadualpermanente=="t")?"checked":"") ?> <?=(($consulta)?'disabled':'')?>> Sim <input type="radio" name="sifforumestadualpermanente" value="FALSE" <?=(($sifforumestadualpermanente=="f")?"checked":"") ?> <?=(($consulta)?'disabled':'')?>> Não</td>
				</tr>
				<tr>
					<td colspan="2">Comente<br><?=campo_textarea('sifforumestadualpermanentejustificativa', 'N', (($consulta)?'N':'S'), '', '70', '4', '250'); ?></td>
				</tr>
				<tr>
					<td>Foi feita articulação com a SEDUC / Secretaria Estadual de Educação?</td>
					<td><input type="radio" name="sifseduc" value="TRUE" <?=(($sifseduc=="t")?"checked":"") ?> <?=(($consulta)?'disabled':'')?>> Sim <input type="radio" name="sifseduc" value="FALSE" <?=(($sifseduc=="f")?"checked":"") ?> <?=(($consulta)?'disabled':'')?>> Não</td>
				</tr>
				<tr>
					<td colspan="2">Comente<br><?=campo_textarea('sifseducjustificativa', 'N', (($consulta)?'N':'S'), '', '70', '4', '250'); ?></td>
				</tr>
				<tr>
					<td>Foi feita articulação com a UNDIME / Secretaria Municipal de Educação?</td>
					<td><input type="radio" name="sifundime" value="TRUE" <?=(($sifundime=="t")?"checked":"") ?> <?=(($consulta)?'disabled':'')?>> Sim <input type="radio" name="sifundime" value="FALSE" <?=(($sifundime=="f")?"checked":"") ?> <?=(($consulta)?'disabled':'')?>> Não</td>
				</tr>
				<tr>
					<td colspan="2">Comente<br><?=campo_textarea('sifundimejustificativa', 'N', (($consulta)?'N':'S'), '', '70', '4', '250'); ?></td>
				</tr>
				<tr>
					<td>Foi feita articulação com a UNCME?</td>
					<td><input type="radio" name="sifuncme" value="TRUE" <?=(($sifuncme=="t")?"checked":"") ?> <?=(($consulta)?'disabled':'')?>> Sim <input type="radio" name="sifuncme" value="FALSE" <?=(($sifuncme=="f")?"checked":"") ?> <?=(($consulta)?'disabled':'')?>> Não</td>
				</tr>
				<tr>
					<td colspan="2">Comente<br><?=campo_textarea('sifuncmejustificativa', 'N', (($consulta)?'N':'S'), '', '70', '4', '250'); ?></td>
				</tr>
				<tr>
					<td>Foi feita articulação com movimentos sociais e outras organizações da sociedade civil?</td>
					<td><input type="radio" name="sifmsoc" value="TRUE" <?=(($sifmsoc=="t")?"checked":"") ?> <?=(($consulta)?'disabled':'')?>> Sim <input type="radio" name="sifmsoc" value="FALSE" <?=(($sifmsoc=="f")?"checked":"") ?> <?=(($consulta)?'disabled':'')?>> Não</td>
				</tr>
				<tr>
					<td colspan="2">Comente<br><?=campo_textarea('sifoutrasarticulacoes', 'N', (($consulta)?'N':'S'), '', '70', '4', '250'); ?></td>
				</tr>
				
				<tr>
					<td>Outras articulações</td>
					<td></td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
		<? if(!$consulta) : ?> 
		<input type="button" name="salvar" value="Salvar" onclick="salvarEstruturaCurso('sisfor.php?modulo=principal/universidade/universidade&acao=A&aba=estrutura_curso');"> 
		<? endif; ?>
		</td>
	</tr>
</table>
</form>