<?php
require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$empid = $_SESSION['obras2']['empid'];
$obrid = $_REQUEST['obrid'];
$slcid = trim($_REQUEST['slcid']);

$pflcods = array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC);

$habilita = 'S';
if (!possui_perfil($pflcods)) {
	$habilita = 'N';
}

if($slcid) {
	$urlP = "/obras2/obras2.php?modulo=principal/solicitacaoAnalise&acao=A&obrid=".$obrid."&slcid=".$slcid;
} else {
	$urlP = "/obras2/obras2.php?modulo=principal/solicitacaoAnalise&acao=A&obrid=".$obrid;
}
$urlP .= $_GET['tslid'] ? ('&tslid[]='.$_GET['tslid'][0]) : '';
print carregaAbaSolicitacao($urlP, $obrid, $slcid);

global $db;
$nome_titulo = 'Análise Solicitaçoes';
$sub_titulo = 'Análise Solicitaçoes - CGIMP/DIGAPE/FNDE';
$perid = 3933;
$queid = QUEID_QUEST_CHKLST_SOLICITACOES;
$sql = <<<DML
    SELECT
        sol.qrpid,
        usu.usunome,
        doc.esdid
    FROM obras2.solicitacao sol
    INNER JOIN workflow.documento                doc ON sol.docid  = doc.docid
    LEFT  JOIN workflow.tipodocumento             tpd ON tpd.tpdid  = doc.tpdid
    LEFT  JOIN workflow.estadodocumento           esd ON esd.esdid  = doc.esdid
    LEFT  JOIN workflow.historicodocumento        hst ON hst.hstid  = doc.hstid
    INNER JOIN questionario.questionarioresposta qrp ON sol.qrpid   = qrp.qrpid
    INNER JOIN questionario.questionario         que ON qrp.queid  = que.queid
    INNER JOIN seguranca.usuario                 usu ON sol.usucpf = usu.usucpf
    WHERE sol.slcid = {$slcid};
DML;
$dados = $db->pegaLinha($sql);
$qrpid = $dados['qrpid'];
$responsavel = $dados['usunome'];
$situacao = $dados['esdid'];

if(!$qrpid) {
    $arParam = array("queid" => $queid, "titulo" => "Checklist da Validação (" . $slcid . " - Checklist da Validação)");
    $qrpid = GerenciaQuestionario::insereQuestionario($arParam);
    $sql = <<<DML
        UPDATE obras2.solicitacao
            SET qrpid = {$qrpid}
            WHERE slcid = {$slcid}
DML;
    $db->executar($sql);
    $db->commit();
}

if($situacao != ESDID_SOLICITACOES_AGUARDANDO_ANALISE && $situacao != ESDID_SOLICITACOES_RETORNADO){
    $habilita = 'N';
}

monta_titulo("Checklist " . $nome_titulo, $sub_titulo);
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript">
    function printQuestionario() {
        var url = "/obras2/obras2.php?modulo=principal/questionarioImpressaoSolicitacoes&acao=A&qrpid=<?=$qrpid?>&obrid=<?=$obrid?>&slcid=<?=$slcid?>&queid=<?=$queid?>";
        popup2 = window.open(
            url,
            "_blank",
            "width=1000,height=650,scrollbars=yes,scrolling=no,resizebled=no"
        );
    }

    function closeWindow() {
        // Antes de fechar a janela, faz uma chamada ajax para zerar as variáveis de sessão do checklist
        var Data = $.ajax({
            type : "GET",
            url : "/obras2/obras2.php?modulo=principal/cadValidacao&acao=A&requisicao=resetchecklist",
            cache : false,
            global : false,
            async : false,
            success : function(data) {
                return data;
            }
        }).responseText;
        opener.location.reload();
        window.close();
    }

    // Ao clicar no botão de fechar a janela, chama a função "closeWindow"
    window.onbeforeunload = closeWindow;

</script>

<table bgcolor="#f5f5f5" align="center" class="tabela" >
    <tr>
        <td colspan="3" class="subtitulocentro">Identificação da Análise</td>
    </tr>
    <tr>
        <td class="subtitulodireita" style="width: 15%">ID da Análise:</td>
        <td style="width: 75%"><?php echo $qrpid; ?></td>
    </tr>
    <tr>
        <td class="subtitulodireita">Responsável:</td>
        <td colspan="2"><?php echo $responsavel; ?></td>
    </tr>
    <tr>
        <td colspan="2" style="width: 900px; max-width: 920px; overflow: auto;">
            <table bgcolor="#f5f5f5" align="center" class="tabela" style="width: 900px; max-width: 920px;">
                <tr>
                    <td>
                        <fieldset style="width: 94%; background: #fff;" >
                            <legend>Questionário</legend>
                            <?php
                                $tela = new Tela(array(
                                    'qrpid' => $qrpid,
                                    'tamDivArvore' => 25,
                                    'tamDivPx' => 250,
                                    'habilitado' => $habilita)
                                );
                            ?>
                        </fieldset>
                    </td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td colspan="2" align="center">
            <button name="btn_fechar" type="button" onclick="window.close()">Fechar</button>
            <button type="button" id="print_quest" onclick="printQuestionario()"> Imprimir </button>
        </td>
    </tr>
</table>