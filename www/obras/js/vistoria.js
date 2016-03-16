
var arrFotosGaleria = new Array();

function validasituacao( id, superuser ){
	
	if( !document.formulario.supvdt.value && ( superuser == 1 ) ){
		alert("Favor inserir a Data da Vistoria!");
		document.formulario.supvdt.focus();
		document.formulario.stoid.value = "";
		return false;		
	}

	var obData = new Data();
	var d = document;

	if( id == 1 || id == 2 || id == 3 || id == 6 ){
/*		
		if( d.formulario.supvdt.value != "" 
			&& d.formulario.obrdtinicio.value != "" 
			&& d.formulario.obrdttermino.value != "" ){
			
			if( !(obData.comparaData( d.formulario.supvdt.value, d.formulario.obrdtinicio.value, '>=' ) 
			      && obData.comparaData( d.formulario.supvdt.value, d.formulario.obrdttermino.value, '<=' )) ){
				alert( "Para inserir uma vistoria com a situação Em Execução a Data da Vistoria deve estar no intervalo entre a Data de Inicío de Execução da Obra ("+d.formulario.obrdtinicio.value+") e Término da Obra ("+d.formulario.obrdttermino.value+")!" );
				document.formulario.supvdt.focus();
				return false;
			}
		}
*/	
	}

	if ( id == 4 ){
		if( d.formulario.supvdt.value != "" && d.formulario.fprdtiniciofaseprojeto.value != "" && d.formulario.fprdtconclusaofaseprojeto.value != "" ){
			if( !(obData.comparaData( d.formulario.supvdt.value, d.formulario.fprdtiniciofaseprojeto.value, '>=' ) && obData.comparaData( d.formulario.supvdt.value, d.formulario.fprdtconclusaofaseprojeto.value, '<=' )) ){
				alert( "Para inserir uma vistoria com a situação Em Elaboração de Projetos a Data da Vistoria deve estar no intervalo entre a Data de Inicío Programado ("+d.formulario.fprdtiniciofaseprojeto.value+") e Término Programado do Projeto ("+d.formulario.fprdtconclusaofaseprojeto.value+")!" );
				document.formulario.supvdt.focus();
				return false;
			}
		}
	}

	if( id == 5 ){
		if( d.formulario.supvdt.value != "" && d.formulario.dtiniciolicitacao.value != "" && d.formulario.dtfinallicitacao.value != "" ){
			if( !(obData.comparaData( d.formulario.supvdt.value, d.formulario.dtiniciolicitacao.value, '>=' ) && obData.comparaData( d.formulario.supvdt.value, d.formulario.dtfinallicitacao.value, '<=' )) ){
				alert( "Para inserir uma vistoria com a situação Em Licitação a Data da Vistoria deve estar no intervalo entre a Data de Inicío Programado ("+d.formulario.dtiniciolicitacao.value+") e Data de Término Programado ("+d.formulario.dtfinallicitacao.value+") da Licitação !" );
				document.formulario.supvdt.focus();
				return false;
			}
		}
	}
	
	return true;

}

function verificasituacao( id ){
	
	var stoid = window.document.getElementById('stoid');
	document.getElementById('msg_paralisacao').style.display = 'none';
	
	if ( id == 2  ){
		if (document.selection){
			document.getElementById('msg_paralisacao').style.display = 'block';
			tr_tplid.style.display  = 'block';
			tr_tgpid.style.display  = 'block';
			tr_hprobs.style.display = 'block';
			
			tr1.style.display = 'block';
			tr2.style.display = 'block';
			tr3.style.display = 'block';
			//tr4.style.display = 'block';
			//tr5.style.display = 'block';
			tr6.style.display = 'block';
			tr7.style.display = 'block';			
			tr8.style.display = 'block';
			tr9.style.display = 'block';
			tr10.style.display = 'block';
			//tr11.style.display = 'block';
			//tr12.style.display = 'block';
		}else{
			document.getElementById('msg_paralisacao').style.display = 'table-row';
			tr_tplid.style.display  = 'table-row';
			tr_tgpid.style.display  = 'table-row';
			tr_hprobs.style.display = 'table-row';
			
			tr1.style.display = 'table-row';
			tr2.style.display = 'table-row';
			tr3.style.display = 'table-row';
			//tr4.style.display = 'table-row';
			//tr5.style.display = 'table-row';
			tr6.style.display = 'table-row';
			tr7.style.display = 'table-row';			
			tr8.style.display = 'table-row';
			tr9.style.display = 'table-row';
			tr10.style.display = 'table-row';
			//tr11.style.display = 'table-row';
			//tr12.style.display = 'table-row';
		}
		
	}else if ( id == 4 ){
		
		if (document.selection){
			tr_elaboracao.style.display  = 'block';
			tr_elaboracao1.style.display = 'block';
			tr_elaboracao2.style.display = 'block';
			
		}else{
			tr_elaboracao.style.display  = 'table-row';
			tr_elaboracao1.style.display = 'table-row';
			tr_elaboracao2.style.display = 'table-row';
	
		}
		
		tr1.style.display = 'none';
		tr2.style.display = 'none';
		tr3.style.display = 'none';
		//tr4.style.display = 'none';
		//tr5.style.display = 'none';
		tr6.style.display = 'none';
		tr7.style.display = 'none';
		
		tr8.style.display = 'none';
		tr9.style.display = 'none';
		tr10.style.display = 'none';
		//tr11.style.display = '';
		//tr12.style.display = 'none';
	
	}else if ( id == 5 ){
	
		tr_tplid.style.display 	     = 'none';
		tr_tgpid.style.display 	     = 'none';
		tr_hprobs.style.display      = 'none';
		tr_elaboracao.style.display  = 'none';
		tr_elaboracao1.style.display = 'none';
		tr_elaboracao2.style.display = 'none';
		//tr12.style.display = 'none';
		
		
		tr1.style.display    = 'none';
		tr2.style.display    = 'none';
		tr3.style.display    = 'none';
		//tr4.style.display  = 'none';
		//tr5.style.display  = 'none';
		tr6.style.display    = 'none';
		tr7.style.display    = 'none';
		
		tr8.style.display    = 'none';
		tr9.style.display    = 'none';
		tr10.style.display   = 'none';
		//tr11.style.display   = '';
		//tr12.style.display = 'none';
		
		document.formulario.obrlincambiental.value  = null;
		document.formulario.obraprovpatrhist.value  = null;
		document.formulario.obrdtprevprojetos.value = null;
		document.formulario.tplid.value  = null;
		//document.formulario.hprobs.value = null;
		
		tr1.style.display = 'none';
		tr2.style.display = 'none';
		tr3.style.display = 'none';
		//tr4.style.display = 'none';
		//tr5.style.display = 'none';
		tr6.style.display = 'none';
		tr7.style.display = 'none';
		
		tr8.style.display = 'none';
		tr9.style.display = 'none';
		tr10.style.display = 'none';
		//tr11.style.display = '';
		//tr12.style.display = 'none';
	
	}else if (id == 99){
		tr1.style.display = 'none';
		tr2.style.display = 'none';
		tr3.style.display = 'none';
		//tr4.style.display = 'none';
		//tr5.style.display = 'none';
		tr6.style.display = 'none';
		tr7.style.display = 'none'; 
		
		tr8.style.display = 'none';
		tr9.style.display = 'none';
		tr10.style.display = 'none';
		//tr11.style.display = 'none';
		//tr12.style.display = 'none';
		
	}else{
		
		tr_tplid.style.display 	     = 'none';
		tr_tgpid.style.display 	     = 'none';
		tr_hprobs.style.display      = 'none';
		tr_elaboracao.style.display  = 'none';
		tr_elaboracao1.style.display = 'none';
		tr_elaboracao2.style.display = 'none';
		document.formulario.obrlincambiental.value  = null;
		document.formulario.obraprovpatrhist.value  = null;
		document.formulario.obrdtprevprojetos.value = null;
		document.formulario.tplid.value  = null;
		document.formulario.hprobs1.value = null;
		if (document.selection){
			
			tr1.style.display = 'block';
			tr2.style.display = 'block';
			tr3.style.display = 'block';
			//tr4.style.display = 'block';
			//tr5.style.display = 'block';
			tr6.style.display = 'block';
			tr7.style.display = 'block';
			tr8.style.display = 'block';
			tr9.style.display = 'block';
			tr10.style.display = 'block';
			//tr11.style.display = 'block';
			//tr12.style.display = 'block';
		}else{
			tr1.style.display = 'table-row';
			tr2.style.display = 'table-row';
			tr3.style.display = 'table-row';
			//tr4.style.display = 'table-row';
			//tr5.style.display = 'table-row';
			tr6.style.display = 'table-row';
			tr7.style.display = 'table-row';
			tr8.style.display = 'table-row';
			tr9.style.display = 'table-row';
			tr10.style.display = 'table-row';
			//tr11.style.display = 'table-row';
			//tr12.style.display = 'table-row';
		}
		
	}
}

function enviaFormulario( superuser ){
	
	var stoid = window.document.getElementById('stoid');
	
	if( stoid.value == 1 ){
		
		var obrcustocontrato = document.getElementById('obrcustocontrato').value;
		var totalvalor 	     = document.getElementById('totalvalor').value;
		
		if( Math.round(totalvalor) < Math.round(obrcustocontrato && ( superuser == 1 ) ) ){
			alert("Aqui Para inserir uma Vistoria com Situação da Obra Em Execução é necessário preencher o Cronograma Físico-Financeiro!");
			return false;
		}

	}
	
	if( stoid.value == 2 ){
		
		if(document.getElementById('tplid').value == "") {
			alert("É obrigatório selecionar o tipo de paralisação");
			return false;
		}

	}
	
	if(!jQuery('[name=supobs]').val()){
		if(superuser != 11){
			alert("Para cadastrar uma vistoria, é necessário preencher o Relatório Técnico!");
			return false;
		}
	}
	
	var num = jQuery('#fotos_supervisao li');

	if( num.size() <= 0 ){
		if(superuser != 11){
			alert("Para cadastrar uma vistoria, é necessário anexar ao menos uma foto!");
			return false;
		}
	}
	
	var dataSup = jQuery('#supvdt').val().split('/');
	
	var diaSup = dataSup[0];
	var mesSup = dataSup[1];
	var anoSup = dataSup[2];
	
	var dataAtual = new Date();
	
	var diaAtual = dataAtual.getDate() < 10 ? '0'+dataAtual.getDate() : dataAtual.getDate();  
	var mesAtual = dataAtual.getMonth() < 9 ? '0'+(parseInt(dataAtual.getMonth())+1) : parseInt(dataAtual.getMonth())+1;
	var anoAtual = dataAtual.getFullYear();
	
	var data1 = anoSup+''+mesSup+''+diaSup;
	var data2 = anoAtual+''+mesAtual+''+diaAtual;
	
	if( data1 > data2 ){
		alert('A data de vistoria não pode ser maior que a data atual!');
		return false;
	}
	
	var arrFotosSupervisao = jQuery( "#fotos_supervisao").sortable( "serialize");
	var arrFotosGaleria    = jQuery( "#fotos_galeria").sortable( "serialize");
	jQuery( "#hdn_fotos_supervisao").val(arrFotosSupervisao);
	jQuery( "#hdn_fotos_galeria").val(arrFotosGaleria);

	if (validaVistoria("formulario", superuser) && validasituacao(stoid.value, superuser)){
		document.getElementById("formulario").submit();
		document.getElementById("salva_vistoria").disabled = "disabled";
	}
	
}

function alteraValor(id, percObra, valor) {
	
	var supervisao      = document.getElementById("supvlrinfsuperivisor_"+id);
	var item_exec_sobra = document.getElementById("percexecsobreobra_"+id);
	var perc_real_obra  = document.getElementById("percrealobra_"+id);
	
	if(supervisao.value != "") {
		
		percObra = Number(percObra) / 100; 
		num = (Number(supervisao.value.replace(",",".")) * percObra);
		perc_real_obra.value = num;
		item_exec_sobra.value = num.toFixed(2).toString().replace(".",",");
	
	}else {
		
		if (Number(valor.replace(",",".")) == 0){
			num = Number((supervisao.value.replace(",",".") * percObra.toString().replace(",",".")) / 100);
			item_exec_sobra.value = num.toFixed(2).toString().replace(".",",");
		}else{
			num = Number((supervisao.value.replace(",",".") * percObra.toString().replace(",",".")) / 100);
			item_exec_sobra.value = num.toFixed(2).toString().replace(".",",");
		}
		
	}
}

function alteraValorInst(id, percObra, valor) {
	
	var supervisao      = document.getElementById("supvlrinfsuperivisor_"+id);

	var existesupervisaoinstituicao = document.getElementById("existesupervisaoinstituicao").value; 
	var perfilinstituicao = document.getElementById("perfilinstituicao").value;
	var requisicao = document.getElementsByName("requisicao")[0].value;
	
	if(parseFloat(existesupervisaoinstituicao) > 0 && parseFloat(perfilinstituicao) != 1 && requisicao == 'cadastra'){
		
		var vlatual = supervisao.value.toString().replace(",",".");
		if(vlatual.indexOf(".")==-1) vlatual = vlatual+".00";
		
		var valorant = valor.toString();
		if(valorant.indexOf(".")==-1) valorant = valorant+".00"; 

//		Comentado a pedido do Mario dia 21/06/2012
//		if(parseFloat(vlatual) < parseFloat(valorant)){
//
//			vlatual = MascaraMonetario(vlatual);
//			valorant = MascaraMonetario(valorant);
//
//			alert('O valor ('+vlatual+') do campo "(%) da Supervisão" não pode ser maior do que o valor anterior ('+valorant+').');
//			supervisao.value = valorant;
//		}
//		FIM Comentado a pedido do Mario dia 21/06/2012
	}
	
}

function obras_verificaPercentual(id){
	
	supervisao = document.getElementById("supvlrinfsuperivisor_"+id);
	supervisao.value = supervisao.value.replace(",",".");
	supervisao.value = mascaraglobal('###,##',Number(supervisao.value).toFixed(2));
	supervisao.value = supervisao.value.replace(",",".");
	
	
	if ( supervisao.value > 100.00 ){
		supervisao.value = supervisao.value.replace(".",",")
		alert('O valor do campo "(%) da Supervisão" não pode ser maior do que 100.');
		supervisao.value = valor_antigo[id].toFixed(2);;
	}
	
		
	if ( supervisao.value == '' ){
		supervisao.value = valor_antigo[id].toFixed(2);
	}

	supervisao.value = supervisao.value.replace(".",",");
}

function obras_calculaTotalVistoria(){
	
	var x = document.getElementById("formulario");
	
	var valor  = 0;
	var soma_s = 0;
	
	for ( var i = 0; i < x.length; i++ ){
		
		// Soma dos valores do percentual executado sobre a obra. 
//		if ( x.elements[i].id.search(/percexecsobreobra_/) >= 0 ){
		if ( x.elements[i].id.search(/percrealobra_/) >= 0 ){
			valor = Number( x.elements[i].value.replace(",",".") );
			soma_s +=  valor;
		}
		
	}
	
	// Atualiza o valor do percentual sobre a obra.
	//document.getElementById("sobreobra").innerHTML  = soma_s.toFixed(2).toString().replace(".",",");
	document.getElementById("sobreobra").innerHTML  = "<input type='text' size='10' readonly='Yes' id='percsupatual' name='percsupatual' value='"+soma_s.toFixed(2).toString().replace(".",",")+"' style='text-align:right;'>";
}	

function repositorioGaleria( idDiv, id, src ){

	if( document.getElementById( "galeria_" + id ).checked == true ){
		
		var img = "<img id='"+id+"' src='"+src+"' style='margin: 0px;opacity: 1' class='imageBox_theImage' onclick='javascript:window.open(\""+src+"\",\"imagem\",\"width=850,height=600,resizable=yes\")'/>";
		
		var hidden = "<input type='hidden' value='" + id + "' id='galeria_"+id+"' name='galeria["+id+"]'/>";
		
		document.getElementById( idDiv + "Galeria").innerHTML = img + hidden;
		
	}else{
		document.getElementById( idDiv + "Galeria").innerHTML = "";
	}

}

function obrEnviaFotosGaleria(){
	
	var campos = document.getElementsByTagName("input");
	
	for(i=0; i<campos.length; i++ ){
		
		if( campos[i].type == "hidden" ){
		
			foto = campos[i].id.substr(0,8); 
		
			if( foto == "galeria_" ){
				arrFotosGaleria.push( campos[i].value );
			}
		}
	}
	
	var url = caminho_atual + '?modulo=principal/inserir_vistoria&acao=A&subacao=galeria&AJAX=1';
	var parametros = "imagens=" + arrFotosGaleria;
		
	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function(resp) {
				alert( resp.responseText );
			}
			
		});
}


var optionsOriginais = null;

function validaDataVistoria (carregaAnteriores){
	
	var data = document.getElementById('supvdt').value;
	
	if (data == '')
	{
		alert('Data inválida');
		document.getElementById('supvdt').value = '';
		return;
	}
	
	var diav  = data.substring(0,2);
	var mesv  = data.substring(3,5);
	var anov  = data.substring(6,10);
	var dataV = new Date(mesv + '/' + diav + '/' + anov);
	
	var currentTime = new Date();
	
	var dia  = currentTime.getDate();
	var mes  = currentTime.getMonth()+1;
	var ano  = currentTime.getFullYear();
	var hoje = new Date(mes + '/' + dia + '/' + ano);
	
	if ( dataV > hoje ) {
		alert('Data da Vistoria não pode ser mais que '+ dia + '/' + mes + '/' + ano);
		document.getElementById('supvdt').value = '';
		document.getElementById('supvdt').focus();
		return;
	}
	
	
	var supvdt = jQuery('#supvdt').val();
	var url = window.location + '?modulo=principal/inserir_vistoria&acao=A&subacao=dataVistoria&AJAX=1&supvdt='+supvdt;
	
	
	if (data != '')
	{
	
	jQuery.ajax({
	  	url: url,
	  	success: function(dado) {
	  		//o if abaixo sera executado caso a data sendo inserida seja uma data intermediaria (menor que a ultima vistoria)
	  		if (dado != '')
	  		{
	  			
	  			if (dado == 'erro')
	  			{
	  				jQuery('#comDataAnterior').val('');
	  				alert('Data inválida');
	  				var dataSup = jQuery('#supvdt').val('');
	  				jQuery('#supvdt').focus();
	  				return;
	  			}
	  			
  				if (dado == 'menor')
	  			{
	  				jQuery('#comDataAnterior').val('');
	  				alert('Não é possível inserir uma data menor que a da primeira vistoria.');
	  				var dataSup = jQuery('#supvdt').val('');
	  				jQuery('#supvdt').focus();
	  				return;
	  			}
  				
  				
  				if (!confirm('Você está tentando inserir uma data menor que da última vistoria.\nDeseja continuar ?'))
				{
					jQuery('#comDataAnterior').val('');
					var dataSup = jQuery('#supvdt').val('');
					jQuery('#supvdt').focus();
					return;
				}
	  			
  				
  				
  				
	  			var dado1;
	  			var dado2;
	  			var supvid;
	  			//formato do retorno do dado ex: 1,1|Execucao,Execucao|2  -> sendo 1,1 ids das situacoes de supervisao
	  			//depois do Execucao,Execucao,  descricoes das situacoes 2 - id da supervisao (supvid) imediatamente anterior ao da data inserida
	  			
	  			//ids
	  			dado1 = dado.split('|')[0];
	  			//descricoes
	  			dado2 = dado.split('|')[1];
	  			//id da supervisao
	  			supvid = dado.split('|')[2];
	  			
	  			dado1 = dado1.split(',');
	  			dado2 = dado2.split(',');
	  			
	  			//guarda os valores originais do combo para recuperar caso a proxima data seja uma de tratamento normal (acima da ultima vistoria)
	  			if (optionsOriginais == null)
	  				optionsOriginais = jQuery('#stoid').clone(true).attr('options');
	  			
	  			// limpa o combo de situacoes de obras e insere valores sendo um o status da vistoria imediatamente anterior ao da data
	  			//sendo inserida e da data imediatamente apos
	  			jQuery('#stoid').empty();
	  			
	  			var options = jQuery('#stoid').attr('options');

	  			var anterior = '';
	  			options[0] = new Option('Selecione...','');
				jQuery.each(dado1, function(key, value) { 
	  			
	  			if (key != value){
	  				options[options.length] = new Option(dado2[key], value);
	  			}
	  				anterior = value;
	  			});
				jQuery('#stoid').val('');
	  			
	  			//sinaliza que esta sendo inserido uma data menor que da ultima
	  			jQuery('#comDataAnterior').val('1');
	  			
	  			//carregar os itens a partir da data imediatamente anterior
	  			
	  			if (carregaAnteriores)
	  			{
		  			var url = window.location + '?modulo=principal/inserir_vistoria&acao=A&subacao=itensVistoria&AJAX=1&supvid='+supvid;
		  			
		  			jQuery.ajax({
		  			  	url: url,
		  			  	success: function(dado) {
		  			  		jQuery('#listaItensVistoria').html(dado) ;
		  			 	}
		  			});
	  			}
	  		}
	  		else
	  		{
	  			
	  			
	  			
	  			//alert(jQuery('#inclusaoEmpresaObraFinalizada').val());
	  			
	  			if (jQuery('#inclusaoEmpresaObraFinalizada').val() == '1')
	  			{
  					jQuery('#comDataAnterior').val('');
	  				alert('Não é possível inserir uma data maior que a da última vistoria.');
	  				var dataSup = jQuery('#supvdt').val('');
	  				jQuery('#supvdt').focus();
	  				return;
	  			}
	  			
	  			jQuery('#comDataAnterior').val('');
	  			
	  			if ( optionsOriginais != null){
	  				jQuery('#stoid').empty();
	  				var options = jQuery('#stoid').attr('options');
	  				
	  				jQuery.each(optionsOriginais, function(i) { 
	  					//alert(value);
	  			        var $elem = jQuery(this);
	  					options[options.length] = new Option( $elem.text(), $elem.val());
	  				});
	  			}
	  		}
	  	}
	});
	}
}

