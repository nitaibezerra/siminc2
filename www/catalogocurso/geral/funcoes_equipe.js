$(document).ready(function() {
	
	$('#voltar').click(function(){
		window.location = 'catalogocurso.php?modulo=principal/cadOrganizacaoCurso&acao=A';
	});
	
	$('#proximo').click(function(){
		window.location = 'catalogocurso.php?modulo=principal/cadPublicoAlvo&acao=A';
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
		
		var mim = parseInt($('#eqcminimo').val());
		var max = parseInt($('#eqcmaximo').val());
		
		if(mim>max){
			alert('Valores inválidos.');
			$('#eqcminimo').focus();
			return false;
		}
		$('#req').val('salvarEquipe');
		$('#frmEquipe').submit();
	});
	
	$('#pesquisar').click(function(){
		$('#req').val('');
		$('#frmEquipe').submit();
	});
	
	$('input[name$="cod_escolaridade[]"]').click(function(){
		if($(this).attr('checked')){
			$('.'+$(this).attr('id')).attr('checked',true);
		}
	});
	
	$('#camid').change(function(){
		//Se for equipe UAB
		if(!$('#gravar').val()){

			$('input[name$="curbolsista"]').attr('disabled',true);
			
			$('#qtdfuncao').attr('disabled',true);
			$('#qtdfuncao').removeClass('obrigatorio');
			
			$('#eqcfuncao').attr('disabled',true);
			$('#eqcfuncao').removeClass('obrigatorio');
			
			$('#eqcminimo').attr('disabled',true);
			$('#eqcminimo').removeClass('obrigatorio');
			
			$('#eqcmaximo').attr('disabled',true);
			$('#eqcmaximo').removeClass('obrigatorio');
			
			$('#unrid').attr('disabled',true);
			$('#unrid').removeClass('obrigatorio');
			
			$('input[name$="cod_escolaridade[]"]').attr('disabled',true);
			
			$('#eqcatribuicao').attr('disabled',true);
			
			$('#eqcoutrosreq').attr('disabled',true);
		}else if($(this).val()==8){
			
			$('input[name$="curbolsista"]').attr('disabled',true);
			$('input[name$="curbolsista"]').attr('checked',false);
			
			$('#qtdfuncao').val('');
			$('#qtdfuncao').attr('disabled',true);
			$('#qtdfuncao').removeClass('obrigatorio');
			
			$('#eqcfuncao').val('');
			$('#eqcfuncao').attr('disabled',true);
			$('#eqcfuncao').removeClass('obrigatorio');
			
			$('#eqcminimo').val('');
			$('#eqcminimo').attr('disabled',true);
			$('#eqcminimo').removeClass('obrigatorio');
			
			$('#eqcmaximo').val('');
			$('#eqcmaximo').attr('disabled',true);
			$('#eqcmaximo').removeClass('obrigatorio');
			
			$('#unrid').val('');
			$('#unrid').attr('disabled',true);
			$('#unrid').removeClass('obrigatorio');
			
			$('input[name$="cod_escolaridade[]"]').attr('disabled',true);
			$('input[name$="cod_escolaridade[]"]').attr('checked',false);
			
			$('#eqcatribuicao').val('');
			$('#eqcatribuicao').attr('disabled',true);
			
			$('#eqcoutrosreq').val('');
			$('#eqcoutrosreq').attr('disabled',true);
		}else{
			
			$('input[name$="curbolsista"]').attr('disabled',false);
			
			$('#qtdfuncao').attr('disabled',false);
			$('#qtdfuncao').addClass('obrigatorio');
			
			$('#eqcfuncao').attr('disabled',false);
			$('#eqcfuncao').addClass('obrigatorio');
			
			$('#eqcminimo').attr('disabled',false);
			$('#eqcminimo').addClass('obrigatorio');
			
			$('#eqcmaximo').attr('disabled',false);
			$('#eqcmaximo').addClass('obrigatorio');
			
			$('#unrid').attr('disabled',false);
			$('#unrid').addClass('obrigatorio');
			
			$('input[name$="cod_escolaridade[]"]').attr('disabled',false);
			
			$('#eqcatribuicao').attr('disabled',false);
			
			$('#eqcoutrosreq').attr('disabled',false);
		}
	});
	
	$('#camid').change();
});

function excluirEquipe( eqcid ) {
	
	if( confirm( 'Realmente deseja excluir esta equipe?' ) ){
		$('#req').val('excluirEquipe');
		$('#eqcid').val(eqcid);
		$('#frmEquipe').submit();
	}
}