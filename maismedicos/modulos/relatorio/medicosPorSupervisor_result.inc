<?php 

$MOSTRA_TEMPO_EXECUCAO = true;

if($MOSTRA_TEMPO_EXECUCAO){
	// Iniciamos o "contador"
	list($usec, $sec) = explode(' ', microtime());
	$script_start = (float) $sec + (float) $usec;
}
?>
<?php 

ini_set("memory_limit", "2048M");

include APPRAIZ. 'includes/classes/relatorio.class.inc';
$agrup = monta_agp();
$col   = monta_coluna();
$sql   = monta_sql();

// Gerar arquivo xls
if( $_POST['req'] == 'geraxls' ){
	
	$arCabecalho = array('UF','Nome da Instituição Supervisora','MUNICÍPIO','IBGE','Nome Médico','CPF Médico','Data da visita','Nome Supervisor', 'CPF Supervisor', 'FORMULÁRIO', 'DSEI');
// 	$arTipo = array(null, null, null, null, null, 'text', 'date', null, 'text');
// 	gerar_excel($sql, $arCabecalho, 'SIMEC_PMMB_MEDICOS_SUPERVISOR_'.date("Ymdhis"), $arTipo);
	
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_PMMB_MEDICOS_SUPERVISOR_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_PMMB_MEDICOS_SUPERVISOR_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($sql,$arCabecalho,100000000,5,'N','100%','');
	die;
}

$dados = $db->carregar($sql);

// Monta agrupadores
function monta_agp()
{
	$agrupador = $_REQUEST['agrupador'];

	$agp = array(
			"agrupador" => array(),
			"agrupadoColuna" => array(
					"idpublicarformulario",
					"estuf",
					"instituicaosupervisora",
					"mundescricao",
					"nomemedico",
					"cpfmedico",
					"dtvisita",
					"nomesupervisor",
					"cpfsupervisor",
					"tipoform",
					"dsei"
			)
	);
	
	if($agrupador){
		
		foreach ($agrupador as $val){
	
			switch ($val) {
					
				case 'estuf':
					array_push($agp['agrupador'], array(
						"campo" => "estuf",
						"label" => "UF")
					);
					continue;
					break;
				case 'instituicaosupervisora':
					array_push($agp['agrupador'], array(
						"campo" => "instituicaosupervisora",
						"label" => "Instituição Supervisora")
					);
					continue;
					break;
				case 'mundescricao':
					array_push($agp['agrupador'], array(
						"campo" => "mundescricao",
						"label" => "Município")
					);
					continue;
					break;
				case 'dtvisita':
					array_push($agp['agrupador'], array(
						"campo" => "dtvisita",
						"label" => "Data de Visita")
					);
					continue;
					break;
				case 'supervisor':
					array_push($agp['agrupador'], array(
						"campo" => "nomesupervisor",
						"label" => "Supervisor")
					);
					continue;
					break;
				case 'tipoform':
					array_push($agp['agrupador'], array(
						"campo" => "tipoform",
						"label" => "Tipo do Formulário")
					);
					continue;
					break;
				case 'dsei':
					array_push($agp['agrupador'], array(
					"campo" => "dsei",
					"label" => "DSEI")
					);
					continue;
					break;
			}
		}
	}

	// Adiciona ultimo nivel, dados da solicitacao
	array_push($agp['agrupador'], array(
			"campo" => "idpublicarformulario",
			"label" => "Id do Formulário")
		);

	return $agp;
}

// Monta colunas conforme agrupadores
function monta_coluna()
{
	
	$agrupador = $_REQUEST['agrupador'];
	$agrupador = $agrupador ? $agrupador : array();
	
	
	$coluna = array();
	if(!in_array('estuf', $agrupador)){
		$coluna[] = array(
				"campo" => "estuf",
				"label" => "UF"
		);
	}
	if(!in_array('instituicaosupervisora', $agrupador)){
		$coluna[] = array(
				"campo" => "instituicaosupervisora",
				"label" => "Instituição Supervisora"
		);
	}
	if(!in_array('mundescricao', $agrupador)){
		$coluna[] = array(
				"campo" => "mundescricao",
				"label" => "Município"
		);
	}
	if(!in_array('nucpf', $agrupador)){
		$coluna[] = array(
				"campo" => "cpfmedico",
				"label" => "CPF do Médico"
		);
	}
	if(!in_array('noprofissional', $agrupador)){
		$coluna[] = array(
				"campo" => "nomemedico",
				"label" => "Nome do Médico"
		);
	}
	if(!in_array('dtvisita', $agrupador)){
		$coluna[] = array(
				"campo" => "dtvisita",
				"label" => "Data de Visita"
		);
	}
	if(!in_array('supervisor', $agrupador)){
		$coluna[] = array(
				"campo" => "cpfsupervisor",
				"label" => "CPF do Supervisor"
		);
	}
	if(!in_array('supervisor', $agrupador)){
		$coluna[] = array(
				"campo" => "nomesupervisor",
				"label" => "Nome do Supervisor"
		);
	}
	if(!in_array('tipoform', $agrupador)){
		$coluna[] = array(
				"campo" => "tipoform",
				"label" => "Tipo do Formulário"
		);
	}
	if(!in_array('dsei', $agrupador)){
		$coluna[] = array(
				"campo" => "dsei",
				"label" => "DSEI"
		);
	}

	return $coluna;
}

// Monta sql do relatorio
function monta_sql()
{

	extract($_REQUEST);

	// Municipio
	if( $muncod[0] && $muncod_campo_flag ){
		$arWhere[] = " f.muncodprofissional ". (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $muncod )."' ) ";
	}
	
	// UF
	if($estuf){
		$arWhere[] = " f.estufprofissional = '{$estuf}' ";
	}
	
	// Tipo do relatorio
	if($tipoform){
		$arWhere[] = " f.idformulario = {$tipoform}";
	}
	
	if( $uniid[0] && $uniid_campo_flag ){
		$arWhere[] = " f.muncodprofissional in (select muncod from maismedicos.universidademunicipio where uniid ". (!$uniid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $uniid )."' )) ";
	}
	
	// Supervisor
	if( $supervisor[0] && $supervisor_campo_flag ){
		$arWhere[] = " f.cpfsupervisor ". (!$supervisor_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $supervisor )."' ) ";
	}
	
	// Medico
	if( $medico[0] && $medico_campo_flag ){
		$arWhere[] = " f.cpfprofissional ". (!$medico_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $medico )."' ) ";
	}
	
	// Mostrar DSEI
	if($incluirdsei == 'on'){
	
		$arWhere[] = " f.idformulario in (".FORM_PRIMEIRA_VISITA.",".
											FORM_SUPERVISAO_PRATICA.",".
											FORM_PRIMEIRA_VISITA_DSEI.",".
											FORM_SUPERVISAO_PRATICA_DSEI.") ";

	}else{
	
		$arWhere[] = " f.idformulario in (".FORM_PRIMEIRA_VISITA.",".
											FORM_SUPERVISAO_PRATICA.") ";
	}
	
	// Periodo
	$mesvisitainicio = str_pad($mesvisitainicio, 2, "0", STR_PAD_LEFT);
	$mesvisitafim = str_pad($mesvisitafim, 2, "0", STR_PAD_LEFT);
	
	if($mesvisitainicio && $anovisitainicio && empty($mesvisitafim) && empty($anovisitafim)){
	
		$arWhere[] = " to_char(f.dtvisita, 'YYYYMM') = '{$anovisitainicio}{$mesvisitainicio}' ";
	
	}elseif($mesvisitainicio && $anovisitainicio && $mesvisitafim && $anovisitafim){
	
		$arWhere[] = " to_char(f.dtvisita, 'YYYYMM') >= '{$anovisitainicio}{$mesvisitainicio}' and to_char(f.dtvisita, 'YYYYMM') <= '{$anovisitafim}{$mesvisitafim}' ";
	
	}else{
		
		if($mostrardtvazia == 'on'){
			$label .= '<tr><td align="right"><b>Mostrar data de visita vazia</b></td><td align="left">Sim</td></tr>';
			$dtVisitaFiltro[] = " (f.dtvisita is null) ";
		}
		
		if($mostrardterro == 'on'){
			$label .= '<tr><td align="right"><b>Mostrar data de visita com erro</b></td><td align="left">Sim</td></tr>';
			$dtVisitaFiltro[] = " (to_char(f.dtvisita, 'YYYYMM') < '201310' or to_char(f.dtvisita, 'YYYYMM') > to_char(now(), 'YYYYMM')) ";
		}
		
		$dtVisitaFiltro[] = " (to_char(f.dtvisita, 'YYYYMM') >= '201310' and to_char(f.dtvisita, 'YYYYMM') <= to_char(now(), 'YYYYMM')) ";
		
		$arWhere[] = " (".implode(' or ', $dtVisitaFiltro).") ";
	
	}
	
	if(in_array(PERFIL_SUPERVISOR, arrayPerfil())){
		$arWhere[] = "f.cpfsupervisor = '{$_SESSION['usucpf']}'";
	}
	
	$arPerfil = arrayPerfil();
	if(in_array(PERFIL_TUTOR, $arPerfil)){
		$arWhere[] = "usr.usucpf = '{$_SESSION['usucpf']}'";
		$arWhereTotal[] = "usr.usucpf = '{$_SESSION['usucpf']}'";
	}else
		if(in_array(PERFIL_SUPERVISOR, arrayPerfil())){
		$arWhere[] = "f.cpfsupervisor = '{$_SESSION['usucpf']}'";
	}
	
	$sql = "select 
			".($_POST['req'] != 'geraxls' ? "'<img src=\"../imagens/acrobat.gif\" height=\"15\" style=\"cursor:pointer;\" alt=\"Gerar PDF com as Respostas\" title=\"Gerar PDF com as Respostas\" onclick=\"gerarPdfRespostas(\'' || f.idpublicarformulario || '\')\"/>&nbsp;Nº:&nbsp;' || f.idpublicarformulario as idpublicarformulario," : "")."
			m.estuf,
			noinstituicao as instituicaosupervisora,			
			coalesce(m.mundescricao, f.nomunicipioprofissional) || '-' || coalesce(m.estuf, f.estufprofissional) as mundescricao,
			".($_POST['req'] == 'geraxls' ? "m.muncod," : "")."
			f.noprofissional as nomemedico,
			".($_POST['req'] == 'geraxls' ? "f.cpfprofissional" : "substr(f.cpfprofissional, 1, 3) || '.' || substr(f.cpfprofissional, 4, 3) || '.' || substr(f.cpfprofissional, 7, 3) || '-' || substr(f.cpfprofissional, 10)")." as cpfmedico,			
			to_char(f.dtvisita,'DD/MM/YYYY') as dtvisita,
			nosupervisor as nomesupervisor,			
			".($_POST['req'] == 'geraxls' ? "f.cpfsupervisor" : "substr(f.cpfsupervisor, 1, 3) || '.' || substr(f.cpfsupervisor, 4, 3) || '.' || substr(f.cpfsupervisor, 7, 3) || '-' || substr(f.cpfsupervisor, 10)" )." as cpfsupervisor,			
			case when idformulario = 33 then 'Primeiras impressões'
			     when idformulario = 35 then 'Primeira Visita de Supervisão'
			     when idformulario = 36 then 'Supervisão Prática'
			     when idformulario = 39 then 'Primeiras impressões - DSEI'
			     when idformulario = 40 then 'Primeira Visita de Supervisão In Loco - DSEI'
			     when idformulario = 43 then 'Supervisão Prática - DSEI'
			else 'Não encontrado' end as tipoform,
			case when f.dsei = true then 'Sim' else 'Não' end as dsei
		from maismedicos.ws_respostas_formulario f
		left join maismedicos.usuarioresponsabilidade usr on usr.uniid = f.uniid
			and usr.pflcod in (".implode(",",$arPerfil).")
		--join maismedicos.medico d on f.cpfprofissional = d.mdccpf and d.mdcstatus = 'A'
		left join territorios.municipio m on m.muncod = f.muncodprofissional
		where f.wssstatus = 'A'
		and f.status = 'Finalizado'		
		".($arWhere ? 'and '.implode(' and ', $arWhere) : '')."
		order by m.estuf, m.mundescricao, f.noprofissional";
	
// 	ver($sql, d);
	return $sql;
}

?>

<html>

	<head>
	
		<title>Relatório de Distribuição Médicos Visitados por Supervisor - Mais Médicos</title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script>

		function gerarPdfRespostas(id)
		{
			var janela = window.open( '?modulo=relatorio/medicosPorSupervisor&acao=A&requisicao=mostraRespotas&id='+id, 'popupRespostas', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			janela.focus();	
		}
		
		</script>
		
	</head>
	
	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	

		<?php
		
			$r = new montaRelatorio();
			$r->setAgrupador($agrup, $dados); 
			$r->setColuna($col);
			$r->setTotNivel(true);
			$r->setMonstrarTolizadorNivel(true);
			$r->setBrasao(true);
			$r->setEspandir( $_REQUEST['expandir'] );
			$r->setTotalizador(true);
			echo $r->getRelatorio();
			
		?>
		
	</body>
	
</html>

<?php 
if($MOSTRA_TEMPO_EXECUCAO){
	// Terminamos o "contador" e exibimos
	list($usec, $sec) = explode(' ', microtime());
	$script_end = (float) $sec + (float) $usec;
	$elapsed_time = round($script_end - $script_start, 5);
	echo '<p>&nbsp;</p><center>Tempo decorrido: ', $elapsed_time, ' secs. Memória usada: ', round(((memory_get_peak_usage(true) / 1024) / 1024), 2), 'Mb</center>';
}
?>