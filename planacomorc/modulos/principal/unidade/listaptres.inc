<?php
if ($_REQUEST['ptrid']) {
    echo popup($_REQUEST['ptrid']);
    die();
}

/* Unidades Obrigatórias (AD, CAPES, INEP, FNDE, FIES, SUP.MEC, EBSERH */
$obrigatorias = UNIDADES_OBRIGATORIAS;

$atiid = $_REQUEST[atiid] ? $_REQUEST[atiid] : $_GET[atiid];
$prgcod = $_REQUEST['prgcod'];
$acacod = $_REQUEST['acacod'];
$sbaid = $_REQUEST['sbaid'];
$prgano = $_SESSION['exercicio'];

$where .= $_REQUEST['sbaid'] ? "AND dt.sbaid = '" . $_REQUEST['sbaid'] . "'" : '';
$where .= $_REQUEST['prgcod'] ? "AND UPPER(ptr.prgcod) LIKE('%" . strtoupper($_REQUEST['prgcod']) . "%')" : '';
$where .= $_REQUEST['acacod'] ? "AND UPPER(ptr.acacod) LIKE('%" . strtoupper($_REQUEST['acacod']) . "%')" : '';
$buscaLivreTmp = removeAcentos(str_replace("-"," ",$_REQUEST['buscalivre']));
$where .= $_REQUEST['buscalivre'] ? "AND (trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||public.removeacento(aca.acadsc)) ilike('%" . $buscaLivreTmp . "%') OR public.removeacento(ptr.ptres) ilike '%" . $buscaLivreTmp . "%')" : '';
$where .= $_REQUEST['unicod'] ? " AND aca.unicod = '{$_REQUEST['unicod']}' " : "";

/* Parametros para montar a consulta */
$params['where'] = $where;
$params['obrigatorio'] = 'n';


$perfis = pegaPerfilGeral();
if (in_array(PFL_GESTAO_ORCAMENTARIA_IFS, $perfis)) {
    $sqlUO = <<<DML
EXISTS (SELECT 1
         FROM planacomorc.usuarioresponsabilidade rpu
         WHERE rpu.usucpf = '%s'
           AND rpu.pflcod = %d
           AND rpu.rpustatus = 'A'
           AND rpu.unicod  = uni.unicod)
DML;

    $whereLista[] = $whereUO = sprintf($sqlUO, $_SESSION['usucpf'], PFL_GESTAO_ORCAMENTARIA_IFS);
    $whereUO = " AND {$whereUO}";
}

/* Chamada para quando for XLS, tem que ser aqui antes de montar o cabeçalho da página */
if ($_REQUEST['requisicao'] == 'exportarXLS') {
    $resultado = relatorioExcel($_REQUEST, $whereUO, $where);
    $listagem = $resultado['listagem'];
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
}

include_once '_funcoesremanejavalorsubacao.php';
include APPRAIZ . 'includes/cabecalho.inc';


function relatorioExcel($post, $whereUO, $where) {

    global $db;
    $listagem = new Simec_Listagem();
    /* Muda o tipo do objeto  */
    if ($post['requisicao'] == 'exportarXLS') {
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
    }
    $cabecalho = array();

    $sql = <<<DML
        SELECT
            ptr.ptres,
            trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' ||
            aca.acatitulo)                           AS descricao,
            uni.unicod || ' - ' || uni.unidsc        AS unidade,
            COALESCE(SUM(ptr.ptrdotacao)+0.00, 0.00) AS dotacaoinicial,
            COALESCE((SELECT SUM(sadvalor) AS valor FROM monitora.pi_subacaodotacao WHERE ptrid = ptr.ptrid ), 0.00) AS det_subacao,
            COALESCE((SELECT SUM(pipvalor) AS valor FROM monitora.pi_planointernoptres JOIN monitora.pi_planointerno USING(pliid) WHERE plistatus = 'A'
                AND ptrid = ptr.ptrid ), 0.00) AS det_pi,
            COALESCE((pemp.total), 0.00)              AS empenhado,
            COALESCE(SUM(ptr.ptrdotacao) - COALESCE((SELECT SUM(sadvalor) AS valor FROM monitora.pi_subacaodotacao WHERE ptrid = ptr.ptrid ), 0.00), 0.00) AS saldo
        FROM monitora.acao aca
        INNER JOIN monitora.ptres ptr ON aca.acaid = ptr.acaid
        INNER JOIN public.unidade uni ON uni.unicod = ptr.unicod
        LEFT JOIN siafi.uo_ptrempenho pemp ON ( pemp.ptres = ptr.ptres AND pemp.exercicio = '2014' AND pemp.unicod = ptr.unicod)
        WHERE aca.prgano='{$_SESSION['exercicio']}'
            AND ptr.ptrano='{$_SESSION['exercicio']}'
            {$where}
            {$whereUO}
            AND ptr.ptrstatus = 'A'
            AND aca.acasnrap = FALSE
            AND aca.unicod NOT IN('26101','26291', '26290', '26298', '26443', '74902', '73107')
        GROUP BY ptr.ptrid,ptr.ptres,descricao,uni.unidsc, pemp.total,  uni.unicod ORDER BY 2
DML;

    $listagem->setCabecalho(array(
        "PTRES",
        "Ação",
        "Unidade Orçamentária",
        "Dotação Atual(R$)",
        "Detalhado em Subação (R$)",
        "Detalhado em Pi (R$)",
        "Empenhado (R$)",
        "Não Detalhado em Subação (R$)"));
    $listagem->addCallbackDeCampo(array(
        'dotacaoinicial', 'det_subacao', 'det_pi', 'empenhado', 'saldo'), 'mascaraMoeda');
    $dados = $db->carregar($sql);
    #ver($sql);
    if (!is_array($dados)) {
        $dados = array();
    }
    $listagem->setDados($dados);
    $listagem->setFormOff();
    /* Mostrar a query em um hidden na tela */
    $saida['listagem'] = $listagem;
    $saida['sql'] = $sql;

    /* Imprime de acordo com a chamada */
    if ($post['requisicao'] == 'exportarXLS') {
        $_REQUEST['_p'] = 'all';
        $listagem->render();
        die();
    } else {
        return $saida;
    }
}

function popup($ptrid) {
    global $db;
    require_once (APPRAIZ . 'includes/library/simec/Listagem.php');
    include_once APPRAIZ . "includes/library/simec/Grafico.php";


    if (isset($_REQUEST['ptrid'])) {
        $ptres = $db->pegaUm("SELECT ptres FROM monitora.ptres WHERE ptrid = {$_REQUEST['ptrid']}");
        $sql = <<<DML
            SELECT
                ptr.ptres,
                trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acadsc) AS descricao,
                uni.unicod,
                uni.unidsc,
                ptr.plocod,
                plo.plotitulo,
                COALESCE((ptr.ptrdotacao)+0.00, 0.00)                            AS dotacaoatual,
                COALESCE((dt.valor), 0.00)                                       AS detalhadosubacao,
                COALESCE(((pemp.total)), 0.00)                                        AS empenhado,
                COALESCE((ptr.ptrdotacao) - COALESCE((dt.valor), 0.00), 0.00) AS naodetalhadosubacao,
                COALESCE((ptr.ptrdotacao) - COALESCE(pemp.total, 0.00), 0.00) AS naoempenhado
            FROM monitora.acao aca
            INNER JOIN monitora.ptres ptr ON aca.acaid = ptr.acaid
            INNER JOIN public.unidade uni ON uni.unicod = ptr.unicod
            LEFT JOIN ( SELECT ptrid, SUM(sadvalor) AS valor FROM monitora.pi_subacaodotacao GROUP BY ptrid) dt ON ptr.ptrid = dt.ptrid
            LEFT JOIN siafi.uo_ptrempenho pemp ON ( pemp.ptres = ptr.ptres AND pemp.exercicio = aca.prgano AND pemp.unicod = aca.unicod)
            LEFT JOIN monitora.planoorcamentario plo ON (plo.prgcod = aca.prgcod AND plo.acacod = aca.acacod AND plo.unicod = aca.unicod AND ptr.plocod = plo.plocodigo)
            WHERE
                aca.prgano ='{$_SESSION['exercicio']}'
                AND ptr.ptrano='{$_SESSION['exercicio']}'
                AND ptr.ptres = '{$ptres}'
                AND plo.exercicio = '{$_SESSION['exercicio']}'
                AND aca.acasnrap = FALSE
DML;
#ver($sql);
        $dadosPtres = $db->carregar($sql);
        $dadosPtres = $dadosPtres[0];
        if(!isset($dadosPtres['dotacaoatual']) || $dadosPtres['dotacaoatual'] == '' ){
            $dadosPtres['dotacaoatual'] = 0.00;
        }
        /* Monta o HTML do grafico de Execução */
        $sqlGrafico = <<<DML
            SELECT
                'Dotação Atual' AS descricao,
                'Total' as categoria,
                {$dadosPtres['dotacaoatual']} AS valor

            UNION ALL
                SELECT
                   'Empenhado'  AS descricao,
                   'Total' as categoria,
                   vlrempenhado AS valor
                FROM (SELECT SUM(vlrempenhado) AS vlrempenhado FROM spo.siopexecucao sex WHERE sex.exercicio = '{$_SESSION['exercicio']}'
                        AND ptres IN ('{$ptres}') GROUP BY ptres ) foo
            UNION ALL
                SELECT
                    'Liquidado'  AS descricao,
                    'Total' as categoria,
                    vlrliquidado AS valor
                FROM (SELECT SUM(vlrliquidado) AS vlrliquidado FROM spo.siopexecucao sex WHERE sex.exercicio = '{$_SESSION['exercicio']}'
                        AND ptres IN ('{$ptres}') GROUP BY ptres ) foo
            UNION ALL
                SELECT
                    'Pago'  AS descricao,
                    'Total' as categoria,
                    vlrpago AS valor
                FROM (SELECT SUM(vlrpago) AS vlrpago FROM spo.siopexecucao sex WHERE sex.exercicio = '{$_SESSION['exercicio']}'
                        AND ptres IN ('{$ptres}') GROUP BY ptres ) foo
DML;

      $grafico = new Grafico(null, false);

    }
    $dotacaoAtual = number_format2($dadosPtres['dotacaoatual']);
    $corDetSubacao = (($dadosPtres['detalhadosubacao'] >= 0) ? 'black' : 'red');
    $detalhadoSubacao = number_format2($dadosPtres['detalhadosubacao']);
    $corNaoDetSubacao = ($dadosPtres['naodetalhadosubacao'] >= 0) ? 'black' : 'red';
    $naoDetSubacao = number_format2($dadosPtres['naodetalhadosubacao']);
    $empenhado = number_format2($dadosPtres['empenhado']);
    $corNaoEmpenhado =  ($dadosPtres['naoempenhado'] >= 0) ? 'black' :'red';
    $naoEmpenhado = number_format2($dadosPtres['naoempenhado']);

    $html = <<<HTML
        <style>.modal-dialog{width:80%;};</style>

        <div style="width: 50%; float: left;">
            <table class="table table-striped table-bordered table-hover" align="center" cellspacing="1" cellpadding="3">
                <tr>
                    <td class="SubTituloDireita" width="200px">PTRES:</td>
                    <td><b style="font-size:14px">{$dadosPtres['ptres']}</b></td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">PO:</td>
                    <td>{$dadosPtres['plocod']} - {$dadosPtres['plotitulo']}</td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Unidade:</td>
                    <td>{$dadosPtres['unicod']} - {$dadosPtres['unidsc']}</td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="200px">Dotação atual:</td>
                    <td><b>{$dotacaoAtual}</b></td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="200px">Detalhado em Subação:</td>
                    <td style="color: {$corDetSubacao}>
                        <b>
                            <span id="saldo_pi_detalhado_ptres">
                                {$detalhadoSubacao}
                            </span>
                        </b>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="200px">Não Detalhado em Subação:</td>
                    <td style="color: {$corNaoDetSubacao};">
                        <b><span id="saldo_nao_detalhado_subacao">
                            {$naoDetSubacao}
                        </span></b>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="200px">Empenhado do PTRES:</td>
                    <td>
                        <b><span id="saldo_nao_orcado">
                            {$empenhado}
                        </span></b>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="200px">Não Empenhado:</td>
                    <td style="color: {$corNaoEmpenhado};">
                        <b><span id="saldo_nao_empenhado">
                            {$naoEmpenhado}
                        </span></b>
                    </td>
                </tr>
            </table>
        </div>
        <div id="graficoDestino" style="width: 50%; float: left;">
HTML;
    $html .= $grafico->setTipo(Grafico::K_TIPO_COLUNA)->setId('graficoPtres')->setTitulo('Execução Orçamentária')->gerarGrafico($sqlGrafico);
    $html .= <<<HTML
        </div>
        <div style="clear: both;"/>
        <table class="table table-striped table-bordered table-hover">
            <tr>
                <td>
                    <div style="background-color:#00ced1; color:#FFF; margin-top: 10px; padding: 5px;">
                    <b>Subações {$_SESSION['exercicio']}</b>
                    </div>
HTML;

    $select = <<<SELECT
        SELECT
            foo.codigo AS sbacod,
            foo.sbatitulo,
            COALESCE(foo.dotacao, 0.00) AS dotacao,
            COALESCE((SELECT sadvalor FROM monitora.pi_subacaodotacao WHERE sbaid = foo.sbaid
                AND ptrid = {$ptrid} ),0.00)  as dotacao_no_ptres,
            COALESCE((SELECT SUM(total) FROM siafi.pliptrempenho WHERE exercicio = '{$_SESSION['exercicio']}'
                AND SUBSTR(plicod, 2,4) = foo.sbacod
                AND ptres = '{$dadosPtres['ptres']}'), '0.00') AS empenhado_no_ptres
SELECT;
    $groupby = array(
        'foo.codigo',
        'foo.sbatitulo',
        'foo.dotacao',
        'foo.sbaid',
        'foo.empenhado',
        'foo.sbacod'
    );
    $orderby = array('1');
    $where = "AND ptr.ptrid = {$ptrid}";
    $sqlSubacao = retornaConsultaSubacao(
        array(
            'SELECT' => $select,
            'groupby' => $groupby,
            'orderby' => $orderby,
            'where' => $where
        ),
        'n'
    );
    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
    $listagem->setCabecalho(array(
        "Código",
        "Subação",
        "Orçamento total da Subação (R$)",
        "Orçamento da Subação neste PTRES (R$)",
        "Empenhado nesta Subação e neste PTRES(R$)"
    ));
    #ver($sqlSubacao);
    $listagem->setQuery($sqlSubacao);
    $listagem->addCallbackDeCampo(array('dotacao', 'dotacao_no_ptres', 'detalhado_pi_no_ptres', 'empenhado_no_ptres', 'nao_detalhado_pi_no_ptres'), 'mascaraMoeda');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);

    $html .= $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    $html.= <<<HTML
            </tr>
            <tr>
                <td>
                    <div style="background-color:#ff6347; color:#FFF; margin-top: 10px; padding: 5px;">
                        <b>PIs {$_SESSION['exercicio']}</b>
                    </div>
HTML;
    /* Consulta  de PI */
    $sqlPi = <<<SQL
        SELECT
            ppe.plicod,
            uni.unicod || ' - ' || uni.unidsc AS unidsc,
            SUM(ppe.total)                    AS empenhado
        FROM monitora.pi_planointerno pli
        JOIN siafi.uo_pliptrempenho ppe ON pli.plicod = ppe.plicod  AND pli.unicod = ppe.unicod AND pli.pliano = '{$_SESSION['exercicio']}'
        INNER JOIN public.unidade uni ON pli.unicod = uni.unicod
        WHERE exercicio = '{$_SESSION['exercicio']}'
            AND ppe.plicod <> ''
            AND pli.pliid IN (SELECT pliid FROM monitora.pi_planointernoptres WHERE ptrid = {$ptrid})
        GROUP BY ppe.plicod, uni.unicod || ' - ' || uni.unidsc
SQL;
#ver($sqlPi,d);
    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO, Simec_Listagem::RETORNO_BUFFERIZADO);
    $listagem->setCabecalho(array(
        "PI's subtraindo recursos<br />do PTRES",
        "Título do PI",
        "Empenhado do PI no PTRES (R$)"
        ));
    $listagem->setQuery($sqlPi);
    $listagem->addCallbackDeCampo(array('empenhado'), 'mascaraMoeda');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $retornoPi = $listagem->render();
    $html .= $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    $html .= <<<HTML
                </td>
            </tr>
        </table>
HTML;
    /* Gato do Grafico */
    $html.= '<script>jQuery("#graficoPtres").appendTo("#graficoDestino");</script>';
    return $html;
}


if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}
//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
?>

<script>
 $(document).ready(function() {
        // -- Ação do botão buscar
        $('#buscar').click(function() {
            var uri = "planacomorc.php?modulo=principal/unidade/listaptres&acao=A"
                + '&prgcod=' + $('#prgcod').val()
                + '&acacod=' + $('#acacod').val()
                + '&unicod=' + $('#unicod').val()
                + '&buscalivre=' + $('#buscalivre').val();

            window.location = uri;
            //return false;
        });
        $('#exportar').click(function() {
            $('#requisicao').attr('value', 'exportarXLS');
            //$("body").prepend('<div class="ajaxCarregando"></div>');
            //$(".ajaxCarregando").hide().html('Carregando, aguarde...').fadeIn();
            $('#formulario').submit();
        });

    });

    function reload(){
    location.href = 'planacomorc.php?modulo=principal/unidade/listaptres&acao=A';
}

function detalhePtres(ptrid) {
        //divCarregando();
        $.post("planacomorc.php?modulo=principal/unidade/listaptres&acao=A&ptrid=" + ptrid, function(html) {
            $('#modal-confirm .modal-body p').html(html);
            $('#modal-confirm .modal-title').html('Dados do PTRES');
            $('#modal-confirm .btn-primary').remove();
            $('#modal-confirm .btn-default').html('Fechar');
            $('.modal-dialog').show();
            $('#modal-confirm').modal();
            //divCarregado();
        });
    }
</script>
<?php
$sql_lista = <<<DML
    SELECT
        ptr.ptrid AS codigo,
        ptr.ptres,
        trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' ||
        aca.acatitulo)                           AS descricao,
        uni.unicod || ' - ' || uni.unidsc        AS unidade,
        COALESCE(SUM(ptr.ptrdotacao)+0.00, 0.00) AS dotacaoinicial,
        COALESCE((SELECT
                SUM(sadvalor) AS valor
                FROM monitora.pi_subacaodotacao
                WHERE ptrid = ptr.ptrid ), 0.00) AS det_subacao,
        COALESCE((SELECT
                SUM(pipvalor) AS valor
                FROM monitora.pi_planointernoptres
                JOIN monitora.pi_planointerno USING(pliid)
                WHERE plistatus = 'A'
                    AND ptrid = ptr.ptrid ), 0.00) AS det_pi,
        COALESCE((pemp.total), 0.00)              AS empenhado,
        COALESCE(SUM(ptr.ptrdotacao) -
            COALESCE((SELECT
                    SUM(sadvalor) AS valor
                    FROM monitora.pi_subacaodotacao
                    WHERE ptrid = ptr.ptrid ), 0.00), 0.00) AS saldo
    FROM monitora.acao aca
    INNER JOIN monitora.ptres ptr ON aca.acaid = ptr.acaid
    INNER JOIN public.unidade uni ON uni.unicod = ptr.unicod
    LEFT JOIN siafi.uo_ptrempenho pemp ON (pemp.ptres = ptr.ptres
        AND pemp.exercicio = '{$_SESSION['exercicio']}'
        AND pemp.unicod = ptr.unicod)
    WHERE aca.prgano='{$_SESSION['exercicio']}'
        AND ptr.ptrano='{$_SESSION['exercicio']}'
        {$where}
        {$whereUO}
        AND ptr.ptrstatus = 'A'
        AND aca.acasnrap = FALSE
        AND aca.unicod NOT IN('26101','26291', '26290', '26298', '26443', '74902', '73107')
    GROUP BY ptr.ptrid,ptr.ptres,descricao,uni.unidsc, pemp.total, uni.unicod
    ORDER BY 1
DML;
#ver($sql_lista);
$cabecalho = array(
    "PTRES",
    "Ação",
    "Unidade Orçamentária",
    "Dotação Atual(R$)",
    "Detalhado em<br />Subação (R$)",
    "Detalhado em<br />PI (R$)",
    "Empenhado&nbsp;(R$)",
    "Não&nbsp;Detalhado&nbsp;em<br />Subação (R$)"
);
?>
<div id="dialog" title="Dados do PTRES"></div>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Instituições Federais / Lista de PTRES <?php echo $_SESSION['exercicio']; ?> </li>
    </ol>
</div>
<div class="col-lg-12">
    <div class="well">
        <fieldset>
            <form id="formulario" name="formulario" method="POST"
                  class="form-horizontal">
                <input id="requisicao" type="hidden" name="requisicao">
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Programa:</label>
                    <div class="col-lg-10">
                        <?php
                        $sql = "SELECT p.prgcod as codigo, (p.prgcod || ' - ' || p.prgdsc) as descricao
			FROM monitora.programa p
			WHERE p.prgano = '" . $prgano . "' ORDER BY prgcod";
                        $db->monta_combo('prgcod', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'prgcod', null, isset($_REQUEST['prgcod']) ? $_REQUEST['prgcod'] : '', null, 'class="form-control chosen-select" style="width=100%;""', null, null);
                        ?>
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputNatureza" class="col-lg-2 control-label">Ação:</label>
                    <div class="col-lg-10">
                        <?php
                        $sql = "SELECT ac.acacod as codigo, (ac.acacod || ' - ' || ac.acadsc) as descricao
 			FROM monitora.acao ac
 			WHERE ac.prgano = '" . $prgano . "' AND ac.acastatus = 'A' AND ac.acasnrap = false
 			GROUP BY ac.acacod, ac.acadsc
 			ORDER BY ac.acacod";
                        $db->monta_combo('acacod', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'acacod', null, isset($_REQUEST['acacod']) ? $_REQUEST['acacod'] : '', null, 'class="form-control chosen-select" style="width=100%;" required="required""', null, null);
                        ?> </div>
                </div>
                <div class="form-group">
                    <label for="inputNatureza" class="col-lg-2 control-label">Unidade:</label>
                    <div class="col-lg-10">
                    <?php
                        $unidade = $_REQUEST['unidade'];
                        $sql = <<<DML
                                                        SELECT
                        uni.unicod AS codigo,
                        uni.unicod || ' - ' || unidsc AS descricao
                        FROM
                        public.unidade uni
                        WHERE
                            (
                                uni.orgcod = '26000'
                            OR  uni.unicod IN('74902',
                                              '73107'))
                        AND uni.unistatus = 'A'
                         {$whereUO}
                        AND uni.unicod NOT IN($obrigatorias)
                        ORDER BY uni.unicod
DML;
//ver($sql,d);
                        if (in_array(PFL_GESTAO_ORCAMENTARIA, $perfis) ||
                                in_array(PFL_SUPERUSUARIO, $perfis)) {
                            $unicod = $_REQUEST['unicod'];
                        } else {
                            $recuperaunicod = $db->pegaLinha($sql);
                            $unicod = $recuperaunicod['codigo'];
                        }
                        $db->monta_combo('unicod', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'unicod', null, isset($unidade) ? $unidade : '', null, 'class="form-control chosen-select" style="width=100%;""', null, null);
                    ?>
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputNatureza" class="col-lg-2 control-label">Buscar:</label>
                    <div class="col-lg-10">
                        <input type="text" class="CampoEstilo normal form-control obrigatorio" title="" id="buscalivre" value="" maxlength="150" size="68" name="buscalivre">
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-2">
                        <button class="btn btn-warning" type="reset" onclick="reload()">Limpar</button>
                        <button class="btn btn-primary" id="buscar" type="button">Buscar</button>
                        <button type="button" class="btn btn-danger" id="exportar">Exportar XLS</button>
                    </div>
                </div>
            </form>
        </fieldset>
    </div>
<?php
    #ver($sql_lista);
    $listagem = new Simec_Listagem();
    $listagem->setCabecalho(array(
        "PTRES",
        "Ação",
        "Unidade Orçamentária",
        "Dotação Atual(R$)",
        "Detalhado em Subação (R$)",
        "Detalhado em Pi (R$)",
        "Empenhado (R$)",
        "Não Detalhado em Subação (R$)"));
    $listagem->setQuery($sql_lista);
    $listagem->addCallbackDeCampo(array(
        'dotacaoinicial', 'det_subacao', 'det_pi', 'empenhado', 'saldo'), 'mascaraMoeda');
    $listagem->setAcoes(array(
        'view' => 'detalhePtres'
    ));
    $listagem->addCallbackDeCampo(array('descricao', 'unidade' ), 'alinhaParaEsquerda');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $listagem->turnOnPesquisator();
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>
    </div>
</html>
