<?php
require_once APPRAIZ . "includes/funcoesspo_componentes.php";

if(isset($_GET['id'])){
    $dados = buscarAssociacao($_GET['id'], true);
}
?>
<script type="text/javascript" language="javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" language="javascript">
    $(document).ready(function(){
        $('#cadastrar').on('click',function(){
            if($('#periodo').val() == ""){
                bootbox.alert('Campo de selação de Período de Referência obrigatório.');
                return false;
            }
            if($('#questao').val() == ""){
                bootbox.alert('Campo de selação de Questionário obrigatório.');
                return false;
            }
            if($('#acoes').val() == null ){
                bootbox.alert('Campo de selação de Ações obrigatório.');
                return false;
            }
        });
        $('#acao_box').hide();
        $('#box').hide();
        $('#questao_box').hide();
        $('#periodo').on('change',function(){
            if($(this).val() == null || $(this).val() == ''){
                $('#acao_box').hide();
                $('#box').hide();
                $('#questao_box').hide();
            }else{
                $.ajax({
                    type: "POST",
                    url: window.location.href + '&ajax=questionario&func=abreAcoes&prfid='+$(this).val(),
                    success: function(result){
                        //$('#acao_box').show();
                        //$('#acao_box').find('div').html(result.periodo);
                        $('#questao_box').show();
                        $('#questao_box').find('div').html(result.questao);
                        //$('#acoes').chosen();
                        $('#questao').chosen();
                    },
                    dataType: "json"
                });
                $('#acao_box').hide();
            }
        });
         $('#questao').on('change',function(){
            if($(this).val() == null || $(this).val() == ''){
                $('#acao_box').hide();
                $('#box').hide();
            }else{
                $.ajax({
                    type: "POST",
                    url: window.location.href + '&ajax=acao&prfid='+$('#periodo').val() + '&qstid='+$(this).val(),
                    success: function(result){
                        $('#box').show();
                        $('#acao_box').show();
                        $('#acao_box').find('div').html(result.acao);
                        $('#acoes').chosen();
                    },
                    dataType: "json"
                });
            }
        });
        if($('#periodo').val() != ''){
            $('#acao_box').show();
            $('#box').show();
            $('#questao_box').show();
        }

        $('[name=selecao]').on('change',function(){
            if($('[name=selecao][value=true]').is(':checked')){
                $('#acoes option').prop('selected', true);
            }else{
                $('#acoes option').prop('selected', false);
            }
            $('.chosen-select-no-single').trigger('chosen:updated');
        });
    });
    function abreAcoes(){
        if($('#questao').val() == null || $('#questao').val() == ''){
            $('#acao_box').hide();
            $('#box').hide();
        }else{
            $.ajax({
                type: "POST",
                url: window.location.href + '&ajax=acao&prfid='+$('#periodo').val() + '&qstid='+$('#questao').val(),
                success: function(result){
                    $('#box').show();
                    $('#acao_box').show();
                    $('#acao_box').find('div').html(result.acao);
                    $('#acoes').chosen();
                },
                dataType: "json"
            });
        }
    }
</script>
<section class="well">
    <form name="regra" id="form-regra" method="POST" class="form-horizontal" role="form" novalidate="novalidate">
        <input type="hidden" name="requisicao" id="requisicao" value="cadastroAssociacaoTCU" />
        <input type="hidden" name="qrtid" value="<?=$dados['qrtid']?>" />

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
                if($_REQUEST['prfid']){
                    inputCombo('qstid', sprintf(retornaQueryQuestionario(),$_REQUEST['prfid'],$tipo), $_REQUEST['qstid'], 'questao');
                }
                ?>
            </div>
        </div>

        <div id="box">
            <div class="form-group">
                <label for="" class="col-lg-2 control-label">Selecionar todas as Ações:</label>
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
            
            <div class="form-group" id="acao_box" >
                <label for="acoes" class="col-lg-2 control-label">Ações:</label>
                <div class="col-lg-10">
                    <?php
                    if($_REQUEST['prfid']){
                        inputCombo('acacod[]', sprintf(retornaQueryComboRelatorioTCU(),$_REQUEST['prfid']), $questionarioModelo->capturaTCUPorAssociacao($_REQUEST['prfid'],$_REQUEST['qstid']), 'acoes', array('multiple' => true, 'titulo' => 'Selecione um ou mais itens.'));
                    }
                    ?>
                </div>
            </div>
        </div>

        <button type="button" class="btn btn-warning" id="buttonBack" data-url="acomporc.php?modulo=principal/associacao/associacao&acao=A&tipo=<?=$tipo?>"><span class="glyphicon glyphicon-arrow-left"></span> Voltar</button>
        <button type="submit" class="btn btn-success" id="cadastrar"><span class="glyphicon glyphicon-plus-sign"></span> <?= $_GET['requisicao'] == 'new' ? 'Cadastrar' : 'Atualizar' ?></button>
    </form>
</section>