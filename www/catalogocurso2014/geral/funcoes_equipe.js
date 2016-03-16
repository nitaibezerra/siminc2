$(document).ready(function() {
	$('#voltar').click(function(){
		window.location = 'catalogocurso2014.php?modulo=principal/cadOrganizacaoCurso&acao=A';
	});
	
	$('#proximo').click(function(){
		window.location = 'catalogocurso2014.php?modulo=principal/cadPublicoAlvo&acao=A';
	});
	
	$('#salvarC').click(function(){
		$('#link').val('proximo');
		$('#salvar').click();
	});
	
	$('#pesquisar').click(function(){
		$('#req').val('');
		$('#frmEquipe').submit();
	});
});

function excluirEquipe(eqcid) {
	if(confirm('Realmente deseja excluir esta equipe?')){
		$('#req').val('excluirEquipe');
		$('#eqcid').val(eqcid);
		$('#frmEquipe').submit();
	}
}

function carregarEquipeAtribuicaoEscolaridade(fueid){
	jQuery.ajax({
		type: 'POST',
		url: 'catalogocurso2014.php?modulo=principal/cadEquipe&acao=A',
		data: {req:'carregarEquipeAtribuicao', fueid: fueid},
		dataType: 'json',
		async: false,
		success: function(data){
			if(data.funcao == 'N'){
				jQuery('#eqcatribuicao').val('');
				jQuery('#divExperiencia').html('Selecione a Função.');	
			} else {
				jQuery('#eqcatribuicao').val(data.fueatribuicao);
				jQuery('#divExperiencia').html(data.fueexperiencia);
			}
	    }
	});
	jQuery.ajax({
		type: 'POST',
		url: 'catalogocurso2014.php?modulo=principal/cadEquipe&acao=A',
		data: {req:'carregarEquipeEscolaridade', fueid: fueid},
		async: false,
		success: function(data){
			jQuery('#divNivelEscolaridade').html(data);
	    }
	});	
}