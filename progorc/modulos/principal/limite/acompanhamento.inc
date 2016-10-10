<?php
/**
 * Arquivo de exibição do acompanhamento dos pedidos de alteração de limite.
 *
 * $Id: acompanhamento.inc 75661 2014-02-19 14:51:19Z KamylaSakamoto $
 */
if ($_REQUEST['status'] != '') {

    $sql = "SELECT
    ccr.anxcod,
    esd.esdid,
        SUM(pde.pddvaloramplicacao) as ampliacao,

    SUM(pde.pddvalorreducao) as reducao,

    SUM(pde.pddvaloramplicacaoatendido) as ampliacao_atendido,

    SUM(pde.pddvalorreducaoatendido) as reducao_atendido

FROM
    progorc.pedidolimite pli
INNER JOIN
    progorc.pedidodetalhe pde
ON
    pde.pdlid = pli.pdlid
INNER JOIN
    progorc.contacorrente ccr
ON
    ccr.ccrid = pde.ccrid
INNER JOIN
    progorc.anexo ane
ON
    ane.anxcod = ccr.anxcod
LEFT JOIN
    workflow.documento doc
ON
    doc.docid = pli.docid
LEFT JOIN
    workflow.estadodocumento esd
ON
    esd.esdid = doc.esdid
WHERE
    EXTRACT(YEAR FROM pli.pdldatapedido) = 2014
AND ccr.anxcod = '{$_REQUEST['anexo']}'
AND esd.esdid = '{$_REQUEST['status']}'
GROUP BY
    ccr.anxcod,
    esd.esdid";

    $pedidostatus = $db->pegaLinha($sql);

    if ($pedidostatus) {

        echo simec_json_encode($pedidostatus);
    }
    die();
}


include APPRAIZ . "includes/cabecalho.inc";
include APPRAIZ . "www/recorc/_funcoes.php";

/* Funções de Gráficos do Cockpit */
include_once '../../pde/www/_funcoes_cockpit.php';
// -- Tratamento de perfis
$perfis = pegaPerfilGeral();

if (in_array(PFL_UO_EQUIPE_TECNICA, $perfis)) {
    $sqlUO = <<<DML
EXISTS (SELECT 1
         FROM progorc.usuarioresponsabilidade rpu
         WHERE rpu.usucpf = '%s'
           AND rpu.pflcod = %d
           AND rpu.rpustatus = 'A'
           AND rpu.unicod  = pli.unicod)
DML;
    $where[] = $whereUO = sprintf($sqlUO, $_SESSION['usucpf'], PFL_UO_EQUIPE_TECNICA);
    $whereUO = " AND {$whereUO}";
}
?>
<script type="text/javascript" language="javascript">
    jQuery(document).ready(function() {
        // -- Ação do botão buscar
        $('#unicod').change(function() {
            window.location = "progorc.php?modulo=principal/limite/acompanhamento&acao=A"
                    + '&unicod=' + $('#unicod').val()
        });
    });
    function listaAnexo(anexo){
        window.location = 'progorc.php?modulo=principal/limite/listar&acao=A&anexo='+anexo;
    }
    function teste(status, anexo, qnt) {
        if(qnt == 0){
            return false;
        }
        $.ajax({
            type: 'POST',
            url: window.location,
            dataType: 'json',
            data: {
                'status': status,
                'anexo': anexo
            },
            success: function(response) {

                $('#modal-info-status .has-error').removeClass('has-error');
                $('#modal-info-status #ampliacao').text(number_format(response.ampliacao, 2, ',', '.'));
                $('#modal-info-status #reducao').text(number_format(response.reducao, 2, ',', '.'));
                $('#modal-info-status #ampliacao_atendido').text(number_format(response.ampliacao_atendido, 2, ',', '.'));
                $('#modal-info-status #reducao_atendido').text(number_format(response.reducao_atendido, 2, ',', '.'));

                if (status == '1066') {
                    $('#modal-info-status #status').text('Não Enviado');
                }
                if (status == '1067') {
                    $('#modal-info-status #status').text('Análise SPO');
                }
                if (status == '1072') {
                    $('#modal-info-status #status').text('Acertos SPO');
                }
                if (status == '1068') {
                    $('#modal-info-status #status').text('Análise Secretaria');
                }
                if (status == '1069') {
                    $('#modal-info-status #status').text('Acertos Secretaria');
                }
                if (status == '1070') {
                    $('#modal-info-status #status').text('Atendido');
                }
                if (status == '1071') {
                    $('#modal-info-status #status').text('Recusado');
                }
                $('#modal-info-status #anexo').text('Anexo ' + anexo);
                $('#modal-info-status #qnt').text(qnt);
                    $('#modal-info-status').modal();
                }
            });
        }
</script>
<style>
    rigth{float:right}
    .text-center{text-align:left !important;}
</style>
<div class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li>Pedidos de Limites</li>
        <li class="active">Acompanhamento dos Pedidos de Limite <?php echo $_SESSION['exercicio']; ?></li>
    </ol>
    <section class="well">
    <?php if (in_array(PFL_SUPER_USUARIO, $perfis) || in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis)) { ?>
        <section class="form-horizontal">
            <section class="form-group">
                <label for="inputUnidade" class="col-lg-2 control-label">Unidade Orçamentária (UO)</label>
                <section class="col-lg-10">
                    <?php
                    $sql = "
                                SELECT
uni.unicod AS codigo,
uni.unicod || ' - ' || unidsc AS descricao
FROM
public.unidade uni
WHERE
    (
        uni.orgcod = '". CODIGO_ORGAO_SISTEMA. "'
    OR  uni.unicod IN('74902',
                      '73107'))
AND uni.unistatus = 'A'
ORDER BY uni.unicod
";
                    $unicod = $_REQUEST['unicod'];
                    $db->monta_combo('unicod', $sql, 'S', 'Selecione uma Unidade', null, null, null, null, 'N', 'unicod', null, '', null, 'class="form-control chosen-select" style="width=100%;""', null, (isset($unicod) ? $unicod : null));

                    /* Incrementa o filtro por UOs */
                    if ($unicod) {
                        $whereUO .= " AND pli.unicod = '{$unicod}'";
                    }
                    ?>
                </section>
            </section>
        </section>
    </section>

<?php } ?>
<br>
    <section class="col-md-7">
    <?php
    $nao_enviado = STDOC_NAO_ENVIADO;
    $acerto_spo = STDOC_ACERTOS_SPO;
    $em_analise_spo = STDOC_ANALISE_SPO;
    $em_analise_secretaria = STDOC_ANALISE_SECRETARIA;
    $acertos_secretaria = STDOC_ACERTOS_SECRETARIA;
    $atendido = STDOC_ATENDIDO;
    $recusado = STDOC_RECUSADO;
// -- Query de listagem de acompanhamentos
    $sql = <<<DML
                        SELECT
                        anxcod as acao,
'Anexo ' || anxcod  as anexo,
'<a href="#" onclick="teste(\'1066\',\''||anxcod||'\',\''||SUM(nao_enviado)||'\')">'||SUM(nao_enviado)||'</a>' as nao_enviado,
'<a href="#" onclick="teste(\'1067\',\''||anxcod||'\',\''||SUM(em_analise_spo)||'\')">'||SUM(em_analise_spo)||'</a>' as em_analise_spo,
'<a href="#" onclick="teste(\'1072\',\''||anxcod||'\',\''||SUM(acerto_spo)||'\')">'||SUM(acerto_spo)||'</a>' as acerto_spo,
'<a href="#" onclick="teste(\'1068\',\''||anxcod||'\',\''||SUM(em_analise_secretaria)||'\')">'||SUM(em_analise_secretaria)||'</a>' as em_analise_secretaria,
'<a href="#" onclick="teste(\'1069\',\''||anxcod||'\',\''||SUM(acertos_secretaria)||'\')">'||SUM(acertos_secretaria)||'</a>' as acertos_secretaria,
'<a href="#" onclick="teste(\'1070\',\''||anxcod||'\',\''||SUM(atendido)||'\')">'||SUM(atendido)||'</a>' as atendido,
'<a href="#" onclick="teste(\'1071\',\''||anxcod||'\',\''||SUM(recusado)||'\')">'||SUM(recusado)||'</a>' as recusado
FROM
(
SELECT
    anxcod,
    COALESCE(
        CASE
            WHEN esdid = {$nao_enviado}
            THEN SUM(valor)
        END, 0) AS nao_enviado,

         COALESCE(
        CASE
            WHEN esdid = {$acerto_spo}
            THEN SUM(valor)
        END, 0) AS acerto_spo,
    COALESCE(
        CASE
            WHEN esdid = {$em_analise_spo}
            THEN SUM(valor)
        END, 0) AS em_analise_spo,
    COALESCE(
        CASE
            WHEN esdid = {$em_analise_secretaria}
            THEN SUM(valor)
        END, 0) AS em_analise_secretaria,
        COALESCE(
        CASE
            WHEN esdid = {$acertos_secretaria}
            THEN SUM(valor)
        END, 0) AS acertos_secretaria,
    COALESCE(
        CASE
            WHEN esdid = {$atendido}
            THEN SUM(valor)
        END, 0) AS atendido,
        COALESCE(
        CASE
            WHEN esdid = {$recusado}
            THEN SUM(valor)
        END, 0) AS recusado
FROM
    (
        SELECT
            ccr.anxcod,
            esd.esddsc AS descricao,
            esd.esdid,
           COUNT(distinct pli.pdlid) AS valor
        FROM
            progorc.pedidolimite pli
        INNER JOIN
            progorc.pedidodetalhe pde
        ON
            pde.pdlid = pli.pdlid
        INNER JOIN
            progorc.contacorrente ccr
        ON
            ccr.ccrid = pde.ccrid
        INNER JOIN
            progorc.anexo ane
        ON
            ane.anxcod = ccr.anxcod
        LEFT JOIN
            workflow.documento doc
        ON
            doc.docid = pli.docid
        LEFT JOIN
            workflow.estadodocumento esd
        ON
            esd.esdid = doc.esdid
        WHERE
            EXTRACT(YEAR FROM pli.pdldatapedido) = {$_SESSION['exercicio']}
            {$whereUO}
        GROUP BY
            ccr.anxcod,
            esd.esdid,
            esd.esddsc ) AS foo
WHERE
    esdid IS NOT NULL

GROUP BY
    esdid,
    anxcod
) tabela
group by anxcod
order by anxcod
DML;
    $listagem = new Simec_Listagem();
    $cabecalhoTabela = array('Anexo', 'Não Enviados', 'Análise SPO', 'Acertos SPO', 'Análise Secretaria', 'Acertos Secretaria', 'Atendidos', 'Recusados');
    $listagem->setAcoes(array(
        'view' => 'listaAnexo'
    ));
    $listagem->setQuery($sql);
    $listagem->setCabecalho($cabecalhoTabela);
    $listagem->render();
    ?>
    </section>
    <section class="col-md-5">
    	<section class="panel panel-primary">
    		<section class="panel-heading"><strong>GRÁFICO</strong></section>
    		<section class="panel-body" style="background-color: royalblue;">
    <?php
    $sql = "
                SELECT
                    'Anexo ' || ccr.anxcod                AS descricao,
                    COUNT(DISTINCT pli.pdlid) AS valor
                FROM
                    progorc.pedidolimite pli
                INNER JOIN
                    progorc.pedidodetalhe pde
                ON
                    pde.pdlid = pli.pdlid
                INNER JOIN
                    progorc.contacorrente ccr
                ON
                    ccr.ccrid = pde.ccrid
                INNER JOIN
                    progorc.anexo ane
                ON
                    ane.anxcod = ccr.anxcod
                WHERE
                    EXTRACT(YEAR FROM pli.pdldatapedido) = {$_SESSION['exercicio']}
                    {$whereUO}
                GROUP BY
                    ccr.anxcod
                    ";
    $dados = $db->carregar($sql);
    if ($dados) {
        echo geraGrafico($dados, $nomeUnico = 'pedidos', $titulo = 'Pedidos de Limite', $formatoDica = "", $formatoValores = "{point.y} ({point.percentage:.2f} %)", $nomeSerie = "", $mostrapopudetalhes = false, $caminhopopupdetalhes = "", $largurapopupdetalhes = "", $alturapopupdetalhes = "", $mostrarLegenda = true, $aLegendaConfig = false, $legendaClique = false);
    }
    ?>
            </section>
        </section>
    </section>
</div>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>
<link rel="stylesheet" href="css/progorc.css">

<div class="modal fade" id="modal-info-status">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                <h4 class="modal-title">Limites Orçamentários</h4>
            </div>
            <div class="modal-body">
                <div class="panel panel-default">
                    <div class="panel-heading">Dados</div>
                    <table class="table">
                        <tr>
                            <td><strong>Status:</strong></td>
                            <td colspan="3"><span id="status"></td>
                        </tr>
                        <tr>
                            <td><strong>Anexo:</strong></td>
                            <td colspan="3"><span id="anexo"></span></td>
                        </tr>
                        <tr>
                            <td><strong>Quantidade:</strong></td>
                            <td colspan="3"><span id="qnt"></span></td>
                        </tr>
                        <tr>
                            <td><strong>Total Ampliação (R$): </strong></td>
                            <td colspan="3"><span id="ampliacao"></span></td>
                        </tr>
                        <tr>
                            <td><strong>Total Redução (R$):</strong></td>
                            <td colspan="3"><span id="reducao"></span></td>
                        </tr>
                        <tr>
                            <td><strong>Total Ampliação(Atendido) (R$):</strong></td>
                            <td colspan="3"><span id="ampliacao_atendido"></span></td>
                        </tr>
                        <tr>
                            <td><strong>Total Redução(Atendido) (R$):</strong></td>
                            <td colspan="3"><span id="reducao_atendido"></span></td>
                        </tr>

                    </table>
                </div>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">OK</button>

            </div>
        </div>
    </div>
</div>
