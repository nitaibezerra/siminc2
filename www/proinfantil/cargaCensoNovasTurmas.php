<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

session_start();

//$_SESSION['usucpf'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "SELECT uf, muncod, municipio, dependencia, categoria, convenio, inep, 
		       escola, mat_creche_parcial, mat_creche_integral, mat_preescola_parcial, 
		       mat_preescola_integral, turma_creche, turma_preescola, turma_unificada, 
		       turma_multietapa
		  FROM carga.censonovasturmas2014 c
			where 
            	c.categoria in ('FILANTRÓPICA', 'CONFESSIONAL', 'COMUNITÁRIA')
		        and c.dependencia in ('PRIVADA')
		        and c.convenio in ('MUNICIPAL', 'ESTADUAL E MUNICIPA')
            	and muncod in(
select distinct
			muncod
		from(
		    select
		    	'privada' as tipo,
		        c.muncod,
		        c.municipio,
		        sum(c.mat_creche_parcial) as mat_creche_parcial_carga,
		        cen.mat_creche_parcial,
		        sum(c.mat_creche_integral) as mat_creche_integral_carga,
		        cen.mat_creche_integral,
		        sum(c.mat_preescola_parcial) as mat_preescola_parcial_carga,
		        cen.mat_preescola_parcial,
		        sum(c.mat_preescola_integral) as mat_preescola_integral_carga,
		        cen.mat_preescola_integral,
		        	        
		        sum(c.turma_creche) as turma_creche_carga,
		        cen.turma_creche,
		        sum(c.turma_preescola) as turma_preescola_carga,
		        cen.turma_preescola,
		        sum(c.turma_unificada) as turma_unificada_carga,        
		        cen.turma_unificada
		    from
		        carga.censonovasturmas2014 c
		        inner join(
		            SELECT
		                dc.muncod,
		                m.mundescricao as municipio,
		                (COALESCE(SUM(dc.ntcqtdalunocrecheparcialconveniada),0)) as mat_creche_parcial,
		                (COALESCE(SUM(dc.ntcqtdalunocrecheintegralconveniada),0)) as mat_creche_integral,
		                (COALESCE(SUM(dc.ntcqtdalunopreescolaparcialconveniada),0)) as mat_preescola_parcial,
		                (COALESCE(SUM(dc.ntcqtdalunopreescolaintegralconveniada),0)) as mat_preescola_integral,
		                					
		                (COALESCE(SUM(dc.ntcqtdturmacrecheconveniada),0)) as turma_creche,
		                (COALESCE(SUM(dc.ntcqtdturmapreescolaconveniada),0)) as turma_preescola,
		                (COALESCE(SUM(dc.ntcqtdturmaunificadaconveniada),0)) as turma_unificada
		            FROM proinfantil.novasturmasdadoscenso dc
		                inner join territorios.municipio m on m.muncod = dc.muncod
		            WHERE 
		                dc.ntcanocenso = '2014' and dc.ntcstatus = 'A'
		            GROUP BY
		                dc.ntcanocenso, dc.muncod, m.mundescricao
		        ) cen on cen.muncod = c.muncod
		     where c.categoria in ('FILANTRÓPICA', 'CONFESSIONAL', 'COMUNITÁRIA')
		        and c.dependencia in ('PRIVADA')
		        and c.convenio in ('MUNICIPAL', 'ESTADUAL E MUNICIPA')
		    group by 
		        c.muncod,
		        c.municipio,
		        cen.mat_creche_parcial,
		        cen.mat_creche_integral,
		        cen.mat_preescola_parcial,
		        cen.mat_preescola_integral,
		        cen.turma_creche,
		        cen.turma_preescola,
		        cen.turma_unificada
		   /* union all
		    select
		    	'municipal' as tipo,
		        c.muncod,
		        c.municipio,
		        sum(c.mat_creche_parcial) as mat_creche_parcial_carga,
		        cen.mat_creche_parcial,
		        sum(c.mat_creche_integral) as mat_creche_integral_carga,
		        cen.mat_creche_integral,
		        sum(c.mat_preescola_parcial) as mat_preescola_parcial_carga,
		        cen.mat_preescola_parcial,
		        sum(c.mat_preescola_integral) as mat_preescola_integral_carga,
		        cen.mat_preescola_integral,
		        	        
		        sum(c.turma_creche) as turma_creche_carga,
		        cen.turma_creche,
		        sum(c.turma_preescola) as turma_preescola_carga,
		        cen.turma_preescola,
		        sum(c.turma_unificada) as turma_unificada_carga,        
		        cen.turma_unificada
		    from
		        carga.censonovasturmas2014 c
		        inner join(
		            SELECT
		                dc.muncod,
		                m.mundescricao as municipio,
		                (COALESCE(SUM(dc.ntcqtdalunocrecheparcialpublica),0)) as mat_creche_parcial,
		                (COALESCE(SUM(dc.ntcqtdalunocrecheintegralpublica),0)) as mat_creche_integral,
		                (COALESCE(SUM(dc.ntcqtdalunopreescolaparcialpublica),0)) as mat_preescola_parcial,
		                (COALESCE(SUM(dc.ntcqtdalunopreescolaintegralpublica),0)) as mat_preescola_integral,
		                					
		                (COALESCE(SUM(dc.ntcqtdturmacrechepublica),0)) as turma_creche,
		                (COALESCE(SUM(dc.ntcqtdturmapreescolapublica),0)) as turma_preescola,
		                (COALESCE(SUM(dc.ntcqtdturmaunificadapublica),0)) as turma_unificada
		            FROM proinfantil.novasturmasdadoscenso dc
		                inner join territorios.municipio m on m.muncod = dc.muncod
		            WHERE 
		                dc.ntcanocenso = '2014' and dc.ntcstatus = 'A'
		            GROUP BY
		                dc.ntcanocenso, dc.muncod, m.mundescricao
		        ) cen on cen.muncod = c.muncod
		     where c.dependencia in ('MUNICIPAL')
		    group by 
		        c.muncod,
		        c.municipio,
		        cen.mat_creche_parcial,
		        cen.mat_creche_integral,
		        cen.mat_preescola_parcial,
		        cen.mat_preescola_integral,
		        cen.turma_creche,
		        cen.turma_preescola,
		        cen.turma_unificada*/
		) as foo
		where
			(mat_creche_parcial <> mat_creche_parcial_carga
		          or mat_creche_integral <> mat_creche_integral_carga
		          or mat_preescola_parcial <> mat_preescola_parcial_carga
		          or mat_preescola_integral <> mat_preescola_integral_carga
		          or turma_creche <> turma_creche_carga
		          or turma_preescola <> turma_preescola_carga
		          or turma_unificada <> turma_unificada_carga)
			--and muncod not in (select muncod from proinfantil.novasturmasdadosmunicipiospormes where ntmmano = '2015' and ntmmstatus = 'A')
            )";
$arrCenso = $db->carregar($sql);
$arrCenso = $arrCenso ? $arrCenso : array();

$contPrivada = 0;
$contMunicipal = 0;
foreach ($arrCenso as $v) {
	
	$sql = "select count(n.ntcid) from proinfantil.novasturmasdadoscenso n 
			where n.muncod = '{$v['muncod']}' 
				and n.entcodent = '{$v['inep']}' 
			    and n.ntcanocenso = '2014' 
			    and n.ntcstatus = 'A'";
	
	$boTem = $db->pegaUm($sql);
	
	if( (int)$boTem > 0 ){
		//if( $v['tipo'] == 'privada' ){
			$sql = "UPDATE proinfantil.novasturmasdadoscenso SET					
						ntcqtdalunocrecheparcialconveniada		= {$v['mat_creche_parcial']},
						ntcqtdalunocrecheintegralconveniada 	= {$v['mat_creche_integral']},
						ntcqtdalunopreescolaparcialconveniada 	= {$v['mat_preescola_parcial']},
						ntcqtdalunopreescolaintegralconveniada 	= {$v['mat_preescola_integral']},
						
						ntcqtdturmacrecheconveniada 			= {$v['turma_creche']},
						ntcqtdturmapreescolaconveniada 			= {$v['turma_preescola']}, 
						ntcqtdturmaunificadaconveniada 			= {$v['turma_unificada']},
						ntcstatus = 'A'
					 WHERE 
					 	ntcanocenso 	= '2014' 
					 	and muncod 		= '{$v['muncod']}'
					 	and entcodent 	= '{$v['inep']}'
					 	and ntcstatus 	= 'A'";	
			$db->executar($sql);
			$contPrivada++;
		/*} else {
			$sql = "UPDATE proinfantil.novasturmasdadoscenso SET					
					ntcqtdalunocrecheparcialpublica 	= {$v['mat_creche_parcial']},
					ntcqtdalunocrecheintegralpublica 	= {$v['mat_creche_integral']},
					ntcqtdalunopreescolaparcialpublica 	= {$v['mat_preescola_parcial']},
					ntcqtdalunopreescolaintegralpublica = {$v['mat_preescola_integral']},
					
					ntcqtdturmacrechepublica 			= {$v['turma_creche']},
					ntcqtdturmapreescolapublica  		= {$v['turma_preescola']},
					ntcqtdturmaunificadapublica 		= {$v['turma_unificada']}
				 WHERE 
				 	ntcanocenso 	= '2014'
				 	and muncod 		= '{$v['muncod']}'
				 	and entcodent 	= '{$v['inep']}'
				 	and ntcstatus 	= 'A'";
			$db->executar($sql);
			$contMunicipal++;
		}*/
	} else {
		ver($v,d);
	}
	$db->commit();
}

ver($contPrivada, $contMunicipal);
/* $sql = "UPDATE proinfantil.novasturmasdadoscenso SET 
					ntcqtdalunocrecheparcialpublica = {$v['mat_creche_parcial']},
					ntcqtdalunocrecheintegralpublica = {$v['mat_creche_integral']},
					ntcqtdalunopreescolaparcialpublica = {$v['mat_preescola_parcial']},
					ntcqtdalunopreescolaintegralpublica = {$v['mat_preescola_integral']},
					
					ntcqtdturmacrechepublica = {$v['turma_creche']},
					ntcqtdturmapreescolapublica  = {$v['turma_preescola']},
					ntcqtdturmaunificadapublica = {$v['turma_unificada']},
					
					ntcqtdalunocrecheparcialconveniada	= {$v['mat_creche_parcial']},
					ntcqtdalunocrecheintegralconveniada = {$v['mat_creche_integral']},
					ntcqtdalunopreescolaparcialconveniada = {$v['mat_preescola_parcial']},
					ntcqtdalunopreescolaintegralconveniada = {$v['mat_preescola_integral']},
					
					ntcqtdturmacrecheconveniada = {$v['turma_creche']},
					ntcqtdturmapreescolaconveniada = {$v['turma_preescola']}, 
					ntcqtdturmaunificadaconveniada = {$v['turma_unificada']}
				 WHERE 
				 	ntcanocenso = '2014', 
				 	and muncod = {$v['muncod']}
				 	and entcodent = {$v['inep']}
				 	and ntcstatus = 'A'"; */
