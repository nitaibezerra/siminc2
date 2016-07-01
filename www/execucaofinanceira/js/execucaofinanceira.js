function marcarChk(obj) {

	var codigo 				= obj.value;
	var emcpercentualemp 	= $('#emcpercentualemp_'+codigo);
	var emcvaloremp 		= $('#emcvaloremp_'+codigo);	
	
	if( obj.checked ) {
		emcpercentualemp.attr('className', 'normal');
		emcpercentualemp.attr('readOnly', false);
		
		emcvaloremp.attr('className', 'normal');
		emcvaloremp.attr('readOnly', false);
		emcpercentualemp.focus();
	} else {
		emcpercentualemp.attr('className', 'disabled');
		emcpercentualemp.attr('readOnly', true);
		
		emcvaloremp.attr('className', 'disabled');
		emcvaloremp.attr('readOnly', true);
	}
	
	calcularTotal();
	/*	
	//faço o AJAX para poder carregar o combo da fonte de recurso
	$.ajax({
		type: "POST",
		url: "par.php?modulo=principal/solicitacaoEmpenhoPar&acao=A",
		data: "requisicao=carregarFonteRecurso&sbdid="+obj.value,
		async: false,
		success: function(msg){
			document.getElementById('fonteRecursoSPAN').innerHTML=msg;
	}
	});

	//faço o AJAX para poder carregar o combo do Plano Interno
	$.ajax({
		type: "POST",
		url: "par.php?modulo=principal/solicitacaoEmpenhoPar&acao=A",
		data: "requisicao=carregarPlanoInterno&sbdid="+obj.value,
		async: false,
		success: function(msg){
			document.getElementById('planointernoSPAN').innerHTML=msg;
	}
	});*/
	
	//filtraPTRES( document.getElementById('planointerno').value );
}

function calculaEmpenhoPorPorcento(obj) {
	var codigo = obj.id.split('_');
		codigo = codigo[1];
	var porcento = obj.value;
	var valorsubacao = retiraPontos($('#valorsubacao_'+codigo).val());
	
	var total = (parseFloat(valorsubacao) * parseFloat(porcento) ) / 100;
	var total_mac = mascaraglobal('###.###.###.###,##',total.toFixed(2));
	//$('#debug').html(total_mac);
	
	$('#emcvaloremp_'+codigo).val(total_mac);
	
	calcularTotal();
}

function calculaEmpenhoPorValor(obj) {	
	var codigo 			= obj.id.split('_');
		codigo 			= codigo[1];
	var valorinformado 	= retiraPontos(obj.value);
	var valorsubacao 	= retiraPontos($('#valorsubacao_'+codigo).val());	
	var total_mac 		= mascaraglobal('###.###.###.###,##',valorinformado);
	var percent 		= (parseFloat(valorinformado) * 100) / parseFloat(valorsubacao);
	
	$('#emcpercentualemp_'+codigo).val(percent.toFixed(2));

	calcularTotal();
}

function calcularTotal() {
	
	var total = 0;
	var valor = 0;
	
	$('[id*="chk_"]').each(function(){
		var codigo 	= this.id.split('_');
			codigo 	= codigo[1];
		
		if(this.checked == true ){
			valor = retiraPontos( $('#emcvaloremp_'+codigo).val() );
			valor = parseFloat(valor);
			total = total + valor;
		}
	});
	
	$('#vlrtotalempenho').val( mascaraglobal('###.###.###.###,##',total.toFixed(2)) );
}

function verificaPreenchimentoPorcentagem(obj){
	
	var codigo = obj.id.split('_');
		codigo = codigo[1];
	
	var valorSubacao 	= retiraPontos($('#valorsubacao_'+codigo).val());
	var valorInformado 	= retiraPontos($('#emcvaloremp_'+codigo).val());
	
	if( parseFloat(valorInformado) > parseFloat(valorSubacao) ){
		alert('O valor informado para empenho ultrapassa 100% do valor da Subação.');
		$('#emcvaloremp_'+codigo).val('0,00');
		$('#emcpercentualemp_'+codigo).val('0');
		calcularTotal();
	}
	
}

function retiraPontos(v){
	var valor = v.replace(/\./gi,"");
	valor = valor.replace(/\,/gi,".");
	
	return valor;
}