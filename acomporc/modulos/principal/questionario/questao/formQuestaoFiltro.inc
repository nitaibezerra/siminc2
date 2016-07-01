
<script language="JavaScript" src="../includes/funcoes.js"></script>

<div class="well">
    <h3 class="">Questionário: <small><?=$nomeQuestionario?></small></h3>
    <br>
    <div class="">
        <button class="btn btn-warning" id="buttonBack" data-url="acomporc.php?modulo=principal/questionario/questionario&acao=A&tipo=<?=$tipo?>" type="button"><span class="glyphicon glyphicon-arrow-left"></span> Voltar aos Questionários</button>
        <button class="btn btn-success" id="buttonSend" data-url="acomporc.php?modulo=principal/questionario/questao&acao=A&questionario=<?=$questionario?>&tipo=<?=$tipo?>&requisicao=new" type="button"><span class="glyphicon glyphicon-plus-sign"></span> Nova Questão</button>
    </div>
</div>
<?php
require(dirname(__FILE__) . "/listarQuestao.inc");
?>