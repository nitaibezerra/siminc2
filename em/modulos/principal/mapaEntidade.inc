<?php 
if($_REQUEST['requisicao'] == "gravarPonto") {
	
	$arrLat = $_REQUEST['lat'];
	$arrLng = $_REQUEST['lng'];
	$zoom 	= $_REQUEST['zoom'];
	
	// tratando latitude
	if($arrLat > 0) $poloLat = "N";
	else {
		$arrLat = ($arrLat*-1);
		$poloLat = "S";
	}
	$arrLatGraus = floor($arrLat);
	$minutos     = (($arrLat - floor($arrLat)) * 60);
	$arrLatMin   = floor($minutos);
	$segundos    = (($minutos - $arrLatMin) * 60);
	$arrLatSeg   = round($segundos);

	
	// tratando longitude
	if($arrLng > 0) $poloLng = "E";
	else {
		$arrLng = ($arrLng*-1);
		$poloLng = "W";
	}
	$arrLngGraus = floor($arrLng);
	$minutos     = (($arrLng - floor($arrLng)) * 60);
	$arrLngMin   = floor($minutos);
	$segundos    = (($minutos - $arrLngMin) * 60);
	$arrLngSeg   = round($segundos);
	
	if($_REQUEST['mundsc']) {
		$_arrTer = $db->pegaLinha("SELECT muncod, estuf FROM territorios.municipio WHERE mundescricao ilike TRIM('".iconv( "UTF-8", "ISO-8859-1", $_REQUEST['mundsc'])."')");	
	}

	
	$sql = "INSERT INTO entidade.endereco(
            tpeid, endstatus, estuf, muncod, endbai, endlog, endcep, medlatitude, medlongitude, endzoom)
		    VALUES ('1', 
				    'A', 
				    ".(($_arrTer['estuf'])?"'".$_arrTer['estuf']."'":"NULL").", 
				    ".(($_arrTer['muncod'])?"'".$_arrTer['muncod']."'":"NULL").", 
				    ".(($_REQUEST['endbai'])?"'".$_REQUEST['endbai']."'":"NULL").",
				    ".(($_REQUEST['endlog'])?"'".$_REQUEST['endlog']."'":"NULL").",
				    ".(($_REQUEST['endcep'])?"'".str_replace("-", "", $_REQUEST['endcep'])."'":"NULL").",
				    '". $arrLatGraus.".".$arrLatMin.".".$arrLatSeg.".".$poloLat."', 
				    '".$arrLngGraus.".".$arrLngMin.".".$arrLngSeg.".".$poloLng."', 
				    '".$zoom."') RETURNING endid;";
	
	$endid = $db->pegaUm($sql);
	
	
	$sql = "INSERT INTO pdeescola.mepontodiversos(
            divnome, divdescricao, endid, divtipo)
    		VALUES ('".$_REQUEST['divnome']."', '".$_REQUEST['divdescricao']."', '".$endid."', '".$_REQUEST['divtipo']."');";
	
	$db->executar($sql);
	
	$db->commit();
	
	echo "Ponto gravado com sucesso";
	
	exit;
}


if($_GET['requisicao'] == "pontosEscola") {
	header('content-type:text/xml; charset=ISO-8859-1');
	
	if( $_GET['muncod'] ){
		$where = "AND mun.muncod = '{$_GET['muncod']}'";
	}
	
	$sql = "select distinct
				entnome,
				CASE 
				WHEN (medlatitude is not null AND medlatitude <> '...S' AND medlatitude <> '00000000') THEN 
					( 	 --############### LATITUDE ###################### --
								CASE WHEN (SPLIT_PART(medlatitude, '.', 1) <>'' AND SPLIT_PART(medlatitude, '.', 2) <>'' AND split_part(medlatitude, '.', 3) <>'') THEN
					               CASE WHEN split_part(medlatitude, '.', 4) <>'N' THEN
					                   (((split_part(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int)))*(-1)
					                ELSE
					                   ((SPLIT_PART(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int))
					               END
					            ELSE
					            -- Valores do IBGE convertidos em  decimal
					            CASE WHEN (length (medlatitude)=8) THEN
					                CASE WHEN length(REPLACE('0' || medlatitude,'S','')) = 8 THEN
					                    ((SUBSTR(REPLACE('0' || medlatitude,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || medlatitude,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || medlatitude,'S',''),1,2)::double precision))*(-1)
					                ELSE
					                    (SUBSTR(REPLACE('0' || medlatitude,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || medlatitude,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || medlatitude,'N',''),1,2)::double precision)
					                END
					            ELSE
					                CASE WHEN length(REPLACE(medlatitude,'S','')) = 8 THEN
					                   ((SUBSTR(REPLACE(medlatitude,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(medlatitude,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(medlatitude,'S',''),1,2)::double precision))*(-1)
					                ELSE
					                  0--((SUBSTR(REPLACE(medlatitude,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(medlatitude,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(medlatitude,'N',''),1,2)::double precision))
					                END
					            END 
					            END
					       --############### LATITUDE ###################### --
					  ) 
				ELSE (
						 --############### LATITUDE ###################### --
								CASE WHEN (SPLIT_PART(munmedlat, '.', 1) <>'' AND SPLIT_PART(munmedlat, '.', 2) <>'' AND split_part(munmedlat, '.', 3) <>'') THEN
					               CASE WHEN split_part(munmedlat, '.', 4) <>'N' THEN
					                   (((split_part(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int)))*(-1)
					                ELSE
					                   ((SPLIT_PART(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int))
					               END
					            ELSE
					            -- Valores do IBGE convertidos em  decimal
					            CASE WHEN (length (munmedlat)=8) THEN
					                CASE WHEN length(REPLACE('0' || munmedlat,'S','')) = 8 THEN
					                    ((SUBSTR(REPLACE('0' || munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),1,2)::double precision))*(-1)
					                ELSE
					                    (SUBSTR(REPLACE('0' || munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),1,2)::double precision)
					                END
					            ELSE
					                CASE WHEN length(REPLACE(munmedlat,'S','')) = 8 THEN
					                   ((SUBSTR(REPLACE(munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'S',''),1,2)::double precision))*(-1)
					                ELSE
					                  0--((SUBSTR(REPLACE(munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'N',''),1,2)::double precision))
					                END
					            END
					            END
					         --############### LATITUDE ###################### --
				     )
				END as latitude,
				CASE 
					WHEN (medlongitude is not null AND medlongitude <> '...W' AND medlongitude <> '..' AND medlongitude <> '00000000') THEN 
						( 	 --############### LONGITUDE ###################### --
						            CASE WHEN (SPLIT_PART(medlongitude, '.', 1) <>'' AND SPLIT_PART(medlongitude, '.', 2) <>'' AND split_part(medlongitude, '.', 3) <>'') THEN
						               ((split_part(medlongitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlongitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlongitude, '.', 1)::int))*(-1)
						            ELSE
						                -- Valores do IBGE convertidos em  decimal
						               (SUBSTR(REPLACE(medlongitude,'W',''),1,2)::double precision + (SUBSTR(REPLACE(medlongitude,'W',''),3,2)::double precision/60)) *(-1)
						            END
						         --############### FIM LONGITUDE ###################### --
						  ) 
					ELSE (
							 --############### LONGITUDE ###################### --
						            CASE WHEN (SPLIT_PART(munmedlog, '.', 1) <>'' AND SPLIT_PART(munmedlog, '.', 2) <>'' AND split_part(munmedlog, '.', 3) <>'') THEN
						               ((split_part(munmedlog, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlog, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlog, '.', 1)::int))*(-1)
						            ELSE
						                -- Valores do IBGE convertidos em  decimal
						               (SUBSTR(REPLACE(munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(munmedlog,'W',''),3,2)::double precision/60)) *(-1)
						            END
						         --############### FIM LONGITUDE ###################### --
					     )
				END as longitude,
				mun.mundescricao,
				ed.estuf,
				ed.endcep,
				ed.endlog,
				ed.endnum,
				ed.endbai
			FROM
				entidade.entidade e
			INNER JOIN
				pdeescola.memaiseducacao m ON m.entid = e.entid
			LEFT JOIN 
				entidade.endereco ed ON e.entid = ed.entid
			LEFT JOIN 
				territorios.municipio mun ON ed.muncod = mun.muncod
			WHERE
				ed.estuf = '{$_GET['estuf']}'
				{$where}
			AND
				e.entstatus = 'A'";
	
		$dados = $db->carregar($sql);
		
		if($dados[0]):
				
				$conteudo .= "<markers> "; // inicia o XML
				
				foreach($dados as $d):
												
					$conteudo .= "<marker "; //inicia um ponto no mapa
					$conteudo .= "entnome=\"". simec_htmlspecialchars($d['entnome']) ."\" "; // adiciona o nome da instituição;
					$conteudo .= "mundsc=\"". simec_htmlspecialchars($d['mundescricao']) ."\" "; // adiciona a descrição do município;
					$conteudo .= "estuf=\"". $d['estuf'] ."\" "; // adiciona UF;
					$conteudo .= "endereco=\"". $d['endlog'] . ( $d['endnum'] ? "," . $d['endnum'] : ""). ( $d['endbai'] ? "," . $d['endbai'] : "") . ($d['endcep'] ? ", CEP " . $d['endcep'] : ""). "\" "; // adiciona UF;
					$conteudo .= "lat=\"".$d['latitude']."\" "; // adiciona a latitude;
					$conteudo .= "lng=\"".$d['longitude']."\" "; //adiciona a longitude;
					$conteudo .= "/> ";
				
				endforeach;
				
				$conteudo .= "</markers> ";
				print $conteudo;
				
			endif;
		exit;
	
}




if($_GET['requisicao'] == "pontosCulturais"){
	header('content-type:text/xml; charset=ISO-8859-1');
	
	if( $_GET['muncod'] ){
		$where = "AND mun.muncod = '{$_GET['muncod']}'";
	}
	
	$sql = "SELECT
					mpcnome,
					CASE WHEN (medlatitude is not null AND medlatitude <> '...S' AND medlatitude <> '00000000') THEN 
						(CASE WHEN split_part(medlatitude, '.', 4) <>'N' THEN
							(((split_part(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int)))*(-1)
						 ELSE
						 	((SPLIT_PART(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int))
						 END)
		            END as latitude,
		            
					CASE WHEN (medlongitude is not null AND medlongitude <> '...W' AND medlongitude <> '..' ) THEN 
							(CASE WHEN (SPLIT_PART(medlongitude, '.', 1) <>'' AND SPLIT_PART(medlongitude, '.', 2) <>'' AND split_part(medlongitude, '.', 3) <>'') THEN
							               ((split_part(medlongitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlongitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlongitude, '.', 1)::int))*(-1)
							 END) 
					END as longitude,
					mun.mundescricao,
					mpcdescricao,
					ed.estuf,
					ed.endcep,
					ed.endlog,
					ed.endnum,
					ed.endbai
				FROM
					pdeescola.mepontocultura e
				LEFT JOIN 
					entidade.endereco ed ON e.endid = ed.endid
				LEFT JOIN 
					territorios.municipio mun ON ed.muncod = mun.muncod
				WHERE
					ed.estuf = '{$_GET['estuf']}'
					{$where}";
	
		$dados = $db->carregar($sql);
		
		if($dados[0]):
				
				$conteudo .= "<markers> "; // inicia o XML
				
				foreach($dados as $d):
					$conteudo .= "<marker "; //inicia um ponto no mapa
					$conteudo .= "entnome=\"". simec_htmlspecialchars($d['mpcnome']) ."\" "; // adiciona o nome da instituição;
					$conteudo .= "mundsc=\"". simec_htmlspecialchars($d['mundescricao']) ."\" "; // adiciona a descrição do município;
					$conteudo .= "estuf=\"". $d['estuf'] ."\" "; // adiciona UF;
					$conteudo .= "descricao=\"". simec_htmlspecialchars($d['mpcdescricao']) ."\" "; // adiciona UF;
					$conteudo .= "endereco=\"". $d['endlog'] . ( $d['endnum'] ? "," . $d['endnum'] : ""). ( $d['endbai'] ? "," . $d['endbai'] : "") . ($d['endcep'] ? ", CEP " . $d['endcep'] : ""). "\" "; // adiciona UF;
					$conteudo .= "telefone=\"". $telefone . "\" "; // adiciona UF;
					$conteudo .= "lat=\"".$d['latitude']."\" "; // adiciona a latitude;
					$conteudo .= "lng=\"".$d['longitude']."\" "; //adiciona a longitude;
					$conteudo .= "/> ";
				
				endforeach;
				
				$conteudo .= "</markers> ";
				print $conteudo;
				
			endif;
		exit;
	
}

if($_GET['requisicao'] == "pontosDiversos"){
	header('content-type:text/xml; charset=ISO-8859-1');
	
	if( $_GET['muncod'] ){
		$where = "AND mun.muncod = '{$_GET['muncod']}'";
	}
	
	$sql = "SELECT
					divnome,
					CASE WHEN (medlatitude is not null AND medlatitude <> '...S' AND medlatitude <> '00000000') THEN 
						(CASE WHEN split_part(medlatitude, '.', 4) <>'N' THEN
							(((split_part(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int)))*(-1)
						 ELSE
						 	((SPLIT_PART(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int))
						 END)
		            END as latitude,
		            
					CASE WHEN (medlongitude is not null AND medlongitude <> '...W' AND medlongitude <> '..' ) THEN 
							(CASE WHEN (SPLIT_PART(medlongitude, '.', 1) <>'' AND SPLIT_PART(medlongitude, '.', 2) <>'' AND split_part(medlongitude, '.', 3) <>'') THEN
							               ((split_part(medlongitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlongitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlongitude, '.', 1)::int))*(-1)
							 END) 
					END as longitude,
					mun.mundescricao,
					divdescricao,
					ed.estuf,
					ed.endcep,
					ed.endlog,
					ed.endnum,
					ed.endbai,
					divtipo
				FROM
					pdeescola.mepontodiversos e
				LEFT JOIN 
					entidade.endereco ed ON e.endid = ed.endid
				LEFT JOIN 
					territorios.municipio mun ON ed.muncod = mun.muncod
				WHERE
					ed.estuf = '{$_GET['estuf']}'
					{$where}";
	
		$dados = $db->carregar($sql);
		
		if($dados[0]):
				
				$conteudo .= "<markers> "; // inicia o XML
				
				foreach($dados as $d):
					$conteudo .= "<marker "; //inicia um ponto no mapa
					$conteudo .= "entnome=\"". simec_htmlspecialchars($d['divnome']) ."\" "; // adiciona o nome da instituição;
					$conteudo .= "mundsc=\"". simec_htmlspecialchars($d['mundescricao']) ."\" "; // adiciona a descrição do município;
					$conteudo .= "estuf=\"". $d['estuf'] ."\" "; // adiciona UF;
					$conteudo .= "descricao=\"". simec_htmlspecialchars($d['divdescricao']) ."\" "; // adiciona UF;
					$conteudo .= "endereco=\"". $d['endlog'] . ( $d['endnum'] ? "," . $d['endnum'] : ""). ( $d['endbai'] ? "," . $d['endbai'] : "") . ($d['endcep'] ? ", CEP " . $d['endcep'] : ""). "\" "; // adiciona UF;
					$conteudo .= "telefone=\"". $telefone . "\" "; // adiciona UF;
					$conteudo .= "tipo=\"". $d['divtipo'] . "\" "; // adiciona UF;
					$conteudo .= "lat=\"".$d['latitude']."\" "; // adiciona a latitude;
					$conteudo .= "lng=\"".$d['longitude']."\" "; //adiciona a longitude;
					$conteudo .= "/> ";
				
				endforeach;
				
				$conteudo .= "</markers> ";
				print $conteudo;
				
			endif;
		exit;
	
}



if($_GET['requisicao'] == "bibliotecas"){
	header('content-type:text/xml; charset=ISO-8859-1');
	
	if( $_GET['muncod'] ){
		$where = "AND mun.muncod = '{$_GET['muncod']}'";
	}
	
	$sql = "SELECT
					bibnome,
					CASE WHEN (medlatitude is not null AND medlatitude <> '...S' AND medlatitude <> '00000000') THEN 
						(CASE WHEN split_part(medlatitude, '.', 4) <>'N' THEN 
							CASE WHEN split_part(medlatitude, '.', 3) <> '' AND split_part(medlatitude, '.', 2) <> '' AND split_part(medlatitude, '.', 1) <> '' THEN
								(((split_part(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int)))*(-1)
							END
						 ELSE
						 	((SPLIT_PART(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int))
						 END)
		            END as latitude,
		            
					CASE WHEN (medlongitude is not null AND medlongitude <> '...W' AND medlongitude <> '..' ) THEN 
							(CASE WHEN (SPLIT_PART(medlongitude, '.', 1) <>'' AND SPLIT_PART(medlongitude, '.', 2) <>'' AND split_part(medlongitude, '.', 3) <>'') THEN
							               ((split_part(medlongitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlongitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlongitude, '.', 1)::int))*(-1)
							 END) 
					END as longitude,
					mun.mundescricao,
					bibdescricao,
					ed.estuf,
					ed.endcep,
					ed.endlog,
					ed.endnum,
					ed.endbai
				FROM
					pdeescola.mepontobiblioteca e
				LEFT JOIN 
					entidade.endereco ed ON e.endid = ed.endid
				LEFT JOIN 
					territorios.municipio mun ON ed.muncod = mun.muncod
				WHERE
					ed.estuf = '{$_GET['estuf']}'
					{$where}";
	
		$dados = $db->carregar($sql);
		
		if($dados[0]):
				
				$conteudo .= "<markers> "; // inicia o XML
				
				foreach($dados as $d):
					$conteudo .= "<marker "; //inicia um ponto no mapa
					$conteudo .= "entnome=\"". simec_htmlspecialchars($d['bibnome']) ."\" "; // adiciona o nome da instituição;
					$conteudo .= "mundsc=\"". simec_htmlspecialchars($d['mundescricao']) ."\" "; // adiciona a descrição do município;
					$conteudo .= "estuf=\"". $d['estuf'] ."\" "; // adiciona UF;
					$conteudo .= "descricao=\"". simec_htmlspecialchars($d['bibdescricao']) ."\" "; // adiciona UF;
					$conteudo .= "endereco=\"". $d['endlog'] . ( $d['endnum'] ? "," . $d['endnum'] : ""). ( $d['endbai'] ? "," . $d['endbai'] : "") . ($d['endcep'] ? ", CEP " . $d['endcep'] : ""). "\" "; // adiciona UF;
					$conteudo .= "telefone=\"". $telefone . "\" "; // adiciona UF;
					$conteudo .= "lat=\"".$d['latitude']."\" "; // adiciona a latitude;
					$conteudo .= "lng=\"".$d['longitude']."\" "; //adiciona a longitude;
					$conteudo .= "/> ";
				
				endforeach;
				
				$conteudo .= "</markers> ";
				print $conteudo;
				
			endif;
		exit;
	
}


if($_GET['entidEscolaDiv']){
	header('content-type:text/html; charset=ISO-8859-1');
	
	$sql = "SELECT
	 
				CASE 
				WHEN (medlatitude is not null AND medlatitude <> '...S') THEN 
					( 	 --############### LATITUDE ###################### --
								CASE WHEN (SPLIT_PART(medlatitude, '.', 1) <>'' AND SPLIT_PART(medlatitude, '.', 2) <>'' AND split_part(medlatitude, '.', 3) <>'') THEN
					               CASE WHEN split_part(medlatitude, '.', 4) <>'N' THEN
					                   (((split_part(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int)))*(-1)
					                ELSE
					                   ((SPLIT_PART(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int))
					               END
					            ELSE
					            -- Valores do IBGE convertidos em  decimal
					            CASE WHEN (length (medlatitude)=8) THEN
					                CASE WHEN length(REPLACE('0' || medlatitude,'S','')) = 8 THEN
					                    ((SUBSTR(REPLACE('0' || medlatitude,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || medlatitude,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || medlatitude,'S',''),1,2)::double precision))*(-1)
					                ELSE
					                    (SUBSTR(REPLACE('0' || medlatitude,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || medlatitude,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || medlatitude,'N',''),1,2)::double precision)
					                END
					            ELSE
					                CASE WHEN length(REPLACE(medlatitude,'S','')) = 8 THEN
					                   ((SUBSTR(REPLACE(medlatitude,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(medlatitude,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(medlatitude,'S',''),1,2)::double precision))*(-1)
					                ELSE
					                  0--((SUBSTR(REPLACE(medlatitude,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(medlatitude,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(medlatitude,'N',''),1,2)::double precision))
					                END
					            END 
					            END
					       --############### LATITUDE ###################### --
					  ) 
				ELSE (
						 --############### LATITUDE ###################### --
								CASE WHEN (SPLIT_PART(munmedlat, '.', 1) <>'' AND SPLIT_PART(munmedlat, '.', 2) <>'' AND split_part(munmedlat, '.', 3) <>'') THEN
					               CASE WHEN split_part(munmedlat, '.', 4) <>'N' THEN
					                   (((split_part(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int)))*(-1)
					                ELSE
					                   ((SPLIT_PART(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int))
					               END
					            ELSE
					            -- Valores do IBGE convertidos em  decimal
					            CASE WHEN (length (munmedlat)=8) THEN
					                CASE WHEN length(REPLACE('0' || munmedlat,'S','')) = 8 THEN
					                    ((SUBSTR(REPLACE('0' || munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),1,2)::double precision))*(-1)
					                ELSE
					                    (SUBSTR(REPLACE('0' || munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),1,2)::double precision)
					                END
					            ELSE
					                CASE WHEN length(REPLACE(munmedlat,'S','')) = 8 THEN
					                   ((SUBSTR(REPLACE(munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'S',''),1,2)::double precision))*(-1)
					                ELSE
					                  0--((SUBSTR(REPLACE(munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'N',''),1,2)::double precision))
					                END
					            END
					            END
					         --############### LATITUDE ###################### --
				     )
				END as latitude,
				CASE 
					WHEN (medlongitude is not null AND medlongitude <> '...W' AND medlongitude <> '..' ) THEN 
						( 	 --############### LONGITUDE ###################### --
						            CASE WHEN (SPLIT_PART(medlongitude, '.', 1) <>'' AND SPLIT_PART(medlongitude, '.', 2) <>'' AND split_part(medlongitude, '.', 3) <>'') THEN
						               ((split_part(medlongitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlongitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlongitude, '.', 1)::int))*(-1)
						            ELSE
						                -- Valores do IBGE convertidos em  decimal
						               (SUBSTR(REPLACE(medlongitude,'W',''),1,2)::double precision + (SUBSTR(REPLACE(medlongitude,'W',''),3,2)::double precision/60)) *(-1)
						            END
						         --############### FIM LONGITUDE ###################### --
						  ) 
					ELSE (
							 --############### LONGITUDE ###################### --
						            CASE WHEN (SPLIT_PART(munmedlog, '.', 1) <>'' AND SPLIT_PART(munmedlog, '.', 2) <>'' AND split_part(munmedlog, '.', 3) <>'') THEN
						               ((split_part(munmedlog, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlog, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlog, '.', 1)::int))*(-1)
						            ELSE
						                -- Valores do IBGE convertidos em  decimal
						               (SUBSTR(REPLACE(munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(munmedlog,'W',''),3,2)::double precision/60)) *(-1)
						            END
						         --############### FIM LONGITUDE ###################### --
					     )
				END as longitude

            FROM 
            	entidade.endereco en 
			LEFT JOIN 
				territorios.municipio mun ON en.muncod = mun.muncod
            WHERE 
            	entid='".$_REQUEST['entidEscolaDiv']."'";
	
	$dadosEscola = $db->pegaLinha($sql);
	
	if($dadosEscola['latitude'] && $dadosEscola['longitude']) {
	
		$sql = "SELECT
					divnome,
					
					CASE WHEN (medlatitude is not null AND medlatitude <> '...S' AND medlatitude <> '00000000') THEN 
						(CASE WHEN split_part(medlatitude, '.', 4) <>'N' THEN
							(((split_part(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int)))*(-1)
						 ELSE
						 	((SPLIT_PART(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int))
						 END)
		            END as latitude,
		            
					CASE WHEN (medlongitude is not null AND medlongitude <> '...W' AND medlongitude <> '..' ) THEN 
							(CASE WHEN (SPLIT_PART(medlongitude, '.', 1) <>'' AND SPLIT_PART(medlongitude, '.', 2) <>'' AND split_part(medlongitude, '.', 3) <>'') THEN
							               ((split_part(medlongitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlongitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlongitude, '.', 1)::int))*(-1)
							 END) 
					END as longitude,
					
					mun.mundescricao,
					divdescricao,
					ed.estuf,
					ed.endcep,
					ed.endlog,
					ed.endnum,
					ed.endbai,
					e.divtipo
				FROM
					pdeescola.mepontodiversos e
				LEFT JOIN 
					entidade.endereco ed ON e.endid = ed.endid
				LEFT JOIN 
					territorios.municipio mun ON ed.muncod = mun.muncod
	
				WHERE distanciaPontosGPS({$dadosEscola['latitude']},{$dadosEscola['longitude']},CASE WHEN (medlatitude is not null AND medlatitude <> '...S' AND medlatitude <> '00000000') THEN 
																										(CASE WHEN split_part(medlatitude, '.', 4) <>'N' THEN
																											(((split_part(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int)))*(-1)
															 											ELSE
																											((SPLIT_PART(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int))
															 											END)
												    											END,
												    											CASE WHEN (medlongitude is not null AND medlongitude <> '...W' AND medlongitude <> '..' ) THEN 
																										(CASE WHEN (SPLIT_PART(medlongitude, '.', 1) <>'' AND SPLIT_PART(medlongitude, '.', 2) <>'' AND split_part(medlongitude, '.', 3) <>'') THEN
									               															((split_part(medlongitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlongitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlongitude, '.', 1)::int))*(-1)
									 															END) END) < 5000";
		
	//dbg($sql);
		$dados = $db->carregar($sql);
			//dbg($dados,1);
		if($dados[0]):
				
				$conteudo .= "<markers> "; // inicia o XML
				
				foreach($dados as $d):
					$conteudo .= "<marker "; //inicia um ponto no mapa
					$conteudo .= "entnome=\"". simec_htmlspecialchars($d['divnome']) ."\" "; // adiciona o nome da instituição;
					$conteudo .= "mundsc=\"". simec_htmlspecialchars($d['mundescricao']) ."\" "; // adiciona a descrição do município;
					$conteudo .= "estuf=\"". $d['estuf'] ."\" "; // adiciona UF;
					$conteudo .= "descricao=\"". simec_htmlspecialchars($d['divdescricao']) ."\" "; // adiciona UF;
					$conteudo .= "endereco=\"". $d['endlog'] . ( $d['endnum'] ? "," . $d['endnum'] : ""). ( $d['endbai'] ? "," . $d['endbai'] : "") . ($d['endcep'] ? ", CEP " . $d['endcep'] : ""). "\" "; // adiciona UF;
					$conteudo .= "telefone=\"". $telefone . "\" "; // adiciona UF;
					$conteudo .= "tipo=\"". $d['divtipo'] . "\" "; // adiciona UF;
					$conteudo .= "lat=\"".$d['latitude']."\" "; // adiciona a latitude;
					$conteudo .= "lng=\"".$d['longitude']."\" "; //adiciona a longitude;
					$conteudo .= "/> ";
				
				endforeach;
				
				$conteudo .= "</markers> ";
				print $conteudo;
				
			endif;
		exit;
	
	}
}




if($_GET['entidEscola']){
	header('content-type:text/html; charset=ISO-8859-1');
	
	$sql = "SELECT
	 
				CASE WHEN (medlatitude is not null AND medlatitude <> '...S' AND medlatitude <> '00000000') THEN 
					CASE WHEN split_part(medlatitude, '.', 4) <>'N' THEN
						(((split_part(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int)))*(-1)
					ELSE
						((SPLIT_PART(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int))
					END
				END as latitude,
				
				CASE WHEN (medlongitude is not null AND medlongitude <> '...W' AND medlongitude <> '..' ) THEN
                	((split_part(medlongitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlongitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlongitude, '.', 1)::int))*(-1) 
                END as longitude

            FROM 
            	entidade.endereco en
            WHERE 
            	entid='".$_GET['entidEscola']."'";
	
	$dadosEscola = $db->pegaLinha($sql);
	
	if($dadosEscola['latitude'] && $dadosEscola['longitude']) {
	
		$sql = "SELECT
					mpcnome,
					
					CASE WHEN (medlatitude is not null AND medlatitude <> '...S' AND medlatitude <> '00000000') THEN 
						(CASE WHEN split_part(medlatitude, '.', 4) <>'N' THEN
							(((split_part(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int)))*(-1)
						 ELSE
						 	((SPLIT_PART(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int))
						 END)
		            END as latitude,
		            
					CASE WHEN (medlongitude is not null AND medlongitude <> '...W' AND medlongitude <> '..' ) THEN 
							(CASE WHEN (SPLIT_PART(medlongitude, '.', 1) <>'' AND SPLIT_PART(medlongitude, '.', 2) <>'' AND split_part(medlongitude, '.', 3) <>'') THEN
							               ((split_part(medlongitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlongitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlongitude, '.', 1)::int))*(-1)
							 END) 
					END as longitude,
					
					mun.mundescricao,
					mpcdescricao,
					ed.estuf,
					ed.endcep,
					ed.endlog,
					ed.endnum,
					ed.endbai
				FROM
					pdeescola.mepontocultura e
				LEFT JOIN 
					entidade.endereco ed ON e.endid = ed.endid
				LEFT JOIN 
					territorios.municipio mun ON ed.muncod = mun.muncod
	
				WHERE distanciaPontosGPS({$dadosEscola['latitude']},{$dadosEscola['longitude']},CASE WHEN (medlatitude is not null AND medlatitude <> '...S' AND medlatitude <> '00000000') THEN 
																										(CASE WHEN split_part(medlatitude, '.', 4) <>'N' THEN
																											(((split_part(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int)))*(-1)
															 											ELSE
																											((SPLIT_PART(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int))
															 											END)
												    											END,
												    											CASE WHEN (medlongitude is not null AND medlongitude <> '...W' AND medlongitude <> '..' ) THEN 
																										(CASE WHEN (SPLIT_PART(medlongitude, '.', 1) <>'' AND SPLIT_PART(medlongitude, '.', 2) <>'' AND split_part(medlongitude, '.', 3) <>'') THEN
									               															((split_part(medlongitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlongitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlongitude, '.', 1)::int))*(-1)
									 															END) END) < 5000";
		
	//dbg($sql);
		$dados = $db->carregar($sql);
			//dbg($dados,1);
		if($dados[0]):
				
				$conteudo .= "<markers> "; // inicia o XML
				
				foreach($dados as $d):
					$conteudo .= "<marker "; //inicia um ponto no mapa
					$conteudo .= "entnome=\"". simec_htmlspecialchars($d['mpcnome']) ."\" "; // adiciona o nome da instituição;
					$conteudo .= "mundsc=\"". simec_htmlspecialchars($d['mundescricao']) ."\" "; // adiciona a descrição do município;
					$conteudo .= "estuf=\"". $d['estuf'] ."\" "; // adiciona UF;
					$conteudo .= "descricao=\"". simec_htmlspecialchars($d['mpcdescricao']) ."\" "; // adiciona UF;
					$conteudo .= "endereco=\"". $d['endlog'] . ( $d['endnum'] ? "," . $d['endnum'] : ""). ( $d['endbai'] ? "," . $d['endbai'] : "") . ($d['endcep'] ? ", CEP " . $d['endcep'] : ""). "\" "; // adiciona UF;
					$conteudo .= "telefone=\"". $telefone . "\" "; // adiciona UF;
					$conteudo .= "lat=\"".$d['latitude']."\" "; // adiciona a latitude;
					$conteudo .= "lng=\"".$d['longitude']."\" "; //adiciona a longitude;
					$conteudo .= "/> ";
				
				endforeach;
				
				$conteudo .= "</markers> ";
				print $conteudo;
				
			endif;

	
	}
	
	exit;
}






if($_GET['entidEscolaBib']){
	header('content-type:text/html; charset=ISO-8859-1');
	
	$sql = "SELECT
	 
				CASE WHEN (medlatitude is not null AND medlatitude <> '...S' AND medlatitude <> '00000000') THEN 
					CASE WHEN split_part(medlatitude, '.', 4) <>'N' THEN
						(((split_part(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int)))*(-1)
					ELSE
						((SPLIT_PART(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int))
					END
				END as latitude,
				
				CASE WHEN (medlongitude is not null AND medlongitude <> '...W' AND medlongitude <> '..' ) THEN
                	((split_part(medlongitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlongitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlongitude, '.', 1)::int))*(-1) 
                END as longitude

            FROM 
            	entidade.endereco en
            WHERE 
            	entid='".$_GET['entidEscolaBib']."'";
	
	$dadosEscola = $db->pegaLinha($sql);
	
	if($dadosEscola['latitude'] && $dadosEscola['longitude']) {
	
		$sql = "SELECT
					bibnome,
					
					CASE WHEN (medlatitude is not null AND medlatitude <> '...S' AND medlatitude <> '00000000') THEN 
						(CASE WHEN split_part(medlatitude, '.', 4) <>'N' THEN
							CASE WHEN split_part(medlatitude, '.', 3) <> '' AND split_part(medlatitude, '.', 2) <> '' AND split_part(medlatitude, '.', 1) <> '' THEN
								(((split_part(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int)))*(-1)
							END
						 ELSE
						 	((SPLIT_PART(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int))
						 END)
		            END as latitude,
		            
					CASE WHEN (medlongitude is not null AND medlongitude <> '...W' AND medlongitude <> '..' ) THEN 
							(CASE WHEN (SPLIT_PART(medlongitude, '.', 1) <>'' AND SPLIT_PART(medlongitude, '.', 2) <>'' AND split_part(medlongitude, '.', 3) <>'') THEN
							               ((split_part(medlongitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlongitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlongitude, '.', 1)::int))*(-1)
							 END) 
					END as longitude,
					
					mun.mundescricao,
					bibdescricao,
					ed.estuf,
					ed.endcep,
					ed.endlog,
					ed.endnum,
					ed.endbai
				FROM
					pdeescola.mepontobiblioteca e
				LEFT JOIN 
					entidade.endereco ed ON e.endid = ed.endid
				LEFT JOIN 
					territorios.municipio mun ON ed.muncod = mun.muncod
	
				WHERE distanciaPontosGPS({$dadosEscola['latitude']},{$dadosEscola['longitude']},CASE WHEN (medlatitude is not null AND medlatitude <> '...S' AND medlatitude <> '00000000') THEN 
																										(CASE WHEN split_part(medlatitude, '.', 4) <>'N' THEN
																											CASE WHEN split_part(medlatitude, '.', 3) <> '' AND split_part(medlatitude, '.', 2) <> '' AND split_part(medlatitude, '.', 1) <> '' THEN
																												(((split_part(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int)))*(-1)
																											END
															 											ELSE
																											((SPLIT_PART(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int))
															 											END)
												    											END,
												    											CASE WHEN (medlongitude is not null AND medlongitude <> '...W' AND medlongitude <> '..' ) THEN 
																										(CASE WHEN (SPLIT_PART(medlongitude, '.', 1) <>'' AND SPLIT_PART(medlongitude, '.', 2) <>'' AND split_part(medlongitude, '.', 3) <>'') THEN
									               															((split_part(medlongitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlongitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlongitude, '.', 1)::int))*(-1)
									 															END) END) < 5000";
		$dados = $db->carregar($sql);

		if($dados[0]):
				
				$conteudo .= "<markers> "; // inicia o XML
				
				foreach($dados as $d):
					$conteudo .= "<marker "; //inicia um ponto no mapa
					$conteudo .= "entnome=\"". simec_htmlspecialchars($d['bibnome']) ."\" "; // adiciona o nome da instituição;
					$conteudo .= "mundsc=\"". simec_htmlspecialchars($d['mundescricao']) ."\" "; // adiciona a descrição do município;
					$conteudo .= "estuf=\"". $d['estuf'] ."\" "; // adiciona UF;
					$conteudo .= "descricao=\"". simec_htmlspecialchars($d['bibdescricao']) ."\" "; // adiciona UF;
					$conteudo .= "endereco=\"". $d['endlog'] . ( $d['endnum'] ? "," . $d['endnum'] : ""). ( $d['endbai'] ? "," . $d['endbai'] : "") . ($d['endcep'] ? ", CEP " . $d['endcep'] : ""). "\" "; // adiciona UF;
					$conteudo .= "telefone=\"". $telefone . "\" "; // adiciona UF;
					$conteudo .= "lat=\"".$d['latitude']."\" "; // adiciona a latitude;
					$conteudo .= "lng=\"".$d['longitude']."\" "; //adiciona a longitude;
					$conteudo .= "/> ";
				
				endforeach;
				
				$conteudo .= "</markers> ";
				print $conteudo;
				
			endif;
	
	}
	exit;
}


?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="br" lang="br">
<head>
<title>Google Maps</title>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1" />
<?php
//Pega endereço para marcação de ponto
if($_GET['entid']){
	
	$sql = "select
				entnome,
				medlatitude,
				medlongitude,
				CASE 
				WHEN (medlatitude is not null AND medlatitude <> '...S') THEN 
					( 	 --############### LATITUDE ###################### --
								CASE WHEN (SPLIT_PART(medlatitude, '.', 1) <>'' AND SPLIT_PART(medlatitude, '.', 2) <>'' AND split_part(medlatitude, '.', 3) <>'') THEN
					               CASE WHEN split_part(medlatitude, '.', 4) <>'N' THEN
					                   (((split_part(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int)))*(-1)
					                ELSE
					                   ((SPLIT_PART(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int))
					               END
					            ELSE
					            -- Valores do IBGE convertidos em  decimal
					            CASE WHEN (length (medlatitude)=8) THEN
					                CASE WHEN length(REPLACE('0' || medlatitude,'S','')) = 8 THEN
					                    ((SUBSTR(REPLACE('0' || medlatitude,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || medlatitude,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || medlatitude,'S',''),1,2)::double precision))*(-1)
					                ELSE
					                    (SUBSTR(REPLACE('0' || medlatitude,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || medlatitude,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || medlatitude,'N',''),1,2)::double precision)
					                END
					            ELSE
					                CASE WHEN length(REPLACE(medlatitude,'S','')) = 8 THEN
					                   ((SUBSTR(REPLACE(medlatitude,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(medlatitude,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(medlatitude,'S',''),1,2)::double precision))*(-1)
					                ELSE
					                  0--((SUBSTR(REPLACE(medlatitude,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(medlatitude,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(medlatitude,'N',''),1,2)::double precision))
					                END
					            END 
					            END
					       --############### LATITUDE ###################### --
					  ) 
				ELSE (
						 --############### LATITUDE ###################### --
								CASE WHEN (SPLIT_PART(munmedlat, '.', 1) <>'' AND SPLIT_PART(munmedlat, '.', 2) <>'' AND split_part(munmedlat, '.', 3) <>'') THEN
					               CASE WHEN split_part(munmedlat, '.', 4) <>'N' THEN
					                   (((split_part(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int)))*(-1)
					                ELSE
					                   ((SPLIT_PART(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int))
					               END
					            ELSE
					            -- Valores do IBGE convertidos em  decimal
					            CASE WHEN (length (munmedlat)=8) THEN
					                CASE WHEN length(REPLACE('0' || munmedlat,'S','')) = 8 THEN
					                    ((SUBSTR(REPLACE('0' || munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),1,2)::double precision))*(-1)
					                ELSE
					                    (SUBSTR(REPLACE('0' || munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),1,2)::double precision)
					                END
					            ELSE
					                CASE WHEN length(REPLACE(munmedlat,'S','')) = 8 THEN
					                   ((SUBSTR(REPLACE(munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'S',''),1,2)::double precision))*(-1)
					                ELSE
					                  0--((SUBSTR(REPLACE(munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'N',''),1,2)::double precision))
					                END
					            END
					            END
					         --############### LATITUDE ###################### --
				     )
				END as latitude,
				CASE 
					WHEN (medlongitude is not null AND medlongitude <> '...W' AND medlongitude <> '..' ) THEN 
						( 	 --############### LONGITUDE ###################### --
						            CASE WHEN (SPLIT_PART(medlongitude, '.', 1) <>'' AND SPLIT_PART(medlongitude, '.', 2) <>'' AND split_part(medlongitude, '.', 3) <>'') THEN
						               ((split_part(medlongitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlongitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlongitude, '.', 1)::int))*(-1)
						            ELSE
						                -- Valores do IBGE convertidos em  decimal
						               (SUBSTR(REPLACE(medlongitude,'W',''),1,2)::double precision + (SUBSTR(REPLACE(medlongitude,'W',''),3,2)::double precision/60)) *(-1)
						            END
						         --############### FIM LONGITUDE ###################### --
						  ) 
					ELSE (
							 --############### LONGITUDE ###################### --
						            CASE WHEN (SPLIT_PART(munmedlog, '.', 1) <>'' AND SPLIT_PART(munmedlog, '.', 2) <>'' AND split_part(munmedlog, '.', 3) <>'') THEN
						               ((split_part(munmedlog, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlog, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlog, '.', 1)::int))*(-1)
						            ELSE
						                -- Valores do IBGE convertidos em  decimal
						               (SUBSTR(REPLACE(munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(munmedlog,'W',''),3,2)::double precision/60)) *(-1)
						            END
						         --############### FIM LONGITUDE ###################### --
					     )
				END as longitude,
				mun.mundescricao,
				ed.estuf,
				ed.endcep,
				ed.endlog,
				ed.endnum,
				ed.endbai
			FROM
				entidade.entidade e
			LEFT JOIN 
				entidade.endereco ed ON e.entid = ed.entid
			LEFT JOIN 
				territorios.municipio mun ON ed.muncod = mun.muncod
			WHERE
				e.entid = {$_GET['entid']}
			AND
				e.entstatus = 'A'";

$dados = $db->pegaLinha($sql);

$endereco  = $dados['endlog'] . ( $dados['endnum'] ? "," . $dados['endnum'] : ""). ( $dados['endbai'] ? "," . $dados['endbai'] : "") . ($dados['endcep'] ? ", CEP " . $dados['endcep'] : "");

	
}

function curPageURL() {
	$pageURL = 'http';
	if ($_SERVER["HTTPS"] == "on") {$pageURL .= "s";}
		$pageURL .= "://";
	if ($_SERVER["SERVER_PORT"] != "80") {
		$pageURL .= $_SERVER["SERVER_NAME"].":".$_SERVER["SERVER_PORT"].$_SERVER["REQUEST_URI"];
	} else {
		$pageURL .= $_SERVER["SERVER_NAME"].$_SERVER["REQUEST_URI"];
	}
	return $pageURL;
}

$local= explode("/",curPageURL());?>
<?if ($local[2]=="simec.mec.gov.br" ){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxQhVwj8ALbvbyVgNcB-R-H_S2MIRxTIdhrqjcwTK3xxl_Nu_YMC5SdLWg" type="text/javascript"></script>
	<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxQhVwj8ALbvbyVgNcB-R-H_S2MIRxTIdhrqjcwTK3xxl_Nu_YMC5SdLWg"; ?>
<? } ?>
<?if ($local[2]=="simec-d.mec.gov.br"){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxRYtD8tuHxswJ_J7IRZlgTxP-EUtxT_Cz5IMSBe6d3M1dq-XAJNIvMcpg" type="text/javascript"></script>
	<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxRYtD8tuHxswJ_J7IRZlgTxP-EUtxT_Cz5IMSBe6d3M1dq-XAJNIvMcpg"; ?> 
<? } ?>
<?if ($local[2]=="simec" ){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxTNzTBk8zukZFuO3BxF29LAEN1D1xSIcGWxF7HCjMwks0HURg6MTfdk1A" type="text/javascript"></script>
	<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxTNzTBk8zukZFuO3BxF29LAEN1D1xSIcGWxF7HCjMwks0HURg6MTfdk1A"; ?>
<? } ?>
<?if ($local[2]=="simec-d"){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxTFm3qU4CVFuo3gZaqihEzC-0jfaRTY9Fe8UfzYeoYDxtThvI3nGbbZEw" type="text/javascript"></script>
	<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxTFm3qU4CVFuo3gZaqihEzC-0jfaRTY9Fe8UfzYeoYDxtThvI3nGbbZEw"; ?> 
<? } ?>
<?if ($local[2]=="simec-local"){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxRzjpIxsx3o6RYEdxEmCzeJMTc4zBSMifny_dJtMKLfrwCcYh5B01Pq_g" type="text/javascript"></script>
	<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxRzjpIxsx3o6RYEdxEmCzeJMTc4zBSMifny_dJtMKLfrwCcYh5B01Pq_g"; ?> 	
<? } ?>
<?if ($local[2]=="painel.mec.gov.br"){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxTPkFYZwQy2nvpGvFj08HQmPOt9ZBT2EJmQsTms0WQqU_5GvEj7bMZd7g" type="text/javascript"></script>
	<? $GKey = "ABQIAAAAwN0kvNsueYw8CBs704pusxTPkFYZwQy2nvpGvFj08HQmPOt9ZBT2EJmQsTms0WQqU_5GvEj7bMZd7g"; ?> 	
<? } ?>

<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>

<script type="text/javascript">

var countModal = 1;

function montaShowModal() {
	var campoTextArea = ''+
		'<p align=\'justify\'>OBSERVÇÃO IMPORTANTE: O georreferenciamento aqui apresentado tem por finalidade auxiliar as escolas a traçar e visualizar os possíveis itinerários educativos para a educação integral, por isso é preciso atentar para alguns detalhes:'+
		'<br /><br /> Utilizamos uma base de dados pública que tem limitações, a maioria dos municípios brasileiros não está georreferenciado (latitude e longitude) em sua totalidade com precisão de rua, número, bairro e CEP, desta forma, em alguns casos, é possível'+
		'<br /><br />que o posicionamento aqui apresentado não corresponda a real localização do referido espaço, por isso faz-se necessário um contato (telefone/e-mail) e visita prévia ao espaço escolhido,'+
		'<br /><br />por parte da escola, para planejamento estratégico das ações antes de levarem as crianças e adolescentes aos espaços aqui disponíveis.';
	var alertaDisplay = '<div class="titulo_box" >Prezado usuário, <br/ >'+campoTextArea+'</div><div class="links_box" ><center><input type="button" onclick=\'closeMessage(); return false \' value="Fechar" /></center>';
	displayStaticMessage(alertaDisplay,false);
	return false;
}

function displayStaticMessage(messageContent,cssClass) {
	messageObj = new DHTML_modalMessage();	// Cria-se apenas um objeto desta classe
	messageObj.setShadowOffset(5);	// Sombra
	
	messageObj.setHtmlContent(messageContent);
	messageObj.setSize(450,355);
	messageObj.setCssClassMessageBox(cssClass);
	messageObj.setSource(false);	// Sem source HTML, já que a mensagem é estática.
	messageObj.setShadowDivVisible(false);	// Desativando sombras para estas caixas	
	messageObj.display();
}

function closeMessage() {
	messageObj.close();	
	//window.location.href='pdeescola.php?modulo=principal/mapaEntidade&acao=A&entid=<?php echo $_REQUEST['entid']?>';	
}

var markerGroups = { '1': [], '2': [], '3': [], '4': []};

// Converter em Graus
function dec2grau(valor){
	ddLat=valor+"";
	if (ddLat.substr(0,1) == "-") {
		ddLatVal = ddLat.substr(1,ddLat.length-1);
	} else {
		ddLatVal = ddLat;
	}
		
	// Graus 
	ddLatVals = ddLatVal.split(".");
	dmsLatDeg = ddLatVals[0];
		
	// * 60 = mins
	ddLatRemainder  = ("0." + ddLatVals[1]) * 60;
	dmsLatMinVals   = ddLatRemainder.toString().split(".");
	dmsLatMin = dmsLatMinVals[0];
			
	// * 60 novamente = secs
	ddLatMinRemainder = ("0." + dmsLatMinVals[1]) * 60;
	dmsLatSec  = Math.round(ddLatMinRemainder);
		
	valor=dmsLatDeg+unescape('%B0')+" "+dmsLatMin+"' "+dmsLatSec+"''";

   	return valor;
}


var map;
var geocoder;
var address;
var t;

function showAddress(response) {
	if (!response || response.Status.code != 200) {
		alert("Status Code:" + response.Status.code);
	} else {
		place = response.Placemark[0];
		var uf_ponto='';
		var mundsc_ponto='';
        var bairro_ponto='';
        var cep_ponto='';
        
        if(place.AddressDetails.Country.AdministrativeArea.AdministrativeAreaName) {uf_ponto = place.AddressDetails.Country.AdministrativeArea.AdministrativeAreaName;}
        if(place.AddressDetails.Country.AdministrativeArea.Locality) {
	        if(place.AddressDetails.Country.AdministrativeArea.Locality.LocalityName) {mundsc_ponto = place.AddressDetails.Country.AdministrativeArea.Locality.LocalityName;}
	        if(place.AddressDetails.Country.AdministrativeArea.Locality.DependentLocality) {
	        	if(place.AddressDetails.Country.AdministrativeArea.Locality.DependentLocality.DependentLocalityName != '') {
	        		bairro_ponto = place.AddressDetails.Country.AdministrativeArea.Locality.DependentLocality.DependentLocalityName;
	        	}
	        	if(place.AddressDetails.Country.AdministrativeArea.Locality.DependentLocality.PostalCode) {
	        		if(place.AddressDetails.Country.AdministrativeArea.Locality.DependentLocality.PostalCode.PostalCodeNumber) {
	        			cep_ponto = place.AddressDetails.Country.AdministrativeArea.Locality.DependentLocality.PostalCode.PostalCodeNumber;
	        		}
	        	}
	        }
	    }
        var tileCoordinate = new GPoint();
         var tilePoint = new GPoint();
	     var currentProjection = G_NORMAL_MAP.getProjection();
	          tilePoint = currentProjection.fromLatLngToPixel(address, map.getZoom());
	          var myHtml = "<table class=listagem width=400px>" +
					"<tr><td class=SubTituloDireita>Nome:</td><td><input type=text class=normal maxlength=255 size=48 id=divnome name=divnome></td></tr>" + 
					"<tr><td class=SubTituloDireita>Descrição:</td><td><textarea name=divdescricao id=divdescricao class=normal rows=5 cols=45></textarea></td></tr>" +
					"<tr><td class=SubTituloDireita>Tipo:</td><td><select id=divtipo style=width:auto; class=CampoEstilo name=divtipo><option value=>Selecione</option><option value=museus>MUSEUS</option><option value=casas_do_patrimonio>CASAS DO PATRÍMÔNIO</option><option value=centro_cultural>CENTRO CULTURAL</option><option value=teatro>TEATRO</option><option value=cinema>CINEMA</option><option value=parque_da_cidade>PARQUE DA CIDADE</option><option value=assoc_de_moradores>ASSOCIAÇÃO DE MORADORES</option><option value=clube_das_maes>CLUBE DE MÃES</option><option value=campo_de_futebol>CAMPO DE FUTEBOL</option><option value=praca>PRAÇA</option><option value=igreja>IGREJA</option><option value=clubes>CLUBES</option><option value=ref_assis_social>CENTRO DE REFERÊNCIA DE ASSISTÊNCIA SOCIAL</option><option value=ref_juventude>CENTRO DE REFERÊNCIA DA JUVENTUDE</option><option value=clube_de_idosos>CLUBE DE IDOSOS</option><option value=clube_de_saude>POSTO DE SAÚDE</option><option value=hospital>HOSPITAL</option></select></td></tr>" +
					"<tr><td class=SubTituloDireita>Latitude:</td><td>" + dec2grau(address.lat()) + "</td></tr>" +
					"<tr><td class=SubTituloDireita>Longitude:</td><td>" + dec2grau(address.lng()) + "</td></tr>" +
					"<tr><td class=SubTituloDireita>Zoom:</td><td>" + map.getZoom() + "</td></tr>" +
					"<tr><td class=SubTituloDireita>Endereço:</td><td>" + place.address + "<input type=hidden name=endlog id=endlog value='"+ place.address +"'></td></tr>" +
					"<tr><td class=SubTituloDireita colspan=2><input type=button name=gravar value=Gravar onclick=gravarPonto("+address.lat()+","+address.lng()+","+map.getZoom()+");><input type=hidden name=estuf id=estuf value="+uf_ponto+"><input type=hidden name=mundsc id=mundsc value="+mundsc_ponto+"><input type=hidden name=endbai id=endbai value="+bairro_ponto+"><input type=hidden name=endcep id=endcep value="+cep_ponto+"></td>" +
					"</table>";
					
	          map.openInfoWindow(address, myHtml);

  }
}


function initialize() {
	if (GBrowserIsCompatible()) { // verifica se o navegador é compatível
			map = new GMap2(document.getElementById("google_map")); // inicila com a div mapa
			var zoom = 4;	var lat_i = -14.689881; var lng_i = -52.373047;	//Brasil	
			map.setCenter(new GLatLng(lat_i,lng_i), parseInt(zoom)); //Centraliza e aplica o zoom
			
			// Início Controles
			map.addControl(new GMapTypeControl());
			map.addControl(new GLargeMapControl3D());
	        map.addControl(new GOverviewMapControl());
	        map.enableScrollWheelZoom();
	        map.addMapType(G_PHYSICAL_MAP);
	        // Fim Controles
	        <? if($db->testa_superuser()): ?>
	        
	        GEvent.addListener(map,"click", function(overlay,latlng) {
			  if (latlng != null) {
			    address = latlng;
			    geocoder.getLocations(latlng, showAddress);
			  }
			  

	        });
	        
  			geocoder = new GClientGeocoder();
  			
  			<? endif; ?>
	
	}
}

<? if($_REQUEST['entid']): ?>
function criaPontoEscola(){
	
	html = "<div style=\"font-family:verdana;font-size:11px;padding:10px\" >";
	html += "<b>Instituição:</b> <?php echo $dados['entnome']?><br />";
	html += "<b>Endereço:</b> <?php echo $endereco ?><br />";
	html += "<b>Localização:</b> <?php echo $dados['mundescricao']?>/<?php echo $dados['estuf']?><br /><br />";
	html += "</div>";
	
	var title = '<?php echo $dados['entnome']?>';
	
	var baseIcon1 = new GIcon();
	baseIcon1.iconSize = new GSize(15, 17);
	baseIcon1.iconAnchor = new GPoint(11, 13);
	baseIcon1.infoWindowAnchor = new GPoint(9, 2);
	baseIcon1.image='/imagens/icones/tr.png';
					
	var point = new GLatLng(<?php echo $dados['latitude']?>,<?php echo $dados['longitude']?>);				
	
	// Cria o marcador na tela
	var marker = createMarker(point,title,baseIcon1,html);
	
	markerGroups['1'].push(marker);
	
	map.addOverlay(marker);
	
	map.setCenter(new GLatLng(<?php echo $dados['latitude']?>,<?php echo $dados['longitude']?>), 6); //Centraliza e aplica o zoom
	

}
<?php endif; ?>


function gravarPonto(lat, lng, zoom) {
	$.ajax({
   		type: "POST",
   		url: "pdeescola.php?modulo=principal/mapaEntidade&acao=A",
   		data: "requisicao=gravarPonto&lat="+lat+"&lng="+lng+"&zoom="+zoom+"&divnome="+document.getElementById('divnome').value+"&divdescricao="+document.getElementById('divdescricao').value+"&divtipo="+document.getElementById('divtipo').value+"&estuf="+document.getElementById('estuf').value+"&mundsc="+document.getElementById('mundsc').value+"&endbai="+document.getElementById('endbai').value+"&endcep="+document.getElementById('endcep').value+"&endlog="+document.getElementById('endlog').value,
   		success: function(msg){
   			alert(msg);
   			map.closeInfoWindow();
			<?php if($_REQUEST['entid']): ?>
				criaPontosDiversos("pdeescola.php?modulo=principal/mapaEntidade&acao=A&entidEscolaDiv=<? echo $_REQUEST['entid']?>");
			<?php endif; ?>
			<?php if($_GET['estuf']): ?>
				criaPontosDiversosPorEstado('<? echo $_REQUEST['estuf'] ?>');
			<?php endif; ?>
			

   		}
 		});


}


function criaBibliotecas(xml_filtro){	

	var baseIcon2 = new GIcon();
	baseIcon2.iconSize = new GSize(15, 17);
	baseIcon2.iconAnchor = new GPoint(11, 13);
	baseIcon2.infoWindowAnchor = new GPoint(9, 2);
	baseIcon2.image='/imagens/icones/ty.png';
		
	// Criando os Marcadores com o resultado
	GDownloadUrl(xml_filtro, function(data) {
		var xml = GXml.parse(data);
		
		var markers = xml.documentElement.getElementsByTagName("marker");
		
		if(markers.length > 0) {
			var lat_ant=0;
			var lng_ant=0;
				
			for (var i = 0; i < markers.length; i++) {
						
				var entnome = markers[i].getAttribute("entnome");
				var mundsc = markers[i].getAttribute("mundsc");
				var estuf = markers[i].getAttribute("estuf");
				var descricao = markers[i].getAttribute("descricao");
				var endereco = markers[i].getAttribute("endereco");
				var telefone = markers[i].getAttribute("telefone");
				title = entnome; 
										
				icon = baseIcon2;
				
				var lat = markers[i].getAttribute("lat");
				var lng = markers[i].getAttribute("lng");
				
				var html;
						
				html = "<div style=\"font-family:verdana;font-size:11px;padding:10px\" >";
				html += "<b>Biblioteca:</b> " + entnome + "<br />";
				html += "<b>Descrição:</b> " + descricao + "<br />";
				html += "<b>Endereço:</b> " + endereco + "<br />";
				html += "<b>Localização:</b> " + mundsc + "/" + estuf + "<br /><br />";
				html += "</div>";

				// Verifica pontos em um mesmo lugar e move o seguinte para a direita
				if(lat_ant==markers[i].getAttribute("lat") && lng_ant==markers[i].getAttribute("lng"))
					var point = new GLatLng(markers[i].getAttribute("lat"),	markers[i].getAttribute("lng"));
				else
					var point = new GLatLng(markers[i].getAttribute("lat"),	parseFloat(markers[i].getAttribute("lng"))+0.0005);				
	
				lat_ant=markers[i].getAttribute("lat");
				lng_ant=markers[i].getAttribute("lng");
				
				// Cria o marcador na tela
				var marker = createMarker(point,title,icon,html);
				
				markerGroups['3'].push(marker);
				
				map.addOverlay(marker);
				
		}
		}
	});
}



function criaPontosDiversos(xml_filtro){	

	var baseIcon2 = new GIcon();
	baseIcon2.iconSize = new GSize(15, 17);
	baseIcon2.iconAnchor = new GPoint(11, 13);
	baseIcon2.infoWindowAnchor = new GPoint(9, 2);
	baseIcon2.image='/imagens/icones/tp.png';
		
	// Criando os Marcadores com o resultado
	GDownloadUrl(xml_filtro, function(data) {
		var xml = GXml.parse(data);
		
		var markers = xml.documentElement.getElementsByTagName("marker");
		
		if(markers.length > 0) {
			var lat_ant=0;
			var lng_ant=0;
				
			for (var i = 0; i < markers.length; i++) {
						
				var entnome = markers[i].getAttribute("entnome");
				var mundsc = markers[i].getAttribute("mundsc");
				var estuf = markers[i].getAttribute("estuf");
				var descricao = markers[i].getAttribute("descricao");
				var endereco = markers[i].getAttribute("endereco");
				var telefone = markers[i].getAttribute("telefone");
				var tipo = markers[i].getAttribute("tipo");
				title = entnome; 
										
				icon = baseIcon2;
				
				var lat = markers[i].getAttribute("lat");
				var lng = markers[i].getAttribute("lng");
				
				var html;
						
				html = "<div style=\"font-family:verdana;font-size:11px;padding:10px\" >";
				html += "<b>Nome do ponto:</b> " + entnome + "<br />";
				html += "<b>Tipo do ponto:</b> " + tipo + "<br />";
				html += "<b>Descrição:</b> " + descricao + "<br />";
				html += "<b>Endereço:</b> " + endereco + "<br />";
				html += "<b>Localização:</b> " + mundsc + "/" + estuf + "<br /><br />";
				html += "</div>";

				// Verifica pontos em um mesmo lugar e move o seguinte para a direita
				if(lat_ant==markers[i].getAttribute("lat") && lng_ant==markers[i].getAttribute("lng"))
					var point = new GLatLng(markers[i].getAttribute("lat"),	markers[i].getAttribute("lng"));
				else
					var point = new GLatLng(markers[i].getAttribute("lat"),	parseFloat(markers[i].getAttribute("lng"))+0.0005);				
	
				lat_ant=markers[i].getAttribute("lat");
				lng_ant=markers[i].getAttribute("lng");
				
				// Cria o marcador na tela
				var marker = createMarker(point,title,icon,html);
				
				markerGroups['4'].push(marker);
				
				map.addOverlay(marker);
				
		}
		}
	});
}




function criaPontosCulturais(xml_filtro){	

	var baseIcon2 = new GIcon();
	baseIcon2.iconSize = new GSize(15, 17);
	baseIcon2.iconAnchor = new GPoint(11, 13);
	baseIcon2.infoWindowAnchor = new GPoint(9, 2);
	baseIcon2.image='/imagens/icones/tb.png';
		
	// Criando os Marcadores com o resultado
	GDownloadUrl(xml_filtro, function(data) {
		var xml = GXml.parse(data);
		
		var markers = xml.documentElement.getElementsByTagName("marker");
		
		if(markers.length > 0) {
			var lat_ant=0;
			var lng_ant=0;
				
			for (var i = 0; i < markers.length; i++) {
						
				var entnome = markers[i].getAttribute("entnome");
				var mundsc = markers[i].getAttribute("mundsc");
				var estuf = markers[i].getAttribute("estuf");
				var descricao = markers[i].getAttribute("descricao");
				var endereco = markers[i].getAttribute("endereco");
				var telefone = markers[i].getAttribute("telefone");
				title = entnome; 
										
				icon = baseIcon2;
				
				var lat = markers[i].getAttribute("lat");
				var lng = markers[i].getAttribute("lng");
				
				var html;
						
				html = "<div style=\"font-family:verdana;font-size:11px;padding:10px\" >";
				html += "<b>Ponto de Cultura:</b> " + entnome + "<br />";
				html += "<b>Descrição:</b> " + descricao + "<br />";
				html += "<b>Endereço:</b> " + endereco + "<br />";
				html += "<b>Localização:</b> " + mundsc + "/" + estuf + "<br /><br />";
				html += "</div>";

				// Verifica pontos em um mesmo lugar e move o seguinte para a direita
				if(lat_ant==markers[i].getAttribute("lat") && lng_ant==markers[i].getAttribute("lng"))
					var point = new GLatLng(markers[i].getAttribute("lat"),	markers[i].getAttribute("lng"));
				else
					var point = new GLatLng(markers[i].getAttribute("lat"),	parseFloat(markers[i].getAttribute("lng"))+0.0005);				
	
				lat_ant=markers[i].getAttribute("lat");
				lng_ant=markers[i].getAttribute("lng");
				
				// Cria o marcador na tela
				var marker = createMarker(point,title,icon,html);
				
				markerGroups['2'].push(marker);
				
				map.addOverlay(marker);
				
		}
		}
	});
}

function createMarker(posn, title, icon, html,muncod) {
	var marker = new GMarker(posn, {title: title, icon: icon, draggable:false });
	if(html != false){
		GEvent.addListener(marker, "click", function() {
		marker.openInfoWindowHtml(html);
		});
	}
	return marker;
}

function exibePontos(obj){
	
	if(obj.checked == true){
		if(markerGroups[obj.value]){
			for (var i = 0; i < markerGroups[obj.value].length; i++) {
		        var marker = markerGroups[obj.value][i];
		        marker.show();
			}
		}	
	}else{
		if(markerGroups[obj.value]){
			for (var i = 0; i < markerGroups[obj.value].length; i++) {
		        var marker = markerGroups[obj.value][i];
		        marker.hide();
			}
		}
	}	
		

}

function carregaEscolaPorEstado(estuf){
	window.location.href='pdeescola.php?modulo=principal/mapaEntidade&acao=A&estuf=' + estuf;
}

function carregaEscolaPorMunicipio(muncod){
	estuf = document.getElementById('estadoselecionado').value;
	window.location.href='pdeescola.php?modulo=principal/mapaEntidade&acao=A&estuf=' + estuf + '&muncod=' + muncod;
}


function criaEscolaPorEstado(estuf){

	if(document.getElementById('municipioselecionado').value){
		muncod = document.getElementById('municipioselecionado').value;
		xml_filtro = "pdeescola.php?modulo=principal/mapaEntidade&acao=A&requisicao=pontosEscola&estuf=" + estuf + "&muncod=" + muncod;
	}else{
		xml_filtro = "pdeescola.php?modulo=principal/mapaEntidade&acao=A&requisicao=pontosEscola&estuf=" + estuf;
	}

	var baseIcon1 = new GIcon();
	baseIcon1.iconSize = new GSize(15, 17);
	baseIcon1.iconAnchor = new GPoint(11, 13);
	baseIcon1.infoWindowAnchor = new GPoint(9, 2);
	baseIcon1.image='/imagens/icones/tr.png';
		
	// Criando os Marcadores com o resultado
	GDownloadUrl(xml_filtro, function(data) {
		var xml = GXml.parse(data);
		
		var markers = xml.documentElement.getElementsByTagName("marker");
		
		if(markers.length > 0) {
			var lat_ant=0;
			var lng_ant=0;
				
			for (var i = 0; i < markers.length; i++) {
						
				var entnome = markers[i].getAttribute("entnome");
				var mundsc = markers[i].getAttribute("mundsc");
				var estuf = markers[i].getAttribute("estuf");
				var endereco = markers[i].getAttribute("endereco");
				title = entnome; 
										
				icon = baseIcon1;
				
				var lat = markers[i].getAttribute("lat");
				var lng = markers[i].getAttribute("lng");
				
				var html;
						
				html = "<div style=\"font-family:verdana;font-size:11px;padding:10px;width:300px\" >";
				html += "<b>Instituição:</b> " + entnome + "<br />";
				html += "<b>Endereço:</b> " + endereco + "<br />";
				html += "<b>Localização:</b> " + mundsc + "/" + estuf + "<br /><br />";
				html += "</div>";

				// Verifica pontos em um mesmo lugar e move o seguinte para a direita
				if(lat_ant==markers[i].getAttribute("lat") && lng_ant==markers[i].getAttribute("lng"))
					var point = new GLatLng(markers[i].getAttribute("lat"),	markers[i].getAttribute("lng"));
				else
					var point = new GLatLng(markers[i].getAttribute("lat"),	parseFloat(markers[i].getAttribute("lng"))+0.0005);				
	
				lat_ant=markers[i].getAttribute("lat");
				lng_ant=markers[i].getAttribute("lng");
				
				// Cria o marcador na tela
				var marker = createMarker(point,title,icon,html);
				
				markerGroups['1'].push(marker);
				
				map.addOverlay(marker);
				
		}
		}else{
			alert("Não existem escolas!")
		}
	});

}

function criaBibliotecasPorEstado(estuf) {

	if(document.getElementById('municipioselecionado').value){
		muncod = document.getElementById('municipioselecionado').value;
		xml_filtro = "pdeescola.php?modulo=principal/mapaEntidade&acao=A&requisicao=bibliotecas&estuf=" + estuf + "&muncod=" + muncod;
	}else{
		xml_filtro = "pdeescola.php?modulo=principal/mapaEntidade&acao=A&requisicao=bibliotecas&estuf=" + estuf;
	}

	var baseIcon2 = new GIcon();
	baseIcon2.iconSize = new GSize(15, 17);
	baseIcon2.iconAnchor = new GPoint(11, 13);
	baseIcon2.infoWindowAnchor = new GPoint(9, 2);
	baseIcon2.image='/imagens/icones/ty.png';
		
	// Criando os Marcadores com o resultado
	GDownloadUrl(xml_filtro, function(data) {
		var xml = GXml.parse(data);
		
		var markers = xml.documentElement.getElementsByTagName("marker");
		
		if(markers.length > 0) {
			var lat_ant=0;
			var lng_ant=0;
				
			for (var i = 0; i < markers.length; i++) {
						
				var entnome = markers[i].getAttribute("entnome");
				var mundsc = markers[i].getAttribute("mundsc");
				var estuf = markers[i].getAttribute("estuf");
				var descricao = markers[i].getAttribute("descricao");
				var endereco = markers[i].getAttribute("endereco");
				var telefone = markers[i].getAttribute("telefone");
				title = entnome; 
										
				icon = baseIcon2;
				
				var lat = markers[i].getAttribute("lat");
				var lng = markers[i].getAttribute("lng");
				
				var html;
						
				html = "<div style=\"font-family:verdana;font-size:11px;padding:10px;width:300px;\" >";
				html += "<b>Ponto de Cultura:</b> " + entnome + "<br />";
				html += "<b>Descrição:</b> " + descricao + "<br />";
				html += "<b>Endereço:</b> " + endereco + "<br />";
				html += "<b>Localização:</b> " + mundsc + "/" + estuf + "<br /><br />";
				html += "</div>";

				// Verifica pontos em um mesmo lugar e move o seguinte para a direita
				if(lat_ant==markers[i].getAttribute("lat") && lng_ant==markers[i].getAttribute("lng"))
					var point = new GLatLng(markers[i].getAttribute("lat"),	markers[i].getAttribute("lng"));
				else
					var point = new GLatLng(markers[i].getAttribute("lat"),	parseFloat(markers[i].getAttribute("lng"))+0.0005);				
	
				lat_ant=markers[i].getAttribute("lat");
				lng_ant=markers[i].getAttribute("lng");
				
				// Cria o marcador na tela
				var marker = createMarker(point,title,icon,html);
				
				markerGroups['3'].push(marker);
				
				map.addOverlay(marker);
				
		}
		}else{
			alert("Não existem bibliotecas!")
		}
	});

}

function criaPontosCulturaisPorEstado(estuf){

	if(document.getElementById('municipioselecionado').value){
		muncod = document.getElementById('municipioselecionado').value;
		xml_filtro = "pdeescola.php?modulo=principal/mapaEntidade&acao=A&requisicao=pontosCulturais&estuf=" + estuf + "&muncod=" + muncod;
	}else{
		xml_filtro = "pdeescola.php?modulo=principal/mapaEntidade&acao=A&requisicao=pontosCulturais&estuf=" + estuf;
	}

	var baseIcon2 = new GIcon();
	baseIcon2.iconSize = new GSize(15, 17);
	baseIcon2.iconAnchor = new GPoint(11, 13);
	baseIcon2.infoWindowAnchor = new GPoint(9, 2);
	baseIcon2.image='/imagens/icones/tb.png';
		
	// Criando os Marcadores com o resultado
	GDownloadUrl(xml_filtro, function(data) {
		var xml = GXml.parse(data);
		
		var markers = xml.documentElement.getElementsByTagName("marker");
		
		if(markers.length > 0) {
			var lat_ant=0;
			var lng_ant=0;
				
			for (var i = 0; i < markers.length; i++) {
						
				var entnome = markers[i].getAttribute("entnome");
				var mundsc = markers[i].getAttribute("mundsc");
				var estuf = markers[i].getAttribute("estuf");
				var descricao = markers[i].getAttribute("descricao");
				var endereco = markers[i].getAttribute("endereco");
				var telefone = markers[i].getAttribute("telefone");
				title = entnome; 
										
				icon = baseIcon2;
				
				var lat = markers[i].getAttribute("lat");
				var lng = markers[i].getAttribute("lng");
				
				var html;
						
				html = "<div style=\"font-family:verdana;font-size:11px;padding:10px;width:300px;\" >";
				html += "<b>Ponto de Cultura:</b> " + entnome + "<br />";
				html += "<b>Descrição:</b> " + descricao + "<br />";
				html += "<b>Endereço:</b> " + endereco + "<br />";
				html += "<b>Localização:</b> " + mundsc + "/" + estuf + "<br /><br />";
				html += "</div>";

				// Verifica pontos em um mesmo lugar e move o seguinte para a direita
				if(lat_ant==markers[i].getAttribute("lat") && lng_ant==markers[i].getAttribute("lng"))
					var point = new GLatLng(markers[i].getAttribute("lat"),	markers[i].getAttribute("lng"));
				else
					var point = new GLatLng(markers[i].getAttribute("lat"),	parseFloat(markers[i].getAttribute("lng"))+0.0005);				
	
				lat_ant=markers[i].getAttribute("lat");
				lng_ant=markers[i].getAttribute("lng");
				
				// Cria o marcador na tela
				var marker = createMarker(point,title,icon,html);
				
				markerGroups['2'].push(marker);
				
				map.addOverlay(marker);
				
		}
		}
	});



}

function criaPontosDiversosPorEstado(estuf){

	if(document.getElementById('municipioselecionado').value){
		muncod = document.getElementById('municipioselecionado').value;
		xml_filtro = "pdeescola.php?modulo=principal/mapaEntidade&acao=A&requisicao=pontosDiversos&estuf=" + estuf + "&muncod=" + muncod;
	}else{
		xml_filtro = "pdeescola.php?modulo=principal/mapaEntidade&acao=A&requisicao=pontosDiversos&estuf=" + estuf;
	}

	var baseIcon2 = new GIcon();
	baseIcon2.iconSize = new GSize(15, 17);
	baseIcon2.iconAnchor = new GPoint(11, 13);
	baseIcon2.infoWindowAnchor = new GPoint(9, 2);
	baseIcon2.image='/imagens/icones/tp.png';
		
	// Criando os Marcadores com o resultado
	GDownloadUrl(xml_filtro, function(data) {
		var xml = GXml.parse(data);
		
		var markers = xml.documentElement.getElementsByTagName("marker");
		
		if(markers.length > 0) {
			var lat_ant=0;
			var lng_ant=0;
				
			for (var i = 0; i < markers.length; i++) {
						
				var entnome = markers[i].getAttribute("entnome");
				var mundsc = markers[i].getAttribute("mundsc");
				var estuf = markers[i].getAttribute("estuf");
				var descricao = markers[i].getAttribute("descricao");
				var endereco = markers[i].getAttribute("endereco");
				var telefone = markers[i].getAttribute("telefone");
				var tipo = markers[i].getAttribute("tipo");
				title = entnome; 
										
				icon = baseIcon2;
				
				var lat = markers[i].getAttribute("lat");
				var lng = markers[i].getAttribute("lng");
				
				var html;
						
				html = "<div style=\"font-family:verdana;font-size:11px;padding:10px;width:300px;\" >";
				html += "<b>Ponto de Cultura:</b> " + entnome + "<br />";
				html += "<b>Descrição:</b> " + descricao + "<br />";
				html += "<b>Endereço:</b> " + endereco + "<br />";
				html += "<b>Tipo:</b> " + tipo + "<br />";
				html += "<b>Localização:</b> " + mundsc + "/" + estuf + "<br /><br />";
				html += "</div>";

				// Verifica pontos em um mesmo lugar e move o seguinte para a direita
				if(lat_ant==markers[i].getAttribute("lat") && lng_ant==markers[i].getAttribute("lng"))
					var point = new GLatLng(markers[i].getAttribute("lat"),	markers[i].getAttribute("lng"));
				else
					var point = new GLatLng(markers[i].getAttribute("lat"),	parseFloat(markers[i].getAttribute("lng"))+0.0005);				
	
				lat_ant=markers[i].getAttribute("lat");
				lng_ant=markers[i].getAttribute("lng");
				
				// Cria o marcador na tela
				var marker = createMarker(point,title,icon,html);
				
				markerGroups['4'].push(marker);
				
				map.addOverlay(marker);
				
		}
		}
	});



}


</script>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>

</head>

<body onload="montaShowModal()">


<table width="100%" border="0" class="tabela" cellpadding="3" cellspacing="1">
<input type="hidden" name="estadoselecionado" id="estadoselecionado" value="<?php echo $_GET['estuf']; ?>" />
<input type="hidden" name="municipioselecionado" id="municipioselecionado" value="<?php echo $_GET['muncod']; ?>" />
	<tr
		<td class="SubTituloEsquerda" >Visualizar por Estado:</td>
		<td bgcolor="#F7F7F7" >
		<?php 		
		$sql = "select 
					estdescricao as descricao,
					estuf as codigo
				from
					territorios.estado
				ORDER BY
					descricao
				";
		$estuf = $_GET['estuf'];
		$db->monta_combo($estuf,$sql,"S","Selecione","carregaEscolaPorEstado","","","","","","",$_GET['estuf']);
		
		if( $_GET['estuf'] ){
		?> - por Município: <?php
			if( $_GET['estuf'] ){
				$sql = "select 
							mundescricao as descricao,
							muncod as codigo
						from
							territorios.municipio
						WHERE
							estuf = '".$_GET['estuf']."'
						ORDER BY
							descricao
						";
				$muncod = $_GET['muncod'];
				$db->monta_combo($muncod,$sql,"S","Selecione","carregaEscolaPorMunicipio","","","","","","",$_GET['muncod']);
			}
		}
		?>
		</td>
	</tr>
	<tr>
		<td colspan="2" align='right' class="SubTituloEsquerda" ><? echo $dados['entnome'] ?></td>
	</tr>
	<tr>
		<td bgcolor="#F7F7F7" valign="top" style="width:135px"  >
			<input type="checkbox" checked="checked" name="ponto1" onclick="exibePontos(this)" value="1" id="chk_ponto1" /> <img src="/imagens/icones/tr.png" /> Escola <br />
			<input type="checkbox" checked="checked" name="ponto2" onclick="exibePontos(this)" value="2" id="chk_ponto2" /> <img src="/imagens/icones/tb.png" /> Pontos de Cultura <br />
			<input type="checkbox" checked="checked" name="ponto3" onclick="exibePontos(this)" value="3" id="chk_ponto3" /> <img src="/imagens/icones/ty.png" /> Bibliotecas <br />
			<input type="checkbox" checked="checked" name="ponto4" onclick="exibePontos(this)" value="4" id="chk_ponto4" /> <img src="/imagens/icones/tp.png" /> Diversos
		</td>
		<td valign="top"><div style="height:550px;width:100%"  id="google_map"></div></td>
	</tr>
	<tr>
		<td bgcolor="#DCDCDC" colspan="2" align='center' ><input type="button" name="fechar" value="Fechar" onclick="window.close();" /></td>
	</tr>
</table>

<script>
	initialize();
	<?php if($_REQUEST['entid']): ?>
		criaPontoEscola();
		criaPontosCulturais("pdeescola.php?modulo=principal/mapaEntidade&acao=A&entidEscola=<? echo $_REQUEST['entid']?>");
		criaBibliotecas("pdeescola.php?modulo=principal/mapaEntidade&acao=A&entidEscolaBib=<? echo $_REQUEST['entid']?>");
		criaPontosDiversos("pdeescola.php?modulo=principal/mapaEntidade&acao=A&entidEscolaDiv=<? echo $_REQUEST['entid']?>");
	<?php endif; ?>
	<?php if($_REQUEST['estuf']): ?>
		criaEscolaPorEstado('<? echo $_REQUEST['estuf'] ?>');
		criaPontosCulturaisPorEstado('<? echo $_REQUEST['estuf'] ?>');
		criaBibliotecasPorEstado('<? echo $_REQUEST['estuf'] ?>');
		criaPontosDiversosPorEstado('<? echo $_REQUEST['estuf'] ?>');
	<?php endif; ?>
</script>
</body>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
</html>