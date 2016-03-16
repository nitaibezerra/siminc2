var Inventario = 
{
		COMPLEXIDADE : { 'B' : 'Baixa', 'M' : 'Média', 'A' : 'Alta'},
    
		calcularComplexidadeHandler    : function() {
        var tipoFuncionalidade  = $('#co_tipo_funcionalidade').val(),
            qtdTRAR             = $('input[name="qtdalrrlr"]').val(),
            qtdTD               = $('input[name="qtdtd"]').val(),
            complexidade        = '',
            qtdPF               = 0;
            
        	$('input[name="tipo_complexidade"]').val( '' );
        	$('#tp_complexidade').val( '' );
        	$('input[name="qtd_pf"]').val( '' );
        
        	if(tipoFuncionalidade == '' || qtdTRAR == '' || qtdTD == '' || qtdTRAR == 0 || qtdTD == 0 ){
        		return false;
        	}
        
        	complexidade    = Inventario.calcularComplexidade(tipoFuncionalidade, qtdTRAR, qtdTD);
        	qtdPF           = Inventario.calcularQuantidadePF(tipoFuncionalidade, complexidade );
        
        	$('input[name="tipo_complexidade"]').val( Inventario.COMPLEXIDADE[complexidade] );
        	$('#tp_complexidade').val( complexidade );
        	$('input[name="qtd_pf"]').val( mascaraglobal( '[#].###,##', qtdPF ) );
        	return true;
		},
    
    calcularComplexidade    : function( tipoFuncionalidade, qtdTRAR, qtdTD ) {
    	var tipoFuncao = TipoFuncao.retornaTipoCalculo( tipoFuncionalidade );
    	return tipoFuncao.calcularComplexidade( qtdTRAR, qtdTD );
    },
    
    calcularQuantidadePF    : function( tipoFuncionalidade, complexidade ) {
    	var tipoFuncao = TipoFuncao.retornaTipoCalculo( tipoFuncionalidade );
    	return tipoFuncao.calcularQuantidadePF( complexidade );
    },
    
    	validarFormulario	: function(){
    		var erros   = 0,
    		qtdTD       = $('input[name="qtdtd"]').val(),
    		qtdALRRLR   = $('input[name="qtdalrrlr"]').val();
         
        
         if( !$('#form-inserirInventario').valid() ){
             erros++;
         }
         
         if( erros > 0 ){
             alert('Favor preencher todos os campos obrigatórios');
             return false;
         }
         
         if( jQuery.trim($('#ds_alr_rlr').val()) == '' ){
             alert('Campo Descrição de ALR/RLR é de preenchimento obrigatório.');
             return false;
         }
         
         if( jQuery.trim($('#ds_td').val()) == '' ){
             alert('Campo Descrição de TD é de preenchimento obrigatório.');
             return false;
         }
         
         if(qtdTD == 0 || qtdALRRLR == 0 ){
             alert('A quantide de TD/ALR/RLR não pode ser igual a zero');
             return false;
         }
         
         return true;
    },
    
    salvarFuncionalidadeHandler   : function(){
       if( !Inventario.validarFormulario() )
        {
            return false;
        }
        
        var params = $('#form-inserirInventario').serialize(true);
      
         $.ajax({
            beforeSend: function(){
                $("#dialogAjax").show();
            },
            type: 'post',
            url:'geral/inventario/salvar.php',
            dataType: 'json',
            data: params,
            success: function(response){
                if( response.status == true )
                {
                    alert('Funcionalidade cadastrada com sucesso.');
                    if  (!confirm("Deseja manter os dados preenchidos?")){
                    	window.location = 'fabrica.php?modulo=sistema/geral/inventario/inserirFuncionalidade&acao=A&co_inventario='+ response.dados['co_inventario'];	
                    }
                }else{
                    alert( response.msg );
                }
            },
            complete: function(){
                $("#dialogAjax").hide();
            }
        });
     
        return true;
    },
    
    salvarInventarioHandler   : function(){
        if( !Inventario.validarFormulario() )
         {
             return false;
         }
         
         var params = $('#form-inserirInventario').serialize(true);
       
          $.ajax({
             beforeSend: function(){
                 $("#dialogAjax").show();
             },
             type: 'post',
             url:'geral/inventario/salvar.php',
             dataType: 'json',
             data: params,
             success: function(response){
                 if( response.status == true )
                 {
                     alert('Inventário cadastrado com sucesso.');
                     if( Inventario.validarFormulario() ){
                     	window.location = 'fabrica.php?modulo=sistema/geral/inventario/alterarInventario&acao=A&co_inventario='+ response.dados['co_inventario'];	
                     }
                 }else{
                     alert( response.msg );
                 }
             },
             complete: function(){
                 $("#dialogAjax").hide();
             }
         });
      
         return true;
     },
    
    alterarFuncionalidadeHandler   : function(){
        if( !Inventario.validarFormulario() )
        {
            return false;
        }
            
         
         var params = $('#form-inserirInventario').serialize(true);
       
          $.ajax({
             beforeSend: function(){
                 $("#dialogAjax").show();
             },
             type: 'post',
             url:'geral/inventario/salvar.php',
             dataType: 'json',
             data: params,
             success: function(response){
                 if( response.status == true )
                 {
                     alert('Inventário alterado com sucesso.');
                     window.location = 'fabrica.php?modulo=sistema/geral/inventario/alterarInventario&acao=A&co_inventario='+ response.dados['co_inventario'];
                 }else{
                     alert( response.msg );
                 }
             },
             complete: function(){
                 $("#dialogAjax").hide();
             }
         });
      
         return true;
     },
    
    iniciarModal    : function(){
        $('#modalDescricaoALRRLR').dialog({
            autoOpen    : false,
            dialogClass : 'modalFabrica',
            modal       : true,
            title       : 'Descrição ALR/RLR',
            width       : 300,
            resizable   : false,
            draggable   : true,
            buttons     : {
                "Fechar"    : function(){
                    $( '#ds_alr_rlr' ).val( $('#descricao_alr_rlr').val() );
                    $( this ).dialog("close");
                }
            }
        });
        
        $('#modalDescricaoTD').dialog({
            autoOpen    : false,
            dialogClass : 'modalFabrica',
            modal       : true,
            title       : 'Descrição TD',
            width       : 300,
            resizable   : false,
            draggable   : true,
            buttons     : {
                "Fechar"    : function(){
                    $( '#ds_td' ).val( $('#descricao_td').val() );
                    $(this).dialog("close");
                }
            }
        });
    },
     
	listarAgrupadores : function(){
		 
		$.ajax({
            beforeSend: function(){
                $("#dialogAjax").show();
            },
            type: 'post',
            url:'geral/inventario/listar.php',
            dataType: 'json',
            data: { sidid : $('#sidid').val() },
            success: function(response){
                
            },
            complete: function(){
                $("#dialogAjax").hide();
            }
        });	 
	}
};

