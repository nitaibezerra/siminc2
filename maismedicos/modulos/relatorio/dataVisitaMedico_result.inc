<?php 

$MOSTRA_TEMPO_EXECUCAO = $_REQUEST['requisicao']!='exibirxls' ? true : false;

set_time_limit(100000);
ini_set("memory_limit", "10000M");

if($MOSTRA_TEMPO_EXECUCAO){
	// Iniciamos o "contador"
	list($usec, $sec) = explode(' ', microtime());
	$script_start = (float) $sec + (float) $usec;
}
?>
<?php 

if($_REQUEST['mostrarRespostas']){	
	include 'medicosPorSupervisor_respostas.inc';
	die;
}

if($_POST) extract($_POST);

if($_REQUEST['carregarDatas']){
	
	$cpf = str_replace(array('.','-'), '', $cpf);
	
	$sql = "select distinct
				idpublicarformulario,
				to_char(dtvisita, 'DD/MM/YYYY') as data_visita,
				to_char(dtenvio, 'DD/MM/YYYY HH24:MI:SS') as data_envio,
				to_char(coalesce(dtultimaatualizacao,dtpublicacao), 'DD/MM/YYYY HH24:MI:SS') as data_atualizacao,
				dtvisita,
				status,
				nuciclo,
				nuetapa,
				estufprofissional,
				nomunicipioprofissional,
				noinstituicao
			from maismedicos.ws_respostas_formulario
			where wssstatus = 'A'
			and cpfprofissional = '{$cpf}'
			order by dtvisita desc";
	
	$rs = $db->carregar($sql);
	
	if($rs){
		
	
		echo '<tr class="tr_medico_cpf_'.$cpf.'"><td colspan="7">';
		echo '<table width="100%" cellpadding="3" cellspacing="1" align="center">
				<thead>
					<tr>
						<td align="center">PDF</td>
						<td align="center">Visita</td>
						<td align="center">Envio</td>
						<td align="center">Ciclo</td>
						<td align="center">Etapa</td>
						<td align="center">Instituição Supervisora</td>
						<td align="center">Município</td>
						<td align="center">UF</td>
						<td align="center">Cod. formulário</td>
						<td align="center">Status</td>
					</tr>
				</thead>';
		
		foreach($rs as $dados){	
			echo '<tr onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'\';">		
						 <td>&nbsp;<img src="../imagens/seta_filho.gif" />&nbsp;<img src="../imagens/acrobat.gif" title="Mostrar respostas" alt="Mostrar respostas" style="cursor:pointer;" class="abrirRespostas" id="'.$dados['idpublicarformulario'].'"/>&nbsp;</td>
						 <td>'.($dados['data_visita'] ? $dados['data_visita'] : 'Não preenchido').'</td>
						 <td>'.($dados['data_envio'] ? $dados['data_envio'] : 'Não preenchido').'</td>						 		
						 <td align="center">'.($dados['nuciclo'] ? $dados['nuciclo'] : 'Não preenchido').'</td>
						 <td align="center">'.($dados['nuetapa'] ? $dados['nuetapa'] : 'Não preenchido').'</td>
						 <td>'.($dados['noinstituicao'] ? $dados['noinstituicao'] : 'Não preenchido').'</td>
						 <td>'.($dados['nomunicipioprofissional'] ? $dados['nomunicipioprofissional'] : 'Não preenchido').'</td>
						 <td align="center">'.($dados['estufprofissional'] ? $dados['estufprofissional'] : 'Não preenchido').'</td>						 		
						 <td align="center">'.($dados['idpublicarformulario'] ? $dados['idpublicarformulario'] : 'Não preenchido').'</td>
						 <td>'.$dados['status'].'</td>
					 </tr>';
			
		}
		
		echo '</table>';
		echo '</td></tr>';
	}	
	die;
}

// Periodo
$mesvisitainicio = str_pad($mesvisitainicio, 2, "0", STR_PAD_LEFT);
$mesvisitafim = str_pad($mesvisitafim, 2, "0", STR_PAD_LEFT);

if($mesvisitainicio && $anovisitainicio && empty($mesvisitafim) && empty($anovisitafim)){

	$label .= '<tr><td align="right"><b>Período</b></td><td align="left">'.$mesvisitainicio.'/'.$anovisitainicio.'</td></tr>';
	$arWhere[] = " to_char(fom.dtvisita, 'YYYYMM') = '{$anovisitainicio}{$mesvisitainicio}' ";

}elseif($mesvisitainicio && $anovisitainicio && $mesvisitafim && $anovisitafim){

	$label .= '<tr><td align="right"><b>Período</b></td><td align="left">'.$mesvisitainicio.'/'.$anovisitainicio.' até '.$mesvisitafim.'/'.$anovisitafim.'</td></tr>';
	$arWhere[] = " to_char(fom.dtvisita, 'YYYYMM') >= '{$anovisitainicio}{$mesvisitainicio}' and to_char(fom.dtvisita, 'YYYYMM') <= '{$anovisitafim}{$mesvisitafim}' ";

}else{

	if($mostrardtvazia == 'on'){
		$label .= '<tr><td align="right"><b>Mostrar data de visita vazia</b></td><td align="left">Sim</td></tr>';
		$dtVisitaFiltro[] = " (fom.dtvisita is null) ";
	}
	
	if($mostrardterro == 'on'){
		$label .= '<tr><td align="right"><b>Mostrar data de visita com erro</b></td><td align="left">Sim</td></tr>';
		$dtVisitaFiltro[] = " (to_char(fom.dtvisita, 'YYYYMM') < '201310' or to_char(fom.dtvisita, 'YYYYMM') > to_char(now(), 'YYYYMM')) ";
	}
	
	$dtVisitaFiltro[] = " (to_char(fom.dtvisita, 'YYYYMM') >= '201310' and to_char(fom.dtvisita, 'YYYYMM') <= to_char(now(), 'YYYYMM')) ";
	
	$arWhereForm[] = " (".implode(' or ', $dtVisitaFiltro).") ";

}

// Mostrar DSEI
if($incluirdsei == 'on'){

	$arWhereForm[] = " fom.idformulario in (".FORM_PRIMEIRA_VISITA.",".
										FORM_SUPERVISAO_PRATICA.",".
										FORM_PRIMEIRA_VISITA_DSEI.",".
										FORM_SUPERVISAO_PRATICA_DSEI.") ";

}else{

	$arWhereForm[] = " fom.idformulario in (".FORM_PRIMEIRA_VISITA.",".
										FORM_SUPERVISAO_PRATICA.") ";
}

// UF
if($estuf){
	$arWhere[] = " fom.estufprofissional = '{$estuf}' ";
	$label .= '<tr><td align="right"><b>UF</b></td><td align="left">'.$estuf.'</td></tr>'; 
}

// Municipio
if( $muncod[0] && $muncod_campo_flag ){
	$arWhere[] = " fom.muncodprofissional ". (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $muncod )."' ) ";
	$label .= '<tr><td align="right"><b>Município(s)</b></td><td align="left">'.implode('<br/>', $db->carregarColuna("select mundescricao from territorios.municipio where muncod in ('".implode( "','", $muncod )."')")).'</td></tr>';
}

// So capitais
if($socapitais == 'on'){
	$arWhere[] = " fom.muncodprofissional in ('3550308','3304557','2927408','5300108','2304400','3106200','1302603','4106902','2611606','4314902','1501402','5208707',
								'2111300','2704302','2408102','2211001','5002704','2507507','2800308','5103403','1100205','4205407','1600303','1200401',
								'3205309','1400100','1721000')";
	$label .= '<tr><td align="right"><b>Só capitais</b></td><td align="left">Sim</td></tr>';
}

// Instituicao Supervisora
if( $uniid[0] && $uniid_campo_flag ){
	$arWhere[] = " fom.uniid ". (!$uniid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $uniid )."' ) ";
	$label .= '<tr><td align="right"><b>Instituição(ões) Supervisora(s)</b></td><td align="left">'.implode('<br/>', $db->carregarColuna("select uninome from maismedicos.universidade where uniid in ('".implode( "','", $uniid )."')")).'</td></tr>';
}

if($nome){
	$arWhere[] = " fom.noprofissional ilike '%{$nome}%' ";
	$label .= '<tr><td align="right"><b>Nome (contém)</b></td><td align="left">'.$nome.'</td></tr>';
}

if($cpf){
	$label .= '<tr><td align="right"><b>CPF</b></td><td align="left">'.$cpf.'</td></tr>';
	$cpf = str_replace(array('.','-'), '', $cpf);
	$arWhere[] = " fom.cpfprofissional = '{$cpf}'";	
}

// Medico
if( $medico[0] && $medico_campo_flag ){
	$arWhere[] = " fom.cpfprofissional ". (!$medico_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $medico )."' ) ";
	$label .= '<tr><td align="right"><b>Médico(s)</b></td><td align="left">'.implode('<br/>', $db->carregarColuna("select mdcnome from maismedicos.medico where mdccpf in ('".implode( "','", $medico )."')")).'</td></tr>';
} 


$arPerfil = arrayPerfil();
if(in_array(PERFIL_TUTOR, $arPerfil)){
	$arWhere[] = "usr.usucpf = '{$_SESSION['usucpf']}'";
}else
if(in_array(PERFIL_SUPERVISOR, arrayPerfil())){
	$arWhere[] = "fom.cpfsupervisor = '{$_SESSION['usucpf']}'";
}
$stCampoDtVisita = "
			,case when fom.cpfprofissional is not null and dtvisita is null then '-' 
				when fom.cpfprofissional is null and dtvisita is null then '' 
				else to_char(dtvisita, 'YYYY-MM-DD') end as dtvisita";
$sql = "select 
			".($_REQUEST['requisicao'] != 'exibirxls' ? "substr(fom.cpfprofissional, 1, 3) || '.' || substr(fom.cpfprofissional, 4, 3) || '.' || substr(fom.cpfprofissional, 7, 3) || '-' || substr(fom.cpfprofissional, 10)" : "fom.cpfprofissional" )." as cpfprofissional,
			fom.noprofissional as noprofissional
			".($_REQUEST['requisicao'] == 'exibirxls' ? $stCampoDtVisita.",fom.status,fom.noinstituicao,mun.muncod,mun.mundescricao,mun.estuf" : '')."			
		from maismedicos.ws_respostas_formulario fom		
		inner join ( select max(wssid) as wssid, nucpf from maismedicos.ws_profissionais where wssstatus = 'A' group by nucpf ) b ON b.nucpf = fom.cpfprofissional
				
		left join maismedicos.universidade uni on uni.uniid =fom.uniid
		left join maismedicos.usuarioresponsabilidade usr on usr.uniid = uni.uniid
			and usr.pflcod in (".implode(",",$arPerfil).")	 
		left join territorios.municipio mun on mun.muncod = fom.muncodprofissional
		where fom.wssstatus = 'A'
		".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."
		and fom.status = 'Finalizado'
		and  fom.idformulario in (35,36,40,43)
		".(is_array($arWhereForm) ? ' and '.implode(' and ', $arWhereForm) : '')."
		order by fom.noprofissional".($_REQUEST['requisicao'] == 'exibirxls' ? ",dtvisita desc" : "");

//ver($sql, d);

if($_REQUEST['requisicao'] == 'exibirxls'){ 

	$arCabecalho = array('CPF', 'Nome', 'Data de Visista', 'Status', 'Instituição Supervisora', 'IBGE', 'Município', 'UF');
// 	$arTipo = array('text', null, 'date');
// 	gerar_excel($sql, $arCabecalho, 'SIMEC_PMMB_DATA_VISITA_MEDICO_'.date("Ymdhis"), $arTipo);

	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_PMMB_DATA_VISITA_MEDICO_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_PMMB_DATA_VISITA_MEDICO_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($sql,$arCabecalho,100000000,5,'N','100%','');
	die;
	
}

?>
	
<html>

	<head>
	
		<title>Relatório de Data de Visitas por Médico - Mais Médicos</title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script>
		
			function mostrarDatas(cpf, obj)
			{

				img = $(obj).attr('src');
				params = $('#paramentros').val();

				cpf = replaceAll(replaceAll(cpf,'.',''), '-', '');
				
				if(img == '../imagens/menos.gif'){
					$(obj).attr('src', '../imagens/mais.gif');
					$('.tr_medico_cpf_'+cpf).hide();
				}else{
					$(obj).attr('src', '../imagens/menos.gif');
					$('.tr_medico_cpf_'+cpf).show();
				}

				if($('.tr_medico_cpf_'+cpf).length==0){
				
					tr = $(obj).parent().parent();
					tr.after('<tr id="tr_carregando_'+cpf+'"><td colspan="6">&nbsp;<img align="absmiddle" src="../imagens/carregando.gif" />&nbsp;Carregando...</td></tr>');
					
					$.ajax({
						url		: '/maismedicos/maismedicos.php?modulo=relatorio/dataVisitaMedico&acao=A',
						type	: 'post',
						data	: 'requisicao=true&carregarDatas=true&cpf='+cpf+'&'+params,
						success	: function(e){							
							tr.after(e);
							$('#tr_carregando_'+cpf).remove();
							
						}
					});	
				}
			}

			$(function(){
				$('.abrirRespostas').live('click', function(){
					var url = '/maismedicos/maismedicos.php?modulo=relatorio/dataVisitaMedico&acao=A&requisicao=true&mostrarRespostas=true&id='+this.id;
					var janela = window.open( url, 'resultadoProfissionais', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
					janela.focus();
				});
			});
			
		</script>
		<style>
		.labelEsquerda{
			width: 50%;
			padding: 5px;
		}
		.labelDireita{
			width: 50%;
			padding: 5px;
			float:left;
		}
		</style>
		
	</head>
	
	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
	
		<br/>
		<?php 
		$titulo_modulo = "Data de Visitas por Médico";
		monta_titulo( $titulo_modulo, '' );		

		if($label){
			echo '<center>';
			echo '<b>FILTRO(S):</b>';
			echo '<table cellspacing="1" cellpadding="3" align="center">';
			echo $label;
			echo '</table>';
			echo '</center>';
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

		$rs = $db->carregar($sql);
		?>
		<input type="hidden" name="parametros" id="parametros" value="<?php echo $stParams; ?>" />
		<?php if($rs): ?>
			<table class="listagem" cellpadding="3" cellspacing="1" align="center" width="95%">
				<thead>
					<tr>
						<th>CPF</th>
						<th>Nome</th>						
					</tr>
				</thead>
				<tbody>
				<?php foreach($rs as $i => $dados): ?>
					<tr bgcolor="<?php echo fmod($i,2) == 0 ? '' : '#F7F7F7'; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo fmod($i,2) == 0 ? '' : '#F7F7F7'; ?>';">
						<td>
							<img id="img_<?php echo $dados['cpfprofissional']; ?>" src="../imagens/mais.gif" style="cursor:pointer" onclick="mostrarDatas('<?php echo $dados['cpfprofissional']; ?>', this)" />
							&nbsp;<?php echo formatar_cpf($dados['cpfprofissional']); ?>
						</td>
						<td><?php echo $dados['noprofissional']; ?></td>						
					</tr>
				<?php endforeach; ?>
				</tbody>
				<tfoot>
					<tr bgcolor="#f0f0f0">
						<td colspan="7"><b><?php echo count($rs); ?>&nbsp;registros</b></td>
					</tr>
				</tfoot>
			</table>
		<?php else: ?>
			<center><p><b>Sem registros!</b></p></center>	
		<?php endif; ?>
		
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