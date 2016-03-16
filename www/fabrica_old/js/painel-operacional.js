var PainelOperacionalView = {
    
    init : function()
    {
        $('.estadoSolicitacaoServico').click( PainelOperacionalView.listarSolicitacaoServicoClickHandler );
        $('.estadoOrdemServico').click( PainelOperacionalView.listarOrdemServicoClickHandler );
        $('.generica').live( 'click',  PainelOperacionalView.abrirOSEmpresaItem1 );
        
        if( $('#recarregarAction').val() != '' && $('#recarregarSituacao').val() != '' )
        {
            //alert( $('#recarregar').val() );
            var id        = $('#recarregarSituacao').val();
            var strJquery = '';
            
            if( $('#recarregarAction').val() == 'listarSolicitacaoServico' ){

                strJquery = '.estadoSolicitacaoServico[id='+ id +']';
                $( strJquery ).trigger('click');
                
            }else if( $('#recarregarAction').val() == 'listarOrdemServico' ){

                strJquery = '.estadoOrdemServico[id='+ id +']';
                $( strJquery ).trigger('click');
            }
        }
        
        jQuery.ajaxSetup({
            beforeSend: function(){
                $("#dialogAjax").show();
            },
            complete: function(){
                $("#dialogAjax").hide();
            }
        });
    },
    
    
    listarSolicitacaoServicoClickHandler : function()
    {
        var el      = this,
            esdId   = $(this).attr('id'),
            params  = {
                'esdid': esdId,
                'action': 'listarSolicitacaoServico',
                'ordemlista' : $("#ordemlista").val()
            };
            
       $('#container-listagem').html('');
       $('#nomeListagem').html( ''  );
       $.get( 'ajax_painel_operacional.php', params, function(response){
           //$('#nomeListagem').html( response.nomeEstado  );
           //$('#container-listagem').html(response.listagem);
           $('#container-listagem').html( response );
           
           var inputEsdid  = $('<input>', { type : 'hidden', name : 'esdid',  value : params.esdid } );
           var inputAction = $('<input>', { type : 'hidden', name : 'action', value : params.action } );
           
           $('form[name="formlista"]').append( inputEsdid );
           $('form[name="formlista"]').append( inputAction );

       }, 'html');
            
    },
    
    listarOrdemServicoClickHandler : function()
    {
        var el          = this,
            esdId       = $(this).attr('id'),
            params      = {},
            arrDados    = [],
            tosId       = '';
            
        if( esdId.indexOf("_") != -1 );
        {
            arrDados    = esdId.split('_');
            esdId       = arrDados[0];
            tosId       = arrDados[1];
        }
        
        params = {
            'esdid' : esdId,
            'tosid' : tosId,
            'action' : 'listarOrdemServico',
            'ordemlista' : $("#ordemlista").val()
        }
        
        $('#container-listagem').html('');
        $('#nomeListagem').html( ''  );
        //$.get( 'fabrica.php?modulo=principal/painelOperacional&acao=A', params, function(response){
        $.get( 'ajax_painel_operacional.php', params, function(response){
           //$('#nomeListagem').html( response.nomeEstado  );
           //$('#container-listagem').html( response.listagem  );
           $('#container-listagem').html( response );
           
           var inputEsdid  = $('<input>', { type : 'hidden', name : 'esdid',  value : params.esdid } );
           var inputAction = $('<input>', { type : 'hidden', name : 'action', value : params.action } );
           
           $('form[name="formlista"]').append( inputEsdid );
           $('form[name="formlista"]').append( inputAction );
       }, 'html');
    }, 
    
    
    abrirOSEmpresaItem1 : function()
    {
        var odsId = $(this).attr('id');
        window.open('fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid='+ odsId, 'Observações', 'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
    },
    
    ordenaListagem : function()
    {
        //$('.estadoSolicitacaoServico').click( PainelOperacionalView.listarSolicitacaoServicoClickHandler );
        //$('.estadoOrdemServico').click( PainelOperacionalView.listarOrdemServicoClickHandler );
        //$('.generica').live( 'click',  PainelOperacionalView.abrirOSEmpresaItem1 );
    }
};