$(document).ready(function() {
	
	/*===========================================================================================
	 * CRÉDITO ORÇAMENTÁRIO
	 *===========================================================================================
	 */	
	$('.scrollCreditoOrc').fixedHeaderTable();
	
	$('.scrollCreditoOrc').find('table tbody tr')
	.live('mouseover',function(){
		$('td', this).addClass('highlight');
	})
	.live('mouseout',function(){
		$('td', this).removeClass('highlight');
	});
	
	$('.alterarCreditoOrc').live('click', function() {
		var tr 		 = $(this).closest('tr');
		var rowIndexCreditoOrc = tr.prevAll().length;
		
		var cdoid = $(this).attr('id');
		var param = new Array();
			param.push({name : 'requisicao', value : 'carregarCreditoOrc'}, 
					   {name : 'cdoid', value : cdoid}
			);
		$.ajax({
			type	 : "POST",
			url		 : "academico.php?modulo=principal/receitaDespesa&acao=A",
			data	 : param,
			async    : false,
			dataType : 'json',
			success	 : function(data){
							//$('#divDebug').html(data);
							$('#rowIndexCreditoOrc').val(rowIndexCreditoOrc);
							$('#cdoid').val(cdoid);
							$('#ndpidCreditoOrc').val(data.ndpidCreditoOrc);
							$('#ndpidCreditoOrc_dsc').val(data.ndpidCreditoOrc_dsc);
							$('#cdovalor').val(data.cdovalor);
					   }
			 });
	});
	
	$('.excluirCreditoOrc').live('click', function() {
		if(!confirm('Deseja realmente excluir este registro.')){
			return false;
		}
		
		var tr = $(this).closest('tr');
		
		var param = new Array();
			param.push({name : 'requisicao', value : 'excluirCreditoOrc'}, 
					   {name : 'cdoid', value : $(this).attr('id')}
			);
		$.ajax({
			type	 : "POST",
			url		 : "academico.php?modulo=principal/receitaDespesa&acao=A",
			data	 : param,
			async    : false,
			//dataType : 'json',
			success	 : function(data){
							if(trim(data) == 'sucesso'){
								tr.remove();
								calcularTotal('CreditoOrc');
								//alert('Operação realizada com sucesso.');
							}
					   }
			 });
	});
	
	$('#salvarCreditoOrc').click(function() {
		var msg = "";
		if($('#ndpcodCreditoOrc').val() == ''){
			msg += "O campo Natureza é obrigatório.\n";
		}
		if($('#cdovalor').val() == ''){
			msg += "O campo Valor é obrigatório.";
		}
		if(msg){
			alert(msg);
			return false;
		}
		
		var param = new Array();
			param.push({name : 'requisicao', value : 'salvarCreditoOrc'}, 
					   {name : 'rowIndexCreditoOrc', value : $('#rowIndexCreditoOrc').val()},
					   {name : 'cdoid', value : $('#cdoid').val()},
					   {name : 'prcidCreditoOrc', value : $('#prcidCreditoOrc').val()},
					   {name : 'ndpcodCreditoOrc', value : $('#ndpcodCreditoOrc').val()},
					   {name : 'cdovalor', value : $('#cdovalor').val()}
			);	
		
		jQuery.ajax({
			type	 : "POST",
			url		 : "academico.php?modulo=principal/receitaDespesa&acao=A",
			data	 : param,
			async    : false,
			dataType : 'json',
			success	 : function(data){
				if(data.msg == 'sucesso'){
					//$('#divDebug').html(data);
					// Verificamos se é alteração
					if(parseInt(data.rowIndexCreditoOrc) >= 0 ){
						var tr = $('#trCreditoOrc_'+parseInt(data.rowIndexCreditoOrc));
						
						var classe = "";
						if(tr.find("td:first").hasClass("even")){
							classe = 'even';
						} else {
							classe = 'odd';
						}
						
						var html = '<tr id="trCreditoOrc_'+parseInt(data.rowIndexCreditoOrc)+'" class="trCreditoOrc">';
							html +='	<td class="'+classe+' borderRight firstCell"><div class="tableData" style="width: 133px;"><p class="tableData">'+data.ndpcod+'</p></div></td>';
							html +='	<td class="'+classe+' borderRight"><div class="tableData" style="width: 119px;"><p class="tableData">'+data.cdovalor+'</p></div></td>';
							html +='	<td class="'+classe+'"><div class="tableData" style="width: 44px;"><p class="tableData"><img border="0" class="alterarCreditoOrc" id="'+data.cdoid+'" style="cursor: pointer;" title="Alterar" src="/imagens/alterar.gif">&nbsp;<img border="0" id="'+data.cdoid+'" class="excluirCreditoOrc" title="Excluir" style="cursor: pointer;" src="/imagens/excluir.gif"></p></div></td>';
							html +='</tr>';
						
						tr.after(html).remove();
					} else {
						var rowIndexCreditoOrc = 0;
						if(jQuery('#tbodyCreditoOrc').children().length > 0){
							var rowIndexCreditoOrc = jQuery('#tbodyCreditoOrc').children().length + 1;
						}
						var tr = jQuery('#tbodyCreditoOrc').find('tr:last');
						
						var classe = "";
						if(tr.find("td:first").hasClass("even")){
							classe = 'even';
						} else {
							classe = 'odd';
						}
						
						var html = '<tr id="trCreditoOrc_'+parseInt(rowIndexCreditoOrc)+'" class="trCreditoOrc">';
							html +='	<td class="'+classe+' borderRight firstCell"><div class="tableData" style="width: 133px;"><p class="tableData">'+data.ndpcod+'</p></div></td>';
							html +='	<td class="'+classe+' borderRight"><div class="tableData" style="width: 119px;"><p class="tableData">'+data.cdovalor+'</p></div></td>';
							html +='	<td class="'+classe+'"><div class="tableData" style="width: 44px;"><p class="tableData"><img border="0" class="alterarCreditoOrc" id="'+data.cdoid+'" style="cursor: pointer;" title="Alterar" src="/imagens/alterar.gif">&nbsp;<img border="0" id="'+data.cdoid+'" class="excluirCreditoOrc" title="Excluir" style="cursor: pointer;" src="/imagens/excluir.gif"></p></div></td>';
							html +='</tr>';
						
						if(rowIndexCreditoOrc > 0){
							tr.after(html);					
						} else {
							jQuery('#tbodyCreditoOrc').append(html);
						}
					}
					calcularTotal('CreditoOrc');
					//alert('Dados gravado com sucesso.');
				} else if(data.msg == 'erro') {
					alert('Ocorreu algum erro.');						
				}
			}
		});
		
		limparCampos('cdo','CreditoOrc');
		
	});
	
	calcularTotal('CreditoOrc');
	
	/*===========================================================================================
	 * CRÉDITO ORÇAMENTÁRIO
	 *===========================================================================================
	 */	
	
	/*===========================================================================================
	 * DESPESA ORÇAMENTÁRIA
	 *===========================================================================================
	 */	
	
	$('.scrollDespesaOrc').fixedHeaderTable();

	$('.scrollDespesaOrc').find('table tbody tr')
		.live('mouseover',function(){
			$('td', this).addClass('highlight');
	    })
	    .live('mouseout',function(){
	    	$('td', this).removeClass('highlight');
	    });
    
	$('.alterarDespesaOrc').live('click', function() {
		var tr 		 = $(this).closest('tr');
		var rowIndexDespesaOrc = tr.prevAll().length;
		
		var dpoid = $(this).attr('id');
		var param = new Array();
			param.push({name : 'requisicao', value : 'carregarDespesaOrc'}, 
					   {name : 'dpoid', value : dpoid}
		);
			
		$.ajax({
			type	 : "POST",
			url		 : "academico.php?modulo=principal/receitaDespesa&acao=A",
			data	 : param,
			async    : false,
			dataType : 'json',
			success	 : function(data){
							$('#rowIndexDespesaOrc').val(rowIndexDespesaOrc);
							$('#dpoid').val(dpoid);
							$('#ndpidDespesaOrc').val(data.ndpidDespesaOrc);
							$('#ndpidDespesaOrc_dsc').val(data.ndpidDespesaOrc_dsc);
							$('#dpovalor').val(data.dpovalor);
						}
		});
	});
	
	$('.excluirDespesaOrc').live('click', function() {
		if(!confirm('Deseja realmente excluir este registro.')){
			return false;
		}
		
		var tr = $(this).closest('tr');
		
		var param = new Array();
			param.push({name : 'requisicao', value : 'excluirDespesaOrc'}, 
					   {name : 'dpoid', value : $(this).attr('id')}
			);
		$.ajax({
			type	 : "POST",
			url		 : "academico.php?modulo=principal/receitaDespesa&acao=A",
			data	 : param,
			async    : false,
			//dataType : 'json',
			success	 : function(data){
							if(trim(data) == 'sucesso'){
								tr.remove();
								calcularTotal('DespesaOrc');
								//alert('Operação realizada com sucesso.');
							}
					   }
			 });
	});
	
	$('#salvarDespesaOrc').click(function() {
		var msg = "";
		if($('#ndpidDespesaOrc').val() == ''){
			msg += "O campo Natureza é obrigatório.\n";
		}
		if($('#dpovalor').val() == ''){
			msg += "O campo Valor é obrigatório.";
		}
		if(msg){
			alert(msg);
			return false;
		}
		
		var param = new Array();
		param.push({name : 'requisicao', value : 'salvarDespesaOrc'}, 
				   {name : 'rowIndexDespesaOrc', value : $('#rowIndexDespesaOrc').val()},
				   {name : 'dpoid', value : $('#dpoid').val()},
				   {name : 'prcidDespesaOrc', value : $('#prcidDespesaOrc').val()},
				   {name : 'ndpcodDespesaOrc', value : $('#ndpcodDespesaOrc').val()},
				   {name : 'dpovalor', value : $('#dpovalor').val()}
				  );	

		jQuery.ajax({
			   type		: "POST",
			   url		: "academico.php?modulo=principal/receitaDespesa&acao=A",
			   data		: param,
			   async    : false,
			   dataType: 'json',
			   success	: function(data){
					//$('#divDebug').html(data); return false;
					if(data.msg == 'sucesso'){
						// Verificamos se é alteração
						if(parseInt(data.rowIndexDespesaOrc) >= 0 ){
							var tr = $('#trDespesaOrc_'+parseInt(data.rowIndexDespesaOrc));
							
							var classe = "";
							if(tr.find("td:first").hasClass("even")){
								classe = 'even';
							} else {
								classe = 'odd';
							}
							
							var html = '<tr id="trDespesaOrc_'+parseInt(data.rowIndexDespesaOrc)+'" class="trDespesaOrc">';
								html +='	<td class="'+classe+' borderRight firstCell"><div class="tableData" style="width: 133px;"><p class="tableData">'+data.ndpcod+'</p></div></td>';
								html +='	<td class="'+classe+' borderRight"><div class="tableData" style="width: 119px;"><p class="tableData">'+data.dpovalor+'</p></div></td>';
								html +='	<td class="'+classe+'"><div class="tableData" style="width: 44px;"><p class="tableData"><img border="0" class="alterarDespesaOrc" id="'+data.dpoid+'" style="cursor: pointer;" title="Alterar" src="/imagens/alterar.gif">&nbsp;<img border="0" id="'+data.dpoid+'" class="excluirDespesaOrc" title="Excluir" style="cursor: pointer;" src="/imagens/excluir.gif"></p></div></td>';
								html +='</tr>';
							
							tr.after(html).remove();
						} else {
							var rowIndexDespesaOrc = 0;
							if(jQuery('#tbodyDespesaOrc').children().length > 0){
								var rowIndexDespesaOrc = jQuery('#tbodyDespesaOrc').children().length + 1;								
							}
							
							var tr = jQuery('#tbodyDespesaOrc').find('tr:last');
							
							var classe = "";
							if(tr.find("td:first").hasClass("even")){
								classe = 'even';
							} else {
								classe = 'odd';
							}
							
							var html = '<tr id="trDespesaOrc_'+parseInt(rowIndexDespesaOrc)+'" class="trDespesaOrc">';
								html +='	<td class="'+classe+' borderRight firstCell"><div class="tableData" style="width: 133px;"><p class="tableData">'+data.ndpcod+'</p></div></td>';
								html +='	<td class="'+classe+' borderRight"><div class="tableData" style="width: 119px;"><p class="tableData">'+data.dpovalor+'</p></div></td>';
								html +='	<td class="'+classe+'"><div class="tableData" style="width: 44px;"><p class="tableData"><img border="0" class="alterarDespesaOrc" id="'+data.dpoid+'" style="cursor: pointer;" title="Alterar" src="/imagens/alterar.gif">&nbsp;<img border="0" id="'+data.dpoid+'" class="excluirDespesaOrc" title="Excluir" style="cursor: pointer;" src="/imagens/excluir.gif"></p></div></td>';
								html +='</tr>';
							
							if(rowIndexDespesaOrc > 0){
								tr.after(html);
							} else {
								jQuery('#tbodyDespesaOrc').append(html);								
							}
								
						}
						calcularTotal('DespesaOrc');
						//alert('Dados gravado com sucesso.');
					} else if(data.msg == 'erro') {
						alert('Ocorreu algum erro.');						
					}
				}
		});
		
		limparCampos('dpo','DespesaOrc');		
		
	});
	
	calcularTotal('DespesaOrc');
	
	/*===========================================================================================
	 * DESPESA ORÇAMENTÁRIA
	 *===========================================================================================
	 */
	
	
	/*===========================================================================================
	 * RECURSO FINANCEIRO CONCEDENTE
	 *===========================================================================================
	 */	
	$('.scrollRecursoFinConc').fixedHeaderTable();
	
	$('.scrollRecursoFinConc').find('table tbody tr')
	.live('mouseover',function(){
		$('td', this).addClass('highlight');
	})
	.live('mouseout',function(){
		$('td', this).removeClass('highlight');
	});
	
	$('.alterarRecursoFinConc').live('click', function() {
		var tr 		 = $(this).closest('tr');
		var rowIndexRecursoFinConc = tr.prevAll().length;
		
		var rfcid = $(this).attr('id');
		var param = new Array();
			param.push({name : 'requisicao', value : 'carregarRecursoFinConc'}, 
					   {name : 'rfcid', value : rfcid}
			);
		$.ajax({
			type	 : "POST",
			url		 : "academico.php?modulo=principal/receitaDespesa&acao=A",
			data	 : param,
			async    : false,
			dataType : 'json',
			success	 : function(data){
							//$('#divDebug').html(data);
							$('#rowIndexRecursoFinConc').val(rowIndexRecursoFinConc);
							$('#rfcid').val(rfcid);
							$('#ndpidRecursoFinConc').val(data.ndpidRecursoFinConc);
							$('#ndpidRecursoFinConc_dsc').val(data.ndpidRecursoFinConc_dsc);
							$('#rfcvalor').val(data.rfcvalor);
					   }
			 });
	});
	
	$('.excluirRecursoFinConc').live('click', function() {
		if(!confirm('Deseja realmente excluir este registro.')){
			return false;
		}
		
		var tr = $(this).closest('tr');		
		
		var param = new Array();
			param.push({name : 'requisicao', value : 'excluirRecursoFinConc'}, 
					   {name : 'rfcid', value : $(this).attr('id')}
			);
		$.ajax({
			type	 : "POST",
			url		 : "academico.php?modulo=principal/receitaDespesa&acao=A",
			data	 : param,
			async    : false,
			//dataType : 'json',
			success	 : function(data){
							if(trim(data) == 'sucesso'){
								tr.remove();
								calcularTotal('RecursoFinConc');
								//alert('Operação realizada com sucesso.');
							}
					   }
			 });
	});
	
	$('#salvarRecursoFinConc').click(function() {
		var msg = "";
		if($('#ndpidRecursoFinConc').val() == ''){
			msg += "O campo Natureza é obrigatório.\n";
		}
		if($('#rfcvalor').val() == ''){
			msg += "O campo Valor é obrigatório.";
		}
		if(msg){
			alert(msg);
			return false;
		}
		
		var param = new Array();
			param.push({name : 'requisicao', value : 'salvarRecursoFinConc'}, 
					   {name : 'rowIndexRecursoFinConc', value : $('#rowIndexRecursoFinConc').val()},
					   {name : 'rfcid', value : $('#rfcid').val()},
					   {name : 'prcidRecursoFinConc', value : $('#prcidRecursoFinConc').val()},
					   {name : 'ndpcodRecursoFinConc', value : $('#ndpcodRecursoFinConc').val()},
					   {name : 'rfcvalor', value : $('#rfcvalor').val()}
			);	
		
		jQuery.ajax({
			type	 : "POST",
			url		 : "academico.php?modulo=principal/receitaDespesa&acao=A",
			data	 : param,
			async    : false,
			dataType : 'json',
			success	 : function(data){
				if(data.msg == 'sucesso'){
					//$('#divDebug').html(data);
					// Verificamos se é alteração
					if(parseInt(data.rowIndexRecursoFinConc) >= 0 ){
						var tr = $('#trRecursoFinConc_'+parseInt(data.rowIndexRecursoFinConc));
						
						var classe = "";
						if(tr.find("td:first").hasClass("even")){
							classe = 'even';
						} else {
							classe = 'odd';
						}
						
						var html = '<tr id="trRecursoFinConc_'+parseInt(data.rowIndexRecursoFinConc)+'" class="trRecursoFinConc">';
							html +='	<td class="'+classe+' borderRight firstCell"><div class="tableData" style="width: 133px;"><p class="tableData">'+data.ndpcod+'</p></div></td>';
							html +='	<td class="'+classe+' borderRight"><div class="tableData" style="width: 119px;"><p class="tableData">'+data.rfcvalor+'</p></div></td>';
							html +='	<td class="'+classe+'"><div class="tableData" style="width: 44px;"><p class="tableData"><img border="0" class="alterarRecursoFinConc" id="'+data.rfcid+'" style="cursor: pointer;" title="Alterar" src="/imagens/alterar.gif">&nbsp;<img border="0" id="'+data.rfcid+'" class="excluirRecursoFinConc" title="Excluir" style="cursor: pointer;" src="/imagens/excluir.gif"></p></div></td>';
							html +='</tr>';
						
						tr.after(html).remove();
					} else {
						var rowIndexRecursoFinConc = 0;
						if(jQuery('#tbodyRecursoFinConc').children().length > 0){
							var rowIndexRecursoFinConc = jQuery('#tbodyRecursoFinConc').children().length + 1;
						}
						var tr = jQuery('#tbodyRecursoFinConc').find('tr:last');
						
						var classe = "";
						if(tr.find("td:first").hasClass("even")){
							classe = 'even';
						} else {
							classe = 'odd';
						}
						
						var html = '<tr id="trRecursoFinConc_'+parseInt(rowIndexRecursoFinConc)+'" class="trRecursoFinConc">';
							html +='	<td class="'+classe+' borderRight firstCell"><div class="tableData" style="width: 133px;"><p class="tableData">'+data.ndpcod+'</p></div></td>';
							html +='	<td class="'+classe+' borderRight"><div class="tableData" style="width: 119px;"><p class="tableData">'+data.rfcvalor+'</p></div></td>';
							html +='	<td class="'+classe+'"><div class="tableData" style="width: 44px;"><p class="tableData"><img border="0" class="alterarRecursoFinConc" id="'+data.rfcid+'" style="cursor: pointer;" title="Alterar" src="/imagens/alterar.gif">&nbsp;<img border="0" id="'+data.rfcid+'" class="excluirRecursoFinConc" title="Excluir" style="cursor: pointer;" src="/imagens/excluir.gif"></p></div></td>';
							html +='</tr>';
						
						if(rowIndexRecursoFinConc > 0){
							tr.after(html);					
						} else {
							jQuery('#tbodyRecursoFinConc').append(html);
						}
					}
					calcularTotal('RecursoFinConc');
					//alert('Dados gravado com sucesso.');						
				} else if(data.msg == 'erro') {
					alert('Ocorreu algum erro.');						
				}
			}
		});
		
		limparCampos('rfc','RecursoFinConc');
		
	});
	
	calcularTotal('RecursoFinConc');
	
	/*===========================================================================================
	 * RECURSO FINANCEIRO CONCEDENTE
	 *===========================================================================================
	 */
	
	/*===========================================================================================
	 * DESPESA FINANCEIRA CONCEDENTE
	 *===========================================================================================
	 */	
	$('.scrollDespFinanConc').fixedHeaderTable();
	
	$('.scrollDespFinanConc').find('table tbody tr')
	.live('mouseover',function(){
		$('td', this).addClass('highlight');
	})
	.live('mouseout',function(){
		$('td', this).removeClass('highlight');
	});
	
	$('.alterarDespFinanConc').live('click', function() {
		var tr 		 = $(this).closest('tr');
		var rowIndexDespFinanConc = tr.prevAll().length;
		
		var dfcid = $(this).attr('id');
		var param = new Array();
			param.push({name : 'requisicao', value : 'carregarDespFinanConc'}, 
					   {name : 'dfcid', value : dfcid}
			);
		$.ajax({
			type	 : "POST",
			url		 : "academico.php?modulo=principal/receitaDespesa&acao=A",
			data	 : param,
			async    : false,
			dataType : 'json',
			success	 : function(data){
							//$('#divDebug').html(data);
							$('#rowIndexDespFinanConc').val(rowIndexDespFinanConc);
							$('#dfcid').val(dfcid);
							$('#ndpidDespFinanConc').val(data.ndpidDespFinanConc);
							$('#ndpidDespFinanConc_dsc').val(data.ndpidDespFinanConc_dsc);
							$('#dfcvalor').val(data.dfcvalor);
					   }
			 });
	});
	
	$('.excluirDespFinanConc').live('click', function() {
		if(!confirm('Deseja realmente excluir este registro.')){
			return false;
		}
		
		var tr = $(this).closest('tr');
		
		var param = new Array();
			param.push({name : 'requisicao', value : 'excluirDespFinanConc'}, 
					   {name : 'dfcid', value : $(this).attr('id')}
			);
		$.ajax({
			type	 : "POST",
			url		 : "academico.php?modulo=principal/receitaDespesa&acao=A",
			data	 : param,
			async    : false,
			//dataType : 'json',
			success	 : function(data){
							if(trim(data) == 'sucesso'){
								tr.remove();
								calcularTotal('DespFinanConc');
								//alert('Operação realizada com sucesso.');
							}
					   }
			 });
	});
	
	$('#salvarDespFinanConc').click(function() {
		var msg = "";
		if($('#ndpidDespFinanConc').val() == ''){
			msg += "O campo Natureza é obrigatório.\n";
		}
		if($('#dfcvalor').val() == ''){
			msg += "O campo Valor é obrigatório.";
		}
		if(msg){
			alert(msg);
			return false;
		}
		
		var param = new Array();
			param.push({name : 'requisicao', value : 'salvarDespFinanConc'}, 
					   {name : 'rowIndexDespFinanConc', value : $('#rowIndexDespFinanConc').val()},
					   {name : 'dfcid', value : $('#dfcid').val()},
					   {name : 'prcidDespFinanConc', value : $('#prcidDespFinanConc').val()},
					   {name : 'ndpcodDespFinanConc', value : $('#ndpcodDespFinanConc').val()},
					   {name : 'dfcvalor', value : $('#dfcvalor').val()}
			);	
		
		jQuery.ajax({
			type	 : "POST",
			url		 : "academico.php?modulo=principal/receitaDespesa&acao=A",
			data	 : param,
			async    : false,
			dataType : 'json',
			success	 : function(data){
				if(data.msg == 'sucesso'){
					//$('#divDebug').html(data);
					// Verificamos se é alteração
					if(parseInt(data.rowIndexDespFinanConc) >= 0 ){
						var tr = $('#trDespFinanConc_'+parseInt(data.rowIndexDespFinanConc));
						
						var classe = "";
						if(tr.find("td:first").hasClass("even")){
							classe = 'even';
						} else {
							classe = 'odd';
						}
						
						var html = '<tr id="trDespFinanConc_'+parseInt(data.rowIndexDespFinanConc)+'" class="trDespFinanConc">';
							html +='	<td class="'+classe+' borderRight firstCell"><div class="tableData" style="width: 133px;"><p class="tableData">'+data.ndpcod+'</p></div></td>';
							html +='	<td class="'+classe+' borderRight"><div class="tableData" style="width: 119px;"><p class="tableData">'+data.dfcvalor+'</p></div></td>';
							html +='	<td class="'+classe+'"><div class="tableData" style="width: 44px;"><p class="tableData"><img border="0" class="alterarDespFinanConc" id="'+data.dfcid+'" style="cursor: pointer;" title="Alterar" src="/imagens/alterar.gif">&nbsp;<img border="0" id="'+data.dfcid+'" class="excluirDespFinanConc" title="Excluir" style="cursor: pointer;" src="/imagens/excluir.gif"></p></div></td>';
							html +='</tr>';
						
						tr.after(html).remove();
					} else {
						var rowIndexDespFinanConc = 0;
						if(jQuery('#tbodyDespFinanConc').children().length > 0){
							var rowIndexDespFinanConc = jQuery('#tbodyDespFinanConc').children().length + 1;
						}
						var tr = jQuery('#tbodyDespFinanConc').find('tr:last');
						
						var classe = "";
						if(tr.find("td:first").hasClass("even")){
							classe = 'even';
						} else {
							classe = 'odd';
						}
						
						var html = '<tr id="trDespFinanConc_'+parseInt(rowIndexDespFinanConc)+'" class="trDespFinanConc">';
							html +='	<td class="'+classe+' borderRight firstCell"><div class="tableData" style="width: 133px;"><p class="tableData">'+data.ndpcod+'</p></div></td>';
							html +='	<td class="'+classe+' borderRight"><div class="tableData" style="width: 119px;"><p class="tableData">'+data.dfcvalor+'</p></div></td>';
							html +='	<td class="'+classe+'"><div class="tableData" style="width: 44px;"><p class="tableData"><img border="0" class="alterarDespFinanConc" id="'+data.dfcid+'" style="cursor: pointer;" title="Alterar" src="/imagens/alterar.gif">&nbsp;<img border="0" id="'+data.dfcid+'" class="excluirDespFinanConc" title="Excluir" style="cursor: pointer;" src="/imagens/excluir.gif"></p></div></td>';
							html +='</tr>';
						
						if(rowIndexDespFinanConc > 0){
							tr.after(html);					
						} else {
							jQuery('#tbodyDespFinanConc').append(html);
						}
					}
					calcularTotal('DespFinanConc');
					//alert('Dados gravado com sucesso.');						
				} else if(data.msg == 'erro') {
					alert('Ocorreu algum erro.');						
				}
			}
		});
		
		limparCampos('dfc','DespFinanConc');
		
	});
	
	calcularTotal('DespFinanConc');
	
	/*===========================================================================================
	 * DESPESA FINANCEIRA CONCEDENTE
	 *===========================================================================================
	 */
	
	/*===========================================================================================
	 * RECURSO FINANCEIRO PRÓPRIO
	 *===========================================================================================
	 */	
	$('.scrollRecursoFinanPro').fixedHeaderTable();
	
	$('.scrollRecursoFinanPro').find('table tbody tr')
	.live('mouseover',function(){
		$('td', this).addClass('highlight');
	})
	.live('mouseout',function(){
		$('td', this).removeClass('highlight');
	});
	
	$('.alterarRecursoFinanPro').live('click', function() {
		var tr 		 = $(this).closest('tr');
		var rowIndexRecursoFinanPro = tr.prevAll().length;
		
		var rfpid = $(this).attr('id');
		var param = new Array();
			param.push({name : 'requisicao', value : 'carregarRecursoFinanPro'}, 
					   {name : 'rfpid', value : rfpid}
			);
		$.ajax({
			type	 : "POST",
			url		 : "academico.php?modulo=principal/receitaDespesa&acao=A",
			data	 : param,
			async    : false,
			dataType : 'json',
			success	 : function(data){
							//$('#divDebug').html(data);
							$('#rowIndexRecursoFinanPro').val(rowIndexRecursoFinanPro);
							$('#rfpid').val(rfpid);
							$('#ndpidRecursoFinanPro').val(data.ndpidRecursoFinanPro);
							$('#ndpidRecursoFinanPro_dsc').val(data.ndpidRecursoFinanPro_dsc);
							$('#rfpvalor').val(data.rfpvalor);
					   }
			 });
	});
	
	$('.excluirRecursoFinanPro').live('click', function() {
		if(!confirm('Deseja realmente excluir este registro.')){
			return false;
		}
		
		var tr = $(this).closest('tr');
		
		var param = new Array();
			param.push({name : 'requisicao', value : 'excluirRecursoFinanPro'}, 
					   {name : 'rfpid', value : $(this).attr('id')}
			);
		$.ajax({
			type	 : "POST",
			url		 : "academico.php?modulo=principal/receitaDespesa&acao=A",
			data	 : param,
			async    : false,
			//dataType : 'json',
			success	 : function(data){
							if(trim(data) == 'sucesso'){
								tr.remove();
								calcularTotal('RecursoFinanPro');
								//alert('Operação realizada com sucesso.');
							}
					   }
			 });
	});
	
	$('#salvarRecursoFinanPro').click(function() {
		var msg = "";
		if($('#ndpidRecursoFinanPro').val() == ''){
			msg += "O campo Natureza é obrigatório.\n";
		}
		if($('#rfpvalor').val() == ''){
			msg += "O campo Valor é obrigatório.";
		}
		if(msg){
			alert(msg);
			return false;
		}
		
		
		var param = new Array();
			param.push({name : 'requisicao', value : 'salvarRecursoFinanPro'}, 
					   {name : 'rowIndexRecursoFinanPro', value : $('#rowIndexRecursoFinanPro').val()},
					   {name : 'rfpid', value : $('#rfpid').val()},
					   {name : 'prcidRecursoFinanPro', value : $('#prcidRecursoFinanPro').val()},
					   {name : 'ndpcodRecursoFinanPro', value : $('#ndpcodRecursoFinanPro').val()},
					   {name : 'rfpvalor', value : $('#rfpvalor').val()}
			);	
		
		jQuery.ajax({
			type	 : "POST",
			url		 : "academico.php?modulo=principal/receitaDespesa&acao=A",
			data	 : param,
			async    : false,
			dataType : 'json',
			success	 : function(data){
				//$('#divDebug').html(data); return false;
				if(data.msg == 'sucesso'){
					// Verificamos se é alteração
					if(parseInt(data.rowIndexRecursoFinanPro) >= 0 ){
						var tr = $('#trRecursoFinanPro_'+parseInt(data.rowIndexRecursoFinanPro));
						
						var classe = "";
						if(tr.find("td:first").hasClass("even")){
							classe = 'even';
						} else {
							classe = 'odd';
						}
						
						var html = '<tr id="trRecursoFinanPro_'+parseInt(data.rowIndexRecursoFinanPro)+'" class="trRecursoFinanPro">';
							html +='	<td class="'+classe+' borderRight firstCell"><div class="tableData" style="width: 133px;"><p class="tableData">'+data.ndpcod+'</p></div></td>';
							html +='	<td class="'+classe+' borderRight"><div class="tableData" style="width: 119px;"><p class="tableData">'+data.rfpvalor+'</p></div></td>';
							html +='	<td class="'+classe+'"><div class="tableData" style="width: 44px;"><p class="tableData"><img border="0" class="alterarRecursoFinanPro" id="'+data.rfpid+'" style="cursor: pointer;" title="Alterar" src="/imagens/alterar.gif">&nbsp;<img border="0" id="'+data.rfpid+'" class="excluirRecursoFinanPro" title="Excluir" style="cursor: pointer;" src="/imagens/excluir.gif"></p></div></td>';
							html +='</tr>';
						
						tr.after(html).remove();
					} else {
						var rowIndexRecursoFinanPro = 0;
						if(jQuery('#tbodyRecursoFinanPro').children().length > 0){
							var rowIndexRecursoFinanPro = jQuery('#tbodyRecursoFinanPro').children().length + 1;
						}
						var tr = jQuery('#tbodyRecursoFinanPro').find('tr:last');
						
						var classe = "";
						if(tr.find("td:first").hasClass("even")){
							classe = 'even';
						} else {
							classe = 'odd';
						}
						
						var html = '<tr id="trRecursoFinanPro_'+parseInt(rowIndexRecursoFinanPro)+'" class="trRecursoFinanPro">';
							html +='	<td class="'+classe+' borderRight firstCell"><div class="tableData" style="width: 133px;"><p class="tableData">'+data.ndpcod+'</p></div></td>';
							html +='	<td class="'+classe+' borderRight"><div class="tableData" style="width: 119px;"><p class="tableData">'+data.rfpvalor+'</p></div></td>';
							html +='	<td class="'+classe+'"><div class="tableData" style="width: 44px;"><p class="tableData"><img border="0" class="alterarRecursoFinanPro" id="'+data.rfpid+'" style="cursor: pointer;" title="Alterar" src="/imagens/alterar.gif">&nbsp;<img border="0" id="'+data.rfpid+'" class="excluirRecursoFinanPro" title="Excluir" style="cursor: pointer;" src="/imagens/excluir.gif"></p></div></td>';
							html +='</tr>';
						
						if(rowIndexRecursoFinanPro > 0){
							tr.after(html);					
						} else {
							jQuery('#tbodyRecursoFinanPro').append(html);
						}
					}
					calcularTotal('RecursoFinanPro');
					//alert('Dados gravado com sucesso.');						
				} else if(data.msg == 'erro') {
					alert('Ocorreu algum erro.');						
				}
			}
		});
		
		limparCampos('rfp','RecursoFinanPro');
		
	});
	
	calcularTotal('RecursoFinanPro');
	
	/*===========================================================================================
	 * RECURSO FINANCEIRO PRÓPRIO
	 *===========================================================================================
	 */
	
	/*===========================================================================================
	 * DESPESA FINANCEIRA PRÓPRIO
	 *===========================================================================================
	 */	
	$('.scrollDespesaFinanPro').fixedHeaderTable();
	
	$('.scrollDespesaFinanPro').find('table tbody tr')
	.live('mouseover',function(){
		$('td', this).addClass('highlight');
	})
	.live('mouseout',function(){
		$('td', this).removeClass('highlight');
	});
	
	$('.alterarDespesaFinanPro').live('click', function() {
		var tr 		 = $(this).closest('tr');
		var rowIndexDespesaFinanPro = tr.prevAll().length;
		
		var dfpid = $(this).attr('id');
		var param = new Array();
			param.push({name : 'requisicao', value : 'carregarDespesaFinanPro'}, 
					   {name : 'dfpid', value : dfpid}
			);
		$.ajax({
			type	 : "POST",
			url		 : "academico.php?modulo=principal/receitaDespesa&acao=A",
			data	 : param,
			async    : false,
			dataType : 'json',
			success	 : function(data){
							//$('#divDebug').html(data);
							$('#rowIndexDespesaFinanPro').val(rowIndexDespesaFinanPro);
							$('#dfpid').val(dfpid);
							$('#ndpidDespesaFinanPro').val(data.ndpidDespesaFinanPro);
							$('#ndpidDespesaFinanPro_dsc').val(data.ndpidDespesaFinanPro_dsc);
							$('#dfpvalor').val(data.dfpvalor);
					   }
			 });
	});
	
	$('.excluirDespesaFinanPro').live('click', function() {
		if(!confirm('Deseja realmente excluir este registro.')){
			return false;
		}
		
		var tr = $(this).closest('tr');
		
		var param = new Array();
			param.push({name : 'requisicao', value : 'excluirDespesaFinanPro'}, 
					   {name : 'dfpid', value : $(this).attr('id')}
			);
		$.ajax({
			type	 : "POST",
			url		 : "academico.php?modulo=principal/receitaDespesa&acao=A",
			data	 : param,
			async    : false,
			//dataType : 'json',
			success	 : function(data){
							if(trim(data) == 'sucesso'){
								tr.remove();
								calcularTotal('DespesaFinanPro');
								//alert('Operação realizada com sucesso.');
							}
					   }
			 });
	});
	
	$('#salvarDespesaFinanPro').click(function() {
		var msg = "";
		if($('#ndpidDespesaFinanPro').val() == ''){
			msg += "O campo Natureza é obrigatório.\n";
		}
		if($('#dfpvalor').val() == ''){
			msg += "O campo Valor é obrigatório.";
		}
		if(msg){
			alert(msg);
			return false;
		}
		
		var param = new Array();
			param.push({name : 'requisicao', value : 'salvarDespesaFinanPro'}, 
					   {name : 'rowIndexDespesaFinanPro', value : $('#rowIndexDespesaFinanPro').val()},
					   {name : 'dfpid', value : $('#dfpid').val()},
					   {name : 'prcidDespesaFinanPro', value : $('#prcidDespesaFinanPro').val()},
					   {name : 'ndpcodRecursoFinanPro', value : $('#ndpcodRecursoFinanPro').val()},
					   {name : 'dfpvalor', value : $('#dfpvalor').val()}
			);	
		
		jQuery.ajax({
			type	 : "POST",
			url		 : "academico.php?modulo=principal/receitaDespesa&acao=A",
			data	 : param,
			async    : false,
			dataType : 'json',
			success	 : function(data){
				if(data.msg == 'sucesso'){
					//$('#divDebug').html(data);
					// Verificamos se é alteração
					if(parseInt(data.rowIndexDespesaFinanPro) >= 0 ){
						var tr = $('#trDespesaFinanPro_'+parseInt(data.rowIndexDespesaFinanPro));
						
						var classe = "";
						if(tr.find("td:first").hasClass("even")){
							classe = 'even';
						} else {
							classe = 'odd';
						}
						
						var html = '<tr id="trDespesaFinanPro_'+parseInt(data.rowIndexDespesaFinanPro)+'" class="trDespesaFinanPro">';
							html +='	<td class="'+classe+' borderRight firstCell"><div class="tableData" style="width: 133px;"><p class="tableData">'+data.ndpcod+'</p></div></td>';
							html +='	<td class="'+classe+' borderRight"><div class="tableData" style="width: 119px;"><p class="tableData">'+data.dfpvalor+'</p></div></td>';
							html +='	<td class="'+classe+'"><div class="tableData" style="width: 44px;"><p class="tableData"><img border="0" class="alterarDespesaFinanPro" id="'+data.dfpid+'" style="cursor: pointer;" title="Alterar" src="/imagens/alterar.gif">&nbsp;<img border="0" id="'+data.dfpid+'" class="excluirDespesaFinanPro" title="Excluir" style="cursor: pointer;" src="/imagens/excluir.gif"></p></div></td>';
							html +='</tr>';
						
						tr.after(html).remove();
					} else {
						var rowIndexDespesaFinanPro = 0;
						if(jQuery('#tbodyDespesaFinanPro').children().length > 0){
							var rowIndexDespesaFinanPro = jQuery('#tbodyDespesaFinanPro').children().length + 1;
						}
						var tr = jQuery('#tbodyDespesaFinanPro').find('tr:last');
						
						var classe = "";
						if(tr.find("td:first").hasClass("even")){
							classe = 'even';
						} else {
							classe = 'odd';
						}
						
						var html = '<tr id="trDespesaFinanPro_'+parseInt(rowIndexDespesaFinanPro)+'" class="trDespesaFinanPro">';
							html +='	<td class="'+classe+' borderRight firstCell"><div class="tableData" style="width: 133px;"><p class="tableData">'+data.ndpcod+'</p></div></td>';
							html +='	<td class="'+classe+' borderRight"><div class="tableData" style="width: 119px;"><p class="tableData">'+data.dfpvalor+'</p></div></td>';
							html +='	<td class="'+classe+'"><div class="tableData" style="width: 44px;"><p class="tableData"><img border="0" class="alterarDespesaFinanPro" id="'+data.dfpid+'" style="cursor: pointer;" title="Alterar" src="/imagens/alterar.gif">&nbsp;<img border="0" id="'+data.dfpid+'" class="excluirDespesaFinanPro" title="Excluir" style="cursor: pointer;" src="/imagens/excluir.gif"></p></div></td>';
							html +='</tr>';
						
						if(rowIndexDespesaFinanPro > 0){
							tr.after(html);					
						} else {
							jQuery('#tbodyDespesaFinanPro').append(html);
						}
					}
					calcularTotal('DespesaFinanPro');
					//alert('Dados gravado com sucesso.');						
				} else if(data.msg == 'erro') {
					alert('Ocorreu algum erro.');						
				}
			}
		});
		
		limparCampos('dfp','DespesaFinanPro');
		
	});
	
	calcularTotal('DespesaFinanPro');
	
	/*===========================================================================================
	 * DESPESA FINANCEIRA PRÓPRIO
	 *===========================================================================================
	 */
	
});

function limparCampos(iniciais,tela)
{
	$('#rowIndex'+tela).val('');
	$('#'+iniciais+'id').val('');
	$('#'+iniciais+'valor').val('');
	campo_popup_remover_item( 'ndpid'+tela);
	return false;
}

function calcularTotal(tela)
{
	var total = 0;
	$('#tbody'+tela).find('tr').each(function(){
		var valor = jQuery(this).find('td').next().text();
			valor = parseFloat(replaceAll(replaceAll(valor,".",""),",","."));
			total = total + valor;
	});
	$('#tdTotal'+tela).html("<strong>"+mascaraglobal('###.###.###.###,##',total.toFixed(2).replace(".",","))+"</strong>");
	return false;
	
}
