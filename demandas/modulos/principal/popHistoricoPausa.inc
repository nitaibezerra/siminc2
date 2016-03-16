<?php

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
				$sql = "select dmdhorarioatendimento, 
							   to_char(dmddatafimprevatendimento::timestamp,'DD/MM/YYYY HH24:MI') AS dmddatafimprevatendimento,
							   --datahist as dataconc
							   (select to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI')
								 		from 	workflow.historicodocumento a
								 			inner join workflow.comentariodocumento b on a.hstid = b.hstid
								 			inner join workflow.documento c on c.docid = a.docid
								 	where aedid in (146, 191) 
								 	and c.esdid in (93,95,109,111)
								 	and a.docid = d.docid
								) as dataconc
						from demandas.demanda d
						/*
						LEFT JOIN (  (select a.docid, to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datahist
								 		from 	workflow.historicodocumento a
								 			inner join workflow.comentariodocumento b on a.hstid = b.hstid
								 			inner join workflow.documento c on c.docid = a.docid
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
	 
				return $textotempopausa;
}


?>
<html>
<body style="font-size:7pt;" leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">


<script language="JavaScript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>

<form name="formulario" action="" method="post">

<table width="99%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td style="background-color: #d0d0d0; text-align: center;" colspan="2">
			<b>HISTÓRICO DA PAUSA - DEMANDA Nº <?=$_REQUEST['dmdid']?></b>
		</td>
	</tr>
	<tr>
		<td colspan=2>
		<?
		$historicoPausa = montaPausa($_REQUEST['dmdid']);
		echo $historicoPausa;
		?>
		</td>
	</tr>	
</table>
</form>
</body>
</html>

