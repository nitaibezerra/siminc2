<?php
/**
 * Formulário de confirmação de substituição de limites.
 *
 * $Id: limitesConfirmar.inc 102352 2015-09-11 14:52:35Z maykelbraz $
 *
 * @filesource
 */
?>
<link rel="stylesheet" href="/library/bootstrap-toggle/css/bootstrap-toggle.min.css">
<script src="/library/bootstrap-toggle/js/bootstrap-toggle.min.js"></script>
<form method="POST" id="confirmacoes">
    <input type="hidden" name="requisicao" value="confirmarSubstituicao" />
    <input type="hidden" name="confirmacoes[dspid]" value="<?php echo $_REQUEST['dspid']; ?>" />
<?php
$list = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
$list->setDados($_SESSION['proporc']['importacao-limites']['confirmacoes'])
    ->setCabecalho(array('Confirmar (S/N)', 'UO', 'Fonte', 'Valor atual (R$)', 'Novo valor(R$)'))
    ->addCallbackDeCampo('id', 'formatarCheckbox')
    ->addCallbackDeCampo('valor', 'mascaraMoeda')
    ->addCallbackDeCampo('novovalor', 'formatarAlteracaoValor')
    ->setLarguraColuna('unicod', '10%')
    ->turnOffForm()
    ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
$_SESSION['proporc']['importacao-limites'] = null;
unset($_SESSION['proporc']['importacao-limites']);
?>
    <button type="submit" class="btn btn-primary" /><span class="glyphicon glyphicon-ok"></span> Confirmar</button>
</form>