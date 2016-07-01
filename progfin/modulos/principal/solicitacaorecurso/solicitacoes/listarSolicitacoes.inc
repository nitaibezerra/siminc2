<?php
/**
 * Listagem de solicitações.
 * $Id: listarSolicitacoes.inc 102315 2015-09-10 17:45:07Z maykelbraz $
 */
$where = array();
if (chaveTemValor($_REQUEST, 'unicod') && $_REQUEST['unicod'] != 'undefined') {
    $where[] = " uni.unicod = '{$_REQUEST['unicod']}'";
}
if (chaveTemValor($_REQUEST, 'ungcod') && $_REQUEST['ungcod'] != 'undefined') {
    $where[] = " ung.ungcod = '{$_REQUEST['ungcod']}'";
}

if (chaveTemValor($_REQUEST, 'trccod') && $_REQUEST['trccod'] != 'undefined') {
    $where[] = "lfn.stccod = '{$_REQUEST['trccod']}'";
}
if (chaveTemValor($_REQUEST, 'fdsid') && $_REQUEST['fdsid'] != 'undefined') {
    $where[] = "lfn.ftrcod = '{$_REQUEST['fdsid']}'";
}
if (chaveTemValor($_REQUEST, 'ctgcod') && $_REQUEST['ctgcod'] != 'undefined') {
    $where[] = "lfn.ctgcod = '{$_REQUEST['ctgcod']}'";
}
if (chaveTemValor($_REQUEST, 'clpid') && $_REQUEST['clpid'] != 'undefined') {
    $where[] = "lfn.clpid IN({$_REQUEST['clpid']})";
}
if (isset($_REQUEST['tpreg'])) {
    if ('I' == $_REQUEST['tpreg']) {
        $where[] = 'lfn.llfid IS NULL';
    } elseif ('L' == $_REQUEST['tpreg']) {
        $where[] = 'lfn.llfid IS NOT NULL';
    }
}
if (isset($_REQUEST['esdid'])) {
    $where[] = "esd.esdid IN({$_REQUEST['esdid']})";
}

if (!empty($whereAdicionalUO)) {
    $where[] = 'uni.' . $whereAdicionalUO[0];
}

if (!empty($where)) {
    $where = '  WHERE ' . implode(' AND ', $where);
} else {
    $where = '';
}

$query = <<<DML
    SELECT
        DISTINCT plf.plfid                                 ,
        lpad(plf.plfid::text, 7,'0') AS numpedido ,
        to_char(dataultimaatualizacao,'dd/mm/yyyy') as datapedido,
        uni.unicod || ' - ' || uni.unidsc AS unidade  ,
        --ung.ungcod || ' - ' || ung.ungdsc AS ug  ,
        COUNT( 0 )                        AS liberacoes,
        SUM(lfnvalorsolicitado) AS solicitado,
        SUM(lfnvalorautorizado) AS autorizado,
        SUM(lfnvaloratendido) AS atendido,
        esd.esddsc                         AS situacao,
        esd.esdid
    FROM progfin.pedidoliberacaofinanceira plf
    JOIN public.unidade uni ON uni.unicod = plf.unicod
    JOIN public.unidadegestora ung ON ung.unicod = uni.unicod
    LEFT JOIN progfin.liberacoesfinanceiras lfn ON lfn.plfid = plf.plfid and lfn.ungcodfavorecida = ung.ungcod
    JOIN workflow.documento doc ON plf.docid = doc.docid
    LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
    {$where}
    AND lfnvalorsolicitado is not null
    GROUP BY 1,2,3,4,9,10
    ORDER BY 1 DESC
DML;
# ver($query) ;

/* Altera o SQL para a tabela Detalhada*/
if ($_REQUEST['tipotabela'] == 'detalhada') {
    $enviado_sucesso = ESDID_LIBERACAO_ENVIO_SUCESSO;
    $query = <<<DML
        SELECT DISTINCT
            plf.plfid                                                        ,
            lpad( plf.plfid::text , 7 , '0' )               AS numpedido     ,
            TO_CHAR( dataultimaatualizacao , 'dd/mm/yyyy' ) AS datapedido    ,
            uni.unicod || ' - ' || uni.unidsc               AS unidade       ,
            ung.ungcod || ' - ' || ung.ungdsc               AS ug            ,
            clp.clpdsc                                      AS classificacao ,
            COALESCE( lfn.acacod , acacod_2 )               AS acao          ,
            lfn.stccod                                      AS sitcontabil   ,
            lfn.ftrcod                                      AS fonte         ,
            lfn.vincod                                      AS vinculacao    ,
            SUM( lfnvalorsolicitado )                       AS solicitado    ,
            SUM( lfnvalorautorizado )                       AS autorizado    ,
            SUM( lfnvaloratendido )                         AS atendido      ,
            esd.esddsc                                      AS situacao      ,
            numdocsiafi                                     AS pf            ,
            (
            SELECT
               TO_CHAR(htddata, 'dd/mm/yyyy' )
            FROM
               workflow.historicodocumento wf
               inner join workflow.documento wd using (docid)
            WHERE
               docid = lfn.docid
               AND wd.esdid = {$enviado_sucesso}
            ORDER BY
               htddata DESC limit 1 ) as datasituacao
        FROM progfin.pedidoliberacaofinanceira plf
        JOIN public.unidade uni ON uni.unicod = plf.unicod
        JOIN public.unidadegestora ung ON ung.unicod = uni.unicod
        LEFT JOIN progfin.liberacoesfinanceiras lfn ON lfn.plfid = plf.plfid AND lfn.ungcodfavorecida = ung.ungcod
        LEFT JOIN progfin.classificacaopedido clp USING (clpid)
        JOIN workflow.documento doc ON lfn.docid = doc.docid
        LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
        {$where}
            AND lfnvalorsolicitado IS NOT NULL
        GROUP BY
            1          ,
            2          ,
            3          ,
            4          ,
            5          ,
            6          ,
            7          ,
            8          ,
            9          ,
            10         ,
            numdocsiafi ,
            esd.esddsc ,
            lfn.docid
        ORDER BY 1 DESC
DML;
# ver($query) ;
}
?>
<link rel="stylesheet" href="/library/bootstrap-toggle/css/bootstrap-toggle.min.css">
<script src="/library/bootstrap-toggle/js/bootstrap-toggle.min.js"></script>
<style type="text/css">
    label.btn.active{background-color:#e6e6e6!important;border-color:#adadad!important;color:#333!important}
    .acertos_uo{ color: red;}
</style>
<script type="text/javascript">
    function abrirPedido(id)
    {
        if (id) {
            location.href = "/progfin/progfin.php?modulo=principal/solicitacaorecurso/novasolicitacao&acao=A&id=" + id;
        }
    }
    function deletarPedido(id)
    {
        if (id) {
            bootbox.confirm('Você realmente deseja excluir este pedido?', function(res){
                if(res){
                    $.post(location.href, {requisicao: 'deletaPedido', id: id}, function(retorno){
                        if(retorno){
                            bootbox.alert('Pedido excluído com sucesso!',function(){
                                location.href = location.href;
                            });
                        }else{
                            bootbox.alert('Falha ao excluir pedido.');
                        }
                    });
                }
            });

        }
    }
</script>
<?php
$listagem = new Simec_Listagem();
$cabecalho = array(
    'Pedido',
    'Data',
    'Unidade Orçamentária (UO)',
    'Liberações',
    'Liberação de Recursos (R$)' => array('Solicitado', 'Aprovado', 'Atendido'),
    'Situação'
);
#ver($_REQUEST);
/* Altera o Cabeçalho para a tabela Detalhada*/
if ($_REQUEST['tipotabela'] == 'detalhada') {
    $cabecalho = array(
        'Pedido',
        'Data',
        'Unidade Orçamentária (UO)',
        'Unidade Gestora (UG)',
        'Classificação do Pedido',
        'Ação Orçamentária',
        'Situação Contábil',
        'Fonte',
        'Vinculação',
        'Liberação de Recursos (R$)' => array('Solicitado', 'Aprovado', 'Atendido'),
        'Situação',
        'PF',
        'Data da Liberação'
    );
}

$listagem->setCabecalho($cabecalho)
    ->addCallbackDeCampo(array('solicitado', 'autorizado', 'atendido', 'diferenca'), 'mascaraMoeda')
    ->addCallbackDeCampo('numpedido', 'formatarPedido')
    ->addCallbackDeCampo(array('unidade', 'ug'), 'alinhaParaEsquerda')
    ->addCallbackDeCampo(array('unidade', 'ug'), 'alinhaParaEsquerda')
    ->addRegraDeLinha(
        array('campo' => 'situacao', 'op' => 'igual', 'valor' => 'Ajustes UO', 'classe' => 'acertos_uo')
    )->addAcao('view', 'abrirPedido');
if ($_REQUEST['tipotabela'] != 'detalhada') {
    $listagem->addAcao('delete', 'deletarPedido')
    ->setAcaoComoCondicional('delete', array(array('campo' => 'esdid', 'valor' => array('1400','1403'), 'op' => 'contido')));
}
$listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('solicitado', 'autorizado', 'atendido', 'diferenca'))
    ->turnOnPesquisator()
    ->esconderColunas(array('llfid', 'esdid', 'lfnmensagem'))
    ->setQuery($query)
    ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
