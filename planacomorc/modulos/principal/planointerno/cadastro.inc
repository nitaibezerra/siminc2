<?php
/**
 * Formulário de cadastro de Planos Internos.
 * @version $Id: cadastro.inc 102359 2015-09-11 18:26:07Z maykelbraz $
 */

if ($_REQUEST['listaptres'] == 'ok') {
    echo popupPtres($_REQUEST['pliid'], $_REQUEST['sbaid'], $_REQUEST['tipo']);
    die();
}

if ($_REQUEST['detalhe']) {
    if ($_REQUEST['detalhe'] == 'ptres') {
        echo popupDetalhePtres($_REQUEST['ptrid']);
    } elseif ($_REQUEST['detalhe'] == 'unidade') {
        echo popupDetalheUnidade($_REQUEST['ptrid']);
    }
    die();
}
include_once '_funcoespi.php';

$fm = new Simec_Helper_FlashMessage('planacomorc/ptr/cadadstroalteracaopi');

function popupPtres($pliid, $sbaid, $tipo) {
    global $db;

    // verificando se é undidade ou unidade gestora
    $where .= $_REQUEST['sbaid'] ? "AND dt.ptrid IN (SELECT ptrid FROM monitora.pi_subacaodotacao WHERE sbaid = " . $_REQUEST['sbaid'] . ")" : '';
    $where .= $_POST['prgcod'] ? "AND UPPER(dtl.prgcod) LIKE('%" . strtoupper($_POST['prgcod']) . "%')" : '';
    $where .= $_POST['acacod'] ? "AND UPPER(dtl.acacod) LIKE('%" . strtoupper($_POST['acacod']) . "%')" : '';
    $where .= $_POST['buscalivre'] ? "AND (trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) ilike('%" . $_POST['buscalivre'] . "%') OR dtl.ptres ilike '%" . $_POST['buscalivre'] . "%')" : '';

    // -- Query utilizada também em: simec/www/planacomorc/_funcoespi.php

    /* Parametros para montar a consulta */
    $params['where'] = $where;

    $params['obrigatorio'] = 'S';
    if ($_REQUEST['tipo'] == 'pi') {
        $params['SELECT'] = <<<SQL
            SELECT
                dtl.ptrid as acao,
                dtl.ptres as ptres,trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acatitulo) AS descricao,
                uni.unidsc,
                COALESCE(SUM(dtl.ptrdotacao), 0.00) AS dotacaoinicial,
                COALESCE(SUM(dt.valor), 0.00) AS det_subacao,
                -- dotacaoinicial - det_subacao
                (COALESCE(SUM(dtl.ptrdotacao), 0.00) -
                    COALESCE(SUM(dt.valor), 0.00)) AS nao_det_subacao,
                --COALESCE(SUM(dt2.valorpi), 0.00) AS det_pi,
                -- det_subacao - det_pi
                --(COALESCE(SUM(dt.valor), 0.00) - COALESCE(SUM(dt2.valorpi), 0.00)) AS nao_det_pi,
                COALESCE((pemp.total), 0.00) AS empenhado,
                COALESCE(SUM(dtl.ptrdotacao), 0.00) - COALESCE(pemp.total, 0.00) AS nao_empenhado
SQL;
    } else {
        $params['SELECT'] = <<<SQL
            SELECT
                dtl.ptrid as acao,'<div class="make-switch switch-mini" data-on-label="S" data-off-label="N" data-off="danger"><input type="checkbox" id="chk_'||dtl.ptres||'" onclick="resultado(1,this,'''||dtl.ptrid||''','''||dtl.ptres||''')"></div>' AS checkbox,
                dtl.ptres as ptres,trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acatitulo) AS descricao,
                uni.unidsc,
                COALESCE(SUM(dtl.ptrdotacao), 0.00) AS dotacaoinicial,
                COALESCE(SUM(dt.valor), 0.00) AS det_subacao,
                -- dotacaoinicial - det_subacao
                (COALESCE(SUM(dtl.ptrdotacao), 0.00) -
                    COALESCE(SUM(dt.valor), 0.00)) AS nao_det_subacao,
                COALESCE(SUM(dt2.valorpi), 0.00) AS det_pi,
                -- det_subacao - det_pi
                (COALESCE(SUM(dt.valor), 0.00) - COALESCE(SUM(dt2.valorpi), 0.00)) AS nao_det_pi,ve
                COALESCE((pemp.total), 0.00) AS empenhado,
                COALESCE(SUM(dtl.ptrdotacao), 0.00) - COALESCE(pemp.total, 0.00) AS nao_empenhado
SQL;
    }
    $sql_lista = retornaConsultaPTRES($params, 'NOVO');
    //ver($sql_lista,d);
    $html = "
        <html>
            <head>
                <link rel=\"stylesheet\" href=\"/library/bootstrap-switch/stylesheets/bootstrap-switch.css\">
                <script src=\"/library/bootstrap-switch/js/bootstrap-switch.min.js\"></script>
                <script type='text/javascript' src='../includes/funcoes.js'></script>
                <script type='text/javascript'>
                    function selecionarLinha(){
                        alert($(this).attr(\"data-id\"));
                    }
                    jQuery(document).ready(function() {
                        apresentaLinhasSelecionadas($('#orcamento'),2);
                        $('.switch-mini').on('switch-change', function(e, d){
                            d.value;
                            var id = $('input[type=checkbox]', e.target).attr('id');
                            var onclick = $('#' + id).attr('onclick');
                            onclick = onclick.replace('this', '$(\'#\' + id)[0]');
                            eval(onclick);
                        });
                    });
                </script>
            </head>
    ";

    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO, Simec_Listagem::RETORNO_BUFFERIZADO);

    if ($_REQUEST['tipo'] == 'pi') {
        $listagem->setCabecalho(array(
            "PTRES",
            "Ação",
            "Unidade Orçamentária",
            "Dotação&nbsp;Atual&nbsp;(R$)",
            '<center>Subação (R$)</center>' => array('Detalhado', 'Não Detalhado'),
            '<center>Empenho (R$)</center>' => array('Empenhado', 'Não Empenhado'),
        ));
    } else {
        $listagem->setCabecalho(array(
            "",
            "PTRES",
            "Ação",
            "Unidade Orçamentária",
            "Dotação&nbsp;Atual&nbsp;(R$)",
            '<center>Subação (R$)</center>' => array('Detalhado', 'Não Detalhado'),
            '<center>PI (R$)</center>' => array('Detalhado', 'Não Detalhado'),
            '<center>Empenho (R$)</center>' => array('Empenhado', 'Não Empenhado')
        ));
    }
    #ver($sql_lista);
    $listagem->setQuery($sql_lista);
    $listagem->setAcoes(array('view' => 'visualizarRegistro'));
    $listagem->turnOnPesquisator();
    $listagem->addAcao('select', array('func' => 'resultado', 'extra-params' => array('acao')));
    $listagem->setIdLinha();
    $listagem->addCallbackDeCampo(array(
        'dotacaoinicial', 'det_subacao', 'nao_det_subacao', 'det_pi', 'nao_det_pi', 'empenhado', 'nao_empenhado', 'saldo'), 'mascaraMoeda');
    $retorno = $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    $html .= '<div id="pesquisa">' . $retorno . '</div>';

    return $html;
}

function popupDetalhePtres($ptrid) {
    global $db;

    if (isset($_REQUEST['ptrid'])) {
        $ptres = $db->pegaUm("SELECT ptres FROM monitora.ptres WHERE ptrid = {$ptrid}");
        $sql = "
            SELECT dtl.ptres,
                trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acadsc) AS descricao,
                uni.unidsc,
                dtl.plocod,
                plo.plotitulo,
                COALESCE(SUM(DISTINCT dtl.ptrdotacao)+0.00, 0.00)                            AS dotacaoatual,
                COALESCE(SUM(dt.valor), 0.00)                                       AS detalhadosubacao,
                COALESCE(SUM(dt2.valorpi), 0.00)                                    AS detalhadopi,
                COALESCE((pemp.total), 0.00)                                        AS empenhado,
                COALESCE(SUM(DISTINCT dtl.ptrdotacao) - COALESCE(SUM(dt2.valorpi), 0.00), 0.00) AS naodetalhadopi,
                COALESCE(SUM(DISTINCT dtl.ptrdotacao) - COALESCE(SUM(dt.valor), 0.00), 0.00) AS naodetalhadosubacao,
                COALESCE(SUM(DISTINCT dtl.ptrdotacao) - COALESCE(pemp.total, 0.00), 0.00) AS naoempenhado
            FROM monitora.acao aca
            INNER JOIN monitora.ptres dtl ON (aca.acaid = dtl.acaid)
            INNER JOIN public.unidade uni ON (uni.unicod = dtl.unicod)
            LEFT JOIN (SELECT ptrid,
                                SUM(sadvalor) AS valor
                           FROM monitora.pi_subacaodotacao
                           GROUP BY ptrid) dt
                ON (dtl.ptrid = dt.ptrid)
            LEFT JOIN (SELECT ptrid,
                                SUM(dtl.valorpi) AS valorpi
                           FROM monitora.v_pi_detalhepiptres dtl
                           GROUP BY dtl.ptrid) dt2
                ON (dtl.ptrid = dt2.ptrid)
            LEFT JOIN siafi.ptrempenho pemp ON (pemp.ptres = dtl.ptres AND pemp.exercicio = aca.prgano)
            LEFT JOIN monitora.planoorcamentario plo
                  ON (plo.prgcod = aca.prgcod
                      AND plo.acacod = aca.acacod
                      AND plo.unicod = aca.unicod
                      AND dtl.plocod = plo.plocodigo)
            WHERE aca.prgano ='{$_SESSION['exercicio']}'
                AND dtl.ptrano='{$_SESSION['exercicio']}'
                AND dtl.ptres = '{$ptres}'
                AND plo.exercicio = '{$_SESSION['exercicio']}'
                AND aca.acasnrap = FALSE
            GROUP BY dtl.ptrid,
                dtl.ptres,
                descricao,
                uni.unidsc,
                pemp.total,
                dtl.plocod,
                plo.plotitulo
";
#ver($sql);
        $dadosPtres = $db->carregar($sql);
        $dadosPtres = $dadosPtres[0];
    }
    $html = '<style>.modal-dialog{width:70%};</style><div class="col-lg-12">
    <div class="well">
        <fieldset>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-3 control-label">PTRES:</label>
                    <div class="col-lg-09">' . $dadosPtres['ptres'] . '
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-3 control-label">PO:</label>
                    <div class="col-lg-09">' . $dadosPtres['plocod'] . ' - ' . $dadosPtres['plotitulo'] . '
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-3 control-label">Unidade:</label>
                    <div class="col-lg-09">' . $dadosPtres['unidsc'] . '
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-3 control-label">Dotação atual:</label>
                    <div class="col-lg-09"><b>' . number_format2($dadosPtres['dotacaoatual']) . '
                    </b></div>
                </div>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-3 control-label">Detalhado em PI:</label>
                    <div class="col-lg-09" style="color:' . (($dadosPtres['detalhadopi'] >= 0) ? 'black' : 'red') . '">
                         <b>
                <span id="saldo_pi_detalhado_ptres">' . number_format2($dadosPtres['detalhadopi']) . '
                </span>
            </b>
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-3 control-label">Não Detalhado em PI:</label>
                    <div class="col-lg-09" style="color:' . (($dadosPtres['naodetalhadopi'] >= 0) ? 'black' : 'red') . '"><b>'
            . number_format2($dadosPtres['naodetalhadopi']) . '</b>
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-3 control-label">Detalhado em Subação:</label>
                    <div class="col-lg-09" style="color:' . (($dadosPtres['detalhadosubacao'] >= 0) ? 'black' : 'red') . '">
                    <b>
                <span id="saldo_pi_detalhado_ptres">' . number_format2($dadosPtres['detalhadosubacao']) . '
                </span>
                     </b>
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-3 control-label">Não Detalhado em Subação:</label>
                    <div class="col-lg-09" style="color:' . (($dadosPtres['naodetalhadosubacao'] >= 0) ? 'black' : 'red') . '">
                    <b>' . number_format2($dadosPtres['naodetalhadosubacao']) . '
                     </b>
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-3 control-label">Empenhado do PTRES:</label>
                    <div class="col-lg-09">
                    <b>
                <span id="saldo_nao_orcado">' . number_format2($dadosPtres['empenhado']) . '
                </span>
                     </b>
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-3 control-label">Não Empenhado:</label>
                    <div class="col-lg-09" style="color:' . (($dadosPtres['naoempenhado'] >= 0) ? 'black' : 'red') . '">
                    <b>' . number_format2($dadosPtres['naoempenhado']) . '
                     </b>
                    </div>
                </div>
           </div><div style="background-color:#00ced1; color:#FFF; margin-top: 10px; padding: 5px;">
                <b>
                    Subações ' . $_SESSION['exercicio'] . '
               </b>
            </div>';
    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO, Simec_Listagem::RETORNO_BUFFERIZADO);

    $listagem->setCabecalho(array(
        "Código",
        "Subação",
        "Orçamento total da Subação (R$)",
        "Orçamento da Subação neste PTRES (R$)",
        "Detalhado em PI nesta Subação e neste PTRES (R$)",
        "Empenhado nesta Subação e neste PTRES(R$)",
        "Não Detalhado em PI nesta Subação e neste PTRES (R$)"
    ));

    $select = <<<SELECT
SELECT  foo.codigo AS sbacod,
        foo.sbatitulo,
        COALESCE(foo.dotacao, 0.00) AS dotacao,
        COALESCE((SELECT
            sadvalor
        FROM
            monitora.pi_subacaodotacao
        WHERE
            sbaid = foo.sbaid
        AND ptrid = {$ptrid} ),0.00)  as dotacao_no_ptres,
       COALESCE((
            SELECT
                 COALESCE(SUM(dtl.valorpi),0.00)
            FROM
                monitora.v_pi_detalhepiptres dtl
            WHERE
                prgano = '{$_SESSION['exercicio']}'
            AND ptrid = {$ptrid}
        AND sbaid = foo.sbaid
               ), '0.00') AS detalhado_pi_no_ptres,
       COALESCE((
            SELECT
             SUM(total)
         FROM
             siafi.pliptrempenho
         WHERE
             exercicio = '{$_SESSION['exercicio']}'
         AND SUBSTR(plicod, 2,4) = foo.sbacod
         AND ptres = '{$dadosPtres['ptres']}'
        ), '0.00') AS empenhado_no_ptres,
       COALESCE(
        (SELECT
            sadvalor
        FROM
            monitora.pi_subacaodotacao
        WHERE
            sbaid = foo.sbaid
        AND ptrid = {$ptrid} )
            -
        (
            SELECT
                 COALESCE(SUM(dtl.valorpi),0.00)
            FROM
                monitora.v_pi_detalhepiptres dtl
            WHERE
                prgano = '{$_SESSION['exercicio']}'
            AND ptrid = {$ptrid}
        AND sbaid = foo.sbaid
        )
       , '0.00') AS nao_detalhado_pi_no_ptres
SELECT;
    $groupby = array(
        'foo.codigo',
        'foo.sbatitulo',
        'foo.dotacao',
        'foo.sbaid',
        'foo.empenhado',
        'foo.detalhado_pi',
        'foo.sbacod'
    );
    $orderby = array('1');
    $where = "AND ptr.ptrid = {$ptrid}";
    $sql = retornaConsultaSubacao(
            array(
                'SELECT' => $select,
                'groupby' => $groupby,
                'orderby' => $orderby,
                'where' => $where
            )
    );

    $listagem->setQuery($sql);
//        $listagem->setAcoes(array(
//    'edit' => 'editaRegistro'
//));
    $listagem->addCallbackDeCampo(array(
        'dotacao', 'dotacao_no_ptres', 'detalhado_pi_no_ptres', 'empenhado_no_ptres', 'nao_detalhado_pi_no_ptres'), 'mascaraMoeda');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $retorno = $listagem->render();
    if (!$retorno):
        $html .=' <div class="alert alert-info col-md-4 col-md-offset-4 text-center">
        Nenhum registro encontrado
    </div>';
    else:
        $html .= '<div id="pesquisa">' . $retorno . '</div>';
    endif;

    $html .='<div style="background-color:#FF6347; color:#FFF; margin-top: 10px; padding: 5px;">
                <b>
                    Plano Interno ' . $_SESSION['exercicio'] . '
                </b>
            </div>';


    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO, Simec_Listagem::RETORNO_BUFFERIZADO);

    $listagem->setCabecalho(array(
        "PI's subtraindo recursos<br />do PTRES",
        "Título do PI",
        "Orçamento Total do PI (R$)",
        "Orçamento do PI no PTRES (R$)",
        "Empenhado do PI no PTRES (R$)",
        "Não Empenhado do PI no PTRES (R$)"
    ));


    $params['SELECT'] = <<<SQL
SELECT
        gmb.codigo,
        gmb.titulo,
        COALESCE(SUM(pip.pipvalor),0.00) as dotacao_total,
        COALESCE((SELECT
            pip.pipvalor
        FROM
            monitora.pi_planointernoptres pip
        LEFT JOIN
            monitora.ptres
        ON
            pip.ptrid = ptres.ptrid
        WHERE
            ptres.ptrano = '{$_SESSION['exercicio']}'
        AND pip.pliid= gmb.pliid
        AND ptres.ptrid={$ptrid}),0.00) as dotacao_pip_ptres,
        COALESCE((SELECT
            total
        FROM
            siafi.pliptrempenho ppe
        WHERE
            plicod = gmb.codigo
        AND ptres = '{$dadosPtres['ptres']}' AND exercicio = '{$_SESSION['exercicio']}'),0.00) as empenhado_pi_ptres,
        COALESCE((SELECT
            pip.pipvalor
        FROM
            monitora.pi_planointernoptres pip
        LEFT JOIN
            monitora.ptres
        ON
            pip.ptrid = ptres.ptrid
        WHERE
            ptres.ptrano = '{$_SESSION['exercicio']}'
        AND pip.pliid= gmb.pliid
        AND ptres.ptrid={$ptrid}),0.00) - COALESCE((SELECT
            total
        FROM
            siafi.pliptrempenho ppe
        WHERE
            plicod = gmb.codigo
        AND ptres = '{$dadosPtres['ptres']}' AND exercicio = '{$_SESSION['exercicio']}'),0.00) as nao_empenhado_pi_ptres
SQL;
    /* Variáveis da Consulta */
    $params['v_ptrid'] = $ptrid;
    /* Filtros da Consulta */
    $params['where'] = " AND pip.ptrid= {$ptrid} ";
    $params['filtroNoUnionSiafi'] = " AND ptr.ptrid = {$ptrid} ";

    $sql = retornaConsultaPI($params);
    $listagem->setQuery($sql);
    $listagem->addCallbackDeCampo(array(
        'dotacao_total', 'dotacao_pip_ptres', 'empenhado_pi_ptres', 'nao_empenhado_pi_ptres'), 'mascaraMoeda');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $retorno = $listagem->render();
    if (!$retorno):
        $html .=' <div class="alert alert-info col-md-4 col-md-offset-4 text-center">
        Nenhum registro encontrado
    </div>';
    else:
        $html .= '<div id="pesquisa">' . $retorno . '</div>';
    endif;
    return $html;
}

if ($_REQUEST['carregarComboUG'] == 'ok') {
    echo carregarComboUG($_REQUEST['unicod']);
    die();
}
if ($_REQUEST['carregarComboSubacao'] == 'ok') {
    echo carregarComboSubacao($_REQUEST['unicod'], null, $retornaSQL = false);
    die();
}
if ($_REQUEST['sbaAjax']) {
    echo buscaDadosSubacao($_REQUEST['sbaAjax'], $_REQUEST['capidAjax']);
    die();
}

if ($_REQUEST['carregarComboEnquadramentoPorSubacao'] == 'ok') {
    echo carregarComboEnquadramentoPorSubacao($_REQUEST['sbaid']);
    die();
}

/**
 *
 */
require(APPRAIZ . 'planacomorc/modulos/principal/planointerno/cadastropi_requisicoes.inc');

// -- Identificando se o usuário atual faz parte do CGSO/CPMO
$pulaSolicitacao = pulaSolicitacao();

// -- Definições iniciais das variáveis de controle do formulário
$habilitado = 'S';
$obrigatorio = 'S';
$ehSolicitacao = true;
$dadosPI = array();

if (validaRequisicao('pliid')) { // -- Edição de PI
    // -- Carrega dados do PI e libera apenas o titulo e a descrição para edição
    $dadosPI = carregarPI($_REQUEST['pliid']);
    $enquadramento_pi = carregarEnquadramentoPI($_REQUEST['pliid']);
    $habilitado = 'N';

    $ehSolicitacao = false;
    $obrigatorio = 'S';

    // -- Requisição de apagar transação - Não exibe os botões do rodapé, e nem valida o formulário
    if (validaRequisicao('apagar')) {
        $tipoTransacao = '-';
    }
} elseif (validaRequisicao('scpid')) { // -- Transações de cadastro de PI
    // -- Carrega os dados da transação de criação e, de acordo com o tipo:
    // -- E: apenas exibe os dados - read only de uma transação já executada
    // -- S: deixa o formulário aberto para edição e liga as regras de validação - edição de
    // -- informações para confirmar a transação.
    $dadosPI = carregarTransacao($_REQUEST['scpid']);
    $enquadramento_pi = carregarEnquadramentoPI($_REQUEST['scpid']);
    if ('S' == $dadosPI['tipotransacao']) {
        $tipoTransacao = 'E';
        $obrigatorio = 'S';

        // -- Visualização de uma transação já executada - não há botões de comando no rodapé
    } elseif ('E' == $dadosPI['tipotransacao']) {
        $tipoTransacao = '-';
        $habilitado = 'S';
    }
} else { // -- Criação de uma nova transação de cadastro de PI
    // -- Se o perfil for CGSO, criar uma nova transação do tipo E
    // -- senão, cria uma nova transação do tipo S
    $tipoTransacao = 'S';

    if ($pulaSolicitacao) {
        $obrigatorio = 'S';
        $tipoTransacao = 'E';
    }
}
/* Habilita a Edição para Super Usuário */
if ($db->testa_superuser()) {
    $habilitado = 'S';
}

/* Habilita a Edição de PTRES para CPMO e Gestão Orçamentária (Também SU) */
if (
        in_array(PFL_GESTAO_ORCAMENTARIA, pegaPerfilGeral()) ||
        in_array(PFL_CPMO, pegaPerfilGeral()) ||
        in_array(PFL_SUPERUSUARIO, pegaPerfilGeral())
) {
    $habilitadoVincularPtres = 'S';
}
/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

?>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li><a href="<?php echo MODULO; ?>.php?modulo=principal/planointerno/listapimanter&acao=A">Plano Interno</a></li>
        <li class="active"><?php echo $dadosPI?'Alterar':'Cadastrar'; ?> Plano Interno</li>
    </ol>
</div>
<div class="row">
    <div class="col-lg-11">
        <div class="well">
            <fieldset>
                <form id="formulario" name="formulario" method="post" action="" class="form-horizontal">
                    <input type="hidden" name="obrigatoria" id="obrigatoria" value="0" />
                    <input type="hidden" name="hidcodpi" id="hidcodpi" value="" />
                    <input type="hidden" name="evento" id="evento" value="<?php echo(($ehSolicitacao) ? "L" : "S"); ?>" />
                    <input type="hidden" name="plicod" id="plicod" value="<?= $dadosPI['plicod']; ?>" />
                    <input type="hidden" name="pliid" id="pliid" value="<?= $_REQUEST['pliid']; ?>" />
                    <input type="hidden" name="sbacod" id="sbacod" value="<?php echo $dadosPI['sbacod']; ?>" />
                    <input type="hidden" name="sbaidAnterior" id="sbaidAnterior" value="" />
                    <input type="hidden" name="ehSolicitacao" id="ehSolicitacao" value="<?php echo $ehSolicitacao; ?>" />
                    <input type="hidden" name="tipotransacao" id="tipotransacao" value="<?php echo $tipoTransacao; ?>" />
                    <input type="hidden" name="scpid" id="scpid" value="<?php echo $_REQUEST['scpid']; ?>" />
                    <input type="hidden" name="requisicao" id="requisicao" />

                    <?php if ($_GET['apagar']): ?>
                        <div class="form-group">
                            <label for="inputUnidade" class="col-lg-2 control-label"></label>
                            <div class="col-lg-10">
                                <input type="button" class="btn btn-danger" name="btassociar4" id="btassociar4" value="APAGAR"
                                       onclick="removerpi();" />
                            </div>
                        </div>
                    <?php endif; ?>
                    <div class="form-group">
                        <label for="inputUnidade" class="col-lg-2 control-label">Exercício:</label>
                        <div class="col-lg-10">
                            <p class="form-control-static"><?php echo!empty($dadosPI['pliano']) ? $dadosPI['pliano'] : $_SESSION['exercicio']; ?></p>
                        </div>
                    </div>
                    <div class="form-group">

                        <label for="inputUnidade" class="col-lg-2 control-label">Unidade Orçamentária (UO):</label>
                        <div class="col-lg-10">
                            <?php
                            $semFiltroPerfil = false;
                            if (isset($_REQUEST['scpid'])) {
                                $semFiltroPerfil = true;
                            }

                            $sql = consultaUOs($semFiltroPerfil);
                            $unicod = $dadosPI['unicod'];

                            $db->monta_combo('unicod', $sql, 'S', 'Selecione', 'carregarUG', null, null, null, 'N', 'unicod', null, (isset($unicod) ? $unicod : null), null, 'class="form-control chosen-select" style="width=100%;""', null, null);
                            ?>
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="inputUnidade" class="col-lg-2 control-label">Unidade:</label>
                        <div class="col-lg-10 carregaUG">
                            <?php
                            $ungcod = $dadosPI['ungcod'];
                            $ungdata = array();
                            if (!empty($ungcod)) {
                                $sql = <<<DML
                                SELECT ung.ungcod AS codigo,
                                       ung.ungcod || ' - ' || ung.ungabrev AS descricao
                                  FROM public.unidadegestora ung
                                  WHERE ung.ungstatus= 'A'
                                    AND ung.ungcod = '%s' {$filtroPerfilUG}
                                  ORDER BY ung.unicod
DML;
                                $ungdata = sprintf($sql, $ungcod);
                            }
                            $db->monta_combo('ungcod', $ungdata, 'S', 'Selecione', 'carregarSubacao', null, null, null, 'N', 'ungcod', null, (isset($ungcod) ? $ungcod : null), null, 'class="form-control chosen-select" style="width=100%;""', null, null);
                            ?>
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="inputUnidade" class="col-lg-2 control-label">Subação:</label>
                        <div class="col-lg-10 carregaSubacao">
                            <?php
                            $sbaid = $dadosPI['sbaid'];
                            $sbadata = array();
                            if (!empty($unicod)) {
                                $sbadata = carregarComboSubacao($unicod, $ungcod, $retornaSQL = true);
                            }
                            $db->monta_combo('sbaid', $sbadata, 'S', 'Selecione', 'selecionarsubacao', null, null, null, 'N', 'sbaid', null, '', null, 'class="form-control chosen-select" '
                                    . 'style="width=100%;""', null, (isset($sbaid) ? $sbaid : null));
                            $sbadata = '';
                            ?>
                        </div>
                    </div>
                    <?php
                    if ($sbaid) {
                        $sbadata = buscaDadosSubacao($sbaid, null, $retornarArray = true);
                        if ($dadosPI['dotacao']) {
                            $sbadata['dotacao'] = $dadosPI['dotacao'];
                        }
                        if ($dadosPI['empenhado']) {
                            $sbadata['empenhado'] = $dadosPI['empenhado'];
                        }
                    }
                    ?>
                     <?php
                    if(is_array($sbadata)){
                    ?>
                        <div class="form-group">
                            <label for="inputUnidade" class="col-lg-2 control-label">Descrição da Subação:</label>
                            <div class="col-lg-10">
                                <p class="form-control-static" id="sbadsc"> <?php echo (empty($sbadata['sbadsc']) ? 'Descrição...' : $sbadata['sbadsc']); ?></p>
                            </div>
                        </div>

                        <div class="form-group">
                            <label for="inputUnidade" class="col-lg-2 control-label">Dotação da Subação (R$):</label>
                            <div class="col-lg-10">
                                <p class="form-control-static" id="valor_dotacao"><?php echo (empty($sbadata['dotacao']) ? '0,00' : $sbadata['dotacao']); ?></p>
                                <input type="hidden" name="scpdotacaosubacao" id="scpdotacaosubacao" value="<?php echo $sbadata['dotacao']; ?>" />
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="inputUnidade" class="col-lg-2 control-label">Empenhado da Subação (R$):</label>
                            <div class="col-lg-10">
                                <p class="form-control-static" id="valor_empenhado"><?php echo (empty($sbadata['empenhado']) ? '0,00' : $sbadata['empenhado']); ?></p>
                                <input type="hidden" name="scpempenhadosubacao" id="scpempenhadosubacao" value="<?php echo $sbadata['empenhado'] ?>" />
                            </div>
                        </div>
                    <?php
                    }
                    ?>
                    <div class="form-group">
                        <label for="inputUnidade" class="col-lg-2 control-label">Enquadramento da Despesa:</label>
                        <div class="col-lg-10 carregaEnq">
                            <?php
                            $sql = <<<DML
                                SELECT eqdid AS codigo,
                                    eqdcod || ' - ' || eqddsc AS descricao
                                FROM monitora.pi_enquadramentodespesa
                                WHERE eqdstatus = 'A'
                                    AND eqdano = '{$_SESSION['exercicio']}'
                                ORDER BY eqdcod
DML;
                            $eqdid = $dadosPI['eqdid'];
                            $db->monta_combo('eqdid', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', null, null, null, 'N', 'eqdid', null, '', null, 'class="form-control chosen-select" style="width=100%;""', null, (isset($eqdid) ? $eqdid : null));
                            ?>
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="inputUnidade" class="col-lg-2 control-label">Seguimento Cultural:</label>
                        <div class="col-lg-10">
                            <?php
                            $sql = <<<DML
                                SELECT neeid AS codigo,
                                        neecod ||' - '|| needsc AS descricao
                                FROM monitora.pi_niveletapaensino
                                WHERE neeano = '{$_SESSION['exercicio']}'
                                    AND neestatus = 'A'
                                ORDER BY neecod
DML;
                            $neeid = $dadosPI['neeid'];
                            $db->monta_combo('neeid', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', null, null, null, 'N', 'neeid', null, '', null, 'class="form-control chosen-select" style="width=100%;""', null, (isset($neeid) ? $neeid : null));
                            ?>
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="inputUnidade" class="col-lg-2 control-label">Modalidade de Pactuação:</label>
                        <div class="col-lg-10">
                            <?php
                            $sql = <<<DML
                                SELECT capid AS codigo,
                                    capcod ||' - '|| capdsc AS descricao
                                FROM monitora.pi_categoriaapropriacao
                                WHERE capano = '{$_SESSION['exercicio']}'
                                    AND capstatus = 'A'
                                ORDER BY capcod
DML;
                            $capid = $dadosPI['capid'];
                            $db->monta_combo('capid', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', null, null, null, 'N', 'capid', null, '', null, 'class="form-control chosen-select" style="width=100%;""', null, (isset($capid) ? $capid : null));
                            ?>
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="inputUnidade" class="col-lg-2 control-label">Codificação da Unidade(livre):</label>
                        <div class="col-lg-10">
                            <?php
                            $plilivre = $dadosPI['plilivre'];
                            ?>
                            <input type="text" value="<?php
                            if ($dadosPI['plilivre']) {
                                echo $dadosPI['plilivre'];
                            }
                            ?>" style="width:5%;" class="CampoEstilo normal form-control obrigatorio" name="plilivre" id="idCodificacao"
                                   title="" id="idCodificacao" onkeyup="atualizarCodigoLivre();
                                                   atualizarPrevisaoPI();" value="" maxlength="2" size="4" name="plilivre" style="text-align:">

                        </div>
                    </div>
                    <div class="form-group">
                        <label for="inputUnidade" class="col-lg-2 control-label">Área Cultural:</label>
                        <div class="col-lg-10">
                            <?php
                            $sql = <<<DML
                                SELECT
                                    MAX(mdeid) AS codigo,
                                    mdecod ||' - '|| upper(public.removeacento(mdedsc)) AS descricao
                                FROM monitora.pi_modalidadeensino
                                WHERE mdestatus = 'A'
                                    AND mdeano = '{$_SESSION['exercicio']}' -- Resolver 2014
                                GROUP BY descricao
                                ORDER BY descricao ASC
DML;
                            $mdeid = $dadosPI['mdeid'];
                            $db->monta_combo('mdeid', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', null, null, null, 'N', 'mdeid', null, '', null, 'class="form-control chosen-select" style="width=100%;""', null, (isset($mdeid) ? $mdeid : null));
                            ?>
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="inputUnidade" class="col-lg-2 control-label">Título:</label>
                        <div class="col-lg-1">
                            <p id="prefixotitulo" class="form-control-static"><?php echo $dadosPI['sbasigla']; ?></p>
                        </div>
                        <div class="col-lg-9">
                            <input type="text" value="<?php
                            if ($dadosPI['plititulo']) {
                                echo $dadosPI['plititulo'];
                            }
                            ?>" class="CampoEstilo normal form-control obrigatorio" title="" id="plititulo" value="" maxlength="250" size="2" name="plititulo">
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="inputUnidade" class="col-lg-2 control-label">Descrição / Finalidade:</label>
                        <div class="col-lg-10">
                            <? inputTextArea('plidsc', $dadosPI['plidsc'], 'plidsc', 900); ?>
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="inputUnidade" class="col-lg-2 control-label">Data:</label>
                        <div class="col-lg-10">
                            <p class="form-control-static"><?php echo!empty($dadosPI['pliano']) ? $dadosPI['pliano'] : $_SESSION['exercicio']; ?></p>
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="meta" class="col-lg-2 control-label">Outras Metas do PNC Relacionadas: </label>
                        <div class="col-lg-10">
                        <?php
                            $sql = <<<DML
                                SELECT
                                    eqdid AS codigo,
                                    eqdcod || ' - ' || eqddsc AS descricao
                                FROM monitora.pi_enquadramentodespesa
                                WHERE eqdstatus = 'A'
                                    AND eqdano = '{$_SESSION['exercicio']}'
                                ORDER BY eqdcod
DML;
                            inputCombo('m_eqdid[]', $sql, $enquadramento_pi, 'meta',array('multiple'=>'multiple','titulo'=>'Selecione alguma opção'));
                        ?>
                        </div>
                    </div>
                    <div class="panel panel-default">
                        <div class="panel-heading"><strong>Previsão do Plano Interno</strong></div>
                        <table class="table table-striped table-bordered table-hover text-center">
                            <thead>
                                <tr>
                                    <th>Enquadramento</th>
                                    <th colspan="3">Sequencial</th>
                                    <th>Seguimento</th>
                                    <th>Pactuação</th>
                                    <th>Codificação</th>
                                    <th>Área</th>
                                </tr>
                            </thead>
                            <tbody>
                                <tr>
                                    <td>
                                        <span id="enquadramento"><?php echo $dadosPI['eqdcod']; ?></span>
                                    </td>
                                    <td colspan="3">
                                        <span id="subacao"><?php echo (empty($dadosPI['sbacod']) ? 'XXXX' : $dadosPI['sbacod']); ?></span>
                                    </td>
                                    <td>
                                        <span id="nivel"><?php echo $dadosPI['neecod']; ?></span>
                                    </td>
                                    <td>
                                        <span id="apropriacao"><?php echo $dadosPI['capcod']; ?></span>
                                    </td>
                                    <td>
                                        <span id="codificacao"><?php echo $dadosPI['plilivre']; ?></span>
                                    </td>
                                    <td>
                                        <span id="modalidade"><?php echo $dadosPI['mdecod']; ?></span>
                                    </td>
                                </tr>
                            </tbody>
                            <tfoot>
                                <tr>
                                    <td colspan="8">
                                        <b>Código PI / SIAFI:
                                            <div id="codigoPi" style="display:inline">
                                                <span id="enquadramento_i"><?php echo $dadosPI['eqdcod']; ?></span>
                                                <span id="subacao_i"><?php echo (empty($dadosPI['sbacod']) ? 'XXXX' : $dadosPI['sbacod']); ?></span>
                                                <span id="nivel_i"><?php echo $dadosPI['neecod']; ?></span>
                                                <span id="apropriacao_i"><?php echo $dadosPI['capcod']; ?></span>
                                                <span id="codificacao_i"><?php echo $dadosPI['plilivre']; ?></span>
                                                <span id="modalidade_i"><?php echo $dadosPI['mdecod']; ?></span>
                                            </div>
                                        </b>
                                    </td>
                                </tr>
                            </tfoot>
                        </table>
                    </div>
                    <div class="panel panel-default">
                        <div class="panel-heading">
                            <strong style="margin-top: 15px;">Detalhamento Orçamentário</strong>
                            <input type="button" class="btn btn-info" style="float:right;" onclick="abrir_lista();" id="btn_selecionar_acaptres"
                                value="Selecionar Ação/PTRES" <?php echo ('N' == $habilitadoVincularPtres ? 'disabled' : ''); ?> />
                            <br style="clear:both;"/>
                        </div>
                        <table class="table table-hover table-bordered table-hover text-center" id="orcamento" >
                            <thead>
                                <tr>
                                    <th rowspan="2">PTRES<input type="hidden" name="pliptres"></th>
                                    <th rowspan="2">Ação</th>
                                    <th colspan="2">Detalhado em Subação (R$)</th>
                                    <th colspan="2">Empenho (R$)</th>
                                </tr>
                                <tr>
                                    <th>Detalhado</th>
                                    <th>Não&nbsp;Detalhado</th>
                                    <th>Empenhado</th>
                                    <th>Não&nbsp;Empenhado</th>
                                </tr>
                            </thead>
                            <tbody>
                            <?php
                            if ($dadosPI['pliid'] || $dadosPI['scpid']) {
                                if ($dadosPI['pliid']) {
                                    $dadosPTRES = buscarPTRESdoPI($dadosPI['pliid'], $dadosPI['sbaid']);
                                } elseif ($dadosPI['scpid'] && $dadosPI['sbaid']) {
                                    $dadosPTRES = buscarPTRESdaSolicitacao($dadosPI['scpid'], $dadosPI['sbaid']);
                                }
                                //ver($dadosPTRES,d);
                                if ($dadosPTRES) {
                                    $valortotalpi = 0;
                                    foreach ($dadosPTRES as $ptres): ?>
                                <tr id="ptres_<?php echo $ptres['ptrid']; ?>">
                                    <td align="center"><?php echo $ptres['ptres']; ?></td>
                                    <td align="left"><?php echo $ptres['descricao']; ?></td>
                                    <td align="right"><?php echo $ptres['det_subacao']; ?></td>
                                    <td><?php echo $ptres['nao_det_subacao']; ?></td>
                                    <td><?php echo $ptres['empenhado']; ?></td>
                                    <td><?php echo $ptres['nao_empenhado']; if ($ehSolicitacao){ echo "<input type='hidden' value='".$ptres['ptrid']."' name='acaid[".$ptres['ptres']."]'>
                                        <input type='hidden' value='0.00' data-ptres='".$ptres['ptres']."' size='28' name='plivalor[".$ptres['ptres']."][".$ptres['ptrid']."]'>"; }  ?></td>
                                </tr>
                                    <?php endforeach;
                                }
                            }
                            ?>
                            </tbody>
                        </table>
                    </div>
                    <div class="form-group">
                        <div class="col-lg-10 col-lg-offset-2">
                            <button class="btn btn-warning" type="reset" onclick="window.location = window.location;">Limpar</button>
                            <?php if ('-' != $tipoTransacao): ?>

                                <?php if ($_POST['pliid'] && $plisituacao == 'E'): ?>
                                    <input type="button" value="Enviar para Homologação"
                                           onclick="vincular('R', '<?php echo $_POST['pliid']; ?>')"
                                           style="cursor:pointer" />
                                       <?php endif; ?>
                                       <?php
                                       // -- Label do botão de confirmação
                                       //ver($ehSolicitacao,d);
                                       if ($ehSolicitacao) {
                                           if ($pulaSolicitacao) {
                                               $btnConfirmar = 'Homologar PI';
                                           } else {
                                               $btnConfirmar = 'Solicitar novo PI';
                                           }
                                       } else {
                                           $btnConfirmar = 'Salvar PI';
                                       }
                                       ?>
                                <input type="button" class="btn btn-primary" onclick="submeter('<?php echo $dadosPI['plicod']; ?>');"
                                       value="<?php echo $btnConfirmar; ?>" <?php echo $disabled; ?> />
                                   <?php endif; ?>
                        </div>
                    </div>
                </form>
            </fieldset>
        </div>
    </div>
    <div class="col-lg-1">
    <?php
    /* Não exibe nenum botão para o perfil Gabinete */
    $perfis = pegaPerfilGeral();

    if ($dadosPI['pliid']) {
        if (!in_array(PFL_APOIO_GESTAO, $perfis)) {
            $sql = <<<DML
    SELECT pli.unicod
      FROM monitora.pi_planointerno pli
      WHERE pli.pliid = {$dadosPI['pliid']}
DML;

            $arUOdoPI = $db->carregar($sql);
            $arUOdoPI = ($arUOdoPI)?$arUOdoPI:array();

            $unicodsPI = array();
            foreach ($arUOdoPI as $ptres) {
                array_push($unicodsPI, $ptres['unicod']);
            }

            $arUOdoPTRES = array();
            if (in_array('26101', $unicodsPI)) {
                $sql = <<<DML
            SELECT unicod
              FROM monitora.pi_planointernoptres pip
                INNER JOIN monitora.ptres p ON pip.ptrid = p.ptrid
              WHERE pip.pliid = {$dadosPI['pliid']}
DML;
                $arPtres = $db->carregar($sql);
                $arPtres = ($arPtres) ? $arPtres : array();
                foreach ($arPtres as $ptres) {
                    array_push($arUOdoPTRES, $ptres['unicod']);
                }
            }

            if (true) {
                $perfisUsuario = pegaPerfilGeral($_SESSION['usucpf'], $_REQUEST['sisid']);
                $pflcod = PFL_GESTAO_ORCAMENTARIA;
                $sql = <<<DML
        SELECT uni.unicod AS codigo
          FROM planacomorc.usuarioresponsabilidade ur
            INNER JOIN public.unidade uni USING(unicod)
          WHERE ur.usucpf = '{$_SESSION['usucpf']}'
            AND ur.pflcod = '{$pflcod}'
            AND ur.rpustatus = 'A'
DML;
                $usuarioresponsabilidade = $db->carregar($sql);
                $usuarioresponsabilidade = $usuarioresponsabilidade ? $usuarioresponsabilidade : array();
                foreach ($usuarioresponsabilidade as &$ur) {
                    $ur = $ur['codigo'];
                }

                $sql2 = "SELECT plisituacao FROM monitora.pi_planointerno WHERE pliid = {$dadosPI['pliid']}";
                $ultimaSituacao = $db->pegaUm($sql2);

                switch ($ultimaSituacao) {
                    case 'P' : // -- Pendente
                        botaoEnviarRevisao();
                        botaoAprovar($unicodsPI, $perfisUsuario, $usuarioresponsabilidade, $pflcod, $arUOdoPTRES);
                        break;
                    case 'H' : // -- Homologação
                        botaoEnviarRevisao();
                        botaoAprovar($unicodsPI, $perfisUsuario, $usuarioresponsabilidade, $pflcod, $arUOdoPTRES);
                        break;
                    case 'E' : // -- Enviado para Revisão
                        botaoTornarPendente();
                        break;
                    case 'A' : // -- Aprovado
                        botaoCadastrarSIAFI($pliid);
                        break;
                    case 'T' : // -- Atualizar no SIAFI - Setado pelo sistema
                        botaoAtualizarSIAFI($pliid);
                        break;
                    case 'C' ://Cadastrado no SIAFI -- adicionado conforme demanda Cod 240042
                        botaoEnviarRevisao();
                        break;
                }
            }
        }
    }
    ?>
    </div>
</div>

<script type="text/javascript">

    /* Atualizando o PI */
    atualizarPrevisaoPI();
   /**
     * Carrega novo conteúdo para o select de UGs via requisição ajax.
     */
    function carregarUG(unicod) {
        $.post(window.location + '&carregarComboUG=ok&unicod=' + unicod, function(response) {
            $('#ungcod').remove();
            $('.carregaUG').html(response);
            $(".chosen-select").chosen();
            carregarSubacao();
        });
    }

    /**
     * Carrega novo conteúdo para o select de Subações via requisição ajax.
     */
    function carregarSubacao()
    {
        var unicod = $('#unicod option:selected').val();
        var ungcod = $('#ungcod option:selected').val();
        var params = '&carregarComboSubacao=ok&unicod=' + unicod;
        if (ungcod) {
            params += '&ungcod=' + ungcod;
        }
        $.post(window.location + params, function(response) {
            $('#sbaid').remove();
            $('.carregaSubacao').html(response);
            $(".chosen-select").chosen();
        });
    }

    /**
     * Apaga todo o conteúdo o formulário. O select de subação é opcional.
     */
    function limparFormulario(limparSubacao)
    {
        if (limparSubacao) {
            $('sbaid').length = 0;
            var sbaid = $('sbaid');
            sbaid.options[sbaid.options.length] = new Option('Selecione', '');
        }
        $('sbadsc').update('Descrição...');
        $('valor_dotacao').update('0,00');
        $('scpdotacaosubacao').value = '0,00';
        $('valor_empenhado').update('0,00');
        $('scpempenhadosubacao').value = '0,00';

        $('eqdid').length = 0;
        var eqdid = $('eqdid');
        eqdid.options[eqdid.options.length] = new Option('Selecione', '');
        $('plititulo').update();
        $('prefixotitulo').update();
        $('enquadramento').update();
        $('enquadramento_i').update();
        $('subacao').update('XXXX');
        $('subacao_i').update('XXXX');
        $('nivel').update();
        $('nivel_i').update();
        $('apropriacao').update();
        $('apropriacao_i').update();
        $('codificacao').update();
        $('codificacao_i').update();
        $('modalidade').update();
        $('modalidade_i').update();
    }

    function validar(plicod)
    {
        var msg = "";
        var tabela = $('#orcamento');
        var ehSolicitacao = $('#ehSolicitacao').val();
        var tipoTransacao = $('#tipotransacao').val();

        if (!$('#unicod').val()) {
            msg += "<li>O preenchimento do campo Unidade Orçamentária é obrigatório.</li>";
        }
        if (!$('#ungcod').val()) {
            msg += "<li>O preenchimento do campo Unidade é obrigatório.</li>";
        }
        if (!$('#sbaid').val()) {
            msg += "<li>O preenchimento do campo Subação é obrigatório.</li>";
        }
        // testa se foi selecionada o enquadramento
        if (document.getElementById("eqdid").value == '') {
            msg += "<li>O preenchimento do campo Enquadramento da Despesa é obrigatório.</li>";
        }
        if (document.getElementById("neeid").value == '') {
            msg += "<li>O preenchimento do campo Seguimento Cultural é obrigatório.</li>";
        }
        if (document.getElementById("capid").value == '') {
            msg += "<li>O preenchimento do campo Categoria de Apropriação é obrigatório.</li>";
        }
        if (document.formulario.plilivre.value.length != 2) {
            msg += "<li>O preenchimento do campo Codifição da Unidade é obrigatório e igual a 2(dois dígitos).</li>";
        }
        if (!$('#mdeid').val()) {
            msg += "<li>O preenchimento do campo Área Cultural é obrigatório.</li>";
        }
        if (document.formulario.plititulo.value == '') {
            msg += "<li>O preenchimento do campo Título é obrigatório.</li>";
        }
        if (document.formulario.plidsc.value == '') {
            msg += "<li>O preenchimento do campo Descrição é obrigatório.</li>";
        }

        if (msg != "") {
            alert('<ul style="text-align:left;list-style-type:circle">'+msg+'</ul>');
            return false;
        } else {
            if (!ehSolicitacao || 'E' == tipoTransacao) {
                var pi = document.getElementById("enquadramento").innerHTML
                        + document.getElementById("subacao").innerHTML
                        + document.getElementById("nivel").innerHTML
                        + document.getElementById("apropriacao").innerHTML
                        + document.getElementById("codificacao").innerHTML
                        + document.getElementById("modalidade").innerHTML;

                // limpa os espacos
                pi.replace(/ /g, '');

                if (validapi(pi)) {
                    document.getElementById("plicod").value = pi.replace(/ /g, '');
                    $('sbacod').value = $('subacao').innerHTML;
                    return true;
                } else {
                    return false;
                }
            }

            return true;
        }
    }

    function ptresApagar(ptres) {
        var input = document.createElement("input");
        input.setAttribute("type", "hidden");
        input.setAttribute("name", "ids_apagar[]");
        input.setAttribute("value", ptres);
        document.getElementById("formulario").appendChild(input);

    }

    function validapi(pi)
    {
            var retorno = true;
        $.post(window.location + '&piAjax=' + pi, function(res) {
            x = res.responseText;
            retorno = valida2(x);
        });
        return retorno;
    }

    function valida2(pi)
    {
        if (pi.indexOf('pijaexiste') != -1) {
            while (pi.substr(0, 1) == ' ') {
                pi = pi.substr(1, pi.length);
            }
        } else {
            pi = pi.replace(/ /g, '');
        }
        if (!pi || pi == '') {
            return true;
        } else {
            if (pi.substr(0, 10) == 'pijaexiste') {
                pi = pi.replace('pijaexiste', "");
                var alertaDisplay = '<div class="titulo_box">Atenção!</div><div class="texto_box" >Plano Interno já existe!</div><div class="conteudo_box" >Veja abaixo os dados o Plano Interno cadastrado :</div><div class="conteudo_box" >' + pi + '</div><div class="links_box" ><input type="button" onclick=\'closeMessage();return false\' value="Cancelar" /></center>';
                displayStaticMessage(alertaDisplay, false);
                return false;
            }
            var alertaDisplay = '<div class="titulo_box">Atenção!</div><div class="texto_box" >Já existe(m) PI(s) criado(s) com esta estrutura. Veja abaixo a relação dos PI(s) encontrados:</div><div class="conteudo_box" >' + pi + '</div><div class="links_box" >Deseja realmente criar?<br><center><input type="button" onclick=\'document.formulario.submit();\' value="Confirmar" /> <input type="button" onclick=\'closeMessage();return false\' value="Cancelar" /></center>';
            displayStaticMessage(alertaDisplay, false);
            return false;
        }
    }

    function removerpi()
    {
        var conf = confirm("Você realmente deseja excluir este PI?");
        if (conf) {
            document.getElementById('evento').value = 'E';
            document.formulario.submit();
        }
    }

    function Trim(str)
    {
        return str.replace(/^\s+|\s+$/g, "");
    }

    //coloca tabindex no campo valor
    function tabindexcampo() {
        var x = document.getElementsByTagName("input");
        var y = 1;
        for (i = 0; i < x.length; i++) {
            if (x[i].type == "text") {
                if (x[i].name.substr(0, 8) == 'plivalor') {
                    x[i].tabIndex = y;
                    y++;
                }
            }
        }
    }

    function vincular(situacao, pliid) {
        if (pliid) {
            var url = window.location.href;
            var parametros = "requisicao=vincular&pliid=" + pliid + '&situacao=' + situacao;
            var myAjax = new Ajax.Request(
                    url,
                    {
                        method: 'post',
                        parameters: parametros,
                        asynchronous: false,
                        onComplete: function(r) {
                            if (r.responseText) {
                                alert('Dados gravados com Sucesso.');
                                // feito isso por causa da presa.
                                //window.location.reload();
                                document.formulario.submit();
                            }
                        }
                    }
            );
        }
    }

    function selecionarsubacao(sbaid) {
        var sbaidAnterior = document.getElementById("sbaidAnterior").value;
        if (!sbaidAnterior) {
            document.getElementById("sbaidAnterior").value = sbaid;
        }

        if (sbaid) {
            document.getElementById("subacao").innerHTML = "XXXX";
            document.getElementById("subacao_i").innerHTML = "XXXX";

            for (var i = (document.getElementById("orcamento").rows.length - 3); i > 2; i--) {
                document.getElementById("orcamento").deleteRow(i);
            }

            $.post(window.location + '&sbaAjax=' + sbaid, function(response) {

                var dados = response;
                dados = dados.split("!@#");
                $('#prefixotitulo').html(Trim(dados[1]) + ' - ');
                $('#plititulo').value = '';
                $('prefixotitulo').attr('maxlength', 45 - 3 - Trim(dados[1]).length);
                $("#subacao").html(dados[0]);
                $("#subacao_i").html(dados[0]);
                $('#sbadsc').html(dados[3]);
                $('#valor_dotacao').html(dados[4]);
                $('#scpdotacaosubacao').val(dados[4]);
                $('#valor_empenhado').html(dados[6]);
                $('#scpempenhadosubacao').val(dados[6]);
            });

            atualizaTituloCodigoLivre();
            carregarComboEnquadramentoPorSubacao(sbaid);
            document.getElementById("sbaidAnterior").value = sbaid;

        } else {
            $("#subacao").html('XXXX');
            $("#subacao_i").html('XXXX');
            $("#sbadsc").html('XXXX');

            for (var i = (document.getElementById("orcamento").rows.length - 3); i > 1; i--) {
                document.getElementById("orcamento").deleteRow(i);
            }
        }
    }

    function submeter(plicod) {
        var validado = true;
        if (plicod == '') {
            var plicod = jQuery('#hidcodpi').text();
        } else {
            var plicod = pliid;
        }

        if (validar(plicod)) {
            if (validado) {
                $('plititulo').value = $('prefixotitulo').textContent + $('plititulo').value;
                document.formulario.submit();
            }
        }
    }

    function submeterComSituacao(pliid, situacao) {
        var validado = true;

        if (!validar()) {
            return false;
        }

        if (validado) {
            document.formulario.plisituacao.value = situacao;
            document.formulario.submit();
        }
    }

    function alterarpi(pliid) {
        document.getElementById('evento').value = 'A';
        document.getElementById('pliid').value = pliid;
        document.formulario.submit();
    }

    /* Função para subustituir todos */
    function replaceAll(str, de, para) {
        var pos = str.indexOf(de);
        while (pos > -1) {
            str = str.replace(de, para);
            pos = str.indexOf(de);
        }
        return (str);
    }

    function abrir_lista() {
        if (document.getElementById("sbaid").value == '') {
            alert("Selecione uma subação");
            return false;
        }
        $.post(window.location + "&obrigatorio=n&tipo=pi&listaptres=ok&sbaid=" + document.getElementById("sbaid").value, function(html) {
            $('#modal-ptres .modal-body p').html(html);
            $('#modal-ptres .modal-title').html('Seleção de PTRES');
            $('#modal-ptres .btn-primary').remove();
            $('#modal-ptres .btn-default').html('Fechar');
            $('.modal-dialog-ptres').show();
            $('#modal-ptres').modal();
        });
    }

    function atualizarPrevisaoPI() {
        var apropriacao = '';
        if ($('#capid').val()) {
            apropriacao = $('#capid option:selected').text();
            apropriacao = apropriacao.split(' - ');
            apropriacao = apropriacao[0];
        }
        $('#apropriacao').text(apropriacao);
        $('#apropriacao_i').text(apropriacao);

        var enquadramento = '';
        if ($('#eqdid').val()) {
            enquadramento = $('#eqdid option:selected').text();
            enquadramento = enquadramento.split(" - ");
            enquadramento = enquadramento[0];
        }
        $('#enquadramento').text(enquadramento);
        $('#enquadramento_i').text(enquadramento);

        var nivel = '';
        if ($('#neeid').val()) {
            nivel = $('#neeid option:selected').text();
            nivel = nivel.split(" - ");
            nivel = nivel[0];
        }
        $('#nivel').text(nivel);
        $('#nivel_i').text(nivel);

        var modalidade = '';
        if ($('#mdeid').val()) {
            var modalidade = $('#mdeid option:selected').text();
            modalidade = modalidade.split(' - ');
            modalidade = modalidade[0];

            $('#modalidade').text(modalidade);
            $('#modalidade_i').text(modalidade);
        }

        document.getElementById("idCodificacao").value = document.getElementById("idCodificacao").value.toUpperCase();
        document.getElementById("codificacao").innerHTML = document.getElementById("idCodificacao").value;
        document.getElementById("codificacao_i").innerHTML = document.getElementById("idCodificacao").value;

        var subacao = document.getElementById('sbaid').options[document.getElementById('sbaid').selectedIndex].text.split(" - ");
        $('#hidcodpi').val(
            enquadramento
            + subacao[0]
            + nivel
            + apropriacao
            + $('#idCodificacao').val()
            + modalidade
        );
        atualizaTituloCodigoLivre();
    }


    function atualizarCodigoLivre() {
        document.getElementById("idCodificacao").value = document.getElementById("idCodificacao").value.toUpperCase();
        document.getElementById("codificacao").innerHTML = document.getElementById("idCodificacao").value;
        atualizaTituloCodigoLivre();
    }

    function carregarComboEnquadramentoPorSubacao(sbaid) {
         $.post(window.location + '&carregarComboEnquadramentoPorSubacao=ok&sbaid=' + sbaid, function(response) {
            $('#eqdid').remove();
            $('.carregaEnq').html(response);
            $(".chosen-select").chosen();
        });
        atualizarPrevisaoPI();
    }

    function atualizaTituloCodigoLivre() {
        var idCodificacao = document.getElementById("idCodificacao").value;
        if (idCodificacao == '00') {
            var msg = "";
            if (!document.getElementById('sbaid').value) {
                msg += 'Favor escolha uma Subação.\n';
            }

            if (!document.getElementById('capid').value) {
                msg += 'Favor escolha uma Categoria de Apropriação.\n';
            }

            if (msg) {
                alert(msg);
                return false;
            } else {
//                var req = new Ajax.Request(window.location.href, {
//                    method: 'post',
//                    parameters: '&sbaAjax=' + document.getElementById('sbaid').value + '&capidAjax=' + document.getElementById('capid').value,
//                    asynchronous: false,
//                    onComplete: function(res) {
//                        if (res.responseText) {
//                            var dados = res.responseText;
//                            dados = dados.split("!@#");
//                            var apropriacao = dados[2];
//                        }
//                    }
//                });
            }
        } else if ('' != idCodificacao) {
//            var req = new Ajax.Request(window.location.href, {
//                method: 'post',
//                parameters: '&sbaAjax=' + document.getElementById('sbaid').value + '&capidAjax=' + document.getElementById('capid').value,
//                asynchronous: false,
//                onComplete: function(res) {
//                    if (res.responseText) {
//                        var dados = res.responseText;
//                        dados = dados.split("!@#");
//                        var apropriacao = dados[2];
//                    }
//                }
//            });
        }
    }

    function calculovalorPI() {
        var tabela = document.getElementById('orcamento');
        var tot = 0;

        jQuery('.somar').each(function() {
            var valor = jQuery(this).val();

            if ('' == valor) {
                valor = '0';
            }

            valor = replaceAll(valor, '.', '');
            valor = replaceAll(valor, ',', '.');
            tot += parseFloat(valor);
        });

        var c = tot.toString();
        var valortotalpi = document.getElementById('valortotalpi');

        if (valortotalpi) {
            if (c.indexOf('.') == -1) {
                valortotalpi.value = tot.toFixed(2);
            } else {
                valortotalpi.value = Arredonda(tot, 2);
            }
            valortotalpi.onkeyup();
        }
    }

    function Arredonda(valor, casas) {
        var novo = Math.round(valor * Math.pow(10, casas)) / Math.pow(10, casas);
        var c = novo.toString();
        if (c.indexOf('.') == -1) {
            alert(novo);
            return novo;
        } else {
            return novo.toFixed(casas);
        }
    }

    function verificaDisponivel(campo, ptres, vlold) {
        var linha = document.getElementById('ptres_' + ptres);
        var celDetEmPI = 6;
        valordisp = parseFloat(replaceAll(replaceAll(linha.cells[celDetEmPI].innerHTML, ".", ""), ",", "."));
        valoratual = parseFloat(replaceAll(replaceAll(campo.value, ".", ""), ",", "."));
        if (valoratual > (valordisp + parseFloat(replaceAll(replaceAll(vlold, ".", ""), ",", ".")))) {
            alert('Valor não pode ser maior que o "Não Detalhado do PI".');
            campo.value = vlold;
            calculovalorPI();
        }
    }

    function selecionarprograma(sba) {
        document.formulario.evento.value = "SUBACAO";
        document.formulario.submit();
    }

    //   INICIO Tabela PTRES
    function carregaAcao(prgcod) {
        jQuery.ajax({
            url: 'monitora.php?modulo=principal/planotrabalhoUG/listarProgramaUG&acao=A'
            , type: 'POST'
            , data: {prgcod: prgcod, comboacao: true}
            , success: function(data) {
                $(".comboAcao").html(data);
            }
        });
    }

    /* CARREGANDO OS DADOS DE PTRES */
    var tabelaorigem = document.getElementById('orcamento');
    for (i = 2; i < tabelaorigem.rows.length - 2; i++) {
        if (document.getElementById("chk_" + tabelaorigem.rows[i].cells[0].innerHTML)) {
            document.getElementById("chk_" + tabelaorigem.rows[i].cells[0].innerHTML).checked = true;
        }
    }
    /* FIM CARREGANDO OS DADOS DE PTRES */

    function resultado(id, selecionado, adicionais) {
        var ptres = $('tr#' + id + ' td:nth-child(3)').text();
        if (!ptres) {
            alert('Não existe PTRES. Entre em contato com o administrador do sistema.');
            return false;
        }

        if (selecionado == false) {
            var tabelaorigem = document.getElementById('orcamento');
            var colunaPtres = document.getElementById('ptres_' + id);
            if (colunaPtres != null) {
                tabelaorigem.deleteRow(document.getElementById('ptres_' + id).rowIndex);
            }

            // -- Criando a nova linha na tabela da página de origem
            var linha = tabelaorigem.insertRow(2);
            linha.id = "ptres_" + id;
            linha.style.height = '30px';

            // -- O número da primeira linha da tabela onde serão inseridos os novos dados
            var linha = 2;

            // setando o ptres
            var celPTRES = tabelaorigem.rows[linha].insertCell(0);
            celPTRES.style.textAlign = "center";
            celPTRES.innerHTML = ptres;

            var celAcao = tabelaorigem.rows[linha].insertCell(1);
            var acao = $('tr#' + id + ' td:nth-child(4)').text();
            celAcao.innerHTML = acao;

            var celSubacaoDet = tabelaorigem.rows[linha].insertCell(2);
            celSubacaoDet.style.textAlign = "right";
            var subacaoDet = $('tr#' + id + ' td:nth-child(7)').text();
            celSubacaoDet.innerHTML = subacaoDet;

            var celSubacaoNaoDet = tabelaorigem.rows[linha].insertCell(3);
            celSubacaoNaoDet.style.textAlign = "right";
            var subacaoNaoDet = $('tr#' + id + ' td:nth-child(8)').text();
            celSubacaoNaoDet.innerHTML = subacaoNaoDet;

            var celEmpenhoEmp = tabelaorigem.rows[linha].insertCell(4);
            celEmpenhoEmp.style.textAlign = "right";
            var empenhoEmp = $('tr#' + id + ' td:nth-child(9)').text();
            celEmpenhoEmp.innerHTML = empenhoEmp + "<input type='hidden' name='plivalor[" + ptres + "][" + id + "]' size='28'"
                    + "data-ptres='" + ptres + "' value ='0.00' />";
            ;

            var celEmpenhoNaoEmp = tabelaorigem.rows[linha].insertCell(4);
            celEmpenhoNaoEmp.style.textAlign = "right";
            var empenhoNaoEmp = $('tr#' + id + ' td:nth-child(10)').text();
            celEmpenhoNaoEmp.innerHTML = empenhoNaoEmp + "<input type='hidden' name='acaid[" + ptres + "]' value='" + id + "'>";
        } else {
            var tabelaorigem = document.getElementById('orcamento');

            tabelaorigem.deleteRow(document.getElementById('ptres_' + id).rowIndex);
            calculovalorPI();
            //jogando ids desmarcados para verificar se existem no banco, se sim deletar vinculo.
            ptresApagar(id);
        }

    }

    function visualizarRegistro(ptrid) {
        url = 'planacomorc.php?modulo=principal/planointerno/cadastro&acao=A&detalhe=ptres&ptrid=' + ptrid;
        $.post(url, function(html) {
            $('#modal-confirm .modal-body p').html(html);
            $('#modal-confirm .modal-title').html('Detalhamento PTRES');
            $('#modal-confirm .btn-primary').remove();
            $('#modal-confirm .btn-default').html('Fechar');
            $('.modal-dialog').show();
            $('#modal-confirm').modal();
        });
    }
    function apresentaLinhasSelecionadas(tabelaOrigem,apartirDe){
             var val;
             for (i = apartirDe; i < tabelaOrigem.find('tr').length; i++) {
                    val = tabelaOrigem.find('tr')[i].id.split("_");
                    if (document.getElementById(val[1])) {
                           campo = $('tr#' + val[1] + ' td:nth-child(2)').find('span');
                           campo.attr('class','glyphicon glyphicon-remove');
                           campo.attr('title','Remover item');
                           campo.css('color','gray');
                    }
             }
    }
    //   FIM Tabela PTRES

</script>
<div id="divTeste"></div>
<div class="row">
    <div class="col-md-12">
        <div id="modal-ptres" class="modal fade">
            <div class="modal-dialog-ptres">
                <div class="modal-content">
                    <div class="modal-header">
                        <button class="close" aria-hidden="true" data-dismiss="modal" type="button">×</button>
                        <h4 class="modal-title">Aviso!</h4>
                    </div>
                    <div class="modal-body">
                        <p></p>
                    </div>
                    <div class="modal-footer">
                        <button class="btn btn-default" data-dismiss="modal" type="button">Não</button>
                        <button class="btn btn-primary" type="button">Sim</button>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<div class="modal fade" id="modal-revisao" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal"><span aria-hidden="true">&times;</span><span class="sr-only">Close</span></button>
                <h4 class="modal-title" id="myModalLabel">Observações para revisão</h4>
            </div>
            <div class="modal-body">
                <form id="form" name="form">
                    <?php inputTextArea('pihobs', null, 'pihobs', 500); ?>
                </form>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                <button type="button" class="btn btn-primary" onclick="trocarSituacao('E');">Enviar para Revisão</button>
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">
function modalRevisao()
{
    $('#modal-revisao').modal();
}

<?php if ($dadosPI['pliid']): ?>
function trocarSituacao(situacao)
{
    var pliid = <?php echo $dadosPI['pliid']; ?>,
        url = 'planacomorc.php?modulo=principal/planointerno/popuphistoricoplanointerno&acao=A&requisicao=vincular'
            + '&pliid=' + pliid
            + '&situacao=' + situacao;

    if ('E' === situacao) {
        url += '&pihobs='+$('#pihobs').val();
    }

    $.ajax({
        url:url,
        success: function(response) {
            alert('Registro alterado com Sucesso.');
            window.location.reload();
        }
    });
}
<?php endif; ?>
</script>