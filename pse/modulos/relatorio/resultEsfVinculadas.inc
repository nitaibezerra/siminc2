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
$r->setTotNivel(false);
$r->setEspandir(false);
$r->setMonstrarTolizadorNivel(false);
$r->setTotalizador(true);
$r->setTolizadorLinha(false);
//$r->setBrasao(true);

if($_POST['tipo_relatorio'] == 'xls'){
	ob_clean();
	$nomeDoArquivoXls="relatorio_ESF_Vinculadas_".date('d-m-Y_H_i');
	echo $r->getRelatorioXls();
}elseif($_POST['tipo_relatorio'] == 'html'){
	echo $r->getRelatorio();
}
?>
</body>
</html>
<?php 
function monta_sql(){
	global $filtroSession;
	
	extract($_POST);
	
	$select = array();
	$from	= array();
	
	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		$where[1] = " AND ende.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}
	if( $municipio[0] && ( $municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[2] = " AND mun.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	if( $equipe[0] && ( $equipe_campo_flag || $equipe_campo_flag == '1' )){
		$where[3] = " AND sc.cnecodigocnes " . (( $equipe_campo_excludente == null || $equipe_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $equipe ) . "') ";		
	}
	if( $escola[0] && ( $escola_campo_flag || $escola_campo_flag == '1' )){
		$where[4] = " AND ent.entid " . (( $escola_campo_excludente == null || $escola_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $escola ) . "') ";		
	}
			
	$sql = "SELECT 
				ent.entcodent || ' - ' || ent.entnome AS escola, 
				ende.estuf as estado, 
				mun.mundescricao as municipio, 
				sc.cnecodigocnes ||' - '|| sc.cnenomefantasia as equipe
			FROM 
				entidade.entidade ent				
			INNER JOIN
				entidade.endereco ende ON ende.entid = ent.entid
			INNER JOIN
				territorios.municipio mun ON mun.muncod = ende.muncod
			INNER JOIN
				pse.planoacaoc1 pa ON pa.entid = ent.entid
			INNER JOIN
				pse.escolaesf esf ON esf.pamid = pa.pamid
			INNER JOIN
				pse.scnes sc ON sc.cneid = esf.cneid
			INNER JOIN
				pse.avaliacao av ON av.cneid = sc.cneid
			WHERE	
				ent.entstatus='A' 
				".$where[0]."
				".$where[1]."
				".$where[2]."
				".$where[3]."
				".$where[4]."
			ORDER BY 
				ende.estuf, mun.mundescricao, ent.entnome, sc.cnecodigocnes, sc.cnenomefantasia";

	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											/*"escola",
											"equipe",
											"fone",
											"emailsecretaria",
											"endereco"*/		
										  )	  
				);
				
	foreach ($agrupador as $val):
		switch ($val) {
			case 'estado':
				array_push($agp['agrupador'], array(
													"campo" => "estado",
											  		"label" => "Estado")										
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
		    case 'escola':
				array_push($agp['agrupador'], array(
													"campo" => "escola",
											  		"label" => "Escola")										
									   				);					
		    	continue;
		    break;
		    case 'equipe':
				array_push($agp['agrupador'], array(
													"campo" => "equipe",
											  		"label" => "Equipe")										
									   				);					
		    	continue;
		    break;
		}
		$count++;
	endforeach;
	
	return $agp;
}

function monta_coluna(){
/*	
	$arrPerfil = array(
						PERFIL_SUPERUSUARIO,
						PERFIL_ADMINISTRADOR
					  );
	
	$permissao = academico_possui_perfil($arrPerfil);
	
	$coluna    = array(
						array(
							  "campo" => "projetado",
					   		  "label" => "Projetado <br/>(A)",
							  "type"  => "numeric"
						),
	//					array(
	//						  "campo" => "autorizado",
	//				   		  "label" => "Autorizado",
	//						  "type"  => "numeric"
	//					),
						array(
							  "campo" => "autorizado",
					   		  "label" => "Concursos <br/>Autorizados <br/>(B)",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "publicado",
					   		  "label" => "Concursos <br/>Publicados <br/>(C)",
							  "type"  => "numeric"
						)
					);
					
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

	}			

	return $coluna;			  	
*/	
}
?>
