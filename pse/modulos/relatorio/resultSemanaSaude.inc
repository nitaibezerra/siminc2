<?

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
	
	extract($_POST);
	
	$select = array();
	$from	= array();
	

	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		$where[0] = " AND ende.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}
	if( $municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[1] = " AND mun.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	
	$where[2] = " AND mp.mssdataacordo is not null ";
	
	/*
	if($tiporelatorio[0] == '1'){
		$where[2] = " AND mp.mssdataacordo is not null ";
	}	
	elseif($tiporelatorio[0] == '2'){
		$where[2] = " AND mp.mssdataacordo is null ";
	}
	*/
	
	
	/*
	$sql = "select et.entid, et.entnome from entidade.entidade et
			inner join entidade.endereco ed on ed.entid=et.entid
			inner join entidade.funcaoentidade fe on  fe.entid = et.entid AND fe.funid = 7
			where fe.fuestatus='A' and muncod = '".$_SESSION['pse']['muncod']."'";
	$secretaria = $db->pegaLinha($sql);
	*/	

		$sql = "SELECT
					ent.entid,
					ent.entnome as secretaria,
					mun.muncod as muncod,
					mun.mundescricao as municipio,
					mun.estuf as estado 
				FROM
					pse.municipioparsse mp
				INNER JOIN pse.contratualizacao c ON c.conid = mp.conid
				INNER JOIN entidade.entidade ent ON ent.entid = c.entid
				INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
				INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
				WHERE
				1=1
				".$where[0]."
				".$where[1]."
				".$where[2]."
				ORDER BY estado, municipio, secretaria";
	
	//ver ($sql, d);
	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"secretaria"		
										  )	  
				);
				
				
	foreach ($agrupador as $val):
		
		switch ($val) {
			case 'estado':
				array_push($agp['agrupador'], array(
													"campo" => "estado",
											  		"label" => "Estado(s)")										
									   				);				
		   		continue;
		    break;
		    case 'municipio':
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
											  		"label" => "Município(s)")										
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
							array(
								  "campo" => "secretaria",
						   		  "label" => "Secretária(s)",
								  "type"  => "string"
							)
					);
	
											   		
	return $coluna;			  	
	
}
?>