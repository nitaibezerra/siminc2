<?php


function popUpMunicipio($dados) {
	global $db;
	echo '<script language="JavaScript" src="../includes/funcoes.js"></script>';
	echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>";
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	
	$sql = "select 
            m.mundescricao 
	from 
	            par.pontuacao p
	inner join par.criterio c ON c.crtid = p.crtid 
	inner join par.indicador i ON i.indid = c.indid 
	inner join par.area are ON are.areid = i.areid 
	inner join par.dimensao d ON d.dimid = are.dimid 
	inner join par.instrumentounidade iu ON iu.inuid = p.inuid
	left join territorios.municipio m ON m.muncod = iu.muncod
	where c.crtid='".$dados['crtid']."'
	and iu.itrid = 2 
	and c.crtstatus = 'A'
	and i.indstatus = 'A'
	and are.arestatus = 'A'
	and d.dimstatus = 'A'
	and p.ptostatus = 'A'
	and m.estuf = '".$dados['estuf']."'
	ORDER BY 1";
	$db->monta_lista_simples($sql,array("Lista de municípios"),50,5,'N','100%',$par2);
	
	echo "<p align=center><input type=button value=Fechar onclick=window.close();></p>";
}


ini_set("memory_limit","2500M");
set_time_limit(0);

if($_REQUEST['requisicao']){
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include  APPRAIZ."includes/cabecalho.inc";

echo "<br/>";

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
function abrirPopupMun(estuf,crtid) {
	window.open('snf.php?modulo=principal/diagnosticoPar&acao=A&requisicao=popUpMunicipio&estuf='+estuf+'&crtid='+crtid,'Municípios','scrollbars=no,height=300,width=500,status=no,toolbar=no,menubar=no,location=no');
}
function setarInuId(estuf) {
	window.location='snf.php?modulo=principal/diagnosticoPar&acao=A&requisicao=setarInuIdPorEstuf&estuf='+estuf+'&goto=diagnosticoPar';
}
</script>
<?

/* montando o menu */
echo montarAbasArray(montaMenuAtendimentoForum(), $_SERVER['REQUEST_URI']);
/* fim montando o menu */

/*
 * Este script é uma cópia do script do PAR par\modulos\relatorio\impressao_par.inc, com adaptações para atender as necessidades do novo sistema
 * Esta cópia foi realizada 03/09/12
 */

$var123 = "true";

$perfis = pegaPerfilGeral();
if((in_array(PERFIL_FORUM_APROVACAO,$perfis) || in_array(PERFIL_FORUM_DISTRIBUICAO,$perfis))) {
	
	$sql = "SELECT DISTINCT estuf FROM snf.usuarioresponsabilidade WHERE pflcod IN('".PERFIL_FORUM_APROVACAO."','".PERFIL_FORUM_DISTRIBUICAO."') AND usucpf='".$_SESSION['usucpf']."' AND rpustatus='A'";
	$estuf = $db->pegaUm($sql);
	
	if($estuf) setarInuIdPorEstuf(array("estuf" => $estuf));
	
}

if($_SESSION['snf']['inuid']) {
	
	$status = $_REQUEST['status'] ? $_REQUEST['status'] : 'A';
	$ptostatus = isset( $_REQUEST['ptostatus'] ) ? $_REQUEST['ptostatus'] : 'A';
	$sql = "SELECT
				d.dimid,
				d.dimcod,
				d.dimcod || ' - ' || d.dimdsc as dimensao,
				area.arecod || ' - ' || area.aredsc as area,
				area.arecod,
				area.areid,
				i.indcod || ' - ' || i.inddsc as indicador,
				i.indid,
				i.indcod,
				c.crtpontuacao as pontuacao,
				p.ptojustificativa,
				p.ptodemandamunicipal,
				p.ptodemandaestadual,
				CASE WHEN ".$_SESSION['snf']['itrid']." = 1
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
				iu.inuid = '".$_SESSION['snf']['inuid']."' AND d.dimcod = 2
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
	$muncod = $_SESSION['snf']['muncod'];
	$descricao = recuperaMunicipioEstado();

}

$arrIndMun = Array();
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<?php if( !isset( $cabecalhoImprecao ) || $cabecalhoImprecao !== false ): ?>

	<thead>
		<th class="tdDegradde01" colspan="2" align="left">
		<?
			if( $_SESSION['snf']['itrid'] == 2 ){
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
				<? if($db->testa_superuser()) : ?>
				<table cellspacing="0" cellpadding="3" border="0"
					style="width: 80px;">
					<tr style="text-align: center;">
						<td style="font-size: 7pt; text-align: center;">
						<?
						$sql = "SELECT estuf as codigo, estdescricao as descricao FROM territorios.estado ORDER BY estdescricao"; 
						$db->monta_combo('estuf', $sql, 'S', 'Selecione o Estado', 'setarInuId', '', '', '200', 'N', 'estuf', '', $_SESSION['snf']['estuf']); 
						?>
						</td>
					</tr>
				</table>
				<? endif; ?>
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
			
			</th>
	
	</thead>
	<?php endif; ?>
	<? 
	while ( $i < $totalreg ) {

		$dimensao = $dado[$i]['dimid'];
		$dimensaocod = $dado[$i]['dimcod'];
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
		$areacod = $dado[$i]['arecod'];

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
		$indicadorcod = $dado[$i]['indcod'];
			
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
		<td class="SubTituloDireita tdDegradde05" nowrap><b>Critério / Pontuação</b>
		</td>
		<td class="tdDegradde05" style="text-align: left"><?echo $dado[$i]['pontuacao'] . ' - '.$dado[$i]['crtdsc']; ?>
		</td>
	</tr>
	<?php 
	$arrIndMun[] = Array('dimcod' => $dimensaocod,
						 'arecod' => $areacod,
						 'indcod' => $indicadorcod);
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
	<?
	$totalGeralIndicadorAno0 = 0;
	$totalGeralIndicadorAno1 = 0;
	$totalGeralIndicadorAno2 = 0;
	$totalGeralIndicadorAno3 = 0;

	}
	?>
	<?
	$totalGeralAreaAno0 = 0;
	$totalGeralAreaAno1 = 0;
	$totalGeralAreaAno2 = 0;
	$totalGeralAreaAno3 = 0;

	}
	?>

	<?
	$totalGeralDimensaAno0 = 0;
	$totalGeralDimensaAno1 = 0;
	$totalGeralDimensaAno2 = 0;
	$totalGeralDimensaAno3 = 0;
	?>
	<? } ?>
</table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td colspan="2"> <p align="center"><b>Resumo PAR Municipal</b></p> </td>
	</tr>
<?php 
foreach( $arrIndMun as $indMun ){
	$sql = "SELECT DISTINCT
				'2.'||are.arecod||'.'||i.indcod||' - '||i.inddsc as Indicador
			FROM
				par.pontuacao p
			INNER JOIN par.criterio 			  c ON c.crtid 		= p.crtid
			INNER JOIN par.indicador 			  i ON i.indid 		= c.indid
			INNER JOIN par.area 				are ON are.areid 	= i.areid
			INNER JOIN par.dimensao 			  d ON d.dimid 		= are.dimid
			INNER JOIN par.instrumentounidade 	 iu ON iu.inuid 	= p.inuid
			LEFT  JOIN territorios.municipio 	  m ON m.muncod 	= iu.muncod
			WHERE
				d.dimcod = 2 AND
				i.indcod = '".$indMun['indcod']."' AND
				are.arecod = '".$indMun['arecod']."' AND
				iu.mun_estuf = '".$_SESSION['snf']['estuf']."' AND
				iu.itrid = 2 AND ---- 2 = Municipio
				p.ptostatus = 'A' AND
				i.indstatus = 'A'
			GROUP BY are.arecod, are.aredsc, i.indcod, i.inddsc
			ORDER BY 1";
	$indicadorMun = $db->pegaUm($sql);
?>
	<tr>
		<td class="SubTituloDireita tdDegradde04"><b>Indicador</b>
		</td>
		<td class="tdDegradde04" style="text-align: left"><?=$indicadorMun; ?>
		</td>
	</tr>
	<tr>
		<td colspan="2">
		<?
		$sql = "select '<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"abrirPopupMun(\''||m.estuf||'\','||c.crtid||');\">' as consulta,
            Count(m.mundescricao), 
            c.crtpontuacao as pontuacao, c.crtdsc
			from 
			            par.pontuacao p
			inner join par.criterio c ON c.crtid = p.crtid 
			inner join par.indicador i ON i.indid = c.indid 
			inner join par.area are ON are.areid = i.areid 
			inner join par.dimensao d ON d.dimid = are.dimid 
			inner join par.instrumentounidade iu ON iu.inuid = p.inuid
			left join territorios.municipio m ON m.muncod = iu.muncod
			where d.dimcod = '".$indMun['dimcod']."' --?? 
			and are.arecod = '".$indMun['arecod']."'  --??
			and i.indcod = '".$indMun['indcod']."'   --??
			and iu.itrid = 2 
			and c.crtstatus = 'A'
			and i.indstatus = 'A'
			and are.arestatus = 'A'
			and d.dimstatus = 'A'
			and p.ptostatus = 'A'
			and m.estuf = '".$_SESSION['snf']['estuf']."'
			GROUP BY c.crtpontuacao,c.crtdsc,m.estuf,c.crtid 
			ORDER BY 3";
		
		$db->monta_lista_simples($sql,array("&nbsp;","Número de municípios","Pontuação","Critério"),50,5,'N','100%',$par2);
		?>
		</td>
	</tr>
<?php 
}
?>
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