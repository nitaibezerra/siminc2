<?php

/************************* INCLUDES **********************************/
include_once(APPRAIZ.'www/cte/monitoraFinanceiro/includes/constantes.php');
include_once(APPRAIZ.'includes/classes/dateTime.inc');
include_once(APPRAIZ.'cte/modulos/principal/monitoraFinanceiro/funcoes.inc');
include_once(APPRAIZ .'cte/classes/HistoricoMonitoramentoDadosBancarios.class.inc');
include_once(APPRAIZ .'cte/classes/HistoricoMonitoramentoConvenio.class.inc');
include_once(APPRAIZ .'cte/classes/HistoricoMonitoramentoConvSubac.class.inc');
include_once(APPRAIZ .'cte/classes/HistoricoConvItemComposicao.class.inc');

/************************* CARREGA MONITORAMENTO DE ACORDO COM O MÊS. *************************/
$sql = "SELECT 	hm.hmdmes, 
				hm.hmdano,
				hm.hmdid, 
				hc.hmcstatus,
				ps.prsnumconvsape,
				CASE WHEN hc.hmcstatus = 'I' THEN 'Em andamento.' ELSE 'Finalizado.' END AS hmcstatusdescricao
		FROM cte.historicomonitoramentoconvenio hc 
		INNER JOIN cte.historicomonitoramentodadosbancarios hm ON hm.hmdid = hc.hmdid
		INNER JOIN cte.projetosape ps ON ps.prsid = hc.prsid
		WHERE hc.hmcid = ". $_REQUEST['hmcid']." AND hc.prsid = ". $_REQUEST['prsid'];

$monitoraMes 	= $db->pegaLinha($sql);
$mes 		 	= $monitoraMes['hmdmes'];
$ano 		 	= $monitoraMes['hmdano'];
$hmdid 		 	= $monitoraMes['hmdid'];
$status 	 	= $monitoraMes['hmcstatus'];
$numConvenio 	= $monitoraMes['prsnumconvsape'];
$descStatus  	= $monitoraMes['hmcstatusdescricao'];
$display 	 	= '';
$tudoMonitorado = false;

/************************* CABEÇALHO E TÍTULO ******************************/
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
$url = "?modulo=principal/monitoraFinanceiro/estrutura_mes&acao=A&anoreferencia=".$_SESSION['ano'];
cteMontaTituloFinanceiro( 'Total Subações', '&nbsp;', $url );
?>
<html>
	<head>
		<script type="text/javascript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="../../includes/prototype.js"></script>
		<script type="text/javascript" src="../../includes/LyteBox/lytebox.js"></script>
		<script type="text/javascript" src="../../cte/monitoraFinanceiro/includes/funcoes.js"></script>
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
				<td  class="tdTopo" style="text-align: left;" >Monitoramento do mês: <?=$mesTexto; ?> / <?=$ano; ?> do convênio n°: <?=$numConvenio;?>  </td>
			</tr>
			<tr>
				<td width="170px;" class="tdTopo" style="text-align: left;" >Status deste monitoramento: <?php echo $descStatus; ?></td>
			</tr>
			<tr>
				<td class="textoEsquerda">
					<?php 
						//CARREGA SUBAÇÕES
						$convSub = new HistoricoMonitoramentoConvSubac();
						$arSubacao = $convSub->carregaSubacoesResumo($_REQUEST['prsid'], $_REQUEST['hmcid']);
						?>

						<table align="center" border="0" class="tabela tabelaMonitoramento" cellpadding="3" cellspacing="1">
							<tr>
								<th width="40%">Descrição da Subações</th>
								<th width="8%">Mês / Ano de repasse</th>
								<th width="8%" colspan="2" >Status da <br> subação</th>
								<th width="8%">Qtd. Itens pendentes</th>
								<th width="12%">Total Programado</th>
								<th width="12%">Total Executado Acumulado</th>
							</tr>
							<?php 
							
//							ver( $arSubacao, d );

							$nrTotalProgramado = 0;
							$nrTotalAcumulado = 0;
							foreach($arSubacao as $subacao){ 

								if($subacao["ssuid"] == STATUSNAOSELECIONADO){
									$imagem =  "../../imagens/atencao.png";
									$descricaoStatusSub = "Pendente";
									$subacao["ssuid"] = 0;
								}else{
									$imagem = "../../imagens/check_p.gif";
									$descricaoStatusSub = $subacao['ssudescricao'];
								}
								
								$dadosItens 		= $convSub->itensComPendencia($subacao["sbaid"], $subacao['pssano'], $subacao['hmcid']);
								$itensFaltam 		= $dadosItens['faltam'];
								$itensTotal 		= $dadosItens['total'];
								$itensMonitorados 	= $dadosItens['monitorados'];								
								
								?>
								<tr class="SubTituloEsquerda">
									<td><?php echo $subacao['sbadsc']; ?></td>
									<td align="center"><?php echo $subacao['pssmes'] . '/' . $subacao['pssano']; ?></td>
									<td><?php echo $descricaoStatusSub; ?></td>
									<td align="center"><img align="absmiddle"  src="<?php echo $imagem; ?>" width="18" height="18" alt="Situação"></td>
									<td align="center"><?php echo $itensFaltam .' de '. $itensTotal; ?></td>
									<td align="right"><?php echo 'R$ ' . number_format($subacao['totalprogramado'], 2, ',', '.'); ?></td>
									<td align="right"><?php echo 'R$ ' . number_format($subacao['totalacumulado'], 2, ',', '.'); ?></td>
								</tr>
							<?php 
								$nrTotalProgramado += $subacao['totalprogramado'];
								$nrTotalAcumulado += $subacao['totalacumulado'];
							} ?>
							<tr class="SubTituloEsquerda">
								<th colspan="5" style="text-align: left !important;">Total:</th>
								<th style="text-align: right !important;"><?php echo 'R$ ' . number_format($nrTotalProgramado, 2, ',', '.'); ?></th>
								<th style="text-align: right !important;"><?php echo 'R$ ' . number_format($nrTotalAcumulado, 2, ',', '.'); ?></th>
							</tr>
				</td>
			</tr>
			<tr>
				<td style="text-align: center;">
					<input type="button" value="imprimir" onClick="javascript:window.print()" id="botao" />
				</td>
			</tr>
		</table>
	</body>
</html>
