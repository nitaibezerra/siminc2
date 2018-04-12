
    /**
     * Ações efetuadas quando a tela de lista de PI é iniciada.
     * 
     * @returns VOID
     */
    function initListaEmendas(){
        
        $('.dataTablesSP').DataTable({
            bPaginate: false,
            responsive: true,
            dom: '<"html5buttons"B>lTfgitp',
            "language": {
                "url": "/zimec/public/temas/simec/js/plugins/dataTables/Portuguese-Brasil.json"
            }
        });

        $('.editar_situacao').click(function(){
            $('#proposta_modal .modal-body').load('emendas.php?modulo=principal/emenda&acao=A&req=proposta_modal&benid='+ $(this).data('benid'));
            $('#proposta_modal').modal();
        });
        
        $('.btn-salvar').click(function(){
            $('#formulario-selecionar-proposta').submit();
        });
        
        $('#btn_novo').click(function(){
            window.location.href = '?modulo=principal/emenda_form&acao=A';
        });
        
        $('.btn-limpar').click(function(){
            $('#requisicao').val('limpar');
            $('#filtropi').submit();
        });
        
    }

