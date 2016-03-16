<?php 

$MOSTRA_TEMPO_EXECUCAO = true;

if($MOSTRA_TEMPO_EXECUCAO){
	// Iniciamos o "contador"
	list($usec, $sec) = explode(' ', microtime());
	$script_start = (float) $sec + (float) $usec;
}
?>
<?php 
$TOTAL_COLUNAS_MESES = $_REQUEST['qtde_meses'] ? $_REQUEST['qtde_meses'] : 3;
$MOSTRAR_OUTROS = false;

if($_REQUEST['mostraPostagens']){
	
	extract($_GET);
	
	if($mes){

		$operador = $outro=='true' ? '<' : '=';
		$arWhere[] = " to_char(f.dtvisita, 'YYYYMM') {$operador} '{$mes}' ";
	}
	
	if($incluirdsei == 'on'){
		
		$arWhere[] = " f.idformulario in (".FORM_PRIMEIRAS_IMPRESSOES.",".
											FORM_PRIMEIRA_VISITA.",".
											FORM_SUPERVISAO_PRATICA.",".
											FORM_PRIMEIRAS_IMPRESSOES_DSEI.",".
											FORM_PRIMEIRA_VISITA_DSEI.",".
											FORM_SUPERVISAO_PRATICA_DSEI.") ";
		
	}else{
		
		$arWhere[] = " f.idformulario in (".FORM_PRIMEIRAS_IMPRESSOES.",".
											FORM_PRIMEIRA_VISITA.",".
											FORM_SUPERVISAO_PRATICA.") ";
		
	}
	
	if($muncod){
		$arWhere[] = " f.muncodprofissional = '{$muncod}' ";
	}
	
	if($ciclo>=0){
		$arWhere[] = " p.nuciclo ".($ciclo==0 ? ' is null ' : ' = '.$ciclo);
	}
	
	$sql = "select 
				coalesce(f.cpfsupervisor,t.tutcpf) as tutcpf,
				coalesce(f.nosupervisor,t.tutnome) as tutnome,
				coalesce(f.cpfprofissional,p.nucpf) as nucpf,
				coalesce(f.noprofissional,p.noprofissional) as noprofissional,
				coalesce(f.nuciclo,p.nuciclo) || 'º ciclo' as nuciclo,
				m.mundescricao,
				m.estuf,
				to_char(f.dtvisita, 'DD/MM/YYYY') as data
			from maismedicos.ws_respostas_formulario f
			left join maismedicos.ws_profissionais p on p.muncod = f.muncodprofissional 
				and to_char(f.dtvisita, 'YYYYMM') = to_char(p.wssdata, 'YYYYMM')
				and f.cpfprofissional = p.nucpf
				and p.situacao = 1
				and p.wssstatus = 'A'
			left join maismedicos.tutor t on f.cpfsupervisor = t.tutcpf
				and t.tutstatus = 'A'
			join territorios.municipio m on m.muncod = f.muncodprofissional
			where f.wssstatus = 'A'
			and to_char(f.dtvisita, 'YYYYMM') >= '201310'  and  to_char(f.dtvisita, 'YYYYMM') <= to_char(now(), 'YYYYMM')
			".( is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '' )."
			order by p.nuciclo";
	
// 	ver($sql, d);
	if($_REQUEST['requisicao']=='exibirxls'){
	
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_MAISMEDICOS_POSTAGENS_".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=SIMEC_MAISMEDICOS_POSTAGENS_".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
	
		$cabecalho = array('CPF do Supervisor', 'Nome do Supervisor', 'CPF do Médico', 'Nome do Médico', 'Ciclo', 'Município', 'UF', 'Mês');
		$db->monta_lista_tabulado($sql,$cabecalho,100000,5,'N','100%','');
		die;
		
	}
	
	$rs = $db->carregar($sql);
	
	monta_titulo('Relatórios', 'Mostra as respostas dos relatórios de supervisão');
	
	if($rs){
		
		echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
			  <link rel="stylesheet" type="text/css" href="../includes/listagem.css">';
		
		echo '<table class="listagem" width="95%" align="center">
				<thead>
					<tr>
						<td>CPF do Supervisor</td>
						<td>Nome do Supervisor</td>
						<td>CPF do Médico</td>
						<td>Nome do Médico</td>
						<td>Ciclo</td>
						<td>Município</td>
						<td>UF</td>
						<td>Mês</td>
					</tr>
				</head>
				<tbody>';
		
		$count=0;
		foreach($rs as $pro){
			++$count;
			echo '<tr>
					<td>'.formatar_cpf($pro['tutcpf']).'</td>
					<td>'.$pro['tutnome'].'</td>
					<td>'.formatar_cpf($pro['nucpf']).'</td>
					<td>'.$pro['noprofissional'].'</td>
					<td>'.$pro['nuciclo'].'</td>
					<td>'.$pro['mundescricao'].'</td>
					<td>'.$pro['estuf'].'</td>
					<td>'.$pro['data'].'</td>
				  </tr>';
			
			++$arCiclos[$pro['nuciclo']];
		}
		
		echo '</tbody>';
		echo '<tr><td colspan="8">'.$count.' registros.</td></tr>';
		echo '</table>';
		
		if($arCiclos){
			echo '<center><br/>';
			echo '<table>';
			foreach($arCiclos as $nuCiclo => $qtdeCiclo){
				$nuCiclo = $nuCiclo ? $nuCiclo : 'Ciclo não encontrado';
				echo '<tr><td><b><font size="3">'.$nuCiclo.':</font></b></td><td><font size="3">'.$qtdeCiclo.'</font></td></tr>';
			}
			echo '</table>';
			echo '</center>';	
		}
		
		echo '<center><p><input type="button" value="Gerar Excel" onclick="document.location.href=document.location+\'&requisicao=exibirxls\'" /></p></center>';
	}
	die;
}

if($_REQUEST['mostraMedicos']){
	
	extract($_GET);
	
	if($mes){
		$arWhere[] = " to_char(p.wssdata, 'YYYYMM') = '{$mes}' ";
	}else{
		$arWhere[] = " to_char(p.wssdata, 'YYYY-MM-DD') = (select to_char(max(wssdata), 'YYYY-MM-DD') from maismedicos.ws_profissionais where wssstatus = 'A') ";
	}
	
	if($muncod){
		$arWhere[] = " p.muncod = '{$muncod}' ";
	}
	
	if($ciclo>=0){
		$arWhere[] = " p.nuciclo ".($ciclo ? ' = '.$ciclo : ' is null');
	}
	
	$sql = "select distinct				
				p.nucpf,
				p.noprofissional,
				p.nuciclo || 'º ciclo' as ciclo,				
				m.mundescricao,
				m.estuf,
				to_char(p.wssdata, 'DD/MM/YYYY') as data
			from maismedicos.ws_profissionais p
			join territorios.municipio m on m.muncod = p.muncod
			where p.wssstatus = 'A'
			and p.situacao = 1			
			".( is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."
			order by p.nuciclo || 'º ciclo'";
	
	if($_REQUEST['requisicao']=='exibirxls'){
	
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_MAISMEDICOS_MEDICOS_".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=SIMEC_MAISMEDICOS_MEDICOS_".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
	
		$cabecalho = array('CPF do Médico', 'Nome do Médico', 'Ciclo', 'Município', 'UF', 'Mês');
		$db->monta_lista_tabulado($sql,$cabecalho,100000,5,'N','100%','');
		die;
	}
	
	$rs = $db->carregar($sql);
	
	monta_titulo('Médicos', 'Mostra as informações dos médicos ativos');
	
	if($rs){
	
		echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
			  <link rel="stylesheet" type="text/css" href="../includes/listagem.css">';
	
		echo '<table class="listagem" width="95%" align="center">
				<thead>
					<tr>
						<td>CPF</td>
						<td>Nome</td>
						<td>Ciclo</td>
						<td>Município</td>
						<td>UF</td>
						<td>Mês</td>
					</tr>
				</head>
				<tbody>';
	
		$count=0;
		foreach($rs as $pro){
			++$count;
			echo '<tr>
					<td>'.formatar_cpf($pro['nucpf']).'</td>
					<td>'.$pro['noprofissional'].'</td>
					<td>'.$pro['ciclo'].'</td>
					<td>'.$pro['mundescricao'].'</td>
					<td>'.$pro['estuf'].'</td>
					<td>'.$pro['data'].'</td>
				  </tr>';
			
			++$arCiclos[$pro['ciclo']];
		}
	
		echo '</tbody>';
		echo '<tfoot>';
		echo '<tr><td colspan="6">'.$count.' registros.</td></tr>';
		echo '</tfoot>';
		echo '</table>';
	}
	
	if($arCiclos){
		echo '<center><br/>';
		echo '<table>';
		foreach($arCiclos as $nuCiclo => $qtdeCiclo){
			echo '<tr><td><b><font size="3">'.$nuCiclo.'</font></b></td><td><font size="3">'.$qtdeCiclo.'</font></td></tr>';
		}
		echo '</table>';
		echo '</center>';	
	}
	
	echo '<center><p><input type="button" value="Gerar Excel" onclick="document.location.href=document.location+\'&requisicao=exibirxls\'" /></p></center>';
	die;
}

if($_REQUEST['carregarCiclos']){
	
	extract($_REQUEST);
	
	if($muncodCiclo){
		$arWhere[] = "pro.muncod = '{$muncodCiclo}'";
	}
	
	if($incluirdsei == 'on'){
		$arWhereForm[] = "fom.idformulario in (33,35,36,39,40,43)";
	}else{
		$arWhereForm[] = "fom.idformulario in (33,35,36)";
	}
	
	if($ciclo){
		$arWhere[] = "pro.nuciclo = '{$ciclo}'";
	}	
	
	// Instituicao supervisora
	if( $uniid[0] && $uniid_campo_flag ){
		$arWhereForm[] = " fom.muncodprofissional in (select muncod from maismedicos.universidademunicipio where uniid ". (!$uniid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $uniid )."' )) ";
	}
	
	// Supervisor
	if( $supervisor[0] && $supervisor_campo_flag ){
		$arWhereForm[] = " fom.cpfsupervisor ". (!$supervisor_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $supervisor )."' ) ";
	}
	
	// Medico
	if( $medico[0] && $medico_campo_flag ){
		$arWhere[] = " pro.nucpf ". (!$medico_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $medico )."' ) ";
	}
	
	$sql = "select 			
			coalesce(pro.nuciclo,0) as ciclo,
			to_char(pro.wssdata,'YYYYMM') as mes_visita,
			count(distinct fom.idpublicarformulario) as postagens,
			count(distinct pro.nucpf) as medicos
		from maismedicos.ws_profissionais pro
		left join maismedicos.ws_respostas_formulario fom on to_char(pro.wssdata, 'YYYYMM') = to_char(fom.dtvisita, 'YYYYMM')				
			and pro.muncod = fom.muncodprofissional
			and coalesce(pro.nuciclo,0)  = coalesce(fom.nuciclo,0)
			and fom.wssstatus = 'A'
			and fom.status = 'Finalizado'
		 			
			".(is_array($arWhereForm) ? ' and '.implode(' and ',$arWhereForm) : '')."
						  		
		where pro.wssstatus = 'A'
		and pro.situacao = 1
					
		".(is_array($arWhere) ? ' and '.implode(' and ',$arWhere) : '')."
		 
		group by pro.nuciclo, to_char(pro.wssdata,'YYYYMM')
		order by pro.nuciclo";
	
// 	ver($sql, d);
	$rsCiclos = $db->carregar($sql);
	
	$arMeses = $_POST['meses_colunas'];
	$arMesesOutros = $_POST['meses_outros'];
	
	if($rsCiclos){
		
		foreach($rsCiclos as $ciclo){
			$arCiclos[$ciclo['ciclo']][$ciclo['mes_visita']]['ciclo'] = $ciclo['ciclo'];
			$arCiclos[$ciclo['ciclo']][$ciclo['mes_visita']]['medicos'] = $ciclo['medicos'];
			$arCiclos[$ciclo['ciclo']][$ciclo['mes_visita']]['postagens'] = $ciclo['postagens'];
		}
		
		if($arCiclos){
			foreach($arCiclos as $ciclo => $dados){
				echo '<tr class="tr_cliclos_muncod_'.$muncodCiclo.'">';
				echo '<td colspan="2" style="background:#F5F5F5">&nbsp;&nbsp;&nbsp;&nbsp;<img src="../imagens/seta_filho.gif" />&nbsp;'.($ciclo>0 ? $ciclo.'º Ciclo' : 'Sem ciclo').'</td>';
				
				$indice=0;
				$totalPostagens=0;
				foreach($arMeses as $mes){
						
					echo '<td class="numero" style="background:#FBFBFB" ><a href="javascript:void(0)" onclick="mostraDetalhes(\''.$muncodCiclo.'\', \''.$mes.'\', \'mostraPostagens\', \''.str_replace(array('º Ciclo','Sem ciclo'),array('','null'),$ciclo).'\')">'.($dados[$mes]['postagens']>0 ? $dados[$mes]['postagens'] : 0).'</a></td>';
					echo '<td class="numero" style="background:#EEEEEE" ><a href="javascript:void(0)" onclick="mostraDetalhes(\''.$muncodCiclo.'\', \''.$mes.'\', \'mostraMedicos\', \''.str_replace(array('º Ciclo','Sem ciclo'),array('','null'),$ciclo).'\')">'.($dados[$mes]['medicos']>0 ? $dados[$mes]['medicos'] : 0).'</a></td>';
					$totalPostagens += $dados[$mes]['postagens'];
					$indice++;					
				}
				
				if($arMesesOutros && $MOSTRAR_OUTROS){
					$postagensOutros=0;								
					foreach($arMesesOutros as $mes){ 
						$postagensOutros += $dados[$mes]['postagens']; 
					}													
					echo '<td class="numero" bgcolor="white">'.($postagensOutros ? '<a href="javascript:void(0)" onclick="mostraDetalhes(\''.$muncod.'\', \'outros_'.$mesFinalOutros.'\', \'mostraPostagens\')" title="Ver detalhes  das postagens" alt="Ver detalhes  das postagens">'.$postagensOutros.'</a>' : 0).'</td>';
					$totalPostagens += $postagensOutros; 
				}
				
				$totalMedicos = $db->pegaUm("select count(distinct mdccpf) from maismedicos.medico where mdcstatus = 'A' and muncod = '{$muncodCiclo}' and nuciclo ".($ciclo>0 ? '='.$ciclo : 'is null'));				
				echo '<td class="numero">'.($totalPostagens ? $totalPostagens : 0).'</td>';
				echo '<td class="numero">'.($totalMedicos ? $totalMedicos : 0).'</td>';
				echo '<tr>';
			}
		}
	}	
	die;
}
	
extract($_REQUEST);

// Periodo
$mesvisitainicio = str_pad($mesvisitainicio, 2, "0", STR_PAD_LEFT);
$mesvisitafim = str_pad($mesvisitafim, 2, "0", STR_PAD_LEFT);

if($mesvisitainicio && $anovisitainicio && empty($mesvisitafim) && empty($anovisitafim)){

	$arWhere[] = " to_char(fom.dtvisita, 'YYYYMM') = '{$anovisitainicio}{$mesvisitainicio}' ";	
	$label .= '<tr><td align="right"><b>Período</b></td><td align="left">'.$mesvisitainicio.'/'.$anovisitainicio.'</td></tr>';

}elseif($mesvisitainicio && $anovisitainicio && $mesvisitafim && $anovisitafim){
	$TOTAL_COLUNAS_MESES = 10000;
	$arWhere[] = " to_char(fom.dtvisita, 'YYYYMM') >= '{$anovisitainicio}{$mesvisitainicio}' and to_char(fom.dtvisita, 'YYYYMM') <= '{$anovisitafim}{$mesvisitafim}' ";
	$label .= '<tr><td align="right"><b>Período</b></td><td align="left">'.$mesvisitainicio.'/'.$anovisitainicio.' até '.$mesvisitafim.'/'.$anovisitafim.'</td></tr>';

}else{

	$menorMes = date( "Ym", strtotime( "-{$TOTAL_COLUNAS_MESES} month" ) );
	$menorMes = $menorMes<'201310' ? '201310' : $menorMes;
	$arWhere[] = " to_char(fom.dtvisita, 'YYYYMM') >= '{$menorMes}' and to_char(fom.dtvisita, 'YYYYMM') <= to_char(now(), 'YYYYMM')";	
}

// Municipio
if( $muncod[0] && $muncod_campo_flag ){
	$arWhere[] = " fom.muncodprofissional ". (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $muncod )."' ) ";
	$label .= '<tr><td align="right"><b>Município(s)</b></td><td align="left">'.implode('<br/>', $db->carregarColuna("select mundescricao from territorios.municipio where muncod in ('".implode( "','", $muncod )."')")).'</td></tr>';
}

// UF
if($estuf){
	$arWhere[] = " fom.estufprofissional = '{$estuf}' ";
	$label .= '<tr><td align="right"><b>UF</b></td><td align="left">'.$estuf.'</td></tr>';
}

// Ciclo
if($ciclo){
	$arWhere[] = " fom.nuciclo = {$ciclo} ";
	$label .= '<tr><td align="right"><b>Ciclo</b></td><td align="left">'.$ciclo.'</td></tr>';
}

// Somente capitais
if($socapitais == 'on'){
	$arWhere[] = " fom.muncodprofissional in ('3550308','3304557','2927408','5300108','2304400','3106200','1302603','4106902','2611606','4314902','1501402','5208707',
								'2111300','2704302','2408102','2211001','5002704','2507507','2800308','5103403','1100205','4205407','1600303','1200401',
								'3205309','1400100','1721000') ";
	
	$label .= '<tr><td align="right"><b>Só capitais</b></td><td align="left">Sim</td></tr>';
	
}

// Instituicao supervisora
if( $uniid[0] && $uniid_campo_flag ){
	$arWhere[] = " fom.muncodprofissional in (select muncod from maismedicos.universidademunicipio where uniid ". (!$uniid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $uniid )."' )) ";
	$label .= '<tr><td align="right"><b>Instituição(ões) Supervisora(s)</b></td><td align="left">'.implode('<br/>', $db->carregarColuna("select uninome from maismedicos.universidade where uniid in ('".implode( "','", $uniid )."')")).'</td></tr>';
}

// Supervisor
if( $supervisor[0] && $supervisor_campo_flag ){
	$arWhere[] = " fom.cpfsupervisor ". (!$supervisor_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $supervisor )."' ) ";
	$label .= '<tr><td align="right"><b>Supervisor(es)</b></td><td align="left">'.implode('<br/>', $db->carregarColuna("select tutnome from maismedicos.tutor where tutcpf in ('".implode( "','", $supervisor )."')")).'</td></tr>';
}

// Medico
if( $medico[0] && $medico_campo_flag ){
	$arWhere[] = " fom.cpfprofissional ". (!$medico_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $medico )."' ) ";
	$label .= '<tr><td align="right"><b>Médico(s)</b></td><td align="left">'.implode('<br/>', $db->carregarColuna("select mdcnome from maismedicos.medico where mdccpf in ('".implode( "','", $medico )."')")).'</td></tr>';
}

// Mostrar DSEI
if($incluirdsei == 'on'){

	$arWhere[] = " fom.idformulario in (".FORM_PRIMEIRAS_IMPRESSOES.",".
											FORM_PRIMEIRA_VISITA.",".
											FORM_SUPERVISAO_PRATICA.",".
											FORM_PRIMEIRAS_IMPRESSOES_DSEI.",".
											FORM_PRIMEIRA_VISITA_DSEI.",".
											FORM_SUPERVISAO_PRATICA_DSEI.") ";
	
	$label .= '<tr><td align="right"><b>Mostrar DSEI</b></td><td align="left">Sim</td></tr>';

}else{

	$arWhere[] = " fom.idformulario in (".FORM_PRIMEIRAS_IMPRESSOES.",".
											FORM_PRIMEIRA_VISITA.",".
											FORM_SUPERVISAO_PRATICA.") ";
}

if($_REQUEST['requisicao']!='exibirxls'){
	$leftjoin = "left join maismedicos.ws_profissionais pro on to_char(pro.wssdata, 'YYYYMM') = to_char(fom.dtvisita, 'YYYYMM')				
				and pro.muncod = fom.muncodprofissional
				and pro.wssstatus = 'A'
				and pro.situacao = 1";
}

$arPerfil = arrayPerfil();
if(in_array(PERFIL_TUTOR, $arPerfil)){
	$arWhere[] = "usr.usucpf = '{$_SESSION['usucpf']}'";
	$arWhereTotal[] = "usr.usucpf = '{$_SESSION['usucpf']}'";
}else
	if(in_array(PERFIL_SUPERVISOR, arrayPerfil())){
	$arWhere[] = "fom.cpfsupervisor = '{$_SESSION['usucpf']}'";
}

$sql = "select 
			coalesce(mun.estuf, '-') as estuf,
			mun.mundescricao || ' - ' || mun.estuf as mundescricao,
			fom.muncodprofissional as muncod,
			".($_REQUEST['requisicao']!='exibirxls' ? "to_char(fom.dtvisita,'YYYYMM') as mes_visita,count(distinct pro.nucpf) as medicos," : "")."
			count(distinct fom.idpublicarformulario) as postagens,			
			(select count(distinct mdc.mdccpf) from maismedicos.medico mdc where mdc.muncod = fom.muncodprofissional and mdcstatus = 'A') as total_medicos_municipio
		from maismedicos.ws_respostas_formulario fom
		left join maismedicos.usuarioresponsabilidade usr on usr.uniid = fom.uniid
				and usr.pflcod in (".implode(",",$arPerfil).")
		{$leftjoin}
		left join territorios.municipio mun on mun.muncod = fom.muncodprofissional
		where fom.wssstatus = 'A'
		and fom.status = 'Finalizado'	
		".(is_array($arWhere) ? ' and '.implode(' and ',$arWhere) : '')."		 
		group by mun.estuf, mun.mundescricao, fom.muncodprofissional ".($_REQUEST['requisicao']!='exibirxls' ? ",to_char(fom.dtvisita,'YYYYMM')" : "")."
		order by mun.estuf, mun.mundescricao";
// 	ver($sql, d);
if($_REQUEST['requisicao']=='exibirxls'){
	
	$arCabecalho = array('UF', 'MUNICÍPIO', 'IBGE', 'RELATÓRIOS POSTADOS', 'QUANTIDADE DE MÉDICOS ATIVOS NO MUNICÍPIO');
	$arTipo = array();
	gerar_excel($sql, $arCabecalho, 'SIMEC_PMMB_VISITAS_MUNICIPIO_'.date("Ymdhis"), $arTipo);
	die;
			
}else{
	
	?>
	
	<html>
		<head>
			<title> Sistema Integrado de Monitoramento do Ministério da Educação </title>
			<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
			<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
			<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
			
			<script>
			function mostrarCiclo(muncod, obj, meses)
			{					
				img = $('#img_'+muncod).attr('src');
				params = $('#params').val();

				if(img == '../imagens/menos.gif'){
					$('#img_'+muncod).attr('src', '../imagens/mais.gif');
					$('.tr_cliclos_muncod_'+muncod).hide();
				}else{
					$('#img_'+muncod).attr('src', '../imagens/menos.gif');
					$('.tr_cliclos_muncod_'+muncod).show();
				}

				if($('.tr_cliclos_muncod_'+muncod).length==0){
				
					tr = $(obj).parent().parent();
					tr.after('<tr id="tr_carregando_'+muncod+'"><td colspan="6">&nbsp;<img align="absmiddle" src="../imagens/carregando.gif" />&nbsp;Carregando...</td></tr>');
					
					$.ajax({
						url		: '/maismedicos/maismedicos.php?modulo=relatorio/visitasMunicipio&acao=A',
						type	: 'post',
						data	: 'requisicao=true&carregarCiclos=true&muncodCiclo='+muncod+'&meses='+meses+'&'+params,
						success	: function(e){							
							tr.after(e);
							$('#tr_carregando_'+muncod).remove();
							
						}
					});	
				}
			}

			function mostraDetalhes(muncod, mes, tipo, ciclo)
			{
				
				ciclo = ciclo ? ciclo : -1;
				
				outro = 'false';
				if(mes.match(/outro/)){
					arParamMes = mes.split('_');
					mes = arParamMes[1];
					outro = 'true'; 
				}				
					
				var janela = window.open( '/maismedicos/maismedicos.php?modulo=relatorio/visitasMunicipio&acao=A&muncod='+muncod+'&mes='+mes+'&'+tipo+'=true&requisicao=true'+'&ciclo='+ciclo+'&outro='+outro, 'resultadoProfissionais', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
				janela.focus();
			}

			</script>
			<style>
				.numero{
					text-align: right;
					color: blue;
				}
			</style>
		</head>
		<body>
		<?php
		
		$linha1 = 'Relatório Visitas Município';
		$linha2 = '';
		monta_titulo($linha1, $linha2);
				
		if($label){
			echo '<center>';
			echo '<b>FILTRO(S):</b>';
			echo '<table cellspacing="1" cellpadding="3" align="center">';
			echo $label;
			echo '</table>';
			echo '</center>';
		}
		
		$rsVisita = $db->carregar($sql);
		
		if($rsVisita){			
			foreach($rsVisita as $visita){
				$arVisita[$visita['muncod']]['estuf'] = $visita['estuf'];
				$arVisita[$visita['muncod']]['mundescricao'] = $visita['mundescricao'];
				$arVisita[$visita['muncod']]['total_medicos_municipio'] = $visita['total_medicos_municipio'];
				$arVisita[$visita['muncod']][$visita['mes_visita']]['postagens'] = $visita['postagens'];
				$arVisita[$visita['muncod']][$visita['mes_visita']]['medicos'] = $visita['medicos'];				
				$arMesesTmp[$visita['mes_visita']] = $visita['mes_visita'];
			}
		}
		
		if($arMesesTmp) rsort($arMesesTmp);
		
		$totalMeses=0;
		if($arMesesTmp){			
			foreach($arMesesTmp as $k=>$v){
				if(!in_array($v, array('000000','999999'))){
					if($totalMeses<$TOTAL_COLUNAS_MESES&&!in_array($v, array('000000','999999'))){
						$arMeses[$v] = $v;
					}else{
						$arMesesOutros[$v] = $v;
					}				
					$totalMeses++;
				}else{
					
				}
			}
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
		
// 		ver($sql, $arMesesTmp, $arMeses, $arMesesOutros, d);
		?>
		
		<?php if($arVisita): ?>
			<style>
				thead{
					text-align: center;
				}
			</style>
			<input type="hidden" name="params" id="params" value="<?php echo $stParams; ?>" />
			<table <?php if($_REQUEST['requisicao']!='exibirxls') echo 'class="listagem" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width:95%"'; ?> >
				<thead>
					<tr>
						<td rowspan="2" width="40">UF</td>
						<td rowspan="2">Município</td>
						
						<?php foreach($arMeses as $mes): ?>
							<td colspan="2"><?php echo substr($mes,4,2).'/'.substr($mes,0,4); ?></td>		
						<?php endforeach; ?>
						
						<?php if($totalMeses>$TOTAL_COLUNAS_MESES && $MOSTRAR_OUTROS): ?>
							<td rowspan="2">Outros meses<br/>Qtde. de Relatórios Postados</td>
						<?php endif; ?>
						
						<td rowspan="2">Total de Relatórios Postados </td>
						<td rowspan="2">Total de Médicos Ativos no Município </td>
					</tr>
					<tr>					
						<?php foreach($arMeses as $mes): ?>
							<td>Qtde. de Relatórios Postados</td>
 							<td>Qtde. de Médicos Ativos no Município</td>
						<?php endforeach; ?>
					</tr>
				</thead>
				
				<tbody>	
					<?php 
					$totalTotalMedicos=0; 	
					$totalTotalPostagens=0; 
					?>		
					<?php foreach($arVisita as $muncod => $visita): ?>
						<tr>						
							<td>
								<img id="img_<?php echo $muncod; ?>" src="../imagens/mais.gif" onclick="javascript:mostrarCiclo('<?php echo $muncod ?>', this, '<?php echo implode('_',$arMeses); ?>')" style="cursor:pointer" />
								&nbsp;<?php echo $visita['estuf']; ?>
							</td>
							
							<td><?php echo $visita['mundescricao']; ?></td>
							
							<?php $totalPostagens=0; $totalMedicos=0; $countMes=1;?>
							<?php foreach($arMeses as $mes): ?>
								<td class="numero" bgcolor="white"><?php echo $arVisita[$muncod][$mes]['postagens'] ? '<a href="javascript:void(0)" onclick="mostraDetalhes(\''.$muncod.'\', \''.$mes.'\', \'mostraPostagens\')" title="Ver detalhes  das postagens" alt="Ver detalhes  das postagens">'.$arVisita[$muncod][$mes]['postagens'].'</a>' : 0; ?></td>
								<td class="numero"><?php echo $arVisita[$muncod][$mes]['medicos'] ? '<a href="javascript:void(0)" onclick="mostraDetalhes(\''.$muncod.'\', \''.$mes.'\', \'mostraMedicos\')" title="Ver detalhes dos médicos" alt="Ver detalhes dos médicos">'.$arVisita[$muncod][$mes]['medicos'].'</a>' : 0; ?></td>
								<?php $totalPostagens += $arVisita[$muncod][$mes]['postagens']; ?>
								<?php $totalMedicos += $arVisita[$muncod][$mes]['medicos']; ?>
								<?php $varPostagensMes = 'postagens'.$mes; ${$varPostagensMes} += $arVisita[$muncod][$mes]['postagens']; ?>
								<?php $varMedicoMes = 'medicos'.$mes; ${$varMedicoMes} += $arVisita[$muncod][$mes]['medicos']; ?>
								<?php if(count($arMeses)==$countMes) $mesFinalOutros = $mes; $countMes++; ?>
							<?php endforeach; ?>			
											
							<?php if($totalMeses>$TOTAL_COLUNAS_MESES && $MOSTRAR_OUTROS): ?>								
								<?php 
								$postagensOutros=0;								
								foreach($arMesesOutros as $mes){ 
									$postagensOutros += $arVisita[$muncod][$mes]['postagens']; 
								}
								?>								
								<td class="numero" bgcolor="white"><?php echo $postagensOutros ? '<a href="javascript:void(0)" onclick="mostraDetalhes(\''.$muncod.'\', \'outros_'.$mesFinalOutros.'\', \'mostraPostagens\')" title="Ver detalhes  das postagens" alt="Ver detalhes  das postagens">'.$postagensOutros.'</a>' : 0; ?></td>
								<?php $totalPostagens += $postagensOutros; ?>
							<?php endif; ?>
							
							<?php $totalMedicos = $arVisita[$muncod]['total_medicos_municipio']; ?>
							<td class="numero" bgcolor="white"><?php echo $totalPostagens ? $totalPostagens : 0; ?></td>
							<td class="numero" bgcolor="white"><?php echo $totalMedicos ? $totalMedicos : 0; ?></td>
						</tr>
						<?php $totalTotalPostagens += $totalPostagens; ?>
						<?php $totalTotalMedicos += $totalMedicos; ?>
						<?php $totalTotalOutros += $postagensOutros; ?>
					<?php endforeach; ?>
				</tbody>
				<tfoot>
					<tr>						
						<td colspan="2" style="color:blue;">
							<b><?php echo count($arVisita); ?>&nbsp;município(s)</b>
						</td>
						<?php foreach($arMeses as $mes): ?>
							<?php $varPostagensMes = 'postagens'.$mes; ?>
							<?php $varMedicoMes = 'medicos'.$mes; ?>
							<td class="numero"><b><?php echo ${$varPostagensMes}; ?></b></td>
							<td class="numero"><b><?php echo ${$varMedicoMes}; ?></b></td>
						<?php endforeach; ?>
						<?php if($totalMeses>$TOTAL_COLUNAS_MESES&&$MOSTRAR_OUTROS): ?>
							<td class="numero"><b><?php echo $totalTotalOutros ? $totalTotalOutros : 0; ?></b></td>
						<?php endif; ?>
						<td class="numero"><b><?php echo $totalTotalPostagens ? $totalTotalPostagens : 0; ?></b></td>
						<td class="numero"><b><?php echo $totalTotalMedicos ? $totalTotalMedicos : 0; ?></b></td>
					</tr>
				</tfoot>
			</table>
		<?php else: ?>
			<center><p>Sem registros!</p></center>
		<?php endif; ?>
		
		</body>
	</html>
	<?php
}
?>

<?php 
if($MOSTRA_TEMPO_EXECUCAO){
	// Terminamos o "contador" e exibimos
	list($usec, $sec) = explode(' ', microtime());
	$script_end = (float) $sec + (float) $usec;
	$elapsed_time = round($script_end - $script_start, 5);
	echo '<p>&nbsp;</p><center>Tempo decorrido: ', $elapsed_time, ' secs. Memória usada: ', round(((memory_get_peak_usage(true) / 1024) / 1024), 2), 'Mb</center>';
}
?>