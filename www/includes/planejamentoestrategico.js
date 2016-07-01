	function popularCombo(primeiroId, idSelecionado, url){
	  var primeiro = document.getElementById(primeiroId);
	  var selecionado = document.getElementById(idSelecionado);
	  var carregando = "carregando...";
	
	  if(!primeiro){
	     return;
	  }
	  if(primeiro.options[primeiro.selectedIndex] == null || primeiro.options[primeiro.selectedIndex].value == 0){
	     selecionado.options.length = 1;
	     return;
	  }
	
	  url = url + ".php?value=" + primeiro.options[primeiro.selectedIndex].value+"&item="+primeiroId;
	
	  selecionado.disabled = true;
	  selecionado.options[0] = new Option("", carregando);
	  var request;
	
	  try {
	     request = new ActiveXObject("Microsoft.XMLHTTP"); //ie
	  }
	  catch(e){
	     request = new XMLHttpRequest(); //outros navegadores
	  }
	  request.onreadystatechange = function() { ;
	     if (request.readyState == 4){
	        if(request.status  == 200 || request.status  == 0) {
	           txt = request.responseText;
			   vOptions = txt.split("#%")
			   selecionado.options.length = vOptions.length;
			   for(i = 0; i < vOptions.length; i++){
			      vData = vOptions[i].split("|");
			   	  selecionado.options[i] = new Option(vData[1],vData[0]);
			   }
			   selecionado.disabled = false;
	        } else {
	           selecionado.options[0] = new Option("", carregando);
	        }
	     }else{
	     	selecionado.options[0] = new Option("", carregando);
	     }
	  }
	  request.open('GET', url,  true);
	  request.send(null);
	  selecionado.focus();
	}
	    
	function popularCombos(selecionado, primeiroPreenchido, url1, segundoPreenchido, url2){
	  var sel = document.getElementById(selecionado);
	  var preenchido1 = document.getElementById(primeiroPreenchido);
	  var preenchido2 = document.getElementById(segundoPreenchido);
	
	  var carregando = "carregando...";
	
	  if(!sel){
	     return;
	  }
	  if(sel.options[sel.selectedIndex] == null || sel.options[sel.selectedIndex].value == 0){
	     preenchido1.options.length = 1;
	     preenchido2.options.length = 1;
	     return;
	  }
	
	  url1 = url1 + ".php?value=" + sel.options[sel.selectedIndex].value+"&item=estcod";
	
	  preenchido1.disabled = true;
	  preenchido1.options[0] = new Option("", carregando);
	  var request;
	
	  try {
	     request = new ActiveXObject("Microsoft.XMLHTTP"); //ie
	  }
	  catch(e){
	     request = new XMLHttpRequest(); //outros navegadores
	  }
	  request.onreadystatechange = function() { ;
	     if (request.readyState == 4){
	        if(request.status  == 200 || request.status  == 0) {
	           txt = request.responseText;
			   vOptions = txt.split("#%");
			   preenchido1.options.length = vOptions.length;
			   for(i = 0; i < vOptions.length; i++){
			      vData = vOptions[i].split("|");
			   	  preenchido1.options[i] = new Option(vData[1],vData[0]);
			   }
			   preenchido1.disabled = false;
	        } else {
	           preenchido1.options[0] = new Option("", carregando);
	        }
	     }else{
	     	preenchido1.options[0] = new Option("", carregando);
	     }
	  }
	  request.open('GET', url1,  true);
	  request.send(null);
	
	
	  url2 = url2 + ".php?value=" + sel.options[sel.selectedIndex].value;
	
	  preenchido2.disabled = true;
	  preenchido2.options[0] = new Option("", carregando);
	  var request2;
	
	  try {
	     request2 = new ActiveXObject("Microsoft.XMLHTTP"); //ie
	  }
	  catch(e2){
	     request2 = new XMLHttpRequest(); //outros navegadores
	  }
	  request2.onreadystatechange = function() { ;
	     if (request2.readyState == 4){
	        if(request2.status  == 200 || request2.status  == 0){
	           var txt2 = request2.responseText;
			   var vOptions2 = txt2.split("#%");
			   var tamanho = vOptions2.length;
			   preenchido2.options.length = tamanho;
			   for(j = 0; j < tamanho; j++){
			      var vData2 = vOptions2[j].split("|");
			   	  preenchido2.options[j] = new Option(vData2[1],vData2[0]);
			   }
			   preenchido2.disabled = false;
	        } else {
	           preenchido2.options[0] = new Option("", carregando);
	        }
	     }else{
	     	preenchido2.options[0] = new Option("", carregando);
	     }
	  }
	  request2.open('GET', url2,  true);
	  request2.send(null);
	  //preenchido2.focus();
	}
	
	function exibirRelatorio(valortipo){
		document.getElementById('tipoRelatorio').value = valortipo;
        var formulario = document.getElementById('formulario');
        formulario.tipo.value = valortipo;
        if(document.getElementById('estcod').value == ""){
	        alert('Selecione um estado');
	        return;
        }
        //selectAllOptions( document.getElementById( 'fodid' ) );
        //selectAllOptions( document.getElementById( 'municipios' ) );
        //selectAllOptions( document.getElementById( 'microregioes' ) );
        //selectAllOptions( document.getElementById( 'estcod' ) );
               // submete formulario
        formulario.target = 'planejamentoestrategico_resultado';
        var janela = window.open('', 'planejamentoestrategico_resultado', 'width=1024,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
        formulario.submit();
        janela.focus();
    }

	