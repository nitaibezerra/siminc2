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
    $cumprimentoObjetoDiligencia = new CumprimentoObjetoDiligencia();
    $cumprimentoObjetoDiligencia->excluir($_REQUEST['arqid']);
    echo"<script>alert('Arquivo excluido com sucesso!');window.location.href = 'obras2.php?modulo=principal/cadDiligenciaCumprimentoObjeto&acao=A';</script>";
    exit;
}

if($_POST['requisicao'] && $_POST['requisicao'] == 'arquivos') {
    $cumprimentoObjetoDiligencia = new CumprimentoObjetoDiligencia();
    $cumprimentoObjetoDiligencia->salvarDados($_POST);
    echo "<script> alert('Registro salvo com sucesso!'); </script>";
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
            #verificação para limitação de acesso;
            echo "<script>alert('Você não possui permissão de acesso à esta aba'); window.location.href = 'obras2.php?modulo=principal/cadObra&acao=A'</script>";
            die;
        }
        if($estado['esdid'] == ESDID_CUMPRIMENTO_CADASTRAMENTO) {
            if(!possui_perfil(array(PFLCOD_SUPER_USUARIO))) {
            echo "<script>alert('Acesso não permitido. Favor, tramitar workflow antes de prosseguir.'); window.location.href='obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A';</script>";
            die;
            }
        }
        
        
        print cabecalhoObra($obrid);
        $cumprimentoObjeto->criaSubAba($url, $habilitado,$obrid, $estado['esdid']);
        if ($estado['esdid'] != ESDID_CUMPRIMENTO_DILIGENCIADO && $estado['esdid'] != ESDID_CUMPRIMENTO_VALIDACAO_FNDE) {
            $habilitado = false;
        }
        if(possui_perfil(array(PFLCOD_SUPER_USUARIO))) {
        $habilitado = true;
        }
        monta_titulo('Diligência', '');
    }
}

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<style>
    fieldset {
        border: 1px solid #c0c0c0!important;
        margin: 0 2px!important;
        padding: 0.35em 0.625em 0.75em!important;
    }
    table p {
        color: #cc0102;
        font-weight: bold;
        font-size: 10px;
        text-align: justify;
    }
</style>
<script>
    $(document).ready(function(){
    });
    function excluir(arqid){
        if(confirm('Você deseja realmente excluir este arquivo?')) {
            window.location = '?modulo=principal/cadDiligenciaCumprimentoObjeto&acao=A&excluir=S&arqid='+arqid;
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
            <form method="post" action="" id="form" enctype="multipart/form-data">
                <input type="hidden" name="requisicao" id="requisicao" value="arquivos">
                <input type="hidden" name="coid" id="coid" value="<?=$cumprimentoObjeto->coid?>">
                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                    <tr>
                        <td class="SubTituloDireita" width="20%">Descrição:</td>
                        <td><?php echo campo_texto('descricao','S',!$habilitado,'',43,100,'','', '', '', '', 'id="descricao"', '');?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Tipo:</td>
                        <td>
                        <?php
                            $dadoAr = array(array('codigo' => 'Entidade', 'descricao' => 'Entidade'),array('codigo' => 'FNDE', 'descricao' => 'FNDE'));
                            $db->monta_combo("tipo", $dadoAr, 'S', "Selecione...", "", '', '', '', 'S', 'tipo');
                        ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Arquivo:</td>
                        <td>
                            <input type="file" name="arquivo" id="arquivo" class="obrigatorio" <?=!$habilitado ? 'disabled' : ''?>/>
                            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                        </td>
                    </tr>
                    <tr bgcolor="#c0c0c0">
                        <td colspan="2" style="text-align: center">
                            <input type="submit" value="Salvar" style="cursor:pointer;" <?=!$habilitado ? 'disabled' : ''?>/>
                        </td>
                    </tr>
                </table>
            </form>
            <?php
            $imagem = "'<img src=\"../imagens/excluir.gif\" onclick=\"excluir('||arq.arqid||')\" style=\"border:0; cursor:pointer;\" title=\"Excluir Arquivo\">&nbsp;'";
            $sqlEntidade = "
            SELECT
                $imagem as acoes,
                arq.arqdescricao,
                '<a href=\"javascript:void(0);\"
                    id=\"' || arq.arqid || '\"
                    onclick=\"window.location=\'?modulo=principal/cadCumprimentoObjeto&acao=A&download=S&arqid='||arq.arqid||'\'\"
                    class=\"download\"
                    title=\"Baixar Arquivo\">'
                    || arq.arqnome || '.' || arq.arqextensao ||
                '</a>',
                TO_CHAR(arq.arqdata,'DD/MM/YYYY') as arqdata,
                usu.usunome
            FROM obras2.cumprimento_objeto_diligencia cod
            INNER JOIN public.arquivo arq ON (arq.arqid = cod.arqid)
            INNER JOIN seguranca.usuario usu ON (usu.usucpf = cod.usucpf)
            WHERE arq.arqstatus = 'A'
                AND cod.coid = {$cumprimentoObjeto->coid}
                AND cod.tipo = 'Entidade'
            ";
            $sql = "
            SELECT
                $imagem as acoes,
                arq.arqdescricao,
                '<a href=\"javascript:void(0);\"
                    id=\"' || arq.arqid || '\"
                    onclick=\"window.location=\'?modulo=principal/cadCumprimentoObjeto&acao=A&download=S&arqid='||arq.arqid||'\'\"
                    class=\"download\"
                    title=\"Baixar Arquivo\">'
                    || arq.arqnome || '.' || arq.arqextensao ||
                '</a>',
                TO_CHAR(arq.arqdata,'DD/MM/YYYY') as arqdata,
                usu.usunome
            FROM obras2.cumprimento_objeto_diligencia cod
            INNER JOIN public.arquivo arq ON (arq.arqid = cod.arqid)
            INNER JOIN seguranca.usuario usu ON (usu.usucpf = cod.usucpf)
            WHERE arq.arqstatus = 'A'
                AND cod.coid = {$cumprimentoObjeto->coid}
                AND cod.tipo = 'FNDE'
            ";
            $cabecalho = array('Ações', 'Descrição', 'Arquivo', 'Data de Inclusão', 'Gravado por');?>
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                <tr>
                    <th>Arquivos FNDE</th>
                </tr>
                <tr>
                    <td>
                        <?php
                        $db->monta_lista($sql,$cabecalho,50,5,'N','center',$par2, "formulario");?>
                    </td>
                </tr>
                <tr>
                    <th>Arquivos Entidade</th>
                </tr>
                <tr>
                    <td>
                        <?php
                        $db->monta_lista($sqlEntidade,$cabecalho,50,5,'N','center',$par2, "formulario");?>
                    </td>
                </tr>
            </table>
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
<script>
    $(document).ready(function(){
        $('#div_dialog_workflow').next().css('width','100%');
    });
</script>
<?php
endif;?>