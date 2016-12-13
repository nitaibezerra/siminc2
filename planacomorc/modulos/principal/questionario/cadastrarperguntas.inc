<?php
/**
 * Cadastramento de perguntas do questionário de acompanhamento das ações.
 * $Id: cadastrarperguntas.inc 92293 2015-01-02 18:05:27Z lindalbertofilho $
 */
$strSQL = <<<DML
    SELECT
      coalesce(sum(length(mqpdescricao)+mqprespnumcaracteres)+1,0) as tot
    FROM planacomorc.monqstperguntas
    WHERE qstid = '%s'
DML;
$stmt = sprintf($strSQL, $_SESSION['planacomorc']['qstid']);
$qsttotal = $db->pegaUm($stmt);

if($_GET['fnc'] && $_GET['fnc'] == 'carregaPergunta'){
    $strSQL = <<<DML
        SELECT
            COALESCE(length(mqpdescricao),0) as descricao,      
            COALESCE(mqprespnumcaracteres,0) as resposta
        FROM planacomorc.monqstperguntas
        WHERE mqpid = '%s'
DML;
    $stmt = sprintf($strSQL, $_GET['mqpid']);
    $detalhe_questionario = $db->pegaLinha($stmt);
    $descricao = $detalhe_questionario['descricao'];
    $resposta = $detalhe_questionario['resposta'];
}

if (isset($_REQUEST['fnc'])) {
    switch ($_REQUEST['fnc']) {
        case 'carregaPergunta':
            $frmData = $_REQUEST['fnc']($_REQUEST);
            break;
        case 'excluiPergunta':
            $_REQUEST['fnc']($_REQUEST['mqpid']);
            break;
        case 'salvarPergunta':
            $_REQUEST['fnc']($_POST);
            break;
        default:
    }
}
?>
<script type="text/javascript">    
    var i = 0;
    $(document).ready(function(){
        // -- Correção paliativa do bug #9920 do bootstrap.
        // -- Correção agendada para a versão 3.0.3 do bootstrap.
        // -- <https://github.com/twbs/bootstrap/issues/9920>
        $("input:radio").change(function() {
            $(this).prop('checked', true);
        });
    });
    function SomenteNumero(e){
        var tecla=(window.event)?event.keyCode:e.which;
        if((tecla>47 && tecla<58)) return true;
        else{
            if (tecla==8 || tecla==0) return true;
            else return false;
        }
    }
    function onValidade() {
        if (jQuery('#qsttotal').val() > 5000) {
            alert('Número total do questionário não pode exceder 5000 caracteres');
            jQuery('#qsttotal').focus();
            return false;
        }

        if (jQuery('#mqpdescricao').val() == '') {941
            alert('Pergunta em branco');
            jQuery('#mqpdescricao').focus();
            return false;
        }

        if (jQuery('#mqprespnumcaracteres').val() == '') {
            alert('Número de caracteres da respostas em branco');
            jQuery('#mqprespnumcaracteres').focus();
            return false;
        }
        
        if (jQuery('#mqpordem').val() == '') {
            alert('É obrigatório informar a ordem da questão');
            jQuery('#mqpordem').focus();
            return false;
        }

        if (jQuery("input[name='mqpid']").val()!= '') {
            jQuery("#act").attr({
                name:"fnc"
              , value:"salvarPergunta"
            });
        }
    }

    function calcularTotalQuestionario() {        
        var qsttotal = <?= (($qsttotal) ? $qsttotal - $resposta - $descricao -1 : '0')?>
          , mqprespnumcaracteres = 0;

        if (jQuery('#mqprespnumcaracteres').val() != '') {
            mqprespnumcaracteres = parseInt(jQuery('#mqprespnumcaracteres').val());
        }
        
        qsttotal += jQuery('#mqpdescricao').val().length + mqprespnumcaracteres + 1;        
        jQuery('#qsttotal').val(qsttotal);
    }


    function viewPergunta(mqpid, qstid) {
        var prefixUrl = 'planacomorc.php?modulo=principal/questionario/gerenciarquestionario'
          , paramsUrl = '&acao=A&aba=cadastrarperguntas&fnc=carregaPergunta&qstid='+qstid+'&mqpid='+mqpid;

        window.location = prefixUrl+paramsUrl;
    }

    function deletePergunta(mqpid, qstid) {
        if(confirm('Você realmente deseja excluir esta pergunta ?')){
            var prefixUrl = 'planacomorc.php?modulo=principal/questionario/gerenciarquestionario'
            , paramsUrl = '&acao=A&aba=cadastrarperguntas&fnc=excluiPergunta&mqpid='+mqpid;        
            
            window.location = prefixUrl+paramsUrl;        
        }
    }
    
    function limpar(){
        window.location = 'planacomorc.php?modulo=principal/questionario/gerenciarquestionario&acao=A&aba=cadastrarperguntas';
    }
    
    $(document).ready(function(){
        $('#mqpdescricao').keyup(function(){
            calcularTotalQuestionario();
        });
    });
</script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<section class="col-md-12">
    <section class="well">        
        <form class="form-horizontal" name="formquestionario" id="formquestionario" action="" onsubmit="return onValidade();" method="POST" role="form">            
            <input type="hidden" id="act" name="requisicao" value="inserirPerguntas">
            <input type="hidden" name="mqpid" value="<?= !isset($frmData['mqpid'])?'':$frmData['mqpid']; ?>">
            <div class="form-group">
                <label class="control-label col-md-3" for="qstnome">Número total de caracteres do questionário:</label>
                <div class="col-md-9">
                    <?= inputTexto('qsttotal', (($qsttotal) ? $qsttotal : '0'), 'qsttotal', 9, false, array("habil" => "N",'masc'=>'#######')); ?>
                </div>
            </div>
            <div class="form-group">
                <label class="control-label col-md-3" for="mqpdescricao">Pergunta:</label>
                <div class="col-md-9">                    
                    <?inputTextArea('mqpdescricao', $frmData['mqpdescricao'], 'mqpdescricao', 400,array('rows'=>3)); ?>
                </div>
            </div>
            <div class="form-group">
                <label class="control-label col-md-3" for="mqprespnumcaracteres">Número máximo de caracteres da resposta:</label>
                <div class="col-md-9">
                    <?php inputTexto('mqprespnumcaracteres', $frmData['mqprespnumcaracteres'], 'mqprespnumcaracteres', 9,false,array('masc'=>'#######','evtkeyup'=>'calcularTotalQuestionario();'));?>
                </div>
            </div>
            <div class="form-group">
                <label class="control-label col-md-3" for="mqpresppadrao">Resposta pré-definida:</label>
                <div class="col-md-9">
                    <?= inputTextArea('mqpresppadrao', $frmData['mqpresppadrao'], 'mqpresppadrao', '85') ?>
                </div>
            </div>
            <div class="form-group">
                <label class="control-label col-md-3" for="id_periodo_referencia">Pergunta facultativa:</label>
                <div class="col-md-9">                                        
                    <div class="btn-group" data-toggle="buttons">
                        <label class="btn btn-default <?= ($frmData['mqpfacultativo']==1) ? 'active' : ''; ?>">
                            <input type="radio" name="mqpfacultativo" id="mqpfacultativo_1" <?= ($frmData['mqpfacultativo']==1) ? 'checked' : ''; ?> value="1">
                            Sim
                        </label>
                        <label class="btn btn-default <?= ($frmData['mqpfacultativo']==2 || empty($frmData['mqpfacultativo'])) ? 'active' : ''; ?>">												
                            <input type="radio" name="mqpfacultativo" id="mqpfacultativo_2" value="2" <?= ($frmData['mqpfacultativo']==2 || empty($frmData['mqpfacultativo'])) ? 'checked' : ''; ?>>
                            Não
                        </label>															
                    </div>                    
                </div>
            </div>
            <div class="form-group">
                <label class="control-label col-md-3" for="mqpordem">Ordem:</label>
                <div class="col-md-9">
                    <script>
                        $(function(){$('#mqpordem').width('20px');});
                    </script>
                    <?= inputTexto('mqpordem', $frmData['mqpordem'], 'mqpordem', 2); ?>
                </div>
            </div>
            <div class="form-group">        
                <div class="col-md-9 col-md-offset-3">
                    <button type="submit" class="btn btn-primary" id="btn-Salva-Questionario">Salvar</button>
                    <? if($_GET['qstid']){ ?>
                    <input type="button" class="btn btn-success" value="Novo" onclick="limpar();">                    
                    <? } ?>
                </div>
            </div>
        </form>
    </section>
    
    <div class="">                
    <?php
        $colunms = array(
            'Ordem',
            'Pergunta',
            'Resposta pré-definida',
            'Número máximo de caracteres da resposta',
            'Facultativa'                
        );

        $query = <<<DML
            SELECT
                mqpid,
                qstid,
                CASE mqpordem::TEXT
                    WHEN  NULL THEN '<span class=\"label label-danger\">N/A</span>'
                    ELSE '<span class=\"label label-info\">'||mqpordem::text||'</span>'
                END as mqpordem,
                --COALESCE(mqpordem::Text, '<span class=\"label label-danger\">N/A</span>')as mqpordem,
                mqpdescricao,
                COALESCE(mqpresppadrao, '<span class=\"label label-info\">Nada Descrito</span>') AS mqpresppadrao,
                mqprespnumcaracteres,
                '<center>' || CASE WHEN 1 = mqpfacultativo THEN 'Sim'
                ELSE 'Não' END || '</center>' AS mqpfacultativo                    
            FROM planacomorc.monqstperguntas
            WHERE qstid = '%s'
            ORDER BY mqpordem
DML;

        if ($_SESSION['planacomorc']['qstid']) {

            $query = sprintf($query, (int) $_SESSION['planacomorc']['qstid']);

            require_once(APPRAIZ . 'includes/library/simec/Listagem.php');

            $list = new Simec_Listagem();
            $list->setCabecalho($colunms);
            $list->addAcao('edit', array('func' => 'viewPergunta', 'extra-params' => array('qstid')));
            $list->addAcao('delete', array('func' => 'deletePergunta', 'extra-params' => array('qstid')));
            $list->esconderColunas(array('qstid'));
            $list->setQuery($query);
            $list->addCallbackDeCampo('mqpdescricao', 'alinhaParaEsquerda');
            $list->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
            $list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
        }
    ?>
    </div>
</section>