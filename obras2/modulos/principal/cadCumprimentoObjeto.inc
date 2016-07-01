<?php
/**
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */
verificaSessao('orgao');
$empid = $_SESSION['obras2']['empid'];
$_SESSION['obras2']['obrid'] = (int) ($_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid']);
$obrid = $_SESSION['obras2']['obrid'];
$pflcods = array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC);
$obra = new Obras($_SESSION['obras2']['obrid']);
$_SESSION['obras2']['empid'] = $obra->empid ? $obra->empid : $_SESSION['obras2']['empid'];

if($_REQUEST['download'] == 'S') {
    $questaoCumprimentoArquivo = new QuestaoCumprimentoArquivo();
    $questaoCumprimentoArquivo->download($_REQUEST['arqid']);
    echo"<script>window.location.href = 'obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A';</script>";
    exit;
}
if($_REQUEST['excluir'] == 'S') {
    $questaoCumprimentoArquivo = new QuestaoCumprimentoArquivo();
    $questaoCumprimentoArquivo->excluir($_REQUEST['arqid']);
    echo"<script>alert('Arquivo excluido com sucesso!');window.location.href = 'obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A';</script>";
    exit;
}
if($_POST) {
    $cumprimentoObjetoDocumentacao = new CumprimentoObjetoDocumentacao();
    $cumprimentoObjetoDocumentacao->salvar($_POST);
}

include APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
if($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ) {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros,array());
} else {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros,array());
}

include_once APPRAIZ . '/includes/workflow.php';

$habilPag = false;
$estadoWorkflowObra = wf_pegarEstadoAtual($obra->docid);
if($estadoWorkflowObra) {
    if($estadoWorkflowObra['esdid'] == ESDID_OBJ_CONCLUIDO || $estadoWorkflowObra['esdid'] == ESDID_OBJ_INACABADA) {
        $habilPag = true;
        $cumprimentoObjeto = new CumprimentoObjeto();
        $coid = $cumprimentoObjeto->verificaExistencia($obrid);
        $wfEstadoAtualCumprimento = wf_pegarEstadoAtual($cumprimentoObjeto->docid);
        $cumprimentoObjetoDocumentacao = new CumprimentoObjetoDocumentacao();
        $cumprimento = $cumprimentoObjetoDocumentacao->montaQuestionario($obrid);
        
        $habilitado = false;
        if (possui_perfil($pflcods)) {
            $habilitado = true;
        } else {
            #verificação para limitação de acesso;
            if (!$cumprimentoObjeto->obrasPermitidas($obra->obrid)) {
                echo "<script>alert('Você não possui permissão de acesso à esta aba'); window.location.href = 'obras2.php?modulo=principal/cadObra&acao=A'</script>";
                die;
            }
        }

        print cabecalhoObra($obrid);
        $cumprimentoObjeto->criaSubAba($url, $habilitado, $obrid, $wfEstadoAtualCumprimento['esdid']);
        monta_titulo( 'Documentação', '<b> 1 - O Cumprimento de Objeto faz parte da Prestação de Contas;<br>
        2 - É necessário que não haja pendências nesta aba, situação que ocorre quando o ícone <img src="/imagens/valida1.gif"> aparece no campo "observação";<br>
        3 - Ao concluir o preenchimento desta aba, a aba "Questionário" será liberada para preenchimento;<br>
        4 - Caso o ente não possua o documento solicitado deverá inserir declaração justificando o fato, assinada pelo Gestor responsável (Prefeito ou Secretário de Educação no caso de Estado), na respectiva questão;<br>
        5 - Após responder todas as questões da aba "Questionário" deverá ser tramitada para análise do FNDE.</b>');
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
    .link {
        font-size: 10px;
        cursor: pointer;
    }
    .documentos {
        border-collapse: collapse;
    }
    .documentos, .documentos td, .documentos th {
        border: 1px solid #ccc;
    }
    .observacao p{
        color: #cc0102;
        font-weight: bold;
        font-size: 10px;
        text-align: justify;
    }
</style>
<script>
    $(document).ready(function(){
        $('#janela').on('click',function(){
            janela('/par/par.php?modulo=principal/programas/proinfancia/visualizarPreObra&acao=A&preid=<?=$obra->preid?>&div=planilha',800,600,'preobra');
        });
    });
    function excluirArquivo(arqid){
        if(confirm('Você deseja realmente excluir este arquivo?')) {
            window.location = '?modulo=principal/cadCumprimentoObjeto&acao=A&excluir=S&arqid='+arqid;
        }
    }
</script>


<?php
if (!$habilPag):
    ?>
<div class="col-md-12">
    <center>
    <span style="background: #f00; color: #fff; padding:5px; text-align: center;">
        Esta aba será liberada após a conclusão da obra.
    </span>
    </center>
</div>
    <?php
else:
    $questionarioDocumento = (object) $cumprimento['questionario'];
    $questionarioConstrutora = (object) $cumprimento['questionarioConstrutora'];
    $vinculadas = $cumprimento['vinculadas'];
?>
<div class="row" style="position:static;">
    <div class="col-md-10" style="position:inherit;">
    <!-- Formulário de Documentos da Obra -->
    <form method="post" id="formulario" name="formulario" enctype="multipart/form-data">
        <input type="hidden" name="codid" value="<?=$questionarioDocumento->codid?>">
        <input type="hidden" name="coid" value="<?=$coid?>">
        <input type="hidden" name="obrid" value="<?=$questionarioDocumento->obrid?>">
        <input type="hidden" name="itcid" value="<?=$questionarioDocumento->itcid?>">
        <table class="tabela documentos" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" align="center">
            <thead>
                <tr>
                    <th colspan="7"><?=$questionarioDocumento->descricaoItem?></th>
                </tr>
                <tr>
                    <td style="text-align:center;font-weight: bold;">Questão</td>
                    <td style="text-align:center;font-weight: bold;" colspan="3">Resposta</td>
                    <td style="text-align:center;font-weight: bold;">Observação</td>
                    <td style="text-align:center;font-weight: bold;">Arquivos</td>
                    <td style="text-align:center;font-weight: bold;">Ação</td>
                </tr>
            </thead>
            <tbody>
                <? foreach($questionarioDocumento->questoes as $questao): ?>
                <tr>
                    <td width="20%">
                        <span><?=$questao->qcodNumero ?></span>) <?=$questao->qcodDsc?>
                    </td>
                    <td width="4%">
                        <input type="radio" disabled name="qcodresposta[<?=$questao->qstId?>]" value="t" <?=$questao->respostaTrue?>> SIM
                    </td>
                    <td width="4%">
                        <input type="radio" disabled name="qcodresposta[<?=$questao->qstId?>]" value="f" <?=$questao->respostaNA && $questao->qcodNumero == 8 ? $questao->respostaNA : $questao->respostaFalse?>> NÃO
                    </td>
                    <td width="8%">
                        <!-- Só mostra NA para questões 1, 2 e 3. -->
                        <? if($questao->qcodNumero == 1 || $questao->qcodNumero == 2 || $questao->qcodNumero == 3): ?>
                        <input type="radio" disabled name="qcodresposta[<?=$questao->qstId?>]" value="n" <?=$questao->respostaNA?>> NÃO SE APLICA
                        <? endif; ?>
                    </td>
                    <td class="observacao">
                        <!-- O item 6 possui regra oposta. Quando SIM, apresentar a mensagem. -->
                        <p><img title='<?=htmlspecialchars($questao->qstObs)?>' src="/imagens/<?=$questao->qcodResposta == 't' && $questao->qcodNumero == 6 ? 'valida5.gif': ($questao->qcodNumero == 6 ? 'valida1.gif' : $questao->imgValidacao)?>"/>
                        <?=$questao->qcodResposta == 'f' && $questao->qcodNumero != 6 ? $questao->qstObs : ($questao->qcodResposta == 't' && $questao->qcodNumero == 6 ? $questao->qstObs : '' ) ?>
                        </p>
                    </td>
                    <td>
                        <?
                        foreach($questao->arquivos as $arquivo):
                            #Se puder remover arquivo.
                            if($arquivo['del']):
                            ?>
                            <a class="link" onclick="excluirArquivo('<?=$arquivo['arqid']?>');">
                                <img src="/imagens/excluir.gif" title="Excluir arquivo <?=$arquivo['arqnome']?>">
                            </a>
                            <?
                            endif;
                        ?>
                        <a class="link" onclick="window.location='?modulo=principal/cadCumprimentoObjeto&acao=A&download=S&arqid=<?=$arquivo['arqid']?>'">
                            <img src='/imagens/salvar.png' border='0'>
                            <?= $arquivo['arqnome'] .'.'. $arquivo['arqextensao'] ?>
                        </a><br>
                        <?
                        endforeach;
                        #Excessão à questão número 3.
                        if ($questao->qcodNumero == 3 && $questao->qcodResposta == 'n'): ?>
                        <a class="link" id="janela">
                            <img src='/imagens/IconeAjuda.gif' border='0'>
                            <b style="vertical-align: sub;">Planilha Orçamentária</b>
                        </a>
                        <? endif;?>
                    </td>
                    <td width="15%">
                        <input type="hidden" name="qcodresposta[<?=$questao->qstId?>]" value="<?=$questao->qcodResposta?>">
                        <?
                        #Questões 4, 5 e 6 não contém arquivos. "Não se aplica" também.
                        if ($questao->qcodResposta != 'n' && $questao->qcodNumero != '4' && $questao->qcodNumero != '5' && $questao->qcodNumero != '6'):
                        ?>
                            <input type="file" name="<?=$questao->qstId?>">
                        <?
                        endif;
                        ?>
                    </td>
                </tr>
                <? endforeach; ?>
            </tbody>
            <tfoot>
                <tr style="background-color:#C0C0C0;">
                    <td colspan="7">
                        <center>
                            <input type="submit" id="salvar" value="Salvar" style="cursor: pointer">
                        </center>
                    </td>
                </tr>
            </tfoot>
        </table>
    </form>
    <!-- FIM Formulário de Documentos da Obra -->

    <!-- Formulário de Construtora -->
    <form method="post" id="formulario" name="formulario" enctype="multipart/form-data">
        <input type="hidden" name="codid" value="<?=$questionarioConstrutora->codid?>">
        <input type="hidden" name="coid" value="<?=$coid?>">
        <input type="hidden" name="obrid" value="<?=$questionarioConstrutora->obrid?>">
        <input type="hidden" name="itcid" value="<?=$questionarioConstrutora->itcid?>">
        <table class="tabela documentos" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" align="center">
            <thead>
                <tr>
                    <th colspan="7"><?=$questionarioConstrutora->descricaoItem?></th>
                </tr>
                <tr>
                    <td style="text-align:center;font-weight: bold;">Questão</td>
                    <td style="text-align:center;font-weight: bold;" colspan="3">Resposta</td>
                    <td style="text-align:center;font-weight: bold;">Observação</td>
                    <td style="text-align:center;font-weight: bold;">Arquivos</td>
                    <td style="text-align:center;font-weight: bold;">Ação</td>
                </tr>
            </thead>
            <tbody>
                <? foreach($questionarioConstrutora->questoes as $questao): ?>
                <tr>
                    <td width="20%">
                        <span><?=$questao->qcodNumero ?></span>) <?=$questao->qcodDsc?>
                    </td>
                    <td width="4%">
                        <input type="radio" disabled name="qcodresposta[<?=$questao->qstId?>]" value="t" <?=$questao->respostaTrue?>> SIM
                    </td>
                    <td width="4%">
                        <input type="radio" disabled name="qcodresposta[<?=$questao->qstId?>]" value="f" <?=$questao->respostaFalse?>> NÃO
                    </td>
                    <td width="8%">
                        <? if($questao->qcodNumero == 5): ?>
                        <input type="radio" disabled name="qcodresposta[<?=$questao->qstId?>]" value="n" <?=$questao->respostaNA?>> NÃO SE APLICA
                        <? endif;?>
                    </td>
                    <td class="observacao">
                        <p><img title="<?=$questao->qstObs?>" src="/imagens/<?=$questao->imgValidacao?>"/> <?=$questao->qcodResposta == 'f' ? $questao->qstObs : ''?></p>
                    </td>
                    <td>
                        <?
                        foreach($questao->arquivos as $arquivo):
                            if($arquivo['del']):
                        ?>
                        <a class="link" onclick="excluirArquivo('<?=$arquivo['arqid']?>');">
                            <img src="/imagens/excluir.gif" title="Excluir arquivo <?=$arquivo['arqnome']?>">
                        </a>
                        <?
                            endif;
                        ?>
                        <a class="link" onclick="window.location='?modulo=principal/cadCumprimentoObjeto&acao=A&download=S&arqid=<?=$arquivo['arqid']?>'">
                            <img src='/imagens/salvar.png' border='0'>
                            <?= $arquivo['arqnome'] .'.'. $arquivo['arqextensao'] ?>
                        </a><br>
                        <?
                        endforeach;
                        ?>
                    </td>
                    <td>
                        <input type="hidden" name="qcodresposta[<?=$questao->qstId?>]" value="<?=$questao->qcodResposta?>">
                        <?
                        if ($questao->qcodResposta != 'n'):
                        ?>
                            <input type="file" name="<?=$questao->qstId?>">
                        <?
                        endif;
                        ?>
                    </td>
                </tr>
                <? endforeach; ?>
            </tbody>
            <tfoot>
                <tr style="background-color:#C0C0C0;">
                    <td colspan="7">
                        <center>
                            <input type="submit" id="salvar" value="Salvar" style="cursor: pointer">
                        </center>
                    </td>
                </tr>
            </tfoot>
        </table>
    </form>
    <!-- FIM Formulário de Construtora -->


    <!-- Obras Vinculadas -->
    <?
    foreach($vinculadas as $vinculada):
        $questionarioConstrutoraVinculada = (object) $vinculada['questionarioConstrutora'];?>
        <!-- Formulário da Construtora Obra Vinculada -->
        <form method="post" id="formulario" name="formulario" enctype="multipart/form-data">
            <input type="hidden" name="codid" value="<?=$questionarioConstrutoraVinculada->codid?>">
            <input type="hidden" name="coid" value="<?=$coid?>">
            <input type="hidden" name="obrid" value="<?=$questionarioConstrutoraVinculada->obrid?>">
            <input type="hidden" name="itcid" value="<?=$questionarioConstrutoraVinculada->itcid?>">
            <table class="tabela documentos" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" align="center">
                <thead>
                    <tr>
                        <th colspan="7"><?=$questionarioConstrutoraVinculada->descricaoItem?></th>
                    </tr>
                    <tr>
                        <td style="text-align:center;font-weight: bold;">Questão</td>
                        <td style="text-align:center;font-weight: bold;" colspan="3">Resposta</td>
                        <td style="text-align:center;font-weight: bold;">Observação</td>
                        <td style="text-align:center;font-weight: bold;">Arquivos</td>
                        <td style="text-align:center;font-weight: bold;">Ação</td>
                    </tr>
                </thead>
                <tbody>
                    <? foreach($questionarioConstrutoraVinculada->questoes as $questao): ?>
                    <tr>
                        <td width="20%">
                            <span><?=$questao->qcodNumero ?></span>) <?=$questao->qcodDsc?>
                        </td>
                        <td width="4%">
                            <input type="radio" disabled name="qcodresposta[<?=$questao->qstId?>]" value="t" <?=$questao->respostaTrue?>> SIM
                        </td>
                        <td width="4%">
                            <input type="radio" disabled name="qcodresposta[<?=$questao->qstId?>]" value="f" <?=$questao->respostaFalse?>> NÃO
                        </td>
                        <td width="8%">
                            <? if($questao->qcodNumero == 5): ?>
                            <input type="radio" disabled name="qcodresposta[<?=$questao->qstId?>]" value="n" <?=$questao->respostaNA?>> NÃO SE APLICA
                            <? endif; ?>
                        </td>
                        <td class="observacao">
                            <p><img title="<?=$questao->qstObs?>" src="/imagens/<?=$questao->imgValidacao?>"/> <?=$questao->qcodResposta == 'f' ? $questao->qstObs : ''?></p>
                        </td>
                        <td>
                            <?
                            foreach($questao->arquivos as $arquivo):
                                if($arquivo['del']):
                            ?>
                            <a class="link" onclick="excluirArquivo('<?=$arquivo['arqid']?>');">
                                <img src="/imagens/excluir.gif" title="Excluir arquivo <?=$arquivo['arqnome']?>">
                            </a>
                            <?
                                endif;
                            ?>
                            <a class="link" onclick="window.location='?modulo=principal/cadCumprimentoObjeto&acao=A&download=S&arqid=<?=$arquivo['arqid']?>'">
                                <img src='/imagens/salvar.png' border='0'>
                                <?= $arquivo['arqnome'] .'.'. $arquivo['arqextensao'] ?>
                            </a><br>
                            <?
                            endforeach;
                            ?>
                        </td>
                        <td>
                            <input type="hidden" name="qcodresposta[<?=$questao->qstId?>]" value="<?=$questao->qcodResposta?>">
                            <?
                            if ($questao->qcodResposta != 'n'):
                            ?>
                                <input type="file" name="<?=$questao->qstId?>">
                            <?
                            endif;
                            ?>
                        </td>
                    </tr>
                    <? endforeach; ?>
                </tbody>
                <tfoot>
                    <tr style="background-color:#C0C0C0;">
                        <td colspan="7">
                            <center>
                                <input type="submit" id="salvar" value="Salvar" style="cursor: pointer">
                            </center>
                        </td>
                    </tr>
                </tfoot>
            </table>
        </form>
        <!-- Fim Formulário Construtora Obra Vinculada -->
    <?
    endforeach;?>
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
<?
endif;?>
<!-- Fim Obras Vinculadas --> 