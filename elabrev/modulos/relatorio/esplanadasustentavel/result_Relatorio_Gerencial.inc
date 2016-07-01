<?php

	if ($_REQUEST['filtrosession']){
		$filtroSession = $_REQUEST['filtrosession'];
	}
	
	if ($_POST['tipo_relatorio']){
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
	
	if( $tipo_unigest == 'N' ){
		$join = "join elabrev.unidadesustentavel us on us.unscodigo = cast(u.unicod as integer)"; 
		$select = "'Select unicod, dpsid, lcsvalormeta From elabrev.lancamentosustentavel Order by 1,2,3'";
		$where[0] = " Where us.unsstatus = 'A'  "; 
	}else{
		$join = "Join elabrev.unidadeordenadora o on o.unicod = u.unicod";
		$select = "'Select unicod, dpsid, lsametareducaoaluno From elabrev.lancamentosustentavelaluno Order by 1,2,3'";
		$complem = " and u.unicod not in ('26101', '26104', '26105', '26290', '26291', '26292', '26298') ";
		$where[0] = " Where o.uniordstatus = 'A' ";
	}
	
	if( $unicod[0] && ( $unicod_campo_flag || $insid_campo_flag == '1' )){
		$where[0] .= " and u.unicod " . (( $unicod_campo_excludente == null || $unicod_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $unicod ) . "') ";
	}	
	
	$sql = "
		Select 	distinct u.unicod,
				u.unidsc,
				vl.valor_1,
				vl.valor_2, 
				--vl.valor_3,
				vl.valor_4,
				vl.valor_5,
				vl.valor_6,
				vl.valor_7,
				vl.valor_8,
				vl.valor_9,
				--vl.valor_10,
				vl.valor_11,
				vl.valor_12,
				vl.valor_13
		From public.unidade u
		".$join."		
		Join(
			Select unicod, valor_1, valor_2, 
					--valor_3, 
					valor_4, valor_5, valor_6, valor_7, valor_8, valor_9, 
					--valor_10,
			 		valor_11, valor_12, valor_13
			From crosstab(
				".$select."
			) as d (unicod varchar, valor_1 numeric, valor_2 numeric, 
					--valor_3 numeric, 
					valor_4 numeric, valor_5 numeric,
					valor_6 numeric, valor_7 numeric, valor_8 numeric, valor_9 numeric, 
					--valor_10 numeric,
					valor_11 numeric, valor_12 numeric, valor_13 numeric
				)
		) as vl on vl.unicod = u.unicod ".$complem."

		--Parametro 1 (caixa de seleção 1)
		".$where[0]."
		
		order by 1
	";
	
	//ver($sql, d);
	return $sql;
}

function monta_coluna(){
	
	$colunas = array();
	
	$coluna = array(
				/*array(	"campo"		=> "unicod",
						"label" 	=> "Código",
					   	"type"	  	=> "string"
				),*/
				array(	"campo" 	=> "unidsc",
			   		   	"label"		=> "Unidade Gestora",
			   		   	"type"	  	=> "string"
				),
				array(	"campo" 	=> "valor_1",
			   		   	"label"		=> "Água e Esgoto",
			   		   	"type"	  	=> "string"
				),
				array(	"campo" 	=> "valor_2",
			   		   	"label"		=> "Apoio Administrativo, Técnico e Operacional",
			   		   	"type"	  	=> "string"
				),/*
				array(	"campo" 	=> "valor_3",
			   		   	"label"		=> "Diárias",
			   		   	"type"	  	=> "string"
				),*/
				array(	"campo" 	=> "valor_4",
			   		   	"label"		=> "Energia Elétrica",
			   		   	"type"	  	=> "string"
				),
				array(	"campo" 	=> "valor_5",
			   		   	"label"		=> "Limpeza e Conservação",
			   		   	"type"	  	=> "string"
				),
				array(	"campo" 	=> "valor_6",
			   		   	"label"		=> "Locação de Imóveis",
			   		   	"type"	  	=> "string"
				),
				array(	"campo" 	=> "valor_7",
			   		   	"label"		=> "Locação de Veículos",
			   		   	"type"	  	=> "string"
				),
				array(	"campo" 	=> "valor_8",
			   		   	"label"		=> "Manutenção e Conservação de Bens Imóveis",
			   		   	"type"	  	=> "string"
				),
				array(	"campo" 	=> "valor_9",
			   		   	"label"		=> "Material de Consumo",
			   		   	"type"	  	=> "string"
				),/*
				array(	"campo" 	=> "valor_10",
			   		   	"label"		=> "Passagens e Locomoção",
			   		   	"type"	  	=> "string"
				),*/
				array(	"campo" 	=> "valor_11",
			   		   	"label"		=> "Serviço de Processamento de Dados",
			   		   	"type"	  	=> "string"
				),
				array(	"campo" 	=> "valor_12",
			   		   	"label"		=> "Telecomunicações",
			   		   	"type"	  	=> "string"
				),
				array(	"campo" 	=> "valor_13",
			   		   	"label"		=> "Vigilância",
			   		   	"type"	  	=> "string"
				)
		);
	//ver($coluna, d);	
	return $coluna;
}

function monta_agp(){
	
	//$agrupador = $_POST['agrupador'];

	$agp = 	array(	"agrupador" => array(), 
					"agrupadoColuna" => array("unicod", "unidsc", "valor_1", "valor_2",
											#"valor_3",
											"valor_4","valor_5","valor_6","valor_7","valor_8","valor_9",
											#"item_10",
											"valor_10","valor_11","valor_12","valor_13")	  
			);
		
	array_push($agp['agrupador'], array("campo" => "unicod", "label" => "$var Código") );

	return $agp;
}
?>