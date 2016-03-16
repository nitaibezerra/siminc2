<?php
/**
 * Formulário de filtro de listagem e gestão de regras.
 * $Id: formRegra.inc 78244 2014-04-01 18:26:00Z maykelbraz $
 */
require_once APPRAIZ . "includes/funcoesspo_componentes.php";
?>
<script type="text/javascript" language="javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" language="javascript">
    $(document).ready(function(){
        $('#cadastrar').on('click',function(){
            if(trim($('#periodo').val()) == ""){
                bootbox.alert('Campo de selação de Período de Referência obrigatório.');
                return false;
            }
            if($('#questao').val() == ""){
                bootbox.alert('Campo de selação de Questionário obrigatório.');
                return false;
            }
            if($('#subacoes').val() == null ){
                bootbox.alert('Campo de selação de Subações obrigatório.');
                return false;
            }
        });

        $('#sba_box').hide();
        $('#box').hide();
        $('#questao_box').hide();
        $('#periodo').on('change',function(){
            if($(this).val() == null || $(this).val() == ''){
                $('#sba_box').hide();
                $('#box').hide();
                $('#questao_box').hide();
            }else{
                $.ajax({
                    type: "POST",
                    url: window.location.href + '&ajax=questionario&func=abreSubacoes&prfid='+$(this).val(),
                    success: function(result){
                        $('#questao_box').show();
                        $('#questao_box').find('div').html(result.questao);
                        $('#questao').chosen();
                    },
                    dataType: "json"
                });
                $('#sba_box').hide();
            }
        });

        $('#questao').on('change',function(){
            if($(this).val() == null || $(this).val() == ''){
                $('#sba_box').hide();
                $('#box').hide();
            }else{
                $.ajax({
                    type: "POST",
                    url: window.location.href + '&ajax=subacao&prfid='+$('#periodo').val() + '&qstid='+$(this).val(),
                    success: function(result){
                        $('#box').show();
                        $('#sba_box').show();
                        $('#sba_box').find('div').html(result.subacao);
                        $('#subacoes').chosen();
                    },
                    dataType: "json"
                });
            }
        });
        if($('#periodo').val() != ''){
            $('#sba_box').show();
            $('#box').show();
            $('#questao_box').show();
        }

        $('[name=selecao]').on('change',function(){
            if($('[name=selecao][value=true]').is(':checked')){
                $('#subacoes option').prop('selected', true);
            }else{
                $('#subacoes option').prop('selected', false);
            }
            $('.chosen-select-no-single').trigger('chosen:updated');
        });
    });
    function abreSubacoes(){
        if($('#questao').val() == null || $('#questao').val() == ''){
            $('#sba_box').hide();
            $('#box').hide();
        }else{
            $.ajax({
                type: "POST",
                url: window.location.href + '&ajax=subacao&prfid='+$('#periodo').val() + '&qstid='+$('#questao').val(),
                success: function(result){
                    $('#box').show();
                    $('#sba_box').show();
                    $('#sba_box').find('div').html(result.subacao);
                    $('#subacoes').chosen();
                },
                dataType: "json"
            });
        }
    }
</script>
<section class="well">
    <form name="regra" id="form-regra" method="POST" class="form-horizontal" role="form" novalidate="novalidate">
        <input type="hidden" name="requisicao" id="requisicao" value="cadastroAssociacaoSubacao" />
        <input type="hidden" name="qsaid" value="<?=$dados['qsaid']?>" />

        <div class="form-group">
            <label for="tipo" class="col-lg-2 control-label">Tipo de Questionário:</label>
            <div class="col-lg-10">
                <p class="form-control-static">
                    <span class="label label-info"><?=$gerenciador->__get('typeName')?></span>
                </p>
            </div>
        </div>

        <div class="form-group">
            <label for="periodo" class="col-lg-2 control-label">Período de Referência:</label>
            <div class="col-lg-10">
                <?php
                inputCombo('prfid', sprintf(retornaQueryComboPeriodo(),$_SESSION['exercicio'],$tipo), $_REQUEST['prfid'], 'periodo',array('mantemSelecaoParaUm' => false));
                ?>
            </div>
        </div>

        <div class="form-group" id="questao_box">
            <label for="questao" class="col-lg-2 control-label">Questionário:</label>
            <div class="col-lg-10">
                <?php
                if($_REQUEST['qstid']){
                    inputCombo('qstid', sprintf(retornaQueryQuestionario(),$_REQUEST['prfid'],$tipo), $_REQUEST['qstid'], 'questao');
                }
                ?>
            </div>
        </div>

        <div id="box">
            <div class="form-group">
                <label for="" class="col-lg-2 control-label">Selecionar todas as Subações:</label>
                <div class="col-lg-10">
                    <section class="btn-group" data-toggle="buttons">
                        <label class="btn btn-default">
                            <input type="radio" name="selecao" value="true">
                            <span class="glyphicon glyphicon-thumbs-up" style="color:green"></span>
                            Sim
                        </label>
                        <label class="btn btn-default active">
                            <input type="radio" name="selecao" value="false" checked="checked">
                            <span class="glyphicon glyphicon-thumbs-down" style="color:red"></span>
                            Não
                        </label>
                    </section>
                </div>
            </div>

            <div class="form-group" id="sba_box">
                <label for="subacoes" class="col-lg-2 control-label">Subações:</label>
                <div class="col-lg-10">
                    <?php
                    if($_REQUEST['prfid']){
                        inputCombo('sbacod[]', sprintf(retornaQueryComboSubacao(),$_REQUEST['prfid']), $questionarioModelo->capturaSubacoesPorAssociacao($_REQUEST['prfid'],$_REQUEST['qstid']), 'subacoes', array('multiple' => true));
                    }
                    ?>
                </div>
            </div>
        </div>

        <button type="button" class="btn btn-warning" id="buttonBack" data-url="acomporc.php?modulo=principal/associacao/associacao&acao=A&tipo=<?=$tipo?>"><span class="glyphicon glyphicon-arrow-left"></span> Voltar</button>
        <button type="submit" class="btn btn-success" id="cadastrar"><span class="glyphicon glyphicon-plus-sign"></span> <?= $_GET['requisicao'] == 'new' ? 'Cadastrar' : 'Atualizar' ?></button>
    </form>
</section>