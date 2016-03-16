<?php 

$MOSTRA_TEMPO_EXECUCAO = $_REQUEST['requisicao']!='exibirxls' ? true : false;

if($MOSTRA_TEMPO_EXECUCAO){
	// Iniciamos o "contador"
	list($usec, $sec) = explode(' ', microtime());
	$script_start = (float) $sec + (float) $usec;
}
?>
<?php 

if($_REQUEST['carregarMunicipios'] == 'true'){
	
	extract($_POST);
	
	if($uf){
		$arWhere[] = " mdc.estuf = '{$uf}' ";
	}
	
	// Instituicao Supervisora
	if( $uniid[0] && $uniid_campo_flag ){
		$arWhere[] = " mdc.uniid ". (!$uniid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $uniid )."' ) ";
	}
	
	$sql = "select 
				coinstituicaosupervisora,
				mdcnomeinstituicao,
				mun.muncod,
				mun.mundescricao,
				coalesce(mdc.nuciclo, 999) as nuciclo,
				coalesce(count(distinct mdc.mdccpf),0) as total
			from maismedicos.medico mdc
			join territorios.municipio mun on mun.muncod = mdc.muncod
			where mdc.mdcstatus = 'A'
			and mdc.mdccpf not in (
				select distinct
					cpfprofissional
				from maismedicos.ws_respostas_formulario fom
				where fom.wssstatus = 'A'
				and fom.status = 'Finalizado'
				and fom.cpfprofissional is not null
			)		
			".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."
			group by mun.muncod, mun.mundescricao, mdc.nuciclo, coinstituicaosupervisora, mdcnomeinstituicao
			order by mun.mundescricao";

	$rs = $db->carregar($sql);
	
	$arCiclos = explode('_', $ciclos);
	
	if($rs){
		
		$arUniid = array();
		$arMuncod = array();
		
		foreach($rs as $dados){
			
			$arDados[$dados['muncod']][$dados['nuciclo']] = $dados['total'];
			$arDadosInstituicao[$dados['coinstituicaosupervisora']][$dados['nuciclo']] += $dados['total'];
			
			if(!in_array($dados['coinstituicaosupervisora'], $arUniid)){
				$arInstituicoes[$dados['coinstituicaosupervisora']] = $dados['mdcnomeinstituicao'];
				$arUniid[] = $dados['coinstituicaosupervisora'];				
			}
			
			if(!in_array($dados['muncod'], $arMuncod)){
				$arMunicipios[$dados['muncod']] = $dados['mundescricao'];
				$arMuncodUniid[$dados['coinstituicaosupervisora']][] = $dados['muncod'];				
				$arMuncod[] = $dados['muncod'];				
			}
		}
	}
	
// 	if($arMunicipios){
		
// 		foreach($arMuncod as $muncod){
// 			$totalMunicipio=0;
// 			echo '<tr class="tr_municipios_uf_'.$uf.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'\';"><td><img src="../imagens/seta_filho.gif" />'.$arMunicipios[$muncod].'</td>';
// 			foreach($arCiclos as $ciclo){
// 				$totalMunicipio += $arDados[$muncod][$ciclo];
// 				echo '<td align="center" style="cursor:pointer;color:blue;" onclick="mostraDetalhes(\''.$muncod.'\', \''.$ciclo.'\', \'muncod\')">'.($arDados[$muncod][$ciclo] ? $arDados[$muncod][$ciclo] : 0).'</td>';				
// 			}
// 			echo '<td align="center" style="color:blue;"><b>'.$totalMunicipio.'</b></td></tr>';
// 		}
// 	}	

	if($arInstituicoes){
		foreach($arUniid as $uniid){
			$totalInstituicao=0;
			echo '<tr bgcolor="#E8E8E8" class="tr_municipios_uf_'.$uf.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'#E8E8E8\';">';
			echo '<td><img src="../imagens/seta_filho.gif" />'.$arInstituicoes[$uniid].'</td>';
			foreach($arCiclos as $ciclo){
				$totalInstituicao += $arDadosInstituicao[$uniid][$ciclo];
				echo '<td align="center" style="cursor:pointer;color:blue;" onclick="mostraDetalhes(\''.$uniid.'\', \''.$ciclo.'\', \'coinstituicaosupervisora\')">'.($arDadosInstituicao[$uniid][$ciclo] ? $arDadosInstituicao[$uniid][$ciclo] : 0).'</td>';
			}
			echo '<td align="center" style="color:blue;"><b>'.$totalInstituicao.'</b></td></tr>';
			
			foreach($arMuncodUniid[$uniid] as $muncod){
				$totalMunicipio=0;
				echo '<tr bgcolor="#f0f0f0" class="tr_municipios_uf_'.$uf.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'#f0f0f0\';"><td>&nbsp;&nbsp;&nbsp;&nbsp;<img src="../imagens/seta_filho.gif" />'.$arMunicipios[$muncod].'</td>';
				foreach($arCiclos as $ciclo){
					$totalMunicipio += $arDados[$muncod][$ciclo];
					echo '<td align="center" style="cursor:pointer;color:blue;" onclick="mostraDetalhes(\''.$muncod.'\', \''.$ciclo.'\', \'muncod\')">'.($arDados[$muncod][$ciclo] ? $arDados[$muncod][$ciclo] : 0).'</td>';
				}
				echo '<td align="center" style="color:blue;"><b>'.$totalMunicipio.'</b></td></tr>';
			}
		}
	}
	die;
}

if($_REQUEST['carregaDetalhes']){
	
	if(!$_REQUEST['gerarXls']){
		
		echo "<script>
					function gerarXlsDetalhe(url)
					{
						document.location.href = url+'&gerarXls=true';
					}
				</script>";
		
		echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		  <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
		
		$titulo_modulo = "Medicos sem visita";
		monta_titulo( $titulo_modulo, '' );
		
		echo '<center><h3>'.($ciclo ? $ciclo.'º ciclo' : 'Sem ciclo').'</h3></center>';
		
		if($_REQUEST['campo']=='estuf'){
			$est = $db->pegaUm("select estdescricao from territorios.estado where estuf = '{$_REQUEST['valor']}' ");
			echo '<center><p><b>'.$est.'</b></p></center>';
		}else{
			$mun = $db->pegaUm("select mundescricao || '-' || estuf from territorios.municipio where muncod = '{$_REQUEST['valor']}' ");
			echo '<center><p><b>'.$mun.'</b></p></center>';
		}
	}
	
	extract($_GET);
	
	if($ciclo){
		$arWhere[] = ' mdc.nuciclo '.($ciclo=='999' ? 'is null' : ' = '.$ciclo);
	}

	$sql = "select 
				substr(mdccpf, 1, 3) || '.' || substr(mdccpf, 4, 3) || '.' || substr(mdccpf, 7, 3) || '-' || substr(mdccpf, 10) as mdccpf,
				mdcnome,
				mundescricao,
				mun.estuf,
				nuciclo,
				nuetapa,
				case when dsei = true then 'Sim' else 'Não' end as dsei
			from maismedicos.medico mdc
			join territorios.municipio mun on mun.muncod = mdc.muncod
			where mdcstatus = 'A'
			and mdccpf not in (
				select distinct
					cpfprofissional
				from maismedicos.ws_respostas_formulario fom
				where fom.wssstatus = 'A'
				and fom.status = 'Finalizado'
				and fom.cpfprofissional is not null 
			)
			and mdc.{$_REQUEST['campo']} = '{$_REQUEST['valor']}'
			".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."
			order by mdcnome";
	
	$arCabecalho = array('CPF', 'Nome', 'Município', 'UF', 'Ciclo', 'Etapa', 'DSEI');
	
	if($_REQUEST['gerarXls']){
		
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_MAISMEDICOS_PANORAMA_POSTAGEM_".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=SIMEC_MAISMEDICOS_SEM_VISITA_".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		
		$db->monta_lista_tabulado($sql, $arCabecalho, 100000000, 100000);
		die;
			
	}else{
		
		$db->monta_lista($sql, $arCabecalho, 100, 10, 'N', '', '');
	}

	echo '<center><p><input type="button" value="Gerar XLS" onclick="gerarXlsDetalhe(\''.$_SERVER['REQUEST_URI'].'\')" /></p></center>';
	die;
}

if($_POST){
	foreach($_POST as $k => $v){
		if(is_array($v)){
			foreach($v as $value){
				$arParams[] = $k.'[]='.$value;
			}
		}elseif($v!='exibir'){
			$arParams[] = $k.'='.$v;
		}
	}
	if($arMeses){
		foreach($arMeses as $meses){
			$arParams[] = 'meses_colunas[]='.$meses;
		}
	}
	if($arMesesOutros){
		foreach($arMesesOutros as $meses){
			$arParams[] = 'meses_outros[]='.$meses;
		}
	}
	if($arParams) $stParams = implode('&', $arParams);
}

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

$sql = "select 
			mdc.estuf,
			".($_REQUEST['requisicao'] == 'exibirxls' ? 'mundescricao,mun.muncod,' : '')."
			coalesce(mdc.nuciclo, 999) as nuciclo,
			coalesce(count(distinct mdc.mdccpf),0) as total
		from maismedicos.medico mdc
		join territorios.municipio mun on mun.muncod = mdc.muncod
		left join maismedicos.universidade u on u.idunasus = mdc.coinstituicaosupervisora  
		left join maismedicos.usuarioresponsabilidade usr on usr.uniid = u.uniid
			and usr.pflcod in (".implode(",",$arPerfil).")
		where mdc.mdcstatus = 'A'
		and mdc.mdccpf not in (
			select distinct
				cpfprofissional
			from maismedicos.ws_respostas_formulario fom
			where fom.wssstatus = 'A'
			and fom.status = 'Finalizado'
			and fom.cpfprofissional is not null
		)
		".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."
		group by mdc.estuf, mdc.nuciclo".($_REQUEST['requisicao'] == 'exibirxls' ? ',mun.muncod,mundescricao' : '')."
		order by mdc.estuf, mdc.nuciclo";

// ver($sql, d);

if($_REQUEST['requisicao'] == 'exibirxls'){ 

	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_PMMB_SEM_VISITA_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_PMMB_SEM_VISITA_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	
	$rs = $db->carregar($sql);
	
	if($rs){
		
		$arCiclos = array();
		$arMuncods = array();
		
		foreach($rs as $dados){
			
			if(!in_array($dados['nuciclo'], $arCiclos))
				$arCiclos[$dados['nuciclo']] = $dados['nuciclo'];
			
			if(!in_array($dados['muncod'], $arMuncods))
				$arMuncods[$dados['muncod']] = $dados['muncod'];
			
			$arDados[$dados['muncod']]['ciclo'][$dados['nuciclo']] = $dados['total'];
			$arDados[$dados['muncod']]['mundescricao'] = $dados['mundescricao'];
			$arDados[$dados['muncod']]['estuf'] = $dados['estuf'];
			$arDados[$dados['muncod']]['muncod'] = $dados['muncod'];
			
		}
	}
	sort($arCiclos);
	
	if($arMuncods){
		
		$html .= '<table cellpadding="0" cellspacing="0" border="1">
					<tr>
						<td rowspan="2">uf_atuacao</td>
						<td rowspan="2">IBGE</td>
						<td rowspan="2">municipio_atuacao</td>
						<td colspan="'.count($arCiclos).'" style="text-align:center">ciclos</td>
						<td rowspan="2">Total Geral</td>
					</tr>
					<tr>';
		foreach($arCiclos as $ciclo){
			$html .= '		<td>'.($ciclo=='999' ? 'Sem ciclo' : $ciclo).'</td>';
		}
		$html .= '
					</tr>';
		
		foreach($arMuncods as $muncod){
			
			$total=0;
			foreach($arCiclos as $ciclo){
				$total += $arDados[$muncod]['ciclo'][$ciclo];
			}
			
			$html .= '<tr>
						<td>'.$arDados[$muncod]['estuf'].'</td>
						<td>'.$muncod.'</td>
						<td>'.$arDados[$muncod]['mundescricao'].'</td>';
			foreach($arCiclos as $ciclo){				
				$html .= '<td>'.$arDados[$muncod]['ciclo'][$ciclo].'</td>';
			}
			$html .= '  <td>'.$total.'</td>
					  </tr>
				';
		}
		
		$html .= '</table>';
	}
	
	
// 	$arCabecalho = array('UF', 'Município', 'IBGE', 'Instituição Supervisora', 'Ciclo', 'Total');
// 	$db->monta_lista_tabulado($sql, $arCabecalho, 100000000, 10000);

	echo $html;
	die;
	
}else{

?>
	
<html>

	<head>
	
		<title>Relatório de Lista Nominal Tutor com Supervisor - Mais Médicos</title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>			
		<script>
		
			function mostrarNivel(uf, obj)
			{					
				img = $('#img_'+uf).attr('src');
				params = $('#paramentros').val();
				ciclos = $('#ciclos').val();

				if(img == '../imagens/menos.gif'){
					$('#img_'+uf).attr('src', '../imagens/mais.gif');
					$('.tr_municipios_uf_'+uf).hide();
				}else{
					$('#img_'+uf).attr('src', '../imagens/menos.gif');
					$('.tr_municipios_uf_'+uf).show();
				}

				if($('.tr_municipios_uf_'+uf).length==0){
				
					tr = $(obj).parent().parent();
					tr.after('<tr id="tr_carregando_'+uf+'"><td colspan="6">&nbsp;<img align="absmiddle" src="../imagens/carregando.gif" />&nbsp;Carregando...</td></tr>');
					
					$.ajax({
						url		: '/maismedicos/maismedicos.php?modulo=relatorio/semVisita&acao=A',
						type	: 'post',
						data	: 'requisicao=true&carregarMunicipios=true&uf='+uf+'&'+params+'&ciclos='+ciclos,
						success	: function(e){							
							tr.after(e);
							$('#tr_carregando_'+uf).remove();
							
						}
					});	
				}
			}
			
			function mostraDetalhes(valor, ciclo, campo)
			{
				var params = $('#paramentros').val();
				var url = '/maismedicos/maismedicos.php?modulo=relatorio/semVisita&acao=A&requisicao=true&carregaDetalhes=true&campo='+campo+'&'+params+'&ciclo='+ciclo+'&valor='+valor;					
				var janela = window.open( url, 'resultadoProfissionais', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
				janela.focus();
			}

		</script>
		
	</head>
	
	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
	
		<br/>
		<?php 
		$rs = $db->carregar($sql);
		
		if($rs){
		
			$arUFs = array();
			$arCiclos = array();
		
			foreach($rs as $dados){
		
				$arDados[$dados['estuf']][$dados['nuciclo']] = $dados['total'];
		
				if(!in_array($dados['nuciclo'], $arCiclos))
					$arCiclos[] = $dados['nuciclo'];
		
				if(!in_array($dados['estuf'], $arUFs))
					$arUFs[] = $dados['estuf'];
			}
		}
		
		if($arCiclos) sort($arCiclos);
		
		$titulo_modulo = "Municipio Sem Visita por UF";
		monta_titulo( $titulo_modulo, '' );
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
			
		<?php if($rs): ?>
		
			<input type="hidden" name="paramentros" id="paramentros" value="<?php echo $stParams; ?>" />
			<input type="hidden" name="ciclos" id="ciclos" value="<?php echo implode('_', $arCiclos); ?>" />
		
			<table class="listagem" cellpadding="3" cellspacing="1" width="95%" align="center">
				<thead>
					<tr>
						<th rowspan="2">UF</th>
						<th colspan="<?php echo count($arCiclos); ?>">Ciclos</th>
						<th rowspan="2">Totais</th>
					</tr>
					<tr>
						<?php foreach($arCiclos as $ciclo): ?>
							<th><?php echo $ciclo=='999' ? 'Sem ciclo' : $ciclo; ?></th>
						<?php endforeach; ?>
					</tr>
				</thead>
				<tbody>
				<?php foreach($arUFs as $i => $uf): ?>
					<?php $totalUf=0; ?>
					<tr bgcolor="<?php echo fmod($i,2) == 0 ? '' : '#F7F7F7'; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo fmod($i,2) == 0 ? '' : '#F7F7F7'; ?>';">
						<td>
							<img id="img_<?php echo $uf; ?>" src="../imagens/mais.gif" style="cursor:pointer;" onclick="mostrarNivel('<?php echo $uf; ?>', this)" />
							<?php echo $uf; ?>
						</td>
						<?php foreach($arCiclos as $ciclos): ?>
							<?php $totalCiclo{$ciclos} += $arDados[$uf][$ciclos]; ?>
							<td align="center" onclick="mostraDetalhes('<?php echo $uf; ?>', '<?php echo $ciclos; ?>', 'estuf')" style="cursor:pointer;color:blue;">
									<?php echo $arDados[$uf][$ciclos] ? $arDados[$uf][$ciclos] : 0; ?>
							</td>
							<?php $totalUf += $arDados[$uf][$ciclos]; ?>
						<?php endforeach; ?>
						<td align="center" style="color:blue;"><b><?php echo $totalUf; ?></b></td>
					</tr>
				<?php endforeach; ?>
				</tbody>	
				<tfoot>
					<tr bgcolor="#f0f0f0">
						<td>Totais</td>
						<?php $totalTotalCiclos=0; ?>						
						<?php foreach($arCiclos as $ciclos): ?>
							<td align="center" style="color:blue;"><b><?php echo $totalCiclo{$ciclos}; ?></b></td>
							<?php $totalTotalCiclos += $totalCiclo{$ciclos}; ?>
						<?php endforeach; ?>
						<td align="center" style="color:blue;"><b><?php echo $totalTotalCiclos; ?></b></td>
					</tr>
				</tfoot>			
			</table>
			
		<?php else: ?>
			
			<center><p><b>Sem registros!</b></p></center>
			
		<?php endif; ?>
		
<?php if($_REQUEST['requisicao'] != 'exibirxls'): ?>
		
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
<br/><br/>