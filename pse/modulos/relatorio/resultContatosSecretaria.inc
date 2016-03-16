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
$r->setTolizadorLinha(false);
//$r->setBrasao(true);

if($_POST['tipo_relatorio'] == 'xls'){
	ob_clean();
	$nomeDoArquivoXls="relatorio_Contatos_Secretaria_".date('d-m-Y_H_i');
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
		$where[0] = " AND ende.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}
	if( $municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[1] = " AND mun.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	
	
	
	if( $tiporelatorio == 1 ){
		
		$sql = "select 'Educação' as tipo, consenomesecretario as nome, conseemailsecretario as emailsecretario, conseendereco as endereco, '('||conseddd||') ' || consetelefone as fone, conseemailsecretaria as emailsecretaria,
						mun.estuf as estado, mun.mundescricao as municipio
				from pse.contratualizacao co
				inner join entidade.entidade ent on ent.entid = co.entid
				inner join entidade.endereco ende on ende.entid = ent.entid
				inner join territorios.municipio mun on mun.muncod = ende.muncod
				inner join entidade.funcaoentidade fun on fun.entid = ende.entid AND fun.funid = 7
				where fun.fuestatus='A'
				".$where[0]."
				".$where[1]."
				ORDER BY 7, 8, 1, 2";
		
	} elseif( $tiporelatorio == 2 ){
		
		$sql = "select 'Saúde' as tipo, conssnomesecretario as nome, conssemailsecretario as emailsecretario, conssendereco as endereco, '('||conssddd||') ' || consstelefone as fone, conssemailsecretaria as emailsecretaria,
						mun.estuf as estado, mun.mundescricao as municipio
				from pse.contratualizacao co
				inner join entidade.entidade ent on ent.entid = co.entid
				inner join entidade.endereco ende on ende.entid = ent.entid
				inner join territorios.municipio mun on mun.muncod = ende.muncod
				inner join entidade.funcaoentidade fun on fun.entid = ende.entid AND fun.funid = 7
				where fun.fuestatus='A'
				".$where[0]."
				".$where[1]."
				ORDER BY 7, 8, 1, 2";
		
	} else {
		
		$sql = "select 'Saúde' as tipo, conssnomesecretario as nome, conssemailsecretario as emailsecretario, conssendereco as endereco, '('||conssddd||') ' || consstelefone as fone, conssemailsecretaria as emailsecretaria,
						mun.estuf as estado, mun.mundescricao as municipio
				from pse.contratualizacao co
				inner join entidade.entidade ent on ent.entid = co.entid
				inner join entidade.endereco ende on ende.entid = ent.entid
				inner join territorios.municipio mun on mun.muncod = ende.muncod
				inner join entidade.funcaoentidade fun on fun.entid = ende.entid AND fun.funid = 7
				where fun.fuestatus='A'
				".$where[0]."
				".$where[1]."
						
				union
					
				select 'Educação' as tipo, consenomesecretario as nome, conseemailsecretario as emailsecretario, conseendereco as endereco, '('||conseddd||') ' || consetelefone as fone, conseemailsecretaria as emailsecretaria,
						mun.estuf as estado, mun.mundescricao as municipio
				from pse.contratualizacao co
				inner join entidade.entidade ent on ent.entid = co.entid
				inner join entidade.endereco ende on ende.entid = ent.entid
				inner join territorios.municipio mun on mun.muncod = ende.muncod
				inner join entidade.funcaoentidade fun on fun.entid = ende.entid AND fun.funid = 7
				where fun.fuestatus='A'
				".$where[0]."
				".$where[1]."
				
				ORDER BY 7, 8, 1, 2";
		
	}
	//ver ($sql, d);
	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"nome",
											"emailsecretario",
											"fone",
											"emailsecretaria",
											"endereco"		
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
		}
		$count++;
	endforeach;
	
	array_push($agp['agrupador'], array(
													"campo" => "tipo",
											  		"label" => "Tipo")										
									   				);				
	
	return $agp;
}

function monta_coluna(){
	
	$coluna    = array(
							array(
								  "campo" => "nome",
						   		  "label" => "Nome do Secretário(a)",
								  "type"  => "string"
							)
					);
	
	array_push($coluna, array(
									  "campo" => "emailsecretario",
								 	  "label" => "E-mail do Secretário(a)",
								  	  "type"  => "string"	
								)										
					  );
	array_push($coluna, array(
									  "campo" => "endereco",
								 	  "label" => "Endereço da Secretária",
								  	  "type"  => "string"	
								)										
					  );
	array_push($coluna, array(
									  "campo" => "fone",
								 	  "label" => "Telefone da Secretária",
								  	  "type"  => "string"	
								)										
					  );
	array_push($coluna, array(
									  "campo" => "emailsecretaria",
								 	  "label" => "E-mail da Secretária",
								  	  "type"  => "string"	
								)										
					  );	
					
	return $coluna;			  	
	
}
?>