<?php

include_once '_funcoessubacoes.php';
include APPRAIZ ."includes/workflow.php";
require APPRAIZ . 'includes/library/simec/Listagem.php';

$sql = "SELECT * FROM planacomorc.subacao WHERE id_subacao='" . $_REQUEST['id_subacao'] . "'";
$subacao = $db->pegaLinha($sql);
if (!isset($_REQUEST['id_periodo_referencia'])) {
    $sql = "
        SELECT
            id_periodo_referencia AS codigo
        FROM planacomorc.periodo_referencia p
        WHERE id_exercicio = '".$_SESSION['exercicio']."'
        ORDER BY id_periodo_referencia desc
        LIMIT 1
    ";
    $_REQUEST['id_periodo_referencia'] = $db->pegaUm($sql);
}

$sql = "
    SELECT DISTINCT pgt.pigdsc AS urs
    FROM planacomorc.snapshot_dotacao_subacao sds
    INNER JOIN planacomorc.subacao sac USING(id_subacao)
    INNER JOIN monitora.pi_gestor pgt ON id_gestor_subacao = pgt.pigid
    WHERE sac.id_subacao = {$_REQUEST['id_subacao']}
";
$arURS = array();
$dados = $db->carregar($sql);
if($dados){
    foreach ($dados as $urs) {
        $arURS[] = $urs['urs'];
    }
}
$sql = "
    SELECT
        u.usunome
    FROM planacomorc.subacao sac
    INNER JOIN planacomorc.usuarioresponsabilidade ur using(id_subacao)
    INNER JOIN seguranca.usuario u on ur.usucpf = u.usucpf
    WHERE sac.id_subacao = {$_REQUEST['id_subacao']}
        AND rpustatus='A'
";
$dadosCoordenador = $db->pegaLinha($sql);
?>

<script>
    $(document).ready(function(){
        $('#id_periodo_referencia_chosen').css('width','100%');
    });

    function selecionarPeriodo(id_periodo_referencia) {
        url = "planacomorc.php?modulo=principal/acoes/dadosubacao&acao=A&id_subacao="+<?=$_REQUEST['id_subacao']?>+"&id_periodo_referencia="+id_periodo_referencia;
        $.post(url, function(html) {
            $('#modal-confirm .modal-body p').html(html);
        });
    }

    function calcularIndicadores() {
        // Valor da Dotação Atual
        var valordotacaoatual = parseFloat('<?= (($dadosfinanceirossiafi['dfsdotacaoatual']) ? $dadosfinanceirossiafi['dfsdotacaoatual'] : '0.00') ?>');
        // Meta Física na LOA
        var metafisicaloa = parseInt('<?= (($quantitativosof['qtsquantidadefisico']) ? $quantitativosof['qtsquantidadefisico'] : '0') ?>');
        // Valor Liquidado
        var valorliquidado = parseFloat('<?= (($dadosfinanceirossiafi['dfsliquidado']) ? $dadosfinanceirossiafi['dfsliquidado'] : '0.00') ?>');
        // Meta Física Realizada
        if (jQuery('#acoexecutadofisico').val() == '') {
            var metafisicarealizada = 0;
        } else {
            var metafisicarealizada = parseInt(jQuery('#acoexecutadofisico').val());
        }
        // Valor da Reprogramação Fin.
        if (jQuery('#acoreprogramadofinanceiro').val() == '') {
            var valorreprogramacaofinanceiro = 0;
        } else {
            var valorreprogramacaofinanceiro = parseFloat(replaceAll(replaceAll(jQuery('#acoreprogramadofinanceiro').val(), '.', ''), ',', '.'));
        }
        // Meta Física Reprogr.
        if (jQuery('#acoreprogramadofisico').val() == '') {
            var metafisicareprogramacao = 0;
        } else {
            var metafisicareprogramacao = parseInt(jQuery('#acoreprogramadofisico').val());
        }

        var EFLOA = 0;

        if (metafisicaloa > 0 && valorliquidado > 0 && metafisicarealizada > 0) {
            EFLOA = ((valordotacaoatual / metafisicaloa) / (valorliquidado / metafisicarealizada));
            jQuery('#EFLOA').html(EFLOA.toFixed(1));
            jQuery('#EFLOA_color').css('background-color', definirCor(EFLOA.toFixed(1)));

        } else {
            jQuery('#EFLOA').html('-');
            jQuery('#EFLOA_color').css('background-color', '#FFFFFF');
        }

        var EFREP = 0;

        if (metafisicareprogramacao > 0 && valorliquidado > 0 && metafisicarealizada > 0) {
            EFREP = ((valorreprogramacaofinanceiro / metafisicareprogramacao) / (valorliquidado / metafisicarealizada));
            jQuery('#EFREP').html(EFREP.toFixed(1));
            jQuery('#EFREP_color').css('background-color', definirCor(EFREP.toFixed(1)));
        } else {
            jQuery('#EFREP').html('-');
            jQuery('#EFREP_color').css('background-color', '#FFFFFF');
        }

        var ECLOA = 0;
        if (metafisicaloa > 0) {
            ECLOA = (metafisicarealizada / metafisicaloa);
            jQuery('#ECLOA').html(ECLOA.toFixed(1));
            jQuery('#ECLOA_color').css('background-color', definirCor(ECLOA.toFixed(1)));
        } else {
            jQuery('#ECLOA').html('-');
            jQuery('#ECLOA_color').css('background-color', '#FFFFFF');
        }

        var ECREP = 0;
        if (metafisicareprogramacao > 0) {
            ECREP = (metafisicarealizada / metafisicareprogramacao);
            jQuery('#ECREP').html(ECREP.toFixed(1));
            jQuery('#ECREP_color').css('background-color', definirCor(ECREP.toFixed(1)));
        } else {
            jQuery('#ECREP').html('-');
            jQuery('#ECREP_color').css('background-color', '#FFFFFF');
        }
    }

    function definirCor(num) {
        if (num < 0.8 || num > 1.2)
            return '#FF0000';
        if ((num >= 0.8 && num < 0.9) || (num > 1.1 && num < 1.2))
            return '#FFFF00';
        if (num >= 0.9 && num <= 1.1)
            return '#005A04';
    }

<?  if ($acompanhamentoacao['acocod']) { ?>
        jQuery(document).ready(function() {
            calcularIndicadores();
        });
<?  } ?>

    function calcExecucaoTotal(obj) {
        var produto_spo_classe = jQuery(obj).attr('class').replace('normal', '').replace(' ', '');
        var total_produto_spo = 0;
        jQuery('.' + produto_spo_classe).each(function() {
            if (jQuery(this).val()) {
                total_produto_spo += parseInt(jQuery(this).val());
            }
        });
        jQuery('.' + produto_spo_classe + '_total').attr('value', total_produto_spo);
    }

    function alteraBotaoCollapse() {
        if ($('#botao').hasClass('btn-success')) {
            $('.collapsandeando').collapse('show');
            $('#botao').removeClass('btn-success');
            $('#botao').addClass('btn-warning');
            $('#botao').html('<span class="glyphicon glyphicon-resize-small"></span> Retrair Ações');

        } else {
            $('.collapsandeando').collapse('hide');
            $('#botao').removeClass('btn-warning');
            $('#botao').addClass('btn-success');
            $('#botao').html('<span class="glyphicon glyphicon-resize-full"></span> Expandir Ações');
        }
    }
</script>
<script type="text/javascript" src="../includes/funcoes.js"></script>

<section class="col-md-12">
    <section class="row">
        <section class="well">
            <form method="post" id="formPeriodo">
            Selecione o período de referência:
            <?
            $sql = "
	 	SELECT
                    id_periodo_referencia as codigo,
                    titulo || ' : ' || to_char(inicio_validade,'DD/MM/YYYY') ||' a ' || to_char(fim_validade,'DD/MM/YYYY') as descricao
	    	FROM planacomorc.periodo_referencia p
	    	WHERE id_exercicio='" . $_SESSION['exercicio'] . "'
	    	ORDER BY id_periodo_referencia DESC
            ";
            $pers = $db->carregar($sql);
            if ($pers[0]) {
                if (count($pers) == 1) {
                    $periodoreferencia = current($pers);
                    if (!$_REQUEST['id_periodo_referencia'])
                        $_REQUEST['id_periodo_referencia'] = $periodoreferencia['codigo'];
                }
                inputCombo('id_periodo_referencia', $pers, $_REQUEST['id_periodo_referencia'], 'id_periodo_referencia', array('acao' => 'selecionarPeriodo'));
            } else {
                echo "<section class=\"alert alert-danger text-center\">Não existem períodos de referência</section>";
            }
            ?>
            </form>
        </section>
        <section class="panel panel-info">
            <section class="panel-heading"><strong> <?= 'Subação: ' . $subacao['codigo'] . ' - ' . $subacao['titulo'] ?></strong></section>
            <section class="panel-body">
                <strong>Unidade Responsável pela Subação: </strong> <?php echo implode(' - ', $arURS); ?>
                <div class="page-header">
                    Coordenador da Subação: <?php echo $dadosCoordenador['usunome'] ? $dadosCoordenador['usunome'] : '<strong class="label label-warning">Não encontrado</strong>' ?>
                </div>
            </section>
            <section class="panel-footer">
                <button type="button" id="botao" onclick="alteraBotaoCollapse();" class="btn btn-warning"><span class="glyphicon glyphicon-resize-small"></span> Retrair Ações</button>
            </section>
        </section>
    </section>
</section>

<section class="col-md-12" id="acoes">
    <div class="panel-group" id="accordion">
        <?php
        $sqlAcoes = <<<DML
            SELECT DISTINCT 
                aca.codigo AS codigo,
                (SELECT 
                    aca_m.acacod ||' - '|| aca_m.acatitulo AS acatitulo
                FROM monitora.acao aca_m
                WHERE aca.codigo = aca_m.acacod
                    AND aca_m.prgano = '{$_SESSION['exercicio']}'
                LIMIT 1) AS acatitulo,
                TO_CHAR(sds.datareferencia, 'dd/mm/yyyy') AS datareferencia
            FROM planacomorc.snapshot_dotacao_subacao sds
            LEFT JOIN planacomorc.ptres pt on pt.id_ptres = sds.id_ptres
            LEFT JOIN planacomorc.acao_programatica apr on apr.id_acao_programatica = pt.id_acao_programatica --AND apr.id_exercicio = '{$_SESSION['exercicio']}'
            LEFT JOIN planacomorc.acao aca on aca.id_acao = apr.id_acao
            WHERE sds.id_subacao = '{$_REQUEST['id_subacao']}'
                AND sds.id_periodo_referencia = {$_REQUEST['id_periodo_referencia']}
                AND aca.codigo IS NOT NULL
DML;
        $acoes = $db->carregar($sqlAcoes);
        #ver($sqlAcoes);
        if (is_array($acoes) && $acoes) {
            foreach ($acoes as $acao) {
                ?>
                <div class="panel panel-primary">
                    <div class="panel-heading" style="overflow: auto;">
                        <h4 class="panel-title" >
                            <a data-toggle="collapse" style="" class="text-uppercase" data-parent="#accordion" href="<?= "#" . $acao['codigo'] ?>">
                                AÇÃO: <?= strtoupper($acao['acatitulo']); ?>
                            </a>
                            <strong style="float:right;clear:both;font-weight:normal;">Data de Referência: <?= $acao['datareferencia'];?></strong>
                        </h4>
                    </div>
                    <div id="<?= $acao['codigo'] ?>" class="panel-collapse collapsandeando in" style="overflow-x: auto;">

                    <?php
                    $sql = "
                        SELECT
                            DISTINCT pt.ptres,
                            (select plocod FROM monitora.ptres where ptres = pt.ptres AND ptrano = '{$_SESSION['exercicio']}' LIMIT 1) AS pocodigo,
                            CASE WHEN po.titulo IS NULL THEN '<span class=\"label label-info\">Não selecionado</span>' ELSE pt.plocod ||' - '|| po.titulo END AS plotitulo,
                            CASE WHEN sds.produto IS NULL THEN '<span class=\"label label-info\">Nada descrito</span>' ELSE sds.produto END as proddsc,
                            COALESCE(sds.meta::text,'<span class=\"label label-info\">Nada descrito</span>') as metafisica,
                            COALESCE(pas.comentario_ptres::text,'<span class=\"label label-info\">Nada descrito</span>') as comentario,
                            COALESCE(pas.fisico_convertido_acao::text,'<span class=\"label label-info\">Nada descrito</span>') as fisico,
                            sds.dotacao_atual,
                            sds.empenhado,
                            sds.liquidado,
                            sds.pago
                        FROM planacomorc.snapshot_dotacao_subacao sds
                        JOIN planacomorc.ptres pt on pt.id_ptres = sds.id_ptres
                        JOIN planacomorc.acao_programatica apr on apr.id_acao_programatica=pt.id_acao_programatica
                        JOIN planacomorc.acao aca on aca.id_acao = apr.id_acao
                        LEFT JOIN planacomorc.metodologia_conversao mc on mc.id_acao = aca.id_acao and mc.id_subacao  = sds.id_subacao
                        JOIN planacomorc.dados_acao_exercicio dae on dae.id_acao = aca.id_acao and dae.id_exercicio = apr.id_exercicio
                        LEFT JOIN planacomorc.plano_orcamentario po on po.id_plano_orcamentario = pt.id_plano_orcamentario
                        LEFT JOIN planacomorc.acompanhamento_subacao pas ON (pt.id_ptres = pas.id_ptres and pas.id_subacao = sds.id_subacao)
                        WHERE sds.id_subacao = '{$_REQUEST['id_subacao']}'
                            --AND dae.id_exercicio = {$_SESSION['exercicio']}
                            AND sds.id_periodo_referencia = {$_REQUEST['id_periodo_referencia']}
                            AND aca.codigo = '{$acao['codigo']}'
                        ORDER BY ptres
                    ";
                    //alinhar PO a esquerda
                    $cabecalho = array('PTRES', 'Plano Orçamentário', 'Produto', 'Meta', 'Análise da Execução da Subação no PTRES', 'Contribuição da Subação na meta Física', 'Orçamento do PTRES nesta Subação', 'Empenhado do PTRES nesta Subação', 'Liquidado do PTRES nesta Subação', 'Pago do PTRES nesta Subação');
                    $tabela = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                    $tabela->setCabecalho($cabecalho);
                    $tabela->addCallbackDeCampo(array('plotitulo'), 'alinhaParaEsquerda');
                    $tabela->addCallbackDeCampo(array('dotacao_atual', 'empenhado', 'liquidado', 'pago'), 'mascaraMoeda');
                    $tabela->esconderColuna(array('pocodigo'));
                    $tabela->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('dotacao_atual', 'empenhado', 'liquidado', 'pago'));
                    $tabela->setFormOff();
                    $tabela->setQuery($sql);
                    $tabela->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

                    ?>
                    </div>
                </div>
                <?php
            }
        }
        ?>
    </div>
</section>

<section class="col-md-12" style="clear:both;">
    <br/>
    <div class="page-header">
        <h4 title="O questionário tem por objetivo orientar o preenchimento do acompanhamento orçamentário, para que este gere informações que expliquem de forma clara, objetiva e completa, os resultados de execução física apurados no período.">Questionário de acompanhamento das Subações</h4>
    </div>
    <section class="form-horizontal">
        <?php
        $sql = "
            SELECT
                *
            FROM planacomorc.monsubacaquestionario mq
            INNER JOIN planacomorc.monqstsubacaperiodo ma ON ma.qstid = mq.qstid
            WHERE ma.id_subacao='" . $_REQUEST['id_subacao']. "'
                AND mq.id_periodo_referencia='" . $_REQUEST['id_periodo_referencia'] . "'
                AND qststatus='A'
        ";
        $monquestionario = $db->carregar($sql);
        if ($monquestionario) {
            foreach ($monquestionario as $questionario) {
                ?>
                <section class="form-group">
                    <section class="col-md-12">
                        <b>Questionário</b>: <?= $questionario['qstnome']; ?>
                    </section>
                </section>
                <?php
                $sql = "SELECT * FROM planacomorc.monqstsubacaperguntas mp WHERE qstid='" . $questionario['qstid'] . "'";
                $monqstperguntas = $db->carregar($sql);
                if ($monqstperguntas[0]) {
                    foreach ($monqstperguntas as $per) {
                        $val_perg[] = $per['mqpid'];
                        ?>
                        <section class="form-group">
                            <section class="col-md-12">
                                <label class=""><?= $per['mqpdescricao'] ?></label>
                                <?php
                                $sql = "SELECT mqrresposta FROM planacomorc.monsubacaquestionarioresposta WHERE id_subacao = '{$_REQUEST['id_subacao']}' AND mqpid = '{$per['mqpid']}'";
                                $mqrresposta = $db->pegaUm($sql);
                                $obrigatorio = (1 == $per['mqpfacultativo']) ? 'N' : 'S';
                                echo "<br/>";
                                echo (!$mqrresposta) ? '<span class="label label-info">Nenhuma resposta inserida</span>' : $mqrresposta;
                                //inputTextArea('resposta_' . $per['mqpid'], $mqrresposta, 'resposta_' . $per['mqpid'], $per['mqprespnumcaracteres'], array('habil' => 'N'));
                                ?>
                            </section>
                        </section>
                        <?php
                    }
                }
            }
        } else {
            ?>
            <section class="alert alert-danger text-center col-md-6 col-md-offset-3">Não existem questionários cadastrados</section>
            <br style="clear:both" />
            <?php
        }
        if ($val_perg) {
            foreach ($val_perg as $p) {
                echo '<input type="hidden" name="idperguntas[]" value="' . $p . '">';
            }
        }
        ?>
        <!-- FIM Questionário-->

    </section>
</section>
<?php
/**
 * Edição do acompanhamento de subações. Só o acompanhamento do período atual
 * pode ser editado. Além disso, as seguintes regras devem ser atendidas:
 * 1) O usuário é SUPER USER;
 * 2) O perfil do usuário é o COORDENADOR de SUBAÇÃO e o estado do WF é EM PREENCHIMENTO;
 * 3) O perfil do usuário é o CPMO e o estado do WF é EM ANÁLISE;
 */
$bloqueiaForm = true;
$esdid = pegaEstadoAtual($docid);
$perfis = pegaPerfilGeral();


if (1 == $_SESSION['superuser']) {
    $bloqueiaForm = false;
}

if ($bloqueiaForm && !empty($docid) && (WF_SUBACAO_EM_PREENCHIMENTO == $esdid) && in_array(PFL_COORDENADORSUBACAO, $perfis)) {
    $bloqueiaForm = false;
}

if ($bloqueiaForm && !empty($docid) && WF_SUBACAO_EM_ANALISE == $esdid && in_array(PFL_CPMO, $perfis)) {
    $bloqueiaForm = false;
}


?>
<script>
    function validarPerguntasQuestionario(alerta) {
    <?
    if ($val_perg) {
        foreach ($val_perg as $perg) {
            ?>
            $('#resposta_<?= $perg ?>').keyup();
            if (($('#resposta_<?= $perg ?>').val() == '') && ($('#resposta_<?= $perg ?>').hasClass('obrigatorio'))) {
                if (alerta) {
                    alert('Todas as perguntas indicadas como obrigatórias devem ser respondidas');
                }
                return false;
            }
            <?
        }
    }
    ?>
        return true;
    }
</script>

<?
exit();
?>