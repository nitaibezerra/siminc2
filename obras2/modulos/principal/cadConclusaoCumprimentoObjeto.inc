<?php
/**
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";

verificaSessao('orgao');
$empid = $_SESSION['obras2']['empid'];
$_SESSION['obras2']['obrid'] = (int) ($_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid']);
$obrid = $_SESSION['obras2']['obrid'];
$pflcods = array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC);
$obra = new Obras($_SESSION['obras2']['obrid']);
$_SESSION['obras2']['empid'] = $obra->empid ? $obra->empid : $_SESSION['obras2']['empid'];

if($_GET['download']){
    $obraArquivo = new ObrasArquivos();
    $arDados = $obraArquivo->buscaDadosPorArqid($_GET['download']);
    $eschema = 'obras2';

    $file = new FilesSimec(null,null,$eschema);
    $file->getDownloadArquivo($_GET['download']);

    die('<script type="text/javascript">
        window.close();
        </script>');
}

if($_REQUEST['excluir'] == 'S') {
    $cumprimentoObjetoConclusao = new CumprimentoObjetoConclusao();
    $cumprimentoObjetoConclusao->excluir($_REQUEST['arqid'], $_REQUEST['tipo']);
    echo"<script>alert('Arquivo excluido com sucesso!');window.location.href = 'obras2.php?modulo=principal/cadConclusaoCumprimentoObjeto&acao=A';</script>";
    exit;
}

if($_POST['requisicao'] && $_POST['requisicao'] == 'concluir') {
    #$cumprimentoObjeto = new CumprimentoObjeto();
    #$cumprimentoObjeto->tramitaConclusao($_POST);
    $cumprimentoObjetoConclusao = new CumprimentoObjetoConclusao();
    $cumprimentoObjetoConclusao->salvarConclusao($_POST);
    echo "<script> alert('Registro salvo com sucesso!'); window.location.href = 'obras2.php?modulo=principal/cadConclusaoCumprimentoObjeto&acao=A';</script>";
    exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
if ($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ) {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,'obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A',$parametros,array());
} else {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA,'obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A',$parametros,array());
}


$habilPag = false;
$estadoWorkflowObra = wf_pegarEstadoAtual($obra->docid);
if($estadoWorkflowObra) {
    if($estadoWorkflowObra['esdid'] == ESDID_OBJ_CONCLUIDO || $estadoWorkflowObra['esdid'] == ESDID_OBJ_INACABADA) {
        $habilPag = true;
        $cumprimentoObjeto = new CumprimentoObjeto();
        $coid = $cumprimentoObjeto->verificaExistencia($obrid);
        $estado = wf_pegarEstadoAtual($cumprimentoObjeto->docid);
        
        $habilitado = false;
        if (possui_perfil($pflcods)) {
            $habilitado = true;
        } else {
            echo "<script>alert('Você não possui permissão de acesso à esta aba'); window.location.href = 'obras2.php?modulo=principal/cadObra&acao=A'</script>";
            die;
        }
        if($estado['esdid'] == ESDID_CUMPRIMENTO_CADASTRAMENTO || $estado['esdid'] == ESDID_CUMPRIMENTO_DILIGENCIADO) {
            if(!possui_perfil(array(PFLCOD_SUPER_USUARIO))) {
            echo "<script>alert('Acesso não permitido. Favor, tramitar workflow antes de prosseguir.'); window.location.href='obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A';</script>";
            die;
            }
        }
        print cabecalhoObra($obrid);
        $cumprimentoObjeto->criaSubAba($url, $habilitado,$obrid, $estado['esdid']);
        monta_titulo('Conclusão da Obra', '');
        if ($estado['esdid'] != ESDID_CUMPRIMENTO_VALIDACAO_FNDE) {
            $habilitado = false;
        }
        if(possui_perfil(array(PFLCOD_SUPER_USUARIO))) {
            $habilitado = true;
        }
        $cumprimentoObjetoConclusao = new CumprimentoObjetoConclusao();
        $cumprimentoObjetoConclusao->capturaDados($coid);
    }
}

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script>
    $(document).ready(function(){
    });
    function excluir(arqid,tipo){
        if(confirm('Você deseja realmente excluir este arquivo?')) {
            window.location = '?modulo=principal/cadConclusaoCumprimentoObjeto&acao=A&excluir=S&arqid='+arqid+'&tipo='+tipo;
        }
    }
</script>
<?php
if (!$habilPag):?>
    <div class="col-md-12">
        <center>
        <span style="background: #f00; color: #fff; padding:5px; text-align: center;">
            Esta aba será liberada após a conclusão da obra.
        </span>
        </center>
    </div>
<?php
else: ?>
    <div class="row" style="position:static;">
        <div class="col-md-10" style="position:inherit;">
            <form method="post" action="" id="form_conclusao" enctype="multipart/form-data">
                <input type="hidden" name="requisicao" id="requisicao" value="concluir">
                <input type="hidden" name="coid" id="coid" value="<?=$cumprimentoObjeto->coid?>">
                <input type="hidden" name="cocid" id="cocid" value="<?=$cumprimentoObjetoConclusao->cocid?>">
                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                    <tr>
                        <td class="SubtituloDireita" width="50%">
                            <input type="radio" id="estadoAT" name="aprovacao" value="A" <?=$cumprimentoObjetoConclusao->aprovacao == 'A' ? 'checked' : ''?> <?=!$habilitado ? 'disabled' : ''?>>
                        </td>
                        <td>
                            <label for="estadoAT"> Aprovado 100% - Objetivo Cumprido</label>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">
                            <input type="radio" id="estadoRT" name="aprovacao" value="R" <?=$cumprimentoObjetoConclusao->aprovacao == 'R'? 'checked' : ''?> <?=!$habilitado ? 'disabled' : ''?>>
                        </td>
                        <td>
                            <label for="estadoRT"> Reprovado 100% - Objetivo Não Cumprido</label>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">
                            <input type="radio" id="estadoAP" name="aprovacao" value="P" <?=$cumprimentoObjetoConclusao->aprovacao == 'P'? 'checked' : ''?> <?=!$habilitado ? 'disabled' : ''?>>
                        </td>
                        <td>
                            <label for="estadoAP"> Aprovação parcial - Objetivo Cumprido</label>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">Valor a Devolver ao Erário Público</td>
                        <td><input type="text" value="<?= mascaraglobal($cumprimentoObjetoConclusao->valor_devolucao, '###.####.###.###,##')?>" onkeyup="this.value=mascaraglobal('[.###],##',this.value);" name="valordevolucao" <?=!$habilitado ? 'disabled' : ''?>></td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <?
                            global $db;
                            if($obra->preid) {
                                $sql = <<<DML
                                    SELECT
                                        p.pagparcela,
                                        pagvalorparcela,
                                        pagdatapagamento
                                    FROM par.pagamento p
                                    JOIN (
                                        SELECT preid, pagid, pobvalorpagamento as valor
                                        FROM par.pagamentoobra
                                        UNION
                                        SELECT preid, pagid, popvalorpagamento as valor
                                        FROM par.pagamentoobrapar) po ON po.pagid = p.pagid
                                    WHERE preid = {$obra->preid}
                                        AND p.pagstatus = 'A'
                                        AND p.pagsituacaopagamento = '2'
DML;
                                $cabecalho = array('Parecela', 'Valor', 'Data de Pagamento');
                                $db->monta_lista($sql,$cabecalho,50,5,'N','center',$par2, "formulario");
                            }
                            ?>
                        </td>
                    </tr>
                    <tr bgcolor="">
                        <td colspan="2" style="text-align: center">
                            <input type="button" value="Salvar" id="salvar_conclusao" style="cursor:pointer;" <?=!$habilitado ?>/>
                        </td>
                    </tr>
                </table>
            </form>
        </div>
        <div class="col-md-1 pull-right" style="position:inherit;">
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                <tr>
                    <td>
                    <?php
                    wf_desenhaBarraNavegacao($cumprimentoObjeto->docid, array('coid' => $cumprimentoObjeto->coid, 'docid' => $cumprimentoObjeto->docid, 'obrid' => $cumprimentoObjeto->obrid)); ?>
                    </td>
                </tr>
            </table>
        </div>
    </div>
<?php
endif;?>
<script>
    $(document).ready(function(){
        $('#div_dialog_workflow').next().css('width','100%');
        $('#salvar_conclusao').on('click',function(){
            if($('[name=aprovacao]:checked').val() == undefined) {
                alert('Selecione o campo de rádio: Aprovado, Reprovado ou Aprovado Parcialmente.');
                return false;
            }

            if($('[name=valordevolucao]').val() != undefined) {
                if($('[name=valordevolucao]').val() == '') {
                    alert('Campo \'Valor a Devolver ao Erário Público\' é de preenchimento obrigatório.');
                    return false;
                }
            }
            
            $('#form_conclusao').submit();
        });
    });
</script>