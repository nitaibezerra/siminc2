<?php

//recupera perfil do usuário
$perfil = arrayPerfil();


//coloca periodo e valor do ponto no titulo
if(!$_REQUEST['titulo']) $_REQUEST['titulo'] = "Relatório Tramite de Demandas";

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
								SIMEC- Sistema Integrado do Ministério da Educação<br/> MEC / SE - Secretaria Executiva <br />
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
							<td style="padding-left:20px; font-size:10px;" width="30%">
								<img src="../imagens/seta_filho.gif" align="absmiddle"/>&nbsp;<b><?=$dados[$i]['nudemanda']?></b>
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
							<td align="center" style="color:#000000;">
								<?
									//pega historico de tramites
									if($dados[$i]['nudemanda'] && $dados[$i]['docid']){
										
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
	 			  	 	 
		 WHERE d.dmdstatus = 'A' " . ( $where[0] ? ' AND' . implode(' AND ', $where) : '' ) . "
		 ORDER BY  1
         -- Fim - SQL";	
		//dbg($sql,1);	 	
	return $sql;
	

}

?>