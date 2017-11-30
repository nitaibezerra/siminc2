<?php
/**
 * Processa as requições recebidas pelo arquivo cadastro.inc.
 * São chamadas funções incluídas no arquivo _funcoespi.php, incluído
 * em cadastro.inc. Também são utilizadas constantes definidas em
 * _constantes.php
 *
 * @see cadastro.inc
 * @see _funcoespi.php
 * @see _constantes.php
 *
 * $Id$
 */

/* Monta a combo de UGs filtrando por UO */
if ($_POST['carregarComboUG']) {
	header('content-type: text/html; charset=ISO-8859-1');
    carregarComboUG($_POST['unicod']);
    exit;
}

/* Monta a combo de subações */
if ($_POST['carregarComboSubacao']) {
	header('content-type: text/html; charset=ISO-8859-1');
	carregarComboSubacao($_POST['unicod'], $_POST['ungcod']);
	exit;
}

/* Monta a combo de subações */
if ($_POST['carregarComboSubacaoInstituicoes']) {
	header('content-type: text/html; charset=ISO-8859-1');
	carregarComboSubacaoUO($_POST['unicod']);
	exit;
}

//valida cod pi AJAX
if ($_POST['piAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	validaCodPi($_POST['piAjax'], $_POST['pliid']);
	exit;
}

//valida cod pi AJAX
if ($_REQUEST['sbaAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	buscaDadosSubacao($_POST['sbaAjax'], $_POST['capidAjax']);
	exit;
}

if ($_REQUEST['carregarComboEnquadramentoPorSubacao']) {
	header('content-type: text/html; charset=ISO-8859-1');
	carregarComboEnquadramentoPorSubacao($_POST['sbaid']);
	exit;
}

$perfis = pegaPerfilGeral();

if (strpos($_SERVER['HTTP_REFERER'], 'unidade')) {
    $href = $href ? $href : 'planacomorc.php?modulo=principal/unidade/listapimanter'. $sufixoOrigemRequisicao. '&acao=A';
} else {
    $href = 'planacomorc.php?modulo=principal/planointerno/listapimanter'. $sufixoOrigemRequisicao. '&acao=A';
}

//salvarPI($dados);
// -- Salvando o novo PI
if (isset($_REQUEST['evento'])) {
    $func = '';
    switch ($_REQUEST['evento']) {
        case 'S':
            $func = 'salvarPI';
            $params = array($_REQUEST, true);
            break;
        case 'L':
            $func = 'salvarSolicitacaoPI';
            break;
        case 'E':
            $func = 'inativarPI';
            break;
        case 'AS':
            $func = 'excluirTransacao';
            break;
    }
    if (!$func) {
        simec_redirecionar($href, 'error');
    } else {
        $retorno = false;
        if ('salvarPI' == $func) {
            $retorno = call_user_func_array($func, $params);
//salvarPI($params);
            $href = 'planacomorc.php?modulo=principal/unidade/cadastro_pi'. $sufixoOrigemRequisicao. '&acao=A&pliid=' . $retorno;
        } else {
            $retorno = $func($_REQUEST);
        }
        if ($retorno) {
            simec_redirecionar($href, 'success', 'Operação realizada com sucesso! <br /><br /> Favor enviar para aprovação.');
        } else {
            simec_redirecionar($href, 'error');
        }
    }
    
    exit;
}
