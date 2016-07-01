<?
$strSql = <<<DML
    SELECT 
        coalesce(sum(length(mqpdescricao)+mqprespnumcaracteres),0) as tot 
    FROM planacomorc.monqstsubacaperguntas 
    WHERE qstid= '{$_SESSION ['planacomorc'] ['qstid']}'
DML;
$qsttotal = $db->pegaUm($strSql);

if($_GET['fnc'] && $_GET['fnc'] == 'carregaPergunta'){
    $strSQL = "
        SELECT
            COALESCE(length(mqpdescricao),0) as descricao,      
            COALESCE(mqprespnumcaracteres,0) as resposta
        FROM planacomorc.monqstsubacaperguntas
        WHERE mqpid = '%s'
    ";
    $stmt = sprintf($strSQL, $_GET['mqpid']);
    $detalhe_questionario = $db->pegaLinha($stmt);
    $descricao = $detalhe_questionario['descricao'];
    $resposta = $detalhe_questionario['resposta'];
}

if (isset ( $_REQUEST ['fnc'] )) {	
    switch ($_REQUEST ['fnc']) {
        case 'carregaPergunta' :
            $frmData = $_REQUEST ['fnc'] ( $_REQUEST );
            break;
        case 'excluiPergunta' :
            $_REQUEST ['fnc'] ( $_REQUEST ['mqpid'] );
            break;
        case 'salvarPergunta' :
            $_REQUEST ['fnc'] ( $_POST );
            break;
        default :
    }
}

?>
<script>
    function SomenteNumero(e){
        var tecla=(window.event)?event.keyCode:e.which;
        if((tecla>47 && tecla<58)) return true;
        else{
            if (tecla==8 || tecla==0) return true;
            else return false;
        }
    }
    function inserirPerguntas() {
        if (jQuery('#qsttotal').val() > 5000) {
            alert('Número total do questionário não pode exceder 5000 caracteres');
            return false;
        }

        if (jQuery('#mqpdescricao').val() == '') {
            alert('Pergunta em branco');
            return false;
        }

        if (jQuery('#mqprespnumcaracteres').val() == '') {
            alert('Número de caracteres da respostas em branco');
            return false;
        }
        
        if (jQuery("input[name='mqpid']").val()!= '') {
            jQuery("#act").attr({
                name:"fnc"
              , value:"salvarPergunta"
            });
        }

        document.getElementById('formperguntas').submit();
    }

    function calcularTotalQuestionario() {

        var qsttotal = <?= (($qsttotal) ? $qsttotal - $resposta - $descricao -1 : '0')?>
          , mqprespnumcaracteres = 0;
        console.log(qsttotal);
        if (jQuery('#mqprespnumcaracteres').val() != '') {
            mqprespnumcaracteres = parseInt(jQuery('#mqprespnumcaracteres').val());
        }
        console.log(mqprespnumcaracteres );
        console.log(jQuery('#mqpdescricao').val().length);
        qsttotal += jQuery('#mqpdescricao').val().length + mqprespnumcaracteres + 1;

        jQuery('#qsttotal').val(qsttotal);
    }

    function alterarPergunta(mqpid, qstid) {
        var prefixUrl = 'planacomorc.php?modulo=principal/questionariosubacoes/gerenciarquestionario'
          , paramsUrl = '&acao=A&aba=cadastrarperguntas&fnc=carregaPergunta&qstid=' + qstid + '&mqpid=' + mqpid;

        window.location = prefixUrl + paramsUrl;
    }

    function excluirPergunta(mqpid) {
        if(confirm('Você realmente deseja excluir esta pergunta ?')){
            var prefixUrl = 'planacomorc.php?modulo=principal/questionariosubacoes/gerenciarquestionario'
              , paramsUrl = '&acao=A&aba=cadastrarperguntas&fnc=excluiPergunta&mqpid=' + mqpid;

            window.location = prefixUrl + paramsUrl;
        }
    }
    
    function limpar(){
        window.location = 'planacomorc.php?modulo=principal/questionariosubacoes/gerenciarquestionario&acao=A&aba=cadastrarperguntas';
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
        <form class="form-horizontal" method="post" name="formperguntas" id="formperguntas">
            <input type="hidden" id="act" name="requisicao"	value="inserirPerguntas"> 
            <input type="hidden" name="mqpid" value="<?= !isset($frmData['mqpid'])?'':$frmData['mqpid']; ?>">
            <section class="form-group">
                <label class="control-label col-md-3" for="num">Número total de	caracteres do questionário:</label>
                <section class="col-md-9">            
                    <? inputTexto('qsttotal', (($qsttotal) ? $qsttotal : '0'), 'qsttotal', 9,false,array('habil'=>'N','masc'=>'#######'));?>	            
                </section>	    		
            </section>
            <section class="form-group">
                <label class="control-label col-md-3" for="mqpdescricao">Pergunta:</label>
                <section class="col-md-9">                    
                    <? inputTextArea('mqpdescricao', $frmData['mqpdescricao'], 'mqpdescricao', 150,array('rows'=>2)); ?>
                </section>
            </section>
            <section class="form-group">
                <label class="control-label col-md-3" for="mqprespnumcaracteres">Número máximo de caracteres da resposta:</label>
                <section class="col-md-9">
                    <? inputTexto('mqprespnumcaracteres', $frmData['mqprespnumcaracteres'], 'mqprespnumcaracteres', 9,false,array('masc'=>'#######','evtkeyup'=>'calcularTotalQuestionario();'));?>
                </section>
            </section>
            <section class="form-group">
                <label class="control-label col-md-3">Pergunta facultativa:</label>
                <section class="col-md-9">
                    <div class="btn-group" data-toggle="buttons">
                        <label class="btn btn-default <?= ($frmData['mqpfacultativo']==1) ? 'active' : ''; ?>">
                            <input type="radio" name="mqpfacultativo" id="mqpfacultativo_1"
                            <?= ($frmData['mqpfacultativo']==1) ? 'checked' : ''; ?> value="1">
                            Sim
                        </label>
                        <label class="btn btn-default <?= ($frmData['mqpfacultativo']==2 || empty($frmData['mqpfacultativo'])) ? 'active' : ''; ?>">												
                            <input type="radio" name="mqpfacultativo" id="mqpfacultativo_2" value="2"
                            <?= ($frmData['mqpfacultativo']==2 || empty($frmData['mqpfacultativo'])) ? 'checked' : ''; ?>>
                            Não
                        </label>															
                    </div>					
                </section>
            </section>
            <section class="form-group">
                <section class="col-md-offset-3">
                    <input type="button" class="btn btn-primary" name="salvar" value="Salvar" onclick="inserirPerguntas();">
                    <? if($_GET['qstid']){ ?>
                    <input type="button" class="btn btn-success" value="Novo" onclick="limpar();">                    
                    <? } ?>
                </section>
            </section>
        </form>        
    </section>
<?
$sql = <<<DML
    SELECT
        mqpid AS excluir, 
        qstid AS alterar,			
        mqpdescricao,
        mqprespnumcaracteres,
        '<center>' || CASE WHEN 1 = mqpfacultativo then 'Sim'
        ELSE 'Não' END || '</center>' AS mqpfacultativo
    FROM planacomorc.monqstsubacaperguntas
    WHERE qstid= '{$_SESSION ['planacomorc']['qstid']}'
DML;
$cabecalho = array (	
    "Pergunta",
    "Número máximo de caracteres da resposta",
    "Facultativa?" 
);	
$listagem = new Simec_Listagem();
$listagem->setQuery($sql);
$listagem->setCabecalho($cabecalho);	
$listagem->setAcoes(array('edit'=> array('func' => 'alterarPergunta','extra-params'=>array('alterar')), 'delete' => 'excluirPergunta'));
$listagem->esconderColuna('alterar');
$listagem->addCallbackDeCampo('mqpdescricao', 'alinhaParaEsquerda');
$listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
$listagem->render(Simec_Listagem::SEM_REGISTROS_RETORNO);
?>                      
</section>
