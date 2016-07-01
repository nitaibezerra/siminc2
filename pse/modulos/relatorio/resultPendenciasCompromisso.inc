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
	<script>
		//function mostraTermo(conid, muncod)
		function mostraTermo(arqid)
		{
			//window.open('pse.php?modulo=relatorio/formPendenciasCompromisso&acao=A&termo=ok&conid='+conid+'&muncod='+muncod, 'termo', 'height=600, width=780, scrollbars=1');
			window.open('pse.php?modulo=relatorio/formPendenciasCompromisso&acao=A&arqid='+arqid);
		}
	</script>	
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
				$dados[$v]['pendencia2'] = $dados[$v]['pendencia2'].'PSE '.$valores;
			}
		}
		
		//se for pendencia e se n]ao tiver nada na pergunta ele apaga o index do array, pois nao precisa ser listado
		if( $_POST['tiporelatorio'] == 2 ){
			if( $dados[$v]['pendencia2'] == '' ){
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
		$where[0] = " AND ende.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}
	if( $municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[1] = " AND mun.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	if( $ano[0] && ( $ano_campo_flag ||$ano_campo_flag == '1' )){
		$where[2] = " AND port.porano " . (( $ano_campo_excludente == null || $ano_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $ano ) . "') ";		
	}
	
	
	if ($tiporelatorio[0] == '3'){
		$where[3] = " AND (select sum(plana.pamquanteduseratend) FROM pse.planoacaoc1 plana WHERE plana.conid = plan.conid) >= c.concoberturacomp1 
					AND (select sum(planac.pacquanteduseratend) FROM pse.planoacaoc2 planac WHERE planac.conid = plano.conid) >= c.concoberturacomp2  ";
	}
	elseif($tiporelatorio[0] == '2'){
		if($tipoanexo == '2') $where[3] = " AND (c.arqid) IS NOT NULL ";
		if($tipoanexo == '3') $where[3] = " AND (c.arqid) IS NULL ";
	}
	
	
	if( $tiporelatorio == 1 ){
		$sql = "select 
					ende.entid,
					port.porano as portaria,
					mun.muncod as muncod,
					mun.mundescricao as municipio,
					mun.estuf as estado
				from pse.portariapse port
				inner join entidade.endereco ende on port.pormunicipio = ende.muncod
				inner join territorios.municipio mun on mun.muncod = ende.muncod
				inner join entidade.funcaoentidade fun on fun.entid = ende.entid
				where fun.funid = 7
				AND ende.entid not in (select entid from pse.contratualizacao)
				".$where[0]."
				".$where[1]."
				".$where[2]."
				ORDER BY portaria, estado, municipio";
	} elseif( $tiporelatorio == 2 ){
		$sql = "SELECT
					ende.entid,
					port.porano as portaria,
					mun.muncod as muncod,
					mun.mundescricao as municipio,
					mun.estuf as estado,
					(case when 
							(c.conesfatuarapse) IS NULL
					 then
							'Território de Responsabilidade<br>'
					 else
							''	
					 end ||
					 case when 
							(select sum(plan.pamquanteduseratend) from pse.planoacaoc1 plan 
							where plan.conid = c.conid) >= c.concoberturacomp1
					 then
							''
					 else
							'Plano de Ação - Componente 1<br>'	
					 end ||
					 case when 
							(select sum(plan.pacquanteduseratend) from pse.planoacaoc2 plan 
							where plan.conid = c.conid) >= c.concoberturacomp2
					 then
							''
					 else
							'Plano de Ação - Componente 2<br>'	
					 end ||
					 case when 
							(c.arqid) IS NULL
					 then
							'Não Anexou o Termo'
					 else
							''	
					 end ||
					 case when 
							(select count(mt.mpaid) from pse.metapactuada mt
							where mt.conid = c.conid) = 0
					 then
							'Pactuação<br>'
					 else
							''	
					 end ) AS pendencia2,

					 c.consenomesecretario as c1,
					 '('||c.conseddd||') ' || c.consetelefone as c2,
					 c.conseemailsecretaria as c3,
					 c.congtisenome as c4,
					 '('||c.congtiseddd||') ' || c.congtisetelefone as c5,
					 c.congtiseemail as c6,
					 
					 c.conssnomesecretario as c7,
					 '('||c.conssddd||') ' || c.consstelefone as c8,
					 c.conssemailsecretaria as c9,
					 c.congtissnome as c10,
					 '('||c.congtissddd||') ' || c.congtisstelefone as c11,
					 c.congtissemail as c12
					 
				FROM
					entidade.entidade ent
				INNER JOIN pse.contratualizacao c ON c.entid = ent.entid
				INNER JOIN entidade.endereco ende ON ende.entid = c.entid
				INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
				INNER JOIN pse.portariapse port ON port.pormunicipio = ende.muncod
				WHERE
				1=1
				".$where[0]."
				".$where[1]."
				".$where[2]."
				".$where[3]."
				ORDER BY portaria, estado, municipio";
	} elseif( $tiporelatorio == 3 ) {
		$sql = "SELECT
					ent.entid,
					port.porano as portaria,
					mun.muncod as muncod,
					mun.mundescricao as municipio,
					mun.estuf as estado,
					c.concoberturacomp1 as total1, 
					sum(plan.pamquanteduseratend) as total2, 
					c.concoberturacomp2 as total3, 
					sum(plano.pacquanteduseratend) as total4,
					(case when 
							(c.arqid) IS NULL
					 then
							''
					 else
							'<img border=\"0\" title=\"Visualizar Termo de Compromisso.\" src=\"../imagens/consultar.gif\" style=\"cursor: hand\" onclick=\"mostraTermo(\''||c.arqid||'\');\">'	
					 end) as termo
				FROM
					entidade.entidade ent
				INNER JOIN pse.contratualizacao c ON c.entid = ent.entid
				INNER JOIN pse.planoacaoc1 plan ON plan.conid = c.conid
				INNER JOIN pse.planoacaoc2 plano ON plano.conid = c.conid
				INNER JOIN entidade.endereco ende ON ende.entid = c.entid
				INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
				INNER JOIN pse.portariapse port ON port.pormunicipio = ende.muncod
				WHERE
				c.arqid is not null
				".$where[0]."
				".$where[1]."
				".$where[2]."
				".$where[3]."
				GROUP BY
					c.concoberturacomp1, c.concoberturacomp2, ent.entid, porano, mun.mundescricao,mun.estuf,mun.muncod, c.arqid, c.conid
				ORDER BY portaria, estado, municipio";
	}
	//ver ($sql, d);
	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"pendencia2",
											"termo",
											"c1",
											"c2",
											"c3",
											"c4",
											"c5",
											"c6",
											"c7",
											"c8",
											"c9",
											"c10",
											"c11",								
											"c12"		
										  )	  
				);
				
				
	foreach ($agrupador as $val):
		if($count == 1){
			if($_POST['tiporelatorio'] == '1'){
				$var = "PSE - Programa Saúde na Escola - Relação de Secretarias que não preencheram o Termo de Compromisso.<br>";
			} else if ($_POST['tiporelatorio'] == '2'){
				$var = "PSE - Programa Saúde na Escola - Relação de Secretarias que possuem pendências no Termo de Compromisso.<br>";
			} else if ($_POST['tiporelatorio'] == '3'){
				$var = "PSE - Programa Saúde na Escola - Relação de Secretarias sem prendências no Termo de Compromisso.<br>";
			}
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
		    case 'portar':
				array_push($agp['agrupador'], array(
													"campo" => "portaria",
											  		"label" => "$var Portaria")										
									   				);					
		    	continue;
		    break;			    	
		}
		$count++;
	endforeach;
	return $agp;
}

function monta_coluna(){
	
	
	
	if($_POST['tiporelatorio'] == '2'){
		$coluna    = array(
							array(
								  "campo" => "pendencia2",
						   		  "label" => "Pendências",
								  "type"  => "string"
							)
					);
					
		if($_POST['tiposecretario'] == '1'){	
			array_push($coluna, array(
									  "campo" => "c1",
								 	  "label" => "Nome do Secretário de Educação",
								  	  "type"  => "string"	
								)										
					  );
			array_push($coluna, array(
									  "campo" => "c2",
								 	  "label" => "Telefone da Secretaria de Educação",
								  	  "type"  => "string"	
								)										
					  );
			array_push($coluna, array(
									  "campo" => "c3",
								 	  "label" => "E-mail da Secretaria de Educação",
								  	  "type"  => "string"	
								)										
					  );
			array_push($coluna, array(
									  "campo" => "c4",
								 	  "label" => "Representante da Secretaria de Educação",
								  	  "type"  => "string"	
								)										
					  );
			array_push($coluna, array(
									  "campo" => "c5",
								 	  "label" => "Telefone do Representante da Secretaria de Educação",
								  	  "type"  => "string"	
								)										
					  );
			array_push($coluna, array(
									  "campo" => "c6",
								 	  "label" => "E-mail do Representante da Secretaria de Educação",
								  	  "type"  => "string"	
								)										
					  );
			array_push($coluna, array(
									  "campo" => "c7",
								 	  "label" => "Nome do Secretário de Saúde",
								  	  "type"  => "string"	
								)										
					  );
			array_push($coluna, array(
									  "campo" => "c8",
								 	  "label" => "Telefone da Secretaria de Saúde",
								  	  "type"  => "string"	
								)										
					  );
			array_push($coluna, array(
									  "campo" => "c9",
								 	  "label" => "E-mail da Secretaria de Saúde",
								  	  "type"  => "string"	
								)										
					  );
			array_push($coluna, array(
									  "campo" => "c10",
								 	  "label" => "Representante da Secretaria de Saúde",
								  	  "type"  => "string"	
								)										
					  );
			array_push($coluna, array(
									  "campo" => "c11",
								 	  "label" => "Telefone do Representante da Secretaria de Saúde",
								  	  "type"  => "string"	
								)										
					  );
			array_push($coluna, array(
									  "campo" => "c12",
								 	  "label" => "E-mail do Representante da Secretaria de Saúde",
								  	  "type"  => "string"	
								)										
					  );
					  
		}	
		
	}
	
	if($_POST['tiporelatorio'] == '3'){
		$coluna    = array(
							array(
								  "campo" => "termo",
						   		  "label" => "Termo",
								  "type"  => "string"
							)
					);
	}
											   		
	return $coluna;			  	
	
}
?>