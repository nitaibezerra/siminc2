<?php 
header('content-type: text/html; charset=ISO-8859-1');

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();


	if( $_REQUEST['buscaTextual'] ){
		$filtro[] = "UPPER(ref.rfsnome) like UPPER('%".$_REQUEST['buscaTextual']."%')";
	}
	if( $_REQUEST['uf'] ){
		$filtro[] = "ref.estuf in (".str_replace("\'","'",$_REQUEST['uf']).") ";
	}
	
	if($_REQUEST['chk']) {
		if(in_array("1", $_REQUEST['chk'])) $classif[] = "1 - Preexistentes";			
		if(in_array("2", $_REQUEST['chk']))	$classif[] = "2 - Criadas (2003/2010)";			
		if(in_array("3", $_REQUEST['chk']))	$classif[] = "3 - Propostas (2013/2014)";
		
		if($classif) $filtro[] = "ref.rfstipo IN ('".implode("','",$classif)."') ";
	}
	

	$sql = "SELECT
				ref.rfsnome as nome,
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
				ref.estuf,
				e.entid,
				ref.rfsid,
				ref.rfstipo,
				ref.rfscaracteristica, 
				CASE 
				     WHEN ref.rfstipo='1 - Preexistentes' THEN '1'
				     WHEN ref.rfstipo='2 - Criadas (2003/2010)' THEN '2'
				     WHEN ref.rfstipo='3 - Propostas (2013/2014)' THEN '3'
				END as tipo
				
		FROM 
			academico.redefederalsuperior2 ref
		LEFT JOIN
			entidade.entidade e ON e.entid = ref.entid 
		LEFT JOIN 
			academico.campus cam ON cam.entid = e.entid 
		LEFT JOIN 
			entidade.endereco ed ON e.entid = ed.entid
		LEFT JOIN 
			territorios.municipio mun ON ref.muncod = mun.muncod::character varying(255)
		".(($filtro)?"WHERE ".implode(" AND ", $filtro):"")."
		ORDER BY
			 ref.rfstipo, e.entsig, e.entnome";
	
	$dados = $db->carregar($sql);
	
	if($dados):
		
		$conteudo .= "<markers> "; // inicia o XML
		
		foreach($dados as $d):
										
			$conteudo .= "<marker "; //inicia um ponto no mapa
			$conteudo .= "nome=\"". $d['nome'] ."\" "; // adiciona o nome da instituição;
			$conteudo .= "entid=\"". $d['entid'] ."\" "; // adiciona o nome da instituição;
			$conteudo .= "rfsid=\"". $d['rfsid'] ."\" "; // adiciona o nome da instituição;
			$conteudo .= "tipo=\"". $d['tipo'] ."\" "; // adiciona o tipo;
			$conteudo .= "mundsc=\"". $d['mundescricao'] ."\" "; // adiciona a descrição do município;
			$conteudo .= "estuf=\"". $d['estuf'] ."\" "; // adiciona UF;
			$conteudo .= "rfscaracteristica=\"". $d['rfscaracteristica'] ."\" "; // adiciona UF;
			$conteudo .= "lat='{$d['latitude']}' "; // adiciona a latitude;
			$conteudo .= "lng='{$d['longitude']}' "; //adiciona a longitude;
			$conteudo .= "/> ";
		
		endforeach;
		
		$conteudo .= "</markers> ";
		print $conteudo;
		
	endif;
	
?>