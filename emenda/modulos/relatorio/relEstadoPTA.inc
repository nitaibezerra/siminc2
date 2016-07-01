<?php

if( $_REQUEST["requisicao"] == "listauf" ){
	
	if( !empty($_REQUEST['resid']) ){
		$filtro = 'and resid = '. $_REQUEST['resid'];
	}
	
	$sql = "SELECT
				CASE WHEN estuf <> '' 
				THEN '<a onclick=\"listaPTA( \'' || estuf || '\', {$_REQUEST["esdid"]} );\" style=\"cursor:pointer;\">' || estuf || '</a>'
				ELSE 'Não Informado' END as uf,
				(SELECT count(p.ptrid) FROM 
					    	 emenda.planotrabalho p
					         LEFT JOIN workflow.documento d 
					          INNER JOIN workflow.estadodocumento e 
					          	ON e.esdid = d.esdid
					         ON p.docid = d.docid
					         LEFT JOIN 
							    emenda.entidadebeneficiada ent 
                            	INNER JOIN  territorios.municipio mun ON mun.muncod = ent.muncod
                            ON p.enbid = ent.enbid 
					     WHERE 
					    	p.ptrstatus = 'A'
					    	and p.ptrexercicio = '{$_SESSION["exercicio"]}'
					    	and e.tpdid = ".TPDID_EMENDAS."
					    	AND e.esdid = {$_REQUEST["esdid"]} 
					    	AND mun.estuf = ee.estuf 
					        $filtro ) as pta,
				sum(pti.ptivalorconcedente) as valorconcedente,
			    sum(pti.ptivalorproponente) as valorproponente,
			    sum(pti.ptivalortotal) as valor
			FROM 
			    emenda.planotrabalho ep
			left JOIN 
				emenda.v_ptiniciativa pti ON pti.ptrid = ep.ptrid
			LEFT JOIN 
			    emenda.entidadebeneficiada ee ON ep.enbid = ee.enbid 
			LEFT JOIN 
			    workflow.documento doc 
			    INNER JOIN 
			        workflow.estadodocumento esd ON esd.esdid = doc.esdid
			    ON ep.docid = doc.docid 
			WHERE
				ptrstatus = 'A' AND esd.esdid = {$_REQUEST["esdid"]}
				AND ptrexercicio = '{$_SESSION['exercicio']}' ".$filtro." 
			GROUP BY 
				estuf
			ORDER BY
				estuf";
	$cabecalho = array( "UF", "Qtd de PTA's", "Valor do Concedente(R$)", "Valor do Proponente(R$)", "Valor Total (R$)" );
	$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%');
	die;
		
}

require_once APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';
// Monta as abas e o título
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( 'Quadro Resumo', '');

?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr>
		<td colspan="" class="subtitulocentro">Geral</td>
	</tr>
	<tr>
		<td>
			<?php
				
				$sql = "SELECT DISTINCT
					    esd.esdid as id,
					    esd.esddsc as situacao,
					    (SELECT count(p.ptrid) FROM 
					    	 emenda.planotrabalho p
					         LEFT JOIN workflow.documento d 
					          INNER JOIN workflow.estadodocumento e 
					          	ON e.esdid = d.esdid
					         ON p.docid = d.docid 
					     WHERE 
					    	p.ptrstatus = 'A'
					    	and p.ptrexercicio = '{$_SESSION["exercicio"]}'
					    	and e.tpdid = ".TPDID_EMENDAS."
					        and e.esdid = esd.esdid) as ptas,
					    sum(pti.ptivalorconcedente) as valorconcedente,
					    sum(pti.ptivalorproponente) as valorproponente,
					    sum(pti.ptivalortotal) as valor
					FROM 
					    emenda.planotrabalho ptr 
					INNER JOIN 
					    emenda.v_ptiniciativa pti ON pti.ptrid = ptr.ptrid
					LEFT JOIN 
					    emenda.entidadebeneficiada ee ON ptr.enbid = ee.enbid 
					LEFT JOIN 
					    workflow.documento doc 
					    INNER JOIN 
					        workflow.estadodocumento esd ON esd.esdid = doc.esdid
					    ON ptr.docid = doc.docid 
					WHERE 
					    ptr.ptrstatus = 'A'
					    and ptr.ptrexercicio = '{$_SESSION["exercicio"]}'
					    and esd.tpdid = ".TPDID_EMENDAS."
					GROUP BY esd.esdid, esd.esddsc";
				$dadosListaPTA = $db->carregar( $sql );

			?>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr>
					<td class="subtitulocentro">Ação</td>
					<td class="subtitulocentro">Situação</td>
					<td class="subtitulocentro">Qtd. de PTA's</td>
					<td class="subtitulocentro">Valor do Concedente (R$)</td>
					<td class="subtitulocentro">Valor do Proponente (R$)</td>
					<td class="subtitulocentro">Valor Total (R$)</td>
				</tr>
				
				<?php 
					$ptas			= 0;
					$vlrConcedente	= 0;
					$vlrProponente	= 0;
					$vlr 			= 0;
				
					for( $i = 0; $i < count($dadosListaPTA); $i++ ){
						
						$cor = ($i % 2) ? "#f4f4f4" : "#e0e0e0";
						
						print "<tr bgColor='{$cor}'>" 
							. "	   <td align='center' id='geral_acao_{$dadosListaPTA[$i]["id"]}'>"
							. "        <img src='/imagens/mais.gif' style='cursor:pointer;' onclick='abreEstados({$dadosListaPTA[$i]["id"]}, \"geral\");'/>"
							. "	   </td>"
							. "	   <td >"
							.          $dadosListaPTA[$i]["situacao"]
							. "	   </td>"
							. "	   <td align='right'>"
							.          $dadosListaPTA[$i]["ptas"]
							. "	   </td>"
							. "	   <td align='right'>"
							.          number_format( $dadosListaPTA[$i]["valorconcedente"], 2, ",", "." )
							. "	   </td>"
							. "	   <td align='right'>"
							.          number_format( $dadosListaPTA[$i]["valorproponente"], 2, ",", "." )
							. "	   </td>"
							. "	   <td align='right'>"
							.          number_format( $dadosListaPTA[$i]["valor"], 2, ",", "." )
							. "	   </td>"
							. "</tr>"
							. "<tr id='geral_{$dadosListaPTA[$i]["id"]}' style='display:none;'>"
							. "    <td align='center' valign='center'><img src='/imagens/seta_filho.gif'/></td>"
							. "    <td colspan='6' id='geral_filho_{$dadosListaPTA[$i]["id"]}'></td>"
							. "</tr>";

						$ptas			+= $dadosListaPTA[$i]["ptas"];
						$vlrConcedente	+= $dadosListaPTA[$i]["valorconcedente"]; 
						$vlrProponente	+= $dadosListaPTA[$i]["valorproponente"];
						$vlr 			+= $dadosListaPTA[$i]["valor"];
					}
					
				print "<tr bgColor='FOFOFO'>" 
					. "	   <td colspan='2' align='right'>"
					. "         <b>Totais</b>"
					. "	   </td>"
					. "	   <td align='right'>"
					.          $ptas
					. "	   </td>"
					. "	   <td align='right'>"
					.          number_format( $vlrConcedente, 2, ",", "." )
					. "	   </td>"
					. "	   <td align='right'>"
					.          number_format( $vlrProponente, 2, ",", "." )
					. "	   </td>"
					. "	   <td align='right'>"
					.          number_format( $vlr, 2, ",", "." )
					. "	   </td>"
					. "</tr>";
				?>
				
			</table>
			<br/>
		</td>
	</tr>
	<tr>
		<td colspan="" class="subtitulocentro">SESU</td>
	</tr>
	<tr>
		<td>
			<?php				
				$sql = "SELECT DISTINCT
					    esd.esdid as id,
					    esd.esddsc as situacao,
					    (SELECT count(p.ptrid) FROM 
					    	 emenda.planotrabalho p
					         LEFT JOIN workflow.documento d 
					          INNER JOIN workflow.estadodocumento e 
					          	ON e.esdid = d.esdid
					         ON p.docid = d.docid 
					     WHERE 
					    	p.ptrstatus = 'A'
					    	and p.ptrexercicio = '{$_SESSION["exercicio"]}'
					    	and e.tpdid = ".TPDID_EMENDAS."
					    	and p.resid = 1
					        and e.esdid = esd.esdid) as ptas,
					    sum(pti.ptivalorconcedente) as valorconcedente,
					    sum(pti.ptivalorproponente) as valorproponente,
					    sum(pti.ptivalortotal) as valor
					FROM 
					    emenda.planotrabalho ptr 
					INNER JOIN 
					    emenda.v_ptiniciativa pti ON pti.ptrid = ptr.ptrid
					LEFT JOIN 
					    emenda.entidadebeneficiada ee ON ptr.enbid = ee.enbid 
					LEFT JOIN 
					    workflow.documento doc 
					    INNER JOIN 
					        workflow.estadodocumento esd ON esd.esdid = doc.esdid
					    ON ptr.docid = doc.docid 
					WHERE 
					    ptr.ptrstatus = 'A'
					    and ptr.ptrexercicio = '{$_SESSION["exercicio"]}'
					    and esd.tpdid = ".TPDID_EMENDAS."
					    and ptr.resid = 1
					GROUP BY esd.esdid, esd.esddsc";
			
				$dadosListaPTA = $db->carregar( $sql );

			?>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr>
					<td class="subtitulocentro">Ação</td>
					<td class="subtitulocentro">Situação</td>
					<td class="subtitulocentro">Qtd. de PTA's</td>
					<td class="subtitulocentro">Valor do Concedente (R$)</td>
					<td class="subtitulocentro">Valor do Proponente (R$)</td>
					<td class="subtitulocentro">Valor Total (R$)</td>
				</tr>
				
				<?php 
					$ptas			= 0;
					$vlrConcedente	= 0;
					$vlrProponente	= 0;
					$vlr 			= 0;
				
					for( $i = 0; $i < count($dadosListaPTA); $i++ ){
						
						$cor = ($i % 2) ? "#f4f4f4" : "#e0e0e0";
						
						print "<tr bgColor='{$cor}'>" 
							. "	   <td align='center' id='superior_acao_{$dadosListaPTA[$i]["id"]}'>"
							. "        <img src='/imagens/mais.gif' style='cursor:pointer;' onclick='abreEstados({$dadosListaPTA[$i]["id"]}, \"superior\");'/>"
							. "	   </td>"
							. "	   <td >"
							.          $dadosListaPTA[$i]["situacao"]
							. "	   </td>"
							. "	   <td align='right'>"
							.          $dadosListaPTA[$i]["ptas"]
							. "	   </td>"
							. "	   <td align='right'>"
							.          number_format( $dadosListaPTA[$i]["valorconcedente"], 2, ",", "." )
							. "	   </td>"
							. "	   <td align='right'>"
							.          number_format( $dadosListaPTA[$i]["valorproponente"], 2, ",", "." )
							. "	   </td>"
							. "	   <td align='right'>"
							.          number_format( $dadosListaPTA[$i]["valor"], 2, ",", "." )
							. "	   </td>"
							. "</tr>"
							. "<tr id='superior_{$dadosListaPTA[$i]["id"]}' style='display:none;'>"
							. "    <td align='center' valign='center'><img src='/imagens/seta_filho.gif'/></td>"
							. "    <td colspan='6' id='superior_filho_{$dadosListaPTA[$i]["id"]}'></td>"
							. "</tr>";

						$ptas			+= $dadosListaPTA[$i]["ptas"];
						$vlrConcedente	+= $dadosListaPTA[$i]["valorconcedente"]; 
						$vlrProponente	+= $dadosListaPTA[$i]["valorproponente"];
						$vlr 			+= $dadosListaPTA[$i]["valor"];
					}
				print "<tr bgColor='FOFOFO'>" 
					. "	   <td colspan='2' align='right'>"
					. "         <b>Totais</b>"
					. "	   </td>"
					. "	   <td align='right'>"
					.          $ptas
					. "	   </td>"
					. "	   <td align='right'>"
					.          number_format( $vlrConcedente, 2, ",", "." )
					. "	   </td>"
					. "	   <td align='right'>"
					.          number_format( $vlrProponente, 2, ",", "." )
					. "	   </td>"
					. "	   <td align='right'>"
					.          number_format( $vlr, 2, ",", "." )
					. "	   </td>"
					. "</tr>";				
				?>
				
			</table>
			<br/>
		</td>
	</tr>
	<tr>
		<td colspan="" class="subtitulocentro">SETEC</td>
	</tr>
	<tr>
		<td>
			<?php
				
				$sql = "SELECT DISTINCT
					    esd.esdid as id,
					    esd.esddsc as situacao,
					    (SELECT count(p.ptrid) FROM 
					    	 emenda.planotrabalho p
					         LEFT JOIN workflow.documento d 
					          INNER JOIN workflow.estadodocumento e 
					          	ON e.esdid = d.esdid
					         ON p.docid = d.docid 
					     WHERE 
					    	p.ptrstatus = 'A'
					    	and p.ptrexercicio = '{$_SESSION["exercicio"]}'
					    	and e.tpdid = ".TPDID_EMENDAS."
					    	and p.resid = 2
					        and e.esdid = esd.esdid) as ptas,
					    sum(pti.ptivalorconcedente) as valorconcedente,
					    sum(pti.ptivalorproponente) as valorproponente,
					    sum(pti.ptivalortotal) as valor
					FROM 
					    emenda.planotrabalho ptr 
					INNER JOIN 
					    emenda.v_ptiniciativa pti ON pti.ptrid = ptr.ptrid
					LEFT JOIN 
					    emenda.entidadebeneficiada ee ON ptr.enbid = ee.enbid 
					LEFT JOIN 
					    workflow.documento doc 
					    INNER JOIN 
					        workflow.estadodocumento esd ON esd.esdid = doc.esdid
					    ON ptr.docid = doc.docid 
					WHERE 
					    ptr.ptrstatus = 'A'
					    and ptr.ptrexercicio = '{$_SESSION["exercicio"]}'
					    and esd.tpdid = ".TPDID_EMENDAS."
					    and ptr.resid = 2
					GROUP BY esd.esdid, esd.esddsc";
			
				$dadosListaPTA = $db->carregar( $sql );

			?>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr>
					<td class="subtitulocentro">Ação</td>
					<td class="subtitulocentro">Situação</td>
					<td class="subtitulocentro">Qtd. de PTA's</td>
					<td class="subtitulocentro">Valor do Concedente (R$)</td>
					<td class="subtitulocentro">Valor do Proponente (R$)</td>
					<td class="subtitulocentro">Valor Total (R$)</td>
				</tr>
				
				<?php 
					$ptas			= 0;
					$vlrConcedente	= 0;
					$vlrProponente	= 0;
					$vlr 			= 0;
				
					for( $i = 0; $i < count($dadosListaPTA); $i++ ){
						
						$cor = ($i % 2) ? "#f4f4f4" : "#e0e0e0";
						
						print "<tr bgColor='{$cor}'>" 
							. "	   <td align='center' id='medio_acao_{$dadosListaPTA[$i]["id"]}'>"
							. "        <img src='/imagens/mais.gif' style='cursor:pointer;' onclick='abreEstados({$dadosListaPTA[$i]["id"]}, \"medio\");'/>"
							. "	   </td>"
							. "	   <td >"
							.          $dadosListaPTA[$i]["situacao"]
							. "	   </td>"
							. "	   <td align='right'>"
							.          $dadosListaPTA[$i]["ptas"]
							. "	   </td>"
							. "	   <td align='right'>"
							.          number_format( $dadosListaPTA[$i]["valorconcedente"], 2, ",", "." )
							. "	   </td>"
							. "	   <td align='right'>"
							.          number_format( $dadosListaPTA[$i]["valorproponente"], 2, ",", "." )
							. "	   </td>"
							. "	   <td align='right'>"
							.          number_format( $dadosListaPTA[$i]["valor"], 2, ",", "." )
							. "	   </td>"
							. "</tr>"
							. "<tr id='medio_{$dadosListaPTA[$i]["id"]}' style='display:none;'>"
							. "    <td align='center' valign='center'><img src='/imagens/seta_filho.gif'/></td>"
							. "    <td colspan='6' id='medio_filho_{$dadosListaPTA[$i]["id"]}'></td>"
							. "</tr>";
						
						$ptas			+= $dadosListaPTA[$i]["ptas"];
						$vlrConcedente	+= $dadosListaPTA[$i]["valorconcedente"]; 
						$vlrProponente	+= $dadosListaPTA[$i]["valorproponente"];
						$vlr 			+= $dadosListaPTA[$i]["valor"];
					}
				print "<tr bgColor='FOFOFO'>" 
					. "	   <td colspan='2' align='right'>"
					. "         <b>Totais</b>"
					. "	   </td>"
					. "	   <td align='right'>"
					.          $ptas
					. "	   </td>"
					. "	   <td align='right'>"
					.          number_format( $vlrConcedente, 2, ",", "." )
					. "	   </td>"
					. "	   <td align='right'>"
					.          number_format( $vlrProponente, 2, ",", "." )
					. "	   </td>"
					. "	   <td align='right'>"
					.          number_format( $vlr, 2, ",", "." )
					. "	   </td>"
					. "</tr>";				
				?>
				
			</table>
			<br/>
		</td>
	</tr>
	<tr>
		<td colspan="" class="subtitulocentro">FNDE</td>
	</tr>
	<tr>
		<td>
			<?php
				
				/*$sql = "SELECT DISTINCT
							esd.esdid as id,
							esd.esddsc as situacao,
							count(ptr.ptrid) as ptas,
							sum(pti.ptivalorconcedente) as valorconcedente,
							sum(pti.ptivalorproponente) as valorproponente,
							sum(pti.ptivalortotal) as valor
						FROM 
							emenda.planotrabalho ptr 
						INNER JOIN 
							emenda.v_ptiniciativa pti ON pti.ptrid = ptr.ptrid
						LEFT JOIN 
							entidade.entidade ee ON ptr.entid = ee.entid 
						LEFT JOIN 
							entidade.endereco ed ON ed.entid = ee.entid 
						LEFT JOIN 
							workflow.documento doc 
							INNER JOIN 
								workflow.estadodocumento esd ON esd.esdid = doc.esdid
							ON ptr.docid = doc.docid 
						WHERE 
							ptr.ptrstatus = 'A' AND 
							ptr.ptrexercicio = '{$_SESSION["exercicio"]}' AND 
							esd.tpdid = " . TPDID_EMENDAS . " AND
							resid = 3
						GROUP BY esd.esdid, esd.esddsc";*/
			
				$sql = "SELECT DISTINCT
					    esd.esdid as id,
					    esd.esddsc as situacao,
					    (SELECT count(p.ptrid) FROM 
					    	 emenda.planotrabalho p
					         LEFT JOIN workflow.documento d 
					          INNER JOIN workflow.estadodocumento e 
					          	ON e.esdid = d.esdid
					         ON p.docid = d.docid 
					     WHERE 
					    	p.ptrstatus = 'A'
					    	and p.ptrexercicio = '{$_SESSION["exercicio"]}'
					    	and e.tpdid = ".TPDID_EMENDAS."
					    	and p.resid = 3
					        and e.esdid = esd.esdid) as ptas,
					    sum(pti.ptivalorconcedente) as valorconcedente,
					    sum(pti.ptivalorproponente) as valorproponente,
					    sum(pti.ptivalortotal) as valor
					FROM 
					    emenda.planotrabalho ptr 
					INNER JOIN 
					    emenda.v_ptiniciativa pti ON pti.ptrid = ptr.ptrid
					LEFT JOIN 
					    emenda.entidadebeneficiada ee ON ptr.enbid = ee.enbid 
					LEFT JOIN 
					    workflow.documento doc 
					    INNER JOIN 
					        workflow.estadodocumento esd ON esd.esdid = doc.esdid
					    ON ptr.docid = doc.docid 
					WHERE 
					    ptr.ptrstatus = 'A'
					    and ptr.ptrexercicio = '{$_SESSION["exercicio"]}'
					    and esd.tpdid = ".TPDID_EMENDAS."
					    and ptr.resid = 3
					GROUP BY esd.esdid, esd.esddsc";
				
				$dadosListaPTA = $db->carregar( $sql );

			?>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr>
					<td class="subtitulocentro">Ação</td>
					<td class="subtitulocentro">Situação</td>
					<td class="subtitulocentro">Qtd. de PTA's</td>
					<td class="subtitulocentro">Valor do Concedente (R$)</td>
					<td class="subtitulocentro">Valor do Proponente (R$)</td>
					<td class="subtitulocentro">Valor Total (R$)</td>
				</tr>
				
				<?php 
					$ptas			= 0;
					$vlrConcedente	= 0;
					$vlrProponente	= 0;
					$vlr 			= 0;
				
					for( $i = 0; $i < count($dadosListaPTA); $i++ ){
						
						$cor = ($i % 2) ? "#f4f4f4" : "#e0e0e0";
						
						print "<tr bgColor='{$cor}'>" 
							. "	   <td align='center' id='basico_acao_{$dadosListaPTA[$i]["id"]}'>"
							. "        <img src='/imagens/mais.gif' style='cursor:pointer;' onclick='abreEstados({$dadosListaPTA[$i]["id"]}, \"basico\");'/>"
							. "	   </td>"
							. "	   <td >"
							.          $dadosListaPTA[$i]["situacao"]
							. "	   </td>"
							. "	   <td align='right'>"
							.          $dadosListaPTA[$i]["ptas"]
							. "	   </td>"
							. "	   <td align='right'>"
							.          number_format( $dadosListaPTA[$i]["valorconcedente"], 2, ",", "." )
							. "	   </td>"
							. "	   <td align='right'>"
							.          number_format( $dadosListaPTA[$i]["valorproponente"], 2, ",", "." )
							. "	   </td>"
							. "	   <td align='right'>"
							.          number_format( $dadosListaPTA[$i]["valor"], 2, ",", "." )
							. "	   </td>"
							. "</tr>"
							. "<tr id='basico_{$dadosListaPTA[$i]["id"]}' style='display:none;'>"
							. "    <td align='center' valign='center'><img src='/imagens/seta_filho.gif'/></td>"
							. "    <td colspan='6' id='basico_filho_{$dadosListaPTA[$i]["id"]}'></td>"
							. "</tr>";
						
						$ptas			+= $dadosListaPTA[$i]["ptas"];
						$vlrConcedente	+= $dadosListaPTA[$i]["valorconcedente"]; 
						$vlrProponente	+= $dadosListaPTA[$i]["valorproponente"];
						$vlr 			+= $dadosListaPTA[$i]["valor"];
					}
				print "<tr bgColor='FOFOFO'>" 
					. "	   <td colspan='2' align='right'>"
					. "         <b>Totais</b>"
					. "	   </td>"
					. "	   <td align='right'>"
					.          $ptas
					. "	   </td>"
					. "	   <td align='right'>"
					.          number_format( $vlrConcedente, 2, ",", "." )
					. "	   </td>"
					. "	   <td align='right'>"
					.          number_format( $vlrProponente, 2, ",", "." )
					. "	   </td>"
					. "	   <td align='right'>"
					.          number_format( $vlr, 2, ",", "." )
					. "	   </td>"
					. "</tr>";				
				?>
				
			</table>
			<br/>
		</td>
	</tr>
</table>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">

	function abreEstados( id, tipo ){

		switch( tipo ){
			case 'geral':
				var acao  = 'geral_acao_';
				var td    = 'geral_';
				var filho = 'geral_filho_';
				var param = '';
			break;
			case 'superior':
				var acao  = 'superior_acao_';
				var td    = 'superior_';
				var filho = 'superior_filho_';
				var param = '&resid=1';
			break;
			case 'medio':
				var acao  = 'medio_acao_';
				var td    = 'medio_';
				var filho = 'medio_filho_';
				var param = '&resid=2';
			break;
			case 'basico':
				var acao  = 'basico_acao_';
				var td    = 'basico_';
				var filho = 'basico_filho_';
				var param = '&resid=3';
			break;	
		}

		var url = '?modulo=relatorio/relEstadoPTA&acao=A&requisicao=listauf&esdid=' + id + param;
		
		var myAjax = new Ajax.Updater(
		document.getElementById( filho + id ),
		url,
		{
			method: 'post',
			asynchronous: false
		});

		if (document.selection){
			document.getElementById(td + id).style.display = 'block';
		}else{
			document.getElementById(td + id).style.display = 'table-row';
		}
		
		document.getElementById( acao + id ).innerHTML = "<img src='/imagens/menos.gif' style='cursor:pointer;' onclick='fechaEstados(" + id + ", \"" + tipo + "\");'/>";
		
	}

	function fechaEstados( id, tipo ){

		switch( tipo ){
		case 'geral':
			var acao  = 'geral_acao_';
			var td    = 'geral_';
			var filho = 'geral_filho_';
		break;
		case 'superior':
			var acao  = 'superior_acao_';
			var td    = 'superior_';
			var filho = 'superior_filho_';
		break;
		case 'medio':
			var acao  = 'medio_acao_';
			var td    = 'medio_';
			var filho = 'medio_filho_';
		break;
		case 'basico':
			var acao  = 'basico_acao_';
			var td    = 'basico_';
			var filho = 'basico_filho_';
		break;	
	}
		
		document.getElementById( td + id ).style.display = 'none';
		document.getElementById( filho + id ).innerHTML = '';
		document.getElementById( acao + id ).innerHTML = "<img src='/imagens/mais.gif' style='cursor:pointer;' onclick='abreEstados(" + id + ", \"" + tipo + "\");'/>";
		
	}
	
	function listaPTA( estuf, esdid ){
		//window.location.href = '/emenda/emenda.php?modulo=principal/listaPlanoTrabalho&acao=A&estuf=' + estuf + '&esdid=' + esdid
		window.location.href = '/emenda/emenda.php?modulo=principal/listaPtaAnalise&acao=A&requisicao=filtrapesquisa&estuf=' + estuf + '&esdid=' + esdid
	}

</script>