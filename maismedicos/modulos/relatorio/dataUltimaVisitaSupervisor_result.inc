<?php 

$MOSTRA_TEMPO_EXECUCAO = $_REQUEST['requisicao']!='exibirxls' ? true : false;

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
	
	$sql = "select distinct
				idpublicarformulario,
				to_char(dtvisita, 'DD/MM/YYYY') as data_visita,
				to_char(dtenvio, 'DD/MM/YYYY HH24:MI:SS') as data_envio,
				to_char(coalesce(dtultimaatualizacao,dtpublicacao), 'DD/MM/YYYY HH24:MI:SS') as data_atualizacao,
				dtvisita,
				status
			from maismedicos.ws_respostas_formulario
			where wssstatus = 'A'
			and cpfprofissional = '{$cpf}'
			order by dtvisita desc";
	
	$rs = $db->carregar($sql);
	
	if($rs){
		foreach($rs as $dados){	
			echo '<tr class="tr_medico_cpf_'.$cpf.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'\';">
					 <td colspan="7">
						&nbsp;<img src="../imagens/seta_filho.gif" />&nbsp;
						 <img src="../imagens/report.gif" title="Mostrar respostas" alt="Mostrar respostas" style="cursor:pointer;" class="abrirRespostas" id="'.$dados['idpublicarformulario'].'"/>&nbsp;
						 <b>Visita:&nbsp;</b>'.($dados['data_visita'] ? $dados['data_visita'] : 'Não preenchido').',&nbsp;
						 <b>Envio:&nbsp;</b>'.($dados['data_envio'] ? $dados['data_envio'] : 'Não preenchido').',&nbsp;
						 <b>Cod. formulário:&nbsp;</b>'.($dados['idpublicarformulario'] ? $dados['idpublicarformulario'] : 'Não preenchido').',&nbsp;
						 <b>Status:&nbsp;</b>'.$dados['status'].'
					 </td>										
				  </tr>';
			
		}
	}	
	die;
}

// UF
if($estuf){
	$arWhere[] = " mdc.estuf = '{$estuf}' ";
	$label .= '<tr><td align="right"><b>UF</b></td><td align="left">'.$estuf.'</td></tr>'; 
}

// Municipio
if( $muncod[0] && $muncod_campo_flag ){
	$arWhere[] = " mdc.muncod ". (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $muncod )."' ) ";
	$label .= '<tr><td align="right"><b>Município(s)</b></td><td align="left">'.implode('<br/>', $db->carregarColuna("select mundescricao from territorios.municipio where muncod in ('".implode( "','", $muncod )."')")).'</td></tr>';
}

// So capitais
if($socapitais == 'on'){
	$arWhere[] = " mdc.muncod in ('3550308','3304557','2927408','5300108','2304400','3106200','1302603','4106902','2611606','4314902','1501402','5208707',
								'2111300','2704302','2408102','2211001','5002704','2507507','2800308','5103403','1100205','4205407','1600303','1200401',
								'3205309','1400100','1721000')";
	$label .= '<tr><td align="right"><b>Só capitais</b></td><td align="left">Sim</td></tr>';
}

// Instituicao Supervisora
if( $uniid[0] && $uniid_campo_flag ){
	$arWhere[] = " mdc.uniid ". (!$uniid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $uniid )."' ) ";
	$label .= '<tr><td align="right"><b>Instituição(ões) Supervisora(s)</b></td><td align="left">'.implode('<br/>', $db->carregarColuna("select uninome from maismedicos.universidade where uniid in ('".implode( "','", $uniid )."')")).'</td></tr>';
}

if($nome){
	$arWhere[] = " mdc.mdcnome ilike '%{$nome}%' ";
	$label .= '<tr><td align="right"><b>Nome (contém)</b></td><td align="left">'.$nome.'</td></tr>';
}

if($cpf){
	$label .= '<tr><td align="right"><b>CPF</b></td><td align="left">'.$cpf.'</td></tr>';
	$cpf = str_replace(array('.','-'), '', $cpf);
	$arWhere[] = " mdc.mdccpf = '{$cpf}'";	
}

// Medico
if( $medico[0] && $medico_campo_flag ){
	$arWhere[] = " mdc.mdccpf ". (!$medico_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $medico )."' ) ";
	$label .= '<tr><td align="right"><b>Médico(s)</b></td><td align="left">'.implode('<br/>', $db->carregarColuna("select mdcnome from maismedicos.medico where mdccpf in ('".implode( "','", $medico )."')")).'</td></tr>';
}

if(in_array(PERFIL_SUPERVISOR, arrayPerfil())){
	$arWhere[] = "fom.cpfsupervisor = '{$_SESSION['usucpf']}'";
}

$arPerfil = arrayPerfil();
if(in_array(PERFIL_TUTOR, $arPerfil)){
	$arWhere[] = "usr.usucpf = '{$_SESSION['usucpf']}'";
}else
	if(in_array(PERFIL_SUPERVISOR, arrayPerfil())){
	$arWhere[] = "fom.cpfsupervisor = '{$_SESSION['usucpf']}'";
}

//'<img src=\"../imagens/acrobat.gif\" height=\"15\" style=\"cursor:pointer;\" alt=\"Gerar PDF com as Respostas\" title=\"Gerar PDF com as Respostas\" onclick=\"gerarPdfRespostas(\'' || f.idpublicarformulario || '\')\"/>&nbsp;Nº:&nbsp;' || f.idpublicarformulario' as idpublicarformulario,

$sql = "select distinct
			".($_REQUEST['requisicao'] != 'exibirxls' ? "'<img src=\"../imagens/acrobat.gif\" title=\"Mostrar respostas\" alt=\"Mostrar respostas\" style=\"cursor:pointer;\" class=\"abrirRespostas\" id=\" ' || idpublicarformulario || ' \"/>' as pdf," : "")." 
			mdcnome as noprofissional,			
			".($_REQUEST['requisicao'] != 'exibirxls' ? "substr(mdccpf, 1, 3) || '.' || substr(mdccpf, 4, 3) || '.' || substr(mdccpf, 7, 3) || '-' || substr(mdccpf, 10)" : "mdccpf" )." as cpfprofissional,
			case when fom.cpfprofissional is not null and dtvisita is null then to_char(dtenvio, 'DD/MM/YYYY') else to_char(dtvisita, 'YYYY-MM-DD') end as dtvisita,
			noinstituicao,
			".($_REQUEST['requisicao'] == 'exibirxls' ? "muncodprofissional," : "")."
			coalesce(mun.mundescricao,mun2.mundescricao) || '/' || coalesce(mun.estuf,mun2.estuf) as municipio,
			nosupervisor,
			".($_REQUEST['requisicao'] != 'exibirxls' ? "substr(cpfsupervisor, 1, 3) || '.' || substr(cpfsupervisor, 4, 3) || '.' || substr(cpfsupervisor, 7, 3) || '-' || substr(cpfsupervisor, 10)" : "cpfsupervisor" )." as cpfsupervisor,
			case when fom.cpfprofissional is not null and dtvisita is null then '-'  
				when fom.cpfprofissional is null and dtvisita is null then '' 
				else to_char(now() - dtvisita, 'DD') end as periodo_sem_visita,
			case when to_char(now() - dtvisita, 'DD')::integer <= 30 then 'Menos de 30 dias'
				 when to_char(now() - dtvisita, 'DD')::integer >= 31 and to_char(now() - dtvisita, 'DD')::integer <= 60 then 'Entre 30 e 60 dias'  
				 when to_char(now() - dtvisita, 'DD')::integer >= 61 and to_char(now() - dtvisita, 'DD')::integer <= 90 then 'Entre 60 e 90 dias'				 
				 when to_char(now() - dtvisita, 'DD')::integer > 90 then 'Mais de 90 dias'
			end as periodo
		from maismedicos.medico mdc
left join (
	select * from maismedicos.ws_respostas_formulario f3
	join (
		select distinct max(rfoid) as rfoid, cpfprofissional as cpfprofissional1
		from (
			--15185
			select max(coalesce(dtvisita,dtenvio)) as dtvisita2, f.cpfprofissional as cpfprofissional2 from maismedicos.ws_respostas_formulario  f
			where f.wssstatus = 'A' and f.status = 'Finalizado' and f.cpfprofissional is not null
			group by f.cpfprofissional
		) as f1
		join maismedicos.ws_respostas_formulario f2 on f1.cpfprofissional2 = f2.cpfprofissional
			and f1.dtvisita2 = coalesce(f2.dtvisita,f2.dtenvio)		
		group by cpfprofissional
	) f4 on f3.rfoid = f4.rfoid
) fom on fom.cpfprofissional = mdc.mdccpf
join territorios.municipio mun2 on mun2.muncod = mdc.muncod
left join territorios.municipio mun on mun.muncod = fom.muncodprofissional
where mdc.mdcstatus = 'A'
		".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."";

// ver($sql, d);

if($_REQUEST['requisicao'] == 'exibirxls'){ 

	$arCabecalho = array('Nome do Médico', 'CPF Médico', 'Data Ultima Visita', 'Instituição Supervisora', 'COD IBGE',  'Municipio/UF', 'Nome do Supervisor', 'CPF Supervisor', 'Dias sem visita', 'Período sem visita');
	$arTipo = array(null, 'text', 'date', null, null, null, null, 'text');
	gerar_excel($sql, $arCabecalho, 'SIMEC_PMMB_DATA_ULTIMA_VISITA_'.date("Ymdhis"), $arTipo);
	die;
	
}

?>
	
<html>

	<head>
	
		<title>Relatório de Data de Última Visita Médicos - Mais Médicos</title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script>

			$(function(){
				$('.abrirRespostas').live('click', function(){
					var url = '/maismedicos/maismedicos.php?modulo=relatorio/dataUltimaVisitaSupervisor&acao=A&requisicao=true&mostrarRespostas=true&id='+this.id;
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
		$titulo_modulo = "Data de Última Visita";
		monta_titulo( $titulo_modulo, '' );		

		if($label){
			echo '<center>';
			echo '<b>FILTRO(S):</b>';
			echo '<table cellspacing="1" cellpadding="3" align="center">';
			echo $label;
			echo '</table>';
			echo '</center>';
		}
		
		$arCabecalho = array('PDF', 'Nome do Médico:', 'CPF Médico', 'Data Ultima Visita', 'Instituição Supervisora',  'Municipio/UF', 'Nome do Supervisor', 'CPF Supervisor', 'Dias sem visita', 'Período sem visita');
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