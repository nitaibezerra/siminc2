<?php
/**
 * Filtros da listagem de alteração orçamentaria
 * $Id: formPedidos.inc 96806 2015-04-30 20:06:52Z maykelbraz $
 */

/*Tratamento de perfis*/
if (in_array(PFL_UO_EQUIPE_TECNICA, $perfis)) {
    $where[] = $whereUO =' uni.unicod IN(' . pegaResposabilidade($_SESSION['usucpf'], PFL_UO_EQUIPE_TECNICA, 'unicod') . ')';
    $whereUO = " AND {$whereUO}";
}
?>
<style type="text/css">
    #modal-alert .modal-body ul{text-align:left;margin-top:5px;list-style:circle}
</style>
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script type="text/javascript" language="javascript">
$(document).ready(function(){
    $('#limparFiltros').click(function(){
        window.location='altorc.php?modulo=principal/pedido&acao=A&clean';
    });

    <?php if(podeCriarPedido()): ?>
    $('#novoPedidoAlteracaoCredito').click(function(){
        // -- Alterando endereço da requisição
        $('#alteracaoorcamentaria').attr('action', 'altorc.php?modulo=principal/pedido/inicio&acao=A');
        // -- Validando os itens do formulario obrigatorios para criar um novo pedido
        validarFormulario(['mcrid', 'tcrid', 'unicod'], 'alteracaoorcamentaria', 'novoPedido');
        // -- Retirando endereço da requisição para evitar erro no botão de busca
        $('#alteracaoorcamentaria').attr('action', '');
    });
    <?php endif; ?>
});

/**
 * Carrega as opções do combo de tipo de credito, com base no momento escolhido.
 */
function carregaTipoCredito()
{
    // -- requisicao de dados do tipo de crédito, por momento
    $.post(
        window.location,
        {'requisicao':'tipoCredito', 'dados[mcrid]':$('#mcrid').val()},
        function(data){
            var _data = JSON.parse(data);
            // -- limpando o select
            $('#tcrid').find('option').remove().end().append($('<option>', {value: '', text:'Selecione um Tipo de crédito'}));
            if (_data) {
                $.each(_data, function(index, value){
                    $('#tcrid').append($('<option>', {value:value.codigo, text:value.descricao}));
                });
            }
            $('#tcrid.chosen-select').trigger('chosen:updated');
        },
        'text'
    );
}
</script>
<form name="alteracaoorcamentaria" id="alteracaoorcamentaria" method="POST" role="form">
    <input type="hidden" name="requisicao" id="requisicao" value="" />
    <div class="form-group row">
        <div class="col-md-2">
            <label class="control-label" for="mcrid">Momento de crédito:</label>
        </div>
        <div class="col-md-10">
        <?php
        #$mcrid = $_SESSION['altorc']['filtro_params']['mcrid'];

        if(isset($_REQUEST['dados']['mcrid']))
            $mcrid =  $_REQUEST['dados']['mcrid'];
        else if(isset($_SESSION['altorc']['filtro_params']['mcrid']))
            $mcrid = $_SESSION['altorc']['filtro_params']['mcrid'];
        else
            list($mcrid,) = momentoDeCreditoAtual ($_SESSION['exercicio'], null, true);

        $sqlMomentoCredito = <<<DML
            SELECT mcr.mcrid AS codigo,
                mcr.mcrdsc AS descricao
            FROM altorc.momentocredito mcr
            WHERE mcr.mcrstatus = 'A'
                AND mcr.mcrano = '%s'
            ORDER BY mcr.mcraltinicio DESC
DML;

        $sqlMomentoCredito = sprintf($sqlMomentoCredito, $_SESSION['exercicio']);
        $db->monta_combo('dados[mcrid]', $sqlMomentoCredito, 'S', 'Selecione um Momento de crédito', 'carregaTipoCredito', null, null, null, 'N', 'mcrid', null, $mcrid , null, 'class="form-control chosen-select" style="width=100%"');
        ?>
        </div>
    </div>
    <div class="form-group row">
        <div class="col-md-2">
            <label class="control-label" for="tcrid">Tipo de crédito:</label>
        </div>
        <div class="col-md-10">
        <?php
        $opcoesTcrid = array();
        if(isset($_REQUEST['dados']['tcrid']))
            $tcrid = $_REQUEST['dados']['tcrid'];
        else if(isset($_SESSION['altorc']['filtro_params']['tcrid']))
            $tcrid = $_SESSION['altorc']['filtro_params']['tcrid'];

        if ($tcrid || $mcrid)
            $opcoesTcrid = carregarTipoCredito($_SESSION['exercicio'], $mcrid, $asJSON = false);

        $db->monta_combo('dados[tcrid]', $opcoesTcrid, 'S', 'Selecione um Tipo de crédito', null, null, null, null, 'N', 'tcrid', null, $tcrid, null, 'class="form-control chosen-select" style="width=100%"');
        ?>
        </div>
    </div>
    <div class="form-group row">
        <div class="col-md-2">
            <label class="control-label" for="unicod">Unidade Orçamentária (UO):</label>
        </div>
        <div class="col-md-10">
        <?php
        if(isset($_REQUEST['dados']['unicod']))
            $unicod = $_REQUEST['dados']['unicod'];
        else if($_SESSION['altorc']['filtro_params']['unicod'])
            $unicod = $_SESSION['altorc']['filtro_params']['unicod'];

        $sql = <<<DML
            SELECT
                uni.unicod AS codigo,
                uni.unicod || ' - ' || unidsc AS descricao
            FROM public.unidade uni
            WHERE uni.unistatus = 'A'
                AND EXISTS (SELECT 1
                    FROM altorc.snapshotacao sna
                    WHERE sna.unicod = uni.unicod
                        AND sna.snaexercicio = '%d')
            {$whereUO}
            ORDER BY uni.unicod
DML;
        $sql = sprintf($sql, $_SESSION['exercicio']);
        $db->monta_combo('dados[unicod]', $sql, 'S', 'Selecione uma Unidade', null, null, null, null, 'N', 'unicod', null, $unicod, null, 'class="form-control chosen-select" style="width=100%"');
        ?>
        </div>
    </div>
    <div class="form-group row">
        <?
        if(isset($_REQUEST['dados']['wrfstatus']))
            $wrfstatus = $_REQUEST['dados']['wrfstatus'];
        else if(isset($_SESSION['altorc']['filtro_params']['wrfstatus']))
            $wrfstatus = $_SESSION['altorc']['filtro_params']['wrfstatus'];
        else
            $wrfstatus = "todos";
        ?>
        <label for="inputName" class="col-lg-2 control-label">Status do Pedido:</label>
        <div class="col-md-10">
            <div class="btn-group" data-toggle="buttons">
                <label class="btn btn-default <?php if ($wrfstatus == 'todos'){echo "active";}?>" >
                    <input type="radio" name="dados[wrfstatus]" id="wrfstatus_todos" value="todos"
                           <?php if ($wrfstatus == 'todos'){echo "checked=checked";}?>/>Todos
                </label>
                <label class="btn btn-default <?php if ($wrfstatus == STDOC_EM_PREENCHIMENTO){echo "active";}?>">
                    <input type="radio" name="dados[wrfstatus]" <?php if ($wrfstatus == STDOC_EM_PREENCHIMENTO){echo "checked=checked";}?>
                           id="wrfstatus_<?php echo STDOC_EM_PREENCHIMENTO; ?>"
                           value="<?php echo STDOC_EM_PREENCHIMENTO; ?>" />Em Preenchimento UO
                </label>
                <label class="btn btn-default <?php if ($wrfstatus == STDOC_ANALISE_SPO){echo "active";}?>">
                    <input type="radio" name="dados[wrfstatus]" <?php if ($wrfstatus == STDOC_ANALISE_SPO){echo "checked=checked";}?>
                           id="wrfstatus_<?php echo STDOC_ANALISE_SPO; ?>"
                           value="<?php echo STDOC_ANALISE_SPO; ?>" />Análise SPO
                </label>
                <label class="btn btn-default <?php if ($wrfstatus == STDOC_AJUSTE_UO){echo "active";}?>">
                    <input type="radio" name="dados[wrfstatus]" <?php if ($wrfstatus == STDOC_AJUSTE_UO){echo "checked=checked";}?>
                           id="wrfstatus_<?php echo STDOC_AJUSTE_UO; ?>"
                           value="<?php echo STDOC_AJUSTE_UO; ?>" />Ajuste UO
                </label>
                <label class="btn btn-default <?php if ($wrfstatus == STDOC_CADASTRAR_SIOP){echo "active";}?>">
                    <input type="radio" name="dados[wrfstatus]" <?php if ($wrfstatus == STDOC_CADASTRAR_SIOP){echo "checked=checked";}?>
                           id="wrfstatus_<?php echo STDOC_CADASTRAR_SIOP; ?>"
                           value="<?php echo STDOC_CADASTRAR_SIOP; ?>" />Confirmado no SIOP
                </label>
            </div>
        </div>
    </div>
    <div class="form-group row">
        <label for="inputName" class="col-lg-2 control-label">Status no SIOP:</label>
        <div class="col-md-10">
            <div class="btn-group" data-toggle="buttons">
                <?php
                if(isset($_REQUEST['dados']['retornosiop']))
                    $retornosiop = $_REQUEST['dados']['retornosiop'];
                else if(isset($_SESSION['altorc']['filtro_params']['retornosiop']))
                    $retornosiop = $_SESSION['altorc']['filtro_params']['retornosiop'];
                else
                    $retornosiop = "todos";
                ?>
                <label class="btn btn-default <?php
                    if ($retornosiop  == 'todos') {
                        echo 'active';
                    }
                    ?>"> <input type="radio" name="dados[retornosiop]" id="retornosiop_todos" value="todos" />
                    Todos
                </label>
                <label
                    class="btn btn-default <?php
                    if ($retornosiop == 'OK') {
                        echo 'active';
                    }
                    ?>">
                    <input type="radio" name="dados[retornosiop]"
                         id="retornosiop_OK" value="OK"
                         <?php
                         if ($retornosiop == 'OK') {
                             echo 'checked=checked';
                         }
                         ?> /> <span class="glyphicon glyphicon-thumbs-up" style="color:green"></span>
                </label>
                <label
                    class="btn btn-default <?php
                    if ($retornosiop == 'ERRO') {
                        echo 'active';
                    }
                    ?>"> <input type="radio" name="dados[retornosiop]"
                         id="retornosiop_ERRO" value="ERRO"
                         <?php
                         if ($retornosiop == 'ERRO') {
                             echo 'checked=checked';
                         }
                         ?> /> <span class="glyphicon glyphicon-thumbs-down" style="color:red"></span>
                </label>
            </div>
        </div>
    </div>
    <button type="submit" class="btn btn-primary">Buscar</button>
    <button type="reset" class="btn btn-warning" id="limparFiltros">Limpar</button>
    <?php if (podeCriarPedido()): ?>
    <button type="button" class="btn btn-success" id="novoPedidoAlteracaoCredito">Novo Pedido</button>
    <?php endif; ?>
</form>
