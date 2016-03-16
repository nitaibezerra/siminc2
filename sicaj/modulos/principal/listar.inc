<?php
/**
 * Listagem de pedidos do SICAJ
 *
 * @version $Id: listar.inc 100556 2015-07-28 20:37:34Z maykelbraz $
 * @package SiMEC
 * @subpackage SICAJ
 */

if (isset($_POST) && $_POST['requisicao'] == 'xls') {
    listaPedidos($_POST, $_POST['requisicao']);
    die;
}

if (isset($_GET['deleteAcao']) && is_numeric($_GET['deleteAcao'])) {
    apagaPedido($_GET['deleteAcao']);
    $fm = new Simec_Helper_FlashMessage('sicaj/pedido');
    $fm->addMensagem('Pedido foi apagado com sucesso!', Simec_Helper_FlashMessage::SUCESSO);
}

//Chamada para cabeçalho padrão SIMEC
include APPRAIZ . 'includes/cabecalho.inc';
?>
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<style type="text/css">
.table .acertos_uo{color:#FF0000!important}
.table .acertos_segep{color:#FFA500!important}
.table .nao_enviado{color:#f0ad4e!important}
</style>
<div class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Pedidos</li>
    </ol>
    <div class="well">
        <?php if (is_object($fm)) echo $fm->getMensagens(); ?>
        <form name="filtrosicaj" id="filtrosicaj" method="POST" role="form" class="form-horizontal">
            <input type="hidden" name="requisicao" id="requisicao" value="" />
            <div class="form-group">
                <label for="pdccodacaojudicial" class="col-md-2 control-label">Cód.SICAJ</label>
                    <div class="col-md-10">
                       <?php inputTexto('pdccodacaojudicial', $dados['pdccodacaojudicial'], 'pdccodacaojudicial', '', false, array('limite' => '6')); ?>
                    </div>
             </div>
            <div class="form-group">
                <label class="control-label col-md-2" for="mesref">Mês Referência:</label>
                <div class="col-md-10">
                    <?php
                    $mesRef = array(
                        array('codigo' => 1, 'descricao' => 'Janeiro'),
                        array('codigo' => 2, 'descricao' => 'Fevereiro'),
                        array('codigo' => 3, 'descricao' => 'Março'),
                        array('codigo' => 4, 'descricao' => 'Abril'),
                        array('codigo' => 5, 'descricao' => 'Maio'),
                        array('codigo' => 6, 'descricao' => 'Junho'),
                        array('codigo' => 7, 'descricao' => 'Julho'),
                        array('codigo' => 8, 'descricao' => 'Agosto'),
                        array('codigo' => 9, 'descricao' => 'Setembro'),
                        array('codigo' => 10, 'descricao' => 'Outubro'),
                        array('codigo' => 11, 'descricao' => 'Novembro'),
                        array('codigo' => 12, 'descricao' => 'Dezembro'),
                    );
                    ?>
                    <?php inputCombo('mesref', $mesRef, isset($_POST['mesref']) ? $_POST['mesref'] : '', 'mesref'); ?>
                </div>
            </div>
                <div class="form-group">
                    <label class="control-label col-md-2" for="unicod">Unidade Orçamentária (UO):</label>
                    <div class="col-md-10">
                        <?php
                        $whereAdicional = array();
                        if (in_array(PERFIL_UO_EQUIPE_TECNICA, pegaPerfilGeral($_SESSION['usucpf']))) {

                            $where = <<<DML
  EXISTS (SELECT 1
            FROM sicaj.usuarioresponsabilidade rpu
            WHERE rpu.usucpf = '%s'
              AND rpu.pflcod = '%s'
              AND rpu.unicod = uni.unicod
              AND rpu.rpustatus = 'A')
DML;
                            $where = sprintf($where, $_SESSION['usucpf'], PERFIL_UO_EQUIPE_TECNICA);
                            $whereAdicional[] = $where;
                        }
                        inputComboUnicod(isset($_POST['dados']['unicod'])?$_POST['dados']['unicod']:'', $whereAdicional);
                        unset($whereAdicional);
                        ?>
                    </div>
                </div>
                <div class="form-group">
                    <label class="control-label col-md-2">Tipo de Homologação:</label>
                    <div class="col-md-10">
                        <?php
                        $opcoes = array(
                            'Todos' => null,
                            'Inicial' => 'H',
                            'Reativação' => 'R',
                            'Anulação' => 'D'
                        );
                        $pdcTipo = !isset($_POST['dados']['pdctipo']) ? getStorage('pdctipo') : $_POST['dados']['pdctipo'];
                        inputChoices('dados[pdctipo]', $opcoes, $pdcTipo, 'pdctipo_');
                        ?>
                    </div>
                </div>
                <div class="form-group">
                    <label class="control-label col-md-2">Status do Pedido:</label>
                    <div class="col-md-10">
                        <div class="btn-group" data-toggle="buttons">
                            <?php
                            $fonte = !isset($_POST['dados']['fonte']) ? getStorage('fonte') : $_POST['dados']['fonte']; ?>
                            <label
                                class="btn btn-default <?php
                                if ($fonte == '' || !$fonte || $fonte == 'todos') {
                                    echo 'active';
                                }
                                ?>">
                                    <input
                                        type="radio"
                                        name="dados[fonte]"
                                        id="fonte_todos"
                                        value="todos" />Todos
                            </label>
                            <label
                                class="btn btn-default <?php
                                if ($fonte == NAO_ENVIADO) {
                                    echo 'active';
                                }
                                ?>">
                                    <input
                                        type="radio"
                                        name="dados[fonte]"
                                        id="fonte_O"
                                        value="<?= NAO_ENVIADO; ?>"
                                    <?php
                                    if ($fonte == NAO_ENVIADO) {
                                        echo 'checked=checked';
                                    }
                                    ?> /> Não Enviado
                            </label>
                            <label
                                class="btn btn-default <?php
                                if (!empty($fonte)) {
                                    if (strstr('1288, 1340', $fonte))
                                        echo 'active';
                                }
                                ?>">
                                    <input
                                        type="radio"
                                        name="dados[fonte]"
                                        id="fonte_P"
                                        value="1288, 1340"
                                    <?php
                                    if (!empty($fonte)) {
                                        if (strstr('1288, 1340', $fonte))
                                            echo 'checked=checked';
                                    }
                                    ?> /> Análise SPO
                            </label>
                            <label
                                class="btn btn-default <?php
                                if (!empty($fonte)) {
                                    if (ANALISE_COORDENACAO == $fonte)
                                        echo 'active';
                                }
                                ?>">
                                    <input
                                        type="radio"
                                        name="dados[fonte]"
                                        id="fonte_P"
                                        value="<?=ANALISE_COORDENACAO?>"
                                    <?php
                                    if (!empty($fonte)) {
                                        if (ANALISE_COORDENACAO == $fonte)
                                            echo 'checked=checked';
                                    }
                                    ?> /> Análise Coordenação
                            </label>
                            <label
                                class="btn btn-default <?php
                                if (!empty($fonte)) {
                                    if (strstr('1290, 1341', $fonte))
                                        echo 'active';
                                }
                                ?>">
                                <input
                                    type="radio"
                                    name="dados[fonte]"
                                    id="fonte_P"
                                    value="1290, 1341"
                                    <?php
                                    if (!empty($fonte)) {
                                        if (strstr('1290, 1341', $fonte))
                                            echo 'checked=checked';
                                    }
                                    ?> /> Acertos
                            </label>
                            <label
                                class="btn btn-default <?php
                                if ($fonte == ANALISE_SOF) {
                                    echo 'active';
                                }
                                ?>">
                                <input
                                    type="radio"
                                    name="dados[fonte]"
                                    id="fonte_P"
                                    value="<?php echo ANALISE_SOF; ?>"
                                    <?php
                                    if ($fonte == ANALISE_SOF) {
                                        echo 'checked=checked';
                                    }
                                    ?> /> Análise SOF
                            </label>
                            <label
                                class="btn btn-default <?php
                                if ($fonte == '1291,1343,1342') {
                                    echo 'active';
                                }
                                ?>">
                                <input
                                    type="radio"
                                    name="dados[fonte]"
                                    id="fonte_P"
                                    value="1291,1343,1342"
                                    <?php
                                    if ($fonte == '1291') {
                                        echo 'checked=checked';
                                    }
                                    ?> /> Concluído
                            </label>
                            <label
                                class="btn btn-default <?php
                                if ($fonte == PEDIDO_CANCELADO) {
                                    echo 'active';
                                }
                                ?>">
                                <input
                                    type="radio"
                                    name="dados[fonte]"
                                    id="fonte_P"
                                    value="<?php echo PEDIDO_CANCELADO; ?>"
                                    <?php
                                    if ($fonte == PEDIDO_CANCELADO) {
                                        echo 'checked=checked';
                                    }
                                    ?> /> Cancelado
                            </label>
                        </div>
                    </div>
                </div>
                <button type="reset" class="btn btn-warning" id="limpar">Limpar</button>
                <button type="button" class="btn btn-primary" id="buscar">Buscar</button>
                <button type="button" class="btn btn-success" id="inserir">Inserir</button>
                <button type="button" class="btn btn-danger" id="gerarXls">
                    <span class="glyphicon glyphicon-download-alt"></span>&nbsp;Gerar XLS
                </button>
        </form>
    </div>
    <?php listaPedidos($_POST, 'paginado', $where); ?>
</div>

<script type="text/javascript">
function abrirPedido(codPedido, unicod) {
    if (codPedido && unicod) {
        location.href="sicaj.php?modulo=principal/principal&acao=A&unicod="+unicod+"&id="+codPedido;
    }
}

function apagarPedido(codPedido) {
    if (codPedido) {
        if (confirm("Deseja realmente apagar o pedido?")) {
            location.href="sicaj.php?modulo=principal/listar&acao=A&deleteAcao="+codPedido;
        }
    }
}

$(function(){
    $("#inserir").on("click", function() {
        var unicod = $("#unicod").val(),
            pdctipo = $('input[type="radio"][name="dados[pdctipo]"]:checked').val();

        if (unicod && pdctipo) {
            location.href = "sicaj.php?modulo=principal/principal&acao=A"
                + "&unicod=" + unicod
                + '&pdctipo=' + pdctipo;
        } else {
            alert("Você precisa escolher uma unidade orçamentária e um tipo de pedido.");
        }
    });

    $("#buscar").on("click", function(){
        $("#requisicao").val("buscar");
        $("#filtrosicaj").submit();
    });

    $("#gerarXls").on("click", function(){
        $("#requisicao").val("xls");
        $("#filtrosicaj").submit();
    });

    $("#limpar").on("click", function(){
        location.href="sicaj.php?modulo=principal/listar&acao=A";
    });

    $('.tabela-listagem span[data-toggle="popover"]').popover(
        {trigger: 'hover', title: 'Status', placement:'left'}
    );
});
</script>