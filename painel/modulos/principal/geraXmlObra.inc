<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

$sql = "SELECT
			oi.obrid,
			pre.preid, 
			pre.preid as n_protocolo_simec,
		    pf.prfdesc as tipo_obra_programa,
		    pre.predescricao as nome_obra,
		    'Município' as proponente,
		    tmn.tpmdsc as grupo_pac,
		    pto.ptodescricao as tipo_projeto,
		    CASE WHEN ptoclassificacaoobra='P' THEN 'Proinfancia'
				 WHEN ptoclassificacaoobra='Q' THEN 'Quadra' END as tipo_obra,
		    --begin endereco_obra
		    pre.prelatitude as latitude,
		    pre.prelongitude as longitude,
		    pre.prelogradouro as logradouro,
		    pre.precep as cep_endereco,
		    mun.muncod as cod_ibge_endereco,
		    pre.estuf as uf_endereco,
		    --end endereco_obra
		    --begin municipio_beneficiado
		    mun.muncod as cod_ibge_municipio,
		    mun.mundescricao as nome_municipio,
		    mun.estuf as uf_municipio,
		    --end municipio_beneficiado
		    'Urbana' as classificacao_area,
		    (select to_char(terdatainclusao,'dd/mm/YYYY') from par.termocompromissopac ter 
					 inner join par.termoobra teo on teo.terid = ter.terid 
					 where ter.terassinado=true and teo.preid = pre.preid and ter.terstatus='A' order by ter.terid desc limit 1) as data_termo,
		    (select sum(pp.prevalorobra) from obras.preobra pp 
					 inner join par.empenhoobra eo2 on eo2.preid = pp.preid 
					 inner join par.empenho ep2 on ep2.empid = eo2.empid and empstatus = 'A' and eobstatus = 'A' 
					 inner join par.processoobra po2 on po2.pronumeroprocesso = ep2.empnumeroprocesso and po2.prostatus = 'A'
					 where po2.proid = (select ter.proid from par.termocompromissopac ter 
					 inner join par.termoobra teo on teo.terid = ter.terid 
					 where teo.preid = pre.preid and ter.terstatus='A' order by ter.terid desc limit 1)) as valor_termo,
		    pre.prevalorobra as valor_obra,
		    esd.esddsc as situacao_analise_fnde,
		    emo.eobvalorempenho as valor_empenho_solicitado,
		    ((select sum(pp2.pagvalorparcela) from par.pagamento pp2 
					 inner join par.empenho ep3 on ep3.empid = pp2.empid and empstatus = 'A'
					 where pp2.pagstatus='A' AND ep3.empnumeroprocesso = emp.empnumeroprocesso ) / 
					 (select sum(pp.prevalorobra) from obras.preobra pp 
					 inner join par.empenhoobra eo2 on eo2.preid = pp.preid 
					 inner join par.empenho ep2 on ep2.empid = eo2.empid and empstatus = 'A' and eobstatus = 'A' 
					 where ep2.empnumeroprocesso = emp.empnumeroprocesso)*pre.prevalorobra) as valor_pagamento_solicitado,
		    (select sum(pp2.pagvalorparcela) from par.pagamento pp2 
					 inner join par.empenho ep3 on ep3.empid = pp2.empid and empstatus = 'A'
					 where pp2.pagstatus='A' AND ep3.empnumeroprocesso = emp.empnumeroprocesso ) / 
					 (select sum(pp.prevalorobra) from obras.preobra pp 
					 inner join par.empenhoobra eo2 on eo2.preid = pp.preid 
					 inner join par.empenho ep2 on ep2.empid = eo2.empid and empstatus = 'A' and eobstatus = 'A' 
					 where ep2.empnumeroprocesso = emp.empnumeroprocesso) as percentual_pagamento_solicitado,
		    ent.entnome as unidade_implantadora,
		    oi.obrcustocontrato as valor_contrato_obra,
		    '' as termos_aditivos,
		    '' as licitacao,
		    ent1.entnome as nome_construtora,
		    oi.obrdtassinaturacontrato as data_assinatura_contrato_construtora,
		    so.stodesc as situacao_obra,
		    oi.obrqtdconstruida as total_area_a_ser_construida,
		    '' as cronograma_fisico,
		    '' as restricoes,
		    oi.obrpercexec as percentual_conclusao_obra
		    
		FROM
			obras.preobra pre
		    left join obras.obrainfraestrutura oi 
		      left join obras.programafonte pf on pf.prfid = oi.prfid
		      left join entidade.entidade ent on ent.entid = oi.entidunidade
		      left join entidade.entidade ent1 on ent1.entid = oi.entidempresaconstrutora
		      left join obras.situacaoobra so on so.stoid = oi.stoid
		    on oi.preid = pre.preid and oi.obsstatus = 'A'
		    INNER JOIN workflow.documento doc ON doc.docid = pre.docid 
		    LEFT JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid 
		    LEFT JOIN territorios.municipio mun ON mun.muncod = pre.muncod 
		    LEFT JOIN territorios.muntipomunicipio mtm ON mtm.muncod = pre.muncod AND mtm.estuf = pre.estuf 
		    LEFT JOIN territorios.tipomunicipio tmn ON tmn.tpmid = mtm.tpmid 
		    LEFT JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid   
		    LEFT JOIN par.empenhoobra emo ON emo.preid = pre.preid 
		    LEFT JOIN par.empenho emp ON emp.empid = emo.empid and empstatus = 'A' and eobstatus = 'A' 
		    LEFT JOIN par.processoobra pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
		    LEFT JOIN par.resolucao res ON res.resid = pro.resid
		WHERE tmn.tpmid IN(150,151,152) AND esd.esdid IN('228','360') and pre.prestatus = 'A' 
		ORDER BY pre.preid";

$arrRegistro = $db->carregar( $sql );
$arrRegistro = $arrRegistro ? $arrRegistro : array();
$arqXml = '';
$arqXml .= "
<data>
	<obras>";
foreach ($arrRegistro as $key => $v) {
	$n_protocolo_simec = $v['n_protocolo_simec']; 
	$tipo_obra_programa = $v['tipo_obra_programa'];
	$nome_obra = $v['nome_obra'];
	$proponente = $v['proponente'];
	$grupo_pac = $v['grupo_pac'];
	$tipo_projeto = $v['tipo_projeto'];
	$tipo_obra = $v['tipo_obra'];
	$latitude = $v['latitude'];  
	$longitude = $v['longitude'];  
	$logradouro = $v['logradouro'];
	$cep_endereco = $v['cep_endereco'];
	$cod_ibge_endereco = $v['cod_ibge_endereco'];
	$uf_endereco = $v['uf_endereco'];
	$cod_ibge_municipio = $v['cod_ibge_municipio'];
	$nome_municipio = $v['nome_municipio'];
	$uf_municipio = $v['uf_municipio'];
	$classificacao_area = $v['classificacao_area'];
	$data_termo = $v['data_termo'];
	$valor_termo = $v['valor_termo'];
	$valor_obra = $v['valor_obra'];
	$situacao_analise_fnde = $v['situacao_analise_fnde'];
	$valor_empenho_solicitado = $v['valor_empenho_solicitado'];
	$valor_pagamento_solicitado = $v['valor_pagamento_solicitado'];
	$percentual_pagamento_solicitado = $v['percentual_pagamento_solicitado'];
	$unidade_implantadora = $v['unidade_implantadora'];
	$valor_contrato_obra = $v['valor_contrato_obra'];
	$termos_aditivos = $v['termos_aditivos'];
	$licitacao = $v['licitacao'];
	$nome_construtora = $v['nome_construtora'];
	$data_assinatura_contrato_construtora = $v['data_assinatura_contrato_construtora'];
	$situacao_obra = $v['situacao_obra'];
	$total_area_a_ser_construida = $v['total_area_a_ser_construida'];
	$cronograma_fisico = $v['cronograma_fisico'];
	$restricoes = $v['restricoes'];
	$percentual_conclusao_obra = $v['percentual_conclusao_obra'];
	
	$xmlRestricao = '';
	$xmlCronograma = '';
	$xmlLicitacao = '';
	$termos_aditivos = '';
	if( !empty($v['obrid']) ){
		
		$sql = "SELECT 
					fl.*,
					tfl.tfldesc, tfl.tflordem   
				FROM 
					obras.faselicitacao fl 
				INNER JOIN 
					obras.tiposfaseslicitacao tfl ON fl.tflid = tfl.tflid
				WHERE 
					fl.obrid = '". $v['obrid'] . "' AND fl.flcstatus = 'A' ORDER BY tfl.tflordem";
		
		$arrLicitacao = $db->carregar( $sql );
		$arrLicitacao = $arrLicitacao ? $arrLicitacao : array();
		
		foreach ($arrLicitacao as $vl) {
			extract($vl);	
			if($tflid ==2){
				$flcdata = $flcpubleditaldtprev;
			}
			if($tflid ==5){
				$flcdata = $flcdtrecintermotivo;
			}
			if($tflid ==6){
				$flcdata = $flcordservdt;
			}
			if($tflid ==9){
				$flcdata = $flchomlicdtprev; 
			}
			if($tflid ==7){
				$flcdata = $flcaberpropdtprev;
			}
			$xmlLicitacao .= $tfldesc.', '.$flcdata; 
		}
		
		$sql = "SELECT 
					ic.itcdesc,
					icodtinicioitem,
					icodterminoitem
				FROM 
					obras.itenscomposicaoobra i,
					obras.itenscomposicao ic 
				WHERE 
					i.obrid = ".$v['obrid']." 
					and i.itcid = ic.itcid 
					and i.icostatus='A' 
					AND i.icovigente = 'A'
					 AND traid IS null
				ORDER BY 
					i.icoordem";
		$arrCronograma = $db->carregar( $sql );
		$arrCronograma = $arrCronograma ? $arrCronograma : array();
		
		foreach ($arrCronograma as $vc) {
			$xmlCronograma.= $vc['itcdesc'].', '.$vc['icodtinicioitem'].', '.$vc['icodterminoitem'];
		}
		
		$sql = "SELECT trtdesc FROM obras.restricaoobra r 
					INNER JOIN obras.tiporestricao t on t.trtid = r.trtid	 
				WHERE rststatus = 'A' AND obrid = ".$v['obrid'];
		$arRestricao = $db->carregarColuna( $sql );
		$arRestricao = $arRestricao ? $arRestricao : array();
		
		$xmlRestricao.= implode(', ', $arRestricao);
	}
	
	$sql = "select to_char(terdatainclusao,'dd/mm/YYYY') as data, (select sum(pp.prevalorobra)as valor from obras.preobra pp 
                                                         inner join par.empenhoobra eo2 on eo2.preid = pp.preid 
                                                         inner join par.empenho ep2 on ep2.empid = eo2.empid and empstatus = 'A' and eobstatus = 'A' 
                                                         inner join par.processoobra po2 on po2.pronumeroprocesso = ep2.empnumeroprocesso and po2.prostatus = 'A'
                                                         where po2.proid in (select ter1.proid from par.termocompromissopac ter1 
                                                                            inner join par.termoobra teo on teo.terid = ter1.terid 
                                                                         where teo.preid = ".$v['preid']." and ter1.terid = ter.terid and ter1.terstatus='A') ) as valor 
			from par.termocompromissopac ter 
			 	inner join par.termoobra teo on teo.terid = ter.terid 
			where ter.terassinado=true and teo.preid = ".$v['preid']." and ter.terstatus='A' order by ter.terid desc";
	
	$arTermo = $db->carregar( $sql );
	$arTermo = $arTermo ? $arTermo : array();
	
	foreach ($arTermo as $v) {
		$termos_aditivos .= $v['valor'].', '.$v['data'].', '.$v['data'].'/'.$v['valor'];	
	}

$arqXml .= "
		<obra>
			<n_protocolo_simec>$n_protocolo_simec</n_protocolo_simec>
			<tipo_obra_programa>$tipo_obra</tipo_obra_programa>
			<nome_obra>$nome_obra</nome_obra>
			<proponente>$proponente</proponente>
			<grupo_pac>$grupo_pac</grupo_pac>
			<tipo_projeto>$tipo_projeto</tipo_projeto>
			<tipo_obra>$tipo_obra</tipo_obra>
			<endereco_obra>
				<coordenadas_geograficas>
					<latitude>$latitude</latitude>
					<longitude>$longitude</longitude>
				</coordenadas_geograficas>
				<logradouro>$logradouro</logradouro>
				<cep>$cep_endereco</cep>
				<cod_ibge>$cod_ibge_endereco</cod_ibge>
				<uf>$uf_endereco</uf>
			</endereco_obra>
			<municipio_beneficiado>
				<cod_ibge>$cod_ibge_municipio</cod_ibge>
				<nome>$nome_municipio</nome>
				<uf>$uf_municipio</uf>
			</municipio_beneficiado>
			<classificacao_area>$classificacao_area</classificacao_area>
			<data_termo>$data_termo</data_termo>
			<valor_termo>$valor_termo</valor_termo>
			<valor_obra>$valor_obra</valor_obra>
			<situacao_analise_fnde>$situacao_analise_fnde</situacao_analise_fnde>
			<valor_empenho_solicitado >$valor_empenho_solicitado</valor_empenho_solicitado >
			<percentual_empenho_solicitado>$percentual_empenho_solicitado</percentual_empenho_solicitado>
			<valor_pagamento_solicitado>$valor_pagamento_solicitado</valor_pagamento_solicitado>
			<percentual_pagamento_solicitado>$percentual_pagamento_solicitado</percentual_pagamento_solicitado>
			<unidade_implantadora>$unidade_implantadora</unidade_implantadora>
			<valor_contrato_obra>$valor_contrato_obra</valor_contrato_obra>
			<termos_aditivos>$termos_aditivos</termos_aditivos>
			<licitacao>$xmlLicitacao</licitacao>
			<nome_construtora>$nome_construtora</nome_construtora>
			<data_assinatura_contrato_construtora>$data_assinatura_contrato_construtora</data_assinatura_contrato_construtora>
			<situacao_obra>$situacao_obra</situacao_obra>
			<total_area_a_ser_construida>$total_area_a_ser_construida</total_area_a_ser_construida>
			<cronograma_fisico>$xmlCronograma</cronograma_fisico>
			<restricoes>$xmlRestricao</restricoes>
			<percentual_conclusao_obra>$percentual_conclusao_obra</percentual_conclusao_obra>
		</obra>
";

}
$arqXml .= "
	</obras>
</data>
";
//ver( simec_htmlentities( $arqXml ),d );
echo $arqXml;

$filename = str_replace(" ", "_", 'xmlobras.xml');
header( 'Content-type: text/xml');
header( 'Content-Disposition: attachment; filename='.$filename);
die;
//ver( simec_htmlentities($arqXml),d );
?>