<?php

/* configurações */
ini_set("memory_limit", "3000M");
set_time_limit(30000);
/* FIM configurações */

$_REQUEST['baselogin'] = "simec_espelho_producao";
//$_REQUEST['baselogin'] = "simec_desenvolvimento";

// carrega as funções gerais
include_once "/var/www/simec/global/config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "www/obras/_constantes.php";


// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<?php
		/** SQL que recupera a Lista de Obras que estão na Situação: "Em Supervisão", "Em Avaliação de Supervisão(MEC)", "Ajuste de Supervisão(Empresa)", 
		 * "Reavaliação da Supervisão(MEC)" ou "Reajuste da supervisão (Empresa)", que estão desatualizadas de acordo com a sinalização: "Vermelho ou Amarelo".
		 */
		$sql = " SELECT DISTINCT
						ig.gpdid AS grupo,
						' ( Id: '||oi.obrid||' ) Obra: '||oi.obrdesc  AS obra,
						tm.mundescricao AS municipio,
						ee.entnome AS nome_da_entidade,
						'<FONT '||
							 /*Situação: Em Supervisão*/
						CASE WHEN e.esdid = ".OBREMSUPERVISAOIND." AND DATE_PART('days', NOW() - (MAX(h.htddata))::timestamp) > 20 AND DATE_PART('days', NOW() - (MAX(h.htddata))::timestamp) <= 30 THEN 'COLOR=\"#BB9900\" />'||e.esddsc
						     WHEN e.esdid = ".OBREMSUPERVISAOIND." AND DATE_PART('days', NOW() - (MAX(h.htddata))::timestamp) > 30 THEN 'COLOR=\"#DD0000\" />'||e.esddsc
						     /*Situação: Em Avaliação de Supervisão(MEC)*/
						     WHEN e.esdid = ".OBRAAVALIACAOSUPERVISAO_MEC." AND DATE_PART('days', NOW() - (MAX(h.htddata))::timestamp) > 15 AND DATE_PART('days', NOW() - (MAX(h.htddata))::timestamp) <= 20 THEN 'COLOR=\"#BB9900\" />'||e.esddsc 
						     WHEN e.esdid = ".OBRAAVALIACAOSUPERVISAO_MEC." AND DATE_PART('days', NOW() - (MAX(h.htddata))::timestamp) > 20 THEN 'COLOR=\"#DD0000\" />'||e.esddsc
						     /*Situação: Ajuste de Supervisão(Empresa)*/ 
						     WHEN e.esdid = ".OBRAAJUSTESUPERVISAO_EMPRESA." AND DATE_PART('days', NOW() - (MAX(h.htddata))::timestamp) > 5  AND DATE_PART('days', NOW() - (MAX(h.htddata))::timestamp) <= 10 THEN 'COLOR=\"#BB9900\" />'||e.esddsc
						     WHEN e.esdid = ".OBRAAJUSTESUPERVISAO_EMPRESA." AND DATE_PART('days', NOW() - (MAX(h.htddata))::timestamp) > 10	THEN 'COLOR=\"#DD0000\" />'||e.esddsc
						     /*Situação: Reavaliação da Supervisão(MEC)*/
						     WHEN e.esdid = ".OBRAREAVALIACAOSUPERVISAO_MEC." AND DATE_PART('days', NOW() - (MAX(h.htddata))::timestamp) > 7  AND DATE_PART('days', NOW() - (MAX(h.htddata))::timestamp) <= 10 THEN 'COLOR=\"#BB9900\" />'||e.esddsc 
						     WHEN e.esdid = ".OBRAREAVALIACAOSUPERVISAO_MEC." AND DATE_PART('days', NOW() - (MAX(h.htddata))::timestamp) > 10 THEN 'COLOR=\"#DD0000\" />'||e.esddsc
						     /*Situação: Reajuste da supervisão (Empresa)*/
						     WHEN e.esdid = ".OBRAREAJUSTESUPERVISAO_EMPRESA." AND DATE_PART('days', NOW() - (MAX(h.htddata))::timestamp) > 5  AND DATE_PART('days', NOW() - (MAX(h.htddata))::timestamp) <= 10 THEN 'COLOR=\"#BB9900\" />'||e.esddsc
						     WHEN e.esdid = ".OBRAREAJUSTESUPERVISAO_EMPRESA." AND DATE_PART('days', NOW() - (MAX(h.htddata))::timestamp) > 10	THEN 'COLOR=\"#DD0000\" />'||e.esddsc 
						END||'</FONT>' AS situacoes_obras_em_alerta,
						MAX(TO_CHAR(h.htddata,'DD/MM/YYYY')) AS data_tramitacao,
						DATE_PART('days', NOW() - (MAX(h.htddata))::timestamp)||' dia(s)' AS dias_apos_tramitacao
				  FROM
						obras.itemgrupo ig
				  INNER JOIN
						obras.repositorio ore ON ore.repid = ig.repid
				  INNER JOIN
						obras.obrainfraestrutura oi ON oi.obrid = ore.obrid
				  INNER JOIN
						entidade.entidade ee ON ee.entid = oi.entidunidade
				  INNER JOIN
						entidade.endereco ed ON ed.endid = oi.endid
				  INNER JOIN
						territorios.municipio tm ON tm.muncod = ed.muncod
			      INNER JOIN 
						workflow.documento d ON d.docid = oi.docid
				  INNER JOIN 
						workflow.estadodocumento e ON e.esdid = d.esdid
												   AND e.esdid IN (".OBREMSUPERVISAOIND.", ".OBRAAVALIACAOSUPERVISAO_MEC.", ".OBRAAJUSTESUPERVISAO_EMPRESA.", ".OBRAREAVALIACAOSUPERVISAO_MEC.", ".OBRAREAJUSTESUPERVISAO_EMPRESA.")
				  INNER JOIN 
						workflow.historicodocumento h ON h.docid = oi.docid
				  INNER JOIN
						obras.grupodistribuicao gd ON ig.gpdid = gd.gpdid 
												   AND gd.gpdstatus = 'A'	
				  INNER JOIN 
						workflow.documento wd ON wd.docid = gd.docid
				  INNER JOIN 
						workflow.estadodocumento we ON we.esdid = wd.esdid 
													AND we.esdstatus = 'A' 
													AND we.esdid <> ". OBRSUPFINALIZADA ."			
				  WHERE
						ore.repsitsupervisao <> ''
						AND oi.obsstatus = 'A'
						AND ( 
						      e.esdid = ".OBREMSUPERVISAOIND." AND ((DATE_PART('days', NOW() - h.htddata)) > 20 AND (DATE_PART('days', NOW() - h.htddata)) <= 30  OR (DATE_PART('days', NOW() - h.htddata)) > 30)
							 OR 
						      e.esdid = ".OBRAAVALIACAOSUPERVISAO_MEC." AND ((DATE_PART('days', NOW() - h.htddata)) > 15 AND (DATE_PART('days', NOW() - h.htddata)) <= 20  OR (DATE_PART('days', NOW() - h.htddata)) > 20)
							 OR 
						      e.esdid = ".OBRAAJUSTESUPERVISAO_EMPRESA." AND ((DATE_PART('days', NOW() - h.htddata)) > 5  AND (DATE_PART('days', NOW() - h.htddata)) <= 10  OR (DATE_PART('days', NOW() - h.htddata)) > 10)
							 OR 
						      e.esdid = ".OBRAREAVALIACAOSUPERVISAO_MEC." AND ((DATE_PART('days', NOW() - h.htddata)) > 7  AND (DATE_PART('days', NOW() - h.htddata)) <= 10  OR (DATE_PART('days', NOW() - h.htddata)) > 10)
							 OR 
						      e.esdid = ".OBRAREAJUSTESUPERVISAO_EMPRESA." AND ((DATE_PART('days', NOW() - h.htddata)) > 5  AND (DATE_PART('days', NOW() - h.htddata)) <= 10  OR (DATE_PART('days', NOW() - h.htddata)) > 10)
						    )
				  GROUP BY
						grupo,
						obra,
						municipio,
						nome_da_entidade,
						e.esddsc,
						e.esdid	
				  ORDER BY
						 5";
		
		$dados = (array) $db->carregar( $sql );
	 
		if ( is_array($dados[0]) ){
			$htmObras  = "<style type='text/css'>				
						body{
							font-size: 1em;
							font-family: Arial;
						}
						
						table{
							font-size: 0.8em;
						}
					
						div.scrollTable{
							background: #fff;
							/*border: 1px solid #888;*/
						}
					
						div.scrollTable table.header, div.scrollTable div.scroller table{
							width: 100%;
							border-collapse: collapse;
						}
						
						div.scrollTable table.header th, div.scrollTable div.scroller table td{
							/*border: 1px solid #444;*/
							padding: 3px 5px;
						}
						
						div.scrollTable table.header th{
							background: #ddd;
						}
					
						div.scrollTable div.scroller{
							height: 200px;
							overflow: scroll;
						}
					
						div.scrollTable .coluna75px{
							width: 75px;
						}
					
						div.scrollTable .coluna100px{
							width: 100px;
						}
					
						div.scrollTable .coluna150px{
							width: 150px;
						}
					</style>
					 <center><h2>Lista de Obras</h2></center>
						<table cellpadding=\"1\" cellspacing=\"1\" style=\"border:2px #BEBEBE solid\"> 
							<tbody>
								<tr bgcolor=\"#e7e7e7\">
									<th class='coluna100px' align=\"center\">Grupo</th>
									<th class='coluna100px' align=\"center\">Obra</th>
									<th class='coluna100px' align=\"center\">Município/UF</th>
									<th class='coluna100px' align=\"center\">Entidade</th>
									<th class='coluna100px' align=\"center\">Situação da Supervisão</th>
									<th class='coluna100px' align=\"center\">Data da Tramitação</th>
									<th class='coluna100px' align=\"center\">Dias após a Tramitação</th>
								</tr>
							</tbody>
							<tbody>";
			foreach($dados as $dado){
				$cor = ( ($i % 2 == 0) ? "bgcolor=\"\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#ffffcc';\"" : "bgcolor=\"#f7f7f7\" onmouseout=\"this.bgColor='#F7F7F7';\" onmouseover=\"this.bgColor='#ffffcc';\"" );
				$htmObras .= "<tr {$cor}>
									<td class='coluna100px' align=\"center\">{$dado['grupo']}</td>
									<td class='coluna100px' >{$dado['obra']}</td>
									<td class='coluna100px' >{$dado['municipio']}</td>
									<td class='coluna100px' >{$dado['nome_da_entidade']}</td>
									<td class='coluna100px' >{$dado['situacoes_obras_em_alerta']}</td>
									<td class='coluna100px' align=\"center\">{$dado['data_tramitacao']}</td>
									<td class='coluna100px' align=\"center\">{$dado['dias_apos_tramitacao']}</td>
							  </tr>";
				$i++;
			}
			$htmObras  .= "</tbody>
					</table>
					<table cellpadding=\"1\" cellspacing=\"1\" style=\"border:1px #BEBEBE solid\" align=\"center\" width=\"100%\">
						<tbody>
							<tr bgcolor=\"#ffffff\">
								<td>
									<b>Total de Registros: ". count($dados)."</b>
								</td>
								<td>
								</td>
							</tr>
						</tbody>
					</table>";
		}else{
		
			$htmObras  = " <center><h2>Lista do Grupos de Obras</h2></center>
								<table cellpadding=\"1\" cellspacing=\"1\" style=\"border:1px #BEBEBE solid\" align=\"center\" width=\"100%\">
									<tbody>
										<tr bgcolor=\"#e7e7e7\">
											<th class='coluna100px' align=\"center\">Grupo</th>
											<th class='coluna100px' align=\"center\">Obra</th>
											<th class='coluna100px' align=\"center\">Município/UF</th>
											<th class='coluna100px' align=\"center\">Entidade</th>
											<th class='coluna100px' align=\"center\">Situação da Supervisão</th>
											<th class='coluna100px' align=\"center\">Data da Tramitação</th>
											<th class='coluna100px' align=\"center\">Dias após a Tramitação</th>
											<th class='coluna10px' align=\"center\">Situação da Obra</th>
										</tr>
									</tbody>
								</table>
								<table cellpadding=\"1\" cellspacing=\"1\" style=\"border:1px #BEBEBE solid\" align=\"center\" width=\"100%\">	
									<tbody>
										<tr>
											<td align=\"center\" style=\"color: rgb(204, 0, 0);\"><b>Não foram encontrados Registros.</b></td>
										</tr>
									</tbody>
								</table>";
		}

		/** SQL que recupera a Lista dos Grupos de Obras que estão na Situação: "Grupo em Supervisão" ou "Aguardando Início de Supervisão pela Empresa",
		 *  que estão desatualizadas de acordo com a sinalização: "Vermelho ou Amarelo".
		 */
	    $sql = " SELECT DISTINCT
						unidade_federativa,
						numero_controle,
						empresa,
						total_de_obras,
						responsavel,
						dtinclusao,
						situacao,
						datramitacao,
						qtddias,
						datatotal
				 FROM
			      		(
							SELECT DISTINCT
									gd.estuf AS unidade_federativa,
									gd.gpdid AS numero_controle,
									CASE WHEN gd.epcid is not null THEN entnome ELSE 'Não Informada' END AS empresa,
									(SELECT 
											COUNT(ig.itgid)
									 FROM 
											obras.itemgrupo ig 
									 INNER JOIN 
											obras.repositorio ore ON ore.repid = ig.repid
									 INNER JOIN
											obras.obrainfraestrutura oi ON oi.obrid = ore.obrid
									 WHERE 
											ore.repsitsupervisao <> ''
											AND oi.obsstatus = 'A'
											AND ig.gpdid = gd.gpdid) AS total_de_obras,
									upper(usunome) AS responsavel,
									to_char(gpddtcriacao, 'DD/MM/YYYY') AS dtinclusao,
									/*Situação: Grupo em Supervisão*/
									CASE WHEN wd.esdid = 297  AND DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(MAX(wh.htddata), 'YYYY-mm-dd'))::timestamp) > 90
											 THEN DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(MAX(wh.htddata), 'YYYY-mm-dd'))::timestamp)
									     /* Situação: Aguardando Início de Supervisão pela Empresa */ 
									     WHEN wd.esdid = 216 AND DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(MAX(wh.htddata), 'YYYY-mm-dd'))::timestamp) <= 20
											 THEN DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(MAX(wh.htddata), 'YYYY-mm-dd'))::timestamp)
									     WHEN wd.esdid = 216 AND DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(MAX(wh.htddata), 'YYYY-mm-dd'))::timestamp) > 20 AND DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(MAX(wh.htddata), 'YYYY-mm-dd'))::timestamp) <= 24
											 THEN DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(MAX(wh.htddata), 'YYYY-mm-dd'))::timestamp)
									     WHEN wd.esdid = 216 AND DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(MAX(wh.htddata), 'YYYY-mm-dd'))::timestamp) > 24
											 THEN DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(MAX(wh.htddata), 'YYYY-mm-dd'))::timestamp)
									END AS dias_de_grupos_desatualizados ,     
										 /* As demais Situações do Grupo */
									'<FONT '||
									/*Situação: Grupo em Supervisão*/
									CASE WHEN wd.esdid = ".GRUPOEMSUPERVISAO."  AND DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(MAX(wh.htddata), 'YYYY-mm-dd'))::timestamp) > 90
											  THEN 'COLOR=\"#DD0000\" />'||we.esddsc
										 /* Situação: Aguardando Início de Supervisão pela Empresa */ 
										 WHEN wd.esdid = ".GRUPOAGUARDANDOINICIOSUPERVISAO." AND DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(MAX(wh.htddata), 'YYYY-mm-dd'))::timestamp) <= 20
											  THEN 'COLOR=\"#008000\" />'||we.esddsc
										 WHEN wd.esdid = ".GRUPOAGUARDANDOINICIOSUPERVISAO." AND DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(MAX(wh.htddata), 'YYYY-mm-dd'))::timestamp) > 20 AND DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(MAX(wh.htddata), 'YYYY-mm-dd'))::timestamp) <= 24
											  THEN 'COLOR=\"#BB9900\" />'||we.esddsc
										 WHEN wd.esdid = ".GRUPOAGUARDANDOINICIOSUPERVISAO." AND DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(MAX(wh.htddata), 'YYYY-mm-dd'))::timestamp) > 24
											  THEN 'COLOR=\"#DD0000\" />'||we.esddsc
										 ELSE/* As demais Situações do Grupo */
												CASE WHEN we.esddsc IS NOT NULL
													 	  THEN 'COLOR=\"#000000\" />'||we.esddsc
												END 
									END ||'</FONT>'AS situacao,
									'<center>'||to_char(MAX(wh.htddata), 'DD/MM/YYYY')||'</center>' AS datramitacao,
									'<center>'||DATE_PART('days', NOW() - (to_char(MAX(wh.htddata), 'YYYY-mm-dd'))::timestamp)||' dia(s)</center>' AS qtddias,
									--número de dias até a ultima tramitação
									'<center>'|| (SELECT DATE_PART('days', MAX(hd.htddata) - MIN(hd.htddata)) AS qtd FROM workflow.historicodocumento hd WHERE hd.docid = gd.docid )||' dia(s)</center>' AS datatotal
							FROM
									obras.grupodistribuicao gd
							INNER JOIN
									workflow.documento wd ON wd.docid = gd.docid
							LEFT JOIN
									workflow.historicodocumento wh ON wh.docid = gd.docid
							INNER JOIN
									workflow.estadodocumento we ON we.esdid = wd.esdid
							INNER JOIN	
									obras.itemgrupo itg ON itg.gpdid = gd.gpdid
							INNER JOIN
									obras.repositorio ore ON ore.repid = itg.repid --AND ore.repstatus = 'A'
							INNER JOIN
									obras.obrainfraestrutura oi ON oi.obrid = ore.obrid
							INNER JOIN
									entidade.endereco ed ON ed.endid = oi.endid
							INNER JOIN
									obras.orgao AS o ON o.orgid = oi.orgid 
							LEFT JOIN
									obras.empresacontratada ec ON ec.epcid = gd.epcid 
							LEFT JOIN
									entidade.entidade ee ON ee.entid = ec.entid
							LEFT JOIN
									seguranca.usuario su ON su.usucpf = gd.usucpf
							LEFT JOIN
									workflow.documento wdobr ON wdobr.docid = oi.docid
							LEFT JOIN 
									workflow.documento wdc ON wdc.docid = gd.docid
							LEFT JOIN 
									workflow.estadodocumento wed ON wed.esdid = wdc.esdid AND wed.esdstatus = 'A'
							LEFT JOIN 
									workflow.historicodocumento whd ON whd.docid = gd.docid AND whd.aedid = 336				
							WHERE
									gpdstatus = 'A' 
									AND we.esdid  IN ( ".GRUPOEMSUPERVISAO." ,".GRUPOAGUARDANDOINICIOSUPERVISAO." )
							GROUP BY
									gd.gpdid,
									wd.esdid,
									gd.estuf,
									gd.epcid,
									ee.entnome,
									su.usunome,
									gd.gpddtcriacao,
									we.esddsc,
									datatotal,
									whd.aedid
							ORDER BY
									2
			      		) AS grupos	 
				WHERE ( 
			      		  ( dias_de_grupos_desatualizados  > 90 )
			      		OR
			      		  ( dias_de_grupos_desatualizados  > 20 AND dias_de_grupos_desatualizados  <= 24 OR dias_de_grupos_desatualizados  > 24 )
			          )
				ORDER BY
						2";
	  	
	    $dados = (array) $db->carregar( $sql );
	 
		if ( is_array($dados[0]) ){
			$htmGrupos  = "<style type='text/css'>				
						body{
							font-size: 1em;
							font-family: Arial;
						}
						
						table{
							font-size: 0.8em;
						}
					
						div.scrollTable{
							background: #fff;
							/*border: 1px solid #888;*/
						}
					
						div.scrollTable table.header, div.scrollTable div.scroller table{
							width: 100%;
							border-collapse: collapse;
						}
						
						div.scrollTable table.header th, div.scrollTable div.scroller table td{
							/*border: 1px solid #444;*/
							padding: 3px 5px;
						}
						
						div.scrollTable table.header th{
							background: #ddd;
						}
					
						div.scrollTable div.scroller{
							height: 200px;
							overflow: scroll;
						}
					
						div.scrollTable .coluna75px{
							width: 75px;
						}
					
						div.scrollTable .coluna100px{
							width: 100px;
						}
					
						div.scrollTable .coluna150px{
							width: 150px;
						}
					</style>
					 <center><h2>Lista do Grupos de Obras</h2></center>
						<table cellpadding=\"1\" cellspacing=\"1\" style=\"border:2px #BEBEBE solid\">
							<tbody>
								<tr bgcolor=\"#e7e7e7\">
									<th class='coluna100px' align=\"center\">UF</th>
									<th class='coluna100px' align=\"center\">Número de Controle</th>
									<th class='coluna100px' align=\"center\">Empresa</th>
									<th class='coluna100px' align=\"center\">Total de Obras</th>
									<th class='coluna100px'align=\"center\">Responsável</th>
									<th class='coluna100px' align=\"center\">Data de Inclusão</th>
									<th class='coluna100px' align=\"center\">Situação</th>
									<th class='coluna100px' align=\"center\">Data de Tramitação</th>
									<th class='coluna100px' align=\"center\">Quantidade de Dias</th>
									<th class='coluna100px' align=\"center\">Total de Dias após a Tramitação</th>
								</tr>
							</tbody>
							<tbody>";
			foreach($dados as $dado){
				$cor = ( ($i % 2 == 0) ? "bgcolor=\"\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#ffffcc';\"" : "bgcolor=\"#f7f7f7\" onmouseout=\"this.bgColor='#F7F7F7';\" onmouseover=\"this.bgColor='#ffffcc';\"" );
				$htmGrupos .= "<tr {$cor}>
									<td class='coluna100px' align=\"center\">{$dado['unidade_federativa']}</td>
									<td class='coluna100px' align=\"center\">{$dado['numero_controle']}</td>
									<td class='coluna100px' >{$dado['empresa']}</td>
									<td class='coluna100px' align=\"center\">{$dado['total_de_obras']}</td>
									<td class='coluna100px' >{$dado['responsavel']}</td>
									<td class='coluna100px' align=\"center\">{$dado['dtinclusao']}</td>
									<td class='coluna100px' >{$dado['situacao']}</td>
									<td class='coluna100px' align=\"center\">{$dado['datramitacao']}</td>
									<td class='coluna100px' align=\"center\">{$dado['qtddias']}</td>
									<td class='coluna100px' align=\"center\">{$dado['datatotal']}</td>
								</tr>";
				$i++;
			}
			$htmGrupos  .= "</tbody>
					</table>
					<table cellpadding=\"1\" cellspacing=\"1\" style=\"border:1px #BEBEBE solid\" align=\"center\" width=\"100%\">
						<tbody>
							<tr bgcolor=\"#ffffff\">
								<td>
									<b>Total de Registros: ". count($dados)."</b>
								</td>
								<td>
								</td>
							</tr>
						</tbody>
					</table>";
		}else{
		
			$htmGrupos  = " <center><h2>Lista do Grupos de Obras</h2></center>
								<table cellpadding=\"1\" cellspacing=\"1\" style=\"border:2px #BEBEBE solid\" align=\"center\" width=\"100%\">
									<tbody>
										<tr bgcolor=\"#e7e7e7\">
											<th class='coluna100px' align=\"center\">UF</th>
											<th class='coluna100px' align=\"center\">Número de Controle</th>
											<th class='coluna100px' align=\"center\">Empresa</th>
											<th class='coluna100px' align=\"center\">Total de Obras</th>
											<th class='coluna100px'align=\"center\">Responsável</th>
											<th class='coluna100px' align=\"center\">Data de Inclusão</th>
											<th class='coluna100px' align=\"center\">Situação</th>
											<th class='coluna100px' align=\"center\">Data de Tramitação</th>
											<th class='coluna100px' align=\"center\">Quantidade de Dias</th>
											<th class='coluna100px' align=\"center\">Total de Dias após a Tramitação</th>
											<th class='coluna100px' align=\"center\">Dias do Grupos desatualizados</th>
										</tr>
									</tbody>
									</table>
								<table cellpadding=\"1\" cellspacing=\"1\" style=\"border:1px #BEBEBE solid\" align=\"center\" width=\"100%\">	
									<tbody>
										<tr>
											<td align=\"center\" style=\"color: rgb(204, 0, 0);\"><b>Não foram encontrados Registros.</b></td>
										</tr>
									</tbody>
								</table>";
		}
		
		$remetente     = array("nome" => SIGLA_SISTEMA. " - Monitoramento de Obras", "email" => $_SESSION['email_sistema']);	
		$destinatario  = $_SESSION['email_sistema'];
		$assunto       = " Listas de Obras e do Grupo de Obras que estão desatualizadas."; 
        $textoInicio   = " <br><br><b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Segue lista de Obras e Lista do Grupo de Obras que estão desatualizadas e possuem a sinalização Vermelho ou Amarelo.</b>&nbsp;<br>&nbsp;<br>&nbsp; ";
        $dataAtual     = date("d / m / Y");
        $horaAtual     = date("H : i : s");
  		$dataHoraEnvio = " <b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Data do envio: ". $dataAtual ."<br><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Hora do envio: ". $horaAtual ."</b>";
		$conteudo      = $textoInicio."<br><br>".$dataHoraEnvio."<br><br>".$htmObras."<br><br>".$htmGrupos;
		
		//Função que envia o Email para o Monitoramento de Obras, para informar  as Listas de Obras e de Grupo de Obras que estão desatualizadas. 
		enviar_email( $remetente, $destinatario, $assunto, $conteudo, $cc );

?>
