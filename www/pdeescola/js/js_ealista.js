jQuery.noConflict();

/*** Quando o DOM estiver pronto ***/
jQuery(document).ready(function()
{
	/*** Redireciona para a página do relatório ***/
	jQuery("#btGeralConsolidado").click(function()
	{
		window.location = 'pdeescola.php?modulo=earelatorio/relatorio_plano_atendimento&acao=A';
	});
}); 