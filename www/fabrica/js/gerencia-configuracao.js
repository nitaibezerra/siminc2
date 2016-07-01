var GerenciaConfiguracao = {
		
	init : function() {

		$('#data_abertura_fim, #data_abertura_inicio').click(function(){
			$('#error_primeiro_periodo').hide();
			$('#data_abertura_fim').removeClass('error');
		});
		
		$('#data_finalizacao_fim, #data_finalizacao_inicio').click(function(){
			$('#error_segundo_periodo').hide();
			$('#data_finalizacao_fim').removeClass('error');
		});
	
		$("#submit-listaAuditoriaGc").click(function(event){
			event.preventDefault();
			
			var data_abertura_inicio = $('#data_abertura_inicio').val();
			var data_abertura_fim = $('#data_abertura_fim').val();
			var data_finalizacao_inicio = $('#data_finalizacao_inicio').val();
			var data_finalizacao_fim = $('#data_finalizacao_fim').val();
			
			var valido = true;
			if (data_abertura_inicio != '' && data_abertura_fim){
				data_abertura_inicio = data_abertura_inicio.substring(6,10) + data_abertura_inicio.substring(3,5) + data_abertura_inicio.substring(0,2);
				data_abertura_fim = data_abertura_fim.substring(6,10) + data_abertura_fim.substring(3,5) + data_abertura_fim.substring(0,2);
				if (data_abertura_inicio > data_abertura_fim) {
					$('#data_abertura_fim').addClass('error');
					$('#error_primeiro_periodo').show();
				}
				valido = false;
			}
			
			if (data_finalizacao_inicio != '' && data_finalizacao_fim){
				data_finalizacao_inicio = data_finalizacao_inicio.substring(6,10) + data_finalizacao_inicio.substring(3,5) + data_finalizacao_inicio.substring(0,2);
				data_finalizacao_fim = data_finalizacao_fim.substring(6,10) + data_finalizacao_fim.substring(3,5) + data_finalizacao_fim.substring(0,2);
				if (data_finalizacao_inicio > data_finalizacao_fim) {
					$('#data_finalizacao_fim').addClass('error');
					$('#error_segundo_periodo').show();
				}
				valido = false;
			}
			
			if (valido){
				$("#form-listaAuditoriaGc").submit();
			}
//			GerenciaConfiguracao.atribuiFocusAoCampoSS();
		});
		
//		$("#submit-listaTodasSSGc").click(function(event){
//			event.preventDefault();
//			GerenciaConfiguracao.recupereTodasSolicitacoesDosSistema();
//		});
		
		$("#reset-limparAuditoriaGc").click(function(event){
			event.preventDefault();
			$('#scsid').val("").focus();
			$('#data_abertura_inicio').val("");
			$('#data_abertura_fim').val("");
			$('#data_finalizacao_inicio').val("");
			$('#data_finalizacao_fim').val("");
			$('select option').removeAttr('selected');
			$('#gc-fiscais-sistemas, #gc-fiscais-selecione, #gc-situacoes-selecione').attr("selected", "selected");
	        $("#situacoes").removeAttr("disabled");
	        $('input[type=text]').removeClass('error');
	        $('#gc-minhasAuditorias').removeAttr('checked');
	        $('label.error').hide();
	    });
		
		$("#cancelarModalArtefatosVisaoMEC").click(function(event){
			event.preventDefault();
			$("#modalArtefatosVisaoMEC").dialog("close");
			parent.location='?modulo=principal/gerencia-configuracao/listarArtefatos&acao=A&solicitacao=' + $("#idSS").text();
		});
		
	    $("#salvarModalArtefatosVisaoMEC").click(function(event){
	        event.preventDefault();
	        if ($("#resultadoAuditoriaVisaoMEC").val()!=0){
	        	GerenciaConfiguracao.salvarModalArtefatos();
	        } else {
	        	$("#resultadoAuditoriaVisaoMEC").addClass('error');
	        	$("#error_resultado_auditoria").show();
	        }
	    });
	    
	    $("#cancelarModalArtefatosCasoDeUso").click(function(event){
	        event.preventDefault();
	        $("#modalArtefatosCasoDeUso").dialog("close");
	        parent.location='?modulo=principal/gerencia-configuracao/listarArtefatos&acao=A&solicitacao=' + $("#idSS").text();
	    });
		
	    $("#salvarModalArtefatosCasoDeUso").click(function(event){
	        event.preventDefault();
	        if ($("#formModalArtefatosCasoDeUso").valid()){
	    		GerenciaConfiguracao.validarArtefato();
	        }
	    });
	    
	    $("#gc-salvarAuditoriaArtefatos").click(function(event){
	        event.preventDefault();
	        if ($("#form-listaArtefatosGc").valid()){ 
	        	GerenciaConfiguracao.salvarAuditoriaArtefatos();
	        }
	    });
	    
	    $("#gc-cancelarArtefatos").click(function(event){
	        event.preventDefault();
	        parent.location='?modulo=principal/gerencia-configuracao/listar&acao=A';
	    });
	    
	    $("#modalArtefatosCasoDeUso, #modalArtefatosVisaoMEC").dialog({
	        autoOpen: false,
	        modal: true,
	        resizable: false,
	        width: 600,
	        draggable: false,
	        dialogClass: 'modalFabrica'
	    });
	    
	    $("#modalVerHistorico").dialog({
	        autoOpen: false,
	        modal: true,
	        resizable: false,
	        width: 600,
	        height: 600,
	        draggable: false,
	        title: 'Histórico de Auditoria',
	        dialogClass: 'modalFabrica',
	        buttons: {
	    		"fechar": function() {
	    			$(this).dialog("close"); 
	    		} 
	    	} 
	    });
	    
	    GerenciaConfiguracao.atribuiFocusAoCampoSS();
	    
	    GerenciaConfiguracao.registraValidadores();
	    
	    $('#repositorioCasoDeUso').focus(function(){
	    	$(this).removeClass('error');
	    });
	    
	    $('span.linkVerHistorico').click(function(){
	    	var idAuditoria = $(this).attr('id');
	    	GerenciaConfiguracao.verHistorico(idAuditoria);
	    });
	    
	    $('table[style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;"]').css('width','15%').css('border-width','1px').css('border-top','0px none');
	    
	},
	
	atribuiFocusAoCampoSS : function(){
		$('#scsid').focus();
	},
	
	registraValidadores : function(){
		
		jQuery.validator.addMethod("intervaloData", function(value, element, dataInicial) {
	        //Deve ser utilizado na data final passando o o elemento da data Inicial
	        //valor que esta sendo validado
	        //elemento que esta sendo validado
	        var valido = true;
			
	        var dataInicio = $(dataInicial).val();
	        var dataInicioConvertida = dataInicio.substring(6,10) + dataInicio.substring(3,5) + dataInicio.substring(0,2);
	        var dataFimConvertida = value.substring(6,10) + value.substring(3,5) + value.substring(0,2);
			
	        if (value != "" && $(dataInicial).val()==""){
	            valido = false;
	        }
			
	        if (dataInicioConvertida > dataFimConvertida){
	            valido = false
	        }
			
	        return valido;
	    },"Período inválido");
		
		
//		$("#form-listaAuditoriaGc").validate({
//	        rules:{
//	            data_abertura_fim: {
//	                intervaloData: "#data_abertura_inicio"
//	            },
//	            data_finalizacao_fim: {
//	                intervaloData: "#data_finalizacao_inicio"
//	            }
//	        }, 
//	        messages:{
//	            dataAberturaFim: "Período inválido",
//	            dataFinalizacaoFim: "Período inválido"
//	        }
//	    });
		
		$("#formModalArtefatosCasoDeUso").validate({
	        rules:{
				repositorioCasoDeUso: "required",
				padraoNomesCasoDeUso: "required",
				padraoDiretorioCasoDeUso: "required",
				encontradoCasoDeUso: "required",
				atualizadoCasoDeUso: "required",
				necessarioCasoDeUso: "required"
	        }, 
	        messages:{
	        	repositorioCasoDeUso: "Campo de preenchimento obrigatório",
	        	padraoNomesCasoDeUso: "Campo de preenchimento obrigatório",
				padraoDiretorioCasoDeUso: "Campo de preenchimento obrigatório",
				encontradoCasoDeUso: "Campo de preenchimento obrigatório",
				atualizadoCasoDeUso: "Campo de preenchimento obrigatório",
				necessarioCasoDeUso: "Campo de preenchimento obrigatório"
	        }
	    });
		
	},
	
	recupereTodasSolicitacoesDosSistema: function(){
		$.ajax({
			beforeSend: function(){
				$('#aguarde').css('visibility', 'visible');
				$('#aguarde').show();
			},
			type: 'post',
			url:'geral/gerencia-configuracao/recupereTodasSS.php',
			cache: false,
			dataType: 'html',
			success: function(data){
				$('#grid').empty();
				$('#grid').html(data);
			},
			complete: function(){
				$('#aguarde').css('visibility', 'hidden');
				$('#aguarde').hide();
			}
		});
	},
	
	recupereSolicitacoesPassiveisAuditoria : function(){
		$.ajax({
			beforeSend: function(){
				$('#aguarde').css('visibility', 'visible');
				$('#aguarde').show();
			},
			type: 'post',
			url:'geral/gerencia-configuracao/listar.php',
			cache: false,
			dataType: 'html',
			data: $('#form-listaAuditoriaGc').serialize(),
			success: function(data){
				$('#grid').empty();
				$('#grid').html(data);
			},
			complete: function(){
				$('#aguarde').css('visibility', 'hidden');
				$('#aguarde').hide();
			}
		});
	},
	
	salvarModalArtefatos : function() {
		 $.ajax({
            beforeSend: function(){
                $("#dialogAjax").show();
            },
            type: 'post',
            url:'geral/gerencia-configuracao/salvarDetalhesAuditoria.php',
            cache: false,
            dataType: 'html',
            data: $('#formModalArtefatosVisaoMEC').serialize(),
            success: function(data){
                if ($.isNumeric(data)){
                    $("#dialogAjax").hide();
                    $("#modalArtefatosVisaoMEC").dialog("close");
                }
            },
            complete: function(){
                parent.location='?modulo=principal/gerencia-configuracao/listarArtefatos&acao=A&solicitacao=' + $("#idSS").text();
            }
        });
	},
	
	salvarModalArtefatosCasoDeUso : function(){
		$.ajax({
            beforeSend: function(){
				$('#aguarde').css('visibility', 'visible');
				$('#aguarde').show();
            },
            type: 'post',
            url:'geral/gerencia-configuracao/salvarServicoFaseProduto.php',
            cache: false,
            dataType: 'html',
            data: $('#formModalArtefatosCasoDeUso').serialize(),
            success: function(data){
                if ($.isNumeric(data)){
    				$('#aguarde').css('visibility', 'hidden');
    				$('#aguarde').hide();
                    $("#modalArtefatosCasoDeUso").dialog("close");
                    parent.location='?modulo=principal/gerencia-configuracao/listarArtefatos&acao=A&solicitacao=' + $("#idSS").text();
                }
            },
            complete: function(){
                $("#repositorioCasoDeUso").val('');
                $("#idServicoFaseProdutoCasoDeUso").val('');
                $('#formModalArtefatosCasoDeUso option').removeAttr('selected');
            }
        });
	},
	
	salvarAuditoriaArtefatos : function() {
        $.ajax({
            beforeSend: function(){
				$('#aguarde').css('visibility', 'visible');
				$('#aguarde').show();
            },
            type: 'post',
            url:'geral/gerencia-configuracao/listarArtefatos.php',
            cache: false,
            dataType: 'json',
            data: $('#form-listaArtefatosGc').serialize(),
            success: function(data){
				$('#aguarde').css('visibility', 'hidden');
				$('#aguarde').hide();
            },
            complete: function(){
            	parent.location='?modulo=principal/gerencia-configuracao/listarArtefatos&acao=A&solicitacao=' + $("#idSS").text();
            }
        });
	},
	
	validarArtefato : function(){
		$.ajax({
            beforeSend: function(){
				$('#aguarde').css('visibility', 'visible');
				$('#aguarde').show();
            },
            type: 'post',
            url:'geral/gerencia-configuracao/validarArtefato.php',
            cache: false,
            dataType: 'json',
            data: $('#formModalArtefatosCasoDeUso').serialize(),
            success: function(data){
            	if (data.STATUS != "ERROR"){
            		GerenciaConfiguracao.salvarModalArtefatosCasoDeUso();
            	} else {
            		alert(data.MENSAGEM);
            		$('#repositorioCasoDeUso').addClass('error');
            	}
            }
        });
	},
	
	verHistorico : function( idAuditoria ){
		$.ajax({
            beforeSend: function(){
				$('#aguarde').css('visibility', 'visible');
				$('#aguarde').show();
            },
            type: 'post',
            url:'geral/gerencia-configuracao/verHistorico.php',
            cache: false,
            dataType: 'html',
            data: 'idAuditoria=' + idAuditoria,
            success: function(data){
            	$("#modalVerHistorico").html(data);
            	$("#modalVerHistorico").dialog('open');
            },
			complete: function(){
				$('#aguarde').css('visibility', 'hidden');
				$('#aguarde').hide();
			}
        });	
	},
	
	alterarQtdeRegistrosPesquisa : function( qtde ){
		$('#paginador span.qtdeTotalRegistros').text(qtde);
	}

}

$(function(){
	GerenciaConfiguracao.init();
});