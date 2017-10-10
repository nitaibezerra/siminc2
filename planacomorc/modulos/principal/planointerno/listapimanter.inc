<?php
/**
 * Listagem de PI do módulo Planejamento e Acompanhamento Orçamentário.
 * @version $Id: listapimanter.inc 102359 2015-09-11 18:26:07Z maykelbraz $
 */

$fm = new Simec_Helper_FlashMessage('planacomorc/pli/listapimanter');

if($_REQUEST['monta_grafico']){
    require_once APPRAIZ . "planacomorc/classes/Pi_PlanoInterno.class.inc";
    require_once APPRAIZ . "planacomorc/classes/Pi_PlanoInternoHistorico.class.inc";
$pliid = $_REQUEST['pliid'];
    $obPi_PlanoInterno = new Pi_PlanoInterno($pliid);
    $sql = "SELECT plicod FROM monitora.pi_planointernohistorico WHERE pliid = $pliid ORDER BY pihdata DESC LIMIT 1";
    if (!$plicodOrigem = $db->pegaUm($sql)) {
        $plicodOrigem = $obPi_PlanoInterno->plicod;
    }
    $plicod = $plicodOrigem;
    $unicod = $obPi_PlanoInterno->unicod;

    $sqlGrafico = "
               SELECT
                   'Empenhado'  AS descricao,
                   'Total' as categoria,
                   vlrempenhado AS valor
               FROM
                   (
                       SELECT
                           SUM(vlrempenhado) AS vlrempenhado
                       FROM
                           spo.siopexecucao sex
                       WHERE
                           sex.exercicio = '{$_SESSION['exercicio']}'
                       AND plicod  = '{$plicod}' AND unicod = '{$unicod}'
                       GROUP BY
                           plicod ) foo
               UNION ALL
               SELECT
                   'Liquidado'  AS descricao,
                   'Total' as categoria,
                   vlrliquidado AS valor
               FROM
                   (
                       SELECT
                           SUM(vlrliquidado) AS vlrliquidado
                       FROM
                           spo.siopexecucao sex
                       WHERE
                           sex.exercicio = '{$_SESSION['exercicio']}'
                       AND plicod  = '{$plicod}' AND unicod = '{$unicod}'
                       GROUP BY
                           plicod ) foo
               UNION ALL
               SELECT
                   'Pago'  AS descricao,
                   'Total' as categoria,
                   vlrpago AS valor
               FROM
                   (
                       SELECT
                           SUM(vlrpago) AS vlrpago
                       FROM
                           spo.siopexecucao sex
                       WHERE
                           sex.exercicio = '{$_SESSION['exercicio']}'
                       AND plicod  = '{$plicod}' AND unicod = '{$unicod}'
                       GROUP BY
                           plicod ) foo";

    $grafico = new Grafico(null, false);
    $grafico->setTipo(Grafico::K_TIPO_COLUNA)->setId('graficoPi')->setTitulo('Execução Orçamentária')->gerarGrafico($sqlGrafico);
    die;
}

if ($_REQUEST['pliid']) {
    echo popup($_REQUEST['pliid']);
    die();
}

if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}
/**
 * Funções de gerenciamento de subações.
 * @see _funcoesremanejavalorsubacao.php
 */
include_once '_funcoesremanejavalorsubacao.php';

/**
 * Funçõeses de apoio à gestão de PIs.
 * @see _funcoespi.php
 */
include_once '_funcoespi.php';

function popup($pliid){
    global $db;
    require_once APPRAIZ . "planacomorc/classes/Pi_PlanoInterno.class.inc";
    require_once APPRAIZ . "planacomorc/classes/Pi_PlanoInternoHistorico.class.inc";

    if($pliid){
    $obPi_PlanoInterno = new Pi_PlanoInterno($pliid);
    $sql = "SELECT plicod FROM monitora.pi_planointernohistorico WHERE pliid = $pliid ORDER BY pihdata DESC LIMIT 1";
    if (!$plicodOrigem = $db->pegaUm($sql)) {
        $plicodOrigem = $obPi_PlanoInterno->plicod;
    }
    $plicod = $plicodOrigem;
    $unicod = $obPi_PlanoInterno->unicod;
    $sql = "SELECT pl.pliid,
				   ptr.ptres,
				   pt.ptrid,
				   pt.pipvalor,
				   ptr.acaid,
				   TRIM(ac.prgcod||'.'||ac.acacod||'.'||ac.unicod||'.'||ac.loccod||' - '||ac.acadsc) AS descricao,
				   SUM(ptr.ptrdotacao) AS dotacaoinicial,
				   ROUND(SUM(COALESCE(sad.sadvalor, 0)), 2) AS dotacaosubacao,
                   ROUND(SUM(COALESCE(ppe.total, 0)), 2) AS empenhado
			FROM monitora.pi_planointerno pl
			INNER JOIN monitora.pi_planointernoptres pt ON pt.pliid = pl.pliid
			LEFT JOIN monitora.ptres ptr ON ptr.ptrid = pt.ptrid
			LEFT JOIN monitora.acao ac ON ac.acaid = ptr.acaid
			LEFT JOIN monitora.pi_subacaodotacao sad ON ptr.ptrid = sad.ptrid and sad.sbaid = pl.sbaid
			LEFT JOIN siafi.pliptrempenho ppe ON ppe.ptres = ptr.ptres AND pl.plicod = ppe.plicod AND ppe.exercicio = '{$_SESSION['exercicio']}'
			WHERE pl.pliid = '{$obPi_PlanoInterno->pliid}'
              AND pl.plistatus = 'A'
	    	GROUP BY pl.pliid, pt.ptrid, ptr.ptres, pl.plistatus, pt.pipvalor, ac.prgcod, ptr.acaid, ac.acacod, ac.unicod, ac.loccod, ac.acadsc
	    	ORDER BY ptr.ptres";

    $acoespl = $db->carregar($sql);
    $acoespl = ($acoespl) ? $acoespl : array();

        $sqlGrafico = "
               SELECT
                   'Empenhado'  AS descricao,
                   'Total' as categoria,
                   vlrempenhado AS valor
               FROM
                   (
                       SELECT
                           SUM(vlrempenhado) AS vlrempenhado
                       FROM
                           spo.siopexecucao sex
                       WHERE
                           sex.exercicio = '{$_SESSION['exercicio']}'
                       AND plicod  = '{$plicod}' AND unicod = '{$unicod}'
                       GROUP BY
                           plicod ) foo
               UNION ALL
               SELECT
                   'Liquidado'  AS descricao,
                   'Total' as categoria,
                   vlrliquidado AS valor
               FROM
                   (
                       SELECT
                           SUM(vlrliquidado) AS vlrliquidado
                       FROM
                           spo.siopexecucao sex
                       WHERE
                           sex.exercicio = '{$_SESSION['exercicio']}'
                       AND plicod  = '{$plicod}' AND unicod = '{$unicod}'
                       GROUP BY
                           plicod ) foo
               UNION ALL
               SELECT
                   'Pago'  AS descricao,
                   'Total' as categoria,
                   vlrpago AS valor
               FROM
                   (
                       SELECT
                           SUM(vlrpago) AS vlrpago
                       FROM
                           spo.siopexecucao sex
                       WHERE
                           sex.exercicio = '{$_SESSION['exercicio']}'
                       AND plicod  = '{$plicod}' AND unicod = '{$unicod}'
                       GROUP BY
                           plicod ) foo";

    $grafico = new Grafico(null, false);
    }
    $html = <<<HTML
<style>
.modal-dialog{width:80%}
button{cursor:pointer}
table.horizontal th{text-align:right;background-color:#EFEFEF!important}
</style>
<div style="width:50%;float:left">
    <table class="tabela table table-striped table-bordered table-hover text-center horizontal">
    <tr>
        <th>Código do PI:</th>
        <td><b>{$obPi_PlanoInterno->plicod}</b></td>
    </tr>
    <tr>
        <th>Enquadramento da Despesa:</th>
        <td>
HTML;
    if ($obPi_PlanoInterno->eqdid) {
        $html .= $db->pegaUm("SELECT eqdcod || ' - ' || eqddsc FROM monitora.pi_enquadramentodespesa WHERE eqdid='{$obPi_PlanoInterno->eqdid}'");
    }
    $html.='</td>
        </tr>
        <tr>
            <th>Subação:</th>
            <td>';
    if ($obPi_PlanoInterno->sbaid) {
        $html .= $db->pegaUm("SELECT sbacod || ' - ' || sbatitulo FROM monitora.pi_subacao WHERE sbaid='{$obPi_PlanoInterno->sbaid}'");
    }
    $html .='</td>
        </tr>
        <tr>
            <th>Nível/Etapa de Ensino:</th>
            <td>';
    $html .= $db->pegaUm("SELECT neecod || ' - ' || needsc FROM monitora.pi_niveletapaensino WHERE neeid='{$obPi_PlanoInterno->neeid}'");

    $html .= '</td>
        </tr>
        <tr>
            <th>Categoria de Apropriação:</th>
            <td>';
    $html .= $db->pegaUm("SELECT capcod || ' - ' || capdsc FROM monitora.pi_categoriaapropriacao WHERE capid='{$obPi_PlanoInterno->capid}'");
    $html .='</td>
        </tr>
        <tr>
            <th>Codificação da Unidade(livre):</th>
            <td>';
    $html .= $obPi_PlanoInterno->plilivre;
    $html.= '</td>
        </tr>
        <tr>
            <th>Modalidade de Ensino/Tema/Público:</th>
            <td>';
    if ($obPi_PlanoInterno->mdeid) {
        $html .= $db->pegaUm("SELECT mdecod || ' - ' || mdedsc FROM monitora.pi_modalidadeensino WHERE mdeid='{$obPi_PlanoInterno->mdeid}'");
    }
    $html .= '
            </td>
        </tr>
        <tr>
            <th>Título:</th>
            <td>';
    $html .= $obPi_PlanoInterno->plititulo . '</td>
        </tr>
        <tr>
            <th>Descrição do PI:</th>
            <td>' . $obPi_PlanoInterno->plidsc . '</td>
        </tr>
        </tr>
        <tr>
            <th>Unidade Orçamentária:</th>
            <td>';
    $ug = $db->pegaUm("SELECT  unicod || ' - ' || unidsc FROM public.unidade WHERE unicod = '" . $obPi_PlanoInterno->unicod . "'");
    if ($ug == null || $ug == '') {
        $ugS = $db->pegaUm("SELECT  unicod || '- ' || unidsc FROM public.unidade WHERE unicod = (SELECT unicod FROM public.unidadegestora WHERE ungcod = '" . $obPi_PlanoInterno->ungcod . "')");
        if ($ugS == null || $ugS == '') {
            $ugS = " - ";
            $html.= $ugS;
        }
    } else {
        $html.= $ug;
    }
    $html .= '</td>
        </tr>
        </tr>
        <tr>
            <th>Unidade Gestora:</th>
            <td>';
    $ug = $db->pegaUm("SELECT ungcod || ' - ' || ungdsc FROM public.unidadegestora WHERE ungcod = '" . $obPi_PlanoInterno->ungcod . "'");
    if ($ug == null || $ug == '') {
        $ug = " - ";
    }
    $html .= $ug . '</td>
        </tr>
        </table></div>';

    $html.='
    <div id="graficoDestino" style="width:50%;float:left;">
    </div>
    <div style="clear:both" />';

    $sql = <<<DML
SELECT TO_CHAR(pih.pihdata, 'DD/MM/YYYY HH24:MI:SS') AS pihdata,
       '<center>' || CASE WHEN pih.pihsituacao = 'P' THEN ' Pendente '
                          WHEN pih.pihsituacao = 'A' THEN ' Aprovado '
                          WHEN pih.pihsituacao = 'R' THEN ' Revisado '
                          WHEN pih.pihsituacao = 'C' THEN ' Cadastrado no SIAFI '
                          WHEN pih.pihsituacao = 'E' THEN ' Enviado para Revisão '
                          WHEN pih.pihsituacao = 'H' THEN ' Homologado '
                          WHEN pih.pihsituacao = 'T' THEN ' <span style="color:red">Cadastrado no SIAFI</span> '
                          ELSE pih.pihsituacao
                       END || '</center>' AS situacao,
       COALESCE(pih.pihobs, '<center>-</center>') AS pihobs,
       u.usunome
  FROM monitora.pi_planointernohistorico pih
    INNER JOIN seguranca.usuario u ON pih.usucpf = u.usucpf
  WHERE pliid = {$obPi_PlanoInterno->pliid}
  ORDER BY pih.pihdata DESC
DML;

    $sql2 = "SELECT plisituacao FROM monitora.pi_planointerno WHERE pliid = $obPi_PlanoInterno->pliid ";
    $ultimaSituacao = $db->pegaUm($sql2);

    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO, Simec_Listagem::RETORNO_BUFFERIZADO);
    $listagem->setCabecalho(array(
        "Data Hora",
        "Situação",
        "Observação",
        "Cadastrado Por"
    ));

    $listagem->setQuery($sql);
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $retorno = $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    $html .= $retorno;

    if ($acoespl[0]) {
        $html .= $db->pegaUm("SELECT unicod || ' - ' || unidsc FROM public.unidade WHERE unicod = '" . $_SESSION['monitora_var']['unicod'] . "'") . '
        <table class="tabela table table-striped table-bordered table-hover text-center" width="98%" id="orcamento">
            <thead>
                <tr>
                    <th colspan="7" style="text-align:center">Detalhamento Orçamentário</th>
                </tr>
                <tr>
                    <th style="text-align:center">PTRES<input type="hidden" name="pliptres"></th>
                    <th style="text-align:center">Ação</th>
                    <th style="text-align:center">Dotação do PTRES</th>
                    <th style="text-align:center">Dotação da Subação</th>
                    <th style="text-align:center">Empenhado no PI</th>
                </tr>
            </thead>
            <tbody>';

        $valortotalpi = 0;
        foreach ($acoespl as $acpl) {
            $acpl['dotacaoinicial'] = mascaraMoeda($acpl['dotacaoinicial']);
            $acpl['dotacaosubacao'] = mascaraMoeda($acpl['dotacaosubacao']);
            $acpl['empenhado'] = mascaraMoeda($acpl['empenhado']);

            $html.= <<<HTML
                <tr id="ptres_{$acpl['ptres']}">
                        <td>{$acpl['ptres']}</td>
                        <td>{$acpl['descricao']}</td>
                        <td>{$acpl['dotacaoinicial']}</td>
                        <td>{$acpl['dotacaosubacao']}</td>
                        <td>{$acpl['empenhado']}</td>
                    </tr>
HTML;
            $valortotalpi = $valortotalpi + $acpl['pipvalor'];
        }
    $html .= '
            </td>
        </tr>
    </tbody>
</table>';
    }
    /* Gato do Grafico */
    $html.='<script>jQuery("#graficoPi").appendTo("#graficoDestino");</script>';
    return $html;
}

/* Apenas Obrigat?rias */
$_REQUEST['apenas_obrigatorias'] = 1;
include APPRAIZ . 'includes/cabecalho.inc';
?>
<style>
.red{color:red}
.linkSubacao{font-weight:bold;color:#00529b;cursor:pointer}
</style>
<script type="text/javascript">
$(document).ready(function(){
    jQuery.expr[':'].contains = function(a, i, m) {
        return jQuery(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
    };
    estilo = '<style> .marcado{background-color:#C1FFC1;} .remover{ display:none;} </style>';
    $('.listagem').before(estilo + '  ');
    $("#textFind").keyup(function()
    {
        $('table.listagem tbody tr td').removeClass('marcado');
        $('table.listagem tbody tr').removeClass('remover');
        stringPesquisa = $("#textFind").val();
        if (stringPesquisa) {
            $('table.listagem tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
            $('table.listagem tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
        }
    });
});
function reload()
{
    window.location = 'planacomorc.php?modulo=principal/planointerno/listapimanter&acao=A';
}
function detalhePI(pliid) {
    $.post("planacomorc.php?modulo=principal/planointerno/listapimanter&acao=A&pliid=" + pliid, function(html) {
        $('#modal-confirm .modal-body p').html(html);
        $('#modal-confirm .modal-title').html('Dados do Plano Interno');
        $('#modal-confirm .btn-primary').remove();
        $('#modal-confirm .btn-default').html('Fechar');
        $('#graficoDestino').load('planacomorc.php?modulo=principal/planointerno/listapimanter&acao=A&monta_grafico=1&pliid='+pliid);
        $('.modal-dialog').show();
        $('#modal-confirm').modal();
    });
}
/* Editar a Suba??o */
function editaRegistro(pliid) {
    location.href = 'planacomorc.php?modulo=principal/planointerno/cadastro&acao=A&pliid=' + pliid;
}
/* Excluir a Suba??o */
function excluirRegistro(pliid) {
    if (confirm('Deseja apagar o PI?')) {
        location.href = 'planacomorc.php?modulo=principal/planointerno/cadastro&acao=A&apagar=true&pliid=' + pliid;
    }
}
/* Mostra o Hist?rico do PI */
function mostrahistoricoplanointerno(pliid) {
    window.open('../monitora/monitora.php?modulo=principal/planotrabalhoUN/popuphistoricoplanointernoUN&acao=A'
            + '&pliid=' + pliid
            + '&sisid=' + <?php echo $_SESSION['sisid']; ?>, '', 'toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=500');
}
function enviar() {
    //var form = document.getElementById("formulario");
    var uri = "planacomorc.php?modulo=principal/planointerno/listapimanter&acao=A"
            + '&pi=' + $('#pi').val()
            + '&ptres=' + $('#ptres').val()
            + '&unidade=' + $('#unidade').val()
            + '&titulodescricao=' + $('#titulodescricao').val();

    // -- Verificar porque ao clicar duas vezes em uma op??o, o comando retorna undefined.
    var plisituacao = $('input[name=plisituacao]:checked').val();
    //if ('todos' != wrfstatus) {
    uri += '&plisituacao=' + plisituacao;
    //}

    window.location = uri;
    return false;
    //form.submit();
}
function enviarApenasEmpenhados() {
    var form = document.getElementById("formulario");
    $('#apenasEmpenhados').val(true);
    form.submit();
}
</script>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Plano Interno</li>
    </ol>
<div class="col-lg-12">
   <div class="well">
        <fieldset>
            <form id="formulario" name="formulario" method="post" action="" class="form-horizontal">
                <input type='hidden' name='apenasEmpenhados' id='apenasEmpenhados' />
                <div class="form-group">
                    <label for="pi" class="col-lg-2 control-label">Plano Interno</label>
                    <div class="col-lg-10">
                        <input type="text" class="CampoEstilo normal form-control" name="pi" id="pi" value="<?php echo $_REQUEST['pi']; ?>" />
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputNatureza" class="col-lg-2 control-label">PTRES:</label>
<?php
$obrigatorias = UNIDADES_OBRIGATORIAS;
if (count($_REQUEST['ptres']) != 0 || $_REQUEST['ptres'] != 'null' || $_REQUEST['ptres'] != '' || $_REQUEST['ptres'] != null) {
    $andPtres = "AND pt.ptrid in (" . (is_array($_REQUEST['ptres'])? join($_REQUEST['ptres'], ','): $_REQUEST['ptres']) . ")";
}
if ($_REQUEST['ptres'][0]) {
    $sqlPTRES = "SELECT
						pt.ptrid as codigo,
						'(PTRES:'||pt.ptres||') - '|| aca.unicod ||'.'|| aca.prgcod ||'.'|| aca.acacod ||' - '|| aca.acadsc as descricao
                                            FROM monitora.acao aca
                                                    inner join monitora.ptres pt on pt.acaid=aca.acaid
                                            WHERE aca.prgano='" . $_SESSION['exercicio'] . "' and aca.acasnrap = false
                                                AND pt.ptrano='{$_SESSION['exercicio']}'
                                                AND aca.unicod IN($obrigatorias) "
            . $andPtres . "
                                            GROUP BY codigo , descricao
                                            ORDER BY
                                            1";
    $ptres = $db->carregar($sqlPTRES);
}
$sqlComboAcoes = "SELECT
						pt.ptrid as codigo,
						'(PTRES:'||pt.ptres||') - '|| aca.unicod ||'.'|| aca.prgcod ||'.'|| aca.acacod as descricao
                                            FROM monitora.acao aca
                                                    inner join monitora.ptres pt on pt.acaid=aca.acaid
                                            WHERE
                                                aca.prgano='{$_SESSION['exercicio']}'
                                                AND pt.ptrano='{$_SESSION['exercicio']}'
                                                AND aca.acasnrap = false
                                                AND aca.unicod IN($obrigatorias)
                                            GROUP BY codigo , descricao
                                            ORDER BY 1";

$combo = $db->carregar($sqlComboAcoes);
?>
                    <div class="col-lg-10">
                        <select name="ptres[]" id="ptres" multiple class="form-control chosen-select-no-single" required="required" data-placeholder="Selecione">
                            <option value=""></option>
                    <?php
                    if($combo){
                        if ($ptresSe) {
                            foreach ($combo as $PTRES):
                                foreach ($ptres as $ptresF):
                                    ?>
                                            <option <?php if ($PTRES['codigo'] == $ptresF['codigo']) echo 'selected="selected"' ?>  value="<?php echo $PTRES['codigo'] ?>"><?php echo $PTRES['descricao'] ?></option>
                                            <?php
                                        endforeach;
                                    endforeach;
                        }else {
                                    ?>

                                    <?php foreach ($combo as $PTRES): ?>
                                        <option  value="<?php echo $PTRES['codigo'] ?>"><?php echo $PTRES['descricao'] ?></option>
                                        <?php
                                    endforeach;
                        }
                    }
                            ?>
                        </select>
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Unidade Orçamentária (UO):</label>
                    <div class="col-lg-10">
<?php
$unidade = $_REQUEST['unidade'];
$sql = "SELECT
                        DISTINCT uni.unicod                  AS codigo,
                        uni.unicod || ' - '||uni.unidsc AS descricao
                    FROM
                        public.unidade uni
                    WHERE unicod IN ({$obrigatorias})
                    ORDER BY
                        uni.unicod";
$db->monta_combo('unidade', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'unidade', null, isset($unidade) ? $unidade : '', null, 'class="form-control chosen-select" style="width=100%;""', null, null);
?>
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Situação:</label>
                    <div class="col-lg-10">
                        <div class="btn-group" data-toggle="buttons">
                            <label class="btn btn-default <?php if ($_REQUEST['plisituacao'] == '' || $_REQUEST['apenasEmpenhados'] == 'true' || $_REQUEST['plisituacao'] == 'undefined') {
                            echo 'active';
                        } ?>">
                                <input type="radio" name="plisituacao" id="plisituacao_todos" value="todos" />Todos
                            </label>
                            <label class="btn btn-default <?php if ($_REQUEST['plisituacao'] == 'A' && $_REQUEST['apenasEmpenhados'] != 'true') {
                            echo 'active';
                        } ?>">
                                <input type="radio" name="plisituacao"
                                       id="plisituacao_APR"
                                       value="A" />Aprovado
                            </label>
                            <label class="btn btn-default <?php if ($_REQUEST['plisituacao'] == 'E' && $_REQUEST['apenasEmpenhados'] != 'true') {
                            echo 'active';
                        } ?>">
                                <input type="radio" name="plisituacao"
                                       id="plisituacao_EPR"
                                       value="E" />Enviado para Revisão
                            </label>
                            <label class="btn btn-default <?php if ($_REQUEST['plisituacao'] == 'P' && $_REQUEST['apenasEmpenhados'] != 'true') {
                            echo 'active';
                        } ?>">
                                <input type="radio" name="plisituacao"
                                       id="plisituacao_PEN"
                                       value="P" />Pendente
                            </label>
                            <label class="btn btn-default <?php if ($_REQUEST['plisituacao'] == 'C' && $_REQUEST['apenasEmpenhados'] != 'true') {
                            echo 'active';
                        } ?>">
                                <input type="radio" name="plisituacao"
                                       id="plisituacao_CSI"
                                       value="C" />Cadastro no SIAF
                            </label>
                            <label class="btn btn-default <?php if ($_REQUEST['plisituacao'] == 'H' && $_REQUEST['apenasEmpenhados'] != 'true') {
                            echo 'active';
                        } ?>">
                                <input type="radio" name="plisituacao"
                                       id="plisituacao_HOM"
                                       value="H" />Homologado
                            </label>
                            <label class="btn btn-default <?php if ($_REQUEST['plisituacao'] == 'T' && $_REQUEST['apenasEmpenhados'] != 'true') {
                            echo 'active';
                        } ?>">
                                <input type="radio" name="plisituacao"
                                       id="plisituacao_CSI"
                                       value="T" ><span class="red">Cadastro no SIAF</span>
                            </label>
                        </div>
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Titulo ou Descrição:</label>
                    <div class="col-lg-10">
                        <input type="text" class="CampoEstilo normal form-control" name="titulodescricao" id="titulodescricao" value="<?php echo $_REQUEST['titulodescricao']; ?>" />
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-2">
                        <button class="btn btn-warning" type="reset" onclick="reload()">Limpar</button>
                        <button class="btn btn-primary" id="buscar" type="button" onclick="enviar()">Buscar</button>
                        <button class="btn btn-info" type="button" onclick="location.href = 'planacomorc.php?modulo=principal/planointerno/cadastro&acao=A'" value="Cadastrar">Cadastrar</button>
                        <button class="btn btn-success" type="button" onclick="enviarApenasEmpenhados()" value="Listar PIs com Empenho e não cadastrados no SIMEC (26.000)">Listar PIs com Empenho e não cadastrados no <?php echo SIGLA_SISTEMA; ?> (26.000)</button>
                    </div>
                </div>
            </form>
        </fieldset>
    </div>
</div>

<?php
$sql_lista = listaPisManterAt($_REQUEST);
$listagem = new Simec_Listagem();
if ($_REQUEST['apenasEmpenhados'] == 'true') {
    $listagem->setCabecalho(array(
        "Código",
        "Unidade",
        "Empenho Total do Pi(R$)"));
} else {

    $listagem->setCabecalho(array(
        "Código",
        "Título",
        "Unidade",
        "Situação",
        "Empenho Total do PI (R$)"));
}
$listagem->setQuery($sql_lista);
$listagem->addCallbackDeCampo(array('empenhado', 'empenhado_total'), 'mascaraMoeda');
$listagem->addCallbackDeCampo(array('titulo', 'unidsc' ), 'alinhaParaEsquerda');
if ($_REQUEST['apenasEmpenhados'] != 'true') {
    $listagem->setAcoes(array(
        'edit' => 'editaRegistro',
        'delete' => 'excluirRegistro',
        'view' => 'detalhePI'
    ));
}
//$listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
$listagem->turnOnPesquisator();
$listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('empenhado', 'empenhado_total'));
$listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
//ver($sql_lista);
?>
</div>