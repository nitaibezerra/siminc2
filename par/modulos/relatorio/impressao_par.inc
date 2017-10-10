<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css' />
<script type="text/javascript" src="../includes/funcoes.js"></script>
<?php

ini_set("memory_limit","2500M");
set_time_limit(0);

$var123 = "true";

if (!$_SESSION['par']['inuid']){
	echo "<script>
	alert('A sessão expirou.');
	window.opener.location.href = '/par/par.php?modulo=principal/listaMunicipios&acao=A';
	window.close();
	</script>";
	die;
}
else
{
	print '<br/>';
	
	$status = $_REQUEST['status'] ? $_REQUEST['status'] : 'A';
	$ptostatus = isset( $_REQUEST['ptostatus'] ) ? $_REQUEST['ptostatus'] : 'A';
	$sql = "SELECT
				d.dimid,
				d.dimcod || ' - ' || d.dimdsc as dimensao,
				area.arecod || ' - ' || area.aredsc as area,
				area.areid,
				i.indcod || ' - ' || i.inddsc as indicador,
				i.indid,
				i.indcod,
				c.crtpontuacao as pontuacao,
				p.ptojustificativa,
				p.ptodemandamunicipal,
				p.ptodemandaestadual,
				CASE WHEN ".$_SESSION['par']['itrid']." = 1
					THEN 'Estadual'
					ELSE 'Municipal'
				END as Tipo,
				acao.aciid,
				acao.ptoid,
				acao.acidsc,
				acao.acinomeresponsavel as acirpns,
				acao.acicargoresponsavel as acicrg,
				to_char(acao.acidata,'dd/mm/yyyy') as acidtinicial, --coloquei a mesma pras 2
				to_char(acao.acidata,'dd/mm/yyyy') as acidtfinal,
				acao.aciresultadoesperado as acirstd,
				'' as acilocalizador,--acao.acilocalizador,
				subacao.sbaid,
				subacao.sbacronograma,
				Case subacao.sbacronograma
					when 2 then 'Escola'
					else 'Global'
				end as sbaporescola,
				prg.prgdsc as sbaprm,
				coalesce(u.unddsc,'') as unddsc,
				coalesce(f.frmdsc,'') as frmdsc,
				c.crtdsc,
				c.crtpontuacao,
				f.frmid,
				subacao.sbadsc,
				COALESCE(subacao.sbaestrategiaimplementacao,'N/A') as sbastgmpl,
				'' as sbauntdsc,--subacao.sbauntdsc,
				length(p.ptodemandaestadual) as demandaestadual,
				length(p.ptodemandamunicipal) as demandamunicipal
			FROM
				par.dimensao d
			INNER JOIN par.area area ON area.dimid = d.dimid
			INNER JOIN par.indicador i ON i.areid = area.areid
			INNER JOIN par.criterio c ON c.indid = i.indid AND c.crtstatus = 'A'
			INNER JOIN par.pontuacao p ON p.crtid = c.crtid and p.ptostatus = '$status'
			INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
			INNER JOIN par.acao acao ON acao.ptoid = p.ptoid AND acao.acistatus = '$status'
			INNER JOIN par.subacao subacao ON subacao.aciid = acao.aciid AND subacao.sbastatus = '$status'
			LEFT  JOIN par.unidademedida u ON u.undid = subacao.undid
			LEFT  JOIN par.formaexecucao f ON f.frmid = subacao.frmid
			LEFT  JOIN par.programa prg ON prg.prgid = subacao.prgid
			WHERE
				iu.inuid = '".$_SESSION['par']['inuid']."'
			ORDER BY
				d.dimcod, area.arecod, i.indcod, p.ptoid, acao.aciid, subacao.sbadsc";

	$dado = $db->carregar($sql);
	$i= 0;
	$totalreg = count($dado);

	$novaDimensao = $dado[$i]['dimid'];
	$novaArea = $dado[$i]['areid'];
	$novoIndicador = $dado[$i]['indid'];
	$novaAcao = $dado[$i]['aciid'];
	$novasubAcao = $dado[$i]['sbaid'];

	$totalGeralAno0 = 0;
	$totalGeralAno1 = 0;
	$totalGeralAno2 = 0;
	$totalGeralAno3 = 0;
	$demanda 		= 0;

	$muncod = $_SESSION['par']['muncod'];
	$descricao = recuperaMunicipioEstado();
}
?>


<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<?php if( !isset( $cabecalhoImprecao ) || $cabecalhoImprecao !== false ): ?>

	<thead>
		<th class="tdDegradde01" colspan="2" align="left">
		<?
			if( $_SESSION['par']['itrid'] == 2 ){
				$desc = 'Município';
			} else {
				$desc = 'Estado';
			}
		?>
			<h1 class="notprint">
				PAR do
				<?=$desc ?>
				:
				<?= $descricao; ?>
			</h1>
			<div id="noprint">
			<style>
			@media print {
				#noprint {
					display: none;
				}
				.right {
					position: absolute;
					right: 0px;
					width: 300px;
				}
			}
			</style>

				<table cellspacing="0" cellpadding="3" border="0"
					style="width: 80px;">

					<tr style="text-align: center;">
						<td style="font-size: 7pt; text-align: center;"><a
							onclick="window.print(); return false;" title="Versão para impressão"
							style="cursor: pointer;"> <img title="Versão para impressão" src="/imagens/print.png">&nbsp;Imprimir PAR

						</a></td>
					</tr>
				</table>
			</div>

			<table width="100%" bgcolor="#ffffff" border="0" cellpadding="0"
				cellspacing="0" class="notscreen">
				<tr>
					<td><img src="../imagens/brasao.gif" width="50" height="50"
						border="0"></td>
					<td height="20" nowrap><b><?php echo SIGLA_SISTEMA; ?></b>- Sistema Integrado de
						Ministério da Educação<br> Ministério da Educação / SE -
						Secretaria Executiva<br> <b>.:: PAR Analítico do <?=$desc ?>: <?= $descricao; ?>
					</b><br>
					</td>
					<td height="20" align="right">Impresso por: <strong><?= $_SESSION['usunome']; ?>
					</strong><br> <!--Órgão: <?= $_SESSION['usuorgao']; ?><br> --> Hora
						da Impressão: <?= date("d/m/Y - H:i:s") ?>
					</td>
				</tr>
				<tr>
					<td colspan="2">&nbsp;</td>
				</tr>

			</table>
	
	</thead>
	<?php endif; ?>
	<? 
	while ( $i < $totalreg ) {

		$dimensao = $dado[$i]['dimid'];
		?>

	<tr>
		<td class="tdDivisaoItens" colspan="2"></td>
	</tr>
	
	
	<tr>
		<td class="SubTituloDireita tdDegradde02"><b>Dimensão</b>
		</td>
		<td class="tdDegradde02" style="text-align: left"><?=$dado[$i]['dimensao']; ?>
		</td>
	</tr>
	<?
	while ($dimensao == $novaDimensao){

		if($i >= $totalreg ) break;

		$area = $dado[$i]['areid'];

		?>
	<tr>
		<td class="SubTituloDireita tdDegradde03"><b>Área</b>
		</td>
		<td class="tdDegradde03" style="text-align: left"><?=$dado[$i]['area']; ?>
		</td>
	</tr>
	<?
	while ($area == $novaArea )
	{
		if($i >= $totalreg ) break;

		$indicador = $dado[$i]['indid'];
			
		while ($indicador == $novoIndicador )
		{
			if($i >= $totalreg ) break;
			?>
	<tr>
		<td class="SubTituloDireita tdDegradde04"><b>Indicador</b>
		</td>
		<td class="tdDegradde04" style="text-align: left"><?=$dado[$i]['indicador']; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita tdDegradde05"><b>Critério / Pontuação</b>
		</td>
		<td class="tdDegradde05" style="text-align: left"><?echo $dado[$i]['pontuacao'] . ' - '.$dado[$i]['crtdsc']; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"><b>Justificativa</b>
		</td>
		<td class="" style="text-align: left"><?echo $dado[$i]['ptojustificativa']; ?>
		</td>
	</tr>
	<?if( $dado[$i]['ptodemandaestadual']){
		?>
	<tr>
		<td class="SubTituloDireita"><b>Demanda para Rede Estadual</b>
		</td>
		<td class="" style="text-align: left"><?echo $dado[$i]['ptodemandaestadual']; ?>
		</td>
	</tr>
	<?}



	?>
	<?if($dado[$i]['ptodemandamunicipal']){
		?>
	<tr>
		<td class="SubTituloDireita"><b>Demanda para Redes Municipais</b>
		</td>
		<td class="" style="text-align: left"><?echo $dado[$i]['ptodemandamunicipal']; ?>
		</td>
	</tr>
	<?
	}
	$acao = $dado[$i]['aciid'];
	while ($acao == $novaAcao && $acao != '')
	{
		if($i >= $totalreg ) break;
		
		$sql = "select 
					MIN(sbd.sbdano || '-' || sbd.sbdinicio || '-01' ) as datainicio,
					MAX(sbd.sbdano || '-' || sbd.sbdfim || '-30' ) as datafim
				from par.subacaodetalhe sbd
				inner join par.subacao s on s.sbaid = sbd.sbaid
				WHERE 
					s.aciid = ".$dado[$i]['aciid'];

		$data = $db->pegaLinha( $sql );
		?>
	<tr>
		<td class="SubTituloDireita"><b>Ação</b>
		</td>
		<td class="" style="text-align: left">
			<table class="listagem" width="100%" bgcolor="#f5f5f5"
				cellSpacing="1" cellPadding="3" align="left">
				<tr>
					<td class="SubTituloDireita" style="width: 20%;">Demanda:</td>

					<td><?=$dado[$i]['tipo'];?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Descrição da Ação:</td>

					<td><?=$dado[$i]['acidsc']; ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Nome do Responsável:</td>

					<td><?=$dado[$i]['acirpns']; ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Cargo do Responsável:</td>

					<td><?=$dado[$i]['acicrg']; ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Período Inicial:</td>

					<td><?=formata_data($data['datainicio']); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Período Final:</td>

					<td><?=formata_data($data['datafim']); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Resultado Esperado:</td>

					<td><?=$dado[$i]['acirstd']; ?>
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<?
	$subacao 	= $dado[$i]['sbaid'];
	$tipo 		= $dado[$i]['tipo'];
	$demandaE 	= $dado[$i]["demandaestadual"];
	$demandaM 	= $dado[$i]["demandamunicipal"];

	while ($acao == $novaAcao ){

		mostra_subacao();
		if($i >= $totalreg ) break;
	}
	}
		}
		?>
	<tr>
		<td class="SubTituloDireita"><b>Total Geral por Indicador</b>
		</td>
		<td>
			<table class="listagem" width="100%">
				<thead>
					<th align="center"><b>2011</b>
					</th>
					<th align="center"><b>2012</b>
					</th>
					<th align="center"><b>2013</b>
					</th>
					<th align="center"><b>2014</b>
					</th>
					<th align="center"><b>Total</b>
					</th>
				</thead>
				<tr>
					<td align="right"><?=number_format($totalGeralIndicadorAno0,2,',','.');?>
					</td>
					<td align="right"><?=number_format($totalGeralIndicadorAno1,2,',','.');?>
					</td>
					<td align="right"><?=number_format($totalGeralIndicadorAno2,2,',','.');?>
					</td>
					<td align="right"><?=number_format($totalGeralIndicadorAno3,2,',','.');?>
					</td>
					<td align="right"><?=number_format($totalGeralIndicadorAno0 + $totalGeralIndicadorAno1 + $totalGeralIndicadorAno2 + $totalGeralIndicadorAno3 ,2,',','.');?>
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<?
	$totalGeralIndicadorAno0 = 0;
	$totalGeralIndicadorAno1 = 0;
	$totalGeralIndicadorAno2 = 0;
	$totalGeralIndicadorAno3 = 0;

	}
	?>
	<tr>
		<td class="SubTituloDireita"><b>Total Geral por Área</b>
		</td>
		<td>
			<table class="listagem" width="100%">
				<thead>
					<th align="center"><b>2011</b>
					</th>
					<th align="center"><b>2012</b>
					</th>
					<th align="center"><b>2013</b>
					</th>
					<th align="center"><b>2014</b>
					</th>
					<th align="center"><b>Total</b>
					</th>
				</thead>
				<tr>
					<td align="right"><?=number_format($totalGeralAreaAno0,2,',','.');?>
					</td>
					<td align="right"><?=number_format($totalGeralAreaAno1,2,',','.');?>
					</td>
					<td align="right"><?=number_format($totalGeralAreaAno2,2,',','.');?>
					</td>
					<td align="right"><?=number_format($totalGeralAreaAno3,2,',','.');?>
					</td>
					<td align="right"><?=number_format($totalGeralAreaAno0 + $totalGeralAreaAno1 + $totalGeralAreaAno2 + $totalGeralAreaAno3,2,',','.');?>
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<?
	$totalGeralAreaAno0 = 0;
	$totalGeralAreaAno1 = 0;
	$totalGeralAreaAno2 = 0;
	$totalGeralAreaAno3 = 0;

	}
	?>
	<tr>
		<td class="SubTituloDireita"><b>Total Geral por Dimensão</b>
		</td>
		<td>
			<table class="listagem" width="100%">
				<thead>
					<th align="center"><b>2011</b>
					</th>
					<th align="center"><b>2012</b>
					</th>
					<th align="center"><b>2013</b>
					</th>
					<th align="center"><b>2014</b>
					</th>
					<th align="center"><b>Total</b>
					</th>
				</thead>
				<tr>
					<td align="right"><?=number_format($totalGeralDimensaAno0,2,',','.');?>
					</td>
					<td align="right"><?=number_format($totalGeralDimensaAno1,2,',','.');?>
					</td>
					<td align="right"><?=number_format($totalGeralDimensaAno2,2,',','.');?>
					</td>
					<td align="right"><?=number_format($totalGeralDimensaAno3,2,',','.');?>
					</td>
					<td align="right"><?=number_format($totalGeralDimensaAno0 + $totalGeralDimensaAno1 + $totalGeralDimensaAno2 + $totalGeralDimensaAno3,2,',','.');?>
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<?
	$totalGeralDimensaAno0 = 0;
	$totalGeralDimensaAno1 = 0;
	$totalGeralDimensaAno2 = 0;
	$totalGeralDimensaAno3 = 0;


	}?>
	<tr>
		<td class="SubTituloDireita" colspan="2" style="text-align: center;">
			<b>Total Geral</b>
		</td>
	</tr>
	<tr>
		<td colspan="2">
			<table class="listagem" width="100%">
				<thead>
					<th align="center"><b>2011</b>
					</th>
					<th align="center"><b>2012</b>
					</th>
					<th align="center"><b>2013</b>
					</th>
					<th align="center"><b>2014</b>
					</th>
					<th align="center"><b>Total</b>
					</th>
				</thead>
				<tr>
					<td align="right"><?=number_format($totalGeralAno0,2,',','.');?>
					</td>
					<td align="right"><?=number_format($totalGeralAno1,2,',','.');?>
					</td>
					<td align="right"><?=number_format($totalGeralAno2,2,',','.');?>
					</td>
					<td align="right"><?=number_format($totalGeralAno3,2,',','.');?>
					</td>
					<td align="right"><?=number_format($totalGeralAno0 + $totalGeralAno1 + $totalGeralAno2 + $totalGeralAno3,2,',','.');?>
					</td>
				</tr>
			</table>
		</td>
	</tr>
</table>


<?
function recuperaQuantidades($sbaid, $cronograma){
	global $db;

	if($cronograma == 'Escola'){
		$sqlQuantidade="SELECT
		sum(ano2011) AS ano2011, -- QUANTIDADE 2011
		sum(ano2012) AS ano2012, -- QUANTIDADE 2012
		sum(ano2013) AS ano2013, -- QUANTIDADE 2013
		sum(ano2014) AS ano2014, -- QUANTIDADE 2014
		sum(qfaqtd)  AS qfaqtd
		FROM (
		SELECT
		CASE WHEN sesano = '2011'  THEN sum(sesquantidade) END AS ano2011, -- QUANTIDADE 2011
		CASE WHEN sesano = '2012'  THEN sum(sesquantidade) END AS ano2012, -- QUANTIDADE 2012
		CASE WHEN sesano = '2013'  THEN sum(sesquantidade) END AS ano2013, -- QUANTIDADE 2013
		CASE WHEN sesano = '2014'  THEN sum(sesquantidade) END AS ano2014, -- QUANTIDADE 2014
		sum(sesquantidade) as qfaqtd
		FROM par.subacaoescolas
		WHERE sbaid = '".$sbaid."'
		GROUP BY sesano
		) AS quantidade";

		$quantidade = $db->carregar($sqlQuantidade);
		return $quantidade;
	}else{
		                              
		 $sqlQuantidade="SELECT
		sum(ano2011) AS ano2011, -- QUANTIDADE 2011
		sum(ano2012) AS ano2012, -- QUANTIDADE 2012
		sum(ano2013) AS ano2013, -- QUANTIDADE 2013
		sum(ano2014) AS ano2014, -- QUANTIDADE 2014
		sum(qfaqtd)  AS qfaqtd
		FROM (
		SELECT
		CASE WHEN sbdano = '2011'  THEN sum(sbdquantidade) END AS ano2011, -- QUANTIDADE 2011
		CASE WHEN sbdano = '2012'  THEN sum(sbdquantidade) END AS ano2012, -- QUANTIDADE 2012
		CASE WHEN sbdano = '2013'  THEN sum(sbdquantidade) END AS ano2013, -- QUANTIDADE 2013
		CASE WHEN sbdano = '2014'  THEN sum(sbdquantidade) END AS ano2014, -- QUANTIDADE 2014
		sum(sbdquantidade) as qfaqtd
		FROM par.subacaodetalhe
		WHERE sbaid = '".$sbaid."'
		GROUP BY sbdano
		) AS quantidade";

		$quantidade = $db->carregar($sqlQuantidade);
		return $quantidade;
	}
}

function racuperaValor($sbaid){
	global $db;
	if($sbaid){
		$sql = 'SELECT
		sum(ano2011)  AS ano2011,
		sum(ano2012)  AS ano2012,
		sum(ano2013)  AS ano2013,
		sum(ano2014)  AS ano2014
		FROM (
		SELECT
		CASE WHEN icoano = 2011 THEN (icoquantidade * icovalor)  END AS ano2011,
		CASE WHEN icoano = 2012 THEN (icoquantidade * icovalor)  END AS ano2012,
		CASE WHEN icoano = 2013 THEN (icoquantidade * icovalor)  END AS ano2013,
		CASE WHEN icoano = 2014 THEN (icoquantidade * icovalor)  END AS ano2014
		FROM par.subacaoitenscomposicao where sbaid = ' . $sbaid . ') AS valores';
			
		return (array) $db->carregar($sql);
	}else{
		return false;
	}

}


function recuperaDadosSub($sbaid){
	global $db;

	$sql="SELECT
	sum(sba0ini) AS sba0ini,
	sum(sba1ini) AS sba1ini,
	sum(sba2ini) AS sba2ini,
	sum(sba3ini) AS sba3ini,

	sum(sba0fim) AS sba0fim,
	sum(sba1fim) AS sba1fim,
	sum(sba2fim) AS sba2fim,
	sum(sba3fim) AS sba3fim

	FROM (
	SELECT
	CASE WHEN subp.sbdano = '2011'  THEN coalesce( subp.sbdinicio , 0 )END AS sba0ini,
	CASE WHEN subp.sbdano = '2012'  THEN coalesce( subp.sbdinicio , 0 )END AS sba1ini,
	CASE WHEN subp.sbdano = '2013'  THEN coalesce( subp.sbdinicio , 0 )END AS sba2ini,
	CASE WHEN subp.sbdano = '2014'  THEN coalesce( subp.sbdinicio , 0 )END AS sba3ini,

	CASE WHEN subp.sbdano = '2011'  THEN coalesce( subp.sbdfim , 0 )END AS sba0fim,
	CASE WHEN subp.sbdano = '2012'  THEN coalesce( subp.sbdfim , 0 )END AS sba1fim,
	CASE WHEN subp.sbdano = '2013'  THEN coalesce( subp.sbdfim , 0 )END AS sba2fim,
	CASE WHEN subp.sbdano = '2014'  THEN coalesce( subp.sbdfim , 0 )END AS sba3fim
	FROM
	par.subacao subacao
	LEFT JOIN par.subacaodetalhe subp ON subp.sbaid = subacao.sbaid
	WHERE	subacao.sbastatus = 'A' AND subacao.sbaid = '".$sbaid."'
	) AS dados";

	return $db->carregar($sql);

}

function mostra_subacao(){

	global $db;
	global $dado, $i, $totalreg;
	global $total, $totalGeralAno0 ,$totalGeralAno1, $totalGeralAno2, $totalGeralAno3, $totalGeralAno4;
	global $totalGeralIndicadorAno0 , $totalGeralIndicadorAno1, $totalGeralIndicadorAno2, $totalGeralIndicadorAno3, $totalGeralIndicadorAno4;
	global $totalGeralAreaAno0 , $totalGeralAreaAno1, $totalGeralAreaAno2, $totalGeralAreaAno3, $totalGeralAreaAno4;
	global $totalGeralDimensaAno0 ,$totalGeralDimensaAno1, $totalGeralDimensaAno2, $totalGeralDimensaAno3, $totalGeralDimensaAno4;
	global $novaDimensao, $novaArea, $novoIndicador, $novaAcao, $novasubAcao;
	global $tipo, $demandaE, $demandaM ;


	if( $tipo == "Estadual" ){
		$demanda = $demandaE;
	}else if( $tipo == "Municipal" ){
		$demanda = $demandaM;
	}

	if( $demanda >= 0 ){

		if($novasubAcao != NULL){
			$sub 		= recuperaDadosSub($novasubAcao);
			$valor 		= 0;
			$valor 		= racuperaValor($novasubAcao);
			$quantidade = recuperaQuantidades($novasubAcao, $dado[$i]['sbaporescola']);

			$valor2011 = $valor[0]['ano2011'] / ($quantidade[0]['ano2011'] > 0 ? $quantidade[0]['ano2011'] : 1) ;
			$valor2012 = $valor[0]['ano2012'] / ($quantidade[0]['ano2012'] > 0 ? $quantidade[0]['ano2012'] : 1);
			$valor2013 = $valor[0]['ano2013'] / ($quantidade[0]['ano2013'] > 0 ? $quantidade[0]['ano2013'] : 1);
			$valor2014 = $valor[0]['ano2014'] / ($quantidade[0]['ano2014'] > 0 ? $quantidade[0]['ano2014'] : 1);
			$existevalor = $valor2011 + $valor2012 + $valor2013 + $valor2014;

		}

		if($dado[$i]['sbaporescola'] == 'Escola'){
			$valorGeral = $quantidade[0]['qfaqtd'];
		}else{
			$valorGeral = $quantidade[0]['sptunt'];
		}

		for($x=0; $x < 5; $x++){
			switch($sub[0]['sba'.$x.'ini'] ) {
				case '1':
					$sub[0]['sba'.$x.'ini'] = "janeiro";
					break;
				case '2':
					$sub[0]['sba'.$x.'ini'] = "fevereiro";
					break;
				case '3':
					$sub[0]['sba'.$x.'ini'] = "março";
					break;
				case '4':
					$sub[0]['sba'.$x.'ini'] = "abril";
					break;
				case '5':
					$sub[0]['sba'.$x.'ini'] = "maio";
					break;
				case '6':
					$sub[0]['sba'.$x.'ini'] = "junho";
					break;
				case '7':
					$sub[0]['sba'.$x.'ini'] = "julho";
					break;
				case '8':
					$sub[0]['sba'.$x.'ini'] = "agosto";
					break;
				case '9':
					$sub[0]['sba'.$x.'ini'] = "setembro";
					break;
				case '10':
					$sub[0]['sba'.$x.'ini'] = "outubro";
					break;
				case '11':
					$sub[0]['sba'.$x.'ini'] = "novembro";
					break;
				case '12':
					$sub[0]['sba'.$x.'ini'] = "dezembro";
					break;
			}

			switch($sub[0]['sba'.$x.'fim'] ) {
				case '1':
					$sub[0]['sba'.$x.'fim'] = "janeiro";
					break;
				case '2':
					$sub[0]['sba'.$x.'fim'] = "fevereiro";
					break;
				case '3':
					$sub[0]['sba'.$x.'fim'] = "março";
					break;
				case '4':
					$sub[0]['sba'.$x.'fim'] = "abril";
					break;
				case '5':
					$sub[0]['sba'.$x.'fim'] = "maio";
					break;
				case '6':
					$sub[0]['sba'.$x.'fim'] = "junho";
					break;
				case '7':
					$sub[0]['sba'.$x.'fim'] = "julho";
					break;
				case '8':
					$sub[0]['sba'.$x.'fim'] = "agosto";
					break;
				case '9':
					$sub[0]['sba'.$x.'fim'] = "setembro";
					break;
				case '10':
					$sub[0]['sba'.$x.'fim'] = "outubro";
					break;
				case '11':
					$sub[0]['sba'.$x.'fim'] = "novembro";
					break;
				case '12':
					$sub[0]['sba'.$x.'fim'] = "dezembro";
					break;
			}

		}

		?>
<tr>
	<td class="SubTituloDireita"><b>Sub-Ação</b>
	</td>
	<td class="" style="text-align: left">

		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1"
			cellPadding="3" align="left">
			<tr>
				<td class="SubTituloDireita" style="width: 20%;">Descrição da
					Subação:</td>

				<td><?=$dado[$i]['sbadsc']; ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Estratégia de Implementação:</td>

				<td><?=$dado[$i]['sbastgmpl']; ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Programa:</td>

				<td><?=$dado[$i]['sbaprm']; ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Unidade de Medida:</td>

				<td><?=$dado[$i]['unddsc']; ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Forma de Execução</td>

				<td><?=$dado[$i]['frmdsc'];?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Quantidades e Cronograma Físico</td>
				<td>
					<table class="listagem" width="100%">
						<tr>
							<th>&nbsp;</th>
							<th align="center"><b>2011</b>
							</th>
							<th align="center"><b>2012</b>
							</th>
							<th align="center"><b>2013</b>
							</th>
							<th align="center"><b>2014</b>
							</th>
							<th align="center"><b>Total</b>
							</th>
						</tr>
						<tr>
							<td align="right"><b>Quantidades:</b>
							</td>
							<td align="right"><?=number_format($quantidade[0]['ano2011'],0); ?>
							</td>
							<td align="right"><?=number_format($quantidade[0]['ano2012'],0); ?>
							</td>
							<td align="right"><?=number_format($quantidade[0]['ano2013'],0); ?>
							</td>
							<td align="right"><?=number_format($quantidade[0]['ano2014'],0); ?>
							</td>
							<td align="right"><?
							$total =
							$quantidade[0]['ano2011'] +
							$quantidade[0]['ano2012'] +
							$quantidade[0]['ano2013'] +
							$quantidade[0]['ano2014'];
							echo number_format($total,0);
							?>
							</td>
						</tr>
						<tr>
							<td style="width: 20%;" align="right"><b>Cronograma Físico:</b>
							</td>
							<td align="right"><? if(!Empty($sub[0]['sba0ini'])) echo $sub[0]['sba0ini'] . " até " ; ?>
								<?=$sub[0]['sba0fim']; ?>
							</td>
							<td align="right"><? if(!Empty($sub[0]['sba1ini'])) echo $sub[0]['sba1ini'] . " até " ; ?>
								<?=$sub[0]['sba1fim']; ?>
							</td>

							<td align="right"><?if(!Empty($sub[0]['sba2ini'])) echo $sub[0]['sba2ini'] . " até " ; ?>
								<?=$sub[0]['sba2fim']; ?>
							</td>
							<td align="right"><?if(!Empty($sub[0]['sba3ini'])) echo $sub[0]['sba3ini'] . " até " ; ?>
								<?=$sub[0]['sba3fim']; ?>
							</td>
							<td align="right">&nbsp;</td>
						</tr>


						<?php 
						if ( $existevalor > 0 ) {
							$valorUnitarioGeral  = $valor2011 + $valor2012 + $valor2013 + $valor2014;
							?>
						<tr>
							<th align="right"><b></b>
							</th>
							<th align="right"><b>2011</b>
							</th>
							<th align="right"><b>2012</b>
							</th>
							<th align="right"><b>2013</b>
							</th>
							<th align="right"><b>2014</b>
							</th>
							<th><b>Total</b>
							</th>
						</tr>
						<tr>
							<td style="width: 20%;" align="right"><b>Valor Unitário:</b>
							</td>
							<td align="right"><?=number_format($valor2011,2,',','.');?>
							</td>
							<td align="right"><?=number_format($valor2012,2,',','.');?>
							</td>
							<td align="right"><?=number_format($valor2013 ,2,',','.');?>
							</td>
							<td align="right"><?=number_format($valor2014 ,2,',','.');?>
							</td>
							<td align="right"><?=number_format($valorUnitarioGeral ,2,',','.');?>
							</td>

						</tr>
						<tr>
							<th>&nbsp;</th>
							<th align="center"><b>2011</b>
							</th>
							<th align="center"><b>2012</b>
							</th>
							<th align="center"><b>2013</b>
							</th>
							<th align="center"><b>2014</b>
							</th>
							<th align="center"><b>Total</b>
							</th>
						</tr>
						<tr>
							<?
							$ano0 = $valor[0]['ano2011'];
							$ano1 = $valor[0]['ano2012'];
							$ano2 = $valor[0]['ano2013'];
							$ano3 = $valor[0]['ano2014'];

							$total = $ano0 + $ano1 + $ano2 + $ano3;

							$totalGeralAno0 += $ano0;
							$totalGeralAno1 += $ano1;
							$totalGeralAno2 += $ano2;
							$totalGeralAno3 += $ano3;

							$totalGeralIndicadorAno0 += $ano0;
							$totalGeralIndicadorAno1 += $ano1;
							$totalGeralIndicadorAno2 += $ano2;
							$totalGeralIndicadorAno3 += $ano3;

							$totalGeralAreaAno0 += $ano0;
							$totalGeralAreaAno1 += $ano1;
							$totalGeralAreaAno2 += $ano2;
							$totalGeralAreaAno3 += $ano3;

							$totalGeralDimensaAno0 += $ano0;
							$totalGeralDimensaAno1 += $ano1;
							$totalGeralDimensaAno2 += $ano2;
							$totalGeralDimensaAno3 += $ano3;


							?>
							<td style="width: 20%;" align="right"><b>Valores Anuais:</b>
							</td>
							<td align="right"><?=number_format($ano0,2,',','.');?>
							</td>
							<td align="right"><?=number_format($ano1,2,',','.');?>
							</td>
							<td align="right"><?=number_format($ano2,2,',','.');?>
							</td>
							<td align="right"><?=number_format($ano3,2,',','.');?>
							</td>
							<td align="right"><?=number_format($total,2,',','.') ?>
							</td>
						</tr>

						<?php }  ?>
					</table>
				</td>
			</tr>

			<?php 
			if($novasubAcao != NULL){
				$status = $_REQUEST['status'] ? $_REQUEST['status'] : 'A';
				$sqlComposicao = " select sic.sbaid, sic.icoano, sic.unddid, sic.icovalor, sic.picid, s.sbacronograma, sic.icoquantidade, sic.icoid, sic.icodescricao from par.subacaoitenscomposicao sic inner join par.subacao s on s.sbaid = sic.sbaid AND s.sbastatus = '{$status}' where sic.sbaid = ".$novasubAcao;
				if( $detalhamentoComposicao = $db->carregar($sqlComposicao) ){ ?>

			<tr>
				<td colspan="2"
					style="text-align: center; font-weight: bold; background: #ccc;">Detalhamento
					dos Itens de Composição</td>
			</tr>
			<?php 
			foreach( $detalhamentoComposicao as $arDetalhe ){
				$arDetalhes[$arDetalhe["icoano"]][] = $arDetalhe;
			}

			ksort($arDetalhes);

			foreach( $arDetalhes as $nrAno => $arDetalhe ){
				?>
			<tr>
				<td class="SubTituloDireita"><?php echo $nrAno ?></td>
				<td>
					<table class="listagem" width="100%">
						<thead>
							<th>Identificação do Item</th>
							<th>Un. Medida</th>
							<th>Quantidade</th>
							<th>Valor Unitário</th>
							<th>Total</th>
						</thead>
						<?php
						usort( $arDetalhe, "ordenarArray" );

						foreach( $arDetalhe as $arValores ){
							if( $arValores["unddid"] ){
								$sqlUnidadeMedida = " select umidescricao from par.unidademedidadetalhamentoitem where umiid = ". $arValores["unddid"];
								$unidadeMedida = $db->pegaUm( $sqlUnidadeMedida );
							} else {
								$unidadeMedida = '0';
							}
							if( $arValores["icovalor"] == '' ){
								$sqlUnidadeMedida = "select dicvalor from par.detalheitemcomposicao where picid = ". $arValores["picid"];
								$valor = $db->pegaUm( $sqlUnidadeMedida );
							} else {
								$valor = $arValores["icovalor"];
							}
							if( $arValores["sbacronograma"] == 1 ){
								$quantidade = $arValores["icoquantidade"];
							} else {
								$sql = "SELECT
								coalesce(sum(seiqtd), 0) as icoqtd
								FROM
								par.subacaoitenscomposicao sic
								LEFT JOIN
								par.subacaoescolas ses on ses.sbaid = sic.sbaid
								LEFT JOIN
								par.subescolas_subitenscomposicao ssc ON ssc.icoid = sic.icoid AND ssc.sesid = ses.sesid
								WHERE
								sic.icostatus = 'A'
								AND
								sic.sbaid = {$arValores['sbaid']}
								AND sesano = {$arValores['icoano']}
								AND sic.icoid = {$arValores['icoid']}
								GROUP BY
								sic.icoid";
								$quantidade = $db->pegaUm( $sql );
							}
							?>
						<tr>
							<td><?php echo $arValores["icodescricao"] ?></td>
							<td><?php echo $unidadeMedida ?></td>
							<td align="right"><?php echo number_format( $quantidade, 0, '.', '' ) ?>
							</td>
							<td align="right"><?php echo "R$ ". number_format( $valor, 2, ',', '.' ) ?>
							</td>
							<td align="right"><?php echo "R$ ". number_format( $quantidade * $valor, 2, ',', '.' ) ?>
							</td>
						</tr>
						<?php } ?>
					</table>
				</td>
			</tr>
			<?php }  ?>
			<?php }  
			/* //Não tem mais beneficiários
			 $sql2 = "
			SELECT
			sb.sabqtdurbano,
			sb.sabqtdrural,
			b.bendsc,
			(sb.sabqtdurbano+sb.sabqtdrural) as total
			FROM
			par.beneficiario b,
			par.subacaobeneficiario sb
			WHERE
			b.benid = sb.benid AND
			sb.sbaid = ".$dado[$i]['sbaid']."";
			$sbene = $db->carregar($sql2);
			if($sbene){
			?>
			<tr>
			<td class="SubTituloDireita">
			Beneficiário
			</td>
			<td>
			<table class="listagem" width="100%"  cellspacing="1" cellpadding="3" >
			<tr>
			<th>Beneficiario</th>
			<th>Zona Rural</th>
			<th>Zona Urbana</th>
			<th>Total</th>
			</tr>
			<?php
			for ($ax=0; $ax < count($sbene); $ax++):
			?>
			<tr>
			<td><?php echo $sbene[$ax][bendsc]; ?>&nbsp</td>
			<td><?php echo $sbene[$ax][vlrrural]; ?></td>
			<td><?php echo $sbene[$ax][vlrurbano]; ?></td>
			<td><?php echo $sbene[$ax][total]; ?></td>
			</tr>
			<?php
			endfor;
			?>
			</table>
			</td>
			</tr>
			<? } */


			if( $dado[$i]['sbacronograma'] == 2 ){ //Por escola
				$sql = "SELECT
				ent.entnome as nome,
				ent.entcodent as entcodent,
				se.sesano as ano
				FROM
				par.subacaoescolas se
				INNER JOIN
				par.escolas e ON e.escid = se.escid
				INNER JOIN
				entidade.entidade ent ON ent.entid = e.entid
				WHERE
				se.sbaid = ".$dado[$i]['sbaid']." AND
				se.sesstatus = 'A'";
				$sbaescolas = $db->carregar($sql);
				if($sbaescolas){

					foreach( $sbaescolas as $arDetalheEsc ){
						$arDetalhesEsc[$arDetalheEsc["ano"]][] = $arDetalheEsc;
					}

					ksort($arDetalhesEsc);

					?>
			<tr>
				<td colspan="2"
					style="text-align: center; font-weight: bold; background: #ccc;">Detalhamento
					de Escolas</td>
			</tr>
			<?php
			foreach( $arDetalhesEsc as $nrAno => $arEsc ){
				?>
			<tr>
				<td class="SubTituloDireita"><?=$nrAno ?>
				</td>
				<td>
					<table class="listagem" width="100%" cellspacing="1"
						cellpadding="3">
						<tr>
							<th>Escola</th>
							<th>Código INEP</th>
						</tr>
						<?php
						foreach($arEsc as $detalhe){
							?>
						<tr>
							<td><?php echo $detalhe['nome']; ?></td>
							<td><?php echo $detalhe['entcodent']; ?></td>
						</tr>
						<?php
						}
						?>
					</table>
				</td>
			</tr>
			<? } 
				}
			}
			}?>


		</table>
	</td>
</tr>

<?	
	}
	$i++;
	$demanda = 0;
	$novaDimensao = $dado[$i]['dimid'];
	$novaArea = $dado[$i]['areid'];
	$novoIndicador = $dado[$i]['indid'];
	$novaAcao = $dado[$i]['aciid'];
	$novasubAcao = $dado[$i]['sbaid'];

}

function ordenarArray( $a, $b ){
	return strcmp( strtolower( $a["icodescricao"] ), strtolower( $b["icodescricao"] ) );
}

?>
