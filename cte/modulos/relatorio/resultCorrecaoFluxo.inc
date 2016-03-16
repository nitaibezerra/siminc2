<?

if ( $_REQUEST['filtrosession'] ){
	$filtroSession = $_REQUEST['filtrosession'];
}

if ($_POST['agrupador']){
	header( 'Content-Type: text/html; charset=iso-8859-1' ); 
}
ini_set("memory_limit","250M");
set_time_limit(0);
?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<?php
include APPRAIZ. 'includes/classes/relatorio.class.inc';

$sql   = monta_sql();
$dados = $db->carregar($sql);
$agrup = monta_agp();
$col   = monta_coluna();
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setBrasao(true);
$r->setTotNivel(true);
$r->setEspandir(false);
$r->setMonstrarTolizadorNivel(false);
$r->setTotalizador(true);
$r->setTolizadorLinha(true);

//$r->setBrasao(true);
echo $r->getRelatorio();
?>
</body>
</html>
<?php 
function monta_sql(){
	global $filtroSession;
	
	extract($_POST);
	
	$select = array();
	$from	= array();
	
//	if ($f_tipoensino){
//		$where[] = " orgid = '$f_tipoensino'";
//	}
	
//	if (!empty($_SESSION["academico"]["orgid"])){
//		$funid = $_SESSION["academico"]["orgid"] == 1 ? 18 : 17;	
//	}
	/*
	$equipe = !is_array($equipe) ? explode(',', $equipe) : $equipe;
	$estado = !is_array($estado)   ? explode(',', $estado)   : $estado;
	$municipio = !is_array($municipio)    ? explode(',', $municipio)    : $municipio;
	$programa  = !is_array($programa)  ? explode(',', $programa)  : $programa;
	$classe    = !is_array($classe)    ? explode(',', $classe)    : $classe;
*/
	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		$where[0] = " AND iu.mun_estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}
	if( $municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[1] = " AND iu.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
//	if( $nivel[0] && ( $nivel_campo_flag ||$nivel_campo_flag == '1' )){
//		$where[] = " AND ano.nieid " . (( $nivel_campo_excludente == null || $nivel_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $nivel ) . "') ";		
//	}
//	if( $ano[0] && ( $ano_campo_flag ||$ano_campo_flag == '1' )){
//		$where[] = " AND ano.paeanoreferencia " . (( $ano_campo_excludente == null || $ano_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $ano ) . "') ";		
//	}
//	if( $pse[0] && ( $pse_campo_flag ||$pse_campo_flag == '1' )){
//		$where[] = " AND es.espparticipapse2009 " . (( $pse_campo_excludente == null || $pse_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $pse ) . "') ";		
//	}
//	
//	if( $unidade[0] && ( $unidade_campo_flag || $unidade_campo_flag == '1' )){
//		$where[] = " idunidade " . (( $unidade_campo_excludente == null || $unidade_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $unidade ) . "') ";
//	}elseif ($_SESSION['academico']['entid'] && $filtroSession && $filtroSession == 'unidade'){
//		$where[] = " idunidade = " . $_SESSION['academico']['entid'];
//	}
//		
//	if( $campus[0] && ( $campus_campo_flag || $campus_campo_flag == '1' )){
//		$where[] = " idcampus " . (($campus_campo_excludente == null || $campus_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $campus ) . "') ";
//	}elseif ($_SESSION['academico']['entidcampus'] && $filtroSession && $filtroSession != 'unidade'){
//		$where[] = " idcampus = " . $_SESSION['academico']['entidcampus'];
//	}
//	
//	if( $programa[0] && ( $programa_campo_flag || $programa_campo_flag == '1' )){
//		$where[] = " idprograma " . (( $programa_campo_excludente == null || $programa_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $programa ) . "') ";
//	}
//	
//	if( $classe[0] && ( $classe_campo_flag || $classe_campo_flag == '1' )){
//		$where[] = " idclasse " . (( $classe_campo_excludente == null || $classe_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $classe ) . "') ";
//	}
//	
//	$agrupador = is_array($agrupador) ? implode(',', $agrupador) : $agrupador; 
	
	//if (strpos($agrupador, "unidade") !== false){
//		$select[] = "COALESCE(unidade , 'Não informado') as unidade";
//		$select[] = "idunidade as idunidade";		
//	//}
//
//	//if (strpos($agrupador, "campus") !== false){
//		$select[] = "COALESCE(campus, 'Não informado') as campus";
//		$select[] = "idcampus as idcampus";	
//	//}	
//
//	//if (strpos($agrupador, "classe") !== false){
//		$select[] = "COALESCE(classe, 'Não informado') as classe";
//		$select[] = "idclasse as idclasse";	
//	//}	
//
//	//if (strpos($agrupador, "tipoensino") !== false || !$agrupador){
//		$select[] = "tipoensino as tipoensino";	
//	//}	
//	
//	//if (strpos($agrupador, "programa") !== false){
//		$select[] = "programa as programa";
//		$select[] = "idprograma as idprograma";	
//	//}	
//
//	//if (strpos($agrupador, "ano") !== false){
//		$select[] = "ano";
//	//}	
//	
//	//if (strpos($agrupador, "portaria") !== false){
//		$select[] = "'Nº controle: ' || COALESCE(portaria::VARCHAR,'Não informado') || ' / ' || 'Nº portaria: ' || COALESCE(numeroportaria::VARCHAR, 'Não informado') as portaria";
//	//}	
	$ordem = implode(',',$_POST['agrupador']);
	
	$sql = "(
				-- Verifica desistente
				SELECT
					qr.qrpid AS id,
					q.quetitulo as questionario,
					qq.inuid,
					iu.mun_estuf AS estado,
					iu.muncod AS municipio,
					m.mundescricao,
					qr.qrpdata AS data_cadastro,
					'1' as idpergunta,
					'Desistente?' AS pergunta,
					case when fe.flestatus = 'I' then 'Desistente' else 'Não Desistente' end AS resposta,
					1 as campo
				FROM
					cte.ctequestionario qq
				JOIN cte.instrumentounidade iu USING (inuid)
				JOIN cte.fluxoescolar fe USING (inuid)
				JOIN questionario.questionarioresposta qr USING (qrpid)
				JOIN questionario.questionario q ON q.queid = qr.queid
				JOIN territorios.municipio m USING (muncod)
				WHERE
					1 = 1
					".$where[0]."
					".$where[1]."
			)UNION ALL(
				-- Perguntas filhas de questionarios e de resposta textual
				SELECT
					qr.qrpid AS id,
					q.quetitulo as questionario,
					qq.inuid,
					iu.mun_estuf AS estado,
					iu.muncod AS municipio,
					m.mundescricao,
					qr.qrpdata AS data_cadastro,
					2 as idpergunta,
					p.pertitulo AS pergunta,
					r.resdsc AS resposta,
					1 as campo	
				FROM
					cte.ctequestionario qq
				JOIN cte.instrumentounidade iu USING (inuid)
				JOIN questionario.questionarioresposta qr USING (qrpid)
				JOIN questionario.questionario q ON q.queid = qr.queid
				JOIN questionario.pergunta p ON q.queid = p.queid
				JOIN questionario.resposta r ON r.perid = p.perid
								AND r.qrpid = qr.qrpid
								AND r.itpid IS NULL
				JOIN territorios.municipio m USING (muncod)
				WHERE
					1 = 1
					".$where[0]."
					".$where[1]."
			)UNION ALL(
				-- Perguntas vinculadas a grupos filhos de questionarios e de resposta textual
				SELECT
					qr.qrpid AS id,
					q.quetitulo as questionario,
					qq.inuid,
					iu.mun_estuf AS estado,
					iu.muncod AS municipio,
					m.mundescricao,
					qr.qrpdata AS data_cadastro,
					p.perid as idpergunta,
					p.pertitulo AS pergunta,
					r.resdsc AS resposta,
					1 as campo	
				FROM
					cte.ctequestionario qq
				JOIN cte.instrumentounidade iu USING (inuid)
				JOIN questionario.questionarioresposta qr USING (qrpid)
				JOIN questionario.questionario q ON q.queid = qr.queid
				JOIN questionario.grupopergunta gp ON gp.queid = q.queid
				JOIN questionario.pergunta p ON p.grpid = gp.grpid
				JOIN questionario.resposta r ON r.perid = p.perid
								AND r.qrpid = qr.qrpid
								AND r.itpid IS NULL
				JOIN territorios.municipio m USING (muncod)
				WHERE
					1 = 1
					".$where[0]."
					".$where[1]."
				---WHERE 
					--qq.estuf IS NULL
					--AND qq.mun_estuf IS NOT NULL
					--AND qq.muncod IS NOT NULL
					--AND estuf = 'RS'
					--AND (pertitulo like '5%' OR pertitulo like '2%')
			)UNION ALL(
				-- Perguntas vinculadas a grupos filhos de questionarios e que possuem item como resposta
				SELECT
					qr.qrpid AS id,
					q.quetitulo as questionario,
					qq.inuid,
					iu.mun_estuf AS estado,
					iu.muncod AS municipio,
					m.mundescricao,
					qr.qrpdata AS data_cadastro,
					p.perid as idpergunta,
					p.pertitulo AS pergunta,
					ip.itptitulo AS resposta,
					1 as campo
					
				FROM
					cte.ctequestionario qq
				JOIN cte.instrumentounidade iu USING (inuid)
				JOIN questionario.questionarioresposta qr USING (qrpid)
				JOIN questionario.questionario q ON q.queid = qr.queid
				JOIN questionario.grupopergunta gp ON gp.queid = q.queid
				JOIN questionario.pergunta p ON p.grpid = gp.grpid
				JOIN questionario.itempergunta ip ON ip.perid = p.perid
				JOIN questionario.resposta r ON r.perid = p.perid
								AND r.qrpid = qr.qrpid
								AND r.itpid = ip.itpid
				JOIN territorios.municipio m USING (muncod)
				WHERE
					1 = 1
					".$where[0]."
					".$where[1]."
				--WHERE 
					--qq.estuf IS NULL
					--AND qq.mun_estuf IS NOT NULL
					--AND qq.muncod IS NOT NULL
					--AND estuf = 'RS'
					--AND (pertitulo like '5%' OR pertitulo like '2%')
			)UNION ALL(
				-- Perguntas de resposta textual filhas de itens de perguntas vinculadas a grupos filhos de questionarios
				SELECT
					qr.qrpid AS id,
					q.quetitulo as questionario,
					qq.inuid,
					iu.mun_estuf AS estado,
					iu.muncod AS municipio,
					m.mundescricao,
					qr.qrpdata AS data_cadastro,
					p.perid as idpergunta,
					p.pertitulo ||' - '|| ip.itptitulo || ' - ' || p1.pertitulo AS pergunta ,
					r.resdsc AS resposta,
					1 as campo
					
				FROM
					cte.ctequestionario qq
				JOIN cte.instrumentounidade iu USING (inuid)
				JOIN questionario.questionarioresposta qr USING (qrpid)
				JOIN questionario.questionario q ON q.queid = qr.queid
				JOIN questionario.grupopergunta gp ON gp.queid = q.queid
				JOIN questionario.pergunta p ON p.grpid = gp.grpid
				JOIN questionario.itempergunta ip ON ip.perid = p.perid
				JOIN questionario.pergunta p1 ON p1.itpid = ip.itpid
				JOIN questionario.resposta r ON r.perid = p1.perid
								AND r.qrpid = qr.qrpid
								AND r.itpid IS NULL
				JOIN territorios.municipio m USING (muncod)
				WHERE
					1 = 1
					".$where[0]."
					".$where[1]."
				--WHERE 
				--	qq.estuf IS NULL
				--	AND qq.mun_estuf IS NOT NULL
				--	AND qq.muncod IS NOT NULL
				--	--AND estuf = 'RS'
				--	AND (p.pertitulo like '5%' OR p.pertitulo like '2%')
			)UNION ALL(
			-- Perguntas de resposta textual filhas de grupos filhos de itens de perguntas vinculadas a grupos filhos de questionarios
				SELECT
					qr.qrpid AS id,
					q.quetitulo as questionario,
					qq.inuid,
					iu.mun_estuf AS estado,
					iu.muncod AS municipio,
					m.mundescricao,
					qr.qrpdata AS data_cadastro,
					p.perid as idpergunta,
					p.pertitulo ||' - '|| ip.itptitulo || ' - ' || p1.pertitulo AS pergunta ,
					r.resdsc AS resposta,
					1 as campo
					
				FROM
					cte.ctequestionario qq
				JOIN cte.instrumentounidade iu USING (inuid)
				JOIN questionario.questionarioresposta qr USING (qrpid)
				JOIN questionario.questionario q ON q.queid = qr.queid
				JOIN questionario.grupopergunta gp ON gp.queid = q.queid
				JOIN questionario.pergunta p ON p.grpid = gp.grpid
				JOIN questionario.itempergunta ip ON ip.perid = p.perid
				JOIN questionario.grupopergunta gp1 ON gp1.itpid = ip.itpid
				JOIN questionario.pergunta p1 ON p1.grpid = gp1.grpid
				JOIN questionario.resposta r ON r.perid = p1.perid
								AND r.qrpid = qr.qrpid
								AND r.itpid IS NULL
				JOIN territorios.municipio m USING (muncod)
				WHERE
					1 = 1
					".$where[0]."
					".$where[1]."
			--	WHERE 
			--		qq.estuf IS NULL
			--		AND qq.mun_estuf IS NOT NULL
			--		AND qq.muncod IS NOT NULL
			--		--AND estuf = 'RS'
			--		AND (p.pertitulo like '5%' OR p.pertitulo like '2%')
			)UNION ALL(
				-- Perguntas de resposta textual filhas de grupos vinculadas a grupos filhos de questionarios
				SELECT
					qr.qrpid AS id,
					q.quetitulo as questionario,
					qq.inuid,
					iu.mun_estuf AS estado,
					iu.muncod AS municipio,
					m.mundescricao,
					qr.qrpdata AS data_cadastro,
					p.perid as idpergunta,
					p.pertitulo AS pergunta,
					r.resdsc AS resposta,
					1 as campo
					
				FROM
					cte.ctequestionario qq
				JOIN cte.instrumentounidade iu USING (inuid)
				JOIN questionario.questionarioresposta qr USING (qrpid)
				JOIN questionario.questionario q ON q.queid = qr.queid
				JOIN questionario.grupopergunta gp ON gp.queid = q.queid
				JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
				JOIN questionario.pergunta p ON p.grpid = gp1.grpid
				JOIN questionario.resposta r ON r.perid = p.perid
								AND r.qrpid = qr.qrpid
								AND r.itpid IS NULL
				JOIN territorios.municipio m USING (muncod)
				WHERE
					1 = 1
					".$where[0]."
					".$where[1]."
				--WHERE 
				--	qq.estuf IS NULL
				--	AND qq.mun_estuf IS NOT NULL
				--	AND qq.muncod IS NOT NULL
					--AND estuf = 'RS'
				--	AND (p.pertitulo like '5%' OR p.pertitulo like '2%')
			)UNION ALL(
				-- Perguntas de resposta por itens filhas de grupos vinculadas a grupos filhos de questionarios
				SELECT
					qr.qrpid AS id,
					q.quetitulo as questionario,
					qq.inuid,
					iu.mun_estuf AS estado,
					iu.muncod AS municipio,
					m.mundescricao,
					qr.qrpdata AS data_cadastro,
					p.perid as idpergunta,
					p.pertitulo AS pergunta,
					ip.itptitulo AS resposta,
					1 as campo
					
				FROM
					cte.ctequestionario qq
				JOIN cte.instrumentounidade iu USING (inuid)
				JOIN questionario.questionarioresposta qr USING (qrpid)
				JOIN questionario.questionario q ON q.queid = qr.queid
				JOIN questionario.grupopergunta gp ON gp.queid = q.queid
				JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
				JOIN questionario.pergunta p ON p.grpid = gp1.grpid
				JOIN questionario.itempergunta ip ON ip.perid = p.perid
				JOIN questionario.resposta r ON r.perid = p.perid
								AND r.itpid = ip.itpid
								AND r.qrpid = qr.qrpid
								AND r.itpid IS NOT NULL
				JOIN territorios.municipio m USING (muncod)
				WHERE
					1 = 1
					".$where[0]."
					".$where[1]."
			--	WHERE 
			--		qq.estuf IS NULL
			--		AND qq.mun_estuf IS NOT NULL
			--		AND qq.muncod IS NOT NULL
					--AND estuf = 'RS'
			--		AND (p.pertitulo like '5%' OR p.pertitulo like '2%')
			)UNION ALL(
				-- Perguntas de resposta textual filhas de itens de perguntas filhas de grupos vinculadas a grupos filhos de questionarios
				SELECT
					qr.qrpid AS id,
					q.quetitulo as questionario,
					qq.inuid,
					iu.mun_estuf AS estado,
					iu.muncod AS municipio,
					m.mundescricao,
					qr.qrpdata AS data_cadastro,
					p.perid as idpergunta,
					p.pertitulo ||' - '|| ip.itptitulo || ' - ' || p1.pertitulo AS pergunta ,
					r.resdsc AS resposta,
					1 as campo
					
				FROM
					cte.ctequestionario qq
				JOIN cte.instrumentounidade iu USING (inuid)
				JOIN questionario.questionarioresposta qr USING (qrpid)
				JOIN questionario.questionario q ON q.queid = qr.queid
				JOIN questionario.grupopergunta gp ON gp.queid = q.queid
				JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
				JOIN questionario.pergunta p ON p.grpid = gp1.grpid
				JOIN questionario.itempergunta ip ON ip.perid = p.perid
				JOIN questionario.pergunta p1 ON p1.itpid = ip.itpid
				JOIN questionario.resposta r ON r.perid = p1.perid
								AND r.qrpid = qr.qrpid
								AND r.itpid IS NULL
				JOIN territorios.municipio m USING (muncod)
				WHERE
					1 = 1
					".$where[0]."
					".$where[1]."
				--WHERE 
				--	qq.estuf IS NULL
				--	AND qq.mun_estuf IS NOT NULL
				--	AND qq.muncod IS NOT NULL
					--AND estuf = 'RS'
				--	AND (p.pertitulo like '5%' OR p.pertitulo like '2%')
			)UNION ALL(
			-- Perguntas de resposta textual filhas de grupos filhos de itens de perguntas filhas de grupos vinculadas a grupos filhos de questionarios
				SELECT
					qr.qrpid AS id,
					q.quetitulo as questionario,
					qq.inuid,
					iu.mun_estuf AS estado,
					iu.muncod AS municipio,
					m.mundescricao,
					qr.qrpdata AS data_cadastro,
					p.perid as idpergunta,
					p.pertitulo ||' - '|| ip.itptitulo || ' - ' || p1.pertitulo AS pergunta ,
					r.resdsc AS resposta,
					1 as campo
					
				FROM
					cte.ctequestionario qq
				JOIN cte.instrumentounidade iu USING (inuid)
				JOIN questionario.questionarioresposta qr USING (qrpid)
				JOIN questionario.questionario q ON q.queid = qr.queid
				JOIN questionario.grupopergunta gp ON gp.queid = q.queid
				JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
				JOIN questionario.pergunta p ON p.grpid = gp1.grpid
				JOIN questionario.itempergunta ip ON ip.perid = p.perid
				JOIN questionario.grupopergunta gp2 ON gp2.itpid = ip.itpid
				JOIN questionario.pergunta p1 ON p1.grpid = gp2.grpid
				JOIN questionario.resposta r ON r.perid = p1.perid
								AND r.qrpid = qr.qrpid
								AND r.itpid IS NULL
				JOIN territorios.municipio m USING (muncod)
				WHERE
					1 = 1
					".$where[0]."
					".$where[1]."
			) ORDER BY 
				".$ordem;
	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
		
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"campo"
									//		"qnt2",
										//	"percent"
											/*"projetado",	
									   		"autorizado", 
 									   		"provimento",
 									   		"publicado",
 									   		"homologado",
											"lepvlrprovefetivados",
											"provimentosnaoefetivados",
											"provimentopendencia",
										    "homocolor"
										    */
										  )	  
				);
	
	$count = 1;
	foreach ($agrupador as $val):
		if($count == 1){
			$var = "PAR - Correção de Fluxo<br>";
		} else {
			$var = "";		
		}
		switch ($val) {
			case 'estado':
				array_push($agp['agrupador'], array(
													"campo" => "estado",
											  		"label" => "$var Estado")										
									   				);				
		   		continue;
		    break;
		    case 'municipio':
				array_push($agp['agrupador'], array(
													"campo" => "mundescricao",
											  		"label" => "$var Município")										
									   				);					
		    	continue;
		    break;		    	
		    case 'questionario':
				array_push($agp['agrupador'], array(
													"campo" => "questionario",
											 		"label" => "$var Questionário")										
									   				);					
		    	continue;			
		    break;
		    case 'idpergunta':
				array_push($agp['agrupador'], array(
													"campo" => "pergunta",
											 		"label" => "$var Pergunta")										
									   				);					
		    	continue;			
		    break;
		    case 'resposta':
				array_push($agp['agrupador'], array(
													"campo" => "resposta",
											 		"label" => "$var Resposta",
													"html"  => "{resposta} ({campo})")
									   				);					
		    	continue;			
		    break;
//		    case 'escola':
//				array_push($agp['agrupador'], array(
//													"campo" => "escolas",
//											 		"label" => "$var Escola")										
//									   				);					
//		   		continue;			
//		    break;
		}
		$count++;
	endforeach;
	return $agp;
}

function monta_coluna(){
	
	$coluna    = array(
						array(
							  "campo" => "campo",
					   		  "label" => "Qtd. Resposta",
							  "type"  => "numeric",
							  "blockAgp" => array("questionario", "mundescricao", "estado")
							 // "html"  	 => "<span style='color:{color}'>{equipe}</span>",
						)
//						array(
//							  "campo" => "resposta",
//					   		  "label" => "Resposta",
//							  "type"  => "string"
//						)
//						array(
//							  "campo" => "qnt2",
//					   		  "label" => "Quantidade Atendida",
//							  "type"  => "numeric"
//						),
//						array(
//							  "campo" => "percent",
//					   		  "label" => "Percentual <br>de Defasagem (%)",
//							  "type"  => "numeric"
//						)
					);
		/*			
	if ( $permissao ){				
		array_push($coluna, array(
								  "campo" 	 => "homologado",
						   		  "label" 	 => "Concursos <br/>Homologados <br/>(D)",
								  "type"  	 => "numeric",
								  "html"  	 => "<span style='color:{color}'>{homologado}</span>",
								  "php" 	 => array(
									  					"expressao" => "{homologado} > {autorizado}",
														"var" 		=> "color",
														"true"	    => "red",
														"false"     => "#0066CC",
									  				 )	
							)
				  );			
	}else{
		array_push($coluna, array(
								  "campo" 	 => "homologado",
						   		  "label" 	 => "Concursos <br/>Homologados <br/>(D)",
								  "type"  	 => "numeric"
								 )
				  );			

	}
	
	array_push($coluna,array(
							  "campo" => "provimento",
					   		  "label" => "Provimentos <br/>Autorizados <br/>(E)",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "lepvlrprovefetivados",
					   		  "label" => "Provimentos <br/>Efetivados <br/>(F)",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "provimentosnaoefetivados",
					   		  "label" => "Provimentos <br/>Não Efetivados <br/>(G)=(E)-(F)",
							  "type"  => "numeric"
						)
				);	

				
	if ( $permissao ){				
		array_push($coluna, array(
									  "campo" => "provimentopendencia",
							   		  "label" => "Aut. Provimento <br/>Pendente <br/>(H)=(D)-(E)",
									  "type"  => "numeric",
									  "html"  => "{valorcampo}",
									  "php"   => array(
									  					"expressao" => "({homologado} - {provimento} >= 0) || ({provimentopendencia} > 0)",
														"type"		=> "string",
									  					"var"       => "valorcampo",
														"true"		=> "{provimentopendencia}",
														"false"		=> "<span style='color:red;'>-</span>"					
									  				   )
								)
				  );			
	}else{
		array_push($coluna, array(
									  "campo" => "provimentopendencia",
							   		  "label" => "Aut. Provimento <br/>Pendente <br/>(H)=(D)-(E)",
									  "type"  => "numeric"		
								  )
				  );			

	}		*/	

	return $coluna;			  	
	
}
?>
