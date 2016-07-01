//var formulario = document.getElementById('formulario');

function populaHidden( valor, id ){
	document.getElementById( 'pervalor[' + id + ']' ).value = valor;	
}

function limpaCampos(){
	$('pteid').value = '';
	$('iceid').value = '';
	$('ptequantidade').value = '';
	$('ptevalorproponente').value = '';
	$('ptevalorunitario').value = '';
	$('ptedatainicio').value = '';
	$('ptedatafim').value = '';
	$('totalEmendaInformada').value = '0.00';
	$('hidConcedente').value = '0';
	$('hidValorTotal').value = '';	
	$('aceid1').value = '';
}

function pesquisar(){
	$('loader-container').show();
	var myajax = new Ajax.Request('emenda.php?modulo=principal/insereEspecificacaoReformulacaoPTA&acao=A', {
		        method:     'post',
		        parameters: '&pesquisa=true&ptiid='+$('ptiid').value+'&pteid='+$('pteid').value,
		        asynchronous: false,
		        onComplete: function (res){
					$('lista').innerHTML = res.responseText;
		        }
		  });
	$('loader-container').hide();
}
function calculaTotal(valor){

	var result = "";
	if( (valor) || ($('ptevalorunitario').value != "") ){
		valor = $('ptevalorunitario').value;
		if(parseInt($('ptequantidade').value) <= 0){
			alert('A quantidade não pode se menor ou igual a 0!');
			$('ptequantidade').focus();
			return false;
		}else{				
			valor = valor.replace(/\./gi,"");
			valor = valor.replace(/\,/gi,".");
			result = parseInt($('ptequantidade').value) * valor;

			$('hidValorTotal').value = result;
			$('total').innerHTML = 'R$ ' + mascaraglobal('###.###.###.###,##', result.toFixed(2));
			
			atualizaValorConcedente(result);
			
			if($('aceid1').value != ""){
				verificaValorTotal($('ptevalorproponente').value);
			}
		}
	}
}
function montaComboUnidade(iceid){
	$('loader-container').show();
	var req = new Ajax.Request('emenda.php?modulo=principal/insereEspecificacaoReformulacaoPTA&acao=A', {
	        method:     'post',
	        parameters: '&montaComboUnidade=true&iceid='+iceid,
	        asynchronous: false,
	        onComplete: function (res){
	        	$('unidade').innerHTML = res.responseText;
	        	
	        	$('pteid').value = "";		        	
	        	$('ptevalorunitario').value = "";
	        	$('ptequantidade').value = "";
	        	$('ptevalorproponente').value = "";
	        	//$('ptedatainicio').value = "";
	        	//$('ptedatafim').value = "";
	        	$('total').innerHTML = "";
	        	$('concedente').innerHTML = "";
	        	$('hidValorTotal').value = "";
				$('hidConcedente').value = "";
	        	
				if( $('espkit').value == "t" ){
					$('ptevalorunitario').disabled = true;
					$('ptequantidade').value = "1";
					$('ptequantidade').disabled = true;
					alert('Deverão ser informados todos os itens que compõem esta especificação.\nPara informá-los, selecione a opção + no campo Unidade de Medida.');
					$('unidade').innerHTML = res.responseText + " <img id='addKit' border='0' style='cursor: pointer' onclick='AddKit();' title='Adicionar Kit' alt='Ir' src='/imagens/gif_inclui.gif'/> <input type='hidden' name='ptequantidadeH' id='ptequantidadeH' value=''> <input type='hidden' name='ptevalorunitarioH' id='ptevalorunitarioH' value=''>";
					$('ptequantidadeH').value = "1";
					AddKit();
				}else{
					$('ptevalorunitario').disabled = false;
					$('ptequantidade').disabled = false;
				}
				
				/*if( $('espvalorunitariopadrao').value != "" ){
					$('ptevalorunitario').disabled = false;
					$('ptequantidade').value = "1";
					$('ptevalorunitario').value = mascaraglobal('###.###.###.###,##', $('espvalorunitariopadrao').value);
					calculaTotal('');
				}*/
	        }
	  });
	$('loader-container').hide();
}

function AddKit(){
	if($('pteid').value == ""){
		insereEspecificacaoPendente();
	}
	
	
	window.open('emenda.php?modulo=principal/popupCadastrarKit&acao=A&pteid='+$('pteid').value+'&iceid='+$('iceid').value+'&ptrid='+$('ptrid').value,'page','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no, height=450, width=1010');
}
function AlteraKit(pteid, iceid, visualiza){
	window.open('emenda.php?modulo=principal/popupCadastrarKit&acao=A&pteid='+pteid+'&iceid='+iceid+'&visualiza='+visualiza+'&ptrid='+$('ptrid').value,'page','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no, height=450, width=1010');
}

function verificaValorTotal(valor){
	var valorTotal = $('hidValorTotal').value;
	
	var valor = valor.replace(/\./gi,"");
	valor = valor.replace(/\,/gi,".");
	
	var result = 0;
	if(parseFloat(valor) > parseFloat(valorTotal) ){
		alert('O valor informado para o proponente não pode ser maior que Valor Total!');
		$('ptevalorproponente').focus();
		return false;
	}else{
		result = valorTotal - valor;
		atualizaValorConcedente(result);
		return true;
	}
}
function atualizaValorConcedente(valor){
	$('concedente').innerHTML = 'R$ ' + mascaraglobal('###.###.###.###,##', valor.toFixed(2));
	$('concedente2').innerHTML = 'R$ ' + mascaraglobal('###.###.###.###,##', valor.toFixed(2));
	$('hidConcedente').value = valor.toFixed(2);

	if($('boValor').value == "true"){
		$('pervalor_'+$('boPedid').value).value = mascaraglobal('###.###.###.###,##', valor.toFixed(2));
		$('pervalor['+$('boPedid').value+']').value = mascaraglobal('###.###.###.###,##', parseFloat($('hidConcedente').value).toFixed(2));
	}
	calculaEmendaEspecificacaoRecurso();
}

function calculaEmendaEspecificacaoRecurso(){
	var getValores = getValoresRecurso().split("|");
	var soma = 0;
	var concedente = $('hidConcedente').value;
	var restante = '';
	
	if(getValores != ""){
		for(i=0; i<getValores.length; i++){
			if(getValores[i] == ''){
				soma = soma + parseFloat(0);
			} else {
				soma = soma + parseFloat(getValores[i]);
			}
		}
	}
	
	$('totalEmendaInformada').value = soma.toFixed(2);
	restante = parseFloat($('hidConcedente').value) - parseFloat(soma.toFixed(2));
	if(restante < 0){
		restante = 'R$ -' + mascaraglobal('###.###.###.###,##', restante.toFixed(2));
	}else{		
		restante = 'R$ ' + mascaraglobal('###.###.###.###,##', restante.toFixed(2));
	}
	$('restante').innerHTML = restante;
	$('informado').innerHTML = 'R$ ' + mascaraglobal('###.###.###.###,##', soma.toFixed(2));
}

function verificaTotalEmendaInformado(){
	var soma = $('totalEmendaInformada').value;
	var concedente = $('hidConcedente').value;
	
	if(parseFloat(soma) > parseFloat(concedente) ){
		alert('A soma dos valores informados nos recursos deverá ser igual ao valor do concedente!');
		return false;
	} else if(parseFloat(soma) < parseFloat(concedente)){
		alert('A soma dos valores informados nos recursos deverá ser igual ao valor do concedente!');
		return false;
	} else {
		return true;
	}
}

function incluirEspecificacao(){
	var nomeform 		= 'formulario';
	var submeterForm 	= false;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();
	
	if($('espkit').value == 't'){
	
		campos[1] 			= "ptequantidade";
		campos[2] 			= "ptedatainicio";
		campos[3] 			= "ptedatafim";
						 
		tiposDeCampos[1] 	= "texto";
		tiposDeCampos[2] 	= "texto";
		tiposDeCampos[3] 	= "texto";
		
		if($('ptevalorunitario').value == ""){
			alert('A especificação selecionada é do tipo KIT e, para preenchê-la,\n é necessário informar todos os itens que compõem esse kit. Para informar esses itens, \n selecione a opção + no campo Unidade de Medida!');
			AddKit();
			return false;
		}
		
	}else{
		if(!$('pteid').value){
			campos[0] 			= "iceid";
			campos[1] 			= "ptequantidade";
			campos[2] 			= "ptevalorunitario";
			campos[3] 			= "ptedatainicio";
			campos[4] 			= "ptedatafim";
							 
			tiposDeCampos[0] 	= "select";
			tiposDeCampos[1] 	= "texto";
			tiposDeCampos[2] 	= "texto";
			tiposDeCampos[3] 	= "texto";
			tiposDeCampos[4] 	= "texto";
		}else{
			campos[1] 			= "ptequantidade";
			campos[2] 			= "ptevalorunitario";
			campos[3] 			= "ptedatainicio";
			campos[4] 			= "ptedatafim";
							 
			tiposDeCampos[1] 	= "texto";
			tiposDeCampos[2] 	= "texto";
			tiposDeCampos[3] 	= "texto";
			tiposDeCampos[4] 	= "texto";
		}
	}
	
	if(validaForm(nomeform, campos, tiposDeCampos, submeterForm ) ){

		var valorUnit = $('ptevalorunitario').value.replace(/\./gi,"");
		valorUnit = valorUnit.replace(/\,/gi,".");

		if(parseInt($('ptequantidade').value) <= 0){
			alert('A quantidade não pode ser menor ou igual a 0.');
			$('ptequantidade').focus();
			return false;
		}else if( valorUnit <= 0){
			alert('O valor unitário não pode ser menor ou igual a 0.');
			$('ptevalorunitario').focus();
			return false;
		}
		
		if(!verificaValorTotal($('ptevalorproponente').value)){
			$('ptevalorproponente').focus();
			return false;
		}

		/*if(!validaData($('ptedatainicio') ) ) {
			alert('Data início está no formato incorreto.');
			$('ptedatainicio').focus();
			return false;
		}else if(!validaData($('ptedatafim') ) ) {
			alert('Data fim está no formato incorreto.');
			$('ptedatafim').focus();
			return false;
		}else if( !validaDataMaior( $('ptedatainicio'), $('ptedatafim') ) ){
			alert("A data inicial não pode ser maior que data final.");
				$('ptedatainicio').focus();
			return false;
		}*/
		//document.formulario.submit();
		
		if(verificaTotalEmendaInformado()){
			insereEspecificacao();
		}
	}
	
}
function formataValor(valor){
	if(valor.value == '0' || valor.value == '00' || valor.value == ''){
		this.valor = '0,00';
	}
} 
function retiraPontos(v){
	var valor = v.replace(/\./gi,"");
	valor = valor.replace(/\,/gi,".");
	
	return valor;
}
function montaStringFormulario(){

	var arrayValor = "";
	var nomeCampo = "";
	var arCodigo = "";
	var arValor = "";
	for(i=0; i<formulario.length; i++){
		if(formulario.elements[i].name.indexOf('[') != -1 ){
			arCodigo = formulario.elements[i].name.split('['); 
			arrayValor = formulario.elements[i].name + "=> Array(["+arCodigo[1].substring(0, (arCodigo[1].length - 1) )+"] =>"+formulario.elements[i].value;
			nomeCampo = nomeCampo +"&"+arrayValor;
			alert(arrayValor);
		}else{
			nomeCampo = nomeCampo +"&"+formulario.elements[i].name+"="+formulario.elements[i].value;
		}
		
	}
	
}
function getValoresRecurso(){
	var valorRecurso = "";
	for(i=0; i<formulario.length; i++){
		
		if(formulario.elements[i].type == "text"){
			if(formulario.elements[i].id.indexOf('pervalor') != -1){
				if(valorRecurso == ""){
					valorRecurso = retiraPontos(formulario.elements[i].value);
				} else {
					valorRecurso = valorRecurso + "|" + retiraPontos(formulario.elements[i].value);
				}		
			}
		}
	}
	return valorRecurso;
}
function insereEspecificacao(){
	/*var params = 'ptiid=' + $F('ptiid') + '&iceid=' + $F('iceid') + '&aceid1=' + $F('aceid1') + '&ptequantidade=' + $F('ptequantidade') + 
				 '&ptevalorunitario=' + $F('ptevalorunitario') + '&ptevalorproponente=' + $F('ptevalorproponente') + 
				 '&ptedatainicio=' + $F('ptedatainicio') + '&ptedatafim=' + $F('ptedatafim') + '&pteid=' + $F('pteid') + '&espkit=' + $F('espkit') + 
				 '&pervalor=' + $('pervalor').value + '&emecod='+ $('emecod').value + '';*/
				 
	var valorRecurso = getValoresRecurso();
	var msg = '';
	//montaStringFormulario();
	
	$('submeter').value = 'salvar';
	$('espkit1').value = $('espkit').value
	$('valorRecurso').value = valorRecurso;
	
	$('formulario').submit();

	/*$('loader-container').show();
	var req = new Ajax.Request('emenda.php?modulo=principal/insereEspecificacaoReformulacaoPTA&acao=A', {
	        method:     'post',
	        // não estava funcionando...
	        parameters: '&insereEspecificacao=true&valorrecurso='+valorRecurso+'&espkit1='+$F('espkit')+'&'+$('formulario').serialize(),
	        //parameters: '&insereEspecificacao=true&'+params+'&valorrecurso='+valorRecurso,
	        asynchronous: false,
	        onComplete: function (res){
	        	$('erro').innerHTML = res.responseText;
	        	msg = res.responseText.split('|');

	        	if(msg != ''){
		        	for(i=0; i<msg.length; i++){
		        	
		        		if(msg[i] == '1'){
		        			alert('Operação realizada com sucesso!');
		        			if( $('ptridpai').value == '' && $('refid').value != '' ){
		        				window.location.href = 'emenda.php?modulo=principal/insereEspecificacaoReformulacaoPTA&acao=A';
		        			} else {
				        		pesquisar(); 
								limpaCampos();
							}
		        		} else {
		        			ini = msg[i].indexOf('<script', 0);
		        			if( ini != -1 ){
		        				extrairScript(msg[i]);
		        			} else {
		        				alert(msg[i]);
		        			}
		        		}
		        	}
	        	} else {
	        		alert('Operação não realizada!');
	        	}
	        }
	  });
	$('loader-container').hide();*/
}

function insereEspecificacaoPendente(){
		
	var req = new Ajax.Request('emenda.php?modulo=principal/insereEspecificacaoReformulacaoPTA&acao=A', {
	        method:     'post',
	        parameters: '&insereEspecificacaoPendente=true&'+$('formulario').serialize(),
	        asynchronous: false,
	        onComplete: function (res){
	        	//$('erro').innerHTML = res.responseText;
	        	$('pteid').value = res.responseText;
	        }
	  });
}

function alterar(pteid, celula){
	var retorno = "";
	
	/*var tabela = $('tbListaEspecificacao');
	
	for(i=0; i<tabela.rows.length; i++){
		if(i % 2){
			tabela.rows[i].style.backgroundColor = "#dedfde";				
		} else {
			tabela.rows[i].style.backgroundColor = "#F5F5F5";
		}
	}
	celula.parentNode.parentNode.parentNode.style.background = '#ffffcc';*/

	$('loader-container').show();
	var req = new Ajax.Request('emenda.php?modulo=principal/insereEspecificacaoReformulacaoPTA&acao=A', {
	        method:     'post',
	        parameters: '&alterar=true&pteid='+pteid,
	        asynchronous: false,
	        onComplete: function (res){		        	
				//$('erro').innerHTML = res.responseText;
				$('combo').style.display = 'none';
				$('texto').style.display = '';

				var json = res.responseText.evalJSON();

				$('btnIncluir').value = 'Alterar';

				$('pteid').value = json.pteid;
				$('ptiid').value = json.ptiid;
				$('aceid1').value = json.iceid;
				$('ptequantidade').value = (json.ptequantidade == 0 ? 1 : json.ptequantidade);
				$('ptevalorunitario').value = mascaraglobal('###.###.###.###,##', json.ptevalorunitario ) ;
				$('total').update('R$ ' + mascaraglobal('###.###.###.###,##', json.total ));
				$('hidValorTotal').value = json.total;
				$('hidConcedente').value = json.concedente;					
				$('ptevalorproponente').value = mascaraglobal('###.###.###.###,##', json.ptevalorproponente);
				$('concedente').update('R$ ' + mascaraglobal('###.###.###.###,##', json.concedente));
				$('ptedatainicio').value = json.dataini;
				$('ptedatafim').value = json.datafim;
				$('texto').update(json.espnome);
				
				
				$('unidade1').update("<input type='hidden' name='espkit' id='espkit' value=''> <input type='hidden' value='' id='espvalorunitariopadrao' name='espvalorunitariopadrao'>");
				$('espkit').value = json.espkit; 
				
				if(json.espkit == "t"){
					$('unidade').update(json.espunidademedida + " <img id='addKit' border='0' style='cursor: pointer' onclick='AddKit();' title='Adicionar Kit' alt='Ir' src='/imagens/gif_inclui.gif'/> <input type='hidden' name='ptevalorunitarioH' id='ptevalorunitarioH' value=''> <input type='hidden' name='ptequantidadeH' id='ptequantidadeH' value=''>");
					$('ptevalorunitario').disabled = true;
					$('ptequantidade').disabled = true;
					$('ptequantidadeH').value = $('ptequantidade').value;
					$('ptevalorunitarioH').value = mascaraglobal('###.###.###.###,##', json.ptevalorunitario )
				}else{
					//$('ptevalorunitario').disabled = false;
					//$('ptequantidade').disabled = false;
					$('unidade').update(json.espunidademedida);
					
					$('espvalorunitariopadrao').value = json.espvalorunitariopadrao;
					/*if( $('espvalorunitariopadrao').value != "" && $('espvalorunitariopadrao').value != "null" ){
						$('ptevalorunitario').disabled = true;
					}*/
				}
				if( json.ptiid != '' && json.pteid != '' ){
					atualizaEmendasQuadro( json.ptiid, json.pteid );
				}
	        }
	  });
	$('loader-container').hide();
}

function atualizaEmendasQuadro( ptiid, pteid ){
	
	var url = 'emenda.php?modulo=principal/insereEspecificacaoReformulacaoPTA&acao=A&alterarQuadroEmenda=true&pteid=' + pteid + '&ptiid='+ptiid;
	
	var myAjax = new Ajax.Updater(
		"iniciativaEspecificacaoRecurso",
		url,
		{
			method: 'post',
			asynchronous: false
		});
	
	for(i=0; i<formulario.length; i++){
		if(formulario.elements[i].type == "text"){
		
			if(formulario.elements[i].id.indexOf('pervalor') != -1){
				var arId = formulario.elements[i].id.split('_');
				populaHidden( formulario.elements[i].value, arId[1] );	
			}
		}
	}
	
	
	calculaEmendaEspecificacaoRecurso();
	$('concedente2').innerHTML = $('concedente').innerHTML;
	
	if($('boValor').value == "true"){
		$('pervalor_'+$('boPedid').value).value = mascaraglobal('###.###.###.###,##', parseFloat($('hidConcedente').value).toFixed(2));
		$('pervalor['+$('boPedid').value+']').value = mascaraglobal('###.###.###.###,##', parseFloat($('hidConcedente').value).toFixed(2));
	}
}

function alteraPTARecurso(pteid){
	var retorno = "";

	$('loader-container').show();
	var req = new Ajax.Request('emenda.php?modulo=principal/insereEspecificacaoReformulacaoPTA&acao=A', {
	        method:     'post',
	        parameters: '&alteraPTARecurso=true&pteid='+pteid,
	        asynchronous: false,
	        onComplete: function (res){
				$('iniciativaEspecificacaoRecurso').innerHTML = res.responseText;
	        }
	  });
	$('loader-container').hide();
}
function excluir(pteid){
	if(confirm('Deseja realmente excluir esta especificação?')) {
		$('loader-container').show();
		var req = new Ajax.Request('emenda.php?modulo=principal/insereEspecificacaoReformulacaoPTA&acao=A', {
		        method:     'post',
		        parameters: '&excluir=true&pteid='+pteid,
		        asynchronous: false,
		        onComplete: function (res){		 
		        	//$('erro').innerHTML = res.responseText;       	
					if(res.responseText == "1" ){
						alert('Operação realizada com sucesso!');
						pesquisar();
						limpaCampos();
					}else{
						alert('Operação não realizada!');
					}
		        }
		  });
		$('loader-container').hide();
	}
}
function Cancelar(){
	$('loader-container').show();

	var myajax = new Ajax.Request('emenda.php?modulo=principal/insereEspecificacaoReformulacaoPTA&acao=A', {
		        method:     'post',
		        parameters: '&cancelar=true&pteid='+$('pteid').value+'&ptiid='+$('ptiid').value,
		        asynchronous: false,
		        onComplete: function (res){

		        	//$('erro').innerHTML = res.responseText;
		        	if($('espkit').value == "t"){
		        		if($('btnIncluir').value == "Alterar"){
		        			pesquisar();
		        		}else{
		        			if(Number(res.responseText) > 0){
		        				excluirKit();
		        			}else{
		        				pesquisar();
		        			}
		        		}		
		        	}else{
		        		pesquisar();
		        	}
		        	
		        }
		  });
	$('loader-container').hide();
}

function excluirKit(){
	if(confirm('Deseja realmente cancelar a inserção dos dados?')) {
		$('loader-container').show();
		var req = new Ajax.Request('emenda.php?modulo=principal/insereEspecificacaoReformulacaoPTA&acao=A', {
		        method:     'post',
		        parameters: '&excluirKit=true&pteid='+$('pteid').value,
		        asynchronous: false,
		        onComplete: function (res){		        	
					if(res.responseText == "1" ){
						//alert('Operação realizada com sucesso!');
						pesquisar();
					}else{
						alert('Operação não realizada!');
					}
		        }
		  });
		$('loader-container').hide();
	}
}

/*function carregaDetalheEmenda(idImg, emeid, div_nome, tr_nome){	
	var img = document.getElementById( idImg );
	var boCarrega = false;

	if($(div_nome).style.display == 'none'){
		$(div_nome).style.display = '';
		$(tr_nome).style.display = '';
		img.src = '../imagens/menos.gif';
	} else {
		$(div_nome).style.display = 'none';
		$(tr_nome).style.display = 'none';
		img.src = '/imagens/mais.gif';
	}
}*/
function carregaDetalheEmenda(idImg, emeid, div_nome, tr_nome){	
	var img = document.getElementById( idImg );
	var boCarrega = false;

	if($(div_nome).style.display == 'none'){
		$(div_nome).style.display = '';
		$(tr_nome).style.display = '';
		img.src = '../imagens/menos.gif';
	} else {
		$(div_nome).style.display = 'none';
		$(tr_nome).style.display = 'none';
		img.src = '/imagens/mais.gif';
	}
}