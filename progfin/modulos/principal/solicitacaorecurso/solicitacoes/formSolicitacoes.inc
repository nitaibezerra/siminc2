<?php
/**
 * Formulário de filtragem das solicitações.
 * $Id: formSolicitacoes.inc 98758 2015-06-17 19:49:01Z werteralmeida $
 */
?>
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script type="text/javascript">
    $(document).ready(function () {
        $('#new').click(function () {
            validarFormulario(['unicod'], 'filtrosolicitacoes', 'criarNovaSolicitacao');
        });
        $('#clear').click(function () {
            $('#requisicao').val('limparFiltros');
            $('#filtrosolicitacoes').submit();
        });
        $('#search').click(function () {
            var uri = "progfin.php?modulo=principal/solicitacaorecurso/solicitacoes&acao=A";

            var value;
            uri += (value = $('#unicod').val()) ? '&unicod=' + value : '';
            uri += (value = $('#ungcod').val()) ? '&ungcod=' + value : '';
            uri += (value = $('#clpid').val()) ? '&clpid=' + value : '';
            uri += (value = $('#fdsid').val()) ? '&fdsid=' + value : '';
            uri += (value = $('#ctgcod').val()) ? '&ctgcod=' + value : '';


            uri += (value = $('input[type="radio"][name="tpreg"]:checked').val()) ? '&tpreg=' + value : '';
            uri += (value = $('input[type="radio"][name="esdid"]:checked').val()) ? '&esdid=' + value : '';
            uri += (value = $('input[type="radio"][name="tipotabela"]:checked').val()) ? '&tipotabela=' + value : '';

            window.location = uri;
        });
    });
</script>
<style type="text/css">
    #modal-alert .modal-body ul{text-align:left;margin-top:5px;list-style:circle}
</style>
<form name="filtrosolicitacoes" id="filtrosolicitacoes" method="POST" role="form" class="form-horizontal">
    <input type="hidden" name="requisicao" id="requisicao" />
    <div class="form-group row">
        <label class="control-label col-md-2" for="unicod">Unidade Orçamentária (UO):</label>
        <div class="col-md-10">
            <?php
            $whereAdicionalUO = array();
            if (in_array(PFL_UO_EQUIPE_TECNICA, pegaPerfilGeral($_SESSION['usucpf']))) {
                $whereAdicionalUO[] = 'unicod IN(' . pegaResposabilidade($_SESSION['usucpf'], PFL_UO_EQUIPE_TECNICA, 'unicod') . ')';
            }
            inputComboUnicod($dados['unicod'], $whereAdicionalUO);
            ?>
        </div>
    </div>
    <div class="form-group row">
        <label class="control-label col-md-2" for="unicod">Unidade Gestora (UG):</label>
        <div class="col-md-10">
            <?php
            inputComboUngcod($dados['ungcod'], $whereAdicionalUO);
            ?>
        </div>
    </div>
    <div class="form-group row">
        <label class="control-label col-md-2" for="fdsid">Fonte:</label>
        <div class="col-md-10">
            <?php inputComboFonteDetalhada($dados['fdsid'], $_SESSION['exercicio']); ?>
        </div>
    </div>
    <div class="form-group row">
        <label class="control-label col-md-2" for="ctgcod">Cat. Gasto:</label>
        <div class="col-md-10">
            <?php inputComboGND($dados[ctgcod]); ?>
        </div>
    </div>
    <div class="form-group row">
        <label class="control-label col-md-2" for="clpid">Classificação do Pedido:</label>
        <div class="col-md-10">
            <?php
            $classificacoes = explode(',', $_REQUEST['clpid']);
            $opcoes['multiple'] = true;
            $opcoes['titulo'] = "Selecione a(s) Classificação(ões)";
            inputComboClassificacao($classificacoes, 'TODOS', $opcoes);
            ?>
        </div>
    </div>
    <div class="form-group row">
        <label class="control-label col-md-2">Situação:</label>
        <div class="col-md-10">
            <?php
            $opcaoMarcada = isset($_GET['esdid']) ? $_GET['esdid'] : '';
            if ($_REQUEST['tipotabela'] == 'detalhada') {
                inputChoices('esdid', situacaoDocumentosLiberacao(), $opcaoMarcada, 'esdid');
            } else {

                inputChoices('esdid', situacaoDocumentosPedido(), $opcaoMarcada, 'esdid');
            }
            ?>
        </div>
    </div>
    <div class="form-group row">
        <label class="control-label col-md-2">Tipo de Resultado:</label>
        <div class="col-md-10">
            <?php
            $opcaoMarcada = isset($_REQUEST['tipotabela']) ? $_REQUEST['tipotabela'] : 'normal';
            inputChoices('tipotabela', array('Normal' => 'normal', 'Detalhada' => 'detalhada'), $opcaoMarcada, 'tipotabela');
            ?>
        </div>
    </div>
    <hr />
    <br />
    <button type="button" class="btn btn-warning" id="clear">Limpar</button>
    <button type="button" class="btn btn-success" id="new">Novo</button>
    <button type="button" class="btn btn-primary" id="search">Buscar</button>
</form>