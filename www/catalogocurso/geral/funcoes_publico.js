$(document).ready(function() {
	
	$('.booTutor').click(function(){
		if($(this).val()=='S'){
			$('#divTutor').show();
			$('#tutor').removeAttr('disabled');
		}else{
			$('#divTutor').hide();
			$('#tutor').attr('disabled','disabled');
		}
	});
	
	$('#voltar').click(function(){
		window.location = 'catalogocurso.php?modulo=principal/cadEquipe&acao=A';
	});
	
	$('#proximo').click(function(){
		window.location = 'catalogocurso.php?modulo=principal/cadContato&acao=A';
	});
	
	$('#salvarC').click(function(){
		$('#link').val('proximo');
		$('#salvar').click();
	});
	
	$('#salvar').click(function(){
		$('#req').val('salvarPublicoAlvo');
		selectAllOptions( document.getElementById('cod_area_ocde') );
		selectAllOptions( document.getElementById('cod_disciplina') );
		selectAllOptions( document.getElementById('cod_etapa_ensino') );
		selectAllOptions( document.getElementById('fexid') );
		selectAllOptions( document.getElementById('padid') );
		
		var vazio;
		var erro;
		var docente = false;
		$('#fexid option').each(function(){
			if($(this).val()==''){
				erro = true;
				vazio = $('#fexid');
			}
			if($(this).val()==1){
				docente = true
			}
		});
		if(erro){
			alert('Campo obrigatório.');
			vazio.focus();
			$(this).removeAttr('disabled');
			return false;
		}
		
		if( docente ){
			var test = false;
			erro = true;
			$('input[name$="cod_mod_ensino[]"]').each(function(){
				test = $(this).attr('checked');
				if(test){
					erro = false;
				}
			});
			if(erro){
				alert('Campo obrigatório.');
				$('input[name$="cod_mod_ensino[]"]').focus();
				$(this).removeAttr('disabled');
				return false;
			}
			erro = true;
			$('input[name$="curpademsocial"]').each(function(){
				test = $(this).attr('checked');
				if(test){
					erro = false;
				}
			});
			if(erro){
				alert('Campo obrigatório.');
				$('input[name$="curpademsocial"]').focus();
				$(this).removeAttr('disabled');
				return false;
			}
			erro = false;
			$('.obrigatorio').each(function(){
				if($(this).val() == ''){
					vazio = $(this);
					erro = true;
					return false;
				}
			});
			$('#cod_etapa_ensino option').each(function(){
				if($(this).val()==''){
					erro = true;
					vazio = $('#cod_etapa_ensino');
				}
			});
			$('#cod_disciplina option').each(function(){
				if($(this).val()==''){
					erro = true;
					vazio = $('#cod_disciplina');
				}
			});
			if( !$('#cod_area_ocde').attr('disabled') ){
				$('#cod_area_ocde option').each(function(){
					if($(this).val()==''){
						erro = true;
						vazio = $('#cod_area_ocde');
					}
				});
			}
			if( $('.tr_demsoc').css('display') != 'none' ){
				if( $('#curpademsocialpercmax').val() == '' ){
					alert('Campo obrigatório.');
					$('#curpademsocialpercmax').focus();
					return false;
				}
				$('#padid option').each(function(){
					if($(this).val()==''){
						erro = true;
						vazio = $('#padid');
					}
				});
			}
			
			if(erro){
				alert('Campo obrigatório.');
				vazio.focus();
				$(this).removeAttr('disabled');
				return false;
			}
		}
		
		
		$('#frmPublicoAlvo').submit();
	});
	
	$('input[name$="cod_escolaridade[]"]').click(function(){
		if($(this).attr('checked')){
			$('.'+$(this).attr('id')).attr('checked',true);
		}
	});
	
	$('#camid').change();
	
	$('#curpademsocialpercmax').keyup(function(){
		
		if( parseInt($(this).val()) > 100 ){
			$(this).val('100');
		}
	});
	
	$('[name="curpademsocial"]').click(function(){
		if($(this).val()=="S"){
			$('.tr_demsoc').show();
		}else{
			$('.tr_demsoc').hide();
		}
	});
	
	if($('[name="curpademsocial"]:checked').val()=="S"){
		$('.tr_demsoc').show();
	}else{
		$('.tr_demsoc').hide();
	}
});