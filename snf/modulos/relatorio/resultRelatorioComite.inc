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
	
	if( $insid[0] && ( $insid_campo_flag || $insid_campo_flag == '1' )){		
		$where[1] = " and ies.insid " . (( $insid__campo_excludente == null || $insid__campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $insid ) . "') ";		
	}
	
	if( $papid[0] && ($papid_campo_flag || $papid_campo_flag == '1' )){
		$where[2] = " and pc.papid " . (( $papid_campo_excludente == null || $papid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $papid ) . "') ";		
	}

	
	$sql = "
		Select	ies.insid,
				ies.insnome,
				mc.memid,
				mc.memnome, 
				mc.memcpf, 
				mc.mememail, 
				pc.papdescricao 
		From snf.instituicaoensino ies 
		inner join snf.comite c using(insid) 
		inner join snf.membrocomite mc using(comid) 
		inner join snf.papelcomite pc using(papid) 
		
		Where ies.insstatus = 'A' and mc.memstatus = 'A' and pc.papstatus = 'A'
		
		--Parametro 1 (caixa de seleção 1)
		".$where[1]."
		
		--Parametro 2 (caixa de seleção 2)
		".$where[2]."
		
		Order by 1,2
	";
	//ver($sql, d);
	return $sql;
}

function monta_coluna(){
	
	$colunas = array();
	
	$coluna = array(
				array(	"campo"		=> "insnome",
						"label" 	=> "Instituição de ensino",
					   	"type"	  	=> "string"
				),
				array(	"campo" 	=> "memnome",
			   		   	"label"		=> "Membro",
			   		   	"type"	  	=> "string"
				),
				array(	"campo" 	=> "memcpf",
			   		   	"label"		=> "CPF",
			   		   	"type"	  	=> "string"
				),
				array(	"campo" 	=> "mememail",
			   		   	"label"		=> "E-mail",
			   		   	"type"	  	=> "string"
				),
				array(	"campo" 	=> "papdescricao",
			   		   	"label"		=> "Papel",
			   		   	"type"	  	=> "string"
				) 

		);

	//ver($coluna, d);	
	return $coluna;
}

function monta_agp(){
	
	//$agrupador = $_POST['agrupador'];

	$agp = 	array(	"agrupador" => array(), 
					"agrupadoColuna" => array("insid","insnome","memnome","memcpf","mememail","papdescricao")	  
			);
	
		//array_push($agp['agrupador'], array("campo" => "insid", "label" => "$var Cód. Instituição") );

		//array_push($agp['agrupador'], array("campo" => "insnome", "label" => "$var Instituição") );
		
		array_push($agp['agrupador'], array("campo" => "memid", "label" => "$var Código") );

		//array_push($agp['agrupador'], array("campo" => "memnome", "label" => "$var Membro"));

		//array_push($agp['agrupador'], array("campo" => "memcpf","label" => "$var CPF"));

		//array_push($agp['agrupador'], array("campo" => "mememail","label" => "$var E-mail"));
		
		//array_push($agp['agrupador'], array("campo" => "mememail","papdescricao" => "$var Papel"));

/*
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
	//array_push($agp['agrupador'], array("campo" => "pdeid","label" => "Cód."));
*/
		
	return $agp;
}
?>