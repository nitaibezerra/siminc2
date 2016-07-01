<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?
/* configurações */
ini_set("memory_limit", "3024M");
set_time_limit(0);
/* FIM configurações */


if($_REQUEST['esdid']) {
	if(is_numeric($_REQUEST['esdid'])) {
		$where[] = "e.esdid='".$_REQUEST['esdid']."'";
	} else {
		$where[] = "e.esdid IS NULL";
	}
}

if($_REQUEST['estuf']) {
	$where[] = "p.estuf='".$_REQUEST['estuf']."'";
}

if($_REQUEST['muncod']) {
	$where[] = "p.muncod='".$_REQUEST['muncod']."'";
}

$sql = "select p.pdicodinep, 
			   p.pdenome, 
			   p.estuf,
			   m.mundescricao,
			   ROUND(COALESCE(s.rlstotalprimeiraparcela,0),2) as rlstotalprimeiraparcela, 
			   ROUND(COALESCE(s.rlsprimeiraparcela,0),2) as rlsprimeiraparcela, 
			   ROUND(COALESCE((s.rlstotalprimeiraparcela-s.rlsprimeiraparcela),0),2) as saldoprimeira, 
			   ROUND(COALESCE(s.rlstotalsegundaparcela,0),2) as rlstotalsegundaparcela, 
			   ROUND(COALESCE(s.rlssegundaparcela,0),2) as rlssegundaparcela, 
			   ROUND(COALESCE((s.rlstotalsegundaparcela-s.rlssegundaparcela),0),2) as saldosegunda 
		from pdeinterativo.relatorio_saldo s 
		left join pdeinterativo.pdinterativo p on p.pdeid = s.pdeid
		left join workflow.documento d on d.docid = p.docid 
		left join workflow.estadodocumento e on e.esdid = d.esdid 
		left join territorios.municipio m on m.muncod = p.muncod  
		".(($where)?"WHERE ".implode(" AND ",$where):"")." 
		order by p.estuf, m.estuf, m.mundescricao";

$cabecalho = array("Inep","Nome","UF","Município","Total(1ªparcela)","Utilizado(1ªparcela)","Saldo(1ªparcela)","Total(2ªparcela)","Utilizado(2ªparcela)","Saldo(2ªparcela)");

if($_REQUEST['visualizar']=='html') {
	
	?>
	<script>
	function filtro_relatorio_acompanhamento(vlr) {
		var filtro_municipio='';
		if(document.getElementById('muncod')) {
			filtro_municipio = '&muncod='+document.getElementById('muncod').value;
		}
		window.location='pdeinterativo.php?modulo=relatorio/relatoriosgerais&acao=A&visualizar=html&buscar=1&relatorio=relatgeral_saldo.inc&esdid='+document.getElementById('esdid').value+'&estuf='+document.getElementById('estuf').value+filtro_municipio;
	}

	</script> 
	<table class="tabela">
	<tr>
		<td>Situação:</td>
		<td>
		<?
		$sql_combo = "SELECT * FROM (
					  SELECT esdid::text as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='".TPD_WF_FLUXO."'
					  ) foo
					  UNION ALL (SELECT 'NULL' as codigo, 'Não iniciado' as descricao)";
		$db->monta_combo('esdid', $sql_combo, 'S', 'Selecione', 'filtro_relatorio_acompanhamento', '', '', '200', 'S', 'esdid', '', $_REQUEST['esdid']);
		?>
		</td>
	</tr>
	<tr>
		<td>Estado:</td>
		<td>
		<?
		$sql_combo = "SELECT estuf as codigo, estdescricao as descricao FROM territorios.estado";
		$db->monta_combo('estuf', $sql_combo, 'S', 'Selecione', 'filtro_relatorio_acompanhamento', '', '', '200', 'S', 'estuf', '', $_REQUEST['estuf']);
		?>
		</td>
	</tr>
	<? if($_REQUEST['estuf']) :  ?>
	<tr>
		<td>Município:</td>
		<td>
		<?
		$sql_combo = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$_REQUEST['estuf']."' ORDER BY mundescricao";
		$db->monta_combo('muncod', $sql_combo, 'S', 'Selecione', 'filtro_relatorio_acompanhamento', '', '', '200', 'S', 'muncod', '', $_REQUEST['muncod']);
		?>
		</td>
	</tr>
	<? endif; ?>
	</table>
	<?
	$_SESSION['pdeinterativo_vars']['btn_cancelar'] = 'pdeinterativo.php?modulo=relatorio/relatoriosgerais&acao=A&visualizar=html&buscar=1&relatorio=relatgeral_saldo.inc&esdid='.$_REQUEST['esdid'].'&estuf='.$_REQUEST['estuf'].'&muncod='.$_REQUEST['muncod'];
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N',true);
	$db->monta_lista_simples("SELECT 'TOTAL', count(*) FROM (".$sql.") foo",array(),100000,5,'N','100%','N',true);
	
} else {
	
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatSaldo".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_RelatSaldo".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($sql,$cabecalho,100000,5,'N','100%',$par2,'N',true);
	
}

?>