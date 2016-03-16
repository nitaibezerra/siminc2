$(document).ready(function() {
	
	$('#voltar').click(function(){
		window.location = 'catalogocurso.php?modulo=principal/cadCatalogo&acao=A';
	});
	
	$('#proximo').click(function(){
		window.location = 'catalogocurso.php?modulo=principal/cadEquipe&acao=A';
	});
	
	$('#salvarC').click(function(){
		$('#link').val('proximo');
		$('#salvar').click();
	});
	
	$('#salvar').click(function(){
		
		$(this).attr('disabled',true);
		
		var erro = false;
		if($('#tioid').val()!=1){
			$('.obrigatorio').each(function(){
				if($(this).val() == ''){
					vazio = $(this);
					erro = true;
					return false;
				}
			});
		}
		
		if(erro){
			alert('Campo obrigatório.');
			vazio.focus();
			$(this).removeAttr('disabled');
			return false;
		}
		
		var orcchmim  = parseInt($('#orcchmim').val() );
		var orcchmax  = parseInt($('#orcchmax').val() );
		
		if( orcchmim > orcchmax ){
			alert('Campo MÍNIMO deve ser MENOR ou IGUAL a campo MÁXIMO');
			$('#orcchmim').focus();
			$(this).attr('disabled',false);
			return false;
		}
		
		$('#req').val('salvarOrganizacaoCurso');
		$(this).attr('disabled',false);
		$('#frmOrganizacao').submit();
	});
	
	$('#pesquisar').click(function(){
		$('#req').val('');
		$('#frmOrganizacao').submit();
	});
	
	$('#tioid').change(function(){
		if($(this).val()==1){
			bloquearCamposClasse('addClass');
			bloquearCampos(true);
		}else{
			bloquearCamposClasse('removeClass');
			bloquearCampos(false);
		}
	});
	if($('#tioid').val()=='1'||$('#gravar').val()!=1){
		bloquearCamposClasse('addClass');
		bloquearCampos(true);
	}else{
		bloquearCamposClasse('removeClass');
		bloquearCampos(false);
	}
//	
//	if($('#modidCur').val()=='1'){
//		$('#modid').attr('disabled',true);
//		$('#preexigida').hide();
//	}
	
	$('#modid').change(function(){
		
		if($(this).val()==1){
			$('#preexigida').hide();
		}else{
			$('#preexigida').show();
		}
	});
	
});

function bloquearCamposClasse( func ){
	jQuery.globalEval("$('#orcdesc')."+func+"('disabled');");
	jQuery.globalEval("$('#pacod_mod_ensino')."+func+"('disabled2');");
	jQuery.globalEval("$('#orcchmim')."+func+"('disabled');");
	jQuery.globalEval("$('#orcchmax')."+func+"('disabled');");
	jQuery.globalEval("$('#orcpercpremim')."+func+"('disabled');");
	jQuery.globalEval("$('#orcpercpremax')."+func+"('disabled');");
	jQuery.globalEval("$('#orcementa')."+func+"('disabled2');");
}

function bloquearCampos(val){modid
	$('#orcdesc').attr('disabled',val);
	$('#modid').attr('disabled',val);
	$('#pacod_mod_ensino').attr('disabled',val);
	$('#orcchmim').attr('disabled',val);
	$('#orcchmax').attr('disabled',val);
	$('#orcpercpremim').attr('disabled',val);
	$('#orcpercpremax').attr('disabled',val);
	$('#orcementa').attr('disabled',val);
}

function excluirOrganizacao( orcid ) {
	
	if( confirm( 'Realmente deseja excluir esta organização?' ) ){
		$('#req').val('excluirOrganizacaoCurso');
		$('#orcid').val(orcid);
		$('#frmOrganizacao').submit();
	}
}