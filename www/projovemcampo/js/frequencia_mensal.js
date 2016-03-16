var DiarioFrequenciaMensal    = {
    init : function(){
        $('#entid').change( DiarioFrequenciaMensal.escolaChangeHandler );
        $('#btnVisualizarDiario').click( DiarioFrequenciaMensal.visualizarDiarioFrequenciaMensalHandler )
        .attr('disabled', true);
        
        jQuery.ajaxSetup({
            beforeSend: function(){
                $("#dialogAjax").show();
            },
            complete: function(){
                $("#dialogAjax").hide();
            }
        });
    },
    verificaSomaAulaDadas : function() {
        
        var  totalAulaInformada         = 0
        , totalAulaPrevista     = parseInt($('#qtd_aula_prevista').val())
        , valorAulaComponente   = 0;
                
        $('input[name^="qtdaulasdadas["]').each(function( idx, el ){
        	
            valorAulaComponente = parseInt($(el).val());

            if( isNaN( valorAulaComponente  ) )
                valorAulaComponente = 0;

            totalAulaInformada += valorAulaComponente;
        });

        if( totalAulaInformada > totalAulaPrevista )
            return false;
        
        return true;
    },
    escolaChangeHandler : function(){
        
        $('#msg').html('');

        var entid   = $('#entid').val(), 
        params  = {};

        $('#container-diario-frequencia-mensal').html('');
            
        if( entid == '' )
        {
            $('#turid option:[value!=""]').remove();
            $('#perid option:[value!=""]').remove();
        	
            $('#turid option:first').html("Selecione uma escola");
            $('#perid option:first').html("Selecione uma turma");

            return false;
        }
        
        params['entid'] = entid;
        params['acao']  = 'listarTurma';
        
        $.post( 'geral/ajax.php', params, function(response){
            $('#container-turma').html( response );
            $('#turid').change( DiarioFrequenciaMensal.turmaChangeHandler );
        }, 'html' );
            
            
        return true;
    },
    
    turmaChangeHandler : function(){
    	
        $('#container-diario-frequencia-mensal').html('');
        $('#msg').html('');

        var turid       = $('#turid').val()
        , params    = {};
        
        if( turid == '' )
        {
            $('#perid option:[value!=""]').remove();
            $('#perid option:first').html("Selecione uma turma");
        	
            return false;
        }
        
        params['turid'] = turid;
        params['acao']  = 'listarPeriodos';
        
        $.post( 'geral/ajax.php', params, function(response){
            $('#container-diario').html( response );
            $('select#perid').change( DiarioFrequenciaMensal.periodoChangeHandler );
        }, 'html' );
            
        return true;
    },
    
    
    visualizarDiarioFrequenciaMensalHandler : function(){

        if( $('#frmDiarioFrequenciaMensal').valid() == false )
        {
            return false;
        }

        var params  = {},
        perid   = $('#perid').val(),
        turid   = $('#turid').val(),
        entid   = $('#entid').val();
    	
        params['acao']  = 'visualizarDiarioFrequenciaMensal';
        params['perid'] = perid;
        params['turid'] = turid;
        params['entid'] = entid;
        
        $.post( 'geral/ajax.php', params, function(response){
            $('#container-diario-frequencia-mensal').html( response );
            $('#salvarDiarioFrequenciaMensal').click( DiarioFrequenciaMensal.salvarDiarioHandler );
            $('#btnFecharTrabalho').click(  DiarioFrequenciaMensal.fecharDiario );
            
            
//            $('input[name^="qtdaulas["]').blur(function(){
//            
//                var input = this
//                , totalAulaInformada
//                , arrCodAlunoDisciplina, diaId
//                , totalAulasDadasComponente;
//                
//                arrCodAlunoDisciplina = $(this).attr('id').replace('qtdaulas_', '').split('_');
//
//                diaId = arrCodAlunoDisciplina[0];
//
//                totalAulasDadasComponente   = parseInt($('input[name="qtdaulasdadas['+ diaId +']"]').val());
//                totalAulaInformada          = parseInt($(input).val());
//
//
//                if( totalAulaInformada > totalAulasDadasComponente )
//                {
//                    alert('A quantidade de presenças informada é maior que a quantidade de aulas dadas.');
//                    $(input).val('');
//                    $(input).focus();
//                }
//
//            });

//            $('input[name^="qtdaulasdadas["]').blur(function(){
//            	
//                var tempo = this
//                    , idElemento
//                    , cpdata
//                    , cpClass
//                    , cpValor
//                    , valert = "";
//                
//                $(this).each(function( idx, el ){
//
//                    idElemento = $(el).attr('name').replace('qtdaulasdadas[', '').replace(']','');
//
//                    cpClass = '.qtdaulas_' + idElemento;
//                    cpValor = $(el).val();
//                    
//                    if (cpValor == ""){
//                        valert = "ATENÇÃO: campo Aulas Dadas vazio, favor preencher!";
//                        $(el).focus();
//                        return false;
//                    }
//                    
//                    $(cpClass).each(function( idx2, el2 ){
//                    	if( parseInt($(el2).val()) > parseInt(cpValor) ){
//                            valert = 'A quantidade de presenças informada é maior que a quantidade de aulas dadas.';
//                        }
//                    });
//                });
//                
//                if (valert != ""){ 
//                    $(this).focus();
//                    alert(valert);
//                    return false;
//                }    
//                
//            });
            
        }, 'html' );
        
        return true;
    },
    fecharDiario :function()
    {
    	$('#btnFecharTrabalho').attr('disabled', true);
        if( confirm('Deseja realmente fechar esse diário?\nApós o seu fechamento o mesmo não poderá ser mais editado.') )
            {
                var params;
                
                params = $('#frmDiarioFrequenciaMensal').serialize();
                params += '&acao=fecharDiario';
                
                params['diaid'] = $('#diaid').val();
                
                $.post( 'geral/ajax.php', params, function(response){
                	
                    console.log(response);

                    if( response == 1  )
                    {
                    	alert('Diario fechado com sucesso');
                    	DiarioFrequenciaMensal.visualizarDiarioFrequenciaMensalHandler();
                    }else if(response == 2){
                    	alert('Erro ao fechar o diário. Não foi possível localizar o diário');
                    }else if(response ==3){
                    	alert('Não foi possível fechar o diário.');
                    }else if(response ==4){
                        alert('Erro ao fechar o diário. É necessário vincular uma agência a essa escola.');
                    }
                    
                }, 'html' );
            }
    },

    salvarDiarioHandler : function(){
        
        var idElemento
            , valert = ""
            , cpClass
            , cpValor
            , params
            , paramsRedirecionamento;
        
        $('input[name^="qtdaulasdadas["]').each(function( idx, el ){
            idElemento = $(el).attr('name').replace('qtdaulasdadas[', '').replace(']','');

            cpClass = '.qtdaulas_' + idElemento;
            cpValor = $(el).val();

            if(cpValor == ""){
                valert = "ATENÇÃO: campo referente a 'Aulas Dadas' está vazio, favor preencher!";
                $(el).focus();
                return false;
            }
                
            $(cpClass).each(function( idx2, el2 ){
                if( parseInt($(el2).val()) > parseInt(cpValor) ){
                    valert = 'A quantidade de presenças informada é maior que a quantidade de aulas dadas.';
                }
            });
        });
            
        if (valert != ""){
            alert(valert);
            return false;
        }  
            
            
//        if( DiarioFrequenciaMensal.verificaSomaAulaDadas() == false )
//        {
//            alert('A soma de aulas dadas é maior que o total de aulas previstas');
//            return false;
//        }
        
        params = $('#frmDiarioFrequenciaMensal').serialize();
        
        paramsRedirecionamento  = {
            perid   : $('#perid').val(),
            turid   : $('#turid').val(),
            entid   : $('#entid').val()
        };
        
        params += '&acao=salvarDiarioFrequenciaMensal';
        
        $('#salvarDiarioFrequenciaMensal').attr("disabled", "disabled");
        $('#salvarDiarioFrequenciaMensal').val("Salvando...Aguarde!");
        
        $.post( 'geral/ajax.php', params, function(response){
            
            var json = jQuery.parseJSON(response);
            
            if(json.status == true)
            {
                //$('#btnVisualizarDiario').trigger('click');
                alert( json.retorno );
                document.location.href = 'projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=frequenciaMensal';
            }
            
        }, 'html' );
        return true;
    	
    },
    
    //FIXME - Até aqui foi refatorado
    
    visualizarDiarioPeriodoHandler : function(){
        
        if( $('#frmDiarioFrequenciaMensal').valid() == false )
        {
            return false;
        }
        
        var params = $('#frmDiarioFrequenciaMensal').serialize();
        
        params += '&acao=visualizarDiarioPeriodo';
        
        $.post( 'geral/ajax.php', params, function(response){
            $('#container-diario').html( response );
            $('.visualizarDiarioGrid').click( DiarioFrequenciaMensal.visualizarDiarioHandler );
            $('.visualizarDiarioTrabalhoGrid').click( DiarioFrequenciaMensal.visualizarDiarioTrabalhoHandler );
        }, 'html' );
        
        return true;
    },
    
    visualizarDiarioHandler : function(){
        var diaId   = parseInt($(this).attr('id'));
        var params  = {};
        if( diaId == NaN )
        {
            alert(' Erro ao buscar o diário. Informe o desenvolvedor do sistema.');
            return false;
        }
        
        params['diaId'] = diaId;
        params['acao']  = 'visualizarDiario';
        
        
        $.post( 'geral/ajax.php', params, function(response){
            $('#container-diario').html( response );
            $('.visualizarDiarioGrid').click( DiarioFrequenciaMensal.visualizarDiarioHandler );
            
        }, 'html' );
            
        return true;
    },
    
    visualizarDiarioTrabalhoHandler : function(){
        var diaId   = parseInt($(this).attr('id'));
        var params  = {};
        
        if( diaId == NaN )
        {
            alert(' Erro ao buscar o diário. Informe o desenvolvedor do sistema.');
            return false;
        }
        
        params['diaId'] = diaId;
        params['acao']  = 'visualizarDiarioTrabalho';
        
        
        $.post( 'geral/ajax.php', params, function(response){
            $('#container-diario').html( response );
        }, 'html' );
            
        return true;
    },
    periodoChangeHandler : function(){
      
        var params = {};
        var periodo = $('select#perid').val();
        
        $('#container-diario-frequencia-mensal').html('');
        $('#msg').html('');
        
        if (periodo != ''){
          
            var dtUltimoPeriodo = $("select#perid option:selected").text().split(" ");

            params['acao']  = 'retornaDtUltimoPeriodo';
            params['dtUltimoPeriodo'] = dtUltimoPeriodo[5];
            params['qtdDiasSomar'] = '5';

            $.post( 'geral/ajax.php', params, function(resposta){
                
                var response = jQuery.parseJSON(resposta);
                
                if( response.status == true ) 
                {
                    $('#btnVisualizarDiario').attr('disabled', false);
                }
                else 
                {
                    var proHtml = $('<span/>');
                    proHtml.css('color', '#cc0000');
                    $('#msg').text( response.retorno ).css('color', '#cc0000'); 
                    $('#btnVisualizarDiario').attr('disabled', true);
                }
                
            }, 'html' );

            return true;
        }

        return false;        
    }
};