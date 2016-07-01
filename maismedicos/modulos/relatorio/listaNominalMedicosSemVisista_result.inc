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
	$arWhere[] = " mdc.estuf = '{$estuf}' ";
}

// Municipio
if( $muncod[0] && $muncod_campo_flag ){
	$label .= '<tr><td align="right"><b>Município(s)</b></td><td align="left">'.implode('<br/>', $db->carregarColuna("select mundescricao from territorios.municipio where muncod in ('".implode( "','", $muncod )."')")).'</td></tr>';
	$arWhere[] = " mdc.muncod ". (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $muncod )."' ) ";
}

// So capitais
if($socapitais == 'on'){
	$label .= '<tr><td align="right"><b>Só Capitais</b></td><td align="left">Sim</td></tr>';
	$arWhere[] = " mdc.muncod in ('3550308','3304557','2927408','5300108','2304400','3106200','1302603','4106902','2611606','4314902','1501402','5208707',
								'2111300','2704302','2408102','2211001','5002704','2507507','2800308','5103403','1100205','4205407','1600303','1200401',
								'3205309','1400100','1721000')";
}

// Instituicao Supervisora
if( $uniid[0] && $uniid_campo_flag ){
	$label .= '<tr><td align="right"><b>Instituição(ões) Supervisora(s)</b></td><td align="left">'.implode('<br/>', $db->carregarColuna("select uninome from maismedicos.universidade where uniid in ('".implode( "','", $uniid )."')")).'</td></tr>';
	$arWhere[] = " mdc.uniid ". (!$uniid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $uniid )."' ) ";
}

if($nome){
	$label .= '<tr><td align="right"><b>Nome</b></td><td align="left">'.$nome.'</td></tr>';
	$arWhere[] = " mdc.mdcnome ilike '%{$nome}%' ";
}

if($cpf){
	$label .= '<tr><td align="right"><b>CPF</b></td><td align="left">'.$cpf.'</td></tr>';
	$cpf = str_replace(array('.','-'), '', $cpf);
	$arWhere[] = " mdc.mdccpf = '{$cpf}'";
}

$arPerfil = arrayPerfil();
if(in_array(PERFIL_SUPERVISOR, $arPerfil) || in_array(PERFIL_TUTOR, $arPerfil)){
	$arWhere[] = "usr.usucpf = '{$_SESSION['usucpf']}'";
}

$sql = "select distinct		
				mdcnome,
				".($_REQUEST['requisicao'] != 'exibirxls' ? "substr(mdccpf, 1, 3) || '.' || substr(mdccpf, 4, 3) || '.' || substr(mdccpf, 7, 3) || '-' || substr(mdccpf, 10)" : "mdccpf" )." as mdccpf,
				mdcnomeinstituicao,
				mun.estuf,
				mundescricao,				
				".($_REQUEST['requisicao'] == 'exibirxls' ? 'mdc.muncod,' : '')."
				nuciclo,
				nuetapa,
				case when dsei = true then 'Sim' else 'Não' end as dsei
			from maismedicos.medico mdc
			join territorios.municipio mun on mun.muncod = mdc.muncod
			left join maismedicos.universidade u on u.idunasus = mdc.coinstituicaosupervisora  
			left join maismedicos.usuarioresponsabilidade usr on usr.uniid = u.uniid
				and usr.pflcod in (".implode(",",$arPerfil).")
			where mdcstatus = 'A'
			and mdccpf not in (
				select distinct
					cpfprofissional
				from maismedicos.ws_respostas_formulario fom
				where fom.wssstatus = 'A'
				and fom.status = 'Finalizado'
					and fom.idformulario in (35,36,40,43)
				and fom.cpfprofissional is not null 
			)			
			".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."
			order by mdcnome";

if($_REQUEST['requisicao'] == 'exibirxls'){ 

	$arCabecalho = array('nome_profissional', 'cpf', 'nome_instituicao', 'uf_atuacao', 'municipio_atuacao', 'IBGE',  'ciclo', 'etapa', 'DSEI');
	$arTipo = array(null, 'text');
	gerar_excel($sql, $arCabecalho, 'SIMEC_PMMB_LISTA_NOMINAL_MEDICOS_SEM_VISITA_'.date("Ymdhis"), $arTipo);
	die;
	
}

?>
	
<html>

	<head>
	
		<title>Relatório de Lista Nominal de Médicos sem Visita - Mais Médicos</title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		
	</head>
	
	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
	
		<br/>
		<?php 
		$titulo_modulo = "Lista Nominal de Médicos sem Visita";
		monta_titulo( $titulo_modulo, '' );
		if($label){
			echo '<center>';
			echo '<b>FILTRO(S):</b>';
			echo '<table cellspacing="1" cellpadding="3" align="center">';
			echo $label;
			echo '</table>';
			echo '</center>';
		}
		
		$arCabecalho = array('Nome', 'CPF', 'Instituição Supervisora', 'UF', 'Município',  'Ciclo', 'Etapa', 'DSEI');
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