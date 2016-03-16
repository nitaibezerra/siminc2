<script>
    $(document).ready(function(){
        $('#periodo').on('change',function(){
           if($('#periodo').val() != ''){
               window.location.href = 'acomporc.php?modulo=principal/acompanhamento/subacao&acao=A&tipo='+$('input[name="tp"]').val()+'&periodo='+$('#periodo').val();
           }
        });
    });
</script>
<section class="well">
    <form class="form-horizontal" method="get" action="">
        <input type="hidden" name="tp" value="<?=$tipo?>">
        <div class="form-group">
            <label for="periodo" class="col-lg-2 control-label">Período de Referência:</label>
            <div class="col-lg-10">
                <?php
                inputCombo('prfid', sprintf(retornaQueryComboPeriodo(),$_SESSION['exercicio'],$tipo), $prfid, 'periodo',array('mantemSelecaoParaUm' => false));
                ?>
            </div>
        </div>
        <div class="form-group">
            <div class="col-lg-2"></div>
            <div class="col-lg-10"></div>
        </div>
    </form>
</section>
<?php
require(dirname(__FILE__) . "/listarAcompanhamento.inc");
?>

