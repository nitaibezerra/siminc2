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
        $msg = 'Sua requisição é inválida.';
    } else {
        $retorno = false;
        if ('salvarPI' == $func) {
            $retorno = call_user_func_array($func, $params);
        } else {
            $retorno = $func($_REQUEST);
        }
        if ($retorno) {
            $msg = 'Sua requisição foi executada com sucesso.';
        } else {
            $msg = 'Não foi possível realizar sua requisição.';
        }
    }
    $perfis = pegaPerfilGeral();

    if (strpos($_SERVER['HTTP_REFERER'], 'unidade')) {
        $href = 'planacomorc.php?modulo=principal/unidade/listapimanter&acao=A';
    } else {
        $href = 'planacomorc.php?modulo=principal/planointerno/listapimanter&acao=A';
    }
?>
<script type="text/javascript">
    alert('<?php echo $msg; ?>');
    window.location.href="<?php echo $href; ?>";
</script>
<?php
    exit;
}
