<?php
ini_set("memory_limit", "1024M");
set_time_limit(0);

function carregaTela() {

    include_once APPRAIZ . 'par/modulos/principal/acompanhamento/ferramentas/reprogramacao.inc';
}

include APPRAIZ . "includes/cabecalho.inc";

echo'<br>';
$db->cria_aba($abacod_tela, $url, '');

monta_titulo('Acompanhamento FNDE', '');

if ($_REQUEST['req']) {
    $_REQUEST['req']();
    die();
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JQuery/carouFredSel-6.2.1/jquery.carouFredSel-6.2.1.js"></script>
<script type="text/javascript">
    $(document).ready(function() {
        $('.ferramenta').live('click', function() {
            window.location.href = $(this).attr('id');
        });
    });
</script>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
    <tr>
        <td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top; -webkit-border-radius: 20px;	-moz-border-radius: 20px;	border-radius: 20px;" colspan="4">
<?php
$param = Array();
$param['tamanho_icone'] = 90;
$param['icones'] = Array(
						Array('url' => '../imagens/par/reprogramacao_orcamentaria3.png',
					        'titulo' => 'Reprogramação',
					        'class' => 'ferramenta',
					        'height' => '102',
					        'id' => 'par.php?modulo=principal/acompanhamento/ferramentas/reprogramacao&acao=A'),
					    Array('url' => '../imagens/par/reenvio3.png',
					        'titulo' => 'Adesões pendentes para envio manual',
					        'class' => 'ferramenta',
					        'height' => '102',
					        'id' => 'par.php?modulo=principal/acompanhamento/ferramentas/listaAdesoesPendentes&acao=A'),
					    Array('url' => '../imagens/par/contratosfirmados.png',
					        'titulo' => 'Termos com contratos firmados',
					        'class' => 'ferramenta',
					        'height' => '102',
					        'id' => 'par.php?modulo=principal/acompanhamento/ferramentas/termoContratosNotas&acao=A'),
					    Array('url' => '../imagens/par/demandas.png',
					        'titulo' => 'Lista de Demandas',
					        'class' => 'ferramenta',
					        'height' => '102',
					        'id' => 'par.php?modulo=principal/demandas/demandas&acao=A'),
					    Array('url' => '../imagens/par/icon-exclamation.png',
					        'titulo' => 'Restrições',
					        'class' => 'ferramenta',
					        'height' => '102',
					        'id' => 'par.php?modulo=sistema/tabelasapoio/restricoes_fnde/acompanhamentoRestricao&acao=A'),
					    Array('url' => '../imagens/par/nutricionista.png',
					        'titulo' => 'Analisar Cadastro de Nutricionistas',
					        'class' => 'ferramenta',
					        'height' => '102',
					        'id' => 'par.php?modulo=principal/acompanhamento/ferramentas/analiseTecnicoNutricionista&acao=A'),
                        Array('url' => '../imagens/validacao-execucao.png',
					        'titulo' => 'Validação e Execução',
					        'class' => 'ferramenta',
					        'height' => '102',
					        'id' => 'par.php?modulo=principal/acompanhamento/validacaoExecucao&acao=A'),
					);

$perfil = pegaPerfilGeral();
if( in_array(PAR_PERFIL_SUPER_USUARIO,$perfil))
{
	$param['icones'][] = Array('url' => '../imagens/par/cancelar-processo.png',
			'titulo' => 'Cancelar Processo',
			'class' => 'ferramenta',
			'height' => '102',
			'id' => 'par.php?modulo=principal/cancelarProcesso&acao=A');
	$param['icones'][] = Array('url' => '../imagens/par/acompanhamento-consolidado.png',
			'titulo' => 'Acompanhamento Consolidado',
			'class' => 'ferramenta',
			'height' => '102',
			'id' => 'par.php?modulo=principal/acompanhamento/ferramentas/acompanhamentoConsolidado&acao=A');
}
/*
$param['icones'][] = Array('url' => '../imagens/par/acwessanhamento-consolidado.png',
		'titulo' => 'Acompanhamento Adesões SIGARP',
		'class' => 'ferramenta',
		'height' => '102',
		'id' => 'par.php?modulo=principal/acompanhamento/ferramentas/acompanhamentoAdesaoSIGARP&acao=A');*/
$param['icones'][] = Array('url' => '../imagens/par/acompanhamento-pagamento-desembolso.png',
		'titulo' => 'Pagamento Desembolso',
		'class' => 'ferramenta',
		'height' => '102',
		'id' => 'par.php?modulo=principal/acompanhamento/ferramentas/pagamentoDesembolso&acao=A');
?>
            <?php desenha_slideShow($param); ?>
        </td>
    </tr>
</table>
<div id="janela"></div>