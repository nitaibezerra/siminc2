
    /**
     * Ações efetuadas quando a tela de lista de Proposta é iniciada.
     *
     */
    function initListaProposta(){

        $('.btn-limpar').click(function(){
            $('#requisicao').val('limpar');
            $('#filtroprop').submit();
        });

        $('.btn-novo').click(function(){
            window.document.location.href = '?modulo=principal/proposta_form&acao=A';
        });

    }