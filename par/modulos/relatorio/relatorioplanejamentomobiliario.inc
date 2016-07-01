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
echo "	<input type=button name=xls value=XLS onclick=\"window.location='par.php?modulo=relatorio/relatorioplanejamentomobiliario&acao=A&exibirxls=1';\">";
echo "</p>";



if($_REQUEST['exibirxls']){	
	
	$sql = "select 
                rel.uf,rel.mund,rel.muncod,rel.sbadsc,rel.ordem,rel.sbaid,rel.quantidade,sum(rel.empenho_solicitado_subacao) Empenhado, sum (rel.pagamento_solicitado_subacao) Pago, rel.tipo ,ptres
from 
                (select case when inu.itrid = 1 then inu.estuf else inu.mun_estuf end UF ,
                               mun.mundescricao mund,
                               mun.muncod, 
                               sba.sbaid,
                               sba.sbadsc sbadsc,
                               sba.sbaordem ordem,
                               emp.empnumero,
                               CASE WHEN sba.sbacronograma = 1 THEN
                                               sum(ico.icoquantidadetecnico)
                               ELSE
                                               sum(ssi.seiqtdtecnico)
                               END as quantidade,                                                                               
                               empsub.eovalor + coalesce(ref.vlrref,0) empenho_solicitado_subacao, 
                               pag.valorpag pagamento_solicitado_subacao, 
                               case when inu.itrid = 2 and inu.inuid <> 1 then 'Municipal' else 'Estadual' end   tipo ,
                               sbd.sbdptres ptres
                from 
                               par.subacao sba 
                left join (select distinct sbaid sbaid, sbdptres sbdptres from par.subacaodetalhe where sbdptres is not null ) sbd on sbd.sbaid = sba.sbaid 
                inner join (select empid empid, sbaid sbaid, sum(eobvalorempenho) eovalor from par.empenhosubacao where eobstatus = 'A' group by empid,sbaid) empsub on empsub.sbaid = sba.sbaid
				left join (select empnumeroprocesso, empidpai, sum(empvalorempenho) as vlrref, empcodigoespecie 
							from par.empenho
							where empcodigoespecie in ('02') and empstatus = 'A'
							group by 
								empnumeroprocesso,
								empcodigoespecie,
								empidpai) as ref on ref.empidpai = empsub.empid			
                left join (select empid empipag, sum (pagvalorparcela) valorpag from par.pagamento where pagsituacaopagamento not ilike '%CANCELADO%' AND pagstatus = 'A' GROUP BY EMPID) pag ON pag.empipag = empsub.empid
                inner join par.empenho emp on emp.empid = empsub.empid and empstatus = 'A'
                inner join par.subacaoitenscomposicao ico on ico.sbaid = sba.sbaid and ico.icoano = 2012 and ico.icostatus = 'A' and ico.icovalidatecnico = 'S'
                --left join par.subacaoescolas sbe ON sbe.sbaid = sba.sbaid and sbe.sesano = 2012 AND sbe.sesstatus = 'A'
                left join par.subescolas_subitenscomposicao ssi ON ssi.icoid = ico.icoid --AND ssi.sesid = sbe.sesid
                inner join par.propostaitemcomposicao pic on pic.picid = ico.picid and pic.picstatus = 'A'
                inner join par.processopar prp on prp.prpnumeroprocesso = emp.empnumeroprocesso and prp.prpstatus = 'A'
                inner join par.instrumentounidade inu on inu.inuid = prp.inuid 
                left join territorios.municipio mun on mun.muncod = prp.muncod
                where emp.empsituacao <> 'CANCELADO' and sba.sbastatus = 'A' and ico.picid in (468,467,466,471,472,469,470)
                               --and sba.sbacronograma = 2
                               --and sba.sbaid = 2604808
                GROUP BY uf, mun.mundescricao, mun.muncod,sba.sbaid,sba.sbaordem,sba.sbadsc,empenho_solicitado_subacao,pag.valorpag,tipo,sba.sbacronograma,emp.empnumero,sbd.sbdptres 
                --having sum(ico.icoquantidadetecnico) > 0
                order by tipo,uf,mund)  rel 
group by 
                rel.uf,rel.mund,rel.muncod,rel.sbadsc,rel.ordem,rel.sbaid,rel.quantidade,rel.tipo,ptres
order by 
                rel.tipo,rel.uf,rel.mund";
	
	$registros = $db->carregar($sql);
	
	if($registros[0]) {

		$arCabecalho = array("uf",
							 "mund",
							 "muncod",
							 "sbadsc",
							 "ordem",
							 "sbaid",
							 "quantidade",
							 "Empenhado",
							 "Pago",
							 "tipo",
							 "ptres"
		);
	
		foreach($registros as $reg) {
			$arDados[] = array('uf'=>$reg['uf'],
							   'mund'=>$reg['mund'],
							   'muncod'=>$reg['muncod'],
							   'sbadsc'=>$reg['sbadsc'],
							   'ordem'=>$reg['ordem'],
							   'sbaid'=>$reg['sbaid'],
							   'quantidade'=>$reg['quantidade'],
							   'Empenhado'=>$reg['empenhado'],
							   'Pago'=>$reg['pago'],
							   'tipo'=>$reg['tipo'],
							   'ptres'=>$reg['ptres']
			
			
			);
		}
	}
	
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_RelatPlanejamentoPAR".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados, $arCabecalho,100000,5,'N','100%',$par2);
	exit;
}
?>