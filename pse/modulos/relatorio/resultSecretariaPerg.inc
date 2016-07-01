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
	

	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		$where[0] = " AND mun.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}
	if( $municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[1] = " AND mun.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	if( $portaria[0] && ( $portaria_campo_flag ||$portaria_campo_flag == '1' )){
		$where[2] = " AND port.porano " . (( $portaria_campo_excludente == null || $portaria_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $portaria ) . "') ";		
	}
	if( $esfera[0] != '' && ( $esfera_campo_flag ||$esfera_campo_flag == '1' )){
		$where[3] = " AND m.empflagestmun " . (( $esfera_campo_excludente == null || $esfera_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esfera ) . "') ";		
	}
	if( $exer[0] && ( $exer_campo_flag || $exer_campo_flag == '1' )){
		$where[4] = " AND m.empano " . (( $exer_campo_excludente == null || $exer_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $exer ) . "') ";		
	}
	
	$sql = "SELECT distinct
							mun.estuf as estado, mun.mundescricao as municipio,
							case when m.empflagestmun = 'm' then 'Municipal' else 'Estadual' end as esfera,
						
								(case when 
									(select count(a.empid) from pse.estadomunicipiopse a inner join pse.secretariaestmun b on b.empid=a.empid
									where a.muncod=mun.muncod) = 0
								then
									'Identificação da Secretaria<br>'
								else
									''	
								end ||
								case when
									(select count(p.perid) from pse.pergunta p inner join pse.resposta r on r.perid=p.perid inner join pse.estadomunicipiopse e on e.empid=r.empid	
									where p.perid = 1 and e.muncod=mun.muncod) = 0
								then
									'Representante<br>'
								else
									''	
								end ||
								case when
									(select count(r.resid) from pse.pergunta p inner join pse.resposta r on r.perid=p.perid inner join pse.estadomunicipiopse e on e.empid=r.empid	
									where p.perid IN ('2','3','4','5') and e.muncod=mun.muncod) = 0
								then
									'Grupo Gestor<br>'
								else
									''	
								end ||
								case when
									(select count(r.resid) from pse.pergunta p inner join pse.resposta r on r.perid=p.perid inner join pse.estadomunicipiopse e on e.empid=r.empid	
									where p.perid IN ('6','7','8','9') and e.muncod=mun.muncod) = 0
								then	
									'Projeto<br>'
								else
									''	
								end ||
								case when
									(select count(r.resid) from pse.pergunta p inner join pse.resposta r on r.perid=p.perid inner join pse.estadomunicipiopse e on e.empid=r.empid	
									where p.perid IN ('10','11','12','13') and e.muncod=mun.muncod) = 0
								then
									'Gestão<br>'
								else
									''	
								end ||
								case when
									(select count(r.resid) from pse.pergunta p inner join pse.resposta r on r.perid=p.perid inner join pse.estadomunicipiopse e on e.empid=r.empid	
									where p.perid IN ('14','15','16','17') and e.muncod=mun.muncod) = 0
								then
									'Educação/Capacitação<br>'
								else
									''	
								end ||
								case when
									(select count(pse.pamid) from pse.pseanoescolamun pse inner join pse.estadomunicipiopse est ON est.empid=pse.empid
									where est.muncod=mun.muncod and pse.pamanoreferencia=2009) = 0
								then
									'PSE 2009<br>'
								else
									''	
								end ||
								case when
									(select count(pse.pamid) from pse.pseanoescolamun pse inner join pse.estadomunicipiopse est ON est.empid=pse.empid
									where est.muncod=mun.muncod and pse.pamanoreferencia=2010) = 0
								then
									'PSE 2010<br>'
								else
									''	
								end ||
								case when
									(select count(pse.pamid) from pse.pseanoescolamun pse inner join pse.estadomunicipiopse est ON est.empid=pse.empid
									where est.muncod=mun.muncod and pse.pamanoreferencia=2011) = 0
								then
									'PSE 2011<br>'
								else
									''	
								end ||
								case when
									(select count(e.qtdid) from pse.quantidadeescola e left join pse.atividade a on a.atvid=e.atvid inner join pse.componente c on c.copid=a.copid
									inner join pse.estadomunicipiopse d on d.empid=e.empid where d.muncod = m.muncod and c.copid IN ('2','3','4')) = 0
								then
									'Componente 01<br>'
								else
									''	
								end ||
								case when
									(select count(e.qtdid) from pse.quantidadeescola e left join pse.atividade a on a.atvid=e.atvid inner join pse.componente c on c.copid=a.copid
									inner join pse.estadomunicipiopse d on d.empid=e.empid where d.muncod = m.muncod and c.copid = 5) = 0
								then
									'Componente 02<br>'
								else
									''	
								end ||
								case when
									(select count(e.qtdid) from pse.quantidadeescola e left join pse.atividade a on a.atvid=e.atvid inner join pse.componente c on c.copid=a.copid
									inner join pse.estadomunicipiopse d on d.empid=e.empid where d.muncod = m.muncod and c.copid = 6) = 0
								then
									'Componente 03<br>'
								else
									''	
								end ||
								case when
									(select count(e.qtdid) from pse.quantidadeescola e left join pse.atividade a on a.atvid=e.atvid inner join pse.componente c on c.copid=a.copid
									inner join pse.estadomunicipiopse d on d.empid=e.empid where d.muncod = m.muncod and c.copid = 8) = 0
								then
									'Componente 04<br>'
								else
									''	
								end ||
								case when
									(select count (i.iteid) from pse.itemselecionado i inner join pse.resposta r on r.resid = i.resid inner join pse.pergunta p on p.perid=r.perid
									inner join pse.estadomunicipiopse e on e.empid=r.empid where p.perid = 19 and e.muncod=m.muncod and e.empflagestmun = 'm') = 0
								then
									'Material Clínico e Didático<br>'
								else
									''	
								end) AS pergunta
						
						FROM
							pse.portariapse port
						INNER JOIN pse.estadomunicipiopse m ON port.pormunicipio = m.muncod
						INNER JOIN territorios.municipio mun ON mun.muncod = m.muncod
						--INNER JOIN entidade.endereco ende ON ende.muncod = mun.muncod
						--INNER JOIN pse.escolapse ep ON ep.entid = ende.entid
						WHERE
							mun.estuf <>''
							".$where[0]."
							".$where[1]."
							".$where[2]."
							".$where[3]."
							".$where[4]."
						ORDER BY
							mun.estuf, mun.mundescricao";

	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
		
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"pergunta"
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
		foreach( $_POST['esfera'] as $esfera => $valor ){
			if( $valor == 'm' ){
				$vari = "PSE - Programa Saúde na Escola - Relatório de Secretarias- Secretarias Municipais<br>";
				$i++;
			} elseif( $valor == 'e' ){
				$vari = "PSE - Programa Saúde na Escola - Relatório de Secretarias- Secretarias Estaduais<br>";
				$i++;
			}
		}
		
		if( $i > 1 || $i == 0 ){
			$vari = "PSE - Programa Saúde na Escola - Relatório de Secretarias- Secretarias Estaduais e Municipais<br>";	
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
													"campo" => "estado",
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
//		    case 'nivel':
//				array_push($agp['agrupador'], array(
//													"campo" => "nivdescricao",
//											 		"label" => "$var Nível de Ensino")										
//									   				);					
//		    	continue;			
//		    break;
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
							  "campo" => "pergunta",
					   		  "label" => "Não preenchidas",
							  "type"  => "string"
							 // "html"  	 => "<span style='color:{color}'>{equipe}</span>",
						)
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
