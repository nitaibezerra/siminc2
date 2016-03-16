<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 11/08/2015
 * Time: 15:17
 */

global $db;

if ( $_POST['action'] == 'relatorio' ){
    include("relatorio_GabSecExecutiva_resultado.inc");
    exit;
}

include  APPRAIZ."includes/cabecalho.inc";
?>
<div class="container">
    <form id="formulario" name="formulario" method="post" class="form-horizontal">
        <input type="hidden" name="action" value="relatorio"/>

        <div class="col-md-12">
            <div class="well">
                <legend>Demonstrativo de Documentos</legend>

                <div class="form-group">
                    <label class="col-lg-4 col-md-4 control-label">Emissão:</label>
                    <div class="col-lg-5 col-md-5">
                        <select name="opcRel" id="opcRel" class="form-control" data-placeholder="Selecione">
                            <option value="">Selecione</option>
                            <option value="1">Sem Atraso</option>
                            <option value="2">Com Atraso</option>
                            <option value="3">Com Reiteração</option>
                        </select>
                    </div>
                </div>

                <div class="form-group">
                    <label class="col-lg-4 col-md-4 control-label">Período:</label>
                    <div class="col-lg-2 col-md-2" style="padding-top: 5px;">
                        <input id="dtini" name="dtini" type="text" class="form-control" placeholder="dd/mm/aaaa" value="">
                    </div>
                    <div class="col-lg-2 col-md-2" style="padding-top: 5px;">
                        <input id="dtfim" name="dtfim" type="text" class="form-control" placeholder="dd/mm/aaaa" value="">
                    </div>
                </div>

                <div class="text-center">
                    <button id="btnGeraRel" title="Pesquisar" class="btn btn-primary" onclick="geraRelatorio()" type="button"><span
                            class="glyphicon"></span> Gerar Relatório
                    </button>
                </div>

            </div>
        </div>
    </form>
</div>
<script>
    setTimeout(function(){
        $('#dtini').datepicker();
        $('#dtini').mask('99/99/9999');
        $('#dtfim').datepicker();
        $('#dtfim').mask('99/99/9999');
    });

    function geraRelatorio(){
        if($('#opcRel').val() != '') {
            if ($('#dtini').val() != '' && $('#dtfim').val() != '') {
                var janela = window.open('', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
                document.formulario.target = 'relatorio';
                document.formulario.submit();
            } else {
                alert('Informe o período corretamente.');
            }
        } else {
            alert('Informe a opção da emissão.');
        }
    }
</script>
