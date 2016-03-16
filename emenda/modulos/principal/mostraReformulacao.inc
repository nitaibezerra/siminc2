<?php
include APPRAIZ."includes/cabecalho.inc";
echo'<br>';

monta_titulo( $titulo_modulo, 'Tela de Reformulação' );

?>
<form method="post" name="formulario" id="formulario">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
<tr>
	<td class="SubTituloDireita" width="25%">Informe o Número do PTA:</td>
	<td><? echo campo_texto('ptrcod', 'N', 'S', '', 11, 10, '#########', '', '', '', '', 'id="ptrcod"', '', $_REQUEST['ptrcod']); ?></td>
</tr>
<tr>
	<td colspan="2" class="SubTituloCentro"><input type="button" name="buscar" value="Buscar" onclick="buscarPlanoTrabalho();"></td>
</tr>
</table>
</form>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script>
function buscarPlanoTrabalho() {
	document.getElementById('formulario').submit();
}
</script>
<?
if( !empty($_REQUEST['ptrcod']) ){
	$ptrcod = $_REQUEST['ptrcod']; 
	
	echo '<br>';
	$sql = "SELECT ptr.ptrid, ret.rftid, ret.trefid, tr.trefnome, ret.refid
			FROM 
				emenda.ptminreformulacao ptr
			    left join emenda.reformulatipos ret
					inner join emenda.tiporeformulacao tr on tr.trefid = ret.trefid
			    on ret.refid = ptr.refid
			WHERE ptr.ptrid in (select ptrid from emenda.planotrabalho where ptrcod = ".$ptrcod.") and ptr.refstatus = 'A' order by ptrid";
	monta_titulo( '<br>', 'Lista de Reformulações' );
	$cabecalho = array("ptrid", "rftid", "trefid", "trefnome", "refid");
	$db->monta_lista( $sql, $cabecalho, 20, 4, 'N','Center','','formref');
	
	echo '<br>';
	$sql = "SELECT distinct
			    p.ptrid,
			    '<center>'||p.ptrstatus||'</center>' as statuspta,
			    ((SELECT sum(pedvalor) FROM emenda.ptemendadetalheentidade pt1
                	inner join emenda.v_emendadetalheentidade ve1 on ve1.edeid = pt1.edeid WHERE ptrid = p.ptrid and ve1.edestatus = 'A') + p.ptrvalorproponente) as totalPTA,
			    p.ptrvalorproponente as ptrvalorproponente,
			    (SELECT sum(pedvalor) FROM emenda.ptemendadetalheentidade pt
                	inner join emenda.v_emendadetalheentidade ve on ve.edeid = pt.edeid WHERE ptrid = p.ptrid and ve.edestatus = 'A') as pedvalor,
			    (select sum(e.ptequantidade) from emenda.ptiniciativaespecificacao e
				inner join emenda.ptiniciativa pti on pti.ptiid = e.ptiid where pti.ptrid = p.ptrid and e.ptestatus = 'A' ) as ptequantidade,
			    (select sum(e.ptevalorproponente) from emenda.ptiniciativaespecificacao e 
				inner join emenda.ptiniciativa pti on pti.ptiid = e.ptiid where pti.ptrid = p.ptrid and e.ptestatus = 'A' ) as ptevalorproponente,
			    (select sum(e.ptevalorunitario) from emenda.ptiniciativaespecificacao e 
				inner join emenda.ptiniciativa pti on pti.ptiid = e.ptiid where pti.ptrid = p.ptrid and e.ptestatus = 'A') as ptevalorunitario,
			    to_char(ptv.vigdatainicio, 'DD/MM/YYYY') as dtinicio,
			    ptv.vigdias,
			    to_char(ptv.vigdatafim, 'DD/MM/YYYY') as dtfim,
			    ptv.vigtipo,
			    (SELECT count(1) FROM emenda.ptiniciativaespecificacao e
				inner join emenda.ptiniciativa pti on pti.ptiid = e.ptiid where pti.ptrid = p.ptrid and e.ptestatus = 'A') as especificacao,
				
			    (SELECT sum(pib.ptbquantidaderural) + sum(pib.ptbquantidadeurbana) as total FROM emenda.ptiniciativabeneficiario pib
			        inner join emenda.iniciativabeneficiario ib on ib.icbid = pib.icbid and ib.icbstatus = 'A'
			        inner join emenda.ptiniciativa pti on pib.ptiid = pti.ptiid where pti.ptrid = p.ptrid) as beneficiario,
			     array_to_string(array(select b.trefnome from emenda.reformulatipos a, emenda.tiporeformulacao b where (a.refid = p.refid) and (a.trefid = b.trefid)), '<br>') as tipos
			     
			FROM 
			    emenda.planotrabalho p
			    left join emenda.ptvigencia ptv on ptv.ptrid = p.ptrid and ptv.vigstatus = 'A' and ptv.vigtipo <> 'A'
			WHERE
				p.ptrcod = $ptrcod
			    and ptv.ptrid not in (SELECT ptrid FROM emenda.ptminreformulacao WHERE refsituacaoreformulacao = 'E' 
			    						and ptrid in ( SELECT p.ptrid FROM emenda.planotrabalho p 
			                            				WHERE ptrcod = $ptrcod))
			ORDER BY
			    p.ptrid asc";
	/*$arrDados = $db->carregar($sql);
	
	$arrRegistro = array();
	foreach ($arrDados as $key => $v) {
		$tiposRef = array();
		if( $v['vigtipo'] != 'P' ){
			$arrTipos = pegaTipoReformulacao($v['ptrid']);
			if( $arrTipos[0] ){
				foreach ($arrTipos as $tipo) {
					array_push($tiposRef, $tipo['descricao']);	
				}
			}
		} else {
			array_push($tiposRef, 'PTA Original');
		}
		
		$arrRegistro[$key] = array(
								'ptrid' 				=> $v['ptrid'],
								'totalpta' 				=> $v['totalpta'],
								'ptrvalorproponente' 	=> $v['ptrvalorproponente'],
								'pedvalor' 				=> $v['pedvalor'],
								'ptequantidade' 		=> $v['ptequantidade'],
								'ptevalorproponente' 	=> $v['ptevalorproponente'],
								'ptevalorunitario' 		=> $v['ptevalorunitario'],
								'dtinicio' 				=> $v['dtinicio'],
								'vigdias' 				=> $v['vigdias'],
								'dtfim' 				=> $v['dtfim'],
								'vigtipo' 				=> $v['vigtipo'],
								'especificacao' 		=> $v['especificacao'],
								'beneficiario'			=> $v['beneficiario'],
								'tiporeformulacao'		=> implode('<br>', $tiposRef)
								);
	}*/
	
	monta_titulo( '<br>', 'Lista Possiveis Tipos de Reformulação (Dados do PTA)' );
	$cabecalho = array("ptrid", "Status PTA", "Total PTA + Proponente", "Proponente", "Valor Recurso", "QTD Especificacao", "Proponente Especificação", "Vlr Unit Especificação", "Vigência Inicio", "Vigência Dias", "Vigência Fim", "Vigência Tipo", "QTD Especificação", "Total Beneficiario", 'Tipo de REformulação');
	//$db->monta_lista_simples( $sql, $cabecalho, 200, 4, 'N', '95%', '', true, false, false, true);
	$db->monta_lista( $sql, $cabecalho, 20, 4, 'N','Center','','formpta');
	
	echo '<br>';
	$sql = "SELECT ptrid, enbid, ptrexercicio, ptrstatus, resid, ptrvalorproponente, bcoid, ptragenciabancaria, ptrcontrapartida, docid, ptrnumdocumenta, ptrnumconvenio, ptranoconvenio, ptrnumprocessoempenho, 
  				ptridpai, ptrcod, ptrsituacao, refid
			FROM emenda.planotrabalho WHERE ptrcod = ".$ptrcod." order by ptrid";
	//monta_titulo( '<br>', 'Lista de PlanoTrabalho' );
	$cabecalho = array("ptrid", "enbid", "ptrexercicio", "ptrstatus", "resid", "ptrvalorproponente", "bcoid", "ptragenciabancaria", "ptrcontrapartida", "docid", "ptrnumdocumenta", "ptrnumconvenio", "ptranoconvenio", "ptrnumprocessoempenho", "ptridpai", "ptrcod", "ptrsituacao", "refid");
	//$db->monta_lista( $sql, $cabecalho, 20, 4, 'N','Center','','formpta');
	
	echo '<br>';
	monta_titulo( '', 'Lista de PtVigencia' );
	$sql = "SELECT vigid, ptrid, pmcid, to_char(vigdatainicio, 'DD/MM/YYYY') as vigdatainicio, vigdias, to_char(vigdatafim, 'DD/MM/YYYY') as vigdatafim, vigstatus, to_char(vigdatainclusao, 'DD/MM/YYYY HH24:MI:SS') as vigdatainclusao, usucpfinclusao, refid, vigtipo FROM emenda.ptvigencia WHERE ptrid in (select ptrid from emenda.planotrabalho where ptrcod = $ptrcod) and vigstatus = 'A' order by vigid";
	$cabecalho = array("vigid", "ptrid", "pmcid", "vigdatainicio", "vigdias", "vigdatafim", "vigstatus", "vigdatainclusao", "usucpfinclusao", "refid", "vigtipo");
	$db->monta_lista( $sql, $cabecalho, 20, 4, 'N','Center','','formvigencia');
	echo '<br>';
	monta_titulo( '', 'Lista de PtminReformulacao' );
	$sql = "SELECT refid, ptrid, pmcid, mdoid, refstatus, usucpfinclusao, to_char(refdatainclusao, 'DD/MM/YYYY HH24:MI:SS') as refdatainclusao, refsituacao, refdataalteracao, usucpfalteracao,
  				refsituacaoreformulacao, refsituacaoanalise, refprorrogacaooficio, refcpfexclusao
			FROM emenda.ptminreformulacao WHERE ptrid in (select ptrid from emenda.planotrabalho where ptrcod = $ptrcod) order by refid";
	$cabecalho = array("refid", "ptrid", "pmcid", "mdoid", "refstatus", "usucpfinclusao", "refdatainclusao", "refsituacao", "refdataalteracao", "usucpfalteracao", "refsituacaoreformulacao", "refsituacaoanalise", "refprorrogacaooficio", "refcpfexclusao");
	$db->monta_lista( $sql, $cabecalho, 20, 4, 'N','Center','','formreformulacao');
	echo '<br>';
	monta_titulo( '', 'Lista de Analise' );
	$sql = "SELECT anaid, ptrid, uniid, anatipo, analote, anasituacaoparecer, usucpf, to_char(anadataconclusao, 'DD/MM/YYYY HH24:MI:SS') as anadataconclusao, to_char(anadatainclusao, 'DD/MM/YYYY : HH24:MI:SS') as anadatainclusao, anastatus, refid
			FROM emenda.analise WHERE ptrid in (select ptrid from emenda.planotrabalho where ptrcod = $ptrcod) order by anaid";
	$cabecalho = array("anaid", "ptrid", "uniid", "anatipo", "analote", "anasituacaoparecer", "usucpf", "anadataconclusao", "anadatainclusao", "anastatus", "refid");
	$db->monta_lista( $sql, $cabecalho, 20, 4, 'N','Center','','formconvenio');
	echo '<br>';
	monta_titulo( '', 'Lista de PtminutaConvenio' );
	$sql = "SELECT pmcid, ptrid, pmcstatus, pmcresolucao, pmcdataconversaosiafi, pmcdataassinatura, pmcnumconveniosiafi,
  				pmcdatarap, mdoid, pmcdatainclusao, usucpfinclusao, pmcdataalteracao
			FROM emenda.ptminutaconvenio WHERE ptrid in (select ptrid from emenda.planotrabalho where ptrcod = $ptrcod) and pmcstatus = 'A' order by pmcid";
	$cabecalho = array("pmcid", "ptrid", "pmcstatus", "pmcresolucao", "pmcdataconversaosiafi", "pmcdataassinatura", "pmcnumconveniosiafi", "pmcdtatualizaconveniosiafi,
  				pmcdatarap", "mdoid", "pmcdatainclusao", "usucpfinclusao", "pmcdataalteracao");
	$db->monta_lista( $sql, $cabecalho, 20, 4, 'N','Center','','formconvenio');
}
?>