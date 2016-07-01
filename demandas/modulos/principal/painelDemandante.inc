<?php
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past

ini_set("memory_limit", "1024M");


if ($_GET['dmdidver']){
	session_start();
	$_SESSION['dmdid'] = $_GET['dmdidver'];
	header( "Location: demandas.php?modulo=principal/dadosDemanda&acao=A" );
	exit();
}

//Ajax pausa
if ($_REQUEST['dmdidPausaAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaPausa($_REQUEST['dmdidPausaAjax']);
	exit;
}


function montaPausa($dmdid){
	global $db;
	
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	$classdata = new Data;

	
				//verifica pausa da demanda
				$sql = "select t.tpadsc, p.pdmdatainiciopausa, p.pdmdatafimpausa, p.pdmjustificativa, to_char(p.pdmdatainiciopausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausaini, to_char(p.pdmdatafimpausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausafim
						from demandas.pausademanda p 
						inner join demandas.tipopausademanda t ON t.tpaid = p.tpaid
						where p.dmdid = ". (int) $dmdid;
						
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
				$sql = "select dmdhorarioatendimento, 
							   to_char(dmddatafimprevatendimento::timestamp,'DD/MM/YYYY HH24:MI') AS dmddatafimprevatendimento,
							   --datahist as dataconc
							   (select to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI')
								 		from 	workflow.historicodocumento a
								 			inner join workflow.comentariodocumento b on a.hstid = b.hstid
								 			inner join workflow.documento c on c.docid = a.docid and c.tpdid in (31,35) 
								 	where aedid in (146, 191) and c.esdid in (93,95,109,111) and a.docid = d.docid
								  ) as dataconc
						from demandas.demanda d
						/*
						LEFT JOIN (  (select a.docid, to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datahist
								 		from 	workflow.historicodocumento a
								 			inner join workflow.comentariodocumento b on a.hstid = b.hstid
								 			inner join workflow.documento c on c.docid = a.docid and c.tpdid in (31,35) 
								 	where aedid in (146, 191) and c.esdid in (93,95,109,111)
								  group by a.docid ) ) as hst ON hst.docid = d.docid
						*/						
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






include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );



$codigo = $_REQUEST['codigo'];
$palavra_chave = $_REQUEST['palavra_chave'];
$mesini = $_REQUEST['mesini'];
$mesfim = $_REQUEST['mesfim'];
$anoini = $_REQUEST['anoini'];
$anofim = $_REQUEST['anofim'];

if(!$mesini || !$mesfim){
	
	$mes = date('m');
	if(strlen($mes) == 1) $mes = "0".$mes;
	
	$ano = date('Y');
	
	if($mes == '01'){
		$mesini = "12";
		$anoini = $ano - 1;
		
		$mesfim = "01";
		$anofim = $ano;
	}
	else{
		$mesini = date('m')-1;
		if(strlen($mesini) == 1) $mesini = "0".$mesini;
		$anoini = $ano;

		$mesfim = date('m');
		if(strlen($mesfim) == 1) $mesfim = "0".$mesfim;
		$anofim = $ano;
	}
}

if(!$_REQUEST['mesini']) $_REQUEST['mesini'] = $mesini;
if(!$_REQUEST['mesfim']) $_REQUEST['mesfim'] = $mesfim;
if(!$_REQUEST['anoini']) $_REQUEST['anoini'] = $anoini;
if(!$_REQUEST['anofim']) $_REQUEST['anofim'] = $anofim;



function dscMes($mes){
	
	$mes = (int) $mes;
	
	switch ($mes) {
    case 1:
        echo "JANEIRO";
        break;
    case 2:
        echo "FEVEREIRO";
        break;
    case 3:
        echo "MARÇO";
        break;
    case 4:
        echo "ABRIL";
        break;
    case 5:
        echo "MAIO";
        break;
    case 6:
        echo "JUNHO";
        break;
    case 7:
        echo "JULHO";
        break;
    case 8:
        echo "AGOSTO";
        break;
    case 9:
        echo "SETEMBRO";
        break;
    case 10:
        echo "OUTUBRO";
        break;
    case 11:
        echo "NOVEMBRO";
        break;
    case 12:
        echo "DEZEMBRO";
        break;
	}    
}


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
			 <script type="text/javascript" src="../includes/prototype.js"></script>
			 <script type="text/javascript" src="../includes/funcoes.js"></script>
			 <script type="text/javascript" src="../includes/remedial.js"></script>
			 <script type="text/javascript" src="../includes/superTitle.js"></script>
			 <script language="JavaScript" src="../includes/wz_tooltip.js"></script>
			 
			 <link rel="stylesheet" href="/includes/LyteBox/lytebox.css" type="text/css" media="screen" />
			 <script type="text/javascript" src="/includes/LyteBox/lytebox.js" ></script>
	</head>
<body>

<form action="" method="POST" name="formulario">
		
	<input type="hidden" name="tipoconsulta" value="">	
		
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="0">
		<tr>
			<td style="background-color:#e9e9e9; color:#404040;" >
			
			  <div style="float: left;position:relative;top:17px;left:70px">
			  	<img src="../imagens/obras/incluir.png" style="width: 20px; vertical-align: bottom;"/>
			  	<input style="cursor:pointer;" title="Clique aqui para cadastrar demandas." type="button" onclick="location.href='demandas.php?modulo=principal/cadDemanda&acao=A';" name="cad" value="Cadastrar Demandas"/>
			  </div>
			  
			  <table align="center" border="0"  cellpadding="0" cellspacing="0" >
			  	<tr>
			  		<td style="background-color:#e9e9e9; color:#404040;">
			  		
						<table border="0"  cellpadding="3" cellspacing="0">
							<tr>
								<td width="10%">
									<B>Código:</B>
									<br>
									<?= campo_texto( 'codigo', 'N', 'S', '', 10, 15, '##########', '', '','','','','buscaCodigo(event)' ); ?>
								</td>							
								<td>
									&nbsp;<B>Assunto / Descrição:</B>
									<br>
									<?= campo_texto( 'palavra_chave', 'N', 'S', '', 40, 40, '', '', '', '', '', 'id="palavra_chave"', '', ''); ?>
								</td>	
								<td nowrap="nowrap">
									<B>Período:</B>
									<br>
									<?php 
										$sql = "select '01' as codigo, 'Janeiro' as descricao
  					   						union all
  					   						select '02' as codigo, 'Fevereiro' as descricao
  					   						union all
  					   						select '03' as codigo, 'Março' as descricao
  					   						union all
  					   						select '04' as codigo, 'Abril' as descricao
  					   						union all
  					   						select '05' as codigo, 'Maio' as descricao
  					   						union all
  					   						select '06' as codigo, 'Junho' as descricao
  					   						union all
  					   						select '07' as codigo, 'Julho' as descricao
  					   						union all
  					   						select '08' as codigo, 'Agosto' as descricao
  					   						union all
  					   						select '09' as codigo, 'Setembro' as descricao
  					   						union all
  					   						select '10' as codigo, 'Outubro' as descricao
  					   						union all
  					   						select '11' as codigo, 'Novembro' as descricao
  					   						union all
  					   						select '12' as codigo, 'Dezembro' as descricao
  					   						";
									
										$db->monta_combo( 'mesini', $sql, 'S', '--','', '' );	
										/*
										$sql = "select '2010' as codigo, '2010' as descricao
  						   						union all
  						   						select '2011' as codigo, '2011' as descricao
  						   						";
										$db->monta_combo( 'anoini', $sql, 'S', '--','', '' );
										*/
										$aini = (int) 2010;
										$afim = (int) date("Y");
										
										for($i=$aini; $i<=$afim; $i++){
											$sqlai .= " select '$i' as codigo, '$i' as descricao ";
											if($i != $afim) $sqlai .= " union ";
										}
										
										$sqlai .= " order by 1 desc ";
										
										$db->monta_combo( 'anoini', $sqlai, 'S', '--','', '' );										
									?>
									&nbsp;&nbsp;
									até
									&nbsp;&nbsp;
									<?php 
										$sql = "select '01' as codigo, 'Janeiro' as descricao
  					   						union all
  					   						select '02' as codigo, 'Fevereiro' as descricao
  					   						union all
  					   						select '03' as codigo, 'Março' as descricao
  					   						union all
  					   						select '04' as codigo, 'Abril' as descricao
  					   						union all
  					   						select '05' as codigo, 'Maio' as descricao
  					   						union all
  					   						select '06' as codigo, 'Junho' as descricao
  					   						union all
  					   						select '07' as codigo, 'Julho' as descricao
  					   						union all
  					   						select '08' as codigo, 'Agosto' as descricao
  					   						union all
  					   						select '09' as codigo, 'Setembro' as descricao
  					   						union all
  					   						select '10' as codigo, 'Outubro' as descricao
  					   						union all
  					   						select '11' as codigo, 'Novembro' as descricao
  					   						union all
  					   						select '12' as codigo, 'Dezembro' as descricao
  					   						";
									
										$db->monta_combo( 'mesfim', $sql, 'S', '--','', '' );
										/*
										$sql = "select '2010' as codigo, '2010' as descricao
  						   						union all
  						   						select '2011' as codigo, '2011' as descricao
  						   						";
										$db->monta_combo( 'anofim', $sql, 'S', '--','', '' );										
										*/
										$aini = (int) 2010;
										$afim = (int) date("Y");
										
										for($i=$aini; $i<=$afim; $i++){
											$sqlaf .= " select '$i' as codigo, '$i' as descricao ";
											if($i != $afim) $sqlaf .= " union ";
										}
										
										$sqlaf .= " order by 1 desc ";
										
										$db->monta_combo( 'anofim', $sqlaf, 'S', '--','', '' );										
									?>		
															
												
								</td>
								<td nowrap="nowrap">
									<br>
									&nbsp;&nbsp;
									<input style="cursor:pointer;" type="button" onclick="pesquisa();" name="pesquisar" value="Pesquisar"/>
									&nbsp;
									<input style="cursor:pointer;" type="button" onclick="limpa();" name="limpar" value="Limpar"/>
								</td>
							</tr>
							</table>
							
						</td>
					</tr>
					</table>
			  		
			  		
			  	</td>
			  </tr>
			  <tr>
				<td align=center style="background-color:#e9e9e9;">
					<FONT COLOR=#00008B><B>PERÍODO: <?=dscMes($mesini)?>/<?=$anoini?> ATÉ <?=dscMes($mesfim)?>/<?=$anofim?></B></FONT> 
				</td>
			  </tr>
			</table>	
			  
			  
			<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"> 
				<tr>
					<th width ="50%">				
						<font color="#00008B">Em Atendimento</font>				
					</th>
					<th>
						<font color="#00008B">Minhas Avaliações</font>	
					</th>
				</tr>
				<tr>
					<td valign="top">
						<div style="margin: 0 auto; padding: 0; height: 200px; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="atendimento">
							<?=painelDemandante('atendimento')?>
						</div>
					</td>
					<td valign="top">
						<div style="margin: 0 auto; padding: 0; height: 200px; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="finalizadas">
							<?=painelDemandante('avaliacoes')?>
						</div>
					</td>
				</tr>
			</table>
		
			<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"> 
				<tr>
					<th width ="50%">				
						<font color="#00008B">Finalizadas</font>				
					</th>
					<th>
						<font color="#00008B"><div id="divtotal">Meu Consumo (R$ 0,00)</div></font>	
					</th>
				</tr>
				<tr>
					<td valign="top">
						<div style="margin: 0 auto; padding: 0; height: 200px; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="avaliacoes">
							<?=painelDemandante('finalizadas')?>
						</div>
					</td>
					<td valign="top">
						<div style="margin: 0 auto; padding: 0; height: 200px; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="consumo">
							<?=painelDemandante('consumo')?>
						</div>
					</td>
				</tr>
			</table>
	
			  
			  
			 	
</form>
</body>
</html>

<?php


function painelDemandante($tipoconsulta){
	
	global $db;
	
	$and = " and d.dmdidorigem is null ";
	
	if($tipoconsulta == "atendimento"){
		$atendimento   =  true;
		$and .= " and (doc.esdid is null or doc.esdid in(91,107,92,108) ) ";
	}
	elseif($tipoconsulta == "finalizadas"){
		$finalizadas   =  true;
		$and .= " and doc.esdid in(93,111,95,109,170,135,136,100,110) ";
	}
	elseif($tipoconsulta == "avaliacoes"){
		$avaliacoes   =  true;
		$and .= " and doc.esdid in(93,111,95,109,170) ";
		$orderby = " avaliacao desc, ";
	}
	elseif($tipoconsulta == "consumo"){
		$consumo 	= true;
		$and .= " and doc.esdid in(93,111,95,109,170) ";
	}	
	
	$codigo 		= (isset($_POST["codigo"]) && $_POST["codigo"]!="") ? $_POST["codigo"] : NULL;
	$palavra_chave 	= (isset($_POST["palavra_chave"]) && $_POST["palavra_chave"]!="") ? $_POST["palavra_chave"] : NULL;
	$mesini 		= (isset($_POST["mesini"]) && $_POST["mesini"]!="") ? $_POST["mesini"] : $_REQUEST["mesini"];
	$mesfim 		= (isset($_POST["mesfim"]) && $_POST["mesfim"]!="") ? $_POST["mesfim"] : $_REQUEST["mesfim"];
	$anoini			= (isset($_POST["anoini"]) && $_POST["anoini"]!="") ? $_POST["anoini"] : $_REQUEST["anoini"];
	$anofim			= (isset($_POST["anofim"]) && $_POST["anofim"]!="") ? $_POST["anofim"] : $_REQUEST["anofim"];
	
	if ($codigo){
		$codigo = str_replace("#","",$codigo);
		$and .= " and d.dmdid = {$codigo}";
	}
	if ($palavra_chave){
		$and .= " and (d.dmdtitulo ILIKE '%{$palavra_chave}%' OR d.dmddsc ILIKE '%{$palavra_chave}%')";
	}	
	
	if ($mesini && $mesfim){
		$lastday = date('t',strtotime($anofim.'-'.$mesfim.'-01'));
		$and .= " and d.dmddatainclusao between '$anoini/$mesini/01 00:00:00' and '$anofim/$mesfim/$lastday 23:59:59'";
	}
	elseif($tipoconsulta != "atendimento"){

		$dia = date('d');
		if(strlen($dia) == 1) $dia = "0".$dia;
		
		$mes = date('m');
		if(strlen($mes) == 1) $mes = "0".$mes;
		
		$ano = date('Y');
		
		if($mes == '01'){
			$mesini = "12";
			$anoini = $ano - 1;
			
			$mesfim = $mes;
			$anofim = $ano;
			
			
		}
		else{
			$mesini = date('m')-1;
			if(strlen($mesini) == 1) $mesini = "0".$mesini;
			$anoini = $ano;

			$mesfim = date('m');
			if(strlen($mesfim) == 1) $mesfim = "0".$mesfim;
			$anofim = $ano;
		}
		
		$and .= " and d.dmddatainclusao between '$anoini/$mesini/01 00:00:00' and '$anofim/$mesfim/$dia 23:59:59'";
	}	
	
	
	
	if($tipoconsulta == "atendimento"){
		
		$sql = "

				SELECT
					 DISTINCT
					 '<a style=\'color:#0066CC\'  href=\'demandas.php?modulo=principal/painelDemandante&acao=A&dmdidver=' || d.dmdid || '\'>'||d.dmdid||'</a>' AS id,
					 
					 (CASE 
					 	--WHEN dmdidpausa > 0 AND ( dttempopausa is null OR dttempopausa > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) THEN
					 	WHEN ( select count(pp.dmdid) 
					 			 from demandas.pausademanda pp
 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35) 
								 where pp.dmdid = d.dmdid) > 0 
						AND ( ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') 
					 			 from demandas.pausademanda pp
 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35) 
 								 where pp.dmdid = d.dmdid
								 group by dd.dmddatafimprevatendimento) is null 
						OR ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') 
					 			 from demandas.pausademanda pp
 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35) 
 								 where pp.dmdid = d.dmdid
								 group by dd.dmddatafimprevatendimento) > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) 
						THEN
							'<a href=\'javascript:void(0);\'><img src=\'../imagens/pause.gif\' border=0  title=\' \' align=\'absmiddle\'  onmouseout=\'SuperTitleOff(this);\' onmousemove=\"SuperTitleAjax(\'demandas.php?modulo=principal/painelDemandante&acao=A&dmdidPausaAjax='|| d.dmdid ||'\',this);\"></a>'
						ELSE
							''	
					 END) || d.dmdtitulo  AS assunto,
					 					 
					 orddescricao AS origemdemanda,
					 
					 CASE
					  WHEN d.docid IS NOT NULL THEN 
						  CASE esddsc WHEN 'Cancelada' THEN '<span style=\'color:red;\'>' || esddsc || '</span>'
						  	ELSE  esddsc
						  END
					  ELSE '<span style=\'color:blue;\' title=\'Em Processamento\'>Em Processamento</span>'
					 END AS situacao,
					 
					  CASE
					  	WHEN u2.usucpf != '' THEN upper(u2.usunome)
					  	ELSE '<span style=\'color:red;\' title=\'Não Atribuído\'>N/A</span>'
					  END AS tecnicoresponsavel,
					
					 to_char(d.dmddatainclusao, 'DD/MM/YYYY HH24:MI') AS dataabertura,
					 
					 to_char(d.dmddatafimprevatendimento, 'DD/MM/YYYY HH24:MI') AS dataprevisaotermino,
					 
					d.dmdid,

					d.dmdidorigem
					 
				FROM
				
					 demandas.demanda d
					 /*
					 LEFT JOIN ( select pp.dmdid, count(pp.dmdid) as dmdidpausa, to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') as dttempopausa 
					 			 from demandas.pausademanda pp
 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35) 
								 group by dd.dmddatafimprevatendimento,pp.dmdid) ps ON ps.dmdid = d.dmdid
					 */
					 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
					 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
					 LEFT JOIN demandas.sistemadetalhe sis ON sis.sidid = d.sidid
					 LEFT JOIN demandas.sistemacelula sis_c ON sis_c.sicid = sis.sidid
					 LEFT JOIN demandas.celula cel ON cel.celid = sis_c.celid
					 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
					 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
					 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
					WHERE 
					 d.dmdstatus = 'A'	
					 AND d.usucpfdemandante = '".$_SESSION['usucpf']."'	
					 $and	 
					ORDER BY $orderby d.dmdid DESC limit 50";
		
	}
	elseif($tipoconsulta == "finalizadas"){

		$sql = "

				SELECT
					 DISTINCT
					 '<a style=\'color:#0066CC\'  href=\'demandas.php?modulo=principal/painelDemandante&acao=A&dmdidver=' || d.dmdid || '\'>'||d.dmdid||'</a>' AS id,
					 
					 d.dmdtitulo  AS assunto,
					 					 
					 orddescricao AS origemdemanda,
					 
					 CASE
					  WHEN d.docid IS NOT NULL THEN 
						  CASE esddsc WHEN 'Cancelada' THEN '<span style=\'color:red;\'>' || esddsc || '</span>'
						  	ELSE  esddsc
						  END
					  ELSE '<span style=\'color:blue;\' title=\'Em Processamento\'>Em Processamento</span>'
					 END AS situacao,
					 
					  CASE
					  	WHEN u2.usucpf != '' THEN upper(u2.usunome)
					  	ELSE '<span style=\'color:red;\' title=\'Não Atribuído\'>N/A</span>'
					  END AS tecnicoresponsavel,
					
					 to_char(d.dmddatainclusao, 'DD/MM/YYYY HH24:MI') AS dataabertura,
					 
					 --datahist as dataconclusao,
					 
					 (select to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') 
					  from workflow.historicodocumento a
					  where a.aedid in (146, 191) and doc.esdid in (93,95,109,111,135,136,170) and a.docid = doc.docid
					 ) as dataconclusao,
					 
					 d.dmdid,

					 d.dmdidorigem
					 
				FROM
				
					 demandas.demanda d
					 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
					 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
					 LEFT JOIN demandas.sistemadetalhe sis ON sis.sidid = d.sidid
					 LEFT JOIN demandas.sistemacelula sis_c ON sis_c.sicid = sis.sidid
					 LEFT JOIN demandas.celula cel ON cel.celid = sis_c.celid
					 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
					 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
					 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
					 /*
					 LEFT JOIN (  (select a.docid, to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datahist
								 		from 	workflow.historicodocumento a
								 			inner join workflow.documento c on c.docid = a.docid and c.tpdid in (31,35)
								 	where aedid in (146, 191) and c.esdid in (93,95,109,111,135,136,170)
								  group by a.docid ) ) as hst ON hst.docid = d.docid	
					*/
					WHERE 
					 d.dmdstatus = 'A'	
					 AND d.usucpfdemandante = '".$_SESSION['usucpf']."'	
					 $and	 
					ORDER BY $orderby d.dmdid DESC limit 50";
		
	}
	elseif($tipoconsulta == "avaliacoes"){

		$sql = "

				SELECT
					 DISTINCT
					 '<a style=\'color:#0066CC\'  href=\'demandas.php?modulo=principal/painelDemandante&acao=A&dmdidver=' || d.dmdid || '\'>'||d.dmdid||'</a>' AS id,
					 
					 d.dmdtitulo  AS assunto,
					 					 
					 orddescricao AS origemdemanda,
					 
					 CASE
					  WHEN d.docid IS NOT NULL THEN 
						  CASE esddsc WHEN 'Cancelada' THEN '<span style=\'color:red;\'>' || esddsc || '</span>'
						  	ELSE  esddsc
						  END
					  ELSE '<span style=\'color:blue;\' title=\'Em Processamento\'>Em Processamento</span>'
					 END AS situacao,
					 
					  CASE
					  	WHEN u2.usucpf != '' THEN upper(u2.usunome)
					  	ELSE '<span style=\'color:red;\' title=\'Não Atribuído\'>N/A</span>'
					  END AS tecnicoresponsavel,
					
					--datahist as dataconclusao,
					
					(select to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') 
					  from workflow.historicodocumento a
					  where a.aedid in (146, 191) and doc.esdid in (93,95,109,111,135,136,170) and a.docid = doc.docid
					 ) as dataconclusao,
					 
					CASE WHEN ctavaliacao > 0 THEN
							'<font color=blue><b>SIM</b></font>'
						 ELSE
						 	'<font color=red><b>NÃO</b></font>'
					END AS  avaliacao,
					
					d.dmdid,

					d.dmdidorigem
					 
				FROM
				
					 demandas.demanda d
					 LEFT JOIN (select dmdid,  count(dmdid) as ctavaliacao from demandas.avaliacaodemanda group by dmdid) avd ON avd.dmdid = d.dmdid
					 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
					 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
					 LEFT JOIN demandas.sistemadetalhe sis ON sis.sidid = d.sidid
					 LEFT JOIN demandas.sistemacelula sis_c ON sis_c.sicid = sis.sidid
					 LEFT JOIN demandas.celula cel ON cel.celid = sis_c.celid
					 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
					 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
					 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
					 
					 /*
					 LEFT JOIN (  (select a.docid, to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datahist
								 		from 	workflow.historicodocumento a
								 			inner join workflow.documento c on c.docid = a.docid and c.tpdid in (31,35)
								 	where aedid in (146, 191) and c.esdid in (93,95,109,111,135,136,170)
								  group by a.docid ) ) as hst ON hst.docid = d.docid
					 */	
					WHERE 
					 d.dmdstatus = 'A'	
					 AND d.usucpfdemandante = '".$_SESSION['usucpf']."'	
					 $and	 
					ORDER BY $orderby d.dmdid DESC limit 50";
		
	}
	elseif($tipoconsulta == "consumo"){

			$sql = "

				SELECT
					 DISTINCT
					 '<a style=\'color:#0066CC\'  href=\'demandas.php?modulo=principal/painelDemandante&acao=A&dmdidver=' || d.dmdid || '\'>'||d.dmdid||'</a>' AS id,
					 
					 d.dmdtitulo  AS assunto,
					 					 
					 orddescricao AS origemdemanda,
					 
					 CASE
					  WHEN d.docid IS NOT NULL THEN 
						  CASE esddsc WHEN 'Cancelada' THEN '<span style=\'color:red;\'>' || esddsc || '</span>'
						  	ELSE  esddsc
						  END
					  ELSE '<span style=\'color:blue;\' title=\'Em Processamento\'>Em Processamento</span>'
					 END AS situacao,
					 
					CASE WHEN od.ordid = 3 and esddsc <> 'Cancelada' THEN
					
							 (cast (pt.tsppontuacao as bigint)*
							 (CASE WHEN d.dmdqtde > 0 THEN 
											   d.dmdqtde
								       ELSE
											    1          
							 END)) * COALESCE(crtvlponto,0)
							 
						ELSE
							0
					END	as valor,
					
					d.dmdid,

					d.dmdidorigem
					 
				FROM
				
					 demandas.demanda d
					 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
					 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
					 LEFT JOIN demandas.sistemadetalhe sis ON sis.sidid = d.sidid
					 LEFT JOIN demandas.sistemacelula sis_c ON sis_c.sicid = sis.sidid
					 LEFT JOIN demandas.celula cel ON cel.celid = sis_c.celid
					 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
					 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
					 LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid and pt.priid = d.priid and pt.tspstatus = 'A'
					 --LEFT JOIN (select crtvlponto, crtdtinicio, crtdtfim from demandas.contrato where crtstatus='A') as con on d.dmddatainclusao between con.crtdtinicio and con.crtdtfim
			 		 --LEFT JOIN demandas.contrato con on (od.ordid = con.ordid and con.crtstatus='A' and d.dmddatainclusao between con.crtdtinicio and con.crtdtfim)
			 		LEFT JOIN (select crtvlponto, crtdtinicio, crtdtfim, ordid from demandas.contrato where crtstatus='A') as con on d.dmddatainclusao between con.crtdtinicio and con.crtdtfim and od.ordid = con.ordid
					WHERE 
					 d.dmdstatus = 'A'	
					 AND d.usucpfdemandante = '".$_SESSION['usucpf']."'	
					 $and	 
					
					group by crtvlponto, d.dmdid, d.dmdtitulo, od.orddescricao, d.docid, ed.esddsc, od.ordid, pt.tsppontuacao, d.dmdqtde, d.dmdidorigem
					
					ORDER BY $orderby d.dmdid DESC 
					
					limit 50";
		
	}	
	

		//dbg($sql);
				 
	//ver($sql);
	
	if($sql) $RS = $db->carregar($sql);

	

?>


		<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr>
				<td align="left" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Código</strong>
				<td align="left" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Assunto</strong>
				<td align="left" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Origem / Tipo</strong>
				<td align="left" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Situação</strong>
				<?if(!$consumo){?>
					<td align="left" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Técnico Responsável</strong>
				<?}?>
				<?if(!$avaliacoes  && !$consumo){?>
					<td align="left" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Data Abertura</strong>
				<?}?>
				<?if($atendimento){?>
					<td align="left" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Data Prevista de Término</strong>
				<?}?>
				<?if($finalizadas || $avaliacoes){?>
					<td align="left" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Data Conclusão</strong>
				<?}?>
				<?if($consumo){?>
					<td align="center" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Valor da Demanda (R$)</strong>
				<?}?>
				<?if($avaliacoes){?>
					<td align="center" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Avaliação</strong>
				<?}?>
			</tr>
			
				<? if ( $RS ) : ?>
				
					<?$cor = ''; ?>
					<?$ct = 0; ?>
					<?foreach ( $RS as $dado ) : 
					
						$cor = $cor == '#F7F7F7' ? '#ffffff' : '#F7F7F7' ;
						?>
						<tr bgcolor="<?=$cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$cor ?>';">
							<td align="left" nowrap><?=$dado['id']?></td>
							<td ><?=$dado['assunto']?></td>
							<td ><?=$dado['origemdemanda'] ?></td>
							<td ><?=$dado['situacao'] ?></td>
							<?if(!$consumo){?>
								<td ><?=$dado['tecnicoresponsavel'] ?></td>
							<?}?>
							<?if(!$avaliacoes && !$consumo){?>
								<td ><?=$dado['dataabertura'] ?></td>
							<?}?>
							<?if($atendimento){?>
								<td ><?=$dado['dataprevisaotermino'] ?></td>
							<?}?>	
							<?if($finalizadas || $avaliacoes){?>
								<td ><?=$dado['dataconclusao'] ?></td>
							<?}?>	
							<?if($consumo){?>
								<td align="center"><?=number_format($dado['valor'], 2, ',', '.')?></td>
							<?}?>
							<?if($avaliacoes){?>
								<td align="center"><a title="Clique para avaliar esta demanda." href="javascript:buscarAval(<?=$dado['dmdid']?>);"><div id="div<?=$dado['dmdid']?>"><?=$dado['avaliacao'] ?></div></a></td>
							<?}?>
						</tr>
						<? 
							$Pai = $dado['dmdid'];
							$filhos = dmdFilhoSub($Pai,0,1,$cor,$tipoconsulta);
							echo $filhos;
						?>
					<?endforeach;?>
					
					<?if($consumo){
						$and = str_replace("and d.dmdidorigem is null", "", $and);
						$sqlTotal = " select 
											(sum(cast (pt.tsppontuacao as bigint)*
											 (CASE WHEN d.dmdqtde > 0 THEN 
															   d.dmdqtde
												       ELSE
															    1          
											 END)) * COALESCE(crtvlponto,0) ) 
											 as valortotal
									 from demandas.demanda d
									 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
									 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
									 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
									 LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid and pt.priid = d.priid and pt.tspstatus = 'A'
									 LEFT JOIN (select crtvlponto, crtdtinicio, crtdtfim from demandas.contrato where crtstatus='A') as con on d.dmddatainclusao between con.crtdtinicio and con.crtdtfim
									 --LEFT JOIN demandas.contrato con on (od.ordid = con.ordid and con.crtstatus='A' and d.dmddatainclusao between con.crtdtinicio and con.crtdtfim)
									 where d.dmdstatus = 'A' and od.ordid = 3 -- suporte atendimento
									 $and
									 AND d.usucpfdemandante = '".$_SESSION['usucpf']."'	
									 group by crtvlponto";
									 
						//ver($sqlTotal);
						
						$valortotal = $db->pegaUm($sqlTotal);
						if(!$valortotal) $valortotal = 0;
						?>
						<tr>
							<td align="right" colspan="4"><B>TOTAL:<B/></td>
							<td align="center"><?=number_format($valortotal, 2, ',', '.')?></td>
						</tr>
						<script>
							document.getElementById('divtotal').innerHTML = "Meu Consumo (R$ <?=number_format($valortotal, 2, ',', '.')?>)";
						</script>
					<?}?>
					
				<?php else : ?>
					<tr>
						<td colspan=29 style="text-align:center; padding:15px; background-color:#fafafa; color:red; font-weight:bold; font-size: 10px;" >
							Não existem demandas.
						</td>
					</tr>
				<?php endif; ?>
		</table>

<?php }?>


<?php 
function dmdFilhoSub ($dmdid,$width,$profundidade = null,$cor = null,$tipoconsulta = null){
	global $db;

	/*
	if($profundidade == 1){
		$sql = "select dm.dmdid, dm.dmdtitulo from demandas.demanda dm where dm.dmdid = $dmdid order by dmdid desc";
		$dadosDemanda = $db->carregar($sql);
		$caminho = $_SERVER ['REQUEST_URI'];
		$caminho = explode("&dmdid=",$caminho);
		$caminho = $caminho[0]."&dmdid={$dadosDemanda[0]['dmdid']}";
		($_SESSION['dmdid'] == $dadosDemanda[0]['dmdid'])? $cor = "font-weight:bold" : $cor="";
		$tr_filhos .= "<div style=\"text-align: left;background: rgb(238, 238, 238);$cor\" ><a href=\"$caminho\"> Cód. # {$dadosDemanda[0]['dmdid']} -  {$dadosDemanda[0]['dmdtitulo']}</a></div>";
	}
	*/
	($profundidade)? $profundidade++ : $profundidade = $profundidade;
	//$sql="select dm.dmdid, dm.dmdtitulo from demandas.demanda dm where dm.dmdidorigem = $dmdid order by dmdid desc";
	
	if($tipoconsulta == "atendimento"){
		
		$sql = "

				SELECT
					 DISTINCT
					 '<a style=\'color:#0066CC\'  href=\'demandas.php?modulo=principal/painelDemandante&acao=A&dmdidver=' || d.dmdid || '\'>'||d.dmdid||'</a>' AS id,
					 
					 (CASE 
					 	--WHEN dmdidpausa > 0 AND ( dttempopausa is null OR dttempopausa > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) THEN
					 	WHEN ( select count(pp.dmdid) 
					 			 from demandas.pausademanda pp
 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35) 
								 where pp.dmdid = d.dmdid) > 0 
						AND ( ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') 
					 			 from demandas.pausademanda pp
 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35)
 								 where pp.dmdid = d.dmdid 
								 group by dd.dmddatafimprevatendimento) is null 
						OR ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') 
					 			 from demandas.pausademanda pp
 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35)
 								 where pp.dmdid = d.dmdid 
								 group by dd.dmddatafimprevatendimento) > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) 
						THEN
							'<a href=\'javascript:void(0);\'><img src=\'../imagens/pause.gif\' border=0  title=\' \' align=\'absmiddle\'  onmouseout=\'SuperTitleOff(this);\' onmousemove=\"SuperTitleAjax(\'demandas.php?modulo=principal/painelDemandante&acao=A&dmdidPausaAjax='|| d.dmdid ||'\',this);\"></a>'
						ELSE
							''	
					 END) || d.dmdtitulo  AS assunto,
					 					 
					 orddescricao AS origemdemanda,
					 
					 CASE
					  WHEN d.docid IS NOT NULL THEN 
						  CASE esddsc WHEN 'Cancelada' THEN '<span style=\'color:red;\'>' || esddsc || '</span>'
						  	ELSE  esddsc
						  END
					  ELSE '<span style=\'color:blue;\' title=\'Em Processamento\'>Em Processamento</span>'
					 END AS situacao,
					 
					  CASE
					  	WHEN u2.usucpf != '' THEN upper(u2.usunome)
					  	ELSE '<span style=\'color:red;\' title=\'Não Atribuído\'>N/A</span>'
					  END AS tecnicoresponsavel,
					
					 to_char(d.dmddatainclusao, 'DD/MM/YYYY HH24:MI') AS dataabertura,
					 
					 to_char(d.dmddatafimprevatendimento, 'DD/MM/YYYY HH24:MI') AS dataprevisaotermino,
					 
					d.dmdid,

					d.dmdidorigem
					 
				FROM
				
					 demandas.demanda d
					 /*
					 LEFT JOIN ( select pp.dmdid, count(pp.dmdid) as dmdidpausa, to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') as dttempopausa 
					 			 from demandas.pausademanda pp
 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35) 
								 group by dd.dmddatafimprevatendimento,pp.dmdid) ps ON ps.dmdid = d.dmdid
					 */
					 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
					 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
					 LEFT JOIN demandas.sistemadetalhe sis ON sis.sidid = d.sidid
					 LEFT JOIN demandas.sistemacelula sis_c ON sis_c.sicid = sis.sidid
					 LEFT JOIN demandas.celula cel ON cel.celid = sis_c.celid
					 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
					 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
					 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
					WHERE 
					 d.dmdstatus = 'A'	
					 and d.dmdidorigem = $dmdid
					 AND d.usucpfdemandante = '".$_SESSION['usucpf']."'		 
					ORDER BY d.dmdid DESC ";
	}
	elseif($tipoconsulta == "finalizadas"){
		
		$sql = "

				SELECT
					 DISTINCT
					 '<a style=\'color:#0066CC\'  href=\'demandas.php?modulo=principal/painelDemandante&acao=A&dmdidver=' || d.dmdid || '\'>'||d.dmdid||'</a>' AS id,
					 
					 d.dmdtitulo  AS assunto,
					 					 
					 orddescricao AS origemdemanda,
					 
					 CASE
					  WHEN d.docid IS NOT NULL THEN 
						  CASE esddsc WHEN 'Cancelada' THEN '<span style=\'color:red;\'>' || esddsc || '</span>'
						  	ELSE  esddsc
						  END
					  ELSE '<span style=\'color:blue;\' title=\'Em Processamento\'>Em Processamento</span>'
					 END AS situacao,
					 
					  CASE
					  	WHEN u2.usucpf != '' THEN upper(u2.usunome)
					  	ELSE '<span style=\'color:red;\' title=\'Não Atribuído\'>N/A</span>'
					  END AS tecnicoresponsavel,
					
					 to_char(d.dmddatainclusao, 'DD/MM/YYYY HH24:MI') AS dataabertura,
					 
					--datahist as dataconclusao,
					
					(select to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') 
					  from workflow.historicodocumento a
					  where a.aedid in (146, 191) and doc.esdid in (93,95,109,111,135,136,170) and a.docid = doc.docid
					 ) as dataconclusao,
					 
					d.dmdid,

					d.dmdidorigem
					 
				FROM
				
					 demandas.demanda d
					 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
					 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
					 LEFT JOIN demandas.sistemadetalhe sis ON sis.sidid = d.sidid
					 LEFT JOIN demandas.sistemacelula sis_c ON sis_c.sicid = sis.sidid
					 LEFT JOIN demandas.celula cel ON cel.celid = sis_c.celid
					 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
					 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
					 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
					 /*
					 LEFT JOIN (  (select a.docid, to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datahist
								 		from 	workflow.historicodocumento a
								 			inner join workflow.documento c on c.docid = a.docid and c.tpdid in (31,35)
								 	where aedid in (146, 191) and c.esdid in (93,95,109,111,135,136,170)
								  group by a.docid ) ) as hst ON hst.docid = d.docid
					 */	
					WHERE 
					 d.dmdstatus = 'A'	
					 and d.dmdidorigem = $dmdid
					 AND d.usucpfdemandante = '".$_SESSION['usucpf']."'		 
					ORDER BY d.dmdid DESC ";
	}
	elseif($tipoconsulta == "avaliacoes"){
		
		$sql = "

				SELECT
					 DISTINCT
					 '<a style=\'color:#0066CC\'  href=\'demandas.php?modulo=principal/painelDemandante&acao=A&dmdidver=' || d.dmdid || '\'>'||d.dmdid||'</a>' AS id,
					 
					 d.dmdtitulo  AS assunto,
					 					 
					 orddescricao AS origemdemanda,
					 
					 CASE
					  WHEN d.docid IS NOT NULL THEN 
						  CASE esddsc WHEN 'Cancelada' THEN '<span style=\'color:red;\'>' || esddsc || '</span>'
						  	ELSE  esddsc
						  END
					  ELSE '<span style=\'color:blue;\' title=\'Em Processamento\'>Em Processamento</span>'
					 END AS situacao,
					 
					  CASE
					  	WHEN u2.usucpf != '' THEN upper(u2.usunome)
					  	ELSE '<span style=\'color:red;\' title=\'Não Atribuído\'>N/A</span>'
					  END AS tecnicoresponsavel,
					
					--datahist as dataconclusao,
					
					(select to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') 
					  from workflow.historicodocumento a
					  where a.aedid in (146, 191) and doc.esdid in (93,95,109,111,135,136,170) and a.docid = doc.docid
					 ) as dataconclusao,
					 
					CASE WHEN ctavaliacao > 0 THEN
							'<font color=blue><b>SIM</b></font>'
						 ELSE
						 	'<font color=red><b>NÃO</b></font>'
					END AS  avaliacao,
					
					d.dmdid,

					d.dmdidorigem
					 
				FROM
				
					 demandas.demanda d
					 LEFT JOIN (select dmdid,  count(dmdid) as ctavaliacao from demandas.avaliacaodemanda group by dmdid) avd ON avd.dmdid = d.dmdid
					 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
					 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
					 LEFT JOIN demandas.sistemadetalhe sis ON sis.sidid = d.sidid
					 LEFT JOIN demandas.sistemacelula sis_c ON sis_c.sicid = sis.sidid
					 LEFT JOIN demandas.celula cel ON cel.celid = sis_c.celid
					 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
					 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
					 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
					 /*
					 LEFT JOIN (  (select a.docid, to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datahist
								 		from 	workflow.historicodocumento a
								 			inner join workflow.documento c on c.docid = a.docid and c.tpdid in (31,35)
								 	where aedid in (146, 191) and c.esdid in (93,95,109,111,135,136,170)
								  group by a.docid ) ) as hst ON hst.docid = d.docid
					 */	
					WHERE 
					 d.dmdstatus = 'A'	
					 and d.dmdidorigem = $dmdid
					 AND d.usucpfdemandante = '".$_SESSION['usucpf']."'		 
					ORDER BY d.dmdid DESC ";
	}
	elseif($tipoconsulta == "consumo"){
		
		$sql = "

				SELECT
					 DISTINCT
					 '<a style=\'color:#0066CC\'  href=\'demandas.php?modulo=principal/painelDemandante&acao=A&dmdidver=' || d.dmdid || '\'>'||d.dmdid||'</a>' AS id,
					 
					 d.dmdtitulo  AS assunto,
					 					 
					 orddescricao AS origemdemanda,
					 
					 CASE
					  WHEN d.docid IS NOT NULL THEN 
						  CASE esddsc WHEN 'Cancelada' THEN '<span style=\'color:red;\'>' || esddsc || '</span>'
						  	ELSE  esddsc
						  END
					  ELSE '<span style=\'color:blue;\' title=\'Em Processamento\'>Em Processamento</span>'
					 END AS situacao,
					 
					CASE WHEN od.ordid = 3 and esddsc <> 'Cancelada' THEN
					
							 (cast (pt.tsppontuacao as bigint)*
							 (CASE WHEN d.dmdqtde > 0 THEN 
											   d.dmdqtde
								       ELSE
											    1          
							 END)) * COALESCE(crtvlponto,0)
							 
						ELSE
							0
					END	as valor,
					
					d.dmdid,

					d.dmdidorigem
					 
				FROM
				
					 demandas.demanda d
					 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
					 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
					 LEFT JOIN demandas.sistemadetalhe sis ON sis.sidid = d.sidid
					 LEFT JOIN demandas.sistemacelula sis_c ON sis_c.sicid = sis.sidid
					 LEFT JOIN demandas.celula cel ON cel.celid = sis_c.celid
					 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
					 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
					 LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid and pt.priid = d.priid and pt.tspstatus = 'A'
					 --LEFT JOIN (select crtvlponto, crtdtinicio, crtdtfim from demandas.contrato where crtstatus='A') as con on d.dmddatainclusao between con.crtdtinicio and con.crtdtfim
					 LEFT JOIN demandas.contrato con on (od.ordid = con.ordid and con.crtstatus='A' and d.dmddatainclusao between con.crtdtinicio and con.crtdtfim)
					WHERE 
					 d.dmdstatus = 'A'	
					 and d.dmdidorigem = $dmdid
					 AND d.usucpfdemandante = '".$_SESSION['usucpf']."'		 
					ORDER BY d.dmdid DESC ";
	}
	
	//dbg($sql,1);
	$filhos = $db->carregar($sql);
	if($filhos){
		
		$nivel = 1;
		
		foreach($filhos AS $fl){
			
			$arvore = "1.$x$nivel";
			//$caminho = $_SERVER ['REQUEST_URI'];
			//$caminho = explode("&dmdid=",$caminho);
			//$caminho = $caminho[0]."&dmdid={$fl['dmdid']}";
			//($_SESSION['dmdid'] == $fl['dmdid'])? $cor = "font-weight:bold" : $cor="";
			//$tr_filhos .= ("<div style=\"text-align: left;background: rgb(238, 238, 238);padding-left:".(($nivel == 1)? $width=$width+15 : $width=$width)."px;$cor\" ><img src='../imagens/seta_filho.gif' ><a href=\"$caminho\" > Cód. # {$fl['dmdid']} - {$fl['dmdtitulo']}</a></div>");
			
			//$cor = $cor == '#F7F7F7' ? '#ffffff' : '#F7F7F7' ;
			//$cor = '#ffffcc';
			if($tipoconsulta == "atendimento"){
				$tr_filhos .= "
							<tr bgcolor=".$cor." onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='".$cor."';\">
								<td nowrap><div style=\"text-align: left;padding-left:".(($nivel == 1)? $width=$width+15 : $width=$width)."px;$cor\"><img src='../imagens/seta_filho.gif'>{$fl['id']}</div></td>
								<td >".$fl['assunto']."</td>
								<td >".$fl['origemdemanda']."</td>
								<td >".$fl['situacao']."</td>
								<td >".$fl['tecnicoresponsavel']."</td>
								<td >".$fl['dataabertura']."</td>
								<td >".$fl['dataprevisaotermino']."</td>
							</tr>
				";
			}
			elseif($tipoconsulta == "finalizadas"){
				$tr_filhos .= "
							<tr bgcolor=".$cor." onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='".$cor."';\">
								<td nowrap><div style=\"text-align: left;padding-left:".(($nivel == 1)? $width=$width+15 : $width=$width)."px;$cor\"><img src='../imagens/seta_filho.gif'>{$fl['id']}</div></td>
								<td >".$fl['assunto']."</td>
								<td >".$fl['origemdemanda']."</td>
								<td >".$fl['situacao']."</td>
								<td >".$fl['tecnicoresponsavel']."</td>
								<td >".$fl['dataabertura']."</td>
								<td >".$fl['dataconclusao']."</td>
							</tr>
				";
			}
			elseif($tipoconsulta == "avaliacoes"){
				$tr_filhos .= "
							<tr bgcolor=".$cor." onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='".$cor."';\">
								<td nowrap><div style=\"text-align: left;padding-left:".(($nivel == 1)? $width=$width+15 : $width=$width)."px;$cor\"><img src='../imagens/seta_filho.gif'>{$fl['id']}</div></td>
								<td >".$fl['assunto']."</td>
								<td >".$fl['origemdemanda']."</td>
								<td >".$fl['situacao']."</td>
								<td >".$fl['tecnicoresponsavel']."</td>
								<td >".$fl['dataconclusao']."</td>
								<td align=\"center\"><a title=\"Clique para avaliar esta demanda.\" href=\"javascript:buscarAval(".$fl['dmdid'].");\"><div id=\"div".$fl['dmdid']."\">".$fl['avaliacao']."</div></td>
							</tr>
				";
			}
			elseif($tipoconsulta == "consumo"){
				$tr_filhos .= "
							<tr bgcolor=".$cor." onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='".$cor."';\">
								<td nowrap><div style=\"text-align: left;padding-left:".(($nivel == 1)? $width=$width+15 : $width=$width)."px;$cor\"><img src='../imagens/seta_filho.gif'>{$fl['id']}</div></td>
								<td >".$fl['assunto']."</td>
								<td >".$fl['origemdemanda']."</td>
								<td >".$fl['situacao']."</td>
								<td align=\"center\">".number_format($fl['valor'], 2, ',', '.')."</td>
							</tr>
				";
			}
						
			
			$filho = dmdFilhoSub($fl['dmdid'],$width,$profundidade,$cor,$tipoconsulta);
			($filho)? $profundidade = $profundidade : "" ;
			$tr_filhos .= $filho;
			$nivel++;
		}

	}
	
	return $tr_filhos;
}


?>

<script type="text/javascript">
	//varial global
	d = document;

	function pesquisa() {
		var vlini = parseInt(d.formulario.anoini.value+d.formulario.mesini.value);
		var vlfim = parseInt(d.formulario.anofim.value+d.formulario.mesfim.value);
		
		var totalmeses = (d.formulario.anofim.value - d.formulario.anoini.value)*12;		 
	    totalmeses = totalmeses + (d.formulario.mesfim.value - d.formulario.mesini.value);
	    
		if( vlini > vlfim ){
			alert("Mês/Ano fim não pode ser menor que Mês/Ano início!");
			return;
		}
		
		if( totalmeses > 12 ){
			alert("Limite máximo para o Período são de 12 meses!");
			return;
		}
		
		d.formulario.submit();
	}

	function limpa() {
		d.formulario.codigo.value = '';
		d.formulario.palavra_chave.value = '';
		d.formulario.mesini.value = '';
		d.formulario.mesfim.value = '';
		d.formulario.anoini.value = '';
		d.formulario.anofim.value = '';
		d.formulario.submit();
	}	



	function buscarAval(dmdid){
	    inicialytebox('popCadAvaliacao.php?codseg=simecok&dmdid=' + dmdid, dmdid);
	}
	
	function inicialytebox(url, dmdid) {
	    var anchor = this.document.createElement('a'); // cria um elemento de link
	    anchor.setAttribute('rev', 'width: 800px; height: 600px; scrolling: auto;');
	    anchor.setAttribute('title', '');
	    anchor.setAttribute('href', url);
	    anchor.setAttribute('rel', 'lyteframe');
	    // paramentros para do lytebox //
	    myLytebox.maxOpacity = 50;
	    myLytebox.outerBorder = true;
	    myLytebox.doAnimations = false;
	    myLytebox.start(anchor, false, true);
		/*
	    d.getElementById('lbClose').onclick = function() { 
	        alert(dmdid); window.location.reload();
	        //d.getElementById('div'+dmdid).innerHTML = "<font color=blue><b>SIM</b></font>";
	        myLytebox.end(); 
	        return false; 
	    }
	    */
	    return false;
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


<?php 

//alerta o usuário sobre avaliações não realizadas
$sql = "select distinct 
			count(d.dmdid) as total, min(to_char(d.dmddatainclusao, 'YYYY-MM-DD')) as dtinicio 
		from demandas.demanda d 
		inner join workflow.documento doc on doc.docid = d.docid and doc.tpdid in (31,35)
		where d.dmdstatus = 'A'
		and d.usucpfdemandante = '".$_SESSION['usucpf']."'
		and doc.esdid in(93,111,95,109,170) --finalizada
		and dmdid not in (select dmdid from demandas.avaliacaodemanda)
		and d.dmddatainclusao between (now() - INTERVAL '30 DAYS') and now()";
$dAvaliacao = $db->pegaLinha($sql);

if($dAvaliacao){
	$totAvaliacao = $dAvaliacao['total'];
	$dtinicio = explode("-",$dAvaliacao['dtinicio']);
	$anoinicio = $dtinicio[0];
	$mesinicio = $dtinicio[1];
}

//$_SESSION['avaliacao'] = "";
if($totAvaliacao > 0 && !$_SESSION['avaliacao']){
	$mesi = $mesinicio;
	$anoi = $anoinicio;
	$mesf = date('m');
	if(strlen($mesf) == 1) $mesf = "0".$mesf;
	$anof = date('Y');
	$_SESSION['avaliacao'] = "ok";
	?>
		<script>
			alert('Existe(m) <?=$totAvaliacao?> demanda(s) que não foi(ram) avaliada(s)! \nVerifique na área "Minhas Avaliações" e clique no link "NÃO" para avaliar.\nPor favor, avalie e colabore para as melhorias de nosso atendimento.');
			//location.href="demandas.php?modulo=principal/painelDemandante&acao=A&mesini=<?=$mesi?>&anoini=<?=$anoi?>&mesfim=<?=$mesf?>&anofim=<?=$anof?>";
			
			document.formulario.mesini.value = '<?=$mesi?>';
			document.formulario.mesfim.value = '<?=$mesf?>';
			document.formulario.anoini.value = '<?=$anoi?>';
			document.formulario.anofim.value = '<?=$anof?>';
			document.formulario.submit();
			
		 </script>
	<?
}
else{
	$_SESSION['avaliacao'] = "";
}
?>