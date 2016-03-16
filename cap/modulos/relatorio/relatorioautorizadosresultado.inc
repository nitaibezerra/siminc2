<?php
if ($_REQUEST['filtrosession']){
	$filtroSession = $_REQUEST['filtrosession'];
}

ini_set("memory_limit", "2048M");
header('Content-Type: text/html; charset=iso-8859-1');  ?>
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


if( $_POST['req'] == 2){
	ob_clean();
	$nomeDoArquivoXls="Relatório_Afastamento_Autorizados".date('d-m-Y_H_i');
	echo $r->getRelatorioXls();
} else {
	echo $r->getRelatorio();
}

function monta_sql(){
	global $filtroSession;
	extract($_POST);
	//Área MEC
	if($uamid[0]){
		$aryWhere[] = " afa.uamid ". (!$uamid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "', '", $uamid )."' ) ";
	} 
	
	//País
	if($paiid){
		$join_pais = "INNER JOIN (SELECT afpid FROM cap.afastamentotrecho WHERE paiid = {$paiid} GROUP BY paiid, afpid) as pais ON pais.afpid = afa.afpid";
	} else {
		$join_pais = "";
	}	

	//Período
	if(!empty($afpdtrealizacaoinicial) && !empty($afpdtrealizacaofinal)){
		$afpdtrealizacaoinicial = formata_data_sql($afpdtrealizacaoinicial);
	    $afpdtrealizacaofinal = formata_data_sql($afpdtrealizacaofinal);	
		$aryWhere[] = "( afa.afpdtrealizacaoinicial BETWEEN '{$afpdtrealizacaoinicial}'  AND '{$afpdtrealizacaofinal}' AND  afa.afpdtrealizacaofinal BETWEEN '{$afpdtrealizacaoinicial}'  AND '{$afpdtrealizacaofinal}')";
	}	
	
	$sql = "SELECT			uam.uamsigla || ' - ' || uam.uamdsc as uamdsc,
							afa.afpnumprocesso,
							afa.afpnumsiape,
							sim.no_servidor,
							to_char(afa.afpdtrealizacaoinicial,'DD/MM/YYYY') || ' a ' || to_char(afa.afpdtrealizacaofinal,'DD/MM/YYYY') AS periodo,
							array_to_string(array(SELECT DISTINCT te.paidescricao FROM cap.afastamentotrecho at INNER JOIN territorios.pais te ON at.paiid = te.paiid WHERE at.afpid = afa.afpid),',') AS paises
			FROM			cap.afastamento afa
			LEFT JOIN		siape.tb_servidor_simec sim ON sim.nu_cpf = afa.fdpcpf
			LEFT JOIN 		public.unidadeareamec uam ON uam.uamid = afa.uamid
			$join_area	
			$join_pais
							".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."
			ORDER BY   		uam.uamdsc, afa.afpdtrealizacaoinicial, afa.afpdtrealizacaofinal, sim.no_servidor";	
 	//ver($sql,$uamid,d);
	return $sql;
}

function monta_agp(){
	$agp = array("agrupador" => array(),"agrupadoColuna" => array("afpnumsiape","no_servidor","periodo","paises"));
	array_push($agp['agrupador'], array("campo" => "uamdsc","label" => "Área MEC"));
	array_push($agp['agrupador'], array("campo" => "afpnumprocesso","label" => ""));
	return $agp;
}

function monta_coluna(){
	$colunas = array('periodo','afpnumsiape','no_servidor','paises');
	$coluna = array();
	
	if($colunas){
		foreach ($colunas as $val): 
			switch ($val) {
				case 'periodo':
					array_push($coluna, array(
										"campo" => "periodo",
								  		"label" => "Período"));				
					continue;
			        break;
				case 'afpnumsiape':
					array_push($coluna, array(
										"campo" => "afpnumsiape",
								  		"label" => "SIAPE",
										"type"  => "string"));				
					continue;
			        break;
			    case 'no_servidor':
					array_push($coluna, array(
										"campo" => "no_servidor",
								  		"label" => "Nome",
				  						"type"  => "string"));					
					continue;
				    break;			        
			    case 'paises':
					array_push($coluna, array(
										"campo" => "paises",
								  		"label" => "País",
				  						"type"  => "string"));				
					continue;
				    break;
				}
			endforeach;
		}
			//ver($coluna, d);	
	return $coluna;			  	
}
?>
</body>