function vizualizarTermo(id)
{
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: "&req=vizualizaDocumento&dopid="+id,
   		async: false,
   		success: function(msg){
			$('#html_modal-form').html(msg);
			$('#html_modal-form').css('overflow', 'auto');
		    $('#modal-form').modal();
   		}
	});
}

function listaHistorico(id)
{
	alert(listaHistorico);
	return false;
	jQuery.ajax({
		type: "POST",
		url: window.location.href,
		data: "&req=listaHistorico&dopid="+id,
		async: false,
		success: function(msg){
			$('#html_modal-form').html(msg);
			$('#html_modal-form').css('overflow', 'auto');
			$('#modal-form').modal();
		}
	});
}

function baixarArquivoDopid(dopid)
{
	window.open(window.location.href+'&req=baixarArquivoDopid&dopid='+dopid,'Download','width=10, height=10');
//	$(window).attr('location',window.location.href+'&req=baixarArquivoDopid&dopid='+dopid);
}