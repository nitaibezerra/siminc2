
<script language="JavaScript" src="../includes/funcoes.js"></script>
<?  if(isset($tipo)): ?>
<script>
    $(document).ready(function(){
        $('#periodo').on('change',function(){
           if($('#periodo').val() != ''){
               window.location.href = 'acomporc.php?modulo=principal/associacao/associacao&acao=A&tipo='+$('input[name="tp"]').val()+'&periodo='+$('#periodo').val();
           }
        });
    });
</script>

<div class="well">
    <input type="hidden" name="tp" value="<?=$tipo?>">
    <div class="form-horizontal">
        <div class="form-group">
            <label for="periodo" class="col-lg-2 control-label">Período de Referência:</label>
            <div class="col-lg-10">
                <?php
                inputCombo('prfid', sprintf(retornaQueryComboPeriodo(),$_SESSION['exercicio'],$tipo), $prfid, 'periodo',array('mantemSelecaoParaUm' => false));
                ?>
            </div>
        </div>
    </div>
    <div class="">
        <button class="btn btn-success" id="buttonSend" data-url="acomporc.php?modulo=principal/associacao/associacao&acao=A&tipo=<?=$tipo?>&requisicao=new" type="button"><span class="glyphicon glyphicon-plus-sign"></span> Nova Associação</button>
    </div>
</div>
<?php
    require(dirname(__FILE__) . "/listarAssociacao.inc");
    else: 
        apresentaComboTipo();
    endif;