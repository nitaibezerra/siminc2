<?php

/************************* INCLUDES **********************************/
include_once(APPRAIZ.'www/cte/monitoraFinanceiro/includes/constantes.php');
include_once(APPRAIZ.'includes/classes/dateTime.inc');
include_once(APPRAIZ.'cte/modulos/principal/monitoraFinanceiro/funcoes.inc');
include_once(APPRAIZ .'cte/classes/HistoricoMonitoramentoDadosBancarios.class.inc');
include_once(APPRAIZ .'cte/classes/HistoricoMonitoramentoConvenio.class.inc');
include_once(APPRAIZ .'cte/classes/HistoricoMonitoramentoConvSubac.class.inc');
include_once(APPRAIZ .'cte/classes/HistoricoConvItemComposicao.class.inc');

/************************* INSTANCIANDO OBJETOS *************************/
global $db;
$meses 				= new HistoricoMonitoramentoDadosBancarios();

/************************* CABEÇALHO E TÍTULO ******************************/
$url = "?modulo=principal/monitoraFinanceiro/estrutura_mes&acao=A&anoreferencia=".$_SESSION['ano'];
cteMontaTituloFinanceiro( 'Monitoramento Mês Convênio', '&nbsp;', $url );
?>
<html>
	<head>
		<link rel="stylesheet" type="text/css" href="../../cte/monitoraFinanceiro/includes/monitora_financeiro.css"/>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
			
		<script type="text/javascript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="../../cte/monitoraFinanceiro/includes/funcoes.js"></script>
		<script type="text/javascript" src="../../includes/prototype.js"></script>
		<script type="text/javascript" src="../../includes/LyteBox/lytebox.js"></script>
		
		<script type="text/javascript" src="../../includes/remedial.js"></script>
		<script type="text/javascript" src="../../includes/superTitle.js"></script>
		
		<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>
		<link rel="stylesheet" type="text/css" href="../../cte/monitoraFinanceiro/includes/monitora_financeiro.css"/>
		<link rel="stylesheet" type="text/css" href="../../includes/LyteBox/lytebox.css"/>
		<!--[if IE]>
			<link rel="stylesheet" type="text/css" href="../../cte/monitoraFinanceiro/includes/monitora_financeiro_ie.css"/>
		<![endif]-->
	</head>
	<body>
	    <!-- CARREGA DADOS DO MONITORAMENTO -->
	    <table align="center" border="0" class="tabela tabelaMonitoramento" cellpadding="3" cellspacing="1" width="95%">
			<tr>
				<td class="textoEsquerda">
					<?php 
					//CARREGA MESES
					$conteudo 	= $meses->carregaMeses($_REQUEST['prsid'], $_SESSION['ano'], false);
					echo $conteudo;
					?>
				</td>
			</tr>
		</table>
	</body>
</html>
