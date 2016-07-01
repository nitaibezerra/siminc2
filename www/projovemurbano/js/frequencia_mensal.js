var DiarioFrequenciaMensal    = {
    
    init : function(){
        $('#nucid').change( DiarioFrequenciaMensal.nucleoChangeHandler );
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
    
    nucleoChangeHandler : function(){
        
        $('#msg').html('');

        var nucid   = $('#nucid').val(), 
        params  = {};

        $('#container-diario-frequencia-mensal').html('');
            
        if( nucid == '' )
        {
            $('#turid option:[value!=""]').remove();
            $('#perid option:[value!=""]').remove();
        	
            $('#turid option:first').html("Selecione um núcleo");
            $('#perid option:first').html("Selecione uma turma");

            return false;
        }
        
        params['nucid'] = nucid;
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
        nucid   = $('#nucid').val();
    	
        params['acao']  = 'visualizarDiarioFrequenciaMensal';
        params['perid'] = perid;
        params['turid'] = turid;
        params['nucid'] = nucid;
        
        $.post( 'geral/ajax.php', params, function(response){
            $('#container-diario-frequencia-mensal').html( response );
            $('#salvarDiarioFrequenciaMensal').click( DiarioFrequenciaMensal.salvarDiarioHandler );
            
            $('input[name^="qtdaulas["]').blur(function(){
            
                var input = this
                , totalAulaInformada
                , arrCodAlunoDisciplina, difId
                , totalAulasDadasComponente;

                arrCodAlunoDisciplina = $(this).attr('id').replace('qtdaulas_', '').split('_');

                difId = arrCodAlunoDisciplina[0];

                totalAulasDadasComponente   = parseInt($('input[name="qtdaulasdadas['+ difId +']"]').val());
                totalAulaInformada          = parseInt($(input).val());


                if( totalAulaInformada > totalAulasDadasComponente )
                {
                    alert('A quantidade de presenças informada é maior que a quantidade de aulas dadas.');
                    $(input).val('');
                    $(input).focus();
                }

            });

            $('input[name^="qtdaulasdadas["]').blur(function(){

                var tempo = this
                    , idElemento
                    , cpdata
                    , cpClass
                    , cpValor
                    , valert = "";

                $(this).each(function( idx, el ){

                    idElemento = $(el).attr('name').replace('qtdaulasdadas[', '').replace(']','');

                    cpClass = '.qtdaulas_' + idElemento;
                    cpValor = $(el).val();
                    
                    if (cpValor == ""){
                        valert = "ATENÇÃO: campo Aulas Dadas vazio, favor preencher!";
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
                    $(this).focus();
                    alert(valert);
                    return false;
                }    
                
            });
            
        }, 'html' );
        
        return true;
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
            
            
        if( DiarioFrequenciaMensal.verificaSomaAulaDadas() == false )
        {
            alert('A soma de aulas dadas é maior que o total de aulas previstas');
            return false;
        }
        
        params = $('#frmDiarioFrequenciaMensal').serialize();
        
        paramsRedirecionamento  = {
            perid   : $('#perid').val(),
            turid   : $('#turid').val(),
            nucid   : $('#nucid').val()
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
                document.location.href = 'projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=trabalhoMensal&'+ $.param(paramsRedirecionamento);
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
        var difId   = parseInt($(this).attr('id'));
        var params  = {};
        if( difId == NaN )
        {
            alert(' Erro ao buscar o diário. Informe o desenvolvedor do sistema.');
            return false;
        }
        
        params['difid'] = difId;
        params['acao']  = 'visualizarDiario';
        
        
        $.post( 'geral/ajax.php', params, function(response){
            $('#container-diario').html( response );
            $('.visualizarDiarioGrid').click( DiarioFrequenciaMensal.visualizarDiarioHandler );
            
        }, 'html' );
            
        return true;
    },
    
    visualizarDiarioTrabalhoHandler : function(){
        var difId   = parseInt($(this).attr('id'));
        var params  = {};
        
        if( difId == NaN )
        {
            alert(' Erro ao buscar o diário. Informe o desenvolvedor do sistema.');
            return false;
        }
        
        params['difid'] = difId;
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