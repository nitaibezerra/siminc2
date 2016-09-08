<?php

//recupera perfil do usuário
$perfil = arrayPerfil();


//coloca periodo e valor do ponto no titulo
if(!$_REQUEST['titulo']) $_REQUEST['titulo'] = "Relatório Tramites SS";

$txtdtinifim = "";
if( $_REQUEST['dtinicio'] && $_REQUEST['dtfim'] ) $txtdtinifim = "<br><font style='font-size:10px;'>Período de Abertura: ".$_REQUEST['dtinicio']." à ".$_REQUEST['dtfim']."</font>";

$_REQUEST['titulo'] = $_REQUEST['titulo'] . $txtdtinifim;

 

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
	
		<table width="100%" cellspacing="1" cellpadding="5" border="0" align="center" class="tabela">
			<tr><td colspan="100">
				<center>
				<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">
					<tr bgcolor="#ffffff">
							<td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>
							<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
								SIMEC- Sistema Integrado de Monitoramento Execução e Controle<br/> MEC / SE - Secretaria Executiva <br />
							</td>		
							<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
								Impresso por: <b><?=$_SESSION['usunome']?></b><br/> Hora da Impressão:<?=date("d/m/Y H:i:s");?><br />
							</td>	
					</tr>
					<tr>		
							<td colspan="2" align="center" valign="top" style="padding:0 0 5px 0;">
								<b><font style="font-size:14px;"><?=$_REQUEST['titulo']?></font></b>		
							</td>	
					</tr>
				</table>	
				</center>
			</td></tr>
			<TR style="background:#D9D9D9;">
				<TD>
					<div style="font-weight:bold;">&nbsp;Nº da SS </div>
				</TD>
				<TD align="center" valign="top" style="font-weight:bold;">Necessidade</TD>
				<TD align="center" valign="top" style="font-weight:bold;">Sistema</TD>
				<TD align="center" valign="top" style="font-weight:bold;">Requisitante</TD>
				<TD align="center" valign="top" style="font-weight:bold;">Data de Abertura</TD>
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
								<img src="../imagens/seta_filho.gif" align="absmiddle"/>&nbsp;<b><?=$dados[$i]['nuss']?></b>
							</td>
							<td align="left" style="color:#000000;" >
								<?=$dados[$i]['necessidade']?>
							</td>
							<td align="left" style="color:#000000;">
								<?=$dados[$i]['sistema']?>
							</td>
							<td align="left" style="color:#000000;">
								<?=$dados[$i]['requisitante']?>
							</td>
							<td align="left" style="color:#000000;">
								<?=$dados[$i]['dataabertura']?>
							</td>
							<td align="left" style="color:#000000;">
								<?=$dados[$i]['situacao']?>
							</td>
							<td align="center" style="color:#000000;" width="25%">
								<?
									//pega historico de tramites
									if($dados[$i]['nuss'] && $dados[$i]['docid']){
										
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
						<td align="left" colspan="7"><font color="red"><center>Não existem registros.</center></font></td>
					 </tr>';
			}
		
		?>	
		<tr style="background:#DFDFDF;">
			<td align="left" colspan="7">&nbsp;</td>
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
	
	// tipo serviço
	if( $tiposervico[0] && $tiposervico_campo_flag != '' ){
		array_push($where, " ts.tpsid " . (!$tiposervico_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tiposervico ) . "') ");
	}
	
	// sistema
	if( $sistema[0] && $sistema_campo_flag != '' ){
		array_push($where, " sis.sidid " . (!$sistema_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $sistema ) . "') ");
	}
			
	// requisitante
	if($requisitante[0] != ''){
		array_push($where, " ss.usucpfrequisitante " . (!$requisitante_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $requisitante ) . "') ");
	}
		
	
	// situacao da demanda
	if( $situacao[0] && $situacao_campo_flag != '' ){
		array_push($where, " ess.esdid " . (!$situacao_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $situacao ) . ") ");
	}	


	
	// período de abertura
	if( $dtinicio && $dtfim ){
		$dtinicio = formata_data_sql($dtinicio);
		$dtfim = formata_data_sql($dtfim);
		array_push($where, " ss.dataabertura BETWEEN '{$dtinicio} 00:00:00' AND '{$dtfim} 23:59:59' ");
	}
	
	
	//SQL tunada atual
	$sql = "-- Início - SQL
            select distinct 
				ss.scsid as nuss, 
				ss.scsnecessidade as necessidade,
				sis.siddescricao as sistema,
				u.usunome as requisitante,
				to_char(ss.dataabertura::timestamp,'DD/MM/YYYY HH24:MI:SS') as dataabertura,
				ess.esddsc as situacao,
				ts.tpsdsc as tiposervico,
				ss.docid,
				an.ansid as ansid,
				UPPER(ug.ungdsc) as unidade
			from fabrica.solicitacaoservico ss
			left join fabrica.analisesolicitacao an on an.scsid = ss.scsid
			left join fabrica.tiposervico ts on ts.tpsid = an.tpsid
			left join workflow.documento dss on dss.docid = ss.docid
			left join workflow.estadodocumento ess on ess.esdid = dss.esdid
			LEFT JOIN workflow.historicodocumento a ON a.hstid = dss.hstid
			left join demandas.sistemadetalhe sis on sis.sidid = ss.sidid
			left join seguranca.usuario u on u.usucpf = ss.usucpfrequisitante
			left join public.unidadegestora ug on ug.ungcod = u.ungcod
			WHERE ss.scsstatus = 'A' " . ( $where[0] ? ' AND' . implode(' AND ', $where) : '' ) . "
			ORDER BY  1
	        -- Fim - SQL";	
			//dbg($sql,1);	 	
	return $sql;
	

}

?>