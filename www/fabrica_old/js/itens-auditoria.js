var ItensAuditoria = {
	
	init : function(){
		$('#botaoPesquisarItensAuditoria').click(function( event ){
			event.preventDefault();
			ItensAuditoria.recupereItensAuditoria();
		});
		
		$('#botaoSalvarItensAuditoriaVisualizar, #botaoSalvarNovoItemAuditoria').click(function( event ){
			event.preventDefault();
			if ($('#form-itensAuditoria').valid()){
				ItensAuditoria.armazeneItemAuditoria();
			}
		});
		
		$('#form-itensAuditoria').validate({
		    errorElement: "label",
		    errorClass: "error",
		    rules:{
				itemAuditoria:"required"
		    },
		    messages:{
		    	itemAuditoria:"Campo Obrigatório"
		    }
		});
		
	},
	
	recupereItensAuditoria : function (){
		$.ajax({
			beforeSend: function(){
				$('#aguarde').css('visibility', 'visible');
				$('#aguarde').show();
			},
			type: 'post',
			url: 'geral/itens-auditoria/itensAuditoria.php',
			cache: false,
			dataType: 'html',
			data: $('form').serialize(),
			success: function(data){
				console.log(data);
				$('#grid').empty();
				$('#grid').html(data);
			},
			complete: function(){
				$('#aguarde').css('visibility', 'hidden');
				$('#aguarde').hide();
			}
		});
	},
	
	armazeneItemAuditoria : function (){
		$.ajax({
			beforeSend: function(){
				$('#aguarde').css('visibility', 'visible');
				$('#aguarde').show();
			},
			type: 'post',
			url: 'geral/itens-auditoria/armazeneItemAuditoria.php',
			cache: false,
			dataType: 'json',
			data: $('form').serialize(),
			success: function(data){
				$('#aguarde').css('visibility', 'hidden');
				$('#aguarde').hide();
				if (data.status){
					parent.location='?modulo=sistema/geral/itens-auditoria/listar&acao=A';
				} else {
					alert('Ocorreu um erro ao persistir no banco de dados.');
				}
			}
		});
	}
	
}

ItensAuditoria.init();