$(document).ready(function() {
    function selectAllOptionsCatalogo( campo_select ){
        if ( !campo_select ){
            return;
        }
        var j = campo_select.options.length;
        for ( var i = 0; i < j; i++ ){
            campo_select.options[i].selected = true;
        }
    }

	$('#bt_salvar_perfil').click(function(){
	    selectAllOptionsCatalogo(document.getElementById('pk_cod_area_ocde'));
	    selectAllOptionsCatalogo(document.getElementById('pk_cod_disciplina'));
	    selectAllOptionsCatalogo(document.getElementById('pk_cod_etapa_ensino'));
	    selectAllOptionsCatalogo(document.getElementById('cod_etapa_ensino'));
	    pk_cod_area_ocde = $('#pk_cod_area_ocde').val();
	    pk_cod_disciplina = $('#pk_cod_disciplina').val();
	    pk_cod_etapa_ensino = $('#pk_cod_etapa_ensino').val();
	    cod_etapa_ensino = $('#cod_etapa_ensino').val();
	    $('#frmPerfil').submit();
	});	
	
	$('#bt_salvar_perfilcontinuar').click(function(){
		$('#linkp').val('proximo');
		$('#bt_salvar_perfil').click();
	});
	
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
		window.location = 'catalogocurso2014.php?modulo=principal/cadEquipe&acao=A';
	});
	
	$('#proximo').click(function(){
		window.location = 'catalogocurso2014.php?modulo=principal/cadIesOfertante&acao=A';
	});
	
	$('#salvarC').click(function(){
		$('#link').val('proximo');
		$('#salvar').click();
	});

	$('#salvar').click(function(){
        selectAllOptionsCatalogo(document.getElementById('padid'));
        selectAllOptionsCatalogo(document.getElementById('cod_etapa_ensino'));
        
    	var e = document.getElementsByName('cod_etapa_ensino[]')[0];
    	var etapas = '';		
    	
    	if (e.options.length > 0){
    		for (var i=0; i<e.options.length; i++){
	    		if (e.options[i].value != ''){
	    			etapas += "'" + e.options[i].value + "',";
	    		}
    		}      
    	}
    	
    	var p = document.getElementsByName('padid[]')[0];
    	var publico = '';		
    	
    	if (p.options.length > 0){
    		for (var i=0; i<p.options.length; i++){
	    		if (p.options[i].value != ''){
	    			publico += "'" + p.options[i].value + "',";
	    		}
    		}      
    	}    	
    		
		if(etapas == ''){
			alert('O campo "Etapa de ensino a que se destina" é obrigatório!');
			jQuery('#cod_etapa_ensino').focus();
			return false;
		}    		
        
		if(jQuery('[name=cursalamulti]:checked').length <= 0){
			alert('O campo "Sala de Recursos Multifuncionais" é obrigatório!');
			jQuery('[name=cursalamulti]').focus();
			return false;
		}	        

		if(jQuery('#lesid').val() == ''){
			alert('O campo "Localização da Escola" é obrigatório!');
			jQuery('#lesid').focus();
			return false;
		}				
		
		if(jQuery('#ldeid').val() == ''){
			alert('O campo "Localização Diferenciada da Escola" é obrigatório!');
			jQuery('#ldeid').focus();
			return false;
		}			
		
		if(jQuery('[name=curpademsocial]:checked').length <= 0){
			alert('O campo "Curso disponivel para demanda social?" é obrigatório!');
			jQuery('[name=curpademsocial]').focus();
			return false;
		}	      		
		
		if($('[name="curpademsocial"]:checked').val()=="S"){
			if(jQuery('#curpademsocialpercmax').val() == ''){
				alert('O campo "Percentual máximo de participantes na demanda social" é obrigatório!');
				jQuery('#curpademsocialpercmax').focus();
				return false;
			}
			
			if(publico == ''){
				alert('O campo "Público-alvo da demanda social" é obrigatório!');
				jQuery('#padid').focus();
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
	} else {
		$('.tr_demsoc').hide();
	}
});


function excluirPerfil(pafid){
    if(confirm('Deseja realmente excluir este item?')){
    	jQuery.ajax({
    		type: 'POST',
    		url: 'catalogocurso2014.php?modulo=principal/cadPublicoAlvo&acao=A',
    		data: { req: 'excluirPerfil', pafid: pafid},
    		async: false,
    		success: function(data) {
    			alert('Perfil excluído com sucesso!');
                setTimeout(function(){
                    window.location.href = 'catalogocurso2014.php?modulo=principal/cadPublicoAlvo&acao=A';
                }, 1500);
    	    }
    	});
    }
}

function editarPerfil(pafid){
    window.location.href = '/catalogocurso2014/catalogocurso2014.php?modulo=principal/cadPublicoAlvo&acao=A&pafid='+pafid;
}

$('.chosen-select').chosen({allow_single_deselect:true});

$('#tipo_lista').change(function(){
    carregarListaTipo();
});

$('#perfil').change(function(){
    carregarListaTipo();
});