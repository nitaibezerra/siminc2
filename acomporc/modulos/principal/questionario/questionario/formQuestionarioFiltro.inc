<script language="JavaScript" src="../includes/funcoes.js"></script>

<?  if(isset($tipo)): ?>
<div class="well">    
    <div class="">
        <button class="btn btn-success" id="buttonSend" data-url="acomporc.php?modulo=principal/questionario/questionario&acao=A&tipo=<?=$tipo?>&requisicao=new" type="button"><span class="glyphicon glyphicon-plus-sign"></span> Novo Questionário</button>
    </div>
</div>
<?php
    require(dirname(__FILE__) . "/listarQuestionario.inc");
    else: 
        apresentaComboTipo();
    endif;
?>