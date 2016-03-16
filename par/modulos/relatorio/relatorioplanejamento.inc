<?php

include APPRAIZ . "includes/cabecalho.inc";
include APPRAIZ . 'includes/Agrupador.php';
echo'<br>';

function mascaraglobal($value, $mask) {
	$casasdec = explode(",", $mask);
	// Se possui casas decimais
	if($casasdec[1])
		$value = sprintf("%01.".strlen($casasdec[1])."f", $value);

	$value = str_replace(array("."),array(""),$value);
	if(strlen($mask)>0) {
		$masklen = -1;
		$valuelen = -1;
		while($masklen>=-strlen($mask)) {
			if(-strlen($value)<=$valuelen) {
				if(substr($mask,$masklen,1) == "#") {
						$valueformatado = trim(substr($value,$valuelen,1)).$valueformatado;
						$valuelen--;
				} else {
					if(trim(substr($value,$valuelen,1)) != "") {
						$valueformatado = trim(substr($mask,$masklen,1)).$valueformatado;
					}
				}
			}
			$masklen--;
		}
	}
	return $valueformatado;
}
monta_titulo( $titulo_modulo, 'Clique no botão XLS para gerar o relatório' );

/* configurações do relatorio - Memoria limite de 2048 Mbytes */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 2048 Mbytes */

echo "<p align=center>";
echo "	<input type=button name=xls value=XLS onclick=\"window.location='par.php?modulo=relatorio/relatorioplanejamento&acao=A&exibirxls=1';\">";
echo "</p>";



if($_REQUEST['exibirxls']){	
	
	$sql = "SELECT      distinct CASE WHEN ptoclassificacaoobra='P' THEN 'Proinfancia' WHEN ptoclassificacaoobra='Q' THEN 'Quadra' WHEN ptoclassificacaoobra='C' THEN 'Cobertura' END as tipo_obra,
                  pre.preid as n_protocolo_simec,
                  pre.estuf as uf,
                  o.obrid as obrid,
                  mun.mundescricao as municipio_beneficiado,
                  mun.muncod as cod_ibge,
                  tmn.tpmdsc as grupo_pac,
                  pto.ptodescricao as tipo_projeto,
                  pre.prevalorobra as valor_obra,
                  esd.esddsc as situacao_analise_fnde,
                  CASE WHEN pre.preesfera = 'E' THEN 'Estadual' WHEN pre.preesfera ='M' THEN 'Municipal' END as preesfera,
                  pre.predescricao as nome_obra,
                  pre.prelogradouro || ', ' || pre.precomplemento || ', ' || pre.precep as endereco_obra,
                  pre.prelatitude || ' / ' || pre.prelongitude as coordenada_geografica,
                  ter.data_termo,
                  SUM(emo.eobvalorempenho) as valor_empenho_solicitado,
                  (select count(*) from par.pagamento pp2 
                  inner join par.empenho ep3 on ep3.empid = pp2.empid and empstatus = 'A' 
                  inner join par.empenhoobra emo2 on emo2.empid = ep3.empid and eobstatus = 'A' 
                  where pp2.pagstatus='A' and emo2.preid=pre.preid) as numero_parcelas,
                  (select to_char(MIN(pp2.pagdatapagamentosiafi),'dd/mm/YYYY') from par.pagamento pp2 
                  inner join par.empenho ep3 on ep3.empid = pp2.empid and empstatus = 'A'
                  where pp2.pagstatus='A' AND ep3.empnumeroprocesso = emp.empnumeroprocesso AND pp2.pagparcela=1) as data_pagamento_siafi_1_parcela,
                  (	SELECT sum(ppo.pobvalorpagamento) 
					FROM par.pagamentoobra ppo
					INNER JOIN par.pagamento pag ON pag.pagid = ppo.pagid AND pag.agpstatus = 'A'
                  	WHERE ppo.preid = pre.preid)  as valor_pagamento_solicitado ,
                  res.resdescricao,
                  CASE WHEN pre.preesfera='E' THEN 'Estadual' WHEN pre.preesfera='M' THEN 'Municipal' END as Esfera,
                  esd_obra.esddsc as situacao_obra,
                  coalesce(o.obrpercentultvistoria,0) as percentual_execucao,
                  pre.preanometa as ano_meta,
                  to_char(o.obrdtinicio,'dd/mm/YYYY') as data_de_inicio_da_obra,
                  dataob.dataobra as data_de_termino_da_obra 
            FROM obras.preobra pre
            INNER JOIN workflow.documento doc ON doc.docid = pre.docid 
            INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid 
            LEFT JOIN obras2.obras o ON o.preid = pre.preid AND o.obrstatus = 'A' AND o.obridpai IS NULL
            LEFT JOIN (
					SELECT oi.obrid AS obrid,
						(SELECT to_char (min(supdata),'dd/mm/yyyy') FROM obras2.supervisao supv WHERE supv.obrid = oi.obrid AND supstatus = 'A' AND staid = 3) AS dataobra
					FROM obras2.obras oi
					INNER JOIN workflow.documento doc2 ON doc2.docid = oi.docid 
					WHERE oi.obrstatus = 'A'
					AND doc2.esdid = 693) dataob ON dataob.obrid = o.obrid
            LEFT JOIN workflow.documento doc_obra ON doc_obra.docid  = o.docid
            left JOIN workflow.estadodocumento esd_obra  on esd_obra.esdid = doc_obra.esdid 
            LEFT JOIN territorios.municipio mun ON mun.muncod = pre.muncod 
            LEFT JOIN territorios.muntipomunicipio mtm ON mtm.muncod = pre.muncod AND mtm.estuf = pre.estuf 
            LEFT JOIN territorios.tipomunicipio tmn ON tmn.tpmid = mtm.tpmid 
            LEFT JOIN workflow.estadodocumento esd   ON doc.esdid = esd.esdid   
            LEFT JOIN (SELECT preid,empid,SUM(eobpercentualemp) eobpercentualemp,SUM(eobvalorempenho) eobvalorempenho FROM par.empenhoobra where eobstatus = 'A' group by preid,empid) emo ON emo.preid = pre.preid 
            LEFT JOIN par.empenho emp ON emp.empid = emo.empid and empstatus = 'A' 
            LEFT JOIN par.processoobra pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
            LEFT JOIN par.resolucao res ON res.resid = pro.resid
            
            Left Join(
                  Select to_char(MAX(terdatainclusao),'dd/mm/YYYY') data_termo, teo.preid
                  From par.termocompromissopac ter 
                  Join par.termoobra teo on teo.terid = ter.terid 
                  Where ter.terassinado = true and ter.terstatus='A' 
				  GROUP BY teo.preid
                  --Order by ter.terid desc limit 1
            ) as ter on ter.preid = pre.preid
            
            WHERE tmn.tpmid IN(163,164,165)   AND pre.docid IS NOT NULL AND pre.prestatus = 'A' AND pre.tooid = 1  
	    // WHERE tmn.tpmid IN(163,164,165) AND doc.esdid IN('228','360','365','366','367')  AND pre.docid IS NOT NULL AND pre.prestatus = 'A' AND pre.tooid = 1 tirei a pedido do Paulo da SE - Daniel Brito
            
            GROUP BY  
                  pto.ptoclassificacaoobra,
                  pre.preid,
                  uf,
                  o.obrid,
                  mun.mundescricao, mun.muncod,
                  tmn.tpmdsc,
                  pto.ptodescricao,
                  pre.prevalorobra,
                  esd.esddsc,
                  pre.predescricao,
                  endereco_obra,
                  coordenada_geografica,
                  data_termo,
                  numero_parcelas,
                  data_pagamento_siafi_1_parcela,
                  valor_pagamento_solicitado ,
                  res.resdescricao,
                  esfera,
                  situacao_obra,
                  percentual_execucao,
                  pre.preanometa,
                  o.obrdtinicio,
                  data_de_termino_da_obra
            ORDER BY 
                  pre.preid";
	//ver(simec_htmlentities($sql), d);
	$registros = $db->carregar($sql);
	
	if($registros[0]) {
		
		$arCabecalho = array("tipo_obra",
							 "n_protocolo_simec",
							 "uf",
							 "obrid",
							 "municipio_beneficiado",
							 "cod_ibge",
							 "grupo_pac",
							 "tipo_projeto",
							 "valor_obra",
							 "situacao_analise_fnde",
							 "nome_obra",
							 "endereco_obra",
							 "coordenada_geografica",
							 "data_termo",
				  			 "resdescricao",
							 "valor_empenho_solicitado",
							 "valor_pagamento_solicitado",
							 "numero_parcelas",
							 "data_pagamento_siafi_1_parcela",
							 "preesfera",
							 "situacao_obra",
							 "percentual_execucao",
							 "ano_meta",
							 "data_de_inicio_da_obra",
							 "data_de_termino_da_obra"
		);
	
		foreach($registros as $reg) {
			
			$arDados[] = array('tipo_obra'=>$reg['tipo_obra'],
							   'n_protocolo_simec'=>$reg['n_protocolo_simec'],
							   'uf'=>$reg['uf'],
							   'obrid'=>$reg['obrid'],
							   'municipio_beneficiado'=>$reg['municipio_beneficiado'],
							   'cod_ibge'=>$reg['cod_ibge'],
							   'grupo_pac'=>$reg['grupo_pac'],
							   'tipo_projeto'=>$reg['tipo_projeto'],
							   'valor_obra'=>str_replace(".",",",round($reg['valor_obra'],2)),
							   'situacao_analise_fnde'=>$reg['situacao_analise_fnde'],
							   'nome_obra'=>$reg['nome_obra'],
							   'endereco_obra'=>$reg['endereco_obra'],
							   'coordenada_geografica'=>$reg['coordenada_geografica'],
							   'data_termo'=>$reg['data_termo'],
							   'resdescricao'=>$reg['resdescricao'],
							   'valor_empenho_solicitado'=>str_replace(".",",",round($reg['valor_empenho_solicitado'],2)),
							   'valor_pagamento_solicitado'=>str_replace(".",",",round($reg['valor_pagamento_solicitado'],2)),
							   'numero_parcelas'=>$reg['numero_parcelas'],
							   'data_pagamento_siafi'=>$reg['data_pagamento_siafi_1_parcela'],
							   'preesfera'=>$reg['preesfera'],
							   'situacao_obra'=>$reg['situacao_obra'],
							   'percentual_execucao'=>$reg['percentual_execucao'],
							   'ano_meta'=>$reg['ano_meta'],
							   'data_de_inicio_da_obra'=>$reg['data_de_inicio_da_obra'],
							   'data_de_termino_da_obra'=>$reg['data_de_termino_da_obra']
			
			
			);
		}
	}
	
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_RelatDocente".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados, $arCabecalho,100000,5,'N','100%',$par2);
	exit;
}
?>