<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<meta http-equiv="Cache-Control" content="no-cache">

<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?

if($_SESSION['projovemurbano']['ppuid'] == 1){

	$sql = "SELECT DISTINCT
				mun.estuf,
				mun.mundescricao,
				cgm.cmemeta,
				CASE WHEN pro.adesaotermo=TRUE THEN '1' ELSE '' END as adesaosim,
				CASE WHEN pro.adesaotermo=FALSE THEN '1' ELSE '' END as adesaonao,
				CASE WHEN sta.suaverdade=TRUE THEN sta.suametasugerida::text ELSE '' END as metasugerida,
				CASE WHEN sta.suaverdade=TRUE THEN sta.suametaajustada::text ELSE '' END as metaajustada,
				--A pedido do Julio 22/05/2013
				--CASE WHEN sta.suaverdade=TRUE AND pro.adesaotermoajustado=TRUE 
		        --                THEN sta.suametaajustada::text 
		        --             WHEN sta.suaverdade=FALSE
		        CASE WHEN pro.adesaotermo=TRUE 
	            	THEN coalesce(sta.suametaajustada::text, cgm.cmemeta::text) 
	            	ELSE ''
				END as metafinalizada
	        FROM 
	        	projovemurbano.projovemurbano pro 
	        INNER JOIN territorios.municipio mun ON mun.muncod = pro.muncod 
	        INNER JOIN projovemurbano.cargameta cgm ON cgm.cmecodibge::character(7) = mun.muncod AND cgm.ppuid = ".$_SESSION['projovemurbano']['ppuid']."
	        LEFT  JOIN projovemurbano.sugestaoampliacao sta ON sta.pjuid = pro.pjuid
	        WHERE 
	        	pro.adesaotermo IS NOT NULL 
	        	AND pro.muncod IS NOT NULL 
	        	AND pro.ppuid = ".$_SESSION['projovemurbano']['ppuid']."
	        ORDER BY 
	        	mun.estuf, 
	        	mun.mundescricao";
//	ver($sql);
}elseif($_SESSION['projovemurbano']['ppuid'] == 2){
	$sql ="SELECT DISTINCT 
			mun.estuf,
			mun.mundescricao,
			cgm.cmemeta,
			CASE WHEN pro.adesaotermo=TRUE THEN '1' ELSE '' END as adesaosim,
			CASE WHEN pro.adesaotermo=FALSE THEN '1' ELSE '' END as adesaonao,
			CASE WHEN pro.adesaotermo=TRUE 
				THEN (SELECT SUM(mtpvalor)
					FROM
						projovemurbano.metasdoprograma mtp
					WHERE
						tpmid in (8,11,14) AND mtp.pjuid = pro.pjuid)::text ELSE '' END as metasugerida,
						
			CASE WHEN pro.adesaotermo=TRUE 
				THEN (SELECT SUM(mtpvalor)
					FROM
						projovemurbano.metasdoprograma mtp
					WHERE					
						tpmid in (9,12,15) AND mtp.pjuid = pro.pjuid)::text
				ELSE '' 
			END as metaajustada,
						
			CASE WHEN pro.adesaotermoajustado=TRUE
				THEN ( SELECT SUM(mtpvalor)
					FROM
						projovemurbano.metasdoprograma mtp
					WHERE					
						tpmid in (9,12,15) AND mtp.pjuid = pro.pjuid)::text 
					ELSE cgm.cmemeta::text
					END as metafinalizada
			FROM 
				projovemurbano.projovemurbano pro 
			INNER JOIN territorios.municipio mun ON mun.muncod = pro.muncod
			INNER JOIN projovemurbano.metasdoprograma mtp ON mtp.pjuid = pro.pjuid
			INNER JOIN projovemurbano.cargameta cgm ON cgm.cmecodibge::character(7) = mun.muncod AND cgm.ppuid = 2
			LEFT  JOIN projovemurbano.sugestaoampliacao sta ON sta.pjuid = pro.pjuid
			WHERE 
				pro.adesaotermo IS NOT NULL 
				AND mun.estuf IS NOT NULL 
				AND pro.ppuid = 2
			ORDER BY 
				mun.estuf, 
			mun.mundescricao";
//			ver($sql);
}elseif($_SESSION['projovemurbano']['ppuid'] == 3){
	

	$sql ="SELECT DISTINCT 
			mun.estuf,
			mun.mundescricao,
			(cgm.geral + cgm.juventude + cgm.prisional) as cmemeta,
			CASE WHEN pro.adesaotermo=TRUE THEN '1' ELSE '' END as adesaosim,
			CASE WHEN pro.adesaotermo=FALSE THEN '1' ELSE '' END as adesaonao,
			CASE WHEN sta.suaverdade = TRUE 
				THEN (SELECT SUM(mtpvalor)
					FROM
						projovemurbano.metasdoprograma mtp
					WHERE
						tpmid in (8,11,14) AND mtp.pjuid = pro.pjuid)::text ELSE '' END as metasugerida,
						
			CASE WHEN pro.adesaotermo=TRUE 
				THEN (SELECT SUM(mtpvalor)
					FROM
						projovemurbano.metasdoprograma mtp
					WHERE					
						tpmid in (9,12,15) AND mtp.pjuid = pro.pjuid)::text
				ELSE '' 
			END as metaajustada,
						
			CASE WHEN sta.suaverdade=TRUE AND pro.adesaotermoajustado=TRUE 
                             THEN metas_ajustadas.mtpvalor::varchar(19)
                         WHEN pro.adesaotermo=TRUE
                            THEN (cgm.geral + cgm.juventude + cgm.prisional)::text
                            ELSE ''
                    END as metafinalizada
			FROM 
				projovemurbano.projovemurbano pro 
			INNER JOIN territorios.municipio mun ON mun.muncod = pro.muncod
			INNER JOIN projovemurbano.metasdoprograma mtp ON mtp.pjuid = pro.pjuid
			INNER JOIN projovemurbano.cargameta cgm ON cgm.cmecodibge::character(7) = mun.muncod AND cgm.ppuid = {$_SESSION['projovemurbano']['ppuid']}
			LEFT  JOIN projovemurbano.sugestaoampliacao sta ON sta.pjuid = pro.pjuid
			LEFT JOIN projovemurbano.metasdoprograma as metasoriginais on metasoriginais.pjuid::varchar(10) = sta.pjuid::varchar(10) AND metasoriginais.tpmid in ( 7, 13 )
            LEFT JOIN projovemurbano.metasdoprograma as metas_ajustadas on metas_ajustadas.pjuid::varchar(10) = sta.pjuid::varchar(10) AND metas_ajustadas.tpmid in ( 9, 15 )
			WHERE 
				pro.adesaotermo IS NOT NULL 
				AND mun.estuf IS NOT NULL 
				AND pro.ppuid = {$_SESSION['projovemurbano']['ppuid']}
				AND pro.pjustatus = 'A'
			ORDER BY 
				mun.estuf, 
			mun.mundescricao";
// 			ver($sql);
}
echo "<table class=listagem width=100%><tr><td class=SubTituloCentro>ADESÃO - Lista de Municípios</td></tr></table>";

$sql_total = "SELECT 'Total de registros:', COUNT(*) FROM ( $sql ) foo";
$db->monta_lista_simples($sql_total,array(),500,5,'N','100%',$par2,'N',false);


$cabecalho = array("UF","MUNICÍPIO","META","ADESÃO - Sim","ADESÃO - Não","META - Sugerida","META - Análise","Adesão Finalizada");
$db->monta_lista_simples($sql,$cabecalho,500,5,'S','100%',$par2,'N',true);

if($_SESSION['projovemurbano']['ppuid'] == 1){
	$sql = "SELECT  
				est.estuf,
				est.estdescricao,
				cgm.cmemeta,
				CASE WHEN pro.adesaotermo=TRUE THEN '1' ELSE '' END as adesaosim,
				CASE WHEN pro.adesaotermo=FALSE THEN '1' ELSE '' END as adesaonao,
				CASE WHEN sta.suaverdade=TRUE THEN sta.suametasugerida::text ELSE '' END as metasugerida,
				CASE WHEN sta.suaverdade=TRUE THEN sta.suametaajustada::text ELSE '' END as metaajustada,
				CASE WHEN pro.adesaotermo=TRUE 
		                        THEN coalesce(sta.suametaajustada::text, cgm.cmemeta::text) 
		                        ELSE ''
				END as metafinalizada
	        FROM 
	        	projovemurbano.projovemurbano pro 
	        LEFT  JOIN territorios.estado est ON est.estuf = pro.estuf 
	        INNER JOIN projovemurbano.cargameta cgm ON cgm.cmecodibge::character(7) = est.estcod AND cgm.ppuid = ".$_SESSION['projovemurbano']['ppuid']."
	        LEFT  JOIN projovemurbano.sugestaoampliacao sta ON sta.pjuid = pro.pjuid
	        WHERE 
	        	pro.adesaotermo IS NOT NULL 
	        	AND pro.estuf IS NOT NULL 
	        	AND pro.ppuid = ".$_SESSION['projovemurbano']['ppuid']."
	        ORDER BY 
				est.estuf";
}elseif($_SESSION['projovemurbano']['ppuid'] == 2){
	
	$sql ="SELECT DISTINCT 
			est.estuf,
			est.estdescricao,
			cgm.cmemeta,
			CASE WHEN pro.adesaotermo=TRUE THEN '1' ELSE '' END as adesaosim,
			CASE WHEN pro.adesaotermo=FALSE THEN '1' ELSE '' END as adesaonao,
			CASE WHEN pro.adesaotermo=TRUE 
				THEN (SELECT SUM(mtpvalor)
					FROM
						projovemurbano.metasdoprograma mtp
					WHERE
						tpmid in (8,11,14) AND mtp.pjuid = pro.pjuid)::text 
					ELSE '' END as metasugerida,
						
			CASE WHEN pro.adesaotermo=TRUE 
				THEN (SELECT SUM(mtpvalor)
					FROM
						projovemurbano.metasdoprograma mtp
					WHERE					
						tpmid in (9,12,15) AND mtp.pjuid = pro.pjuid)::text
					ELSE '' 
			END as metaajustada,
						
			CASE WHEN pro.adesaotermoajustado=TRUE
				THEN ( SELECT SUM(mtpvalor)
					FROM
						projovemurbano.metasdoprograma mtp
					WHERE					
						tpmid in (9,12,15) AND mtp.pjuid = pro.pjuid)::text 
				ELSE cgm.cmemeta::text END as metafinalizada
			FROM 
				projovemurbano.projovemurbano pro 
			LEFT  JOIN territorios.estado est ON est.estuf = pro.estuf 
			INNER JOIN projovemurbano.metasdoprograma mtp ON mtp.pjuid = pro.pjuid
			INNER JOIN projovemurbano.cargameta cgm ON cgm.cmecodibge::character(7) = est.estcod AND cgm.ppuid = ".$_SESSION['projovemurbano']['ppuid']."
			LEFT  JOIN projovemurbano.sugestaoampliacao sta ON sta.pjuid = pro.pjuid
			WHERE 
				pro.adesaotermo IS NOT NULL 
				AND pro.estuf IS NOT NULL 
				AND pro.ppuid = ".$_SESSION['projovemurbano']['ppuid']."
			ORDER BY 
				est.estuf";
}elseif($_SESSION['projovemurbano']['ppuid'] == 3){
	
	
	$sql ="SELECT DISTINCT
			est.estuf,
			est.estdescricao,
			(cgm.geral + cgm.juventude + cgm.prisional) as cmemeta,
			CASE WHEN pro.adesaotermo=TRUE THEN '1' ELSE '' END as adesaosim,
			CASE WHEN pro.adesaotermo=FALSE THEN '1' ELSE '' END as adesaonao,
			CASE WHEN sta.suaverdade = TRUE
				THEN (SELECT SUM(mtpvalor)
					FROM
						projovemurbano.metasdoprograma mtp
					WHERE
						tpmid in (8,11,14) AND mtp.pjuid = pro.pjuid)::text
					ELSE '' END as metasugerida,
	
			CASE WHEN pro.adesaotermo=TRUE
				THEN (SELECT SUM(mtpvalor)
					FROM
						projovemurbano.metasdoprograma mtp
					WHERE
						tpmid in (9,12,15) AND mtp.pjuid = pro.pjuid)::text
					ELSE ''
			END as metaajustada,
	
			CASE WHEN sta.suaverdade=TRUE AND pro.adesaotermoajustado=TRUE 
                             THEN (SELECT SUM(mtpvalor)
								FROM
									projovemurbano.metasdoprograma mtp
								WHERE
									tpmid in (9,12,15) AND mtp.pjuid = pro.pjuid)::text
                			WHEN pro.adesaotermo=TRUE
                            THEN (cgm.geral + cgm.juventude + cgm.prisional)::text
                            ELSE ''
            END as metafinalizada
			FROM
				projovemurbano.projovemurbano pro
			LEFT  JOIN territorios.estado est ON est.estuf = pro.estuf
			INNER JOIN projovemurbano.metasdoprograma mtp ON mtp.pjuid = pro.pjuid
			INNER JOIN projovemurbano.cargameta cgm ON cgm.cmecodibge::character(7) = est.estcod AND cgm.ppuid = ".$_SESSION['projovemurbano']['ppuid']."
			LEFT  JOIN projovemurbano.sugestaoampliacao sta ON sta.pjuid = pro.pjuid
            LEFT JOIN projovemurbano.metasdoprograma as metasoriginais on metasoriginais.pjuid::varchar(10) = sta.pjuid::varchar(10) AND metasoriginais.tpmid in ( 7, 13 )
            LEFT JOIN projovemurbano.metasdoprograma as metas_ajustadas on metas_ajustadas.pjuid::varchar(10) = sta.pjuid::varchar(10) AND metas_ajustadas.tpmid in ( 9, 15 )
			WHERE
				pro.adesaotermo IS NOT NULL
				AND pro.estuf IS NOT NULL
				AND pro.ppuid = ".$_SESSION['projovemurbano']['ppuid']."
            		AND pro.pjustatus = 'A'
			ORDER BY
				est.estuf";
}
// ver($sql);
echo "<table class=listagem width=100%><tr><td class=SubTituloCentro>ADESÃO - Lista de Estados</td></tr></table>";

$sql_total = "SELECT 'Total de registros:', COUNT(*) FROM ( $sql ) foo";
$db->monta_lista_simples($sql_total,array(),500,5,'N','100%',$par2,'N',false);


$cabecalho = array("UF","ESTADO","META","ADESÃO - Sim","ADESÃO - Não","META - Sugerida","META - Análise","Adesão Finalizada");
$db->monta_lista_simples($sql,$cabecalho,500,5,'S','100%',$par2,'N',true);


?>