<?
if (isset ( $_REQUEST ['fnc'] )) {
    switch ($_REQUEST ['fnc']) {		
        case 'carregaPergunta' :
            $frmData = $_REQUEST ['fnc'] ( $_REQUEST );
            break;
        case 'excluiPergunta' :
            $_REQUEST ['fnc'] ( $_REQUEST ['tqpid'] );
            break;
        case 'salvarPergunta' :
            $_REQUEST ['fnc'] ( $_POST );
            break;
        default :
    }
}

?>
<script>
    function inserirPerguntas() {
        if (jQuery('#tqppergunta').val() == '') {
            alert('Pergunta em branco');
            return false;
        }
        
        if (jQuery("input[name='tqpid']").val()!= '') {
            jQuery("#act").attr({
                name:"fnc"
              , value:"salvarPergunta"
            });
        }
        document.getElementById('formperguntas').submit();
    }

    function alterarPergunta(id_pergunta, id_questionario) {
        var prefixUrl = 'planacomorc.php?modulo=principal/questionariorelatoriogestao/gerenciarquestionario'
        ,paramsUrl = '&acao=A&aba=cadastrarperguntas&fnc=carregaPergunta&tqtid=' + id_questionario + '&tqpid=' + id_pergunta;
        window.location = prefixUrl + paramsUrl;
    }

    function excluirPergunta(id_pergunta) {
        if(confirm('Você realmente deseja excluir esta pergunta ?')){
            var prefixUrl = 'planacomorc.php?modulo=principal/questionariorelatoriogestao/gerenciarquestionario'
              , paramsUrl = '&acao=A&aba=cadastrarperguntas&fnc=excluiPergunta&tqpid=' + id_pergunta;

            window.location = prefixUrl + paramsUrl;
        }
    }
    
    function limpar(){
        window.location = 'planacomorc.php?modulo=principal/questionariorelatoriogestao/gerenciarquestionario&acao=A&aba=cadastrarperguntas';
    }
</script>
<section class="col-md-12">
    <section class="well">
        <form class="form-horizontal" method="post" name="formperguntas" id="formperguntas">
            <input type="hidden" id="act" name="requisicao"	value="inserirPerguntas"> 
            <input type="hidden" name="tqpid" value="<?= !isset($frmData['tqpid'])?'':$frmData['tqpid']; ?>">			
            <section class="form-group">
                <label class="control-label col-md-2" for="tqppergunta">Pergunta:</label>
                <section class="col-md-10">                    
                    <? inputTextArea('tqppergunta', $frmData['tqppergunta'], 'tqppergunta', 3000,array('rows'=>2)); ?>
                </section>
            </section>	

            <section class="form-group">
                <section class="col-md-offset-2">
                    <input type="button" class="btn btn-primary" name="salvar" value="Salvar" onclick="inserirPerguntas();">
                    <? if($_GET['tqpid']){ ?>
                    <input type="button" class="btn btn-success" value="Novo" onclick="limpar();">                    
                    <? } ?>
                </section>
            </section>
        </form>        
    </section>   
       
        
    <?
    $sql = <<<DML
        SELECT			
            tqpid AS id_pergunta,
            tqtid AS id_questionario, 
            tqppergunta AS pergunta			
        FROM planacomorc.tcuquestpergunta
        WHERE tqtid='{$_SESSION ['planacomorc'] ['tqtid']}'
            AND tqpestado LIKE('A')			
DML;
    $cabecalho = array ("Pergunta");
    $listagem = new Simec_Listagem();
    $listagem->setQuery($sql);
    $listagem->setCabecalho($cabecalho);
    $listagem->setAcoes(array('edit'=> array('func' => 'alterarPergunta','extra-params'=>array('id_questionario')), 'delete' => 'excluirPergunta'));
    $listagem->esconderColuna('id_questionario');
    $listagem->addCallbackDeCampo('pergunta', 'alinhaParaEsquerda');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    ?>
</section>
