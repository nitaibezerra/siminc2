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
	
if( $_REQUEST['buscaTextual'] ){
	$filtro[] = "UPPER(entcampus.entnome) like UPPER('%".$_REQUEST['buscaTextual']."%')";
}
if( $_REQUEST['estuf'][0] ){
	$filtro[] = "est.estuf in ('".implode("','",$_REQUEST['estuf'])."') ";
}
if( $_REQUEST['muncod'][0] ){
	$filtro[] = "mun.muncod in ('".implode("','",$_REQUEST['muncod'])."') ";
}

if($_REQUEST['chk_profissional']) {
	if(in_array("1", $_REQUEST['chk_profissional'])) $classif[] = "Preexistentes";			
	if(in_array("2", $_REQUEST['chk_profissional']))	$classif[] = "Criados em 2003/2010";			
	if(in_array("3", $_REQUEST['chk_profissional']))	$classif[] = "Novos 2011/2012";
	if(in_array("4", $_REQUEST['chk_profissional']))	$classif[] = "Novos 2013/2014";
	
	if($classif) $filtro[] = "exi.exidsc IN ('".implode("','",$classif)."') ";
}

$sql = "select 
entcampus.entnome as nome, 
ST_Y(munlatlong) as latitude,
ST_X(munlatlong) as longitude,
mun.mundescricao,
mun.muncod,
trim(est.estuf) as estuf,
entcampus.entid,
exi.exidsc,
edt.edtdsc, 
CASE
     WHEN exi.exidsc='Preexistentes' THEN  '1'
     WHEN exi.exidsc='Criados em 2003/2010' THEN   '2'
     WHEN exi.exidsc='Novos 2011/2012' THEN '3'
     WHEN exi.exidsc='Novos 2013/2014' THEN '4'
END as tipo
from entidade.entidade entcampus 
inner join entidade.endereco edc ON edc.entid = entcampus.entid 
LEFT JOIN territoriosgeo.municipio mun ON edc.muncod = mun.muncod
LEFT JOIN territorios.estado est ON est.estuf = mun.estuf 
LEFT JOIN territorios.regiao reg ON reg.regcod = est.regcod 
inner join entidade.funcaoentidade fn on entcampus.entid = fn.entid and fn.funid = 17 and entcampus.entstatus = 'A' and fn.fuestatus = 'A'
inner join entidade.funentassoc a on a.fueid = fn.fueid
inner join entidade.entidade entinst on entinst.entid = a.entid 
inner join entidade.funcaoentidade fn2 on entinst.entid = fn2.entid and fn2.funid = 11 and entinst.entstatus = 'A' and fn2.fuestatus = 'A'
inner join academico.campus cmp on cmp.entid = entcampus.entid
inner join academico.existencia exi on exi.exiid = cmp.exiid
left join academico.entidadedetalhe edt on edt.entid = entcampus.entid 
".(($filtro)?"WHERE ".implode(" AND ",$filtro):"")." ORDER BY exi.exidsc, entinst.entsig, entinst.entnome";

/*
$sql = "SELECT
			ref.rfecampus as nome,
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
			ref.rfeuf,
			e.entid,
			ref.rfeid,
			ref.rfeclassificacao,
			CASE
			     WHEN ref.rfeclassificacao='1 - Câmpus Preexistentes' THEN '1'
			     WHEN ref.rfeclassificacao='2 - Criadas (2003/2010)' THEN '2'
			     WHEN ref.rfeclassificacao='3 - Previstos (2011/2012)' THEN '3'
			     WHEN ref.rfeclassificacao='4 - Propostos (2013/2014)' THEN '4'
			END as tipo,
			ref.rfecaracteristica,
			mun.muncod
			
	FROM 
		academico.redefederal ref
	LEFT JOIN
		entidade.entidade e ON e.entid::character varying(255) = ref.rfecodigocampus
	LEFT JOIN 
		entidade.endereco ed ON e.entid = ed.entid
	LEFT JOIN 
		territorios.municipio mun ON ref.rfeibge = mun.muncod::character varying(255)
	".(($filtro)?"WHERE ".implode(" AND ", $filtro):"")."
	ORDER BY
		 ref.rfeclassificacao, e.entsig, e.entnome";
*/

$dados = $db->carregar($sql);

ob_clean();
header('content-type: text/xml; charset=ISO-8859-1');

if($dados):
	
	$conteudo .= "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?><markers>"; // inicia o XML
	
	foreach($dados as $d):
									
		$conteudo .= "<marker "; //inicia um ponto no mapa
		$conteudo .= "rfecaracteristica=\"". addslashes(str_replace(array("\"","'","&"),"",$d['edtdsc'])) ."\" "; // adiciona o nome da instituição;
		$conteudo .= "nome=\"". $d['nome'] ."\" "; // adiciona o nome da instituição;
		$conteudo .= "entid=\"". $d['entid'] ."\" "; // adiciona o nome da instituição;
		$conteudo .= "tipo=\"". $d['tipo'] ."\" "; // adiciona o tipo;
		$conteudo .= "muncod=\"". $d['muncod'] ."\" "; // adiciona a descrição do município;
		$conteudo .= "mundsc=\"". $d['mundescricao'] ."\" "; // adiciona a descrição do município;
		$conteudo .= "estuf=\"". $d['estuf'] ."\" "; // adiciona UF;
		$conteudo .= "lat='{$d['latitude']}' "; // adiciona a latitude;
		$conteudo .= "lng='{$d['longitude']}' "; //adiciona a longitude;
		$conteudo .= "/> ";
	
	endforeach;
	
	$conteudo .= "</markers> ";
	print $conteudo;
	
endif;
	
?>