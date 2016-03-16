<?php
/**
 * <$apenasListar> é definida em <_funcoessubacoes.php> na função <carregarSubacao2> que,
 * em seguida, inclui esta página <monitorarsubacao.inc>.
 * Essa forma de carregar a tela de monitorar ação é utilizada para que ela possa ser
 * exibida sem que o usuário altere suas informações.
 * $Id: monitorarsubacao.inc 92952 2015-01-21 16:52:29Z lindalbertofilho $
 */

if (!isset($_REQUEST['id_periodo_referencia'])) {
    $_REQUEST['id_periodo_referencia'] = recuperaUltimoPeriodoReferencia();
}
$apenasListar = false;
if ($_REQUEST['id_periodo_referencia'] <> 5) {/* trocar para o período atual */
    $apenasListar = true;
}
//if (isset($_REQUEST['id_acao_programatica'])) {
//    $apenasListar = true;
//}

/**
 * Utilizado somente para exercicio em 2013
 * @param type $id_ptres
 * @return type
 */
function capturaProdutos($id_ptres){
    $sql = "
        SELECT id_produto_spo_subacao,
            nome,
            descricao,
            valor_fisico
            --psp.id_produto_spo
        FROM planacomorc.produto_spo psp
        INNER JOIN planacomorc.produto_spo_subacao pss USING(id_produto_spo)
        LEFT JOIN planacomorc.acompanhamento_subacao asb ON(asb.id_subacao = pss.id_subacao
            AND asb.id_ptres = {$id_ptres}
            AND asb.id_periodo_referencia = {$_REQUEST['id_periodo_referencia']})
        LEFT JOIN planacomorc.acompanhamento_subacao_produto_spo pspo
            USING(id_produto_spo, id_acompanhamento_subacao, id_periodo_referencia)
        WHERE pss.id_subacao = {$_SESSION['planacomorc']['id_subacao']}
            AND psp.st_ativo = 'A'
    ";

    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO,Simec_Listagem::RETORNO_BUFFERIZADO);
    $listagem->setQuery($sql);
    $listagem->addCallbackDeCampo(array('nome','descricao'), 'alinhaParaEsquerda');
    $listagem->setCabecalho(array('Produtos da Subacao','Descrição dos produtos','Físico executado no período'));
    $listagem->esconderColunas('id_produto_spo_subacao');
    return $listagem->render();
}

/**
 * Utilizado somente para exercicio em 2013
 * @param type $id_ptres
 * @return type
 */
function capturaFisico($id_ptres){
    $sql = "
        SELECT
            fisico_convertido_acao
        FROM planacomorc.acompanhamento_subacao
        WHERE id_subacao = '{$_SESSION['planacomorc']['id_subacao']}'
            AND id_ptres = '{$id_ptres}'
            AND id_periodo_referencia = '{$_REQUEST['id_periodo_referencia']}'
    ";
    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO,Simec_Listagem::RETORNO_BUFFERIZADO);
    $listagem->setQuery($sql);
    return $listagem->render();
}

/* Bloqueia o preenchimento fora do período para os usuário UO */
if (in_array(PFL_COORDENADORSUBACAO, $perfis)) {
    $sql = "
        SELECT
            CASE WHEN CURRENT_DATE NOT BETWEEN inicio_preenchimento AND fim_preenchimento
            THEN TRUE
            ELSE FALSE
            END AS validade_periodo
        FROM planacomorc.periodo_referencia
        WHERE id_periodo_referencia = {$_REQUEST['id_periodo_referencia']}
    ";

    if ($db->pegaUm($sql) == 't') {
        $apenasListar = true;
    }
}
/* Capturando Unidade responsável pela Subação. */
$sql = "
    SELECT
        DISTINCT
        COALESCE(pgt.pigdsc,'<span class=\"label label-info\">Não cadastrado</span>') AS urs
    FROM planacomorc.snapshot_dotacao_subacao sds
    LEFT JOIN planacomorc.subacao sac USING(id_subacao)
    LEFT JOIN monitora.pi_gestor pgt ON id_gestor_subacao = pgt.pigid
    WHERE sac.id_subacao = {$_SESSION['planacomorc']['id_subacao']}
";
$ur = $db->pegaUm($sql);

$sql = "
    SELECT
        u.usunome
    FROM planacomorc.subacao sac
    INNER JOIN planacomorc.usuarioresponsabilidade ur using(id_subacao)
    INNER JOIN seguranca.usuario u on ur.usucpf = u.usucpf
    WHERE sac.id_subacao = {$_SESSION['planacomorc']['id_subacao']}
        AND rpustatus='A'
";
$dadosCoordenador = $db->pegaLinha($sql);
?>
<script>
$(document).ready(function(){
    if (validarAnaliseEContribuicao(false)
        && validarPerguntasQuestionario(false)
        && ($('#formulario textarea[name^="resposta_"]').length)
        && ($('#formulario .verificacao').length)) {
        $('#apresentar').css('display', 'inherit');
        $('#acoes').removeClass('col-md-12').addClass('col-md-11');
    } else {
        <?
        if($_SESSION['exercicio'] != '2013'){
        ?>
        $('#mensagem').addClass('alert alert-warning text-center').html('<span class="glyphicon glyphicon-warning-sign"></span> Preencha todo o acompanhamento para enviar.');
        <?
        }
        ?>
        $('#acoes').removeClass('col-md-11').addClass('col-md-12');

    }

    $('#id_periodo_referencia_chosen').css('width','100%');
    $('.number').blur();
});

function selecionarPeriodo(id_periodo_referencia) {
    window.location = 'planacomorc.php?modulo=principal/subacoes/gerenciarsubacoes&acao=A&aba=monitorarsubacao&id_periodo_referencia=' + id_periodo_referencia;
}
<?php
if (!$apenasListar) {
    ?>
    function gravarAcompanhamentoAcao() {
        if ($('#id_periodo_referencia').val() == '') {
            alert('Período de Referência em branco');
            return false;
        }
        if (!validarPerguntasQuestionario(true)) {
            return false;
        }
        if (!validarAnaliseEContribuicao(true)) {
            return false;
        }
        document.getElementById('formulario').submit();
    }
    <?
}
?>

    function validarAnaliseEContribuicao(alerta) {
        result = true;
        $.each($('.verificacao'), function() {
            if ($(this).val() == "") {
                if (alerta) {
                    alert("Preencha todo o acompanhamento para enviar");
                }
                result = false;
                return false;
            }
        });
        return result;
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

<?
    if ($acompanhamentoacao['acocod']) { ?>
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

<section class="col-md-12">
    <section class="row">
        <section class="well">
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
        </section>
        <section class="panel panel-info">
            <section class="panel-heading"><strong> <?= 'Subação: ' . $subacao['codigo'] . ' - ' . $subacao['titulo'] ?></strong></section>
            <section class="panel-body">
                <strong>Unidade Responsável pela Subação: </strong> <?php echo $ur; ?>
                <div class="page-header">
                    Coordenador da Subação: <?php echo $dadosCoordenador['usunome'] ? $dadosCoordenador['usunome'] : '<strong class="label label-warning">Não encontrado</strong>' ?>
                </div>
            </section>
            <section class="panel-footer">
                <button type="button" id="botao" onclick="alteraBotaoCollapse();" class="btn btn-warning"><span class="glyphicon glyphicon-resize-small"></span> Retrair Ações</button>
            </section>
        </section>
        <section id="mensagem" class="col-md-8 col-md-offset-2">

        </section>
    </section>
</section>

<form method="post" class="" name="formulario" id="formulario" enctype="multipart/form-data">
    <section class="col-md-11" id="acoes">
        <?php if (!$apenasListar) { ?>
            <input type="hidden" name="requisicao" value="gravarAcompanhamentoSubAcao">
        <?php } ?>
        <input type="hidden" name="id_periodo_referencia" value="<?= $_REQUEST['id_periodo_referencia'] ?>">
        <input type="hidden" name="id_subacao" value="<?= $_SESSION['planacomorc']['id_subacao']; ?>">
        <?
        /* Se o exercício for diferente de 2013 carrega com os novos dados normalmente, senão, captura os dados
         * de acordo com as querys antigas. */
        if($_SESSION['exercicio'] != '2013'){
        ?>
        <div class="panel-group" id="accordion">
            <?php
            $sqlAcoes = "
                SELECT
                    DISTINCT
                    aca.codigo as codigo,
                        (SELECT
                            acacod ||' - '|| acatitulo as acatitulo
                        FROM monitora.acao
                        WHERE acaid = (
                            SELECT
                                acaid
                            FROM monitora.ptres
                            WHERE ptres = pt.ptres
                                AND ptrano = '{$_SESSION['exercicio']}' LIMIT 1
                            )
                        )AS acatitulo,
                    to_char(sds.datareferencia,'dd/mm/yyyy') as datareferencia
                FROM planacomorc.snapshot_dotacao_subacao sds
                LEFT JOIN planacomorc.ptres pt on pt.id_ptres = sds.id_ptres
                LEFT JOIN planacomorc.acao_programatica apr on apr.id_acao_programatica = pt.id_acao_programatica --AND apr.id_exercicio = '{$_SESSION['exercicio']}'
                LEFT JOIN planacomorc.acao aca on aca.id_acao = apr.id_acao
                WHERE sds.id_subacao = '{$_SESSION['planacomorc']['id_subacao']}'
                    AND sds.id_periodo_referencia = {$_REQUEST['id_periodo_referencia']}
                    AND aca.codigo IS NOT NULL
            ";
                          #ver($sqlAcoes)    ;
            $acoes = $db->carregar($sqlAcoes);
            if (is_array($acoes)) {
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
                                sds.plocod AS pocodigo,
                                CASE WHEN sds.plocod IS NULL THEN '<span class=\"label label-info\">Não selecionado</span>' ELSE sds.plocod ||' - '|| sds.plodsc END AS plotitulo,
                                CASE WHEN sds.produto IS NULL THEN '<span class=\"label label-info\">Nada descrito</span>' ELSE sds.produto END as proddsc,
                                COALESCE(sds.meta::Text ,'<span class=\"label label-info\">Nada descrito</span>') as metafisica,
                                pt.id_ptres as idptres,
                                pt.id_ptres as fisico,
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
                            LEFT JOIN planacomorc.acompanhamento_subacao pas ON pt.id_ptres = pas.id_ptres
                            WHERE sds.id_subacao = '{$_SESSION['planacomorc']['id_subacao']}'
                                AND sds.id_periodo_referencia = {$_REQUEST['id_periodo_referencia']}
                                AND aca.codigo = '{$acao['codigo']}'
                                AND sds.empenhado > 0
                            ORDER BY ptres
                        ";
                        #ver($sql);
                        //alinhar PO a esquerda
                        $cabecalho = array('PTRES', 'Plano Orçamentário', 'Produto', 'Meta do PTRES', 'Análise da Execução da Subação no PTRES', 'Contribuição da Subação na meta Física do PTRES', 'Orçamento do PTRES nesta Subação', 'Empenhado do PTRES nesta Subação', 'Liquidado do PTRES nesta Subação', 'Pago do PTRES nesta Subação');
                        $tabela = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                        $tabela->setCabecalho($cabecalho);
                        $tabela->addCallbackDeCampo(array('idptres'), 'textoComentarioPtres');
                        $tabela->addCallbackDeCampo(array('plotitulo'), 'alinhaParaEsquerda');
                        $tabela->addCallbackDeCampo(array('metafisica'), 'mascaraNumero');
                        $tabela->addCallbackDeCampo('fisico', 'fisicoExecutado');
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
        <?
        }
        /*
         *Caso exercicio = 2013 apresentar dados com sql antigo.
        */
        else{
            $sql = "
                SELECT DISTINCT
                    pt.ptres,
                    aca.codigo AS acacodigo,
                    COALESCE(pt.plocod ||' - '|| po.titulo,'<span class=\"label label-info\">Não selecionado</span>') AS potitulo,
                    sds.dotacao_atual,
                    sds.empenhado,
                    sds.liquidado,
                    sds.pago
                FROM planacomorc.snapshot_dotacao_subacao sds
                JOIN planacomorc.ptres pt ON pt.id_ptres = sds.id_ptres
                JOIN planacomorc.acao_programatica apr ON apr.id_acao_programatica=pt.id_acao_programatica
                JOIN planacomorc.acao aca ON aca.id_acao = apr.id_acao
                JOIN planacomorc.dados_acao_exercicio dae ON dae.id_acao = aca.id_acao AND dae.id_exercicio = apr.id_exercicio
                LEFT JOIN planacomorc.plano_orcamentario PO on po.id_plano_orcamentario = pt.id_plano_orcamentario
                WHERE sds.id_subacao = {$_SESSION['planacomorc']['id_subacao']}
                    AND dae.id_exercicio = {$_SESSION['exercicio']}
                    AND sds.id_periodo_referencia = {$_REQUEST['id_periodo_referencia']}
                ORDER BY ptres
            ";
            #ver($sql);
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
            $listagem->setQuery($sql);
            $listagem->addCallbackDeCampo(array('dotacao_atual','empenhado','liquidado','pago'), 'mascaraMoeda');
            $listagem->setCabecalho(array('PTRES','Ação Orçamentária','Plano Orçamentário','Orçamento do PTRES nesta subação','Empenhado','Liquidado','Pago'));
            $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA,array('dotacao_atual','empenhado','liquidado','pago'));
            $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

            $sql = "
                SELECT DISTINCT
                    pt.id_ptres,
                    pt.ptres,
                    aca.codigo AS acacodigo,
                    COALESCE(pt.plocod ||' - '|| po.titulo,'<span class=\"label label-info\">Não selecionado</span>') AS potitulo,
                    prod.descricao AS proddsc,
                    pt.id_ptres AS produto,
                    COALESCE(texto_metodologia,'<p class=\"text-center\"><span class=\"label label-info\">Nada descrito</span></p>') AS texto_metodologia,
                    pt.id_ptres AS fisico
                FROM planacomorc.snapshot_dotacao_subacao sds
                join planacomorc.ptres pt on pt.id_ptres = sds.id_ptres
                join planacomorc.acao_programatica apr on apr.id_acao_programatica=pt.id_acao_programatica and apr.id_exercicio = {$_SESSION['exercicio']}
                join planacomorc.acao aca on aca.id_acao = apr.id_acao
                left join planacomorc.metodologia_conversao mc on mc.id_acao = aca.id_acao and mc.id_subacao  = sds.id_subacao
                join planacomorc.dados_acao_exercicio dae on dae.id_acao = aca.id_acao and dae.id_exercicio = apr.id_exercicio
                left join planacomorc.plano_orcamentario po on po.id_plano_orcamentario = pt.id_plano_orcamentario
                left join planacomorc.produto prod on prod.id_produto = dae.id_produto
                where sds.id_subacao = '{$_SESSION['planacomorc']['id_subacao']}'
                    and dae.id_exercicio = {$_SESSION['exercicio']}
                    and sds.id_periodo_referencia = {$_REQUEST['id_periodo_referencia']}
                order by ptres
            ";
            #ver($sql);
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
            $listagem->setQuery($sql);
            $listagem->esconderColunas('id_ptres');
            $listagem->addCallbackDeCampo('produto', 'capturaProdutos');
            $listagem->addCallbackDeCampo('proddsc', 'alinhaParaEsquerda');
            $listagem->addCallbackDeCampo('texto_metodologia', 'alinhaParaEsquerda');
            $listagem->addCallbackDeCampo('fisico', 'capturaFisico');
            $listagem->setCabecalho(array('PTRES','Ação Orçamentária','Plano Orçamentário','Produto da Ação','Produto da SUBAÇÃO / Físico executado no período com o recurso do PTRES',
                'Metodologia de conversão do produto da SUBAÇÃO no produto da AÇÃO','Físico executado convertido para o produto da AÇÃO'));
            $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

        }
        ?>
    </section>
    <section class="col-md-1" id="apresentar" style="display:none;">
        <?php
        /* Cria um DOCID caso exita um acompanhamento para a subacao */
        $sql = "
            SELECT
                id_subacao,
                id_periodo_referencia,
                docid
            FROM planacomorc.acompanhamento_subacao_tramitacao
            WHERE id_subacao = {$_SESSION['planacomorc']['id_subacao']}
                AND id_periodo_referencia = {$_REQUEST['id_periodo_referencia']}
        ";
        $docid = 0;
        $acompSubacaoTramit = $db->pegaLinha($sql);
        if($acompSubacaoTramit){
            $docid = $acompSubacaoTramit['docid'];
        }else{
            $docid = criaDocumentoSubacao($_SESSION['planacomorc']['id_subacao']);
            $sql = "INSERT INTO planacomorc.acompanhamento_subacao_tramitacao (id_subacao, id_periodo_referencia, docid) VALUES({$_SESSION['planacomorc']['id_subacao']},{$_REQUEST['id_periodo_referencia']},{$docid})";
            $db->carregar($sql);
        }
        if ($docid) {
            ?>
            <section class="col-md-1" style="margin:0;">
            <?php
            wf_desenhaBarraNavegacao($docid, array('id_subacao' => $_SESSION['planacomorc']['id_subacao']));
            ?>
            </section>
            <?php
        }
        ?>
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
		WHERE ma.id_subacao='" . $_SESSION['planacomorc']['id_subacao'] . "'
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
                                    $sql = "SELECT mqrresposta FROM planacomorc.monsubacaquestionarioresposta WHERE id_subacao = '{$_SESSION['planacomorc']['id_subacao']}' AND mqpid = '{$per['mqpid']}'";
                                    $mqrresposta = $db->pegaUm($sql);
                                    $obrigatorio = (1 == $per['mqpfacultativo']) ? 'N' : 'S';
                                    inputTextArea('resposta_' . $per['mqpid'], $mqrresposta, 'resposta_' . $per['mqpid'], $per['mqprespnumcaracteres'], array('obrig' => $obrigatorio));
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
            <?php
            if (!$apenasListar) {
                ?>
                <section class="form-group">
                    <section class="col-md-12">
                        <input class="btn btn-primary" type="button" name="salvar" id="salvar" value="Salvar" onclick="gravarAcompanhamentoAcao();">
                    </section>
                </section>
                <?php
            }else{
                gravacaoDesabilitada('Periodo de preenchimento expirado.');
            }
            ?>
        </section>
    </section>
</form>
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

if ($bloqueiaForm && $apenasListar) {
    ?>
    <script>
        jQuery(document).ready(function() {
            jQuery('input, textarea, button').attr('disabled', true);
            jQuery('#salvar').remove();
        });
    </script>
    <?php
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