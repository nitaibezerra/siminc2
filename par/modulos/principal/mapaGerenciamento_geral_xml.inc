<?php

	header("content-type: text/xml");

	$conteudo = "";
	$filtro="";
	
	// Filtros
	if ( $_REQUEST["esdid"] ){
		$esdid = str_replace("}{",",",$_REQUEST["esdid"]);
		$esdid = str_replace("}","",$esdid);
		$esdid = str_replace("{","",$esdid);
		$esdid = str_replace("\'","'",$esdid);
		$filtro .= " AND ed.esdid IN (".$esdid.") ";
	}
	if ( $_REQUEST["ptoid"] ){
		$ptoid = str_replace("}{",",",$_REQUEST["ptoid"]);
		$ptoid = str_replace("}","",$ptoid);
		$ptoid = str_replace("{","",$ptoid);
		$ptoid = str_replace("\'","'",$ptoid);
		$filtro .= " AND tpo.ptoid IN (".$ptoid.") ";
	}
	if ($_REQUEST["estuf"]){
		$estuf = str_replace("}{",",",$_REQUEST["estuf"]);
		$estuf = str_replace("}","",$estuf);
		$estuf = str_replace("{","",$estuf);
		$estuf = str_replace("\'","'",$estuf);
		$filtro .= " AND mun.estuf IN (".$estuf.") ";
	}
	if ( $_REQUEST["muncod"] ){
		$muncod = str_replace("}{",",",$_REQUEST["muncod"])	;
		$muncod = str_replace("}","",$muncod);
		$muncod = str_replace("{","",$muncod);
		$muncod = str_replace("\'","'",$muncod);
		$filtro .= " AND mun.muncod IN (".$muncod.") ";
	}
	
	$sql = "SELECT
				po.preid as idobra,
				po.preid || ' - ' || po.predescricao as obrdesc,
				CASE WHEN (SPLIT_PART(po.prelatitude, '.', 1) <>'' AND SPLIT_PART(po.prelatitude, '.', 2) <>'' AND split_part(po.prelatitude, '.', 3) <>'') 
					THEN
			                CASE WHEN split_part(po.prelatitude, '.', 4) <>'N' 
						THEN
							(((split_part(po.prelatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(po.prelatitude, '.', 2)::double precision / 60) + (SPLIT_PART(po.prelatitude, '.', 1)::int)))*(-1)
						ELSE
							((SPLIT_PART(po.prelatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(po.prelatitude, '.', 2)::double precision / 60) + (SPLIT_PART(po.prelatitude, '.', 1)::int))
					END
					ELSE
					-- Valores do IBGE convertidos em  decimal
					CASE WHEN (length (mun.munmedlat)=8) 
						THEN 
						CASE WHEN length(REPLACE('0' || mun.munmedlat,'S','')) = 8 
							THEN
								((SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),1,2)::double precision))*(-1)
							ELSE
								(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),1,2)::double precision)
						END
						ELSE
						CASE WHEN length(REPLACE(mun.munmedlat,'S','')) = 8
							THEN
								((SUBSTR(REPLACE(mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),1,2)::double precision))*(-1)
							ELSE
								0--((SUBSTR(REPLACE(mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'N',''),1,2)::double precision))
						END
					END  
				END as latitude,
				CASE WHEN (SPLIT_PART(po.prelongitude, '.', 1) <>'' AND SPLIT_PART(po.prelongitude, '.', 2) <>'' AND split_part(po.prelongitude, '.', 3) <>'') 
					THEN
						((split_part(po.prelongitude, '.', 3)::double precision / 3600) +(SPLIT_PART(po.prelongitude, '.', 2)::double precision / 60) + (SPLIT_PART(po.prelongitude, '.', 1)::int))*(-1)
					ELSE
						-- Valores do IBGE convertidos em  decimal
						(SUBSTR(REPLACE(mun.munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(mun.munmedlog,'W',''),3,2)::double precision/60)) *(-1)
				END as longitude,
				po.ptoid
			FROM
				obras.preobra po
			INNER JOIN
				territorios.municipio   mun ON mun.muncod = po.muncod
			INNER JOIN
				obras.pretipoobra       tpo ON tpo.ptoid = po.ptoid
			INNER JOIN
				workflow.documento      doc ON doc.docid = po.docid
			INNER JOIN
				workflow.estadodocumento ed ON ed.esdid = doc.esdid 
			WHERE
				po.prestatus = 'A' {$filtro}
			ORDER BY 
				mun.munmedlog";
	
	$dados = $db->carregar($sql);
	
	$conteudo .= "<markers> ";
	for( $i=0; $i < count($dados); $i++ ){
		
		 $idobra    = $dados[$i]["idobra"];
		 $latitude  = $dados[$i]["latitude"];
		 $longitude = $dados[$i]["longitude"];
		 $obrdesc   = $dados[$i]["obrdesc"];
		 $obrdesc   = str_replace('"',"",$obrdesc)	;
		 $obrdesc   = str_replace('\'',"",$obrdesc)	;
		 $obrdesc   = str_replace('\\',"",$obrdesc)	;
		 $obrdesc   = str_replace('<',"",$obrdesc)	;
		 $obrdesc   = str_replace('>',"",$obrdesc)	;
		 $ptoid     = $dados[$i]["ptoid"];
		
		 $conteudo .= "<marker ";
		 $conteudo .= "idobra='$idobra' ";
		 $conteudo .= "obrdesc=\"". $obrdesc ."\" ";
		 $conteudo .= "lat='$latitude' ";
		 $conteudo .= "lng='$longitude' ";
		 $conteudo .= "ptoid='$ptoid' ";
		 $conteudo .= "/> ";
		 
	}
	$conteudo .= "</markers> ";
	print $conteudo;
?>			