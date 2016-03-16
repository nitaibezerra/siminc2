$(function() {
	
	oTable = $('.paginacao').dataTable({
		"iDisplayLength":20,
		"bLengthChange": false,
		"bFilter": false,
		"sPaginationType": "full_numbers",
		"bRetrieve": true,
		"oLanguage":
			{
				"sInfo": "Total de Registros: _TOTAL_",
				"sEmptyTable": "Não foram encontrados registros",
				"sInfoEmpty": "Total de Registros: 0",
		        "sZeroRecords": "Não foram encotrados registros",
				"oPaginate":
					{
						"sPrevious": "Paginas: &laquo;",
						"sNext": "&raquo;"
					}
	
			}
	});
	
});