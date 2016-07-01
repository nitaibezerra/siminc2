$(document).ready(function() {
	
	$('#voltar').click(function(){
		window.location = 'catalogocurso.php?modulo=principal/cadPublicoAlvo&acao=A';
	});
	
	$('#salvar').click(function(){
		
		var vazio;
		var erro = false;
		$('.obrigatorio').each(function(){
			if(trim($(this).val()) == ''){
				vazio = $(this);
				erro = true;
				return false;
			}
		});
		
		if(erro){
			alert('Campo obrigatório.');
			vazio.focus();
			return false;
		}
		
		var cont  = $('#curconttel').val();
		if(cont.length != '12' && cont != ''){
			alert('O Telefone deve estar no formato ##-####-####');
			$('#curconttel').focus();
			return false;
		}
		
		cont  = $('#curconttel2').val();
		if(cont.length != 12 && cont != ''){
			alert('O Telefone deve estar no formato ##-####-####');
			$('#curconttel2').focus();
			return false;
		}
		
		$('#req').val('salvarContato');
		$('#frmContato').submit();
	});
	
});
