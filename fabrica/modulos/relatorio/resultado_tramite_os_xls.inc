<?php

$arquivo = 'SIMEC_FABRICA_TRAMITE_OS'.date("dmYHis").'.xls';

header ("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
header ("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
header ("Cache-Control: no-cache, must-revalidate");
header ("Pragma: no-cache");
header ("Content-type: application/x-msexcel");
header ("Content-Disposition: attachment; filename=\"{$arquivo}\"" );
header ("Content-Description: PHP Generated Data" );


// Inclui componente de relatórios
include_once APPRAIZ . 'includes/workflow.php';

?>


<html>
	<head>
	</head>

	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
	
		<table width="100%" cellspacing="1" cellpadding="5" border="1" align="center" class="tabela">
			<TR style="background:#D9D9D9;">
				<TD>
					<div style="font-weight:bold;">&nbsp;Nº da OS </div>
				</TD>
				<TD align="center" valign="top" style="font-weight:bold;">Nº da SS</TD>
				<TD align="center" valign="top" style="font-weight:bold;">Tipo da OS</TD>
				<TD align="center" valign="top" style="font-weight:bold;">Detalhamento</TD>
				<TD align="center" valign="top" style="font-weight:bold;">Prev. Início</TD>
				<TD align="center" valign="top" style="font-weight:bold;">Prev. Término</TD>
				<TD align="center" valign="top" style="font-weight:bold;">Qtd. PF. Estimada</TD>
				<TD align="center" valign="top" style="font-weight:bold;">Qtd. PF. Detalhada</TD>
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
							<td style="padding-left:20px; font-size:10px;" width="5%">
								&nbsp;<b><?=$dados[$i]['nuos']?></b>
							</td>
							<td align="left" style="color:#000000;" >
								<?=$dados[$i]['nuss']?>
							</td>
							<td align="left" style="color:#000000;" >
								<?=$dados[$i]['tipoordemservico']?>
							</td>
							<td align="left" style="color:#000000;" >
								<?=$dados[$i]['detalhamento']?>
							</td>
							<td align="left" style="color:#000000;">
								<?=$dados[$i]['previni']?>
							</td>
							<td align="left" style="color:#000000;">
								<?=$dados[$i]['prevfim']?>
							</td>
							<td align="left" style="color:#000000;">
								<?=$dados[$i]['qtdestimada']?>
							</td>
							<td align="left" style="color:#000000;">
								<?=$dados[$i]['qtddetalhada']?>
							</td>
							<td align="left" style="color:#000000;">
								<?=$dados[$i]['situacaoos']?>
							</td>
							<td align="center" style="color:#000000;" width="25%">
								<?
									//pega historico de tramites
									if($dados[$i]['nuos'] && $dados[$i]['docid']){
										
										// Busca historico
									    $docid           = (integer) $dados[$i]['docid'];
									    $dadoEstadoAtual = wf_pegarEstadoAtual( $docid );
									    $dadoHistorico   = wf_pegarHistorico( $docid );
									    
										if($dadoHistorico){
											$a = 1;
											$histtramite = '<div align=left>';
											
											foreach($dadoHistorico as $val){
												$histtramite .= '<b>Seq.: '. ($a) . '<br>Onde Estava: '. $val['esddsc'] .'<br>O que aconteceu: '. $val['aeddscrealizada'] .'<br>Quem fez: '. $val['usunome'] .'<br>Quando fez: '. $val['htddata'];
							
												if($val['cmddsc']){
													$histtramite .= '<br>Justificativa: '. $val['cmddsc'];
												}
												
												$histtramite .= '</b><br><br>';
												$a++;
											}
											
											$histtramite .= '<b>Estado atual: '.str_to_upper($dadoEstadoAtual['esddsc']).'</b><br><br><br>';
											
											$histtramite .= '</div>';
										}
										else{
											$histtramite = '<div align=left><b>Em Processamento</b></div>';
										}
									}
									else{
										$histtramite = '<div align=left><b>Em Processamento</b></div>';
									}
									echo $histtramite;
								?>
							</td>
						</tr>
					<?
					
				}
				
				
				
			}else{
				echo '<tr style="background:#DFDFDF;">
						<td align="left" colspan="10"><font color="red"><center>Não existem registros.</center></font></td>
					 </tr>';
			}
		
		?>	
		<tr style="background:#DFDFDF;">
			<td align="left" colspan="10">&nbsp;</td>
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
	
	// tipo serviço OS
	if( $tipoos[0] && $tipoos_campo_flag != '' ){
		array_push($where, " tos.tosid " . (!$tipoos_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tipoos ) . "') ");
	}
	
	// situacao 
	if( $situacao[0] && $situacao_campo_flag != '' ){
		array_push($where, " eos.esdid " . (!$situacao_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $situacao ) . ") ");
	}	


	
	// período de abertura
	if( $dtinicio && $dtfim ){
		$dtinicio = formata_data_sql($dtinicio);
		$dtfim = formata_data_sql($dtfim);
		array_push($where, " os.odsdtprevinicio BETWEEN '{$dtinicio} 00:00:00' AND '{$dtfim} 23:59:59' ");
	}
	
	
	//SQL tunada atual
	$sql = "-- Início - SQL
            select distinct 
				os.odsid as nuos,
            	ss.scsid as nuss, 
            	os.odsdetalhamento as detalhamento,
				to_char(os.odsdtprevinicio::timestamp,'DD/MM/YYYY') as previni,
				to_char(os.odsdtprevtermino::timestamp,'DD/MM/YYYY') as prevfim,
				case when eos.esddsc is null then
						'Criada'
					else
						eos.esddsc
				end as situacaoos,
				eospf.esddsc as situacaoosc,
				tos.tosdsc as tipoordemservico,
				os.odsqtdpfestimada as qtdestimada,
				os.odsqtdpfdetalhada as qtddetalhada,
				os.docid
			from fabrica.solicitacaoservico ss
			left join fabrica.analisesolicitacao an on an.scsid = ss.scsid
			left join fabrica.tiposervico ts on ts.tpsid = an.tpsid
			left join fabrica.ordemservico os on os.scsid = ss.scsid
			left join fabrica.tipoordemservico tos on tos.tosid = os.tosid
			left join workflow.documento dos on dos.docid = os.docid
			left join workflow.estadodocumento eos on eos.esdid = dos.esdid
			left join workflow.documento dospf on dospf.docid = os.docidpf
			left join workflow.estadodocumento eospf on eospf.esdid = dospf.esdid
			LEFT JOIN workflow.historicodocumento aos ON aos.hstid = dospf.hstid
			left join workflow.documento dss on dss.docid = ss.docid
			left join workflow.estadodocumento ess on ess.esdid = dss.esdid
			LEFT JOIN workflow.historicodocumento a ON a.hstid = dss.hstid
			WHERE ss.scsstatus = 'A' " . ( $where[0] ? ' AND' . implode(' AND ', $where) : '' ) . "
			ORDER BY  1
	        -- Fim - SQL";	
			//dbg($sql,1);	 	
	return $sql;
	

}

?>