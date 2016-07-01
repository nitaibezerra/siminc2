<?
ini_set("memory_limit", "2048M");
if(!$consulta) $consulta = verificaPermissao();
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<script>

jQuery(document).ready(function() {
  jQuery(window).keydown(function(event){
    if(event.keyCode == 13) {
      event.preventDefault();
      return false;
    }
  });
});

function salvarDados(goto) {
		  
	jQuery('#iuscpf').val(mascaraglobal('###.###.###-##',jQuery('#iuscpf').val()));
	
	if(jQuery('#iuscpf').val()=='') {
		alert('CPF em branco');
		return false;
	}
	
	if(!validar_cpf(jQuery('#iuscpf').val())) {
		alert('CPF inválido');
		return false;
	}
	
	if(jQuery('#iusnome').val()=='') {
		alert('Nome em branco');
		return false;
	}
	
	if(jQuery('#iusdatanascimento').val()=='') {
		alert('Data de Nascimento em branco');
		return false;
	}
	
	if(!validaData(document.getElementById('iusdatanascimento'))) {
		alert('Data de Nascimento inválida');
		return false;
	}
	
	var iussexo_r = jQuery("[name^='iussexo']:enabled:checked").length;
	
	if(iussexo_r==0) {
		alert('Sexo em branco');
		return false;
	}
	
	if(jQuery('#iusnomemae').val()=='') {
		alert('Nome da mãe em branco');
		return false;
	}

	if(jQuery('#estuf_nascimento').val()=='') {
		alert('Estado - Local Nascimento em branco');
		return false;
	}
	
	if(document.getElementById('muncod_nascimento')) {
		if(jQuery('#muncod_nascimento').val()=='') {
			alert('Município - Local Nascimento em branco');
			return false;
		}
	} else {
		alert('Município - Local Nascimento em branco');
		return false;
	}
	
	if(jQuery('#nacid').val()=='') {
		alert('Nacionalidade em branco');
		return false;
	}

	if(jQuery('#eciid').val()=='') {
		alert('Estado Civil em branco');
		return false;
	}

	if(jQuery('#eciid').val()=='<?=ECI_CASADO ?>' || jQuery('#eciid').val()=='<?=ECI_UNIAO_ESTAVEL ?>') {
		if(jQuery('#iusnomeconjuge').val()=='') {
			alert('Nome do Conjuge em branco');
			return false;
		}
	}
	
	if(jQuery('#itdufdoc').val()=='') {
		alert('Estado - Documento em branco');
		return false;
	}
	
	if(jQuery('#tdoid').val()=='') {
		alert('Tipo - Documento em branco');
		return false;
	}
	
	if(jQuery('#itdnumdoc').val()=='') {
		alert('Número do Documento em branco');
		return false;
	}
	
	if(jQuery('#itddataexp').val()=='') {
		alert('Data Expedição em branco');
		return false;
	}
	
	if(!validaData(document.getElementById('itddataexp'))) {
		alert('Data Expedição inválida');
		return false;
	}
	
	if(jQuery('#itdnoorgaoexp').val()=='') {
		alert('Orgão Expedidor em branco');
		return false;
	}
	
	if(jQuery('#tvpid').val()=='') {
		alert('Vínculo em branco');
		return false;
	}
	
	if(jQuery('#funid').val()=='') {
		alert('Função em branco');
		return false;
	}
	
	if(jQuery('#foeid').val()=='') {
		alert('Formação (Escolaridade) em branco');
		return false;
	}
	
	if(jQuery('#foeid').val()=='<?=FOE_ESPECIALIZACAO ?>' || jQuery('#foeid').val()=='<?=FOE_MESTRADO ?>' || jQuery('#foeid').val()=='<?=FOE_DOUTORADO ?>' || jQuery('#foeid').val()=='<?=FOE_SUPERIOR_COMPLETO_PEDAGOGIA ?>' || jQuery('#foeid').val()=='<?=FOE_SUPERIOR_COMPLETO_LICENCIATURA ?>' || jQuery('#foeid').val()=='<?=FOE_SUPERIOR_COMPLETO_OUTRO ?>') {	   

		if(document.getElementById('cufid')) {
			if(jQuery('#cufid').val()=='') {
				alert('Área de Formação em branco');
				return false;
			}
		} else {
			alert('Selecione a Área');
			return false;
		}
	
	}
	
	var iufsituacaoformacao_r = jQuery("[name^='iufsituacaoformacao']:enabled:checked").length;
	
	if(iufsituacaoformacao_r==0) {
		alert('Situação formação em branco');
		return false;
	}
	
	if(jQuery('#iufdatainiformacao').val()=='') {
		alert('Início - Formação em branco');
		return false;
	}
	
	if(!validaData(document.getElementById('iufdatainiformacao'))) {
		alert('Início - Formação inválida');
		return false;
	}
	
	if(document.getElementById('iufsituacaoformacao_C').checked) {

		if(jQuery('#iufdatafimformacao').val()=='') {
			alert('Fim - Formação em branco');
			return false;
		}
		
		if(!validaData(document.getElementById('iufdatafimformacao'))) {
			alert('Fim - Formação inválida');
			return false;
		}
		
		var data1    = jQuery('#iufdatainiformacao').val();
		data1 		 = data1.split('/');
		var data2    = jQuery('#iufdatafimformacao').val();
		data2 		 = data2.split('/');
	
		if(data2[2]+data2[1]+data2[0] < data1[2]+data1[1]+data1[0]) {
			alert('Data de Inicio de Formação é maior do que Data Fim');
			return false;
		}
	
	}
	
	jQuery('#iencep').val(mascaraglobal('#####-###',jQuery('#iencep').val()));
	
	if(jQuery('#iencep').val()=='') {
		alert('CEP em branco');
		return false;
	}
	
	if(document.getElementById('muncod_endereco')) {
		if(jQuery('#estuf_endereco').val()=='') {
			alert('Estado - Endereço em branco');
			return false;
		}
	} else {
		alert('Estado - Endereço em branco');
		return false;
	}
	
	if(document.getElementById('muncod_endereco')) {
		if(jQuery('#muncod_endereco').val()=='') {
			alert('Município - Endereço em branco');
			return false;
		}
	} else {
		alert('Município - Endereço em branco');
		return false;
	}
	
	var ientipo_r = jQuery("[name^='ientipo']:enabled:checked").length;
	
	if(ientipo_r==0) {
		alert('Tipo - Endereço em branco');
		return false;
	}

	if(jQuery('#ienlogradouro').val()=='') {
		alert('Logradouro em branco');
		return false;
	}
	
	if(jQuery('#ienbairro').val()=='') {
		alert('Bairro em branco');
		return false;
	}
	
	jQuery('#iennumero').val(mascaraglobal('#####',jQuery('#iennumero').val()));
	
	if(jQuery('#iennumero').val()=='') {
		alert('Número em branco');
		return false;
	}
	
	if(jQuery('#itedddtel_T').val()=='') {
		alert('DDD Trabalho em branco');
		return false;
	}
	
	if(jQuery('#itedddtel_T').val().length!=2) {
		alert('DDD Trabalho deve possuir 2 dígitos');
		return false;
	}
	
	if(jQuery('#itenumtel_T').val()=='') {
		alert('Telefone Trabalho em branco');
		return false;
	}
	
	if(jQuery('#itenumtel_T').val().length!=9) {
		alert('Telefone Trabalho deve possuir 8 dígitos');
		return false;
	}
	
	if(jQuery('#itedddtel_R').val()!='') {
		jQuery('#itedddtel_R').val(mascaraglobal('##',jQuery('#itedddtel_R').val()));
	}
	
	if(jQuery('#itenumtel_R').val()!='') {
		jQuery('#itenumtel_R').val(mascaraglobal('####-####',jQuery('#itenumtel_R').val()));
	}

	
	if(jQuery('#iusemailprincipal').val()=='') {
		alert('Email Principal em branco');
		return false;
	}
	
    if(!validaEmail(jQuery('#iusemailprincipal').val())) {
    	alert('Email Principal inválido');
    	return false;
    }

	if(jQuery('#iusemailopcional').val()!='') {
	    if(!validaEmail(jQuery('#iusemailopcional').val())) {
	    	alert('Email Opcional inválido');
	    	return false;
	    }
    }
    
    var email_unico = true;
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: 'requisicao=verificarEmailUnico&iusemailprincipal='+jQuery('#iusemailprincipal').val()+'&iusd='+jQuery('#iusd').val(),
   		async: false,
   		success: function(html){
   			if(html) {
   				alert('Por questões de segurança, o e-mail deve ser único por pessoa. Constatamos que o email utilizado ('+jQuery('#iusemailprincipal').val()+') se encontra cadastrado em outros usuários:\n\n'+html);
				email_unico = false;
   			}
   		}
	});
	
	if(email_unico==false) {
		return false;
	}
    
	if(jQuery('#iusagenciasugerida').val()=='') {
		alert('Agência em branco');
		return false;
	}
	
	if(jQuery('#iusagenciaend').val()=='') {
		alert('Endereço em branco');
		return false;
	}
    
    if(jQuery('#iustermocompromisso').attr('checked')==false) {
    	alert('É necessário aceitar o Termo de Compromisso');
    	return false;
    }
    
    divCarregando();
    
    jQuery('#goto').val(goto);

	document.getElementById('formulario').submit();

}

function verificaEstadoCivil(eciid) {

	if(eciid=='<?=ECI_CASADO ?>' || eciid=='<?=ECI_UNIAO_ESTAVEL ?>') {
		jQuery('#tr_iusnomeconjuge').css('display','');	
	} else {
		jQuery('#iusnomeconjuge').val('');
		jQuery('#tr_iusnomeconjuge').css('display','none');
	}

}

function verificaFormacaoEscolar(foeid) {
	
	if(foeid) {
	   	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: 'requisicao=verificarFormacaoCompleta&foeid='+foeid,
   		async: false,
   		success: function(res){
			jQuery("[name^='iufsituacaoformacao'][value=C]").attr('disabled',false);
			jQuery("[name^='iufsituacaoformacao'][value=I]").attr('disabled',false);
			jQuery("[name^='iufsituacaoformacao'][value=E]").attr('disabled',false);

			jQuery("[name^='iufsituacaoformacao'][value=C]").attr('checked',false);
			jQuery("[name^='iufsituacaoformacao'][value=I]").attr('checked',false);
			jQuery("[name^='iufsituacaoformacao'][value=E]").attr('checked',false);

			if(res=='T') {
				jQuery("[name^='iufsituacaoformacao'][value=C]").attr('checked',true);
				jQuery("[name^='iufsituacaoformacao'][value=I]").attr('disabled',true);
				jQuery("[name^='iufsituacaoformacao'][value=E]").attr('disabled',true);
			}
			if(res=='F') {
				jQuery("[name^='iufsituacaoformacao'][value=I]").attr('checked',true);
				jQuery("[name^='iufsituacaoformacao'][value=C]").attr('disabled',true);
				jQuery("[name^='iufsituacaoformacao'][value=E]").attr('disabled',true);
			}
   		}
		});
	}

	if(foeid=='<?=FOE_ESPECIALIZACAO ?>' || foeid=='<?=FOE_MESTRADO ?>' || 
	   foeid=='<?=FOE_DOUTORADO ?>' || foeid=='<?=FOE_SUPERIOR_COMPLETO_PEDAGOGIA ?>' || 
	   foeid=='<?=FOE_SUPERIOR_COMPLETO_LICENCIATURA ?>' || foeid=='<?=FOE_SUPERIOR_COMPLETO_OUTRO ?>' ||
	   foeid=='<?=FOE_SUPERIOR_INCOMPLETO ?>') {
	   
	   	jQuery('#cufcodareageral').val('');
		jQuery('#cufcodareageral').attr('disabled','');
		jQuery('#td_listaCursosFormacao').html('Selecione uma Área');

	} else {	
	   	jQuery('#cufcodareageral').val('');
		jQuery('#cufcodareageral').attr('disabled','disabled');
		jQuery('#td_listaCursosFormacao').html('Selecione uma Área');
	}

}


function listarAgencias() {
	jQuery("#modalAgencia").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });
}

function buscarAgencias(muncod) {
	if(document.getElementById('estuf_agencias')) {
		if(jQuery('#muncod_agencias').val()!='') {
			ajaxatualizar('requisicao=listarAgencias&muncod='+muncod,'listaAgencias');
			jQuery('#tr_agencia').css('display','');
		} else {
			alert('Selecione um município');
			return false;
		}
	} else {
		alert('Selecione um estado');
		return false;
	}

}

function carregarCursosFormacao(cod) {
	if(cod) {
		ajaxatualizar('requisicao=listarCursosFormacao&cufcodareageral='+cod,'td_listaCursosFormacao');
	} else {
		jQuery('#td_listaCursosFormacao').html('Selecione uma Área');
	}
}

function selecionarAgencia() {
	if(jQuery('#dados_agencia').val()!='') {
		var dadosagencia = jQuery('#dados_agencia').val();
		dadosagencia = dadosagencia.split('_');
		var iusagenciasugerida 		= dadosagencia[0];
		var cep 			= dadosagencia[1];
		var iusagenciaend		= 'Endereço não encontrado';
		
		jQuery.ajax({
	   		type: "POST",
	   		url: '/geral/consultadadosentidade.php',
	   		data: 'requisicao=pegarenderecoPorCEP&endcep='+cep,
	   		async: false,
	   		success: function(resposta){
				var dadosendereco = resposta.split("||");
				iusagenciaend = 'CEP: '+mascaraglobal('#####-###',cep)+', Logradouro: '+dadosendereco[0]+', Bairro: '+dadosendereco[1]+', Cidade: '+dadosendereco[2]+', UF: '+dadosendereco[3];
			}
		});
		
		jQuery('#iusagenciasugerida').val(iusagenciasugerida);
		jQuery('#iusagenciaend').val(iusagenciaend);
		
		jQuery('#modalAgencia').dialog('close');
		
		
	} else {
		alert('Selecione uma agência');
		return false;
	}
}

function anexarDocumentoDesignacao() {

	if(jQuery('#documentodesignacao').val()=='') {
		alert('Selecione um arquivo');
		return false;	
	}
	
	jQuery('#requisicao').val('anexarDocumentoDesignacao');
	
    divCarregando();
    
    jQuery('#goto').val('<?=$goto_sal ?>');

	document.getElementById('formulario').submit();

}

function atualizarNomeUsuario() {
	window.location = 'sisindigena2.php?modulo=<?=$_REQUEST['modulo'] ?>&acao=A&requisicao=atualizarNomeUsuario&iuscpf=<?=$iuscpf ?>';
}

<? if($consulta) : ?>
jQuery(function() {
jQuery("[id='btnagencias']").remove();
jQuery("[type='text']").attr('class','disabled');
jQuery("[type='text']").attr('disabled','disabled');
jQuery("[type='radio']").attr('disabled','disabled');
jQuery("[name='cufid'],[name='cufcodareageral'],[name='tdoid'],[name='itdufdoc'],[name='eciid'],[name='nacid'],[name='muncod_nascimento'],[name='estuf_nascimento'],[name='iusagenciaend'],[name='tvpid'],[name='funid'],[name='foeid']").attr('disabled','disabled');
});
<? endif; ?>

</script>
<div id="modalAgencia" style="display:none;">
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloCentro" colspan="2">Localizar Agências</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%"><b>Estado:</b></td><td><? 
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('estuf_agencias', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF4', '', '', '', 'S', 'estuf_agencias', ''); 
	?></td>
</tr>
<tr>	
	<td class="SubTituloDireita" width="20%"><b>Município:</b></td>
	<td id="td_municipio4">Selecione uma UF</td>
</tr>
<tr id="tr_agencia" style="display:none;">
	<td class="SubTituloDireita" width="20%"><b>Agência:</b></td>
	<td id="listaAgencias"></td>
</tr>
<tr>	
	<td class="SubTituloCentro" colspan="2"><input type="button" value="Selecionar Agência" onclick="selecionarAgencia();"></td>
</tr>

</table>
</div>
<form method="post" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="mensagemalert" value="<?=$mensagemalert ?>">
<input type="hidden" name="requisicao" id="requisicao" value="atualizarDadosIdentificacaoUsuario">
<input type="hidden" name="iusd" id="iusd" value="<?=$iusd ?>">
<input type="hidden" name="goto" id="goto" value="">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Dados</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">Dados Pessoais</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">CPF</td>
	<td><?=campo_texto('iuscpf', "S", "N", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="iuscpf"', '', mascaraglobal($iuscpf,"###.###.###-##")); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Nome</td>
	<td><?=campo_texto('iusnome', "S", "N", "Nome", 67, 150, "", "", '', '', 0, 'id="iusnome"', ''); ?> <? if($iustermocompromisso!='t'): ?><span style="cursor:pointer;" onclick="atualizarNomeUsuario();"><img src="../imagens/refresh2.gif"> Caso o nome não esteja condizente com CPF. Clique para atualizar.</span><? endif; ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Data de Nascimento</td>
	<td><?=campo_data2('iusdatanascimento','S', 'S', 'Data nascimento', 'S', '', '', '', '', '', 'iusdatanascimento'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Local Nascimento</td>
	<td>
	
	<table>
	<tr>
	<td>Estado:</td><td><? 
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('estuf_nascimento', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF2', '', '', '', 'S', 'estuf_nascimento', ''); 
	?></td>
	<td>Município:</td>
	<td id="td_municipio2">
	<? 
	if($estuf_nascimento) : 
		$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$estuf_nascimento."' ORDER BY mundescricao";
		$db->monta_combo('muncod_nascimento', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'muncod_nascimento', '');
	else :
		echo "Selecione uma UF";
	endif;
	?> 
	</td>
	</tr>
	</table>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Sexo</td>
	<td><input type="radio" name="iussexo" value="M" <?=(($iussexo=="M")?"checked":"") ?>> Masculino <input type="radio" name="iussexo" value="F" <?=(($iussexo=="F")?"checked":"") ?>> Feminino</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Nome da mãe</td>
	<td><?=campo_texto('iusnomemae', "S", "S", "Nome da mãe", 60, 60, "", "", '', '', 0, 'id="iusnomemae"' ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Nacionalidade</td>
	<td><?
	if(!$nacid) $nacid = NAC_BRASIL;
	$sql = "SELECT nacid as codigo, nacdesc as descricao FROM sisindigena2.nacionalidade ORDER BY naccodpais";
	$db->monta_combo('nacid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'nacid', ''); 
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Estado Civil</td>
	<td><?
	$sql = "SELECT eciid as codigo, ecidesc as descricao FROM sisindigena2.estadocivil ORDER BY ecidesc";
	$db->monta_combo('eciid', $sql, 'S', 'Selecione', 'verificaEstadoCivil', '', '', '200', 'S', 'eciid', ''); 
	?></td>
</tr>
<tr id="tr_iusnomeconjuge" <?=(($eciid==ECI_CASADO || $eciid==ECI_UNIAO_ESTAVEL)?"":"style=\"display:none;\"") ?>>
	<td class="SubTituloDireita" width="20%">Nome do Cônjuge</td>
	<td><?=campo_texto('iusnomeconjuge', "S", "S", "Nome do Conjuge", 67, 150, "", "", '', '', 0, 'id="iusnomeconjuge"' ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Documento de identificação</td>
	<td>
	<table>
	<tr>
		<td>Estado</td>
		<td><? 
		$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
		$db->monta_combo('itdufdoc', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'itdufdoc', ''); 
		?></td>
	</tr>
	<tr>
		<td>Tipo</td>
		<td><? 
		if(!$tdoid) $tdoid = TDO_RG;
		$sql = "SELECT tdoid as codigo, tdodesc as descricao FROM sisindigena2.tipodocumento WHERE tdostatus='A' ORDER BY tdodesc";
		$db->monta_combo('tdoid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'tdoid', ''); 
		?></td>
	</tr>
	<tr>
		<td>Número do Documento</td>
		<td><?=campo_texto('itdnumdoc', "S", "S", "Número do Documento", 30, 30, "", "", '', '', 0, 'id="itdnumdoc"' ); ?></td>
	</tr>
	<tr>
		<td>Data Expedição</td>
		<td><?=campo_data2('itddataexp','S', 'S', 'Data Expedição', 'S', '', '', '', '', '', 'itddataexp'); ?></td>
	</tr>
	<tr>
		<td>Orgão Expedidor</td>
		<td><?=campo_texto('itdnoorgaoexp', "S", "S", "Orgão Expedidor", 50, 50, "", "", '', '', 0, 'id="itdnoorgaoexp"' ); ?></td>
	</tr>
	</table>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">Dados Profissionais</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Vínculo</td>
	<td><?
	$sql = "SELECT tvpid as codigo, tvpdsc as descricao FROM public.tipovinculoprofissional WHERE tvpstatus='A'";
	$db->monta_combo('tvpid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'tvpid', ''); 
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Função atual ou similar</td>
	<td><?
	$sql = "SELECT funid as codigo, funnome as descricao FROM sisindigena2.funcao WHERE funstatus='A'";
	$db->monta_combo('funid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'funid', ''); 
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Formação (Escolaridade)</td>
	<td><?
	$sql = "SELECT foeid as codigo, foecod||' - '||foedesc as descricao FROM sisindigena2.formacaoescolaridade WHERE foestatus='A' ORDER BY foecod";
	$db->monta_combo('foeid', $sql, 'S', 'Selecione', 'verificaFormacaoEscolar', '', '', '', 'S', 'foeid', '');

	$disabled_iufsituacaoformacao = array();
	$habil_areaformacao = "S";
	
	if($foeid) {
		$foecompleto = $db->pegaUm("SELECT foecompleto FROM sisindigena2.formacaoescolaridade WHERE foeid='".$foeid."'");
		if($foecompleto=="t") {
			$iufsituacaoformacao = "C";
			$disabled_iufsituacaoformacao = array("I","A"); 
		}
		if($foecompleto=="f") {
			$iufsituacaoformacao = "I";
			$disabled_iufsituacaoformacao = array("C","A");
		}
		if($foeid==FOE_ESPECIALIZACAO || $foeid==FOE_MESTRADO || 
	   	   $foeid==FOE_DOUTORADO || $foeid==FOE_SUPERIOR_COMPLETO_PEDAGOGIA || 
	   	   $foeid==FOE_SUPERIOR_COMPLETO_LICENCIATURA || $foeid==FOE_SUPERIOR_COMPLETO_OUTRO || $foeid==FOE_SUPERIOR_INCOMPLETO) {
	   	   	
	   	   	$habil_areaformacao = "S";
	   	   	
	   } else {
	   	   	$habil_areaformacao = "N";
	   }
		
	}
	
	
	
	?></td>
</tr>

<tr>
	<td class="SubTituloDireita" width="20%">Área de formação</td>
	<td>
	<table>
	<tr>
		<td>Área</td>
		<td><? 
		$sql = "SELECT cufcodareageral as codigo, cufareageraldesc as descricao FROM sisindigena2.cursoformacao WHERE cufstatus='A' GROUP BY cufcodareageral, cufareageraldesc ORDER BY cufareageraldesc";
		$db->monta_combo('cufcodareageral', $sql, $habil_areaformacao, 'Selecione', 'carregarCursosFormacao', '', '', '', 'S', 'cufcodareageral', ''); 
		
		?></td>
	</tr>

	<tr>
		<td>Área de Formação (Selecione a mais relevante, se houver mais de uma)</td>
		<td id="td_listaCursosFormacao"><?
		 
		if($cufcodareageral)  {
			$sql = "SELECT cufid as codigo, cufcursodesc as descricao FROM sisindigena2.cursoformacao WHERE cufstatus='A' AND cufcodareageral='".$cufcodareageral."' ORDER BY cufcursodesc";
			$db->monta_combo('cufid', $sql, 'S', 'Selecione', '', '', '', '400', 'S', 'cufid', '');
		} else {
			echo "Selecione uma Área";
		}
		
		?></td>
	</tr>
	<tr>
		<td>Situação formação</td>
		<td><input type="radio" name="iufsituacaoformacao" id="iufsituacaoformacao_C" value="C" <?=(($iufsituacaoformacao=="C")?"checked":"") ?> <?=((in_array("C",$disabled_iufsituacaoformacao))?"disabled":"") ?>> Completo <input type="radio" name="iufsituacaoformacao" value="I" <?=(($iufsituacaoformacao=="I")?"checked":"") ?> <?=((in_array("I",$disabled_iufsituacaoformacao))?"disabled":"") ?>> Incompleto <input type="radio" name="iufsituacaoformacao" value="E" <?=(($iufsituacaoformacao=="E")?"checked":"") ?> <?=((in_array("A",$disabled_iufsituacaoformacao))?"disabled":"") ?>> Em andamento</td>
	</tr>
	<tr>
		<td>Início</td>
		<td><?=campo_data2('iufdatainiformacao','S', 'S', 'Início', 'S', '', '', '', '', '', 'iufdatainiformacao'); ?></td>
	</tr>
	<tr>
		<td>Fim</td>
		<td><?=campo_data2('iufdatafimformacao','S', 'S', 'Fim', 'S', '', '', '', '', '', 'iufdatafimformacao'); ?></td>
	</tr>
	</table>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">Contatos</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Endereço</td>
	<td>
	<table>
	<tr>
		<td>Tipo</td>
		<td><input type="radio" name="ientipo" value="R" <?=(($ientipo=="R")?"checked":"") ?>> Residencial <input type="radio" name="ientipo" value="P" <?=(($ientipo=="P")?"checked":"") ?>> Profissional <input type="radio" name="ientipo" value="D" <?=(($ientipo=="D")?"checked":"") ?>> Descanso</td>
	</tr>
	<tr>
		<td>CEP</td>
		<td><?=campo_texto('iencep', "S", "S", "CEP", 9, 10, "#####-###", "", '', '', 0, 'id="iencep"', '', mascaraglobal($iencep,"#####-###"), 'if(this.value.length==9){carregarEnderecoPorCEP_coordenadorLocal(this.value);}else{limparEnderecoPorCEP_coordenadorLocal();}' ); ?></td>
	</tr>
	<tr>
		<td>UF</td>
		<td><?
		$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
		$db->monta_combo('estuf_endereco', $sql, 'N', 'Selecione', 'carregarMunicipiosPorUF3', '', '', '', 'S', 'estuf_endereco', ''); 
		?></td>
	</tr>
	<tr>
		<td>Município</td>
		<td id="td_municipio3">
		<? 
		if($muncod_endereco) : 
			$sql = "SELECT mundescricao as descricao FROM territorios.municipio WHERE muncod='".$muncod_endereco."'";
			$mundescricao = $db->pegaUm($sql);
			echo $mundescricao;
			echo '<input type="hidden" id="muncod_endereco" name="muncod_endereco" value="'.$muncod_endereco.'">';
		else :
			echo "Digite CEP";
		endif;
		?>
		</td>
	</tr>
	<tr>
		<td>Logradouro</td>
		<td><?=campo_texto('ienlogradouro', "S", "S", "Logradouro", 60, 150, "", "", '', '', 0, 'id="ienlogradouro"' ); ?></td>
	</tr>
	<tr>
		<td>Bairro</td>
		<td><?=campo_texto('ienbairro', "S", "S", "Bairro", 60, 60, "", "", '', '', 0, 'id="ienbairro"' ); ?></td>
	</tr>
	<tr>
		<td>Complemento</td>
		<td><?=campo_texto('iencomplemento', "N", "S", "Complemento", 50, 50, "", "", '', '', 0, 'id="iencomplemento"' ); ?></td>
	</tr>
	<tr>
		<td>Número</td>
		<td><?=campo_texto('iennumero', "S", "S", "Número", 6, 5, "#####", "", '', '', 0, 'id="iennumero"' ); ?></td>
	</tr>
	</table>
	</td>
</tr>

<tr>
	<td class="SubTituloDireita" width="20%">Telefone</td>
	<td>
	<table>
	<tr>
		<td>Residencial</td>
		<td><?=campo_texto('itedddtel[R]', "N", "S", "DDD", 3, 2, "##", "", '', '', 0, 'id="itedddtel_R"', '', $telefones['R']['itedddtel'] ); ?> <?=campo_texto('itenumtel[R]', "N", "S", "Telefone", 10, 9, "####-####", "", '', '', 0, 'id="itenumtel_R"', '', $telefones['R']['itenumtel'] ); ?></td>
	</tr>
	<tr>
		<td>Trabalho</td>
		<td><?=campo_texto('itedddtel[T]', "N", "S", "DDD", 3, 2, "##", "", '', '', 0, 'id="itedddtel_T"', '', $telefones['T']['itedddtel'] ); ?> <?=campo_texto('itenumtel[T]', "S", "S", "Telefone", 10, 9, "####-####", "", '', '', 0, 'id="itenumtel_T"', '', $telefones['T']['itenumtel'] ); ?></td>
	</tr>
	<tr>
		<td>Celular</td>
		<td><?=campo_texto('itedddtel[C]', "N", "S", "DDD", 3, 2, "##", "", '', '', 0, 'id="itedddtel_C"', '', $telefones['C']['itedddtel'] ); ?> <?=campo_texto('itenumtel[C]', "N", "S", "Telefone", 10, 9, "####-####", "", '', '', 0, 'id="itenumtel_C"', '', $telefones['C']['itenumtel'] ); ?></td>
	</tr>
	<tr>
		<td>Outro</td>
		<td><?=campo_texto('itedddtel[F]', "N", "S", "DDD", 3, 2, "##", "", '', '', 0, 'id="itedddtel_F"', '', $telefones['F']['itedddtel'] ); ?> <?=campo_texto('itenumtel[F]', "N", "S", "Telefone", 10, 9, "####-####", "", '', '', 0, 'id="itenumtel_F"', '', $telefones['F']['itenumtel'] ); ?></td>
	</tr>
	</table>
	</td>
</tr>

<tr>
	<td class="SubTituloDireita" width="20%">E-mail</td>
	<td>
	<table>
	<tr>
		<td>Principal</td>
		<td><?=campo_texto('iusemailprincipal', "S", "S", "Principal", 60, 60, "", "", '', '', 0, 'id="iusemailprincipal"'); ?></td>
	</tr>
	<tr>
		<td>Opcional</td>
		<td><?=campo_texto('iusemailopcional', "N", "S", "Opcional", 60, 60, "", "", '', '', 0, 'id="iusemailopcional"'); ?></td>
	</tr>
	</table>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">Dados para recebimento da bolsa de estudo</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Agência do Banco do Brasil sugerida</td>
	<td>
	<table>
	<tr>
		<td>Agência</td>
		<td><?=campo_texto('iusagenciasugerida', "N", "N", "Agência Sugerida", 5, 4, "####", "", '', '', 0, 'id="iusagenciasugerida"' ); ?> <img align="absmiddle" src="../imagens/consultar.gif" border="0" onclick="listarAgencias();" id="btnagencias" style="cursor:pointer;"></td>
	</tr>
	<tr>
		<td>Endereço</td>
		<td><?=campo_textarea('iusagenciaend', 'N', 'S', '', '70', '4', '250'); ?></td>
	</tr>
	</table>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">TERMO DE COMPROMISSO</td>
	<td>
	<table width="100%">
	<? if($sis=='coordenadorlocal') : ?>
	<tr>
		<td>
		<p>Declaro ter ciência dos direitos e das obrigações inerentes à qualidade de bolsista na função de Coordenador Local e COMPROMETO-ME a respeitar as cláusulas descritas na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		<p>Declaro ainda, estar ciente que não é permitido acumular mais de uma de bolsa de estudo ou pesquisa proveniente de outros programas regidos pela Lei nº 11.273/2006. Desta forma, estou ciente que caso eu já seja bolsista terei direito de receber apenas uma bolsa.</p>
		<p>Declaro também, sob as penas da lei, que as informações aqui prestadas são a expressão da verdade e que, caso não receba outra bolsa, preencho plenamente os requisitos para o recebimento desta, expressos na referida Lei nº 11.273 de 6 de fevereiro de 2006 e na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		<p>Autorizo o Fundo Nacional de Desenvolvimento da Educação a bloquear valores que porventura venham a ser indevidamente creditados em meu favor, mediante solicitação direta ao Banco do Brasil S/A, ou a proceder ao desconto nos pagamentos subsequentes, nas seguintes situações: determinação do Poder Judiciário ou requisição do Ministério Público; constatação de irregularidades na comprovação de frequência do bolsista e constatação de incorreções nas informações cadastrais do bolsista. Se não houver pagamentos futuros de bolsas, comprometo-me a restituir tais recursos na forma previstas no Art. 14 da  Resolução FNDE nº 54/2013.</p>
		<p>Estou ciente que a inobservância dos requisitos citados acima implicará no cancelamento da(s) bolsa(s), com a restituição integral e imediata dos recursos, de acordo com as regras previstas na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		</td>
	</tr>
	<tr>
		<td><input type="checkbox" name="iustermocompromisso" id="iustermocompromisso" value="TRUE" <?=(($iustermocompromisso=='t')?'checked=true disabled=true':'') ?>> <b>Eu li e concordo com o TERMO DE COMPROMISSO</b><br/>
		<input type="checkbox" name="iusnaodesejosubstituirbolsa" id="iusnaodesejosubstituirbolsa" value="TRUE" <?=(($iusnaodesejosubstituirbolsa=='t')?'checked=true':'') ?>> <b>Já sou bolsista do FNDE/MEC e não desejo substituir minha bolsa atual pela bolsa do SISIndígena</b></td>
	</tr>
	<? elseif($sis=='formadories') : ?>
	<tr>
		<td>
		<p>Declaro ter ciência dos direitos e das obrigações inerentes à qualidade de bolsista na função de Formador da Instituição de Ensino Superior e COMPROMETO-ME a respeitar as cláusulas descritas na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		<p>Declaro ainda, estar ciente que não é permitido acumular mais de uma de bolsa de estudo ou pesquisa proveniente de outros programas regidos pela Lei nº 11.273/2006. Desta forma, estou ciente que caso eu já seja bolsista terei direito de receber apenas uma bolsa.</p>
		<p>Declaro também, sob as penas da lei, que as informações aqui prestadas são a expressão da verdade e que, caso não receba outra bolsa, preencho plenamente os requisitos para o recebimento desta, expressos na referida Lei nº 11.273 de 6 de fevereiro de 2006 e na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		<p>Autorizo o Fundo Nacional de Desenvolvimento da Educação a bloquear valores que porventura venham a ser indevidamente creditados em meu favor, mediante solicitação direta ao Banco do Brasil S/A, ou a proceder ao desconto nos pagamentos subsequentes, nas seguintes situações: determinação do Poder Judiciário ou requisição do Ministério Público; constatação de irregularidades na comprovação de frequência do bolsista e constatação de incorreções nas informações cadastrais do bolsista. Se não houver pagamentos futuros de bolsas, comprometo-me a restituir tais recursos na forma previstas no Art. 14 da  Resolução FNDE nº 54/2013.</p>
		<p>Estou ciente que a inobservância dos requisitos citados acima implicará no cancelamento da(s) bolsa(s), com a restituição integral e imediata dos recursos, de acordo com as regras previstas na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		</td>
	</tr>
	<tr>
		<td><input type="checkbox" name="iustermocompromisso" id="iustermocompromisso" value="TRUE" <?=(($iustermocompromisso=='t')?'checked=true disabled=true':'') ?>> <b>Eu li e concordo com o TERMO DE COMPROMISSO</b><br/>
		<input type="checkbox" name="iusnaodesejosubstituirbolsa" id="iusnaodesejosubstituirbolsa" value="TRUE" <?=(($iusnaodesejosubstituirbolsa=='t')?'checked=true':'') ?>> <b>Já sou bolsista do FNDE/MEC e não desejo substituir minha bolsa atual pela bolsa do SISIndígena</b></td>
	</tr>
	<? elseif($sis=='supervisories') : ?>
	<tr>
		<td>
		<p>Declaro ter ciência dos direitos e das obrigações inerentes à qualidade de bolsista na função de Supervisor da Instituição de Ensino Superior e COMPROMETO-ME a respeitar as cláusulas descritas na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		<p>Declaro ainda, estar ciente que não é permitido acumular mais de uma de bolsa de estudo ou pesquisa proveniente de outros programas regidos pela Lei nº 11.273/2006. Desta forma, estou ciente que caso eu já seja bolsista terei direito de receber apenas uma bolsa.</p>
		<p>Declaro também, sob as penas da lei, que as informações aqui prestadas são a expressão da verdade e que, caso não receba outra bolsa, preencho plenamente os requisitos para o recebimento desta, expressos na referida Lei nº 11.273 de 6 de fevereiro de 2006 e na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		<p>Autorizo o Fundo Nacional de Desenvolvimento da Educação a bloquear valores que porventura venham a ser indevidamente creditados em meu favor, mediante solicitação direta ao Banco do Brasil S/A, ou a proceder ao desconto nos pagamentos subsequentes, nas seguintes situações: determinação do Poder Judiciário ou requisição do Ministério Público; constatação de irregularidades na comprovação de frequência do bolsista e constatação de incorreções nas informações cadastrais do bolsista. Se não houver pagamentos futuros de bolsas, comprometo-me a restituir tais recursos na forma previstas no Art. 14 da  Resolução FNDE nº 54/2013.</p>
		<p>Estou ciente que a inobservância dos requisitos citados acima implicará no cancelamento da(s) bolsa(s), com a restituição integral e imediata dos recursos, de acordo com as regras previstas na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		</td>
	</tr>
	<tr>
		<td><input type="checkbox" name="iustermocompromisso" id="iustermocompromisso" value="TRUE" <?=(($iustermocompromisso=='t')?'checked=true disabled=true':'') ?>> <b>Eu li e concordo com o TERMO DE COMPROMISSO</b><br/>
		<input type="checkbox" name="iusnaodesejosubstituirbolsa" id="iusnaodesejosubstituirbolsa" value="TRUE" <?=(($iusnaodesejosubstituirbolsa=='t')?'checked=true':'') ?>> <b>Já sou bolsista do FNDE/MEC e não desejo substituir minha bolsa atual pela bolsa do SISIndígena</b></td>
	</tr>
	<? elseif($sis=='orientadorestudo') : ?>
	<tr>
		<td>
		<p>Declaro ter ciência dos direitos e das obrigações inerentes à qualidade de bolsista na função de Orientador de Estudo e COMPROMETO-ME a respeitar as cláusulas descritas na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		<p>Declaro ainda, estar ciente que não é permitido acumular mais de uma de bolsa de estudo ou pesquisa proveniente de outros programas regidos pela Lei nº 11.273/2006. Desta forma, estou ciente que caso eu já seja bolsista terei direito de receber apenas uma bolsa.</p>
		<p>Declaro também, sob as penas da lei, que as informações aqui prestadas são a expressão da verdade e que, caso não receba outra bolsa, preencho plenamente os requisitos para o recebimento desta, expressos na referida Lei nº 11.273 de 6 de fevereiro de 2006 e na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		<p>Autorizo o Fundo Nacional de Desenvolvimento da Educação a bloquear valores que porventura venham a ser indevidamente creditados em meu favor, mediante solicitação direta ao Banco do Brasil S/A, ou a proceder ao desconto nos pagamentos subsequentes, nas seguintes situações: determinação do Poder Judiciário ou requisição do Ministério Público; constatação de irregularidades na comprovação de frequência do bolsista e constatação de incorreções nas informações cadastrais do bolsista. Se não houver pagamentos futuros de bolsas, comprometo-me a restituir tais recursos na forma previstas no Art. 14 da  Resolução FNDE nº 54/2013.</p>
		<p>Estou ciente que a inobservância dos requisitos citados acima implicará no cancelamento da(s) bolsa(s), com a restituição integral e imediata dos recursos, de acordo com as regras previstas na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		</td>
	</tr>
	<tr>
		<td><input type="checkbox" name="iustermocompromisso" id="iustermocompromisso" value="TRUE" <?=(($iustermocompromisso=='t')?'checked=true disabled=true':'') ?>> <b>Eu li e concordo com o TERMO DE COMPROMISSO</b><br/>
		<input type="checkbox" name="iusnaodesejosubstituirbolsa" id="iusnaodesejosubstituirbolsa" value="TRUE" <?=(($iusnaodesejosubstituirbolsa=='t')?'checked=true':'') ?>> <b>Já sou bolsista do FNDE/MEC e não desejo substituir minha bolsa atual pela bolsa do SISIndígena</b></td>
	</tr>
	<? elseif($sis=='coordenadoradjuntoies') : ?>
	<tr>
		<td>
		<p>Declaro ter ciência dos direitos e das obrigações inerentes à qualidade de bolsista na função de Coordenador Adjunto da Instituição de Ensino Superior e COMPROMETO-ME a respeitar as cláusulas descritas na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		<p>Declaro ainda, estar ciente que não é permitido acumular mais de uma de bolsa de estudo ou pesquisa proveniente de outros programas regidos pela Lei nº 11.273/2006. Desta forma, estou ciente que caso eu já seja bolsista terei direito de receber apenas uma bolsa.</p>
		<p>Declaro também, sob as penas da lei, que as informações aqui prestadas são a expressão da verdade e que, caso não receba outra bolsa, preencho plenamente os requisitos para o recebimento desta, expressos na referida Lei nº 11.273 de 6 de fevereiro de 2006 e na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		<p>Autorizo o Fundo Nacional de Desenvolvimento da Educação a bloquear valores que porventura venham a ser indevidamente creditados em meu favor, mediante solicitação direta ao Banco do Brasil S/A, ou a proceder ao desconto nos pagamentos subsequentes, nas seguintes situações: determinação do Poder Judiciário ou requisição do Ministério Público; constatação de irregularidades na comprovação de frequência do bolsista e constatação de incorreções nas informações cadastrais do bolsista. Se não houver pagamentos futuros de bolsas, comprometo-me a restituir tais recursos na forma previstas no Art. 14 da  Resolução FNDE nº 54/2013.</p>
		<p>Estou ciente que a inobservância dos requisitos citados acima implicará no cancelamento da(s) bolsa(s), com a restituição integral e imediata dos recursos, de acordo com as regras previstas na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		</td>
	</tr>
	<tr>
		<td><input type="checkbox" name="iustermocompromisso" id="iustermocompromisso" value="TRUE" <?=(($iustermocompromisso=='t')?'checked=true disabled=true':'') ?>> <b>Eu li e concordo com o TERMO DE COMPROMISSO</b><br/>
		<input type="checkbox" name="iusnaodesejosubstituirbolsa" id="iusnaodesejosubstituirbolsa" value="TRUE" <?=(($iusnaodesejosubstituirbolsa=='t')?'checked=true':'') ?>> <b>Já sou bolsista do FNDE/MEC e não desejo substituir minha bolsa atual pela bolsa do SISIndígena</b></td>
	</tr>
	<? elseif($sis=='professoralfabetizador') : ?>
	<tr>
		<td>
		<p>Declaro ter ciência dos direitos e das obrigações inerentes à qualidade de bolsista na função de Professor Alfabetizador e COMPROMETO-ME a respeitar as cláusulas descritas na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		<p>Declaro ainda, estar ciente que não é permitido acumular mais de uma de bolsa de estudo ou pesquisa proveniente de outros programas regidos pela Lei nº 11.273/2006. Desta forma, estou ciente que caso eu já seja bolsista terei direito de receber apenas uma bolsa.</p>
		<p>Declaro também, sob as penas da lei, que as informações aqui prestadas são a expressão da verdade e que, caso não receba outra bolsa, preencho plenamente os requisitos para o recebimento desta, expressos na referida Lei nº 11.273 de 6 de fevereiro de 2006 e na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		<p>Autorizo o Fundo Nacional de Desenvolvimento da Educação a bloquear valores que porventura venham a ser indevidamente creditados em meu favor, mediante solicitação direta ao Banco do Brasil S/A, ou a proceder ao desconto nos pagamentos subsequentes, nas seguintes situações: determinação do Poder Judiciário ou requisição do Ministério Público; constatação de irregularidades na comprovação de frequência do bolsista e constatação de incorreções nas informações cadastrais do bolsista. Se não houver pagamentos futuros de bolsas, comprometo-me a restituir tais recursos na forma previstas no Art. 14 da  Resolução FNDE nº 54/2013.</p>
		<p>Estou ciente que a inobservância dos requisitos citados acima implicará no cancelamento da(s) bolsa(s), com a restituição integral e imediata dos recursos, de acordo com as regras previstas na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		</td>
	</tr>
	<tr>
		<td><input type="checkbox" name="iustermocompromisso" id="iustermocompromisso" value="TRUE" <?=(($iustermocompromisso=='t')?'checked=true disabled=true':'') ?>> <b>Eu li e concordo com o TERMO DE COMPROMISSO</b><br/>
		<input type="checkbox" name="iusnaodesejosubstituirbolsa" id="iusnaodesejosubstituirbolsa" value="TRUE" <?=(($iusnaodesejosubstituirbolsa=='t')?'checked=true':'') ?>> <b>Já sou bolsista do FNDE/MEC e não desejo substituir minha bolsa atual pela bolsa do SISIndígena</b></td>
	</tr>
	<? elseif($sis=='universidade') : ?>
	<tr>
		<td>
		<p>Declaro ter ciência dos direitos e das obrigações inerentes à qualidade de bolsista na função de Coordenador da Instituição de Ensino Superior e COMPROMETO-ME a respeitar as cláusulas descritas na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		<p>Declaro ainda, estar ciente que não é permitido acumular mais de uma de bolsa de estudo ou pesquisa proveniente de outros programas regidos pela Lei nº 11.273/2006. Desta forma, estou ciente que caso eu já seja bolsista terei direito de receber apenas uma bolsa.</p>
		<p>Declaro também, sob as penas da lei, que as informações aqui prestadas são a expressão da verdade e que, caso não receba outra bolsa, preencho plenamente os requisitos para o recebimento desta, expressos na referida Lei nº 11.273 de 6 de fevereiro de 2006 e na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		<p>Autorizo o Fundo Nacional de Desenvolvimento da Educação a bloquear valores que porventura venham a ser indevidamente creditados em meu favor, mediante solicitação direta ao Banco do Brasil S/A, ou a proceder ao desconto nos pagamentos subsequentes, nas seguintes situações: determinação do Poder Judiciário ou requisição do Ministério Público; constatação de irregularidades na comprovação de frequência do bolsista e constatação de incorreções nas informações cadastrais do bolsista. Se não houver pagamentos futuros de bolsas, comprometo-me a restituir tais recursos na forma previstas no Art. 14 da  Resolução FNDE nº 54/2013.</p>		
		<p>Estou ciente que a inobservância dos requisitos citados acima implicará no cancelamento da(s) bolsa(s), com a restituição integral e imediata dos recursos, de acordo com as regras previstas na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		</td>
	</tr>
	<tr>
		<td>
		<input type="checkbox" name="iustermocompromisso" id="iustermocompromisso" value="TRUE" <?=(($iustermocompromisso=='t')?'checked=true disabled=true':'') ?>> <b>Eu li e concordo com o TERMO DE COMPROMISSO</b><br/>
		<input type="checkbox" name="iusnaodesejosubstituirbolsa" id="iusnaodesejosubstituirbolsa" value="TRUE" <?=(($iusnaodesejosubstituirbolsa=='t')?'checked=true':'') ?>> <b>Já sou bolsista do FNDE/MEC e não desejo substituir minha bolsa atual pela bolsa do SISIndígena</b>
		</td>
	</tr>
	<? elseif($sis=='conteudista') : ?>
	<tr>
		<td>
		<p>Declaro ter ciência dos direitos e das obrigações inerentes à qualidade de bolsista na função de Coordenador da Instituição de Ensino Superior e COMPROMETO-ME a respeitar as cláusulas descritas na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		<p>Declaro ainda, estar ciente que não é permitido acumular mais de uma de bolsa de estudo ou pesquisa proveniente de outros programas regidos pela Lei nº 11.273/2006. Desta forma, estou ciente que caso eu já seja bolsista terei direito de receber apenas uma bolsa.</p>
		<p>Declaro também, sob as penas da lei, que as informações aqui prestadas são a expressão da verdade e que, caso não receba outra bolsa, preencho plenamente os requisitos para o recebimento desta, expressos na referida Lei nº 11.273 de 6 de fevereiro de 2006 e na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		<p>Autorizo o Fundo Nacional de Desenvolvimento da Educação a bloquear valores que porventura venham a ser indevidamente creditados em meu favor, mediante solicitação direta ao Banco do Brasil S/A, ou a proceder ao desconto nos pagamentos subsequentes, nas seguintes situações: determinação do Poder Judiciário ou requisição do Ministério Público; constatação de irregularidades na comprovação de frequência do bolsista e constatação de incorreções nas informações cadastrais do bolsista. Se não houver pagamentos futuros de bolsas, comprometo-me a restituir tais recursos na forma previstas no Art. 14 da  Resolução FNDE nº 54/2013.</p>
		<p>Estou ciente que a inobservância dos requisitos citados acima implicará no cancelamento da(s) bolsa(s), com a restituição integral e imediata dos recursos, de acordo com as regras previstas na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		</td>
	</tr>
	<tr>
		<td>
		<input type="checkbox" name="iustermocompromisso" id="iustermocompromisso" value="TRUE" <?=(($iustermocompromisso=='t')?'checked=true disabled=true':'') ?>> <b>Eu li e concordo com o TERMO DE COMPROMISSO</b><br/>
		<input type="checkbox" name="iusnaodesejosubstituirbolsa" id="iusnaodesejosubstituirbolsa" value="TRUE" <?=(($iusnaodesejosubstituirbolsa=='t')?'checked=true':'') ?>> <b>Já sou bolsista do FNDE/MEC e não desejo substituir minha bolsa atual pela bolsa do SISIndígena</b>
		</td>
	</tr>
	<? elseif($sis=='pesquisador') : ?>
	<tr>
		<td>
		<p>Declaro ter ciência dos direitos e das obrigações inerentes à qualidade de bolsista na função de Coordenador da Instituição de Ensino Superior e COMPROMETO-ME a respeitar as cláusulas descritas na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		<p>Declaro ainda, estar ciente que não é permitido acumular mais de uma de bolsa de estudo ou pesquisa proveniente de outros programas regidos pela Lei nº 11.273/2006. Desta forma, estou ciente que caso eu já seja bolsista terei direito de receber apenas uma bolsa.</p>
		<p>Declaro também, sob as penas da lei, que as informações aqui prestadas são a expressão da verdade e que, caso não receba outra bolsa, preencho plenamente os requisitos para o recebimento desta, expressos na referida Lei nº 11.273 de 6 de fevereiro de 2006 e na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		<p>Autorizo o Fundo Nacional de Desenvolvimento da Educação a bloquear valores que porventura venham a ser indevidamente creditados em meu favor, mediante solicitação direta ao Banco do Brasil S/A, ou a proceder ao desconto nos pagamentos subsequentes, nas seguintes situações: determinação do Poder Judiciário ou requisição do Ministério Público; constatação de irregularidades na comprovação de frequência do bolsista e constatação de incorreções nas informações cadastrais do bolsista. Se não houver pagamentos futuros de bolsas, comprometo-me a restituir tais recursos na forma previstas no Art. 14 da  Resolução FNDE nº 54/2013.</p>
		<p>Estou ciente que a inobservância dos requisitos citados acima implicará no cancelamento da(s) bolsa(s), com a restituição integral e imediata dos recursos, de acordo com as regras previstas na Resolução que trata sobre a Formação de Professores Alfabetizadores no âmbito do Saberes Indígena na Escola.</p>
		</td>
	</tr>
	<tr>
		<td>
		<input type="checkbox" name="iustermocompromisso" id="iustermocompromisso" value="TRUE" <?=(($iustermocompromisso=='t')?'checked=true disabled=true':'') ?>> <b>Eu li e concordo com o TERMO DE COMPROMISSO</b><br/>
		<input type="checkbox" name="iusnaodesejosubstituirbolsa" id="iusnaodesejosubstituirbolsa" value="TRUE" <?=(($iusnaodesejosubstituirbolsa=='t')?'checked=true':'') ?>> <b>Já sou bolsista do FNDE/MEC e não desejo substituir minha bolsa atual pela bolsa do SISIndígena</b>
		</td>
	</tr>
	<? endif; ?>
	</table>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td><input type="button" value="Anterior" onclick="divCarregando();window.location='<?=$goto_ant ?>';">
		<? if(!$consulta) : ?> 
		<input type="button" name="salvar" value="Salvar" onclick="salvarDados('<?=$goto_sal ?>');">
		<? if($goto_pro) : ?>
		<input type="button" value="Salvar e Continuar" onclick="salvarDados('<?=$goto_pro ?>');">
		<? endif; ?>  
		<? endif; ?>
		<? if($goto_pro) : ?>
		<input type="button" value="Próximo" onclick="divCarregando();window.location='<?=$goto_pro ?>';">
		<? endif; ?>
		</td>
</tr>
</table>
</form>