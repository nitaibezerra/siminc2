<?php 

$MOSTRA_TEMPO_EXECUCAO = $_REQUEST['req']!='geraxls' ? true : false;

if($MOSTRA_TEMPO_EXECUCAO){
	// Iniciamos o "contador"
	list($usec, $sec) = explode(' ', microtime());
	$script_start = (float) $sec + (float) $usec;
}
?>
<?php 
ini_set("memory_limit", "2048M");

extract($_REQUEST);

// Municipio
if( $muncod[0] && $muncod_campo_flag ){
	
	$label .= '<tr><td align="right"><b>Município(s)</b></td><td align="left">'.implode('<br/>', $db->carregarColuna("select mundescricao from territorios.municipio where muncod in ('".implode( "','", $muncod )."')")).'</td></tr>';
	$arWhere[] = " f.muncodprofissional ". (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $muncod )."' ) ";
	$arWhereMedico[] = " mdc.muncod ". (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $muncod )."' ) ";
}

// UF
if($estuf){
	
	$label .= '<tr><td align="right"><b>UF</b></td><td align="left">'.$estuf.'</td></tr>';
	$arWhere[] = " f.estufprofissional = '{$estuf}' ";
	$arWhereMedico[] = " mdc.estuf = '{$estuf}' ";
}

if( $uniid[0] && $uniid_campo_flag ){
	
	$label .= '<tr><td align="right"><b>Instituição(ões) Supervisora(s)</b></td><td align="left">'.implode('<br/>', $db->carregarColuna("select uninome from maismedicos.universidade where uniid in ('".implode( "','", $uniid )."')")).'</td></tr>';
	$arWhere[] = " f.muncodprofissional in (select muncod from maismedicos.universidademunicipio where uniid ". (!$uniid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $uniid )."' )) ";
	$arWhereMedico[] = " mdc.uniid ". (!$uniid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $uniid )."' ) ";
}

// Supervisor
if( $supervisor[0] && $supervisor_campo_flag ){
	$label .= '<tr><td align="right"><b>Supervisor(es)</b></td><td align="left">'.implode('<br/>', $db->carregarColuna("select tutnome from maismedicos.tutor where tutcpf in ('".implode( "','", $supervisor )."')")).'</td></tr>';
	$arWhere[] = " f.cpfsupervisor ". (!$supervisor_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $supervisor )."' ) ";
}

// Medico
if( $medico[0] && $medico_campo_flag ){
	$label .= '<tr><td align="right"><b>Médico(s)</b></td><td align="left">'.implode('<br/>', $db->carregarColuna("select mdcnome from maismedicos.medico where mdccpf in ('".implode( "','", $medico )."')")).'</td></tr>';
	$arWhere[] = " f.cpfprofissional ". (!$medico_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $medico )."' ) ";
}

// Periodo
$mesvisitainicio = str_pad($mesvisitainicio, 2, "0", STR_PAD_LEFT);
$mesvisitafim = str_pad($mesvisitafim, 2, "0", STR_PAD_LEFT);

if($mesvisitainicio && $anovisitainicio && empty($mesvisitafim) && empty($anovisitafim)){

	$label .= '<tr><td align="right"><b>Período</b></td><td align="left">'.$mesvisitainicio.'/'.$anovisitainicio.'</td></tr>';
	$arWhere[] = " to_char(f.dtvisita, 'YYYYMM') = '{$anovisitainicio}{$mesvisitainicio}' ";

}elseif($mesvisitainicio && $anovisitainicio && $mesvisitafim && $anovisitafim){
	
	$label .= '<tr><td align="right"><b>Período</b></td><td align="left">'.$mesvisitainicio.'/'.$anovisitainicio.' até '.$mesvisitafim.'/'.$anovisitafim.'</td></tr>';
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

$arPerfil = arrayPerfil();
if(in_array(PERFIL_TUTOR, $arPerfil)){
// 	$arWhere[] = "usr.usucpf = '{$_SESSION['usucpf']}'";
}else
	if(in_array(PERFIL_SUPERVISOR, arrayPerfil())){
	$arWhere[] = "f.cpfsupervisor = '{$_SESSION['usucpf']}'";
}

$sql   = monta_sql('false', $arWhere, $arWhereMedico);
$dados = $db->carregar($sql);

$sql   = monta_sql('true', $arWhere, $arWhereMedico);
$dados_dsei = $db->carregar($sql);

$sql = munta_sql_sem_visita($arWhere, $arWhereMedico);
$dados_sem_visita = $db->carregar($sql);
if($dados_sem_visita ){
	foreach($dados_sem_visita as $dsei){
		$dsei['nuciclo'] = $dsei['nuciclo']>0 ? $dsei['nuciclo'] : 0;
		$arSemVisita[$dsei['nuciclo']][$dsei['dsei']] = $dsei['total'];
	}
}

if(in_array(PERFIL_TUTOR, $arPerfil)){
// 	$arWhere[] = "usr.usucpf = '{$_SESSION['usucpf']}'";
}

// Monta sql do relatorio
function monta_sql($dsei = 'false', $arWhere = array(), $arWhereMedico = array())
{
	$sql = "select 
				coalesce(foo.nuciclo,0) as numero_ciclo,
				coalesce(sum(total_primeira_visita),0) as total_primeira_visita,
				coalesce(sum(total_supervisao_pratica),0) as total_supervisao_pratica,
				
				(select 
						coalesce(count(distinct mdccpf),0) 
					from maismedicos.medico mdc 
					--left join maismedicos.usuarioresponsabilidade usr on usr.uniid = mdc.uniid
					--and usr.pflcod in (".implode(",",arrayPerfil()).")
					where mdc.mdcstatus = 'A' 
					and coalesce(mdc.nuciclo,0) = coalesce(foo.nuciclo,0) 
					and dsei = {$dsei}
					--".(in_array(PERFIL_TUTOR, arrayPerfil()) ? "and usr.usucpf = '{$_SESSION['usucpf']}'" : '')."
					".($arWhereMedico ? 'and '.implode(' and ', $arWhereMedico) : '')."
				) as total_medicos
				
			from (
				select 
					f.nuciclo,
					case when f.idformulario = ".($dsei=='true' ? '40' : '35')." then count(distinct f.idpublicarformulario) end as total_primeira_visita,
					case when f.idformulario = ".($dsei=='true' ? '43' : '36')." then count(distinct f.idpublicarformulario) end as total_supervisao_pratica
				from maismedicos.ws_respostas_formulario f
				--left join maismedicos.usuarioresponsabilidade usr on usr.uniid = f.uniid
				--	and rpustatus = 'A'
				where f.wssstatus = 'A'
				and f.status = 'Finalizado'
				and f.idformulario in (".($dsei=='true' ? '40,43' : '35,36').")							
				".($arWhere ? 'and '.implode(' and ', $arWhere) : '')."
				group by f.nuciclo, f.idformulario
			) as foo
			group by foo.nuciclo";
	
// 	ver($sql, d);
	return $sql;
}

function munta_sql_sem_visita($arWhere = array(), $arWhereMedico = array())
{
	$arPerfil = arrayPerfil();
	if(in_array(PERFIL_SUPERVISOR, $arPerfil) || in_array(PERFIL_TUTOR, $arPerfil)){
// 		$arWhereMedico[] = "usr.usucpf = '{$_SESSION['usucpf']}'";
	}
	
	$sql = "select 
					count(distinct mdc.mdccpf) as total, 
					coalesce(mdc.nuciclo,0) as nuciclo,
					case when mdc.dsei = false then 'sem_dsei' else 'com_dsei' end as dsei
			from maismedicos.medico mdc
			left join maismedicos.universidade u on u.idunasus = mdc.coinstituicaosupervisora  
			--left join maismedicos.usuarioresponsabilidade usr on usr.uniid = u.uniid
				--and usr.pflcod in (".implode(",",$arPerfil).")
			where mdc.mdccpf not in (
				select distinct
					cpfprofissional
				from maismedicos.ws_respostas_formulario fom
				where fom.wssstatus = 'A'
				and fom.status = 'Finalizado'
				and fom.cpfprofissional is not null
			)
			and mdc.mdcstatus = 'A'
			".($arWhereMedico ? 'and '.implode(' and ', $arWhereMedico) : '')."
			group by mdc.nuciclo, mdc.dsei";
	
// 	ver($sql, d);
	return $sql;
}


if($_POST){
	$params = array();
	foreach($_POST as $k => $v){
		
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

if($_REQUEST['req']=='geraxls'){

	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_PMMB_VISITAS_CICLO_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_PMMB_VISITAS_CICLO_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );	
	
	$estiloTabela = 'border="1"';
	
}else{

?>

<html>

	<head>
	
		<title>Relatório de Distribuição Médicos Visitados por Supervisor - Mais Médicos</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

		<script type="text/javascript" src="/includes/funcoes.js"></script>
		<script type="text/javascript" src="/includes/JQuery/jquery-1.10.2.min.js"></script>
		<script type="text/javascript">
			function mostrarDetalhes(dsei, grupo, ciclo)
			{
				params = $('#parametros').val();
				var janela = window.open( '?modulo=relatorio/visitasPorCiclo&acao=A&requisicao=mostraDetalhes&dsei='+dsei+'&grupo='+grupo+'&ciclo='+ciclo+'&'+params, 'medicosCiclo', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
				janela.focus();	
			}
		</script>
		<style>
			.link_quantidade{
				cursor: pointer;
				color: blue;
				
			}
			.link_quantidade:hover{
				cursor: pointer;
				color: blue;
				text-decoration: underline;
			}
		</style>
		
	</head>
	
	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
	
		<input type="hidden" name="parametros" id="parametros" value="<?php echo $params; ?>" />
		
		<?php 
		monta_titulo('Frequência de Visitas por Ciclo', $linha2);
		if($_REQUEST['req']!='geraxls'){
			$estiloTabela = 'class="listagem" cellpadding="3" celspacing="1" align="center" bgcolor="#f5f5f5"';
			$estiloTrTotalTabela = 'bgcolor="#f0f0f0"';
		}
		if($label){
			echo '<center>';
			echo '<b>FILTRO(S):</b>';
			echo '<table cellspacing="1" cellpadding="3" align="center">';
			echo $label;
			echo '</table>';
			echo '</center>';
		}
		?>
		
<?php } ?>
	
		<?php if($dados): ?>
								
			<table <?php echo $estiloTabela; ?>>
				<thead>
					<tr>
						<th colspan="<?php echo $_REQUEST['req']!='geraxls' ? 8 : 7 ?>">PROJETO MAIS MÉDICOS PARA O BRASIL</th>
					</tr>
					<tr>
						<td width="140">Ciclo</td>
						<td>Médicos Existentes</td>
						<td>Primeira Visita</td>
						<td>Supervisão Prática</td>
						<td>Não Visitados</td>
						<td>Total de Visitas</td>
						<td>Média de Visita</td>
						<?php if($_REQUEST['req']!='geraxls'): ?>
							<td><center>Visitados</center></td>
						<?php endif; ?>
					</tr>
				</thead>
				<tbody>
				<?php 
				$total_medicos=0;
				$total_primeira_visita=0;
				$total_supervisao_pratica=0;
				$sem_visita=0;
				$total_visitas=0;
				?>
				<style>
					.barra_porcentagem_fora{
						background: white;
						width:100px;
						height: 15px;
						border: 1px solid black;						
					}
					.barra_porcentagem_dentro{
						background: green;
						height: 15px;
					}
				</style>
				<?php foreach($dados as $valor): ?>
					<?php $qtde_sem_visita = $arSemVisita[$valor['numero_ciclo']]['sem_dsei'] ? $arSemVisita[$valor['numero_ciclo']]['sem_dsei'] : 0; ?>
					<tr>
						<td style="text-align:left;"><?php echo $valor['numero_ciclo']>0 ? $valor['numero_ciclo'] : ($_REQUEST['req']=='geraxls' ? '99' : 'Sem ciclo'); ?></td>
						<td style="text-align:left;" class="link_quantidade" onclick="mostrarDetalhes('f', 'total_medicos', '<?php echo $valor['numero_ciclo']; ?>')" style="cursor:pointer" title="Detalhes" alt="Detalhes"><?php echo $valor['total_medicos']; ?></td>
						<td style="text-align:left;" class="link_quantidade" onclick="mostrarDetalhes('f', 'total_primeira_visita', '<?php echo $valor['numero_ciclo']; ?>')" style="cursor:pointer" title="Detalhes" alt="Detalhes"><?php echo $valor['total_primeira_visita']; ?></td>
						<td style="text-align:left;" class="link_quantidade" onclick="mostrarDetalhes('f', 'total_supervisao_pratica', '<?php echo $valor['numero_ciclo']; ?>')" style="cursor:pointer" title="Detalhes" alt="Detalhes"><?php echo $valor['total_supervisao_pratica']; ?></td>
						<td style="text-align:left;" class="link_quantidade" onclick="mostrarDetalhes('f', 'sem_visita', '<?php echo $valor['numero_ciclo']; ?>')" style="cursor:pointer" title="Detalhes" alt="Detalhes"><?php echo $qtde_sem_visita; ?></td>
						<td style="text-align:left;" class="link_quantidade" onclick="mostrarDetalhes('f', 'total_visitas', '<?php echo $valor['numero_ciclo']; ?>')" style="cursor:pointer" title="Detalhes" alt="Detalhes"><?php echo $valor['total_primeira_visita']+$valor['total_supervisao_pratica']; ?></td>
						<td style="text-align:left;">
							<?php
							$total_postagens = $valor['total_primeira_visita']+$valor['total_supervisao_pratica'];
							$media = $valor['total_medicos']>0 ? $total_postagens/$valor['total_medicos'] : 0;
							echo number_format($media,2); 
							?>
						</td>
						<?php if($_REQUEST['req']!='geraxls'): ?>
							<td style="text-align:left;">
								<?php
								$medicos_sem_visita = $qtde_sem_visita;
								$medicos_visitados = $valor['total_medicos']-$medicos_sem_visita;
								$porcentagem = $valor['total_medicos']>0 ? ($medicos_visitados*100)/$valor['total_medicos'] : 0;
								$porcentagem = number_format($porcentagem, ($porcentagem==100 ? 0 : 2));
								?>
								<?php if($_REQUEST['req']!='geraxls') : ?>	
									<div class="barra_porcentagem_fora">
										<div class="barra_porcentagem_dentro" style="width:<?php echo $porcentagem.'%'; ?>">
											<center><font color="<?php echo $porcentagem>30 ? 'white' : 'blue';?>"><?php echo str_replace('.',',',$porcentagem); ?>%</font></center>
										</div>
									</div>
								<?php else: ?>
									<?php echo $porcentagem; ?>%
								<?php endif; ?>
							</td>
						<?php endif; ?>
					</tr>
					<?php 
					$total_medicos+=$valor['total_medicos'];
					$total_primeira_visita+=$valor['total_primeira_visita'];
					$total_supervisao_pratica+=$valor['total_supervisao_pratica'];
					$sem_visita+=$qtde_sem_visita;
					$total_visitas+=$total_postagens;
					?>
				<?php endforeach; ?>
				</tbody>
				<?php if($_REQUEST['req']!='geraxls'): ?>
					<tfoot>
						<tr <?php echo $estiloTrTotalTabela; ?>>
							<td style="text-align:left;"><b>TOTAIS:</b></td>
							<td style="text-align:left;"><b><?php echo $total_medicos; ?></b></td>
							<td style="text-align:left;"><b><?php echo $total_primeira_visita; ?></b></td>
							<td style="text-align:left;"><b><?php echo $total_supervisao_pratica; ?></b></td>
							<td style="text-align:left;"><b><?php echo $sem_visita; ?></b></td>
							<td style="text-align:left;"><b><?php echo $total_visitas; ?></b></td>
							<td>&nbsp;</td>
							<td>&nbsp;</td>						
						</tr>
					</tfoot>
				<?php endif; ?>
			</table>
			
		<?php endif; ?>
		
		<br/><br/>
		
		<?php if($dados_dsei): ?>
			
			<table <?php echo $estiloTabela; ?>>
				<thead>
					<tr>
						<th colspan="<?php echo $_REQUEST['req']!='geraxls' ? 8 : 7 ?>">DSEI</th>
					</tr>
					<tr>
						<td width="140">Ciclo</td>
						<td>Médicos Existentes</td>
						<td>Primeira Visita</td>
						<td>Supervisão Prática</td>
						<td>Não Visitados</td>
						<td>Total de Visitas</td>
						<td>Média de Visita</td>
						<?php if($_REQUEST['req']!='geraxls'): ?>
							<td><center>Visitados</center></td>
						<?php endif; ?>
					</tr>
				</thead>
				<tbody>
				<?php 
				$total_medicos=0;
				$total_primeira_visita=0;
				$total_supervisao_pratica=0;
				$sem_visita=0;
				$total_visitas=0;
				?>
				<?php foreach($dados_dsei as $valor): ?>
					<?php $qtde_sem_visita = $arSemVisita[$valor['numero_ciclo']]['com_dsei'] ? $arSemVisita[$valor['numero_ciclo']]['com_dsei'] : 0; ?>
					<tr>
						<td style="text-align:left;"><?php echo $valor['numero_ciclo']>0 ? $valor['numero_ciclo'] : ($_REQUEST['req']=='geraxls' ? '99' : 'Sem ciclo'); ?></td>
						<td style="text-align:left;" class="link_quantidade" onclick="mostrarDetalhes('t', 'total_medicos', '<?php echo $valor['numero_ciclo']; ?>')" style="cursor:pointer" title="Detalhes" alt="Detalhes"><?php echo $valor['total_medicos']; ?></td>
						<td style="text-align:left;" class="link_quantidade" onclick="mostrarDetalhes('t', 'total_primeira_visita', '<?php echo $valor['numero_ciclo']; ?>')" style="cursor:pointer" title="Detalhes" alt="Detalhes"><?php echo $valor['total_primeira_visita']; ?></td>
						<td style="text-align:left;" class="link_quantidade" onclick="mostrarDetalhes('t', 'total_supervisao_pratica', '<?php echo $valor['numero_ciclo']; ?>')" style="cursor:pointer" title="Detalhes" alt="Detalhes"><?php echo $valor['total_supervisao_pratica']; ?></td>
						<td style="text-align:left;" class="link_quantidade" onclick="mostrarDetalhes('t', 'sem_visita', '<?php echo $valor['numero_ciclo']; ?>')" style="cursor:pointer" title="Detalhes" alt="Detalhes"><?php echo $qtde_sem_visita; ?></td>
						<td style="text-align:left;" class="link_quantidade" onclick="mostrarDetalhes('t', 'total_visitas', '<?php echo $valor['numero_ciclo']; ?>')" style="cursor:pointer" title="Detalhes" alt="Detalhes"><?php echo $valor['total_primeira_visita']+$valor['total_supervisao_pratica']; ?></td>
						<td style="text-align:left;">
							<?php
							$total_postagens = $valor['total_primeira_visita']+$valor['total_supervisao_pratica'];
							$media = $valor['total_medicos']>0 ? $total_postagens/$valor['total_medicos'] : 0;
							echo number_format($media,2); 
							?>
						</td>
						<?php if($_REQUEST['req']!='geraxls'): ?>
							<td style="text-align:left;">
								<?php
								$medicos_sem_visita = $qtde_sem_visita;
								$medicos_visitados = $valor['total_medicos']-$medicos_sem_visita;
								$porcentagem = $valor['total_medicos']>0 ? ($medicos_visitados*100)/$valor['total_medicos'] : 0;
								$porcentagem = number_format($porcentagem, ($porcentagem==100 ? 0 : 2));
								?>
								<?php if($_REQUEST['req']!='geraxls') : ?>	
									<div class="barra_porcentagem_fora">
										<div class="barra_porcentagem_dentro" style="width:<?php echo $porcentagem.'%'; ?>">
											<center><font color="<?php echo $porcentagem>30 ? 'white' : 'blue';?>"><?php echo str_replace('.',',',$porcentagem); ?>%</font></center>
										</div>
									</div>
								<?php else: ?>
									<?php echo $porcentagem; ?>%
								<?php endif; ?>
							</td>
						<?php endif; ?>
					</tr>
					<?php 
					$total_medicos+=$valor['total_medicos'];
					$total_primeira_visita+=$valor['total_primeira_visita'];
					$total_supervisao_pratica+=$valor['total_supervisao_pratica'];
					$sem_visita+=$qtde_sem_visita;
					$total_visitas+=$total_postagens;
					?>
				<?php endforeach; ?>
				</tbody>
				<?php if($_REQUEST['req']!='geraxls'): ?>
					<tfoot>
						<tr <?php echo $estiloTrTotalTabela; ?>>
							<td style="text-align:left;"><b>TOTAIS:</b></td>
							<td style="text-align:left;"><b><?php echo $total_medicos; ?></b></td>
							<td style="text-align:left;"><b><?php echo $total_primeira_visita; ?></b></td>
							<td style="text-align:left;"><b><?php echo $total_supervisao_pratica; ?></b></td>
							<td style="text-align:left;"><b><?php echo $sem_visita; ?></b></td>
							<td style="text-align:left;"><b><?php echo $total_visitas; ?></b></td>
							<td>&nbsp;</td>
							<td>&nbsp;</td>
						</tr>
					</tfoot>
				<?php endif; ?>
			</table>
		<?php endif; ?>
		
<?php if($_REQUEST['req']!='geraxls') : ?>	
		</body>
	</html>
<?php endif; ?>

<?php 
if($MOSTRA_TEMPO_EXECUCAO){
	// Terminamos o "contador" e exibimos
	list($usec, $sec) = explode(' ', microtime());
	$script_end = (float) $sec + (float) $usec;
	$elapsed_time = round($script_end - $script_start, 5);
	echo '<p>&nbsp;</p><center>Tempo decorrido: ', $elapsed_time, ' secs. Memória usada: ', round(((memory_get_peak_usage(true) / 1024) / 1024), 2), 'Mb</center>';
}
?>