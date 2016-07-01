<?php 

$MOSTRA_TEMPO_EXECUCAO = $_REQUEST['requisicao']!='exibirxls' ? true : false;

if($MOSTRA_TEMPO_EXECUCAO){
	// Iniciamos o "contador"
	list($usec, $sec) = explode(' ', microtime());
	$script_start = (float) $sec + (float) $usec;
}
?>
<?php 

if($_POST) extract($_POST);

// UF
if($estuf){
	$label .= '<tr><td align="right"><b>UF</b></td><td align="left">'.$estuf.'</td></tr>';
	$arWhere[] = " tut.estuf = '{$estuf}' ";
}

// Municipio
if( $muncod[0] && $muncod_campo_flag ){
	$label .= '<tr><td align="right"><b>Município(s)</b></td><td align="left">'.implode('<br/>', $db->carregarColuna("select mundescricao from territorios.municipio where muncod in ('".implode( "','", $muncod )."')")).'</td></tr>';
	$arWhere[] = " tut.muncod ". (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $muncod )."' ) ";
}

// So capitais
if($socapitais == 'on'){
	$label .= '<tr><td align="right"><b>Só capitais</b></td><td align="left">Sim</td></tr>';
	$arWhere[] = " tut.muncod in ('3550308','3304557','2927408','5300108','2304400','3106200','1302603','4106902','2611606','4314902','1501402','5208707',
								'2111300','2704302','2408102','2211001','5002704','2507507','2800308','5103403','1100205','4205407','1600303','1200401',
								'3205309','1400100','1721000')";
}

// Instituicao Supervisora
if( $uniid[0] && $uniid_campo_flag ){
	$label .= '<tr><td align="right"><b>Instituição(ões) Supervisora(s)</b></td><td align="left">'.implode('<br/>', $db->carregarColuna("select uninome from maismedicos.universidade where uniid in ('".implode( "','", $uniid )."')")).'</td></tr>';
	$arWhere[] = " tut.uniid ". (!$uniid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $uniid )."' ) ";
}

if($nome){
	$label .= '<tr><td align="right"><b>Nome</b></td><td align="left">'.$nome.'</td></tr>';
	$arWhere[] = " tut.tutnome ilike '%{$nome}%' ";
}

if($cpf){
	$label .= '<tr><td align="right"><b>CPF</b></td><td align="left">'.$cpf.'</td></tr>';
	$cpf = str_replace(array('.','-'), '', $cpf);
	$arWhere[] = " tut.tutcpf = '{$cpf}'";
}

$arPerfil = arrayPerfil();
if(in_array(PERFIL_SUPERVISOR, $arPerfil) || in_array(PERFIL_TUTOR, $arPerfil)){
	$arWhere[] = "usr.usucpf = '{$_SESSION['usucpf']}'";
	$arWhereTotal[] = "usr.usucpf = '{$_SESSION['usucpf']}'";
}

$sql = "select 
			tut.tutnome,
			".($_REQUEST['requisicao'] != 'exibirxls' ? "substr(tut.tutcpf, 1, 3) || '.' || substr(tut.tutcpf, 4, 3) || '.' || substr(tut.tutcpf, 7, 3) || '-' || substr(tut.tutcpf, 10)" : " tut.tutcpf " )." as tutcpf,
			case when tut.tuttipo = 'S' then 'Supervisor'
			     when tut.tuttipo = 'T' then 'Tutor'
			end as funcao
			,uni.uninome
		from maismedicos.tutor tut
		join territorios.municipio mun on mun.muncod = tut.muncod
		join maismedicos.universidade uni on uni.uniid = tut.uniid
		left join maismedicos.usuarioresponsabilidade usr on usr.uniid = uni.uniid
			and usr.pflcod in (".implode(",",$arPerfil).")
		where tut.tutstatus = 'A'
		".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."
		order by tut.tutnome";


if($_REQUEST['requisicao'] == 'exibirxls'){ 

	$arCabecalho = array('Nome', 'CPF', 'Função', 'Instituição Supervisora');
	$arTipo = array(null,'text');
	gerar_excel($sql, $arCabecalho, 'SIMEC_PMMB_LISTA_NOMINAL_TUTOR_SUPERVISOR_'.date("Ymdhis"), $arTipo);
	die;
}

?>
	
<html>

	<head>
	
		<title>Relatório de Lista Nominal Tutor com Supervisor - Mais Médicos</title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		
	</head>
	
	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
	
		<br/>
		<?php 
		$titulo_modulo = "Lista Nominal Tutor com Supervisor";
		monta_titulo( $titulo_modulo, '' );
		if($label){
			echo '<center>';
			echo '<b>FILTRO(S):</b>';
			echo '<table cellspacing="1" cellpadding="3" align="center">';
			echo $label;
			echo '</table>';
			echo '</center>';
		}
		
		$arCabecalho = array('Nome', 'CPF', 'Função', 'Instituição Supervisora');
		$db->monta_lista($sql, $arCabecalho, 1000, 10, 'N', '', '');
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
<br/><br/>