<?php


function desenhaBotaoAprovacao() {
    ?>
    <table cellspacing="0" cellpadding="3" border="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
        <tr>
            <td onmouseout="this.style.backgroundColor = '';" onmouseover="this.style.backgroundColor = '#ffffdd';" style="font-size: 7pt; text-align: center; 
                border-top: 2px solid rgb(208, 208, 208);">
                <a id="abre_aprovacao" title="Enviar para obra aprovada" alt="Enviar para obra aprovada" href="#"> Enviar para obra aprovada</a>
            </td>
        </tr>
    </table>
    <?php
}

$preid = $_REQUEST['preid'];

$muncod = $_SESSION['par']['muncod'];
$docid = prePegarDocid($preid);
$esdid = prePegarEstadoAtual($docid);
$perfil = pegaArrayPerfil($_SESSION['usucpf']);

monta_titulo('Enviar para análise', '');

$arperfil = pegaArrayPerfil($_SESSION['usucpf']);

$obPreObraControle = new PreObraControle();
$oPreObra = new PreObra();

if ($preid) {

    $qrpid = pegaQrpidPAC($preid, 43);

    $pacFNDE = $oPreObra->verificaObraFNDE($preid, SIS_OBRAS);
    $pacDados = $oPreObra->verificaTipoObra($preid, SIS_OBRAS);
    $pacFotos = $oPreObra->verificaFotosObra($preid, SIS_OBRAS);
    $pacDocumentos = $oPreObra->verificaDocumentosObra($preid, SIS_OBRAS, $pacDados);
    /* 	if($pacFNDE == 'f'){
      $pacDocumentosTipoA = $oPreObra->verificaDocumentosObra($preid, SIS_OBRAS, $pacDados, true);
      } */
    $pacQuestionario = $oPreObra->verificaQuestionario($qrpid);
    $boPlanilhaOrcamentaria = $oPreObra->verificaPlanilhaOrcamentaria($preid, SIS_OBRAS, $preid);
//     ver($boPlanilhaOrcamentaria);
    $pacCronograma = $oPreObra->verificaCronograma($preid);
}


$ptoid = $oPreObra->verificaTipoObra($preid, $sistema);

if( $ptoid == '' ){
	
	$sql = "SELECT sbaid, sobano FROM par.subacaoobra WHERE preid = $preid";
	
	$obra = $db->pegaLinha( $sql );
	
	echo "
		<script>
			window.location.href = 'par.php?modulo=principal/subacaoObras&acao=A&sbaid={$obra['sbaid']}&ano={$obra['sobano']}&preid=$preid';
		</script>";
	die();
}

$boPlanilhaOrcamentaria['faltam'] = $boPlanilhaOrcamentaria['itcid'] - $boPlanilhaOrcamentaria['ppoid'];
$msgPlanilha = 'Falta(m) ' . $boPlanilhaOrcamentaria['faltam'] . ' iten(s) a ser(em) preenchido(s) na planilha orçamentaria.';

$sql = "SELECT ptopreencher FROM obras.pretipoobra WHERE ptoid = $ptoid";
$ptopreencher = $db->pegaUm( $sql );

if( $ptopreencher != 't' ) $msgPlanilha = "Falta salvar a planilha orçamentaria.";

$arPendencias = array(
    'Dados do terreno' => 'Falta o preenchimento dos dados.',
    'Relatório de vistoria' => 'Falta o preenchimento dos dados.',
    'Cadastro de fotos do terreno' => 'Deve conter no mínimo 3 fotos do terreno.',
    'Cronograma físico-financeiro' => 'Falta o preenchimento dos dados.',
    'Documentos anexos' => 'Falta anexar os arquivos.',
    'Projetos - Tipo A' => 'Falta anexar os arquivos.',
    'Itens Planilha orçamentária' => "$msgPlanilha",
    'Planilha orçamentária' => "$msgPlanilha",
//     'Valor da planilha orçamentária' => 'O valor {valor} não confere, deve estar entre R$ ' . formata_valor($boPlanilhaOrcamentaria['minimo']) . ' e R$ ' . formata_valor($boPlanilhaOrcamentaria['maximo']) . '.'
    'Valor da planilha orçamentária' => 'O valor {valor} não confere, deve ser maior que R$ ' . formata_valor($boPlanilhaOrcamentaria['minimo']) . '.'
);

cabecalho();
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<script type="text/javascript">

                function editarProrrogacao(preid) {
                    window.open('par.php?modulo=principal/popupProrrogacaoPACAprovacao&acao=A&editar=1&preid=' + preid,
                            'Prorrogação',
                            "height=400,width=600,scrollbars=yes,top=50,left=200").focus();
                }

                function aceitarProrrogacao(preid) {
                    window.open('par.php?modulo=principal/popupProrrogacaoPACAprovacao&acao=A&preid=' + preid,
                            'Prorrogação',
                            "height=400,width=600,scrollbars=yes,top=50,left=200").focus();
                }
                
                function recusarProrrogacao(preid) {
                    window.open('par.php?modulo=principal/popupProrrogacaoPACAprovacao&acao=A&recusar=S&preid=' + preid,
                            'Prorrogação',
                            "height=400,width=600,scrollbars=yes,top=50,left=200").focus();
                }

                function encaminharDocumentoPar(preid) {
                    window.open('par.php?modulo=principal/documentoParObras&acao=A&preid=' + preid + '&estuf', 'Prorrogação', "scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes").focus();
                }

                function openDocumento(dopid) {
                    window.open('par.php?modulo=principal/documentoParObras&acao=A&req=formVizualizaDocumento&dopid=' + dopid,
                            'Documento PAR',
                            "scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes");
                }

                $(document).ready(function () {
                    $('#abre_aprovacao').click(function () {
                        $.ajax({
                            type: "POST",
                            url: window.location.href,
                            data: "&req=popupAprovacaoReformulacao",
                            async: false,
                            success: function (msg) {
                                $('#dialog-aprovacaoRef').show();
                                $("#dialog-aprovacaoRef").html(msg);
                                $("#dialog-aprovacaoRef").dialog({
                                    resizable: false,
                                    height: 600,
                                    width: 600,
                                    modal: true,
                                    show: {effect: 'drop', direction: "up"},
                                    buttons: {
                                        "Fechar": function () {
                                            $(this).dialog("close");
                                            window.location.reload();
                                        }

                                    }
                                });
                            }
                        });
                    });
                });
</script>
<table class="tabela" align="center">

    <?php
// ver($boPlanilhaOrcamentaria, d);
    $x = 0;
    $arrExcTipoObra = array(16, 9, 21, 35);

    foreach ($arPendencias as $k => $v):
        $cor = ($x % 2) ? 'white' : '#d9d9d9;';
        if ((!$pacDados && $k == 'Dados do terreno' ) ||
                ( $k == 'Relatório de vistoria' && $pacQuestionario != 22 ) ||
                ( $pacFotos < 3 && $k == 'Cadastro de fotos do terreno' ) ||
                ( $k == 'Itens Planilha orçamentária' && ( $boPlanilhaOrcamentaria['faltam'] > 0 || $boPlanilhaOrcamentaria['ppoid'] < 1 ) && !in_array($pacDados, $arrExcTipoObra)) ||
                ( $k == 'Planilha orçamentária' && $boPlanilhaOrcamentaria['ppoid'] == 0 && $pacFNDE == 't') ||
                ( $k == 'Valor da planilha orçamentária' &&
                (
                $boPlanilhaOrcamentaria['valor'] < $boPlanilhaOrcamentaria['minimo'] && $boPlanilhaOrcamentaria['minimo'] != '' /*||
                $boPlanilhaOrcamentaria['valor'] > $boPlanilhaOrcamentaria['maximo'] && $boPlanilhaOrcamentaria['maximo'] != ''*/
                )
                ) ||
                ( $k == 'Cronograma físico-financeiro' && !in_array($ptoid, Array(73,74)) && !$pacCronograma && $pacFNDE == 't' ) ||
                //	 ( ($pacDocumentosTipoA['arqid'] != $pacDocumentosTipoA['podid'] || !$pacDocumentosTipoA) && $k == 'Projetos - Tipo A' && $pacFNDE == 'f' ) || 
                ( ($pacDocumentos['arqid'] < $pacDocumentos['podid'] || !$pacDocumentos) && $k == 'Documentos anexos' )
        ):
            if (!$boMsg) {
                ?>
                <tr>
                    <td colspan="3" style="text-align:center;font-size:14px;font-weight:bold;color:#900;height:50px;">
                        O sistema verificou que alguns dados não foram preenchidos:
                    </td>
                </tr>
                <?php
                $boMsg = true;
            }
            ?>
            <tr style="background-color: <?php echo $cor ?>;">
                <td>
                    <?php
                    switch ($k) {
                        case 'Dados do terreno':
                            $aba = 'Dados';
                            break;
                        case 'Relatório de vistoria':
                            $aba = 'Questionario';
                            break;
                        case 'Cadastro de fotos do terreno':
                            $aba = 'Fotos';
                            break;
                        case 'Itens Planilha orçamentária':
                            $aba = 'PlanilhaOrcamentaria';
                            break;
                        case 'Planilha orçamentária':
                            $aba = 'PlanilhaOrcamentaria';
                            break;
                        case 'Planilha orçamentária Tipo B 110v':
                            $aba = 'PlanilhaOrcamentaria';
                            break;
                        case 'Planilha orçamentária Tipo B 220v':
                            $aba = 'PlanilhaOrcamentaria';
                            break;
                        case 'Planilha orçamentária Tipo C 110v':
                            $aba = 'PlanilhaOrcamentaria';
                            break;
                        case 'Planilha orçamentária Tipo C 220v':
                            $aba = 'PlanilhaOrcamentaria';
                            break;
                        case 'Cronograma físico-financeiro':
                            $aba = 'CronogramaFisicoFinanceiro';
                            break;
                        case 'Documentos anexos':
                            $aba = 'Documento';
                            break;
                        default:
                            $aba = "Dados";
                            break;
                    }
                    ?>
                    <a href="par.php?modulo=principal/subacaoObras&acao=A&sbaid=<?php echo $_GET['sbaid'] ?>&ano=<?php echo $_GET['ano'] ?>&aba=<?php echo $aba ?>&preid=<?php echo $preid ?>">
                        <img border="0" src='/imagens/consultar.gif' onclick='javascript:void(0)'>
                    </a>
                </td>
                <td>
                    <b><?php echo $k ?></b>
                    <br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - 
                    <?php echo str_replace("{valor}", "R$ " . formata_valor($boPlanilhaOrcamentaria['valor']), $v) ?><br />
                </td>
                <td style="background:white;width:100px;"></td>
            </tr>
            <?php $x++ ?>
        <?php endif; ?>			
        <?php
    endforeach;
    if (!$boMsg):
        ?>
        <tr>
            <td colspan="3" style="text-align:center;font-size:14px;font-weight:bold;color:#900;height:50px;">
                O sistema não encontrou pendências. 
            </td>
        </tr>
    <?php endif; ?>
    <tr>
        <td colspan="3" height="350px;"></td>
    </tr>		
</table>
<div id="dialog-aprovacaoRef" title="Aprovação de Reformulação de Pre-Obra" style="display:none;">
</div>
<?php
//if( $escrita && !$boMsg ){
echo '<div style="position:relative;margin-top:-403px; margin-left:89%;">';

$arSituacao = Array(
		WF_PAR_EM_CADASTRAMENTO, 
		WF_PAR_EM_DILIGENCIA, 
		WF_PAR_OBRA_APROVADA, 
		WF_PAR_OBRA_ARQUIVADA, 
		WF_PAR_EM_ANALISE_RETORNO_DA_DILIGENCIA, 
		WF_PAR_OBRA_EM_REFORMULACAO, 
		WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL_PAR, 
		WF_TIPO_EM_DILIGENCIA_REFORMULACAO_MI_PARA_CONVENCIONAL_PAR
	);

if ($db->testa_superuser()) {
    wf_desenhaBarraNavegacao($docid, array('preid' => $preid, 'qrpid' => $qrpid, 'boMsg' => $boMsg, array('historico' => true)));
    if ($esdid == WF_PAR_OBRA_EM_VALIDACAO_DEFERIMENTO_REFORMULACAO) {
        desenhaBotaoAprovacao();
    }
} elseif (
		in_array($esdid, $arSituacao) && 
		(
			in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arperfil) || 
			in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arperfil) || 
			in_array(PAR_PERFIL_PREFEITO, $arperfil) || 
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arperfil) || 
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arperfil)
    	)
    ) {
    wf_desenhaBarraNavegacao($docid, array('preid' => $preid, 'qrpid' => $qrpid, 'boMsg' => $boMsg), array('historico' => true));
    if ($esdid == WF_PAR_OBRA_EM_VALIDACAO_DEFERIMENTO_REFORMULACAO) {
        desenhaBotaoAprovacao();
    }
} elseif (in_array(PAR_PERFIL_COORDENADOR_GERAL, $arperfil)) {
    wf_desenhaBarraNavegacao($docid, array('preid' => $preid, 'qrpid' => $qrpid, 'boMsg' => $boMsg), array('historico' => true));
    if ($esdid == WF_PAR_OBRA_EM_VALIDACAO_DEFERIMENTO_REFORMULACAO) {
        desenhaBotaoAprovacao();
    }
} else {
    echo '<div style="position:relative;margin-top:-100px;margin-left:-8%;">';
    echo '<label style="text-align:center;font-size:14px;font-weight:bold;color:#900;height:50px;"> EM ANALISE.</label>';
    echo '</div>';
}

if (in_array(PAR_PERFIL_ADMINISTRADOR, $arperfil) ||
        in_array(PAR_PERFIL_SUPER_USUARIO, $arperfil) ||
        in_array(PAR_PERFIL_COORDENADOR_GERAL, $arperfil)) {

    $sql = "SELECT popdataprazo FROM obras.preobraprorrogacao WHERE popstatus = 'P' AND preid = $preid";
    $data = $db->pegaUm($sql);
    if (verificaProrrogacao($preid)) {
        ?>
        <div style="margin-right:25px; margin-top: -20px;">
            <table align="center" border="0" cellpadding="5" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #d0d0d0; width: 80px;">
                <tr style="background-color: #c9c9c9; text-align: center;">
                    <td style="font-size: 7pt; text-align: center;">
                        <span title=""><strong>Prorrogação para o dia <? echo formata_data($data); ?></strong></span> 
                    </td>
                </tr>

                <tr style="text-align: center;">
                    <td style="font-size: 7pt; text-align: center;">
                        <a style="cursor: pointer" onclick="aceitarProrrogacao(<?php echo $preid; ?>);" title="Aprovar Prorrogação">Aprovar Prorrogação</a>
                    </td>
                </tr>
                <tr style="background-color: #c9c9c9; text-align: center;">
                    <td style="font-size: 7pt; text-align: center;">
                        <span title=""><strong></strong></span> 
                    </td>
                </tr>
                <tr style="text-align: center;">
                    <td style="font-size: 7pt; text-align: center;">
                        <a style="cursor: pointer" onclick="editarProrrogacao(<?php echo $preid; ?>);" title="Editar Prorrogação">Editar Prorrogação</a>
                    </td>
                </tr>
                <tr style="background-color: #c9c9c9; text-align: center;">
                    <td style="font-size: 7pt; text-align: center;">
                        <span title=""><strong></strong></span> 
                    </td>
                </tr>
                <tr style="text-align: center;">
                    <td style="font-size: 7pt; text-align: center;">
                        <a style="cursor: pointer" onclick="recusarProrrogacao(<?php echo $preid; ?>);" title="Recusar Prorrogação">Recusar Prorrogação</a>
                    </td>
                </tr>
            </table>
        </div>
        <?php
    }
    
    $sqlAguardandoProrrogacao = "SELECT 
                                    true 
                                FROM obras.preobra po
                                INNER JOIN workflow.documento doc ON po.docid = doc.docid
                                WHERE po.preid = {$preid} 
                                AND doc.esdid = 1260 
                                AND (SELECT popvalidacao FROM obras.preobraprorrogacao WHERE preid=po.preid ORDER BY popid DESC LIMIT 1) ='t'";
    $booGerarProrrogacao = $db->pegaLinha($sqlAguardandoProrrogacao);
    if($booGerarProrrogacao) {
        ?>
        <div style="margin-right:25px; margin-top: -20px;">
            <table align="center" border="0" cellpadding="5" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #d0d0d0; width: 80px;">
                <tr style="background-color: #c9c9c9; text-align: center;">
                    <td style="font-size: 7pt; text-align: center;">
                        <span title=""><strong>Regerar Termo de Prorrogação de Prazo</strong></span> 
                    </td>
                </tr>
                <tr style="text-align: center;">
                    <td style="font-size: 7pt; text-align: center;">
                        <?php
                        # retorna o dopid da Obra.
                        $sql = "SELECT 
								    dop.dopid
								FROM par.processoobrasparcomposicao poc 
								INNER JOIN par.documentopar dop on dop.proid = poc.proid
								WHERE 
									poc.preid = $preid 
									AND dop.dopstatus='A'";
                        
                        $dopid = $db->pegaUm($sql);
                        ?>                                    
                        <a style="cursor: pointer" onclick="openDocumento(<?php echo $dopid; ?>);" title="Visualizar Documento Atual"><img id="<?php echo $preid; ?>" class="vizualizarDocumento" style="cursor:pointer;" src="../imagens/icone_lupa.png"></a>
                        <a style="cursor: pointer" onclick="encaminharDocumentoPar(<?php echo $preid; ?>);" title="Aguardando Geração de Termo">Aguardando Geração de Termo</a>
                    </td>
                </tr>
            </table>
        </div>

        <?php
    }
}

echo '</div>';
?>