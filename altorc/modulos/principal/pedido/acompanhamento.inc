<?php
/**
 * $Id: acompanhamento.inc 75661 2014-02-19 14:51:19Z KamylaSakamoto $
 */
include APPRAIZ . "includes/cabecalho.inc";
//include APPRAIZ . "www/recorc/_funcoes.php";
/* Funções de Gráficos do Cockpit */
include_once APPRAIZ . "www/altorc/_funcoes.php";
include_once '../../pde/www/_funcoes_cockpit.php';
/**
 * Funções de apoio ao pedido de alteração orçamentária.
 * @see _funcoesalteracaoorcamentaria.php
 */
require APPRAIZ . "www/altorc/_funcoesalteracaoorcamentaria.php";
// -- Tratamento de perfis
$perfis = pegaPerfilGeral();
?>
<script type="text/javascript" language="javascript">
    jQuery(document).ready(function () {
        // -- Ação do botão buscar
        $('#mcrid').change(function () {
            window.location = "altorc.php?modulo=principal/pedido/acompanhamento&acao=A"
                    + '&mcrid=' + $('#mcrid').val()
        });
    });
</script>
<style>
    rigth{float:right}
    .text-center{text-align:left !important;}
</style>
<ol class="breadcrumb">
    <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
    <li class="active">Acompanhamento dos Pedidos</li>
</ol>
<section class="well">
    <section class="form-horizontal">
        <section class="form-group">
            <label for="mrdic" class="col-lg-2 control-label">Momento de crédito:</label>
            <section class="col-md-10">
                <?php
                list($mcrid, ) = momentoDeCreditoAtual($_SESSION['exercicio'], $_REQUEST['dados']);
                if (isset($_REQUEST['mcrid']) && $_REQUEST['mcrid'] != '') {
                    $mcrid = $_REQUEST['mcrid'];
                }
                $sqlMomentoCredito = <<<DML
SELECT mcr.mcrid AS codigo,
       mcr.mcrdsc AS descricao
  FROM altorc.momentocredito mcr
  WHERE mcr.mcrstatus = 'A'
    AND mcr.mcrano = '%s'
DML;
                $sqlMomentoCredito = sprintf($sqlMomentoCredito, $_SESSION['exercicio']);
                $db->monta_combo('mcrid', $sqlMomentoCredito, 'S', 'Selecione um Momento de crédito', 'trocaMomentoCredito', null, null, null, 'N', 'mcrid', null, $mcrid, null, 'class="form-control chosen-select" style="width=100%"');
                ?>
            </section>
        </section>
    </section>
</section>
<br>
<section class="col-md-7">
    <?php
    $em_preenchimento = STDOC_EM_PREENCHIMENTO;
    $em_analise_spo = STDOC_ANALISE_SPO;
    $ajuste_uo = STDOC_AJUSTE_UO;
    $cadastrar_siop = STDOC_CADASTRAR_SIOP;
// -- Query de listagem de acompanhamentos
    $sql = <<<DML
SELECT
    unidsc                AS unidade,
    SUM(em_preenchimento) AS em_preenchimento,
    SUM(em_analise_spo)   AS em_analise_spo,
    SUM(ajuste_uo)        AS ajuste_uo,
    SUM(cadastrar_siop)   AS cadastrar_siop
FROM
    (
        SELECT
            unidsc,
            COALESCE(
                CASE
                    WHEN esdid = 1092
                    THEN SUM(valor)
                END, 0) AS em_preenchimento,
            COALESCE(
                CASE
                    WHEN esdid = 1093
                    THEN SUM(valor)
                END, 0) AS em_analise_spo,
            COALESCE(
                CASE
                    WHEN esdid = 1094
                    THEN SUM(valor)
                END, 0) AS ajuste_uo,
            COALESCE(
                CASE
                    WHEN esdid = 1095
                    THEN SUM(valor)
                END, 0) AS cadastrar_siop
        FROM
            (
                SELECT
                    uni.unicod || ' - ' || uni.unidsc AS unidsc,
                    esd.esdid,
                    COUNT(DISTINCT pao.tcrid) AS valor
                FROM
                    altorc.pedidoalteracaoorcamentaria pao
                INNER JOIN
                    altorc.tipocredito tcr
                ON
                    (
                        pao.tcrid = tcr.tcrid
                    AND tcr.tcrano = '{$_SESSION['exercicio']}')
                INNER JOIN
                    public.unidade uni
                USING
                    (unicod)
                LEFT JOIN
                    altorc.pedidoalteracaofinanceiro pas
                USING
                    (paoid)
                LEFT JOIN
                    altorc.requisicoespedido rqp
                USING
                    (rqpid)
                LEFT JOIN
                    workflow.documento doc
                USING
                    (docid)
                LEFT JOIN
                    workflow.estadodocumento esd
                USING
                    (esdid)
                LEFT JOIN
                    altorc.momentocredito mcr
                USING
                    (mcrid)
                WHERE
                    pao.paostatus = 'A'
                    AND pao.mcrid = {$mcrid}
                GROUP BY
                    uni.unicod,
                    uni.unidsc ,
                    esd.esdid ) foo
        GROUP BY
            foo.unidsc,
            foo.esdid )foo2
GROUP BY
    foo2.unidsc
ORDER BY
    foo2.unidsc
DML;
    /* Caso não venha o momento poe o SQL como falso */
    if (!isset($mcrid) || $mcrid == '') {
        $sql = "SELECT 'Momento de crédito inválido.'";
    }
    $listagem = new Simec_Listagem();
    $listagem->setCabecalho(array(
        //    "Alterar/Excluir",
        'Unidade Orçamentária', 'Em Preenchimento', 'Análise SPO', 'Ajustes UO', 'Confirmado no SIOP'));
    $listagem->setQuery($sql);
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    ?>
    <?php if (false === $listagem->render()): ?>
        <div class="alert alert-info col-md-4 col-md-offset-4 text-center">
            Nenhum registro encontrado
        </div>
    <?php endif; ?>
</section>
<section class="col-md-5">
    <section class="panel panel-primary">
        <section class="panel-heading"><strong>GRÁFICO</strong></section>
        <section class="panel-body" style="background-color: royalblue;">
            <?php
            $sql = "
                SELECT
                    esd.esddsc AS descricao,
                    COUNT(0)   AS valor
                FROM
                     altorc.pedidoalteracaoorcamentaria pao
                LEFT JOIN
                    workflow.documento doc
                ON
                    doc.docid = pao.docid
                LEFT JOIN
                    workflow.estadodocumento esd
                ON
                    esd.esdid = doc.esdid
                WHERE
                    pao.paostatus = 'A'
                    AND pao.mcrid = {$mcrid}
                GROUP BY
                    esd.esddsc
                    ";
            /* Caso não venha o momento poe o SQL como falso */
            if (empty($mcrid)) {
                $sql = "SELECT 'Momento de crédito inválido.'";
            }
            $dados = $db->carregar($sql);
            if ($dados) {
                echo geraGrafico($dados, $nomeUnico = 'pedidos', $titulo = 'Status dos Pedidos de Alteração', $formatoDica = "", $formatoValores = "{point.y} ({point.percentage:.2f} %)", $nomeSerie = "", $mostrapopudetalhes = false, $caminhopopupdetalhes = "", $largurapopupdetalhes = "", $alturapopupdetalhes = "", $mostrarLegenda = true, $aLegendaConfig = false, $legendaClique = false);
            }
            ?>
        </section>
    </section>
</section>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>
<link rel="stylesheet" href="css/progorc.css">
