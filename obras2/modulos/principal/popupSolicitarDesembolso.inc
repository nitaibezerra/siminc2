<?php


$_SESSION['obras2']['obrid'] = (!empty($_GET['obrid'])) ? $_GET['obrid'] : $_SESSION['obras2']['obrid'];
$obrid                       = $_SESSION['obras2']['obrid'];

$habilitado = true;
$habilita = 'S';

$pflcods = array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_GESTOR_UNIDADE);

if(!possui_perfil( $pflcods )){
    $habilitado = false;
    $habilita = 'N';
}


if($obrid) {
    $obra = new Obras($obrid);
    $sql = "
            SELECT
                COUNT(*)
            FROM obras2.solicitacao_desembolso sv
            JOIN obras2.obras o ON o.obrid = sv.obrid AND o.obridpai IS NULL AND o.obrstatus IN ('A', 'P')
            JOIN workflow.documento d ON d.docid = sv.docid
            WHERE d.esdid NOT IN(".ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO.", ".ESDID_SOLICITACAO_DESEMBOLSO_INDEFERIDO.") AND sv.sldstatus = 'A' AND o.obrid = $obrid";

    if ($db->pegaUm($sql) > 0) {
        $habilitado = false;
        $habilita = 'N';
        $msgB = '<p style=" font-weight: bold;color:#cc0000;"><img src="../imagens/exclama.gif"/> Esta obra possui uma solicitação em análise, aguarde a conclusão da solicitação para solicitar novamente.</p>';
    } else {

        // Verifica se possui uma solicitação DEFERIDA e se possui uma vistoria inserida após a soolicitação
        $sql = "
            SELECT
                sv.supid
            FROM obras2.solicitacao_desembolso sv
            JOIN obras2.obras o ON o.obrid = sv.obrid AND o.obridpai IS NULL AND o.obrstatus IN ('A', 'P')
            JOIN workflow.documento d ON d.docid = sv.docid
            WHERE d.esdid IN(".ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO.") AND sv.sldstatus = 'A' AND o.obrid = $obrid
            ORDER BY sv.supid DESC";
        $ultSupidSolicitacao = $db->pegaUm($sql);
        $supervisao = new Supervisao();
        $ultSupid = $supervisao->pegaUltSupidByObra($obrid);
        if($ultSupidSolicitacao == $ultSupid){
            $habilitado = false;
            $habilita = 'N';
            $msgB .= '<p style=" font-weight: bold;color:#cc0000;"><img src="../imagens/exclama.gif"/> Já existe uma solicitação referente a última vistória. Para fazer uma nova solicitação é necessário inserir uma nova vistoria.</p>';
        }


        // Verifica se cronograma da obra possui etapas vencidas
        $sql = "SELECT
                COUNT(*)
            FROM obras2.obras o
            JOIN obras2.cronograma c ON c.obrid = o.obrid AND c.crostatus = 'A'
            JOIN obras2.itenscomposicaoobra ic ON ic.obrid = o.obrid AND ic.croid = c.croid AND ic.icostatus = 'A' AND ic.relativoedificacao = 'D'
            JOIN obras2.itenscomposicao i ON i.itcid = ic.itcid AND i.itcstatus = 'A'
            WHERE o.obrid = $obrid AND (ic.icodterminoitem < now() AND icopercexecutado < 100)";
        if ($db->pegaUm($sql) > 0) {
            $habilitado = false;
            $habilita = 'N';
            $msgB .= '<p style=" font-weight: bold;color:#cc0000;"><img src="../imagens/exclama.gif"/> Para fazer a solicitação é necessário atualizar as datas de início e término de cada etapa do cronograma, conforme previsão de execução física da obra. Clique <a href="#" onclick="abrePopupEditaCronograma('.$obrid.')">aqui</a> para editar os prazos do cronograma.</p>';
        }

        // Verificar se o percentual da solicitação é valido
        $solicitacaoDesembolso = new SolicitacaoDesembolso();
        $percSolicitado = (isset($_REQUEST['sldid'])) ? $sldpercsolicitado : $solicitacaoDesembolso->pegaPercentualSolicitacao($obrid, null);

        if($percSolicitado < 3){
            $habilitado = false;
            $habilita = 'N';
            $msgB .= '<p style=" font-weight: bold;color:#cc0000;"><img src="../imagens/exclama.gif"/> Para fazer a solicitação é necessário um percentual mínimo de 3%.</p>';
        }
    }
}


if($_GET['download']){
    require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    $obraArquivo = new ObrasArquivos();
    $arDados = $obraArquivo->buscaDadosPorArqid( $_GET['download'] );
    $eschema = ($arDados[0]['obrid_1'] ? 'obras' : 'obras2');

    $file = new FilesSimec(null,null,$eschema);
    $file->getDownloadArquivo( $_GET['download'] );

    die('<script type="text/javascript">
			window.close();
		  </script>');
}

if($_POST){
    $dados = $_POST;
    $solicitacaoDesembolso = new SolicitacaoDesembolso();

    if ($dados['sldid']) {
        $solicitacaoDesembolso->carregarPorId($dados['sldid']);
    } else {

        $sql = "
            SELECT
                COUNT(*)
            FROM obras2.solicitacao_desembolso sv
            JOIN obras2.obras o ON o.obrid = sv.obrid AND o.obridpai IS NULL AND o.obrstatus IN ('A', 'P')
            JOIN workflow.documento d ON d.docid = sv.docid
            WHERE d.esdid NOT IN(".ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO.", ".ESDID_SOLICITACAO_DESEMBOLSO_INDEFERIDO.") AND sv.sldstatus = 'A' AND o.obrid = $obrid";

        if ($db->pegaUm($sql) > 0) {
            echo '<script type="text/javascript">
                        alert("Solicitação enviada com sucesso.");
                        window.close();
                  </script>';
            exit;
        }

        $solicitacaoDesembolso->docid = wf_cadastrarDocumento(TPDID_SOLICITACAO_DESEMBOLSO, 'Fluxo da Solicitação de Desembolso');
        $solicitacaoDesembolso->usucpf = $_SESSION['usucpf'];
        $solicitacaoDesembolso->commit();
    }

    if ($_FILES['arqid']['name'] != '') {
        require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $file = new FilesSimec('arqid', null, 'obras2');
        $file->setUpload('Boletim de Medição', 'arqid', false);
        $arqid = $file->getIdArquivo();
        $solicitacaoDesembolso->arqid = $arqid;
    }


    $solicitacaoDesembolso->obrid = $obrid;
    $solicitacaoDesembolso->sldjustificativa = $dados['sldjustificativa'];
    $solicitacaoDesembolso->sldobs = $dados['sldobs'];

    $solicitacaoDesembolso->sldpercsolicitado = desformata_valor($dados['sldpercsolicitado']);
    $solicitacaoDesembolso->sldpercpagamento = desformata_valor($dados['sldpercpagamento']);
    $solicitacaoDesembolso->sldobstec = $dados['sldobstec'];
    $solicitacaoDesembolso->supid = $dados['supid'];

    $solicitacaoDesembolso->salvar();
    $solicitacaoDesembolso->commit();

    foreach ($dados['qsvresposta'] as $key => $value) {
        $questao = new QuestaoSolicitacaoDesembolso();
        if (!$questao->carregaPorQstideSldid($key, $solicitacaoDesembolso->sldid)) {
            $questao->sldid = $solicitacaoDesembolso->sldid;
            $questao->qstid = $key;
        }
        $questao->qsdresposta = $value;
        $questao->salvar();
    }

    if ($_FILES['arquivo']['name'][0] != '') {

        require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        $arquivos = $_FILES;

        foreach ($arquivos['arquivo']['name'] as $key => $value) {

            if(empty($value))
                continue;

            $files = array(
                'arquivo' => array(
                    'name' => $arquivos['arquivo']['name'][$key],
                    'type' => $arquivos['arquivo']['type'][$key],
                    'tmp_name' => $arquivos['arquivo']['tmp_name'][$key],
                    'error' => $arquivos['arquivo']['error'][$key],
                    'size' => $arquivos['arquivo']['size'][$key],
                )
            );
            $_FILES = $files;
            $file = new FilesSimec('arquivo', null, 'obras2');
            $file->setPasta('obras2');
            $file->setUpload($dados['arquivodescricao'][$key], 'arquivo', false);
            $arqid = $file->getIdArquivo();


            if ($arqid) {
                $_POST['arquivo'][] = $arqid;

                $arquivoSolicitacao = new SolicitacaoDesembolsoArquivos();
                $arquivoSolicitacao->sldid = $solicitacaoDesembolso->sldid;
                $arquivoSolicitacao->arqid = $arqid;
                $arquivoSolicitacao->sdastatus = 'A';
                $arquivoSolicitacao->salvar();
            }
        }

        verificaArqid($solicitacaoDesembolso->sldid, $_POST['arquivo']);

    }
    $solicitacaoDesembolso->commit();

    if ($_POST['tramitacao'] == 1){
        wf_alterarEstado($solicitacaoDesembolso->docid, AEDID_SOLICITACAO_DESEMBOLSO_CORRECAO_PARA_ANALISE, '', array('obrid' => $solicitacaoDesembolso->obrid, 'sldid' => $solicitacaoDesembolso->sldid));
    }

    if(empty($dados['sldid'])) {
        // Quando o percentual da obra é 20% menor que o percentual previsto no cronograma, cria uma inconformidade
        $percentualTotalPrevisto = $solicitacaoDesembolso->percentualCronogramaExecutado($obrid);
        $percentTotal = $obra->pegaPercentualExecucao($obrid);

        if (($percentTotal + 20) <= $percentualTotalPrevisto) {

            $date = new DateTime(date('Y-m-d'));
            $date->modify('+30 day');

            $restricao = new Restricao();
            $arDado = array(
                'tprid' => 13,
                'fsrid' => 1,
                'empid' => NULL,
                'obrid' => $obrid,
                'usucpf' => '',
                'rstdsc' => 'A execução física da obra está divergente do cronograma preenchido.',
                'rstdtprevisaoregularizacao' => $date->format('Y-m-d'),
                'rstdscprovidencia' => 'O cronograma da obra deve ser atualizado, conforme, execução física. Utilize a opção "Editar prazos do cronograma" existente na  lista de opções da obra.',
                'rstitem' => 'I'
            );

            $restricao->popularDadosObjeto($arDado)->salvar(true, true, $arCamposNulo);
            $restricao->atualizaDocidNullRetricao($restricao->rstid);
            $restricao->clearDados();
        }
    }

    echo '<script type="text/javascript">
            alert("Solicitação enviada com sucesso.");
            document.location.href = "/obras2/obras2.php?modulo=principal/popupSolicitarDesembolso&acao=A" + "&sldid=' . $solicitacaoDesembolso->sldid . '";
      </script>';
    exit;
    extract($dados);

}

if($_REQUEST['sldid']){
    $solicitacaoDesembolso = new SolicitacaoDesembolso($_REQUEST['sldid']);
    extract($solicitacaoDesembolso->getDados());
    $estado = wf_pegarEstadoAtual($docid);
//    if($estado['esdid'] == ESDID_DEFERIDO || $estado['esdid'] == ESDID_INDEFERIDO){
        $habilitado = false;
        $habilita = 'N';
//    }

    if($estado['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_CORRECAO){
        $habilitado = true;
        $habilita = 'S';
    }
}


$s = new SolicitacaoDesembolso();

$questao = new Questao();
$sldidS = ($sldid) ? $sldid : "NULL";
$questionario = $questao->pegaTodaEstruturaSolicitacaoDesembolso($sldidS);
$divisao = '';
$c = 0;
$validacaoMsg = array();
$obra = new Obras($obrid);
$doc = wf_pegarDocumento($obra->docid);
$ops = array('1.1', '1.2', '1.3', '1.4', '1.5', '1.6', '2.1');

foreach($questionario as $key => $questao){
    $validador = new Validador();
    $check = $validador->check($obrid, $questao['vdrid']);

    if(empty($questionario[$key]['qsvresposta']))
        $questionario[$key]['qsvresposta'] = ($check) ? 't' : 'f';

    if(!$check && $validador->vdrobrigatorio == 't'){

        if($doc['esdid'] == 693 && in_array($questao['qstnumero'], $ops )){
            continue;
        }

        $habilitado = false;
        $habilita = 'N';
        $validacaoMsg[$questao['qstnumero']] = str_replace('||obrid||', $obrid, $validador->getMessage());
    }
}

echo '<br />';
monta_titulo( 'Solicitação de Desembolso', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.');
?>

<html>
<head>
	<title>Solicitação de Desembolso</title>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
</head>

<body topmargin="0" leftmargin="0">

    <?=cabecalhoObra($obrid, 'simples');?>

    <form method="post" id="formulario" name="formulario" enctype="multipart/form-data">
        <input type="hidden" name="tramitacao" value="" />
        <input type="hidden" name="sldid" value="<?=$sldid?>" />
        <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">

            <?
            $obrasVermelho = $s->verificaObrasEmVermelho($obrid);
            if($obrasVermelho):
                $habilitado = false;
                $habilita = 'N';
                $msgB = '<p style=" font-weight: bold;color:#cc0000;"><img src="../imagens/exclama.gif"/> O seu município possui obras desatualizadas. É necessário que todas as obras estejam atualizadas para fazer a solicitação de desembolso.</p>';
            endif; ?>

            <td colspan="3">
                <? if(!empty($validacaoMsg) || !empty($msgB)):?>
                    <div style="border: 2px solid #cc0000; padding: 10px; margin: 10px; font-size: 14px;">
                        <p style="font-weight: bold; text-align: center; color: #cc0000; font-size: 16px;">Providências</p>
                        <? foreach ($validacaoMsg as $key => $msg): ?>
                            <p style=" font-weight: bold;color:#cc0000;"><img src="../imagens/exclama.gif"/> <?=(is_string($key)) ? $key . ') ' : ''?><?=$msg?></p>
                        <? endforeach; ?>
                        <?=$msgB?>
                    </div>
                <? endif; ?>
            </td>

            <? if($sldid): ?>
            <tr>
                <td class="SubTituloDireita" width="20%">Situação da solicitação:</td>
                </td>
                <td>
                    <? if (possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))): ?>
                        <b><?=$estado['esddsc']?></b>
                    <? else: ?>

                        <? if ($estado['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO) : ?>
                            <b style="color:#00AA00"><?=$solicitacao['esddsc']?></b>
                        <? elseif ( $estado['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_INDEFERIDO) : ?>
                            <b style="color:red"><?=$estado['esddsc']?></b>
                        <? elseif ( $estado['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_CORRECAO) : ?>
                            <b><?=$estado['esddsc']?></b>
                        <? else: ?>
                            <b style="">Aguardando Análise FNDE</b>
                        <? endif; ?>
                    <? endif; ?>

                </td>
            </tr>
        <? endif; ?>

            <tr>
                <td class="SubTituloDireita" width="20%">Vencimento do Termo:</td>
                </td>
                <td>
                    <?

                    $sql = "


                            SELECT
                              TO_CHAR( dt_fim_vigencia_termo, 'DD/MM/YYYY') fim_vigencia,
                              dt_fim_vigencia_termo <= NOW() + '30 days'::interval prorrogacao,
                              f.*
                            FROM obras2.vm_vigencia_obra f
                            WHERE
                              obrid = $obrid

                    ";

                    $vencimentotermo = $db->pegaLinha($sql);
                    $fimvigencia = $vencimentotermo['fim_vigencia'];
                    ?>
                    <?php echo campo_texto('fimvigencia', 'S', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', $fimvigencia); ?>
                    <? if ($vencimentotermo['prorrogacao'] == 't'): ?>
                        <? if ($vencimentotermo['fonte'] == 'PAR'): ?>
                        O termo está prestes a vencer, clique <a target="_blank" href="/par/par.php?modulo=principal/documentoParObras&acao=A">AQUI</a> para prorrogar.
                        <? else: ?>
                        O termo está prestes a vencer, clique <a target="_blank" href="/par/par.php?modulo=principal/termoPac&acao=A">AQUI</a> para prorrogar.
                        <? endif;?>
                    <? endif; ?>
                </td>
            </tr>

            <?
            $supervisao = new Supervisao();
            $supid = ($supid) ? $supid : $supervisao->pegaUltSupidByObra($obrid);
            ?>
            <input type="hidden" name="supid" value="<?=$supid?>"/>
            <tr>
                <td class="SubTituloDireita" width="20%">Percentual Execução:</td>
                </td>
                <td>
                    <?
                    $obras = new Obras($obrid);
                    $sldpercexecucao =  $obras->pegaPercentualExecucaoSupervisao($obrid, $supid);
                    ?>
                    <?php echo campo_texto('sldpercexecucao', 'S', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($sldpercexecucao, 2, ',', '.')); ?>
                </td>
            </tr>

            <tr>
                <td class="SubTituloDireita" width="20%">Percentual Execução menos serviços não pactuados:</td>
                </td>
                <td>
                    <?
                    $obras = new Obras($obrid);
                    $sldpercexecucaopactuado = $obras->pegaPercentualExecucaoPactuadoSupervisao($obrid, $supid);
                    ?>
                    <?php echo campo_texto('sldpercexecucaopactuado', 'S', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($sldpercexecucaopactuado, 2, ',', '.')); ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="20%">Diferença entre o percentual de execução menos os serviços não pactuados e recursos financeiros repassados:</td>
                </td>
                <td>
                    <?
                    $solicitacaoDesembolso = new SolicitacaoDesembolso();

                    $sldpercsolicitado = ($sldpercsolicitado) ? $sldpercsolicitado : $solicitacaoDesembolso->pegaPercentualSolicitacao($obrid, $supid);

                    if ($sldpercsolicitado <= 0) {
                        $habilitado = false;
                        $habilita = 'N';
                    }


                    if(possui_perfil(array(PFLCOD_SUPER_USUARIO))){
                        $habilitado = true;
                        $habilita = 'S';
                    }

                    ?>
                    <?php echo campo_texto('sldpercsolicitado', 'S', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($sldpercsolicitado, 2, ',', '.')); ?>
                </td>
            </tr>

            <tr>
                <td class="SubTituloDireita" width="20%">Checklist de solicitação:</td>
                </td>
                <td>
                    <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" align="center">
                        <? foreach($questionario as $questao): ?>
                            <? if ($divisao != $questao['itcdsc']): $divisao = $questao['itcdsc']; $c++;?>
                                <tr>
                                    <th colspan="4"><?=$c?> - <?=$questao['itcdsc']?></th>
                                </tr>
                            <? endif; ?>
                        <tr>
                            <td><span><?=$questao['qstnumero']?></span>) <?=$questao['qstdsc']?></td>
                            <td><input disabled="true" type="radio" value="t" <?= ($questao['qsvresposta'] == 't')?'checked':''; ?> > SIM </td>
                            <td><input disabled="true" type="radio" value="f" <?= ($questao['qsvresposta'] == 'f' || empty($questao['qsvresposta']))?'checked':''; ?> > NÃO <?= ($validador->vdrobrigatorio == 't') ? '<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">' : '' ?></td>
                            <td><img title="<?=simec_htmlspecialchars($questao['qstobs'])?>" src="/imagens/<?= ($questao['qsvresposta'] == 't') ? 'valida1.gif' : 'valida5.gif'; ?>" /></td>
                        </tr>
                            <input type="hidden" value="<?= $questao['qsvresposta'] ?>" name="qsvresposta[<?= $questao['qstid'] ?>]"/>
                        <? endforeach; ?>
                    </table>
                </td>
                <td style="padding: 16px;">
                    <?php
                    if($docid && possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) wf_desenhaBarraNavegacao($docid, array('obrid' => $obrid, 'sldid' => $sldid));
                    ?>
                </td>
            </tr>

            <tr>
                <td class="SubTituloDireita" width="20%">Justificativa:</td>
                <td>
                    <?
                        $obrigatorio = ($sldpercsolicitado < 10) ? 'S' : 'N';
                    ?>
                    <? echo campo_textarea('sldjustificativa', $obrigatorio, $habilita, '', 100, 5, '', '', '', '', '', '');?>
                </td>
                <td></td>
            </tr>

            <tr>
                <td class="SubTituloDireita" width="20%">Observação:</td>
                <td>
                    <? echo campo_textarea('sldobs', 'N', $habilita, '', 100, 5, '', '', '', '', '', '');?>
                </td>
                <td></td>
            </tr>


<!--            <tr>-->
<!--                <td class="SubTituloDireita" width="20%">Boletim de Medição:</td>-->
<!--                <td>-->
<!--                    --><?// if($arqid): $arquivo = new Arquivo($arqid); ?>
<!--                        <input type="hidden" value="--><?//=$arquivo->arqid?><!--" name="arquivo[]" id="arquivo"/>-->
<!--                        <a target="_blank" href="/obras2/obras2.php?modulo=principal/popupSolicitarVinculada&acao=A&download=--><?//=$arquivo->arqid?><!--">-->
<!--                            --><?//=$arquivo->arqnome?><!--.--><?//=$arquivo->arqextensao?>
<!--                        </a> <br /><br />-->
<!--                        <input --><?//= (!$habilitado) ? 'disabled="disabled"' : '' ?><!-- type="file"  name="arqid" id="arqid"/>-->
<!--                    --><?// else: ?>
<!--                        <input --><?//= (!$habilitado) ? 'disabled="disabled"' : '' ?><!-- type="file" class="obrigatorio" name="arqid" id="arqid"/>-->
<!--                    --><?// endif; ?>
<!--                </td>-->
<!--                <td></td>-->
<!--            </tr>-->


            <tr>
                <td class="SubTituloDireita" style="width: 190px;">Anexos</td>
                <td colspan="2">
                    <div>
                        <table id="table_anexos" align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                            <tr>
                                <td class="">
                                    <? if ($habilitado): ?>
                                        <div style=""><img src="/imagens/gif_inclui.gif" alt=""/> <a href="" id="adicionar_anexo">Adicionar</a></div>
                                    <? endif; ?>
                                </td>
                                <td>Nome</td>
                                <td>Descrição</td>
                            </tr>

                            <?
                            $arquivosV = new SolicitacaoDesembolsoArquivos();
                            $arquivos = ($sldid) ? $arquivosV->pegaArquivosPorSlvid($sldid) : array();
                            ?>
                            <? if (!empty($arquivos)): ?>
                                <? foreach ($arquivos as $arquivo): ?>
                                    <tr class="anexos">
                                        <td class="SubTituloEsquerda" style="width: 56px;">
                                            <? if ($habilitado): ?>
                                                <span style=""><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo" href="">Excluir</a></span>
                                            <? endif; ?>
                                        </td>
                                        <td class="SubTituloEsquerda">
                                            <input type="hidden" value="<?=$arquivo['arqid']?>" name="arquivo[]" id="arquivo"/>
                                            <a target="_blank" href="/obras2/obras2.php?modulo=principal/popupSolicitarVinculada&acao=A&download=<?=$arquivo['arqid']?>">
                                                <?=$arquivo['arqnome']?>.<?=$arquivo['arqextensao']?>
                                            </a>
                                        </td>
                                        <td class="SubTituloEsquerda"><input <?= (!$habilitado) ? 'disabled="disabled"' : '' ?>  type="text" maxlength="255" size="50" value="<?=$arquivo['arqdescricao']?>" name="arquivodescricao[]" id="arquivodescricao"/></td>
                                    </tr>
                                <? endforeach; ?>
                            <? endif; ?>
                            <? if ($habilitado): ?>
                                <tr class="anexos">
                                    <td class="SubTituloEsquerda" style="width: 56px;"><span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo" href="">Excluir</a></span></td>
                                    <td class="SubTituloEsquerda"><input type="file" name="arquivo[]" id="arquivo"/></td>
                                    <td class="SubTituloEsquerda"><input maxlength="255" size="50" type="text" name="arquivodescricao[]" id="arquivodescricao"/></td>
                                </tr>
                            <? endif; ?>

                        </table>
                    </div>
                </td>
            </tr>

            <?
            if(
                $estado['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_TECNICA ||
                $estado['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_INDEFERIDO ||
                $estado['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO
            ){

                $habilitaAnalise = 'N';
                if($estado['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_TECNICA && possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))){
                    $habilitaAnalise = 'S';
                }
                ?>
            <tr>
                <td class="SubTituloDireita" width="20%">Percentual Pagamento:</td>
                <td>
                    <?php echo campo_texto('sldpercpagamento', 'S', $habilitaAnalise, '', 7, 6, '#######,##', '', 'right', '', 0, '', '', number_format($sldpercpagamento, 2, ',', '.')); ?>
                </td>
                <td></td>
            </tr>

            <tr>
                <td class="SubTituloDireita" width="20%">Observação:</td>
                <td>
                    <? echo campo_textarea('sldobstec', 'N', $habilitaAnalise, '', 100, 5, '', '', '', '', '', '');?>
                </td>
                <td></td>
            </tr>
            <?
            }
            ?>

            <tr bgcolor="#DEDEDE">
                <td colspan="3">
                    <?
                    if(!$habilitado && $habilitaAnalise != 'S'): ?>

                        <input class="disabled" type="button" name="botao" value="Solicitar" onclick="" title="<?= (!empty($validacaoMsg) || !empty($msgB)) ? 'Verificar o quadro de providências no topo desta página.' : 'Aguardando Análise FNDE'?>"/>

                    <? else: ?>
                        <input type="button" name="botao" value="<?= ($habilitaAnalise == 'S' || isset($_REQUEST['sldid'])) ? 'Salvar' : 'Solicitar'?>" onclick="salvaSolicitacao();"/>

                        <? if($estado['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_CORRECAO): ?>
                            <input type="button" name="botao" value="Enviar para Análise FNDE" onclick="tramitaAnalise();"/>
                        <? endif; ?>

                    <? endif; ?>
                    <input type="button" name="botao" value="Fechar" onclick="window.close()"/>
                </td>
            </tr>



        </table>
    </form>
</body>
<script type="text/javascript">
    $(function(){
        $('input[name="sldnovocontrato"]').click(function(){
            if($(this).val() == 't')
                $('.novo_percentual').hide();
            else
                $('.novo_percentual').show();
        });
        $('#adicionar_anexo').click(function (e) {
            $('#table_anexos').append($('<tr class="anexos anexos-base"><td class="SubTituloEsquerda" style="width: 56px;"><span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo" href="">Excluir</a></span></td><td class="SubTituloEsquerda"><input type="file" name="arquivo[]" id="arquivo"/></td><td class="SubTituloEsquerda"><input <?= (!$habilitado) ? 'disabled="disabled"' : '' ?>   maxlength="255" size="50" type="text" name="arquivodescricao[]" id="arquivodescricao"/></td></tr>').removeClass('anexos-base'));
            e.preventDefault();
        });
        $('.excluir_anexo').live('click',function (e) {
            $(this).parents('tr.anexos').remove();
            e.preventDefault();
        });
    });


    function tramitaAnalise() {
        $('[name="tramitacao"]').val(1);
        $('#formulario').submit();
    }

    function salvaSolicitacao(){
        var msg = '';

        <? if($obrigatorio == 'S'): ?>
        if(!$('[name="sldjustificativa"]').val()){
            msg += "Informe a Justificativa.\n";
        }
        <? endif; ?>
//        if(!$('[name="arqid"]').val()){
//            msg += "Informe o Boletim de Medição.\n";
//        }

        if(msg != ''){
            alert(msg);return false;
        }

        $('#formulario').submit();
    }

    function abreSolicitacao(sldid){
        window.location = '?modulo=principal/popupSolicitarDesembolso&acao=A&sldid=' + sldid;
    }
</script>



<?


function verificaArqid($sldid, $arqid)
{
    if(is_array($arqid)) {
        foreach ($arqid as $key => $id) {
            $arquivo = new Arquivo($id);
            $arquivo->arqdescricao = $_POST['arquivodescricao'][$key];
            $arquivo->salvar();
            $arquivo->commit();
        }
    }

    $arqid = (is_array($arqid)) ? $arqid : array();
    $arquivosV = new SolicitacaoDesembolsoArquivos();
    $arquivos = $arquivosV->pegaArquivosPorSlvid($sldid);

    if(!empty($arquivos)) {
        foreach ($arquivos as $arquivo) {
            if (array_search($arquivo['arqid'], $arqid) === false) {
                $arquivosV->carregarPorId($arquivo['svaid']);
                $arquivosV->svastatus = 'I';
                $arquivosV->salvar();
                $arquivosV->commit();
                $arquivosV->clearDados();
            }
        }
    }
}


//$solicitacaoDesembolso = new SolicitacaoDesembolso();
//$solicitacaoDesembolso->pegaPercentualSolicitacao($obrid);

?>

<script type="text/javascript">

    function abrePopupEditaCronograma(obrid){
        var url = "/obras2/obras2.php?modulo=principal/etapas_da_obra&acao=E" + "&obrid=" + obrid;
        popup1 = window.open(
            url,
            "editarCronograma",
            "width=1200,height=500,scrollbars=yes,scrolling=no,resizebled=no"
        );

        return false;
    }


    function abreAditivoContrato(obrid){
        var url = "/obras2/obras2.php?modulo=principal/popUpInserirAditivoCronograma&acao=A" +
            "&obrid=" + obrid + "&crtid=" + 35676;
        popup1 = window.open(
            url,
            "editarAditivo",
            "width=1200,height=500,scrollbars=yes,scrolling=no,resizebled=no"
        );

        return false;
    }

    function abreEditarLicitacao(obrid){
        var url = "/obras2/obras2.php?modulo=principal/popupEditarLicitacao&acao=E" +
            "&obrid=" + obrid;
        popup1 = window.open(
            url,
            "editarLicitacao",
            "width=1200,height=500,scrollbars=yes,scrolling=no,resizebled=no"
        );

        return false;
    }


    function abreEditarContrato(obrid){

        var url = "/obras2/obras2.php?modulo=principal/popupEditarContrato&acao=A" +
            "&obrid=" + obrid + "&crtid=" + 35676;
        popup = window.open(
            url,
            "editarContrato",
            "width=1000,height=500,scrollbars=yes,scrolling=no,resizebled=no"
        );

        return false;
    }


    function abreSolicitacoes(obrid, tipo){
        var url = "/obras2/obras2.php?modulo=principal/solicitacao&acao=A" +
            "&obrid=" + obrid +
            "&tslid[]=" + tipo;
        popup1 = window.open(
            url,
            "solicitarVinculada",
            "width=1200,height=500,scrollbars=yes,scrolling=no,resizebled=no"
        );

        return false;
    }

</script>