<?php
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past

ini_set("memory_limit", "1024M");


//controla a classificação do validar demandas p/ GESTOR
$sql = "DELETE FROM demandas.controlevalidacao where usucpf = '".$_SESSION['usucpf']."'";
$db->executar($sql);

//controla a classificação da demanda p/ TEC 1º NIVEL
$sql = "DELETE FROM demandas.controledemanda where usucpf = '".$_SESSION['usucpf']."'";
$db->executar($sql);
$db->commit();



if ($_GET['dmdid']){
	session_start();
	$_SESSION['dmdid'] = $_GET['dmdid'];
	header( "Location: demandas.php?modulo=principal/dadosDemanda&acao=A" );
	exit();
}


$perfil = arrayPerfil();


//mostra solicitante
/*
$mostra_solicitante = false;
if( in_array(DEMANDA_PERFIL_SUPERUSUARIO, $perfil) || 
	in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil) ||
	in_array(DEMANDA_PERFIL_ANALISTA_SISTEMA, $perfil) ||
	in_array(DEMANDA_PERFIL_GESTOR_MEC, $perfil) ||
	in_array(DEMANDA_PERFIL_TECNICO1, $perfil) ||
	in_array(DEMANDA_PERFIL_DBA, $perfil) ){
	
	$mostra_solicitante = true;
}
*/


//Ajax pausa
if ($_REQUEST['dmdidPausaAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaPausa($_REQUEST['dmdidPausaAjax']);
	exit;
}

//Ajax avaliação
if ($_REQUEST['dmdidAvaliacaoAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaAvaliacao($_REQUEST['dmdidAvaliacaoAjax']);
	exit;
}

//Ajax Nserie/Patr
if ($_REQUEST['dmdidNseriepatrAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaNseriepatr($_REQUEST['dmdidNseriepatrAjax']);
	exit;
}

function montaPausa($dmdid){
	global $db;
	
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	$classdata = new Data;

	
				//verifica pausa da demanda
				$sql = "select t.tpadsc, p.pdmdatainiciopausa, p.pdmdatafimpausa, 
							   replace(substr(p.pdmjustificativa, 0 , 4000), chr(13)||chr(10), '<br>') as pdmjustificativa,
							   to_char(p.pdmdatainiciopausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausaini, to_char(p.pdmdatafimpausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausafim
						from demandas.pausademanda p 
						inner join demandas.tipopausademanda t ON t.tpaid = p.tpaid
						where p.pdmstatus = 'A' and p.dmdid = ". (int) $dmdid;
						
				$dadosp = $db->carregar($sql);	
				
				$flagIndeterminado = '';
				$tempototalpausa = 0;
				$textotempopausa = "<div align='left'>";
				
				if($dadosp){
					foreach($dadosp as $dadop){
						
						if($dadop['pdmdatainiciopausa'] && $dadop['pdmdatafimpausa']){
							
							$ano_inip	= substr($dadop['pdmdatainiciopausa'],0,4);
							$mes_inip	= substr($dadop['pdmdatainiciopausa'],5,2);
							$dia_inip	= substr($dadop['pdmdatainiciopausa'],8,2);
							$hor_inip	= substr($dadop['pdmdatainiciopausa'],11,2);
							$min_inip	= substr($dadop['pdmdatainiciopausa'],14,2);
				
							$ano_fimp	= substr($dadop['pdmdatafimpausa'],0,4);
							$mes_fimp	= substr($dadop['pdmdatafimpausa'],5,2);
							$dia_fimp	= substr($dadop['pdmdatafimpausa'],8,2);
							$hor_fimp	= substr($dadop['pdmdatafimpausa'],11,2);
							$min_fimp	= substr($dadop['pdmdatafimpausa'],14,2);
							
							$dinip = mktime($hor_inip,$min_inip,0,$mes_inip,$dia_inip,$ano_inip); // timestamp da data inicial
							$dfimp = mktime($hor_fimp,$min_fimp,0,$mes_fimp,$dia_fimp,$ano_fimp); // timestamp da data final
							
							// pega o tempo total da pausa
							$tempototalpausa = $tempototalpausa + ($dfimp - $dinip);
							
							
							$dtiniinvert = $ano_inip.'-'.$mes_inip.'-'.$dia_inip.' '.$hor_inip.':'.$min_inip.':00';
							$dtfiminvert = $ano_fimp.'-'.$mes_fimp.'-'.$dia_fimp.' '.$hor_fimp.':'.$min_fimp.':00';
							
						}
	
						//monta o texto da tempopausa
						$textotempopausa .= "<b>Tipo:</b> ". $dadop['tpadsc'];
						$textotempopausa .= "<br><b>Justificativa:</b> ". $dadop['pdmjustificativa']."";
						$textotempopausa .= "<br><b>Data início:</b> ". $dadop['datapausaini']."";
						if($dadop['datapausafim']){
							$textotempopausa .= "<br><b>Data término:</b> ". $dadop['datapausafim']."";
						}else{
							$textotempopausa .= "<br><b>Data término:</b> Indeterminado";
						}
						
						if($dadop['pdmdatafimpausa']){
							$tempop = $classdata->diferencaEntreDatas(  $dtiniinvert, $dtfiminvert, 'tempoEntreDadas', 'string','yyyy/mm/dd');
							if(!$tempop) $tempop = '0 minuto';
							$textotempopausa .= "<br><b>Tempo da Pausa:</b> ".$tempop;
						}else{
							$flagIndeterminado = ' + <font color=red>Tempo Indeterminado</font>';
							$textotempopausa .= "<br><b>Tempo da Pausa:</b> Indeterminado";
						}
						
						$textotempopausa .= "<BR><BR>";
						
					}
					
					
					
					//if($flagIndeterminado == '1')
					//	$textotempopausa .= "TOTAL (Tempo da Pausa): Indeterminado";
					//else{
						$datainiaux = date('Y-m-d H:i').':00';
						$ano_aux	= substr($datainiaux,0,4);
						$mes_aux	= substr($datainiaux,5,2);
						$dia_aux	= substr($datainiaux,8,2);
						$hor_aux	= substr($datainiaux,11,2);
						$min_aux	= substr($datainiaux,14,2);
						
						$datafinalaux = mktime($hor_aux,$min_aux,0+$tempototalpausa,$mes_aux,$dia_aux,$ano_aux);
						$datafinalaux2 = strftime("%Y-%m-%d %H:%M:%S", $datafinalaux);
						$tempototalp = $classdata->diferencaEntreDatas(  $datainiaux, $datafinalaux2, 'tempoEntreDadas', 'string','yyyy/mm/dd');
						$textotempopausa .= "<b>TOTAL (Tempo da Pausa):</b> ". $tempototalp . $flagIndeterminado;
					//}
					
				}
				
				
				$resto = $tempototalpausa;
				$horas 			= $resto/3600; //quantidade de horas
		        $intHoras 		= floor($horas);
		        if($intHoras >= 1){	//se houver horas
		            $horasx = $intHoras;
		            $resto 		 = $resto-($intHoras*3600); //retira do total, o tempo em segundos das horas passados
		        }
		                    
		        $minutos 		= $resto/60; //quantidade de minutos
		        $intMinutos 	= floor($minutos);
		        if($intMinutos >= 1){ //se houver minutos
		            $minutosx = $intMinutos;
		            $resto 		 = $resto-($intMinutos*60); //retira do total, o tempo em segundos dos minutos passados
		        }				
		        
	            if(!$horasx) $horasx = "00";
	            if(strlen($horasx) == 1) $horasx = "0".$horasx;
	            if(!$minutosx) $minutosx = "00";
	            if(strlen($minutosx) == 1) $minutosx = "0".$minutosx;
	            
	            $hormin = $horasx.":".$minutosx;
	            
	            
				//pega prioridade e data termino e data conclusão
				$sql = "select dmdhorarioatendimento, to_char(dmddatafimprevatendimento::timestamp,'DD/MM/YYYY HH24:MI') AS dmddatafimprevatendimento, datahist as dataconc
						from demandas.demanda d
						LEFT JOIN (  (select a.docid, to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datahist
								 		from 	workflow.historicodocumento a
								 			inner join workflow.comentariodocumento b on a.hstid = b.hstid
								 			inner join workflow.documento c on c.docid = a.docid
								 	where aedid in (146, 191) and c.esdid in (93,95,109,111)
								  group by a.docid ) ) as hst ON hst.docid = d.docid						
						where d.dmdid = ". (int) $dmdid;
				$dadosdmd = $db->carregar($sql);	
				
				//pega o ordid para passar como parametro
				$sql = "SELECT	t.ordid FROM demandas.demanda d
						LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid	 			
				 		WHERE d.dmdid=".(int) $dmdid;
				$ordid = $db->PegaUm($sql);				
	            
				
				$vfdtfim = verificaCalculoTempoDtfim($dadosdmd[0]['dmddatafimprevatendimento'], $hormin, $dadosdmd[0]['dmdhorarioatendimento'], $dadosdmd[0]['dataconc'], $ordid);
				
				
				if($flagIndeterminado){
					$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=red>Data Indeterminada</font>";
				}
				else{
					$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=black>".$vfdtfim."</font>";
					//$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=black>".$horasx.":".$minutosx."</font>";
				}
				
				$textotempopausa .= "</div>"; 
	 
				echo $textotempopausa;
	exit;
}


function montaAvaliacao($dmdid){
	global $db;
	
	
	//verifica avaliações da demanda
	$sql = "SELECT to_char(avddata::timestamp,'DD/MM/YYYY HH24:MI:SS') as avddata, 
						(CASE avdgeral
						 	WHEN '1' THEN 'RUIM'
						 	WHEN '2' THEN 'REGULAR'
						 	WHEN '3' THEN 'BOM'
						 	WHEN '4' THEN 'ÓTIMO'
						END) AS avdgeral,
		 				avsobs 
				FROM demandas.avaliacaodemanda 
				where avdstatus='A' and dmdid = ". (int) $dmdid . "
				order by avddata desc";	
	
	$dadosp = $db->carregar($sql);	
	
	$textoavaliacao = "<div align='left'>";
	
	if($dadosp){
		foreach($dadosp as $dadop){
			
			//monta o texto da avaliação
			$textoavaliacao .= "<b>Data:</b> ". $dadop['avddata'];
			$textoavaliacao .= "<br><b>Avaliação:</b> ". $dadop['avdgeral']."";
			if($dadop['avsobs']) $textoavaliacao .= "<br><b>Justificativa:</b> ". $dadop['avsobs']."";
			
			
			$textoavaliacao .= "<BR><BR>";
			
		}
	}
	
	$textoavaliacao .= "</div>"; 
 
	echo $textoavaliacao;
	
	exit;
}


function montaNseriepatr($dmdid){
	global $db;
	
	
	//pega nº serie / patrimonio
	$nseriepatr = "";

		if($dmdid){
			$sqln = "select ihdnumserie, ihdnumpatrimonio 
					  from demandas.itemhardwaredemanda 
					  where dmdid = ".$dmdid;
			$nsp = array();
			$nsp = $db->carregar( $sqln );
			$ctnsp = 1;
			if($nsp){
				$nseriepatr = '<div align=left>';
				foreach($nsp as $nspx){
					
					$nseriepatr .= '<b><u>Item '.$ctnsp .'</u> - </b>';
					
					if($nspx['ihdnumserie']) $nseriepatr .= 'Nº Série: '. $nspx['ihdnumserie'];
					if($nspx['ihdnumserie'] && $nspx['ihdnumpatrimonio']) $nseriepatr .= ' / ';
					if($nspx['ihdnumpatrimonio']) $nseriepatr .= 'Nº Patrimônio: '. $nspx['ihdnumpatrimonio'];
					
					$nseriepatr .= '<br>';
					
					$ctnsp++;	
				}
				$nseriepatr .= '</div>';
			}	
			
		}
	
	echo $nseriepatr;
	
	exit;
}




function numeroDemandas($sql){
	global $db;
	
	$sqlCount = "select 
						count(1)
					 from (" . $sql . ") rs";
	$totalRegistro = $db->pegaUm($sqlCount);
			
	//$dados = $db->carregar($sql);
	if($totalRegistro){
		echo " (".$totalRegistro.")";
	}
	else{
		echo " (0)";
	}
}

function montaSistema($celid){
	global $db;
	
	$perfil = arrayPerfil();

	if ( in_array(DEMANDA_PERFIL_GERENTE_FSW, $perfil) ||
		 in_array(DEMANDA_PERFIL_ANALISTA_FSW, $perfil) ||
		 in_array(DEMANDA_PERFIL_FISCAL_TECNICO_FSW, $perfil) 
		){
			$sididx = $db->carregarColuna("SELECT distinct sidid from demandas.usuarioresponsabilidade where rpustatus='A' and sidid is not null and usucpf = '".$_SESSION['usucpf']."'");
			
			if($sididx[0]){
				$sididx = implode(',',$sididx);
				$sididx = " and s.sidid IN (".$sididx.") ";
			}
			else $sididx = "";
	}
 	
	echo "<B>Sistema:</B><br>";
	
	$sql = sprintf("select  
						s.sidid  AS codigo, 
						upper(s.sidabrev) || ' - ' || s.siddescricao AS descricao
					from 
						demandas.sistemadetalhe s
					left join demandas.sistemacelula c on s.sidid = c.sidid  
					where  s.sidstatus = 'A'
					AND celid = %d
					$sididx
					order by s.sidabrev", 
			$celid);
	$db->monta_combo( 'sidid', $sql, 'S', 'Filtro...', '', '','','547','','sidid');
	echo "<script>forcaWidth();</script>";
}

if($_POST['consultaRapida'] == "todas"){
	$_POST['codigo'] 	= false;
	/*
	$_POST['esdid'] 	= false;
	$_POST['ordid'] 	= false;
	$_POST['unaid'] 	= false;
	$_POST['dataini'] 	= false;
	$_POST['datafim'] 	= false;
	$_POST['celid']		= false;
	$_POST['sidid']	 	= false;
	$_POST['dmdnumdocrastreamento'] = false;
	*/
	//$_POST['nao_exib_finaliz_cancela_invalid'] = 'on';
	$codigo   =  false;
	/*
	$situacao = false;
	$origem   = false;
	$setor    = false;
	$dataini  = false;
	$datafim  = false;
	$celula   = false;
	$sistema  = false;
	$dmdnumdocrastreamento = false;
	*/
	//$nao_exib_finaliz_cancela_invalid = 'on';
	$todas = true;
}
elseif($_POST['consultaRapida'] == "hoje"){
	$_POST['codigo'] = false;
	/*
	$_POST['esdid'] = false;
	$_POST['ordid'] = false;
	$_POST['unaid'] = false;
	$_POST['celid'] = false;
	$_POST['sidid'] = false;
	$_POST['dmdnumdocrastreamento'] = false;
	*/
	$codigo   =  false;
	/*
	$situacao = false;
	$origem   = false;
	$setor    = false;
	$celula   = false;
	$sistema  = false;
	$dmdnumdocrastreamento = false;
	*/
	$hoje = true;
	$dataini = $_POST['dataini'];
	$datafim = $_POST['datafim'];
	
}
elseif($_POST['consultaRapida'] == "minhademanda"){
	
	$_POST['codigo'] 	= false;
	/*
	$_POST['esdid'] 	= false;
	$_POST['ordid'] 	= false;
	$_POST['unaid'] 	= false;
	$_POST['dataini'] 	= false;
	$_POST['datafim'] 	= false;
	$_POST['celid']		= false;
	$_POST['sidid']	 	= false;
	$_POST['dmdnumdocrastreamento'] = false;
	*/
	$codigo   =  false;
	/*
	$situacao = false;
	$origem   = false;
	$setor    = false;
	$dataini  = false;
	$datafim  = false;
	$celula   = false;
	$sistema  = false;
	$dmdnumdocrastreamento = false;
	*/
	$minha = true;
	
}
elseif($_POST['consultaRapida'] == "nova"){
	
	$_POST['codigo'] 	= false;
	/*
	$_POST['esdid'] 	= false;
	$_POST['ordid'] 	= false;
	$_POST['unaid'] 	= false;
	$_POST['dataini'] 	= false;
	$_POST['datafim'] 	= false;
	$_POST['celid']		= false;
	$_POST['sidid']	 	= false;
	$_POST['dmdnumdocrastreamento'] = false;
	*/
	$codigo   = false;
	/*
	$situacao = false;
	$origem   = false;
	$setor    = false;
	$dataini  = false;
	$datafim  = false;
	$celula   = false;
	$sistema  = false;
	$dmdnumdocrastreamento = false;
	*/
	$nova = true;
	
}
elseif($_POST['consultaRapida'] == "ematraso"){
	$_POST['codigo'] 	= false;
	/*
	$_POST['esdid'] 	= false;
	$_POST['ordid'] 	= false;
	$_POST['unaid'] 	= false;
	$_POST['dataini'] 	= false;
	$_POST['datafim'] 	= false;
	$_POST['celid']		= false;
	$_POST['sidid']	 	= false;
	$_POST['dmdnumdocrastreamento'] = false;
	*/
	$codigo   =  false;
	/*
	$situacao = false;
	$origem   = false;
	$setor    = false;
	$dataini  = false;
	$datafim  = false;
	$celula   = false;
	$sistema  = false;
	$dmdnumdocrastreamento = false;
	$todas = false;
	*/
	$ematraso = true;
	
	//$dataini = $_POST['dataini'];
	//$datafim = $_POST['datafim'];
}
elseif($_POST['consultaRapida'] == "vencehoje"){
	
	$_POST['codigo'] 	= false;
	/*
	$_POST['esdid'] 	= false;
	$_POST['ordid'] 	= false;
	$_POST['unaid'] 	= false;
	$_POST['dataini'] 	= false;
	$_POST['datafim'] 	= false;
	$_POST['celid']		= false;
	$_POST['sidid']	 	= false;
	$_POST['dmdnumdocrastreamento'] = false;
	*/
	$codigo   =  false;
	/*
	$situacao = false;
	$origem   = false;
	$setor    = false;
	$dataini  = false;
	$datafim  = false;
	$celula   = false;
	$sistema  = false;
	$dmdnumdocrastreamento = false;
	$todas = false;
	*/
	$vencehoje = true;
	
	//$dataini = $_POST['dataini'];
	//$datafim = $_POST['datafim'];
}
elseif($_POST['consultaRapida'] == "emanalise"){
	$_POST['codigo'] 	= false;
	/*
	$_POST['esdid'] 	= false;
	$_POST['ordid'] 	= false;
	$_POST['unaid'] 	= false;
	$_POST['dataini'] 	= false;
	$_POST['datafim'] 	= false;
	$_POST['celid']		= false;
	$_POST['sidid']	 	= false;
	$_POST['dmdnumdocrastreamento'] = false;
	*/
	$codigo   =  false;
	/*
	$situacao = false;
	$origem   = false;
	$setor    = false;
	$dataini  = false;
	$datafim  = false;
	$celula   = false;
	$sistema  = false;
	$dmdnumdocrastreamento = false;
	$todas = false;
	*/
	$emanalise = true;
	
	//$dataini = $_POST['dataini'];
	//$datafim = $_POST['datafim'];
}
elseif($_POST['consultaRapida'] == "ematendimento"){
	$_POST['codigo'] 	= false;
	/*
	$_POST['esdid'] 	= false;
	$_POST['ordid'] 	= false;
	$_POST['unaid'] 	= false;
	$_POST['dataini'] 	= false;
	$_POST['datafim'] 	= false;
	$_POST['celid']		= false;
	$_POST['sidid']	 	= false;
	$_POST['dmdnumdocrastreamento'] = false;
	*/
	$codigo   =  false;
	/*
	$situacao = false;
	$origem   = false;
	$setor    = false;
	$dataini  = false;
	$datafim  = false;
	$celula   = false;
	$sistema  = false;
	$dmdnumdocrastreamento = false;
	$todas = false;
	*/
	$ematendimento = true;
	
	//$dataini = $_POST['dataini'];
	//$datafim = $_POST['datafim'];
}
elseif($_POST['consultaRapida'] == "agvalidacao"){
	$_POST['codigo'] 	= false;
	/*
	$_POST['esdid'] 	= false;
	$_POST['ordid'] 	= false;
	$_POST['unaid'] 	= false;
	$_POST['dataini'] 	= false;
	$_POST['datafim'] 	= false;
	$_POST['celid']		= false;
	$_POST['sidid']	 	= false;
	$_POST['dmdnumdocrastreamento'] = false;
	*/
	$codigo   =  false;
	/*
	$situacao = false;
	$origem   = false;
	$setor    = false;
	$dataini  = false;
	$datafim  = false;
	$celula   = false;
	$sistema  = false;
	$dmdnumdocrastreamento = false;
	$todas = false;
	*/
	$agvalidacao = true;
	
	//$dataini = $_POST['dataini'];
	//$datafim = $_POST['datafim'];
}
elseif($_POST['consultaRapida'] == "agvalidacaodem"){
	$_POST['codigo'] 	= false;
	/*
	$_POST['esdid'] 	= false;
	$_POST['ordid'] 	= false;
	$_POST['unaid'] 	= false;
	$_POST['dataini'] 	= false;
	$_POST['datafim'] 	= false;
	$_POST['celid']		= false;
	$_POST['sidid']	 	= false;
	$_POST['dmdnumdocrastreamento'] = false;
	*/
	$codigo   =  false;
	/*
	$situacao = false;
	$origem   = false;
	$setor    = false;
	$dataini  = false;
	$datafim  = false;
	$celula   = false;
	$sistema  = false;
	$dmdnumdocrastreamento = false;
	$todas = false;
	*/
	$agvalidacaodem = true;
	
	//$dataini = $_POST['dataini'];
	//$datafim = $_POST['datafim'];
}

/*
elseif($_POST['consultaRapida'] == "agavaliacao"){
	$_POST['codigo'] 	= false;
	$_POST['esdid'] 	= false;
	$_POST['ordid'] 	= false;
	$_POST['unaid'] 	= false;
	$_POST['dataini'] 	= false;
	$_POST['datafim'] 	= false;
	$_POST['celid']		= false;
	$_POST['sidid']	 	= false;
	$_POST['dmdnumdocrastreamento'] = false;
	$codigo   =  false;
	$situacao = false;
	$origem   = false;
	$setor    = false;
	$dataini  = false;
	$datafim  = false;
	$celula   = false;
	$sistema  = false;
	$dmdnumdocrastreamento = false;
	$todas = false;
	$agavaliacao = true;
	
	//$dataini = $_POST['dataini'];
	//$datafim = $_POST['datafim'];
}*/
elseif($_POST['consultaRapida'] == "finalizada"){
	$_POST['codigo'] 	= false;
	/*
	$_POST['esdid'] 	= false;
	$_POST['ordid'] 	= false;
	$_POST['unaid'] 	= false;
	$_POST['dataini'] 	= false;
	$_POST['datafim'] 	= false;
	$_POST['celid']		= false;
	$_POST['sidid']	 	= false;
	$_POST['dmdnumdocrastreamento'] = false;
	*/
	$codigo   =  false;
	/*
	$situacao = false;
	$origem   = false;
	$setor    = false;
	$dataini  = false;
	$datafim  = false;
	$celula   = false;
	$sistema  = false;
	$dmdnumdocrastreamento = false;
	$todas = false;
	*/
	$finalizada = true;
	
	//$dataini = $_POST['dataini'];
	//$datafim = $_POST['datafim'];
}
elseif($_POST['consultaRapida'] == "auditada"){
	$_POST['codigo'] 	= false;
	/*
	$_POST['esdid'] 	= false;
	$_POST['ordid'] 	= false;
	$_POST['unaid'] 	= false;
	$_POST['dataini'] 	= false;
	$_POST['datafim'] 	= false;
	$_POST['celid']		= false;
	$_POST['sidid']	 	= false;
	$_POST['dmdnumdocrastreamento'] = false;
	*/
	$codigo   =  false;
	/*
	$situacao = false;
	$origem   = false;
	$setor    = false;
	$dataini  = false;
	$datafim  = false;
	$celula   = false;
	$sistema  = false;
	$dmdnumdocrastreamento = false;
	$todas = false;
	*/
	$auditada = true;
	
	//$dataini = $_POST['dataini'];
	//$datafim = $_POST['datafim'];
}
elseif($_POST['consultaRapida'] == "agpago"){
	$_POST['codigo'] 	= false;
	/*
	$_POST['esdid'] 	= false;
	$_POST['ordid'] 	= false;
	$_POST['unaid'] 	= false;
	$_POST['dataini'] 	= false;
	$_POST['datafim'] 	= false;
	$_POST['celid']		= false;
	$_POST['sidid']	 	= false;
	$_POST['dmdnumdocrastreamento'] = false;
	*/
	$codigo   =  false;
	/*
	$situacao = false;
	$origem   = false;
	$setor    = false;
	$dataini  = false;
	$datafim  = false;
	$celula   = false;
	$sistema  = false;
	$dmdnumdocrastreamento = false;
	$todas = false;
	*/
	$agpago = true;
	
	//$dataini = $_POST['dataini'];
	//$datafim = $_POST['datafim'];
}
elseif($_POST['consultaRapida'] == "pago"){
	$_POST['codigo'] 	= false;
	/*
	$_POST['esdid'] 	= false;
	$_POST['ordid'] 	= false;
	$_POST['unaid'] 	= false;
	$_POST['dataini'] 	= false;
	$_POST['datafim'] 	= false;
	$_POST['celid']		= false;
	$_POST['sidid']	 	= false;
	$_POST['dmdnumdocrastreamento'] = false;
	*/
	$codigo   =  false;
	/*
	$situacao = false;
	$origem   = false;
	$setor    = false;
	$dataini  = false;
	$datafim  = false;
	$celula   = false;
	$sistema  = false;
	$dmdnumdocrastreamento = false;
	$todas = false;
	*/
	$pago = true;
	
	//$dataini = $_POST['dataini'];
	//$datafim = $_POST['datafim'];
}
elseif($_POST['consultaRapida'] == "invalidada"){
	$_POST['codigo'] 	= false;
	/*
	$_POST['esdid'] 	= false;
	$_POST['ordid'] 	= false;
	$_POST['unaid'] 	= false;
	$_POST['dataini'] 	= false;
	$_POST['datafim'] 	= false;
	$_POST['celid']		= false;
	$_POST['sidid']	 	= false;
	$_POST['dmdnumdocrastreamento'] = false;
	*/
	$codigo   =  false;
	/*
	$situacao = false;
	$origem   = false;
	$setor    = false;
	$dataini  = false;
	$datafim  = false;
	$celula   = false;
	$sistema  = false;
	$dmdnumdocrastreamento = false;
	$todas = false;
	*/
	$invalidada = true;
	
	//$dataini = $_POST['dataini'];
	//$datafim = $_POST['datafim'];
}
elseif($_POST['consultaRapida'] == "cancelada"){
	$_POST['codigo'] 	= false;
	/*
	$_POST['esdid'] 	= false;
	$_POST['ordid'] 	= false;
	$_POST['unaid'] 	= false;
	$_POST['dataini'] 	= false;
	$_POST['datafim'] 	= false;
	$_POST['celid']		= false;
	$_POST['sidid']	 	= false;
	$_POST['dmdnumdocrastreamento'] = false;
	*/
	$codigo   =  false;
	/*
	$situacao = false;
	$origem   = false;
	$setor    = false;
	$dataini  = false;
	$datafim  = false;
	$celula   = false;
	$sistema  = false;
	$dmdnumdocrastreamento = false;
	$todas = false;
	*/
	$cancelada = true;
	
	//$dataini = $_POST['dataini'];
	//$datafim = $_POST['datafim'];
}
elseif($_POST['consultaRapida'] == "emprocessamento"){
	$_POST['codigo'] 	= false;
	/*
	$_POST['esdid'] 	= false;
	$_POST['ordid'] 	= false;
	$_POST['unaid'] 	= false;
	$_POST['dataini'] 	= false;
	$_POST['datafim'] 	= false;
	$_POST['celid']		= false;
	$_POST['sidid']	 	= false;
	$_POST['dmdnumdocrastreamento'] = false;
	*/
	$codigo   =  false;
	/*
	$situacao = false;
	$origem   = false;
	$setor    = false;
	$dataini  = false;
	$datafim  = false;
	$celula   = false;
	$sistema  = false;
	$dmdnumdocrastreamento = false;
	$todas = false;
	*/
	$emprocessamento = true;
	
	//$dataini = $_POST['dataini'];
	//$datafim = $_POST['datafim'];
}
elseif($_POST['consultaRapida'] == "urgente"){
	$_POST['codigo'] 	= false;
	/*
	$_POST['esdid'] 	= false;
	$_POST['ordid'] 	= false;
	$_POST['unaid'] 	= false;
	$_POST['dataini'] 	= false;
	$_POST['datafim'] 	= false;
	$_POST['celid']		= false;
	$_POST['sidid']	 	= false;
	$_POST['dmdnumdocrastreamento'] = false;
	*/
	$codigo   =  false;
	/*
	$situacao = false;
	$origem   = false;
	$setor    = false;
	$dataini  = false;
	$datafim  = false;
	$celula   = false;
	$sistema  = false;
	$dmdnumdocrastreamento = false;
	$todas = false;
	*/
	$urgente = true;
	
	//$dataini = $_POST['dataini'];
	//$datafim = $_POST['datafim'];
}
elseif($_POST['consultaRapida'] == "alta"){
	$_POST['codigo'] 	= false;
	/*
	$_POST['esdid'] 	= false;
	$_POST['ordid'] 	= false;
	$_POST['unaid'] 	= false;
	$_POST['dataini'] 	= false;
	$_POST['datafim'] 	= false;
	$_POST['celid']		= false;
	$_POST['sidid']	 	= false;
	$_POST['dmdnumdocrastreamento'] = false;
	*/
	$codigo   =  false;
	/*
	$situacao = false;
	$origem   = false;
	$setor    = false;
	$dataini  = false;
	$datafim  = false;
	$celula   = false;
	$sistema  = false;
	$dmdnumdocrastreamento = false;
	$todas = false;
	*/
	$alta = true;
	
	//$dataini = $_POST['dataini'];
	//$datafim = $_POST['datafim'];
}
elseif($_POST['consultaRapida'] == "normal"){
	$_POST['codigo'] 	= false;
	/*
	$_POST['esdid'] 	= false;
	$_POST['ordid'] 	= false;
	$_POST['unaid'] 	= false;
	$_POST['dataini'] 	= false;
	$_POST['datafim'] 	= false;
	$_POST['celid']		= false;
	$_POST['sidid']	 	= false;
	$_POST['dmdnumdocrastreamento'] = false;
	*/
	$codigo   =  false;
	/*
	$situacao = false;
	$origem   = false;
	$setor    = false;
	$dataini  = false;
	$datafim  = false;
	$celula   = false;
	$sistema  = false;
	$dmdnumdocrastreamento = false;
	$todas = false;
	*/
	$normal = true;
	
	//$dataini = $_POST['dataini'];
	//$datafim = $_POST['datafim'];
}
elseif($_POST['consultaRapida'] == "ndas"){
	$_POST['codigo'] 	= false;
	/*
	$_POST['esdid'] 	= false;
	$_POST['ordid'] 	= false;
	$_POST['unaid'] 	= false;
	$_POST['dataini'] 	= false;
	$_POST['datafim'] 	= false;
	$_POST['celid']		= false;
	$_POST['sidid']	 	= false;
	$_POST['dmdnumdocrastreamento'] = false;
	*/
	$codigo   =  false;
	/*
	$situacao = false;
	$origem   = false;
	$setor    = false;
	$dataini  = false;
	$datafim  = false;
	$celula   = false;
	$sistema  = false;
	$dmdnumdocrastreamento = false;
	$todas = false;
	*/
	$ndas = true;
	
	//$dataini = $_POST['dataini'];
	//$datafim = $_POST['datafim'];
}
elseif(!$_POST['consultaRapida']){
	extract($_POST);
}

if ($dataini){
	$dataini = explode("/",$dataini);
	$dataini = "{$dataini[2]}/{$dataini[1]}/{$dataini[0]}"; 
}

if ($datafim){
	$datafim = explode("/",$datafim);
	$datafim = "{$datafim[2]}/{$datafim[1]}/{$datafim[0]}"; 
}
//unset($_SESSION['dmdid']);

//Ajax
if ($_REQUEST['numeroDemandasAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	$_POST['consultaRapida'] = $_REQUEST['numeroDemandasAjax'];
	if($_REQUEST['numeroDemandasAjax'] == "hoje"){
		$_POST['dataini'] = date("d/m/Y"); 
		$_POST['datafim'] = date("d/m/Y");
	}
	$sql = lista();
	//dbg($sql['sql']);
	numeroDemandas($sql['sql']);	
	exit;
}

//Ajax
if ($_REQUEST['dmdidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaSubLista($_POST['dmdidAjax']);
	exit;
}


//filtra o sistema por AJAX
if ($_REQUEST['celidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaSistema($_POST['celidAjax']);
	exit;
}


include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
#include APPRAIZ . 'includes/Agrupador.php';

// Atribui/Recupera todos os valores dos filtros na sessão usando a função serialize/unserialize.
if($_POST["submetido"]) {
	$_SESSION["filtros_demandas"] = serialize($_POST);
} 
else if(isset($_SESSION["filtros_demandas"]) && $_SESSION["filtros_demandas"] != "") {
	$_POST = unserialize($_SESSION["filtros_demandas"]);
}

 

// Atribui as variáveis $_POST para preenchimento dos campos.
$codigo 							= (isset($_POST["codigo"]) && $_POST["codigo"]!="") ? $_POST["codigo"] : NULL;
//$esdid 								= (isset($_POST["esdid"]) && $_POST["esdid"]!="") ? $_POST["esdid"] : NULL;
$unaid								= (isset($_POST["unaid"]) && $_POST["unaid"]!="") ? $_POST["unaid"] : NULL;
$ordid 								= (isset($_POST["ordid"]) && $_POST["ordid"]!="") ? $_POST["ordid"] : NULL;
$celid 								= (isset($_POST["celid"]) && $_POST["celid"]!="") ? $_POST["celid"] : NULL;
$sidid 								= (isset($_POST["sidid"]) && $_POST["sidid"]!="") ? $_POST["sidid"] : NULL;
$motid 								= (isset($_POST["motid"]) && $_POST["motid"]!="") ? $_POST["motid"] : NULL;
$dmdnumdocrastreamento 				= (isset($_POST["dmdnumdocrastreamento"]) && $_POST["dmdnumdocrastreamento"]!="") ? $_POST["dmdnumdocrastreamento"] : NULL;
$dmdunidadedocrastreamento			= (isset($_POST["dmdunidadedocrastreamento"]) && $_POST["dmdunidadedocrastreamento"]!="") ? $_POST["dmdunidadedocrastreamento"] : NULL;
$palavra_chave 						= (isset($_POST["palavra_chave"]) && $_POST["palavra_chave"]!="") ? $_POST["palavra_chave"] : NULL;
$nserie 							= (isset($_POST["nserie"]) && $_POST["nserie"]!="") ? $_POST["nserie"] : NULL;
$npatrimonio 						= (isset($_POST["npatrimonio"]) && $_POST["npatrimonio"]!="") ? $_POST["npatrimonio"] : NULL;
$anexo								= (isset($_POST["anexo"]) && $_POST["anexo"]!="") ? $_POST["anexo"] : NULL;
$nao_exib_finaliz_cancela_invalid	= (isset($_POST["nao_exib_finaliz_cancela_invalid"]) && $_POST["nao_exib_finaliz_cancela_invalid"]!="") ? $_POST["nao_exib_finaliz_cancela_invalid"] : NULL;
$somente_avaliadas	= (isset($_POST["somente_avaliadas"]) && $_POST["somente_avaliadas"]!="") ? $_POST["somente_avaliadas"] : NULL;
$somente_pausadas	= (isset($_POST["somente_pausadas"]) && $_POST["somente_pausadas"]!="") ? $_POST["somente_pausadas"] : NULL;
$somente_anosanteriores	= (isset($_POST["somente_anosanteriores"]) && $_POST["somente_anosanteriores"]!="") ? $_POST["somente_anosanteriores"] : NULL;

// Coloca as datas no padrão DD/MM/YYYY
if(isset($_POST["dataini"]) && $_POST["dataini"] != "") {
	$arData 	= explode("/", $_POST["dataini"]);
	$dataini 	= $arData[1]."/".$arData[0]."/".$arData[2];
} else {
	$dataini	= NULL;
}

if(isset($_POST["datafim"]) && $_POST["datafim"] != "") {
	$arData		= explode("/", $_POST["datafim"]);
	$datafim	= $arData[1]."/".$arData[0]."/".$arData[2];
} else {
	$datafim	= NULL;
}

if(isset($_POST["dmddatadocrastreamento"]) && $_POST["dmddatadocrastreamento"] != "") {
	$arData					= explode("/", $_POST["dmddatadocrastreamento"]);
	$dmddatadocrastreamento = $arData[1]."/".$arData[0]."/".$arData[2];	
} else {
	$dmddatadocrastreamento = NULL;
}



//faz filtro para gerente projeto e analista de sistemas
$mostra = true;
if ( !in_array(DEMANDA_PERFIL_SUPERUSUARIO, $perfil) ){
	if ( in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil) ||
		 in_array(DEMANDA_PERFIL_ANALISTA_SISTEMA, $perfil) ||
		 in_array(DEMANDA_PERFIL_ANALISTA_TESTE, $perfil) ||
		 in_array(DEMANDA_PERFIL_PROGRAMADOR, $perfil)
		){
			$celid2 = $db->carregarColuna(" SELECT distinct celid from demandas.usuarioresponsabilidade where rpustatus='A' and usucpf = '".$_SESSION['usucpf']."' and not celid is null");
			
			if($celid2){
				$arrayCel = implode(",", $celid2);
				$whereResp = " AND ur.celid IN ($arrayCel) ";
				$mostra = false;
			}
			
			if ( in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil) ){
				$mostra = true;
			}
	}
	else{
		if ( in_array(DEMANDA_PERFIL_DBA, $perfil)  ){
			$whereResp = " AND ur.pflcod IN (".DEMANDA_PERFIL_DBA.")";
		}
		if ( in_array(DEMANDA_PERFIL_ANALISTA_WEB, $perfil)  ){
			$whereResp = " AND ur.pflcod IN (".DEMANDA_PERFIL_ANALISTA_WEB.")";
		}		
		if ( in_array(DEMANDA_PERFIL_GESTOR_MEC, $perfil) || in_array(DEMANDA_PERFIL_TECNICO1, $perfil) || in_array(DEMANDA_PERFIL_TECNICO, $perfil)  ){
			$whereResp = " AND ur.pflcod IN (".DEMANDA_PERFIL_GESTOR_MEC.",".DEMANDA_PERFIL_TECNICO1.",".DEMANDA_PERFIL_TECNICO.")";
		}
		
	}
	
	
	if ( in_array(DEMANDA_PERFIL_GERENTE_FSW, $perfil) ||
		 in_array(DEMANDA_PERFIL_ANALISTA_FSW, $perfil) ||
		 in_array(DEMANDA_PERFIL_FISCAL_TECNICO_FSW, $perfil) ||
		 in_array(DEMANDA_PERFIL_GESTOR_EQUIPE, $perfil) ||
		 in_array(DEMANDA_PERFIL_EQUIPE, $perfil) 
		){
			$mostra = true;
			
			$ordidx = $db->carregarColuna("SELECT distinct ordid from demandas.usuarioresponsabilidade where rpustatus='A' and ordid is not null and usucpf = '".$_SESSION['usucpf']."'");
			$celidx = $db->carregarColuna("SELECT distinct celid from demandas.usuarioresponsabilidade where rpustatus='A' and celid is not null and usucpf = '".$_SESSION['usucpf']."'");
			$sididx = $db->carregarColuna("SELECT distinct sidid from demandas.usuarioresponsabilidade where rpustatus='A' and sidid is not null and usucpf = '".$_SESSION['usucpf']."'");
			
			if($ordidx[0]){
				$ordidx = implode(',',$ordidx);
				$ordidx = " and ordid IN (".$ordidx.") ";
			}
			else $ordidx = "";
			
			if($celidx[0]){
				$arrayCel = implode(",",$celidx);
				$celidx = implode(',',$celidx);
				$celidx = " and c.celid IN (".$celidx.") ";
			}
			else $celidx = "";
			
			if($sididx[0]){
				$sididx = implode(',',$sididx);
				$sididx = " and s.sidid IN (".$sididx.") ";
			}
			else $sididx = "";
	}
	
	
}


//trata combo_popup para paginação e ordenação por coluna
		//campo origem da demanda
		if ( in_array(DEMANDA_PERFIL_TECNICO1, $perfil) || in_array(DEMANDA_PERFIL_TECNICO, $perfil) || in_array(DEMANDA_PERFIL_GESTOR_MEC, $perfil) ){
			if(!$_POST['numero'] && !$_POST['ordemlista']){
				if($_POST['ordidarr'][0]){
					for($i=0; $i<count($_POST['ordidarr']);  $i++){
						$ordidarrx .= "|" . $_POST['ordidarr'][$i];
					}
					$_SESSION['ordidarraux'] = substr($ordidarrx, 1);	
				}
				else{
					$_SESSION['ordidarraux'] = "";
				}
			}
			if($_SESSION['ordidarraux']){
				$ordidarrx = str_replace("|","','",$_SESSION['ordidarraux']);
				//parametro para query
				$sqlx = "SELECT ordid as codigo, orddescricao as descricao FROM demandas.origemdemanda WHERE ordid in ('".$ordidarrx."')"; 
				$_POST['ordidarr'] = $db->carregarColuna($sqlx);
				//preenche combopopup
				$ordidarr = $db->carregar($sqlx);
			}
		}
		
		//campo tipo serviço
		if ( in_array(DEMANDA_PERFIL_TECNICO1, $perfil) ){
			if(!$_POST['numero'] && !$_POST['ordemlista']){
				if($_POST['tiposervico'][0]){
					for($i=0; $i<count($_POST['tiposervico']);  $i++){
						$tiposervicox .= "|" . $_POST['tiposervico'][$i];
					}
					$_SESSION['tiposervicoaux'] = substr($tiposervicox, 1);	
				}
				else{
					$_SESSION['tiposervicoaux'] = "";
				}
			}
			if($_SESSION['tiposervicoaux']){
				$tiposervicox = str_replace("|","','",$_SESSION['tiposervicoaux']);
				//parametro para query
				$sqlx = "SELECT	 t.tipid AS codigo,
							 o.orddescricao ||' -> '|| t.tipnome AS descricao
							FROM
							demandas.tiposervico t
							inner join demandas.origemdemanda o ON o.ordid = t.ordid
							WHERE
							 t.tipstatus = 'A' and t.tipid in ('".$tiposervicox."')
							ORDER BY
							 2";
				$_POST['tiposervico'] = $db->carregarColuna($sqlx);
				//preenche combopopup
				$tiposervico = $db->carregar($sqlx);
			}
		}
		
		
		//campo solicitante
		/*
		if(!$_POST['numero'] && !$_POST['ordemlista']){
			if($_POST['solicitante'][0]){
				for($i=0; $i<count($_POST['solicitante']);  $i++){
					$solicitantex .= "|" . $_POST['solicitante'][$i];
				}
				$_SESSION['solicitanteaux'] = substr($solicitantex, 1);	
			}
			else{
				$_SESSION['solicitanteaux'] = "";
			}
		}
		if($_SESSION['solicitanteaux']){
			$solicitantex = str_replace("|","','",$_SESSION['solicitanteaux']);
			//parametro para query
			$sqlx = "SELECT u.usucpf as codigo, u.usunome as descricao 
											FROM seguranca.usuario u
											INNER JOIN seguranca.usuario_sistema us ON us.usucpf = u.usucpf
											INNER  JOIN seguranca.perfilusuario pu ON pu.usucpf = u.usucpf 
											WHERE us.sisid=44 and u.usucpf in ('".$solicitantex."')
											group by u.usunome, u.usucpf
											order by u.usunome, u.usucpf";
			$_POST['solicitante'] = $db->carregarColuna($sqlx);
			//preenche combopopup
			$solicitante = $db->carregar($sqlx);
		}
		*/

		//campo responsavel
		if(!$_POST['numero'] && !$_POST['ordemlista']){
			if($_POST['responsavel'][0]){
				for($i=0; $i<count($_POST['responsavel']);  $i++){
					$responsavelx .= "|" . $_POST['responsavel'][$i];
				}
				$_SESSION['responsavelaux'] = substr($responsavelx, 1);	
			}
			else{
				$_SESSION['responsavelaux'] = "";
			}
		}
		if($_SESSION['responsavelaux']){
			$responsavelx = str_replace("|","','",$_SESSION['responsavelaux']);
			//parametro para query
			$sqlx = "SELECT 
						u.usucpf AS codigo,
						u.usunome || ' - ' || p.pfldsc AS descricao
					FROM 
						seguranca.usuario u
					INNER JOIN demandas.usuarioresponsabilidade ur ON ur.usucpf = u.usucpf
					INNER JOIN seguranca.perfil p ON p.pflcod = ur.pflcod
					Where ur.rpustatus = 'A' and ur.pflcod in (233,235,237,238,248,257,995,996,1024) and u.usucpf in ('".$responsavelx."')
					GROUP BY u.usucpf, u.usunome, p.pfldsc
					order by p.pfldsc, u.usunome";
			$_POST['responsavel'] = $db->carregarColuna($sqlx);
			//preenche combopopup
			$responsavel = $db->carregar($sqlx);
		}
//fim trata combo_popup

?>
<html>
	<head>
	
			 <style type="text/css">
			 .titulo1{background-color:rgb(227, 227, 227);text-align:center;font-weight:bold;}
			 .linha1{background-color:#f5f5f5;}
			 .linha2{background-color:#fdfdfd;}
			 </style>
			 <link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
			 <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
			 <script src="../includes/calendario.js"></script>
			 <script src="./js/ajax.js" type="text/javascript"></script>
			 <script src="./js/demandas.js" type="text/javascript"></script>
			 <script type="text/javascript" src="../includes/prototype.js"></script>
			 <script type="text/javascript" src="../includes/funcoes.js"></script>
			 <script type="text/javascript" src="../includes/remedial.js"></script>
			 <script type="text/javascript" src="../includes/superTitle.js"></script>
			 <script language="JavaScript" src="../includes/wz_tooltip.js"></script>		
	
	<script type="text/javascript">

	function extraiScript(texto){  
		//desenvolvido por Skywalker.to, Micox e Pita.  
		//http://forum.imasters.uol.com.br/index.php?showtopic=165277  
		var ini, pos_src, fim, codigo;  
		var objScript = null;  
		ini = texto.indexOf('<script', 0)  
		while (ini!=-1){  
			var objScript = document.createElement("script");  
			//Busca se tem algum src a partir do inicio do script  
			pos_src = texto.indexOf(' src', ini)  
			ini = texto.indexOf('>', ini) + 1;
	
			//Verifica se este e um bloco de script ou include para um arquivo de scripts  
			if (pos_src < ini && pos_src >=0){//Se encontrou um "src" dentro da tag script, esta e um include de um arquivo script  
				//Marca como sendo o inicio do nome do arquivo para depois do src  
				ini = pos_src + 4;  
				//Procura pelo ponto do nome da extencao do arquivo e marca para depois dele  
				fim = texto.indexOf('.', ini)+4;  
				//Pega o nome do arquivo  
				codigo = texto.substring(ini,fim);  
				//Elimina do nome do arquivo os caracteres que possam ter sido pegos por engano  
				codigo = codigo.replace("=","").replace(" ","").replace("\"","").replace("\"","").replace("\'","").replace("\'","").replace(">","");  
				// Adiciona o arquivo de script ao objeto que sera adicionado ao documento  
				objScript.src = codigo;  
			}else{
			//Se nao encontrou um "src" dentro da tag script, esta e um bloco de codigo script  
				// Procura o final do script
				fim = texto.indexOf('</script', ini);  
				// Extrai apenas o script
				codigo = texto.substring(ini,fim);  
				// Adiciona o bloco de script ao objeto que sera adicionado ao documento  
				objScript.text = codigo;
			}
			
			//Adiciona o script ao documento  
			document.body.appendChild(objScript);
			
			// Procura a proxima tag de <script>  
			ini = texto.indexOf('<script', fim);
			
			//Limpa o objeto de script  
			objScript = null;  
		}  
	}  

	function numeroDemandas(consulta){
		var tipo = consulta;
		var div = document.getElementById('numero_' + tipo); 
		// Faz uma requisição ajax, passando o parametro 'ordid', via POST
		var req = new Ajax.Request('demandas.php?modulo=principal/lista&acao=A', {
				        method:     'post',
				        parameters: '&numeroDemandasAjax=' + tipo,
				        onComplete: function (res)
				        {	
							div.innerHTML = res.responseText;
							div.style.display = "";
				        }
				  });
	
	}
	
	/**
	 * Alterar visibilidade de um campo.
	 * 
	 * @param string indica o campo a ser mostrado/escondido
	 * @return void
	 */
	function onOffCampo( campo )
	{
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
		}
	}
	
	// Função para exibir os campos da pesquisa avançada
	function mostraPesquisaAvancada() {
		// Variáveis referentes as linhas(tr's) e div's que deverão ser exibidas/escondidas.
		var pesqAvancada	=	document.getElementById('pesq_avancada');
		var fPesqAvancada 	= 	document.getElementById('fechar_pesq_avancada');
		var trMotivador 	=	document.getElementById('tr_motivador');
		var trAnexo			=	document.getElementById('tr_anexo');
		var trNseriepatr	=	document.getElementById('tr_nseriepatr');
		var trSolicitante	= 	document.getElementById('tr_solicitante');
		var trResponsavel	= 	document.getElementById('tr_responsavel');
		var trNaoExibir 	=	document.getElementById('tr_nao_exibir');
		var trAvaliadas 	=	document.getElementById('tr_avaliadas');
		var trAnosanteriores 	=	document.getElementById('tr_anosanteriores');
		var trPausadas 		=	document.getElementById('tr_pausadas');
		var divMotivNumDoc	=	document.getElementById('num_doc');
		var divMotivData	=	document.getElementById('data_motivador');
		var trOrgao			=	document.getElementById('tr_orgao');
		
		// Variáveis referentes aos campos da pesquisa avançada
		var motivador	=	document.getElementById('motid');
		var dataMotiv	=	document.getElementById('dmddatadocrastreamento');
		var numdocMotiv	=	document.getElementById('dmdnumdocrastreamento');
		var orgaoMotiv	=	document.getElementById('dmdunidadedocrastreamento');
		var palavChave	=	document.getElementById('palavra_chave');
		var nSerie		=	document.getElementById('nserie');
		var nPatrimonio	=	document.getElementById('npatrimonio');
		var anexo		=	document.getElementById('anexo');
		var naoExibir	=	document.getElementById('nao_exib_finaliz_cancela_invalid');
		var somenteAvaliadas	=	document.getElementById('somente_avaliadas');
		var somenteAnosanteriores	=	document.getElementById('somente_anosanteriores');
		var somentePausadas	=	document.getElementById('somente_pausadas');
		var solicitante	=	document.getElementById('solicitante');
		var responsavel	=	document.getElementById('responsavel');
		var tamanhoResp = 	responsavel.length;
		var tamanhoSolicit = 	solicitante.length;
		var divRespons	=	document.getElementById('responsavel_campo_on');
		var divSolicit	=	document.getElementById('solicitante_campo_on');
		
		// Se for Internet Explorer usa block (restrição).
		var exibe = (document.selection) ? 'block' : 'table-row';
		
		// Oculta a TR da pesquisa avançada e exibe os campos
		if(pesqAvancada.style.display == exibe) {
			pesqAvancada.style.display	= 'none';
			trMotivador.style.display 	= exibe;
			trAnexo.style.display 		= exibe;
			trNseriepatr.style.display 	= exibe;
			trSolicitante.style.display = exibe;
			trResponsavel.style.display = exibe;
			trNaoExibir.style.display	= exibe;
			trAvaliadas.style.display	= exibe;
			trAnosanteriores.style.display	= exibe;
			trPausadas.style.display	= exibe;
			fPesqAvancada.style.display	= exibe;
		} 
		// Oculta e limpa todos os campos, e exibe a TR da pesquisa avançada
		else {
			// oculta
			pesqAvancada.style.display		= exibe;
			trMotivador.style.display 		= 'none';
			trAnexo.style.display 			= 'none';
			trNseriepatr.style.display 		= 'none';
			trSolicitante.style.display 	= 'none';
			trResponsavel.style.display 	= 'none';
			trNaoExibir.style.display		= 'none';
			trAvaliadas.style.display		= 'none';
			trAnosanteriores.style.display		= 'none';
			trPausadas.style.display		= 'none';
			fPesqAvancada.style.display		= 'none';
			divMotivNumDoc.style.display	= 'none';
			divMotivData.style.display		= 'none';
			trOrgao.style.display			= 'none';
				
			if(naoExibir.checked == true) {	naoExibir.checked = false; }
			if(somenteAvaliadas.checked == true) {	somenteAvaliadas.checked = false; }
			if(somenteAnosanteriores.checked == true) {	somenteAnosanteriores.checked = false; }
			if(somentePausadas.checked == true) {	somentePausadas.checked = false; }
			//if(divSolicit.style.display != 'none') { onOffCampo( 'solicitante' ); }
			if(divRespons.style.display != 'none') { onOffCampo( 'responsavel' ); }
			
			// limpa os campos
			motivador.value = "";
			dataMotiv.value = "";
			numdocMotiv.value = "";
			orgaoMotiv.value = "";
			palavChave.value = "";
			anexo.value = "";

			// Remove todos os options do combo_popup de 'Solicitante'
			for(var i=0; i < tamanhoSolicit; i++) {
				solicitante.remove(0);
			}
			
			// Remove todos os options do combo_popup de 'Responsável'
			for(var i=0; i < tamanhoResp; i++) {
				responsavel.remove(0);
			}
			
			// Insere a opção/frase padrão do componente quando se retira todos os options.
			// Restrição do componente que não pode ficar sem nenhum option na lista.
			var option	 	= document.createElement("option");
			option.text 	= "Duplo clique para selecionar da lista";
			option.value	= "";
			//solicitante.add(option, null);
			responsavel.add(option, null);
		}
	}

	function buscaCodigo(e){

		var key; 
		if(e && e.which){
			key = e.which;
		}
		else if(window.event){
			e = window.event;
			key = e.keyCode;
		}

		if(key == 13 && document.formulario.codigo.value!='') document.formulario.submit();
	}


	</script>
	</head>
<body>
<form action="" method="POST" name="formulario">
	<input type="hidden" name="submetido" value="1" />
	
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="0">
		<tr>
			<td style="background-color:#e9e9e9; color:#404040;" >
			  <table align="center" border="0"  cellpadding="0" cellspacing="0" >
			  	<tr>
			  		<td style="background-color:#e9e9e9; color:#404040;">
			  		
						<table border="0"  cellpadding="3" cellspacing="0">
							<tr>
								<td width="10%" valign="top">
									<B>Código:</B>
									<br>
									<?= campo_texto( 'codigo', 'N', 'S', '', 10, 15, '##########', '', '','','','','buscaCodigo(event)' ); ?>
								</td>							
								<td >
									<B>Setor:</B>
									<br>
									<?php
									$sql = "SELECT
											  unaid AS codigo, 
										  	  upper(unasigla)||' - '||unadescricao as descricao
											 FROM
											  demandas.unidadeatendimento 
											 WHERE
											  unastatus = 'A'
											  order by unasigla, unadescricao";
									$db->monta_combo( "unaid", $sql, 'S', 'Filtro...', '', '','','320','','unaid');
									?>
								</td>									
							</tr>
							<tr>
								<td colspan="2">
									<B>Situação:</B>
									<br>
									<? 
										$esdid = $_REQUEST['esdid'];
										//dbg($esdid);
									?> 
									<select name='esdid'  class='CampoEstilo'  style='width: auto' id="esdid">
										<option value="">Filtro...</option>
										<option value='EP' <?if($esdid=='EP') echo "selected";?> >Em Processamento</option>
										<option value='91,107' <?if($esdid=='91,107') echo "selected";?> >Em Análise</option>
										<option value='92,108' <?if($esdid=='92,108') echo "selected";?> >Em Atendimento</option>
										<option value='93,111' <?if($esdid=='93,111') echo "selected";?> >Aguardando Validação</option>
										<option value='278' <?if($esdid=='278') echo "selected";?> >Aguardando Validação do Demandante</option>
										<option value='772' <?if($esdid=='772,890') echo "selected";?> >Auditada</option>
										<option value='1336' <?if($esdid=='1336') echo "selected";?> >Aguardando Pagamento</option>
										<option value='1337' <?if($esdid=='1337') echo "selected";?> >Pago</option>
										<!-- <option value='94,112' <?if($esdid=='94,112') echo "selected";?> >Aguardando avaliação</option> -->
										<option value='95,109,170' <?if($esdid=='95,109,170') echo "selected";?> >Validada / Validada Sem Pausa / Finalizada</option>
										<option value='135,136' <?if($esdid=='135,136') echo "selected";?> >Invalidada</option>
										<option value='100,110' <?if($esdid=='100,110') echo "selected";?> >Cancelada</option>
										<option value='EA' <?if($esdid=='EA') echo "selected";?> >Em Aberto (Em Processamento, Análise e Atendimento)</option>
									</select>
									 
								</td>
							</tr>
							<tr>
								<td colspan="3" align="left">
									<table border="0" cellpadding="3" cellspacing="0" >
										<?if($mostra){?>
										<tr>
											<td>
												<B>Origem da demanda:</B>
												<br>
												<?
												if ( in_array(DEMANDA_PERFIL_TECNICO1, $perfil) || in_array(DEMANDA_PERFIL_TECNICO, $perfil) || in_array(DEMANDA_PERFIL_GESTOR_MEC, $perfil) ){
													?>
													<div id="ordidarr_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'ordidarr' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
													<div id="ordidarr_campo_on" style="display:none;">
													
													<?php
													$sql_combo = "SELECT ordid AS codigo, orddescricao AS descricao FROM demandas.origemdemanda WHERE ordstatus = 'A' ORDER BY orddescricao";
												 	combo_popup( 'ordidarr', $sql_combo, 'Selecione a(s) Origem(ns)', '400x400', 0, array(), '', 'S', true, true );
													?>
													</div>
													<? if ($ordidarr) { ?> <script type="text/javascript"> onOffCampo( 'ordidarr' ); </script> <? } ?>
												<?}
												else{
													$ordidx = stripslashes($ordidx);
													$sql = "SELECT ordid AS codigo, orddescricao AS descricao FROM demandas.origemdemanda WHERE ordstatus = 'A' $ordidx ORDER BY orddescricao";
													$db->monta_combo( "ordid", $sql, 'S', 'Filtro...', 'mostraCelsis', '','','','','ordid');
												}
												?>
											</td>
											<td id="td_celula" style="display:none">
												<?//if($ordid==1){?>
													<B>Célula:</B>
													<br>
													<?php
													//$celid = $_REQUEST['celid'];
													$sql = "SELECT
															 c.celid AS codigo,
															 c.celnome || ' - Gerente: ' || 
															 CASE 
															 	 WHEN u.usunome != '' THEN u.usunome
																 ELSE ' - '
															 END as DESCRICAO
															FROM
															 demandas.celula c
												 			 LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.celid = c.celid AND ur.pflcod = ".DEMANDA_PERFIL_GERENTE_PROJETO." AND ur.rpustatus = 'A' 
												 			 LEFT JOIN seguranca.usuario u ON u.usucpf = ur.usucpf 
															WHERE
															 c.celstatus = 'A'
															 --and c.celid not in (9) --celula web
															 $celidx
															ORDER BY
															 c.celnome;";
													$db->monta_combo( "celid", $sql, 'S', 'Filtro...', 'forcaWidth();filtraSistema', '','','375','','celid');
												//}
												?>
											</td>
										</tr>
										<?}?>
										<tr>
											<td colspan="2" id="td_sistema" style="display:none">
												<?//if($ordid==1){?>
													<B>Sistema:</B>
													<br>
													<?php
													//$sidid = $_REQUEST['sidid'];
													if(!$celid) $celid = 0;
													if(!$arrayCel) $arrayCel = $celid;
													//if($arrayCel){
														$sql = "SELECT DISTINCT s.sidid  AS codigo
                                                                    , upper(s.sidabrev) || ' - ' || s.siddescricao AS descricao 
																		FROM 
																			demandas.sistemadetalhe s
																		LEFT JOIN demandas.sistemacelula c on s.sidid = c.sidid  
																		WHERE  s.sidstatus = 'A'
																		AND celid in (".$arrayCel.")
																		$sididx";
														$db->monta_combo( "sidid", $sql, 'S', 'Filtro...', '', '','','547','','sidid');
													//}
												//}
												?>
											</td>
										</tr>
										<?if(!$mostra) echo "<script>document.getElementById('td_sistema').style.display = '';</script>"; ?>
									</table>
								</td>
							</tr>
							<?if ( in_array(DEMANDA_PERFIL_TECNICO1, $perfil) ){?>
							<tr>
								<td >
									<B>Tipo Serviço:</B>
									<br>
										<div id="tiposervico_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'tiposervico' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
										<div id="tiposervico_campo_on" style="display:none;">
										
										<?
										$sql_combose = "SELECT t.tipid AS codigo, o.orddescricao ||' -> '|| t.tipnome AS descricao FROM demandas.tiposervico t inner join demandas.origemdemanda o ON o.ordid = t.ordid WHERE t.tipstatus = 'A' ORDER BY 2";
									 	combo_popup( 'tiposervico', $sql_combose, 'Selecione o(s) Tipo(s)', '400x400', 0, array(), '', 'S', true, true );
										?>
										</div>
										<? if ($tiposervico) { ?> <script type="text/javascript"> onOffCampo( 'tiposervico' ); </script> <? } ?>
								</td>
							</tr>
							<?}?>
							<tr>
								<td colspan="3">
									<B>Período:</B>
									<br>
									<?=campo_data('dataini', 'N', 'S', '', 'S' );?>
									&nbsp;&nbsp;&nbsp;&nbsp;
									até
									&nbsp;&nbsp;&nbsp;&nbsp;
									<?=campo_data('datafim', 'N', 'S', '', 'S' );?>
								</td>	
							</tr>
							<tr id="pesq_avancada">
								<td colspan="3">
									<a href="javascript:void(0);" onclick="mostraPesquisaAvancada();">[+] Pesquisa Avançada</a>
								</td>
							</tr>
							<script> 
								if(document.selection)
									document.getElementById("pesq_avancada").style.display = "block";
								else
									document.getElementById("pesq_avancada").style.display = "table-row";
							</script>
							<tr id="tr_motivador" style="display:none;">
								<td colspan="3">
									<div style="float:left;" >
									<B>Motivador:</B>
									<br>
									<?
										//$motid = $_REQUEST['motid'];
										$sql = "select  
															m.motid  AS codigo, 
															m.motdesc AS descricao 
														from 
															demandas.motivador m  
														where  m.motstatus = 'A'
														order by m.motdesc";
										$db->monta_combo( "motid", $sql, 'S', 'Filtro...', 'dadosMotivador', '','','','','motid');
									?>
									</div>
									
									<div id = "data_motivador" style="float:left;padding-left:40px;display:<?($motid == 1 || $motid == 2 || $motid == 3)? print '' : print 'none'; ?>" >
									<B>Data:</B>
									<br>
									<?= campo_data( 'dmddatadocrastreamento', 'N', 'S', '', 'S' ); ?>
									</div>
									<div id = "num_doc" style="float:left;padding-left:40px;display:<?($motid == 1 || $motid == 2)? print '' : print 'none'; ?>" >
									<B>Número do Documento:</B>
									<br>
									<? ($motid != 1 && $motid != 2)? $dmdnumdocrastreamento=NULL : ""; ?>
									<?= campo_texto( 'dmdnumdocrastreamento', 'N', 'S', '', 20, 30, '','','','','','id="dmdnumdocrastreamento"','',''); ?>
									</div>
								</td>	
							</tr>
							<tr id="tr_orgao" style="display:<?($motid == 1 || $motid == 2) ? print '' : print 'none'; ?>;">
								<td colspan="3">
								<div style="float:left;">
								<b>Órgão:</b>
								<br>
								<?php
									//$dmdunidadedocrastreamento = $_REQUEST['dmdunidadedocrastreamento'];
									$sql = "SELECT
											  unaid AS codigo, 
										  	  upper(unasigla)||' - '||unadescricao as descricao 
											 FROM
											  demandas.unidadeatendimento 
											 WHERE
											  unastatus = 'A'
											  order by unasigla, unadescricao";
									$db->monta_combo("dmdunidadedocrastreamento", $sql, 'S', "-- Selecione um Orgão --", '', '', '', '', '', 'dmdunidadedocrastreamento');
								?>
								</div>
								</td>
							</tr>
							<tr id="tr_anexo" style="display:none;">
								<td colspan="3">
									<div style="float:left;">
									<B>Palavra-Chave:</B>
									<br>
									<?= campo_texto( 'palavra_chave', 'N', 'S', '', 30, 60, '', '', '', '', '', 'id="palavra_chave"', '', ''); ?>
									</div>
									<div style="float:left;margin-left:40px;">
									<B>Possui Anexo:</B>
									<br>
									<select style="width: auto;" class="CampoEstilo" name="anexo" id="anexo" >
									<option <?(!$anexo)? "selected=\"selected\"" : "" ?> value="">Filtro...</option>
									<option <?($anexo == "true")? print "selected=\"selected\"" : "" ?>  value="true">Sim</option>
									<option <?($anexo == "false")? print "selected=\"selected\"" : "" ?> value="false">Não</option>
									</select>
									</div>
								</td>
							</tr>
							<?//if($mostra_solicitante){?>
									<tr id="tr_solicitante" style="display:none;">
										<td colspan="3">
											<b>Solicitante:</b>
											<br>
											<!--
											<div id="solicitante_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'solicitante' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
											<div id="solicitante_campo_on" style="display:none;">
											-->
											<?
												/*
											   $sql_combo = "	SELECT 
																	u.usucpf AS codigo,
																	TRANSLATE(upper(u.usunome), 'áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ','aaaaeeiooouucAAAAEEIOOOUUC') || ' - ' AS descricao
																FROM 
																	seguranca.usuario u
																INNER JOIN seguranca.usuario_sistema us ON us.usucpf = u.usucpf
																INNER  JOIN seguranca.perfilusuario pu ON pu.usucpf = u.usucpf
																where us.sisid=44 and us.suscod='A'
																group by u.usucpf, u.usunome
																order by u.usunome
														   	";
												*/
												//$sql_combo = "SELECT distinct u.usucpf AS codigo, u.usunome AS descricao FROM seguranca.usuario u INNER JOIN seguranca.usuario_sistema us ON us.usucpf = u.usucpf INNER JOIN seguranca.perfilusuario pu ON pu.usucpf = u.usucpf where us.sisid=44 and us.suscod='A' and pu.pflcod in (select pflcod from seguranca.perfil where sisid=44) and u.usucpf in (select usucpfdemandante from demandas.demanda group by usucpfdemandante) order by 2";
												/*
												$sql_combo = "SELECT distinct u.usucpf AS codigo, u.usunome AS descricao FROM seguranca.usuario u
																INNER JOIN seguranca.usuario_sistema us ON us.usucpf = u.usucpf
																INNER JOIN demandas.usuarioresponsabilidade ur ON ur.usucpf = u.usucpf
																where us.sisid=44 and us.suscod='A'
																order by 2";
											 	combo_popup( 'solicitante', $sql_combo, 'Selecione o(s) Solicitante(s)', '400x400', 0, array(), '', 'S', true, true );
												*/
												echo campo_texto( 'solicitante', 'N', 'S', '', 48, 60, '', '', '', '', '', 'id="solicitante"', '', '');
											?>
											<!--</div>-->
										</td>
									</tr>
									<? if ($solicitante) { ?> <script type="text/javascript"> //onOffCampo( 'solicitante' ); </script> <? } ?>
							<?//}?>
							<tr id="tr_responsavel" style="display:none;">
								<td colspan="3">
									<b>Técnico Responsável:</b>
									<br>
									<div id="responsavel_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'responsavel' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
									<div id="responsavel_campo_on" style="display:none;">
									<?
										$sql_combo = "SELECT u.usucpf AS codigo, u.usunome || ' (' || p.pfldsc || ')' AS descricao FROM seguranca.usuario u INNER JOIN demandas.usuarioresponsabilidade ur ON ur.usucpf = u.usucpf INNER JOIN seguranca.perfil p ON p.pflcod = ur.pflcod Where ur.rpustatus = 'A' and ur.pflcod in (233,235,237,238,248,257,995,996,1024) $whereResp GROUP BY u.usunome, p.pfldsc, u.usucpf ORDER BY u.usunome";
									 	combo_popup( 'responsavel', $sql_combo, 'Selecione o(s) Responsável(eis)', '400x400', 0, array(), '', 'S', true, true );
									?>
									</div>
								</td>
							</tr>
							<? if ($responsavel) { ?> <script type="text/javascript"> onOffCampo( 'responsavel' ); </script> <? } ?>
							<tr id="tr_nseriepatr" style="display:none;">
								<td colspan="3">
									<div style="float:left;">
									<B>Nº Série:</B>
									<br>
									<?= campo_texto( 'nserie', 'N', 'S', '', 30, 40, '', '', '', '', '', 'id="nserie"', '', ''); ?>
									</div>
									<div style="float:left;margin-left:40px;">
									<B>Nº Patrimônio:</B>
									<br>
									<?= campo_texto( 'npatrimonio', 'N', 'S', '', 30, 40, '', '', '', '', '', 'id="npatrimonio"', '', ''); ?>
									</div>
								</td>
							</tr>							
							
							<tr id="tr_anosanteriores" style="display:none;">
								<td colspan="3">
									<b>Exibir demandas de anos anteriores a 2014 e 2015:</b>
									<input type="checkbox" name="somente_anosanteriores" id="somente_anosanteriores" <? echo $vChecked4 = ($somente_anosanteriores=="on") ? 'checked="checked"' : '' ?>>
								</td>
							</tr>
							<tr id="tr_avaliadas" style="display:none;">
								<td colspan="3">
									<b>Exibir somente demandas avaliadas:</b>
									<input type="checkbox" name="somente_avaliadas" id="somente_avaliadas" <? echo $vChecked2 = ($somente_avaliadas=="on") ? 'checked="checked"' : '' ?>>
								</td>
							</tr>
							<tr id="tr_pausadas" style="display:none;">
								<td colspan="3">
									<b>Exibir somente demandas pausadas:</b>
									<input type="checkbox" name="somente_pausadas" id="somente_pausadas" <? echo $vChecked3 = ($somente_pausadas=="on") ? 'checked="checked"' : '' ?>>
								</td>
							</tr>
							<tr id="tr_nao_exibir" style="display:none;">
								<td colspan="3">
									<b>Exibir demandas finalizadas/validadas/invalidadas/canceladas:</b>
									<input type="checkbox" name="nao_exib_finaliz_cancela_invalid" id="nao_exib_finaliz_cancela_invalid" <? echo $vChecked = ($nao_exib_finaliz_cancela_invalid=="on") ? 'checked="checked"' : '' ?>>
								</td>
							</tr>
							<tr id="fechar_pesq_avancada" style="display:none;">
								<td colspan="3">
									<a href="javascript:void(0);" onclick="mostraPesquisaAvancada();">[-] Fechar Pesquisa Avançada</a>
								</td>
							</tr>
							</table>
						<?php
							// Se algum dos campos da pesquisa avançada tiver sido utilizado, 
							// estes serão exibidos no carregamento da página
							if($motid || $dmdnumdocrastreamento || $anexo || $palavra_chave || $nserie || $npatrimonio || $solicitante ||
							   $responsavel || $nao_exib_finaliz_cancela_invalid || $somente_avaliadas || $somente_anosanteriores || $somente_pausadas || $dmddatadocrastreamento) {
							   		echo '<script type="text/javascript">
							   				 mostraPesquisaAvancada();
							   			  </script>';
							   }
						?>
						<div style="float: left;margin-top:15px;">	
							<input style="cursor:pointer;" type="button" onclick="submeteDados();" name="pesquisar" value="Pesquisar"/>
							<input style="cursor:pointer;" type="button" onclick="limparDados();" name="limpar" value="Limpar"/>
						</div>
					</td>
				</tr>
				</table>
			  		
			  		
			  		</td>
			  		<td style="background-color:#e9e9e9; color:#404040;">
			  		
						<div style="float:center;position:relative;width:350px;">
							<fieldset style="background-color:#ffffff;">
							<legend> <img id="mais_consulta" style="cursor:pointer;display:none" onclick="exibeConsultaRapida();" src="../imagens/mais.gif" /> <img  style="cursor:pointer" id="menos_consulta" onclick="escondeConsultaRapida();" src="../imagens/menos.gif" /> <b>Consultas Rápidas - <a href="javascript:mostraTotais();">(ver totais)</a></b></legend>
							<div style="width:340px"></div>
							<table id="consultas_rapidas" width="100%" >
								<tr>
									<td valign="bottom" height="50%" style="padding-right:15px;">
										<a style="cursor:pointer;<?($todas)? print "font-weight:bold;" : ""; ?>" onclick="selecionaTodas();todasDemandas();"  title="Listar todas as Demandas disponíveis.">Todas<span id="numero_todas"></span></a>
										<input style="display:none" type="radio" id="todas" name="consultaRapida" value="todas" <?=$todas ? 'checked' : ''?>>
									</td>
									<td valign="bottom">
										<a style="cursor:pointer;<?($hoje)? print "font-weight:bold;" : ""; ?>" title="Listar Demandas cadastradas hoje." onclick="selecionaHoje();demandasHoje();">Demandas de hoje<span id="numero_hoje"></span></a>
										<input style="display:none" type="radio" id="hoje" name="consultaRapida" value="hoje" <?=$hoje ? 'checked' : ''?>>
									</td>
								</tr>
								<tr>
									<td valign="bottom">
										<a style="cursor:pointer;<?($nova)? print "font-weight:bold;" : ""; ?>" title="Listar Demandas que ainda não foram iniciadas." onclick="selecionaNovas();novasDemandas();">Novas<span id="numero_nova"></span></a>
										<input style="display:none" type="radio" id="nova" name="consultaRapida" value="nova" <?=$nova ? 'checked' : ''?>>
									</td>
									<td valign="bottom">
										<a style="cursor:pointer;<?($ematraso)? print "font-weight:bold;" : ""; ?>" title="Listar Demandas em Atraso." onclick="selecionaEmAtraso();demandasEmAtraso();">Demandas em atraso<span id="numero_ematraso"></span></a>
										<input style="display:none" type="radio" id="ematraso" name="consultaRapida" value="ematraso" <?=$ematraso ? 'checked' : ''?> >
									</td>
								</tr>
								<tr>
									<td valign="bottom">
										<a style="cursor:pointer;<?($vencehoje)? print "font-weight:bold;" : ""; ?>" title="Listar Demandas com Vencimento Hoje." onclick="selecionaVenceHoje();demandasVenceHoje();">Demandas Vencendo Hoje<span id="numero_vencehoje"></span></a>
										<input style="display:none" type="radio" id="vencehoje" name="consultaRapida" value="vencehoje" <?=$vencehoje ? 'checked' : ''?> >
									</td>
									<td valign="bottom">
										<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
											<a style="cursor:pointer;<?($minha)? print "font-weight:bold;" : ""; ?>" title="Listar minhas Demandas." onclick="selecionaMinhasDemandas();minhasDemandas();">Minhas demandas<span id="numero_minhademanda"></span></a>
											<input style="display:none" type="radio" id="minhademanda" name="consultaRapida" value="minhademanda" <?=$minha ? 'checked' : ''?> >
										<?//}?>
									</td>
								</tr>
													
							</table>
							</fieldset>
		
							<fieldset style="background-color:#ffffff;">
							<legend> <img id="mais_consulta1" style="cursor:pointer;display:none" onclick="exibeConsultaRapida1();" src="../imagens/mais.gif" /> <img  style="cursor:pointer" id="menos_consulta1" onclick="escondeConsultaRapida1();" src="../imagens/menos.gif" /> <b>Consulta por Situação - <a href="javascript:mostraTotais1();">(ver totais)</a></b></legend>
							<div style="width:340px"></div>
							<table id="consultas_rapidas1" width="100%" >
								<tr>
									<td valign="bottom" height="50%" style="padding-right:15px;">
										<a style="cursor:pointer;<?($emprocessamento)? print "font-weight:bold;" : ""; ?>" title="Listar Demandas Em Processamento." onclick="selecionaEmProcessamento();demandasEmProcessamento();">Em Processamento<span id="numero_emprocessamento"></span></a>
										<input style="display:none" type="radio" id="emprocessamento" name="consultaRapida" value="emprocessamento" <?=$emprocessamento ? 'checked' : ''?>>
									</td>
									<td valign="bottom">
										<a style="cursor:pointer;<?($emanalise)? print "font-weight:bold;" : ""; ?>" onclick="selecionaEmAnalise();demandasEmAnalise();"  title="Listar Demandas Em Análise.">Em Análise<span id="numero_emanalise"></span></a>
										<input style="display:none" type="radio" id="emanalise" name="consultaRapida" value="emanalise" <?=$emanalise ? 'checked' : ''?>>
									</td>
								</tr>
								<tr>
									<td valign="bottom">
										<a style="cursor:pointer;<?($ematendimento)? print "font-weight:bold;" : ""; ?>" title="Listar Demandas Em Atendimento." onclick="selecionaEmAtendimento();demandasEmAtendimento();">Em Atendimento<span id="numero_ematendimento"></span></a>
										<input style="display:none" type="radio" id="ematendimento" name="consultaRapida" value="ematendimento" <?=$ematendimento ? 'checked' : ''?>>
									</td>
									<td valign="bottom">
										<a style="cursor:pointer;<?($agvalidacao)? print "font-weight:bold;" : ""; ?>" title="Listar Demandas Aguardando Validação." onclick="selecionaAgValidacao();demandasAgValidacao();">Aguardando Validação<span id="numero_agvalidacao"></span></a>
										<input style="display:none" type="radio" id="agvalidacao" name="consultaRapida" value="agvalidacao" <?=$agvalidacao ? 'checked' : ''?>>
									</td>
								</tr>
								<tr>
									<td valign="bottom">
										<a style="cursor:pointer;<?($invalidada)? print "font-weight:bold;" : ""; ?>" title="Listar Demandas Invalidadas." onclick="selecionaInvalidada();demandasInvalidada();">Invalidada<span id="numero_invalidada"></span></a>
										<input style="display:none" type="radio" id="invalidada" name="consultaRapida" value="invalidada" <?=$invalidada ? 'checked' : ''?> >
									</td>
									<td valign="bottom">
										<a style="cursor:pointer;<?($agvalidacaodem)? print "font-weight:bold;" : ""; ?>" title="Listar Demandas Aguardando Validação do Demandante." onclick="selecionaAgValidacaoDem();demandasAgValidacaoDem();">Aguardando Validação do Demandante<span id="numero_agvalidacaodem"></span></a>
										<input style="display:none" type="radio" id="agvalidacaodem" name="consultaRapida" value="agvalidacaodem" <?=$agvalidacaodem ? 'checked' : ''?>>
									</td>
								</tr>
								<tr>
									<td valign="bottom" >
										<a style="cursor:pointer;<?($auditada)? print "font-weight:bold;" : ""; ?>" title="Listar Demandas Auditada." onclick="selecionaAuditada();demandasAuditada();">Auditada<span id="numero_auditada"></span></a>
										<input style="display:none" type="radio" id="auditada" name="consultaRapida" value="auditada" <?=$auditada ? 'checked' : ''?>>
									</td>
									<td valign="top">
										<a style="cursor:pointer;<?($cancelada)? print "font-weight:bold;" : ""; ?>" title="Listar Demandas Canceladas." onclick="selecionaCancelada();demandasCancelada();">Cancelada<span id="numero_cancelada"></span></a>
										<input style="display:none" type="radio" id="cancelada" name="consultaRapida" value="cancelada" <?=$cancelada ? 'checked' : ''?> >
									</td>
								</tr>
								<tr>
									<td valign="bottom" >
										<a style="cursor:pointer;<?($finalizada)? print "font-weight:bold;" : ""; ?>" title="Listar Demandas Finalizadas." onclick="selecionaFinalizada();demandasFinalizada();">Finalizada / Validada / <br>Validada Sem Pausa<span id="numero_finalizada"></span></a>
										<input style="display:none" type="radio" id="finalizada" name="consultaRapida" value="finalizada" <?=$finalizada ? 'checked' : ''?>>
									</td>
									<td valign="top">
										<a style="cursor:pointer;<?($agpago)? print "font-weight:bold;" : ""; ?>" title="Listar Demandas Aguardando Pagamento." onclick="selecionaAgpago();demandasAgpago();">Aguardando Pagamento<span id="numero_agpago"></span></a>
										<input style="display:none" type="radio" id="agpago" name="consultaRapida" value="agpago" <?=$agpago ? 'checked' : ''?>>
										
										<br>
										
										<a style="cursor:pointer;<?($pago)? print "font-weight:bold;" : ""; ?>" title="Listar Demandas Pago." onclick="selecionaPago();demandasPago();">Pago<span id="numero_pago"></span></a>
										<input style="display:none" type="radio" id="pago" name="consultaRapida" value="pago" <?=$pago ? 'checked' : ''?>>
									</td>
								</tr>
								
							</table>
							</fieldset>
		
							<fieldset style="background-color:#ffffff;">
							<legend> <img id="mais_consulta2" style="cursor:pointer;display:none" onclick="exibeConsultaRapida2();" src="../imagens/mais.gif" /> <img  style="cursor:pointer" id="menos_consulta2" onclick="escondeConsultaRapida2();" src="../imagens/menos.gif" /> <b>Consulta por Prioridade - <a href="javascript:mostraTotais2();">(ver totais)</a></b></legend>
							<div style="width:340px"></div>
							<table id="consultas_rapidas2" width="100%" >
								<tr>
									<td valign="bottom" height="50%" style="padding-right:15px;">
										<a style="cursor:pointer;<?($urgente)? print "font-weight:bold;" : ""; ?>" onclick="selecionaUrgente();demandasUrgente();"  title="Listar Demandas com Prioridade Alta.">Alta<span id="numero_urgente"></span></a>
										<input style="display:none" type="radio" id="urgente" name="consultaRapida" value="urgente" <?=$urgente ? 'checked' : ''?>>
									</td>
									<td valign="bottom">
										<a style="cursor:pointer;<?($alta)? print "font-weight:bold;" : ""; ?>" title="Listar Demandas com Prioridade Média." onclick="selecionaAlta();demandasAlta();">Média<span id="numero_alta"></span></a>
										<input style="display:none" type="radio" id="alta" name="consultaRapida" value="alta" <?=$alta ? 'checked' : ''?>>
									</td>
									<td valign="bottom">
										<a style="cursor:pointer;<?($normal)? print "font-weight:bold;" : ""; ?>" title="Listar Demandas com Prioridade Baixa." onclick="selecionaNormal();demandasNormal();">Baixa<span id="numero_normal"></span></a>
										<input style="display:none" type="radio" id="normal" name="consultaRapida" value="normal" <?=$normal ? 'checked' : ''?>>
									</td>
									<td valign="bottom">
										<a style="cursor:pointer;<?($ndas)? print "font-weight:bold;" : ""; ?>" title="Listar Demandas com Prioridade N/A." onclick="selecionaNdas();demandasNdas();">N/A<span id="numero_ndas"></span></a>
										<input style="display:none" type="radio" id="ndas" name="consultaRapida" value="ndas" <?=$ndas ? 'checked' : ''?>>
									</td>
								</tr>
							</table>
							</fieldset>
							
							<br>
							
							<? 
							//Pega array com os perfis do usuário que podem acessar o icone imprimir demanda.
							if (in_array(DEMANDA_PERFIL_TECNICO1,$perfil) || in_array(DEMANDA_PERFIL_TECNICO,$perfil) ){
							?>
								<img src="../imagens/print.gif" align='absmiddle' onclick="javascript:abrirPopupDemandas();" style="cursor: pointer;" title="Imprimir Demandas" border="0"> <a href="javascript:abrirPopupDemandas();">Imprimir relatório de atendimento do técnico </a>
							<?}?>
							
						</div>
			  		
			  		
			  		
			  		
			  		</td>
			  	</tr>
			  
			  </table>	
			  
			 	
</form>
<?php
if($_POST["submetido"] == '1') {
	$sql = lista();
 	
	//$db->monta_lista( $sql['sql'], $sql['cabecalho'], 50, 10, 'N', '', '' );
	$dadosLista = $db->carregar($sql['sql']);
	if($dadosLista){
		$db->monta_lista_array($dadosLista, $sql['cabecalho'],50,10,'N','center');
	}
	else{
		?>
			<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem"><tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr></table>
		<?
	}
}
else{
	?>
	<table class="listagem" width="95%" align="center">
		<tr>
			<td align="center">
				<font color="red"><b>Escolha seu filtro e clique no botão pesquisar!<b/></font>
			</td>
		</tr>
	</table>
<?}?>
</body>
</html>

<script type="text/javascript">
	//varial global
	d = document;

	//se origem de demanda for igual a 1 (sistemas de informações)
	<?if($mostra){?>
	mostraCelsis(<?=$ordid?>);
	<?}?>
	
	function submeteDados() {
		var resp = document.getElementById("responsavel");
		var solicit = document.getElementById("solicitante");
		
		//limpaConsultaRapida();
		selectAllOptions(formulario.responsavel);
		//selectAllOptions(formulario.solicitante);
		
		if(resp.value == "") {
			resp.remove(0);
		}
		if(solicit.value == "") {
			solicit.remove(0);
		}

		
		<?if ( in_array(DEMANDA_PERFIL_TECNICO1, $perfil) || in_array(DEMANDA_PERFIL_TECNICO, $perfil) || in_array(DEMANDA_PERFIL_GESTOR_MEC, $perfil) ){?>
			var orig = document.getElementById("ordidarr");
			selectAllOptions(formulario.ordidarr);
			if(orig.value == "") {
				orig.remove(0);
			}
		<?}?>
		<?if ( in_array(DEMANDA_PERFIL_TECNICO1, $perfil) ){?>
			var tserv = document.getElementById("tiposervico");
			selectAllOptions(formulario.tiposervico);
			if(tserv.value == "") {
				tserv.remove(0);
			}
		<?}?>

		
		formulario.submit();
	}

	function limparDados() {


		d.formulario.codigo.value = '';
		d.formulario.unaid.value = '';
		d.formulario.esdid.value = '';
		<?if($mostra){?>
			<?if ( in_array(DEMANDA_PERFIL_TECNICO1, $perfil) || in_array(DEMANDA_PERFIL_TECNICO, $perfil) || in_array(DEMANDA_PERFIL_GESTOR_MEC, $perfil) ){?>
				var orig = document.getElementById("ordidarr");
				orig.remove(0);
			<?}else{?>
				d.formulario.ordid.value = '';
			<?}?>
			<?if ( in_array(DEMANDA_PERFIL_TECNICO1, $perfil) ){?>
				var tserv = document.getElementById("tiposervico");
				tserv.remove(0);
			<?}?>
			
			d.formulario.celid.value = '';
		<?}?>
		d.formulario.sidid.value = '';
		d.formulario.dataini.value = '';
		d.formulario.datafim.value = '';
		d.formulario.motid.value = '';
		d.formulario.dmddatadocrastreamento.value = '';
		d.formulario.dmdnumdocrastreamento.value = '';
		d.formulario.dmdunidadedocrastreamento.value = '';
		d.formulario.palavra_chave.value = '';
		d.formulario.solicitante.value = '';
		d.formulario.nserie.value = '';
		d.formulario.npatrimonio.value = '';
		d.formulario.anexo.value = '';
		d.formulario.somente_avaliadas.checked = false;
		d.formulario.somente_anosanteriores.checked = false;
		d.formulario.somente_pausadas.checked = false;
		d.formulario.nao_exib_finaliz_cancela_invalid.checked = false;
				
		limpaConsultaRapida();

		d.formulario.submetido.value = '2';
		
		formulario.submit();
		
	}	

	
	function montaSubLista(dmdid)
	{
		td 	   = document.getElementById('td_'+dmdid);
		img_mais   = document.getElementById('img_mais_'+dmdid);
		img_menos   = document.getElementById('img_menos_'+dmdid);
		
		// Faz uma requisição ajax, passando o parametro 'ordid', via POST
		var req = new Ajax.Request('demandas.php?modulo=principal/lista&acao=A', {
				        method:     'post',
				        parameters: '&dmdidAjax=' + dmdid,
				        onComplete: function (res)
				        {	
							td.style.display = '';
							//td.style.padding = '5px';
							img_mais.style.display = 'none';
							img_menos.style.display = '';
							td.innerHTML = res.responseText;
				        }
				  });
	
	}
	

	
	function desmontaSubLista(dmdid)
	{
		td 	   = document.getElementById('td_'+dmdid);
		img_mais   = document.getElementById('img_mais_'+dmdid);
		img_menos   = document.getElementById('img_menos_'+dmdid);
		
		td.innerHTML = '';
		td.style.display = 'none';
		img_mais.style.display = '';
		img_menos.style.display = 'none';
	
	}
	
	
	function abrirPopupDemandas()
	{
		
		window.open( 'demandas.php?modulo=relatorio/demandasDiarias&acao=I', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		
	}
	
/*
* Faz requisição via ajax
* Filtra o tipo de damanda, atravéz do parametro passado 'ordid'
*/
function filtraSistema(celid) {
	if(d.getElementById('celid').value == "") {
		d.getElementById('sidid').options[0].selected = true;
		return;
	}
		
	//tr 	   = d.getElementById('tr_sistema');	
	td     = d.getElementById('td_sistema');	
	select = td.getElementsByTagName('select')[0];
	//trSis  = d.getElementById('tr_sistema');
	//trCel  = d.getElementById('tr_celula');

	//trSis.style.display = paramSystem == 1 ? (navigator.appName == 'Netscape' ? 'table-row' : 'block') : 'none';	
	//trCel.style.display = paramSystem == 1 ? (navigator.appName == 'Netscape' ? 'table-row' : 'block') : 'none';
	
	// Desabilita o <select>, caso exista, até que o resultado da pesquisa seja carregado
	if (select)
		select.disabled = true;
		
	// Faz uma requisição ajax, passando o parametro 'ordid', via POST
	var req = new Ajax.Request('demandas.php?modulo=principal/lista&acao=A', {
							        method:     'post',
							        parameters: '&celidAjax=' + celid,
							        onComplete: function (res)
							        {			
										td.innerHTML = res.responseText;
										extraiScript(res.responseText);
										//tr.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
							        }
							  });
}

function mostraCelsis(ordid) {
	
	d.getElementById('td_celula').style.display = 'none';
	d.getElementById('td_sistema').style.display = 'none';
	
	if(ordid == 1 || ordid == 18 || ordid == 19 || ordid == 20 || ordid == 21){
		d.getElementById('td_celula').style.display = '';
		d.getElementById('td_sistema').style.display = '';
	}
	else if(ordid == 23 || ordid == 24){
		d.getElementById('td_celula').style.display = '';
		d.getElementById('td_sistema').style.display = 'none';
	}
	else{
		d.getElementById('td_celula').style.display = 'none';
		d.getElementById('td_sistema').style.display = 'none';
	}
	

}

function minhasDemandas(){
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
	document.formulario.submit();

}

function todasDemandas(){
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
	//d.getElementById('nao_exib_finaliz_cancela_invalid').value = 'on';
	document.formulario.submit();

}

function novasDemandas(){
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
	document.formulario.submit();

}

function demandasHoje(){
	d.getElementById('dataini').value = '<? print date('d/m/Y');?>';
	d.getElementById('datafim').value = '<? print date('d/m/Y');?>';
	document.formulario.submit();
}

function demandasEmAtraso(){
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
	document.formulario.submit();
}

function demandasVenceHoje(){
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
	document.formulario.submit();
}

function demandasEmAnalise(){
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
	document.formulario.submit();
}
function demandasEmAtendimento(){
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
	document.formulario.submit();
}
function demandasAgValidacao(){
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
	document.formulario.submit();
}
function demandasAgValidacaoDem(){
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
	document.formulario.submit();
}

/*
function demandasAgAvaliacao(){
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
	document.formulario.submit();
}
*/
function demandasFinalizada(){
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
	document.formulario.submit();
}
function demandasAuditada(){
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
	document.formulario.submit();
}
function demandasAgpago(){
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
	document.formulario.submit();
}
function demandasPago(){
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
	document.formulario.submit();
}
function demandasInvalidada(){
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
	document.formulario.submit();
}
function demandasCancelada(){
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
	document.formulario.submit();
}
function demandasEmProcessamento(){
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
	document.formulario.submit();
}
function demandasUrgente(){
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
	document.formulario.submit();
}
function demandasAlta(){
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
	document.formulario.submit();
}
function demandasNormal(){
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
	document.formulario.submit();
}
function demandasNdas(){
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
	document.formulario.submit();
}


function exibeConsultaRapida(){
	d.getElementById('consultas_rapidas').style.display = "";
	d.getElementById('menos_consulta').style.display = "";
	d.getElementById('mais_consulta').style.display = "none";
}

function mostraTotais(){
	numeroDemandas('todas');
	numeroDemandas('hoje');
	numeroDemandas('nova');
	numeroDemandas('ematraso');
	numeroDemandas('vencehoje');
	numeroDemandas('minhademanda');	
}

function exibeConsultaRapida1(){
	d.getElementById('consultas_rapidas1').style.display = "";
	d.getElementById('menos_consulta1').style.display = "";
	d.getElementById('mais_consulta1').style.display = "none";
}

function mostraTotais1(){
	numeroDemandas('emprocessamento');
	numeroDemandas('emanalise');
	numeroDemandas('ematendimento');
	numeroDemandas('agvalidacao');
	numeroDemandas('agvalidacaodem');
	numeroDemandas('finalizada');
	numeroDemandas('auditada');
	numeroDemandas('agpago');
	numeroDemandas('pago');
	numeroDemandas('invalidada');
	numeroDemandas('cancelada');
}

function exibeConsultaRapida2(){
	d.getElementById('consultas_rapidas2').style.display = "";
	d.getElementById('menos_consulta2').style.display = "";
	d.getElementById('mais_consulta2').style.display = "none";
}

function mostraTotais2(){
	numeroDemandas('urgente');
	numeroDemandas('alta');
	numeroDemandas('normal');
	numeroDemandas('ndas');	
}

function escondeConsultaRapida(){
	d.getElementById('consultas_rapidas').style.display = "none";
	d.getElementById('mais_consulta').style.display = "";
	d.getElementById('menos_consulta').style.display = "none";
	d.getElementById('todas').checked = false;
	d.getElementById('hoje').checked = false;
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = false;
	<?//}?>
	d.getElementById('nova').checked = false;
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
}

function escondeConsultaRapida1(){
	d.getElementById('consultas_rapidas1').style.display = "none";
	d.getElementById('mais_consulta1').style.display = "";
	d.getElementById('menos_consulta1').style.display = "none";
	//d.getElementById('todas').checked = false;
	//d.getElementById('hoje').checked = false;
	//d.getElementById('minhademanda').checked = false;
	//d.getElementById('nova').checked = false;
	//d.getElementById('dataini').value = '';
	//d.getElementById('datafim').value = '';
}

function escondeConsultaRapida2(){
	d.getElementById('consultas_rapidas2').style.display = "none";
	d.getElementById('mais_consulta2').style.display = "";
	d.getElementById('menos_consulta2').style.display = "none";
	//d.getElementById('todas').checked = false;
	//d.getElementById('hoje').checked = false;
	//d.getElementById('minhademanda').checked = false;
	//d.getElementById('nova').checked = false;
	//d.getElementById('dataini').value = '';
	//d.getElementById('datafim').value = '';
}


//traz as consultas rapidas fechadas
//escondeConsultaRapida();
//escondeConsultaRapida1();
//escondeConsultaRapida2();

function limpaCampos(){
	d.getElementById('esdid').value = "";
	d.getElementById('anexo').value = "";
	d.getElementById('unaid').value = "";
	d.getElementById('sidid').value = "";
	<?if($mostra){?>
		<?if ( in_array(DEMANDA_PERFIL_TECNICO1, $perfil) || in_array(DEMANDA_PERFIL_TECNICO, $perfil) || in_array(DEMANDA_PERFIL_GESTOR_MEC, $perfil) ){?>
			var orig = document.getElementById("ordidarr");
			orig.remove(0);
		<?}else{?>
			d.getElementById('ordid').value = "";
		<?}?>
		<?if ( in_array(DEMANDA_PERFIL_TECNICO1, $perfil) ){?>
			var tserv = document.getElementById("tiposervico");
			tserv.remove(0);
		<?}?>
	d.getElementById('celid').value = "";
	<?}?>
	d.getElementById('motid').value = "";
}


function limpaConsultaRapida(){
	d.getElementById('todas').checked = false;
	d.getElementById('hoje').checked = false;
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = false;
	<?//}?>
	d.getElementById('nova').checked = false;
	d.getElementById('ematraso').checked = false;
	d.getElementById('vencehoje').checked = false;
	d.getElementById('emanalise').checked = false;
	d.getElementById('ematendimento').checked = false;
	d.getElementById('agvalidacao').checked = false;
	d.getElementById('agvalidacaodem').checked = false;
	//d.getElementById('agavaliacao').checked = false;
	d.getElementById('finalizada').checked = false;
	d.getElementById('auditada').checked = false;
	d.getElementById('agpago').checked = false;
	d.getElementById('pago').checked = false;
	d.getElementById('invalidada').checked = false;
	d.getElementById('cancelada').checked = false;
	d.getElementById('emprocessamento').checked = false;
	d.getElementById('urgente').checked = false;
	d.getElementById('alta').checked = false;
	d.getElementById('normal').checked = false;
	d.getElementById('ndas').checked = false;
}

function selecionaTodas(){
	limpaCampos();
	d.getElementById('todas').checked = true;
	//d.getElementById('nao_exib_finaliz_cancela_invalid').checked = true;
	d.getElementById('hoje').checked = false;
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = false;
	<?//}?>
	d.getElementById('nova').checked = false;
	d.getElementById('ematraso').checked = false;
	d.getElementById('vencehoje').checked = false;
	d.getElementById('emanalise').checked = false;
	d.getElementById('ematendimento').checked = false;
	d.getElementById('agvalidacao').checked = false;
	d.getElementById('agvalidacaodem').checked = false;
	//d.getElementById('agavaliacao').checked = false;
	d.getElementById('finalizada').checked = false;
	d.getElementById('auditada').checked = false;
	d.getElementById('agpago').checked = false;
	d.getElementById('pago').checked = false;
	d.getElementById('invalidada').checked = false;
	d.getElementById('cancelada').checked = false;
	d.getElementById('emprocessamento').checked = false;
	d.getElementById('urgente').checked = false;
	d.getElementById('alta').checked = false;
	d.getElementById('normal').checked = false;
	d.getElementById('ndas').checked = false;
	
}

function selecionaHoje(){
	limpaCampos();
	d.getElementById('hoje').checked = true;
	d.getElementById('todas').checked = false;
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = false;
	<?//}?>
	d.getElementById('nova').checked = false;
	d.getElementById('ematraso').checked = false;
	d.getElementById('vencehoje').checked = false;
	d.getElementById('emanalise').checked = false;
	d.getElementById('ematendimento').checked = false;
	d.getElementById('agvalidacao').checked = false;
	d.getElementById('agvalidacaodem').checked = false;
	//d.getElementById('agavaliacao').checked = false;
	d.getElementById('finalizada').checked = false;
	d.getElementById('auditada').checked = false;
	d.getElementById('agpago').checked = false;
	d.getElementById('pago').checked = false;
	d.getElementById('invalidada').checked = false;
	d.getElementById('cancelada').checked = false;
	d.getElementById('emprocessamento').checked = false;
	d.getElementById('urgente').checked = false;
	d.getElementById('alta').checked = false;
	d.getElementById('normal').checked = false;
	d.getElementById('ndas').checked = false;
	
}

function selecionaNovas(){
	limpaCampos();
	d.getElementById('nova').checked = true;
	d.getElementById('todas').checked = false;
	d.getElementById('hoje').checked = false;
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = false;
	<?//}?>
	d.getElementById('ematraso').checked = false;
	d.getElementById('vencehoje').checked = false;
	d.getElementById('emanalise').checked = false;
	d.getElementById('ematendimento').checked = false;
	d.getElementById('agvalidacao').checked = false;
	d.getElementById('agvalidacaodem').checked = false;
	//d.getElementById('agavaliacao').checked = false;
	d.getElementById('finalizada').checked = false;
	d.getElementById('auditada').checked = false;
	d.getElementById('agpago').checked = false;
	d.getElementById('pago').checked = false;
	d.getElementById('invalidada').checked = false;
	d.getElementById('cancelada').checked = false;
	d.getElementById('urgente').checked = false;
	d.getElementById('alta').checked = false;
	d.getElementById('normal').checked = false;
	
	
}

function selecionaMinhasDemandas(){
	limpaCampos();
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = true;
	<?//}?>
	d.getElementById('todas').checked = false;
	d.getElementById('hoje').checked = false;
	d.getElementById('nova').checked = false;
	d.getElementById('ematraso').checked = false;
	d.getElementById('vencehoje').checked = false;
	d.getElementById('emanalise').checked = false;
	d.getElementById('ematendimento').checked = false;
	d.getElementById('agvalidacao').checked = false;
	d.getElementById('agvalidacaodem').checked = false;
	//d.getElementById('agavaliacao').checked = false;
	d.getElementById('finalizada').checked = false;
	d.getElementById('auditada').checked = false;
	d.getElementById('agpago').checked = false;
	d.getElementById('pago').checked = false;
	d.getElementById('invalidada').checked = false;
	d.getElementById('cancelada').checked = false;
	d.getElementById('emprocessamento').checked = false;
	d.getElementById('urgente').checked = false;
	d.getElementById('alta').checked = false;
	d.getElementById('normal').checked = false;
	d.getElementById('ndas').checked = false;
	
}

function selecionaEmAtraso(){
	limpaCampos();
	d.getElementById('ematraso').checked = true;
	d.getElementById('todas').checked = false;
	d.getElementById('hoje').checked = false;
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = false;
	<?//}?>
	d.getElementById('nova').checked = false;
	d.getElementById('vencehoje').checked = false;
	d.getElementById('emanalise').checked = false;
	d.getElementById('ematendimento').checked = false;
	d.getElementById('agvalidacao').checked = false;
	d.getElementById('agvalidacaodem').checked = false;
	//d.getElementById('agavaliacao').checked = false;
	d.getElementById('finalizada').checked = false;
	d.getElementById('auditada').checked = false;
	d.getElementById('agpago').checked = false;
	d.getElementById('pago').checked = false;
	d.getElementById('invalidada').checked = false;
	d.getElementById('cancelada').checked = false;
	d.getElementById('emprocessamento').checked = false;
	d.getElementById('urgente').checked = false;
	d.getElementById('alta').checked = false;
	d.getElementById('normal').checked = false;
	d.getElementById('ndas').checked = false;

}

function selecionaVenceHoje(){
	limpaCampos();
	d.getElementById('vencehoje').checked = true;
	d.getElementById('todas').checked = false;
	d.getElementById('hoje').checked = false;
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = false;
	<?//}?>
	d.getElementById('nova').checked = false;
	d.getElementById('ematraso').checked = false;
	d.getElementById('emanalise').checked = false;
	d.getElementById('ematendimento').checked = false;
	d.getElementById('agvalidacao').checked = false;
	d.getElementById('agvalidacaodem').checked = false;
	//d.getElementById('agavaliacao').checked = false;
	d.getElementById('finalizada').checked = false;
	d.getElementById('auditada').checked = false;
	d.getElementById('agpago').checked = false;
	d.getElementById('pago').checked = false;
	d.getElementById('invalidada').checked = false;
	d.getElementById('cancelada').checked = false;
	d.getElementById('emprocessamento').checked = false;
	d.getElementById('urgente').checked = false;
	d.getElementById('alta').checked = false;
	d.getElementById('normal').checked = false;
	d.getElementById('ndas').checked = false;
}

function selecionaEmAnalise(){
	limpaCampos();
	d.getElementById('emanalise').checked = true;
	d.getElementById('vencehoje').checked = false;
	d.getElementById('todas').checked = false;
	d.getElementById('hoje').checked = false;
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = false;
	<?//}?>
	d.getElementById('nova').checked = false;
	d.getElementById('ematraso').checked = false;
	d.getElementById('ematendimento').checked = false;
	d.getElementById('agvalidacao').checked = false;
	d.getElementById('agvalidacaodem').checked = false;
	//d.getElementById('agavaliacao').checked = false;
	d.getElementById('finalizada').checked = false;
	d.getElementById('auditada').checked = false;
	d.getElementById('agpago').checked = false;
	d.getElementById('pago').checked = false;
	d.getElementById('invalidada').checked = false;
	d.getElementById('cancelada').checked = false;
	d.getElementById('emprocessamento').checked = false;
	d.getElementById('urgente').checked = false;
	d.getElementById('alta').checked = false;
	d.getElementById('normal').checked = false;
	d.getElementById('ndas').checked = false;
}
function selecionaEmAtendimento(){
	limpaCampos();
	d.getElementById('ematendimento').checked = true;
	d.getElementById('vencehoje').checked = false;
	d.getElementById('todas').checked = false;
	d.getElementById('hoje').checked = false;
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = false;
	<?//}?>
	d.getElementById('nova').checked = false;
	d.getElementById('ematraso').checked = false;
	d.getElementById('emanalise').checked = false;
	d.getElementById('agvalidacao').checked = false;
	d.getElementById('agvalidacaodem').checked = false;
	//d.getElementById('agavaliacao').checked = false;
	d.getElementById('finalizada').checked = false;
	d.getElementById('auditada').checked = false;
	d.getElementById('agpago').checked = false;
	d.getElementById('pago').checked = false;
	d.getElementById('invalidada').checked = false;
	d.getElementById('cancelada').checked = false;
	d.getElementById('emprocessamento').checked = false;
	d.getElementById('urgente').checked = false;
	d.getElementById('alta').checked = false;
	d.getElementById('normal').checked = false;
	d.getElementById('ndas').checked = false;
}
function selecionaAgValidacao(){
	limpaCampos();
	d.getElementById('agvalidacao').checked = true;
	d.getElementById('agvalidacaodem').checked = false;
	d.getElementById('vencehoje').checked = false;
	d.getElementById('todas').checked = false;
	d.getElementById('hoje').checked = false;
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = false;
	<?//}?>
	d.getElementById('nova').checked = false;
	d.getElementById('ematraso').checked = false;
	d.getElementById('emanalise').checked = false;
	d.getElementById('ematendimento').checked = false;
	//d.getElementById('agavaliacao').checked = false;
	d.getElementById('finalizada').checked = false;
	d.getElementById('auditada').checked = false;
	d.getElementById('agpago').checked = false;
	d.getElementById('pago').checked = false;
	d.getElementById('invalidada').checked = false;
	d.getElementById('cancelada').checked = false;
	d.getElementById('emprocessamento').checked = false;
	d.getElementById('urgente').checked = false;
	d.getElementById('alta').checked = false;
	d.getElementById('normal').checked = false;
	d.getElementById('ndas').checked = false;
}
function selecionaAgValidacaoDem(){
	limpaCampos();
	d.getElementById('agvalidacaodem').checked = true;
	d.getElementById('agvalidacao').checked = false;
	d.getElementById('vencehoje').checked = false;
	d.getElementById('todas').checked = false;
	d.getElementById('hoje').checked = false;
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = false;
	<?//}?>
	d.getElementById('nova').checked = false;
	d.getElementById('ematraso').checked = false;
	d.getElementById('emanalise').checked = false;
	d.getElementById('ematendimento').checked = false;
	//d.getElementById('agavaliacao').checked = false;
	d.getElementById('finalizada').checked = false;
	d.getElementById('auditada').checked = false;
	d.getElementById('agpago').checked = false;
	d.getElementById('pago').checked = false;
	d.getElementById('invalidada').checked = false;
	d.getElementById('cancelada').checked = false;
	d.getElementById('emprocessamento').checked = false;
	d.getElementById('urgente').checked = false;
	d.getElementById('alta').checked = false;
	d.getElementById('normal').checked = false;
	d.getElementById('ndas').checked = false;
}

/*
function selecionaAgAvaliacao(){
	limpaCampos();
	d.getElementById('agavaliacao').checked = true;
	d.getElementById('vencehoje').checked = false;
	d.getElementById('todas').checked = false;
	d.getElementById('hoje').checked = false;
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = false;
	<?//}?>
	d.getElementById('nova').checked = false;
	d.getElementById('ematraso').checked = false;
	d.getElementById('emanalise').checked = false;
	d.getElementById('ematendimento').checked = false;
	d.getElementById('agvalidacao').checked = false;
	d.getElementById('agvalidacaodem').checked = false;
	d.getElementById('finalizada').checked = false;
	d.getElementById('invalidada').checked = false;
	d.getElementById('cancelada').checked = false;
	d.getElementById('emprocessamento').checked = false;
	d.getElementById('urgente').checked = false;
	d.getElementById('alta').checked = false;
	d.getElementById('normal').checked = false;
	d.getElementById('ndas').checked = false;
}
*/
function selecionaFinalizada(){
	limpaCampos();
	d.getElementById('finalizada').checked = true;
	d.getElementById('invalidada').checked = false;
	d.getElementById('vencehoje').checked = false;
	d.getElementById('todas').checked = false;
	d.getElementById('hoje').checked = false;
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = false;
	<?//}?>
	d.getElementById('nova').checked = false;
	d.getElementById('ematraso').checked = false;
	d.getElementById('emanalise').checked = false;
	d.getElementById('ematendimento').checked = false;
	d.getElementById('agvalidacao').checked = false;
	d.getElementById('agvalidacaodem').checked = false;
	//d.getElementById('agavaliacao').checked = false;
	d.getElementById('cancelada').checked = false;
	d.getElementById('emprocessamento').checked = false;
	d.getElementById('urgente').checked = false;
	d.getElementById('alta').checked = false;
	d.getElementById('normal').checked = false;
	d.getElementById('ndas').checked = false;
}
function selecionaAuditada(){
	limpaCampos();
	d.getElementById('auditada').checked = true;
	d.getElementById('agpago').checked = false;
	d.getElementById('pago').checked = false;
	d.getElementById('finalizada').checked = false;
	d.getElementById('invalidada').checked = false;
	d.getElementById('vencehoje').checked = false;
	d.getElementById('todas').checked = false;
	d.getElementById('hoje').checked = false;
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = false;
	<?//}?>
	d.getElementById('nova').checked = false;
	d.getElementById('ematraso').checked = false;
	d.getElementById('emanalise').checked = false;
	d.getElementById('ematendimento').checked = false;
	d.getElementById('agvalidacao').checked = false;
	d.getElementById('agvalidacaodem').checked = false;
	//d.getElementById('agavaliacao').checked = false;
	d.getElementById('cancelada').checked = false;
	d.getElementById('emprocessamento').checked = false;
	d.getElementById('urgente').checked = false;
	d.getElementById('alta').checked = false;
	d.getElementById('normal').checked = false;
	d.getElementById('ndas').checked = false;
}
function selecionaAgpago(){
	limpaCampos();
	d.getElementById('agpago').checked = true;
	d.getElementById('auditada').checked = false;
	d.getElementById('pago').checked = false;
	d.getElementById('finalizada').checked = false;
	d.getElementById('invalidada').checked = false;
	d.getElementById('vencehoje').checked = false;
	d.getElementById('todas').checked = false;
	d.getElementById('hoje').checked = false;
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = false;
	<?//}?>
	d.getElementById('nova').checked = false;
	d.getElementById('ematraso').checked = false;
	d.getElementById('emanalise').checked = false;
	d.getElementById('ematendimento').checked = false;
	d.getElementById('agvalidacao').checked = false;
	d.getElementById('agvalidacaodem').checked = false;
	//d.getElementById('agavaliacao').checked = false;
	d.getElementById('cancelada').checked = false;
	d.getElementById('emprocessamento').checked = false;
	d.getElementById('urgente').checked = false;
	d.getElementById('alta').checked = false;
	d.getElementById('normal').checked = false;
	d.getElementById('ndas').checked = false;
}
function selecionaPago(){
	limpaCampos();
	d.getElementById('pago').checked = true;
	d.getElementById('agpago').checked = false;
	d.getElementById('auditada').checked = false;
	d.getElementById('finalizada').checked = false;
	d.getElementById('invalidada').checked = false;
	d.getElementById('vencehoje').checked = false;
	d.getElementById('todas').checked = false;
	d.getElementById('hoje').checked = false;
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = false;
	<?//}?>
	d.getElementById('nova').checked = false;
	d.getElementById('ematraso').checked = false;
	d.getElementById('emanalise').checked = false;
	d.getElementById('ematendimento').checked = false;
	d.getElementById('agvalidacao').checked = false;
	d.getElementById('agvalidacaodem').checked = false;
	//d.getElementById('agavaliacao').checked = false;
	d.getElementById('cancelada').checked = false;
	d.getElementById('emprocessamento').checked = false;
	d.getElementById('urgente').checked = false;
	d.getElementById('alta').checked = false;
	d.getElementById('normal').checked = false;
	d.getElementById('ndas').checked = false;
}
function selecionaInvalidada(){
	limpaCampos();
	d.getElementById('invalidada').checked = true;
	d.getElementById('finalizada').checked = false;
	d.getElementById('auditada').checked = false;
	d.getElementById('agpago').checked = false;
	d.getElementById('pago').checked = false;
	d.getElementById('vencehoje').checked = false;
	d.getElementById('todas').checked = false;
	d.getElementById('hoje').checked = false;
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = false;
	<?//}?>
	d.getElementById('nova').checked = false;
	d.getElementById('ematraso').checked = false;
	d.getElementById('emanalise').checked = false;
	d.getElementById('ematendimento').checked = false;
	d.getElementById('agvalidacao').checked = false;
	d.getElementById('agvalidacaodem').checked = false;
	//d.getElementById('agavaliacao').checked = false;
	d.getElementById('cancelada').checked = false;
	d.getElementById('emprocessamento').checked = false;
	d.getElementById('urgente').checked = false;
	d.getElementById('alta').checked = false;
	d.getElementById('normal').checked = false;
	d.getElementById('ndas').checked = false;
}
function selecionaCancelada(){
	limpaCampos();
	d.getElementById('cancelada').checked = true;
	d.getElementById('vencehoje').checked = false;
	d.getElementById('todas').checked = false;
	d.getElementById('hoje').checked = false;
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = false;
	<?//}?>
	d.getElementById('nova').checked = false;
	d.getElementById('ematraso').checked = false;
	d.getElementById('emanalise').checked = false;
	d.getElementById('ematendimento').checked = false;
	d.getElementById('agvalidacao').checked = false;
	d.getElementById('agvalidacaodem').checked = false;
	//d.getElementById('agavaliacao').checked = false;
	d.getElementById('finalizada').checked = false;
	d.getElementById('auditada').checked = false;
	d.getElementById('agpago').checked = false;
	d.getElementById('pago').checked = false;
	d.getElementById('invalidada').checked = false;
	d.getElementById('emprocessamento').checked = false;
	d.getElementById('urgente').checked = false;
	d.getElementById('alta').checked = false;
	d.getElementById('normal').checked = false;
	d.getElementById('ndas').checked = false;
}
function selecionaEmProcessamento(){
	limpaCampos();
	d.getElementById('emprocessamento').checked = true;
	d.getElementById('vencehoje').checked = false;
	d.getElementById('todas').checked = false;
	d.getElementById('hoje').checked = false;
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = false;
	<?//}?>
	d.getElementById('nova').checked = false;
	d.getElementById('ematraso').checked = false;
	d.getElementById('emanalise').checked = false;
	d.getElementById('ematendimento').checked = false;
	d.getElementById('agvalidacao').checked = false;
	d.getElementById('agvalidacaodem').checked = false;
	//d.getElementById('agavaliacao').checked = false;
	d.getElementById('finalizada').checked = false;
	d.getElementById('auditada').checked = false;
	d.getElementById('agpago').checked = false;
	d.getElementById('pago').checked = false;
	d.getElementById('invalidada').checked = false;
	d.getElementById('cancelada').checked = false;
	d.getElementById('urgente').checked = false;
	d.getElementById('alta').checked = false;
	d.getElementById('normal').checked = false;
	d.getElementById('ndas').checked = false;
}
function selecionaUrgente(){
	limpaCampos();
	d.getElementById('urgente').checked = true;
	d.getElementById('vencehoje').checked = false;
	d.getElementById('todas').checked = false;
	d.getElementById('hoje').checked = false;
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = false;
	<?//}?>
	d.getElementById('nova').checked = false;
	d.getElementById('ematraso').checked = false;
	d.getElementById('emanalise').checked = false;
	d.getElementById('ematendimento').checked = false;
	d.getElementById('agvalidacao').checked = false;
	d.getElementById('agvalidacaodem').checked = false;
	//d.getElementById('agavaliacao').checked = false;
	d.getElementById('finalizada').checked = false;
	d.getElementById('auditada').checked = false;
	d.getElementById('agpago').checked = false;
	d.getElementById('pago').checked = false;
	d.getElementById('invalidada').checked = false;
	d.getElementById('cancelada').checked = false;
	d.getElementById('emprocessamento').checked = false;
	d.getElementById('alta').checked = false;
	d.getElementById('normal').checked = false;
	d.getElementById('ndas').checked = false;
}
function selecionaAlta(){
	limpaCampos();
	d.getElementById('alta').checked = true;
	d.getElementById('vencehoje').checked = false;
	d.getElementById('todas').checked = false;
	d.getElementById('hoje').checked = false;
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = false;
	<?//}?>
	d.getElementById('nova').checked = false;
	d.getElementById('ematraso').checked = false;
	d.getElementById('emanalise').checked = false;
	d.getElementById('ematendimento').checked = false;
	d.getElementById('agvalidacao').checked = false;
	d.getElementById('agvalidacaodem').checked = false;
	//d.getElementById('agavaliacao').checked = false;
	d.getElementById('finalizada').checked = false;
	d.getElementById('auditada').checked = false;
	d.getElementById('agpago').checked = false;
	d.getElementById('pago').checked = false;
	d.getElementById('invalidada').checked = false;
	d.getElementById('cancelada').checked = false;
	d.getElementById('emprocessamento').checked = false;
	d.getElementById('urgente').checked = false;
	d.getElementById('normal').checked = false;
	d.getElementById('ndas').checked = false;
}
function selecionaNormal(){
	limpaCampos();
	d.getElementById('normal').checked = true;
	d.getElementById('vencehoje').checked = false;
	d.getElementById('todas').checked = false;
	d.getElementById('hoje').checked = false;
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = false;
	<?//}?>
	d.getElementById('nova').checked = false;
	d.getElementById('ematraso').checked = false;
	d.getElementById('emanalise').checked = false;
	d.getElementById('ematendimento').checked = false;
	d.getElementById('agvalidacao').checked = false;
	d.getElementById('agvalidacaodem').checked = false;
	//d.getElementById('agavaliacao').checked = false;
	d.getElementById('finalizada').checked = false;
	d.getElementById('auditada').checked = false;
	d.getElementById('agpago').checked = false;
	d.getElementById('pago').checked = false;
	d.getElementById('invalidada').checked = false;
	d.getElementById('cancelada').checked = false;
	d.getElementById('emprocessamento').checked = false;
	d.getElementById('urgente').checked = false;
	d.getElementById('alta').checked = false;
	d.getElementById('ndas').checked = false;
}
function selecionaNdas(){
	limpaCampos();
	d.getElementById('ndas').checked = true;
	d.getElementById('vencehoje').checked = false;
	d.getElementById('todas').checked = false;
	d.getElementById('hoje').checked = false;
	<?//if(!in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){?>
	d.getElementById('minhademanda').checked = false;
	<?//}?>
	d.getElementById('nova').checked = false;
	d.getElementById('ematraso').checked = false;
	d.getElementById('emanalise').checked = false;
	d.getElementById('ematendimento').checked = false;
	d.getElementById('agvalidacao').checked = false;
	d.getElementById('agvalidacaodem').checked = false;
	//d.getElementById('agavaliacao').checked = false;
	d.getElementById('finalizada').checked = false;
	d.getElementById('auditada').checked = false;
	d.getElementById('agpago').checked = false;
	d.getElementById('pago').checked = false;
	d.getElementById('invalidada').checked = false;
	d.getElementById('cancelada').checked = false;
	d.getElementById('emprocessamento').checked = false;
	d.getElementById('urgente').checked = false;
	d.getElementById('alta').checked = false;
	d.getElementById('normal').checked = false;
}

function dadosMotivador(id){
	var motid = id;
	if(motid){
		d.getElementById('data_motivador').style.display = "";
		
		if(motid == 1 || motid == 2) {
			d.getElementById('num_doc').style.display = "";
			d.getElementById('tr_orgao').style.display = "";
		}
		if(motid != 1 && motid != 2){
			d.getElementById('dmdnumdocrastreamento').value = "";
			d.getElementById('num_doc').style.display = "none";
			
			d.getElementById('dmdunidadedocrastreamento').value = "";
			d.getElementById('tr_orgao').style.display = "none";
		}
	}
	if(!motid){
		d.getElementById('dmdnumdocrastreamento').value = "";
		d.getElementById('num_doc').style.display = "none";
		
		d.getElementById('dmddatadocrastreamento').value = "";
		d.getElementById('data_motivador').style.display = "none";
		
		d.getElementById('dmdunidadedocrastreamento').value = "";
		d.getElementById('tr_orgao').style.display = "none";
	}
}

function forcaWidth(){
	//d.getElementById('sidid').style.width = "350px";
}


</script>