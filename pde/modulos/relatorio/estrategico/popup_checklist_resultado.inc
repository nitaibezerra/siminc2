<?php
function checklist_monta_coluna_relatorio(){
	
	$coluna = array();

	array_push( $coluna, array("campo" 	  => "valor_meta",
					   		   "label" 	  => "Valor da Meta",
					   		   "blockAgp" => "") );
	
	array_push( $coluna, array("campo" 	  => "qtde_meta",
					   		   "label" 	  => "Quantidade da Meta",
					   		   "blockAgp" => "") );
	
	array_push( $coluna, array("campo" 	  => "ultima_atualizacao_meta",
					   		   "label" 	  => "Data da Ultima Atualização",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );
	
	array_push( $coluna, array("campo" 	  => "data_execucao_meta",
					   		   "label" 	  => "Data para Execução",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );
	
	array_push( $coluna, array("campo" 	  => "validata_validacao_metadacao",
					   		   "label" 	  => "Data para Validação",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );
	
	array_push( $coluna, array("campo" 	  => "data_meta",
					   		   "label" 	  => "Data Dinal da Meta",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );
	
	array_push( $coluna, array("campo" 	  => "periodicidade_meta",
					   		   "label" 	  => "Periodicidade da Meta",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );
	
	array_push( $coluna, array("campo" 	  => "valor_execucao",
					   		   "label" 	  => "Valor Executado",
					   		   "blockAgp" => "") );
	
	array_push( $coluna, array("campo" 	  => "qtde_execucao",
					   		   "label" 	  => "Quantidade Executada",
					   		   "blockAgp" => "") );
	
	array_push( $coluna, array("campo" 	  => "estado",
					   		   "label" 	  => "Estado",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );
	
	return $coluna;
	
}


function checklist_monta_agp_relatorio(){
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array("valor_meta","qtde_meta","ultima_atualizacao_meta","data_execucao_meta",
										  "data_validacao_meta","data_meta","periodicidade_meta","valor_execucao","qtde_execucao","estado")
				);

	foreach( $_REQUEST['agrupadores'] as $agrupador ){
		switch( $agrupador ){
			
			case 'projeto':
				array_push( $agp['agrupador'], array("campo" 	  => "projeto",
								   		   			 "label" 	  => "Projeto") );
			break;
			
			case 'atividade1':
				array_push( $agp['agrupador'], array("campo" 	  => "atividade1",
								   		   			 "label" 	  => "Atividade 1° nivel") );
				array_push( $agp['agrupador'], array("campo" 	  => "atividade2",
								   		   			 "label" 	  => "Atividade 2° nivel") );
				array_push( $agp['agrupador'], array("campo" 	  => "atividade3",
								   		   			 "label" 	  => "Atividade 3° nivel") );
			break;
			
			case 'item':
				array_push( $agp['agrupador'], array("campo" 	  => "item",
								   		   		     "label" 	  => "Item de Checklist") );
			break;
			
			case 'estado':
				array_push( $agp['agrupador'], array("campo" 	  => "estado",
								   		   			 "label" 	  => "Estado") );
			break;
	
		}	
	}
	
	array_push( $agp['agrupador'], array("campo" 	  => "meta",
		 					   		     "label" 	  => "Meta") );
	
	return $agp;
	
}


function checklist_monta_sql_relatorio(){
	
	$where = array();
	
	extract($_REQUEST);
	
	if( $projeto[0] && $projeto_campo_flag ){
		array_push($where, "pro.atiid " . (!$projeto_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $projeto ) . "')");
	}
	
	if( $esdid[0] && $esdid_campo_flag ){
		array_push($where, "( ( esd.esdid " . (!$esdid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esdid ) . "') ) OR
							  ( esd2.esdid " . (!$esdid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esdid ) . "') ) OR
							  ( esd3.esdid " . (!$esdid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esdid ) . "') ) )");
	}
	
	if( $perid[0] && $perid_campo_flag ){
		array_push($where, "( ( p.perid " . (!$perid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $perid ) . "') ) OR
							  ( p2.perid " . (!$perid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $perid ) . "') ) OR
							  ( p3.perid " . (!$perid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $perid ) . "') ) )");
	}
	
	// monta o sql 
	$sql = "SELECT DISTINCT
				pro.atidescricao as projeto,
				a._atinumero||'- '||a.atidescricao as atividade1,
				a2._atinumero||' - '||a2.atidescricao as atividade2,
				a3._atinumero||' - '||a3.atidescricao as atividade3,
				coalesce(i.indnome, coalesce(i2.indnome, i3.indnome)) as item,
				coalesce(e.metdesc, coalesce(e2.metdesc, e3.metdesc)) as meta,
				
				coalesce(d.dmivalor, coalesce(d2.dmivalor, d3.dmivalor)) as valor_meta,
				coalesce(d.dmiqtde, coalesce(d2.dmiqtde, d3.dmiqtde)) as qtde_meta,
				to_char(coalesce(d.dmidtcoleta, coalesce(d2.dmidtcoleta, d3.dmidtcoleta)),'DD/MM/YYYY') as ultima_atualizacao_meta,
				to_char(coalesce(d.dmidataexecucao, coalesce(d2.dmidataexecucao, d3.dmidataexecucao)),'DD/MM/YYYY') as data_execucao_meta,
				to_char(coalesce(d.dmidatavalidacao, coalesce(d2.dmidatavalidacao, d3.dmidatavalidacao)),'DD/MM/YYYY') as data_validacao_meta,
				to_char(coalesce(d.dmidatameta, coalesce(d2.dmidatameta, d3.dmidatameta)),'DD/MM/YYYY') as data_meta,
				coalesce(p.perdsc, coalesce(p.perdsc, p.perdsc)) as periodicidade_meta,
				
				coalesce(s.sehvalor, coalesce(s2.sehvalor, s3.sehvalor)) as valor_execucao,
				coalesce(s.sehqtde, coalesce(s2.sehqtde, s3.sehqtde)) as qtde_execucao,
				coalesce(esd.esddsc, coalesce(esd2.esddsc, esd3.esddsc)) as estado
			FROM 
				pde.atividade a
			LEFT  JOIN pde.atividade a2  ON a2.atiidpai = a.atiid AND a2._atiprofundidade = 2
			LEFT  JOIN pde.atividade a3  ON a3.atiidpai = a.atiid AND a3._atiprofundidade = 3
			INNER JOIN pde.atividade pro ON pro.atiid   = a._atiprojeto AND pro.atiidpai IS NULL
			
			LEFT JOIN pde.monitoraitemchecklist 	m 	ON m.atiid = a.atiid
			LEFT JOIN painel.indicador 				i 	ON i.indid = m.indid
			LEFT JOIN painel.metaindicador			e 	ON e.indid = i.indid
			LEFT JOIN painel.periodicidade			p 	ON p.perid = e.perid AND p.perstatus = 'A'
			LEFT JOIN painel.detalhemetaindicador	d 	ON d.metid = e.metid AND d.dmistatus = 'A'
			LEFT JOIN painel.seriehistorica			s 	ON s.dmiid = d.dmiid AND s.sehstatus = 'A'
			LEFT JOIN workflow.documento			doc ON doc.docid = d.docid
			LEFT JOIN workflow.estadodocumento		esd ON esd.esdid = doc.esdid
			
			LEFT JOIN pde.monitoraitemchecklist 	m2 	 ON m2.atiid = a2.atiid
			LEFT JOIN painel.indicador 				i2 	 ON i2.indid = m2.indid
			LEFT JOIN painel.metaindicador			e2 	 ON e2.indid = i2.indid
			LEFT JOIN painel.periodicidade			p2 	 ON p2.perid = e2.perid AND p2.perstatus = 'A'
			LEFT JOIN painel.detalhemetaindicador	d2 	 ON d2.metid = e2.metid AND d2.dmistatus = 'A'
			LEFT JOIN painel.seriehistorica			s2 	 ON s2.dmiid = d2.dmiid AND s2.sehstatus = 'A'
			LEFT JOIN workflow.documento			doc2 ON doc2.docid = d2.docid
			LEFT JOIN workflow.estadodocumento		esd2 ON esd2.esdid = doc2.esdid
			
			LEFT JOIN pde.monitoraitemchecklist 	m3 	 ON m3.atiid = a3.atiid
			LEFT JOIN painel.indicador 				i3 	 ON i3.indid = m3.indid
			LEFT JOIN painel.metaindicador			e3 	 ON e3.indid = i3.indid
			LEFT JOIN painel.periodicidade			p3 	 ON p3.perid = e3.perid AND p3.perstatus = 'A'
			LEFT JOIN painel.detalhemetaindicador	d3   ON d3.metid = e3.metid AND d3.dmistatus = 'A'
			LEFT JOIN painel.seriehistorica			s3 	 ON s3.dmiid = d3.dmiid AND s3.sehstatus = 'A'
			LEFT JOIN workflow.documento			doc3 ON doc3.docid = d3.docid
			LEFT JOIN workflow.estadodocumento		esd3 ON esd3.esdid = doc3.esdid
			WHERE
				pro.atiestrategico = TRUE
				AND pro.atistatus = 'A' 
				AND pro.atiid NOT IN (".PROJETO_PDE.", ".PROJETOENEM.", ".PROJETOSEB.")
				AND a._atiprofundidade = 1
				and a.atiid != a._atiprojeto
				and a.atistatus = 'A'
				and ( a._atiprofundidade <= 3 ) and a.esaid in (1,2,3,4,5)    
				".(($where) ? " AND ". implode(" AND ",$where) : "")." 
			".(($_REQUEST['agrupadores'])?"ORDER BY ".implode(",",$agrupadores):"");

	//ver($sql,d);
	return $sql;
	
}

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */


// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

// monta o sql, agrupador e coluna do relatório
$sql       = checklist_monta_sql_relatorio(); //dbg($sql,1);
$agrupador = checklist_monta_agp_relatorio();
$coluna    = checklist_monta_coluna_relatorio();
$dados 	   = $db->carregar( $sql );

$rel->setAgrupador($agrupador, $dados); 
$rel->setColuna($coluna);
$rel->setTolizadorLinha(false);
$rel->setEspandir($_REQUEST['arvore_']);
$rel->setMonstrarTolizadorNivel(true);

// Gera o XLS do relatório
if ( $_REQUEST['pesquisa'] == '2' ){
	ob_clean();
    $nomeDoArquivoXls = 'relatorio';
    echo $rel->getRelatorioXls();
    die;
}

?>
<html>
	<head>
		<title> Simec - Sistema Integrado de Monitoramento do Ministério da Educação </title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	</head>
	<body>
		<center>
			<!--  Cabeçalho Brasão -->
			<?php echo monta_cabecalho_relatorio( '95' ); ?>
		</center>
		
		<!--  Monta o Relatório -->
		<? echo $rel->getRelatorio(); ?>
		
	</body>
</html>