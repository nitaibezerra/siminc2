var DiarioFrequencia    = {
    
    init : function(){
        $('#nucid').change( DiarioFrequencia.nucleoChangeHandler );
        $('#btnGerarDiario').click(DiarioFrequencia.gerarDiarioHandler );
        $('#btnVisualizarDiario').click( DiarioFrequencia.visualizarDiarioPeriodoHandler );
        $('#cocid').change( DiarioFrequencia.componenteChangeHandler );
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
    
    nucleoChangeHandler : function(){
        var nucid   = $('#nucid').val(), 
            params  = {};
        $('#container-diario').html();
            
        if( nucid == '' )
        {
            return false;
        }
        
        params['nucid'] = nucid;
        params['acao']  = 'listarTurma';
        
        $.post( 'geral/ajax.php', params, function(response){
            $('#container-turma').html( response );
        }, 'html' );
            
            
        return true;
    },
    
    turmaChangeHandler : function(){
        var nucid   = $('#nucid').val(), 
            turid   = $('#turid').val(), 
            ppuid   = $('#ppuid').val(), 
            params  = {};
        
        $('#container-diario').html();
        
        if( nucid == '' || turid == '' || ppuid == '' )
        {
            return false;
        }
        
        params['nucid'] = nucid;
        params['turid'] = turid;
        params['ppuid'] = ppuid;
        params['acao']  = 'listarComponenteCurricular';
        
        $.post( 'geral/ajax.php', params, function(response){
            $('#container-componente-curricular').html( response );
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
            console.log(params);
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
            $('.visualizarDiarioGrid').click( DiarioFrequencia.visualizarDiarioHandler );
            
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
    
  componenteChangeHandler:function(){
        
        var cocid = $('#cocid').val();
       
           $('#btnVisualizarDiario').attr('disabled','');
        if (cocid == 9999) 
             $('#btnVisualizarDiario').attr('disabled','disabled');
         
            
        
    }
};