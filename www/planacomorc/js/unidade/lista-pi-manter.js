
    /**
     * Ações efetuadas quando a tela de lista de PI é iniciada.
     * 
     * @returns VOID
     */
    function initListaPiManter(){
        
        $('[data-toggle="popover"]').popover({placement:'top', trigger:'hover'});
        
        $('.abrir-obra').click(function(){
            abrirObra($(this).attr('data-obrid'));
        });
        
        $('.abrir-ted').click(function(){
            abrirTED($(this).attr('data-tcpid'));
        });
        
        $('.btn-limpar').click(function(){
            window.document.location.href = '';
        });
        
        $('.btn-novo').click(function(){
            window.document.location.href = 'planacomorc.php?modulo=principal/unidade/cadastro_pi&acao=A';
        });
        
        $('.a_listar_delegadas').click(function(){
            var pliid = $(this).attr('data-pi');
            abrirPopupDelegadas(pliid);
        });

    }
    
    /**
     * Abre popup com lista de Sub-Unidades delegadas.
     * 
     * @param integer pliid
     * @returns VOID 
     */
    function abrirPopupDelegadas(pliid) {
       $('#detalhePiDelegadas .modal-body').empty();
       $.post("planacomorc.php?modulo=principal/unidade/listapimanter&acao=A&requisicao=detalharPiDelegadas&pliid=" + pliid, function(html) {
           $('#detalhePiDelegadas .modal-body').html(html);
           $('#detalhePiDelegadas').modal();
       });
    }

    function onFiltropiNovo()
    {
        window.location.assign('planacomorc.php?modulo=principal/unidade/cadastro_pi&acao=A');
    }

    function alterarPi(pliid)
    {
        window.location.assign('planacomorc.php?modulo=principal/unidade/cadastro_pi&acao=A&pliid=' + pliid);
    }

    function removerPi(pliid)
    {
        bootbox.confirm('Tem certeza que deseja apagar o PI?', function(confirmacao){
            if (confirmacao) {
                window.location.assign('planacomorc.php?modulo=principal/unidade/cadastro_pi&acao=A&apagar=true&pliid=' + pliid);
            }
        });
    }

    function detalhePI(pliid) {
       $('#detalhepi .modal-body').empty();
       $.post("planacomorc.php?modulo=principal/unidade/listapimanter&acao=A&requisicao=detalharPi&pliid=" + pliid, function(html) {
           $('#detalhepi .modal-body').html(html);
           $('#detalhepi').modal();
       });
    }

    /*
     * Abre o TED  em outra janela, no módulo de origem
     */
    function abrirTED(ted)
    {
        window.open('http://simec.mec.gov.br/ted/ted.php?modulo=principal/termoexecucaodescentralizada/previsao&acao=A&ted=' + ted);
    }

    /**
     * Abre a OBRA em outra janela, no módulo de origem
     */
    function abrirObra(obrid)
    {
        window.open('http://simec.mec.gov.br/obras/obras.php?modulo=principal/cadastro_pi&acao=A&obrid=' + obrid);
    }
