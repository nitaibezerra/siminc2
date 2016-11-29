<?php
/**
 * $Id: acompanhamento.inc 102347 2015-09-11 14:15:27Z maykelbraz $
 */

/* Variaveis da tela */
if (isset($_REQUEST['prfid']) && $_REQUEST['prfid'] != '') {
    $prfid = $_REQUEST['prfid'];
} else {
    $prfid = recuperaPeriodo('atual');
}
include APPRAIZ . "includes/cabecalho.inc";
?>
<script language="JavaScript" src="../includes/funcoesspo.js"></script>
<script type="text/javascript" language="javascript">
    jQuery(document).ready(function () {
        // -- Ação do botão buscar
        $('#prfid').change(function () {
            window.location = "recorc.php?modulo=principal/captacao/acompanhamento&acao=A"
                    + '&prfid=' + $('#prfid').val()
        });
    });

    function listarCaptacao(unicod) {
        window.location = 'recorc.php?modulo=principal/captacao/listar&acao=A&unicod=' + unicod;
    }
</script>
<style>
    rigth{float:right}
    .text-center{text-align:left !important;}
</style>
<ol class="breadcrumb">
    <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
    <li class="active">Alteração de Receita</li>
    <li class="active">Acompanhamento da Alteração de Previsão <?php echo $_SESSION['exercicio']; ?></li>
</ol>
<section class="well">
    <section class="form-horizontal">
        <section class="form-group">
            <label for="prfid" class="col-lg-2 control-label">Período</label>
            <section class="col-md-10">
                <?php
                $sql = "
                    SELECT
                       prfid AS codigo,
                       prfdsc AS descricao
                    FROM
                       recorc.periodoreferencia
                    WHERE
                        exercicio = '{$_SESSION['exercicio']}'
                        AND prfstatus = 'A'
                    ORDER BY
                       prfid DESC";

                $db->monta_combo(
                        'prfid', $sql, 'S', 'Selecione um período', null, null, null, null, 'N', 'prfid', null, '', null, 'class="form-control chosen-select" style="width=100%;" required="required"'
                );
                ?>
            </section>
        </section>
    </section>
</section>
<br>
    <?php
    /* Filtros (Para as 2 SQL) */
    if (isset($prfid) && $prfid != '') {
        $wherePeriodo = " AND vie.prfid = {$prfid}";
    }
    $perfis = pegaPerfilGeral($_SESSION['usucpf$perfi']);

    if (in_array(PFL_SUPER_USUARIO, $perfis) || in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis)) {
        echo '<section class="col-md-7">';
        $sql = <<<DML
-- SQL SU / CGO
SELECT
    vie.unicod                                 AS acao,
    ''||vie.unicod || ' - ' || uni.unidsc|| '' AS unidade,
    (
        SELECT
            SUM(sex.vlrdotacaoatual)
        FROM
            spo.siopexecucao sex
        WHERE
            sex.unicod = vie.unicod
        AND sex.exercicio = '2014' ) AS vlrloa,
    COALESCE(SUM(prr.valor),0)       AS valor_sof,
    COALESCE(SUM(prr.valor),0)       AS capvaloruo
FROM
    recorc.vinculacaoexercicio vie
LEFT JOIN
    recorc.previsaoreceita prr
ON
    (
        prr.exercicio = vie.exercicio
    AND prr.prfid = vie.prfid
    AND prr.unicod = vie.unicod
    AND prr.foncod = vie.foncod
    AND prr.nrccod = vie.nrccod)
LEFT JOIN
    recorc.captacao cap
ON
    (
        cap.exercicio = vie.exercicio
    AND cap.prfid = vie.prfid
    AND cap.unicod = vie.unicod
    AND cap.foncod = vie.foncod
    AND cap.nrccod = vie.nrccod)
    LEFT JOIN spo.siopexecucao sex ON (sex.unicod = cap.unicod AND sex.exercicio = '2014')
INNER JOIN
    public.unidade uni
ON
    vie.unicod = uni.unicod
WHERE
    vie.exercicio = '{$_SESSION['exercicio']}' {$wherePeriodo}
GROUP BY
    vie.unicod,
    uni.unidsc
ORDER BY
    vie.unicod
DML;

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $listagem->setCabecalho(array(
            'Unidade',
            'Total da LOA R$',
            'Total da SOF R$',
            'Total da UO R$'));
        $listagem->setQuery($sql);
        $listagem->setAcoes(array(
            'view' => 'listarCaptacao'
        ));
        $listagem->addCallbackDeCampo(array(
            'vlrloa', 'valor_sof', 'capvaloruo'), 'mascaraMoeda');
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        if (false === $listagem->render()):
            ?>
            <div class="alert alert-info col-md-4 col-md-offset-4 text-center">
                Nenhum registro encontrado
            </div>
            <?php
        endif;
        echo "</section>";
    }
    
    if (in_array(PFL_UO_EQUIPE_TECNICA, $perfis)) {
        /* Tratamento de perfis */

        $where[] = $whereUO = ' uni.unicod IN(' . pegaResposabilidade($_SESSION['usucpf'], PFL_UO_EQUIPE_TECNICA, 'unicod') . ')';
        $whereUO = " AND {$whereUO}";


        $sql = <<<DML
SELECT
    vie.unicod                                 AS acao,
    ''||vie.unicod || ' - ' || uni.unidsc|| '' AS unidade,
    (
        SELECT
            SUM(sex.vlrdotacaoatual)
        FROM
            spo.siopexecucao sex
        WHERE
            sex.unicod = vie.unicod
        AND sex.exercicio = '2014' ) AS vlrloa,
    COALESCE(SUM(prr.valor),0)       AS valor_sof,
    COALESCE(SUM(prr.valor),0)       AS capvaloruo
FROM
    recorc.vinculacaoexercicio vie
LEFT JOIN
    recorc.previsaoreceita prr
ON
    (
        prr.exercicio = vie.exercicio
    AND prr.prfid = vie.prfid
    AND prr.unicod = vie.unicod
    AND prr.foncod = vie.foncod
    AND prr.nrccod = vie.nrccod)
LEFT JOIN
    recorc.captacao cap
ON
    (
        cap.exercicio = vie.exercicio
    AND cap.prfid = vie.prfid
    AND cap.unicod = vie.unicod
    AND cap.foncod = vie.foncod
    AND cap.nrccod = vie.nrccod)
    LEFT JOIN spo.siopexecucao sex ON (sex.unicod = cap.unicod AND sex.exercicio = '2014')
INNER JOIN
    public.unidade uni
ON
    vie.unicod = uni.unicod
WHERE
    vie.exercicio = '{$_SESSION['exercicio']}' {$wherePeriodo}{$whereUO}
GROUP BY
    vie.unicod,
    uni.unidsc
ORDER BY
    vie.unicod
DML;
//echo "<pre>"; var_dump($sql);die;
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        //Status
        $listagem->setCabecalho(array(
            'Unidade',
            'Total da LOA R$',
            'Total da SOF R$',
            'Total da UO R$'));
        $listagem->setQuery($sql);
        $listagem->setAcoes(array(
            'view' => 'listarCaptacao'
        ));
        $listagem->addCallbackDeCampo(array(
            'vlrloa', 'valor_sof', 'capvaloruo'), 'mascaraMoeda');
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);



//                if (false === $listagem->render()):
//
        ?>
<!--        <div class="alert alert-info col-md-4 col-md-offset-4 text-center">
            Nenhum registro encontrado
        </div>-->
        <?php
//                endif;
    }
?>
<section class="col-md-5">
    <section class="panel panel-primary">
        <section class="panel-heading"><strong>GRÁFICO</strong></section>
        <section class="panel-body" >
            <?php
            $sql = "
                    SELECT
                        COALESCE(esd.esddsc, 'Não iniciado') AS descricao,
                        COUNT(0) as valor
                    FROM
                        recorc.vinculacaoexercicio vie
                    LEFT JOIN
                        recorc.captacao cap
                    ON
                        vie.unicod = cap.unicod
                    AND vie.foncod = cap.foncod
                    AND vie.nrccod = cap.nrccod
                    AND vie.exercicio = cap.exercicio
                    AND vie.prfid = cap.prfid
                    {$wherePeriodo}
                    LEFT JOIN
                        workflow.documento doc
                    ON
                        doc.docid = cap.docid
                        LEFT JOIN
                        public.unidade uni
                   ON
                       vie.unicod = uni.unicod
                    LEFT JOIN
                        workflow.estadodocumento esd
                    ON
                        esd.esdid = doc.esdid
                    WHERE
                        vie.exercicio = '{$_SESSION['exercicio']}'
                        AND vie.prfid = {$prfid}
{$whereUO}
                    GROUP BY
                        esd.esddsc ";

            $grafico = new Grafico(null, false);
            $grafico->setTipo(Grafico::K_TIPO_PIZZA)->setId('graficoAcao')->setTitulo('Acompanhamento da Captação')->gerarGrafico($sql);
            ?>
        </section>
    </section>
</section>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>
<link rel="stylesheet" href="css/recorc.css">
</div>