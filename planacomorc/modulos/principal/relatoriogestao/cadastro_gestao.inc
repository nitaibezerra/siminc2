<?php
/**
 * Entrada do cadastro de relatórios do TCU.
 *
 * @version $Id: cadastro_gestao.inc 93329 2015-02-02 12:08:29Z maykelbraz $
 */

/**
 * Funções de apoio aos sistemas SPO.
 * @see funcoesspo.php
 */
require_once(APPRAIZ . 'includes/funcoesspo.php');

/**
 * Funções específicas da funcionalidade Relatório TCU.
 * @see _funcoestcu.php
 */
require_once(APPRAIZ . 'www/planacomorc/_funcoestcu.php');

/**
 * Gerenciador de mensagens entre requisições.
 * @see Simec_Helper_FlashMessage
 */
include_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
$fm = new Simec_Helper_FlashMessage('planacomorc/relgestao');

// -- Perfis do usuário
$perfis = pegaPerfilGeral($_SESSION['usucpf']);

if ($_POST['execucao'] == 'cadastrar' && !$_POST['id']) {
    try {
        if (empty($_POST['unicod'])) {
            throw new Exception('Para criar um novo relatório, você primeiro deve selecionar uma Unidade Orçamentária.');
        }
        if (empty($_POST['acacod']) && empty($_POST['acaofloa'])) {
            throw new Exception('Parar criar um novo relatório, você primeiro deve selecionar uma Ação.');
        }

        $strSql = <<<DML
SELECT rlgid
  FROM planacomorc.relgestao
  WHERE unicod = '%s'
    AND exercicio = '%s'
    AND acacod = '%s'
DML;
        $acacod = (!empty($_REQUEST['acacod'])?$_REQUEST['acacod']:$_REQUEST['acaofloa']);
        $stmt = sprintf($strSql, $_REQUEST['unicod'], $_SESSION['exercicio'], $acacod);

        if (!($rlgid = $db->pegaUm($strSql))) {
            $sql = <<<DML
INSERT INTO planacomorc.relgestao(unicod, acacod, exercicio, usucpf, acatipo)
  VALUES('%s', '%s', '%s', '%s', '%s')
  RETURNING rlgid
DML;
            $stmt = sprintf($sql, $_REQUEST['unicod'], strtoupper($acacod), $_SESSION['exercicio'], $_SESSION['usucpf'], $_REQUEST['acatipo']);
            $rlgid = $db->pegaUm($stmt);
            $db->commit ();

            $fm->addMensagem('Registro <u>criado</u> com sucesso.');
        } else {
            $fm->addMensagem('Registro <u>aberto</u> com sucesso.', Simec_Helper_FlashMessage::INFO);
        }

        echo "<script>window.location.assign(\"planacomorc.php?modulo=principal/relatoriogestao/relgestao&acao=A&rlgid={$rlgid}\")</script>";
        die();
    } catch (Exception $e) {
        $fm->addMensagem($e->getMessage(), Simec_Helper_FlashMessage::ERRO);
        header("Location: {$_SERVER['HTML_REFERER']}");
        die();
    }
}

// -- Queries da consulta de ações - select
if (isset($_POST['requisicao']) && 'consultar-acoes' == $_POST['requisicao']) {
    filtroAcao($perfis, $_POST['acacod'], $_POST['unicod']);
    die();
}

/**
 * Cabecalho do SIMEC.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<link rel="stylesheet" type="text/css" href="/library/bootstrap-switch/stylesheets/bootstrap-switch.css">
<style type="text/css">
.marcado{background-color:#C1FFC1!important}
.remover{display:none}
</style>
<script src="/library/bootstrap-switch/js/bootstrap-switch.min.js" type="text/javascript"></script>
<script type="text/javascript">
function tudoMaiusculo()
{
    var x = document.getElementById("acacod");
    x.value = x.value.toUpperCase();
}
function editarRelatorio(id) {
    window.location.assign('planacomorc.php?modulo=principal/relatoriogestao/relgestao&acao=A&rlgid='+id);
}
function exportarItem(params, tipo)
{
    window.open('planacomorc.php?modulo=principal/relatoriogestao/relgestao&acao=A&' + tipo + '=true&rlgid=' + params[0]);
}
function apagarRelatorio(id)
{
    bootbox.confirm('Tem certeza que quer apagar este relatório?', function(result){
        if (result) {
            var $form = $('<form />')
                .attr({method: 'post', action:'planacomorc.php?modulo=principal/relatoriogestao/relgestao&acao=A'})
                .appendTo('body');
            $('<input />').attr({type:'hidden', name:'requisicao', value:'apagar'}).appendTo($form);
            $('<input />').attr({type:'hidden', name:'rlgid', value:id}).appendTo($form);
            $form.submit();
        }
    });
}

$(function(){
    $(".btn-acoes").on("click", function(){
        $("#acao-loa-container, #acacod-container").hide();
        if ($(this).attr("data-value") == 'R') {
            $("#acao-loa-container").show();
        } else {
            $("#acacod-container").show();
        }
    });

    // -- Correção paliativa do bug #9920 do bootstrap.
    // -- Correção agendada para a versão 3.0.3 do bootstrap.
    // -- <https://github.com/twbs/bootstrap/issues/9920>
    $("input:radio").change(function() {
        $(this).prop('checked', true);
    });

    $('#unicod').change(function(){
        var unicod = $(this).val();
        $.post(window.location.url, {unicod:unicod, requisicao:'consultar-acoes'}, function(data){
            $('#acacod').html(data);
            $('#acacod').trigger('chosen:updated');
        });
    });
});
</script>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Relatório de Gestão (TCU/CGU)</li>
    </ol>
</div>
<div class="col-lg-12">
    <div class="well">
        <fieldset>
            <form id="formPrincipal" name="formPrincipal" method="POST" class="form-horizontal"
                  enctype="multipart/form-data">
                <div class="form-group">
                    <div class="col-lg-02"></div>
                    <div class="col-lg-10">
                        <input type="hidden" name="execucao" id="execucao" value="cadastrar">
                        <input type="hidden" name="id" id="id" value="<?php echo $id; ?>">
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-02">
                        <label class="col-lg-2 control-label" for="unicod">Unidade Orçamentária:</label>
                    </div>
                    <div class="col-lg-10">
                    <?php
                    $whereUO = '';
                    if (in_array(PFL_RELATORIO_TCU, $perfis)) {
                        $whereUO = <<<DML
    AND EXISTS (SELECT 1
                  FROM planacomorc.relgdados rld
                    INNER JOIN planacomorc.relgestao rlg USING(rlgid)
                    INNER JOIN planacomorc.usuarioresponsabilidade rpu USING(rldid)
                  WHERE rlg.unicod = uni.unicod
                    AND rpu.usucpf = '%s'
                    AND rpu.pflcod = '%s'
                    AND rpu.rpustatus = 'A')
DML;
                        $whereUO = sprintf($whereUO, $_SESSION['usucpf'], PFL_RELATORIO_TCU);
                    }

                    $sql = <<<DML
SELECT uni.unicod AS codigo,
       uni.unicod || ' - ' || unidsc AS descricao
  FROM public.unidade uni
  WHERE uni.unicod IN (SELECT DISTINCT unicod
                         FROM recorc.vinculacaoexercicio
                         WHERE exercicio = '{$_SESSION['exercicio']}')
    AND uni.unistatus = 'A'
    AND unicod IN('26101', '26298')
    {$whereUO}
ORDER BY uni.unicod
DML;
                    $unicod = $_REQUEST['unicod'];
                    inputCombo('unicod', $sql, $unicod, 'unicod');
                    ?>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-02">
                        <label class="col-lg-2 control-label">Tipo:</label>
                    </div>
                    <div class="col-lg-10">
                        <div class="btn-group" data-toggle="buttons">
                            <label class="btn btn-default btn-acoes <?php echo ($_POST['acatipo'] != 'R')?'active':''; ?>" data-value="N">
                                <input type="radio" class="radio_acaorap" id="radio_acaorap_N" value="N" name="acatipo"
                                       <?php echo ($_POST['acatipo'] != 'R')?'checked="checked"':''; ?> /> Ações LOA <?=$_SESSION['exercicio']?>
                            </label>
                            <label class="btn btn-default btn-acoes <?php echo ($_POST['acatipo'] == 'R')?'active':''; ?>" data-value="R">
                                <input type="radio" class="radio_acaorap" id="radio_acaorap_R" value="R" name="acatipo"
                                       <?php echo ($_POST['acatipo'] == 'R')?'checked="checked"':''; ?>/> Ações não Previstas LOA <?=$_SESSION['exercicio']?>
                            </label>
                        </div>
                    </div>
                </div>
                <div class="form-group" id="acao-loa-container" style="display:none">
                    <div class="col-lg-02">
                        <label class="col-lg-2 control-label" for="acaofloa">Ações não previstas<br /> na LOA <?= date('Y'); ?>:</label>
                    </div>
                    <div class="col-lg-10">
                        <?= inputTexto('acaofloa', isset($_POST['acaofloa']) ? $_POST['acaofloa'] : '', 'acaofloa', 4, false); ?>
                    </div>
                </div>
                <div class="form-group" id="acacod-container">
                    <div class="col-lg-02">
                        <label class="col-lg-2 control-label" for="acacod">Ação:</label>
                    </div>
                    <div class="col-lg-10">
                    <?php filtroAcao($perfis, $_POST['acacod'], $_POST['unicod']); ?>
                    </div>
                </div>
                <div class="col-lg-12">
                    <button class="btn btn-warning" type="reset" onclick="window.location = window.location;">Limpar</button>
                    <?php if (!in_array(PFL_RELATORIO_TCU, $perfis)): ?>
                    <button class="btn btn-success" id="inserir" type="submit">
                        <span class="glyphicon glyphicon-ok"></span> Avançar
                    </button>
                    <button class="btn btn-primary" id="btnBuscar" type="button">
                        <span class="glyphicon glyphicon-search"></span> Buscar
                    </button>
                    <?php else: ?>
                    <button class="btn btn-primary" id="btnBuscar" type="button">
                        <span class="glyphicon glyphicon-search"></span> Buscar
                    </button>
                    <?php endif; ?>
                </div>
            </form>
        </fieldset>
    </div>
</div>
<?php echo $fm->getMensagens(); ?>
<div class="col-lg-12">
<script type="text/javascript">
$(document).ready(function(){
    $('#btnBuscar').click(function(){
        $('#execucao').val('filtrar');
        $('#formPrincipal').submit();
    });
});
</script>
<?php
$whereResp = '';
if (in_array(PFL_RELATORIO_TCU, $perfis)) {
    $whereResp = <<<DML
    AND EXISTS (SELECT 1
                  FROM planacomorc.relgdados rld
                    INNER JOIN planacomorc.usuarioresponsabilidade rpu USING(rldid)
                  WHERE rlg.rlgid = rld.rlgid
                    AND rpu.usucpf = '%s'
                    AND rpu.pflcod = '%s'
                    AND rpu.rpustatus = 'A')
DML;
    $whereResp = sprintf($whereResp, $_SESSION['usucpf'], PFL_RELATORIO_TCU);
}

$where = array();
if (isset($_POST['unicod']) && !empty($_POST['unicod'])) {
    $where[] = sprintf("rlg.unicod = '%s'", $_POST['unicod']);
}
if (isset($_POST['acacod']) && !empty($_POST['acacod'])) {
    $where[] = sprintf("rlg.acacod = '%s'", $_POST['acacod']);
}
if (isset($_POST['acatipo']) && !empty($_POST['acatipo'])) {
    $where[] = sprintf("rlg.acatipo = '%s'", $_POST['acatipo']);
}
if (!empty($where)) {
    $where = ' AND ' . implode(' AND ', $where);
} else {
    $where = '';
}

$select = <<<DML
SELECT rlg.rlgid,
       rlg.unicod || ' - ' || uni.unidsc AS unicod,
       rlg.acacod,
       rlg.acatipo,
       TO_CHAR(rlg.dtultimaatualizacao, 'DD/MM/YYYY') AS dtultimaatualizacao,
       (SELECT usu.usunome
          FROM planacomorc.relgdados rld
            LEFT JOIN planacomorc.usuarioresponsabilidade rpu ON(rld.rldid = rpu.rldid AND rpu.rpustatus = 'A')
            LEFT JOIN seguranca.usuario usu USING(usucpf)
          WHERE rld.rlgid = rlg.rlgid
            AND rld.rldtipo IN('acao', 'acaorap')
            AND rpustatus = 'A'
          ORDER BY rpu.rpudata_inc DESC
          LIMIT 1) AS usunome,
       COALESCE((SELECT esddsc
                   FROM planacomorc.relgdados rld
                     INNER JOIN workflow.documento using(docid)
                     INNER JOIN workflow.estadodocumento USING(esdid)
                   WHERE rld.rlgid = rlg.rlgid
                     AND rld.rldtipo IN('acao', 'acaorap')
                   LIMIT 1), 'Em preenchimento') AS esddsc,
       COALESCE((SELECT esdid
                   FROM planacomorc.relgdados rld
                     INNER JOIN workflow.documento using(docid)
                   WHERE rld.rlgid = rlg.rlgid
                     AND rld.rldtipo IN('acao', 'acaorap')
                   LIMIT 1), %d) AS esdid
  FROM planacomorc.relgestao rlg
    LEFT JOIN public.unidade uni USING(unicod)
  WHERE rlg.exercicio = '%s'
  {$whereResp}
  {$where}
DML;
$stmt = sprintf(
    $select,
    ESDID_TCU_EM_PREENCHIMENTO,
    $_SESSION['exercicio']
);

$cabecalho = array('Unidade Orçamentária', 'Ação', 'Tipo', 'Data criação', 'Responsável da ação', 'Estado da ação', '&nbsp;');
$list = new Simec_Listagem();
$list->setQuery($stmt)
    ->setCabecalho($cabecalho)
    ->addAcao('edit', 'editarRelatorio')
    ->addAcao(
        'download',
        array(
            'func' => 'exportarItem',
            'formatos' => array('pdf', 'xls')
        )
    )
    ->addAcao('delete', 'apagarRelatorio')
    ->addCallbackDeCampo('acatipo', 'formatarAcaTipo')
    ->addCallbackDeCampo('esdid', 'formatarStatusIcone')
    ->addCallbackDeCampo(array('unicod', 'usunome'), 'alinhaParaEsquerda')
    ->turnOnPesquisator()
    ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>
</div>