<?php
set_time_limit(0);
ini_set("memory_limit", "3000M");
$where = array();
extract($_REQUEST);

if( $prfid[0] && $prfid_campo_flag ){
	if ( !$prfid_campo_excludente ){
		array_push($where, " oi.prfid  IN (" . implode( ',', $prfid ) . ") ");	
	}else{
		array_push($where, " ( oi.prfid  NOT IN (" . implode( ',', $prfid ) . ") OR oi.prfid is null ) ");
	}
	
}
// Fonte
if( $tooid[0] && $tooid_campo_flag ){
	if ( !$tooid_campo_excludente ){
		array_push($where, " oi.tooid  IN (" . implode( ',', $tooid ) . ") ");	
	}else{
		array_push($where, " ( oi.tooid  NOT IN (" . implode( ',', $tooid ) . ") OR oi.tooid IS NULL ) ");
	}
	
}

if( !empty( $entidunidade ) ) array_push($where, "oi.entidunidade = {$entidunidade} ");
if( !empty( $stoid ) ) array_push($where, "oi.stoid = {$stoid}");
if( !empty( $estuf ) ) array_push($where, "ed.estuf = '{$estuf}'");
if( !empty( $obrtextobusca ) ) array_push($where, "(oi.preid = ".(int)$obrtextobusca." or oi.obrid =".(int)$obrtextobusca.")");

if( $tpoid ) array_push( $where, " tl.tpoid = $tpoid" );
if( $carimbo ){
	if( $carimbo == 'E' ){		
		$tituloCarimbo = 'Externo';
		$descAdequado = 'Se estiver dentro do prazo do FNDE. ';
		//$descAtencao = 'Entre o prazo do FNDE e 3 meses depois. ';
		//$descPreocupante = '3 meses e 1 dia depois do prazo do FNDE.';
		$descAtencao = 'Entre 1 mês e 1 dia antes do prazo do FNDE e 1 mês depois do prazo do FNDE.';
		$descPreocupante = '1 meses e 1 dia depois do prazo do FNDE.';
	}
	if( $carimbo == 'I' ){		
		$tituloCarimbo = 'Interno';
		$descAdequado = 'Até 1 mês antes do prazo do FNDE.';
		$descAtencao = 'Entre 1 mês antes do prazo do FNDE e 1 mês depois do prazo do FNDE.';
		$descPreocupante = '2 meses depois do prazo do FNDE.';
	}
}
//ver($_REQUEST);
$whereNivel = '';
$whereNivel25 = '';
$whereNivel50 = '';
$whereNivel75 = '';
$whereNivel100 = '';
$boNivelHmo = 'N';
$boNivel25 = 'N';
$boNivel50 = 'N';
$boNivel75 = 'N';
$boNivel100 = 'N';


$imgAdequado = '<center><img src=\"/imagens/icones/bg.png \" border=0 alt=\"Ir\" title=\"Adequado\"></center>';
$imgAtenção = '<center><img src=\"/imagens/icones/by.png \" border=0 alt=\"Ir\" title=\"Atenção\"></center>';
$imgPreocupante = '<center><img src=\"/imagens/icones/br.png \" border=0 alt=\"Ir\" title=\"Preocupante\"></center>';

if($_REQUEST['tipoRelatorio'] == "excel"){
	$imgAdequado = 'Adequado';
	$imgAtenção = 'Atenção';
	$imgPreocupante = 'Preocupante';
	$imgConcluido = 'Concluído';
}else{
	$imgAdequado = '<center><img src=\"/imagens/icones/carimbos/adequado.png\" border=0 alt=\"Ir\" title=\"Adequado\"></center>';
	$imgAtenção = '<center><img src=\"/imagens/icones/carimbos/atencao.png\" border=0 alt=\"Ir\" title=\"Atenção\"></center>';
	$imgPreocupante = '<center><img src=\"/imagens/icones/carimbos/preocupante.png\" border=0 alt=\"Ir\" title=\"Preocupante\"></center>';
	$imgConcluido = '<center><img src=\"/imagens/icones/carimbos/concluido.png\" border=0 alt=\"Ir\" title=\"Concluído\"></center>';
}
$arStatusAtual = array();
if($nivel){
	foreach($nivel as $niv){
		
		if(strstr($niv,"S")){ //Homologação
			if(strstr($niv,"D")){ //Adequado
				$arrStatusAtual[] = " status_geral_".strtolower($tituloCarimbo)." = 'verde' ";
				$arStatusAtual[] = $imgAdequado;
			}elseif(strstr($niv,"A")){ //Atenção
				$arrStatusAtual[] = " status_geral_".strtolower($tituloCarimbo)." = '".strtolower($tituloCarimbo)."hmo' ";
				$arStatusAtual[] = $imgAtenção;
			}elseif(strstr($niv,"P")){ //Preocupante
				$arrStatusAtual[] = " status_geral_".strtolower($tituloCarimbo)." = '".strtolower($tituloCarimbo)."25' ";
				$arStatusAtual[] = $imgPreocupante;
			}			
		}else		
		if(strstr($niv,"H")){ //Homologação
			if(strstr($niv,"D")){ //Adequado
				$arrHomologacao[] = " ".strtolower($tituloCarimbo)."hmo = '$imgAdequado' ";
			}elseif(strstr($niv,"A")){ //Atenção
				$arrHomologacao[] = " ".strtolower($tituloCarimbo)."hmo = '$imgAtenção' ";
			}elseif(strstr($niv,"P")){ //Preocupante
				$arrHomologacao[] = " ".strtolower($tituloCarimbo)."hmo = '$imgPreocupante' ";
			}
		}else{ //Outros (25,50,75,100)
			$campo = false;
			$campo = str_replace(array("D","A","P"),array("","",""),$niv);
			if(strstr($niv,"D")){ //Adequado
				$arr[$campo][] = " ".strtolower($tituloCarimbo).$campo." = '$imgAdequado' ";
			}elseif(strstr($niv,"A")){ //Atenção
				$arr[$campo][] = " ".strtolower($tituloCarimbo).$campo." = '$imgAtenção' ";
			}elseif(strstr($niv,"P")){ //Preocupante
				$arr[$campo][] = " ".strtolower($tituloCarimbo).$campo." = '$imgPreocupante' ";
			}
		}
	}
}

$whereExterno = " where 1=1 ";
//$whereExterno .= ( $arrStatusAtual ? " and (".implode(" or ", $arrStatusAtual).") " : "" );
$whereExterno .= ( $arrHomologacao ? " and (".implode(" or ", $arrHomologacao).") " : "" );
$whereExterno .= ( $arr[25] ? " and (".implode(" or ", $arr[25]).") " : "" );
$whereExterno .= ( $arr[50] ? " and (".implode(" or ", $arr[50]).") " : "" );
$whereExterno .= ( $arr[75] ? " and (".implode(" or ", $arr[75]).") " : "" );
$whereExterno .= ( $arr[100] ? " and (".implode(" or ", $arr[100]).") " : "" );

/*
if( $nivel ){
	foreach ($nivel as $vNivel) {
		#homologação
		if( $vNivel == 'HD' ) {
			if( $boNivelHmo == 'S' ){
				$whereNivel .= " or (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= rc.rcdqtdhomologacao";
			} else {
				$boNivelHmo = 'S';
				$whereNivel .= " (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= rc.rcdqtdhomologacao";				
			}
		} elseif( $vNivel == 'HA' ){
			if( $boNivelHmo == 'S' ){
				$whereNivel .= " or (cast(now() as date) - cast(pa.pagdatapagamento as date)) > rc.rcdqtdhomologacao and (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtdhomologacao + 90) and (v.vldstatushomologacao = 'N' or v.vldstatushomologacao is null)";
			} else {
				$boNivelHmo = 'S';
				$whereNivel .= " (cast(now() as date) - cast(pa.pagdatapagamento as date)) > rc.rcdqtdhomologacao and (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtdhomologacao + 90) and (v.vldstatushomologacao = 'N' or v.vldstatushomologacao is null)";				
			}
		} elseif( $vNivel == 'HP' ){
			if( $boNivelHmo == 'S' ){
				$whereNivel .= " or (cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtdhomologacao + 120) and (v.vldstatushomologacao = 'N' or v.vldstatushomologacao is null)";
			} else {
				$boNivelHmo = 'S';
				$whereNivel .= " (cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtdhomologacao + 120) and (v.vldstatushomologacao = 'N' or v.vldstatushomologacao is null)";				
			}
		}
		#execução 25%		
		if( $vNivel == '25D' ){
			if( $boNivel25 == 'S' ){
				$whereNivel25 .= " or (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= rc.rcdqtd30exec";
			} else {
				$boNivel25 = 'S';
				$whereNivel25 .= " (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= rc.rcdqtd30exec";				
			}
		}
		elseif( $vNivel == '25A' ){
			if( $boNivel25 == 'S' ){
				$whereNivel25 .= " or (cast(now() as date) - cast(pa.pagdatapagamento as date)) > rc.rcdqtd30exec and (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtd30exec + 90)";
			} else {
				$boNivel25 = 'S';
				$whereNivel25 .= " (cast(now() as date) - cast(pa.pagdatapagamento as date)) > rc.rcdqtd30exec and (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtd30exec + 90)";				
			}
		}
		elseif( $vNivel == '25P' ){
			if( $boNivel25 == 'S' ){
				$whereNivel25 .= " or (cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtd30exec + 120)";
			} else {
				$boNivel25 = 'S';
				$whereNivel25 .= " (cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtd30exec + 120)";				
			}
		}
		#execução 50%		
		if( $vNivel == '50D' ){
			if( $boNivel50 == 'S' ){
				$whereNivel50 .= " or (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= rc.rcdqtd50exec";
			} else {
				$boNivel50 = 'S';
				$whereNivel50 .= " (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= rc.rcdqtd50exec";				
			}
		}
		elseif( $vNivel == '50A' ){
			if( $boNivel50 == 'S' ){
				$whereNivel50 .= " or (cast(now() as date) - cast(pa.pagdatapagamento as date)) > rc.rcdqtd50exec and (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtd50exec + 90)";
			} else {
				$boNivel50 = 'S';
				$whereNivel50 .= " (cast(now() as date) - cast(pa.pagdatapagamento as date)) > rc.rcdqtd50exec and (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtd50exec + 90)";				
			}
		}
		elseif( $vNivel == '50P' ){
			if( $boNivel50 == 'S' ){
				$whereNivel50 .= " or (cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtd50exec + 120)";
			} else {
				$boNivel50 = 'S';
				$whereNivel50 .= "(cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtd50exec + 120)";				
			}
		}
		#execução 75%		
		if( $vNivel == '75D' ){
			if( $boNivel75 == 'S' ){
				$whereNivel75 .= " or (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= rc.rcdqtd75exec";
			} else {
				$boNivel75 = 'S';
				$whereNivel75 .= "(cast(now() as date) - cast(pa.pagdatapagamento as date)) <= rc.rcdqtd75exec";				
			}
		}
		elseif( $vNivel == '75A' ){
			if( $boNivel75 == 'S' ){
				$whereNivel75 .= " or (cast(now() as date) - cast(pa.pagdatapagamento as date)) > rc.rcdqtd75exec and (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtd75exec + 90)";
			} else {
				$boNivel75 = 'S';
				$whereNivel75 .= "(cast(now() as date) - cast(pa.pagdatapagamento as date)) > rc.rcdqtd75exec and (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtd75exec + 90)";				
			}
		}
		elseif( $vNivel == '75P' ){
			if( $boNivel75 == 'S' ){
				$whereNivel75 .= " or (cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtd75exec + 120)";
			} else {
				$boNivel75 = 'S';
				$whereNivel75 .= "(cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtd75exec + 120)";				
			}
		}
		#execução 100%		
		elseif( $vNivel == '100D' ){
			if( $boNivel100 == 'S' ){
				$whereNivel100 .= " or (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= rc.rcdqtd100exec";
			} else {
				$boNivel100 = 'S';
				$whereNivel100 .= "(cast(now() as date) - cast(pa.pagdatapagamento as date)) <= rc.rcdqtd100exec";				
			}
		}
		elseif( $vNivel == '100A' ){
			if( $boNivel100 == 'S' ){
				$whereNivel100 .= " or (cast(now() as date) - cast(pa.pagdatapagamento as date)) > rc.rcdqtd100exec and (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtd100exec + 90)";
			} else {
				$boNivel100 = 'S';
				$whereNivel100 .= "(cast(now() as date) - cast(pa.pagdatapagamento as date)) > rc.rcdqtd100exec and (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtd100exec + 90)";				
			}
		}
		elseif( $vNivel == '100P' ){
			if( $boNivel100 == 'S' ){
				$whereNivel100 .= " or (cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtd100exec + 120)";
			} else {
				$boNivel100 = 'S';
				$whereNivel100 .= "(cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtd100exec + 120)";				
			}
		}
	}
}
*/

$campo75porcento = "(select supdtinclusao from (select 
						supdtinclusao,
						COALESCE((SELECT 
							sum(( icopercsobreobra * supvlrinfsupervisor ) / 100)
						FROM 
							obras.itenscomposicaoobra i
						INNER JOIN 
							obras.supervisaoitenscomposicao si ON i.icoid = si.icoid 
						WHERE 
							si.supvid = s.supvid 
						AND 
							i.obrid = oi.obrid),0) as percentual
					from 
						obras.supervisao s
					where 
						s.obrid = oi.obrid
					order by
						supdtinclusao asc ) as foo where percentual >= 75 limit 1)";

$campo100porcento = "(select supdtinclusao from (select 
						supdtinclusao,
						COALESCE((SELECT 
							sum(( icopercsobreobra * supvlrinfsupervisor ) / 100)
						FROM 
							obras.itenscomposicaoobra i
						INNER JOIN 
							obras.supervisaoitenscomposicao si ON i.icoid = si.icoid 
						WHERE 
							si.supvid = s.supvid 
						AND 
							i.obrid = oi.obrid),0) as percentual
					from 
						obras.supervisao s
					where 
						s.obrid = oi.obrid
					order by
						supdtinclusao asc ) as foo where percentual >= 100 limit 1)";

$sql = "SELECT * FROM (SELECT distinct

			--Carimbo Externo->Homologação		    
			case when (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= rc.rcdqtdhomologacao then '$imgAdequado'
		    when (cast(now() as date) - cast(pa.pagdatapagamento as date)) > rc.rcdqtdhomologacao and (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtdhomologacao + 90) then
		    	(case when (v.vldstatushomologacao = 'N' or v.vldstatushomologacao is null) then '$imgAtenção' else '$imgAdequado' end)
		    when (cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtdhomologacao + 91) then 
		  		(case when (v.vldstatushomologacao = 'N' or v.vldstatushomologacao is null) then '$imgPreocupante' else '$imgAdequado' end) end as externohmo,
		  			    
		    --Carimbo Externo->Execução 25%
    		case when v.vldstatushomologacao = 'S' then     		
			    (case when rc.rcdqtd30exec is null then '<center>-</center>' else
			    	(case when (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= rc.rcdqtd30exec then '$imgAdequado'
			    when (cast(now() as date) - cast(pa.pagdatapagamento as date)) > rc.rcdqtd30exec and (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtd30exec + 90) then 
			    	(case when oi.obrpercexec >= 25 then '$imgAdequado' else '$imgAtenção' end)
			    when (cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtd30exec + 91) then 
			    	(case when oi.obrpercexec >= 25 then '$imgAdequado' else '$imgPreocupante' end) end) end) 
		    else '<center>-</center>' end as externo25,
		    
		    --Carimbo Externo->Execução 50%
		    case when v.vldstatus25exec = 'S' then
			    (case when rc.rcdqtd50exec is null then '<center>-</center>' else
			    case when (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= rc.rcdqtd50exec then '$imgAdequado'
			    when (cast(now() as date) - cast(pa.pagdatapagamento as date)) > rc.rcdqtd50exec and (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtd50exec + 90) then 
			    	(case when oi.obrpercexec >= 50 then '$imgAdequado' else '$imgAtenção' end)
			    when (cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtd50exec + 91) then 
			    	(case when oi.obrpercexec >= 50 then '$imgAdequado' else '$imgPreocupante' end) end end)
		    else '<center>-</center>' end as externo50,
		    	
		    --Carimbo Externo->Execução 75%		    
			case when v.vldstatus50exec = 'S' AND $campo75porcento IS NOT NULL then		     
				    (case when rc.rcdqtd75exec is null then '<center>-</center>' else
				    case when (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= rc.rcdqtd75exec then '$imgAdequado'
				    when (cast(now() as date) - cast(pa.pagdatapagamento as date)) > rc.rcdqtd75exec and (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtd75exec + 90) then 
				    	(case when oi.obrpercexec >= 75 then '$imgAdequado' else '$imgAtenção' end)
				    when (cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtd75exec + 91) then 
				    	(case when oi.obrpercexec >= 75 then '$imgAdequado' else '$imgPreocupante' end) end end)
	    	else '<center>-</center>' end as externo75,
		    	
		    --Carimbo Interno->Execução 100%
		    case when $campo100porcento IS NOT NULL then
			    (case when rc.rcdqtd100exec is null then '<center>-</center>' else
			    case when (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= rc.rcdqtd100exec then '$imgAdequado'
			    when (cast(now() as date) - cast(pa.pagdatapagamento as date)) > rc.rcdqtd100exec and (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtd100exec + 90) then 
			    	(case when oi.obrpercexec >= 100 then '$imgAdequado' else '$imgAtenção' end)
			    when (cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtd100exec + 91) then 
			    	(case when oi.obrpercexec >= 100 then '$imgAdequado' else '$imgPreocupante' end) end end)
	    	ELSE '<center>-</center>' END as externo100,
		    	
		    --Carimbo Interno->Homologação
		    CASE 
		    	WHEN pa.pagdatapagamento IS NULL
		     		then '$imgNaoExiste'
		     	WHEN vlddtinclusaosthomo IS NOT NULL THEN 
				    CASE 
				    	WHEN v.vlddtinclusaosthomo::date <= (pa.pagdatapagamento::date + rc.rcdqtdhomologacao - 30)::date
				    		THEN '$imgAdequado'
				    	WHEN v.vlddtinclusaosthomo::date > (pa.pagdatapagamento::date + rc.rcdqtdhomologacao - 30)::date AND v.vlddtinclusaosthomo::date <= (pa.pagdatapagamento::date + rc.rcdqtdhomologacao + 30)::date
				    		THEN '$imgAtenção'
				    	ELSE
				    		'$imgPreocupante'
				    	END
			    ELSE
			    	CASE 
				    	WHEN now()::date <= (pa.pagdatapagamento::date + rc.rcdqtdhomologacao - 30)::date
				    		THEN '$imgAdequado'
				    	WHEN now()::date > (pa.pagdatapagamento::date + rc.rcdqtdhomologacao - 30)::date AND now()::date <= (pa.pagdatapagamento::date + rc.rcdqtdhomologacao + 30)::date
				    		THEN '$imgAtenção'
				    	ELSE
				    		'$imgPreocupante'
				    	END
			    	
		    END as internohmo,
		    /*
		    case when (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtdhomologacao-30) then '$imgAdequado'
		    when (cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtdhomologacao-30) and (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtdhomologacao+30) then
		    	(case when (v.vldstatushomologacao = 'N' or v.vldstatushomologacao is null) then '$imgAtenção' else '$imgAdequado' end)
		    when (cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtdhomologacao+30) 
		    	then '$imgPreocupante' else '$imgAdequado' end as internohmo,
		    */
		    --Carimbo Interno->Execução 25%
		     CASE 
		     	WHEN pa.pagdatapagamento IS NULL
		     		then '$imgNaoExiste'
		     	WHEN vlddtinclusaost25exec IS NOT NULL THEN 
				    CASE 
				    	WHEN v.vlddtinclusaost25exec::date <= (pa.pagdatapagamento::date + rc.rcdqtd30exec - 30)::date
				    		THEN '$imgAdequado'
				    	WHEN v.vlddtinclusaost25exec::date > (pa.pagdatapagamento::date + rc.rcdqtd30exec - 30)::date AND v.vlddtinclusaost25exec::date <= (pa.pagdatapagamento::date + rc.rcdqtd30exec + 30)::date
				    		THEN '$imgAtenção'
				    	ELSE
				    		'$imgPreocupante'
				    	END
			    ELSE
			    	CASE 
				    	WHEN now()::date <= (pa.pagdatapagamento::date + rc.rcdqtd30exec - 30)::date
				    		THEN '$imgAdequado'
				    	WHEN now()::date > (pa.pagdatapagamento::date + rc.rcdqtd30exec - 30)::date AND now()::date <= (pa.pagdatapagamento::date + rc.rcdqtd30exec + 30)::date
				    		THEN '$imgAtenção'
				    	ELSE
				    		'$imgPreocupante'
				    	END
			    	
		    END as interno25,
		    /*
		    case when rc.rcdqtd30exec is null then '<center>-</center>' else
		    case when (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtd30exec-30) then '$imgAdequado'
		    when (cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtd30exec-30) and (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtd30exec+30) then
		    	(case when oi.obrpercexec >= 25 then '$imgAdequado' else '$imgAtenção' end)
		    when (cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtd30exec+30) then 
		    	(case when oi.obrpercexec >= 25 then '$imgAdequado' else '$imgPreocupante' end) end end as interno25,
		    */
		    --Carimbo Interno->Execução 50%
		    CASE 
		    	WHEN pa.pagdatapagamento IS NULL
		     		then '$imgNaoExiste'
		     	WHEN vlddtinclusaost50exec IS NOT NULL THEN 
				    CASE 
				    	WHEN v.vlddtinclusaost50exec::date <= (pa.pagdatapagamento::date + rc.rcdqtd50exec - 30)::date
				    		THEN '$imgAdequado'
				    	WHEN v.vlddtinclusaost50exec::date > (pa.pagdatapagamento::date + rc.rcdqtd50exec - 30)::date AND v.vlddtinclusaost50exec::date <= (pa.pagdatapagamento::date + rc.rcdqtd50exec + 30)::date
				    		THEN '$imgAtenção'
				    	ELSE
				    		'$imgPreocupante'
				    	END
			    ELSE
			    	CASE 
				    	WHEN now()::date <= (pa.pagdatapagamento::date + rc.rcdqtd50exec - 30)::date
				    		THEN '$imgAdequado'
				    	WHEN now()::date > (pa.pagdatapagamento::date + rc.rcdqtd50exec - 30)::date AND now()::date <= (pa.pagdatapagamento::date + rc.rcdqtd50exec + 30)::date
				    		THEN '$imgAtenção'
				    	ELSE
				    		'$imgPreocupante'
				    	END
			    	
		    END as interno50,
		    /*
		    case when rc.rcdqtd50exec is null then '<center>-</center>' else
		    case when (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtd50exec-30) then '$imgAdequado'
		    when (cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtd50exec-30) and (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtd50exec+30) then 
		    	(case when oi.obrpercexec >= 50 then '$imgAdequado' else '$imgAtenção' end)
		    when (cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtd50exec+30) then 
		    	(case when oi.obrpercexec >= 50 then '$imgAdequado' else '$imgPreocupante' end) end end as interno50,
		    */
		    --Carimbo Interno->Execução 75%
		     CASE 
		    	WHEN pa.pagdatapagamento IS NULL
		     		then '$imgNaoExiste'
		     	WHEN $campo75porcento IS NOT NULL THEN 
				    CASE 
				    	WHEN $campo75porcento::date <= (pa.pagdatapagamento::date + rc.rcdqtd75exec - 30)::date
				    		THEN '$imgAdequado'
				    	WHEN $campo75porcento::date > (pa.pagdatapagamento::date + rc.rcdqtd75exec - 30)::date AND $campo75porcento::date <= (pa.pagdatapagamento::date + rc.rcdqtd75exec + 30)::date
				    		THEN '$imgAtenção'
				    	ELSE
				    		'$imgPreocupante'
				    	END
			    ELSE
			    	CASE 
				    	WHEN now()::date <= (pa.pagdatapagamento::date + rc.rcdqtd75exec - 30)::date
				    		THEN '$imgAdequado'
				    	WHEN now()::date > (pa.pagdatapagamento::date + rc.rcdqtd75exec - 30)::date AND now()::date <= (pa.pagdatapagamento::date + rc.rcdqtd75exec + 30)::date
				    		THEN '$imgAtenção'
				    	ELSE
				    		'$imgPreocupante'
				    	END
			    	
		    END as interno75,
		    /*
		    case when rc.rcdqtd75exec is null then '<center>-</center>' else
		    case when (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtd75exec-30) then '$imgAdequado'
		    when (cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtd75exec-30) and (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtd75exec+30) then 
		    	(case when oi.obrpercexec >= 75 then '$imgAdequado' else '$imgAtenção' end)
		    when (cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtd75exec+30) then 
		    	(case when oi.obrpercexec >= 75 then '$imgAdequado' else '$imgPreocupante' end) end end as interno75,
		    */
		    --Carimbo Interno->Execução 100%
		    CASE 
		    	WHEN pa.pagdatapagamento IS NULL
		     		then '$imgNaoExiste'
		     	WHEN $campo100porcento IS NOT NULL THEN 
				    CASE 
				    	WHEN $campo100porcento::date <= (pa.pagdatapagamento::date + rc.rcdqtd100exec - 30)::date
				    		THEN '$imgAdequado'
				    	WHEN $campo100porcento::date > (pa.pagdatapagamento::date + rc.rcdqtd100exec - 30)::date AND $campo100porcento::date <= (pa.pagdatapagamento::date + rc.rcdqtd100exec + 30)::date
				    		THEN '$imgAtenção'
				    	ELSE
				    		'$imgPreocupante'
				    	END
			    ELSE
			    	CASE 
				    	WHEN now()::date <= (pa.pagdatapagamento::date + rc.rcdqtd100exec - 30)::date
				    		THEN '$imgAdequado'
				    	WHEN now()::date > (pa.pagdatapagamento::date + rc.rcdqtd100exec - 30)::date AND now()::date <= (pa.pagdatapagamento::date + rc.rcdqtd100exec + 30)::date
				    		THEN '$imgAtenção'
				    	ELSE
				    		'$imgPreocupante'
				    	END
		    END as interno100,
		    /*,
		    case when rc.rcdqtd100exec is null then '<center>-</center>' else
		    case when (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtd100exec-30) then '$imgAdequado'
		    when (cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtd100exec-30) and (cast(now() as date) - cast(pa.pagdatapagamento as date)) <= (rc.rcdqtd100exec+30) then
		    	(case when oi.obrpercexec >= 100 then '$imgAdequado' else '$imgAtenção' end)
		    when (cast(now() as date) - cast(pa.pagdatapagamento as date)) > (rc.rcdqtd100exec+30) then 
		    	(case when oi.obrpercexec >= 100 then '$imgAdequado' else '$imgPreocupante' end) end end as interno100,
		    */
		    
		    -- Coluna de Status Geral
		    CASE WHEN pa.pagdatapagamento IS NOT NULL -- Se existe data de pagamento 
		    	THEN
		    		CASE 
		    			 -- Verifica se a data está em homologação, ou seja, menor que a data de 25% - 30 dias
		    			 WHEN now()::date <= (pa.pagdatapagamento::date + rc.rcdqtd30exec - 30)::date
		    				THEN
		    					CASE
		    						-- Se tiver homologado é verde
		    						WHEN vlddtinclusaosthomo IS NOT NULL
		    							THEN 'verde'
		    						-- Se não tiver a data de hologação, acompanha o status da homologação
		    						ELSE
		    							'internohmo'
		    					END
		    			 -- Verifica se a data está em 25%, ou seja, entre 25% - 30 dias e 50% - 30 dias
		    			 WHEN now()::date BETWEEN (pa.pagdatapagamento + rc.rcdqtd30exec - 30)::date AND (pa.pagdatapagamento + rc.rcdqtd50exec - 30)::date
		    			 	THEN
		    			 		CASE
		    						-- Se tiver a data de 25% é verde
		    						WHEN v.vlddtinclusaost25exec IS NOT NULL
		    							THEN 'verde'
		    						-- Se não tiver a data de 25%, acompanha o status de 25%
		    						ELSE
		    							'interno25'
		    					END
		    			 -- Verifica se a data está em 50%, ou seja, entre 50% - 30 dias e 75% - 30 dias
		    			 WHEN now()::date BETWEEN (pa.pagdatapagamento + rc.rcdqtd50exec - 30)::date AND (pa.pagdatapagamento + rc.rcdqtd75exec - 30)::date
		    			 	THEN
		    			 		CASE
		    						-- Se tiver a data de 50% é verde
		    						WHEN v.vlddtinclusaost50exec IS NOT NULL
		    							THEN 'verde'
		    						-- Se não tiver a data de 50%, acompanha o status de 50%
		    						ELSE
		    							'interno50'
		    					END
		    			  -- Verifica se a data está em 75%, ou seja, entre 75% - 30 dias e 100% - 30 dias
		    			 WHEN now()::date BETWEEN (pa.pagdatapagamento + rc.rcdqtd75exec - 30)::date AND (pa.pagdatapagamento + rc.rcdqtd100exec - 30)::date
		    			 	THEN
		    			 		CASE
		    						-- Se tiver a data de 75% é verde
		    						WHEN ($campo75porcento) IS NOT NULL
		    							THEN 'verde'
		    						-- Se não tiver a data de 75%, acompanha o status de 75%
		    						ELSE
		    							'interno75'
		    					END
		    			 -- Verifica se a data está em 100%, ou seja, maior que 100% - 30 dias
		    			 WHEN now()::date >= (pa.pagdatapagamento + rc.rcdqtd100exec - 30)::date
		    			 	THEN
		    			 		CASE
		    						-- Se tiver a data de 100% é verde
		    						WHEN ($campo100porcento) IS NOT NULL
		    							THEN 'verde'
		    						-- Se não tiver a data de 100%, acompanha o status de 100%
		    						ELSE
		    							'interno100'
		    					END
		    		END	
		    	ELSE -- Se não existe data de pagamento, usa a data de hoje
		    		CASE 
		    			 -- Verifica se a data está em homologação, ou seja, menor que a data de 25% - 30 dias
		    			 WHEN now()::date <= (now()::date + rc.rcdqtd30exec - 30)::date
		    				THEN
		    					CASE
		    						-- Se tiver homologado é verde
		    						WHEN vlddtinclusaosthomo IS NOT NULL
		    							THEN 'verde'
		    						-- Se não tiver a data de hologação, acompanha o status da homologação
		    						ELSE
		    							'internohmo'
		    					END
		    			 -- Verifica se a data está em 25%, ou seja, entre 25% - 30 dias e 50% - 30 dias
		    			 WHEN now()::date BETWEEN (now()::date + rc.rcdqtd30exec - 30)::date AND (now()::date + rc.rcdqtd50exec - 30)::date
		    			 	THEN
		    			 		CASE
		    						-- Se tiver a data de 25% é verde
		    						WHEN v.vlddtinclusaost25exec IS NOT NULL
		    							THEN 'verde'
		    						-- Se não tiver a data de 25%, acompanha o status de 25%
		    						ELSE
		    							'interno25'
		    					END
		    			 -- Verifica se a data está em 50%, ou seja, entre 50% - 30 dias e 75% - 30 dias
		    			 WHEN now()::date BETWEEN (now()::date + rc.rcdqtd50exec - 30)::date AND (now()::date + rc.rcdqtd75exec - 30)::date
		    			 	THEN
		    			 		CASE
		    						-- Se tiver a data de 50% é verde
		    						WHEN v.vlddtinclusaost50exec IS NOT NULL
		    							THEN 'verde'
		    						-- Se não tiver a data de 50%, acompanha o status de 50%
		    						ELSE
		    							'interno50'
		    					END
		    			  -- Verifica se a data está em 75%, ou seja, entre 75% - 30 dias e 100% - 30 dias
		    			 WHEN now()::date BETWEEN (now()::date + rc.rcdqtd75exec - 30)::date AND (now()::date + rc.rcdqtd100exec - 30)::date
		    			 	THEN
		    			 		CASE
		    						-- Se tiver a data de 75% é verde
		    						WHEN ($campo75porcento) IS NOT NULL
		    							THEN 'verde'
		    						-- Se não tiver a data de 75%, acompanha o status de 75%
		    						ELSE
		    							'interno75'
		    					END
		    			 -- Verifica se a data está em 100%, ou seja, maior que 100% - 30 dias
		    			 WHEN now()::date >= (now()::date + rc.rcdqtd100exec - 30)::date
		    			 	THEN
		    			 		CASE
		    						-- Se tiver a data de 100% é verde
		    						WHEN ($campo100porcento) IS NOT NULL
		    							THEN 'verde'
		    						-- Se não tiver a data de 100%, acompanha o status de 100%
		    						ELSE
		    							'interno100'
		    					END
		    		END
		    END as status_geral_interno,
		    
		    -- Coluna de Status Geral Externo
		    CASE WHEN pa.pagdatapagamento IS NOT NULL -- Se existe data de pagamento 
		    	THEN
		    		CASE 
		    			 -- Verifica se a data está em homologação, ou seja, menor que a data de 25%
		    			 WHEN now()::date <= (pa.pagdatapagamento::date + rc.rcdqtd30exec)::date
		    				THEN
		    					CASE
		    						-- Se tiver homologado é verde
		    						WHEN vlddtinclusaosthomo IS NOT NULL
		    							THEN 'verde'
		    						-- Se não tiver a data de hologação, acompanha o status da homologação
		    						ELSE
		    							'externohmo'
		    					END
		    			 -- Verifica se a data está em 25%, ou seja, entre 25% e 50%
		    			 WHEN now()::date BETWEEN (pa.pagdatapagamento + rc.rcdqtd30exec)::date AND (pa.pagdatapagamento + rc.rcdqtd50exec)::date
		    			 	THEN
		    			 		CASE
		    						-- Se tiver a data de 25% é verde
		    						WHEN v.vlddtinclusaost25exec IS NOT NULL
		    							THEN 'verde'
		    						-- Se não tiver a data de 25%, acompanha o status de 25%
		    						ELSE
		    							'externo25'
		    					END
		    			 -- Verifica se a data está em 50%, ou seja, entre 50% e 75%
		    			 WHEN now()::date BETWEEN (pa.pagdatapagamento + rc.rcdqtd50exec)::date AND (pa.pagdatapagamento + rc.rcdqtd75exec)::date
		    			 	THEN
		    			 		CASE
		    						-- Se tiver a data de 50% é verde
		    						WHEN v.vlddtinclusaost50exec IS NOT NULL
		    							THEN 'verde'
		    						-- Se não tiver a data de 50%, acompanha o status de 50%
		    						ELSE
		    							'externo50'
		    					END
		    			  -- Verifica se a data está em 75%, ou seja, entre 75% e 100%
		    			 WHEN now()::date BETWEEN (pa.pagdatapagamento + rc.rcdqtd75exec)::date AND (pa.pagdatapagamento + rc.rcdqtd100exec)::date
		    			 	THEN
		    			 		CASE
		    						-- Se tiver a data de 75% é verde
		    						WHEN ($campo75porcento) IS NOT NULL
		    							THEN 'verde'
		    						-- Se não tiver a data de 75%, acompanha o status de 75%
		    						ELSE
		    							'externo75'
		    					END
		    			 -- Verifica se a data está em 100%, ou seja, maior que 100%
		    			 WHEN now()::date >= (pa.pagdatapagamento + rc.rcdqtd100exec)::date
		    			 	THEN
		    			 		CASE
		    						-- Se tiver a data de 100% é verde
		    						WHEN ($campo100porcento) IS NOT NULL
		    							THEN 'verde'
		    						-- Se não tiver a data de 100%, acompanha o status de 100%
		    						ELSE
		    							'externo100'
		    					END
		    		END	
		    	ELSE -- Se não existe data de pagamento, usa a data de hoje
		    		CASE 
		    			 -- Verifica se a data está em homologação, ou seja, menor que a data de 25%
		    			 WHEN now()::date <= (now()::date + rc.rcdqtd30exec)::date
		    				THEN
		    					CASE
		    						-- Se tiver homologado é verde
		    						WHEN vlddtinclusaosthomo IS NOT NULL
		    							THEN 'verde'
		    						-- Se não tiver a data de hologação, acompanha o status da homologação
		    						ELSE
		    							'externohmo'
		    					END
		    			 -- Verifica se a data está em 25%, ou seja, entre 25% e 50%
		    			 WHEN now()::date BETWEEN (now()::date + rc.rcdqtd30exec)::date AND (now()::date + rc.rcdqtd50exec)::date
		    			 	THEN
		    			 		CASE
		    						-- Se tiver a data de 25% é verde
		    						WHEN v.vlddtinclusaost25exec IS NOT NULL
		    							THEN 'verde'
		    						-- Se não tiver a data de 25%, acompanha o status de 25%
		    						ELSE
		    							'externo25'
		    					END
		    			 -- Verifica se a data está em 50%, ou seja, entre 50% e 75%
		    			 WHEN now()::date BETWEEN (now()::date + rc.rcdqtd50exec)::date AND (now()::date + rc.rcdqtd75exec)::date
		    			 	THEN
		    			 		CASE
		    						-- Se tiver a data de 50% é verde
		    						WHEN v.vlddtinclusaost50exec IS NOT NULL
		    							THEN 'verde'
		    						-- Se não tiver a data de 50%, acompanha o status de 50%
		    						ELSE
		    							'externo50'
		    					END
		    			  -- Verifica se a data está em 75%, ou seja, entre 75% e 100%
		    			 WHEN now()::date BETWEEN (now()::date + rc.rcdqtd75exec)::date AND (now()::date + rc.rcdqtd100exec)::date
		    			 	THEN
		    			 		CASE
		    						-- Se tiver a data de 75% é verde
		    						WHEN ($campo75porcento) IS NOT NULL
		    							THEN 'verde'
		    						-- Se não tiver a data de 75%, acompanha o status de 75%
		    						ELSE
		    							'externo75'
		    					END
		    			 -- Verifica se a data está em 100%, ou seja, maior que 100%
		    			 WHEN now()::date >= (now()::date + rc.rcdqtd100exec)::date
		    			 	THEN
		    			 		CASE
		    						-- Se tiver a data de 100% é verde
		    						WHEN ($campo100porcento) IS NOT NULL
		    							THEN 'verde'
		    						-- Se não tiver a data de 100%, acompanha o status de 100%
		    						ELSE
		    							'externo100'
		    					END
		    		END
		    END as status_geral_externo,
		    
			oi.obrid,
		    oi.preid,
		    ent.entnome,
		    oi.obrdesc,
		    sto.stodesc as situacao,
		    v.vldstatushomologacao,
		    v.vldstatus25exec,
		    v.vldstatus50exec,
		    too.toodescricao,
		    pf.prfdesc,
		    too.toodescricao,
		    COALESCE(oi.obrpercexec, 0.00) as obrpercexec,
		    (cast(now() as date) - cast(pa.pagdatapagamento as date)) as numdias,
		    sto.stoid
		FROM
			obras.obrainfraestrutura oi
		    left join entidade.entidade ent on ent.entid = oi.entidunidade
		    left join entidade.endereco ed on ed.entid = ent.entid
		    left join obras.validacao v on v.obrid = oi.obrid
		    left join obras.tipoorigemobra too on too.tooid = oi.tooid
		    left join obras.programafonte pf on pf.prfid = oi.prfid
		    left join obras.tipologiaobra tl on tl.tpoid = oi.tpoid
		    left join obras.situacaoobra sto on sto.stoid = oi.stoid
		    --left join par.pagamentoobra po on po.preid = oi.preid
		    left join (select min(pagid) as pagid, preid from par.pagamentoobra group by preid) po on po.preid = oi.preid
		    left join par.pagamento pa on pa.pagid = po.pagid and pa.pagstatus = 'A' --and pa.pagparcela = 1
            left join obras.regracarimbo rc on rc.tpoid = tl.tpoid
		WHERE
			oi.obsstatus = 'A'
		    and oi.preid is not null
		    and oi.orgid = 3
		    --and pa.pagdatapagamento = (SELECT pagdatapagamento FROM 
		    --							par.pagamento where pagid = pa.pagid 
		    --                            order by pagdatapagamento asc limit 1)
		                          --and oi.obrid = 19129
		   ".( !empty($where) ? ' AND ' . implode(' AND ', $where) : '' )."
		   ".( !empty($whereNivel) ? ' AND (' . $whereNivel .')' : '' )."
		   ".( !empty($whereNivel25) ? ' AND (' . $whereNivel25 .')' : '' )."
		   ".( !empty($whereNivel50) ? ' AND (' . $whereNivel50 .')' : '' )."
		   ".( !empty($whereNivel75) ? ' AND (' . $whereNivel75 .')' : '' )."
		   ".( !empty($whereNivel100) ? ' AND (' . $whereNivel100 .')' : '' )."
		order by oi.obrid ) TBL $whereExterno";
//dbg( simec_htmlentities($sql),1 );
$arrDados = $db->carregar( $sql );
$arrDados = $arrDados ? $arrDados : array();

$complementoTD = 'class="title" bgcolor="" align="center" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#c0c0c0\';" 
				style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);"';
?>
<html>
	<head>
		<title> SIMEC - Sistema Integrado de Monitoramento do Ministério da Educação </title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	</head>
	<body>
		<center>
			<!--  Cabeçalho Brasão -->
			<?php echo monta_cabecalho_relatorio( '95' ); ?>
		</center>
		<table class="tabela" width="95%" cellspacing="0" cellpadding="2" border="0" align="center">
			<tr>
				<td><b>Nível de Execução <?=$tituloCarimbo; ?>:</b><br>
					<img src="/imagens/icones/carimbos/adequado.png" style="vertical-align:middle;"  border=0 title="Adequado"> <a href="#" onclick="abreRegra();"><b>Adequado:</b> <?=$descAdequado; ?></a> &nbsp;&nbsp;&nbsp;<br> 
					<img src="/imagens/icones/carimbos/atencao.png" style="vertical-align:middle;" border=0 title="Atenção"> <a href="#" onclick="abreRegra();"><b>Atenção:</b> <?=$descAtencao; ?></a> &nbsp;&nbsp;&nbsp;<br> 
					<img src="/imagens/icones/carimbos/preocupante.png" style="vertical-align:middle;" border=0 title="Preocupante"> <a href="#" onclick="abreRegra();"><b>Preocupante:</b> <?=$descPreocupante; ?></a></td>
			</tr>
		</table>
		<? monta_titulo( 'Relatório - Monitoramento de Obras', 'Relatório Nível de Execução' ); ?>
		<? 
		
		
		
		if( $arrDados ){
			$n=0;
			foreach ($arrDados as $key => $v) {
				if( $carimbo == 'I' ){
					
					if($v['status_geral_interno']){
						switch($v['status_geral_interno']){
							case "verde":
								$status_atual = str_replace(array("\"","\'"),array("'","'"),$imgAdequado);
								break;
							default:
								$status_atual = $v[$v['status_geral_interno']];
						}
					}else{
						$status_atual = "-";	
					}
//					$status_atual = $v['status_geral_interno'];
					
					$homologado 	= $v['internohmo'];
					$executado25 	= $v['interno25'];
					$executado50 	= $v['interno50'];
					$executado75 	= $v['interno75'];
					$executado100 	= $v['interno100'];
				} else {
					
					if($v['status_geral_externo']){
						switch($v['status_geral_externo']){
							case "verde":
								$status_atual = str_replace(array("\"","\'"),array("'","'"),$imgAdequado);
								break;
							default:
								$status_atual = $v[$v['status_geral_externo']];
						}
					}else{
						$status_atual = "-";	
					}
					
					$homologado 	= $v['externohmo'];
					$executado25 	= $v['externo25'];
					$executado50 	= $v['externo50'];
					$executado75 	= $v['externo75'];
					$executado100 	= $v['externo100'];
				}
				
				if($_REQUEST['tipoRelatorio'] == "excel"){
					$imgCinza = '';
				}else{
					$imgCinza = '<center>-</center>';
				}
				
				if(!$homologado) $homologado = $imgCinza;
				if(!$executado25) $executado25 = $imgCinza;
				if(!$executado50) $executado50 = $imgCinza;
				if(!$executado75) $executado75 = $imgCinza;
				if(!$executado100) $executado100 = $imgCinza;			
				
				$key % 2 ? $cor = "#dedfde" : $cor = "";
				
				if(empty($status_atual)){
					$imgAdequado2 = str_replace('\"','"',$imgAdequado);
					$status_atual = $imgAdequado2;
					$homologado   = $imgAdequado2;
					$executado25  = $imgAdequado2;
					$executado50  = $imgAdequado2;
					$executado75  = $imgAdequado2;
					$executado100 = $imgAdequado2;	
				}
				
				$arrDadosFinal[$n]['statusatualobra'] = $status_atual;
				$arrDadosFinal[$n]['homologado'] = $homologado;
				$arrDadosFinal[$n]['executado25'] = $executado25;
				$arrDadosFinal[$n]['executado50'] = $executado50;
				$arrDadosFinal[$n]['executado75'] = $executado75;
				$arrDadosFinal[$n]['executado100'] = $executado100;
				$arrDadosFinal[$n]['obrid'] = $v['obrid'];
				$arrDadosFinal[$n]['preid'] = $v['preid'];
				$arrDadosFinal[$n]['entnome'] = $v['entnome'];
				$arrDadosFinal[$n]['obrdesc'] = $v['obrdesc'];
				$arrDadosFinal[$n]['situacao'] = $v['situacao'];
				$arrDadosFinal[$n]['prfdesc'] = $v['prfdesc'];
				$arrDadosFinal[$n]['toodescricao'] = $v['toodescricao'];
				$arrDadosFinal[$n]['obrpercexec'] = $v['obrpercexec'];
				$arrDadosFinal[$n]['numdias'] = $v['numdias'];
				$arrDadosFinal[$n]['vldstatushomologacao'] = $v['vldstatushomologacao'];
				$arrDadosFinal[$n]['vldstatus25exec'] = $v['vldstatus25exec'];
				$arrDadosFinal[$n]['vldstatus50exec'] = $v['vldstatus50exec'];
				
				if(count($arStatusAtual)){
					$comp1 = simec_htmlentities(str_replace('\'','"',$status_atual));
					$comp2 = array();
					foreach($arStatusAtual as $filtrosSt){
						$comp2[] = simec_htmlentities(str_replace('\"','"',$filtrosSt));
					}					
					if(!in_array($comp1, $comp2)){
						if(isset($arrDadosFinal[$n]))
							unset($arrDadosFinal[$n]);
					}else{
						$n++;	
					}
				}else{
					$n++;
				}
			}
	
			if( $carimbo ){
				if( $carimbo == 'I' ){							
				
					$arrCabecalho = array( 
											0 => array(
														"label" => "Nível de Execução Interno", 
														"colunas" => array("Status Atual","Homologação","25% de Execução","50% de Execução","75% de Execução","100% de Execução")
											           ),
											1 => "Id Obra",
											2 => "Id Pré-Obra",
											3 => "Unidade Implantadora",
											4 => "Nome da Obra",
											5 => "Situação da Obra",
											6 => "Programa",
											7 => "Fonte",
											8 => "(%) Execução",
											9 => "Nº de Dias",
											10 => array(
														"label" => "Validações", 
														"colunas" => array("Homologação","Execução 25%","Execução 50%")
											           )
										 );
				}elseif($carimbo == 'E'){
					
					$arrCabecalho = array( 
											0 => array(
														"label" => "Nível de Execução Externo", 
														"colunas" => array("Status Atual","Homologação","25% de Execução","50% de Execução","75% de Execução","100% de Execução")
											           ),
											1 => "Id Obra",
											2 => "Id Pré-Obra",
											3 => "Unidade Implantadora",
											4 => "Nome da Obra",
											5 => "Situação da Obra",
											6 => "Programa",
											7 => "Fonte",
											8 => "(%) Execução",
											9 => "Nº de Dias",
											10 => array(
														"label" => "Validações", 
														"colunas" => array("Homologação","Execução 25%","Execução 50%")
											           )
										 );
				}
			}
			
		
			if($_REQUEST['tipoRelatorio'] == "excel"){
				ob_clean();
				
				header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
				header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
				header ( "Pragma: no-cache" );
				header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
				header ( "Content-Disposition: attachment; filename=planilha_".date("Ymdhis").".xls");
				header ( "Content-Description: MID Gera excel" );
				
				$cabecalho[] = "Nível de Execução - Status Atual";
				$cabecalho[] = "Nível de Execução - Homologação";
				$cabecalho[] = "Nível de Execução - 25% de Execução";
				$cabecalho[] = "Nível de Execução - 50% de Execução";
				$cabecalho[] = "Nível de Execução - 75% de Execução";
				$cabecalho[] = "Nível de Execução - 100% de Execução";
				$cabecalho[] = "Id Obra";
				$cabecalho[] = "Id Pré-Obra";
				$cabecalho[] = "Unidade Implantadora";
				$cabecalho[] = "Nome da Obra";
				$cabecalho[] = "Situação da Obra";
				$cabecalho[] = "Programa";
				$cabecalho[] = "Fonte";
				$cabecalho[] = "(%) Execução";
				$cabecalho[] = "Nº de Dias";
				$cabecalho[] = "Validação da Homologação";
				$cabecalho[] = "Validação da Execução 25%";
				$cabecalho[] = "Validação da Execução 50%";
				$db->monta_lista_tabulado($arrDadosFinal,$cabecalho,1000,100,"N");
				exit;
			}
			
			$db->monta_lista_array($arrDadosFinal,$arrCabecalho,100,10,"N","center",false,$arrayDeTiposParaOrdenacao);
		} else {
			print '<table width="80%" align="left" border="0" cellspacing="0" cellpadding="2" class="listagem">';
			print '<tr><td colspan="17" align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
			print '</table>';
		}
		?>		
	</body>
	<script type="text/javascript">
	function abreRegra(){
		window.open("obras.php?modulo=relatorio/popupRegraCarimbo&acao=A","imagem","width=850,height=500,status=no,menubar=no,toolbar=no,scrollbars=1,resizable=no");
	}
	</script>
</html>
<?
/*else{
	print '<table width="80%" align="left" border="0" cellspacing="0" cellpadding="2" class="listagem">';
	print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
	print '</table>';
}*/
//$db->monta_lista( $sql, $cabecalho, 100000000, 10, 'N', 'center', '', '', '','');
?>