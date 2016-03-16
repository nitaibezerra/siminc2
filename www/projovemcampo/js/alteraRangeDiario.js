var alteraRangeDiario    = {
    
    init : function(){
        $('#entid').change( alteraRangeDiario.escolaChangeHandler );
        $('#btnAlteraRange').click( alteraRangeDiario.alteraRangeHandler );
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
            $('#turid').change( alteraRangeDiario.turmaChangeHandler );
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
        params['gerarDiario'] = '2';
        params['acao']  = 'listarPeriodos';
        
        $.post( 'geral/ajax.php', params, function(response){
            $('#container-periodo').html( response );
        }, 'html' );
            
        return true;
    },
       
    alteraRangeHandler : function(){
    	
    	var confirma = confirm("Você esta alterando a data de início das aulas dessa turma. Deseja continuar?");
    	
    	if(confirma == false){
    		return false;
    	}
    	$('#btnAlteraRange').attr('disabled', true);
    	
    	if( $('#frmalteraRangeDiario').valid() == false )
    	{
    		return false;
    	}
    	
    	var params = $('#frmalteraRangeDiario').serialize();
    	
    	params += '&acao=alteraRange';
    	
    	$.post( 'geral/ajax.php', params, function(response){
    		$('#container-diario').html( response );
    	}, 'html' );
    	
    	return true;
    },
};