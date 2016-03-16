<?php
	if ($_REQUEST['filtrosession']){
		$filtroSession = $_REQUEST['filtrosession'];
	}
	
	if ($_POST['agrupador']){
		header('Content-Type: text/html; charset=iso-8859-1'); 
	}
	ini_set("memory_limit","500M");
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

	if($_POST['tipo_relatorio'] == 'xls'){
		ob_clean();
		$nomeDoArquivoXls="relatorio_analitico_".date('d-m-Y_H_i');
		echo $r->getRelatorioXls();
	}elseif($_POST['tipo_relatorio'] == 'visual'){
		echo $r->getRelatorio();	
	}
?>

</body>
</html>

<?php 
function monta_sql(){
	global $filtroSession;
	extract($_POST);
	
	if( $estuf[0] && ( $estuf_campo_flag || $estuf_campo_flag == '1' )){		
		$where[0] = " and pce.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') ";		
	}
	
	if( $municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[1] = " and m.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	
	if( $area != ''){
		$where[2] = " AND pce.ateid = $area ";		
	}
	
	if( $curso != ''){
		$where[3] = " AND pce.curid = $curso ";		
	}	

	if( $modalidade != ''){
		$where[4] = " AND pce.modid = $modalidade ";		
	}

	if( $demanda_social != ''){
		$where[5] = " AND pce.pridemsoc = '$demanda_social' ";		
	}
	
	$sql = "SELECT  pce.pdeid,
					pce.pdicodinep,
		            pce.pdenome,
		            pce.pdiesfera,
		            pce.estuf,
		            m.mundescricao,
		            pce.pdilocalizacao,
		            pce.atedesc,
		            pce.curdesc,
		            pce.ncudesc,
		            pce.moddesc,
		            pce.pcfano,
		            pce.privagassolicitadas,
		            pce.privagasprevistas
		  	FROM   	snf.prioridadecursoescola pce 
			INNER JOIN territorios.municipio m USING(muncod)
			WHERE pce.pristatus = 'A'
		  	AND pce.pcfano in (2012,2013) -- (Ano de referência) 
		--Estado
		".$where[0]."
		--Município.
		".$where[1]."
		--Área Temática.
		".$where[2]."
		--Curso.
		".$where[3]."
		--Modalidade.
		".$where[4]."
		--Demanda Social
		".$where[5]."";
	
	return $sql;
}

function monta_coluna(){
	
	$colunas = $_POST['colunas'];
	$colunas = $colunas ? $colunas : array();
	
	$coluna = array();
	
	foreach ($colunas as $val){
		switch ($val) {
			case 'estuf':
				array_push( $coluna,
								array(	"campo"		=> "estuf",
										"label" 	=> "Unidade Federativa",
									   	"type"	  	=> "string"
								)
				);
				continue;
			break;
			case 'mundescricao':
				array_push( $coluna, 
								array(	"campo" 	=> "mundescricao",
							   		   	"label"		=> "Município",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'pdiesfera':
				array_push( $coluna, 
								array(	"campo" 	=> "pdiesfera",
							   		   	"label"		=> "Esfera",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'pdicodinep':
				array_push( $coluna, 
								array(	"campo" 	=> "pdicodinep",
							   		   	"label"		=> "Código INEP",
							   		   	"type"	  	=> "string"
								)
				);
				continue;
			break;
			case 'pdenome':
				array_push( $coluna, 
								array(	"campo" 	=> "pdenome",
							   		   	"label"		=> "Escola",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'pdilocalizacao':
				array_push( $coluna, 
								array(	"campo" 	=> "pdilocalizacao",
							   		   	"label"		=> "Localização",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;			
			case 'atedesc':
				array_push( $coluna, 
								array(	"campo" 	=> "atedesc",
							   		   	"label"		=> "Área Temática",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'curdesc':
				array_push( $coluna, 
								array(	"campo" 	=> "curdesc",
							   		   	"label"		=> "Curso",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'ncudesc':
				array_push( $coluna, 
								array(	"campo" 	=> "ncudesc",
							   		   	"label"		=> "Nível",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'moddesc':
				array_push( $coluna, 
								array(	"campo" 	=> "moddesc",
							   		   	"label"		=> "Modalidade",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'pcfano':
				array_push( $coluna, 
								array(	"campo" 	=> "pcfano",
							   		   	"label"		=> "Ano Referência",
							   		   	"type"	  	=> "string"
								) 
				);
				continue;
			break;
			case 'privagassolicitadas':
				array_push( $coluna, 
								array(	"campo" 	=> "privagassolicitadas",
							   		   	"label"		=> "Vagas Solicitadas",
							   		   	"type"	  	=> "numeric"
								) 
				);
				continue;
			break;
			case 'privagasprevistas':
				array_push( $coluna, 
								array(	"campo" 	=> "privagasprevistas",
							   		   	"label"		=> "Vagas Autorizadas",
							   		   	"type"	  	=> "numeric"
								) 
				);
				continue;
			break;
		}
	}	
	//ver($coluna, d);	
	return $coluna;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
	//ver($agrupador, d);
	$agp = 	array(	"agrupador" => array(), 
					"agrupadoColuna" => array("estuf","mundescricao","pdiesfera","pdicodinep","pdenome","pdilocalizacao","atedesc","curdesc","ncudesc","moddesc","pcfano","privagassolicitadas","privagasprevistas")	  
			);
			
	$count = 1;
	$i = 0;

	foreach ($agrupador as $val){
		if($count == 1){
			$var = $vari;
		} else {
			$var = "";		
		}	
		
		switch ($val) {
			case 'estuf':
				array_push($agp['agrupador'], array("campo" => "estuf", "label" => "$var Estado") );				
		   		continue;
		    break;
		    case 'mundescricao':
				array_push($agp['agrupador'], array("campo" => "mundescricao", "label" => "$var Município") );					
		    	continue;
		    break;		    	
		    case 'pdiesfera':
				array_push($agp['agrupador'], array("campo" => "pdiesfera", "label" => "$var Esfera"));					
		    	continue;			
		    break;
		    case 'pdicodinep':
				array_push($agp['agrupador'], array("campo" => "pdicodinep","label" => "$var Código INEP"));					
		   		continue;			
		    break;
		    case 'pdenome':
				array_push($agp['agrupador'], array("campo" => "pdenome","label" => "$var Escola"));					
		   		continue;			
		    break;		    
		}
		$count++;
	}
	#Agrupador padrão. 
	array_push($agp['agrupador'], array("campo" => "pdeid","label" => "Cód."));
		
	return $agp;
}
?>