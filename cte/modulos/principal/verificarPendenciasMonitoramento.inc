<?php

$inuid = $_GET['inuid'];
$perid = $_GET["perid"];
$nrAno = $_GET["nrAno"];

$sql = "select min( perdtinicioref ) as inicio, max( perdtterminoref ) as final from cte.periodoreferencia";
$arVigencia = $db->carregar( $sql );
$arVigencia = $arVigencia ? $arVigencia : array();
$nrAnoFim = substr( $arVigencia[0]["final"], 0, 4 );

$sql = "select perid, perdtinicioref, perdtterminoref, perdsc from cte.periodoreferencia where perid = $perid";

$arPeriodo = $db->pegaLinha( $sql );
$arPeriodo = $arPeriodo ? $arPeriodo : array();

$nrAnoFim = $nrAnoFim == '2011' ? '2010' : $nrAnoFim;

$arExecutado  = recuperarQuantidadeSubacoesVigentesPorPeriodo( $arPeriodo, $nrAnoFim, true, $inuid,  $arPeriodo['perdtterminoref']);
$arCronograma = recuperarQuantidadeSubacoesVigentesPorPeriodo( $arPeriodo, $nrAnoFim, false, $inuid );

$arExecutado  = $arExecutado  ? $arExecutado  : array();
$arCronograma = $arCronograma ? $arCronograma : array();

$arExecutados = array();
foreach( $arExecutado as $arMntid ){
	$arExecutados[] = $arMntid['mntid'];
}

$arCronogramas = array();
foreach( $arCronograma as $arMntid ){
	$arCronogramas[] = $arMntid['mntid'];
}

$arPendencias = array_diff( $arCronogramas, $arExecutados);

$arMonitoramentos = array();
if( count($arPendencias) ){
	$sql = "select d.dimcod, d.dimdsc, ad.ardcod, ad.arddsc, i.indcod, i.inddsc, s.sbaid, s.sbadsc, s.sbaordem, m.mntid
			FROM cte.monitoramentosubacao m
				INNER  JOIN cte.subacaoindicador s on s.sbaid = m.sbaid
				INNER  JOIN cte.acaoindicador a on a.aciid = s.aciid
				INNER  JOIN cte.pontuacao p on p.ptoid = a.ptoid
				INNER  JOIN cte.indicador i on i.indid = p.indid
				INNER JOIN cte.areadimensao ad on ad.ardid = i.ardid
				INNER JOIN cte.dimensao d on d.dimid = ad.dimid
			where m.mntid in ( '" . implode( "', '", $arPendencias ) . "' )
			order by d.dimcod, ad.ardcod, i.indcod, s.sbaordem";
	
	$arMonitoramentos = $db->carregar( $sql );
}
$arMonitoramentos = $arMonitoramentos ? $arMonitoramentos : array();

?>

<html>
	<head>
		<title>Verificação de pendências em subações</title>
		<link rel="stylesheet" type="text/css" href="/includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="/includes/listagem.css">
		<script>
			function alterarMonitoramento(mntid, nrAno, perid){
				var janela=window.open("/cte/cte.php?modulo=principal/monitora_dados&acao=A&_page=execucao&mntid="+mntid+"&perano="+nrAno+"&perid="+perid,"detalhesSubacao","height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes");
				janela.focus();
			}
		</script>
	</head>
	<body>
		<div >
			<table class="tabela">
				<tr>
					<td style="text-align:center;font-size:14px;font-weight:bold;color:#000">
						<h1><?php echo $arPeriodo['perdsc']; ?></h1>
					</td>
				</tr>
				<tr>
					<td style="text-align:center;font-size:14px;font-weight:bold;color:#900">
						O sistema verificou que alguns dados do monitoramento não foram preenchidos
					</td>
				</tr>
				<tr>
					<td style="text-align:center;font-size:14px;font-weight:bold;color:#900">
						Pendências de preenchimento:<br/><span style="font-weight:normal;"><?php echo count($arPendencias); ?> registro(s) encontrado(s)</span>
					</td>
		        </tr>
		        <?php foreach( $arMonitoramentos as $arMonitoramento ){ ?>
					<tr style="background-color: #d9d9d9;">
		            	<td>
		            		<span style="cursor:pointer;" onclick="alterarMonitoramento('<?php echo $arMonitoramento['mntid']; ?>', '<?php echo $nrAno; ?>', '<?php echo $perid; ?>')">
			            		<img style="padding-right:5px;" src="/imagens/consultar.gif" />
								<?php echo $arMonitoramento['dimcod'] . ' - ' . $arMonitoramento['ardcod'] . ' - ' . $arMonitoramento['indcod'] . '(' . $arMonitoramento['sbaordem'] . ') . ' . $arMonitoramento['sbadsc']; ?>
							</span>
						</td>
					</tr>
		        <?php } ?>
			</table>
		</div>

	</body>
</html>