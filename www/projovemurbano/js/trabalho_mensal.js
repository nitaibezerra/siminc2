var TrabalhoMensal    = {
    
    init : function(){
        $('#nucid').change( TrabalhoMensal.nucleoChangeHandler );
        $('#btnVisualizarDiario').click( TrabalhoMensal.visualizarTrabalhoHandler )
                                 .attr('disabled', true);
        jQuery.ajaxSetup({
            beforeSend: function(){
                $("#dialogAjax").show();
            },
            complete: function(){
                $("#dialogAjax").hide();
            }
        });
        
        TrabalhoMensal.verificaCamposPreenchidos();
    },
    
    verificaCamposPreenchidos : function(){
        if( jQuery.trim($('#perid').val()) != '' && 
                jQuery.trim($('#turid').val() != '') ){
                TrabalhoMensal.visualizarTrabalhoHandler();
                $('#btnVisualizarDiario').attr('disabled', false);
        }
    },
    
    nucleoChangeHandler : function(){
        var nucid   = $('#nucid').val(), 
            params  = {};
         $('#container-trabalho-mensal').html('');
            
        if( nucid == '' )
        {
            return false;
        }
        
        params['nucid'] = nucid;
        params['acao']  = 'listarTurma';
        
        $.post( 'geral/ajax.php', params, function(response){
            $('#container-turma').html( response );
            $('#turid').change( TrabalhoMensal.turmaChangeHandler );
        }, 'html' );
            
            
        return true;
    },
    
    turmaChangeHandler : function(){

        var turid   = $('#turid').val(),
            params  = {};
        
        $('#container-trabalho-mensal').html('');
        
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
            $('select#perid').change( TrabalhoMensal.periodoChangeHandler );
        }, 'html' );
            
        return true;
    },
    
    visualizarTrabalhoHandler : function()
    {
        var perId   = $('#perid').val(),
            turId   = $('#turid').val(),
            params  = {};
            
        if( perId == '' )
        {
            alert(' Selecione um período para visualização do(s) diário(s) de trabalho.');
            return false;
        }
        
        params['perid'] = perId;
        params['turid'] = turId;
        params['acao']  = 'visualizarDiarioTrabalhoMensal';
        
        
        $.post( 'geral/ajax.php', params, function(response){
            $('#container-trabalho-mensal').html( response );
            $('#btnSalvarTrabalho').click( TrabalhoMensal.salvarDiarioTrabalhoHandler );
            $('#btnFecharTrabalho').click(  TrabalhoMensal.fecharDiario );
        }, 'html' );
            
        return true;
    },
    
    salvarDiarioTrabalhoHandler : function()
    {
        var params;
        
        $('#acao').val('salvarDiarioTrabalho');
        params = $('#frmTrabalho').serialize();
                    
        $.post( 'geral/ajax.php', params, function(response){
            alert( response );

        });
    },
    
    fecharDiario :function()
    {
        if( confirm('Deseja realmente fechar esse diário?\nApós o seu fechamento o mesmo não poderá ser mais editado.') )
            {
                var params;
        
                $('#acao').val('fecharDiario');
                
                params = $('#frmTrabalho').serialize();
                
                params['diaid'] = $('#diaid').val();
                
                $.post( 'geral/ajax.php', params, function(response){
                    
                    alert(response.retorno);

                    if( response.status == true  )
                    {
                        TrabalhoMensal.visualizarTrabalhoHandler();
                    }
                    
                }, 'json' );
            }
    },
    
    periodoChangeHandler : function(){
      
        var params = {};
        var periodo = $('select#perid').val();
        
        $('#container-trabalho-mensal').html('');
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