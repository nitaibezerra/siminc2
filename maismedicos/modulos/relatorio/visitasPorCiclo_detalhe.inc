<?php 

extract($_GET);

if($_GET){
	$params = array();
	foreach($_GET as $k => $v){

		if(!in_array($k, array('pesquisa','form','req','publico','prtid','carregar','excluir'))){
				
			if(is_array($_POST[$k])){

				foreach($_POST[$k] as $i){
					if(!empty($i))
						$params[] = $k.'[]='.$i;
				}

			}else{
				$params[] = $k.'='.$v;
			}
		}
	}
	$params = implode('&',$params);
}


if( $uniid[0] && $uniid_campo_flag ){
	$arWhere[] = " mdc.uniid ". (!$uniid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $uniid )."' ) ";
}

if( $medico[0] && $medico_campo_flag ){
	$arWhere[] = " mdc.mdccpf ". (!$medico_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $medico )."' ) ";
}

if(in_array($grupo, array('total_medicos','sem_visita'))){
	
	if( $muncod[0] && $muncod_campo_flag ){
		$arWhere[] = " mdc.muncod ". (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $muncod )."' ) ";
	}
	
	if($estuf){
		$arWhere[] = " mdc.estuf = '{$estuf}' ";
	}
	
	if($dsei){
		$arWhere[] = " mdc.dsei = '{$dsei}' ";
	}
	
	if($ciclo){
		$arWhere[] = "mdc.nuciclo = {$ciclo}";
	}elseif(empty($ciclo)){
		$arWhere[] = "mdc.nuciclo is null";
	}
	
	if($grupo == 'sem_visita'){
		
		$arWhere[] = "mdc.mdccpf not in (
							select distinct 
								cpfprofissional 
							from maismedicos.ws_respostas_formulario  
							where wssstatus = 'A' 
							and status = 'Finalizado' 
							and idformulario in (35,36,40,43)
							and cpfprofissional is not null
						)";
						//".($dsei=='t'?'40,43':'35,36')."
	}
	
	if(in_array(PERFIL_TUTOR, arrayPerfil())){
		$arWhere[] = "usr.usucpf = '{$_SESSION['usucpf']}'";
	}
		
	$sql = "select
					mun.estuf,
					mdc.mdcnomeinstituicao as uninome,
					mun.mundescricao || '-' || mun.estuf as mundescricao,
					substr(mdc.mdccpf, 1, 3) || '.' || substr(mdc.mdccpf, 4, 3) || '.' || substr(mdc.mdccpf, 7, 3) || '-' || substr(mdc.mdccpf, 10) as mdccpf,
					mdc.mdcnome,
					mdc.nuciclo,
					mdc.nuetapa
				from maismedicos.medico mdc
				left join maismedicos.usuarioresponsabilidade usr on usr.uniid = mdc.uniid
						and usr.pflcod in (".implode(",",arrayPerfil()).")
				left join territorios.municipio mun on mun.muncod = mdc.muncod
				where mdc.mdcstatus = 'A'
				".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '');
	
	$arCabecalho = array('UF','Instituição Supervisora','Município','CPF','Nome', 'Ciclo', 'Etapa');
	
}else{
	
	// Municipio
	if( $muncod[0] && $muncod_campo_flag ){
		$arWhere[] = " fom.muncodprofissional ". (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $muncod )."' ) ";
	}
	
	// UF
	if($estuf){
		$arWhere[] = " fom.estufprofissional = '{$estuf}' ";
	}
	
	// Tipo da coluna
	if($grupo == 'total_primeira_visita'){
		$arWhere[] = "fom.idformulario = ".($dsei=='t'?'40':'35');
	}elseif ($grupo == 'total_supervisao_pratica'){
		$arWhere[] = "fom.idformulario = ".($dsei=='t'?'43':'36');
	}elseif($grupo == 'total_visitas'){
		$arWhere[] = "fom.idformulario in (".($dsei=='t'?'40,43':'35,36').")";
	}
	
	// Ciclo
	if($ciclo>0){
		$arWhere[] = "fom.nuciclo = {$ciclo}";
	}elseif(empty($ciclo)){
		$arWhere[] = "fom.nuciclo is null";
	}
	
	// CPF do Supervisor
	if( $supervisor[0] && $supervisor_campo_flag ){
		$arWhere[] = " fom.cpfsupervisor ". (!$supervisor_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $supervisor )."' ) and f2.cpfsupervisor ". (!$supervisor_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $supervisor )."' )";
	}
	
	// Periodo
	$mesvisitainicio = str_pad($mesvisitainicio, 2, "0", STR_PAD_LEFT);
	$mesvisitafim = str_pad($mesvisitafim, 2, "0", STR_PAD_LEFT);
	
	if($mesvisitainicio && $anovisitainicio && empty($mesvisitafim) && empty($anovisitafim)){
	
		$arWhere[] = " to_char(fom.dtvisita, 'YYYYMM') = '{$anovisitainicio}{$mesvisitainicio}' ";
	
	}elseif($mesvisitainicio && $anovisitainicio && $mesvisitafim && $anovisitafim){
	
		$arWhere[] = " to_char(fom.dtvisita, 'YYYYMM') >= '{$anovisitainicio}{$mesvisitainicio}' and to_char(fom.dtvisita, 'YYYYMM') <= '{$anovisitafim}{$mesvisitafim}' ";
	
	}else{
	
		$arWhere[] = " to_char(fom.dtvisita, 'YYYYMM') >= '201311' and to_char(fom.dtvisita, 'YYYYMM') <= to_char(now(), 'YYYYMM') ";
	}
	if(in_array(PERFIL_TUTOR, arrayPerfil())){
		$arWhere[] = "usr.usucpf = '{$_SESSION['usucpf']}'";
	}
	
	$sql = "select 
				fom.estufprofissional as estuf,
				fom.noinstituicao as uninome,
				trim(fom.nomunicipioprofissional) as mundescricao,
				fom.noprofissional as mdcnome,
				substr(fom.cpfprofissional, 1, 3) || '.' || substr(fom.cpfprofissional, 4, 3) || '.' || substr(fom.cpfprofissional, 7, 3) || '-' || substr(fom.cpfprofissional, 10) as mdccpf,	
				to_char(fom.dtvisita, 'DD/MM/YYYY') as dtvisita,
				fom.nosupervisor as tutnome,
				substr(fom.cpfsupervisor, 1, 3) || '.' || substr(fom.cpfsupervisor, 4, 3) || '.' || substr(fom.cpfsupervisor, 7, 3) || '-' || substr(fom.cpfsupervisor, 10) as tutcpf,
				fom.nuciclo,
				fom.nuetapa
			from maismedicos.ws_respostas_formulario fom
			left join maismedicos.usuarioresponsabilidade usr on usr.uniid = fom.uniid
						and usr.pflcod in (".implode(",",arrayPerfil()).")
			where fom.wssstatus = 'A'
			and fom.status = 'Finalizado'
			".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."
			order by fom.estufprofissional, fom.nomunicipioprofissional, fom.noprofissional";
	
	$arCabecalho = array('UF','Instituição Supervisora','Município','Nome do Médico','CPF do Médico','Data de Visita','Nome do Supervisor','CPF do Supervisor', 'Ciclo', 'Etapa');
}

// ver($sql, d);
$rs = $db->carregar($sql);

if($_REQUEST['req']=='geraxls'){

	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_MAISMEDICOS_VISITAS_CICLO_DETALHE_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_MAISMEDICOS_VISITAS_CICLO_DETALHE_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );


}else{
?>

<html>

	<head>
	
		<title>Relatório de Frequência de Visitas por Ciclo - Mais Médicos</title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script>
			
			function gerarExcelDetalhe()
			{
				parametros = document.getElementById('parametros').value;
				url = '/maismedicos/maismedicos.php?'+parametros+'&req=geraxls';
				document.location.href = url;
			}
			
		</script>
		
	</head>
	
	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
		
		<p>&nbsp;</p>
		
		<?php monta_titulo('Frequência de Visitas por Ciclo', $linha2); ?>
				
		<input type="hidden" name="parametros" id="parametros" value="<?php echo $params; ?>" />
		
		<center>
			<b><?php echo subTituloMedicos($grupo); ?></b>
			<br/>
			<font size="-1" color="blue"><?php echo $rs ? count($rs) : 0; ?>&nbsp;<?php echo in_array($grupo, array('total_medicos','sem_visita')) ? 'Médicos' : 'Formulários'; ?></font>
			<h3><?php echo $ciclo ? $ciclo.'º Ciclo' : 'Sem ciclo'; ?></h3>		
		</center>

<?php } ?>
		
		<?php if($rs): ?>
			<table class="listagem" cellpadding="3" cellspacing="1" align="center" style="width:95%;">
				<thead>
					<tr>
						<?php foreach($arCabecalho as $v): ?>
							<th><?php echo $v; ?></th>
						<?php endforeach; ?>
					</tr>			
				</thead>
				<tbody>
				<?php foreach($rs as $i => $dados): ?>
					<?php if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7'; ?>				
					<tr bgcolor="<?php echo $marcado; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $marcado; ?>';">
						<?php foreach($dados as $k => $v): ?>
							<td><?php echo $v; ?></td>
						<?php endforeach; ?>
					</tr>
				<?php endforeach; ?>
				</tbody>
				<?php if($_REQUEST['req']!='geraxls'): ?>
					<tfoot>
						<tr style="border-bottom:2px solid black;">
							<td colspan="10" style="text-align:left;">
								<font size="2">
									<b>Total:</b>&nbsp;<?php echo count($rs); ?>&nbsp;registros
								</font>
							</td>
						</tr>
					</tfoot>
				<?php endif; ?>
			</table>
			
			<?php if($_REQUEST['req']!='geraxls') echo '<br/><center><input type="button" value="Gerar Excel" onclick="gerarExcelDetalhe()" /></center>'; ?>
			<p>&nbsp;</p>
		<?php endif; ?>
		
<?php if($_REQUEST['req']!='geraxls') : ?>	
		</body>
	</html>
<?php endif; ?>

<?php 

function subTituloMedicos($var)
{
	switch ($var){
		case 'total_medicos':
			$titulo = 'Médicos Existentes';
			break;
		case 'total_primeira_visita':
			$titulo = 'Primeira Visita';
			break;
		case 'total_supervisao_pratica':
			$titulo = 'Supervisão Prática';
			break;
		case 'sem_visita':
			$titulo = 'Não Visitados';
			break;
		case 'total_visitas':
			$titulo = 'Total de Visitas';
			break;
	}
	
	return $titulo;
}

?>