<?php
/**
 * Formulário de filtro de listagem e gestão de regras.
 * $Id: formRegra.inc 78244 2014-04-01 18:26:00Z maykelbraz $
 */
require_once APPRAIZ . "includes/funcoesspo_componentes.php";

if(isset($_GET['id'])){
    $dados = $questionarioModelo->findById($_GET['id']);
}

?>

<script type="text/javascript" language="javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" language="javascript">
    $(document).ready(function(){
        $('#cadastroQuestionario').on('click',function(){
            if(trim($('#questionario').val()) == ''){
                bootbox.alert('Campo Questionário obrigatório.');
                return false;
            }
            if($('#periodoReferencia').val().trim() == '' ){
                bootbox.alert('Campo Período de Referência obrigatório.');
                return false;
            }
        });
    });
</script>
<section class="well">
    <form name="regra" id="form-regra" method="POST" class="form-horizontal" role="form" novalidate="novalidate">
        <input type="hidden" name="requisicao" id="requisicao" value="<?= $_GET['requisicao'] == 'new' ? 'novoQuestionario' : 'atualizaQuestionario' ?>" />
        <input type="hidden" name="qsttipo" value="<?=$tipo?>" />
        <input type="hidden" name="qstid" value="<?=$dados['qstid']?>" />
        
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
                <?php
                $complemento =  "placeholder = 'Digite o nome do Questionário.' required";
                inputTexto('qstnome', $dados['qstnome'], 'questionario', $limite, FALSE, array('complemento' => $complemento));
                ?>
            </div>
        </div>
        
        <div class="form-group">
            <label for="periodoReferencia" class="col-lg-2 control-label">Período de Referência:</label>
            <div class="col-lg-10">
                <?php
                $query = <<<DML
                    SELECT 
                        prfid as codigo, 
                        prftitulo as descricao 
                    FROM acomporc.periodoreferencia 
                    WHERE prsano = '{$_SESSION['exercicio']}' AND prftipo = '{$tipo}';
DML;
                inputCombo('prfid', $query, $dados['prfid'], 'periodoReferencia');
                ?>
            </div>
        </div>
        
        <button type="button" class="btn btn-warning" id="buttonBack" data-url="acomporc.php?modulo=principal/questionario/questionario&acao=A&tipo=<?=$tipo?>"><span class="glyphicon glyphicon-arrow-left"></span> Voltar</button>
        <button type="submit" class="btn btn-success" id="cadastroQuestionario"><span class="glyphicon glyphicon-plus-sign"></span> <?= $_GET['requisicao'] == 'new' ? 'Cadastrar' : 'Atualizar' ?></button>
    </form>
</section>