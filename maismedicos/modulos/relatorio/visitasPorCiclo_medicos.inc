<?php 

extract($_GET);

if($dsei){
	$arWhere[] = " mdc.dsei = '{$dsei}' ";
}

if($ciclo){
	$arWhere[] = "mdc.nuciclo = {$ciclo}";
}elseif(empty($ciclo)){
	$arWhere[] = "mdc.nuciclo is null";
}

if( $muncod[0] && $muncod_campo_flag ){
	$arWhere[] = " mun.muncod ". (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $muncod )."' ) ";
}

if($estuf){
	$arWhere[] = " mun.estuf = '{$estuf}' ";
}

if( $uniid[0] && $uniid_campo_flag ){
	$arWhere[] = " mdc.uniid ". (!$uniid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $uniid )."' ) ";
}

if( $medico[0] && $medico_campo_flag ){
	$arWhere[] = " mdc.mdccpf ". (!$medico_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $medico )."' ) ";
}

if(in_array($grupo, array('total_medicos','sem_visita'))){

	if($grupo == 'sem_visita'){
		$arWhere[] = "mdc.mdccpf not in (select 
											cpfprofissional 
										from maismedicos.ws_respostas_formulario 
										where status = 'Finalizado' 
										and wssstatus = 'A' 
										and cpfprofissional is not null 
										and idformulario in (35 ,36))";
	}

	$sql = "select
				mun.estuf,
				coalesce(mdc.mdcnomeinstituicao , uni.uninome) as uninome,
				mun.mundescricao || '-' || mun.estuf as mundescricao,				
				substr(mdc.mdccpf, 1, 3) || '.' || substr(mdc.mdccpf, 4, 3) || '.' || substr(mdc.mdccpf, 7, 3) || '-' || substr(mdc.mdccpf, 10) as mdccpf,
				mdc.mdcnome,
				mdc.nuciclo,
				mdc.nuetapa
			from maismedicos.medico mdc
			join territorios.municipio mun on mun.muncod = mdc.muncod
			left join maismedicos.universidade uni on uni.uniid = mdc.uniid
			where mdc.mdcstatus = 'A'
			".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."
			order by mun.estuf, mun.mundescricao, mdc.mdcnome";
	
	$arCabecalho = array('UF','Instituição Supervisora','Município','CPF','Nome', 'Ciclo', 'Etapa');
	
}else{
	
	if($grupo == 'total_primeira_visita'){
		
		$arWhere[] = "fom.idformulario = 35";
		
	}elseif ($grupo == 'total_supervisao_pratica'){
		
		$arWhere[] = "fom.idformulario = 36";
	}
	
	if( $supervisor[0] && $supervisor_campo_flag ){
		$arWhere[] = " fom.cpfenvio ". (!$supervisor_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $supervisor )."' ) and f2.cpfenvio ". (!$supervisor_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $supervisor )."' )";
	}
	
	$sql = "select
				mun.estuf,
				coalesce(mdc.mdcnomeinstituicao , uni.uninome) as uninome,
				mun.mundescricao || '-' || mun.estuf as mundescricao,
				mdc.mdcnome,				
				substr(mdc.mdccpf, 1, 3) || '.' || substr(mdc.mdccpf, 4, 3) || '.' || substr(mdc.mdccpf, 7, 3) || '-' || substr(mdc.mdccpf, 10) as mdccpf,
				to_char(dtvisita, 'DD/MM/YYYY') as dtvisita,
				tut.tutnome,
				substr(tut.tutcpf, 1, 3) || '.' || substr(tut.tutcpf, 4, 3) || '.' || substr(tut.tutcpf, 7, 3) || '-' || substr(tut.tutcpf, 10) as tutcpf,
				mdc.nuciclo,
				mdc.nuetapa				
			from maismedicos.medico mdc
			join maismedicos.ws_respostas_formulario fom on mdc.mdccpf = fom.cpfprofissional
			join territorios.municipio mun on mun.muncod = mdc.muncod
			left join maismedicos.tutor tut on tut.tutcpf = fom.cpfenvio and tut.tutstatus = 'A' and tut.tuttipo = 'S'
			left join maismedicos.universidade uni on uni.uniid = mdc.uniid
			where mdc.mdcstatus = 'A'
			and fom.wssstatus = 'A'
			and fom.status = 'Finalizado'
			and fom.idformulario in (35,36)
			".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."
			order by mun.estuf, mun.mundescricao, mdc.mdcnome";
	
	$arCabecalho = array('UF','Instituição Supervisora','Município','Nome do Médico','CPF do Médico','Data de Visita','Nome do Supervisor','CPF do Supervisor', 'Ciclo', 'Etapa');
}

ver($sql);
$rs = $db->carregar($sql);

?>


<html>

	<head>
	
		<title>Relatório de Frequência de Visitas por Ciclo - Mais Médicos</title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		
	</head>
	
	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
		
		<p>&nbsp;</p>
		
		<?php monta_titulo('Frequência de Visitas por Ciclo', $linha2); ?>
				
		<center>
			<h3><?php echo $ciclo ? $ciclo.'º Ciclo' : 'Sem ciclo'; ?></h3>		
			<b><?php echo subTituloMedicos($grupo); ?></b>
			<br/>
			<font size="-1" color="blue"><?php echo count($rs); ?>&nbsp;Médicos</font>
		</center>
		
		<p>&nbsp;</p>
	
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
				<tfoot>
					<tr>
						<th colspan="10">
							<font size="3">
								<b>Total:</b>&nbsp;<?php echo count($rs); ?>&nbsp;registros
							</font>
						</th>
					</tr>
				</tfoot>
			</table>
		<?php endif; ?>
		
	</body>
</html>
<?php 

function subTituloMedicos($var)
{
	switch ($var){
		case 'total_medicos':
			$titulo = 'Total de Médicos';
			break;
		case 'total_primeira_visita':
			$titulo = 'Primeira Visita';
			break;
		case 'total_supervisao_pratica':
			$titulo = 'Supervisão Prática';
			break;
		case 'sem_visita':
			$titulo = 'Sem Visita';
			break;
		case 'total_visitas':
			$titulo = 'Total de Visitas';
			break;
	}
	
	return $titulo;
}

?>