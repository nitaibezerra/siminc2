<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<meta http-equiv="Cache-Control" content="no-cache">
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?

	$sql ="SELECT DISTINCT
    				mun.estuf,
                    mun.mundescricao,
    				 (  select metvalor from  projovemcampo.meta met where met.tpmid = 1 and met.apcid = c.apcid) as metvalor,
                    CASE WHEN c.apctermoaceito=TRUE THEN '1' ELSE '' END as adesaosim,
                    CASE WHEN c.apctermoaceito=FALSE THEN '1' ELSE '' END as adesaonao,
	    				CASE WHEN c.apcampliameta=TRUE THEN metas_sugeridas.metvalor::text ELSE '' END as metasugerida,
	                    CASE WHEN c.apcampliameta=TRUE THEN  metas_ajustadas.metvalor::text ELSE '' END as metaajustada,
	                CASE WHEN c.apctermoaceito=TRUE AND c.apctermoajustadoaceito=TRUE 
                             THEN metas_ajustadas.metvalor::varchar(19)
		 				  WHEN c.apctermoaceito=TRUE AND c.apcampliameta=FALSE
                             THEN metasoriginais.metvalor::varchar(19)
                         WHEN c.apctermoaceito=FALSE 
                            THEN metasoriginais.metvalor::varchar(19)
                            ELSE ''
                    END as metafinalizada
    				
                   		
            FROM territorios.municipio as mun
			INNER JOIN territorios.estado as e on e.estuf = mun.estuf
			INNER JOIN projovemcampo.adesaoprojovemcampo c ON c.apccodibge::character(7) = mun.muncod and apcesfera in ('M') 
    		
    	    LEFT JOIN projovemcampo.meta as metasoriginais on metasoriginais.apcid::varchar(10) = c.apcid::varchar(10) AND metasoriginais.tpmid in ( 1 )
			LEFT JOIN projovemcampo.meta as metas_sugeridas on metas_sugeridas.apcid::varchar(10) = c.apcid::varchar(10) AND metas_sugeridas.tpmid in ( 2 )
            LEFT JOIN projovemcampo.meta as metas_ajustadas on metas_ajustadas.apcid::varchar(10) =c.apcid::varchar(10) AND metas_ajustadas.tpmid in ( 3 )
           
    		
    		WHERE c.apctermoaceito = 't'  AND apcstatus='t'
			ORDER BY 
				mun.estuf, mun.mundescricao
			";
//			ver($sql);

echo "<table class=listagem width=100%><tr><td class=SubTituloCentro>ADESÃO - Lista de Municípios</td></tr></table>";

$sql_total = "SELECT 'Total de registros:', COUNT(*) FROM ( $sql ) foo";
$db->monta_lista_simples($sql_total,array(),500,5,'N','100%',$par2,'N',false);


$cabecalho = array("UF","MUNICÍPIO","META","ADESÃO - Sim","ADESÃO - Não","META - Sugerida","META - Análise","Adesão Finalizada");
$db->monta_lista_simples($sql,$cabecalho,500,5,'S','100%',$par2,'N',true);


	
	$sql =" SELECT DISTINCT
			e.estuf,
			e.estdescricao,
			 (  select metvalor from  projovemcampo.meta met where met.tpmid = 1 and met.apcid = c.apcid) as metvalor,
			 CASE WHEN c.apctermoaceito=TRUE THEN '1' ELSE '' END as adesaosim,
                    CASE WHEN c.apctermoaceito=FALSE THEN '1' ELSE '' END as adesaonao,
	    				CASE WHEN c.apcampliameta=TRUE THEN  metas_sugeridas.metvalor::text ELSE '' END as metasugerida,
	                    CASE WHEN c.apcampliameta=TRUE THEN  metas_ajustadas.metvalor::text ELSE '' END as metaajustada,
	                 CASE WHEN c.apctermoaceito=TRUE AND c.apctermoajustadoaceito=TRUE 
                             THEN metas_ajustadas.metvalor::varchar(19)
						  WHEN c.apctermoaceito=TRUE AND c.apcampliameta=FALSE
                             THEN metasoriginais.metvalor::varchar(19)
                          WHEN c.apctermoaceito=FALSE 
                            THEN metasoriginais.metvalor::varchar(19)
                            ELSE ''
                    END as metafinalizada
            		
            FROM territorios.estado e 
			inner join projovemcampo.adesaoprojovemcampo c on c.apccodibge::character(2) = e.estcod and apcesfera='E'  
    		LEFT JOIN projovemcampo.meta as metasoriginais on metasoriginais.apcid::varchar(10) = c.apcid::varchar(10) AND metasoriginais.tpmid in ( 1 )
			LEFT JOIN projovemcampo.meta as metas_sugeridas on metas_sugeridas.apcid::varchar(10) = c.apcid::varchar(10) AND metas_sugeridas.tpmid in ( 2 )
            LEFT JOIN projovemcampo.meta as metas_ajustadas on metas_ajustadas.apcid::varchar(10) =c.apcid::varchar(10) AND metas_ajustadas.tpmid in ( 3 )
           
	 		WHERE c.apctermoaceito = 't' AND apcstatus='t'
			ORDER BY 
				e.estuf, e.estdescricao
		";
//ver($sql);
echo "<table class=listagem width=100%><tr><td class=SubTituloCentro>ADESÃO - Lista de Estados</td></tr></table>";

$sql_total = "SELECT 'Total de registros:', COUNT(*) FROM ( $sql ) foo";
$db->monta_lista_simples($sql_total,array(),500,5,'N','100%',$par2,'N',false);


$cabecalho = array("UF","ESTADO","META","ADESÃO - Sim","ADESÃO - Não","META - Sugerida","META - Análise","Adesão Finalizada");
$db->monta_lista_simples($sql,$cabecalho,500,5,'S','100%',$par2,'N',true);


?>