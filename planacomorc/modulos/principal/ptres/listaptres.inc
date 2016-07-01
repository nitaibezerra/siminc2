<?php
if ($_REQUEST['ptrid']) {
    echo popup($_REQUEST['ptrid']);
    die();
}
require_once (APPRAIZ . 'includes/library/simec/Listagem.php');
include APPRAIZ . 'includes/cabecalho.inc';

function popup($ptrid) {
    global $db;   
    require_once APPRAIZ . "includes/library/simec/Grafico.php";
            
    if (isset($_REQUEST['ptrid'])) {
        $ptres = $db->pegaUm("SELECT ptres FROM monitora.ptres WHERE ptrid = {$_REQUEST['ptrid']}");
        $sql = <<<DML
            SELECT
                dtl.ptres,
                trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acadsc) AS descricao,
                uni.unicod,
                uni.unidsc,
                dtl.plocod,
                plo.plotitulo,
                COALESCE(dtl.ptrdotacao +0.00, 0.00)                            AS dotacaoatual,
                COALESCE(dt.valor, 0.00)                                       AS detalhadosubacao,
                COALESCE(pemp.total, 0.00)                                        AS empenhado,
                COALESCE(dtl.ptrdotacao - COALESCE(dt.valor, 0.00), 0.00) AS naodetalhadosubacao,
                COALESCE(dtl.ptrdotacao - COALESCE(pemp.total, 0.00), 0.00) AS naoempenhado
            FROM monitora.acao aca
            INNER JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid
            INNER JOIN public.unidade uni ON uni.unicod = dtl.unicod
            LEFT JOIN (SELECT ptrid, SUM(sadvalor) AS valor FROM monitora.pi_subacaodotacao GROUP BY ptrid) dt ON dtl.ptrid = dt.ptrid
            LEFT JOIN siafi.ptrempenho pemp ON ( pemp.ptres = dtl.ptres AND pemp.exercicio = aca.prgano)
            LEFT JOIN monitora.planoorcamentario plo ON plo.prgcod = aca.prgcod AND plo.acacod = aca.acacod AND plo.unicod = aca.unicod AND dtl.plocod = plo.plocodigo AND plo.exercicio = aca.prgano
            WHERE aca.prgano ='{$_SESSION['exercicio']}'
                AND dtl.ptrano='{$_SESSION['exercicio']}'
                AND dtl.ptres = '{$ptres}'
                AND plo.exercicio = '{$_SESSION['exercicio']}'
                AND aca.acasnrap = FALSE
            
DML;
#ver($sql);
        $dadosPtres = $db->carregar($sql);
        $dadosPtres = $dadosPtres[0];
        if (!isset($dadosPtres['dotacaoatual']) || $dadosPtres['dotacaoatual'] == '') {
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
            FROM (
                SELECT
                    SUM(vlrempenhado) AS vlrempenhado
                FROM spo.siopexecucao sex
                WHERE sex.exercicio = '{$_SESSION['exercicio']}'
                    AND ptres IN ('{$ptres}')
                GROUP BY ptres ) foo
            UNION ALL
            SELECT
                'Liquidado'  AS descricao,
                'Total' as categoria,
                vlrliquidado AS valor
            FROM(
                SELECT
                    SUM(vlrliquidado) AS vlrliquidado
                FROM spo.siopexecucao sex
                WHERE sex.exercicio = '{$_SESSION['exercicio']}'
                    AND ptres IN ('{$ptres}')
                GROUP BY ptres ) foo
            UNION ALL
            SELECT
                'Pago'  AS descricao,
                'Total' as categoria,
                vlrpago AS valor
            FROM (
                SELECT
                    SUM(vlrpago) AS vlrpago
                FROM spo.siopexecucao sex
                WHERE sex.exercicio = '{$_SESSION['exercicio']}'
                    AND ptres IN ('{$ptres}')
                GROUP BY ptres ) foo
DML;
        
    }
    $grafico = new Grafico(Grafico::K_TIPO_COLUNA, false);
    $grafico->setTipo(Grafico::K_TIPO_COLUNA)->setId('graficoPtres')->setTitulo('Execução Orçamentária')->gerarGrafico($sqlGrafico);
    $cor = ($dadosPtres['naodetalhadosubacao'] >= 0) ? 'black' : 'red';
    $cor2 = ($dadosPtres['naoempenhado'] >= 0) ? 'black' : 'red';
    $dotacaoAtual = number_format2($dadosPtres['dotacaoatual']);
    $corDetalheSubacao = ($dadosPtres['detalhadosubacao'] >= 0) ? 'black' : 'red';
    $detalheSubacao = number_format2($dadosPtres['detalhadosubacao']);
    $naoDetalhadoSubacao = number_format2($dadosPtres['naodetalhadosubacao']);
    $empenhado = number_format2($dadosPtres['empenhado']);
    $naoEmpenhado = number_format2($dadosPtres['naoempenhado']);
    
    
    $html = <<<HTML
    <style>.modal-dialog{width:80%;};</style>
        
    <div style="width: 40%; float: left;">
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
                <td style="color:{$corDetalheSubacao}">
                    <b><span id="saldo_pi_detalhado_ptres">{$detalheSubacao}</span></b>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="200px">Não Detalhado em Subação:</td>
                <td style="color:$cor">
                    <b><span id="saldo_nao_detalhado_subacao">{$naoDetalhadoSubacao}</span></b>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="200px">Empenhado do PTRES:</td>
                <td>
                    <b><span id="saldo_nao_orcado">{$empenhado}</span></b>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="200px">Não Empenhado:</td>
                <td style="color:{$cor2}">
                    <b><span id="saldo_nao_empenhado">{$naoEmpenhado}</span></b>
                </td>
            </tr>
        </table>
    </div>
    <div id="graficoDestino" style="width: 60%; float: left;">
    </div>
    <div style="clear: both;"/>
    <table class="table table-striped table-bordered table-hover">
        <tr>
            <td>
                <div style="background-color:#00ced1; color:#FFF; margin-top: 10px; padding: 5px;">
                    <b>Subações  {$_SESSION['exercicio']}</b>
                </div>
HTML;

    $select = <<<SELECT
        SELECT  
            foo.codigo AS sbacod,
            foo.sbatitulo,
            COALESCE(foo.dotacao, 0.00) AS dotacao,
            COALESCE((SELECT sadvalor FROM monitora.pi_subacaodotacao WHERE sbaid = foo.sbaid AND ptrid = {$ptrid} ),0.00)  as dotacao_no_ptres,
            COALESCE(( SELECT SUM(total) FROM siafi.pliptrempenho WHERE exercicio = '{$_SESSION['exercicio']}' AND SUBSTR(plicod, 2,4) = foo.sbacod 
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
        )
    );
    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO, Simec_Listagem::RETORNO_BUFFERIZADO);
    $listagem->setCabecalho(array(
        "Código",
        "Subação",
        "Orçamento total da Subação (R$)",
        "Orçamento da Subação neste PTRES (R$)",
        "Empenhado nesta Subação e neste PTRES(R$)"
    ));
    $listagem->setQuery($sqlSubacao);
    $listagem->addCallbackDeCampo(array(
        'dotacao', 'dotacao_no_ptres', 'detalhado_pi_no_ptres', 'empenhado_no_ptres', 'nao_detalhado_pi_no_ptres'), 'mascaraMoeda');    
    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('dotacao', 'dotacao_no_ptres', 'detalhado_pi_no_ptres', 'empenhado_no_ptres', 'nao_detalhado_pi_no_ptres'));
    $retorno = $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);    
    $html .= $retorno;
    
    $html .= <<<HTML
            </td>
        </tr>
        <tr>
            <td>
                <div style="background-color:#FF6347; color:#FFF; margin-top: 10px; padding: 5px;">
                    <b>Plano Interno {$_SESSION['exercicio']}</b>
                </div>
HTML;

    $params['SELECT'] = <<<SQL
        SELECT 
            gmb.codigo,
            gmb.titulo,
            COALESCE((SELECT total FROM siafi.pliptrempenho ppe WHERE plicod = gmb.codigo AND ptres = '{$dadosPtres['ptres']}' 
                AND exercicio = '{$_SESSION['exercicio']}'),0.00) as empenhado_pi_ptres
SQL;

    /* Variáveis da Consulta */
    $params['v_ptrid'] = $ptrid;
    /* Filtros da Consulta */
    $params['where'] = " AND pip.ptrid= {$ptrid} ";
    $params['filtroNoUnionSiafi'] = " AND ptr.ptrid = {$ptrid} ";
    $sql = retornaConsultaPI($params);
    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO, Simec_Listagem::RETORNO_BUFFERIZADO);
    $listagem->setCabecalho(array(
        "PI's subtraindo recursos<br />do PTRES",
        "Título do PI",
        "Empenhado do PI no PTRES (R$)"
    ));
    $listagem->setQuery($sql);
    $listagem->addCallbackDeCampo(array('empenhado_pi_ptres'), 'mascaraMoeda');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA,array('empenhado_pi_ptres'));
    $retornoPi = $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);    
    $html .= $retornoPi;    
    $html .= <<<HTML
            </td>
        </tr>    
    </table>
HTML;
    /* Gato do Grafico */
    $html.= <<<JAVASCRIPT
        <script>$("#graficoPtres").appendTo("#graficoDestino");</script>
JAVASCRIPT;
    return $html;
}

/* Unidades Obrigatórias (AD, CAPES, INEP, FNDE, FIES, SUP.MEC, EBSERH */
$obrigatorias = UNIDADES_OBRIGATORIAS;

$atiid = $_POST[atiid] ? $_POST[atiid] : $_GET[atiid];
$prgcod = $_POST['prgcod'];
$acacod = $_POST['acacod'];
$sbaid = $_REQUEST['sbaid'];
$prgano = $_SESSION['exercicio'];

if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}
//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
?>


<script>
    $(document).ready(function() {
        pesquisaRapida('textFind');
        $('#buscar').click(function() {
            var uri = "planacomorc.php?modulo=principal/ptres/listaptres&acao=A"
                + '&prgcod=' + $('#prgcod').val()
                + '&acacod=' + $('#acacod').val()
                + '&ptres=' + $('#ptres').val();

            window.location = uri;
        });
        $('#limpar').click(function() {
            $('#prgcod_chosen span').text('Selecione');
            $('#acacod_chosen span').text('Selecione');
            $('#prgcod').val('');
            $('#acacod').val('');
        });
    });
    function pesquisaRapida(id) {
        jQuery.expr[':'].contains = function(a, i, m) {
            return jQuery(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
        };

        $("#" + id).keyup(function()
        {
            $('table.table tbody tr td').removeClass('marcado');
            $('table.table tbody tr').removeClass('remover');
            stringPesquisa = $("#" + id).val();
            if (stringPesquisa) {
                $('table.table tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
                $('table.table tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
            }
        });
    }

    function detalhePtres(ptrid) {
        divCarregando();
        $.post("planacomorc.php?modulo=principal/ptres/listaptres&acao=A&ptrid=" + ptrid, function(html) {
            $('#modal-confirm .modal-body p').html(html);
            $('#modal-confirm .modal-title').html('Dados do PTRES');
            $('#modal-confirm .btn-primary').remove();
            $('#modal-confirm .btn-default').html('OK');
            $('.modal-dialog').show();
            $('#modal-confirm').modal();
            divCarregado();
        });
    }
</script>
<div id="dialog" title="Dados do PTRES"></div>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li>PTRES  - <?php echo $_SESSION['exercicio']; ?></li>
        <li class="active">Lista de PTRES</li>
    </ol>
</div>
<div class="col-lg-12">    
    <div class="well">
        <fieldset>
            <form id="formulario" name="formulario" method="POST"
                  class="form-horizontal">
                <input type="hidden" name="vieid" id="vieid"
                       value="<?php echo $vieid ?>" />
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
                        ?> 
                    </div>
                </div>
                <div class="form-group">
                    <label for="ptres" class="col-lg-2 control-label">PTRES:</label>
                    <div class="col-lg-10">
                        <?php                        
                        inputTexto('ptres', isset($_REQUEST['ptres']) ? $_REQUEST['ptres'] : '', 'ptres', 6,false);
                        ?> 
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-2">
                        <button class="btn btn-warning" id="limpar" >Limpar</button>
                        <button class="btn btn-primary" id="buscar" type="button">Buscar</button>
                    </div>
                </div>
            </form>
        </fieldset>
    </div>
</div>
<?php
$where .= $_REQUEST['sbaid'] ? "AND dt.sbaid = '" . $_REQUEST['sbaid'] . "'" : '';
$where .= $_REQUEST['prgcod'] ? "AND UPPER(dtl.prgcod) LIKE('%" . strtoupper($_REQUEST['prgcod']) . "%')" : '';
$where .= $_REQUEST['acacod'] ? "AND UPPER(dtl.acacod) LIKE('%" . strtoupper($_REQUEST['acacod']) . "%')" : '';
$where .= $_REQUEST['ptres'] ? "AND UPPER(dtl.ptres) LIKE('%" . strtoupper($_REQUEST['ptres']) . "%')" : '';
$buscaLivreTmp = removeAcentos(str_replace("-", " ", $_REQUEST['buscalivre']));
$where .= $_REQUEST['buscalivre'] ? "AND (trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||public.removeacento(aca.acadsc)) ilike('%" . $buscaLivreTmp . "%') OR public.removeacento(dtl.ptres) ilike '%" . $buscaLivreTmp . "%')" : '';

$obrigatorias = UNIDADES_OBRIGATORIAS;
$whereObrigatorio = 'AND aca.unicod IN(' . $obrigatorias . ')';
$whereUO = '';

$sql_lista = <<<DML
    SELECT 
        dtl.ptrid as codigo,
        dtl.ptres,
        trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acatitulo) AS descricao,
        uni.unicod || ' - ' || uni.unidsc as unidade,
        COALESCE(SUM(dtl.ptrdotacao)+0.00, 0.00) AS dotacaoinicial,
        COALESCE(SUM(dt.valor), 0.00) AS det_subacao,
        COALESCE((pemp.total), 0.00) AS empenhado,
        COALESCE(SUM(dtl.ptrdotacao) - COALESCE(SUM(dt.valor), 0.00), 0.00) AS saldo         
    FROM monitora.acao aca
    INNER JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid
    INNER JOIN public.unidade uni ON uni.unicod = dtl.unicod    
    LEFT JOIN (
        SELECT ex.ptres, sum(ex.vlrempenhado) AS total
        FROM spo.siopexecucao ex
        WHERE ex.unicod IN('26101','26291', '26290', '26298', '26443', '74902', '73107')
            AND ex.exercicio = '{$_SESSION['exercicio']}'
        GROUP BY ex.ptres
	) pemp ON pemp.ptres = dtl.ptres
    LEFT JOIN (SELECT ptrid,
        SUM(sadvalor) AS valor
        FROM monitora.pi_subacaodotacao
        GROUP BY ptrid) dt ON dtl.ptrid = dt.ptrid
    WHERE aca.prgano='{$_SESSION['exercicio']}'
        AND dtl.ptrano='{$_SESSION['exercicio']}'  
        {$where}
        AND ptrstatus = 'A'  
        AND aca.acasnrap = FALSE
        {$whereObrigatorio}
        {$whereUO}
    GROUP BY dtl.ptrid,dtl.ptres,descricao,uni.unidsc, pemp.total, uni.unicod 
    ORDER BY 1
DML;
#$sql_lista = retornaConsultaPTRES($params, 'NOVO');
#ver($sql_lista);
?>
    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <div class="col-md-12">
<?php
$listagem = new Simec_Listagem();
$listagem->setCabecalho(array(
    "PTRES",
    "Ação",
    "Unidade Orçamentária",
    "Dotação Atual(R$)",
    "Detalhado em Subação (R$)",
    "Empenhado (R$)",
    "Não Detalhado em Subação (R$)"));
$listagem->setQuery($sql_lista);
$listagem->addCallbackDeCampo(array(
    'dotacaoinicial', 'det_subacao', 'det_pi', 'empenhado', 'saldo'), 'mascaraMoeda');
$listagem->setAcoes(array('view' => 'detalhePtres'));
$listagem->addCallbackDeCampo(array('descricao', 'unidade' ), 'alinhaParaEsquerda');
$listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
$listagem->turnOnPesquisator();
$listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

?>
    </div>
</html>
