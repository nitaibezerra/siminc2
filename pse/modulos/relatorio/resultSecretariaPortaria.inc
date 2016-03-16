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
//Tratamento adicionado para verificar corretamente as pendencias do Ano de Referência na pagina pse2009
if( is_array($dados) ){
	foreach( $dados as $v => $dado ){
		$valores = '';
		$sql = "SELECT (porano + 1) FROM pse.portariapse WHERE pormunicipio = '".$dado['muncod']."'";
		$anoPort = $db->pegaUm( $sql );
		$anos = array();
		//junta os anos q ele deveria ter respondido
		if( $dado['empano'] == $anoPort ){
			array_push( $anos, $anoPort );
			array_push( $anos, $anoPort + 1 );
			array_push( $anos, $anoPort + 2 );
		} elseif( $dado['empano'] == $anoPort + 1 ) {
			array_push( $anos, $anoPort + 1 );
			array_push( $anos, $anoPort + 2 );
		} else {
			array_push( $anos, $anoPort + 2 );
		}

		if( is_array( $anos ) ){
			foreach( $anos as $ano ){
				//se ele tiver pendencia em algum ano q deveria ter respondido ele concatena os anos
				$pos = strpos($dado['pergunta3'], ''.$ano.'');
				if( !($pos === false) ){
					if( $valores ){
						$valores = $valores .', '.$ano;
					} else {
						$valores = $ano;
					}
				}
			}
			
			//coloca os anos pendentes junto com as outras pendencias
			if( $valores ){
				$dados[$v]['pergunta2'] = $dados[$v]['pergunta2'].'PSE '.$valores;
			}
		}
		
		//se for pendencia e se n]ao tiver nada na pergunta ele apaga o index do array, pois nao precisa ser listado
		if( $_POST['portaria'] == 2 ){
			if( $dados[$v]['pergunta2'] == '' ){
				unset($dados[$v]);
			}
		}
	}
}

//reindexa o array
if ($dados){
	$dados = array_values( $dados );
}

//O tratamento encerra aqui.
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
	

	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		$where[0] = " AND f.poruf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}
	if( $municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[1] = " AND f.pormunicipio " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	if( $ano[0] && ( $ano_campo_flag ||$ano_campo_flag == '1' )){
		$where[2] = " AND f.porano " . (( $ano_campo_excludente == null || $ano_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $ano ) . "') ";		
	}
	if($portaria[0] == '1'){
		$where[3] = " AND f.pergunta1 <> '' ";
	} else if ($portaria[0] == '2'){
		$where[3] = " AND ((f.pergunta2 <> '' OR f.pergunta3 <> '') AND pergunta1 = '') ";
	} else if ($portaria[0] == '3'){
		$where[3] = " AND (f.pergunta2 is null OR f.pergunta2 = '') ";
	}
	if( $esfera[0] != '' && ( $esfera_campo_flag ||$esfera_campo_flag == '1' )){
		$where[4] = " AND f.empflagestmun " . (( $esfera_campo_excludente == null || $esfera_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esfera ) . "') ";		
	}
	if( $exer[0] && ( $exer_campo_flag || $exer_campo_flag == '1' )){
		$where[5] = " AND f.empano " . (( $exer_campo_excludente == null || $exer_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $exer ) . "') ";		
	}

	
	$sql = "SELECT 
			--	*, case when f.empflagestmun = 'm' then 'Municipal' else 'Estadual' end as esfera
				DISTINCT porid, pormunicipio, poruf,empid, muncod, empano, mundescricao, pergunta1, pergunta2, pergunta3, case when f.empflagestmun = 'm' then 'Municipal' else 'Estadual' end as esfera
				
				FROM 
				(SELECT 
				p.*,est.*,mun.mundescricao || ' - ' || (case when est.empflagestmun = 'm' then 'Municipal' else 'Estadual' end) as mundescricao, 
				(case when 
						(select count(a.empid) from pse.estadomunicipiopse a inner join pse.secretariaestmun b on b.empid=a.empid
						where a.muncod=mun.muncod and a.empflagestmun=est.empflagestmun) = 0
					then
						'Não Acessou o PSE 2009<br>'
					else
						''	
					end) as pergunta1,
					(case when
						(select count(p.perid) from pse.pergunta p inner join pse.resposta r on r.perid=p.perid inner join pse.estadomunicipiopse e on e.empid=r.empid	
						where p.perid = 1 and e.muncod=mun.muncod and e.empflagestmun = est.empflagestmun) = 0
					then
						'Representante<br>'
					else
						''	
					end ||
					case when
						(select count(r.resid) from pse.pergunta p inner join pse.resposta r on r.perid=p.perid inner join pse.estadomunicipiopse e on e.empid=r.empid	
						where p.perid IN ('2','3','4','5') and e.muncod=mun.muncod and e.empflagestmun = est.empflagestmun) = 0
					then
						'Grupo Gestor<br>'
					else
						''	
					end ||
					case when
						(select count(r.resid) from pse.pergunta p inner join pse.resposta r on r.perid=p.perid inner join pse.estadomunicipiopse e on e.empid=r.empid	
						where p.perid IN ('6','7','8','9') and e.muncod=mun.muncod and e.empflagestmun = est.empflagestmun) = 0
					then	
						'Projeto<br>'
					else
						''	
					end ||
					case when
						(select count(r.resid) from pse.pergunta p inner join pse.resposta r on r.perid=p.perid inner join pse.estadomunicipiopse e on e.empid=r.empid	
						where p.perid IN ('10','11','12','13') and e.muncod=mun.muncod and e.empflagestmun = est.empflagestmun) = 0
					then
						'Gestão<br>'
					else
						''	
					end ||
					case when 
						(select count(e.qtdid) from pse.quantidadeescola e left join pse.atividade a on a.atvid=e.atvid inner join pse.componente c on c.copid=a.copid
						inner join pse.estadomunicipiopse d on d.empid=e.empid where d.muncod = mun.muncod and c.copid IN ('2','3','4') and d.empflagestmun = est.empflagestmun) = 0
					then
						'Componente 01<br>'
					else
						''	
					end ||
					case when
						(select count(e.qtdid) from pse.quantidadeescola e left join pse.atividade a on a.atvid=e.atvid inner join pse.componente c on c.copid=a.copid
						inner join pse.estadomunicipiopse d on d.empid=e.empid where d.muncod = mun.muncod and c.copid = 5 and d.empflagestmun = est.empflagestmun) = 0
					then
						'Componente 02<br>'
					else
						''	
					end ||
					case when
						(select count(e.qtdid) from pse.quantidadeescola e left join pse.atividade a on a.atvid=e.atvid inner join pse.componente c on c.copid=a.copid
						inner join pse.estadomunicipiopse d on d.empid=e.empid where d.muncod = mun.muncod and c.copid = 6 and d.empflagestmun = est.empflagestmun) = 0
					then
						'Componente 03<br>'
					else
						''	
					end ||
					case when
						(	select 
								count(resid) 
							from 
								pse.resposta
									
								inner join pse.estadomunicipiopse d 
								on resposta.empid = d.empid  
								
							where 
								d.muncod = mun.muncod
								and 
								resposta.perid IN (20,21)
								and d.empflagestmun = est.empflagestmun
						) = 0
					then
						'Componente 04<br>'
					else
						''	
					end ||
					case when
						(select count(iteid) from pse.itemselecionado i inner join pse.resposta r on r.resid = i.resid inner join pse.pergunta p on p.perid=r.perid
						inner join pse.estadomunicipiopse e on e.empid=r.empid where p.perid = 19 and e.muncod=mun.muncod and e.empflagestmun = est.empflagestmun) = 0
					then
						'Material Clínico e Didático<br>'
					else
						''	
					end) AS pergunta2, 
					(case when 
						((select count(pse.pamid) from pse.pseanoescolamun pse inner join pse.estadomunicipiopse d ON d.empid=pse.empid
						where d.muncod=mun.muncod and est.empid=d.empid and pse.pamanoreferencia=2009 and d.empflagestmun = est.empflagestmun) = 0 AND (select part.empparticipapse2009 from pse.estadomunicipiopse part where part.muncod = mun.muncod and part.empflagestmun = est.empflagestmun limit 1) = '1')
					then
						'2009, '
					else
						''	
					end ||
					case when
						(select count(pse.pamid) from pse.pseanoescolamun pse inner join pse.estadomunicipiopse d ON d.empid=pse.empid
						where d.muncod=mun.muncod and est.empid=d.empid and pse.pamanoreferencia=2010 and d.empflagestmun = est.empflagestmun) = 0
					then
						'2010, '
					else
						''	
					end ||
					case when
						(select count(pse.pamid) from pse.pseanoescolamun pse inner join pse.estadomunicipiopse d ON d.empid=pse.empid
						where d.muncod=mun.muncod and est.empid=d.empid and pse.pamanoreferencia=2011 and d.empflagestmun = est.empflagestmun) = 0
					then
						'2011<br>'
					else
						''	
					--end ||
					--case when
					--	(select count(pse.pamid) from pse.pseanoescolamun pse inner join pse.estadomunicipiopse d ON d.empid=pse.empid
					--	where d.muncod=mun.muncod and est.empid=d.empid and pse.pamanoreferencia=2012 and d.empflagestmun = est.empflagestmun) = 0
					--then
					--	'2012<br>'
					--else
					--	''	
					--end ||
					--case when
					--	(select count(pse.pamid) from pse.pseanoescolamun pse inner join pse.estadomunicipiopse d ON d.empid=pse.empid
					--	where d.muncod=mun.muncod and est.empid=d.empid and pse.pamanoreferencia=2013 and d.empflagestmun = est.empflagestmun) = 0
					--then
					--	'2013<br>'
					--else
					--	''	
					end) AS pergunta3
			FROM
				pse.portariapse p
			INNER JOIN territorios.municipio mun ON mun.muncod = p.pormunicipio
		--	INNER JOIN entidade.endereco ende ON ende.muncod = mun.muncod
		--	INNER JOIN pse.escolapse ep ON ep.entid = ende.entid
			INNER JOIN pse.estadomunicipiopse est ON est.muncod = p.pormunicipio
			)AS f
			WHERE
						f.poruf <> ''
						".$where[0]."
						".$where[1]."
						".$where[2]."
						".$where[3]."
						".$where[4]."
						".$where[5]."
						ORDER BY
							poruf, mundescricao";
	//ver ($sql);
	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
		
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"pergunta2"
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
				$vari = "- Secretarias Municipais<br>";
				$i++;
			} elseif( $valor == 'e' ){
				$vari = "- Secretarias Estaduais<br>";
				$i++;
			}
		}

		if( $i > 1 || $i == 0 ){
			$vari = "- Secretarias Estaduais e Municipais<br>";	
		}
	foreach ($agrupador as $val):
		if($count == 1){
			if($_POST['portaria'] == "1"){
				$var = "PSE - Programa Saúde na Escola - Relação de Secretarias que não fizeram o cadastro no PSE $vari<br>";
			} else if ($_POST['portaria'] == "2"){
				$var = "PSE - Programa Saúde na Escola - Relação de Secretarias com pendência no PSE $vari<br>";
			} else if ($_POST['portaria'] == "3"){
				$var = "PSE - Programa Saúde na Escola - Relação de Secretarias com cadastro completo no PSE $vari<br>";
			}
		} else {
			$var = "";		
		}
		switch ($val) {
			case 'estado':
				array_push($agp['agrupador'], array(
													"campo" => "poruf",
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
		    case 'exerc':
				array_push($agp['agrupador'], array(
													"campo" => "empano",
											  		"label" => "$var Exercício")										
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
	if($_POST['portaria'] == '2'){
		$coluna    = array(
							array(
								  "campo" => "pergunta2",
						   		  "label" => "Pendências no PSE",
								  "type"  => "string"
							)
					);
	}
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
