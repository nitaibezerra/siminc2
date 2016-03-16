<?php
/**
 * Detalhamento da ação para monitoração.
 *
 * @package SiMEC
 * @subpackage planejamento-acompanhamento-orcamentario
 * @version $Id: monitoraracao.inc 95122 2015-03-11 18:39:55Z fellipesantos $
 */

if (!isset($_REQUEST['percod'])) {
    $_REQUEST['percod'] = PERIODO_ATUAL;
}

if (isset($_REQUEST['id_acao_programatica'])) {
    $sql = "
    	SELECT
            prg.codigo || '.' || a.codigo || '.' || org.codigo || '.' || '{$_REQUEST['loccod']}' AS ptres,
            dae.titulo AS descricao,
            u.id_unidade_medida,
            prg.titulo AS programa,
            u.descricao AS unidademedida,
            prg.codigo AS cod_programa,
            a.codigo AS cod_acao,
            org.codigo AS cod_orgao,
            lpr.id_localizador,
            lpr.id_localizador_programatica,
            prd.descricao AS produto
        FROM planacomorc.acao a
        INNER JOIN planacomorc.acao_programatica apr ON apr.id_acao = a.id_acao
        INNER JOIN planacomorc.dados_acao_exercicio dae ON dae.id_acao = a.id_acao
            AND dae.id_exercicio = apr.id_exercicio
        INNER JOIN planacomorc.orgao org USING(id_orgao)
        INNER JOIN planacomorc.localizador_programatica lpr USING(id_acao_programatica)
        LEFT JOIN planacomorc.programa prg USING(id_programa)
        LEFT JOIN planacomorc.produto prd USING(id_produto)
        LEFT JOIN planacomorc.unidade_medida u ON u.id_unidade_medida = dae.id_unidade_medida
        WHERE id_acao_programatica='" . $_REQUEST['id_acao_programatica'] . "'
            AND lpr.loccod = '{$_REQUEST['loccod']}'
    ";
    $acao = $db->pegaLinha($sql);
} else {
    /* Caso perca a sessão ou a programática, enviar para o início */
    echo "<script>window.location='planacomorc.php?modulo=principal/acoes/listaunidades&acao=A';</script>";
    die;
}
?>
<script src="/planacomorc/js/planacomorc.js"></script>
<script type="text/javascript" language="javascript" src="/planacomorc/js/moment-with-locales.js"></script>
<script type="text/javascript" language="javascript" src="/planacomorc/js/bootstrap-datetimepicker.min.js"></script>
<link href="/planacomorc/css/bootstrap-datetimepicker.min.css" rel="stylesheet" type="text/css">
<script>
function selecionarPeriodo(percod)
{
    window.location = 'planacomorc.php?modulo=principal/acoes/gerenciarunidades&acao=A&aba=monitoraracao'
        + '&id_acao_programatica=<?= $_REQUEST['id_acao_programatica'] ?>'
        + '&loccod=<?= $_REQUEST['loccod'] ?>&percod=' + percod;
}
</script>
<section class="well">
    Selecione Período de Referência
    <?php
    $sql = "
        SELECT
            id_periodo_referencia,
            row_number() OVER (ORDER BY inicio_validade DESC) AS rownum,
            id_periodo_referencia AS codigo,
            titulo || ' : ' || to_char(inicio_validade,'DD/MM/YYYY') ||' a ' || to_char(fim_validade,'DD/MM/YYYY') AS descricao
        FROM planacomorc.periodo_referencia p
        WHERE id_exercicio = '{$_SESSION['exercicio']}'
        ORDER BY id_periodo_referencia DESC
    ";

    $pers = $db->carregar($sql);
    if ($pers[0]) {
        if (!$_REQUEST['percod']) {
            $periodoreferencia = current($pers);
            $_REQUEST['percod'] = $periodoreferencia['id_periodo_referencia'];
        } else {
            foreach ($pers as $periodo) {
                if ($_REQUEST['percod'] == $periodo['id_periodo_referencia']) {
                    $periodoreferencia = $periodo;
                    break;
                }
            }
        }
        $hintCombo = 'Os dados financeiros da ação estão de acordo com o SIAFI e são referentes ao período selecionado.';
        inputCombo('percod', $pers, $_REQUEST['percod'], 'percod',array('txtdica'=>$hintCombo,'acao'=>'selecionarPeriodo'));
    } else {
        echo "<section class=\"alert alert-danger text-center\">Não existem períodos de referência</section>";
    }
    ?>
</section>
<div class="panel panel-info">
    <?php
    $sql = "
        SELECT DISTINCT us.usunome ||' ('||replace(to_char(us.usucpf::numeric, '000:000:000-00'), ':', '.')||')' AS nome,
            '('||COALESCE(us.usufoneddd,'00')||') '||COALESCE(us.usufonenum,'-') AS fone,
            usuemail AS email,
            ur.rpudata_inc
        FROM planacomorc.usuarioresponsabilidade ur
        INNER JOIN seguranca.usuario us ON us.usucpf = ur.usucpf
        INNER JOIN seguranca.perfil pe ON pe.pflcod = ur.pflcod
        WHERE ur.pflcod = '" . PFL_COORDENADORACAO . "'
            AND id_acao_programatica = '" . $_REQUEST['id_acao_programatica'] . "'
            AND id_periodo_referencia = {$_REQUEST['percod']}
            AND rpustatus = 'A'
        ORDER BY ur.rpudata_inc DESC
    ";
            #ver($sql);
    $dadosCoordenador = $db->pegaLinha($sql);

    $sql = "
        SELECT DISTINCT us.usunome ||' ('||replace(to_char(us.usucpf::numeric, '000:000:000-00'), ':', '.')||')' AS nome,
            ' ('||COALESCE(us.usufoneddd,'00')||') '||COALESCE(us.usufonenum,'-') AS fone,
            usuemail AS email,
            ur.rpudata_inc
        FROM planacomorc.usuarioresponsabilidade ur
        INNER JOIN seguranca.usuario us ON us.usucpf = ur.usucpf
        INNER JOIN seguranca.perfil pe ON pe.pflcod = ur.pflcod
        WHERE ur.pflcod='" . PFL_VALIDADORACAO . "'
            AND id_periodo_referencia = {$_REQUEST['percod']}
            AND id_acao_programatica='" . $_REQUEST['id_acao_programatica'] . "'
          AND rpustatus='A'
        ORDER BY ur.rpudata_inc DESC";
            #ver($sql);
    $dadosValidador = $db->pegaLinha($sql);
    ?>
    <div class="panel-heading"><strong>Ação e Responsáveis</strong></div>
        <table class="table table-bordered">
            <tbody>
                <tr>
                    <th style="text-align:right">Ação: </th>
                    <td colspan="4"><?php echo $acao['ptres'] . " - " . $acao['descricao']; ?></td>
                </tr>
                <tr>
                    <th class="text-right">Responsáveis:</th>
                    <th>Nome</th>
                    <th>Telefone</th>
                    <th>E-mail</th>
                    <th></th>
                </tr>
                <tr>
                    <th style="text-align:right">Coordenador:</th>
                    <td><?php echo $dadosCoordenador['nome']; ?></td>
                    <td><?php echo $dadosCoordenador['fone']; ?></td>
                    <td> <a href="mailto:<?=$dadosCoordenador['email'];?>"><?php echo $dadosCoordenador['email']; ?></a></td>
                    <? $perfil = pegaPerfilGeral();
                    $permissao = in_array(PFL_CPMO, $perfil) || in_array(PFL_SUPERUSUARIO, $perfil)? true : false;
                    ?>
                    <td style="width:20px;"><button <?=$permissao ? '' : 'disabled'; ?> class="btn btn-warning btn-sm" onclick="altCoordenador('<?=$acao['ptres']?>','<?=$_REQUEST['id_acao_programatica']?>','<?=$_REQUEST['percod']?>',<?=$permissao ? 1 : 0?>,'coordenador');" title="Alterar Coordenador"><span class="glyphicon glyphicon-user"></span></button></td>
                </tr>
                <tr>
                    <th style="text-align:right">Validador:</th>
                    <td><?php echo $dadosValidador['nome']; ?></td>
                    <td><?php echo $dadosValidador['fone']; ?></td>
                    <td> <a href="mailto:<?=$dadosValidador['email']; ?>"><?php echo $dadosValidador['email']; ?></a></td>
                    <td style="width:20px;"><button <?=$permissao ? '' : 'disabled'; ?> class="btn btn-warning btn-sm" onclick="altCoordenador('<?=$acao['ptres']?>','<?=$_REQUEST['id_acao_programatica']?>','<?=$_REQUEST['percod']?>',<?=$permissao ? 1 : 0?>,'validador');" title="Alterar Validador"><span class="glyphicon glyphicon-user"></span></button></td>
                </tr>
            </tbody>
        </table>
</div>
<?php
if (isset($_REQUEST['percod']) && $_REQUEST['percod']){
    $sql = "
        SELECT
            sdlp.dotacao_inicial,
            sdlp.dotacao_atual,
            sdlp.empenhado,
            sdlp.liquidado,
            sdlp.pago,
            sdlp.rap_np_inscrito_liquido,
            sdlp.rap_np_liquidado_pagar,
            sdlp.rap_np_liquidado_efetivo,
            sdlp.rap_np_pago,
            sdlp.rap_proc_inscrito,
            sdlp.rap_proc_cancelado,
            sdlp.rap_proc_pagar,
            sdlp.rap_proc_pago,
            sdlp.fisico AS quantidade_fisico,
            sdlp.financeiro AS quantidade_financeiro,
            to_char(sdlp.datareferencia,'dd/mm/YYYY') as datareferencia
        FROM planacomorc.localizador_programatica lpr
    	INNER JOIN planacomorc.snapshot_dotacao_localizador_programatica sdlp USING(id_localizador_programatica)
    	INNER JOIN planacomorc.acao_programatica apr USING(id_acao_programatica)
    	INNER JOIN planacomorc.acao aca USING(id_acao)
    	LEFT JOIN planacomorc.orgao org USING(id_orgao)
    	LEFT JOIN planacomorc.programa prg USING(id_programa)
    	LEFT JOIN planacomorc.quantitativo_sof qts USING(id_acao_programatica, loccod)
        WHERE lpr.loccod = '{$_REQUEST['loccod']}'
            AND org.codigo = '{$acao['cod_orgao']}'
            AND prg.codigo = '{$acao['cod_programa']}'
            AND aca.codigo = '{$acao['cod_acao']}'
            AND id_exercicio = {$_SESSION['exercicio']}
            AND id_periodo_referencia = {$_REQUEST['percod']}
    ";
    $dadosfinanceirossiafi = $db->pegaLinha($sql);

    $sqlAcompanhamentoAcao = "
        SELECT
            to_char(data_apuracao,'DD/MM/YYYY' ) as data_apuracao_formatada,
            *
        FROM planacomorc.acompanhamento_acao a
        LEFT JOIN workflow.documento d ON d.docid = a.docid
        WHERE id_periodo_referencia = '{$_REQUEST['percod']}'
        AND id_acao_programatica = '{$_REQUEST['id_acao_programatica']}'
        AND id_localizador = '{$acao['id_localizador']}'
    ";
    $acompanhamentoacao = $db->pegaLinha($sqlAcompanhamentoAcao);

    $consulta = consultarPermissao(array(
        'esdid' => $acompanhamentoacao['esdid'],
        'id_acao_programatica' => $_REQUEST['id_acao_programatica'],
        'id_periodo_referencia' => PERIODO_ATUAL
        )
    );

    $sqlPer = "
        SELECT
            id_periodo_referencia AS codigo,
            titulo || ' : ' || to_char(inicio_validade,'DD/MM/YYYY') ||' a ' || to_char(fim_validade,'DD/MM/YYYY') as descricao
        FROM planacomorc.periodo_referencia p
    	WHERE id_exercicio = '" . $_SESSION['exercicio'] . "'
    	ORDER BY id_periodo_referencia desc limit 1
    ";
    $pers = $db->pegaUm($sqlPer);
    $id_periodo_referencia = is_array($pers) ? $pers['codigo'] : '';
    $habil = $consulta && ($id_periodo_referencia == $periodoreferencia['id_periodo_referencia']) ? 'S' : 'N';

    // carrega dados para preencher tabela "Acompanhamento Físico dos Planos Orçamentários"
    $sqlAcomp = "
        SELECT
            DISTINCT
            sdpp.id_po_programatica AS id_po,
            sdpp.plocod,
            sdpp.plocod || ' - ' || sdpp.plodsc AS descricao,
            sdpp.produto,
            sdpp.unidade_medida,
            sdpp.dotacao,
            sdpp.empenhado,
            sdpp.liquidado,
            sdpp.pago,
            sdpp.meta
        FROM planacomorc.snapshot_dotacao_po_programatica sdpp
    	INNER JOIN planacomorc.snapshot_dotacao_localizador_programatica slp
            ON (sdpp.id_localizador_programatica = slp.id_localizador_programatica
            AND sdpp.id_periodo_referencia = slp.id_periodo_referencia)
    	INNER JOIN planacomorc.localizador_programatica lpr
            ON lpr.id_localizador_programatica = slp.id_localizador_programatica
    	WHERE lpr.id_acao_programatica = {$_REQUEST['id_acao_programatica']}
            AND sdpp.id_periodo_referencia = {$_REQUEST['percod']}
            AND loccod = '{$_REQUEST['loccod']}'
        ORDER BY 2        
    ";
            #ver($sqlAcomp);
    $metasPO = $db->carregar($sqlAcomp);


    //para não validar obrigatoriedade do campo Registro, quando não houver metas no Acompanhamento.
    if (!$metasPO) {
        $_SESSION['boAcaoPO'] = 0;
    } else {
        $_SESSION['boAcaoPO'] = 1;
    }
?>
<script>

    function gravarAcompanhamentoAcao() {
    	jQuery('#acoexecutadofisico').val(mascaraglobal('#########', jQuery('#acoexecutadofisico').val()));
        if (jQuery('#acoexecutadofisico').val() == '') {
            rolaTela('acoexecutadofisico');
            jQuery('#acoexecutadofisico').focus();
            alert('O valor físico executado deve ser preenchido');
            return false;
        }

        jQuery('#acoexecutadorapfisico').val(mascaraglobal('#########', jQuery('#acoexecutadorapfisico').val()));
        if (jQuery('#acoexecutadorapfisico').val() == '') {
            rolaTela('acoexecutadorapfisico');
            jQuery('#acoexecutadorapfisico').focus();
            alert('O valor físico executado com RAP do exercício anterior deve ser preenchido.');
            return false;
        }

        if (jQuery('#acodataapuracao').val() == '') {
            rolaTela('acodataapuracao');
            jQuery('#acodataapuracao').focus();
            alert('Data de apuração deve ser preenchida.');
            return false;
        }

        if (!validaData(document.getElementById('acodataapuracao'))) {
            rolaTela('acodataapuracao');
            jQuery('#acodataapuracao').focus();
            alert('Data de apuração inválida');
            return false;
        }
        var data = $('#acodataapuracao').val();
        var acodataapuracao = new Date(data).getTime();
        var inipediodo = new Date('01/01/2014').getTime();
        var fimperiodo = new Date('31/12/2014').getTime();

        if ((acodataapuracao < inipediodo) || acodataapuracao > fimperiodo) {
            rolaTela('acodataapuracao');
            jQuery('#acodataapuracao').focus();
            alert('Data de apuração fora do período de referência.');
            return false;
        }

        jQuery('#acoreprogramadofisico').val(mascaraglobal('#########', jQuery('#acoreprogramadofisico').val()));
        if (jQuery('#acoreprogramadofisico').val() == '' || jQuery('#acoreprogramadofisico').val() == 0) {
            rolaTela('acoreprogramadofisico');
            jQuery('#acoreprogramadofisico').focus();
            alert('O valor físico reprogramado deve ser preenchido, e, diferente de zero.');
            return false;
        }

        jQuery('#acoreprogramadofinanceiro').val(mascaraglobal('###.###.###.###,##', jQuery('#acoreprogramadofinanceiro').val()));
        if (jQuery('#acoreprogramadofinanceiro').val() == '') {
            rolaTela('acoreprogramadofinanceiro');
            jQuery('#acoreprogramadofinanceiro').focus();
            alert('O valor financeiro reprogramado deve ser preenchido.');
            return false;
        }
        if (!validarPerguntasQuestionario()) {
            return false;
        }
        // -- Confirmações de submissão:
        var dfsliquidado = parseFloat(jQuery('#dfsliquidado').val());
        var acoexecutadofisico = parseInt(jQuery('#acoexecutadofisico').val());
        var rapliquidadoefetivo = parseFloat(jQuery('#rapliquidadoefetivo').val());
        var dfsdotacaoatual = parseFloat(jQuery('#dfsdotacaoatual').val());
        var acoreprogramadofinanceiro = parseFloat(jQuery('#acoreprogramadofinanceiro').val().replace(/\./g, '').replace(/,/g, '.'));
        var dfsempenhado = parseFloat(jQuery('#dfsempenhado').val());
        var ecloa = parseInt(jQuery('#ECLOA').text());
        var ecrep = parseInt(jQuery('#ECREP').text());

        // -- Preencheu o físico, mas o liquidado LOA é 0;
        if ((0 == dfsliquidado) && (acoexecutadofisico > 0)) {
            if (!confirm('Você informou um valor para o físico, mas não tem financeiro LOA. Quer salvar a proposta mesmo assim?')) {
                return false;
            }
        }
            // -- Preencheu o físico, mas o liquidado RAP é 0;
        if ((0 == rapliquidadoefetivo) && (acoexecutadofisico > 0)) {
            if (!confirm('Você informou um valor para o físico, mas não tem financeiro RAP. Quer salvar a proposta mesmo assim?')) {
                return false;
            }
        }
            // -- Há execução financeira LOA e a captação física é 0

        if ((0 == acoexecutadofisico) && (dfsliquidado > 0)) {
            if (!confirm('Você informou o valor 0 para o físico, mas tem execução financeira LOA. Quer salvar a proposta mesmo assim?')) {
            	return false;
            }
        }
        // -- Há execução financeira LOA e a captação física é 0
        if ((0 == acoexecutadofisico) && (rapliquidadoefetivo > 0)) {
            if (!confirm('Você informou o valor 0 para o físico, mas tem execução financeira RAP. Quer salvar a proposta mesmo assim?')) {
                return false;
            }
        }

        // -- A reprogramação é superior à dotação atual
        if (acoreprogramadofinanceiro > dfsdotacaoatual) {
            if (!confirm('A sua reprogramação é superior à dotação atual. Quer salvar a proposta mesmo assim?')) {
            	return false;
            }
        }
        // -- A reprogramação é inferior ao total empenhado
        if (acoreprogramadofinanceiro < dfsempenhado) {
            if (!confirm('A sua reprogramação é inferior ao total empenhado. Quer salvar a proposta mesmo assim?')) {
                return false;
            }
        }

        if (ecloa > 300) {
            if (!confirm('Seu ECLOA é maior que 300%. Quer salvar a proposta mesmo assim?')) {
                return false;
            }
        }

        if (ecrep > 300) {
            if (!confirm('Seu ECREP é maior que 300%. Quer salvar a proposta mesmo assim?')) {
            	return false;
            }
        }

        retorno = true;//inicializando variável que determinará submit do formulário.
        $("input[class*='obrigatorioRealizado']").each(function(elemento, valor) {
            if ($(valor).val() == '') {
                alert("É obrigatório preencher o(s) campo(s) Realizado.");
                retorno = false;
            }
            //add lógica para return false não sair do each e salvar dados em seguida.
            if (retorno == false) {
            	return false;
            }
        });

        //tratando o retorno se forma a validar todos os input da class "obrigatorioRealizado", antes do submit do formulário.
        if (retorno == true) {
            document.getElementById('formulario').submit();
        } else {
            return false;
        }
    }

    function calcularIndicadores() {
    	// Valor da Dotação Atual
        var valordotacaoatual = parseFloat('<?= (($dadosfinanceirossiafi['dotacao_atual']) ? $dadosfinanceirossiafi['dotacao_atual'] : '0.00') ?>');
        // Meta Física na LOA
        var metafisicaloa = parseInt('<?= (($dadosfinanceirossiafi['quantidade_fisico']) ? $dadosfinanceirossiafi['quantidade_fisico'] : '0') ?>');
        // Valor Liquidado
        var valorliquidado = parseFloat('<?= (($dadosfinanceirossiafi['liquidado']) ? $dadosfinanceirossiafi['liquidado'] : '0.00') ?>');
        // Meta Física Realizada
        if (jQuery('#acoexecutadofisico').val() == '') {
            var metafisicarealizada = 0;
        } else {
            var metafisicarealizada = parseInt(replaceAll(jQuery('#acoexecutadofisico').val(),".",""));
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
            var metafisicareprogramacao = parseInt(replaceAll(jQuery('#acoreprogramadofisico').val(),".",""));
        }

        var EFLOA = 0;

        if (metafisicaloa > 0 && valorliquidado > 0 && metafisicarealizada > 0) {
            EFLOA = ((valordotacaoatual / metafisicaloa) / (valorliquidado / metafisicarealizada)) * 100;
            saida = EFLOA.toFixed(2);
            jQuery('#EFLOA').html(saida + '%');
        } else {
            jQuery('#EFLOA').html('-');
        }

        var EFREP = 0;
        if (metafisicareprogramacao > 0 && valorliquidado > 0 && metafisicarealizada > 0) {
        	EFREP = ((valorreprogramacaofinanceiro / metafisicareprogramacao) / (valorliquidado / metafisicarealizada)) * 100;
            saida = EFREP.toFixed(2);
            jQuery('#EFREP').html(saida + '%');
		} else {
        	jQuery('#EFREP').html('-');
		}

        var ECLOA = 0;
        if (metafisicaloa > 0) {
        	ECLOA = (metafisicarealizada / metafisicaloa) * 100;
            saida = ECLOA.toFixed(2);
            jQuery('#ECLOA').html(saida + '%');
		} else {
        	jQuery('#ECLOA').html('-');
		}

        var ECREP = 0;
        if (metafisicareprogramacao > 0) {
        	ECREP = (metafisicarealizada / metafisicareprogramacao) * 100;
            saida = ECREP.toFixed(2);
            jQuery('#ECREP').html(saida + '%');
		} else {
        	jQuery('#ECREP').html('-');
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

	function definirDateTimePicker(){
		$(".data").datetimepicker({
	        language: "pt-br",
	        pickTime: false,
	        pickDate: true,
	        useCurrent: false
	    });
	}

<?
	if ($acompanhamentoacao['acocod']){ ?>
		jQuery(document).ready(function() {
        	calcularIndicadores();
		});
<?
	}
?>
	jQuery(document).ready(function() {
            $('#acoreprogramadofisico').next().remove();
            $('#acoreprogramadofinanceiro').next().remove();

            definirDateTimePicker();
            var form_habilitado = jQuery('#form_habilitado').val();
            if ('S' == form_habilitado) {
                // -- Copiando valor de físico para o RAP, qdo ações forem 20RK e 20RL
                var cod_acao = jQuery('#cod_acao').val();
                if (('20RK' == cod_acao) || ('20RL' == cod_acao)) {
                    jQuery('#acoexecutadofisico').blur(function() {
                        // -- Preenchendo #acoexecutadorapfisico com o valor de #acoexecutadofisico
                        jQuery('#acoexecutadorapfisico').val(jQuery(this).val());
                    });
                }

                // -- Zerando o físico executado no exercicio
                var fisico_executado = jQuery('#acoexecutadofisico').val();
                var rap_inscrito_liquido = jQuery('#rapinscritoliquido').val();
                if (('' == fisico_executado) && ((0 == rap_inscrito_liquido) || ('' == rap_inscrito_liquido))) {
                    jQuery('#acoexecutadorapfisico').val('0');
                }
            }
	});
</script>

<form method="post" name="formulario" id="formulario" enctype="multipart/form-data">
    <input type="hidden" name="cod_acao" id="cod_acao" value="<?php echo $acao['cod_acao']; ?>" />
    <input type="hidden" name="form_habilitado" id="form_habilitado" value="<?php echo $habil; ?>" />
    <input type="hidden" name="requisicao" value="gravarAcompanhamentoAcao">
    <input type="hidden" name="percod" value="<?= $_REQUEST['percod'] ?>">
    <input type="hidden" name="id_localizador" value="<?= $acao['id_localizador']; ?>" />
    <section class="row">
        <section class="col-md-11" style="margin-left:0;padding-left:0;">
            <section class="panel panel-info" >
                <section class="panel-heading">
                    <strong>Dados Financeiros</strong>
                    <strong style="float:right;clear:both">Data de Referência: <?echo $dadosfinanceirossiafi['datareferencia'];?></strong>
                </section>
                <table class="table">
                    <tr>
                        <td>
                            <table class="table table-striped table-bordered">
                                <tr>
                                    <th style="text-align:right">Dotação inicial: </th>
                                    <td>
                                        <?= number_format2($dadosfinanceirossiafi['dotacao_inicial']) ?>
                                        <input type="hidden" name="dfsdotacaoinicial" value="<?= $dadosfinanceirossiafi['dotacao_inicial'] ?>">
                                    </td>
                                </tr>
                                <tr>
                                    <th style="text-align:right">Dotação(L+C): </th>
                                    <td>
                                        <?= number_format2($dadosfinanceirossiafi['dotacao_atual']) ?>
                                        <input type="hidden" name="dfsdotacaoatual" id="dfsdotacaoatual" value="<?= $dadosfinanceirossiafi['dotacao_atual'] ?>">
                                    </td>
                                </tr>
                                <tr>
                                    <th style="text-align:right">Empenhado: </th>
                                    <td>
                                        <?= number_format2($dadosfinanceirossiafi['empenhado']) ?>
                                        <input type="hidden" name="dfsempenhado" id="dfsempenhado" value="<?= $dadosfinanceirossiafi['empenhado'] ?>">
                                    </td>
                                </tr>
                                <tr>
                                    <th style="text-align:right">Liquidado: </th>
                                    <td>
                                        <?= number_format2($dadosfinanceirossiafi['liquidado']) ?>
                                        <input type="hidden" name="dfsliquidado" id="dfsliquidado" value="<?= $dadosfinanceirossiafi['liquidado'] ?>">
                                    </td>
                                </tr>
                                <tr>
                                    <th style="text-align:right">Pago: </th>
                                    <td>
                                        <?= number_format2($dadosfinanceirossiafi['pago']) ?>
                                        <input type="hidden" name="dfspago" value="<?= $dadosfinanceirossiafi['pago'] ?>">
                                    </td>
                                </tr>
                            </table>
                        </td>
                        <td>
                            <table class="table table-striped table-bordered">
                                <tr>
                                    <td class="active text-center" colspan="2">
                                        <strong>RAP não processado</strong> <span class="glyphicon glyphicon-info-sign" title="A partir de 2013, será realizado o acompanhamento da execução física referente aos Restos a Pagar (RAP), cujos dados também são trazidos na tela de dados financeiros."></span>
                                    </td>
                                </tr>
                                <tr>
                                    <th style="text-align:right">Inscrito Líquido: </th>
                                    <td>
                                        <input type="hidden" id="rapinscritoliquido"
                                            value="<?php echo $dadosfinanceirossiafi['rap_np_inscrito_liquido'] ?>" />
                                            <?= number_format2($dadosfinanceirossiafi['rap_np_inscrito_liquido']) ?>
                                    </td>
                                </tr>
                                <tr>
                                    <th style="text-align:right">Liquidado a pagar: </th>
                                    <td><?= number_format2($dadosfinanceirossiafi['rap_np_liquidado_pagar']) ?></td>
                                </tr>
                                <tr>
                                    <th style="text-align:right">Pago: </th>
                                    <td><?= number_format2($dadosfinanceirossiafi['rap_np_pago']) ?></td>
                                </tr>
                                <tr>
                                    <th style="text-align:right">Liquidado efetivo: </th>
                                    <td>
                                        <?= number_format2($dadosfinanceirossiafi['rap_np_liquidado_efetivo']) ?>
                                        <input type="hidden" id="rapliquidadoefetivo" value="<?php echo $dadosfinanceirossiafi['rap_np_liquidado_efetivo'] ?>" />
                                    </td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                    <?php
                    $podeEnviarSIOP = false;
                    if ($podeEnviarSIOP) {
                    ?>
                        <tr class="success" style="color:white;">
                            <td colspan="3">
                            Dados de Envio ao SIOP
                            </td>
                        </tr>
                    <tr>
                        <td class="SubTituloEsquerda" colspan="3" title="">
                            <input class="btn btn-primary" type="button" value="Enviar ao SIOP" onclick="enviarAoSiop()" />
                        </td>
                    </tr>
                    <?php
                    }
                    ?>
                </table>
            </section>
        </section>
        <section class="col-md-1">
            <?php
            if ($acompanhamentoacao['docid']) {
                /* Barra de estado atual e ações e Historico */
                wf_desenhaBarraNavegacao($acompanhamentoacao['docid'], array(
                'acaidentificadorunicosiop' => $_REQUEST['id_acao_programatica'],
                'idacomacao' => $acompanhamentoacao['id_acompanhamento_acao']));
            }else{
            ?>
            <h1><span class="glyphicon glyphicon-time text-warning"></span></h1>
            <?
            }

            ?>
        </section>
    </section>

    <!-- Subações Financiadas pela Ação -->
    <section class="panel panel-info">
    	<section class="panel-heading"><strong>Subações Financiadas pela Ação</strong> <span class="glyphicon glyphicon-info-sign" title="Para a Administração Direta, CAPES, INEP, FNDE e EBSERH, este campo trará o físico e o financeiro executados nas subações , como forma de facilitar o preenchimento dos dados de acompanhamento da ação orçamentária e a elaboração do relatório de gestão. Para as demais Unidades aparecerá a mensagem “Sem subações financiadas por esta ação”, mesmo que haja subações cadastradas no módulo PPA – Monitoramento e Avaliação."></span></section>
        <?php
        $sql = <<<DML
            SELECT DISTINCT sb.id_subacao,
                sb.codigo || ' - ' || sb.titulo as titulo,
                sigla,
                sb.descricao,
                sb.codigo AS codigo,
                sb.id_exercicio
            FROM planacomorc.subacao sb
            JOIN planacomorc.dotacao_subacao ds ON ds.id_subacao = sb.id_subacao
            JOIN planacomorc.ptres pt ON pt.id_ptres = ds.id_ptres
            JOIN monitora.ptres ptr on ptr.ptres = pt.ptres
            WHERE ptr.acacod = '{$acao['cod_acao']}'
                AND ptr.unicod = '{$acao['cod_orgao']}'
                AND ptr.loccod = '{$_REQUEST['loccod']}'
                AND ptr.prgcod = '{$acao['cod_programa']}'
                AND sb.id_exercicio = {$_SESSION['exercicio']}
                AND sb.id_exercicio::varchar = ptr.ptrano
DML;

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $listagem->setCabecalho(array('Subação','Sigla','Descrição','Exercício'));
        $listagem->setQuery($sql);
        $listagem->esconderColuna('codigo');
        $listagem->addCallbackDeCampo('titulo', 'alinhaParaEsquerda');
        $listagem->addCallbackDeCampo('descricao', 'alinhaParaEsquerda');
        $listagem->addAcao('view', array('func'=>'exibirSubacao','extra-params'=>array('codigo')));
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
        ?>
    	<section class="panel-body">
            <section class="alert alert-warning">
                <span class="glyphicon glyphicon-warning-sign"></span>
                <strong>Acompanhamento Físico da Ação </strong>
                <br/>
                <br/>
                Físico Executado em 2014: Informar o quantitativo físico do produto executado até a data de apuração, no período de 01/01/2014 a 31/12/2014.
                <br>*** Atenção***<br>
                Por orientação da Secretaria de Orçamento Federal, a execução física deve ser informada com base no FINANCEIRO LIQUIDADO.
            </section>
            <table class="table">
                <tr>
                    <td style="width:50%;">
                        <table class="table table-bordered">
                            <tr>
                                <td class="text-center active" colspan="2"><strong>Acompanhamento Físico da Ação</strong></td>
                            </tr>
                            <tr>
                                <td class="text-right" style="width:50%;">Produto: </td>
                                <td><?= (($acao['produto']) ? $acao['produto'] : "-") ?></td>
                            </tr>
                            <tr>
                                <td class="text-right" style="width:50%;">Unidade: </td>
                                <td><?= (($acao['unidademedida']) ? $acao['unidademedida'] : "-") ?></td>
                            </tr>
                            <tr>
                                <td class="text-right">Meta Física: </td>
                                <td><?= mascaraNumero($dadosfinanceirossiafi['quantidade_fisico']); ?></td>
                            </tr>
                            <tr>
                                <td class="text-right" style="width:50%; vertical-align: middle;">Físico executado em <?= $_SESSION['exercicio'] ?>: </td>
                                <td>
                                    <?php inputTexto('acoexecutadofisico', $acompanhamentoacao['executado_fisico'], 'acoexecutadofisico', 11,false,array('masc'=>"###.###.###",'evtkeyup'=>'calcularIndicadores();','label'=>'Físico executado')); ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="text-right" style="width:50%;vertical-align: middle;">Físico executado com RAP do exercício anterior: </td>
                                <td>
                                    <?php inputTexto('acoexecutadorapfisico', $acompanhamentoacao['executado_rap_fisico'], 'acoexecutadorapfisico', 11,false,array('masc'=>'###.###.###','label'=>'Físico executado com Restos a Pagar (RAP): Neste campo deverá ser registrada a execução física realizada com Restos a Pagar (RAP), caso tenha ocorrido.')); ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="text-right" style="width:50%;vertical-align: middle;">Data de apuração: </td>
                                <td>
                                <?
                                $metaDataCombo = array(
                                    'size' => 10,
                                    'classe' => 'data',
                                    'label' => 'Informar a data de apuração dos dados da execução física, que deve ser IGUAL OU ANTERIOR a 31/01/2014.'
                                );
                                inputTexto('acodataapuracao', $acompanhamentoacao['data_apuracao_formatada'], 'acodataapuracao', $limite,false,$metaDataCombo);
                                ?>
                                </td>
                            </tr>
                        </table>
                    </td>
                    <td>
                        <?php $perexibirindicadores = $db->pegaUm("SELECT exibir_indicadores FROM planacomorc.periodo_referencia WHERE id_exercicio = '" . $_REQUEST['percod'] . "'"); ?>
                        <table class="table table-bordered" <?= (($perexibirindicadores == 't') ? 'style="display:none;"' : "") ?>>
                            <tr>
                                <td class="text-center active" colspan="2"><strong>Indicadores</strong></td>
                            </tr>
                            <tr>
                                <td style="width:80%;">Eficiência em relação à meta na LOA (EFLOA)</td>
                                <td id="EFLOA"></td>
                            </tr>
                            <tr>
                                <td style="width:80%;">Eficiência em relação à meta após a reprogramação (EFREP)</td>
                                <td id="EFREP"></td>
                            </tr>
                            <tr>
                                <td style="width:80%;">Eficácia em relação à meta da LOA (ECLOA)</td>
                                <td id="ECLOA"></td>
                            </tr>
                            <tr>
                                <td style="width:80%;">Eficácia em relação à meta após a reprogramação (ECREP)</td>
                                <td id="ECREP"></td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
            <!-- Reprogramação -->
            <section class="">
                <table class="table table-bordered text-center table-condensed">
                    <thead>
                        <tr class="active">
                            <th colspan="4" class="text-center">Reprogramação</th>
                        </tr>
                        <tr class="active">
                            <th></th>
                            <th class="text-center">Inicial</th>
                            <th class="text-center">Reprogramado</th>
                            <th></th>
                        </tr>
                    </thead>
                    <tbody class="">
                        <tr>
                            <th class="active">Físico</th>
                            <td>
                                <?= $dadosfinanceirossiafi['quantidade_fisico'] ? mascaraNumero($dadosfinanceirossiafi['quantidade_fisico']) : '-'; ?>
                                <input type="hidden" name="quantidade_fisico" value="<?= $dadosfinanceirossiafi['quantidade_fisico'] ?>">
                            </td>
                            <td>
                                <?php inputTexto('acoreprogramadofisico', $acompanhamentoacao['reprogramado_fisico'], 'acoreprogramadofisico', 11,false,array('label'=>'Reprogramado FÍSICO','masc'=>'###.###.###','evtkeyup'=>'calcularIndicadores();'));?>
                            </td>
                            <td>
                            <? if ($consulta){ ?>
                                <input class="btn btn-primary" type="button" name="repetirfisico" value="Repetir valor inicial"
                                    onclick="jQuery('#acoreprogramadofisico').val('<?= $dadosfinanceirossiafi['quantidade_fisico']; ?>');
                                    calcularIndicadores();" />
                            <? } ?>
                            </td>
                        </tr>
                        <tr>
                            <th class="active">Financeiro</th>
                            <td>
                                <?= number_format2($dadosfinanceirossiafi['dotacao_atual']) ?>
                            </td>
                            <td>
                                <?php inputTexto('acoreprogramadofinanceiro', ($acompanhamentoacao['reprogramado_financeiro']) ? number_format($acompanhamentoacao['reprogramado_financeiro'], 2, ',', '.') : '', 'acoreprogramadofinanceiro', 18,false,array('masc'=>'###.###.###.###,##','label'=>'Reprogramado FINANCEIRO','acao'=>'calcularIndicadores();'));?>
                            </td>
                            <td>
                                <?if ($consulta){ ?>
                                <input class="btn btn-primary" type="button" name="repetirfinanceiro" value="Repetir valor inicial"
                                    onclick="jQuery('#acoreprogramadofinanceiro').val('<?= number_format($dadosfinanceirossiafi['dotacao_atual'], 2, ',', '.') ?>');
                                    calcularIndicadores();" />
                               <? } ?>
                            </td>
                        </tr>
                    </tbody>
                </table>
                <!-- Acompanhamento Físico dos Planos Orçamentários -->
                <table class="table table-bordered">
                    <thead>
                        <tr>
                            <td colspan="4" rowspan="2" class="active text-center"><b>Acompanhamento Físico dos Planos Orçamentários</b></td>
                        </tr>
                    </thead>
                    <tbody>
                    <?
                    if($_REQUEST['percod']){
                        $sql = "
                            SELECT
                                titulo || ' : ' || to_char(inicio_validade,'DD/MM/YYYY') ||' a ' || to_char(fim_validade,'DD/MM/YYYY') AS descricao
                            FROM planacomorc.periodo_referencia p
                            WHERE id_exercicio = '" . $_SESSION['exercicio'] . "'
                                AND id_periodo_referencia = " . $_REQUEST['percod'] . "
                            ORDER BY inicio_validade DESC
                        ";
                        $descricaoper = $db->pegaUm($sql);
                    }
                    if (!$metasPO){
                            echo "<section class=\"alert alert-danger text-center\">Não há POs com meta física para acompanhamento</section>";
                    }else{
                        echo "<input type=\"hidden\" name=\"id_localizador_programatica\" value=\"{$acao['id_localizador_programatica']}\" />";
                        foreach ($metasPO as $po) {
                            $sqlAcompanhamentoPO = "
                                SELECT *
                                FROM planacomorc.snapshot_dotacao_po_programatica AS sdpp
                                LEFT JOIN planacomorc.acompanhamento_po AS po
                                    ON (sdpp.id_po_programatica = po.id_po_programatica)
                                WHERE sdpp.id_localizador_programatica = {$acao['id_localizador_programatica']}
                                    AND sdpp.id_periodo_referencia = {$_REQUEST['percod']}
                                    AND sdpp.plocod = '{$po['plocod']}'
                                    AND po.id_periodo_referencia = {$_REQUEST['percod']}
                                    AND po.plocod = '{$po['plocod']}'
                                    AND sdpp.id_po_programatica = '{$po['id_po']}'
                            ";
                            #ver($sqlAcompanhamentoPO);
                            $dadosAcompPO = $db->pegaLinha($sqlAcompanhamentoPO);
                    ?>
                        <tr>
                            <td class="active" colspan="4"></td>
                        </tr>
                        <tr>
                            <th colspan="4" class="panel-heading">Plano Orçamentário: <font style="color: #2560c7;"><?= (($po['descricao']) ? $po['descricao'] : "--") ?></font></th>
                        </tr>
                        <tr>
                            <input type="hidden" name="plocod" value="<?= $po['plocod'] ?>">
                            <td colspan="2">
                                <label class="control-label">Produto: </label> <font style="color: #2560c7;"><?= (($po['produto']) ? $po['produto'] : "--") ?></font>
                            </td>
                            <td colspan="2">
                                <label class="control-label">Unidade de Medida: </label> <font style="color: #2560c7;"><?= (($po['unidade_medida']) ? $po['unidade_medida'] : "--") ?></font>
                            </td>
                        </tr>
                        <tr>
                            <td colspan="2" class="text-center active">
                                <h4 style="margin-bottom:0;margin-top:0;">Financeiro <small>Acumulado (<?= $descricaoper ?>)</small></h4>
                            </td>
                            <td colspan="2" class="text-center active">
                                <h4 style="margin-bottom:0; margin-top:0;">Físico <small>Acumulado (<?= $descricaoper ?>)</small></h4>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <label class="control-label">Dotação Atual (R$):</label> <?= (($po['dotacao']) ? number_format($po['dotacao'], 2, ',', '.') : "--") ?>
                            </td>
                            <td>
                                <label class="control-label">Empenhado (R$):</label> <?= (($po['empenhado']) ? number_format($po['empenhado'], 2, ',', '.') : "-") ?>
                            </td>
                            <td rowspan="2" style="text-align:center;vertical-align:middle;">
                                <label class="control-label">Meta:</label> <?= (($po['meta']) ? mascaraNumero($po['meta']) : "-") ?>
                            </td>
                            <td rowspan="2" class="form-inline">
                                <section class="col-md-8 col-md-offset-2" style="margin-top:15px;">
                                <label class="control-label" for="realizadoloa<?= $po['plocod'] ?>">Realizado:</label>
                                <?php
                                    if ($po['meta'] != "") {
                                        if ($habil == 'N')
                                            $edital = "readonly='readonly'";
                                        if ($habil == 'N')
                                            $editalvelrealizadoloa = "readonly='readonly'";
                                ?>
                                <input type="text" style="width:100px;" class="form-control obrigatorioRealizado" id="realizadoloa<?= $po['plocod'] ?>"
                                    onblur="MouseBlur(this);mascaraglobal('###.###.###', this.value);"
                                    required="true"
                                    onmouseout="MouseOut(this);"
                                    onfocus="MouseClick(this);"
                                    onmouseover="MouseOver(this);"
                                    onkeyup="this.value = mascaraglobal('###.###.###', this.value);"
                                    value="<?= $dadosAcompPO['realizado_loa']; ?>" maxlength="11" size="15"
                                    name="realizadoloa[<?= $po['plocod'] ?>]" <?php echo $edital ?>/>
                                <?  } else { ?>
                                <span style="color:#5bc0de;" class="text-center">Sem meta para acompanhamento</span>
                                <?  } ?>
                                </section>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <label class="control-label">Liquidado (R$):</label> <?= (($po['liquidado']) ? number_format($po['liquidado'], 2, ',', '.') : "-") ?>
                            </td>
                            <td>
                                <label class="control-label">Pago (R$):</label> <?= (($po['pago']) ? number_format($po['pago'], 2, ',', '.') : "-") ?>
                            </td>
                        </tr>


                        <tr>
                            <td class="active" colspan="4"></td>
                        </tr>
                    <?
                        }
                    }
                    ?>
                    </tbody>
                </table>
                <table class="table table-bordered">
                    <thead>
                        <tr>
                            <th colspan="2" class="active text-center">Questionário de Acompanhamento das Ações <span title="O questionário tem por objetivo orientar o preenchimento do acompanhamento orçamentário, para que este gere informações que expliquem de forma clara, objetiva e completa, os resultados de execução física apurados no período." class="text-primary glyphicon glyphicon-info-sign"></span></th>
                        </tr>
                    </thead>
                    <tbody>
                    <?php
                    $sql = "
                        SELECT *
                        FROM planacomorc.monquestionario mq
                        INNER JOIN planacomorc.monqstacaperiodo ma ON ma.qstid = mq.qstid
                        WHERE ma.id_acao_programatica='" . $_REQUEST['id_acao_programatica'] . "'
                            AND mq.id_periodo_referencia='" . $_REQUEST['percod'] . "'
                            AND qststatus='A'
                        ORDER BY mq.qstid
                    ";
                    $monquestionario = $db->carregar($sql);
                    if ($monquestionario) {
                        foreach ($monquestionario as $questionario){
                    ?>

                        <tr class="active">
                            <th width="25%" class="">Questionário:</th>
                            <td> <?php echo $questionario['qstnome']; ?></td>
                        </tr>
                            <?
                            $monqstperguntas = $db->carregar("SELECT * FROM planacomorc.monqstperguntas mp WHERE qstid='" . $questionario['qstid'] . "' order by mqpordem");
                            if ($monqstperguntas[0]){
                                foreach ($monqstperguntas as $per){
                                    $val_perg[] = $per['mqpid'];
                            ?>
                                    <tr>
                                        <th colspan="2" style="text-align: left !important;"><?php echo $per['mqpdescricao']; ?></th>
                                    </tr>
                                    <tr>
                                        <td colspan="2">
                                        <?
                                        if ($acompanhamentoacao['id_acompanhamento_acao'] && $acompanhamentoacao['id_acompanhamento_acao'] != '') {
                                            $sqlResposta = "
                                                SELECT
                                                    mqrresposta
                                                FROM planacomorc.monquestionarioresposta
                                                WHERE id_acompanhamento_acao = '{$acompanhamentoacao['id_acompanhamento_acao']}'
                                                    AND mqpid = '{$per['mqpid']}'
                                            ";
                                            $mqrresposta = $db->pegaUm($sqlResposta);
                                        } else {
                                            $mqrresposta = $per['mqpresppadrao'];
                                        }
                                        $obrigatorio = (1 == $per['mqpfacultativo']) ? 'N' : 'S';
                                        inputTextArea('resposta_'.$per['mqpid'], $mqrresposta, 'resposta_'.$per['mqpid'], $per['mqprespnumcaracteres'],array('obrig'=>$obrigatorio));
                                        ?>
                                        </td>
                                    </tr>
                            <?
                                }
                            }
                            ?>
                        <?
                        }
                    }else{
                        echo "<tr><td><section class=\"alert alert-danger text-center\">Não existem questionários cadastrados.</section></td></tr>";
                    }
                    if ($val_perg) {
                        foreach ($val_perg as $p) {
                            echo '<input type="hidden" name="idperguntas[]" value="' . $p . '">';
                        }
                    }
                    ?>
                    </tbody>
                </table>
            </section>
    	</section>
    </section>

    <?php
    $perfis = pegaPerfilGeral();
    $statusAtual = pegarEstadoAtual($acompanhamentoacao['id_acompanhamento_acao']);
    if ((in_array(PFL_COORDENADORACAO, $perfis))
        && ($id_periodo_referencia == $periodoreferencia['id_periodo_referencia'])
        && (!$statusAtual || $statusAtual == ESD_EMPREENCHIMENTO || $statusAtual == ESD_EMELABORACAO || ESD_EMVALIDACAO == $statusAtual)
    ) {
        $sql = "
        SELECT
            CASE WHEN CURRENT_DATE NOT BETWEEN inicio_preenchimento AND fim_preenchimento
            THEN TRUE
            ELSE FALSE
            END AS validade_periodo
        FROM planacomorc.periodo_referencia
        WHERE id_periodo_referencia = {$id_periodo_referencia}
        ";

        if ($db->pegaUm($sql) == 't') {
            echo '<section class="col-md-6 col-md-offset-3 alert alert-warning text-center">Periodo de preenchimento expirado.</section>';
        }else{
            echo '<input type="button" name="salvar" class="btn btn-primary" value="Salvar" onclick="gravarAcompanhamentoAcao();" title="IMPORTANTE: não esqueça de salvar as informações.">';
        }
    } elseif ((in_array(PFL_VALIDADORACAO, $perfis) || in_array(PFL_VALIDADOR_SUBSTITUTO, $perfis))
              && ($id_periodo_referencia == $periodoreferencia['id_periodo_referencia'])
              && ((!$statusAtual)
                  || ($statusAtual == ESD_EMPREENCHIMENTO
                      || $statusAtual == ESD_EMANALISE
                      || $statusAtual == ESD_EMELABORACAO
                      || ESD_EMVALIDACAO == $statusAtual))
    ) {
        $sql = "
            SELECT
                CASE WHEN CURRENT_DATE NOT BETWEEN inicio_preenchimento AND fim_preenchimento
                THEN TRUE
                ELSE FALSE
                END AS validade_periodo
            FROM planacomorc.periodo_referencia
            WHERE id_periodo_referencia = {$id_periodo_referencia}
        ";

        if ($db->pegaUm($sql) == 't') {
            echo '<section class="col-md-6 col-md-offset-3 alert alert-warning text-center">Periodo de preenchimento expirado.</section>';
        }else{
            echo '<input type="button" name="salvar" class="btn btn-primary" value="Salvar" onclick="gravarAcompanhamentoAcao();" title="IMPORTANTE: não esqueça de salvar as informações.">';
        }
    } elseif (in_array(PFL_CPMO, $perfis) || in_array(PFL_SUPERUSUARIO, $perfis)) {
        echo '<input type="button" name="salvar" class="btn btn-primary" value="Salvar" onclick="gravarAcompanhamentoAcao();" title="IMPORTANTE: não esqueça de salvar as informações.">';
    } else {
        echo '<section class="alert alert-info text-center">Não habilitado para edição</section>';
    }
    ?>

    <script>
        function validarPerguntasQuestionario() {
            <? if ($val_perg): ?>
                <? foreach ($val_perg as $perg) : ?>
                            jQuery('#resposta_<?= $perg ?>').keyup();
                            if ((jQuery('#resposta_<?= $perg ?>').val() == '')
                                    && (jQuery('#resposta_<?= $perg ?>').hasClass('obrigatorio'))) {
                                alert('Todas as perguntas indicadas como obrigatórias devem ser respondidas');
                                return false;
                            }
                <? endforeach; ?>
            <? endif; ?>
            return true;
        }
        //function chamada ao clicar na imagem + e -, para ocultar tabela com informações "Acompanhamento Físico dos Planos Orçamentários"
        function abrefecha(funcao, tipo) {
            if (funcao == 'abre') {
                $("#" + tipo).show();
            } else {
                $("#" + tipo).hide();
            }
        }

        /*Chamada inicial do Cálculo dos Indicadores*/
        calcularIndicadores();
    </script>
</form>
<?
    }else{
        /* Caso perca a sessão ou a programática, enviar para o início */
        echo "<script>window.location='planacomorc.php?modulo=principal/acoes/listaunidades&acao=A';</script>";
        die;
    }
?>
<div id="modalSubacao" style="display:none;" title="Execução da subação: <?php echo $acao['cod_acao']; ?>"></div>
