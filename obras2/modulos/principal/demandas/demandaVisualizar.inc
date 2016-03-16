<?php
include_once(APPRAIZ . "obras2/modulos/principal/demandas/_funcoes_demandas.inc");
if ($_REQUEST['exibirfoto']) {
    echo '<img src="/slideshow/slideshow/verimagem.php?&arqid=' . $_REQUEST['arqid'] . '"/>';
    die;
}

function downloadArquivo() {

    require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    $file = new FilesSimec(null, null, 'obras2');
    return $file->getDownloadArquivo($_REQUEST['arqid']);

    echo '
		<script type="text/javascript">
			document.location.href = document.location.href;
		</script>';
}

if ($_REQUEST['req']) {
    $_REQUEST['req']();
    die();
}

$id = $_REQUEST['id'];
if (!$id) {
    // @todo
    // verificar responsabilidade do usuario na supervisao
    // verificar se supervisao existe no banco de dados
    echo "<script>alert('Erro! Supervisão não informada.');self.close()</script>";
}
$_SESSION['obras2']['dmdid'] = $id;

global $db;

$sql = "SELECT * FROM obras2.demandas d
		LEFT  JOIN obras2.demandas_origem dmo ON dmo.dmoid = d.dmoid
      	INNER JOIN obras2.demandastipo dt on d.dmtid = dt.dmtid
       	WHERE dmdid ={$id}";
$demanda = $db->pegaLinha($sql);
$esdid = pegaEstadoAtualDocumento($demanda['docid']);

require_once APPRAIZ . 'includes/workflow.php';
?>
<link href="../includes/JQuery/jquery-1.9.1/css/jquery-ui-1.10.3.custom.css" rel="stylesheet">
<script src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
<script src="../includes/JQuery/jquery-1.9.1/jquery-ui-1.10.3.custom.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1/funcoes.js"></script>

<!-- Add mousewheel plugin -->
<script type="text/javascript" src="/library/fancybox/helpers/jquery.mousewheel-3.0.6.pack.js"></script>

<!-- Add fancyBox main JS and CSS files -->
<script type="text/javascript" src="/library/fancybox/jquery.fancybox.js"></script>
<link rel="stylesheet" type="text/css" href="/library/fancybox/jquery.fancybox.css" media="screen" />

<!-- Add Button helper (this is optional) -->
<link rel="stylesheet" type="text/css" href="/library/fancybox/helpers/jquery.fancybox-buttons.css" />
<script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-buttons.js"></script>

<!-- Add Thumbnail helper (this is optional) -->
<link rel="stylesheet" type="text/css" href="/library/fancybox/helpers/jquery.fancybox-thumbs.css" />
<script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-thumbs.js"></script>

<!-- Add Media helper (this is optional) -->
<script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-media.js"></script>

<style type="text/css">
    .fancybox img{
        box-shadow: 1px 1px 4px 1px #000;
        width: 10px !important;
        height: 10px !important;
        margin-right: 10px;
    }
</style>

<script type="text/javascript">
    $(document).ready(function() {
        $('.fancybox').fancybox();
    });
    $('.downloadArquivo').click(function() {
        window.open('obras2.php?modulo=principal/demandas/cadDemandas&acao=A&req=downloadArquivo&arqid=' + $(this).attr('id'), 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
    });

    alteraEstadoWorkflowDiligencia = function(dmdid, docid)
    {
        window.open('obras2.php?modulo=principal/demandas/popUpAlterarEstadoWorkflow&acao=A&dmdid=' + dmdid + '&docid=' + docid, 'page', "height=650,width=900,scrollbars=yes,top=50,left=200");
    };

    wf_exibirHistorico = function(docid)
    {
        var url = 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/historico.php' +
                '?modulo=principal/tramitacao' +
                '&acao=C' +
                '&docid=' + docid;
        window.open(
                url,
                'alterarEstado',
                'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
                );
    };

    wf_alterarEstado = function(aedid, docid, esdid, acao, recarregar)
    {
        if (acao)
            acao = acao.toLowerCase();
        if (!confirm('Deseja realmente ' + acao + ' ?'))
        {
            return;
        }
        var url = 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/alterar_estado.php' +
                '?aedid=' + aedid +
                '&docid=' + docid +
                '&esdid=' + esdid;
        var janela = window.open(
                url,
                'alterarEstado',
                'width=550,height=520,scrollbars=no,scrolling=no,resizebled=no'
                );

        if (!recarregar) {
            janela.focus();
        } else {
            janela.opener.location.reload();
            janela.close();
        }
    };
</script>
<table class="tabela" bgcolor="#f5f5f5" style="width: 100%" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <th colspan="2">Dados da Demanda</th>
    </tr>
    <tr>
        <td style="width: 100%;" valign="top">
            <table class="tabela" bgcolor="#f5f5f5" style="width: 100%" cellSpacing="1" cellPadding="3" align="center">
                <tr>
                    <td class="SubTituloDireita" valign="top">Código:</td>
                    <td><?php echo $id ?></td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" valign="top">Assunto:</td>
                    <td><?php echo simec_htmlentities($demanda['dmdnome']); ?></td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" valign="top"><label for="descricao_detalhada">Origem: </label></td>
                    <td><?php echo simec_htmlentities($demanda['dmonome']); ?></td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" valign="top"><label for="descricao_detalhada">Tipo: </label></td>
                    <td><?php echo simec_htmlentities($demanda['dmtnome']); ?></td>
                </tr>
                <tr>
                    <td class="SubTituloDireita"><label for="assunto">Prioridade:</label></td>
                    <td><?php echo simec_htmlentities($demanda['dmdprioridade']); ?></td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" valign="top"><label for="dmddescricao">Descri&ccedil;&amacr;o detalhada: </label></td>
                    <td>
                        <?php echo simec_htmlentities($demanda['dmddescricao']); ?>
                    </td>
                </tr>
                <?php if ($esdid == PAR_WF_DEMANDA_ESTADO_EM_DILIGENCIA) { ?>
                    <?php
                    $sql = "SELECT
                                cmddsc
                            FROM
                                workflow.comentariodocumento
                            WHERE
                                hstid = (
                                    SELECT
                                        max(hstid)
                                    FROM
                                        workflow.historicodocumento
                                    WHERE
                                        docid = {$demanda['docid']}
                                )";

                    $cmddsc = $db->pegaUm($sql);

                    if ($cmddsc == '') {
                        $cmddsc = 'Não informado';
                    }
                    ?>
                    <tr>
                        <td class="SubTituloDireita" valign="top"><label for="dmddescricao">Motivo da Rejeição: </label></td>
                        <td>
                            <?= simec_htmlentities($cmddsc); ?>
                        </td>
                    </tr>
                <?php } ?>
                <tr>
                    <td class="SubTituloDireita" width="20%"><label for="dmddescricao">Arquivos: </label></td>
                    <td>
                        <?php
                        $acoes = 'case when substring(arqtipo, 1,5) = \'image\' then
                                    \'<a class="fancybox fancybox.ajax" href="obras2.php?modulo=principal/demandas/demandaVisualizar&acao=A&dmdid=' . $demanda['dmdid'] . '&exibirfoto=1&arqid=\'|| arq.arqid ||\'" data-fancybox-group="gallery" title="\' || arq.arqdescricao || \'">
                                        \' || arq.arqdescricao || \'</a>\'
                                else
                                    \'<a style="margin: 0 -5px 0 5px;" href="#" id="\' || arq.arqid || \'" class="downloadArquivo">
                                        \' || arq.arqdescricao || \'
                                                </a>\' end';

                        if ($demanda['dmdid'] != '') {
                            $cabecalho = array('Anexo');
                            $sql = "SELECT
                                        $acoes as acoes
                                    FROM
                                        obras2.demandasanexo aqs
                                    INNER JOIN public.arquivo arq ON arq.arqid = aqs.arqid
                                    INNER JOIN obras2.demandas dmd ON dmd.dmdid = aqs.dmdid
                                    INNER JOIN workflow.documento doc ON doc.docid = dmd.docid
                                    WHERE
                                        aqsstatus = 'A'
                                        AND dmd.dmdid = {$demanda['dmdid']}";

                            $arrArquivos = $db->carregar($sql);

                            if (is_array($arrArquivos)) {
                                $db->monta_lista_simples($arrArquivos, $cabecalho, 50000, 10, '', '100%');
                            }
                        }
                        ?>
                    </td>
                </tr>
            </table>
        </td>
        <td>
<?php
$perfil = pegaPerfilGeral();
/*
if ($_SESSION['usucpf'] == $demanda['dmdcpfsolicitante'] || $db->testa_superuser() ) {
    require_once APPRAIZ . 'includes/workflow.php';
    // Verifica qual estado do documento.. se for em diligência mostra o box personalizado
    if ($esdid == PAR_WF_DEMANDA_ESTADO_EM_DILIGENCIA || $esdid == PAR_WF_DEMANDA_ESTADO_AGUARDANDO_VALIDACAO) {
        $modificacao = wf_pegarUltimaDataModificacao($demanda['docid']);
        $usuario = wf_pegarUltimoUsuarioModificacao($demanda['docid']);
        $comentario = trim(substr(wf_pegarComentarioEstadoAtual($demanda['docid']), 0, 50)) . "...";
        $estadoAtual = wf_pegarEstadoAtual($demanda['docid']);
        $estados = wf_pegarProximosEstados($demanda['docid'], array());
        ?>

                    <table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">

                        <tr style="background-color: #c9c9c9; text-align:center;">
                            <td style="font-size:7pt; text-align:center;">
                                <span title="estado atual">
                                    <b>Estado atual</b>
                                </span>
                            </td>
                        </tr>
                        <tr style="text-align:center;">
                            <td style="font-size:7pt; text-align:center;">
                                <span title="estado atual">
        <?php echo $estadoAtual['esddsc'] ?>
                                </span>
                            </td>
                        </tr>

                        <tr style="background-color: #c9c9c9; text-align: center;">
                            <td style="font-size: 7pt; text-align: center;">
                                <span title="Análise de Turmas"><strong>Ação</strong></span> 
                            </td>
                        </tr>

        <?php
        foreach ($estados as $estado) {
            // Verifica se o estado do documento de destino é igual a 1215 'Devolver Demanda' ou  
            if ($estado['esdid'] == 1215 || $estado['esdid'] == 1208) {
                $onClick = 'onclick="alteraEstadoWorkflowDiligencia(\'' . $demanda['dmdid'] . '\', \'' . $demanda['docid'] . '\');"';
            } else {
                $onClick = 'onclick="wf_alterarEstado(\'' . $estado['aedid'] . '\', \'' . $demanda['docid'] . '\', \'' . $estado['esdid'] . '\', \'' . $estado['aeddscrealizar'] . '\', true );"';
            }

            $action = wf_acaoPossivel2($demanda['docid'], $estado['aedid'], array());

            if ($action === true) {
                ?>
                                <tr>
                                    <td id="td_acao_<?= $estado['aedid'] ?>" style="font-size: 7pt; text-align: center; border-top: 2px solid #d0d0d0;" onmouseover="this.style.backgroundColor = '#ffffdd';" onmouseout="this.style.backgroundColor = '';">
                                        <a href="#" alt="<?php echo $estado['aeddscrealizar'] ?>" title="<?php echo $estado['aeddscrealizar'] ?>" <?php echo $onClick ?>><?php echo $estado['aeddscrealizar'] ?></a>
                                    </td>
                                </tr>
            <?php }
        }
        ?>

                        <tr style="background-color: #c9c9c9; text-align:center;">
                            <td style="font-size:7pt; text-align:center;">
                                <span title="estado atual">
                                    <b>Histórico</b>
                                </span>
                            </td>
                        </tr>
                        <tr style="text-align:center;">
                            <td style="font-size:7pt; border-top: 2px solid #d0d0d0;">
                                <img
                                    style="cursor: pointer;"
                                    src="http://<?php echo $_SERVER['HTTP_HOST'] ?>/imagens/fluxodoc.gif"
                                    title="<?php echo $usuario['usunome'] . " - " . $modificacao . " - " . simec_htmlentities($comentario); ?>"
                                    onclick="wf_exibirHistorico('<?php echo $demanda['docid'] ?>');"
                                    />
                            </td>
                        </tr>

                    </table>
                    <?php
                } else {
                    //wf_desenhaBarraNavegacao($demanda['docid'], array('docid' => $demanda['docid']));
                }
            }*/
            ?>
        </td>
    </tr>
</table>
<?php
exit;