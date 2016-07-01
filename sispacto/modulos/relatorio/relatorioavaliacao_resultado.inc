<?php
/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

$sql   = monta_sql();
$dados = $db->carregar($sql);

$agrup = monta_agp();
$col   = monta_coluna();
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setMonstrarTolizadorNivel(true);
$r->setTotNivel(true);
$r->setBrasao(true);
$r->setEspandir(false);

?>


<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>

	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	

		<!--  Monta o Relatório -->
		<? echo $r->getRelatorio(); ?>

	</body>
</html>



<?

function monta_sql(){
	global $db;
	extract($_REQUEST);

	$where = array();
		
	// universidade
	if( $uniid[0] && $uniid_campo_flag != '' ){
		array_push($where, " u.uniid " . (!$uniid_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $uniid ) . ") ");
	}
	
	// estado
	if( $estuf[0] && $estuf_campo_flag != '' ){
		array_push($where, "CASE WHEN mu.muncod IS NOT NULL THEN mu.estuf " . (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') 
				 			WHEN iu2.muncodatuacao IS NOT NULL THEN mu3.estuf " . (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') 
				 			WHEN mu2.muncod IS NOT NULL THEN mu2.estuf" . (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "')  
				 			ELSE false END");
	}
	
	// municipio
	if( $muncod[0] && $muncod_campo_flag != '' ){
		array_push($where, "CASE WHEN mu.muncod IS NOT NULL THEN mu.muncod " . (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $muncod ) . "') 
				 			WHEN iu2.muncodatuacao IS NOT NULL THEN mu3.muncod " . (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $muncod ) . "') 
				 			WHEN mu2.muncod IS NOT NULL THEN mu2.muncod" . (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $muncod ) . "')  
				 			ELSE false END");
	}

	// parcela
	if( $fpbid[0] && $tipoos_campo_flag != '' ){
		array_push($where, " f.fpbid " . (!$fpbid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $fpbid ) . "') ");
	}
	
	// perfil
	if( $pflcod[0]  && $pflcod_campo_flag != '' ){
		array_push($where, " p.pflcod " . (!$pflcod_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $pflcod ) . "') ");
	}
	
	// situacao
	if( $esdid[0]  && $esdid_campo_flag != '' ){
		array_push($where, " es.esdid " . (!$esdid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esdid ) . "') ");
	}
	
	if($iusnome) {
		array_push($where, " iu2.iusnome ILIKE '%".$iusnome."%' ");
	}
	

	$sql = "SELECT 
			iu2.iusnome || ' ( ' || replace(to_char(iu2.iuscpf::numeric, '000:000:000-00'), ':', '.') || ' )' as avaliado,
			iu.iusnome || ' ( ' || replace(to_char(iu.iuscpf::numeric, '000:000:000-00'), ':', '.') || ' )'   as avaliador,
			u.unisigla || ' / ' || u.uninome 								  as universidade,
			'Período ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' 				  as periodo,
			p.pfldsc 											  as perfil,
			COALESCE(p2.pfldsc,'AVALIADOR DESLIGADO')             as perfilavaliador,
			es.esddsc 											  as situacao,
			CASE WHEN pc.picid IS NULL THEN 'Equipe IES' 
				 WHEN pc.muncod IS NOT NULL THEN 'Rede Municipal' 
				 WHEN pc.estuf IS NOT NULL THEN 'Rede Estadual'
				 ELSE 'Não identificado' END 								  as esfera,
			CASE WHEN mu.muncod IS NOT NULL THEN mu.mundescricao 
				 WHEN iu2.muncodatuacao IS NOT NULL THEN mu3.mundescricao 
				 WHEN mu2.muncod IS NOT NULL THEN mu2.mundescricao 
				 ELSE 'Não identificado' END 								  as municipio,
			CASE WHEN mu.muncod IS NOT NULL THEN mu.estuf 
				 WHEN iu2.muncodatuacao IS NOT NULL THEN mu3.estuf 
				 WHEN mu2.muncod IS NOT NULL THEN mu2.estuf 
				 ELSE 'Não identificado' END 							          as estado,
			to_char(htddata,'dd/mm/YYYY HH24:MI') as datatramitacao,
			mea.mavtotal as notafinal
			FROM sispacto.mensario mem 
			INNER JOIN sispacto.mensarioavaliacoes   mea ON mea.menid = mem.menid 
			INNER JOIN sispacto.identificacaousuario iu  ON iu.iusd = mea.iusdavaliador 
			LEFT JOIN sispacto.tipoperfil 			 t2  ON t2.iusd = iu.iusd 
			LEFT JOIN seguranca.perfil 			 	 p2  ON p2.pflcod = t2.pflcod 
			INNER JOIN sispacto.identificacaousuario iu2 ON iu2.iusd = mem.iusd 
		INNER JOIN sispacto.tipoperfil 		 t   ON t.iusd = iu2.iusd 
		INNER JOIN sispacto.universidadecadastro un  ON un.uncid = iu2.uncid 
		INNER JOIN sispacto.universidade 	 u   ON u.uniid = un.uniid 
		INNER JOIN sispacto.folhapagamento 	 f   ON f.fpbid = mem.fpbid 
		INNER JOIN public.meses 		 m   ON m.mescod::integer = f.fpbmesreferencia 
		INNER JOIN seguranca.perfil 		 p   ON p.pflcod = t.pflcod 
		INNER JOIN workflow.documento 		 d   ON d.docid = mem.docid  AND d.tpdid=".TPD_FLUXOMENSARIO."
		INNER JOIN workflow.estadodocumento 	 es  ON es.esdid = d.esdid
		LEFT JOIN sispacto.pactoidadecerta 	 pc  ON pc.picid = iu2.picid 
		LEFT JOIN territorios.municipio 	 mu  ON mu.muncod = pc.muncod 
		LEFT JOIN territorios.municipio 	 mu2 ON mu2.muncod = iu2.muncod 
		LEFT JOIN territorios.municipio 	 mu3 ON mu3.muncod = u.muncod 
		LEFT JOIN workflow.historicodocumento    hst ON hst.hstid = d.hstid
		".(($where)?"WHERE ".implode(" AND ",$where):"")."";
	
	return $sql;
}

function monta_agp(){
	$agrupador = $_REQUEST['agrupador'];
	$agp = array(
					"agrupador" => array(),
					"agrupadoColuna" => array(
												"avaliado",
												"avaliador",
												"situacao",
												"perfil",
												"periodo",
												"universidade",
												"esfera",
												"municipio",
												"estado",
												"datatramitacao",
												"notafinal"
												
											  )	  
					);
					
	foreach ($agrupador as $val): 
		switch ($val) :		
		    case 'universidade':
				array_push($agp['agrupador'], array(
													"campo" => "universidade",
											 		"label" => "Universidade")										
									   				);					
		    	continue;			
		        break;	
		        
		    case 'perfil':
				array_push($agp['agrupador'], array(
												"campo" => "perfil",
												"label" => "Perfil avaliado")										
										   		);	
				continue;
				break;
		    case 'perfilavaliador':
				array_push($agp['agrupador'], array(
												"campo" => "perfilavaliador",
												"label" => "Perfil avaliador")										
										   		);	
				continue;
				break;
				
		    case 'esfera':
				array_push($agp['agrupador'], array(
												"campo" => "esfera",
												"label" => "Esfera")										
										   		);	
				continue;
				break;
		    case 'municipio':
				array_push($agp['agrupador'], array(
												"campo" => "municipio",
												"label" => "Município")										
										   		);	
				continue;
				break;
		    case 'estado':
				array_push($agp['agrupador'], array(
												"campo" => "estado",
												"label" => "UF")										
										   		);	
				continue;
				break;
		    case 'avaliador':
				array_push($agp['agrupador'], array(
												"campo" => "avaliador",
												"label" => "Avaliador")										
										   		);	
				continue;
				break;
			
		endswitch;
	endforeach;
	
	array_push($agp['agrupador'], array(
									"campo" => "periodo",
									"label" => "Período Referência")										
							   		);	
	

	array_push($agp['agrupador'], array( "campo" => "avaliado",
										 "label" => "Avaliado")										
										);	
	
	return $agp;
}


function monta_coluna(){
	$coluna = array();
	
	array_push($coluna,array( "campo" => "perfil",
					   		  "label" => "Perfil" ) );					
	
	
	array_push($coluna,array( "campo" => "periodo",
					   		  "label" => "Período Referência" ) );

	array_push($coluna,array( "campo" => "notafinal",
					   		  "label" => "Nota",
							  "type"  => "string" ) );					
	
	
	array_push($coluna,array( "campo" => "situacao",
					   		  "label" => "Situação Pagamento" ) );
	
	array_push($coluna,array( "campo" => "datatramitacao",
					   		  "label" => "Data Tramitação" ) );
	

	return $coluna;			  	
}
?>