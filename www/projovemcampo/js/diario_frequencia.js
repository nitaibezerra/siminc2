var DiarioFrequencia    = {
    
    init : function(){
        $('#entid').change( DiarioFrequencia.escolaChangeHandler );
        $('#btnGerarDiario').click(DiarioFrequencia.gerarDiarioHandler );
        $('#btnVisualizarDiario').click( DiarioFrequencia.visualizarDiarioPeriodoHandler );
        $('#perid').change( function(){
            $('#container-diario').html();
        } );
        
        jQuery.ajaxSetup({
            beforeSend: function(){
                $("#dialogAjax").show();
            },
            complete: function(){
                $("#dialogAjax").hide();
            }
        });
    },
    
    escolaChangeHandler : function(){
        var entid   = $('#entid').val(), 
        params  = {};
        $('#container-diario').html();
            
        if( entid == '' )
        {
            return false;
        }
        params['entid'] = entid;
        params['acao']  = 'listarTurma';
        
        $.post( 'geral/ajax.php', params, function(response){
            $('#container-turma').html( response );
            $('#turid').change( DiarioFrequencia.turmaChangeHandler );
        }, 'html' );
            
            
        return true;
    },
    
    turmaChangeHandler : function(){
    	
        $('#container-diario').html('');
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
        params['gerarDiario'] = '1';
        params['acao']  = 'listarPeriodos';
        
        $.post( 'geral/ajax.php', params, function(response){
            $('#container-periodo').html( response );
        }, 'html' );
            
        return true;
    },
    
       
	gerarDiarioHandler : function(){
        
        $('#btnGerarDiario').attr('disabled', true);
        
        if( $('#frmDiarioFrequencia').valid() == false )
        {
            return false;
        }
        
        var params = $('#frmDiarioFrequencia').serialize();
        
        params += '&acao=gerarDiarioPeriodo';
        
        $.post( 'geral/ajax.php', params, function(response){
            $('#container-diario').html( response );
            $('.visualizarDiarioGrid').click( DiarioFrequencia.visualizarDiarioHandler );
            $('.visualizarDiarioTrabalhoGrid').click( DiarioFrequencia.visualizarDiarioTrabalhoHandler );
        }, 'html' );
        
        return true;
    },
    
    visualizarDiarioPeriodoHandler : function(){
        
        if( $('#frmDiarioFrequencia').valid() == false )
        {
            return false;
        }
        
        var params = $('#frmDiarioFrequencia').serialize();
        
        params += '&acao=visualizarDiarioPeriodo';
        
        $.post( 'geral/ajax.php', params, function(response){
            $('#container-diario').html( response );
            $('.visualizarDiarioGrid').click( DiarioFrequencia.visualizarDiarioHandler );
            $('.visualizarDiarioTrabalhoGrid').click( DiarioFrequencia.visualizarDiarioTrabalhoHandler );
        }, 'html' );
        
        return true;
    },
    
    visualizarDiarioHandler : function(){
        var diaId   = parseInt($(this).attr('id'));
        var params  = {};
        var idMateria = parseInt($(this).attr('idMateria'));
        var tipoensino = $(this).attr('tipoensino');
        if( diaId == NaN )
        {
            alert(' Erro ao buscar o diário. Informe o desenvolvedor do sistema.');
            return false;
        }
        params['tipoensino'] = tipoensino;
        params['idMateria'] = idMateria;
        params['diaId'] = diaId;
        params['acao']  = 'visualizarDiario';
        
        
        $.post( 'geral/ajax.php', params, function(response){
            $('#container-diario').html( response );
            $('.visualizarDiarioGrid').click( DiarioFrequencia.visualizarDiarioHandler );
            
        }, 'html' );
            
        return true;
    },
};