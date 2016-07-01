function verificarPessoaServidor(vttdoc, callBackTrue, callBackFalse){
 
	vttdoc = formatarNumeroDocumento(vttdoc, null);
	 
	 if(vttdoc){
	        new Ajax.Request('ajax.php',{
	            method: 'post',
	            parameters: {'servico': 'verificarPessoaServidor', 'vttdoc': vttdoc},
	            onComplete: function(transport){
	            	var resposta = transport.responseText.evalJSON();
	            	
	            	if(resposta){
	                	if(callBackTrue != null) {
	                		callBackTrue();
	                	}
	                }else{
	                	if(callBackFalse != null) {
	                		callBackFalse();
	                	}
	                }
	            },
	            onFailure: function(){ 
	                alert('Ocorreu um erro ao buscar os dados do visitante.');
	            }
	        });
	        
	        
	 }
}

function consultarVisitanteDuplicadoEntrada(vttdoc, tipo){
	 vttdoc = formatarNumeroDocumento(vttdoc, null);
	 
	 if(vttdoc){
	        new Ajax.Request('ajax.php',{
	            method: 'post',
	            parameters: {'servico': 'consultarVisitanteDuplicadoEntrada', 'vttdoc': vttdoc, 'tipo': tipo},
	            onComplete: function(transport){
	            	var resposta = transport.responseText.evalJSON();

	                // consulta retornou 1 registro
	                if(resposta.length == 1) {
	                	// verifica se é visitante ( vttid !== null ) ou servidor.
	                	if(resposta[0].vttid){
	                		
	                		// carrega os dados do visitante nos campos
	                		carregarPessoa(resposta[0].vttid, null);
	                		
	                	} else if(resposta[0].nu_matricula_siape) {
	                		
	                		// carrega os dados do servidor nos campos
	                		carregarPessoa(null, resposta[0].nu_matricula_siape);
	                	}
	                
	                // consulta retornou mais de 1 registro	
	                } else if(resposta.length > 1) {
	                	// popup para selecionar o registro
	                	popupRegistroDuplicado(tipo);
	                // não encontrou nenhum registro
	                } else {
	                	
	                	if(tipo == 'visitante')
	                        alert('Nenhum visitante foi encontrado com o Número de Documento "' + vttdoc + '".');
	                    else
	                        alert('Nenhuma pessoa foi encontrada com o Número de Documento "' + vttdoc + '".');
	                    
	                    limparEntradaVisitante(null, true);
	                } 
	                
	            },
	            onFailure: function(){ 
	                alert('Ocorreu um erro ao buscar os dados do visitante.');
	            }
	        });
	 }
}

function consultarVisitanteEntrada(vttid, vttdoc, nu_matricula_siape, tipo) {

    vttdoc = formatarNumeroDocumento(vttdoc, null);
   
    if(vttid || vttdoc || nu_matricula_siape){
        new Ajax.Request('ajax.php',{
            method: 'post',
            parameters: {'servico': 'consultarVisitanteEntrada', 'vttid': vttid, 'vttdoc': vttdoc, 'nu_matricula_siape': nu_matricula_siape, 'tipo': tipo},
            onComplete: function(transport){

                var resposta = transport.responseText.evalJSON();
                if(resposta.status == 'ok'){

                	// verifica se retornou servidor para setar o vttdoc correto
                	if(resposta.nu_matricula_siape){
                		if($('cargoServidorRol') != undefined) {
	                		$('vttcargo').setValue(resposta.ds_cargo_emprego);
	                    	$('cargoServidorRol').show();
                		}
                		
                		 $('vttid').setValue(resposta.vttid);
                         $('nu_matricula_siape').setValue(resposta.nu_matricula_siape);
                         $('vttdoc').setValue(resposta.nu_matricula_siape);
                         $('vttnome').setValue(resposta.vttnome);
                         $('ds_cargo_emprego').setValue(resposta.ds_cargo_emprego);
                         $('vttobs').setValue(resposta.vttobs);
                         $('vstnumcracha').setValue(resposta.vsnumcrachasistema);
                         
                	} else {
                		if($('cargoServidorRol') != undefined) {
                		  $('vttcargo').setValue("");
                    	  $('cargoServidorRol').hide();
                		}
                    	
                		 $('vttid').setValue(resposta.vttid);
                         $('nu_matricula_siape').setValue(resposta.nu_matricula_siape);
                         $('vttdoc').setValue(resposta.vttdoc);
                         $('vttnome').setValue(resposta.vttnome);
                         $('ds_cargo_emprego').setValue(resposta.ds_cargo_emprego);
                         $('vttobs').setValue(resposta.vttobs);
                         $('vstnumcracha').setValue(resposta.vsnumcrachasistema);
                	}
                    
                    // Verifica se o visitante possui acesso irrestrito e mostra o aviso referente
                    //
                    //alert( "resposta.vttnomeIrrestrito: " + resposta.vttnomeIrrestrito + " resposta.vttfuncaoIrrestrito: " + resposta.vttfuncaoIrrestrito );
                    if(resposta.vttnomeIrrestrito)
                    {
                        $('vttid').setValue('');
                        $('vttnome').setValue('');
                        $('ds_cargo_emprego').setValue('');
                        $('vttobs').setValue('');
                        $('vstnumcracha').setValue('');
                        $('edaid').setValue('');
                        $('dstid').setValue('');
                        $('nu_matricula_siape').setValue('');
                        
                        if(resposta.vttfuncaoIrrestrito){
                            alert( 'Atenção: Este funcionário possui de acesso irrestrito ao MEC (perfil: ' + resposta.vttfuncaoIrrestrito + ')' );
                        }
                    }
                    
                    if (resposta.vstdatsaida){
                    	//alert('Numero do crachá em uso.');
                    	//return;
                    }
                    
                    if(tipo == 'V'){
                        $('edfid').setValue(resposta.edfid ? resposta.edfid : '');
                        var options = $('edaid').options;
                        options.length = 1;
                        for(var i = 0; i < resposta.andares.length; i++){
                            var opt = document.createElement('option');
                            opt.value = resposta.andares[i].codigo;
                            opt.text = resposta.andares[i].descricao;
                             $('edaid').options.add(opt);
                        }
                        $('edaid').setValue(resposta.edaid ? resposta.edaid : '');
                        $('dstid').setValue(resposta.dstid ? resposta.dstid : '');
                        $('btnEditar').enable();
                        $('vstnumcracha').focus();
                    }else{
                        $('nu_matricula_siape').setValue(resposta.nu_matricula_siape);
                    }
                    
                    // Mostra a foto
                    recuperarFoto(resposta.vttid, resposta.nu_matricula_siape);
                }else{
                    
//                    $('vttid').setValue('');
//                    $('vttdoc').setValue('');
//                    $('vttnome').setValue('');
//                    $('ds_cargo_emprego').setValue('');
//                    $('vttobs').setValue('');
//                    $('vstnumcracha').setValue('');
//                    $('edaid').setValue('');
//                    $('dstid').setValue('');
//                    $('nu_matricula_siape').setValue('');
                    
                    if(tipo == 'V')
                        alert('Nenhum visitante foi encontrado com o Número de Documento "' + vttdoc + '".');
                    else
                        alert('Nenhuma pessoa foi encontrada com o Número de Documento "' + vttdoc + '".');
                    
                    limparEntradaVisitante(tipo, false);
                }
            },
            onFailure: function(){ 
                alert('Ocorreu um erro ao buscar os dados do visitante.');
            }
        });
    }
}

function limparEntradaVisitante(tipo, limparEquipamentos){
	if($('audid').getValue() == '')
	{
	    $('vttid').setValue('');
	    $('vttdoc').setValue('');
	    $('vttnome').setValue('');
	    $('vttobs').setValue('');
	    $('nu_matricula_siape').setValue('');
	}
    
    if(tipo == 'V'){
        $('edfid').setValue('');
        $('edaid').options.length = 1;
        $('edaid').setValue('');
        $('dstid').setValue('');
        $('btnEditar').disable();
        $('vstnumcracha').setValue('');
    }
    
    if($('audid').getValue() == '')
    	$('fotoVisitante').innerHTML = '';
    
    if(limparEquipamentos){
        $('eqmid[]').options.length = 0;
    }
    //$('vttdoc').focus();
}

function verificaSeVisitanteFoiRegistradoPeloNumeroEtiquetaManual( vstnumcracha ){
	registrado = '';
	new Ajax.Request('ajax.php',{
        method: 'post',
        asynchronous: false,
        parameters: {'servico': 'verificaSeVisitanteFoiRegistradoPeloNumeroEtiquetaManual', 'vstnumcracha': vstnumcracha},
        onComplete: function(transport){
            registrado = transport.responseText.evalJSON();
        },
        onFailure: function(){
            alert('Ocorreu um erro ao buscar os andares do edifício.');
        }
    });
	return registrado;
	
}

function reimprimirEtiqueta(){
	if( $('vttdoc').getValue() == "" ){
		alert('Informe o número do documento');
		return false;
	}
	
	$('requisicao').setValue('reimprimir');
	
	new Ajax.Request('ajax.php',{
        method: 'post',
        asynchronous: false,
        parameters: $('formularioCadastroVisita').serialize(true),
        onComplete: function(transport){
			resposta = transport.responseText.evalJSON();
			if (resposta.visita.ativo){
				 AbrirPopUp('?modulo=principal/visitante/popup/imprimirEtiqueta&acao=A&file='+resposta.etiqueta.valor, 'imprimirEtiqueta', 'scrollbars=yes, width=1000, height=700');
                                 
			} else { 
				alert('Visita não está ativa. Confirme entrada do visitante');
			}
        },
        onFailure: function(){
            alert('Ocorreu um erro ao recuperar dados');
        }
    });
	
	$('requisicao').setValue('salvar');
}

function recuperarFoto(vttid, nu_matricula_siape){
    new Ajax.Updater('fotoVisitante', 'ajax.php', {
        method: 'post',
        parameters: {'servico': 'recuperarFoto', 'vttid': vttid, 'nu_matricula_siape': nu_matricula_siape}
    });
}

function recuperarEdificio(){
    new Ajax.Updater('edificioVisitante', 'ajax.php', {
        method: 'post',
        parameters: {'servico': 'recuperarEdificio'}
    });
}

function buscarAndarEdificio(edfid) {

    var options = $('edaid').options;
    options.length = 1;

    if(edfid){
        new Ajax.Request('ajax.php',{
            method: 'post',
            parameters: {'servico': 'buscarAndarEdificio', 'edfid': edfid},
            onComplete: function(transport){

                var resposta = transport.responseText.evalJSON();
                if(resposta.status == 'ok'){
                	var options = $('edaid').options;
                    options.length = 1;
                    for(var i = 0; i < resposta.andares.length; i++){
                        var opt = document.createElement('option');
                        opt.value = resposta.andares[i].codigo;
                        opt.text = resposta.andares[i].descricao;
                         $('edaid').options.add(opt);
                    }
                }else{
                    alert('Ocorreu um erro ao buscar os andares do edifício.');
                }
            },
            onFailure: function(){
                alert('Ocorreu um erro ao buscar os andares do edifício.');
            }
        });
    }
}

function buscarEquipamento(etiqueta, tipo){
    //se for entrada de equipamento
    if(tipo == 'E'){
        new Ajax.Request('ajax.php',{
            method:'post',
            parameters: {'servico': 'buscarEquipamento', 'etiqueta': etiqueta},
            onComplete: function(transport){

                var resposta = transport.responseText.evalJSON();

                if(resposta.status == 'ok'){

                	if(resposta.vttid || resposta.nu_matricula_siape) {
                		if($('vttdoc').getValue() == '') {
                			if(resposta.nu_matricula_siape){
                    			$('vttdoc').setValue(resposta.nu_matricula_siape);
                    		}else{
                    			$('vttdoc').setValue(resposta.vttdoc);
                    		}                		
                    		recuperarFoto(resposta.vttid, resposta.nu_matricula_siape);
                            $('vstid').setValue('');
                            $('vttid').setValue(resposta.vttid);
                            $('nu_matricula_siape').setValue(resposta.nu_matricula_siape);
                            $('vttnome').setValue(resposta.vttnome);
                            $('vttobs').setValue(resposta.vttobs);
                		}                		
                	}
                	
                    var opt = document.createElement('option');
                    opt.value = resposta.codigo;
                    opt.text = resposta.descricao;
                    var existe = false;
                    var options = $('eqmid[]').options;
                    
                    for(var i = 0; i < options.length; i++){
                        if(options[i].value == resposta.codigo){
                            existe = true;
                        } else if(options[i].value == ''){
                            document.getElementById('eqmid[]').remove(i);
                        }
                    }

                    if(!existe){
                        $('eqmid[]').options.add(opt, 0);
                    }
                }else{
                    alert('Equipamento não encontrado.');
                }
            },
            onFailure: function(){
                alert('Ocorreu um erro ao buscar o equipamento.');
            }
        });
    }else{
        new Ajax.Request('ajax.php',{
            method:'post',
            parameters: {'servico': 'buscarEquipamentoVisitante', 'etiqueta': etiqueta},
            onComplete: function(transport){

                var resposta = transport.responseText.evalJSON();
                if(resposta.status == 'ok'){
	               // if($('vttdoc').getValue() == '') {
	                    recuperarFoto(resposta.vttid, resposta.nu_matricula_siape);
	                    $('vstid').setValue('');
	                    $('vttid').setValue(resposta.vttid);
	                    $('nu_matricula_siape').setValue(resposta.nu_matricula_siape);
	                    if(resposta.nu_matricula_siape){
	            			$('vttdoc').setValue(resposta.nu_matricula_siape);
	            		}else{
	            			$('vttdoc').setValue(resposta.vttdoc);
	            		}
	                    $('vttnome').setValue(resposta.vttnome);
	                    $('vttobs').setValue(resposta.vttobs);
                	//}
                    recuperarEquipamentos(resposta.equipamentos, resposta.selecionado);
                }else{
                    alert('Nenhum registro de entrada foi encontrado com o Número da Etiqueta informado.');
                    limparSaidaVisitante();
                }
            },
            onFailure: function(){
                alert('Ocorreu um erro ao buscar o equipamento.');
            }
        });    }
}

function consultarVisitanteSaida(vttid, vttdoc, nu_matricula_siape, tipo, vstid, servidor) {

    vttdoc = formatarNumeroDocumento(vttdoc, null);
    if(vttid || vttdoc || nu_matricula_siape || vstid){
        new Ajax.Request('ajax.php',{
            method: 'post',
            parameters: {'servico': 'consultarVisitanteSaida', 'vttid': vttid, 'vttdoc': vttdoc, 'nu_matricula_siape': nu_matricula_siape, 'tipo': tipo, 'vstid': vstid},
            onComplete: function(transport){

                var resposta = transport.responseText.evalJSON();
                		
                if(resposta.status == 'ok'){
                    if(tipo == 'V'){
                        if(resposta.visitas.length > 1){
                            //AbrirPopUp('?modulo=principal/visitante/popup/consultarCracha&acao=A&documento=' + vttdoc + '&tipoConsulta=T', 'Pessoas', '\'scrollbars=yes, width=700, height=290\'');
                        	popupRegistroDuplicado(tipo);
                        }else{
                            recuperarFoto(resposta.vttid, resposta.nu_matricula_siape);
                            if(resposta.expedienteNormal)
                                $('edificioVisitante').innerHTML = '';
                            else
                                recuperarEdificio();
                            $('vstid').setValue(resposta.vstid);
                            $('vttid').setValue(resposta.vttid);
                            if(resposta.nu_matricula_siape){
                            	$('vttdoc').setValue(resposta.nu_matricula_siape);
                            	if(servidor){
                            		$('vttcargo').setValue(resposta.ds_cargo_emprego);
                                	$('cargoServidorRol').show();
                            	}                            	
                            } else {
                            	$('vttdoc').setValue(resposta.vttdoc);
                            	if(servidor){
	                            	$('vttcargo').setValue('');
	                            	$('cargoServidorRol').hide();
                            	}
                            }
                            $('nu_matricula_siape').setValue(resposta.nu_matricula_siape);
                            $('vttnome').setValue(resposta.vttnome);
                            $('vttobs').setValue(resposta.vttobs);
                            $('btnGravar').enable();
                            recuperarEquipamentos(resposta.equipamentos, null);
                        }
                    }else{
                        recuperarFoto(resposta.vttid, resposta.nu_matricula_siape);
                        $('vstid').setValue('');
                        $('vttid').setValue(resposta.vttid);
                        $('nu_matricula_siape').setValue(resposta.nu_matricula_siape);
                        if(resposta.nu_matricula_siape){
                        	$('vttdoc').setValue(resposta.nu_matricula_siape);
                        	if(servidor){
	                        	$('vttcargo').setValue(resposta.ds_cargo_emprego);
	                        	$('cargoServidorRol').show();
                        	}
                        } else {
                        	$('vttdoc').setValue(resposta.vttdoc);
                        	if(servidor){
	                        	$('vttcargo').setValue('');
	                        	$('cargoServidorRol').hide();
                        	}
                        }
                        $('vttnome').setValue(resposta.vttnome);
                        $('vttobs').setValue(resposta.vttobs);
                        recuperarEquipamentos(resposta.equipamentos, null);
                    }
                }else{
                	$('btnGravar').disable()
                    alert('Nenhum registro de entrada foi encontrado com o Número de Documento/Crachá informado.');
                    limparSaidaVisitante();
                }
            },
            onFailure: function(){ 
                alert('Ocorreu um erro ao buscar os dados do visitante.');
            }
        });
    }
}

function limparSaidaVisitante(){
    $('vttid').setValue('');
    $('nu_matricula_siape').setValue('');
    $('vttdoc').setValue('');
    $('vttnome').setValue('');
    $('vttobs').setValue('');
    $('fotoVisitante').innerHTML = '';
    $('lista_equipamentos').innerHTML = '';
    if($('edificioVisitante'))
        $('edificioVisitante').innerHTML = '';
    $('vttdoc').focus();
}

function recuperarEquipamentos(equipamentos, selecionado){
    var table = '';
    if(equipamentos.length > 0){
        table += '<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem" style="color:333333;">';
        table += '<thead><tr>';
        table += '<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">Ação</td>';
        table += '<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">Tipo</td>';
        table += '<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">Marca</td>';
        table += '<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">Número de Série</td></tr></thead>';
        table += '<tbody>';

        for (var i = 0; i < equipamentos.length; i++){
            if (i % 2 == 0){
                table += '<tr bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';">';
            } else{
                table += '<tr bgcolor="#F7F7F7" onmouseout="this.bgColor=\'#F7F7F7\';" onmouseover="this.bgColor=\'#ffffcc\';">';
            }

            table += '<td valign="top" title="Ação"><center><input type="checkbox" value="'
                + equipamentos[i].mveid + '" id="eqmid" name="eqmid[]"' + (selecionado == equipamentos[i].mveid ? ' checked="checked"' : '') + '></center></td>';
            table += '<td valign="top" title="Tipo">' + equipamentos[i].tpedsc + '</td>';
            table += '<td valign="top" title="Marca">' + equipamentos[i].mcedsc + '</td>';
            table += '<td valign="top" title="Número de Série">' + equipamentos[i].eqmnumserie + '</td>';
            table += '</tr>';
        }
        table += '</tbody></table>';
    }else {
        table += '<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem" style="color:333333;">';
        table += '<tbody><tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr></tbody></table>';
    }
    $('lista_equipamentos').innerHTML = table;
}

function consultarEntradaCracha(nu_matricula_siape){

    if(nu_matricula_siape){
        new Ajax.Request('ajax.php',{
            method: 'post',
            parameters: {'servico': 'consultarEntradaCracha', 'nu_matricula_siape': nu_matricula_siape},
            onComplete: function(transport){

                var resposta = transport.responseText.evalJSON();
                if(resposta.status == 'ok'){
                    recuperarFoto(null, resposta.nu_matricula_siape);
                    $('nu_matricula_siape').setValue(resposta.nu_matricula_siape);
                    $('vttnome').setValue(resposta.vttnome);
                    $('ds_cargo_emprego').setValue(resposta.ds_cargo_emprego);
                    $('lotacao').setValue(resposta.lotacao);
                    $('sala').setValue(resposta.sala);
                    $('telefone').setValue(resposta.telefone);
                    $('vstnumcracha').focus();
                }else{
                    alert('Nenhum servidor foi encontrado com o Número de Matrícula "' + nu_matricula_siape + '".');
                    limparEntradaCracha();
                }
            },
            onFailure: function(){ 
                alert('Ocorreu um erro ao buscar os dados do servidor.');
            }
        });
    }
}

function limparEntradaCracha(){
    $('nu_matricula_siape').setValue('');
    $('vttnome').setValue('');
    $('lotacao').setValue('');
    $('sala').setValue('');
    $('telefone').setValue('');
    $('vstnumcracha').setValue('');
    $('fotoVisitante').innerHTML = '';
    $('nu_matricula_siape').focus();
    $('ds_cargo_emprego').setValue('');
}

function consultarBaixaCracha(vstnumcracha) {
    
    var formCarregado = $('formCarregado').getValue();
    //if(vstnumcracha && formCarregado == 0){
        new Ajax.Request('ajax.php',{
            method: 'post',
            parameters: {'servico': 'consultarBaixaCracha', 'vstnumcracha': vstnumcracha},
            onComplete: function(transport){

                var resposta = transport.responseText.evalJSON();
                if(resposta.status == 'ok'){
                    recuperarFoto(null, resposta.nu_matricula_siape);
                    $('cpsid').setValue(resposta.cpsid);
                    $('vstnumcracha').setValue(resposta.vstnumcracha);
                    $('vttnome').setValue(resposta.vttnome);
                    $('formCarregado').setValue(1);
                    $('vstnumcracha').focus();
                }else{
                    alert('Nenhum crachá provisório foi encontrado com o Número "' + vstnumcracha + '".');
                    limparBaixaCracha();
                }
            },
            onFailure: function(){
                alert('Ocorreu um erro ao buscar os dados do crachá provisório.');
            }
        });
   // }
}

function limparBaixaCracha(){
    $('cpsid').setValue('');
    $('vstnumcracha').setValue('');
    $('vttnome').setValue('');
    $('ds_cargo_emprego').setValue('');
    $('fotoVisitante').innerHTML = '';
    $('vstnumcracha').focus();
    $('formCarregado').setValue(0);
}










/**
 * Metodo responsavel por pesquisar e setar o solicitante de uma autorizaço de acesso fora do horário
 * ou as pessoas que serão autorizadas
 * 
 * @name autorizarAcesso
 * @param integer  tipoRequisicao  - indica se é para setar o solicitante da autorização ou as pessoas autorizadas(1 para solicitante, 2 para as pessoas)
 * @param string   vttdoc          - documento do visitante a ser pesquisado 
 * @param string   tipoConsulta    - indica qual o tipo da consulta(V-Visitante,S-Servidor,T-Todos)
 * @author Alysson Rafael
 * @return void
 */
function autorizarAcesso(tipoRequisicao,vttdoc,tipoConsulta){
    
    //caso este ajax tenha sido chamado da tela de autorização
    //de aceso fora de horário(consultar solicitante)
    if(tipoRequisicao == 1){
        
        new Ajax.Updater('solicitante', 'ajax.php', {
            method : 'get',
            parameters : '&servico=consultarSolicitanteAcessoForaHorario&nu_cpf='+vttdoc+'&tipoConsulta='+tipoConsulta,
            onComplete : function(res) {
            }
        });
        
    }
    //caso este ajax tenha sido chamado da tela de autorização
    //de aceso fora de horário(consultar pessoas que serão autorizadas)
    else if(tipoRequisicao == 2){
        new Ajax.Request('ajax.php',
                  {
                    method:'get',
                    parameters : '&servico=consultarPessoasAcessoForaHorario&id='+vttdoc+'&tipoConsulta='+tipoConsulta,
                    onComplete: function(transport){
                        var resposta = transport.responseText.evalJSON();    
                        var optn = document.createElement("OPTION");
                        optn.text = resposta.descricao;
                        optn.value = resposta.codigo;
                        
                        var existe = false;
                        for(var i=0; i<document.getElementById('pessoas[]').options.length; i++){
                            if(document.getElementById('pessoas[]').options[i].value == resposta.codigo){
                                existe = true;
                            }
                        }
                        
                        optn.selected = true;
                        var el = document.getElementsByName('pessoas[]')[0];   
                        
                        if(resposta.descricao != '' && resposta.descricao != null && resposta.codigo != '' && resposta.codigo != null && !existe){
                            el.options.add(optn,0);
                        }
                        
                    },
                    onFailure: function(){ alert('Ocorreu um erro') }
                  });
    }
}

/**
 * Método responsável por reajustar o tamanho das abas
 */
function ajustarAbas(){
    $$('img[src$="aba_dir_sel.gif"], img[src$="aba_dir_sel_fim.gif"], img[src$="aba_esq_sel.gif"], img[src$="aba_esq_sel_ini.gif"], img[src$="aba_nosel.gif"], img[src$="aba_nosel_fim.gif"], img[src$="aba_nosel_ini.gif"]').each(function(d){
        d.style.height = '28px';
     });
}
/**
 * Método responsável por validar se uma string possui caracters alfanuméricos e numéricos
 */
function validarStringNumericoAlfa(value){
    return validarStringAlfa(value) && validarStringNumerico(value);
}
/**
 * Método responsável por validar se uma string possui caracters alfanuméricos
 */
function validarStringAlfa(value){
    if(value != null && value != ''){
        value = value.replace(/[\s]/g, '').toUpperCase();

        return (value.replace(/[^A-Z]/g, '')).length > 0;
    } else{
        return false;
    }
}
/**
 * Método responsável por validar se uma string possui caracters numéricos
 */
function validarStringNumerico(value){
    if(value != null && value != ''){
        value = value.replace(/[\s]/g, '').toUpperCase();

        return (value.replace(/[^\d]/g, '')).length > 0;
    } else{
        return false;
    }
}
/**
 * Método responsável por retornar números distintos em uma string
 */
function retornarNumerosDistintos(value, ignoraZero){
    var distintos = '';
    if(value != null && value != ''){
        value = value.replace(/[\s]/g, '').replace(/[^\d]/g, '');
        
        for (var i = 0; i < value.length; i++){
            var c = value.charAt(i);
            if((c != '0' || !ignoraZero) && distintos.indexOf(c) == -1){
                distintos = distintos + c;
            }
        }
    }
    return distintos;
}
/**
 * Método responsável por retornar caracteres distintos em uma string
 */
function retornarCaracteresDistintos(value){
    var distintos = '';
    if(value != null && value != ''){
        value = value.replace(/[\s]/g, '').replace(/[^A-Z]/g, '');
        
        for (var i = 0; i < value.length; i++){
            var c = value.charAt(i);
            if(distintos.indexOf(c) == -1){
                distintos = distintos + c;
            }
        }
    }
    return distintos;
}

/**
 * Método responsável por retornar caracteres validos para consulta de texto( Alfa numericos )
 */
function retirararCaracteresEspeciaisTexto( strValue )
{
    // Retira cacteres especiais
    strValue = strValue.replace( '!', '' );
    strValue = strValue.replace( '@', '' );
    strValue = strValue.replace( '#', '' );
    strValue = strValue.replace( '%', '' );
    strValue = strValue.replace( '$', '' );
    strValue = strValue.replace( '¨', '' );

    return strValue;
}

/**
 * Método responsável por formatar um número de documento
 */
function formatarNumeroDocumento(valor, tipo){
    if(valor && valor != ''){
        if(tipo && tipo == 'CPF'){
            return mascaraglobal('###.###.###-##', valor);
        }else{
            return valor.toUpperCase().replace(/[\s]/g, '').replace(/[^A-Z0-9]/g, '');
        }
    } else{
        return '';
    }
}

function buscarDocumentoPorEtiqueta(eqmnumetiqueta){
    
    var resposta = false;

    new Ajax.Request('ajax.php', {
        method : 'get',
        asynchronous: false,
        parameters : '&servico=buscarDocumentoPorEtiqueta&eqmnumetiqueta='+eqmnumetiqueta,
        onComplete : function(transport) {
            var dados = transport.responseText.evalJSON();
            if( dados.vttid != ''){
            	consultarVisitanteSaida(dados.vttid,'','','','');
                resposta =  true;
            }
        }
    });
    
    return resposta;
    
}