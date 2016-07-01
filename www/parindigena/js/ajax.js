/*** (INÍCIO) AJAX REQUESTS ***/

function carregaGrupos(url,data) {
	var aj = new Ajax.Request(  
	url, {  
	 method:'get',   
	 parameters: data,   
	 onComplete: getResponseCarregaGrupos
	 }  
	);  
}

function carregaConvenios(url,data) {
	var aj = new Ajax.Request(  
	url, {  
	 method:'get',   
	 parameters: data,   
	 onComplete: getResponseCarregaConvenios
	 }  
	);  
}

function carregaItens(url,data) {
	var aj = new Ajax.Request(  
	url, {  
	 method:'get',   
	 parameters: data,   
	 onComplete: getResponseCarregaItens
	 }  
	);  
}

function carregaEtapas(url,data) {
 
	var aj = new Ajax.Request(  
	url, {  
	 method:'get',   
	 parameters: data,   
	 onComplete: getResponseCarregaEtapas
	 }  
	);  
}

function carregaObras(url,data, index, id, cod){
	
	if( document.getElementById( id.substr(0,2) + '_obras_' + cod ) ){
		
		document.getElementById( id.substr(0,2) + '_obras_' + cod ).style.display = "";
		
	}else{
	
		tr = $('tabela').insertRow(++index);
		tr.id = id.substr(0,2) + '_obras_' + cod;
		 
		td1 = tr.insertCell(0);
		td2 = tr.insertCell(1);
		
		td2.style.textAlign = "center";
		td2.colSpan = "4";
		
		var aj = new Ajax.Request(  
		url, {  
		 method:'get',   
		 parameters: data,   
		 onComplete: function (resp){
		 		
		 		td2.innerHTML = resp.responseText;
		 		
		 	} 
		 }  
		);
	
	}
		
}

function carregaBarraExecucao(cel,data) {
	var aj = new Ajax.Request('ajax.php',  
	{  
		method: 'get',   
		parameters: data,   
		onComplete: function(r)
		{
			array = r.responseText.split('@@');
			cel.innerHTML = array[0];
			cel.status = array[1];
			cel.percentual = array[2];
		}
	});
}

function carregaDataInicioTermino(celinicio,celtermino,data) {
	var aj = new Ajax.Request('ajax.php',  
	{  
		method: 'get',   
		parameters: data,   
		onComplete: function(r)
		{
			array = r.responseText.split('@');
			celinicio.innerHTML = array[0];
			celtermino.innerHTML = array[1];
			
			var objDate = strDateToObjDate( array[1] , 'd/m/Y' , '/' );
			var objToday = new Date();
			
			if( objDate <= objToday ) {
				celtermino.style.color = '#ff2020';
				celtermino.style.fontWeight = 'bold';
			}
		}
	});
}

function atualizaBarraStatus(intBarraStatusId, strStatus, intStatus, intPercentual) {
	var id = intBarraStatusId.replace('td_', '');
	var data = 'tipo=atualiza_barra_status&id='+id+'&codstatus='+intStatus+'&percentual='+intPercentual+'';
	
	var aj = new Ajax.Request('ajax.php',  
	{  
		method: 'get',   
		parameters: data,   
		onComplete: function(r)
		{
			if(r.responseText)
				aposAtualizarBarraStatus(intBarraStatusId, strStatus, intStatus, intPercentual);
			else
				alert("Erro ao atualizar a Situação.");
		}
	});
}

function alteraDataItem( id , strDataAlterada , strNovaData, strDataAntiga ) {
	var data = 'tipo=atualiza_data_item&id='+id+'&data_alterada='+strDataAlterada+'&nova_data='+strNovaData+'';
	
	var aj = new Ajax.Request('ajax.php',  
	{  
		method: 'get',   
		parameters: data,   
		onComplete: function(r)
		{
			if(r.responseText) {
				aposAlterarDataItem( id , strDataAlterada , strNovaData );
			} else {
				alert("Erro ao atualizar a Data.");
				aposAlterarDataItem( id , strDataAlterada , strDataAntiga );	
			}
		}
	});
}
/*** (FIM) AJAX REQUESTS ***/

/*** (INÍCIO) AJAX RESPONSES ***/
function getResponseCarregaGrupos(oReq) {
  if(oReq.responseText.search("@") >= 0) {
	arrayGrupos = oReq.responseText.split('@');
	idGrupos =  oReq.responseText.split('&');
	indexLinhaUF = document.getElementById(arrayGrupos[0]).rowIndex;
	
	corLinhaUF = document.getElementById(arrayGrupos[0]).style.backgroundColor;
	if(corLinhaUF == "rgb(250, 250, 250)")
		cor = "#fafafa";
	else
		cor = "#f0f0f0";
	
	for(i=1; i < arrayGrupos.length; i++) {
		arrayAux = arrayGrupos[i].split('&');
		
		tr = $('tabela').insertRow(++indexLinhaUF);
		tr.id = arrayGrupos[0]+"_"+arrayAux[0];
		
		if(cor == "#fafafa") {
			tr.style.backgroundColor = "#f0f0f0";
			cor = "#f0f0f0";
		}
		else {
			tr.style.backgroundColor = "#fafafa";
			cor = "#fafafa";
		}
			
		//td1 = tr.insertCell(0);
		//td1.style.textAlign = "center";
		//td1.innerHTML = "<img src=\"../imagens/gif_inclui.gif\" style=\"border:0; cursor:pointer;\" title=\"Incluir um convênio\">";
		
        
        arrayMax= arrayGrupos[i].split('&');
        expAr = explode(",",arrayMax);
        
        
        //alert(expAr[0]);
        grpid = idGrupos[3];

        var estuf    = arrayGrupos[0];
        var covcod   = arrayAux[0];
        var vazio    = expAr[2];
        
         if(expAr[3] == 1){
        	imgClipes = "&nbsp;<img src=\"../imagens/clipe1.gif\" onClick=\"abrirPopupVincularConvenio("+expAr[0]+",'"+estuf+"');\" style=\"border:0; cursor:pointer;\" title=\"Vincular Subações Formação Inicial\">";        	
        } else {
        	imgClipes = '';
        }
        
        switch(expAr[0])
        {
            case '1':
                
                td1 = tr.insertCell(0);
                td1.style.textAlign = "center";
                td1.innerHTML = "<img src=\"../imagens/gif_inclui.gif\" onClick=\"abrirPopupFormacaoInicialNew("+expAr[0]+",'"+estuf+"');\" style=\"border:0; cursor:pointer;\" title=\"Incluir um item de Formaçao Inicial\">"+imgClipes;
                                "&nbsp;<img src=\"../imagens/excluir.gif\" style=\"border:0; cursor:pointer;\" title=\"Excluir o item\">";
            break;
            
            case '2':
              td1 = tr.insertCell(0);
              td1.style.textAlign = "center";
              td1.innerHTML = "<img src=\"../imagens/gif_inclui.gif\" onClick=\"abrirPopupFormacaoContinuadaNew("+expAr[0]+",'"+estuf+"');\" style=\"border:0; cursor:pointer;\" title=\"Incluir um item de Formaçao Continuada\">"+imgClipes;
                                "&nbsp;<img src=\"../imagens/excluir.gif\" style=\"border:0; cursor:pointer;\" title=\"Excluir o item\">";
            break;
            
            
            case '5':
                if( ( estuf == 'PA' ) || ( estuf == 'TO' ) )
                {
                    
                   td1 = tr.insertCell(0);
                   td1.style.textAlign = "center";
                   td1.innerHTML = "<img src=\"../imagens/gif_inclui.gif\" onClick=\"abrirPopupEnsinoMedio("+expAr[0]+",'"+estuf+"');\" style=\"border:0; cursor:pointer;\" title=\"Incluir um Curso para Ensino Médio Integrado\">"+imgClipes;
                                   "&nbsp;<img src=\"../imagens/excluir.gif\" style=\"border:0; cursor:pointer;\" title=\"Excluir o item\">";
                 }
            break;
            
            case '3':
               td1 = tr.insertCell(0); 
               td1.style.textAlign = "center";
               td1.innerHTML = "<img src=\"../imagens/gif_inclui.gif\" onClick=\"abrirPopupMaterialDidaticoNew("+expAr[0]+",'"+estuf+"');\" style=\"border:0; cursor:pointer;\" title=\"Incluir um item de Material Didático\">"+imgClipes;
                                "&nbsp;<img src=\"../imagens/excluir.gif\" style=\"border:0; cursor:pointer;\" title=\"Excluir o item\">";
            break;
            
            case '4':
               td1 = tr.insertCell(0);
               td1.style.textAlign = "center";
               td1.innerHTML = "";
                               
            break;
            
            
        }
        
                
		td2 = tr.insertCell(1);
        if( vazio == 'F')
        {
		td2.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\">"+
						"<a href=\"javascript:void(0);\" onclick=\"alteraIcone('"+arrayGrupos[0]+"_"+arrayAux[0]+"', 2);\">"+
						"<img id=\"img_"+arrayGrupos[0]+"_"+arrayAux[0]+"\" src=\"../imagens/mais.gif\" border=\"0\"></a>&nbsp;&nbsp;<b>"+arrayAux[1]+"</b>";
		}
        else
        {
        	
        	if( expAr[0] == 4 ){
        	
        		td2.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\">"+
								"<a href=\"javascript:void(0);\" onclick=\"alteraIcone('"+arrayGrupos[0]+"_"+arrayAux[0]+"', 5);\">"+
								"<img id=\"img_"+arrayGrupos[0]+"_"+arrayAux[0]+"\" src=\"../imagens/mais.gif\" border=\"0\"></a>&nbsp;&nbsp;<b>"+arrayAux[1]+"</b>";
								
        	}else{
        	
	        	td2.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\">"+
						    	"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>"+arrayAux[1]+"</b>";
			
			}
			
        }
            
            
		td3 = tr.insertCell(2);
		td3.innerHTML = "&nbsp;";
		
		td4 = tr.insertCell(3);
		td4.innerHTML = "&nbsp;";
		
		td5 = tr.insertCell(4);
		td5.innerHTML = "&nbsp;";
	}
  }
  else {
  	document.getElementById('img_'+oReq.responseText).src = "../imagens/mais.gif";
  }
}

function getResponseCarregaConvenios(oReq) {
  if(oReq.responseText.search("@") >= 0) {
	arrayConvenios = oReq.responseText.split('@');
	
	indexLinhaGrupo = document.getElementById(arrayConvenios[0]).rowIndex;
	
	corLinhaGrupo = document.getElementById(arrayConvenios[0]).style.backgroundColor;
	if(corLinhaGrupo == "rgb(250, 250, 250)")
		cor = "#fafafa";
	else
		cor = "#f0f0f0";
   
		
	for(i=1; i < arrayConvenios.length; i++) {
        arrayAux = arrayConvenios[i].split('&');
        var covcod = arrayAux[0];
        
        var arrestuf = arrayConvenios[0];
        var estufarr = explode("_", arrestuf);
        var estuf = estufarr[0];
        var grpid = arrayConvenios[0].substr(3);
        
		switch(grpid)
		{
            case '1':
            
				var str = 'title=\"Incluir um item de Formaçao Inicial p/ este convênio\" '+
						  'onclick=\"window.open(\'?modulo=principal/popupCadFormacaoInicial&acao=I'+
						  '&covcod='+covcod+'&grpid='+grpid+'&estuf='+estuf+'\',\'blank\',\'height=850,width=950,status=yes,toolbar=no '+
						  'menubar=no,scrollbars=yes,location=no,resizable=yes\');\"';
				break;
			case '2':
            
				var str = 'title=\"Incluir um item de Formaçao Continuada p/ este convênio\" '+
						  'onclick=\"window.open(\'?modulo=principal/popupCadFormContinuada&acao=I'+
						  '&covcod='+covcod+'&grpid='+grpid+'&estuf='+estuf+'\',\'blank\',\'height=850,width=950,status=yes,toolbar=no,'+
						  'menubar=no,scrollbars=yes,location=no,resizable=yes\');\"';
				break;
				
			case '5':
               if( ( estuf == 'PA' ) || ( estuf == 'TO' ) )
               {
                   var str = 'title=\"Incluir um item de Ensino Médio Integrado p/ este convênio\" '+
                          'onclick=\"window.open(\'?modulo=principal/popupCadEnsinoMedio&acao=I'+
                          '&covcod='+covcod+'&grpid='+grpid+'&estuf='+estuf+'\',\'blank\',\'height=850,width=950,status=yes,toolbar=no,'+
                          'menubar=no,scrollbars=yes,location=no,resizable=yes\');\"';
				}
               break;
               	
			case '3':
         
				var str = 'title=\"Incluir um item de Material Didático p/ este convênio\" '+
						  'onclick=\"window.open(\'?modulo=principal/popupCadMaterial&acao=I'+
						  '&covcod='+covcod+'&grpid='+grpid+'&estuf='+estuf+'\',\'blank\',\'height=850,width=950,status=yes,toolbar=no,'+
						  'menubar=no,scrollbars=yes,location=no,resizable=yes\');\"';				
				break;
			
			/*		
			case '4':
               var str = 'title=\"Incluir um item de Obras\" '+
                                 'onclick=\"window.open(\'?modulo=principal/cadastro_obras_indigenas&acao=A'+
                                 '&covcod='+arrayAux[0]+'\',\'obra\',\'height=700,width=600,status=yes,toolbar=no,'+
                                 'menubar=no,scrollbars=yes,location=no,resizable=yes\');\"';
               break;
            */     
                
            
             
		}
		
		
		
		tr = $('tabela').insertRow(++indexLinhaGrupo);
		tr.id = arrayConvenios[0]+"_"+arrayAux[0];
		
		if(cor == "#fafafa") {
			tr.style.backgroundColor = "#f0f0f0";
			cor = "#f0f0f0";
		}
		else {
			tr.style.backgroundColor = "#fafafa";
			cor = "#fafafa";
		}
			
		td1 = tr.insertCell(0);
		td1.style.textAlign = "center";
		td1.innerHTML = "<img src=\"../imagens/gif_inclui.gif\"style=\"border:0; cursor:pointer;\" "+str+">"+
						"&nbsp;<img src=\"../imagens/consultar.gif\" style=\"border:0; cursor:pointer;\"  onclick=\"abrirPopupConvenio("+arrayAux[0]+");\"  title=\"Visualizar detalhes do convênio\">";
		
   
        
        td2 = tr.insertCell(1);
        td2.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\">"+
                        "<a href=\"javascript:void(0);\" onclick=\"alteraIcone('"+arrayConvenios[0]+"_"+arrayAux[0]+"', 3);\">"+
                        "<img id=\"img_"+arrayConvenios[0]+"_"+arrayAux[0]+"\" src=\"../imagens/mais.gif\" border=\"0\"></a>&nbsp;&nbsp;<b><a href=\"javascript: abrirPopupConvenio("+arrayAux[0]+");\">Convênio nº "+arrayAux[1]+"</a></b>";
                        if( arrayAux[2] == 'P')
                        {  
                         td2.innerHTML += "<a href=\"javascript: abrirPopupConvenio("+arrayAux[0]+");\"><img src=\"../imagens/exclama.gif\" border=\"0\" title=\"Existem valores não informados para este convênio\" /></a>";
                        }

                        
		td3 = tr.insertCell(2);
		td3.innerHTML = "&nbsp;";
		
		td4 = tr.insertCell(3);
		td4.innerHTML = "&nbsp;";
		
		td5 = tr.insertCell(4);
		td5.innerHTML = "&nbsp;";
	}
  }
  else {
  	document.getElementById('img_'+oReq.responseText).src = "../imagens/mais.gif";
  }
}

function getResponseCarregaItens(oReq) {
  if(oReq.responseText.search("@") >= 0) {
	arrayItens = oReq.responseText.split('@');
	

	indexLinhaConvenio = document.getElementById(arrayItens[0]).rowIndex;
	
	corLinhaConvenio = document.getElementById(arrayItens[0]).style.backgroundColor;
	if(corLinhaConvenio == "rgb(250, 250, 250)")
		cor = "#fafafa";
	else
		cor = "#f0f0f0";
		
	for(i=1; i < arrayItens.length; i++) {
		arrayAux = arrayItens[i].split('&');
		
		tr = $('tabela').insertRow(++indexLinhaConvenio);
		tr.id = arrayItens[0]+"_"+arrayAux[0];
		
		if(cor == "#fafafa") {
			tr.style.backgroundColor = "#f0f0f0";
			cor = "#f0f0f0";
		}
		else {
			tr.style.backgroundColor = "#fafafa";
			cor = "#fafafa";
		}
			
		td1 = tr.insertCell(0);
		td1.style.textAlign = "center";
        
                                          

        var exp = explode("_", arrayItens);
        var grupoId = exp[1];  
        var friid = arrayAux[0];
        var frcid = arrayAux[0];
        var madid = arrayAux[0];
        var obrid = arrayAux[0];
        var estuf = exp[0]; 
		
        var arr_ = explode("_", arrayItens[0]);
        var covcod = arr_[2];
        
        //alert(arrayAux[2]);
        
        switch(grupoId)
        {
            case '1':
                td1.innerHTML = "<img src=\"../imagens/alterar.gif\" onClick=\"abrirPopupFormacaoInicial("+friid+",'"+estuf+"');\" style=\"border:0; cursor:pointer;\" title=\"Alterar o item\">"+
                                "&nbsp;<img src=\"../imagens/excluir.gif\" onClick=\"excluirItem("+friid+", 'fi');\" style=\"border:0; cursor:pointer;\" title=\"Excluir o item\">";
          
                td2 = tr.insertCell(1);
                if( arrayAux[2] == 'F' )
                {
                td2.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+
                                "<img src=\"../imagens/seta_filho.gif\">&nbsp;&nbsp;"+
                                "<a href=\"javascript:void(0);\" onclick=\"alteraIcone('"+arrayItens[0]+"_"+arrayAux[0]+"', 4);\">"+
                                "<img id=\"img_"+arrayItens[0]+"_"+arrayAux[0]+"\" src=\"../imagens/mais.gif\" border=\"0\" \></a>"+
                                "<b>&nbsp;&nbsp;<a href=\"javascript: abrirPopupFormacaoInicial("+friid+",'"+estuf+"');\">"+arrayAux[1]+"</a></b>";                 
                }
                else
                {
                    td2.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+
                                    "<img src=\"../imagens/seta_filho.gif\">&nbsp;&nbsp;"+
                                    "<b>&nbsp;&nbsp;<a href=\"javascript: abrirPopupFormacaoInicial("+friid+",'"+estuf+"');\">"+arrayAux[1]+"</a></b>";                 
                }
            break;
            
            case '2':
                td1.innerHTML = "<img src=\"../imagens/alterar.gif\" onClick=\"abrirPopupFormacaoContinuada("+frcid+",'"+estuf+"');\" style=\"border:0; cursor:pointer;\" title=\"Alterar o item\">"+
                                "&nbsp;<img src=\"../imagens/excluir.gif\" onClick=\"excluirItem("+frcid+", 'fc');\"  style=\"border:0; cursor:pointer;\" title=\"Excluir o item\">";
                td2 = tr.insertCell(1);
                if( arrayAux[2] == 'F' )
                {
                td2.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+
                                "<img src=\"../imagens/seta_filho.gif\">&nbsp;&nbsp;"+
                                "<a href=\"javascript:void(0);\" onclick=\"alteraIcone('"+arrayItens[0]+"_"+arrayAux[0]+"', 4);\">"+
                                "<img id=\"img_"+arrayItens[0]+"_"+arrayAux[0]+"\" src=\"../imagens/mais.gif\" border=\"0\" \></a>"+
                                "<b>&nbsp;&nbsp;<a href=\"javascript: abrirPopupFormacaoContinuada("+frcid+",'"+estuf+"');\">"+arrayAux[1]+"</a></b>";                 
                }
                else
                {
                td2.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+
                                "<img src=\"../imagens/seta_filho.gif\">&nbsp;&nbsp;"+
                                "<b>&nbsp;&nbsp;<a href=\"javascript: abrirPopupFormacaoContinuada("+frcid+",'"+estuf+"');\">"+arrayAux[1]+"</a></b>";                 
                }
            break;
            
            case '3':
                td1.innerHTML = "<img src=\"../imagens/alterar.gif\" onClick=\"abrirPopupMaterialDidatico("+madid+",'"+estuf+"');\" style=\"border:0; cursor:pointer;\" title=\"Alterar o item\">"+
                                "&nbsp;<img src=\"../imagens/excluir.gif\" onClick=\"excluirItem("+madid+", 'md');\" style=\"border:0; cursor:pointer;\" title=\"Excluir o item\">";
                td2 = tr.insertCell(1);
                if( arrayAux[2] == 'F' )
                {
                td2.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+
                                "<img src=\"../imagens/seta_filho.gif\">&nbsp;&nbsp;"+
                                "<a href=\"javascript:void(0);\" onclick=\"alteraIcone('"+arrayItens[0]+"_"+arrayAux[0]+"', 4);\">"+
                                "<img id=\"img_"+arrayItens[0]+"_"+arrayAux[0]+"\" src=\"../imagens/mais.gif\" border=\"0\" \></a>"+
                                "<b>&nbsp;&nbsp;<a href=\"javascript: abrirPopupMaterialDidatico("+madid+",'"+estuf+"');\">"+arrayAux[1]+"</a></b>";                 
                }    
                else
                {
                td2.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+
                                "<img src=\"../imagens/seta_filho.gif\">&nbsp;&nbsp;"+
                                "<b>&nbsp;&nbsp;<a href=\"javascript: abrirPopupMaterialDidatico("+madid+",'"+estuf+"');\">"+arrayAux[1]+"</a></b>";                 
                }    
            break;
            
            case '4':
                td1.innerHTML = "<img src=\"../imagens/alterar.gif\" onClick=\"abrirPopupObras('"+obrid+"','"+estuf+"','"+arrayAux[2]+"');\" style=\"border:0; cursor:pointer;\" title=\"Alterar o item\">"+
                                "&nbsp;<img src=\"../imagens/excluir.gif\" onClick=\"excluirItem("+arrayAux[2]+", 'ob');\" style=\"border:0; cursor:pointer;\" title=\"Excluir o item\">";
                td2 = tr.insertCell(1);
                if( arrayAux[2] == 'F' )
                {
                td2.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+
                                "<img src=\"../imagens/seta_filho.gif\">&nbsp;&nbsp;"+
                                "<a href=\"javascript:void(0);\" onclick=\"alteraIcone('"+arrayItens[0]+"_"+arrayAux[0]+"', 4);\">"+
                                "<img id=\"img_"+arrayItens[0]+"_"+arrayAux[0]+"\" src=\"../imagens/mais.gif\" border=\"0\" \></a>"+
                                "<b>&nbsp;&nbsp;<a href=\"javascript: abrirPopupObras_("+obrid+",'"+estuf+"');\">"+arrayAux[1]+"</a></b>";                 
                }
                else
                {
                td2.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+
                                "<img src=\"../imagens/seta_filho.gif\">&nbsp;&nbsp;"+
                                "<b>&nbsp;&nbsp;<a href=\"javascript: abrirPopupObras('"+obrid+"','"+estuf+"','"+arrayAux[2]+"');\">"+arrayAux[1]+"</a></b>";                 
                }            
            break;
            
            case '5':
                td1.innerHTML = "<img src=\"../imagens/alterar.gif\" onClick=\"abrirPopupEnsinoMedio("+friid+",'"+estuf+"');\" style=\"border:0; cursor:pointer;\" title=\"Alterar o item\">"+
                                "&nbsp;<img src=\"../imagens/excluir.gif\" onClick=\"excluirItem("+friid+", 'fi');\" style=\"border:0; cursor:pointer;\" title=\"Excluir o item\">";
          
                td2 = tr.insertCell(1);
                if( arrayAux[2] == 'F' )
                {
                td2.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+
                                "<img src=\"../imagens/seta_filho.gif\">&nbsp;&nbsp;"+
                                "<a href=\"javascript:void(0);\" onclick=\"alteraIcone('"+arrayItens[0]+"_"+arrayAux[0]+"', 4);\">"+
                                "<img id=\"img_"+arrayItens[0]+"_"+arrayAux[0]+"\" src=\"../imagens/mais.gif\" border=\"0\" \></a>"+
                                "<b>&nbsp;&nbsp;<a href=\"javascript: abrirPopupFormacaoInicial("+friid+",'"+estuf+"');\">"+arrayAux[1]+"</a></b>";                 
                }
                else
                {
                    td2.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+
                                    "<img src=\"../imagens/seta_filho.gif\">&nbsp;&nbsp;"+
                                    "<b>&nbsp;&nbsp;<a href=\"javascript: abrirPopupFormacaoInicial("+friid+",'"+estuf+"');\">"+arrayAux[1]+"</a></b>";                 
                }
            break;
        }
		
		td3 = tr.insertCell(2);
		td3.style.textAlign = "center";
		td3.style.cursor = "pointer";
		td3.id = 'td_'+arrayItens[0]+"_"+arrayAux[0];
		$(td3).observe('click', posicionaSlider);
		
		carregaBarraExecucao(td3,'tipo=carrega_barra_execucao&val='+arrayItens[0]+"_"+arrayAux[0]+'');

		td4 = tr.insertCell(3);
		td4.style.textAlign = "center";
		td4.style.cursor = "pointer";
		td4.style.color = "#008000";
		td4.title = "Alterar data de início";
		td4.id = 'datainicio_'+arrayItens[0]+"_"+arrayAux[0]+'';
		$(td4).observe('click', montaCalendario);
		
		td5 = tr.insertCell(4);
		td5.style.textAlign = "center";
		td5.style.cursor = "pointer";
		td5.style.color = "#008000";
		td5.title = "Alterar data de término";
		td5.id = 'datatermino_'+arrayItens[0]+"_"+arrayAux[0]+'';
		$(td5).observe('click', montaCalendario);
		
		carregaDataInicioTermino(td4,td5,'tipo=carrega_data&val='+arrayItens[0]+"_"+arrayAux[0]+'');
	}
  }
  else {
  	document.getElementById('img_'+oReq.responseText).src = "../imagens/mais.gif";
  }
}



function getResponseCarregaEtapas(oReq) {

  if(oReq.responseText.search("@") >= 0) {
	arrayEtapas = oReq.responseText.split('@');
	
 //   alert(arrayEtapas);
    
	indexLinhaItem = document.getElementById(arrayEtapas[0]).rowIndex;
	
	corLinhaItem = document.getElementById(arrayEtapas[0]).style.backgroundColor;
	if(corLinhaItem == "rgb(250, 250, 250)")
		cor = "#fafafa";
	else
		cor = "#f0f0f0";
		
	for(i=1; i < arrayEtapas.length; i++) {
		arrayAux = arrayEtapas[i].split('&');
		
		tr = $('tabela').insertRow(++indexLinhaItem);
		tr.id = arrayEtapas[0]+"_"+arrayAux[0];
		
		if(cor == "#fafafa") {
			tr.style.backgroundColor = "#f0f0f0";
			cor = "#f0f0f0";
		}
		else {
			tr.style.backgroundColor = "#fafafa";
			cor = "#fafafa";
		}
			
		td1 = tr.insertCell(0);
		td1.style.textAlign = "center";
        
        
        var exp = explode("_", arrayEtapas);
        var grupoId = exp[1];  
        var friid = arrayAux[0];
        var frcid = arrayAux[0];
        var madid = arrayAux[0];
        var obrid = arrayAux[0];
        var estuf = exp[0]; 

        //var cont = Number(arrayAux[2]);
        
        
        //arrayAux[1] = data inicial
        
        switch(grupoId)
        {
            case '1':
         
   
                    td1.innerHTML = "&nbsp;&nbsp;";
                    cont = ++cont;
                    td2 = tr.insertCell(1);
                    td2.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+
                                    "<img src=\"../imagens/seta_filho.gif\">&nbsp;&nbsp; <span style=\"font-size:100%; color:#000066;\">"+i+"ª Etapa - "+arrayAux[3]+"</span> ";                 
                         
                                break;
                
            case '2':
     
                td1.innerHTML = "&nbsp;&nbsp;";
                td2 = tr.insertCell(1);
                td2.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+
                                "<img src=\"../imagens/seta_filho.gif\">&nbsp;&nbsp;<span style=\" font-size:100%; color:#000066;\">"+i+"ª Etapa - "+arrayAux[3]+" </a> ";                 
            
            break;
            
            case '3':
         
                td1.innerHTML = "&nbsp;&nbsp;";
                td2 = tr.insertCell(1);
                td2.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+
                                "<img src=\"../imagens/seta_filho.gif\">&nbsp;&nbsp;<span style=\"font-size:100%; color:#000066;\">"+arrayAux[2]+"";                 
                        
            break;
            
           case '4':
          
                td1.innerHTML = "&nbsp;&nbsp;";
                td2 = tr.insertCell(1);
                td2.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+
                "<img src=\"../imagens/seta_filho.gif\">&nbsp;&nbsp;<span style=\" font-size:100%; color:#000066;\">"+i+"ª Etapa </a> ";                 
                                
            break;
            
             case '5':
          
                td1.innerHTML = "&nbsp;&nbsp;";
                td2 = tr.insertCell(1);
                td2.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+
                "<img src=\"../imagens/seta_filho.gif\">&nbsp;&nbsp;<span style=\" font-size:100%; color:#000066;\">"+i+"ª Etapa </a> ";                 
                                
            break;
        }
        
        
               
		
		td3 = tr.insertCell(2);
		td3.style.textAlign = "center";
		td3.style.cursor = "pointer";
		td3.id = 'td_'+arrayEtapas[0]+"_"+arrayAux[0];
		td3.innerHTML = "&nbsp;&nbsp;";
        
		td4 = tr.insertCell(3);
		td4.style.textAlign = "center";
		td4.style.cursor = "pointer";
		td4.style.color = "#008000";
		
        var arrdatai = explode("-",arrayAux[0]);
        var datai = arrdatai[2]+"/"+arrdatai[1]+"/"+arrdatai[0];
        
        var arrdataf = explode("-",arrayAux[1]);
        var dataf = arrdataf[2]+"/"+arrdataf[1]+"/"+arrdataf[0];
        
        
        td4.innerHTML = "&nbsp;&nbsp;"+datai;
        
		td5 = tr.insertCell(4);
		td5.style.textAlign = "center";
		td5.style.cursor = "pointer";
		td5.style.color = "#008000";
        td5.innerHTML = "&nbsp;&nbsp;"+dataf;
        

	}
  }
  else {
  	document.getElementById('img_'+oReq.responseText).src = "../imagens/mais.gif";
  }
}

function getResponseCarregaObras(){

}

/*** (FIM) AJAX RESPONSES ***/


function abrirPopupFormacaoInicial(friid, estuf)
{ 
    return windowOpen('parindigena.php?modulo=principal/popupCadFormacaoInicial&acao=I&friid=' + friid + '&estuf=' + estuf,
                  'adicionarItensComposicao',
                  'height=850,width=950,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');

} 


function abrirPopupEnsinoMedio(friid, estuf)
{ 
    return windowOpen('parindigena.php?modulo=principal/popupCadEnsinoMedio&acao=I&friid=' + friid + '&estuf=' + estuf,
                  'adicionarItensComposicao',
                  'height=850,width=950,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');

} 
         
function abrirPopupFormacaoContinuada(frcid, estuf)
{
    return windowOpen('parindigena.php?modulo=principal/popupCadFormContinuada&acao=I&frcid=' + frcid + '&estuf=' + estuf,
                  'adicionarItensComposicao',
                  'height=850,width=950,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');

}        
               

function abrirPopupMaterialDidatico(madid, estuf)
{
    return windowOpen('parindigena.php?modulo=principal/popupCadMaterial&acao=I&madid=' + madid + '&estuf=' + estuf,
                  'adicionarItensComposicao',
                  'height=850,width=950,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');

}       
function abrirPopupConvenio(itmid, estuf)
{
    return windowOpen('parindigena.php?modulo=principal/cadastro_obras_indigenas&acao=A&itmid=' + itmid + '&estuf=' + estuf,
                  'adicionarItensComposicao',
                  'height=850,width=950,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');

}       

function abrirPopupVincularConvenio(tipo, estuf)
{
    return windowOpen('parindigena.php?modulo=principal/vincularConvenioSubacaoTipo&acao=A&tipo=' + tipo + '&estuf=' + estuf,
                  'vincularConvenio',
                  'height=850,width=950,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');

}

function abrirPopupFormacaoInicialNew(grpid, estuf)
{
    return windowOpen('parindigena.php?modulo=principal/popupCadFormacaoInicial&acao=I&grpid=' + grpid + '&estuf=' + estuf,
                  'adicionarItensComposicao',
                  'height=850,width=950,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');

} 

function abrirPopupEnsinoMedio(grpid, estuf)
{
    return windowOpen('parindigena.php?modulo=principal/popupCadEnsinoMedio&acao=I&grpid=' + grpid + '&estuf=' + estuf,
                  'adicionarItensComposicao',
                  'height=850,width=950,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');

} 
         
function abrirPopupFormacaoContinuadaNew(grpid, estuf)
{
    return windowOpen('parindigena.php?modulo=principal/popupCadFormContinuada&acao=I&grpid=' + grpid + '&estuf=' + estuf,
                  'adicionarItensComposicao',
                  'height=850,width=950,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');

}        
               

function abrirPopupMaterialDidaticoNew(grpid, estuf)
{
    return windowOpen('parindigena.php?modulo=principal/popupCadMaterial&acao=I&grpid=' + grpid + '&estuf=' + estuf,
              'adicionarItensComposicao',
              'height=850,width=950,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');

}       
function abrirPopupObraNew(grpid, estuf)
{
    return windowOpen('parindigena.php?modulo=principal/cadastro_obras_indigenas&acao=A&grpid=' + grpid + '&estuf=' + estuf,
                  'adicionarItensComposicao',
                  'height=520,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');

}       

function abrirPopupConvenio(covcod)
{
    return windowOpen('parindigena.php?modulo=principal/popupCadConvenio&acao=I&covcod=' + covcod,
                  'abrirPopupConvenio',
                  'height=440,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');

}           
         
function abrirPopupObras(obrid, estuf, itmid)
{
    return windowOpen('parindigena.php?modulo=principal/cadastro_obras_indigenas&acao=A'+'&itmid='+itmid+'&obrid='+obrid+'&estuf='+estuf,
        'abrirPopupObras',
        'height=520,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');

}

function excluirItem(id, tipo)
{
    if (confirm('Deseja excluir o registro?')) 
             {
                 switch( tipo )
                 {                    
                    case 'fi':
                        
                        return window.location.href = 'parindigena.php?modulo=inicio&acao=C&excluirFi=' + id;
                    break;
                    
                    case 'fc':
                        
                        return window.location.href = 'parindigena.php?modulo=inicio&acao=C&excluirFc=' + id;
                    break;
                    
                    case 'md':
                        
                        return window.location.href = 'parindigena.php?modulo=inicio&acao=C&excluirMd=' + id;
                    break;
                    
                    case 'ob':
                        
                        return window.location.href = 'parindigena.php?modulo=inicio&acao=C&excluirOb=' + id;
                    break;
                    
                 }
             }
}

function abrirPopupObras_()
{
	return false;
}
