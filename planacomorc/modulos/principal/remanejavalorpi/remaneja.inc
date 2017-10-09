<?php
/**
 * Tela de remanejamento de PIs.
 * $Id: remaneja.inc 79982 2014-05-13 12:59:13Z werteralmeida $
 */

/**
 * Funções de apoio para o remanejamento de PIs.
 */
include_once '_funcoesremanejavalorpi.php';

$perfis = pegaPerfilGeral();
if (isset($_REQUEST['requisicao']) && 'remanejamentoDePI' == $_REQUEST['requisicao']) {
    $url = 'planacomorc.php?modulo=principal/remanejavalorpi/listarsubacao&acao=A';
    $msg = 'Não foi possível executar sua requisição.';

    if (remanejamentoDePI($_POST)) {
        $msg = 'Sua requisição foi executada com sucesso.';
        if ('E' == $_POST['tipotransacao']) {
            $url = 'planacomorc.php?modulo=principal/remanejavalorpi/listartransacoes&acao=A&tipoTransacao=E';
        }
    }
    ?>
    <script type="text/javascript" language="javascript">
        alert('<?php echo $msg; ?>');
        location.href = '<?php echo $url; ?>';
    </script>
    <?php
    exit();
} elseif ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";
echo "<br>";

/* Variaveis gerais */
$obrigatorias = UNIDADES_OBRIGATORIAS;
if (isset($_REQUEST['rmpid'])) {
    $sql = <<<DML
SELECT rpi.ptrid,
       rpi.sbaid,
       tipotransacao,
       ptrid,
       funcprogramatica,
       ptrdotacao,
       ptrempenhado,
       ptrsaldodetalhadopi,
       COALESCE(ptrdotacao, 0.00) - COALESCE(ptrsaldodetalhadopi, 0.00) AS ptrsaldo,
       sbaid,
       sbadotacao,
       sbaempenhado,
       sbasaldo,
       datatransacao,
       usucpf,
       rmpsaldosubtraido,
       rmpsaldoadicionado,
       rmpidorigem,
       dscalteracao,
       dscjustificativa,
       dscexecucao
  FROM planacomorc.remanejamentopi rpi
  WHERE rmpid = %d
DML;
    $stmt = sprintf($sql, $_REQUEST['rmpid']);
    $dadosTransacao = $db->carregar($stmt);
    if ($dadosTransacao) {
        $dadosTransacao = array_shift($dadosTransacao);
    }
    $_REQUEST['ptrid'] = $dadosTransacao['ptrid'];
    $_REQUEST['sbaid'] = $dadosTransacao['sbaid'];
}
$ptrid = $_REQUEST['ptrid'];
$sbaid = $_REQUEST['sbaid'];

/* Pegando dados do PTRES */
$sql = <<<DML
SELECT ptr.funcod,
       ptr.sfucod,
       ptr.prgcod,
       ptr.acacod,
       ptr.loccod,
       ptr.unicod,
       unidsc,
       ptr.plocod || ' - ' || plotitulo AS plocod,
       ptr.ptres,
       ptr.ptrdotacao,
       COALESCE(pte.total, 0.00) AS empenhado,
       COALESCE((SELECT SUM(pip.pipvalor)
                   FROM monitora.pi_planointernoptres pip
                   WHERE pip.ptrid = ptr.ptrid), 0.00) AS detalhadopi,
       (COALESCE(ptr.ptrdotacao, 0.00) - COALESCE((SELECT SUM(pip.pipvalor)
                                                     FROM monitora.pi_planointernoptres pip
                                                     WHERE pip.ptrid = ptr.ptrid), 0.00))  AS saldo
  FROM monitora.ptres ptr
    LEFT JOIN public.unidade uni USING(unicod)
    LEFT JOIN monitora.planoorcamentario plo
      ON (plo.prgcod = ptr.prgcod AND plo.acacod = ptr.acacod AND plo.plocodigo = ptr.plocod)
    LEFT JOIN siafi.ptrempenho pte ON ptr.ptres = pte.ptres AND pte.exercicio = '{$_SESSION['exercicio']}' 
  WHERE ptr.ptrid = {$ptrid} 
DML;

$dadosPtres = $db->pegaLinha($sql);

$sql = <<<DML
SELECT sba.sbacod AS codigo,
       substring(sba.sbadsc from 0 for 50)||' ...' AS descricao,
       sba.sbadsc as descricao_completa,
       CASE WHEN sba.sbasigla IS NULL
              THEN ''
            ELSE ' - ' || sba.sbasigla
         END AS sbasigla,
       (SELECT SUM(sadvalor)
          FROM monitora.pi_subacaodotacao
          WHERE sbaid = sba.sbaid) AS dotacao,
       semp.total AS empenhado
  FROM monitora.pi_subacao sba
        LEFT JOIN siafi.sbaempenho semp on semp.sbacod = sba.sbacod and semp.exercicio = sba.sbaano 
  WHERE sba.sbaid = {$sbaid}
DML;

$dadosSubacao = $db->pegaLinha($sql);

$sql = <<<DML
SELECT sadvalor AS dotacao,
       detp.detalhado AS detalhado_pi,
       COALESCE(sda.sadvalor,0.0) -  COALESCE(detp.detalhado, 0.0) AS saldo
  FROM monitora.pi_subacaodotacao sda
    INNER JOIN monitora.pi_subacao suba ON suba.sbaid = sda.sbaid
    LEFT JOIN (SELECT SUBSTR(pli.plicod, 2, 4) AS sbacod,
                      SUM(pip.pipvalor) AS detalhado
                 FROM monitora.pi_planointerno pli
                   INNER JOIN monitora.pi_planointernoptres pip
                     ON pli.pliid = pip.pliid
                 WHERE pliano = '{$_SESSION['exercicio']}'
                   AND ptrid = {$ptrid}
                GROUP BY SUBSTR(pli.plicod, 2, 4)) detp
      ON detp.sbacod = suba.sbacod
  WHERE sda.ptrid = {$ptrid}
    AND sda.sbaid = {$sbaid}
DML;
$dadosPtresSubacao = $db->pegaLinha($sql);

// -- UO do PTRES utilizada para validar se a transação de solicitação/homologação podem ser puladas.
$UOdoPTRES = $dadosPtres['unicod'];

// -- Composição da funcional programatica do PTRES
$funcProgramatica = "{$dadosPtres['funcod']}.{$dadosPtres['sfucod']}.{$dadosPtres['prgcod']}."
                  . "{$dadosPtres['acacod']}.{$dadosPtres['loccod']}.{$dadosPtres['unicod']}";

//carrega UO do usuário, para tratar PIs que não estão vinculados a UG/UO do usuário
if (in_array(PFL_GESTAO_ORCAMENTARIA, $perfis)) {
    $uoPerfil = pegaUOsPerfil(PFL_GESTAO_ORCAMENTARIA);
} elseif (in_array(PFL_GABINETE, $perfis)) {
    $uoPerfil = pegaUOsPerfil(PFL_GABINETE);
} else {
    $uoPerfil = array();
}

// -- Identificando se o usuário atual faz parte do GO ou do grupo de super usuários
$pulaSolicitacao = pulaSolicitacao();
if (in_array(PFL_GESTAO_ORCAMENTARIA, $perfis)) {
    // -- Valida a(s) UO(s) do perfil e do PTRES para verificar se ainda pode pular a solicitação
    if (!in_array($UOdoPTRES, $uoPerfil)) {
        $pulaSolicitacao = false;
    }
}

// -- Identificando se já foi feita uma requisição inicial, nesse caso, não pode mais
// -- pular as outras etapas da solicitação de remanejamento.
if (isset($_REQUEST['rmpid'])) {
    $pulaSolicitacao = false;
}

monta_titulo("Remanejamento de PI por PTRES", "");
?>
<link rel="stylesheet" type="text/css" href="../includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css"/>
<script src="./js/planacomorc.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<script>
    //Função que executa a busca textual na tabela de subações do ADICIONAR
    $(document).ready(function() {
        jQuery.expr[':'].contains = function(a, i, m) {
            return jQuery(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
        };
        estilo = '<style> .marcado{background-color:#C1FFC1 !important} .remover{display:none} </style>';
        $('#div_subacoesAdicionar').before(estilo + '<p style="margin-left:30px;font-weight:600">Pesquisar: &nbsp;'
                + '<input type="text" id="textFind" class="normal" /></p>');
        $("#textFind").keyup(function() {
            $('#div_subacoesAdicionar > table.listagem tbody tr td').removeClass('marcado');
            $('#div_subacoesAdicionar > table.listagem tbody tr').removeClass('remover');
            stringPesquisa = $("#textFind").val();
            if (stringPesquisa) {
                $('#div_subacoesAdicionar > table.listagem tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
                $('#div_subacoesAdicionar > table.listagem tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
            }
        });
        jQuery('#tabs').tabs();
        // -- Mostrando apenas remanejamentos preenchidos
        jQuery('#remanejamento_1,#remanejamento_2').click(function() { // -- Para cada checkbox de filtro
            var check = this;
            jQuery(this).closest("table").find('.listagem').each(function() { // -- Pegue a tabela mais próximo
                if (jQuery(check).attr('checked')) { // -- Se o checkbox estiver marcado
                    jQuery("input[type='text']", this).each(function() { // -- Pegue todos os inputs do tipo text dentro da tabela
                        var value = jQuery(this).val();
                        if ( value == "" || (value == "0,00") ) { // -- Some toda a tr do cara se ele estiver vazio, ou com zero
                            jQuery(this).parent().closest('tr').addClass('remover');
                        }
                    });
                } else { // -- Se não estiver marcado, mostra todas as tr
                    jQuery('tr.remover', this).removeClass('remover');
                }
            });
        });
    });

    function calculaSaldo(el) {
        var somaSBA = 0;
        var saldoRemanejado = 0;

        /*  Valor saindo dos PIs */
        $('.rem_pi_v').each(function() {
            if ($(this).val() != '') {
                somaSBA += parseFloat(replaceAll(replaceAll($(this).val(), '.', ''), ',', '.'));
            }
        });

        /*  Recalculando saldo das Subações linha a linha*/
        if (el) {
            valorDot = parseFloat(replaceAll(replaceAll(replaceAll($(el).parent().prev().html(), '<br> ', ''), '.', ''), ',', '.'));
            valorDotInput = parseFloat(replaceAll(replaceAll($(el).val(), '.', ''), ',', '.'));
            saldoLinha = valorDot - valorDotInput;
            if (isNaN(saldoLinha)) {
                saldoLinha = valorDot;
            }

            /* Calcula Orçamento da subação após o remanejamento (R$) */
            if (saldoLinha >= 0) {
                $(el).parent().next().html(mascaraglobal('###.###.###.###,##', saldoLinha.toFixed(2)));
                $(el).parent().next().html(mascaraglobal('###.###.###.###,##', saldoLinha.toFixed(2)));
            } else {
                $(el).parent().next().html('<span style="color:red">-' + mascaraglobal('###.###.###.###,##', saldoLinha.toFixed(2)) + '</span>')
                $(el).parent().next().html('<span style="color:red">-' + mascaraglobal('###.###.###.###,##', saldoLinha.toFixed(2)) + '</span>')
            }
            /* Calcula o % de variação na subação */
            variacao = (saldoLinha - valorDot) / valorDot * 100;
            if (variacao >= 0) {
                $(el).parent().next().next().html(mascaraglobal('###.###.###.###,##', variacao.toFixed(2)));
            } else {
                $(el).parent().next().next().html('<span style="color:red">-' + mascaraglobal('###.###.###.###,##', variacao.toFixed(2)) + '</span>')
            }
        }
        saldoRemanejado += somaSBA;
        $('#saldo_remanejado_total').html('R$ ' + mascaraglobal('###.###.###.###,##', saldoRemanejado.toFixed(2)));
        calculaDiferenca();
    }

    function calculaSaldoAdic(el)
    {
        var somaSBA = 0;
        var saldoRemanejado = 0;

        $('.rem_adc_pi_v').each(function() {
            if ($(this).val() != '') {
                somaSBA += parseFloat(replaceAll(replaceAll($(this).val(), '.', ''), ',', '.'));
            }
        });
        /*  Recalculando saldo SOMADO das Subações linha a linha*/
        if (el) {
            // -- Pega o valor da coluna "Orçamento Atual do PI"
            valorDot = parseFloat(replaceAll(replaceAll(replaceAll($(el).parent().prev().prev().html(), '<br> ', ''), '.', ''), ',', '.'));
            valorDotInput = parseFloat(replaceAll(replaceAll($(el).val(), '.', ''), ',', '.'));
            saldoLinha = valorDot + valorDotInput;
            if (isNaN(saldoLinha)) {
                saldoLinha = valorDot;
            }
            if (saldoLinha >= 0) {
                $(el).parent().next().html(mascaraglobal('###.###.###.###,##', saldoLinha.toFixed(2)));
            } else {
                $(el).parent().next().html('<span style="color:red">-' + mascaraglobal('###.###.###.###,##', saldoLinha.toFixed(2)) + '</span>')
            }
            /* Calcula o % de variação na subação */
            variacao = (saldoLinha - valorDot) / valorDot * 100;
            if (variacao >= 0) {
                $(el).parent().next().next().html(mascaraglobal('###.###.###.###,##', variacao.toFixed(2)));
            } else {
                $(el).parent().next().next().html('<span style="color:red">-' + mascaraglobal('###.###.###.###,##', variacao.toFixed(2)) + '</span>')
            }
        }
        saldoRemanejado += somaSBA;
        $('#saldo_remanejado_total_adicionado').html('R$ ' + mascaraglobal('###.###.###.###,##', saldoRemanejado.toFixed(2)));
        calculaDiferenca();
    }

    function calculaDiferenca()
    {
        var diferenca = 0;
        var saldoRemanejado = jQuery('#novo_saldo_nao_orcado').text();
        saldoRemanejado = saldoRemanejado.replace(/\./g, '').replace(',', '.').replace('R$ ', '');
        saldoRemanejado = parseFloat(saldoRemanejado);
        retirado = parseFloat(replaceAll(replaceAll(replaceAll($('#saldo_remanejado_total').html(), 'R$ ', ''), '.', ''), ',', '.'));
        adicionado = parseFloat(replaceAll(replaceAll(replaceAll($('#saldo_remanejado_total_adicionado').html(), 'R$ ', ''), '.', ''), ',', '.'));
        diferenca = saldoRemanejado + retirado - adicionado;
        if (diferenca >= 0) {
            $('#diferenca_saldo').html('R$ ' + mascaraglobal('###.###.###.###,##', diferenca.toFixed(2))).css('color', 'black');
        } else {
            $('#diferenca_saldo').html('R$ -' + mascaraglobal('###.###.###.###,##', diferenca.toFixed(2))).css('color', 'red');
        }
    }

    /* Mostra o Histórico do PI */
    function mostrahistoricoplanointerno(pliid) {
        window.open('../monitora/monitora.php?modulo=principal/planotrabalhoUN/popuphistoricoplanointernoUN&acao=A'
            + '&pliid=' + pliid
            + '&sl=154'
            + '&sisid=<?php echo $_SESSION['sisid']; ?>', '', 'toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=500');
    }

    function remanejar()
    {
        if ('' == jQuery('#dscalteracao').val() && 'S' == jQuery('#tipotransacao').val()) {
            alert('O campo "Detalhamento da alteração" é obrigatório e não pode ser deixado em branco.');
            jQuery('#dscalteracao').select().focus();
            return;
        }
        if ('' == jQuery('#dscjustificativa').val() && 'S' == jQuery('#tipotransacao').val()) {
            alert('O campo "Justificativa da alteração" é obrigatório e não pode ser deixado em branco.');
            jQuery('#dscjustificativa').select().focus();
            return;
        }
        if ('' == jQuery('#dscexecucao').val() && 'E' == jQuery('#tipotransacao').val()) {
            alert('O campo "Detalhamento da execução" é obrigatório e não pode ser deixado em branco.');
            jQuery('#dscexecucao').select().focus();
            return;
        }

        // -- Saldo subtraído
        var rmpsaldosubtraido = jQuery('#saldo_remanejado_total').text();
        rmpsaldosubtraido = rmpsaldosubtraido.replace(/\./g, '').replace(',', '.').replace('R$ ', '');
        jQuery('#rmpsaldosubtraido').val(rmpsaldosubtraido);
        // -- Saldo adicionado
        var rmpsaldoadicionado = jQuery('#saldo_remanejado_total_adicionado').text();
        rmpsaldoadicionado = rmpsaldoadicionado.replace(/\./g, '').replace(',', '.').replace('R$ ', '');
        jQuery('#rmpsaldoadicionado').val(rmpsaldoadicionado);

        var diferenca_saldo = jQuery('#diferenca_saldo').text();
        diferenca_saldo = parseFloat(diferenca_saldo.replace(/\./g, '').replace(',', '.').replace('R$ ', ''));
        if (diferenca_saldo < 0) {
            alert('A diferença de saldo após o remanejamento não pode ser negativa.');
            return;
        }

        // -- Verificando se ao menos um dos inputs de remanejamento foi preenchido
        var qtdSubraindo = jQuery("input[id^=rem_adc_pi]", '#remanejar').not("input[value='']", "#remanejar").length;
        var qtdAdicionando = jQuery("input[id^=rem_pi]", '#remanejar').not("input[value='']", "#remanejar").length;
        if (0 == (qtdSubraindo + qtdAdicionando)) {
            alert('Ao menos um valor de remanejamento deve ser preenchido.');
            return false;
        }

        jQuery('#dscalteracao').removeAttr('disabled');
        jQuery('#dscjustificativa').removeAttr('disabled');

        // -- Enviar o formulário
        jQuery('#remanejar').submit();
    }

    jQuery(document).ready(function() {
        jQuery('.rem_pi_v').blur(function() {
            var limite = parseFloat(
                replaceAll(replaceAll($(this).parent().prev().prev().prev().html(), '.', ''), ',', '.')
            ).toFixed(2);
            var valor = jQuery(this).val();
            valor = valor.replace(/\./g, '').replace(/,/g, '.');
            if (parseFloat(valor) > parseFloat(limite)) {
                jQuery(this).val(limite);
                jQuery(this).keyup();
            }
        });
    jQuery('#enviar').click(remanejar);
    
        /* 
         * alteração de acordo com demanda: 225740, retirando campos de preenchimento "Recurso" quando o perfil Gabinete para PIs que não estão vinculados a UG/UO do usuário. 
         */
        <? //if (in_array(PFL_GABINETE, $perfis) && (!in_array($dadosPtres['unicod'], $uoPerfil))) {?>
			// jQuery("input[id*='rem_pi_']").replaceWith( "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;--")
			// jQuery("input[id*='rem_adc_']").replaceWith( "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;--")
	<?//} ?>
            

    if (<?php echo ('E' != $dadosTransacao['tipotransacao']) ? 'true' : 'false'; ?>) {
<?php
$sql = <<<DML
SELECT *
  FROM planacomorc.remanejamentopi rpi
   LEFT JOIN planacomorc.rmpmovimentacao rmv USING(rmpid)
  WHERE rmpid = %d
DML;
$stmt = sprintf($sql, $_REQUEST['rmpid']);
$dataMovimentacao = $db->carregar($stmt);
if (is_array($dataMovimentacao)) {
    foreach ($dataMovimentacao as $mov) {
        if ('-' == $mov['tipomovimento']) {
            $id = "rem_pi_{$mov['pliid']}";
        } elseif ('+' == $mov['tipomovimento']) {
            $id = "rem_adc_pi_{$mov['pliid']}";
        }
        if ($id) {
            echo "jQuery('#{$id}').val(({$mov['vlrmovimento']}).toFixed(2)).keyup();";
        }
    }
}
// -- Bloqueio de campos se as informações da transação forem do tipo E
if ('E' == $dadosTransacao['tipotransacao']) {
    echo "jQuery('#remanejar :input').attr('disabled', 'disabled');";
}
?>
        }
    });
</script>
<style>
.linkSubacao{font-weight:bold;color:#00529b;cursor:pointer}
.cols,.col1,.col2{float:left;position:relative}
.cols{right:50%;width:100%}
.col1,.col2{overflow:hidden;padding:0 0 1em;width:46%}
.col1{left:52%}
.col2{left:56%}
#tabs{overflow:hidden}
#menubackmenu1{z-index:20}
.listagem thead:first-child td{vertical-align:middle;text-align:center}
a{cursor:pointer}
</style>
<!--ORIGEM-->
<form id="remanejar" name="remanejar" method="POST">
    <input type="hidden" name="requisicao" value="remanejamentoDePI" />
    <input type="hidden" name="rmpidorigem" value="<?php echo $_GET['rmpid']; ?>" />
    <?php
    $novoStatus = 'S';
    if ($pulaSolicitacao) {
        $novoStatus = 'E';
    } elseif ('S' == $dadosTransacao['tipotransacao']) {
        $novoStatus = 'H';
    } elseif ('H' == $dadosTransacao['tipotransacao']) {
        $novoStatus = 'E';
    }
    ?>
    <input type="hidden" name="tipotransacao" id="tipotransacao" value="<?php echo $novoStatus; ?>" />
    <input type="hidden" name="ptrid" value="<?php echo $_REQUEST['ptrid']; ?>" />
    <input type="hidden" name="funcprogramatica" value="<?php echo $funcProgramatica; ?>" />
    <input type="hidden" name="ptrdotacao" value="<?php echo $dadosPtres['ptrdotacao']; ?>" />
    <input type="hidden" name="ptrempenhado" value="<?php echo $dadosPtres['empenhado']; ?>" />
    <input type="hidden" name="ptrsaldo" value="<?php echo $dadosPtres['saldo']; ?>" />
    <input type="hidden" name="ptrsaldodetalhadopi" value="<?php echo $dadosPtres['detalhadopi']; ?>" />

    <input type="hidden" name="sbaid" value="<?php echo $_REQUEST['sbaid']; ?>" />
    <input type="hidden" name="sbadotacao" value="<?php echo $dadosSubacao['dotacao']; ?>" />
    <input type="hidden" name="sbaempenhado" value="<?php echo $dadosSubacao['empenhado']; ?>" />
    <input type="hidden" name="sbasaldo" value="<?php echo $dadosSubacao['saldo']; ?>" />

    <input type="hidden" name="rmpsaldosubtraido" id="rmpsaldosubtraido" />
    <input type="hidden" name="rmpsaldoadicionado" id="rmpsaldoadicionado" />

    <table class="tabela" align="center" cellspacing="1" cellpadding="3">
        <tr>
            <td colspan="2" class="SubTituloDireita" style="text-align:center;font-weight:bold">
                Informações
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="200px">Unidade do PTRES:</td>
            <td><?php echo $dadosPtres['unidsc']; ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="200px">Funcional Programática:</td>
            <td><?php echo $funcProgramatica; ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Data da solicitação:</td>
            <td><?php echo date('d/m/Y'); ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Detalhamento:</td>
            <td>
                <div id="tabs">
                    <ul>
                        <?php if (isset($_REQUEST['rmpid'])): ?>
                        <li><a href="#previamente">No Momento da Solicitação</a></li>
                        <?php endif; ?>
                        <li><a href="#atualmente">Atualmente</a></li>
                    </ul>
                    <?php if (isset($_REQUEST['rmpid'])): ?>
                        <div id="previamente">
                            <div class="cols">
                                <div class="col1">
                                    <table class="tabela">
                                        <tr>
                                            <td class="SubTituloDireita" width="200px">PTRES:</td>
                                            <td><b style="font-size:14px"><?php echo $dadosPtres['ptres']; ?></b></td>
                                        </tr>
                                        <tr>
                                            <td class="SubTituloDireita">PO:</td>
                                            <td><?php echo $dadosPtres['plocod']; ?></td>
                                        </tr>
                                        <tr>
                                            <td class="SubTituloDireita" width="200px">Dotação do PTRES:</td>
                                            <td><b><?php echo number_format2($dadosTransacao['ptrdotacao']); ?></b></td>
                                        </tr>
                                        <tr>
                                            <td class="SubTituloDireita" width="200px">Empenhado do PTRES:</td>
                                            <td>
                                                <span id="prev_saldo_nao_orcado" style="font-weight:bold">
                                                    <?php echo number_format2($dadosTransacao['ptrempenhado']); ?>
                                                </span>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td class="SubTituloDireita" width="200px">Detalhado em PI:</td>
                                            <td style="color:<?php echo(($dadosTransacao['ptrsaldodetalhadopi'] >= 0) ? 'black' : 'red'); ?>">
                                                <b>
                                                    <span id="saldo_pi_detalhado_ptres">
                                                        <?php echo number_format2($dadosTransacao['ptrsaldodetalhadopi']); ?>
                                                    </span>
                                                </b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td class="SubTituloDireita" width="200px">Não Detalhado em PI:</td>
                                            <td style="color:<?php echo(($dadosTransacao['ptrsaldo'] > 0) ? 'seagreen' : 'red'); ?>">
                                                <b>
                                                    <span id="prev_novo_saldo_nao_orcado">
                                                        <?php echo number_format2($dadosTransacao['ptrsaldo']); ?>
                                                    </span>
                                                </b>
                                            </td>
                                        </tr>
                                    </table>
                                </div>
                                <div class="col2">
                                    <table class="tabela">
                                        <tr>
                                            <td class="SubTituloDireita" width="200px">SUBAÇÃO:</td>
                                            <td style="font-size:14px;font-weight:bold">
                                                <?php echo "{$dadosSubacao['codigo']}{$dadosSubacao['sbasigla']}"; ?>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td class="SubTituloDireita" width="200px">Descrição:</td>
                                            <td>
                                                <a title="<?php echo $dadosSubacao['descricao_completa']; ?>">
                                                    <?php echo $dadosSubacao['descricao']; ?>
                                                </a>
                                            </td>
                                        </td>
                                        </tr>
                                        <tr>
                                            <td class="SubTituloDireita" width="200px">Dotação da Subação:</td>
                                            <td><b><?php echo number_format2($dadosTransacao['sbadotacao']); ?></b></td>
                                        </tr>
                                        <tr>
                                            <td class="SubTituloDireita" width="200px">Empenhado da subação:</td>
                                            <td>
                                                <b>
                                                    <span id="prev_saldo_nao_orcado">
                                                        <?php echo number_format2($dadosTransacao['sbaempenhado']); ?>
                                                    </span>
                                                </b>
                                            </td>
                                        </tr>
                                    </table>
                                </div>
                            </div>
                        </div>
                    <?php endif; ?>
                    <div id="atualmente">
                        <div class="cols">
                            <div class="col1">
                                <table class="tabela">
                                    <tr>
                                        <td class="SubTituloDireita" width="200px">PTRES:</td>
                                        <td><b style="font-size:14px"><?php echo $dadosPtres['ptres']; ?></b></td>
                                    </tr>
                                    <tr>
                                        <td class="SubTituloDireita">PO:</td>
                                        <td><?php echo $dadosPtres['plocod']; ?></td>
                                    </tr>
                                    <tr>
                                        <td class="SubTituloDireita" width="200px">Dotação do PTRES:</td>
                                        <td><b><?php echo number_format2($dadosPtres['ptrdotacao']); ?></b></td>
                                    </tr>
                                    <tr>
                                        <td class="SubTituloDireita" width="200px">Empenhado do PTRES:</td>
                                        <td>
                                            <b>
                                                <span id="saldo_nao_orcado">
                                                    <?php echo number_format2($dadosPtres['empenhado']); ?>
                                                </span>
                                            </b>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td class="SubTituloDireita" width="200px">Detalhado em PI:</td>
                                        <td style="color:<?php echo(($dadosPtres['detalhadopi'] >= 0) ? 'black' : 'red'); ?>">
                                            <b>
                                                <span id="saldo_pi_detalhado_ptres">
                                                    <?php echo number_format2($dadosPtres['detalhadopi']); ?>
                                                </span>
                                            </b>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td class="SubTituloDireita" width="200px">Não Detalhado em PI:</td>
                                        <td >
                                            <b>
                                                <?php echo number_format2($dadosPtres['saldo']); ?>
                                            </b>
                                        </td>
                                    </tr>
                                </table>
                            </div>
                            <div class="col2">
                                <table class="tabela">
                                    <tr>
                                        <td class="SubTituloDireita" width="200px">SUBAÇÃO:</td>
                                            <td style="font-size:14px;font-weight:bold">
                                                <?php echo "{$dadosSubacao['codigo']}{$dadosSubacao['sbasigla']}"; ?>
                                            </td>
                                    </tr>
                                    <tr>
                                        <td class="SubTituloDireita" width="200px">Descrição:</td>
                                        <td>
                                            <a title="<?php echo$dadosSubacao['descricao_completa']; ?>"><?php echo$dadosSubacao['descricao']; ?>
                                            </a>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td class="SubTituloDireita" width="200px">Dotação da Subação:</td>
                                        <td><b><?php echo number_format2($dadosSubacao['dotacao']); ?></b></td>
                                    </tr>
                                    <tr>
                                        <td class="SubTituloDireita" width="200px">Empenhado da subação:</td>
                                        <td>
                                            <span id="saldo_nao_orcado" style="font-weight:bold">
                                                <?php echo number_format2($dadosSubacao['empenhado']); ?>
                                            </span>
                                        </td>
                                    </tr>
                                </table>
                            </div>
                        </div>
                    </div>
                </div>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Detalhamento deste PTRES para esta SUBAÇÃO
            </td>
            <td>
                <table class="tabela" style="width:500px;">
                    <tr>
                        <td class="SubTituloDireita" width="200px">Dotação do PTRES na Subação:</td>
                        <td><?php echo number_format2($dadosPtresSubacao['dotacao']); ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" width="200px">Detalhado em PI do PTRES na Subação:</td>
                        <td><?php echo number_format2($dadosPtresSubacao['detalhado_pi']); ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" width="200px">Não detalhado em PI do PTRES na Subação:</td>
                        <td style="color:<?php echo(($dadosPtresSubacao['saldo'] > 0) ? 'seagreen' : 'red'); ?>"><b>
                                <span id="novo_saldo_nao_orcado">
                                    <?php echo number_format2($dadosPtresSubacao['saldo']); ?>
                                </span>
                            </b>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
    </table>
    <br />

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td style="text-align:center;color:#FFF;background-color:seagreen" colspan="2">
                <b>ORIGEM DOS RECURSOS</b>
            </td>
        </tr>
        <tr>
            <td colspan="2">
        <center>
            <table class="tabela">
                <tr>
                    <td colspan="2" style="font-size:14px;text-align:right;padding-right:3em">
                        Saldo Subtraído: 
                        <div style="float:right;color:seagreen;font-weight:bold" id="saldo_remanejado_total">
                            R$ 0,00
                        </div>
                    </td>
                </tr> 
                <tr>
                    <td colspan="2">
                        <p style="margin-left:3em;font-weight:bold;margin-bottom:2px">PIs que subtraem recurso do PTRES na Subação</p>
                        <p style="margin-left:3em;font-weight:bold;margin-top:0;vertical-align:middle">
                            <input type="checkbox" id="remanejamento_1" />
                            <label for="remanejamento_1" style="cursor:pointer">Mostrar apenas preenchidos</label>
                        </p>
                        <div>
                            <?php
                            $selectAdicional = '';
                            $cabecalho = array(
                                "PI's subtraindo recursos<br />do PTRES",
                                "Título do PI",
                                "Orçamento Total do PI (R$)",
                                "Orçamento do PI no PTRES (R$)",
                                "Empenhado do PI no PTRES (R$)",
                                "Não Empenhado do PI no PTRES (R$)"
                            );
                            if ('E' != $dadosTransacao['tipotransacao']) {
                                $cabecalho[] = "Recursos a SUBTRAIR com<br />o remanejamento (R$)";
                                $cabecalho[] = "Orçamento do PI no PTRES após o<br />remanejamento (R$)";
                                $selectAdicional = <<<PARTIAL_DML
,
       COALESCE((SELECT pip.pipvalor
                   FROM monitora.pi_planointernoptres pip
                     LEFT JOIN monitora.ptres
                       ON pip.ptrid = ptres.ptrid
                   WHERE ptres.ptrano = '{$_SESSION['exercicio']}'
                      AND pip.pliid= gmb.pliid 
                      AND ptres.ptrid = {$ptrid}), 0.00) - COALESCE((SELECT total
                                                                       FROM siafi.pliptrempenho ppe
                                                                       WHERE plicod = gmb.codigo
                                                                         AND ptres = '{$dadosPtres['ptres']}'
                                                                         AND ppe.exercicio = '{$_SESSION['exercicio']}'), 0.00) AS saldo_remanejado
PARTIAL_DML;

                            } else {
                                $cabecalho[] = "Recursos SUBTRAÍDOS com o remanejamento (R$)";
                            }
                            $ocultaRecurso = 
                            /* Cabeçalho da Consulta */
                            $params['SELECT'] = <<<SQL
SELECT 
        '<div title=\"Visualizar Dados\" class="linkSubacao" onclick=\"mostrahistoricoplanointerno(\'' || gmb.pliid || '\');\">' || gmb.codigo || '</div>' as codigo,
        gmb.titulo,
        COALESCE(SUM(pip.pipvalor),0.00) as dotacao_total,
        COALESCE((SELECT
            pip.pipvalor
        FROM
            monitora.pi_planointernoptres pip
        LEFT JOIN
            monitora.ptres
        ON
            pip.ptrid = ptres.ptrid
        WHERE
            ptres.ptrano = '{$_SESSION['exercicio']}'
        AND pip.pliid= gmb.pliid 
        AND ptres.ptrid={$ptrid}),0.00) as dotacao_pip_ptres,
       COALESCE((SELECT total
                   FROM siafi.pliptrempenho ppe
                   WHERE plicod = gmb.codigo
                     AND ptres = '{$dadosPtres['ptres']}'
                     AND ppe.exercicio = '{$_SESSION['exercicio']}'), 0.00) AS empenhado_pi_ptres,
        COALESCE((SELECT pip.pipvalor
                    FROM monitora.pi_planointernoptres pip
                      LEFT JOIN monitora.ptres ON pip.ptrid = ptres.ptrid
                    WHERE ptres.ptrano = '{$_SESSION['exercicio']}'
                      AND pip.pliid= gmb.pliid
                      AND ptres.ptrid = {$ptrid}), 0.00) - COALESCE((SELECT total
                                                                       FROM siafi.pliptrempenho ppe
                                                                       WHERE plicod = gmb.codigo
                                                                         AND ptres = '{$dadosPtres['ptres']}'
                                                                         AND ppe.exercicio = '{$_SESSION['exercicio']}'), 0.00) AS nao_empenhado_pi_ptres,
         '<input type="text" class="rem_pi_v normal" size="15" id="rem_pi_' || gmb.pliid || '" name="rem_pi[' || gmb.pliid || ']"'
          || ' onkeyup="this.value=mascaraglobal(''###.###.###.###,##'', this.value); calculaSaldo(this);" />' AS remanejar{$selectAdicional}
SQL;
                            /* Variáveis da Consulta */
                            $params['v_ptrid'] = $ptrid;
                            $params['v_sbaid'] = $sbaid;
                            
                            /* Filtros da Consulta */
                            $params['where'] = " AND SUBSTR(gmb.codigo,2,4) = '{$dadosSubacao['codigo']}'";
                            $params['filtroNoUnion'] = " AND sa.sbaid  = {$sbaid} AND ptr.ptrid = {$ptrid} ";
                            $params['filtroNoUnionSiafi'] = " AND ptr.ptrid = {$ptrid} ";
                            $sql = retornaConsultaPI($params);
//                            ver($sql, d);
                            $db->monta_lista($sql, $cabecalho, 99999, 5, 'S', '100%');
                            ?>
                        </div>
                    </td>
                </tr>
            </table>   
        </center>
        </td>        
        </tr>
    </table>
    <br/>
    <!--DESTINO-->
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td style="text-align:center;color:#FFF;background-color:#00529b"><b>DESTINO DOS RECURSOS</b></td>
        </tr>
        <tr>
            <td>
                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">     
                    <tr>
                        <td colspan="2" style="font-size:14px;text-align:right;padding-right:3em">
                            Saldo Adicionado: 
                            <div style="float:right;color:#00529b;font-weight:bold" id="saldo_remanejado_total_adicionado">
                                R$ 0,00
                            </div>
                        </td>
                    </tr> 
                    <tr>
                        <td > 
                            <p style="margin-left:3em;font-weight:bold;margin-bottom:2px">PIs que subtraem recurso da Subação</p>
                            <p style="margin-left:3em;font-weight:bold;margin-top:0;vertical-align:middle">
                                <input type="checkbox" id="remanejamento_2" />
                                <label for="remanejamento_2" style="cursor:pointer">Mostrar apenas preenchidos</label>
                            </p>
                            <div id="div_subacoesAdicionar" style="height:200px;overflow:auto">
                                <?php
                                $cabecalho = array(
                                    "PI's subtraindo recursos<br />do PTRES",
                                    "Título do PI",
                                    "Orçamento Total do PI (R$)",
                                    "Orçamento do PI no PTRES (R$)",
                                    "Empenhado do PI no PTRES (R$)"
                                );
                                if ('E' != $dadosTransacao['tipotransacao']) {
                                    $cabecalho[] = "Recursos a ACRESCENTAR com<br />o remanejamento (R$)";
                                    $cabecalho[] = "Orçamento do PI no PTRES após o<br />remanejamento (R$)";
                                    $selectAdicional = ", COALESCE((SELECT
                                                            pip.pipvalor
                                                        FROM
                                                            monitora.pi_planointernoptres pip
                                                        LEFT JOIN
                                                            monitora.ptres
                                                        ON
                                                            pip.ptrid = ptres.ptrid
                                                        WHERE
                                                            ptres.ptrano = '{$_SESSION['exercicio']}'
                                                        AND pip.pliid= gmb.pliid 
                                                        AND ptres.ptrid={$ptrid}),0.00) AS saldo_remanejado";
                                } else {
                                    $cabecalho[] = "Recursos ACRESCENTADOS com o remanejamento (R$)";
                                }
                           /* Cabeçalho da Consulta */
                            $params['SELECT'] = <<<SQL
SELECT 
       '<div title=\"Visualizar Dados\" class="linkSubacao" onclick=\"mostrahistoricoplanointerno(\'' || gmb.pliid || '\');\">' || gmb.codigo || '</div>' as codigo,
        gmb.titulo,
        COALESCE(SUM(pip.pipvalor),0.00) as dotacao_total,
        COALESCE((SELECT
            pip.pipvalor
        FROM
            monitora.pi_planointernoptres pip
        LEFT JOIN
            monitora.ptres
        ON
            pip.ptrid = ptres.ptrid
        WHERE
            ptres.ptrano = '{$_SESSION['exercicio']}'
        AND pip.pliid= gmb.pliid 
        AND ptres.ptrid={$ptrid}),0.00) as dotacao_pip_ptres,
        COALESCE((SELECT
            total
        FROM
            siafi.pliptrempenho ppe
        WHERE
            plicod = gmb.codigo
            AND ppe.exercicio = '{$_SESSION['exercicio']}'
        AND ptres = '{$dadosPtres['ptres']}'),0.00) as empenhado_pi_ptres,
         '<input type="text" class="normal rem_adc_pi_v" id="rem_adc_pi_' || gmb.pliid || '" size="15" name="rem_adc_pi[' || gmb.pliid || ']"'
          || 'onkeyup="this.value=mascaraglobal(''###.###.###.###,##'',this.value); calculaSaldoAdic(this);" />' AS orcar{$selectAdicional}
SQL;
                            /* Variáveis da Consulta */
                            $params['v_ptrid'] = $ptrid;
                            $params['v_sbaid'] = $sbaid;
                            
                            /* Filtros da Consulta */
                            $params['where'] = " AND SUBSTR(gmb.codigo,2,4) = '{$dadosSubacao['codigo']}'";
                            $params['filtroNoUnion'] = " AND sa.sbaid  = {$sbaid}";
                            $params['filtroNoUnionSiafi'] = " AND ptr.ptrid = {$ptrid} ";
                            $sql = retornaConsultaPI($params);
                            $db->monta_lista($sql, $cabecalho, 99999, 5, 'S', '100%');
                            ?>
                            </div>
                        </td>
                    </tr>
                </table>
                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                    <tr>
                        <td> 
                            Detalhamento da alteração (500 caracteres):
                            <?php
                            $dscalteracao = $dadosTransacao['dscalteracao'];
                            echo campo_textarea('dscalteracao', 'S', $pulaSolicitacao || isset($_REQUEST['rmpid']) ? 'N' : 'S', '', '85', '5', 500, '', '', '', '', '', '', array('class' => 'facultativo'));
                            ?>
                            Justificativa da alteração (500 caracteres):
                            <?php
                            $dscjustificativa = $dadosTransacao['dscjustificativa'];
                            echo campo_textarea('dscjustificativa', 'S', $pulaSolicitacao || isset($_REQUEST['rmpid']) ? 'N' : 'S', '', '85', '5', 500, '', '', '', '', '', "", array('class' => 'facultativo'));
                            if ($pulaSolicitacao || ('H' == $dadosTransacao['tipotransacao'])):
                                ?>
                                Detalhamento da execução (500 caracteres):
                                <?php
                                $dscexecucao = $dadosTransacao['dscexecucao'];
                                echo campo_textarea('dscexecucao', 'S', 'S', '', '85', '5', 500, '', '', '', '', '', "", array('class' => 'facultativo'));
                            endif;
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td style="text-align:right;font-size:14px;font-weight:bold">
                            Saldo do PTRES após remanejamento<br />
                            <div style="float:right;font-weight:bold<?php echo($dadosPtresSubacao['saldo'] > 0 ? '' : ';color:red'); ?>" id="diferenca_saldo">
                                <?php echo number_format2($dadosPtresSubacao['saldo']); ?>
                            </div>
                            <br/>
                            <br/>
                            <div style="text-align: right;" >
                                <input type="button" id="enviar" value="<?php
                                       $btnSubmeter = 'Solicitar Remanejamento';
                                       if (!isset($_REQUEST['rmpid']) && $pulaSolicitacao) {
                                           $btnSubmeter = 'Confirmar Remanejamento';
                                       } elseif (isset($_REQUEST['rmpid']) && ('S' == $dadosTransacao['tipotransacao'])) {
                                           $btnSubmeter = 'Homologar Remanejamento';
                                       } elseif (isset($_REQUEST['rmpid']) && ('H' == $dadosTransacao['tipotransacao'])) {
                                           $btnSubmeter = 'Confirmar Remanejamento';
                                       }
                                       echo $btnSubmeter; ?>" />
                            </div>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
    </table>
</form>