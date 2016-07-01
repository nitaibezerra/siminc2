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
$r->setMonstrarTolizadorNivel(true);
$r->setTotalizador(true);
$r->setTolizadorLinha(false);
	
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

	foreach($_POST['agrupador'] as $agrup => $v){
		if( $v == 'estado' ){
			$order.= ', ee.estuf';
		} else if( $v == 'municipio' ){
			$order.= ', m.mundescricao';
		} else if( $v == 'escolas' ){
			$order.= ', e.entnome';
		} else if( $v == 'perguntas' ){
			$order.= ', qp.pertitulo';
		} else if( $v == 'respostas' ){
			$order.= ', ip.itptitulo';
		}
	}
	
	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		$where[0] = " AND ee.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";
	}
	if( $perg6[0] && ( $perg6_campo_flag || $perg6_campo_flag == '1' )){
		$where[1] = " AND qp.perid " . (( $perg6_campo_excludente == null || $perg6_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $perg6 ) . "') ";		
	}
	if( $perg7[0] && ( $perg7_campo_flag || $perg7_campo_flag == '1' )){
		$where[2] = " AND qp.perid " . (( $perg7_campo_excludente == null || $perg7_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $perg7 ) . "') ";		
	}
	
	$sql = "SELECT
				qq.quetitulo as questionario,
				ee.estuf as estado, 
				est.estdescricao as descricao,
				m.mundescricao as municipio,
				e.entnome as escolas,
			--	qr.resid,
				qp.perid,
				qp.pertitulo as perguntas,
				CASE
					WHEN
						(qp2.perid is not null and qr2.resdsc is not null)
					THEN
						ip.itptitulo || ' ' || qr2.resdsc
					ELSE
						ip.itptitulo || (
								CASE WHEN (qr2.itpid is not null) THEN ' ' || ip2.itptitulo ELSE ' ' END
								)
				--	CASE WHEN 1 = 1 THEN || ' - teste' 
					end as respostas
			FROM
				pdeescola.pdequestionario pq
			INNER JOIN questionario.questionarioresposta qqr ON qqr.qrpid = pq.qrpid
			INNER JOIN questionario.questionario qq ON qq.queid = qqr.queid
			INNER JOIN questionario.resposta qr ON qr.qrpid = pq.qrpid
			INNER JOIN questionario.pergunta qp ON qp.perid = qr.perid
			INNER JOIN questionario.itempergunta ip ON ip.itpid = qr.itpid
			INNER JOIN entidade.endereco ee ON ee.entid = pq.entid 
			INNER JOIN entidade.entidade e ON e.entid = ee.entid
			INNER JOIN territorios.municipio m ON m.muncod = ee.muncod
			INNER JOIN territorios.estado est ON est.estuf = m.estuf
			LEFT  JOIN questionario.pergunta as qp2 ON ip.itpid = qp2.itpid
			LEFT  JOIN questionario.resposta as qr2 ON qr2.perid = qp2.perid AND qr2.qrpid = qr.qrpid
			LEFT  JOIN questionario.itempergunta as ip2 ON ip2.itpid = qr2.itpid AND qr2.qrpid = qr.qrpid
			WHERE
				qr.perid in ( select perid from questionario.pergunta where grpid in ( select grpid from questionario.grupopergunta where queid IN ( ".$_POST['quest']." )  ) )
			--	AND SUBSTRING(qqr.qrptitulo,length(qqr.qrptitulo) - 4,4) = '".$_SESSION['exercicio']."'
				".$where[0]."
				".$where[1]."
				".$where[2]."
			GROUP BY
				qq.queid, qq.quetitulo, ee.estuf, est.estdescricao, m.mundescricao, e.entnome, qr.resid, qp.pertitulo, qp.perid, ip.itpid, ip.itptitulo, qp2.perid, qr2.resdsc, qp2.pertitulo, qr2.itpid, ip2.itptitulo
			ORDER BY
				qq.quetitulo{$order}";
//	ver($sql);
	return $sql;
}

function monta_agp(){
//	$agrupador = Array( 0 => 'questionario', 1 => 'estado', 2 => 'municipio', 3 => 'escolas', 4 => 'perguntas', 5 => 'respostas' );
//	$agrupador = Array( 0 => 'questionario', 1 => 'estado', 2 => 'municipio', 3 => 'escolas', 4 => 'perguntas' );
	$agrupador = $_POST['agrupador'];
		
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
									//		"respostas"
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
		$i = 0;
		
		if( $i > 1 || $i == 0 ){
			$vari = "ESCOLA - Questionário SEESP<br>";	
		}
	foreach ($agrupador as $val):
		if($count == 1){
			$var = $vari;
		} else {
			$var = "";		
		}
		switch ($val) {
			case 'estado':
				array_push($agp['agrupador'], array(
													"campo" => "descricao",
											  		"label" => "$var Estado")										
									   				);				
		   		continue;
		    break;
		    case 'municipio':
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
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
		    case 'escolas':
				array_push($agp['agrupador'], array(
													"campo" => "escolas",
											 		"label" => "$var Escola")										
									   				);					
		   		continue;			
		    break;
		    case 'perguntas':
				array_push($agp['agrupador'], array(
													"campo" => "perguntas",
											 		"label" => "$var Pergunta")										
									   				);					
		   		continue;			
		    break;
		    case 'respostas':
				array_push($agp['agrupador'], array(
													"campo" => "respostas",
											 		"label" => "$var Resposta")										
									   				);					
		   		continue;			
		    break;
		}
		$count++;
	endforeach;
	return $agp;
}

function monta_coluna(){
	$coluna    = array(
//						array(
//							  "campo" => "respostas",
//					   		  "label" => "Respostas",
//							  "type"  => "string"
//							 // "html"  	 => "<span style='color:{color}'>{equipe}</span>",
//						)
	//					array(
	//						  "campo" => "autorizado",
	//				   		  "label" => "Autorizado",
	//						  "type"  => "numeric"
	//					),
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