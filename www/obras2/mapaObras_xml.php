<?php
	// abre conexão com o servidor de banco de dados
	include_once "config.inc";
	//include "verificasistema.inc";
	include_once APPRAIZ . "includes/classes_simec.inc";
	$db = new cls_banco();
	$conteudo = "";
	$filtro="";
	// Busca
	if ($_REQUEST["texto_busca"]){
		$busca="AND ( UPPER(o.obrdesc) LIKE UPPER('%".$_REQUEST["texto_busca"]."%') OR UPPER(o.obrnome) LIKE UPPER('%".$_REQUEST["texto_busca"]."%') )";
	}
	
	//pontocentral
	if($_REQUEST["pontocentral"]){
		$ll = str_replace(array("(",")"),"",$_REQUEST["pontocentral"]);
		$ll = explode(",",trim($ll));
		$lat = $ll[0];
		$lng = $ll[1];
		$sqlPorForaA = " select * from ( ";
		$sqlPorForaB = " ) as tbl where distanciaPontosGPS($lat,$lng, latitude, longitude) < 10000 ";
	}else{
		unset($_SESSION['obras2']['obrid_mapa']);
	}
	
	// Filtros
	if ($_SESSION['obras2']['obrid_mapa']){
		$filtro.=" AND obr.obrid NOT IN (".implode(",",$_SESSION['obras2']['obrid_mapa']).") ";
	}
	if ($_REQUEST["orgid"]){
		$filtro.=" AND emp.orgid IN (".$_REQUEST["orgid"].") ";
	}
// 	if ($_REQUEST["stoid"]){
// 		$filtro.=" AND o.stoid IN (".$_REQUEST["stoid"].") ";
// 	}
// 	if ($_REQUEST["prfid"]){
// 		$filtro.=" AND obrid.prfid IN (".$_REQUEST["prfid"].") ";
// 	}
// 	if($_REQUEST["painel"] == 2){
// 		$filtro.=" AND o.obrpercexec <= 80  ";
// 	}
// 	if($_REQUEST["painel"] == 3){
// 		$filtro.=" AND o.obrpercexec > 80  ";
// 	}
// 	if ($_REQUEST["tooid"] == 'pac'){
// 		$filtro.=" AND o.tooid IN (1) ";
// 	}
// 	if ($_REQUEST["tooid"] == 'prepac'){
// 		$filtro.=" AND o.tooid IN (2,4) ";
// 	}
// 	if ($_REQUEST["tooid"] == 1){
// 		$filtro.=" AND o.tooid IN (1,2,4) ";
// 	}
	if ($_REQUEST["cloid"]){
		$filtro.=" AND obr.cloid IN (".$_REQUEST["cloid"].") ";
	}
	if ($_REQUEST["estuf"]){
		$estuf=str_replace("\'","'",$_REQUEST["estuf"])	;
		$filtro.=" AND end.estuf IN (".$estuf.") ";
	}
// 	if ($_REQUEST["flag_repositorio"]){
// 		$filtro.=" AND obr.obrid IN ( SELECT DISTINCT obrid FROM obras.repositorio where repstatus = 'A') ";
// 	}
	if ($_REQUEST["entid"]){
		$filtro.=" AND obr.entid IN (".implode("','",$_REQUEST["entid"]).") ";
	}
	
	// alterações para o obras (supervisão p/ empresas contratadas)
	if ( $_REQUEST["obrid"] ){
		$filtro.=" AND obr.obrid IN (".$_REQUEST["obrid"].") ";
	}
	
// 	if( $_REQUEST["requisicao"] == "supervisao"){
// 		$join = "INNER JOIN obras.repositorio ore ON ore.obrid = o.obrid";
// 		$filtro .= " AND repstatus = 'A' ";
// 	}
	
// 	if( $_REQUEST["requisicao"] == "grupo" ){
// 		$join = "INNER JOIN obras.repositorio ore ON ore.obrid = o.obrid
// 				 INNER JOIN obras.itemgrupo otg ON otg.repid = ore.repid AND gpdid = {$_REQUEST["gpdid"]}";
// 	}

	$sql = "$sqlPorForaA 
			SELECT
	            -- ID do registro
	            obr.obrid as idobra,

	            -- Descrição do registro
	            TRIM (obr.obrdsc) as obrdesc,

	            --############### LATITUDE ###################### --
				CASE WHEN (SPLIT_PART(ende.medlatitude, '.', 1) <>'' AND SPLIT_PART(ende.medlatitude, '.', 2) <>'' AND split_part(ende.medlatitude, '.', 3) <>'') THEN
	               CASE WHEN split_part(ende.medlatitude, '.', 4) <>'N' THEN
	                   (((split_part(ende.medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(ende.medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(ende.medlatitude, '.', 1)::int)))*(-1)
	                ELSE
	                   ((SPLIT_PART(ende.medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(ende.medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(ende.medlatitude, '.', 1)::int))
	               END
	            ELSE
	            -- Valores do IBGE convertidos em  decimal
	            CASE WHEN (length (mun.munmedlat)=8) THEN 
	                CASE WHEN length(REPLACE('0' || mun.munmedlat,'S','')) = 8 THEN
	                    ((SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),1,2)::double precision))*(-1)
	                ELSE
	                    (SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),1,2)::double precision)
	                END
	            ELSE
	                CASE WHEN length(REPLACE(mun.munmedlat,'S','')) = 8 THEN
	                   ((SUBSTR(REPLACE(mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),1,2)::double precision))*(-1)
	                ELSE
	                  0--((SUBSTR(REPLACE(mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'N',''),1,2)::double precision))
	                END
	            END  
	            END as latitude, 
	            --############### FIM LATITUDE ###################### --
            
	            --############### LONGITUDE ###################### --
	            CASE WHEN (SPLIT_PART(ende.medlongitude, '.', 1) <>'' AND SPLIT_PART(ende.medlongitude, '.', 2) <>'' AND split_part(ende.medlongitude, '.', 3) <>'') THEN
	               ((split_part(ende.medlongitude, '.', 3)::double precision / 3600) +(SPLIT_PART(ende.medlongitude, '.', 2)::double precision / 60) + (SPLIT_PART(ende.medlongitude, '.', 1)::int))*(-1)
	            ELSE
	                -- Valores do IBGE convertidos em  decimal
	               (SUBSTR(REPLACE(mun.munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(mun.munmedlog,'W',''),3,2)::double precision/60)) *(-1)
	            END as longitude, 
	            --############### FIM LONGITUDE ###################### --

	            --Origem do registro (caso venha do IBGE, somar 100 ao tipo
	            CASE WHEN ende.medlatitude <> '' AND ende.medlatitude is not null THEN
	                emp.orgid
	            ELSE 
	                emp.orgid+100
	            END as tipo

            FROM 
				obras2.obras obr	{$join}
			INNER JOIN obras2.empreendimento emp ON emp.empid = obr.empid
            INNER JOIN entidade.endereco 	ende ON ende.endid=obr.endid and ende.endstatus='A' 
			INNER JOIN territorios.municipio mun ON ende.muncod=mun.muncod
			WHERE 
            	obr.obrstatus='A' 
				$busca  
				$filtro 
				$sqlPorForaB 
            ORDER BY 
            	random() limit 1000";
	
// print_r($sql);print_r($_SESSION['obras2']['obrid_mapa']);die;

		$dados = $db->carregar($sql,null,3200);

		$conteudo .= "<markers>";
		if($dados){
			for($i=0;$i <= count($dados);$i++){
				 $_SESSION['obras2']['obrid_mapa'][] = $dados[$i]["idobra"];
				 $idobra = $dados[$i]["idobra"];
				 $orgid=$dados[$i]["tipo"];
				 $latitude = $dados[$i]["latitude"];
				 $longitude= $dados[$i]["longitude"];
				 $obrdesc=$dados[$i]["obrdesc"];
				 $obrdesc=str_replace(array('"',"&"),"",$obrdesc);
				 $conteudo .= "<marker ";
				 $conteudo .= "idobra='$idobra' ";
				 $conteudo .= "obrdesc=\"". trim($obrdesc) ."\" ";
				 $conteudo .= "orgid='$orgid' ";
				 $conteudo .= "lat='$latitude' ";
				 $conteudo .= "lng='$longitude' ";
				 $conteudo .= "/>";
			}
		}
		$conteudo .= "</markers>";
	
	ob_clean();
	header('content-type:text/xml; charset=ISO-8859-1');
	print $conteudo;
?>			