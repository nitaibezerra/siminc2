<?php
/**
 * $Id: popupptres.inc 102359 2015-09-11 18:26:07Z maykelbraz $
 *
 */
if ($_REQUEST['detalhe']) {
    echo popupDetalhe($_REQUEST['ptrid']);
    die();
}

require_once APPRAIZ . "www/planacomorc/_funcoes.php";
require_once APPRAIZ . "www/planacomorc/_constantes.php";
require_once APPRAIZ . "www/planacomorc/_funcoespi.php";

function popupDetalhe($ptrid) {
    global $db;

    if (isset($_REQUEST['ptrid'])) {
        $ptres = $db->pegaUm("SELECT ptres FROM monitora.ptres WHERE ptrid = {$_REQUEST['ptrid']}");
        $sql = "
            SELECT
                ptr.ptres,
                trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acadsc) AS descricao,
                uni.unicod,
                uni.unidsc,
                ptr.plocod,
                plo.plotitulo,
                COALESCE(SUM(DISTINCT ptr.ptrdotacao)+0.00, 0.00)                            AS dotacaoatual,
                COALESCE(SUM(dt.valor), 0.00)                                       AS detalhadosubacao,
                COALESCE((SUM(pemp.total)), 0.00)                                        AS empenhado,
                COALESCE(SUM(DISTINCT ptr.ptrdotacao) - COALESCE(SUM(dt.valor), 0.00), 0.00) AS naodetalhadosubacao,
                COALESCE(SUM(DISTINCT ptr.ptrdotacao) - COALESCE(pemp.total, 0.00), 0.00) AS naoempenhado
            FROM monitora.acao aca
            INNER JOIN monitora.ptres ptr ON aca.acaid = ptr.acaid
            INNER JOIN public.unidade uni ON uni.unicod = ptr.unicod
            LEFT JOIN ( SELECT ptrid, SUM(sadvalor) AS valor
                    FROM
                        monitora.pi_subacaodotacao
                    GROUP BY
                        ptrid) dt
            ON
                ptr.ptrid = dt.ptrid
            LEFT JOIN
                siafi.uo_ptrempenho pemp
            ON
                (
                    pemp.ptres = ptr.ptres
                AND pemp.exercicio = aca.prgano
                AND pemp.unicod = aca.unicod)
            LEFT JOIN
                monitora.planoorcamentario plo
            ON
                plo.prgcod = aca.prgcod
            AND plo.acacod = aca.acacod
            AND plo.unicod = aca.unicod
            AND ptr.plocod = plo.plocodigo
            WHERE
                aca.prgano ='{$_SESSION['exercicio']}'
                AND ptr.ptrano='{$_SESSION['exercicio']}'
                AND ptr.ptres = '{$ptres}'
                AND plo.exercicio = '{$_SESSION['exercicio']}'
                AND aca.acasnrap = FALSE
            GROUP BY
                ptr.ptrid,
                ptr.ptres,
                descricao,
                uni.unicod,
                uni.unidsc,
                pemp.total,
                ptr.plocod,
                plo.plotitulo
";

        $dadosPtres = $db->carregar($sql);
        $dadosPtres = $dadosPtres[0];
        if(!isset($dadosPtres['dotacaoatual']) || $dadosPtres['dotacaoatual'] == '' ){
            $dadosPtres['dotacaoatual'] = 0.00;
        }
        /* Monta o HTML do grafico de Execução */
        $sqlGrafico = "
            SELECT
                'Dotação Atual' AS descricao,
                'Total' as categoria,
                {$dadosPtres['dotacaoatual']} AS valor

            UNION ALL
            SELECT
                'Empenhado'  AS descricao,
                'Total' as categoria,
                vlrempenhado AS valor
            FROM
                (
                    SELECT
                        SUM(vlrempenhado) AS vlrempenhado
                    FROM
                        spo.siopexecucao sex
                    WHERE
                        sex.exercicio = '{$_SESSION['exercicio']}'
                    AND ptres IN ('{$ptres}')
                    GROUP BY
                        ptres ) foo
            UNION ALL
            SELECT
                'Liquidado'  AS descricao,
                'Total' as categoria,
                vlrliquidado AS valor
            FROM
                (
                    SELECT
                        SUM(vlrliquidado) AS vlrliquidado
                    FROM
                        spo.siopexecucao sex
                    WHERE
                        sex.exercicio = '{$_SESSION['exercicio']}'
                    AND ptres IN ('{$ptres}')
                    GROUP BY
                        ptres ) foo
            UNION ALL
            SELECT
                'Pago'  AS descricao,
                'Total' as categoria,
                vlrpago AS valor
            FROM
                (
                    SELECT
                        SUM(vlrpago) AS vlrpago
                    FROM
                        spo.siopexecucao sex
                    WHERE
                        sex.exercicio = '{$_SESSION['exercicio']}'
                    AND ptres IN ('{$ptres}')
                    GROUP BY
                        ptres ) foo";

      $grafico = new Grafico(null, false);

    }
    $html = '<style>.modal-dialog{width:80%;};</style>

        <div style="width: 50%; float: left;">
    <table class="table table-striped table-bordered table-hover" align="center" cellspacing="1" cellpadding="3">
    <tr>
        <td class="SubTituloDireita" width="200px">PTRES:</td>
        <td><b style="font-size:14px">' . $dadosPtres['ptres'] . '</b></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">PO:</td>
        <td>' . $dadosPtres['plocod'] . ' - ' . $dadosPtres['plotitulo'] . '</td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Unidade:</td>
        <td>' . $dadosPtres['unicod'] . ' - ' . $dadosPtres['unidsc'] . '</td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="200px">Dotação atual:</td>
        <td><b>' . number_format2($dadosPtres['dotacaoatual']) . '</b></td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="200px">Detalhado em Subação:</td>
        <td style="color:' . (($dadosPtres['detalhadosubacao'] >= 0) ? 'black' : 'red') . '>
            <b>
                <span id="saldo_pi_detalhado_ptres">
                    ' . number_format2($dadosPtres['detalhadosubacao']) . '
                </span>
            </b>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="200px">Não Detalhado em Subação:</td>
        <td style="color:';
    if ($dadosPtres['naodetalhadosubacao'] >= 0) {
        $html .= 'black';
    } else {
        $html .= 'red';
    }
    $html .='">
            <b>
            <span id="saldo_nao_detalhado_subacao">
                ' . number_format2($dadosPtres['naodetalhadosubacao']) . '
                    </span>
            </b>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="200px">Empenhado do PTRES:</td>
        <td>
            <b>
                <span id="saldo_nao_orcado">
                    ' . number_format2($dadosPtres['empenhado']) . '
                </span>
            </b>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="200px">Não Empenhado:</td>
        <td style="color:';
    if ($dadosPtres['naoempenhado'] >= 0) {
        $html .= 'black';
    } else {
        $html .= 'red';
    }

        $html .='">
            <b>
                        <span id="saldo_nao_empenhado">
                ' . number_format2($dadosPtres['naoempenhado']) . '
                    </span>
            </b>
        </td>
    </tr>
    </table>
    </div>
    <div id="graficoDestino" style="width: 50%; float: left;">
        '.$grafico->setTipo(Grafico::K_TIPO_COLUNA)->setId('graficoPtres')->setTitulo('Execução Orçamentária')->gerarGrafico($sqlGrafico).'
    </div>
    <div style="clear: both;"/>
    <table class="table table-striped table-bordered table-hover">
        <tr>
            <td>
              <div style="background-color:#00ced1; color:#FFF; margin-top: 10px; padding: 5px;">
                <b>
                    Subações ' . $_SESSION['exercicio'] . '
                </b>
            </div>';

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
             SUM(total)
         FROM
             siafi.pliptrempenho
         WHERE
             exercicio = '{$_SESSION['exercicio']}'
         AND SUBSTR(plicod, 2,4) = foo.sbacod
         AND ptres = '{$dadosPtres['ptres']}'
        ), '0.00') AS empenhado_no_ptres
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
    $listagem->addCallbackDeCampo(array(
        'dotacao', 'dotacao_no_ptres', 'detalhado_pi_no_ptres', 'empenhado_no_ptres', 'nao_detalhado_pi_no_ptres'), 'mascaraMoeda');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);

    $html .= $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    $html.='</tr><tr>
            <td>
              <div style="background-color:#ff6347; color:#FFF; margin-top: 10px; padding: 5px;">
                <b>
                    PIs ' . $_SESSION['exercicio'] . '
                </b>
            </div>';
    /* Consulta  de PI */
    $sqlPi = <<<SQL
        SELECT
            ppe.plicod,
            uni.unicod || ' - ' || uni.unidsc AS unidsc,
            SUM(ppe.total)                    AS empenhado
        FROM monitora.pi_planointerno pli
        JOIN siafi.uo_pliptrempenho ppe ON pli.plicod = ppe.plicod AND pli.unicod = ppe.unicod AND pli.pliano = '{$_SESSION['exercicio']}'
        INNER JOIN public.unidade uni ON pli.unicod = uni.unicod
        WHERE exercicio = '{$_SESSION['exercicio']}'
            AND ppe.plicod <> ''
            AND pli.pliid IN (SELECT   pliid FROM   monitora.pi_planointernoptres WHERE ptrid= {$ptrid})
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
    $html .='</td>
    </tr>

</table>';
    /* Gato do Grafico */
    $html.='<script>jQuery("#graficoPtres").appendTo("#graficoDestino");</script>';
    return $html;
}

if (isAjax() && isset($_POST['prgcod']) && isset($_POST['comboacao'])) {

    $sql = "SELECT ac.acacod as codigo, (ac.acacod || ' - ' || ac.acadsc) as descricao
            FROM monitora.acao ac
            WHERE
                ac.unicod IS NOT NULL AND
                ac.prgano = '" . $_SESSION['exercicio'] . "' AND
                ac.acastatus = 'A' AND
                ac.acasnrap = false AND
                ac.prgcod = '" . (int) $_POST['prgcod'] . "'
            GROUP BY ac.acacod, ac.acadsc
            ORDER BY ac.acacod";
    $db->monta_combo('acacod', $sql, 'S', 'Selecione', '', '', '', 400);
    exit;
}

/* Filtro para a UO */
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
    $whereUO = sprintf($sqlUO, $_SESSION['usucpf'], PFL_GESTAO_ORCAMENTARIA_IFS);
    $whereUO = " AND {$whereUO}";
}

// verificando se é undidade ou unidade gestora
$where .= $_REQUEST['sbaid'] && $_REQUEST['sbaid'] != 'null' ? " AND ptr.ptrid IN (SELECT ptrid FROM monitora.pi_subacaodotacao WHERE sbaid = " . $_REQUEST['sbaid'] . ")" : '';
$where .= $_POST['prgcod'] ? "AND UPPER(ptr.prgcod) LIKE('%" . strtoupper($_POST['prgcod']) . "%')" : '';
$where .= $_POST['acacod'] ? "AND UPPER(aca.acacod) LIKE('%" . strtoupper($_POST['acacod']) . "%')" : '';
$where .= $_REQUEST['unicod'] ? " AND aca.unicod = '{$_REQUEST['unicod']}'" : '';
$where .= $_POST['buscalivre'] ? "AND (trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) ilike('%" . $_POST['buscalivre'] . "%') OR dtl.ptres ilike '%" . $_POST['buscalivre'] . "%')" : '';

/* Modifica a conta para o limite de detalhamento */
$detalhamentoPi = "(COALESCE(ptr.ptrdotacao, 0.00) - COALESCE(SUM(pip.pipvalor), 0.00)) AS nao_det_pi,";
if($_REQUEST['sbaid'] && $_REQUEST['sbaid'] != 'null'){
    $detalhamentoPi = "(COALESCE(SUM(dts.valor), 0.00) - COALESCE(SUM(pip.pipvalor), 0.00)) AS nao_det_pi,";
}
$sql_lista = <<<DML
    SELECT
        ptr.ptrid,
        ptr.ptres,
        trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acatitulo) AS descricao,
        aca.unicod || ' - ' || uni.unidsc as unidsc,
        COALESCE(ptr.ptrdotacao, 0.00) AS dotacaoatual,
        COALESCE(SUM(dts.valor), 0.00) AS det_subacao,
        (COALESCE(SUM(ptr.ptrdotacao), 0.00) - COALESCE(SUM(dts.valor), 0.00)) AS nao_det_subacao,
        COALESCE(SUM(pip.pipvalor), 0.00) AS det_pi,
        $detalhamentoPi
        COALESCE((pemp.total), 0.00) AS empenhado,
        COALESCE(SUM(ptr.ptrdotacao), 0.00) - COALESCE(pemp.total, 0.00) AS nao_empenhado
    FROM monitora.ptres ptr
    INNER JOIN monitora.acao aca on ptr.acaid = aca.acaid
    INNER JOIN public.unidade uni on aca.unicod = uni.unicod
    LEFT JOIN (SELECT pip2.ptrid, pipvalor  FROM monitora.pi_planointernoptres pip2 JOIN monitora.pi_planointerno USING (pliid) WHERE plistatus ='A') pip on ptr.ptrid = pip.ptrid
    LEFT JOIN (SELECT ptrid, SUM(sadvalor) AS valor
        FROM monitora.pi_subacaodotacao
        GROUP BY ptrid) dts ON dts.ptrid = ptr.ptrid
    LEFT JOIN (
        SELECT ex.unicod, ex.ptres, ex.exercicio, sum(ex.vlrempenhado) AS total
        FROM spo.siopexecucao ex
        WHERE ex.unicod  NOT IN('26101','26291', '26290', '26298', '26443', '74902', '73107')
            AND ex.exercicio = '{$_SESSION['exercicio']}'
        GROUP BY ex.unicod, ex.ptres, ex.exercicio
        ) pemp ON (pemp.ptres = ptr.ptres AND pemp.exercicio = ptr.ptrano AND pemp.unicod = ptr.unicod)
    WHERE ptr.ptrano = '{$_SESSION['exercicio']}'
        AND aca.unicod NOT IN('26101','26291', '26290', '26298', '26443', '74902', '73107')
        $where
        $whereUO
        AND ptr.ptrstatus = 'A'
    GROUP BY ptr.ptrid, ptr.ptres,aca.prgcod, aca.acacod,aca.unicod,aca.loccod,aca.acatitulo,uni.unidsc, ptr.ptrdotacao, pemp.total
    ORDER BY ptr.ptres
DML;
#ver($sql_lista);
?>
<html>
    <head>
        <script src="/library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
        <link href="../library/chosen-1.0.0/chosen.css" rel="stylesheet"  media="screen" >
        <script type="text/javascript" lang="JavaScript">
            function parametrosFiltro()
            {
                var params = {};
                $('#modal-ptres #formulario select').each(function() {
                    var inputName = $(this).attr('name'),
                    inputValue = $(this).val();
                    params[inputName] = inputValue;
                });
                $('#modal-ptres #formulario input:hidden').each(function() {
                    var inputName = $(this).attr('name'),
                    inputValue = $(this).val();
                    params[inputName] = inputValue;
                });
                return params;
            }

            function atualizaListagemPTRES(params){
                $('#modal-ptres .modal-body').html('');
                $.post('planacomorc.php?modulo=principal/unidade/popupptres&acao=A&obrigatorio=n', params, function(response) {
                    $('#modal-ptres .modal-body').html(response);
                    //console.log(response);
                    delegatePaginacao();
                    $('#modal-ptres .chosen-select').chosen();
                    $('#modal-ptres .chosen-container').css('width', '100%');
                });
            }

            function delegatePaginacao(){
                $('.container-listing').on('click', 'li[class="pgd-item"]:not(".disabled")', function() {
                    // -- definindo a nova página
                    var novaPagina = $(this).attr('data-pagina');
                    var params = parametrosFiltro();
                    params['listagem[p]'] = novaPagina;
                    atualizaListagemPTRES(params);
                });
            }

            function apresentaLinhasSelecionadas(tabelaOrigem, apartirDe) {
                var val;
                for (i = apartirDe; i < tabelaOrigem.find('tr').length; i++) {
                    val = tabelaOrigem.find('tr')[i].id.split("_");
                    //alert(val);
                    if (document.getElementById(val[1])) {
                        campo = $('tr#' + val[1] + ' td:nth-child(2)').find('span');
                        //alert(campo);
                        campo.attr('class', 'glyphicon glyphicon-remove');
                        campo.attr('title', 'Remover item');
                        campo.css('color', 'gray');
                    }
                }
            }

            $(document).ready(function(){
                apresentaLinhasSelecionadas($('#orcamento'), 2);

                $('#modal-ptres .btn-info').click(function(e) {
                    e.stopPropagation();
                    var params = parametrosFiltro();
                    atualizaListagemPTRES(params);
                    return false;
                });
            });
        </script>
    </head>
    <body>
        <div class="col-lg-12">
            <div class="well">
                <fieldset>
                    <form id="formulario" name="formulario" method="post" action="" class="form-horizontal">
                        <input type="hidden" name="sbaid" value="<?=$_REQUEST['sbaid']?>">
                        <input type="hidden" name="unicod" value="<?=$_REQUEST['unicod']?>">
                        <div class="form-group">
                            <label for="inputUnidade" class="col-lg-2 control-label">Programa:</label>
                            <div class="col-lg-10">
                                <?php
                                $sql = "SELECT p.prgcod as codigo, (p.prgcod || ' - ' || p.prgdsc) as descricao
			FROM monitora.programa p
			WHERE p.prgano = '" . $_SESSION['exercicio'] . "' ORDER BY prgcod";
                                $db->monta_combo('prgcod', $sql, 'S', 'Selecione', 'carregaAcao', null, null, 400, 'N', 'prgcod', null, (isset($_POST['prgcod']) ? $_POST['prgcod'] : null), null, 'class="form-control chosen-select" style="width=100%;""', null, null);
                                ?>
                            </div>
                        </div>
                        <div class="form-group">

                            <label for="inputUnidade" class="col-lg-2 control-label">Ação:</label>
                            <div class="col-lg-10 comboAcao">
                                <?php
                                $sql = "SELECT ac.acacod as codigo, (ac.acacod || ' - ' || ac.acadsc) as descricao
 			FROM monitora.acao ac
 			WHERE ac.unicod IS NOT NULL AND ac.prgano = '" . $_SESSION['exercicio'] . "' AND ac.acastatus = 'A' AND ac.acasnrap = false
 			GROUP BY ac.acacod, ac.acadsc
 			ORDER BY ac.acacod";
                                $db->monta_combo('acacod', $sql, 'S', 'Selecione', '', null, null, 400, 'N', 'acacod', null, (isset($_POST['acacod']) ? $_POST['acacod'] : null), null, 'class="form-control chosen-select" style="width=100%;""', null, null);
                                ?>
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-lg-10 col-lg-offset-2">
                                <input type="submit" class="btn btn-info" value="Pesquisar" name="botao" />
                            </div>
                        </div>
                    </form>
                </fieldset>
            </div>
        </div>
        <br>
        <?php
        $listagem = new Simec_Listagem();
        if ($_REQUEST['tipo'] == 'pi') {
            $listagem->setCabecalho(array(
                "PTRES",
                "Ação",
                "Unidade Orçamentária",
                "Dotação&nbsp;Atual&nbsp;(R$)",
//                '<center>Subação (R$)</center>' => array('Detalhado', 'Não Detalhado'),
                '<center>Empenho (R$)</center>' => array('Empenhado', 'Não Empenhado'),
            ));
        } else {
            $listagem->setCabecalho(array(
                "PTRES",
                "Ação",
                "Unidade Orçamentária",
                "Dotação&nbsp;Atual&nbsp;(R$)",
//                '<center>Subação (R$)</center>' => array('Detalhado', 'Não Detalhado'),
                '<center>PI (R$)</center>' => array('Detalhado', 'Não Detalhado'),
                '<center>Empenho (R$)</center>' => array('Empenhado', 'Não Empenhado')
            ));
        }
        #ver($sql_lista);
        $listagem->setQuery($sql_lista);
        $listagem->setAcoes(array('view' => 'visualizarRegistro'));
        $listagem->addAcao('select', array('func' => 'resultado', 'extra-params' => array('acao')));
        $listagem->setIdLinha();
        $listagem->addCallbackDeCampo(array(
            'dotacaoatual', 'det_subacao', 'nao_det_subacao', 'det_pi', 'nao_det_pi', 'empenhado', 'nao_empenhado', 'saldo'), 'mascaraMoeda');
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
        ?>
        <script type="text/javascript">
            function carregaAcao(prgcod) {
                url = "planacomorc.php?modulo=principal/unidade/listarProgramaUG&acao=A";
                $.ajax({
                    url:url,
                    type: 'POST',
                    async: false,
                    data: {prgcod: prgcod, comboacao:true},
                    success: function(html) {
                        $('.comboAcao').html(html);
                        $(".chosen-select").chosen();
                        $('.comboAcao').find('select').removeAttr('style');
                        $('.comboAcao').find('select').addClass('chosen-select form-control').chosen();
                    },
                    error: function (jqXHR, textStatus, errorThrown) {
                        console.log(jqXHR);
                        console.log(textStatus);
                        console.log(errorThrown);
                    }
                });
            }

            /* CARREGANDO OS DADOS DE PTRES */
            /*var tabelaorigem = document.getElementById('orcamento');
            for (i = 2; i < tabelaorigem.rows.length - 2; i++) {
                if (document.getElementById("chk_" + tabelaorigem.rows[i].cells[0].innerHTML)) {
                    document.getElementById("chk_" + tabelaorigem.rows[i].cells[0].innerHTML).checked = true;
                }
            }*/
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

//                    var celSubacaoDet = tabelaorigem.rows[linha].insertCell(2);
//                    celSubacaoDet.style.textAlign = "right";
//                    var subacaoDet = $('tr#' + id + ' td:nth-child(6)').text();
//                    celSubacaoDet.innerHTML = subacaoDet;
//
//                    var celSubacaoNaoDet = tabelaorigem.rows[linha].insertCell(3);
//                    celSubacaoNaoDet.style.textAlign = "right";
//                    var subacaoNaoDet = $('tr#' + id + ' td:nth-child(7)').text();
//                    celSubacaoNaoDet.innerHTML = subacaoNaoDet;

                    var celEmpenhoEmp = tabelaorigem.rows[linha].insertCell(2);
                    celEmpenhoEmp.style.textAlign = "right";
                    var empenhoEmp = $('tr#' + id + ' td:nth-child(6)').text();
                    celEmpenhoEmp.innerHTML = empenhoEmp;


                    var celEmpenhoNaoEmp = tabelaorigem.rows[linha].insertCell(3);
                    celEmpenhoNaoEmp.style.textAlign = "right";
                    var empenhoNaoEmp = $('tr#' + id + ' td:nth-child(7)').text();
                    celEmpenhoNaoEmp.innerHTML = empenhoNaoEmp;

                    var celEmpenho = tabelaorigem.rows[linha].insertCell(4);
                    celEmpenho.style.textAlign = "right";
                    var empenho = $('tr#' + id + ' td:nth-child(8)').text();
                    celEmpenho.innerHTML = empenho;

                    var celNaoEmpenho = tabelaorigem.rows[linha].insertCell(5);
                    celNaoEmpenho.style.textAlign = "right";
                    var naoEmpenho = $('tr#' + id + ' td:nth-child(9)').text();
                    celNaoEmpenho.innerHTML = naoEmpenho;

                    var celEmpenhoE = tabelaorigem.rows[linha].insertCell(6);
                    celEmpenhoE.style.textAlign = "right";
                    var empenhoE = $('tr#' + id + ' td:nth-child(10)').text();
                    celEmpenhoE.innerHTML = empenhoE + '<input type="hidden" value="'+id+'" name="acaid['+ ptres +']">';

                    var celValorPre = tabelaorigem.rows[linha].insertCell(7);
                    celValorPre.style.textAlign = "right";
                    celValorPre.innerHTML = '<input type="text" class="CampoEstilo normal form-control obrigatorio somar" name="plivalor[' + ptres + '][' + id + ']" size="28" '
                            + 'onKeyUp="this.value=mascaraglobal(\'###.###.###.###,##\',this.value);calculovalorPI()" '
                            + 'onBlur="return verificaDisponivel(this,\'' + id + '\',\'0\')" '
                            + 'data-ptres="' + ptres + '" '
                            + 'onmouseover="MouseOver(this)" onfocus="MouseClick(this);this.select()" '
                            + 'onmouseout="MouseOut(this)" onblur="MouseBlur(this)" style="text-align:right;width:20ex" />';

                } else {
                    var tabelaorigem = document.getElementById('orcamento');

                    tabelaorigem.deleteRow(document.getElementById('ptres_' + id).rowIndex);
                    calculovalorPI();
                    //jogando ids desmarcados para verificar se existem no banco, se sim deletar vinculo.
                    ptresApagar(id);
                }

                function ptresApagar(ptres) {
                    var input = document.createElement("input");
                    input.setAttribute("type", "hidden");
                    input.setAttribute("name", "ids_apagar[]");
                    input.setAttribute("value", ptres);
                    document.getElementById("formulario").appendChild(input);

                }
            }

            function detalhePtres(ptrid) {
                url = 'planacomorc.php?modulo=principal/ptres/detalheptresmodal&acao=A&ptrid=' + ptrid;
                if ($('#obrigatorio').val() == 'n') {
                    url = 'planacomorc.php?modulo=principal/unidade/detalheptresmodal&acao=A&ptrid=' + ptrid;
                }
                jQuery.ajax({
                    url: url,
                    async: false,
                    success: function(html) {
                        jQuery("#dialog").html(html);
                        jQuery("#dialog").dialog({
                            modal: true,
                            width: 900,
                            height: 400
                        });
                    }
                });
            }

            function visualizarRegistro(ptrid) {
                url = 'planacomorc.php?modulo=principal/unidade/popupptres&acao=A&detalhe=ok&ptrid=' + ptrid;
                $.post(url, function(html) {
                    $('#modal-confirm .modal-body p').html(html);
                    $('#modal-confirm .modal-title').html('Detalhamento PTRES');
                    $('#modal-confirm .btn-primary').remove();
                    $('#modal-confirm .btn-default').html('Fechar');
                    $('.modal-dialog').show();
                    $('#modal-confirm').modal();
                });
            }

        </script>
        <div id="dialog" />
    </body>
</html>
<?
/* Fechar Conexão quando não houver rodapé. */
$db->close();
exit();
