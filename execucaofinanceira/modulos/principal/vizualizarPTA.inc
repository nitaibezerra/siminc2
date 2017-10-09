<?php

/*$processo = $_SESSION['execucaofinanceira']['proid'];
if( empty($processo) ) die("<script> alert('Erro ao tentar identificar o processo!'); window.close();</script>");

$obProcesso = new Processo();
$obProcesso->carregarPorId($processo);

// ------------- REQUISIÇÕES AJAX E FUNÇÕES ------------- //
if($_REQUEST['requisicao'] == 'carregarListaDados') {
	
	$obEmpenho = new Empenho();
	$obEmpenho->carregarPorId($empid);

	$obProcesso->controleDeTela(get_class($obEmpenho));
	$obEmpenho->carregarListaDados($obProcesso);
	exit();
}*/
?>
<?php

$ptrid = $_REQUEST["ptrid"];

include_once APPRAIZ.'emenda/classes/PTA.class.inc';
include_once APPRAIZ.'emenda/classes/planoTrabalhoDAO.class.inc';

$obPTA = new PTA();

$obPTA->validaSessionPTA( $ptrid );
$federal = $obPTA->buscaEmendaDescentraliza( $ptrid );

$boMostra = ( ($federal == 'S') ? false : true );

$sql = "UPDATE 
  emenda.planotrabalho  
SET 
  ptrdataimpressao = now()
 
WHERE 
  ptrid = $ptrid
AND ptrimprimirpta = true";

$db->executar($sql);
$db->commit();


/* DADOS DO PLANO DE TRABALHO */
		
			$sql = "SELECT 
					  ent.enbcnpj AS cnpj,
					  ent.enbnome AS orgao_entidade,
					  ptr.ptrexercicio AS exercicio,
					  CASE WHEN ent.muncod is null THEN
						'Dados desatualizados. Clique no link acima para atualizar o endereço da entidade.'
					ELSE
						(select mun.mundescricao || ' / ' || mun.estuf 
						from emenda.entidadebeneficiada ende 
					  	INNER JOIN territorios.municipio mun ON (mun.muncod = ende.muncod)
						WHERE ende.enbstatus = 'A' AND ende.enbid = ent.enbid limit 1)
					END	as endereco,
					  ptr.ptrid,
					  ptr.ptrcod,
					  res.resid, 
					  res.resassunto,
					  (sum(ped.pedvalor) + ptr.ptrvalorproponente) as ptrvalorconcedente,
					  ptp.pubdatapublicacao,
					  pti.ptivalortotal
					FROM
					  emenda.entidadebeneficiada ent 
					  INNER JOIN emenda.planotrabalho ptr ON (ent.enbid = ptr.enbid)
					  INNER JOIN emenda.responsavel res
					  ON (res.resid=ptr.resid)
					  inner join emenda.ptemendadetalheentidade ped
		  				on ped.ptrid = ptr.ptrid
		  			  left join emenda.ptminutaconvenio ptmc
		  			  	inner join emenda.ptpublicacao ptp
                        	on ptp.pmcid = ptmc.pmcid
                            and ptp.pubstatus = 'A'
                      	on ptmc.ptrid = ptr.ptrid
                      	and ptmc.pmcstatus = 'A'
                      LEFT JOIN 
						(SELECT 
							ptrid,
							sum(ptivalortotal) as ptivalortotal
						 FROM emenda.v_ptiniciativa 
						 GROUP BY ptrid) pti ON pti.ptrid = ptr.ptrid
					WHERE
					  ptr.ptrid = {$ptrid}
					  AND ptr.ptrexercicio = ".$_SESSION["exercicio"]."
					  --AND ptr.ptrid NOT IN (SELECT tt.ptridpai FROM emenda.planotrabalho tt WHERE tt.ptridpai = ptr.ptrid and tt.ptrstatus = 'A')
					GROUP BY 
					  ent.enbcnpj,
					  ent.enbid,
					  ent.muncod,
					  orgao_entidade,
					  ptr.ptrexercicio,
					  ptr.ptrid,
					  ptr.ptrcod,
					  res.resid, 
					  res.resassunto,
					  ptr.ptrvalorproponente,
					  ptp.pubdatapublicacao,
					  pti.ptivalortotal";
			
			$dadosPlanoTrabalho = $db->pegaLinha($sql);
			
			if( empty($dadosPlanoTrabalho) ){
				echo "<script>
							alert('Não existem dados para serem exibidos!');
							window.close();
					  </script>";
				die;
			}
			
			/*if( $dadosPlanoTrabalho['pmcdatapublicacao'] ){
				$data = formata_data( $dadosPlanoTrabalho['pmcdatapublicacao'] );
			} else {
				$data = Date("d/m/Y");
			}*/

?>

<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<script type="text/javascript" src="../includes/prototype.js"></script>
	    <script type="text/javascript" src="../includes/entidades.js"></script>
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" >
			<tr>
				<td>
					<img src="/imagens/logo_fnde.gif"/>
				</td>
				<td>
					<b>Ministério da Educação</b> <br/>
					FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO <br/>
					Simec - Sistema Integrado de Monitoramento do Ministério da Educação
				</td>
				<td align="right">
					&nbsp;
					<!-- <b>Data:</b> <?php echo $data; ?><br/>
					<b>Hora:</b> <?php echo Date("H:i:s"); ?> --> 
				</td>
			</tr>
		</table>
		
		<br/>
		
		<?php
			$dadosPlanoTrabalho['cnpj'] = substr($dadosPlanoTrabalho['cnpj'],0,2) . "." .
										  substr($dadosPlanoTrabalho['cnpj'],2,3) . "." .
										  substr($dadosPlanoTrabalho['cnpj'],5,3) . "/" .
										  substr($dadosPlanoTrabalho['cnpj'],8,4) . "-" .
										  substr($dadosPlanoTrabalho['cnpj'],12,2);
										  
			$sql = "SELECT 
						ptr.ptrjustificativa,
						ptr.ptrtipodirigente,
						ptr.entiddirigente,
						ptr.bcoid,
						ptr.ptragenciabancaria,
						eb.bconome
					FROM
						emenda.planotrabalho ptr
					LEFT JOIN
						emenda.banco eb ON eb.bcoid = ptr.bcoid
					WHERE
			            ptr.ptrid = " . $ptrid;
			
			$dadosAdicionais = $db->pegaLinha($sql);
										  
									  
		?>
		
		<table align="center" cellspacing="1" cellpadding="4" style="border-collapse: collapse; border: 1px solid #ccc;" width="95%">
			<tr>
				<td class="subtitulocentro" colspan="3">PLANO DE TRABALHO</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;">
					<b>EXERCÍCIO</b><br/>
					<?php echo $dadosPlanoTrabalho['exercicio']; ?>
				</td>
				<td style="border: 1px solid #ccc;">
					<b>NÍVEL DE ENSINO</b><br/>
					<?php echo $dadosPlanoTrabalho['resassunto']; ?>
				</td>
				<td style="border: 1px solid #ccc;">
					<b>Nº PLANO DE TRABALHO</b><br/>
					<?php echo $dadosPlanoTrabalho['ptrcod']; ?> / <?php echo $dadosPlanoTrabalho['exercicio']; ?>
				</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;">
					<b>CNPJ</b><br/>
					<?php echo $dadosPlanoTrabalho['cnpj']; ?>
				</td>
				<td style="border: 1px solid #ccc;">
					<b>NOME DO ÓRGÃO OU ENTIDADE</b><br/>
					<?php echo $dadosPlanoTrabalho['orgao_entidade']; ?>
				</td>
				<td style="border: 1px solid #ccc;">
					<b>VALOR DO PLANO DE TRABALHO</b><br/>
					R$ <?php echo number_format( $dadosPlanoTrabalho['ptivalortotal'], 2, ",", "." ); ?>
				</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;" colspan="3">
					<b>MUNICÍPIO / UF</b><br/>
					<?php echo $dadosPlanoTrabalho['endereco']; ?>
				</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;" colspan="3" align="justify">
					<b>JUSTIFICATIVA DO PROJETO</b><br/>
					<?php echo $dadosAdicionais['ptrjustificativa']; ?>
				</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;" colspan="3">
					<b>CPF DO DIRIGENTE OU REPRESENTANTE LEGAL</b><br/>
					<?php  
						
						if( $dadosAdicionais["entiddirigente"] ){
							$dirigente = $db->pegaUm("SELECT '( ' || entnumcpfcnpj  ||  ' ) - ' || entnome FROM entidade.entidade WHERE entid = '{$dadosAdicionais["entiddirigente"]}'");
						}
						
						echo $dirigente;
						 
					?>
				</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;" colspan="3">
					<b>BANCO E AGÊNCIA ONDE A CONTA DO CONVÊNIO DEVERÁ SER CRIADA</b><br/>
					<?php echo $dadosAdicionais['bconome']; ?> <br/>
					Ag: <?php echo $dadosAdicionais['ptragenciabancaria']; ?>
				</td>
			</tr>
		</table>
		
		<br/>
		
		<?php 
		
			/* DADOS DOS RECURSOS */
		
			$sql = "SELECT
					  ptr.ptrvalorproponente,
					  res.resid,
					  res.resassunto,
					  ent.entid
					FROM
						emenda.planotrabalho ptr INNER JOIN emenda.responsavel res ON (ptr.resid = res.resid)
					INNER JOIN emenda.entidadebeneficiada ent ON (ptr.enbid = ent.enbid)
					WHERE
					  res.resstatus = 'A'
					  AND ptr.ptrid = $ptrid";

			$arDadosPTA = $db->pegaLinha($sql);
				
			$ptrvalorproponente = number_format($arDadosPTA["ptrvalorproponente"], 2, ',', '.');

		
		?>
		
		<table align="center" cellspacing="1" cellpadding="4" style="border-collapse: collapse; border: 1px solid #ccc;" width="95%">
			<tr>
				<td class="subtitulocentro">RECURSOS</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;" bgcolor="#dcdcdc"><b>CONCEDENTE</b></td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;">
					<?php 
					
						$sql = "SELECT
								  e.emecod,
								  (CASE WHEN e.emetipo = 'E' THEN 'Emenda'
										ELSE 'Complemento' END) as tipoemenda,
								  a.autnome,
								  funcp.fupfuncionalprogramatica,
								  funcp.fupdsc,
								  sum(ede.edevalor) as valor,
								  e.emeid
								FROM emenda.emenda e 
								INNER JOIN emenda.autor a ON (e.autid = a.autid) 
								LEFT JOIN emenda.v_funcionalprogramatica funcp ON (e.acaid = funcp.acaid)
									AND funcp.acastatus = 'A'
									AND funcp.prgano = ".(($ptrid) ? '(SELECT ptrexercicio::varchar FROM emenda.planotrabalho WHERE ptrid = '.$ptrid.')' : '\''.$_SESSION["exercicio"].'\'')."
								INNER JOIN emenda.emendadetalhe ed ON (e.emeid = ed.emeid) 
								INNER JOIN emenda.emendadetalheentidade ede ON (ed.emdid = ede.emdid) 
								LEFT JOIN (Select ped.pedid, ped.edeid, ped.ptrid
											From emenda.ptemendadetalheentidade ped
												inner join emenda.planotrabalho ptr
													ON (ptr.ptrid = ped.ptrid) ) ptede
								  ON (ede.edeid = ptede.edeid)  
								WHERE
								  (".(($ptrid) ? "ptede.ptrid = ".$ptrid : "ptede.ptrid is null").")
								  AND ede.edestatus = 'A'
								  AND e.resid = {$dadosPlanoTrabalho["resid"]}
								  AND ede.enbid = ".(($ptrid) ? '(SELECT enbid FROM emenda.planotrabalho WHERE ptrid = '.$ptrid.')' : $_SESSION["emenda"]["enbid"])."
								  AND ede.ededisponivelpta = 'S'
								GROUP BY
								  e.emecod,
								  a.autnome,
								  funcp.fupfuncionalprogramatica,
								  funcp.fupdsc,
								  e.emeid,
								  e.emetipo";

						$arEmenda = $db->carregar($sql);
					?>
					<table align="center" cellspacing="1" cellpadding="4" style="border-collapse: collapse; border: 1px solid #ccc;" width="95%">
						<tr>
							<td style="border: 1px solid #ccc;" class="subtitulocentro">Código</td>
							<td style="border: 1px solid #ccc;" class="subtitulocentro">Tipo</td>
							<td style="border: 1px solid #ccc;" class="subtitulocentro">Autor</td>
							<td style="border: 1px solid #ccc;" class="subtitulocentro">Funcional Programática</td>
							<td style="border: 1px solid #ccc;" class="subtitulocentro">Subtítulo</td>
							<td style="border: 1px solid #ccc;" class="subtitulocentro" >Valor</td>
						</tr>
						
						<?php
							$arEmenda = ($arEmenda) ? $arEmenda : array();
							for( $i = 0; $i < count($arEmenda); $i++ ){
								
								print "<tr>"
									. "    <td style='border: 1px solid #ccc;'  align='center'>{$arEmenda[$i]["emecod"]}</td>"
									. "    <td style='border: 1px solid #ccc;'>{$arEmenda[$i]["tipoemenda"]}</td>"
									. "    <td style='border: 1px solid #ccc;'>{$arEmenda[$i]["autnome"]}</td>"
									. "    <td style='border: 1px solid #ccc;' align='center'>{$arEmenda[$i]["fupfuncionalprogramatica"]}</td>"
									. "    <td style='border: 1px solid #ccc;'>{$arEmenda[$i]["fupdsc"]}</td>"
									. "    <td style='border: 1px solid #ccc;' align='right'>R$ " . number_format( $arEmenda[$i]["valor"], 2, ",", "." ) . "</td>"
									. "</tr>";
								
								print "<tr>"
									. "    <td valign='middle' align='center'>"
									. "        <img src='/imagens/seta_filho.gif'>"
									. "	   </td>"
									. "	   <td colspan='5'>"
									. "		   <table align='center' cellspacing='1' cellpadding='4' style='border-collapse: collapse; border: 1px solid #ccc;' width='95%'>"
									. "			   <tr>"
									. "				   <td style='border: 1px solid #ccc;' class='subtitulocentro'>GND</td>"
									. "				   <td style='border: 1px solid #ccc;' class='subtitulocentro'>MOD</td>"
									. "				   <td style='border: 1px solid #ccc;' class='subtitulocentro'>Fonte</td>"
									. "				   <td style='border: 1px solid #ccc;' class='subtitulocentro'>Valor</td>"
									. "			   </tr>";
								
								$sql = "SELECT 
										  ed.emdid,
										  ed.emeid,
										  ed.gndcod||' - '||gn.gnddsc as gndcod, 
										  ed.mapcod||' - '||map.mapdsc as mapcod,
										  ed.foncod||' - '||fon.fondsc as foncod,
										  ede.edevalor,
										  ptede.edeid,
										  ptede.pedid,
										  ede.edeid as coddetalhe
										FROM emenda.emenda e 
										LEFT JOIN emenda.v_funcionalprogramatica funcp ON (funcp.acaid = e.acaid)
											AND funcp.acastatus = 'A'
										  	AND funcp.prgano = ".(($ptrid) ? '(SELECT ptrexercicio::varchar FROM emenda.planotrabalho WHERE ptrid = '.$ptrid.')' : '\''.$_SESSION["exercicio"].'\'')."
										INNER JOIN emenda.emendadetalhe ed ON (e.emeid = ed.emeid) 
										INNER JOIN emenda.emendadetalheentidade ede ON (ed.emdid = ede.emdid)
										LEFT JOIN (Select ped.pedid, ped.edeid, ped.ptrid
													From emenda.ptemendadetalheentidade ped
														inner join emenda.planotrabalho ptr
															ON (ptr.ptrid = ped.ptrid) ) ptede
										ON (ede.edeid = ptede.edeid) 
										left join public.gnd gn
										    on gn.gndcod = ed.gndcod
										    and gn.gndstatus = 'A'
										left join public.modalidadeaplicacao map
										    on map.mapcod = ed.mapcod
										left join public.fonterecurso fon
										    on fon.foncod = ed.foncod 
										    and fon.fonstatus = 'A' 
										WHERE
										  ed.emeid = {$arEmenda[$i]["emeid"]}
										  AND (".(($ptrid) ? "ptede.ptrid = ".$ptrid." OR ptede.ptrid is null" : "ptede.ptrid is null").")
										  AND ede.edestatus = 'A'
										  AND ede.ededisponivelpta = 'S'
										  and ede.enbid = ".(($ptrid) ? '(SELECT enbid FROM emenda.planotrabalho WHERE ptrid = '.$ptrid.')' : $_SESSION["emenda"]["enbid"]);
										  
								

								$arEmendaD = $db->carregar($sql);
								
								for( $j = 0; $j < count( $arEmendaD ); $j++ ){
									$emdvalorTotal += $arEmendaD[$j]["edevalor"]; 
									print "<tr>"
										. "    <td style='border: 1px solid #ccc;'  align='center'>{$arEmendaD[$j]["gndcod"]}</td>"
										. "    <td style='border: 1px solid #ccc;'  align='center'>{$arEmendaD[$j]["mapcod"]}</td>"
										. "    <td style='border: 1px solid #ccc;'  align='center'>{$arEmendaD[$j]["foncod"]}</td>"
										. "    <td style='border: 1px solid #ccc;'  align='center'>R$ " 
										. 		   number_format( $arEmendaD[$j]["edevalor"], 2, ",", "." )
										. "    </td>"
										. "</tr>";
									
								}
								print "        </table>"
									. "    </td>"
									. "</tr>";
							}
								print "<tr>"
									. "    <td valign='middle' class='subtitulodireita' style='border: 1px solid #ccc;'>"
									. "        Valor total do Concedente" 
									. "	   </td>"
									. "	   <td colspan='5' style='border: 1px solid #ccc;'>R$ "
									. 		   number_format( $emdvalorTotal, 2, ',', '.' )
									. "	   </td>"
									. "</tr>";
									//$_SESSION['emenda']['emdvalorTotal']
						
						?>
						
						
					</table>
				</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;" bgcolor="#dcdcdc"><b>PROPONENTE</b></td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;">
					<b>VALOR</b> R$ <?php echo $ptrvalorproponente; ?>
				</td>
			</tr>
		</table>
		
		<br/>
		
		<table align="center" cellspacing="1" cellpadding="4" style="border-collapse: collapse; border: 1px solid #ccc;" width="95%">
			<tr>
				<td class="subtitulocentro" colspan="5">INICIATIVAS</td>
			</tr>
			
			<?php 
				
				/* DADOS DAS INICIATIVAS */
			
				$sql = "SELECT
						  pti.ptiid as codigo,
						  ini.ininome as descricao,
						  pti.ptivalortotal,
						  pti.ptivalorconcedente,
						  pti.ptivalorproponente,
						  ptidescricao,
						  tpe.tpedsc
						FROM
						  emenda.v_ptiniciativa pti 
						 INNER JOIN emenda.iniciativa ini
						  ON (pti.iniid = ini.iniid)
						 INNER JOIN emenda.tipoensino tpe 
						  ON (tpe.tpeid = pti.tpeid)
						WHERE
						  pti.ptrid = {$ptrid}";
			
				$dadosIniciativas = $db->carregar($sql);

				for( $i = 0; $i < count($dadosIniciativas); $i++ ){
					
					$dadosIniciativas[$i]["ptivalortotal"]      = number_format( $dadosIniciativas[$i]["ptivalortotal"], 2, ",", "." );
					$dadosIniciativas[$i]["ptivalorconcedente"] = number_format( $dadosIniciativas[$i]["ptivalorconcedente"], 2, ",", "." );
					$dadosIniciativas[$i]["ptivalorproponente"] = number_format( $dadosIniciativas[$i]["ptivalorproponente"], 2, ",", "." );
					
					print "<tr>"
						. "    <td colspan='4'>&nbsp;</td>"
						. "</tr>"
						. "<tr>"
						. "    <td style='border: 1px solid #ccc;' class='subtitulocentro' colspan='4'>"
						. "	       {$dadosIniciativas[$i]["descricao"]}"
						. "    </td>"
						. "</tr>"
						. "<tr>"
						. "    <td style='border: 1px solid #ccc;'>"
						. "	       <b>TOTAL</b><br/>R$ {$dadosIniciativas[$i]["ptivalortotal"]}"
						. "    </td>"
						. "    <td style='border: 1px solid #ccc;'>"
						. "	       <b>TOTAL DO PROPONENTE</b><br/>R$ {$dadosIniciativas[$i]["ptivalorproponente"]}"
						. "    </td>"
						. "    <td style='border: 1px solid #ccc;'>"
						. "	       <b>TOTAL DO CONCEDENTE</b><br/>R$ {$dadosIniciativas[$i]["ptivalorconcedente"]}"
						. "    </td>"
						. "    <td style='border: 1px solid #ccc;'>"
						. "	       <b>NÍVEL DE ENSINO</b><br/>{$dadosIniciativas[$i]["tpedsc"]}"
						. "    </td>"
						. "</tr>";
					
					
					
					
					

					print "<tr>"
						. "    <td style='border: 1px solid #ccc;' align='justify' colspan='4'>"
						. "	       <b>DETALHAMENTO DA INICIATIVA</b><br/>{$dadosIniciativas[$i]["ptidescricao"]}"
						. "    </td>"
						. "</tr>";

					$sql = "SELECT 
						  	  e.espnome,
							  --e.espkit,
							  e.espunidademedida,
							  ptie.ptequantidade,
							  ptie.ptevalorunitario,
							  (ptie.ptequantidade * ptie.ptevalorunitario) as total,
							  ptie.ptevalorproponente,
							  ( CASE WHEN ptie.ptevalorproponente is null THEN (ptie.ptequantidade * ptie.ptevalorunitario)
							         ELSE ((ptie.ptequantidade * ptie.ptevalorunitario) - ptie.ptevalorproponente) END) as concedente,
							  to_char(ptie.ptedatainicio, 'DD/MM/YYYY') as dataini,
							  to_char(ptie.ptedatafim, 'DD/MM/YYYY') as datafim,
							  ptie.pteid
							FROM 
							  emenda.ptiniciativaespecificacao ptie INNER JOIN emenda.iniciativaespecificacao ie ON (ptie.iceid = ie.iceid) 
							  INNER JOIN emenda.especificacao e ON (ie.espid = e.espid)
							  inner join emenda.especificacao_programacaoexercicio epe on epe.espid = e.espid and epe.prsano = '".$_SESSION['exercicio']."'
							WHERE 
							    ptie.ptestatus = 'A'
							    AND ptie.ptiid = {$dadosIniciativas[$i]["codigo"]}
							ORDER BY
								ptie.pteid";
					
					$dados = $db->carregar($sql);
						
					print "<tr>"
						. "    <td style='border: 1px solid #ccc;' align='justify' colspan='4'>"
						. "	       <b>ESPECIFICAÇÕES DA INICIATIVA</b><br/>";

					print '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';
					
					if($dados) {
						print '<thead><tr>'
							. '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Especificações da Iniciativa</label>'
							. '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Unidade de Medida</label>'
							. '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Quantidade</label>'
							. '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Valor Unitário</label>'
							. '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Valor Total</label>'
							. '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Valor Proponente</label>'
							. '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Valor Concedente</label>'
							. '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Data Inicial</label>'
							. '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Data Final</label>'
							. '</tr></thead>'
							. '<tbody>';
							
						$totalQtd = 0;
						$totalValorUnitario = 0;
						$totalValorTotal = 0;
						$totalValorProponente = 0;
						$totalValorConcedente = 0;
						
						for($j=0; $j<count($dados); $j++) {
							$marcado = (fmod($j,2) == 0) ? '' : '#F7F7F7';
							
							$totalQtd += $dados[$j]["ptequantidade"];
							$totalValorUnitario += $dados[$j]["ptevalorunitario"];
							$totalValorTotal += $dados[$j]["total"];
							$totalValorProponente += $dados[$j]["ptevalorproponente"];
							$totalValorConcedente += $dados[$j]["concedente"];
							
							print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">'
								. '<td>'.$dados[$j]["espnome"].'</td>'
								. '<td>'.$dados[$j]["espunidademedida"].'</td>'
								. '<td align="right" style="color:#999999;">'.$dados[$j]["ptequantidade"].'</td>'
								. '<td align="right" style="color:#999999;">'.number_format($dados[$j]["ptevalorunitario"], 2, ',', '.').'</td>'
								. '<td align="right" style="color:#999999;">'.number_format($dados[$j]["total"], 2, ',', '.').'</td>'
								. '<td align="right" style="color:#999999;">'.number_format($dados[$j]["ptevalorproponente"], 2, ',', '.').'</td>'
								. '<td align="right" style="color:#999999;">'.number_format($dados[$j]["concedente"], 2, ',', '.').'</td>'
								. '<td align="center">'.$dados[$j]["dataini"].'</td>'
								. '<td align="center">'.$dados[$j]["datafim"].'</td>'
								. '</tr>';
								
							$sql = "SELECT ptkid, pteid, iteid, itoid, ptkdescricao, ptkunidademedida, ptkquantidade, ptkvalorunitario, total FROM (
										SELECT
														  ptk.ptkid,
														  ptk.pteid,
														  ptk.iteid,
										                  ptk.itoid,
										                  case when pic.picdescricao is not null then pic.picdescricao else ptk.ptkdescricao end as ptkdescricao,
														  ptk.ptkunidademedida,
														  ptk.ptkquantidade,
														  ptk.ptkvalorunitario,
														  (ptk.ptkquantidade * ptk.ptkvalorunitario) as total
														FROM 
														  emenda.ptitemkit ptk
										                  left join emenda.itempar_especificacao ipe on ipe.iteid = ptk.iteid
										                  left join emenda.itempar itp on itp.ipaid = ipe.ipaid
										                  left join par.propostaitemcomposicao pic on pic.picid = itp.picid
										UNION
											SELECT
														  ptk.ptkid,
														  ptk.pteid,
														  ptk.iteid,
										                  ptk.itoid,
										                  case when pto.ptodescricao is not null then pto.ptodescricao else ptk.ptkdescricao end as ptkdescricao,
														  ptk.ptkunidademedida,
														  ptk.ptkquantidade,
														  ptk.ptkvalorunitario,
														  (ptk.ptkquantidade * ptk.ptkvalorunitario) as total
														FROM 
														  emenda.ptitemkit ptk
										                  left join emenda.itempar_especificacao ipe on ipe.itoid = ptk.itoid
										                  left join emenda.itemobras io on io.itoid = ipe.itoid
										                  left join obras.pretipoobra pto on pto.ptoid = io.ptoid
										) as foo
										WHERE 
											pteid = {$dados[$j]["pteid"]}
											and ptkdescricao is not null
										ORDER BY ptkdescricao";
							$kits = $db->carregar($sql);
							
							if($kits) {
								print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">'
									. '<td align="center" valign="center"><img src="/imagens/seta_filho.gif" /></td>'
									. '<td colspan="8">'
									. '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">'
									. '<thead><tr>'
									. '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Descrição</label>'
									. '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Unidade de Medida</label>'
									. '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Quantidade</label>'
									. '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Valor Unitário</label>'
									. '</tr></thead>'
									. '<tbody>';
									
								for($k=0; $k<count($kits); $k++) {
									$marcado = (fmod($k,2) == 0) ? '' : '#F7F7F7';
									
									print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">'
										. '<td>'.$kits[$k]["ptkdescricao"].'</td>'
										. '<td>'.$kits[$k]["ptkunidademedida"].'</td>'
										. '<td align="right" style="color:#999999;">'.$kits[$k]["ptkquantidade"].'</td>'
										. '<td align="right" style="color:#999999;">'.number_format($kits[$k]["ptkvalorunitario"], 2, ',', '.').'</td>'
										. '</tr>';
								}
								
								print '</tbody></table>'
									. '</td></tr>';
							}
						}
							
						print '</tbody>'
							. '<tfoot><tr bgcolor="c0c0c0">'
							. '<td align="left" colspan="2"><b>Totais:</b></td>'
							. '<td align="right">'.$totalQtd.'</td>'
							. '<td align="right">'.number_format($totalValorUnitario, 2, ',', '.').'</td>'
							. '<td align="right">'.number_format($totalValorTotal, 2, ',', '.').'</td>'
							. '<td align="right">'.number_format($totalValorProponente, 2, ',', '.').'</td>'
							. '<td align="right">'.number_format($totalValorConcedente, 2, ',', '.').'</td>'
							. '<td align="center">-</td>'
							. '<td align="center">-</td>'
							. '</tr></tfoot>';
					} 
					else {
						print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
					}
						
					print '</table>';
						
						
					/*$cabecalho = array( "Especificações da Iniciativa", "Unidade de Medida", "Quantidade", "Valor Unitário", 
										"Valor Total", "Valor Proponente", "Valor Concedente", "Data Inicial", "Data Final" );
					$db->monta_lista_simples( $sql, $cabecalho, 100, 10, 'S', '100%', 'S', false );*/
						
					print "    </td>"
						. "</tr>";
	
					// BENEFICIARIOS
					$sql = "select * from (
							SELECT DISTINCT
							  ben.bennome as beneficiario,
							  ptib.ptbquantidaderural as rural,
							  ptib.ptbquantidadeurbana as urbana,
							  (ptib.ptbquantidaderural + ptib.ptbquantidadeurbana) as total
							FROM
							  emenda.ptiniciativa pti INNER JOIN emenda.ptiniciativabeneficiario ptib 
							  ON (pti.ptiid = ptib.ptiid) RIGHT JOIN emenda.iniciativabeneficiario inb
							  ON (ptib.icbid = inb.icbid) INNER JOIN emenda.beneficiario ben
							  ON (inb.benid = ben.benid)
							WHERE
							  inb.icbstatus = 'A'
							  AND ben.benstatus = 'A' ".($dadosIniciativas[$i]["codigo"] ? " AND pti.ptiid = {$dadosIniciativas[$i]["codigo"]} " : "")."
						    union
						    SELECT DISTINCT
							  ben.bennome as beneficiario,
							  ptib.ptbquantidaderural as rural,
							  ptib.ptbquantidadeurbana as urbana,
							  (ptib.ptbquantidaderural + ptib.ptbquantidadeurbana) as total
							FROM
							  emenda.ptiniciativa pti INNER JOIN emenda.ptiniciativabeneficiario ptib 
							  ON (pti.ptiid = ptib.ptiid) RIGHT JOIN emenda.iniciativabeneficiario inb
							  ON (ptib.icbid = inb.icbid) INNER JOIN emenda.beneficiario ben
							  ON (inb.benid = ben.benid)
							WHERE
							  ".($dadosIniciativas[$i]["codigo"] ? " pti.ptiid = {$dadosIniciativas[$i]["codigo"]} " : "")."
						    ) as t
						    ORDER BY beneficiario";
					
					if( $boMostra ){
						print "<tr>"
							. "    <td style='border: 1px solid #ccc;' colspan='4'>"
							. "	       <b>BENEFICIÁRIOS</b><br/>";
							
							$cabecalho = array( "", "Rural", "Urbana", "Total" );
							$db->monta_lista_simples( $sql, $cabecalho, 100, 10, 'S', '100%', 'N', false );
							
						print "    </td>"
							. "</tr>";
					}
				}
				
			?>
				
		</table>
		
		<br/>
		
		<?php 
		
			$sql = "SELECT
						to_char(min(pte.ptedatainicio), 'MM/YYYY') as inicial,
						to_char(max(pte.ptedatafim), 'MM/YYYY') as final
					FROM
						emenda.ptiniciativaespecificacao pte
					INNER JOIN
						emenda.ptiniciativa pti ON pti.ptiid = pte.ptiid
											   AND pti.ptrid = {$ptrid}
					WHERE
						ptestatus = 'A'";

			$execucao = $db->pegaLinha( $sql );
		
		?>
		
		<table align="center" cellspacing="1" cellpadding="4" style="border-collapse: collapse; border: 1px solid #ccc;" width="95%">
			<tr>
				<td class="subtitulocentro" colspan="2">CRONOGRAMA DE EXECUÇÃO E DESEMBOLSO</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;" bgcolor="#dcdcdc" colspan="2"><b>CRONOGRAMA DE EXECUÇÃO</b></td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;">
					<b>MÊS/ANO INICIAL</b><br/>
					<?php echo $execucao["inicial"]; ?>
				</td>
				<td style="border: 1px solid #ccc;">
					<b>MÊS/ANO FINAL</b><br/>
					<?php echo $execucao["final"]; ?>
				</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;" bgcolor="#dcdcdc" colspan="2"><b>DESEMBOLSO DO CONCEDENTE</b></td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;" colspan="2">
					<?php 
					
						$sql = "SELECT 
								to_char(prddata, 'MM/YYYY') as data,
								prdid
							FROM 
								emenda.ptparceladesembolso 
							WHERE 
								ptrid = {$ptrid} 
								AND prdtipo = 'C' 
								AND prdminuta = 'P'
							ORDER BY
								prddata ASC";
						
						$parcelasConcedente = $db->carregar($sql);
					
					?>
					
					<center>
						<table style="border-color:#eeeeee;" align="center" class="Tabela" cellpadding="5" cellspacing="0">
							<tr>
								<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Iniciativa</b></td>
								
								<?php
									for($i=0; $i<count($parcelasConcedente); $i++) {
										echo '<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>'.($i+1).'ª Parcela - '.$parcelasConcedente[$i]["data"].'</b></td>';
									}
								?>
								
								<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Total já informado para a iniciativa</b></td>
								<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Restante</b></td>
								<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Valor da Iniciativa (concedente)</b></td>
							</tr>
							
							<?php 
								
								$arrTotalParcelas 		= array();
								$totalTotalParcelas 	= 0;
								$totalRestante 			= 0;
								$totalValorIniciativa	= 0;
								
								for($i=0; $i<count($dadosIniciativas); $i++) {
									echo '<tr>';
									echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">'.$dadosIniciativas[$i]['descricao'].'</td>';
									
									$sql = "SELECT
												pti.ptiid,
												pri.priid,
												pri.privalor,
												pti.ptivalorconcedente
											FROM
												emenda.v_ptiniciativa pti
											INNER JOIN
												emenda.ptparcelainiciativa pri ON pri.ptiid = pti.ptiid
											INNER JOIN
												emenda.ptparceladesembolso prd ON prd.prdid = pri.prdid
																			  AND prd.prdtipo = 'C'
																			  AND prdminuta = 'P'
											WHERE
												pti.ptiid = ".$dadosIniciativas[$i]["codigo"]."
											ORDER BY
												prddata ASC";
									$valorConcedente = $db->carregar($sql);
									
									//echo '<script> id_ptiid.push('.$valorConcedente[0]["ptiid"].'); </script>';
									
									$totalParcelas = 0;
									for($k=0; $k<count($valorConcedente); $k++) {
										echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
												R$ '.number_format($valorConcedente[$k]["privalor"],2,",",".").'
											  </td>';
										
										$totalParcelas 			+= $valorConcedente[$k]["privalor"];
										$arrTotalParcelas[$k]	+= $valorConcedente[$k]["privalor"];
										
										$restante = ($valorConcedente[$k]["ptivalorconcedente"] - $totalParcelas);
			
										$totalValorIniciativa	+= $valorConcedente[$k]["ptivalorconcedente"];
										
										$ptivalorconcedente		= $valorConcedente[$k]["ptivalorconcedente"];
										$ptiid					= $valorConcedente[$k]["ptiid"];
									}
									
									$totalTotalParcelas 	+= $totalParcelas;
									$totalRestante			+= $restante;
									
									echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">R$ '.number_format($totalParcelas,2,",",".").'</td>';
									echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">R$ '.number_format($restante,2,",",".").'</td>';
									echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">R$ '.number_format($ptivalorconcedente,2,",",".").'</td>';
									echo '</tr>';
									echo '<input type="hidden" id="total_C_'.$ptiid.'" value="'.(($ptivalorconcedente) ? $ptivalorconcedente : 0).'" />';
								}
								
								//total
								echo '<tr>';
								echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: right;" class="SubTituloDireita"><b>Total:</b></td>';
								for($i=0; $i<count($arrTotalParcelas); $i++) {
									echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($arrTotalParcelas[$i],2,",",".").'</b></td>';
								}
								echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($totalTotalParcelas,2,",",".").'</b></td>';
								echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($totalRestante,2,",",".").'</b></td>';
								echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($ptivalorconcedente,2,",",".").'</b></td>';
								echo '</tr>';
					
							?>
							
						</table>
					</center>
				</td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;" bgcolor="#dcdcdc" colspan="2"><b>DESEMBOLSO DO PROPONENTE</b></td>
			</tr>
			<tr>
				<td style="border: 1px solid #ccc;" colspan="2">
					<?php

						$sql = "SELECT 
									to_char(prddata, 'MM/YYYY') as data,
									prdid
								FROM 
									emenda.ptparceladesembolso 
								WHERE 
									ptrid = ".$ptrid." AND 
									prdtipo = 'P'
									AND prdminuta = 'P'
								ORDER BY
									prddata ASC";
						
							$parcelasProponente = $db->carregar($sql);
					
					?>
					<center>
						<table style="border-color:#eeeeee;" align="center" class="Tabela" cellpadding="5" cellspacing="0">
							<tr>
								<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Iniciativa</b></td>
								
								<?php
									for($i=0; $i<count($parcelasProponente); $i++) {
										echo '<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>'.($i+1).'ª Parcela - '.$parcelasProponente[$i]["data"].'</b></td>'; 
									}
								?>
								
								<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Total já informado para a iniciativa</b></td>
								<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Restante</b></td>
								<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Valor da Iniciativa (proponente)</b></td>
							</tr>
							<?php
							
								$arrTotalParcelas 		= array();
								$totalTotalParcelas 	= 0;
								$totalRestante 			= 0;
								$totalValorIniciativa	= 0;
								
								for($i=0; $i<count($dadosIniciativas); $i++) {
									echo '<tr>';
									echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">'.$dadosIniciativas[$i]['descricao'].'</td>';
									
									$sql = "SELECT
												pti.ptiid,
												pri.priid,
												pri.privalor,
												pti.ptivalorproponente
											FROM
												emenda.v_ptiniciativa pti
											INNER JOIN
												emenda.ptparcelainiciativa pri ON pri.ptiid = pti.ptiid
											INNER JOIN
												emenda.ptparceladesembolso prd ON prd.prdid = pri.prdid
																			  AND prd.prdtipo = 'P'
																			  AND prdminuta = 'P'
											WHERE
												pti.ptiid = ".$dadosIniciativas[$i]["codigo"]."
											ORDER BY
												prddata ASC";
									$valorProponente = $db->carregar($sql);
									
									$totalParcelas = 0;
									for($k=0; $k<count($valorProponente); $k++) {
										echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
												R$ '.number_format($valorProponente[$k]["privalor"],2,",",".").'
											  </td>';
										
										$totalParcelas 			+= $valorProponente[$k]["privalor"];
										$arrTotalParcelas[$k]	+= $valorProponente[$k]["privalor"];
										
										$restante = ($valorProponente[$k]["ptivalorproponente"] - $totalParcelas);
										
										$totalValorIniciativa	+= $valorProponente[$k]["ptivalorproponente"];
										
										$ptivalorproponente = $valorProponente[$k]["ptivalorproponente"];
										$ptiid				= $valorProponente[$k]["ptiid"];
									}
									
									$totalTotalParcelas 	+= $totalParcelas;
									$totalRestante			+= $restante;
									
									echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">R$ '.number_format($totalParcelas,2,",",".").'</td>';
									echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">R$ '.number_format($restante,2,",",".").'</td>';
									echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">R$ '.number_format($ptivalorproponente,2,",",".").'</td>';
									echo '</tr>';
									echo '<input type="hidden" id="total_P_'.$ptiid.'" value="'.(($ptivalorproponente) ? $ptivalorproponente : 0).'" />';
								}
								
								//total
								echo '<tr>';
								echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: right;" class="SubTituloDireita"><b>Total:</b></td>';
								for($i=0; $i<count($arrTotalParcelas); $i++) {
									echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($arrTotalParcelas[$i],2,",",".").'</b></td>';
								}
								echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($totalTotalParcelas,2,",",".").'</b></td>';
								echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($totalRestante,2,",",".").'</b></td>';
								echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($ptivalorproponente,2,",",".").'</b></td>';
								echo '</tr>';
							?>
							
						</table>
					</center>
					
				</td>
			</tr>
		</table>
		
		<br/>
		
		<?php

			/* ESCOLAS BENEFICIADAS */
		
			$sql = "SELECT
						ent.entcodent,
					    ent.entnome, 
					    peb.esbquantidadealunos
					FROM
						emenda.ptescolasbeneficiadas peb
					    inner join entidade.entidade ent
					    	on ent.entid = peb.entid
					    inner join entidade.funcaoentidade fent
					        ON fent.entid = ent.entid
					WHERE
						ent.entstatus = 'A'
					    and fent.funid in(3,4) 
					    and peb.ptrid = {$ptrid}";
		if( $boMostra ){		
		?>		
		<table align="center" cellspacing="1" cellpadding="4" style="border-collapse: collapse; border: 1px solid #ccc;" width="95%">
			<tr>
				<td class="subtitulocentro">ESCOLAS BENEFICIADAS</td>
			</tr>
			<tr>
				<td>
					<?php 
						$cabecalho = array( "Código Censo Escolar - INEP", "Nome da Escola", "Alunos Beneficiados" );
						$db->monta_lista_simples( $sql, $cabecalho, 100, 10, 'N', '100%', 'N', false );
					?>
				</td>
			</tr>
		</table>
		<?} ?>
		
		<br/>
		
	</body>
	<table width="95%" align="center">
		<tr>
			<td>
				<input type="button" value="Imprimir" onclick="self.print();" style="cursor: pointer;">
			</td>
		</tr>
	</table>
</html>