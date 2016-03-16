<?php
	
$sql = "SELECT DISTINCT
				ent.estuf,
				ent.enbnome,
				ptr.ptrcod,
				pmc.pmcnumconveniosiafi,
				SUM(ped.pedvalor) as valorconcedido,
				SUM(exe.exfvalor) as valorpago
	
			FROM  emenda.planotrabalho ptr
			LEFT  JOIN emenda.entidadebeneficiada ent ON ptr.enbid = ent.enbid
			LEFT  JOIN entidade.funcaoentidade fe on fe.entid = ent.entid 
			LEFT  JOIN territorios.municipio mun on mun.muncod = ent.muncod
			LEFT  JOIN emenda.ptminutaconvenio pmc on pmc.ptrid = ptr.ptrid and pmc.pmcstatus = 'A'
			LEFT  JOIN emenda.ptemendadetalheentidade ped ON ped.ptrid = ptr.ptrid
			LEFT  JOIN emenda.execucaofinanceira exe ON exe.ptrid = ptr.ptrid AND ped.pedid = exe.pedid and exe.exfstatus = 'A'
			LEFT  JOIN emenda.ordembancaria ord ON ord.exfid = exe.exfid AND ord.spgcodigo = '4'
			INNER JOIN emenda.responsavel res ON ptr.resid = res.resid
			INNER JOIN workflow.documento doc ON ptr.docid = doc.docid
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			WHERE
			    ptr.ptrstatus = 'A' 
			    AND ptr.ptrexercicio IN (2009, 2010)
			    AND ptr.ptrid  in (SELECT ptrid FROM emenda.analise WHERE anastatus = 'A' and anatipo = 'D' and analote = 1)
		    	AND ptr.ptrid NOT IN (SELECT tt.ptridpai FROM emenda.planotrabalho tt WHERE tt.ptridpai = ptr.ptrid and tt.ptrstatus = 'A')
		    	AND funid = 6
		    	AND ent.estuf = '".$_REQUEST['estuf']."'
			GROUP BY
				doc.esdid,
				ptr.ptrid,
				ent.estuf,
				ent.enbnome,
				ptr.ptrcod,
				pmc.pmcnumconveniosiafi
			ORDER BY 
				ent.estuf,
				ent.enbnome,
				ptr.ptrcod,
				pmc.pmcnumconveniosiafi";
//ver($sql);

$cabecalho = array("UF","Órgão ou Entidade", "Nº PTA", "Nº Convênio", "Valor Concedente", "Valor Pago");
//$db->monta_lista_array($arDadosArray, $cabecalho, 20, 10, 'N','Center');
$db->monta_lista($sql,$cabecalho,25,10,'N','Center');
?>
<script type="text/javascript">
</script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>