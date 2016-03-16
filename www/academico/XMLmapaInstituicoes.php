<?php 
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

switch ( $_REQUEST['orgid'] ){
			
			case '1':
				$filtro[] = " ef.funid in ('" . ACA_ID_UNIVERSIDADE . "')";
				$filtro2[] = " ef.funid in ('" . ACA_ID_CAMPUS . "')";
			break;
			
			case '2':
				$filtro[] = " ef.funid in ('" . ACA_ID_ESCOLAS_TECNICAS . "')";
				$filtro2[] = " ef.funid in ('" . ACA_ID_UNED . "')";
			break;
			case '1,2':
				$filtro[] = " ef.funid in ('" . ACA_ID_UNIVERSIDADE . "' , '" . ACA_ID_ESCOLAS_TECNICAS . "')";
				$filtro2[] = " ef.funid in ('" . ACA_ID_CAMPUS . "' , '" . ACA_ID_UNED . "')";
			break;
		}
	
	if( $_REQUEST['buscaTextual'] ){
		$filtro[] = "(UPPER(e.entnome) like UPPER('%".$_REQUEST['buscaTextual']."%') OR UPPER(e.entsig) like UPPER('%".$_REQUEST['buscaTextual']."%') )";
		$filtro2[] = "(UPPER(e.entnome) like UPPER('%".$_REQUEST['buscaTextual']."%') OR UPPER(e.entsig) like UPPER('%".$_REQUEST['buscaTextual']."%') )";
	}
	if( $_REQUEST['uf'] ){
		$filtro[] = "ed.estuf in (".str_replace("\'","'",$_REQUEST['uf']).") ";
		$filtro2[] = "ed.estuf in (".str_replace("\'","'",$_REQUEST['uf']).") ";
	}

	$sql = "
	(
	SELECT
			CASE 
				WHEN entsig <> '' THEN UPPER(entsig) ||  ' - ' || UPPER(entnome)
				ELSE UPPER(entnome) 
			END as nome,
			CASE 
				WHEN ef.funid = ".ACA_ID_UNIVERSIDADE." THEN 'Ensino Superior'
				WHEN ef.funid = ".ACA_ID_ESCOLAS_TECNICAS." THEN 'Ensino Profissional'
				ELSE 'Não Informado'
			END as tipoensino,
			CASE 
				WHEN ef.funid = ".ACA_ID_UNIVERSIDADE." THEN '11'
				WHEN ef.funid = ".ACA_ID_ESCOLAS_TECNICAS." THEN '21'
				ELSE '0'
			END as tipo,
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
			ef.funid,
			e.entid,
			'Instituição' as situacao,
			'Instituição' as subdivisao
		FROM
			entidade.entidade e 
		INNER JOIN
			entidade.funcaoentidade ef ON ef.entid = e.entid
		LEFT JOIN 
			entidade.endereco ed ON e.entid = ed.entid
		LEFT JOIN 
			territorios.municipio mun ON ed.muncod = mun.muncod
		WHERE
			e.entstatus = 'A' ". ( count($filtro) > 0 ? " AND ".implode(" AND ", $filtro) : "" ) . "
		--GROUP BY e.entid, e.entnome,  e.entsig , ef.funid
		ORDER BY
			 e.entsig, e.entnome
	)
	UNION
	(
		 SELECT
				CASE 
					WHEN e.entsig <> '' THEN UPPER(e2.entsig) ||  ' - ' || UPPER(e.entnome)
					ELSE UPPER(e.entnome) 
				END as nome,
				CASE 
					WHEN ef.funid = ".ACA_ID_CAMPUS." THEN 'Campus - Ensino Superior'
					WHEN ef.funid = ".ACA_ID_UNED." THEN 'Campus - Ensino Profissional'
					ELSE 'Não Informado'
				END as tipoensino,
				CASE 
					WHEN ef.funid = ".ACA_ID_CAMPUS." THEN '12'
					WHEN ef.funid = ".ACA_ID_UNED." THEN '22'
					ELSE '0'
				END as tipo,
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
				WHEN (medlongitude is not null AND medlongitude <> '...W') THEN 
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
			tm.mundescricao,
			ed.estuf,
			ef.funid,
			e.entid,
			CASE 
				WHEN cmpsituacao = 'F' THEN 'Funcionando'
				WHEN cmpsituacao = 'N' THEN 'Não Funcionando'
				ELSE 'Não Informado'
			END as situacao,
			ex.exidsc as subdivisao
		FROM
			entidade.entidade e
		INNER JOIN
			entidade.entidade e2 ON e2.entid = e.entid
		INNER JOIN
			entidade.endereco ed ON ed.entid = e.entid
		LEFT JOIN
			territorios.municipio tm ON tm.muncod = ed.muncod
		INNER JOIN
			entidade.funcaoentidade ef ON ef.entid = e2.entid
		INNER JOIN
			entidade.funentassoc ea ON ea.fueid = ef.fueid
		LEFT JOIN
			academico.campus ac ON e.entid = ac.entid
		LEFT JOIN
			academico.existencia ex ON ex.exiid = ac.exiid
			
		WHERE
			ea.entid IN ( 
			
						SELECT
							distinct e.entid
						FROM
							entidade.entidade e 
						INNER JOIN
							entidade.funcaoentidade ef ON ef.entid = e.entid
						LEFT JOIN 
							entidade.endereco ed ON e.entid = ed.entid
						LEFT JOIN 
							territorios.municipio mun ON ed.muncod = mun.muncod
						WHERE
							e.entstatus = 'A' ". ( count($filtro) > 0 ? " AND ".implode(" AND ", $filtro) : "" ) . "
						)
		AND
			e.entstatus = 'A' ". ( count($filtro2) > 0 ? " AND ".implode(" AND ", $filtro2) : "" ) . "
		--GROUP BY e.entid, e.entnome,  e.entsig , ef.funid
		ORDER BY
		 e2.entsig, e2.entnome
	)";
	
	$dados = $db->carregar($sql);
	
	ob_clean();
	header('content-type: text/xml; charset=ISO-8859-1');
	
	if($dados):
		
		$conteudo .= "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?><markers>"; // inicia o XML
		
		foreach($dados as $d):
										
			$conteudo .= "<marker "; //inicia um ponto no mapa
			$conteudo .= "instituicao=\"". $d['nome'] ."\" "; // adiciona o nome da instituição;
			$conteudo .= "entid=\"". $d['entid'] ."\" "; // adiciona o nome da instituição;
			$conteudo .= "tipoensino=\"". $d['tipoensino'] ."\" "; // adiciona o tipo de ensino;
			$conteudo .= "tipo=\"". $d['tipo'] ."\" "; // adiciona o tipo;
			$conteudo .= "mundsc=\"". $d['mundescricao'] ."\" "; // adiciona a descrição do município;
			$conteudo .= "estuf=\"". $d['estuf'] ."\" "; // adiciona UF;
			$conteudo .= "lat='{$d['latitude']}' "; // adiciona a latitude;
			$conteudo .= "lng='{$d['longitude']}' "; //adiciona a longitude;
			$conteudo .= "subdivisao='{$d['subdivisao']}' "; //adiciona a subdivisao;
			$conteudo .= "situacao='{$d['situacao']}' "; //adiciona a situação;
			$conteudo .= "/> ";
		
		endforeach;
		
		$conteudo .= "</markers> ";
		print $conteudo;
		
	endif;
	
//	dbg($sql);
//	dbg($dados);
//	dbg(count($dados));

?>