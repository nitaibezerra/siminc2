<?php


$arquivo = 'SIMEC_DEMANDAS_PAUSA'.date("dmYHis").'.xls';

header ("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
header ("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
header ("Cache-Control: no-cache, must-revalidate");
header ("Pragma: no-cache");
header ("Content-type: application/x-msexcel");
header ("Content-Disposition: attachment; filename=\"{$arquivo}\"" );
header ("Content-Description: PHP Generated Data" );


//recupera perfil do usuário
$perfil = arrayPerfil();
 

// Inclui componente de relatórios
include_once APPRAIZ . 'includes/workflow.php';

?>


<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>

	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
	
		<table width="100%" cellspacing="1" cellpadding="5" border="1" align="center" class="tabela">
			
			<TR style="background:#D9D9D9;">
				<TD>
					<div style="font-weight:bold;">&nbsp;Nº da Demanda </div>
				</TD>
				<TD align="center" valign="top" style="font-weight:bold;">Origem</TD>
				<TD align="center" valign="top" style="font-weight:bold;">Célula</TD>
				<TD align="center" valign="top" style="font-weight:bold;">Sistema</TD>
				<TD align="center" valign="top" style="font-weight:bold;">Situação</TD>
				<TD align="center" valign="top" style="font-weight:bold;">Tramite</TD>
			</TR>
					

		<?
			
			$sql   = monta_sql();
			
			$dados = $db->carregar($sql);
			
			if($dados){
				
			
				for($i = 0; $i<=count($dados)-1; $i++ ){
				
					?>
						<tr style="" bgcolor="#E5E5E5" onmouseout="this.bgColor='#E5E5E5';" onmouseover="this.bgColor='#ffffcc';">
							<td style="padding-left:20px; font-size:10px;">
								&nbsp;<b><?=$dados[$i]['nudemanda']?></b>
							</td>
							<td align="left" style="color:#000000;">
								<?=$dados[$i]['origemdemanda']?>
							</td>
							<td align="left" style="color:#000000;">
								<?=$dados[$i]['celula']?>
							</td>
							<td align="left" style="color:#000000;">
								<?=$dados[$i]['sistema']?>
							</td>
							<td align="left" style="color:#000000;">
								<?=$dados[$i]['situacao']?>
							</td>
							<td align="center" style="color:#000000;" width="80%">
								
								<table width="100%" cellspacing="1" cellpadding="5" border="0" align="center" class="tabela">
									<TR style="background:#D9D9D9;">
										<TD>
											<div style="font-weight:bold;">&nbsp;Estado </div>
										</TD>
										<TD align="center" valign="top" style="font-weight:bold;">Dt. Início Atendimento</TD>
										<TD align="center" valign="top" style="font-weight:bold;">Dt. Fim Atendimento</TD>
										<TD align="center" valign="top" style="font-weight:bold;">Tempo Atendimento (8h)</TD>
										<TD align="center" valign="top" style="font-weight:bold;">Tempo Atendimento (24h)</TD>
										<TD align="center" valign="top" style="font-weight:bold;">Dt. Início Pausa</TD>
										<TD align="center" valign="top" style="font-weight:bold;">Dt. Fim Pausa</TD>
										<TD align="center" valign="top" style="font-weight:bold;">Tempo Pausa (8h)</TD>
										<TD align="center" valign="top" style="font-weight:bold;">Tempo Pausa (24h)</TD>
										<TD align="center" valign="top" style="font-weight:bold;">Total Trabalhados (8h)</TD>
										<TD align="center" valign="top" style="font-weight:bold;">Total Trabalhados (24h)</TD>
									</TR>
									
								<?
									//pega historico de tramites
									if($dados[$i]['nudemanda'] && $dados[$i]['docid']){
										
										
									    $docid           = (integer) $dados[$i]['docid'];
									    $dmdid           = (integer) $dados[$i]['dmdid'];
									    //$dadoEstadoAtual = wf_pegarEstadoAtual( $docid );
									    
									    // Busca historico
									    $sql = "select
													ed.esddsc,
													to_char(hd.htddata::TIMESTAMP,'DD/MM/YYYY HH24:MI') as dtdoc,
													hd.htddata
												from workflow.historicodocumento hd
													inner join workflow.acaoestadodoc ac on
														ac.aedid = hd.aedid
													inner join workflow.estadodocumento ed on
														ed.esdid = ac.esdidorigem
												where
													hd.docid = " . $docid . "
												order by
													hd.htddata asc, hd.hstid asc";
										$dadoHistorico = $db->carregar( $sql );
									    
									    
										if($dadoHistorico){
											
											
											$a = 1;
											
											$dtini = $dados[$i]['dataabertura'];
											
											$histtramite = '<div align=left>';
											
											foreach($dadoHistorico as $val){
												
												unset($tempo8h);
												unset($tempo24h);
												unset($tempoPausa8h);
												unset($tempoPausa24h);
												
												//verifica se tem pausa
												$format_dtini = trim(substr($dtini,6,4)).'-'.trim(substr($dtini,3,2)).'-'.trim(substr($dtini,0,2)).' '.trim(substr($dtini,11,5));
												$format_dtfim = trim(substr($val['dtdoc'],6,4)).'-'.trim(substr($val['dtdoc'],3,2)).'-'.trim(substr($val['dtdoc'],0,2)).' '.trim(substr($val['dtdoc'],11,5));
												
												$sqlp = "select
															to_char(pdmdatainiciopausa::TIMESTAMP,'DD/MM/YYYY HH24:MI') as pdmdatainiciopausa,
															to_char(pdmdatafimpausa::TIMESTAMP,'DD/MM/YYYY HH24:MI') as pdmdatafimpausa
														from demandas.pausademanda 
														where
															pdmstatus = 'A'
														and pdmdatainiciopausa is not null
														and pdmdatafimpausa is not null
														AND ( (pdmdatainiciopausa BETWEEN '$format_dtini' AND '$format_dtfim') 
																OR 
															  (pdmdatafimpausa BETWEEN '$format_dtini' AND '$format_dtfim')
															)
														and dmdid = ".$dmdid."
														order by
															pdmdatainiciopausa";
												//dbg($sqlp);
												$dadosPausa = $db->pegaLinha( $sqlp );
												//dbg($dadosPausa);
												
												//verifica se a data inicio pausa é menor que a data inicio atendimento
													$format_dtinicio_pausa = trim(substr($dadosPausa['pdmdatainiciopausa'],6,4)).'-'.trim(substr($dadosPausa['pdmdatainiciopausa'],3,2)).'-'.trim(substr($dadosPausa['pdmdatainiciopausa'],0,2)).' '.trim(substr($dadosPausa['pdmdatainiciopausa'],11,5));
													
													$format_dtinicio2 = (float) str_replace(":","",str_replace(" ","",str_replace("-","",$format_dtini)));
													$format_dtinicio_pausa2 = (float) str_replace(":","",str_replace(" ","",str_replace("-","",$format_dtinicio_pausa)));
													
													//echo $format_dtinicio_pausa2 .' < '. $format_dtinicio2.'<br>';
													if($format_dtinicio_pausa2 > 0 && $format_dtinicio_pausa2 < $format_dtinicio2){
														$dadosPausa['pdmdatainiciopausa'] = $dtini;
													}
												//fim verifica
												
												//verifica se a data fim pausa é maior que a data fim atendimento
													$format_dtfim_pausa = trim(substr($dadosPausa['pdmdatafimpausa'],6,4)).'-'.trim(substr($dadosPausa['pdmdatafimpausa'],3,2)).'-'.trim(substr($dadosPausa['pdmdatafimpausa'],0,2)).' '.trim(substr($dadosPausa['pdmdatafimpausa'],11,5));
													
													$format_dtfim2 = (float) str_replace(":","",str_replace(" ","",str_replace("-","",$format_dtfim)));
													$format_dtfim_pausa2 = (float) str_replace(":","",str_replace(" ","",str_replace("-","",$format_dtfim_pausa)));
													
													//echo $format_dtfim_pausa2 .' > '. $format_dtfim2.'<br>';
													if($format_dtfim_pausa2 > 0 && $format_dtfim_pausa2 > $format_dtfim2){
														$dadosPausa['pdmdatafimpausa'] = $val['dtdoc'];
													}
												//fim verifica
												
												?>
												<tr style="" bgcolor="#E5E5E5" onmouseout="this.bgColor='#E5E5E5';" onmouseover="this.bgColor='#ffffcc';">
													<td style="padding-left:20px; font-size:10px;" width="30%">
														&nbsp;<b><?=$val['esddsc']?></b>
													</td>
													<td align="center" style="color:#000000;">
														<?=$dtini?>
													</td>
													<td align="center" style="color:#000000;">
														<?=$val['dtdoc']?>
													</td>
													<td align="center" style="color:#000000;">
														<?
															$tempo8h = calculaTempoMinutoCgd($dtini, $val['dtdoc'], '', '8');
															echo $tempo8h . ' m';
														?>
													</td>
													<td align="center" style="color:#000000;">
														<?
															$tempo24h = calculaTempoMinutoCgd($dtini, $val['dtdoc'], '', '24');
															echo $tempo24h . ' m';
														?>
													</td>
													<td align="center" style="color:#000000;">
														<?=($dadosPausa['pdmdatainiciopausa'] ? $dadosPausa['pdmdatainiciopausa'] : '-')?>
													</td>
													<td align="center" style="color:#000000;">
														<?=($dadosPausa['pdmdatafimpausa'] ? $dadosPausa['pdmdatafimpausa'] : '-')?>
													</td>
													<td align="center" style="color:#000000;">
														<?
															if($dadosPausa['pdmdatainiciopausa']){
																$tempoPausa8h = calculaTempoMinutoCgd($dadosPausa['pdmdatainiciopausa'], $dadosPausa['pdmdatafimpausa'], '', '8');
																echo $tempoPausa8h . ' m';
															}else{
																echo '-';
															}
														?>
													</td>
													<td align="center" style="color:#000000;">
														<?
															if($dadosPausa['pdmdatainiciopausa']){
																$tempoPausa24h = calculaTempoMinutoCgd($dadosPausa['pdmdatainiciopausa'], $dadosPausa['pdmdatafimpausa'], '', '24');
																echo $tempoPausa24h . ' m';
															}else{
																echo '-';
															}
														?>
													</td>
													<td align="center" style="color:#000000;">
														<?
															if(!$tempoPausa8h) $tempoPausa8h = 0;
															if($tempo8h){
																echo ($tempo8h-$tempoPausa8h) . ' m';
															}else{
																echo '-';
															}
														?>
													</td>
													<td align="center" style="color:#000000;">
														<?
															if(!$tempoPausa24h) $tempoPausa24h = 0;
															if($tempo24h){
																echo ($tempo24h-$tempoPausa24h) . ' m';
															}else{
																echo '-';
															}
														?>
													</td>
												</tr>
									
												
												<?
												
												$dtini = $val['dtdoc'];
												
												$a++;
											}
											
										}
										
									}
									
									?>
									</table>
									
							</td>
						</tr>
					<?
					
				}
				
				
				
			}else{
				echo '<tr style="background:#DFDFDF;">
						<td align="left" colspan="6"><font color="red"><center>Não existem registros.</center></font></td>
					 </tr>';
			}
		
		?>	
		<tr style="background:#DFDFDF;">
			<td align="left" colspan="6">&nbsp;</td>
		</tr>
		<TR style="background:#FFFFFF;">
			<TD colspan="100" align="right" style="font-weight:bold; font-size:9px; border-top:2px solid black; border-bottom:2px solid black;"><div style="float:left; font-size:11px;">Total de registros: <?=($i > 0 ? $i : 0)?></div></TD>
		</tr>
		</table>

	</body>
</html>

<?

function monta_sql(){
	global $db;
	extract($_REQUEST);

	$perfil = arrayPerfil();
	
	$where = array();
	
	// origem
	if( $origemd[0] && $origemd_campo_flag != '' ){
		array_push($where, " od.ordid " . (!$origemd_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $origemd ) . "') ");
	}
	
	// celula
	if( $celula[0] && $celula_campo_flag != '' ){
		array_push($where, " d.celid " . (!$celula_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $celula ) . "') ");
	}
			
	// Sistema
	if($sidid[0] != ''){
		array_push($where, " d.sidid " . (!$sidid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $sidid ) . "') ");
	}
		
	
	// situacao da demanda
	if( $situacao[0] && $situacao_campo_flag != '' ){
		array_push($where, " ed.esdid " . (!$situacao_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $situacao ) . ") ");
	}	


	
	// período de abertura
	if( $dtinicio && $dtfim ){
		$dtinicio = formata_data_sql($dtinicio);
		$dtfim = formata_data_sql($dtfim);
		array_push($where, " d.dmddatainclusao BETWEEN '{$dtinicio} 00:00:00' AND '{$dtfim} 23:59:59' ");
	}
	
	
	//SQL tunada atual
	$sql = "-- Início - SQL
            SELECT  DISTINCT
                LPAD(CAST(d.dmdid AS VARCHAR), GREATEST( LENGTH(CAST(d.dmdid AS VARCHAR)), 5), '0') AS nudemanda,
                d.dmdid,
                od.ordid AS ordid,
                od.orddescricao AS origemdemanda,
                t.tipnome AS tipodemanda,
                smd.sidabrev || ' - ' || smd.siddescricao AS sistema,
                cel.celnome AS celula,
                d.dmdtitulo AS assunto,
                d.dmddsc AS descricao,
                to_char(d.dmddatainclusao::DATE,'DD/MM/YYYY') ||' '|| to_char(d.dmddatainclusao, 'HH24:MI') AS dataabertura,
                d.dmddatainclusao AS dmddatainclusao,
                to_char(d.dmddatainiprevatendimento::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datainicio,
                to_char(d.dmddatafimprevatendimento::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datafim,
                d.dmddatafimprevatendimento AS datafimordem,
                to_char(d.dmddatafimprevatendimento::TIMESTAMP,'YYYY-MM-DD HH24:MI:00') AS dmddatafimprevatendimento,
                to_char(d.dmddatainiprevatendimento::TIMESTAMP,'YYYY-MM-DD HH24:MI:00') AS dmddatainiprevatendimento,
		 
            CASE
                WHEN ed.esddsc <> '' THEN ed.esddsc
                ELSE 'Em processamento'
            END AS situacao,      
            CASE EXTRACT(MONTH FROM d.dmddatainclusao)
                WHEN '1' THEN 'Janeiro/'
                WHEN '2' THEN 'Fevereiro/'
                WHEN '3' THEN 'Março/'
                WHEN '4' THEN 'Abril/'
                WHEN '5' THEN 'Maio/'
                WHEN '6' THEN 'Junho/'
                WHEN '7' THEN 'Julho/'
                WHEN '8' THEN 'Agosto/'
                WHEN '9' THEN 'Setembro/'
                WHEN '10' THEN 'Outubro/'
                WHEN '11' THEN 'Novembro/'
                WHEN '12' THEN 'Dezembro/'
            END || to_char(d.dmddatainclusao::DATE,'YYYY') AS mesano,
            d.dmdhorarioatendimento,
            
            CASE
                WHEN d.dmdqtde > 0 THEN d.dmdqtde
                ELSE '1' 
            END AS qtdservico,
            '1' AS totaldemandas,
            ed.esdid,
            '' as histtramite,
            d.docid
    
		 FROM
		 demandas.demanda d
		 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
		 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
		 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35) 
		 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid	
		 LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
		 LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid and smc.celid = d.celid
		 LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
	 			  	 	 
		 WHERE d.dmdstatus = 'A' --and d.dmdid=247593 --247593 --254736 --254389
		 " . ( $where[0] ? ' AND' . implode(' AND ', $where) : '' ) . "
		 ORDER BY  1
         -- Fim - SQL";	
		//dbg($sql,1);	 	
	return $sql;
	

}

?>