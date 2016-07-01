<?
set_time_limit(0);
ini_set('memory_limit', '1024M');

?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<?php

	$sql = "SELECT DISTINCT
				e.entid,
				COALESCE('' || UPPER(e.entsig) ||  '' , 'Não informado') as nome,
				e.entunicod,
				campus.codigo,
				campus.descricao,
				campus.ibge,
				campus.municipio,
				campus.uf,
				campus.emec,
				campus.curso as curso,
				campus.turprev as turno,
				COALESCE(campus.prev2007,0) as prev2007, 
				COALESCE(campus.prev2008,0) as prev2008,
				COALESCE(campus.prev2009,0) as prev2009,
				COALESCE(campus.prev2010,0) as prev2010,
				COALESCE(campus.prev2011,0) as prev2011,
				COALESCE(campus.prev2012,0) as prev2012,
				COALESCE(campus.exec2007,0) as exec2007, 
				COALESCE(campus.exec2008,0) as exec2008,
				COALESCE(campus.exec2009,0) as exec2009,
				COALESCE(campus.exec2010,0) as exec2010,
				COALESCE(campus.exec2011,0) as exec2011,
				COALESCE(campus.exec2012,0) as exec2012
			FROM
				entidade.entidade e 
			INNER JOIN
				entidade.funcaoentidade ef ON ef.entid = e.entid
			LEFT JOIN 
				entidade.endereco ed ON e.entid = ed.entid
			LEFT JOIN
				( SELECT
						e.entid AS codigo,
						e.entnome AS descricao,
						mun.muncod AS ibge,
						mun.mundescricao AS municipio,
						mun.estuf AS uf,
						ea.entid AS instituicao,
						curd.cdtcodigoemec as emec,
						curd.cdtvgprevano2007 as prev2007,
						curd.cdtvgprevano2008 as prev2008,
						curd.cdtvgprevano2009 as prev2009,
						curd.cdtvgprevano2010 as prev2010,
						curd.cdtvgprevano2011 as prev2011,
						curd.cdtvgprevano2012 as prev2012,
						curd.cdtvgexecano2007 as exec2007,
						curd.cdtvgexecano2008 as exec2008,
						curd.cdtvgexecano2009 as exec2009,
						curd.cdtvgexecano2010 as exec2010,
						curd.cdtvgexecano2011 as exec2011,
						curd.cdtvgexecano2012 as exec2012,
						prog.pgcid as pgcid,
						prog.pgcdsc as pgcdsc,
						curso.curdsc as curso,
						curso.curid,
						te.turdsc AS turprev
					FROM
						entidade.entidade e
					INNER JOIN 
						entidade.funcaoentidade ef ON e.entid = ef.entid
					INNER JOIN 
						entidade.funentassoc ea ON ef.fueid = ea.fueid 
					LEFT JOIN
						academico.cursodetalhe curd ON curd.entid = e.entid
					LEFT JOIN 
						academico.turno te ON te.turid = curd.turidexecutado
					LEFT JOIN
						public.curso curso ON curd.curid = curso.curid
					LEFT JOIN
						academico.programacurso prog ON prog.pgcid = curd.pgcid
					LEFT JOIN 
						entidade.endereco ed ON e.entid = ed.entid
					LEFT JOIN
						territorios.municipio mun ON ed.muncod = mun.muncod
					WHERE 
						e.entstatus = 'A' 
						AND ef.funid = 18
					) as campus ON campus.instituicao = e.entid
			WHERE
				e.entstatus = 'A'
				AND ef.funid  in ('12')
			GROUP BY
				e.entid, e.entsig, e.entunicod, campus.codigo, e.entnome, campus.descricao, campus.ibge, campus.municipio, campus.uf, campus.emec, campus.prev2007, campus.prev2008, campus.prev2009,
				campus.prev2010, campus.prev2011, campus.prev2012, campus.pgcdsc, campus.curso, campus.turprev,campus.exec2007,
				campus.exec2008, campus.exec2009, campus.exec2010, campus.exec2011, campus.exec2012
			ORDER BY
				 nome";
	
	
	$dados  = $db->carregar($sql);

	ob_clean();
	$cab = array( "Código da Instituição", 
				  "Sigla da Instituição",
				  "Código da UO",
				  "Código do Campus",
				  "Campus",
				  "Código IBGE",
				  "Município",
				  "UF",
				  "Código do Curso(E-MEC)",
				  "Nome do Curso",
				  "Turno",
				 // "código da cidade(IBGE)",
				  "Vagas Previstas 2007",
				  "Vagas Previstas 2008",
				  "Vagas Previstas 2009",
				  "Vagas Previstas 2010",
				  "Vagas Previstas 2011",
				  "Vagas Previstas 2012",
				  "Vagas Executadas 2007",
				  "Vagas Executadas 2008",
				  "Vagas Executadas 2009",
				  "Vagas Executadas 2010",
				  "Vagas Executadas 2011",
				  "Vagas Executadas 2012"
			);
	$db->sql_to_excel($sql,"Relatório",$cab);
	exit;
?>
</body>