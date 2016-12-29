<?php
$fm = new Simec_Helper_FlashMessage("{$_SESSION['sisdiretorio']}/relatorio");

// -- Setando o novo exercicio escolhido via seletor no cabeçalho da página
if (isset($_REQUEST['exercicio'])) {
    $exercicio = $_REQUEST ['exercicio'];
} else {
    $exercicio = $_SESSION['exercicio'];
}
if ($_REQUEST['requisicao'] == 'exportarXLS') {
    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
    $_REQUEST['_p'] = 'all';
} elseif ('filtrarCategorias' == $_REQUEST['requisicao']) {
    $service = new Proporc_Service_Limites();
    $service->gdpid = $_REQUEST['dados']['gdpid'];
    echo simec_json_encode($service->carregarOpcoesCategoria());
    die();
} else {
    $listagem = new Simec_Listagem();
}

$listagem->setCabecalho(
    array(
            'Grupo de Coluna',
            'Coluna',
            'Esfera',
            'UO',
            'Função',
            'Subfunção',
            'Programa',
            'Ação',
            'Localizador',
            'Meta Ação',
            'Produto Ação',
            'Código do PO',
            'Descrição do PO',
            'Meta PO',
            'Produto PO',
            'Subação',
            'Natureza da Despesa',
            'Fonte',
            'Valor Detalhado',
            'Situação'
        )
);

/* Máscara de Moeda */
$listagem->addCallbackDeCampo('valordetalhado', 'mascaraMoeda')
    ->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('valordetalhado'));


/* Filtros */
$filtro = "";
if (isset($_REQUEST['dados']['unicod']) && $_REQUEST['dados']['unicod'] != '') {
    $filtro .= " AND u.unicod = '{$_REQUEST['dados']['unicod']}' ";
}
if (isset($_REQUEST['dados']['acacod']) && $_REQUEST['dados']['acacod'] != '') {
    $filtro .= " AND a.acacod = '{$_REQUEST['dados']['acacod']}' ";
}
if (isset($_REQUEST['dados']['esdid']) && $_REQUEST['dados']['esdid'] != '') {
    $filtro .= " AND doc.esdid = {$_REQUEST['dados']['esdid']} ";
}
if (isset($_REQUEST['dados']['dspid']) && is_numeric($_REQUEST['dados']['dspid'])) {
    $filtro .= " AND dsp.dspid = {$_REQUEST['dados']['dspid']} ";
}
if (isset($_REQUEST['dados']['gdpid']) && $_REQUEST['dados']['gdpid'] != '') {
    $filtro .= " AND gdp.gdpid = {$_REQUEST['dados']['gdpid']} ";
}
$sql = <<<DML
SELECT gdp.gdpnome,
       dsp.dspnome,
       a.esfcod,
       a.unicod,
       a.funcod,
       a.sfucod,
       a.prgcod,
       a.acacod,
       a.loccod,
       a.acaqtdefisico AS meta,
       a.proddesc AS produto,
       pa.plocodigo AS po,
       pa.plotitulo AS podsc,
       mp.mpovalor AS metapo,
       pa.ploproduto AS propo,
       sba.sbacod AS subacao,
       nds.ndpcod AS nd,
       dpa.foncod AS fonte,
       dpa.dpavalor AS valordetalhado,
       COALESCE((SELECT esd.esddsc
                   FROM workflow.documento doc
                     LEFT JOIN workflow.estadodocumento esd USING(esdid)
                   WHERE doc.docid = a.docid), 'Em preenchimento') AS acasituacao
  FROM elabrev.ppaacao_orcamento a
    INNER JOIN unidade u USING(unicod)
    LEFT JOIN elabrev.planoorcamentario pa USING(acaid)
    LEFT JOIN elabrev.metaplanoorcamentario mp ON(mp.ploid = pa.ploid
                                                  AND mp.mpostatus = 'A')
    LEFT JOIN elabrev.despesaacao dpa ON(pa.ploid = dpa.ploid)
    LEFT JOIN elabrev.subacao sba USING(sbaid)
    LEFT JOIN public.naturezadespesa nds USING(ndpid)
    LEFT JOIN workflow.documento doc USING(docid)
    LEFT JOIN proporc.ploafinanceiro plf USING(dpaid)
    LEFT JOIN proporc.despesa dsp ON(plf.mtrid = dsp.dspid)
    LEFT JOIN proporc.grupodespesa gdp USING(gdpid)
  WHERE a.prgano = '{$exercicio}'
    {$filtro}
    AND a.acastatus = 'A'
  ORDER BY gdp.gdpnome,
           dsp.dspnome,
           a.unicod,
           a.acacod,
           a.loccod
DML;

$listagem->addCallbackDeCampo(array('podsc', 'produto'), 'alinhaParaEsquerda')
    ->setQuery($sql);
if ($_REQUEST['requisicao'] == 'exportarXLS') {
    $listagem->render();
    die();
} else {
    $listagem->turnOnPesquisator();
}
include APPRAIZ . "includes/cabecalho.inc";
?>
<br style="clear:both" />
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script type="text/javascript">
$(document).ready(function() {
    $('#pesquisar').click(function() {
        $('#requisicao').attr('value', 'mostrarHTML');
        $('#pesquisar').html('Carregando...');
        $('#pesquisar').attr('disabled', 'disabled');
        $("body").prepend('<div class="ajaxCarregando"></div>');
        $(".ajaxCarregando").hide().html('Carregando, aguarde...').fadeIn();
        $('#filtrouo').submit();
    });
    $('#exportar').click(function() {
        $('#requisicao').attr('value', 'exportarXLS');
        $("body").prepend('<div class="ajaxCarregando"></div>');
        $(".ajaxCarregando").hide().html('Carregando, aguarde...').fadeIn();
        $('#filtrouo').submit();
    });

    $('#gdpid').change(function(){
        $.get(window.location.href, {requisicao:'filtrarCategorias', 'dados[gdpid]':$(this).val()}, function(response){
            $('#dspid').children().remove();
            if (0 === response.length) {
                return;
            }

            $('#dspid').append('<option>Selecione uma categoria</option>');
            for (var x in response) {
                $('#dspid').append('<option value="' + response[x].codigo + '">' + response[x].descricao + '</option>');
            }
            $('#dspid').trigger('chosen:updated');
        }, 'json');
    });
});
</script>
<style type="text/css">
#tb_render{
font-size:10px
}
</style>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Extrato Completo da PLOA <?php echo $_SESSION['exercicio'] + 1; ?></li>
    </ol>
    <?php echo $fm->getMensagens(); ?>
    <br style="clear:both" />
    <form name="filtrouo" id="filtrouo" method="POST" role="form">
        <input type="hidden" name="requisicao" id="requisicao" />
        <div class="well">
            <div class="form-group row">
                <div class="col-md-2">
                    <label class="control-label" for="unicod">Unidade Orçamentária:</label>
                </div>
                <div class="col-md-10">
                    <?php inputComboUnicod($_REQUEST['dados']['unicod']); ?>
                </div>
            </div>
            <div class="form-group row">
                <div class="col-md-2">
                    <label class="control-label" for="unicod">Ação:</label>
                </div>
                <div class="col-md-10">
                    <?php
                    $sql = " SELECT DISTINCT
                                    acacod AS codigo,
                                    acacod AS descricao
                                FROM
                                    elabrev.ppaacao_orcamento
                                WHERE
                                    prgano = '{$exercicio}'
                                ORDER BY
                                    1";
                    inputCombo("dados[acacod]", $sql, $_REQUEST['dados']['acacod'], "acacod");
                    ?>
                </div>
            </div>
            <div class="form-group row">
                <div class="col-md-2">
                    <label class="control-label" for="unicod">Situação:</label>
                </div>
                <div class="col-md-10">
                    <?php
                    $sql = "  SELECT
                                    esd.esdid  AS codigo,
                                    esd.esddsc AS descricao
                                FROM
                                    workflow.estadodocumento esd
                                WHERE
                                    tpdid = 188
                                AND esdstatus = 'A'
                                ORDER BY
                                    esdordem";
                    inputCombo("dados[esdid]", $sql, $_REQUEST['dados']['esdid'], "esdid");
                    ?>
                </div>
            </div>
            <div class="form-group row">
                <div class="col-md-2">
                    <label class="control-label" for="gdpid">Grupo:</label>
                </div>
                <div class="col-md-10">
                    <?php
                    $sql = <<<DML
SELECT DISTINCT
    gdp.gdpid AS codigo,
    gdp.gdpnome AS descricao
FROM proporc.grupodespesa gdp
WHERE
    EXISTS(
        SELECT
            1
        FROM proporc.periodoreferencia prf
        WHERE
            prf.prfid = gdp.prfid
            AND prf.prsano = '{$_SESSION['exercicio']}'
    )
ORDER BY
    descricao
DML;
                    inputCombo("dados[gdpid]", $sql, $_REQUEST['dados']['gdpid'], "gdpid");
                    ?>
                </div>
            </div>
            <div class="form-group row">
                <div class="col-md-2">
                    <label class="control-label" for="dspid">Coluna:</label>
                </div>
                <div class="col-md-10">
                    <?php
                    $sql = <<<DML
SELECT DISTINCT
    dsp.dspid AS codigo,
    dsp.dspnome AS descricao
FROM proporc.despesa dsp
WHERE
    EXISTS (
        SELECT
            1
        FROM proporc.grupodespesa gdp
            JOIN proporc.periodoreferencia prf USING(prfid)
        WHERE
            gdp.gdpid = dsp.gdpid
            AND prf.prsano = '{$_SESSION['exercicio']}'
    )
ORDER BY
    descricao
DML;
                    inputCombo("dados[dspid]", $sql, $_REQUEST['dados']['dspid'], "dspid");
                    ?>
                </div>
            </div>
        </div>
        <button type="button" class="btn btn-primary" id="pesquisar">Pesquisar</button>
        <button type="button" class="btn btn-danger" id="exportar">Exportar XLS</button>
    </form>
</div>
<br />
<div class="row col-md-12">
<?php
    // -- Saída do relatório
$listagem->setFormFiltros('filtrouo')
    ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>
</div>