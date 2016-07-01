<?php
/**
 * Listagem de pedidos de alteração orçamentaria
 * $Id: listarPedidos.inc 102362 2015-09-11 19:01:00Z maykelbraz $
 */

$listagem = new Simec_Listagem();
$listagem->setCabecalho(
    array(
        'Descrição',
        'Momento',
        'Tipo de crédito',
        'Código do SIOP',
        'UO',
        'Créditos (R$)' => array('Suplementação', 'Cancelamento'),
        'Diferença',
        'Validado no SIOP',
        'Status pedido'
    )
);

$listagem->addCallbackDeCampo(array('crd_suplementacao', 'crd_cancelamento', 'diferenca'), 'mascaraMoeda')
    ->addCallbackDeCampo('paostatus', 'statusNoSIOP');
$listagem->addAcao('edit', 'abrirPedido');

if ((array_intersect(array(PFL_SUPER_USUARIO, PFL_CGO_EQUIPE_ORCAMENTARIA), $perfis))
    && ($mcridAtual == $mcrid)) {
    $listagem->addAcao('check', array('func' => 'enviarPedido', 'titulo' => 'Validar no SIOP'));

    // -- Não exibe ação de DELETE qdo um momento for de cancelamento
    if (('t' !== $mcrtipocancelamento)) {
        $listagem->addAcao('delete', array('func' => 'apagarPedido', 'extra-params' => array('paodsc', 'siopid')));
    }
}

if (in_array(PFL_UO_EQUIPE_TECNICA, $perfis) ) {
    $listagem->addAcao('check', array('func' => 'enviarPedido', 'titulo' => 'Validar no SIOP'));
    $listagem->setAcaoComoCondicional(
        'delete',
        array(
            array('campo' => 'esdid', 'valor' => STDOC_ANALISE_SPO, 'op' => 'diferente'),
            array('campo' => 'esdid', 'valor' => STDOC_CADASTRAR_SIOP, 'op' => 'diferente')
        )
    );
}

$listagem->setTotalizador(
    Simec_Listagem::TOTAL_SOMATORIO_COLUNA,
    array('crd_suplementacao', 'crd_cancelamento', 'diferenca')
);

// -- Filtros da query
$params = array($_SESSION['exercicio'], $_SESSION['exercicio']);

$whereFiltros = array();
if (in_array(PFL_UO_EQUIPE_TECNICA, $perfis)) {
    $whereFiltros[] = ' uni.unicod IN(' . pegaResposabilidade($_SESSION['usucpf'], PFL_UO_EQUIPE_TECNICA, 'unicod') . ')';
}

if (isset($_REQUEST['dados'])) {
    unset($_SESSION['altorc']['filtro_params']);
    $_SESSION['altorc']['filtro_params'] = $_REQUEST['dados'];

    if (chaveTemValor($_REQUEST['dados'], 'mcrid')) {
        $whereFiltros[] = 'pao.mcrid = %d';
        $params[] = $_REQUEST['dados']['mcrid'];
    }
    if (chaveTemValor($_REQUEST['dados'], 'tcrid')) {
        $whereFiltros[] = 'pao.tcrid = %d';
        $params[] = $_REQUEST['dados']['tcrid'];
    }
    if (chaveTemValor($_REQUEST['dados'], 'unicod')) {
        $whereFiltros[] = "pao.unicod = '%s'";
        $params[] = $_REQUEST['dados']['unicod'];
    }
    if (chaveTemValor($_REQUEST['dados'], 'tipid')) {
        $whereFiltros[] = "pao.tipid = '%s'";
        $params[] = $_REQUEST['dados']['tipid'];
    }

    if (chaveTemValor($_REQUEST['dados'], 'wrfstatus') && $_REQUEST['dados']['wrfstatus'] != 'todos' ) {
        $whereFiltros[] = "esd.esdid = '%s'";
        $params[] = $_REQUEST['dados']['wrfstatus'];
    }
      if (chaveTemValor($_REQUEST['dados'], 'retornosiop') && $_REQUEST['dados']['retornosiop'] != 'todos' ) {
        $whereFiltros[] = "rqp.rqpsucesso = '%s'";
        if($_REQUEST['dados']['retornosiop'] == 'ERRO'){
            $retornoSiop = 'f';
        }else{
            $retornoSiop = 't';
        }
        $params[] = $retornoSiop;
    }
}

if (!empty($whereFiltros)) {
    $whereFiltros = 'AND ' . implode(' AND ', $whereFiltros);
} else if(!empty($_SESSION['altorc']['filtro_pedido']['whereFiltros'])){
    $whereFiltros = $_SESSION['altorc']['filtro_pedido']['whereFiltros'];
    $params = $_SESSION['altorc']['filtro_pedido']['params'];
} else {
    $whereFiltros = '';
}

$sql = <<<DML
    SELECT pao.paoid,
        pao.paoid AS paoid2,
        pao.paodsc,
        mcr.mcrdsc,
        tcr.tcrcod,
        COALESCE(pao.siopid::varchar, '-') AS siopid,
        uni.unicod || ' - ' || uni.unidsc AS unidsc,
        SUM(pas.valsuplementacao) AS crd_suplementacao,
        SUM(pas.valcancelamento) AS crd_cancelamento,
        COALESCE(SUM(pas.valsuplementacao), 0.00) - COALESCE(SUM(pas.valcancelamento), 0.00) AS diferenca,
        rqp.rqpsucesso AS paostatus,
        COALESCE(esd.esddsc, 'Em preenchimento UO') AS pedidostatus,
        esd.esdid
    FROM altorc.pedidoalteracaoorcamentaria pao
    INNER JOIN altorc.tipocredito tcr ON(pao.tcrid = tcr.tcrid AND tcr.tcrano = '%d')
    INNER JOIN public.unidade uni USING(unicod)
    LEFT JOIN altorc.pedidoalteracaofinanceiro pas USING(paoid)
    LEFT JOIN altorc.requisicoespedido rqp USING(rqpid)
    LEFT JOIN workflow.documento doc USING(docid)
    LEFT JOIN workflow.estadodocumento esd USING(esdid)
    LEFT JOIN altorc.momentocredito mcr USING(mcrid)
    WHERE pao.paostatus = 'A'
        AND pao.paoano = '%d'
        {$whereFiltros}
    GROUP BY
        pao.paoid,
        pao.paodsc,
        tcr.tcrcod,
        uni.unicod,
        uni.unidsc,
        rqp.rqpsucesso,
        esd.esddsc,
        mcr.mcrdsc,
        pao.siopid,
        esd.esdid
DML;
$stmt = vsprintf($sql, $params);
$listagem->setQuery($stmt);
$listagem->esconderColunas(array('paoid2', 'esdid'));
?>
<script type="text/javascript">
    $(document).ready(function() {
        $('#modal-alert .modal-dialog').css('width', '80%');
    });
    function abrirPedido(paoid)
    {
        window.location = 'altorc.php?modulo=principal/pedido/inicio&acao=A&dados[paoid]=' + paoid;
    }
    function enviarPedido(paoid)
    {
        window.location = 'altorc.php?modulo=principal/pedido&acao=A&requisicao=enviarPedido&dados[paoid]=' + paoid;
    }
    function apagarPedido(paoid, desc, siopid)
    {
        var msg = "Tem certeza que deseja apagar o pedido '" + desc + "'?";
        if ('-' !== siopid) {
            msg += ' Ele também será apagado no SIOP.';
        }
        $('#modal-confirm .modal-body').text(msg);
        $('#modal-confirm .btn-primary').click(function(){
            window.location = 'altorc.php?modulo=principal/pedido&acao=A&requisicao=apagarPedido&dados[paoid]=' + paoid;
        });

        $('#modal-confirm').modal();
    }
    function detalharRetornoSIOP(paoid)
    {
        $.post('altorc.php?modulo=principal/pedido&acao=A', {'dados[paoid]':paoid,'requisicao':'retornoSIOP'}, function(data){
            $('#modal-alert .modal-title').text('Retorno SIOP');
            $('#modal-alert .modal-body').html(data);
            $('#modal-alert').modal();
        });
    }
</script>
<?php
    $fm = new Simec_Helper_FlashMessage("altorc/pedido");
    echo $fm->getMensagens();

    // -- Saída do relatório
    $listagem->turnOnPesquisator();
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

    unset($_SESSION['altorc']['filtro_pedido']);
    $_SESSION['altorc']['filtro_pedido']['whereFiltros'] = $whereFiltros;
    $_SESSION['altorc']['filtro_pedido']['params'] = $params;
