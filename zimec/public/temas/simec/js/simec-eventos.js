function attachEvents() {
	$('.cpf').change(function(e) {
    	e.preventDefault();
    	if ($(this).data('pessoa') && $(this).data('pessoa-campos')) {
    		var self = $(this);
        	$.ajax({
    			url: '/includes/webservice/cpf.php',
    			data: {'ajaxCPF' : $(this).val()},
    			method: 'post',
    			success: function (result) {
    				var unparsed = '{"' + result.replace(/\|/g, '","').replace(/#/g, '":"') + '"}';
    				var pessoa = JSON.parse(unparsed);
    				var campos = self.data('pessoa-campos'); 

    				if (pessoa && campos) {
    					for (var i in campos) {
    						$('#' + i).val(pessoa[campos[i]]);
    					}
    				}
    			}
    		});
    	}
    });
}

$(document).ready(function() {
	attachEvents();
})