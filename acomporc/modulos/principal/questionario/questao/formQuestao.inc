<?php
/**
 * Formulário de filtro de listagem e gestão de regras.
 * $Id: formRegra.inc 78244 2014-04-01 18:26:00Z maykelbraz $
 */
require_once APPRAIZ . "includes/funcoesspo_componentes.php";

if(isset($_GET['id'])){
    $dados = $modeloQuestao->buscarQuestao($_GET['id']);
}

?>

<script type="text/javascript" language="javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" language="javascript">
    $(document).ready(function(){
        $('#cadastroQuestao').on('click',function(){
            if($('#questao').val().trim() == '' ){
                bootbox.alert('Campo Pergunta obrigatório.');
                return false;
            }
        });
    });
</script>
<section class="well">
    <form name="regra" id="form-regra" method="POST" class="form-horizontal" role="form" novalidate="novalidate">
        <input type="hidden" name="requisicao" id="requisicao" value="<?= $_GET['requisicao'] == 'new' ? 'cadastroQuestao' : 'atualizaQuestao' ?>" />
        <input type="hidden" name="qstid" value="<?=$questionario?>" />
        <input type="hidden" name="qprid" value="<?=$dados['qprid']?>" />
        
        <div class="form-group">
            <label for="tipo" class="col-lg-2 control-label">Tipo de Questionário:</label>
            <div class="col-lg-10">
                <p class="form-control-static">
                    <span class="label label-info"><?=$gerenciador->__get('typeName')?></span>
                </p>
            </div>
        </div>
        
        <div class="form-group">
            <label for="questionario" class="col-lg-2 control-label">Questionário:</label>
            <div class="col-lg-10">
                <p class="form-control-static">
                    <?=$nomeQuestionario?>
                </p>
            </div>
        </div>
        
        <div class="form-group">
            <label for="questao" class="col-lg-2 control-label">Pergunta:</label>
            <div class="col-lg-10">
                <?php
                $complemento =  array('required' => true);
                inputTextArea('qprpergunta', $dados['qprpergunta'], 'questao', 5000, array('complemento' => $complemento));
                ?>
            </div>
        </div>
        
        <div class="form-group">
            <label for="caracteres" class="col-lg-2 control-label">Número de carácteres:</label>
            <div class="col-lg-10">
                <?php
                $complemento =  "placeholder = 'Digite o número de carácteres máximo para resposta.'";         
                inputTexto('qprnumcaracteres', $dados['qprnumcaracteres'], 'caracteres', $limite, FALSE, array('masc' => '#####','complemento' => $complemento))
                ?>
            </div>
        </div>
        
        <div class="form-group">
            <label for="obrigatorio" class="col-lg-2 control-label">Pergunta obrigatória:</label>
            <div class="col-lg-10">                       
                <section class="btn-group" data-toggle="buttons">
                    <label class="btn btn-default <?= ($dados['qprobrigatorio'] == 't') ? 'active' : '' ?>">
                        <input type="radio" name="qprobrigatorio" value="t" <?= ($dados['qprobrigatorio'] == 't') ? 'checked="checked"' : '' ?>>
                        <span class="glyphicon glyphicon-thumbs-up" style="color:green"></span>
                        Sim
                    </label>
                    <label class="btn btn-default <?= ($dados['qprobrigatorio'] != 't') ? 'active' : '' ?>">
                        <input type="radio" id="obrigatorio" name="qprobrigatorio" value="f" <?= ($dados['qprobrigatorio'] != 't') ? 'checked="checked"' : '' ?>>
                        <span class="glyphicon glyphicon-thumbs-down" style="color:red"></span>
                        Não
                    </label>
                </section>
            </div>                        
        </div>
        
        <div class="form-group">
            <label for="resposta" class="col-lg-2 control-label">Resposta padrão:</label>
            <div class="col-lg-10">
                <?php
                inputTextArea('qprrespostapadrao', $dados['qprrespostapadrao'], 'resposta', 5000, array());
                ?>
            </div>
        </div>
        
        <div class="form-group">
            <label for="ordem" class="col-lg-2 control-label">Ordem:</label>
            <div class="col-lg-10">
                <?php
                $complemento =  "placeholder = 'Digite o número correspondente à ordem.'";
                inputTexto('qprordem', $dados['qprordem'], 'ordem', 2, FALSE, array('masc' => '##','complemento' => $complemento))
                ?>
            </div>
        </div>
        
        <button type="button" class="btn btn-warning" id="buttonBack" data-url="acomporc.php?modulo=principal/questionario/questao&acao=A&questionario=<?=$questionario?>&tipo=<?=$tipo?>"><span class="glyphicon glyphicon-arrow-left"></span> Voltar</button>
        <button type="submit" class="btn btn-success" id="cadastroQuestao"><span class="glyphicon glyphicon-plus-sign"></span> <?= $_GET['requisicao'] == 'new' ? 'Cadastrar' : 'Atualizar' ?></button>
    </form>
</section>