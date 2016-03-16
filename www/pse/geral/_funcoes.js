	function sobreescreveFuncaoAgrupador()
	{ 
		var select 		= document.getElementById( 'agrupadorEscolas' );
		var img 		= document.getElementById( 'imgPassaUm_agrupadorEscolas' );
		var imgTodos	= document.getElementById( 'imgPassaTodos_agrupadorEscolas' );
	
		if(window.addEventListener){ // Firefox
			img.setAttribute( "onclick", "funcao( 'um' )" );
			imgTodos.setAttribute( "onclick", "funcao( 'todos' )" );
			select.setAttribute( "ondblclick", "funcao( 'um' )" );
		}
		else{ // IE hhh
			img.attachEvent( "onclick", function() { funcao( 'um' ) } );
			imgTodos.attachEvent( "onclick", function() { funcao( 'todos' ) } );
			select.attachEvent( "ondblclick", function() { funcao( 'um' ) } );
		}
	}
	
	function consultarSCNES() {
		window.open('pse.php?modulo=principal/popConsultaSCNES&acao=A','page','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=400,height=500');
	}

	function verEquipes(cnes,equipe) {
		var req = new Ajax.Request('pse.php?modulo=principal/pse2009&acao=A', {
		        method:     'post',
		        parameters: 'verEquipesAjax=true&cnes='+cnes+'&equipe='+equipe,
		        asynchronous: false,
		        onComplete: function (res){
		        	$('colequipe').innerHTML = res.responseText;
		        },
		        onLoading: function(){
					destino.innerHTML = '<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>';
				}
		  });
	}	

	function funcao( tipo ){
		
		var arOptions 	= select.options;
		var count 		= 0;
		var maximo 		= document.getElementById( 'empquantescolapse2009' ).value;
		
		//CONTANDO A QUANTIDADE DE ITENS DE ESCOLAS SELECIONADAS
		for( i=0; i<arOptions.length; i++ )
		{
			if( arOptions[i].selected ) count++;
		}
		
		if( tipo == 'um' )
		{
			if( document.getElementById( 'Escolas' ).length <= maximo && ( count + document.getElementById( 'Escolas' ).length ) <= maximo )
			{
				moveSelectedOptions( document.getElementById( 'agrupadorEscolas' ), document.getElementById( 'Escolas' ), true, '' );
				return true;
			}
		}
		else
		{
			if( ( arOptions.length + document.getElementById( 'Escolas' ).length ) <= maximo )
			{
				moveAllOptions( document.getElementById( 'agrupadorEscolas' ), document.getElementById( 'Escolas' ), true, '' );
				return true;
			}
		}
		
		alert( 'Número máximo de escolas atingido!' );
		return false;
		
	}
	
	function habilitaCampos()
	{	
		select = document.getElementById( 'agrupadorEscolas' );
	
		if(document.getElementById( 'empquantescolapse2009' ).value == '')
		{
			select.disabled=true;
			return false;
		}
		 
		select.disabled=false;
	}
	
	function validaFormulario(item1_3)
	{	
	
		/* ------- VALIDANDO CAMPOS OBRIGATÓRIOS ----- */
			var campos 			= new Array();
			var tipoCampos 		= new Array();
			var tiposCamposObri = '';
			var camposObri 		= '';
			
			var camposObri = "#participouPSE#estuf#muncod";
			
			//se for capital 
			if(document.getElementById('capital').value == 'true')
			{
				camposObri +=  "#secretarioEstadualS#enderecoEstadualS#secretarioEstadualE#endedecoEstadualE";
				camposObri +=  "#nomeSecretariaEstadualE#emailEstadualE#telefoneEstadualE#cargoEstadualE#nomeSecretariaEstadualS#emailEstadualS#telefoneEstadualS#cargoEstadualS";
			}
			
			camposObri += "#secretarioMunicipalS#enderecoMunicipalS#secretarioMunicipalE#enderecoMunicipalE#nomeSecretariaMunicipalE#emailMunicipalE#telefoneMunicipalE#cargoMunicipalE#nomeSecretariaMunicipalS#emailMunicipalS#telefoneMunicipalS#cargoMunicipalS";
	
	
			tiposCamposObri = "#radio#select#select";
			
			//se for capital 
			if(document.getElementById('capital').value == 'true')
			{
				tiposCamposObri = tiposCamposObri + "#texto#texto#texto#texto";
				tiposCamposObri = tiposCamposObri + "#texto#texto#texto#texto#texto#texto#texto#texto";
			}
			tiposCamposObri = tiposCamposObri + "#texto#texto#texto#texto#texto#texto#texto#texto#texto#texto#texto#texto";
			
	
			if(!validaForm('formunicipio',camposObri,tiposCamposObri,false))
				return false;
		
		/* ------- FIM VALIDANDO CAMPOS OBRIGATÓRIOS ----- */
		
		/* ------- VALIDANDO OS EMAILS ----- */
		
			//se for capital 
			
			if(document.getElementById('capital').value == 'true' )
			{
				if(!verificaEmail(document.getElementById('emailrepsecestedu')))
					return false;
				
				if(!verificaEmail(document.getElementById('emailrepsecestsaude')))
					return false;
			}		
				
			if(!verificaEmail(document.getElementById('sememailrepresecretariaeducacao')))
				return false;
				
			if(!verificaEmail(document.getElementById('sememailrepresecretariasaude')))
				return false;
			
		/* ------- FIM VALIDANDO OS EMAILS ----- */
		document.getElementById('cadmunicipio').value = true;

		
		var myAjax = new Ajax.Request('pse.php?modulo=principal/identificacaoEstadoMunicipio&acao=A', {
				        method:     'post',
				        parameters:  $('formunicipio').serialize(),
				        onComplete: function (res){	
				       		
							//alert(res.responseText);
							//$('divTeste').update(res.responseText);
							//return false;
				       		alert('Dados salvos com sucesso');
				        }
				  });
	}
	
	function salvarSecretarias()
	{		
		if(document.getElementById( 'empquantescolapse2009' ).value == '')
			document.getElementById( 'empquantescolapse2009' ).value = 0;
			
		if((document.getElementById( 'empquantescolapse2009' ).value) != (document.getElementById( 'Escolas' ).options.length))
		{
			alert('Número de escolas selecionadas não conferem com o valor informado');
			document.getElementById( 'empquantescolapse2009' ).focus();
			return false;
		}
		
		var escolas =  "";
		
		for(var i =0 ; i< document.getElementById('Escolas').length; i++ )
		{
			escolas += (document.getElementById('Escolas')[i].value) + "," ;
		}	
		
		var myAjax = new Ajax.Request('pse.php?modulo=principal/pse2009&acao=A', {
				        method:     'post',
				        parameters:  $('formunicipio').serialize() + '&salvarEscolas=true&escolas=' + escolas,
				        onComplete: function (res){	
				       		
				       		if(res.responseText == 0 )
								alert('Dados salvos com sucesso');
							else
								alert(res.responseText + ' está vinculada à questão 03');
				        }
				  });
	}
	
	function validaRepresentante()
	{
		var camposObri 		= "#per_1";
		var tiposCamposObri	= '#radio';
		
		
		if(!validaForm('formunicipio',camposObri,tiposCamposObri,false))
				return false;
				
		document.getElementById('cadsecretarias').value = true;
		
		var secretarias =  "";
		
		for(var i =0 ; i< document.getElementById('secretarias').length; i++ )
		{
			secretarias += (document.getElementById('secretarias')[i].value) + "," ;
		}	
				
		var myAjax = new Ajax.Request('pse.php?modulo=principal/representante&acao=A', {
				        method:     'post',
				        parameters:  $('formunicipio').serialize() + "&secretarias=" + secretarias,
				        onComplete: function (res){	
				       
				        	if(res.responseText == '' )
								alert(res.responseText);
							else
								alert('Dados salvos com sucesso');
				        }
				  });
	}
	
	function gravaGestao01()
	{		
		var atores =  "";
		
		for(var i =0 ; i< document.getElementById('atores').length; i++ )
		{
			atores += (document.getElementById('atores')[i].value) + "," ;
		}	
				
		var myAjax = new Ajax.Request('pse.php?modulo=principal/gestao01&acao=A', {
				        method:     'post',
				        parameters: '&cadgestao01=true' + $('formunicipio').serialize() + '&atores=' + atores,
				        onComplete: function (res){
				       
				        	if(res.responseText == '' )
								alert(res.responseText);
							else
								alert('Dados salvos com sucesso');

				        }
				  });
	}
	
	function validaProjeto()
	{
		var camposObri 		= "#per_6#per_7#per_8#per_9";
		var tiposCamposObri	= '#radio#radio#radio#radio';
		
		
		if(!validaForm('formunicipio',camposObri,tiposCamposObri,false))
				return false;			
		
		var SitSaude =  "";
		var censo2 =  "";
		
		for(var i =0 ; i< document.getElementById('SitSaude').length; i++ )
		{
			SitSaude += (document.getElementById('SitSaude')[i].value) + "," ;
		}
		var myAjax = new Ajax.Request('pse.php?modulo=principal/projetoEstadualMunicipal&acao=A', {
				        method:     'post',
				        parameters:  '&projeto=true' + $('formunicipio').serialize() + "&SitSaude=" + SitSaude,
				        onComplete: function (res){	
				       
				        	if(res.responseText == 1 ) {
								alert('Selecione pelo menos uma situação de saúde.');
								return false;
							} else if(res.responseText == 2 ) {
								alert('Selecione pelo menos uma parceria intersetorial do PSE.');
								return false;
							} else if(res.responseText == 3 ) {
								alert('Selecione pelo menos uma informação do Censo Escolar.');
								return false;
							} else {
								alert('Dados salvos com sucesso');
							}	
				        	window.location.href='/pse/pse.php?modulo=principal/projetoEstadualMunicipal&acao=A';
				        }
				  });
	}
	
	function validaGestao()
	{
		var camposObri 		= "#per_10#per_11#per_12#per_13";
		var tiposCamposObri	= '#radio#radio#radio#radio';
		
		
		if(!validaForm('formunicipio',camposObri,tiposCamposObri,false))
				return false;
				
		var myAjax = new Ajax.Request('pse.php?modulo=principal/gestao&acao=A', {
				        method:     'post',
				        parameters:  '&gestao=true' + $('formunicipio').serialize(),
				        onComplete: function (res){	
				        	
				        	if(res.responseText == 1 ) {
								alert('Selecione pelo menos um complemento do 3.2.');
								return false;
							} else if(res.responseText == 2 ) {
								alert('Selecione pelo menos um complemento do 3.3.');
								return false;
							} else if(res.responseText == 3 ) {
								alert('Selecione pelo menos um complemento do 3.4.');
								return false;
							} else {
								alert('Dados salvos com sucesso');
							}
				        }
				  });
	}
	function validaComponente01()
	{
		var myAjax = new Ajax.Request('pse.php?modulo=principal/componente01&acao=A', {
				        method:     'post',
				        parameters:  '&componente=true' + $('formunicipio').serialize(),
				        onComplete: function (res){	
				       
				        	if(res.responseText == '' )
								alert(res.responseText);
							else
								alert('Dados salvos com sucesso');
				        }
				  });
	}
	function validaComponente02()
	{
		var myAjax = new Ajax.Request('pse.php?modulo=principal/componente02&acao=A', {
				        method:     'post',
				        parameters:  '&componente2=true' + $('formunicipio').serialize(),
				        onComplete: function (res){	

				        	if(res.responseText == '' )
								alert(res.responseText);
							else
								alert('Dados salvos com sucesso');
								
				        }
				  });
	}
	function validaComponente04()
	{
		var myAjax = new Ajax.Request('pse.php?modulo=principal/componente04&acao=A', {
				        method:     'post',
				        parameters:  '&componente4=true' + $('formunicipio').serialize(),
				        onComplete: function (res){	
				       
				        	if(res.responseText == '' )
								alert(res.responseText);
							else
								alert('Dados salvos com sucesso');

				        }
				  });
	}
	
	function validaPSE2009()
	{
		if(document.formunicipio.pamanoreferencia.value == document.formunicipio.portaria.value){
			var camposObri 		= "#pamanoreferencia#espid#codSCNES#nrequip#moeid#nieid#pamquantprevista#pamquantatendida";
			var tiposCamposObri	= '#select#numero#numero#select#select#select#numero#numero';
		} else if(document.formunicipio.pamanoreferencia.value != document.formunicipio.portaria.value){
			var camposObri 		= "#pamanoreferencia#espid#codSCNES#nrequip#moeid#nieid#pamquantprevista";
			var tiposCamposObri	= '#select#numero#numero#select#select#select#numero';
		} 
		if(document.formunicipio.pamanoreferencia.value == document.formunicipio.exercicio.value){
			var camposObri 		= "#pamanoreferencia#espid#codSCNES#nrequip#moeid#nieid#pamquantatendida";
			var tiposCamposObri	= '#select#numero#numero#select#select#select#numero';
		}
		
		if(!validaForm('formunicipio',camposObri,tiposCamposObri,false))
				return false;
		
		var myAjax = new Ajax.Request('pse.php?modulo=principal/pse2009&acao=A', {
				        method:     'post',
				        parameters:  '&cadpse2009=true&' + $('formunicipio').serialize(),
				        onComplete: function (res){
				        	if(res.responseText == 1)
				       		{
				       			alert('Dados já informados');
				       			return;
				       		} else if(res.responseText == 2){
				       			alert('Erro. Verifique os campos.');
				       			return;
				       		} else {
								alert('Dados salvos com sucesso');
							}
							document.formunicipio.espid.value = '';
				       		document.formunicipio.entid.value = '';
				       		document.formunicipio.codSCNES.value = '';
				       		document.formunicipio.nrequip.value = '';
				       		document.formunicipio.moeid.value = '';
				       		document.formunicipio.nieid.value = '';
				       		document.formunicipio.pamquantprevista.value = '';
				       		document.formunicipio.pamquantatendida.value = '';
				       		document.formunicipio.pamId.value = '';
				       		document.formunicipio.idequipe.value = '';
				       		document.getElementById('imgcnes').style.display = '';
				       		document.getElementById('btpesquisa').disabled = false;
				       		carregaListaAno(document.formunicipio.pamanoreferencia.value);
				        }
				        
				  });
	}
	
	function gravaGestao02()
	{
		var camposObri 		= "#parcerias#texto";
		var tiposCamposObri	= '#select#texto';
		
		
		if(!validaForm('formunicipio',camposObri,tiposCamposObri,false))
				return false;

		var myAjax = new Ajax.Request('pse.php?modulo=principal/gestao02&acao=A', {
				        method:     'post',
				        parameters:  '&cadgestao02=true&' + $('formunicipio').serialize(),
				        onComplete: function (res){
				       		
				        	if(res.responseText == 1)
				       		{
				       			alert('Dados já informados');
				       			return;
				       		}else{
								alert('Dados salvos com sucesso');
							}
				       		carregaParcerias(document.formunicipio.parcerias.value);
				       		document.formunicipio.texto.value = '';
				        }
				        
				  });
	}
	
	function gravaGestao03()
	{
		var myAjax = new Ajax.Request('pse.php?modulo=principal/gestao03&acao=A', {
				        method:     'post',
				        parameters:  '&cadgestao03=true&' + $('formunicipio').serialize(),
				        onComplete: function (res){
				       						        	
				        	if(res.responseText == 1)
				       		{
				       			alert('Selecione pelo menos um valor.');
				       			return;
				       		}else{
								alert('Dados salvos com sucesso');
							}
				       	}
				        
				  });
	}
	
	function gravaGestao04()
	{
		var camposObri 		= "#radio1[]#radio2[]";
		var tiposCamposObri	= '#radio#radio';
		
		if(!validaForm('formunicipio',camposObri,tiposCamposObri,false))
				return false;
		
		var myAjax = new Ajax.Request('pse.php?modulo=principal/gestao04&acao=A', {
				        method:     'post',
				        parameters:  '&cadgestao04=true&' + $('formunicipio').serialize(),
				        onComplete: function (res){
				       		
				        	if(res.responseText == 1)
				       		{
				       			alert('Selecione pelo menos um valor.');
				       			return;
				       		}else{
								alert('Dados salvos com sucesso');
							}
				       	}
				        
				  });
	}
	
	function gravaGestao05()
	{
		var camposObri 		= "#per_6#per_8";
		var tiposCamposObri	= '#radio#radio';
		
		if(!validaForm('formunicipio',camposObri,tiposCamposObri,false))
				return false;
		
		var atores =  "";
		
		for(var i =0 ; i< document.getElementById('atores').length; i++ )
		{
			atores += (document.getElementById('atores')[i].value) + "," ;
		}	
				
		var myAjax = new Ajax.Request('pse.php?modulo=principal/gestao05&acao=A', {
				        method:     'post',
				        parameters: '&cadgestao05=true' + $('formunicipio').serialize() + '&atores=' + atores,
				        onComplete: function (res){
				        	
				        	if(res.responseText == 1)
				       		{
				       			alert('Selecione pelo menos um valor na pergunta 7.1.');
				       			return;
				       		}else{
								alert('Dados salvos com sucesso');
							}
				       	}
				  });
	}
	
	function gravaReconhecimento04()
	{
		var camposObri 		= "#per_28";
		var tiposCamposObri	= '#radio';
		
		if(!validaForm('formunicipio',camposObri,tiposCamposObri,false))
				return false;
		
				
		var myAjax = new Ajax.Request('pse.php?modulo=principal/reconhecimento04&acao=A', {
				        method:     'post',
				        parameters: '&reconhecimento04=true' + $('formunicipio').serialize(),
				        onComplete: function (res){
				        	
				        	if(res.responseText == 1)
				       		{
				       			document.formunicipio.tultexto.value = '';
				       		}
				        	
				        	if(res.responseText)
				       		{
				       			alert('Dados salvos com sucesso');
				       		}else{
								alert('Erro na operação');
				       			return;
							}
				       	}
				        
				  });
	}
	
	function verificaEmail(campo)
	{
		if(!validaEmail(campo.value))
		{
			alert(campo.title + " é inválido");
			campo.focus();
			return false;
		}
		
		return true;
	}
	
	function somaCampos(id)
	{
		if(id==1){
			valor1 = new Number(document.formunicipio.per_1.value);
			valor2 = new Number(document.formunicipio.per_2.value);
			valor3 = new Number(document.formunicipio.per_3.value);
			valor4 = new Number(document.formunicipio.per_4.value);
			valor5 = new Number(document.formunicipio.per_5.value);
			valor6 = new Number(document.formunicipio.per_6.value);
			valor7 = new Number(document.formunicipio.per_7.value);
			valor8 = new Number(document.formunicipio.per_8.value);
			valor9 = new Number(document.formunicipio.per_9.value);
			valor10 = new Number(document.formunicipio.per_10.value);
			valor11 = new Number(document.formunicipio.per_11.value);
			valor12 = new Number(document.formunicipio.per_12.value);
			valor13 = new Number(document.formunicipio.per_13.value);
			document.formunicipio.total1.value = new Number(valor1 + valor2 + valor3 + valor4 + valor5);
			document.formunicipio.total2.value = new Number(valor6 + valor7 + valor8 + valor9);
			document.formunicipio.total3.value = new Number(valor10 + valor11 + valor12 + valor13);
//			document.formunicipio.total.value = new Number(valor1 + valor2 + valor3 + valor4 + valor5 + valor6 + valor7 + valor8 + valor9 + valor10 + valor11 + valor12 + valor13);
		}
		if (id==2){
			valor14 = new Number(document.formunicipio.per_14.value);
			valor15 = new Number(document.formunicipio.per_15.value);
			valor16 = new Number(document.formunicipio.per_16.value);
			valor17 = new Number(document.formunicipio.per_17.value);
			valor18 = new Number(document.formunicipio.per_18.value);
			document.formunicipio.total.value = new Number(valor14 + valor15 + valor16 + valor17 + valor18);
		}
		if (id==3){
			valor19 = new Number(document.formunicipio.per_19.value);
			valor20 = new Number(document.formunicipio.per_20.value);
			valor21 = new Number(document.formunicipio.per_21.value);
			document.formunicipio.total.value = new Number(valor19 + valor20 + valor21);
		}
		if (id==4){
			document.formunicipio.total.value = document.formunicipio.per_22.value;
		}
	}
		
	function filtraMunicipio(estuf) {
	
	if(!estuf)
	{
		document.getElementById("muncod").value = '';
		document.getElementById("muncod").disabled=true;
		return false;
	}
	
	document.getElementById("estcod").value = estuf;
	var destino = document.getElementById("td_municipio");
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: "filtraMunicipio=true&" + "estuf=" + estuf,
			asynchronous: false,
			onComplete: function(resp) {
				if(destino) {
					destino.innerHTML = resp.responseText;
				} 
			},
			onLoading: function(){
				destino.innerHTML = 'Carregando...';
			}
		});
	}
	
	function listarAtributoFormulario()
	{
		var esf2009 		= document.getElementById('esf2009');
		var inep2009 		= document.getElementById('inep2009');
		var modalidade2009 	= document.getElementById('idmodalidade2009');
		var nivel2009 		= document.getElementById('idnivel2009');
		var previsto2009 	= document.getElementById('previsto2009');
		var atendido2009 	= document.getElementById('atendido2009');
		
		var string = "&esf=" + esf2009.value + "&inep=" + inep2009.value + "&modalidade=" + modalidade2009.value + "&nivel=" + nivel2009.value + "&previsto=" + previsto2009.value + "&atendido=" + atendido2009.value;
		
		var myAjax = new Ajax.Request('pse.php?modulo=principal/CadastroEstadoMunicipiof&acao=A', {
					        method:     'post',
					        parameters: '&listarAtributoFormulario=true' + string ,
					        onComplete: function (res){	
								$('lista').innerHTML = res.responseText;
					        }
					  });
	}
	
	function verificaCapital(value)
	{
        
		var myAjax = new Ajax.Request('pse.php?modulo=principal/CadastroEstadoMunicipiof&acao=A', {
					        method:     'post',
					        parameters: '&verificaCapital=true&capital=' + value,
					        onComplete: function (res){
					        	
					        	var capital = false;
					        	document.getElementById('capital').value = false;
					        	
					        	if(res.responseText == 'capital')
					        	{
					        		document.getElementById('capital').value = true;
					        		capital = true;	
					        	}
					        		
					        	validaItem(capital,'dadosOpcionais[]');} 
					        });
	}
	
	function validaItem(value,item)
	{
		var boExibe = ( value == '1' ) ? '' : 'none';
		var arLinhas = getElementsByName_iefix( 'tr', item );
	        
        for( i=0; i<arLinhas.length; i++ ){
        	arLinhas[i].style.display = boExibe;
      	} 
	}
	

	function getElementsByName_iefix(tag, name) {  
	       
		var elem = document.getElementsByTagName(tag);  
		var arr = new Array();  
	    for(i = 0,iarr = 0; i < elem.length; i++) {  
			att = elem[i].getAttribute("name");  
			if(att == name) {  
				arr[iarr] = elem[i];  
				iarr++;  
			}  
		}  
		return arr;  
	} 
	
	function salvaSecretarias()
	{
	
		validaFormulario(true);
	}
	
	function alterarFormulario(idItem,pag){
		var myAjax = new Ajax.Request('pse.php?modulo=principal/pse2009&acao=A', {
							method 	   	 : 'post',
							parameters 	 : '&alteraForm=true&idItem='+ idItem,
							asynchronous : false,
							onComplete   : function(res){
							
							
								dados = res.responseText;
		   	    			 	dados = dados.split('|');
				       			document.formunicipio.entid.value = dados[7];
				       			document.formunicipio.espid.value = dados[10];
				 	      		document.formunicipio.codSCNES.value = dados[9];
				   	    		document.formunicipio.pamquantprevista.value = dados[5];
				   	    		document.formunicipio.pamquantatendida.value = dados[6];
				   	    		document.formunicipio.pamId.value = dados[8];
				   	    		document.formunicipio.idequipe.value = dados[2];
				   	    		document.getElementById('btpesquisa').disabled = true;
				   	    		verEquipes(dados[9],dados[2]);
				   	    		
								if( document.getElementById('novoempid').value != document.getElementById('empid').value ){
					   	    		document.getElementById('imgcnes').style.display = 'none';
					   	    		document.getElementById('nrequip').disabled = true;
					   	    		document.getElementById('moeid').disabled = true;
					   	    		document.getElementById('nieid').disabled = true;
					   	    		document.getElementById('pamquantprevista').readOnly = true;
	       							document.getElementById('pamquantprevista').className = 'disabled';
								}

				   	    		
				   	    		var select = new Array();
								select[3] = 'moeid'; 
								select[4] = 'nieid';
								
								var i = 3;
								while( i <= select.length ){
									var elemento = document.getElementById(select[i]);
									for (a=0; a < elemento.options.length; a++){
										if (dados[i] == elemento.options[a].value){
											elemento.selectedIndex = a;
											continue;	
										}
									}
									i++;
								}
							}
						});
	}
	
	function excluiFormulario(idItem,pag)
	{
		if(confirm("Tem certeza que deseja excluir este formulário?")){
			var myAjax = new Ajax.Request('pse.php?modulo=principal/'+pag+'&acao=A', {
							method 	   	 : 'post',
							parameters 	 : 'excluiFormularioAjax=true&idItem='+ idItem,
							asynchronous : false,
							onComplete   : function(res){
								
								if(res.responseText)
								{
									var ano = document.getElementById('pamanoreferencia').value;
									
									alert("Registro excluído com sucesso!");
									
									var myAjax = new Ajax.Request('pse.php?modulo=principal/pse2009&acao=A', {
									        method:     'post',
									        parameters: '&excluiFormularioAjax=false&carregaLista=true&ano=' + ano,
									        onComplete: function (res){	
												$('lista').innerHTML = res.responseText;
									        }
									  });
								}
								else
								{
									alert("Erro ao excluir o registro!");
								}
							}
						});
				carregaListaAno(document.formunicipio.pamanoreferencia.value);
				document.formunicipio.espid.value = '';
				document.formunicipio.entid.value = '';
				document.formunicipio.codSCNES.value = '';
				document.formunicipio.nrequip.value = '';
				document.formunicipio.moeid.value = '';
				document.formunicipio.nieid.value = '';
				document.formunicipio.pamquantprevista.value = '';
				document.formunicipio.pamquantatendida.value = '';
				document.formunicipio.pamId.value = '';
				document.formunicipio.idequipe.value = '';
				document.getElementById('imgcnes').style.display = '';
				document.getElementById('btpesquisa').disabled = false;
		}
	
	}
	
	function excluiParceria(idItem, isuid)
	{
		if(confirm("Tem certeza que deseja excluir este formulário?")){
			var myAjax = new Ajax.Request('pse.php?modulo=principal/gestao02&acao=A', {
							method 	   	 : 'post',
							parameters 	 : 'excluiFormularioAjax=true&idItem='+ idItem + '&isuid='+ isuid,
							asynchronous : false,
							onComplete   : function(res){
								
								if(res.responseText)
								{
									var ano = document.getElementById('pamanoreferencia').value;
									
									alert("Registro excluído com sucesso!");
									
									var myAjax = new Ajax.Request('pse.php?modulo=principal/gestao02&acao=A', {
									        method:     'post',
									        parameters: '&excluiFormularioAjax=false&carregaLista=true',
									        onComplete: function (res){	
												$('lista').innerHTML = res.responseText;
									        }
									  });
								}
								else
								{
									alert("Erro ao excluir o registro!");
								}
							}
						});
			carregaParcerias(document.formunicipio.parcerias.value);
		}
	
	}
	
	function checkParcerias()
	{	
		rulid = document.formunicipio.rulid.value;
		if(document.formunicipio.checkparcerias.checked){
			if(confirm("Se você marcar esta opção, todos os dados já\ncadastrados serão excluidos! Deseja continuar?")){
				document.formunicipio.parcerias.value='';
				document.formunicipio.texto.value='';
				document.formunicipio.parcerias.disabled = true;
				document.formunicipio.texto.disabled = true;
				document.formunicipio.btsalvar.disabled = true;
				
				var myAjax = new Ajax.Request('pse.php?modulo=principal/gestao02&acao=A', {
								method 	   	 : 'post',
								parameters 	 : 'excluiTudo=true',
								asynchronous : false,
								onComplete   : function(res){
									
									if(res.responseText)
									{
										alert("Registro excluído com sucesso!");
									}
									else
									{
										alert("Erro ao excluir o registro!");
									}
								}
							});
				carregaParcerias(0);
			} else {
				document.formunicipio.checkparcerias.checked = false;
			}
		} else {
			document.formunicipio.parcerias.disabled = false;
			document.formunicipio.texto.disabled = false;
			document.formunicipio.btsalvar.disabled = false;
		}
	}
	
	function pesquisaINEP(ano, exercicio)
	{
		if( ano != 0 ){
			if (ano==exercicio)
			{
				window.open('/pse/pse.php?modulo=principal/consultaINEP&acao=A&ano=true','','width=800,height=600,scrollbars=1');
			} else {
				window.open('/pse/pse.php?modulo=principal/consultaINEP&acao=A','','width=800,height=600,scrollbars=1');
			}
		} else {
			alert("Selecione o Ano de Referência.");
		}
	}
	
	function selecionaESC()
	{
		window.open('/pse/pse.php?modulo=principal/selecionaESC&acao=A','','width=800,height=600,scrollbars=1');
	}
	
	function gravaEsc(entid, empid)
	{
		campo = document.getElementById( 'check_' + entid );
		if (campo.checked){
			var myAjax = new Ajax.Request('pse.php?modulo=principal/selecionaESC&acao=A', {
		        method:     'post',
		        parameters: '&grava=true&entid='+entid+'&empid='+empid ,
		        onComplete: function (res){	
					qntpse2009 = Number(eval(window.opener.document.getElementById('empquantescolapse2009')).value);
					soma = (qntpse2009 + 1);
					window.opener.document.getElementById('empquantescolapse2009').value = soma;
		        }
		  });
		} else {
			var myAjax = new Ajax.Request('pse.php?modulo=principal/selecionaESC&acao=A', {
		        method:     'post',
		        parameters: '&deleta=true&entid='+entid+'&empid='+empid ,
		        onComplete: function (res){
					if(res.responseText == 1){
						alert('Esta escola já está vinculada à questão 03.\nVocê precisa remover o registro primeiro.');
						document.getElementById( 'check_' + entid ).checked = true;
					} else {
						qntpse2009 = Number(eval(window.opener.document.getElementById('empquantescolapse2009')).value);
						soma = (qntpse2009 - 1);
						window.opener.document.getElementById('empquantescolapse2009').value = soma;
					}
		        }
		  });
		}
	}
	
	function carregaESF(entid)
	{
		var myAjax = new Ajax.Request('pse.php?modulo=principal/pse2009&acao=A', {
					        method:     'post',
					        parameters: '&entid=' + entid ,
					        onComplete: function (res){	
								$('campoESF').innerHTML = res.responseText;
					        }
					  });
	}
	
	function selecionaINEP(entcodent,entid)
	{
		window.opener.document.getElementById('espid').value = entcodent;
		window.opener.document.getElementById('entid').value = entid;
		window.opener.carregaESF(entid);
		window.close();
	}
	
	function carregaListaAno(ano)
	{		
		
		window.location.href='/pse/pse.php?modulo=principal/pse2009&acao=A&ano='+ano;
		return true;
		
		var myAjax = new Ajax.Request('pse.php?modulo=principal/pse2009&acao=A', {
					        method:     'post',
					        parameters: '&ano=' + ano + '&carregaLista=true' ,
					        onComplete: function (res){	
								$('lista').innerHTML = res.responseText;
								validaItem(x,'item2[]');
					        }
					  });
	}
	
	function carregaParcerias(parcerias)
	{	
		var myAjax = new Ajax.Request('pse.php?modulo=principal/gestao02&acao=A', {
					        method:     'post',
					        parameters: '&parcerias=' + parcerias + '&carregaParcerias=true' ,
					        onComplete: function (res){	
								$('lista').innerHTML = res.responseText;
					        }
					  });
	}
		
	function salvarGestor()
	{
		var camposObri 		= "#per_2#per_3#per_4#per_5";
		var tiposCamposObri	= '#radio#radio#radio#radio';
		
		
		if(!validaForm('formunicipio',camposObri,tiposCamposObri,false))
				return false;
	
	
		var myAjax = new Ajax.Request('pse.php?modulo=principal/grupoGestor&acao=A', {
				        method:     'post',
				        parameters:  '&grupoGestor=true' + $('formunicipio').serialize(),
				        onComplete: function (res){	

				        	if(res.responseText == '' )
								alert(res.responseText);
							else
								alert('Dados salvos com sucesso');	
				        }
				  });
	}
	
	function salvarEducacao()
	{
		var camposObri 		= "#per_14#per_15#per_16#per_17";
		var tiposCamposObri	= '#radio#radio#radio#radio';
		
		
		if(!validaForm('formunicipio',camposObri,tiposCamposObri,false))
				return false;
				
		var myAjax = new Ajax.Request('pse.php?modulo=principal/educacao&acao=A', {
				        method:     'post',
				        parameters:  '&educacao=true' + $('formunicipio').serialize(),
				        onComplete: function (res){	

				        	if(res.responseText == '' )
								alert(res.responseText);
							else
								alert('Dados salvos com sucesso');	
				        }
				  });
	}
	
	function navega(caminho){
		window.location.href='/pse/pse.php?modulo=principal/' + caminho + '&acao=A';
	
	}				
