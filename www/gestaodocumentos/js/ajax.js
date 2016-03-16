/*** (INÍCIO) AJAX REQUESTS ***/
function montaFilhos(url,data, arFiltros, boCarregaLinkAjax) {
	var aj = new Ajax.Request(  
		url, {  
		method:'post',   
		parameters: data,
		asynchronous: false,
		onLoading: $('aguarde_').show(),
		onLoading: $('tabela_tarefa').setOpacity(0.3),
		onComplete: function(r)
		{
			//$('teste').update(r.responseText);
			if(r.responseText != ""){
				eval(r.responseText);
				// se o array retornado do banco
				var tarpai = arDados[0].tarpai;
				var tartarefa = arDados[0].tartarefa;
				var trId = arDados[0].trId;
				var indexLinha = document.getElementById(trId).rowIndex;
				var corLinha = document.getElementById(trId).style.backgroundColor;
				
				if(corLinha == "rgb(250, 250, 250)"){
					cor = "#fafafa";
				} else {
					cor = "#f0f0f0";
				}
				
				// Iteração do Array de Dados
				if(arDados.length >= 1){
			       	for (var j = 0; j < arDados.length; j++) {
			       		var tarid 					= arDados[j].tarid;
			       		var tartitulo				= arDados[j].tartitulo;
			       		var nomeresponsavel			= arDados[j].nome;
			       		var boFilho 				= arDados[j].boFilho;
			       		var boAnexo 				= arDados[j].boAnexo;
			       		var boRestricao 			= arDados[j].boRestricao;
			       		var tardataprazoatendimento = arDados[j].tardataprazoatendimento;
			       		var img 					= arDados[j].img;
			       		var barraExecucao			= arDados[j].barraExecucao;
			       		var dataPrazo   			= arDados[j].dataPrazo;
			       		var taraberto 				= arDados[j].taraberto;
			       		var setorRespon   			= arDados[j].setorrespon;
			       		var codTarefa   			= arDados[j].codTarefa;
			       		var tardepexterna   		= arDados[j].tardepexterna;
			       		var prioridade   			= arDados[j].prioridade;
                        var docid                   = arDados[j].docid;
                        var status_dsc              = arDados[j].status_dsc;
                        var nvcdsc                  = arDados[j].nvcdsc;
                        var dias_decorridos 		= arDados[j].dias_decorridos;
                        var solicitante      		= arDados[j].solicitante;
			       		
			       		if(tarpai){
                			var st_tarefa_atividade = 'Atividade';
                		} else {
                			var st_tarefa_atividade = 'Tarefa';
                		}
			       		
			       		if(tarid == tartarefa){
							var idTemp = "";
						} else {
							var idTemp = "_"+tarid;
						}
						
						if(tartarefa == tarpai){
							var tarpaiTemp = "";
						}
						
						if(tarpai){
							if(tartarefa == tarpai){
								var tarpaiTemp = "";
							}else if(idTemp == ""){
								var tarpaiTemp = '_'+tarpai+'_';
							} else {
								var tarpaiTemp = '_'+tarpai;
							}		
						} else {
							tarpaiTemp = '';
						}
			       		
			       		tr = $('tabela_tarefa').insertRow(++indexLinha);
						tr.id = trId+'_'+tarid;
						
						var arTrId = trId.split("_");
			       		var espaco     = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
			       		var espacoTemp = espaco;
			       		var margem = 20;

		       			if(arTrId.length > 1){
				       		for (var y = 1; y < arTrId.length; y++) {
			            		espacoTemp = espacoTemp + espaco;
			            	}	       			
		       			} 
		       			margem = margem + (arTrId.length * 5);
						
						if($('tarid').value == tarid){
							tr.style.background = '#ffffcc';
						} else {
							if(cor == "#fafafa") {
								tr.style.backgroundColor = "#f0f0f0";
								cor = "#f0f0f0";
							} else {
								tr.style.backgroundColor = "#fafafa";
								cor = "#fafafa";
							}
						}
							
				        td1 = tr.insertCell(0);
		                td1.style.textAlign = "center";
		                
		                td1.innerHTML = "<img src=\"../imagens/gif_inclui.gif\" onClick=\"window.location.href='gestaodocumentos.php?modulo=principal/cadAtividade&acao=A&tarpai="+tarid+"&tartarefa="+tartarefa+"'\" style=\"border:0; cursor:pointer;\" title=\"Incluir uma atividade a Atividade\">"
		                td1.innerHTML += "&nbsp;<img src=\"../imagens/alterar.gif\" onClick=\"window.location.href='gestaodocumentos.php?modulo=principal/cadAtividade&acao=A&tarid="+tarid+"&tartarefa="+tartarefa+"&tarpai="+tarpai+"'\" style=\"border:0; cursor:pointer;\" title=\"Alterar "+st_tarefa_atividade+"\">"
						td1.innerHTML += "&nbsp;<img src=\"../imagens/excluir.gif\" style=\"border:0; cursor:pointer;\" title=\"Excluir "+st_tarefa_atividade+"\" onClick=\"excluirTarefaAtividade('"+tarid+"');\" >";
						
						if(prioridade == 'U'){
							var imgPrioridade = '<img src=\'../imagens/pd_urgente.JPG\' /> Urgente';
						} else if(prioridade == 'A'){
							var imgPrioridade = '<img src=\'../imagens/pd_alta.JPG\' /> Alta';
						} else {
							var imgPrioridade = '<img src=\'../imagens/pd_normal.JPG\' /> Normal';						
						}
						
						tdPrioridade = tr.insertCell(1);						
						tdPrioridade.innerHTML = imgPrioridade;
		                
		                var imgAnexo = "";
						if(boAnexo){
							imgAnexo = "<img src=\"../imagens/anexo.gif\" onClick=\"window.location.href='gestaodocumentos.php?modulo=principal/cadDocumento&acao=A&tarid="+tarid+"'\" style=\"border:0; cursor:pointer;\" title=\"Anexo\">";
						}
						var imgRestricao = "";
						if(boRestricao){
							imgRestricao = "<img src=\"../imagens/restricao.png\" onClick=\"window.location.href='gestaodocumentos.php?modulo=principal/cadAcompanhamento&acao=A&tarid="+tarid+"&tartarefa="+tartarefa+"&tarpai="+tarpai+"&boPadraoRetricao=1';\" style=\"border:0; cursor:pointer;\" title=\"Restrição\">";
						}
						var imgDepexterna = "";
						if(tardepexterna){
							imgDepexterna = "<img src=\"../imagens/botao_de.png\" title=\"Dependência Externa\">";
						}
		                                
						/**
                		* VERIFICA SE O LINK DEVE SER CARREGADO COM AJAX OU NÃO, SÓ DEVERÁ USAR AJAX NO cadAcompanhamento
                		*/
                		if(boCarregaLinkAjax){
                			var onclick = "onClick=\"carregaCabecalhoAtendimento("+tarid+", this.parentNode.parentNode);\"";
                		} else {
                			var onclick = "onClick=\"window.location.href='gestaodocumentos.php?modulo=principal/cadAcompanhamento&acao=A&tarid="+tarid+"&tartarefa="+tartarefa+"&tarpai="+tarpai+"';\"";
                		}
						td2 = tr.insertCell(2);
				        if( boFilho ) {
							td2.innerHTML = espacoTemp+"<img src=\"../imagens/seta_filho.gif\">"+
											"<a href=\"#\" title=\""+tartitulo+"\" onclick=\"alteraIcone('"+tarid+"','"+ tarpai+"','"+ tartarefa+"', '"+tr.id+"', '"+ taraberto+"', 2, '"+arFiltros+"' );\">"+
											"<img id=\"img_"+tartarefa+tarpaiTemp+idTemp+"\" src=\"../imagens/"+img+"\" border=\"0\"></a>&nbsp;&nbsp;"+
											imgAnexo+imgRestricao+" <a href=\"#\" title=\""+tartitulo+"\" "+onclick+" ><b>"+codTarefa+' - '+tartitulo+"</b></a> "+imgDepexterna;
							//if(taraberto == 't'){
								//alteraIcone(tarid,tarpai,tartarefa,tr.id,taraberto, 1, arFiltros, boCarregaLinkAjax);
							//}
						} else {
				            td2.innerHTML = espacoTemp+"<img src=\"../imagens/seta_filho.gif\">"+
									    	"&nbsp;"+imgAnexo+imgRestricao+" <a href=\"#\" title=\""+tartitulo+"\" "+onclick+" ><b>"+codTarefa+' - '+tartitulo+"</b></a> "+imgDepexterna;
				        }
				        
                        //SOLICITANTE.
                        td3 = tr.insertCell(3);
                        td3.style.color = "#1E90FF";
                        td3.innerHTML = solicitante;
                        td3.id = 'td3_'+tarid;
                        
                        //NIVEL DE COMPLEXIDADE.
                        td4 = tr.insertCell(4);
                        td4.style.color = "#1E90FF";
                        td4.innerHTML = nvcdsc;
                        td4.id = 'td4_'+tarid;
                        
                        //DIAS DECORRIDOS
                        td5 = tr.insertCell(5);
                        td5.style.color = "#1E90FF";
                        td5.setAttribute( 'align', 'center' );
                        td5.innerHTML = dias_decorridos;
                        td5.id = 'td5_'+tarid;
                        
				        //RESPONSÁVEL
						td6 = tr.insertCell(6);
				        td6.style.color = "#1E90FF";
				        td6.style.cursor = "pointer";
						//td6.innerHTML = "<span onclick=\"alteraResponsavel('td6_" +tarid + "')\" >"+setorRespon+' - '+nomeresponsavel+"</span>";
                        td6.innerHTML = setorRespon+' - '+nomeresponsavel;
						td6.id = 'td6_'+tarid;
						
						//SITUAÇÃO
						td7 = tr.insertCell(7);
						td7.style.cursor = "pointer";
						td7.setAttribute( 'align', 'center' );
						td7.id = 'td_'+tarid;
						array = barraExecucao.split('@@');
						td7.innerHTML = "<span onclick=\"posicionaSlider('td_" +tarid + "')\" >"+array[0]+"</span>";
						td7.status = array[1];
						td7.percentual = array[2]; 

						//PRAZO ATENDIMENTO
						td8 = tr.insertCell(8);
						td8.style.textAlign = "center";
						td8.style.cursor = "pointer";
						td8.style.color = "#008000";
						td8.title = "Alterar Prazo de Atendimento";
						td8.id = 'dataprazo_'+tarid;
						mostraDataPrazoFormatada(td8, dataPrazo, tarid);
                        
                        //STATUS WORKFLOW
                        td9 = tr.insertCell(9);
				        td9.style.color = "#1E90FF";
				        td9.style.cursor = "pointer";
						td9.innerHTML = "<span onclick=\"exibirHistorico("+docid+");\" style=\"cursor: pointer; color:#4682B4;\" ><b>"+status_dsc+"</b></span>"
						td9.id = 'td9_'+tarid;
						
						//ORDEM - ESSE BLOCO DE CODIGO FOI COMENTADO PQ DIANTE DAS REGRAS PARA ESSE SISTEMA NÃO NECESSARIO A ALTERAÇÃO DA ORDER.
                        //ESTA SENDEN COMENTADO APENAS ESSE BLOCO OS DEMIAS QUE FAZEM TODO O PROCESSO DE ALTERAÇAO DE POSSIÇÃO NÃO ESTA COMENTADO.
                        /*
						td7 = tr.insertCell(7);
						td7.setAttribute( 'align', 'center' );
						var desabilitadoB = "";
						var desabilitadoC = "";
						var linkB = "onclick=\"mudaPosicao('baixo',this.parentNode.parentNode.rowIndex, '"+tarid+"', '"+tarid+"', '', '"+taraberto+"', '"+arFiltros+"')\"";
						var cursorB = "style=\"cursor: pointer;\""; 
						
						var linkC = "onclick=\"mudaPosicao('cima',this.parentNode.parentNode.rowIndex, '"+tarid+"', '"+tarid+"', '', '"+taraberto+"', '"+arFiltros+"')\"";
						var cursorC = "style=\"cursor: pointer;\""; 
							
						if(j == 0){
							desabilitadoC = "d";
							linkC = "";
							cursorC = "";
						}
						if(j + 1 == arDados.length){
							desabilitadoB = "d";
							linkB = "";
							cursorB = ""; 
						}
						td7.innerHTML = "&nbsp;<img "+linkB+" "+cursorB+" src=\"../imagens/seta_baixo"+desabilitadoB+".gif\" />";
						td7.innerHTML += "&nbsp;<img "+linkC+" "+cursorC+" src=\"../imagens/seta_cima"+desabilitadoC+".gif\" />";
						*/
						espacoTemp = "";
						//alteraIcone(tarid,tarpai,tartarefa,tr.id,taraberto, 1, arFiltros, boCarregaLinkAjax);
			       	}
			       	
			       	/**
			       	* MUDAR COR DAS TR
			       	*/
			       	var cor = "#f0f0f0";
					if($('tabela_tarefa').rows.length > 1){
						for (var i = 1; i < $('tabela_tarefa').rows.length; i++) {
							var tr = $('tabela_tarefa').rows[i];
							if(tr.style.display != 'none'){
								if(tr.style.backgroundColor != 'rgb(255, 255, 204)'){
									if(cor == "#fafafa") {
										tr.style.backgroundColor = "#f0f0f0";
										cor = "#f0f0f0";
									} else {
										tr.style.backgroundColor = "#fafafa";
										cor = "#fafafa";
									}
								}
							}
						}
					}
			       	
			    }
			} else {
		  		document.getElementById('img_'+oReq.responseText).src = "../imagens/mais.gif";
		  	}
		  	$('aguarde_').hide();
		  	$('tabela_tarefa').setOpacity(1);
		} // FIM DO onComplete
	  
	});
}

/*function carregaBarraExecucao(cel,data,tarid) {
	var aj = new Ajax.Request('ajax.php',  
	{  
		method: 'post',   
		parameters: data,
		asynchronous: true,
		onComplete: function(r)
		{
			array = r.responseText.split('@@');
			cel.innerHTML = "<span onclick=\"posicionaSlider('td_" +tarid + "')\" >"+array[0]+"</span>";
			cel.status = array[1];
			cel.percentual = array[2];
		}
	});
}
*/

/*
function carregaDataPrazo(celtermino,data,tarid) {
	var aj = new Ajax.Request('ajax.php',  
	{  
		method: 'post',   
		parameters: data,
		asynchronous: true,
		onComplete: function(r)
		{
			celtermino.innerHTML = "<span id='span_data_"+tarid+"' onclick=\"montaCalendario('dataprazo_" +tarid + "')\" >"+r.responseText+"</span>";
			//celtermino.innerHTML = r.responseText;
			var objDate = strDateToObjDate( r.responseText , 'd/m/Y' , '/' );
			var objDataAtual = new Date();
			
			var objDataMaisQuatroDias = new Date();
			objDataMaisQuatroDias.setDate(objDataMaisQuatroDias.getDate() + 4);
			
			celtermino.style.fontWeight = '';
			
			/**
			* FEITO ASSIM POR CAUSA DA PRESA
			*/
/*
			var objDateTemp = objDate.getDate() + "/" + (objDate.getMonth() + 1) + "/" + objDate.getFullYear();
			var objDataMaisQuatroDias = objDataMaisQuatroDias.getDate() + "/" + (objDataMaisQuatroDias.getMonth() + 1) + "/" + objDataMaisQuatroDias.getFullYear();
			
			if( objDate <= objDataAtual ) {
				celtermino.style.color = '#ff2020';
				celtermino.style.fontWeight = 'bold';
			}else if( objDateTemp == objDataMaisQuatroDias ) {
				celtermino.style.color = '#FFA500';
				celtermino.style.fontWeight = 'bold';
			} else {
				celtermino.style.color = "#008000";
			}
		}
	});
}
*/

function atualizaBarraStatus(intBarraStatusId, strStatus, intStatus, intPercentual, boMontaShowModal ) {
	/*if(boMontaShowModal == 'S'){
		var funcaoParametros = 'atualizaBarraStatus( "'+intBarraStatusId+'" , "'+strStatus+'" , "'+intStatus+'", "'+intPercentual+'", "N" );'
		montaShowModal(funcaoParametros);
		return false;
	}*/
	var id = intBarraStatusId.replace('td_', '');
	var data = 'tipo=atualiza_barra_status&id='+id+'&codstatus='+intStatus+'&percentual='+intPercentual+'';
	
	var aj = new Ajax.Request('ajax.php',  
	{  
		method: 'post',   
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

function alteraData( id , strDataAlterada , strNovaData, strDataAntiga, boMontaShowModal ) {
	if(boMontaShowModal == 'S'){
		var funcaoParametros = 'alteraData( '+id+' , "'+strDataAlterada+'" , "'+strNovaData+'", "'+strDataAntiga+'", "N" );'
		montaShowModal(funcaoParametros);
		return false;
	}
	
	strSpanId = 'dataprazo_' + id;
	var objSpan = document.getElementById( strSpanId );

	var data = 'tipo=atualiza_data&tarid='+id+'&data_alterada='+strDataAlterada+'&nova_data='+strNovaData+'';
	var aj = new Ajax.Request('ajax.php',  
	{  
		method: 'post',   
		parameters: data,
		onLoading: objSpan.innerHTML = '<img align="absmiddle" src="../imagens/wait.gif"/>',
		onComplete: function(r)
		{
			if(r.responseText) {
				aposAlterarDataPrazo( id , strDataAlterada , strNovaData );
			} else {
				alert("Erro ao atualizar a Data.");
				aposAlterarDataPrazo( id , strDataAlterada , strDataAntiga );	
			}
		}
	});
}

function mostraDataPrazoFormatada(td5, dataPrazo, tarid){
	//td5.innerHTML = "<span id='span_data_"+tarid+"' onclick=\"montaCalendario('dataprazo_" +tarid + "')\" >"+dataPrazo+"</span>";
	td5.innerHTML = "<span id='span_data_"+tarid+"'>"+dataPrazo+"</span>";
	var objDate = strDateToObjDate( dataPrazo , 'd/m/Y' , '/' );
	var objDataAtual = new Date();
	
	var objDataMaisQuatroDias = new Date();
	objDataMaisQuatroDias.setDate(objDataMaisQuatroDias.getDate() + 4);
	
	td5.style.fontWeight = '';
	
    //VALIDAÇÃO DE PERMISSÃO
    var resp;
    var data = 'funcao=verificaSeConcluido&tarid='+tarid;
	var aj = new Ajax.Request('_funcoes.php',
        {  
            method: 'POST',   
            parameters: data,
            asynchronous: false,
            onComplete: function(r){
                resp = pegaRetornoAjax('<resp>', '</resp>', r.responseText, true);
            }
    });

	var objDataAtualTemp = objDataAtual.getDate() + "/" + (objDataAtual.getMonth() + 1) + "/" + objDataAtual.getFullYear();
	var objDateTemp = objDate.getDate() + "/" + (objDate.getMonth() + 1) + "/" + objDate.getFullYear();
	var objDataMaisQuatroDias = objDataMaisQuatroDias.getDate() + "/" + (objDataMaisQuatroDias.getMonth() + 1) + "/" + objDataMaisQuatroDias.getFullYear();
	
	if( objDate <= objDataAtual ) {
		td5.style.color = '#ff2020';
		td5.style.fontWeight = 'bold';
	}else if( objDateTemp >= objDataAtualTemp && objDateTemp <= objDataMaisQuatroDias ) {
		td5.style.color = '#FFA500';
		td5.style.fontWeight = 'bold';
    }else if(trim(resp) == 'S'){
        td5.style.color = '#000000';
		td5.style.fontWeight = 'bold';
	} else {
		td5.style.color = "#008000";
	}
    
    
    
}
/*** (FIM) AJAX REQUESTS ***/