/*
* Faz requisição via ajax
* Filtra o tipo de damanda, atravéz do parametro passado 'celid'
*/
function filtraSistema(celid, sidid) {
	
        if(!celid) celid = 999999;
        if(!sidid) sidid = 999999;
        
        $.ajax({
                type: "POST",
                url: "../fabrica/ajax_painel_operacional.php",
                data: "&celidAjax="+celid+"&sididAjax="+sidid,
                success: function(res){
                        $('td#listasistema').html(res);    
                }
        });
}

var PainelOperacionalGerenteProjetoView = {
    
    init : function()
    {
        
        $('#buttonAtualizar').click( PainelOperacionalGerenteProjetoView.listarPainelCelulaSistemaClickHandler );
        $('.painelGerenteProjetosSS').click( PainelOperacionalGerenteProjetoView.painelGerenteProjetosSSClickHandler );
       
       if( $('#recarregarAction').val() != '' && $('#recarregarSituacao').val() != '' )
        {
            var id        = $('#recarregarSituacao').val();
            var strJquery = '';
            if( $('#recarregarAction').val() == 'painelgProjetos' ){
                strJquery = '.painelGerenteProjetosSS[id='+ id +']';
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
    
    listarPainelCelulaSistemaClickHandler : function()
    {
            
            var celid   = $('#celid').val(),
                sidid   = $('#sidid').val(),
            
            params  = {
                'celid': celid,
                'sidid': sidid,
                'listarPainelCelulaSistema': 'listarPainelCelulaSistema'
            };
            
            $('#container-painel').html('');
            $('#container-listagem').html('');

            $.get( 'ajax_painel_operacional.php', params, function(response){
                $('#container-painel').html( response );
            }, 'html');
            
           //$('.realizarPreAnalise').click( PainelOperacionalGerenteProjetoView.painelGerenteProjetosSSClickHandler );
    },
    
    painelGerenteProjetosSSClickHandler : function()
    {
        
        var esdId   = $(this).attr('id'),
            celid   = $('#celid').val(),
            sidid   = $('#sidid').val(),
            
            params  = {
                'esdid': esdId,
                'celid': celid,
                'sidid': sidid, 
                'action': 'painelgProjetos',
                'ordemlista' : $("#ordemlista").val()
            };
            
       $.get( 'ajax_painel_operacional.php', params, function(response){
           
           $('#container-listagem').html( response );
           
           var inputEsdid  = $('<input>', { type : 'hidden', name : 'esdid',  value : params.esdid } );
           var inputAction = $('<input>', { type : 'hidden', name : 'action', value : params.action } );
           var inputCelid  = $('<input>', { type : 'hidden', name : 'celid',  value : params.celid } );
           var inputSidid  = $('<input>', { type : 'hidden', name : 'sididForm',  value : params.sidid } );
           
           $('form[name="formlista"]').append( inputEsdid );
           $('form[name="formlista"]').append( inputAction );
           $('form[name="formlista"]').append( inputCelid );
           $('form[name="formlista"]').append( inputSidid );
           
           $("input#replanejar").click(function() {
                if($('input[type="radio"]:checked').is(":checked")){
                    var odsid = $('input[type="radio"]:checked').val();
                    window.open('?modulo=principal/popup/alterarPrevisaoTermino&acao=A&odsid='+odsid+'&alteracao=OSitem2', 'alterarPrevisaoTermino', 'top=350, left=100, align=center,toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=850,height=320');
                }else{
                    alert("Atenção: para replanejar é necessário selecionar uma Ordem de Serviço.")
                }
             });
             
             
             $("input#pausar").click(function() {
                if($('input[type="radio"]:checked').is(":checked")){
                    var odsid = $('input[type="radio"]:checked').val();
                    window.open('?modulo=principal/popup/pausarServico&acao=A&odsid='+odsid+'&alteracao=OSitem2', 'pausarServico', 'top=350, left=100, align=center,toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=850,height=320');
                }else{
                    alert("Atenção: para pausar é necessário selecionar uma Ordem de Serviço.")
                }
             });
             
             $("input#pausar_ss").click(function() {
                 if($('input[type="radio"]:checked').is(":checked")){
                     var scsid = $('input[type="radio"]:checked').val();
                     window.open('?modulo=principal/popup/pausarServico&acao=A&scsid='+scsid+'&alteracao=OSitem2', 'pausarServico', 'top=350, left=100, align=center,toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=850,height=320');
                 }else{
                     alert("Atenção: para pausar é necessário selecionar uma Solicitação de Serviço.")
                 }
              });

       }, 'html');
           
    },
   
    abrirOSEmpresaItem1 : function()
    {
        var odsId = $(this).attr('id');
        window.open('fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid='+ odsId, 'Observações', 'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
    },
    
    ordenaListagem:  function()
    {
        //$('.estadoSolicitacaoServico').click( PainelOperacionalView.listarSolicitacaoServicoClickHandler );
        //$('.estadoOrdemServico').click( PainelOperacionalView.listarOrdemServicoClickHandler );
        //$('.generica').live( 'click',  PainelOperacionalView.abrirOSEmpresaItem1 );
    },
    
    recarregarPagina: function(){
        var celid   = $('#celid').val(),
            sidid   = $('#recarregarSidid').val();
            
        filtraSistema( celid, sidid );
        this.listarPainelCelulaSistemaClickHandler();
    }    
};

