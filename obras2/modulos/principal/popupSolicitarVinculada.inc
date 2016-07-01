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
    $solicitacaoVinculada = new SolicitacaoVinculada();

    if ($dados['slvid']) {
        $solicitacaoVinculada->carregarPorId($dados['slvid']);
    } else {
        $solicitacaoVinculada->docid = wf_cadastrarDocumento(TPDID_SOLICITACAO_VINCULADA, 'Fluxo da Solicitação de Criação de Obra Vinculada');
        $solicitacaoVinculada->commit();
    }

    if ($_FILES['arqid']['name'] != '') {
        require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $file = new FilesSimec('arqid', null, 'obras2');
        $file->setUpload('Documento de publicação do novo edital', 'arqid', false);
        $arqid = $file->getIdArquivo();
        $solicitacaoVinculada->arqid = $arqid;
    }

    $solicitacaoVinculada->obrid = $obrid;
    $solicitacaoVinculada->supid = $dados['supid'];
    $solicitacaoVinculada->slvpercultvistoria = $dados['slvpercultvistoria'];
    $solicitacaoVinculada->slvpercnovocontrato = ($dados['slvnovocontrato'] == 't') ? $dados['slvpercultvistoria'] : $dados['slvpercnovocontrato'];
    $solicitacaoVinculada->slvjustificativa = $dados['slvjustificativa'];
    $solicitacaoVinculada->slvobs = $dados['slvobs'];
    $solicitacaoVinculada->usucpf = $_SESSION['usucpf'];
    $solicitacaoVinculada->slvnovocontrato = $dados['slvnovocontrato'];
    $solicitacaoVinculada->salvar();

    foreach ($dados['qsvresposta'] as $key => $value) {
        $questao = new QuestaoSolicitacaoVinculada();
        if (!$questao->carregaPorQstideSlvid($key, $solicitacaoVinculada->slvid)) {
            $questao->slvid = $solicitacaoVinculada->slvid;
            $questao->qstid = $key;
        }
        $questao->qsvresposta = $value;
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
            $_POST['arquivo'][] = $arqid;

            $arquivoSolicitacao = new SolicitacaoVinculadaArquivos();
            $arquivoSolicitacao->slvid = $solicitacaoVinculada->slvid;
            $arquivoSolicitacao->arqid = $arqid;
            $arquivoSolicitacao->svastatus = 'A';
            $arquivoSolicitacao->salvar();
        }

    }
    verificaArqid($solicitacaoVinculada->slvid, $_POST['arquivo']);

    $solicitacaoVinculada->commit();

    echo '<script type="text/javascript">
            alert("Solicitação enviada com sucesso.");
            document.location.href = "/obras2/obras2.php?modulo=principal/popupSolicitarVinculada&acao=A" + "&slvid=' . $solicitacaoVinculada->slvid . '";
      </script>';
    exit;
    extract($dados);

}



if($_REQUEST['slvid']){
    $solicitacaoVinculada = new SolicitacaoVinculada($_REQUEST['slvid']);
    extract($solicitacaoVinculada->getDados());
    $estado = wf_pegarEstadoAtual($docid);
//    if($estado['esdid'] == ESDID_DEFERIDO || $estado['esdid'] == ESDID_INDEFERIDO){
        $habilitado = false;
        $habilita = 'N';
//    }

    if($estado['esdid'] == ESDID_INDEFERIDO && possui_perfil( $pflcods )){
        $habilitado = true;
        $habilita = 'S';
    }
} else {

    $sql = "SELECT
            sv.slvid
        FROM obras2.solicitacao_vinculada sv
        JOIN workflow.documento d ON d.docid = sv.docid
        WHERE sv.slvstatus = 'A'  AND sv.obrid = $obrid AND d.esdid IN (1429, 1427)";
    $solilcitacaoExistente = $db->pegaUm($sql);

    if ($solilcitacaoExistente){
        $habilitado = false;
        $habilita = 'N';
        $msg2 = 'Já existe uma solicitação em andamento.';
    }

}

$questao = new Questao();
$slvidS = ($slvid) ? $slvid : "NULL";
$questionario = $questao->pegaTodaEstruturaSolicitacaoVinculada($slvidS);
$divisao = '';
$c = 0;
$validacaoMsg = array();

foreach($questionario as $key => $questao){
    $validador = new Validador();
    $check = $validador->check($obrid, $questao['vdrid']);

    if(empty($questionario[$key]['qsvresposta']))
        $questionario[$key]['qsvresposta'] = ($check) ? 't' : 'f';

    if(!$check && $validador->vdrobrigatorio == 't'){
        $habilitado = false;
        $habilita = 'N';
        $validacaoMsg[] = $validador->getMessage();
    }
}

echo '<br />';
monta_titulo( 'Pedido de Criação de Obra Vinculada', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.');
?>

<html>
<head>
	<title>Pedido de Criação de Obra Vinculada</title>
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
        <input type="hidden" name="slvid" value="<?=$slvid?>" />
        <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
            <tr>
                <td class="SubTituloDireita" width="20%">Última vistoria:</td>
                <td colspan="2">
                    <?
                    $supervisao = new Supervisao();
                    if(!$supid)
                        $vistoria = $supervisao->pegaUltimaVistoriaUnidade($obrid);
                    else
                        $vistoria = $supervisao->pegaVistoriaUnidade($supid);
                    ?>
                    <input type="hidden" name="supid" value="<?=$vistoria['supid']?>"/>
                    <table class="tabela" align="center" style="width: 98%;">
                        <tr>
                            <?php
                            $i = 1;
                            $cabecalho = array(
                                "ID",
                                "Data Acompanhamento",
                                "Data Inclusão",
                                "Inserido Por",
                                "Situação da Obra",
                                "Responsável",
                                "Realizada Por",
                                "% do Acompanhamento",
                                "Validada pelo Supervisor");

                                foreach($cabecalho as $indices => $titulos)
                                    echo '<th>'.$titulos.'</th>';
                            ?>
                        </tr>
                        <tr>
                            <td align="center">
                                <?=$vistoria['supid']?>
                            </td>
                            <td align="center">
                                <?=$vistoria['supdata']?>
                            </td>
                            <td align="center">
                                <?=$vistoria['supdtinclusao']?>
                            </td>
                            <td align="center">
                                <?=$vistoria['usunome']?>
                            </td>
                            <td align="center">
                                <?=$vistoria['stadesc']?>
                            </td>
                            <td align="center">
                                <?=$vistoria['responsavel']?>
                            </td>
                            <td align="center">
                                <?=$vistoria['vistoriador']?>
                            </td>
                            <td align="center">
                                <?=number_format($vistoria['percentual'],2,',','.')?>
                                <? $slvpercultvistoria = $vistoria['percentual']; ?>
                                <input type="hidden" name="slvpercultvistoria" value="<?=$slvpercultvistoria?>" />
                            </td>
                            <td align="center">
                                <?=$vistoria['validado']?>
                            </td>

                        </tr>
                    </table>
                </td>
            </tr>

            <tr>
                <td class="SubTituloDireita" width="20%">Aproveitamento do contrato anterior:</td>
                <td>
                    <p style="font-size: 16px; font-weight: bold;">
                        A última vistoria indicou um percentual total de execução de <?=number_format($vistoria['percentual'],2,',','.')?>% com <?= number_format(100 - $vistoria['percentual'],2,',','.')?>% para ser concluído.
                        O percentual a ser executado no novo contrato é o mesmo apresentado de acordo com a última vistoria dessa obra
                    </p>
                    <input type="radio" <?= (!$habilitado) ? 'disabled="disabled"' : '' ?>  name="slvnovocontrato" id="" <?=( $slvnovocontrato == 't' ) ? "checked='checked'" : ''?> value="t"> Sim <input <?= (!$habilitado) ? 'disabled="disabled"' : '' ?>  type="radio" name="slvnovocontrato" id=""  <?=($slvnovocontrato=='f') ? "checked='checked'" : ''?>  value="f"> Não <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."><br />
                    <p class="novo_percentual" style="<?=( $slvnovocontrato != 'f' ) ? "display: none" : ''?>">Informe o percentual total correspondente a execucao física da obra <?php echo campo_texto('slvpercnovocontrato','S', 'S','', 5, 20, '###.##','', '', '', '', 'id="slvpercnovocontrato"', 'validaPercentual(this)');?> </p>
                </td>
            </tr>

            <tr>
                <td class="SubTituloDireita" width="20%">Checklist de criação:</td>
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
                            <td><input disabled="true" type="radio" value="f" <?= ($questao['qsvresposta'] == 'f' || empty($questao['qsvresposta']))?'checked':''; ?> <?= ($questao['qsvresposta'] == 'f')?'title="'.$validador->getMessage().'"':''; ?> > NÃO <?= ($validador->vdrobrigatorio == 't') ? '<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">' : '' ?></td>
                            <td><img title="<?=$questao['qstobs']?>" src="/imagens/<?= ($questao['qsvresposta'] == 't') ? 'valida1.gif' : 'valida5.gif'; ?>" /></td>
                        </tr>
                            <input type="hidden" value="<?= $questao['qsvresposta'] ?>" name="qsvresposta[<?= $questao['qstid'] ?>]"/>
                        <? endforeach; ?>
                    </table>
                </td>
                <td style="padding: 16px;">
                    <?php
                    if($docid) wf_desenhaBarraNavegacao($docid, array('obrid' => $obrid, 'slvid' => $slvid));
                    ?>
                </td>
            </tr>

            <tr>
                <td class="SubTituloDireita" width="20%">Justificativa:</td>
                <td>
                    <? echo campo_textarea('slvjustificativa', 'S', $habilita, '', 100, 5, '', '', '', '', '', '');?>
                </td>
                <td></td>
            </tr>

            <tr>
                <td class="SubTituloDireita" width="20%">Observação:</td>
                <td>
                    <? echo campo_textarea('slvobs', 'N', $habilita, '', 100, 5, '', '', '', '', '', '');?>
                </td>
                <td></td>
            </tr>

            <tr>
                <td class="SubTituloDireita" width="20%">Documento de publicação do novo edital:</td>
                <td>
                    <? if($arqid): $arquivo = new Arquivo($arqid); ?>
                        <input type="hidden" value="<?=$arquivo->arqid?>" name="arquivo[]" id="arquivo"/>
                        <a target="_blank" href="/obras2/obras2.php?modulo=principal/popupSolicitarVinculada&acao=A&download=<?=$arquivo->arqid?>">
                            <?=$arquivo->arqnome?>.<?=$arquivo->arqextensao?>
                        </a> <br /><br />
                        <input <?= (!$habilitado) ? 'disabled="disabled"' : '' ?> type="file"  name="arqid" id="arqid"/>
                    <? else: ?>
                        <input <?= (!$habilitado) ? 'disabled="disabled"' : '' ?> type="file" class="obrigatorio" name="arqid" id="arqid"/> <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
                    <? endif; ?>
                </td>
                <td></td>
            </tr>

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
                            $arquivosV = new SolicitacaoVinculadaArquivos();
                            $arquivos = ($slvid) ? $arquivosV->pegaArquivosPorSlvid($slvid) : array();
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

            <tr bgcolor="#DEDEDE">
                <td colspan="3">
                    <div>
                        <? if ($msg2): ?>
                        <p style=" font-weight: bold;color:#cc0000;"><img src="../imagens/exclama.gif"/> <?=$msg2?></p>
                        <? endif; ?>
                        <? foreach ($validacaoMsg as $msg): ?>
                            <p style=" font-weight: bold;color:#cc0000;"><img src="../imagens/exclama.gif"/> <?=$msg?></p>
                        <? endforeach; ?>
                    </div>
                    <? if($habilitado): ?>
                    <input type="button" name="botao" value="Salvar" onclick="salvaSolicitacao();"/>
                    <? endif; ?>
                    <input type="button" name="botao" value="Fechar" onclick="window.close()"/>
                </td>
            </tr>
        </table>
    </form>
</body>
<script type="text/javascript">
    $(function(){
        $('input[name="slvnovocontrato"]').click(function(){
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
    function salvaSolicitacao(){
        var msg = '';

        if(!$('[name="slvnovocontrato"]:checked').val()){
            msg += "O campo 'Aproveitamento do contrato anterior'' deve ser preenchido.\n";
        }

        if($('[name="slvnovocontrato"]:checked').val() == 'f' && !$('[name="slvpercnovocontrato"]').val()){
            msg += "Informe o percentual correspondente a obra que será executado no âmbito do novo contrato.\n";
        }

        if(!$('[name="slvjustificativa"]').val()){
            msg += "Informe a Justificativa.\n";
        }

        if(!$('.obrigatorio[name="arqid"]').val() && !$("input[name='arquivo\[\]']").val()){
            msg += "O campo 'Documento de publicação do novo edital' é obrigatório.\n";
        }

        if(msg != ''){
            alert(msg);return false;
        }

        $('#formulario').submit();
    }
    function validaPercentual(obj){
        if($(obj).val() > 100)
            $(obj).val('100.00')
    }
    function abreSolicitacao(slvid){
        window.location = '?modulo=principal/popupSolicitarVinculada&acao=A&slvid=' + slvid;
    }
</script>



<?

monta_titulo( 'Solicitaçoes cadastradas', '');

$where[] = "o.obrid = $obrid";

$sql = "


            SELECT
                '<center>
                    <img
                        align=\"absmiddle\"
                        src=\"/imagens/alterar.gif\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: abreSolicitacao(\'' || sv.slvid  || '\');\"
                        title=\"Alterar Obra\">
                 </center>' AS acao,
                o.obrid,
                o.obrnome,
                m.estuf,
                m.mundescricao,
                sv.slvjustificativa,
                u.usunome usunome1,
                sv.slvpercnovocontrato,
                TO_CHAR(sv.slvdatainclusao, 'DD/MM/YYYY') slvdatainclusao,
                e.esddsc,
                (
                    SELECT ud.usunome FROM workflow.historicodocumento h
                    LEFT JOIN workflow.comentariodocumento  c ON c.hstid = c.hstid AND c.docid = d.docid
                    LEFT JOIN seguranca.usuario ud ON ud.usucpf = h.usucpf
                    WHERE h.hstid = d.hstid ORDER BY h.htddata DESC LIMIT 1
                ) as usunome,
                (
                    SELECT TO_CHAR(h.htddata, 'DD/MM/YYYY') FROM workflow.historicodocumento h WHERE h.hstid = d.hstid ORDER BY h.htddata DESC LIMIT 1
                ) as htddata,
                (
                    SELECT c.cmddsc FROM workflow.historicodocumento h
                    LEFT JOIN workflow.comentariodocumento  c ON c.hstid = c.hstid AND c.docid = d.docid
                    WHERE h.hstid = d.hstid ORDER BY h.htddata DESC LIMIT 1
                ) as cmddsc
            FROM obras2.solicitacao_vinculada sv
            JOIN obras2.obras o ON o.obrid = sv.obrid AND o.obridpai IS NULL AND o.obrstatus IN ('A', 'P')
            JOIN obras2.empreendimento emp ON emp.empid = o.empid
            JOIN entidade.endereco ed ON ed.endid = o.endid
            JOIN territorios.municipio m ON m.muncod = ed.muncod
            JOIN seguranca.usuario u ON u.usucpf = sv.usucpf
            JOIN workflow.documento d ON d.docid = sv.docid
            JOIN workflow.estadodocumento e ON e.esdid = d.esdid
            WHERE sv.slvstatus = 'A' " . (count($where) ? ' AND ' . implode(' AND ',$where) : "");

$cabecalho = array('Ação', 'ID', 'Obra', 'UF', 'Município', 'Justificativa', 'Inserido Por', 'Percentual', 'Data de Cadastro',
    'Situação do Deferimento', 'Inserido Por', 'Data de Cadastro do Deferimento', 'Obs. Análise');

$db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', null, "formulario");

function verificaArqid($slvid, $arqid)
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
    $arquivosV = new SolicitacaoVinculadaArquivos();
    $arquivos = $arquivosV->pegaArquivosPorSlvid($slvid);

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


?>