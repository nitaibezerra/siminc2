<?php

function alteraDemanda($dmdid){
	global $db;
	
	$sql = "SELECT
		 to_char(d.dmddatainclusao, 'DD/MM/YYYY') AS dmddatainclusao,
		 dmddatainiprevatendimento,
		 dmddatafimprevatendimento,		 
		 to_char(d.dmddatainiprevatendimento, 'HH24:MI') AS hiniatendimento,
		 to_char(d.dmddatafimprevatendimento, 'HH24:MI') AS hfimatendimento	 
		FROM
		 demandas.demanda d
		WHERE
		 dmdid =".$dmdid;
	
			$dados = $db->carregar($sql);
	
	echo "<div id=\"aletaDemanada_$dmdid\"  style=\"position:absolute;width:350px;border:#000000 solid 2px;padding: 5px;background-color:#fcfcfc;\";>
	";?>
	<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data" >
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr bgcolor="#F2F2F2">
		<td class="subtitulodireita" width="25%" >Previsão de início do atendimento:</td>
		<td>
		<? $dmddatafimprevatendimento = $dados[0]['dmddatafimprevatendimento'] ?>
		<?= campo_data( 'dmddatainiprevatendimento', 'S', 'S', '', 'S' ); ?>&nbsp;
		<? $hiniatendimento = $dados[0]['hiniatendimento'] ?>
		Hora:<?= campo_texto('hiniatendimento','S','S','',5,5,'##:##','','left','',0, 'id="hiniatendimento"') ?>
		</td>
		<td style="background-color:#fafafa; color:#404040; width: 1px;" valign="top" align="left">
		</td>		
	</tr>
	<tr bgcolor="#F2F2F2">
		<td class="subtitulodireita">Previsão de término do atendimento:</td>
		<td>
		<? $dmddatafimprevatendimento = $dados[0]['dmddatafimprevatendimento'] ?>
		<?= campo_data( 'dmddatafimprevatendimento', 'S', 'S', '', 'S' ); ?>&nbsp;
		<? $hfimatendimento = $dados[0]['hfimatendimento'] ?>
		Hora:<?= campo_texto('hfimatendimento','S','S','',5,5,'##:##','','left','',0,'id="hfimatendimento"') ?>		
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td>&nbsp;</td>
		<td>
	    	<input type="button" value="Salvar" onclick="fechar('aletaDemanada_<?=$dmdid?>');" />  	
		</td>			
	</tr>	
	</table>	
	</div><?
	
}

function exibeDemandasMes($usucpf,$data){
	global $db;
	if($usucpf && $data){
		$xData	= explode("/", $data);
		$xData2 = $xData[2]."-".$xData[1]."-".$xData[0];		
		$sql = "SELECT d.dmdid as cod, d.dmdtitulo as assunto, ed.esddsc as situacao,
					to_char(d.dmddatainiprevatendimento, 'DD-MM-YYYY HH24:MI:SS') AS dtinic,
		            to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') AS dtfim,
		            p.pridsc
		        FROM
					demandas.demanda as d
				LEFT JOIN demandas.prioridade p ON p.priid = d.priid
				LEFT JOIN workflow.documento doc ON doc.docid = d.docid
				LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
				WHERE d.usucpfexecutor = '$usucpf'
				--AND '".$xData2."' BETWEEN  d.dmddatainiprevatendimento AND d.dmddatafimprevatendimento
				--AND d.dmddatainiprevatendimento >= '$xData2 00:00:00' and d.dmddatafimprevatendimento  <= '$xData2 23:59:59'
				AND '$xData2' BETWEEN  to_char(d.dmddatainiprevatendimento::date, 'YYYY-MM-DD') AND to_char(d.dmddatafimprevatendimento::date, 'YYYY-MM-DD')
				";
		//dbg($sql,1);
		$dados = array();
		$dados = $db->carregar($sql);
		echo ("<div><center><b>".count($dados)." demanda(s)</b></center>");
		foreach($dados as $dmd){
			echo ("<b>Cód: </b> {$dmd['cod']} - <b>Situação: </b> {$dmd['situacao']} - <b>Descrição:</b> {$dmd['assunto']} <br />");
		}
		echo "</div>";
	}else{
		echo "Demanda não encontrada";
	}
}


function exibeInformacao($dmdid){
	global $db;
	$sql = "SELECT d.dmdid as cod, d.dmdtitulo as assunto, ed.esddsc as situacao,
				to_char(d.dmddatainiprevatendimento, 'DD-MM-YYYY HH24:MI:SS') AS dtinic,
	            to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') AS dtfim,
	            p.pridsc
	        FROM
				demandas.demanda as d
			LEFT JOIN demandas.prioridade p ON p.priid = d.priid
			LEFT JOIN workflow.documento doc ON doc.docid = d.docid
			LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
			WHERE d.dmdid = $dmdid";
	$dados = $db->carregar($sql);
	
	echo ("<div><center><b>Demanda</b></center><br />
	<b>Cód:</b> {$dados[0]['cod']} <br />
	<b>Descrição:</b> {$dados[0]['assunto']} <br />
	<b>Início:</b> {$dados[0]['dtinic']}<br />
	<b>Término:</b> {$dados[0]['dtfim']}<br />
	<b>Prioridade:</b> {$dados[0]['pridsc']}<br />
	<b>Situação:</b> {$dados[0]['situacao']}<br />
	</div>
	");
}

function exibeDemandaAjax($usucpf){

	global $db;
	
	
	// Inclui componente de relatórios
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	
	$classdata = new Data;	
	
				//verifica se e´ suporte de atendimento ou sistema de informações
				/*
				$sql = "SELECT sidid FROM demandas.demanda WHERE dmdid=".$_SESSION['dmdid'];
				$sididaux = $db->pegaUm($sql);
				*/
				if($sididaux) $ids_estado_documento = "91,92,93,107,108,111";
				else $ids_estado_documento = "91,92,107,108";
	
				$sql = "SELECT
							d.dmdid as cod,
	                    	(CASE WHEN contapausa > 0 THEN
									'<a href=\'javascript:void(0);\'><img src=\'../imagens/pause.gif\' border=0  title=\' \' align=\'absmiddle\'  onmouseout=\'SuperTitleOff(this);\' onmousemove=\"SuperTitleAjax(\'demandas.php?modulo=principal/lista&acao=A&dmdidPausaAjax='|| d.dmdid ||'\',this);\"></a>'
								ELSE
									''	
							END)
							||
							d.dmdtitulo as assunto,
	                    	loc.lcadescricao||' '||a.anddescricao||' '||d.dmdsalaatendimento  as local,			                    
	                    	to_char(d.dmddatainiprevatendimento, 'DD-MM-YYYY HH24:MI:SS') AS dtinic,
	                    	to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') AS dtfim,
	                    	ed.esddsc as situacao,
	                    	'' as observacao 
						FROM
							demandas.demanda as d
						INNER JOIN 
							workflow.documento doc ON doc.docid = d.docid
						INNER JOIN 
							workflow.estadodocumento ed ON ed.esdid = doc.esdid							
						INNER JOIN 
							demandas.localandaratendimento laa ON laa.laaid = d.laaid
						INNER JOIN 
							demandas.andaratendimento a ON a.andid  = laa.andid
						INNER JOIN 
							demandas.localatendimento loc ON loc.lcaid  = laa.lcaid
						INNER JOIN 
							seguranca.usuario u ON u.usucpf  = d.usucpfexecutor
						LEFT JOIN 
							(  select dmdid, count(pdmid) as contapausa from demandas.pausademanda group by dmdid ) ps ON ps.dmdid = d.dmdid
						
						WHERE 
							d.usucpfexecutor = '".$usucpf."'
							AND d.dmdstatus = 'A'
							AND ed.esdstatus = 'A' 
							AND doc.esdid in ({$ids_estado_documento})
							--AND d.dmdid not in( select dmdid from demandas.pausademanda ) 
							";
				$cabecalho = array("Cód","Assuto","Local","Data Início","Data Fim","Situação","Observações");
				//$cabecalho = array("Cód","Assuto","Local","Data Início","Data Fim","Situação","Pausa","Observações");
				//$db->monta_lista( $sql, $cabecalho, 50, 40, 'N', '', '');
				
				
				$sql = $db->carregar($sql);
				
				//dbg($sql);
				
				if($sql){
					
					for($i = 0; $i<count($sql); $i++ ){
						/*
						//verifica pausa da demanda
						$sqlp = "select count(pdmid) as qtdpausa
								from demandas.pausademanda 
								where dmdid = ". (int) $sql[$i]['cod'];
								
						$dadosp = $db->pegaum($sqlp);	
						
						//dbg($dadosp);
						if($dadosp>0){
							//unset($sql[$i]);
						}
						//fim pausa
						*/
						
						
						//verifica observação da demanda
						$sqlob = "select u.usunome, obsdsc, to_char(obsdata::timestamp,'DD/MM/YYYY HH24:MI') AS obsdata 
								  from demandas.observacoes o
								  inner join seguranca.usuario u on u.usucpf = o.usucpf 
								  where obslog IS NULL and obsstatus='A' and dmdid = ".$sql[$i]['cod'];
						$observacao = "";
						$ob = $db->carregar( $sqlob );
						if($ob){
							$observacao = '<div align=left>';
							foreach($ob as $obx){
								$observacao .= $obx['obsdsc'] .'<br><b>Incluído por: '. $obx['usunome'] . '<br> Data: '. $obx['obsdata'] .'</b><br><br>';	
							}
							$observacao .= '</div>';
						}	
						
						
						//passa os valores das observações para o array sql
						$sql[$i]['observacao'] = $observacao;
						
						//echo "<br>".$sql[$i]['cod']." = ".$textotempopausa;
					}
				}
				
				//dbg($sql,1);
				
				$db->monta_lista_array( $sql, $cabecalho, 200, 10, 'N', 'center', '' );	
}


if ($_POST['usucpfAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	exibeDemandaAjax($_POST['usucpfAjax']);
	exit;
}

if ($_POST['tipoSemana'] && $_POST['data'] && $_POST['avancar'] == 1) {
	header('content-type: text/html; charset=ISO-8859-1');
	$dmdid  = $_SESSION['dmdid'];
	$usucpf = $_GET['usucpf'];
	if(strlen(trim($_POST['data'])) == 10){
		$anoD = substr(trim($_POST['data']), -4,4);
		$mesD = substr(trim($_POST['data']), -7,2);
		$diaD = substr(trim($_POST['data']), -10,2);
		
	if($_POST['tipoSemana'] == "1"){
			$data =  date('d-m-Y',mktime(0,0,0,$mesD,$diaD + 1 ,$anoD));
		}
		if($_POST['tipoSemana'] == "mes"){
			$data =  date('d-m-Y',mktime(0,0,0,$mesD +1,$diaD,$anoD));
		}
		if($_POST['tipoSemana'] != 1 && $_POST['tipoSemana'] != "mes"){
			$data =  date('d-m-Y',mktime(0,0,0,$mesD,$diaD + 7 ,$anoD));
		}
		$ano = substr(trim($data), -4,4);
		$mes = substr(trim($data), -7,2);
		$dia = substr(trim($data), -10,2);
		
	}
	
$sql = "SELECT
				d.sidid, t.ordid, t.celid
	 		FROM
	 			demandas.demanda d
			LEFT JOIN 
				demandas.tiposervico t ON t.tipid = d.tipid	 			
	 		WHERE 
	 			d.dmdid=".$dmdid;
	
	$dadosDemanda = $db->carregar($sql);
	
	
	if ($dadosDemanda[0]['ordid'] == 1 || $dadosDemanda[0]['ordid'] == 18 || $dadosDemanda[0]['ordid'] == 19 || $dadosDemanda[0]['ordid'] == 20 || $dadosDemanda[0]['ordid'] == 21){ //sistemas de informação
		$sql = "SELECT DISTINCT
					u.usucpf,
					u.usunome
				FROM
					seguranca.usuario AS u
				INNER JOIN 
					demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf				 
				WHERE 
					ur.rpustatus = 'A' AND 
					ur.celid in (select celid from demandas.sistemacelula where sidid = " . $dadosDemanda[0]['sidid'] . ")
				ORDER BY u.usunome	
				";		
	}
	elseif($dadosDemanda[0]['ordid'] == 2 || $dadosDemanda[0]['ordid'] == 23 || $dadosDemanda[0]['ordid'] == 24){ //2=redes e 23=generica
		$sql = "SELECT DISTINCT
					u.usucpf,
					u.usunome
				FROM
					seguranca.usuario AS u
				INNER JOIN 
					demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf				 
				WHERE 
					ur.rpustatus = 'A' AND 
					ur.celid = ".$dadosDemanda[0]['celid']." 
				ORDER BY u.usunome	
				";		
	}
	else{ 
		$sql = "SELECT DISTINCT
					u.usucpf,
					u.usunome
				FROM
					seguranca.usuario  AS u
				INNER JOIN 
					demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf
				WHERE 
					ur.rpustatus = 'A' AND 
					ur.sidid IS NULL AND
					ur.ordid = ".$dadosDemanda[0]['ordid']." 
				ORDER BY u.usunome
				";
	}

	graphWeek($sql,$horario = $_POST['horario'], $tipoSemana = $_POST['tipoSemana'],$dia,$mes,$ano);
	exit;
}

if ($_POST['tipoSemana'] && $_POST['data'] && $_POST['recuar'] == 1) {
	header('content-type: text/html; charset=ISO-8859-1');
	
	$dmdid  = $_SESSION['dmdid'];
	$usucpf = $_GET['usucpf'];
	if(strlen(trim($_POST['data'])) == 10){
		$anoD = substr(trim($_POST['data']), -4,4);
		$mesD = substr(trim($_POST['data']), -7,2);
		$diaD = substr(trim($_POST['data']), -10,2);
		
		if($_POST['tipoSemana'] == "1"){
			$data =  date('d-m-Y',mktime(0,0,0,$mesD,$diaD - 1 ,$anoD));
		}
		if($_POST['tipoSemana'] == "mes"){
			$data =  date('d-m-Y',mktime(0,0,0,$mesD -1,$diaD,$anoD));
		}
		if($_POST['tipoSemana'] != 1 && $_POST['tipoSemana'] != "mes"){
			$data =  date('d-m-Y',mktime(0,0,0,$mesD,$diaD - 7 ,$anoD));
		}
		$ano = substr(trim($data), -4,4);
		$mes = substr(trim($data), -7,2);
		$dia = substr(trim($data), -10,2);
	}
	
$sql = "SELECT
				d.sidid, t.ordid, t.celid
	 		FROM
	 			demandas.demanda d
			LEFT JOIN 
				demandas.tiposervico t ON t.tipid = d.tipid	 			
	 		WHERE 
	 			d.dmdid=".$dmdid;
	
	$dadosDemanda = $db->carregar($sql);
	
	
	if ($dadosDemanda[0]['ordid'] == 1 || $dadosDemanda[0]['ordid'] == 18 || $dadosDemanda[0]['ordid'] == 19 || $dadosDemanda[0]['ordid'] == 20 || $dadosDemanda[0]['ordid'] == 21){ //sistemas de informação
		$sql = "SELECT DISTINCT
					u.usucpf,
					u.usunome
				FROM
					seguranca.usuario AS u
				INNER JOIN 
					demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf				 
				WHERE 
					ur.rpustatus = 'A' AND 
					ur.celid in (select celid from demandas.sistemacelula where sidid = " . $dadosDemanda[0]['sidid'] . ")
				ORDER BY u.usunome	
				";		
	}
	elseif($dadosDemanda[0]['ordid'] == 2 || $dadosDemanda[0]['ordid'] == 23 || $dadosDemanda[0]['ordid'] == 24){ //2=redes e 23=generica
		$sql = "SELECT DISTINCT
					u.usucpf,
					u.usunome
				FROM
					seguranca.usuario AS u
				INNER JOIN 
					demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf				 
				WHERE 
					ur.rpustatus = 'A' AND 
					ur.celid = ".$dadosDemanda[0]['celid']." 
				ORDER BY u.usunome	
				";		
	}
	else{ 
		$sql = "SELECT DISTINCT
					u.usucpf,
					u.usunome
				FROM
					seguranca.usuario  AS u
				INNER JOIN 
					demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf
				WHERE 
					ur.rpustatus = 'A' AND 
					ur.sidid IS NULL AND
					ur.ordid = ".$dadosDemanda[0]['ordid']." 
				ORDER BY u.usunome
				";
	}
		
	graphWeek($sql,$horario = $_POST['horario'], $tipoSemana = $_POST['tipoSemana'],$dia,$mes,$ano);
	exit;
}


if ($_POST['tipoSemana'] && $_POST['data'] && $_POST['diario'] == 1) {
	header('content-type: text/html; charset=ISO-8859-1');
	
	$dmdid  = $_SESSION['dmdid'];
	$usucpf = $_GET['usucpf'];
	if(strlen(trim($_POST['data'])) == 10){
		$anoD = substr(trim($_POST['data']), -4,4);
		$mesD = substr(trim($_POST['data']), -7,2);
		$diaD = substr(trim($_POST['data']), -10,2);
		
		$_POST['tipoSemana'] = 1;
		
		if($_POST['tipoSemana'] == "1"){
			$data =  date('d-m-Y',mktime(0,0,0,$mesD,$diaD,$anoD));
		}
		else{
			$data =  date('d-m-Y',mktime(0,0,0,$mesD,$diaD,$anoD));
		}
		$ano = substr(trim($data), -4,4);
		$mes = substr(trim($data), -7,2);
		$dia = substr(trim($data), -10,2);
		
	}
	
$sql = "SELECT
				d.sidid, t.ordid, t.celid
	 		FROM
	 			demandas.demanda d
			LEFT JOIN 
				demandas.tiposervico t ON t.tipid = d.tipid	 			
	 		WHERE 
	 			d.dmdid=".$dmdid;
	
	$dadosDemanda = $db->carregar($sql);
	
	
	if ($dadosDemanda[0]['ordid'] == 1 || $dadosDemanda[0]['ordid'] == 18 || $dadosDemanda[0]['ordid'] == 19 || $dadosDemanda[0]['ordid'] == 20 || $dadosDemanda[0]['ordid'] == 21){ //sistemas de informação
		$sql = "SELECT DISTINCT
					u.usucpf,
					u.usunome
				FROM
					seguranca.usuario AS u
				INNER JOIN 
					demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf				 
				WHERE 
					ur.rpustatus = 'A' AND 
					ur.celid in (select celid from demandas.sistemacelula where sidid = " . $dadosDemanda[0]['sidid'] . ")
				ORDER BY u.usunome	
				";		
	}
	elseif($dadosDemanda[0]['ordid'] == 2 || $dadosDemanda[0]['ordid'] == 23 || $dadosDemanda[0]['ordid'] == 24){ //2=redes e 23=generica
		$sql = "SELECT DISTINCT
					u.usucpf,
					u.usunome
				FROM
					seguranca.usuario AS u
				INNER JOIN 
					demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf				 
				WHERE 
					ur.rpustatus = 'A' AND 
					ur.celid = ".$dadosDemanda[0]['celid']." 
				ORDER BY u.usunome	
				";		
	}
	else{ 
		$sql = "SELECT DISTINCT
					u.usucpf,
					u.usunome
				FROM
					seguranca.usuario  AS u
				INNER JOIN 
					demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf
				WHERE 
					ur.rpustatus = 'A' AND 
					ur.sidid IS NULL AND
					ur.ordid = ".$dadosDemanda[0]['ordid']." 
				ORDER BY u.usunome
				";
	}
		
	graphWeek($sql,$horario = $_POST['horario'], $tipoSemana = $_POST['tipoSemana'],$dia,$mes,$ano);
	exit;
}


if ($_POST['tipoSemana'] && $_POST['data'] && $_POST['semanal'] == 1) {
	header('content-type: text/html; charset=ISO-8859-1');
	$dmdid  = $_SESSION['dmdid'];
	$usucpf = $_GET['usucpf'];
	if(strlen(trim($_POST['data'])) == 10){
		$anoD = substr(trim($_POST['data']), -4,4);
		$mesD = substr(trim($_POST['data']), -7,2);
		$diaD = substr(trim($_POST['data']), -10,2);
		
		$_POST['tipoSemana'] = 7;
		
		if($_POST['tipoSemana'] == "1"){
			$data =  date('d-m-Y',mktime(0,0,0,$mesD,$diaD,$anoD));
		}
		else{
			$data =  date('d-m-Y',mktime(0,0,0,$mesD,$diaD,$anoD));
		}
		$ano = substr(trim($data), -4,4);
		$mes = substr(trim($data), -7,2);
		$dia = substr(trim($data), -10,2);
		
	}
	
$sql = "SELECT
				d.sidid, t.ordid, t.celid
	 		FROM
	 			demandas.demanda d
			LEFT JOIN 
				demandas.tiposervico t ON t.tipid = d.tipid	 			
	 		WHERE 
	 			d.dmdid=".$dmdid;
	
	$dadosDemanda = $db->carregar($sql);
	
	
	if ($dadosDemanda[0]['ordid'] == 1 || $dadosDemanda[0]['ordid'] == 18 || $dadosDemanda[0]['ordid'] == 19 || $dadosDemanda[0]['ordid'] == 20 || $dadosDemanda[0]['ordid'] == 21){ //sistemas de informação
		$sql = "SELECT DISTINCT
					u.usucpf,
					u.usunome
				FROM
					seguranca.usuario AS u
				INNER JOIN 
					demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf				 
				WHERE 
					ur.rpustatus = 'A' AND 
					ur.celid in (select celid from demandas.sistemacelula where sidid = " . $dadosDemanda[0]['sidid'] . ")
				ORDER BY u.usunome	
				";		
	}
	elseif($dadosDemanda[0]['ordid'] == 2 || $dadosDemanda[0]['ordid'] == 23 || $dadosDemanda[0]['ordid'] == 24){ //2=redes e 23=generica
		$sql = "SELECT DISTINCT
					u.usucpf,
					u.usunome
				FROM
					seguranca.usuario AS u
				INNER JOIN 
					demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf				 
				WHERE 
					ur.rpustatus = 'A' AND 
					ur.celid = ".$dadosDemanda[0]['celid']." 
				ORDER BY u.usunome	
				";		
	}
	else{ 
		$sql = "SELECT DISTINCT
					u.usucpf,
					u.usunome
				FROM
					seguranca.usuario  AS u
				INNER JOIN 
					demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf
				WHERE 
					ur.rpustatus = 'A' AND 
					ur.sidid IS NULL AND
					ur.ordid = ".$dadosDemanda[0]['ordid']." 
				ORDER BY u.usunome
				";
	}
		
	graphWeek($sql,$horario = $_POST['horario'], $tipoSemana = $_POST['tipoSemana'],$dia,$mes,$ano);
	exit;
}


if ($_POST['tipoSemana'] && $_POST['data'] && $_POST['mensal'] == 1) {
	header('content-type: text/html; charset=ISO-8859-1');
	$dmdid  = $_SESSION['dmdid'];
	$usucpf = $_GET['usucpf'];
	if(strlen(trim($_POST['data'])) == 10){
		$anoD = substr(trim($_POST['data']), -4,4);
		$mesD = substr(trim($_POST['data']), -7,2);
		$diaD = substr(trim($_POST['data']), -10,2);
		
		$_POST['tipoSemana'] = "mes";
		
		if($_POST['tipoSemana'] == "1"){
			$data =  date('d-m-Y',mktime(0,0,0,$mesD,$diaD,$anoD));
		}
		else{
			$data =  date('d-m-Y',mktime(0,0,0,$mesD,$diaD,$anoD));
		}
		$ano = substr(trim($data), -4,4);
		$mes = substr(trim($data), -7,2);
		$dia = substr(trim($data), -10,2);
		
	}
	
$sql = "SELECT
				d.sidid, t.ordid, t.celid
	 		FROM
	 			demandas.demanda d
			LEFT JOIN 
				demandas.tiposervico t ON t.tipid = d.tipid	 			
	 		WHERE 
	 			d.dmdid=".$dmdid;
	
	$dadosDemanda = $db->carregar($sql);
	
	
	if ($dadosDemanda[0]['ordid'] == 1 || $dadosDemanda[0]['ordid'] == 18 || $dadosDemanda[0]['ordid'] == 19 || $dadosDemanda[0]['ordid'] == 20 || $dadosDemanda[0]['ordid'] == 21){ //sistemas de informação
		$sql = "SELECT DISTINCT
					u.usucpf,
					u.usunome
				FROM
					seguranca.usuario AS u
				INNER JOIN 
					demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf				 
				WHERE 
					ur.rpustatus = 'A' AND 
					ur.celid in (select celid from demandas.sistemacelula where sidid = " . $dadosDemanda[0]['sidid'] . ")
				ORDER BY u.usunome	
				";		
	}
	elseif($dadosDemanda[0]['ordid'] == 2 || $dadosDemanda[0]['ordid'] == 23 || $dadosDemanda[0]['ordid'] == 24){ //2=redes e 23=generica
		$sql = "SELECT DISTINCT
					u.usucpf,
					u.usunome
				FROM
					seguranca.usuario AS u
				INNER JOIN 
					demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf				 
				WHERE 
					ur.rpustatus = 'A' AND 
					ur.celid = ".$dadosDemanda[0]['celid']." 
				ORDER BY u.usunome	
				";		
	}
	else{ 
		$sql = "SELECT DISTINCT
					u.usucpf,
					u.usunome
				FROM
					seguranca.usuario  AS u
				INNER JOIN 
					demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf
				WHERE 
					ur.rpustatus = 'A' AND 
					ur.sidid IS NULL AND
					ur.ordid = ".$dadosDemanda[0]['ordid']." 
				ORDER BY u.usunome
				";
	}
		
	graphWeek($sql,$horario = $_POST['horario'], $tipoSemana = $_POST['tipoSemana'],$dia,$mes,$ano);
	exit;
}



if ($_REQUEST['dmdidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	exibeInformacao($_REQUEST['dmdidAjax']);
	exit;
}

if ($_REQUEST['dmdidAlteraAajx']) {
	header('content-type: text/html; charset=ISO-8859-1');
	alteraDemanda($_REQUEST['dmdidAlteraAajx']);
	exit;
}

if ($_REQUEST['usucpfMesAjax'] && $_REQUEST['dataAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	exibeDemandasMes($_REQUEST['usucpfMesAjax'], $_REQUEST['dataAjax']);
	exit;
}

if ($_POST['tipoSemana'] && $_POST['data'] && $_POST['horario']) {
	header('content-type: text/html; charset=ISO-8859-1');
	$dmdid  = $_SESSION['dmdid'];
	$usucpf = $_GET['usucpf'];
	if(strlen(trim($_POST['data'])) == 10){
		$anoD = substr(trim($_POST['data']), -4,4);
		$mesD = substr(trim($_POST['data']), -7,2);
		$diaD = substr(trim($_POST['data']), -10,2);
		
		if($_POST['tipoSemana'] == "1"){
			$data =  date('d-m-Y',mktime(0,0,0,$mesD,$diaD,$anoD));
		}
		else{
			$data =  date('d-m-Y',mktime(0,0,0,$mesD,$diaD,$anoD));
		}
		$ano = substr(trim($data), -4,4);
		$mes = substr(trim($data), -7,2);
		$dia = substr(trim($data), -10,2);
		
	}
	
$sql = "SELECT
				d.sidid, t.ordid, t.celid
	 		FROM
	 			demandas.demanda d
			LEFT JOIN 
				demandas.tiposervico t ON t.tipid = d.tipid	 			
	 		WHERE 
	 			d.dmdid=".$dmdid;
	
	$dadosDemanda = $db->carregar($sql);
	
	
	if ($dadosDemanda[0]['ordid'] == 1 || $dadosDemanda[0]['ordid'] == 18 || $dadosDemanda[0]['ordid'] == 19 || $dadosDemanda[0]['ordid'] == 20 || $dadosDemanda[0]['ordid'] == 21){ //sistemas de informação
		$sql = "SELECT DISTINCT
					u.usucpf,
					u.usunome
				FROM
					seguranca.usuario AS u
				INNER JOIN 
					demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf				 
				WHERE 
					ur.rpustatus = 'A' AND 
					ur.celid in (select celid from demandas.sistemacelula where sidid = " . $dadosDemanda[0]['sidid'] . ")
				ORDER BY u.usunome	
				";		
	}
	elseif($dadosDemanda[0]['ordid'] == 2 || $dadosDemanda[0]['ordid'] == 23 || $dadosDemanda[0]['ordid'] == 24){ //2=redes e 23=generica
		$sql = "SELECT DISTINCT
					u.usucpf,
					u.usunome
				FROM
					seguranca.usuario AS u
				INNER JOIN 
					demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf				 
				WHERE 
					ur.rpustatus = 'A' AND 
					ur.celid = ".$dadosDemanda[0]['celid']." 
				ORDER BY u.usunome	
				";		
	}
	else{ 
		$sql = "SELECT DISTINCT
					u.usucpf,
					u.usunome
				FROM
					seguranca.usuario  AS u
				INNER JOIN 
					demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf
				WHERE 
					ur.rpustatus = 'A' AND 
					ur.sidid IS NULL AND
					ur.ordid = ".$dadosDemanda[0]['ordid']." 
				ORDER BY u.usunome
				";
	}
		
	graphWeek($sql,$horario = $_POST['horario'], $tipoSemana = $_POST['tipoSemana'],$dia,$mes,$ano);
	exit;
}


$dmdid  = $_SESSION['dmdid'];
$usucpf = $_GET['usucpf'];

if (!$dmdid){
	die('<script>
			alert(\'Falta parametros! Tente novamente.\');
			window.close();
		 </script>');
}
monta_titulo( 'Pesquisa de Responsável', 'Escolha o Técnico desejado para adicioná-lo à demanda' );

?>
 <head>
 <style type="text/css">
 
 .titulo1{background-color:rgb(227, 227, 227);text-align:center;font-weight:bold;}
 .linha1{background-color:#f5f5f5;}
 .linha2{background-color:#fdfdfd;}
 </style>
 <link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
 <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
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

d = document;
  			
	/*
	 * Valida e envia para página pai
	*/
	function enviar(){
		usucpf = d.getElementsByName('usucpf')[0];
		
		if (usucpf.value != ''){
			window.opener.d.getElementById('usucpfexecutor').value  	  = usucpf.value;
			window.opener.d.getElementsByName('usunomeexecutor')[0].value = usucpf.options[usucpf.selectedIndex].text;;
		}else{
			window.opener.d.getElementById('usucpfexecutor').value  	  = '';
			window.opener.d.getElementsByName('usunomeexecutor')[0].value = '';
			alert('Nenhum responsável foi vinculado!');			
		}
		window.close();
	}	
	
	function vincula(cpf,nome){
		window.opener.d.getElementById('usucpfexecutor').value  	  = cpf;
		window.opener.d.getElementsByName('usunomeexecutor')[0].value = nome;
		window.close();
	}
	function exibeDemandaAjax(cpf){
		var usucpf = cpf;
		
		var img_mais = d.getElementById('img_mais_' + usucpf);
		var img_menos = d.getElementById('img_menos_' + usucpf);
		var tr = d.getElementById('tr_exibe_' + usucpf);
		var td = d.getElementById('td_exibe_' + usucpf);
		
		if(cpf){		
		
		// Faz uma requisição ajax
		var req = new Ajax.Request('demandas.php?modulo=principal/popup/pesqUser2&acao=A', {
								        method:     'post',
								        parameters: '&usucpfAjax=' + cpf,
								        onComplete: function (res)
								        {			
								        	
											td.innerHTML = res.responseText;
											tr.style.display = '';
											img_mais.style.display = 'none';
											img_menos.style.display = '';
	
								        }
								  });
		}
	}
	
	function montaGrafico(hr_ini,hr_fim,data,usucpf,dmdid,qtd_dmd,cor_h){
		var ini = hr_ini;
		var fim = hr_fim;
		var cpf = usucpf;
		var qtd_d = qtd_dmd;
		var demanda = dmdid;
		var cor = cor_h;
		
		ini = ini.toString().replace("01","1");
		ini = ini.toString().replace("02","2");
		ini = ini.toString().replace("03","3");
		ini = ini.toString().replace("04","4");
		ini = ini.toString().replace("05","5");
		ini = ini.toString().replace("06","6");
		ini = ini.toString().replace("07","7");
		ini = ini.toString().replace("08","8");
		ini = ini.toString().replace("09","9");
		ini = ini * 1;
		fim = fim.toString().replace("01","1");
		fim = fim.toString().replace("02","2");
		fim = fim.toString().replace("03","3");
		fim = fim.toString().replace("04","4");
		fim = fim.toString().replace("05","5");
		fim = fim.toString().replace("06","6");
		fim = fim.toString().replace("07","7");
		fim = fim.toString().replace("08","8");
		fim = fim.toString().replace("09","9");
		fim = fim * 1;
		
		while(ini <= fim){
			var div = 'cpf_' + cpf + '_dem_' + qtd_d + '_hr_' + ini + '_data_' + data;
			
			if(document.getElementById(div)){
				document.getElementById(div).style.backgroundColor = cor;
				document.getElementById(div).style.cursor = 'pointer';
				document.getElementById(div).innerHTML = "<div onmouseout=\"SuperTitleOff(this);\" onmousemove=\"SuperTitleAjax('demandas.php?modulo=principal/popup/pesqUser2&acao=A&dmdidAjax=" + demanda + "',this)\" style=\"width:100%;text-align:center;color: " + cor + "\">-<div>";
			}
			ini= ini + 1; 
		}
	}
	
	function montaGraficoMensal(data,usucpf,dmdid){
		var cpf = usucpf;
		var demanda = dmdid;
		var div = cpf + '_data_' + data;
		data2 = data.toString().replace("_","/");
		data2 = data2.toString().replace("_","/");
		data2 = data2.toString().replace("_","/");
		if(document.getElementById(div)){
			document.getElementById(div).style.backgroundColor = "#87CEFA";
			document.getElementById(div).style.cursor = 'pointer';
			document.getElementById(div).innerHTML = "<div onclick=\"chamaDia('"+ data2 + "');\" onmouseout=\"SuperTitleOff(this);\" onmousemove=\"SuperTitleAjax('demandas.php?modulo=principal/popup/pesqUser2&acao=A&usucpfMesAjax=" + usucpf + "&dataAjax=" + data2 +"',this)\" style=\"width:100%;text-align:center;color:#87CEFA\">-<div>";
		}
	}
	
	
	function selecionaTR(cor,id){
		var cor2 = cor;
		var tr = id;
		document.getElementById('tec_' + tr).style.backgroundColor = cor2;
	}
	
	function chamaDia(dia){
		d.getElementById('data_pesq').value = dia;
		var data = d.getElementById('data_pesq').value
		tipoDiario();
	}
	
	function abreDemanda(demanda){
		var dmdid = demanda;
		//alert('Abrirá demanda dmdid-> ' + dmdid);
	}
	
	function escondeDemandaAjax(cpf){
		var usucpf = cpf;
		d.getElementById('img_menos_' + usucpf).style.display = "none";
		d.getElementById('tr_exibe_' + usucpf).style.display = "none";
		d.getElementById('td_exibe_' + usucpf).innerHTML = "";
		d.getElementById('img_mais_' + usucpf).style.display = "";
	}
	
	function avancar(){
		var data = d.getElementById('data_pesq').value;
		var tipoSemana = d.getElementById('tipoSemana').value;
		var horario = d.getElementById('horario').value;
		if(tipoSemana && data){		
		var div = d.getElementById('exibeDemandas');
		
		// Faz uma requisição ajax
		var req = new Ajax.Request('demandas.php?modulo=principal/popup/pesqUser2&acao=A', {
								        method:     'post',
								        parameters: '&avancar=1&tipoSemana=' + tipoSemana +'&data=' + data +'&horario=' + horario,
								        onComplete: function (res)
								        {			
											div.innerHTML = res.responseText; 
											extraiScript(res.responseText);
								        }
								  });
		}
	}	
	function recuar(){
		var data = d.getElementById('data_pesq').value;
		var tipoSemana = d.getElementById('tipoSemana').value;
		var horario = d.getElementById('horario').value;
		if(tipoSemana && data){		
		var div = d.getElementById('exibeDemandas');
		
		// Faz uma requisição ajax
		var req = new Ajax.Request('demandas.php?modulo=principal/popup/pesqUser2&acao=A', {
								        method:     'post',
								        parameters: '&recuar=1&tipoSemana=' + tipoSemana +'&data=' + data +'&horario=' + horario,
								        onComplete: function (res)
								        {			
											div.innerHTML = res.responseText;
											extraiScript(res.responseText);
								        }
								  });
		}
	}
	function tipoDiario(){
		
		d.getElementById('tipoSemana').value = 1;
		var data = d.getElementById('data_pesq').value;
		var horario = d.getElementById('horario').value;
		var tipoSemana = 1;
		if(tipoSemana && data){		
		var div = d.getElementById('exibeDemandas');
		
		// Faz uma requisição ajax
		var req = new Ajax.Request('demandas.php?modulo=principal/popup/pesqUser2&acao=A', {
								        method:     'post',
								        parameters: '&diario=1&tipoSemana=' + tipoSemana +'&data=' + data +'&horario=' + horario,
								        onComplete: function (res)
								        {			
											div.innerHTML = res.responseText;
											extraiScript(res.responseText);
											d.getElementById('div_semanal').style.color= "#000000";
											d.getElementById('div_diario').style.color= "red";
											d.getElementById('div_mensal').style.color= "#000000";
								        }
								  });
		}
	}
	
	function tipoSemanal(){
		d.getElementById('tipoSemana').value = 7;
		var data = d.getElementById('data_pesq').value;
		var horario = d.getElementById('horario').value;
		var tipoSemana = 7;
		if(tipoSemana && data){		
		var div = d.getElementById('exibeDemandas');

		// Faz uma requisição ajax
		var req = new Ajax.Request('demandas.php?modulo=principal/popup/pesqUser2&acao=A', {
								        method:     'post',
								        parameters: '&semanal=1&tipoSemana=' + tipoSemana +'&data=' + data +'&horario=' + horario,
								        onComplete: function (res)
								        {			
											div.innerHTML = res.responseText;
											extraiScript(res.responseText);
											d.getElementById('div_semanal').style.color= "red";
											d.getElementById('div_diario').style.color= "#000000";
											d.getElementById('div_mensal').style.color= "#000000";
								        }
								  });
		}
	}
	
	function tipoMensal(){
		d.getElementById('tipoSemana').value = "mes";
		var data = d.getElementById('data_pesq').value;
		var horario = d.getElementById('horario').value;
		var tipoSemana = "mes";
		if(tipoSemana && data){		
		var div = d.getElementById('exibeDemandas');

		// Faz uma requisição ajax
		var req = new Ajax.Request('demandas.php?modulo=principal/popup/pesqUser2&acao=A', {
								        method:     'post',
								        parameters: '&mensal=1&tipoSemana=' + tipoSemana +'&data=' + data,
								        onComplete: function (res)
								        {			
											div.innerHTML = res.responseText;
											extraiScript(res.responseText);
											d.getElementById('div_semanal').style.color= "#000000";
											d.getElementById('div_diario').style.color= "#000000";
											d.getElementById('div_simples').style.color= "#000000";
											d.getElementById('div_completo').style.color= "#000000";
											d.getElementById('div_mensal').style.color= "red";
								        }
								  });
		}
	}
	
	
	function tipoSimples(){
		var data = d.getElementById('data_pesq').value;
		var tipoSemana = d.getElementById('tipoSemana').value;
		if(tipoSemana == "mes"){
			d.getElementById('tipoSemana').value = "1";
			d.getElementById('div_mensal').style.color= "#000000";
			d.getElementById('div_diario').style.color= "red";
		}
		d.getElementById('horario').value = "simples";
		if(tipoSemana && data){		
		var div = d.getElementById('exibeDemandas');

		// Faz uma requisição ajax
		var req = new Ajax.Request('demandas.php?modulo=principal/popup/pesqUser2&acao=A', {
								        method:     'post',
								        parameters: '&horario=simples&tipoSemana= ' + tipoSemana +'&data=' + data,
								        onComplete: function (res)
								        {			
											div.innerHTML = res.responseText;
											extraiScript(res.responseText);
											d.getElementById('div_simples').style.color= "red";
											d.getElementById('div_completo').style.color= "#000000";
								        }
								  });
		}
	}
	function tipoCompleto(){
		var data = d.getElementById('data_pesq').value;
		var tipoSemana = d.getElementById('tipoSemana').value;
		if(tipoSemana == "mes"){
			d.getElementById('tipoSemana').value = "1";
			d.getElementById('div_mensal').style.color= "#000000";
			d.getElementById('div_diario').style.color= "red";
		}
		d.getElementById('horario').value = "completo";
		if(tipoSemana && data){		
		var div = d.getElementById('exibeDemandas');

		// Faz uma requisição ajax
		var req = new Ajax.Request('demandas.php?modulo=principal/popup/pesqUser2&acao=A', {
								        method:     'post',
								        parameters: '&horario=completo&tipoSemana= ' + tipoSemana +'&data=' + data,
								        onComplete: function (res)
								        {
											div.innerHTML = res.responseText;
											extraiScript(res.responseText);
											d.getElementById('div_simples').style.color= "#000000";
											d.getElementById('div_completo').style.color= "red";
								        }
								  });
		}
	}
	
	function alteraDemanda(id){
		var dmdid = id;
		var div = d.getElementById('alteraDemanda');
		// Faz uma requisição ajax
		var req = new Ajax.Request('demandas.php?modulo=principal/popup/pesqUser2&acao=A', {
								        method:     'post',
								        parameters: '&dmdidAlteraAajx=' + dmdid,
								        onComplete: function (res)
								        {
											div.style.display = "";
											div.innerHTML = res.responseText;
											extraiScript(res.responseText);
								        }
								  });
	}
	
	function fechar(id){
		var dmdid = id;
		var div = d.getElementById(dmdid);
		div.style.display = "none";
	}
	
	function mostraGrafico()
	{
		document.formulario.submit();
	}
	
</script>
</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
	<?
		$perfil = arrayPerfil();
		
		if ( in_array(DEMANDA_PERFIL_GESTOR_MEC, $perfil) || in_array(DEMANDA_PERFIL_TECNICO1, $perfil) || in_array(DEMANDA_PERFIL_SUPERUSUARIO, $perfil) ){
		
			$dmdid = $_SESSION['dmdid'];
			
			$sql = "SELECT DISTINCT
						u.usucpf as cpf,
						u.usunome as nome
					FROM
						seguranca.usuario  AS u
					INNER JOIN 
						demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf
					inner join 
						demandas.demanda d ON d.usucpfexecutor = u.usucpf
					inner JOIN 
						workflow.documento doc ON doc.docid = d.docid
					inner JOIN 
						workflow.estadodocumento ed ON ed.esdid = doc.esdid	
					INNER JOIN 
						seguranca.usuario_sistema us ON u.usucpf = us.usucpf						
					WHERE 
						us.sisid = ".$_SESSION['sisid']." AND
						us.suscod = 'A' AND
						ur.rpustatus = 'A' AND 
						ur.sidid IS NULL AND
						ur.ordid = (
									SELECT 
										tp.ordid
									FROM 
										demandas.demanda AS d
									LEFT JOIN 
										demandas.tiposervico AS tp ON tp.tipid = d.tipid
									WHERE 
										dmdid = $dmdid)
						and d.unaid = (select unaid from demandas.demanda where dmdid=$dmdid)
						and ed.esdid in (91,92,107,108)
						AND d.dmdid not in( select dmdid from demandas.pausademanda )
					ORDER BY u.usunome"; 
			$dadosx = $db->carregar($sql);
			
			$setor = $db->PegaUm("select ua.unasigla ||' - '|| ua.unadescricao from demandas.demanda d inner join demandas.unidadeatendimento ua ON ua.unaid = d.unaid where dmdid=$dmdid");
			
					?>
					<table class="tabela" bgcolor="#f5f5f5"	cellSpacing="1" cellPadding="3" align="center">
						<tr bgcolor="#DCDCDC">
							<td style="border-bottom: solid 2px black;">
								<b>Técnico / Setor: <?=$setor?></b>
							</td>
						</tr>
						<?if($dadosx){ ?>
							<?$cor = ''; ?>
							<?foreach($dadosx as $dadosx2){?>
								<?$cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ; ?>
								<tr bgcolor="<?= $cor ?>"  onmouseout="this.style.backgroundColor='<?= $cor ?>';" onmouseover="this.style.backgroundColor='#ffffcc';">
									<td>
										<a href="javascript:vincula('<?=$dadosx2['cpf']?>','<?=$dadosx2['nome']?>')"><font color="#3366CC"><?=$dadosx2['nome']?></font></a>
									</td>
								</tr>
							<?}?>
						<?}else{?>
							<tr>
								<td style="text-align:center; background-color:#fafafa; color:red; font-weight:bold; font-size: 10px;" >
									Não existe técnico para este setor.
								</td>
							</tr>
						<?}?>
						
					</table>
					
					<table class="tabela" bgcolor="#f5f5f5"
						cellSpacing="1" cellPadding="3" align="center">
						<tr bgcolor="#DCDCDC">
							<td>
								<b>Técnico / Quantidade</b>
							</td>
						</tr>
					</table>
	<?}?>

	<? demandas_lista_usuarios(); ?>
	
		<form id="formulario" name="formulario" action="" method="post">
			<input type="hidden" name="mostraGraficoWeek" value="1">
			<table class="tabela" bgcolor="#f5f5f5"
				cellSpacing="1" cellPadding="3" align="center">
				<tr bgcolor="#C0C0C0">
					<td align="center">
					<div style="padding:3px;cursor:pointer;border:#000000 solid 1px;background-color:#fcfcfc;width:100px;text-align:center;" onclick="mostraGrafico();">Mostrar Gráfico</div>
					</td>
				</tr>
			</table>
		</form>
		
		
		
<?if($_POST['mostraGraficoWeek']){ ?>

		<form>
		<table class="tabela" bgcolor="#f5f5f5"
			cellSpacing="1" cellPadding="3" align="center">
			<tr bgcolor="#C0C0C0">
				<td>
				<div style="margin-left:5px;float:left;padding:3px;cursor:pointer;border:#000000 solid 1px;background-color:#fcfcfc;width:20px;text-align:center;" onclick="recuar();"><</div>
				<div style="margin-left:5px;float:left;padding:3px;cursor:pointer;border:#000000 solid 1px;background-color:#fcfcfc;width:20px;text-align:center;" onclick="avancar();">></div>
				<div style="margin-left:15px;float:left;padding:3px;cursor:pointer;border:#000000 solid 1px;background-color:#fcfcfc;width:40px;text-align:center;" id="div_diario" onclick="tipoDiario();">Diário</div>
				<div style="margin-left:5px;float:left;padding:3px;cursor:pointer;border:#000000 solid 1px;background-color:#fcfcfc;width:50px;text-align:center;" id="div_semanal" onclick="tipoSemanal();">Semanal</div>
				<div style="margin-left:5px;float:left;padding:3px;cursor:pointer;border:#000000 solid 1px;background-color:#fcfcfc;width:50px;text-align:center;color:red" id="div_mensal" onclick="tipoMensal();">Mensal</div>
				<div style="margin-left:15px;float:left;padding:3px;cursor:pointer;border:#000000 solid 1px;background-color:#fcfcfc;width:40px;text-align:center;color:red;" id="div_simples" onclick="tipoSimples();">Simples</div>
				<div style="margin-left:5px;float:left;padding:3px;cursor:pointer;border:#000000 solid 1px;background-color:#fcfcfc;width:50px;text-align:center;" id="div_completo" onclick="tipoCompleto();">Completo</div>
				</td>
			</tr>
		</table>
		</form>
		
		<center>
		<div class="SubtituloEsquerda" style="padding: 0; margin: 0 auto;text-align: center;width:95%">Relatório de Atividades Técnicas - Demandas</div>
		<div style="display:none;margin: 0 auto;width:1px;"  id="alteraDemanda"></div>
		<div style="margin: 0 auto; padding: 0; height: 70%; width: 95%; background-color: #eee; border: none;" class="div_rolagem" id="exibeDemandas">
		<?php
		if(strlen(trim($_POST['data_pesq'])) == 10){
			$ano = substr(trim($_POST['data_pesq']), -4,4);
			$mes = substr(trim($_POST['data_pesq']), -7,2);
			$dia = substr(trim($_POST['data_pesq']), -10,2);
		}
		
		$sql = "SELECT
						d.sidid, t.ordid, t.celid
			 		FROM
			 			demandas.demanda d
					LEFT JOIN 
						demandas.tiposervico t ON t.tipid = d.tipid	 			
			 		WHERE 
			 			d.dmdid=".$dmdid;
			
			$dadosDemanda = $db->carregar($sql);
			
			
			if ($dadosDemanda[0]['ordid'] == 1 || $dadosDemanda[0]['ordid'] == 18 || $dadosDemanda[0]['ordid'] == 19 || $dadosDemanda[0]['ordid'] == 20 || $dadosDemanda[0]['ordid'] == 21){ //sistemas de informação
				$sql = "SELECT DISTINCT
							u.usucpf,
							u.usunome
						FROM
							seguranca.usuario AS u
						INNER JOIN 
							demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf				 
						WHERE 
							ur.rpustatus = 'A' AND 
							ur.celid in (select celid from demandas.sistemacelula where sidid = " . $dadosDemanda[0]['sidid'] . ")
						ORDER BY u.usunome	
						";		
			}
			elseif($dadosDemanda[0]['ordid'] == 2 || $dadosDemanda[0]['ordid'] == 23 || $dadosDemanda[0]['ordid'] == 24){ //2=redes e 23=generica
				$sql = "SELECT DISTINCT
							u.usucpf,
							u.usunome
						FROM
							seguranca.usuario AS u
						INNER JOIN 
							demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf				 
						WHERE 
							ur.rpustatus = 'A' AND 
							ur.celid = ".$dadosDemanda[0]['celid']." 
						ORDER BY u.usunome	
						";		
			}
			else{ 
				$sql = "SELECT DISTINCT
							u.usucpf,
							u.usunome
						FROM
							seguranca.usuario  AS u
						INNER JOIN 
							demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf
						WHERE 
							ur.rpustatus = 'A' AND 
							ur.sidid IS NULL AND
							ur.ordid = ".$dadosDemanda[0]['ordid']." 
						ORDER BY u.usunome
						";
			}
			
		?>
		
		
				<?=graphWeek($sql,$horario = $_POST['horario'], $tipoSemana = $_POST['tipoSemana'],$diaD = $dia,$mesD = $mes,$anoD = $ano);?>
		</div>
		</center>
		
<?}?>






</body>

</html>
<?php

/**
 * Função que cria a lista com os usuários que podem ser cadastrados
 * a uma determinada demanda
 * 
 * @author Fernando A. Bagno da Silva
 */
function demandas_lista_usuarios(){
	
	global $db,$usucpf;

$sql = "SELECT
				d.sidid, t.ordid, t.celid
	 		FROM
	 			demandas.demanda d
			LEFT JOIN 
				demandas.tiposervico t ON t.tipid = d.tipid	 			
	 		WHERE 
	 			d.dmdid=".$_SESSION['dmdid'];
	
	$dadosDemanda = $db->carregar($sql);
	
	if($dadosDemanda[0]['ordid']){
	
		if ($dadosDemanda[0]['ordid'] == 1 || $dadosDemanda[0]['ordid'] == 18 || $dadosDemanda[0]['ordid'] == 19 || $dadosDemanda[0]['ordid'] == 20 || $dadosDemanda[0]['ordid'] == 21){ //sistemas de informação
			
			if(!$dadosDemanda[0]['sidid']){
				echo '<script>
						alert("É necessário informar o sistema na aba Dados da Demanda!");
						window.close();
					  </script>';
				exit;	
			}
			
			$sql = "SELECT DISTINCT
						u.usucpf,
						u.usunome
					FROM
						seguranca.usuario AS u
					INNER JOIN 
						demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf	
					INNER JOIN 
						seguranca.usuario_sistema us ON u.usucpf = us.usucpf 		 
					WHERE 
						us.sisid = ".$_SESSION['sisid']." AND
						us.suscod = 'A' AND
						ur.rpustatus = 'A' AND 
						ur.celid in (select celid from demandas.sistemacelula where sidid = " . $dadosDemanda[0]['sidid'] . ")
					ORDER BY u.usunome	
					";		
		}
		elseif($dadosDemanda[0]['ordid'] == 2 || $dadosDemanda[0]['ordid'] == 23 || $dadosDemanda[0]['ordid'] == 24){ //2=redes e 23=generica
			$sql = "SELECT DISTINCT
						u.usucpf,
						u.usunome
					FROM
						seguranca.usuario AS u
					INNER JOIN 
						demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf	
					INNER JOIN 
						seguranca.usuario_sistema us ON u.usucpf = us.usucpf			 
					WHERE 
						us.sisid = ".$_SESSION['sisid']." AND
						us.suscod = 'A' AND
						ur.rpustatus = 'A' AND 
						ur.celid = ".$dadosDemanda[0]['celid']." 
					ORDER BY u.usunome	
					";		
		}
		else{ 
			$sql = "SELECT DISTINCT
						u.usucpf,
						u.usunome
					FROM
						seguranca.usuario  AS u
					INNER JOIN 
						demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf
					INNER JOIN 
						seguranca.usuario_sistema us ON u.usucpf = us.usucpf
					WHERE 
						us.sisid = ".$_SESSION['sisid']." AND
						us.suscod = 'A' AND
						ur.rpustatus = 'A' AND 
						ur.sidid IS NULL AND
						ur.ordid = ".$dadosDemanda[0]['ordid']." 
					ORDER BY u.usunome
					";
		}
		
	}
	else{
		echo "<script>alert('É necessário classificar a demanda primeiro para depois selecionar o técnico.'); window.close();</script>";
		exit;	
	}
	//dbg($sql,1);
	
			?>
			<table id="tabela" width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<tr >
					<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color:#E3E3E3; ">
						<b>Nome do Responsável</b>
					</td>
					<td align='center' valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color:#E3E3E3; ">
						<b>Quantidade em Atendimento</b>
					</td>		
					<td align='center' valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; background-color:#E3E3E3; ">
						<b>Quantidade c/ Pausa</b>
					</td>		
				</tr>
				<script language="JavaScript" src="../includes/wz_tooltip.js"></script> 
			<?

			$listas = $db->carregar( $sql );
			$listas = $listas ? $listas : array();	

			foreach($listas as $lts){
				$cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ;	
				
				//verifica se e´ suporte de atendimento ou sistema de informações
				/*
				$sql = "SELECT sidid FROM demandas.demanda WHERE dmdid=".$_SESSION['dmdid'];
				$sididaux = $db->pegaUm($sql);
				*/
				if($sididaux) $ids_estado_documento = "91,92,93,107,108,111";
				else $ids_estado_documento = "91,92,107,108";

				
				$sql = "SELECT 
									count(*) as qtd
								FROM
									demandas.demanda as d
								LEFT JOIN 
									workflow.documento doc ON doc.docid 	  = d.docid
								LEFT JOIN 
									workflow.estadodocumento ed ON ed.esdid = doc.esdid
								/*
								INNER JOIN ( select pp.dmdid, count(pp.dmdid) as dmdidpausa, to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') as dttempopausa 
											 from demandas.pausademanda pp
											 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
											 inner join workflow.documento doc2 ON doc2.docid = dd.docid 
											 where doc2.esdid in (91,92,107,108)
											 group by dd.dmddatafimprevatendimento,pp.dmdid) ps ON ps.dmdid = d.dmdid
								*/	
								WHERE 
									d.usucpfexecutor = '".$lts['usucpf']."'
									AND d.usucpfdemandante is not null
									AND d.dmdstatus = 'A'
									AND ed.esdstatus = 'A' 
									AND doc.esdid in ({$ids_estado_documento})
									--AND dmdidpausa > 0 AND ( dttempopausa is null OR dttempopausa > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') )
									AND ( select count(pp.dmdid) 
											 from demandas.pausademanda pp
											 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
											 inner join workflow.documento doc2 ON doc2.docid = dd.docid 
											 where doc2.esdid in (91,92,107,108)
											 and pp.dmdid = d.dmdid) > 0 
									AND ( ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') 
											 from demandas.pausademanda pp
											 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
											 inner join workflow.documento doc2 ON doc2.docid = dd.docid 
											 where doc2.esdid in (91,92,107,108)
											 and pp.dmdid = d.dmdid
											 group by dd.dmddatafimprevatendimento) is null 
									OR 	  ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') 
											 from demandas.pausademanda pp
											 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
											 inner join workflow.documento doc2 ON doc2.docid = dd.docid 
											 where doc2.esdid in (91,92,107,108)
											 and pp.dmdid = d.dmdid
											 group by dd.dmddatafimprevatendimento) > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') )
									--AND d.dmdid in( select dmdid from demandas.pausademanda )
								";
				//dbg($sql,1);
				$qtdpausa = $db->PegaUm( $sql );	
				if(!$qtdpausa) $qtdpausa = 0;
				
				
				$sql = "SELECT 
									count(*) as qtd
								FROM
									demandas.demanda as d
								LEFT JOIN 
									workflow.documento doc ON doc.docid 	  = d.docid
								LEFT JOIN 
									workflow.estadodocumento ed ON ed.esdid = doc.esdid
								WHERE 
									d.usucpfexecutor = '".$lts['usucpf']."'
									AND d.usucpfdemandante is not null
									AND d.dmdstatus = 'A'
									AND ed.esdstatus = 'A' 
									AND doc.esdid in ({$ids_estado_documento})
									--AND d.dmdid not in( select dmdid from demandas.pausademanda )
								";
				//dbg($sql,1);
				$qtd = $db->PegaUm( $sql );
				if(!$qtd) $qtd = 0;	

				$qtd = $qtd - $qtdpausa;
				/*
				$sql = "SELECT 			
							d.dmdid,
	                    	d.dmdtitulo as assunto,
	                    	loc.lcadescricao||' '||a.anddescricao||' '||d.dmdsalaatendimento  as local,			                    
	                    	to_char(d.dmddatainiprevatendimento, 'DD-MM-YYYY HH24:MI:SS') AS dtinic,
	                    	to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') AS dtfim  
						FROM
							demandas.demanda as d
						INNER JOIN 
							workflow.documento doc ON doc.docid = d.docid
						INNER JOIN 
							workflow.estadodocumento ed ON ed.esdid = doc.esdid							
						INNER JOIN 
							demandas.localandaratendimento laa ON laa.laaid = d.laaid
						INNER JOIN 
							demandas.andaratendimento a ON a.andid  = laa.andid
						INNER JOIN 
							demandas.localatendimento loc ON loc.lcaid  = laa.lcaid
						WHERE 
							d.usucpfexecutor = '".$lts['usucpf']."'
							AND d.dmdstatus = 'A'
							AND ed.esdstatus = 'A' 
							AND doc.esdid in ({$ids_estado_documento})						
							
							";
				$dados = $db->pegaLinha( $sql );	
			  */
			?>
			 	<tr id="<?=$lts['usucpf']?>" bgcolor="<?=$cor?>" onmouseout="this.style.backgroundColor='<?= $cor ?>';" onmouseover="this.style.backgroundColor='#ffffcc';">
			 					 		
			 			<?php 
						if($qtd != 0 || $qtdpausa != 0){
							echo("<td>
									<img style=\"cursor:pointer\" onclick=\"exibeDemandaAjax('".$lts['usucpf']."');\" id=\"img_mais_".$lts['usucpf']."\" src=\"../imagens/mais.gif\" border=\"0\">
									<img style=\"cursor:pointer;display:none\" onclick=\"escondeDemandaAjax('".$lts['usucpf']."');\" id=\"img_menos_".$lts['usucpf']."\" src=\"../imagens/menos.gif\" border=\"0\">
									<a href=\"javascript: vincula('".$lts['usucpf']."','".$lts['usunome']."');\">".$lts['usunome']."</a>
								</td>
								<td align='center' >	
						 			".$qtd."	 			
						 		</td>
								<td align='center' >	
						 			".$qtdpausa."	 			
						 		</td>
						 		</tr><tr style=\"display:none;\"  id=\"tr_exibe_{$lts['usucpf']}\" ><td id=\"td_exibe_{$lts['usucpf']}\" style=\"padding-left:20px;\" colspan=2 ></td></tr>	
								");
						}else{
						?>						
						<td>	
							<a href="javascript: vincula('<?=$lts['usucpf']?>','<?=$lts['usunome']?>');"><?=$lts['usunome']?></a>
				 		</td>
				 		<td align='center' >	
				 			<?=$qtd?>		 			
				 		</td>
				 		<td align='center' >	
				 			<?=$qtdpausa?>		 			
				 		</td>
						<?
						}
			 			?>
			 		
			 	</tr>
			<?
			}
			?>
			</table>
			<?
	
}

/*
Script que lista os dias da semana atual
*/

function graphWeek($sqlx = null,$horario = null, $tipoSemana = null,$diaD = null,$mesD = null,$anoD = null)
{
	global $db;
	
	//Paramentros específicos de datas:
	($diaD)? $dia = $diaD : $dia = date("d");
	($mesD)? $mes = $mesD : $mes = date("m");
	($anoD)? $ano = $anoD : $ano = date("Y");
	$dia_semana = date("w", strtotime ("$ano/$mes/$dia"));
	
	
	//Parametro Horário: Simples (Default) / Completo
	if($horario == 'completo'){
		$hora = array("0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23");
	}
	else{
		$hora = array("8","9","10","11","12","13","14","15","16","17","18");
	}
	//Parametro Tipo de Gráfico: Diário (Default) / Semanal / Mensal
	if(!$tipoSemana) $tipoSemana = 'mes';
	if($tipoSemana == 'mes'){		
	    //Função que pega a quatidade de dias do mês
		$tipo = cal_days_in_month(CAL_GREGORIAN, $mes, $ano);
		$data_inicial =  date('Y-m-d',mktime(0,0,0,$mes - $menos,$dia,$ano));
		$data_final =  date('Y-m-d',mktime(0,0,0,$mes + $mais,$dia,$ano));
	}
	if($tipoSemana == '7'){
		$tipo = 7; //Semanal
		//Soma os dias da semana
		switch($dia_semana){
			case 0:
				$mais = 7;
				$menos = 0;
				break;
			case 1:
				$mais = 6;
				$menos = 1;
				break;
			case 2:
				$mais = 5;
				$menos = 2;
				break;
			case 3:
				$mais = 4;
				$menos = 3;
				break;
			case 4:
				$mais = 3;
				$menos = 4;
				break;
			case 5:
				$mais = 2;
				$menos = 5;
				break;
			case 6:
				$mais = 1;
				$menos = 6;
				break;
			default:
				$mais = 1;
				$menos = 6;
		}
		$data_inicial =  date('Y-m-d',mktime(0,0,0,$mes,$dia-$menos,$ano));
		$data_final =  date('Y-m-d',mktime(0,0,0,$mes,$dia+$mais,$ano));

	}
	if($tipoSemana != '7' && $tipoSemana != 'mes'){
		$tipo = 1; //Dário
	}
	
	//Exibe o início da tabela com a classe do SIMEC
	if($tipoSemana == 'mes'){
		echo "<input type=\"hidden\" name=\"tipoSemana\" id=\"tipoSemana\" value=\"mes\" />";
	}
	if($tipoSemana == '7'){
		echo "<input type=\"hidden\" name=\"tipoSemana\" id=\"tipoSemana\" value=\"7\" />";
	}
	else{
		echo "<input type=\"hidden\" name=\"tipoSemana\" id=\"tipoSemana\" value=\"1\" />";
	}
	if($horario == 'completo'){
		echo "<input type=\"hidden\" name=\"horario\" id=\"horario\" value=\"completo\" />";
	}
	else{
		echo "<input type=\"hidden\" name=\"horario\" id=\"horario\" value=\"simples\" />";
	}
	echo "<input type=\"hidden\" name=\"data_pesq\" id=\"data_pesq\" value=\"$dia/$mes/$ano\" />";
	echo "<table align=center width=95% class=\"tabela\" cellpadding=\"3\" cellspacing=\"1\" >";
	
	if($tipoSemana == 'mes'){
		echo "<tr class=\"titulo1\" ><td colspan=".($tipo +1)." align=center>".mes($mes)."</td></tr>";
	}
	
	echo "<tr class=\"titulo1\" ><td rowspan=2 align=center>Técnico(s)</td>";
	
	if($tipoSemana!= 'mes'){
		//Contador que pecorrerá o tipo de Grafico (até 7 p/ Semanal : até 1 para Diário)
		$cont=0;
		while($cont < $tipo){
			//Exibe cada dia da Semana com colspan de acordo com o tipo de Horário selecionado
			echo "<td colspan=".count($hora)." align=center nowrap>";
			if($tipoSemana == '7'){
				//Pega dia / mes
				$dia_calendario = date("d/m",mktime(0,0,0,$mes,$dia-$dia_semana,$ano));
				//Pega dia da semana e aplica a função "dia" (Dom, Seg, Ter, Qua, Qui, Sex, Sab)
				$dia_s_calendario = dia(date("w",mktime(0,0,0,$mes,$dia-$dia_semana,$ano)));
				//Joga na variável linha o dia da semna / dia e mes
				$linha = $dia_s_calendario .'<br>'. $dia_calendario;
				//Verifica se o dia corrente é o dia correspondente no LOOP
			}
			else{
				$dia_calendario = $dia."/".$mes;
				$linha = dia($dia_semana) .'<br>'. $dia_calendario;
			}
			
			if($dia_calendario == date("d")."/".date("m")){
				//Aplica cor vermelha no texto
				echo "<font color=red>";
			    echo $linha;
			    echo "</font>";
			}
			//Se não for o dia corrente, simplesmente exibe o texto normal
			else{
				echo $linha;
			}
			echo "<br>";
			echo "</td>";
			$dia_semana--; //decrementa o dia da semana
			$cont++; //incrementa o contador
		}
	}
	else{
		//Contador que pecorrerá o tipo de Grafico (até 7 p/ Semanal : até 1 para Diário)
		$cont=1;
		while($cont <= $tipo){
			//pegar dia da semana
			$dia_s =  dia(date("w",mktime(0,0,0,$mes,(strlen($cont) == 1)? "0".$cont : $cont,$ano)));
			
			($dia_s == "Domingo" || $dia_s == "Sábado")? $cor = "#CDC9C9" : $cor = "";
			
			//Exibe cada dia do mes de acordo com o calendário
			echo "<td rowspan=2 style=\"background-color:$cor\"  align=center nowrap width=20 >";
			
			if( ((strlen($cont) == 1)? "0".$cont : $cont)."/".$mes."/".$ano == date("d")."/".date("m")."/".date("Y")){
				//Aplica cor vermelha no texto
				echo "<font color=red>";
			    echo "$cont<br />".substr($dia_s, 0,3);
			    echo "</font>";
			}
			else{
				echo "$cont<br />".substr($dia_s, 0,3);
			}
			echo "</td>";
			$cont++; //incrementa o contador
		}
		echo "<tr></tr>";
	}
	echo "</tr>";
	
	//Se não for mensal exibe as horas
	if($tipoSemana!= 'mes'){
		echo "<tr class=\"titulo1\" >";
		//Zera novamente o contador para utilizar em outro LOOP
		$cont=0;
		//Exibe as horas de acordo com o tipo de Horário escolhido (12 / 24)
		while($cont < $tipo){
			foreach($hora as $hora2){
						echo "<td >".((strlen($hora2) == 1)? "0$hora2" : $hora2). "</td>";
			}
			$cont++;
		}
	}
	echo "</tr>";
	
	//Carrega os dados do SQL
	$listausu = $db->carregar( $sqlx );
	$listausu = $listausu ? $listausu : array();
	
	if($tipoSemana!= 'mes'){	
		$cor = 1; //variável utilizada para exibir as cores das TDs
		//Verifica a existência de dados no array e faz o LOOP
		foreach($listausu as $listausu2){
			//SQL para contar o número de demandas de cada técnico
			$sql = "SELECT count(dmdid)
			 		FROM demandas.demanda
			 		WHERE usucpfexecutor = '{$listausu2['usucpf']}'
		 			AND dmdstatus = 'A' ";
		 	//Carrega a quatidade de demandas voltada do SQL
		 	$qtd_demandas = $db->pegaUm($sql);
			//verifica se exite demanda para exibir a TR do técnico
		 	if($qtd_demandas != 0){
			 	//Faz a alternancia das cores d cada TD
			 	($cor % 2)? $linhaX = "#fdfdfd" : "#f5f5f5";
				//Exibe a TD com o nome do Técnico 
			 	echo "<tr id=\"tec_{$listausu2['usucpf']}\" style=\"background-color:$linhaX;\" onmouseover=\"this.style.backgroundColor='#ffffcc';\" onmouseout=\"this.style.backgroundColor='$linhaX'\">";
				echo "<td ".(($qtd_demandas != 0 && $qtd_demandas != 1)? "rowspan=$qtd_demandas" : " ")." nowrap>";
				echo $listausu2['usunome'];
				echo "</td>";
				
				//verifica o número de demandas do técnico, para criar uma nova linha p/ cada demanda
				if($qtd_demandas != 1){
					//faz o LOOP enquando a demanda for maior que zero
					while($qtd_demandas > 0){		
						unset($dia_calendario); 
						unset($dia_s_calendario);
						unset($linha);
						
						//Paramentros específicos de datas:
						($diaD)? $dia = $diaD : $dia = date("d");
						($mesD)? $mes = $mesD : $mes = date("m");
						($anoD)? $ano = $anoD : $ano = date("Y");
						$dia_semana = date("w", strtotime ("$ano/$mes/$dia"));
							
						$wk=0; //contador dos dias da semana
						//Faz o LOOP enquanto o contador for menor que o tipo ( Diário = 1 / Semanal = 7)
						while($wk < $tipo){
							if($tipoSemana == '7'){
								$dia_calendario = date("d_m_Y",mktime(0,0,0,$mes,$dia-$dia_semana,$ano));
								$linha = $dia_calendario;
							}
							else{
								$dia_calendario = date("d_m_Y",strtotime("$ano/$mes/$dia"));
								$linha = $dia_calendario;
							}
							$hrs=0; //contador para exibir as cores nas TDs de horas
							//Cria as TDs para as horas de acordo com o Horario (12 / 24)
							while($hrs < count($hora)){
								($hrs % 2)? $cor_td = '#fdfdfd' : $cor_td = '#f5f5f5';
								echo "<td onmouseover=\"selecionaTR('#ffffcc','{$listausu2['usucpf']}');\" onmouseout=\"selecionaTR('$linhaX','{$listausu2['usucpf']}');\" style=\"background-color:$cor_td;color:$cor_td\" id=\"cpf_{$listausu2['usucpf']}_dem_{$qtd_demandas}_hr_{$hora[$hrs]}_data_$linha\" ><center>-</center></td>";
								$hrs++;
							}
							$dia_semana--; //decrementa o dia da semana
							$wk++; //incrementa o dia da semana
						}
						$qtd_demandas--; //decrementa as linhas por demanda
						echo "</tr>";
					}
				}else{
					unset($dia_calendario);
					unset($dia_s_calendario);
					unset($linha);
					
					//Paramentros específicos de datas:
					($diaD)? $dia = $diaD : $dia = date("d");
					($mesD)? $mes = $mesD : $mes = date("m");
					($anoD)? $ano = $anoD : $ano = date("Y");
					$dia_semana = date("w", strtotime ("$ano/$mes/$dia"));
					
					$wk=0; // contador para dias da semana
					while($wk < $tipo){		
						if($tipoSemana == '7'){
								$dia_calendario = date("d_m_Y",mktime(0,0,0,$mes,$dia-$dia_semana,$ano));
								$linha = $dia_calendario;
							}
							else{
								$dia_calendario = date("d_m_Y",strtotime("$ano/$mes/$dia"));
								$linha = $dia_calendario;
						}
						//$dia_calendario = date("d_m_Y",mktime(0,0,0,$mes,$dia-$dia_semana,$ano));
						//$linha = $dia_calendario;	
						$hrs=0; //contador para as cores das TDs de horas
						while($hrs < count($hora)){
							($hrs % 2)? $cor_td = '#fdfdfd' : $cor_td = '#f5f5f5';
							echo "<td onmouseover=\"selecionaTR('#ffffcc','{$listausu2['usucpf']}');\" onmouseout=\"selecionaTR('$linhaX','{$listausu2['usucpf']}');\" style=\"background-color:$cor_td;color:$cor_td\" id=\"cpf_{$listausu2['usucpf']}_dem_1_hr_{$hora[$hrs]}_data_$linha\" ><center>-</center></td>";
							$hrs++;
						}
						$dia_semana--;
						$wk++;
					}
			 	}
				$demandas = pegaDemandas($listausu2['usucpf']); //função que faz as marcações nas TDs criadas									
				echo "</tr>";
				$cor++;
		 	}
		}
	}
	else{
		$contador = 1;
		foreach($listausu as $listausu2){
			($contador % 2)? $corTR = "#fdfdfd" : $corTR = "#f5f5f5";
			$contador++;
		 	echo "<tr style=\"background-color:$corTR;\" onmouseover=\"this.style.backgroundColor='#ffffcc';\" onmouseout=\"this.style.backgroundColor='$corTR'\"   ><td nowrap id=\"tec_{$listausu2['usucpf']}\" >{$listausu2['usunome']}</td>";
		 	$td = 1;
		 	while($td <= $tipo){
			 	//pegar dia da semana
				$dia_s =  dia(date("w",mktime(0,0,0,$mes,(strlen($td) == 1)? "0".$td : $td,$ano)));
				
				($dia_s == "Domingo" || $dia_s == "Sábado")? $cor = "#EEE9E9" : $cor = $corTR;
		 		echo "<td id=\"{$listausu2['usucpf']}_data_".((strlen($td) == 1)? "0".$td : $td)."_{$mes}_{$ano}\"  style=\"text-align:center;background-color:".(($cor == "#EEE9E9")? $cor : $corTR).";color:".(($cor == "#EEE9E9")? $cor : $corTR)."\" >-</td>";
		 		$td++;
		 	}
		 	echo "</tr>";
		 	pegaDemandasMensal($listausu2['usucpf']);
		 	
		}
	}
	echo "</table>";
}


function dia($dia)
{
   switch($dia)
   {
      case "0" : return "Domingo"; break;
      case "1" : return "Segunda-Feira"; break;
      case "2" : return "Terça-Feira"; break;
      case "3" : return "Quarta-Feira"; break;
      case "4" : return "Quinta-Feira"; break;
      case "5" : return "Sexta-Feira"; break;
      case "6" : return "Sábado"; break;
   }
}

function mes($mes)
{
   switch($mes)
   {
      case "01" : return "Janeiro"; break;
      case "02" : return "Fevereiro"; break;
      case "03" : return "Março"; break;
      case "04" : return "Abril"; break;
      case "05" : return "Maio"; break;
      case "06" : return "Junho"; break;
      case "07" : return "Julho"; break;
      case "08" : return "Agosto"; break;
      case "09" : return "Setembro"; break;
      case "10" : return "Outubro"; break;
      case "11" : return "Novembro"; break;
      case "12" : return "Dezembro"; break;
   }
}


function pegaDemandas ($usucpf){
	global $db;
	 $sql = "SELECT dmdid,
	 				priid,
	 				dmddatainiprevatendimento,
	 				dmddatafimprevatendimento
	 		FROM demandas.demanda
	 		WHERE usucpfexecutor = '$usucpf'
	 			AND dmdstatus = 'A'";
	
	 $demandas = $db->carregar($sql);

	 (!$demandas)? $demandas = array(): '';
	 
	 $qtd_dmd = 1;
	 foreach($demandas as $data){
	 	$hr_ini = date('H', strtotime($data['dmddatainiprevatendimento']));
	 	$hr_fim = date('H', strtotime($data['dmddatafimprevatendimento']));
	 	$data_i = date('d_m_Y', strtotime($data['dmddatainiprevatendimento']));
	 	$data_f = date('d_m_Y', strtotime($data['dmddatafimprevatendimento']));
	 	$dmdid = $data['dmdid'];
	 	$priid = $data['priid'];
	 	
	 	switch($priid){
	 		case 1:
	 			$cor = "#32CD32";
	 			break;
	 		case 2:
	 			$cor = "#FFFF00";
	 			break;
	 		case 3:
	 			$cor = "#FF0000";
	 			break;
	 		default:
	 			$cor = "#6495ED";
	 			break;
	 	}
	 	
	 	if($data_i == $data_f){
	 		print "<script>montaGrafico('$hr_ini','$hr_fim','$data_i','$usucpf','$dmdid','$qtd_dmd','$cor');</script>";
	 	}
	 	else{
	 		list($diai,$mesi,$anoi) = explode("_", $data_i);
	 		list($diaf,$mesf,$anof) = explode("_", $data_f);
	 		
	 		$data1 = $anoi.$mesi.$diai;
	 		$data2 = $anof.$mesf.$diaf;	
	 		
	 		print "<script>montaGrafico('$hr_ini','23','$data_i','$usucpf','$dmdid','$qtd_dmd','$cor');</script>";
	 		print "<script>montaGrafico('0','$hr_fim','$data_f','$usucpf','$dmdid','$qtd_dmd','$cor');</script>";
	 			 		
	 		while((int)$data1 < (int)$data2){
	 			$ano = substr($data1, -8,4);
	 			$mes = substr($data1, -4,2);
	 			$dia = substr($data1, -2,2);
	 			print "<script>montaGrafico('$hr_ini','23','".$dia."_".$mes."_".$ano."','$usucpf','$dmdid','$qtd_dmd','$cor');</script>";
	 			$hr_ini = 0;
	 			$data1++;
	 		}
	 	}
	 	$qtd_dmd++;
	 }
	 
	 	return $demandas;
}

function pegaDemandasMensal($usucpf){
	global $db;
	 $sql = "SELECT dmdid,
	 				dmddatainiprevatendimento,
	 				dmddatafimprevatendimento
	 		FROM demandas.demanda
	 		WHERE usucpfexecutor = '$usucpf'
	 			AND dmdstatus = 'A'";
	
	 $demandas = $db->carregar($sql);

	 (!$demandas)? $demandas = array(): '';
	 
	 foreach($demandas as $data){
	 	$data_i = date('d_m_Y', strtotime($data['dmddatainiprevatendimento']));
	 	$data_f = date('d_m_Y', strtotime($data['dmddatafimprevatendimento']));
	 	$dmdid = $data['dmdid'];
	 	
	 	if($data_i == $data_f){
	 		print "<script>montaGraficoMensal('$data_i','$usucpf','$dmdid');</script>";
	 	}
	 	else{
	 		list($diai,$mesi,$anoi) = explode("_", $data_i);
	 		list($diaf,$mesf,$anof) = explode("_", $data_f);
	 		
	 		$data1 = $anoi.$mesi.$diai;
	 		$data2 = $anof.$mesf.$diaf;	
	 		
	 		print "<script>montaGraficoMensal('$data_i','$usucpf','$dmdid');</script>";
	 		print "<script>montaGraficoMensal('$data_f','$usucpf','$dmdid');</script>";
	 			 		
	 		while((int)$data1 < (int)$data2){
	 			$ano = substr($data1, -8,4);
	 			$mes = substr($data1, -4,2);
	 			$dia = substr($data1, -2,2);
	 			print "<script>montaGraficoMensal('".$dia."_".$mes."_".$ano."','$usucpf','$dmdid');</script>";
	 			$data1++;
	 		}
	 	}
	 }
}
?>