<?php
/**
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

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
    $cumprimentoObjetoProcesso = new CumprimentoObjetoProcesso();
    $cumprimentoObjetoProcesso->excluir($_REQUEST['arqid'], $_REQUEST['tipo']);
    echo"<script>alert('Arquivo excluido com sucesso!');window.location.href = 'obras2.php?modulo=principal/cadProcessoCumprimentoObjeto&acao=A';</script>";
    exit;
}

if($_POST['requisicao'] && $_POST['requisicao'] == 'processo') {
    $cumprimentoObjetoProcesso = new CumprimentoObjetoProcesso();
    $cumprimentoObjetoProcesso->salvarConclusao($_POST);
    echo "<script> alert('Registro salvo com sucesso!'); window.location.href = 'obras2.php?modulo=principal/cadProcessoCumprimentoObjeto&acao=A';</script>";
    exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
if ($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ) {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,'obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A',$parametros,array());
} else {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA,'obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A',$parametros,array());
}

global $db;
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
        monta_titulo('Conclusão do Processo', '');
        
        $cumprimentoObjetoConclusao = new CumprimentoObjetoConclusao();
        if($cumprimentoObjetoConclusao->verificaExistencia($cumprimentoObjeto->coid)) {
            $numprocesso = $cumprimentoObjeto->retornaNumeroProcesso();
            $cumprimentoObjetoProcesso = new CumprimentoObjetoProcesso();
            $verificacaoObras = $cumprimentoObjetoProcesso->verificaObrasPorProcesso($numprocesso);
            $dados = $cumprimentoObjetoProcesso->capturaDados($numprocesso);
        } else {
            $habilPag = false;
            $msgHabilPag = 'É necessário preencher a aba de Conclusão da Obra antes de prosseguir.';
        }
        
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
            window.location = '?modulo=principal/cadProcessoCumprimentoObjeto&acao=A&excluir=S&arqid='+arqid+'&tipo='+tipo;
        }
    }
</script>
<?php
if (!$habilPag):?>
    <div class="col-md-12">
        <center>
        <span style="background: #f00; color: #fff; padding:5px; text-align: center;">
            <?
            if($msgHabilPag):
                echo $msgHabilPag;
            else:
?>
            Esta aba será liberada após a conclusão da obra.
            <?
            endif;?>
        </span>
        </center>
    </div>
<?php
else: 
    if(!$verificacaoObras['result']): $habilitado = false;?>
    <div class="col-md-12">
        <section class="alert alert-warning text-center">
            Esta aba será liberada após a conclusão do cumprimento do objeto de todas as obras do processo.
        </section>
    </div>
    <?php
    endif;?>
    <div class="row" style="position:static;">
        <div class="col-md-10" style="position:inherit;">
            <form method="post" action="" id="form_processo" enctype="multipart/form-data">
                <input type="hidden" name="requisicao" id="requisicao" value="processo">
                <input type="hidden" name="numprocesso" id="coid" value="<?=$numprocesso?>">
                <input type="hidden" name="copid" id="copid" value="<?=$cumprimentoObjetoProcesso->copid?>">
                
                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                    <tr>
                        <td class="SubtituloCentro">Obra</td>
                        <td class="SubtituloCentro">Aprovado 100% - Objetivo Cumprido</td>
                        <td class="SubtituloCentro">Reprovado 100% - Objetivo Não Cumprido</td>
                        <td class="SubtituloCentro">Aprovação parcial - Objetivo Cumprido</td>
                        <td class="SubtituloCentro">Valor a Devolver ao Erário Público</td>
                    </tr>
                    <?
                    $total_devolucao = 0;
                    $aprovacaoA = 0;
                    $aprovacaoR = 0;
                    $aprovacaoP = 0;
                    foreach($verificacaoObras['obras'] as $key => $obr):
                        $chave = $key +1;
                        $cumprimentoObjetoConclusao = new CumprimentoObjetoConclusao();
                        $cumprimentoObjetoConclusao->capturaDados($obr['coid']);
                        $total_devolucao += $cumprimentoObjetoConclusao->valor_devolucao;
                        switch($cumprimentoObjetoConclusao->aprovacao){
                            case 'A':
                                $aprovacaoA++;
                                break;
                            case 'R':
                                $aprovacaoR++;
                                break;
                            case 'P':
                                $aprovacaoP++;
                                break;
                        }
                        ?>
                    <tr>
                        <td style="text-align: center;"><strong><?=$obr['obrid']?></strong></td>
                        <td style="text-align: center;">
                            <?=$cumprimentoObjetoConclusao->aprovacao == 'A' ? '<img margin src="/imagens/valida1.gif">' : ''?>
                        </td>
                        <td style="text-align: center;">
                            <?=$cumprimentoObjetoConclusao->aprovacao == 'R' ? '<img src="/imagens/valida1.gif">' : ''?>
                        </td>
                        <td style="text-align: center;">
                            <?=$cumprimentoObjetoConclusao->aprovacao == 'P' ? '<img src="/imagens/valida1.gif">' : ''?>
                        </td>
                        <td style="text-align: center;">
                            <input type="text" value="<?= mascaraglobal($cumprimentoObjetoConclusao->valor_devolucao, '###.####.###.###,##')?>" onkeyup="this.value=mascaraglobal('[.###],##',this.value);" name="valordevolucao" disabled>
                        </td>
                    </tr>
                    <?
                    endforeach; ?>
                </table>
                <?
                if($aprovacaoA > $aprovacaoR && $aprovacaoA > $aprovacaoP){
                    $aprovacao = 'A';
                } else if($aprovacaoP > $aprovacaoA) {
                    $aprovacao = 'P';
                } else {
                    $aprovacao = 'P';
                }
                ?>
                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                    <tr>
                        <td class="SubtituloDireita" width="50%">
                            <input type="radio" id="estadoAT" name="aprovacao" value="A" <?=$aprovacao == 'A' ? 'checked' : ''?> disabled>
                        </td>
                        <td>
                            <label for="estadoAT"> Aprovado 100% - Objetivo Cumprido</label>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">
                            <input type="radio" id="estadoRT" name="aprovacao" value="R" <?=$aprovacao == 'R'? 'checked' : ''?> disabled>
                        </td>
                        <td>
                            <label for="estadoRT"> Reprovado 100% - Objetivo Não Cumprido</label>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">
                            <input type="radio" id="estadoAP" name="aprovacao" value="P" <?=$aprovacao == 'P'? 'checked' : ''?> disabled>
                        </td>
                        <td>
                            <label for="estadoAP"> Aprovação parcial - Objetivo Cumprido</label>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">Valor a Devolver ao Erário Público</td>
                        <td><input type="text" value="<?= mascaraglobal($total_devolucao, '###.####.###.###,##')?>" onkeyup="this.value=mascaraglobal('[.###],##',this.value);" name="valordevolucao" disabled><img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">Parecer Final - Infraestrutura FNDE</td>
                        <td>
                            <?
                            if($cumprimentoObjetoProcesso->arqid_pf) :?>
                            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                                <thead>
                                    <tr>
                                        <th>Ações</th>
                                        <th>Arquivo</th>
                                        <th>Data de Inclusão</th>
                                        <th>Gravado por</th>
                                    </tr>
                                </thead>
                                <tbody>
                                    <tr>
                                        <td>
                                            <? if($habilitado): ?>
                                            <img src="../imagens/excluir.gif" style="border:0; cursor:pointer;" title="Excluir Documento Anexo" onclick="excluir(<?=$cumprimentoObjetoProcesso->arqid_pf?>,'arqid_pf')">
                                            <? else: ?>
                                            <img src="../imagens/excluir_01.gif" style="border:0;" title="Excluir Documento Anexo">
                                            <? endif; ?>
                                        </td>
                                        <td>
                                            <a target="_blank" href="/obras2/obras2.php?modulo=principal/cadProcessoCumprimentoObjeto&acao=A&download=<?=$cumprimentoObjetoProcesso->arqid_pf?>">
                                                <?=$dados['pfnome']?> .<?=$dados['pfextensao']?>
                                            </a>
                                        </td>
                                        <td><?=$dados['pfdata']?></td>
                                        <td><?=$dados['pfusunome']?></td>
                                    </tr>
                                </tbody>
                            </table>
                            <?php
                            else: ?>
                            <input type="file" name="parecerfinal" <?=!$habilitado ? 'disabled' : ''?>><img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
                            <?php
                            endif; ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">Parecer Final Revisado - Infraestrutura FNDE</td>
                        <td>
                            <?
                            if($cumprimentoObjetoProcesso->arqid_pf_revisado) :?>
                            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                                <thead>
                                    <tr>
                                        <th>Ações</th>
                                        <th>Arquivo</th>
                                        <th>Data de Inclusão</th>
                                        <th>Gravado por</th>
                                    </tr>
                                </thead>
                                <tbody>
                                    <tr>
                                        <td>
                                            <? if($habilitado): ?>
                                            <img src="../imagens/excluir.gif" style="border:0; cursor:pointer;" title="Excluir Documento Anexo" onclick="excluir(<?=$cumprimentoObjetoProcesso->arqid_pf_revisado?>,'arqid_pf_revisado')">
                                            <? else: ?>
                                            <img src="../imagens/excluir_01.gif" style="border:0;" title="Excluir Documento Anexo">
                                            <? endif; ?>
                                        </td>
                                        <td>
                                            <a target="_blank" href="/obras2/obras2.php?modulo=principal/cadProcessoCumprimentoObjeto&acao=A&download=<?=$cumprimentoObjetoProcesso->arqid_pf_revisado?>">
                                                <?=$dados['pfrnome']?> .<?=$dados['pfrextensao']?>
                                            </a>
                                        </td>
                                        <td><?=$dados['pfrdata']?></td>
                                        <td><?=$dados['pfrusunome']?></td>
                                    </tr>
                                </tbody>
                            </table>
                            <?php
                            else: ?>
                            <input type="file" name="parecerfinalrevisado" <?=!$habilitado ? 'disabled' : ''?>>
                            <?
                            endif;?>
                        </td>
                    </tr>
                    <tr bgcolor="">
                        <td colspan="5" style="text-align: center">
                            <input type="button" value="Salvar" id="salvar_conclusao" style="cursor:pointer;" <?=!$habilitado ? 'disabled' : ''?>/>
                        </td>
                    </tr>
                </table>
                <input type="hidden" name="valor_devolucao" id="valor_devolucao" value="<?=$total_devolucao?>">
                <input type="hidden" name="aprovacao" id="aprovacao" value="<?=$aprovacao?>">
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
            if($('[name=parecerfinal]').val() != undefined) {
                if($('[name=parecerfinal]').val() == '') {
                    alert('Campo \'Parecer Final - Infraestrutura FNDE\' é de preenchimento obrigatório.');
                    return false;
                }
            }

            $('#form_processo').submit();
        });
    });
</script>