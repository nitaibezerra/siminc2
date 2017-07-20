function attachEvents() {
	$('.cpf').change(function(e) {
    	e.preventDefault();
    	if ($(this).data('pessoa') && $(this).data('pessoa-campos') && $(this).val() != '') {
    		var self = $(this);
        	$.ajax({
    			url: '/includes/webservice/cpf.php',
    			data: {'ajaxCPF' : $(this).val()},
    			method: 'post',
    			success: function (result) {
    				if(result != ''){
    					var unparsed = '{"' + result.replace(/\|/g, '","').replace(/#/g, '":"') + '"}';
    					var pessoa = JSON.parse(unparsed);
    					var campos = self.data('pessoa-campos'); 
    					
    					if (pessoa && campos) {
    						for (var i in campos) {
    							$(i).val(pessoa[campos[i]]);
    						}
    					}
    				}else{
    					swal('Erro', 'CPF não encontrado na Receita Federal!', 'error');
    				}
    			}
    		});
    	}
    });
	
	
	$('.cnpj').change(function(e) {
    	e.preventDefault();
    	if ($(this).data('pessoa') && $(this).data('pessoa-campos') && $(this).val() != '') {
    		var self = $(this);
        	$.ajax({
    			url: '/includes/webservice/pj.php',
    			data: {'ajaxPJ' : $(this).val()},
    			method: 'post',
    			success: function (result) {
    				if(result != ''){
	    				var unparsed = '{"' + result.replace(/\$/g, '|').replace(/#/g, '":"').replace(/\|/g, '","').replace(/,",/g, ',"').replace(/,"",/g, ',') + '"}';
	    				var pessoa = JSON.parse(unparsed);
	    				var campos = self.data('pessoa-campos'); 
	
	    				if (pessoa && campos) {
	    					for (var i in campos) {
	    						$(i).val(pessoa[campos[i]]);
	    					}
	    				}
					}else{
						swal('Erro', 'CPF não encontrado na Receita Federal!', 'error');
					}
    			}
    		});
    	}
    });
}

$(document).ready(function() {
	attachEvents();
})