<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	 
<?php
include APPRAIZ. 'includes/classes/relatorio.class.inc';

include_once "LerParametrosFiltroConsulta.php";
include_once "resultSession.php";
 

$sql   = monta_sql();
$dados = $db->carregar($sql);
$agrup = monta_agp();
$col   = monta_coluna();

$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col); 
$r->setBrasao(true); // mostrar cabecalho com brasao
$r->setTotNivel(false); 
$r->setEspandir(true); 
$r->setMonstrarTolizadorNivel(false); 
$r->setTotalizador(false); 
$r->setTolizadorLinha(false); 

echo $r->getRelatorio();

?>
</body>
</html>
<?php 

function monta_sql(){
	global $filtroSession;
//	
	$parametros = new ParametroFiltroConsulta($_POST);
	$parametros->addCondicaoWhereConformeFiltro(" AND pseanoescolamun.pamanoreferencia " , "anoportaria" ); 
	$parametros->addCondicaoWhereConformeFiltro(" AND regiao.regcod  " 				  	 , "regiao" 	 );      
	$parametros->addCondicaoWhereConformeFiltro(" AND estado.estuf " 				     , "estado" 	 );
	$parametros->addCondicaoWhereConformeFiltro(" AND municipio.muncod " 			     , "municipio" 	 );
	$parametros->addCondicaoWhereConformeFiltro(" AND estadomunicipiopse.empflagestmun " , "esfera"		 );
	$parametros->addCondicaoWhereConformeFiltro(" AND estadomunicipiopse.empano " 						 , "exer"		 ); 

	$varClausulaComponentes = montarSubClausulaComponente($parametros); 
	$parametros->addCondicaoWhereCompleta($varClausulaComponentes);
//	
	
	if( usuarioSelecionouAgregadorMunicipio() ) {
		$sql = montarSqlComMunicipio($parametros);
	} else {
		$sql = montarSqlSemMunicipioAgregadoPorUf($parametros); 
	}
	
//	ver ($sql);
//	die();
	
	return $sql;
}

function usuarioSelecionouAgregadorMunicipio() {
	$agrupadores = $_POST['agrupador'];
	$agrupadorMunicipioEncontrado = false;
	foreach ($agrupadores as $agrupador):
		if($agrupador=='municipio') {
			$agrupadorMunicipioEncontrado = true;
			break;
		}
	endforeach;
	return $agrupadorMunicipioEncontrado;
}

function montarSubClausulaComponente($parametros) {
	$varClausulaComponentes = " AND componente.copid IN (";
	
	if( $parametros->estaPreenchido("componente_acao")==false ) {
		$varClausulaComponentes .= "2,3,4,5,6) ";
	} else {
		if( $parametros->verifiqueSeValorPresenteNoAtributo("1","componente_acao") ) {
			$varClausulaComponentes .= "2,3,4,";
		}
		if( $parametros->verifiqueSeValorPresenteNoAtributo("2","componente_acao") ) {
			$varClausulaComponentes .= "5,";
		}
		if( $parametros->verifiqueSeValorPresenteNoAtributo("3","componente_acao") ) {
			$varClausulaComponentes .= "6,";
		}	
		$varClausulaComponentes .= "0) ";
	}
	
	return $varClausulaComponentes;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array("quantidadeprevista"  , 
										  "quantidaderealizada" , 
										  "percentualrealizado"
									 )	  
				);
	
	$count = 1;
		$i = 0;
		foreach( $_POST['esfera'] as $esfera => $valor ){
			if( $valor == 'm' ){
				$vari = "PSE - Programa Saúde na Escola - Relatório Índice Atendimento/Componente- Secretarias Municipais<br>";
				$i++;
			} elseif( $valor == 'e' ){
				$vari = "PSE - Programa Saúde na Escola - Relatório Índice Atendimento/Componente- Secretarias Estaduais<br>";
				$i++;
			}
		}
		
		if( $i > 1 || $i == 0 ){
			$vari = "PSE - Programa Saúde na Escola - Relatório Índice Atendimento/Componente- Secretarias Estaduais e Municipais<br>";	
		}
	foreach ($agrupador as $val):
		if($count == 1){
			$var = $vari;
		} else {
			$var = "";		
		}
		switch ($val) {
			case 'anoportaria':
				array_push($agp['agrupador'], array(
													"campo" => "anoportaria",
											  		"label" => "$var Portaria")										
									   				);				
		   		continue;
		    break;
			
			case 'regiao':
				array_push($agp['agrupador'], array(
													"campo" => "regiao",
											  		"label" => "$var Região")										
									   				);				
		   		continue;
		    break;
		    
		    case 'estado':
				array_push($agp['agrupador'], array(
													"campo" => "uf",
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
		    
		    case 'componente_acao':
				array_push($agp['agrupador'], array(
													"campo" => "descricaocomponente",
											  		"label" => "$var Componente")										
									   				);
				array_push($agp['agrupador'], array(
													"campo" => "acao",
											  		"label" => "$var Ação")										
									   				);													   								
		   		continue;
		    break;

   		    case 'subacao':
				array_push($agp['agrupador'], array(
													"campo" => "subacao",
											  		"label" => "$var Subação")										
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
		}
		$count++;
	endforeach;
	return $agp;
}

function monta_coluna() {

	$arrayBlockAgp = array( "anoportaria" , 
							"regiao"      ,
							"uf"		  ,
							"descricaocomponente" , 
							"acao" 			,
							"percentualrealizado"
	);	    	
	
	$coluna    = array(
						array(
							  "campo" => "quantidadeprevista",
					   		  "label" => "Previsão de atendimento / alunos",
							  "type"  => "numeric" ,
							  "blockAgp" => $arrayBlockAgp 
							  
						),
						array(
							  "campo" => "quantidaderealizada",
					   		  "label" => "Quantidade realizada" ,
							  "type"  => "numeric" ,
							  "blockAgp" => $arrayBlockAgp
							  	
						),					   				   						   	
						array(
							  "campo" => "percentualrealizado",
							  "type"  => "numeric",
					   		  "label" => "Percentual <br>de atendimento (%)",
							  "php"   => array("expressao" => "({quantidadeprevista} == 0)",
											   "var"   => "percentual",
											   "true"  => "0",
							  				   "type"  => "numeric",
											   "false" => "sprintf('%01.1f', round((({quantidaderealizada}/{quantidadeprevista})*100),1) )",
											   "html"  => "{percentual}%") ,
							  "blockAgp" => $arrayBlockAgp  
						)
						
					);
					
	return $coluna;			  	
}

function montarSqlComMunicipio($parametros) {
	$sql = "
		SELECT distinct main.*, case when p.empflagestmun = 'm' then 'Municipal' else 'Estadual' end as esfera 	FROM (
		
			SELECT 	distinct 
				(pseanoescolamun.pamanoreferencia - 1) as anoPortaria , 
				regiao.regdescricao as regiao , 
				estado.estuf as uf ,
				municipio.mundescricao , 
				sum(pseanoescolamun.pamquantprevista) || ' ' as quantidadePrevista ,  
				case 	
					when atividade.copid IN (2,3,4) then 'Componente I' 
					when atividade.copid = 5 	    then 'Componente II' 
					when atividade.copid = 6 	    then 'Componente III' 
				end as descricaoComponente,				
				componente.copdescricao as acao,
				atividade.atvdescricao as subacao, 
				quantidadeescola.qtdescola as quantidadeRealizada, 
				0 as percentualrealizado  
			FROM
				territorios.regiao regiao
						
				inner join territorios.estado estado 
				on regiao.regcod = estado.regcod 
		
				inner join territorios.municipio municipio 
				on estado.estuf = municipio.estuf 				
		
				inner join pse.estadomunicipiopse estadomunicipiopse
				on municipio.muncod = estadomunicipiopse.muncod
		
				inner join pse.pseanoescolamun pseanoescolamun
				on estadomunicipiopse.empid = pseanoescolamun.empid
		
				inner join pse.quantidadeescola quantidadeescola
				on estadomunicipiopse.empid = quantidadeescola.empid
			
				inner join pse.atividade atividade
				on quantidadeescola.atvid = atividade.atvid
			
				inner join pse.componente componente
				on atividade.copid = componente.copid
				
			WHERE	
				0=0
				AND pseanoescolamun.pamanoreferencia IN (
						SELECT distinct 
							(portariapse.porano + 1) as porano
						FROM
							pse.portariapse 
				) 
				
				". $parametros->getCondicoesWhere() . " 
			
			GROUP BY 
				anoPortaria , 
				regiao , 
				uf ,
				municipio.mundescricao , 
				descricaoComponente,
				acao,
				subacao, 
				quantidadeRealizada, 
				percentualrealizado 		
		
			UNION ALL 
			";
	
	$sql .= montarSqlParaQuantidadeEscolaIgualZero(" municipio.mundescricao , ", $parametros);
		
	$sql .= "
		) AS main
		INNER JOIN pse.portariapse port ON port.poruf = main.uf
		INNER JOIN pse.estadomunicipiopse p ON p.muncod = port.pormunicipio
		ORDER BY 
			main.anoPortaria, 
			main.regiao, 
			main.uf,
			main.mundescricao		
	";
	
	//ver($sql);
	
	return $sql;
}

function montarSqlSemMunicipioAgregadoPorUf($parametros) {
	$sql = "
				
		SELECT distinct main.* 	FROM (
	
			SELECT 	distinct 
				quantidadePrevistaPorEstado.pamanoreferenciab as anoPortaria, 
				regiao.regdescricao as regiao ,  
				estado.estuf as uf,
				quantidadePrevistaPorEstado.quantidadePrevista ,  
				case 	
					when atividade.copid IN (2,3,4) then 'Componente I' 
					when atividade.copid = 5 	then 'Componente II' 
					when atividade.copid = 6 	then 'Componente III' 
				end as descricaoComponente,
				componente.copdescricao as acao,
				atividade.atvdescricao as subacao, 		
				quantidadeRealizadaPorEstado.quantidadeRealizada ,
				0 as percentualrealizado 	
							
			FROM
				territorios.regiao regiao
				
				inner join territorios.estado estado 
				on regiao.regcod = estado.regcod 
			
				inner join territorios.municipio municipio 
				on estado.estuf = municipio.estuf 				
			
				left join pse.estadomunicipiopse estadomunicipiopse
				on municipio.muncod = estadomunicipiopse.muncod 
			
				left join pse.pseanoescolamun pseanoescolamun
				on estadomunicipiopse.empid = pseanoescolamun.empid
			
				inner join pse.quantidadeescola quantidadeescola
				on estadomunicipiopse.empid = quantidadeescola.empid
			
				inner join pse.atividade atividade
				on quantidadeescola.atvid = atividade.atvid
			
				inner join pse.componente componente
				on atividade.copid = componente.copid
			
				inner join ( 
					SELECT 
						(pseanoescolamun.pamanoreferencia - 1) as pamanoreferenciab, 
						estado.estuf as uf,
						sum(pseanoescolamun.pamquantprevista) || ' ' as quantidadePrevista 
					FROM
						territorios.regiao regiao
						
						inner join territorios.estado estado 
						on regiao.regcod = estado.regcod 
			
						inner join territorios.municipio municipio 
						on estado.estuf = municipio.estuf
			
						left join pse.estadomunicipiopse estadomunicipiopse
						on municipio.muncod = estadomunicipiopse.muncod 
			
						left join pse.pseanoescolamun pseanoescolamun
						on estadomunicipiopse.empid = pseanoescolamun.empid
					WHERE	
						0=0 
					GROUP BY
						pamanoreferenciab, 
						uf 
					ORDER BY 
						pamanoreferenciab, 
						uf
				) as quantidadePrevistaPorEstado 
				on pseanoescolamun.pamanoreferencia = (quantidadePrevistaPorEstado.pamanoreferenciab + 1) 
				and estado.estuf = quantidadePrevistaPorEstado.uf 

				inner join (
					SELECT distinct realizado.* FROM ( 
							
					" . montarSqlRealizadoCadaAnoPortaria($parametros) ." 

					) AS realizado 
					
				) as quantidadeRealizadaPorEstado
				on pseanoescolamun.pamanoreferencia = (quantidadeRealizadaPorEstado.pamanoreferenciab + 1) 
				and estado.estuf = quantidadeRealizadaPorEstado.uf
				and atividade.atvid = quantidadeRealizadaPorEstado.atvid 				
				
			WHERE	
				0=0
				AND atividade.copid IN (2,3,4,5,6)  
				AND pseanoescolamun.pamanoreferencia IN (
						SELECT distinct 
							(portariapse.porano + 1) as porano
						FROM
							pse.portariapse 
				) 

				". $parametros->getCondicoesWhere() . "
	
			UNION ALL 
			";				
				
	$sql .= montarSqlParaQuantidadeEscolaIgualZero(" ", $parametros);
		
	$sql .= "
	
	) AS main 
	
			
	ORDER BY 
		main.anoPortaria, 
		main.regiao, 
		main.uf	
	";	
	
	return $sql;
}

function montarSqlRealizadoCadaAnoPortaria($parametros) {
	$sqlAnosDePortaria = "
		SELECT distinct 
			(portariapse.porano) as porano
		FROM
			pse.portariapse
		WHERE
			portariapse.porano IS NOT NULL
		";

	global $db;
	$anosDePortaria = $db->carregar($sqlAnosDePortaria);
	
	
//	ver( $anosDePortaria );
//	die();
//	
	
	$sqlResultado = array();
	
	foreach( $anosDePortaria as $ano ):
		$sqlResultado[] = "
			SELECT 	distinct 
				" . $ano['porano'] . " as pamanoreferenciab, 
				estado.estuf as uf,
				atividade.atvid, 		
				sum(quantidadeescola.qtdescola) as quantidadeRealizada 
			FROM
				territorios.regiao regiao
				
				inner join territorios.estado estado 
				on regiao.regcod = estado.regcod 
	
				inner join territorios.municipio municipio 
				on estado.estuf = municipio.estuf
	
				inner join pse.estadomunicipiopse estadomunicipiopse
				on municipio.muncod = estadomunicipiopse.muncod 
	
				inner join pse.quantidadeescola quantidadeescola
				on estadomunicipiopse.empid = quantidadeescola.empid
	
				inner join pse.atividade atividade
				on quantidadeescola.atvid = atividade.atvid
	
				inner join pse.componente componente
				on atividade.copid = componente.copid
			WHERE
				0=0
				" . $parametros->getCondicaoWhereConformeNomeAtributo('regiao')    . "
				" . $parametros->getCondicaoWhereConformeNomeAtributo('estado')    . "
				" . $parametros->getCondicaoWhereConformeNomeAtributo('municipio') . "
			
				" . montarSubClausulaComponente($parametros) . "
				
				AND estadomunicipiopse.empid IN (
					SELECT distinct
						pseanoescolamun.empid
					FROM 
						pse.pseanoescolamun
					WHERE 
						pseanoescolamun.pamanoreferencia  IN  (". $ano['porano'] ." + 1) 	
				)
			GROUP BY 
				pamanoreferenciab, 
				estado.estuf,
				atividade.atvid	
		";
	endforeach;
	
	return implode ( " UNION ALL ", $sqlResultado );
}

function montarSqlParaQuantidadeEscolaIgualZero($camposComplementares,$parametros) {
	$quantidadeEscolaZero = array();
	if( $parametros->estaPreenchido("componente_acao")==false ) {
		$quantidadeEscolaZero[] = montarSubParteUnionAll($camposComplementares,'Componente I'  , $parametros);
		$quantidadeEscolaZero[] = montarSubParteUnionAll($camposComplementares,'Componente II' , $parametros);
		$quantidadeEscolaZero[] = montarSubParteUnionAll($camposComplementares,'Componente III', $parametros);
	} else {
		if( $parametros->verifiqueSeValorPresenteNoAtributo("1","componente_acao") ) {
			$quantidadeEscolaZero[] = montarSubParteUnionAll($camposComplementares,'Componente I'  , $parametros);
		}
		if( $parametros->verifiqueSeValorPresenteNoAtributo("2","componente_acao") ) {
			$quantidadeEscolaZero[] = montarSubParteUnionAll($camposComplementares,'Componente II' , $parametros);
		}
		if( $parametros->verifiqueSeValorPresenteNoAtributo("3","componente_acao") ) {
			$quantidadeEscolaZero[] = montarSubParteUnionAll($camposComplementares,'Componente III' , $parametros);
		}	
	}

	$sqlParteUnionAll = implode( " UNION ALL " , $quantidadeEscolaZero );
	return $sqlParteUnionAll;
}


function montarSubParteUnionAll($camposComplementares, $descricaoComponente, $parametros) {
	$sql = "
			SELECT 	distinct 
				(pseanoescolamun.pamanoreferencia - 1) as anoPortaria , 
				regiao.regdescricao as regiao , 
				estado.estuf as uf ,
				$camposComplementares  
				sum(pseanoescolamun.pamquantprevista) || ' ' as quantidadePrevista ,  
				'$descricaoComponente' as descricaoComponente,
				componente.copdescricao as acao,
				atividade.atvdescricao as subacao,  
				0 as quantidadeRealizada, 
				0 as percentualrealizado  
			FROM
				territorios.regiao regiao
						
				inner join territorios.estado estado 
				on regiao.regcod = estado.regcod 
		
				inner join territorios.municipio municipio 
				on estado.estuf = municipio.estuf 				
		
				inner join pse.estadomunicipiopse estadomunicipiopse
				on municipio.muncod = estadomunicipiopse.muncod
		
				inner join pse.pseanoescolamun pseanoescolamun
				on estadomunicipiopse.empid = pseanoescolamun.empid
		
				full join pse.atividade atividade
				on atividade.copid in (2,3,4,5,6)
		
				inner join pse.componente componente
				on atividade.copid = componente.copid
				
			WHERE	
				0=0
				AND pseanoescolamun.pamanoreferencia IN (
						SELECT distinct 
							(portariapse.porano + 1) as porano
						FROM
							pse.portariapse 
				) 
				
				" . $parametros->getCondicoesWhere() . "
				
				AND estadomunicipiopse.empid in (
					SELECT distinct
						pseanoescolamun.empid
					FROM 
						pse.pseanoescolamun 
				)
		
			GROUP BY 
				anoPortaria , 
				regiao , 
				uf ,
				$camposComplementares
				descricaoComponente,
				acao,
				subacao, 
				quantidadeRealizada, 
				percentualrealizado 	
	";
	return $sql;	
}

?>

<BR><BR><BR></BR>






