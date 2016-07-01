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
	if( $equipe[0] && ( $equipe_campo_flag || $equipe_campo_flag == '1' )){
		$where[0] = " AND sc.cnecodigocnes " . (( $equipe_campo_excludente == null || $equipe_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $equipe ) . "') ";		
	}
	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		$where[1] = " AND ende.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}
	if( $municipio[0] && ( $municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[2] = " AND mun.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	if( $esfera[0] != '' && ( $esfera_campo_flag ||$esfera_campo_flag == '1' )){
		$where[3] = " AND p.empflagestmun " . (( $esfera_campo_excludente == null || $esfera_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esfera ) . "') ";		
	}
    if( $portaria[0] != '' && ( $portaria_campo_flag ||$portaria_campo_flag == '1' )){
		$where[4] = " AND pp.porano " . (( $portaria_campo_excludente == null || $portaria_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $portaria ) . "') ";		
	}
	if( $exer[0] && ( $exer_campo_flag || $exer_campo_flag == '1' )){
		$where[5] = " AND ep.espano " . (( $exer_campo_excludente == null || $exer_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $exer ) . "') ";		
	}
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
			
	$sql = "SELECT 
				distinct(ent.entid),
				case when p.empflagestmun = 'm' then 'Municipal' else 'Estadual' end as esfera,
				entd.entcodent || ' - ' || ent.entnome AS escola, 
				ende.estuf, 
				mun.mundescricao, 
				sc.cnenomefantasia as unidadesaude, 
				sc.cneseqequipe as equipe
			FROM 
				entidade.entidade ent				
			LEFT JOIN
				entidade.endereco ende ON ende.entid = ent.entid
			INNER JOIN
				entidade.funcaoentidade feEscola ON feEscola.entid = ent.entid
			INNER JOIN
				entidade.entidadedetalhe entd ON entd.entid = ent.entid		
			INNER JOIN
				territorios.municipio mun ON mun.muncod = ende.muncod
			INNER JOIN
				pse.portariapse pp ON pp.pormunicipio = mun.muncod
			INNER JOIN
				pse.estadomunicipiopse p ON p.muncod = mun.muncod
			INNER JOIN
				territorios.estado est ON est.estuf = mun.estuf
			INNER JOIN
				pse.escolapse ep ON ep.entid = ent.entid
			INNER JOIN
				pse.equipesaudefamilia esf ON esf.espid = ep.espid
			INNER JOIN
				pse.scnes sc ON esf.cneid = sc.cneid
			RIGHT JOIN
				pse.usuarioresponsabilidade rsp ON rsp.entid = ent.entid
			WHERE	
				ent.entstatus='A' 
				AND	feEscola.funid = 3
				AND rsp.pflcod in (320,321) AND rsp.rpustatus = 'A'
				".$where[0]."
				".$where[1]."
				".$where[2]."
				".$where[3]."
				".$where[4]."
				".$where[5]."
			--	and sc.cnecodigocnes = 9999999 --codigo da Equipe de Saude
			--	and ende.estuf = 'TO' --Estado
			--	and mun.muncod = '1721000' --Municipio
			ORDER BY 
				ende.estuf, mun.mundescricao";

	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
		
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
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
		foreach( $_POST['esfera'] as $esfera => $valor ){
			if( $valor == 'm' ){
				$vari = "PSE - Programa Saúde na Escola - Relatório ESF e Escola- Escolas Municipais<br>";
				$i++;
			} elseif( $valor == 'e' ){
				$vari = "PSE - Programa Saúde na Escola - Relatório ESF e Escola- Escolas Estaduais<br>";
				$i++;
			}
		}
		
		if( $i > 1 || $i == 0 ){
			$vari = "PSE - Programa Saúde na Escola - Relatório ESF e Escola- Escolas Estaduais e Municipais<br>";	
		}
	foreach ($agrupador as $val):
		if($count == 1){
			$var = $vari;
		} else {
			$var = "";		
		}
		switch ($val) {/*
			case 1==1:
				array_push($agp['agrupador'], array(
													"label" => "PSE - Programa de Saúde na Escola")										
									   				);				
		   		continue;
		    break;*/
			case 'estado':
				array_push($agp['agrupador'], array(
													"campo" => "estuf",
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
		    case 'equipe':
				array_push($agp['agrupador'], array(
													"campo" => "unidadesaude",
											 		"label" => "$var Equipe de Saúde")										
									   				);					
		    	continue;			
		    break;	
		    case 'escola':
				array_push($agp['agrupador'], array(
													"campo" => "escola",
											 		"label" => "$var INEP - Escola")										
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
