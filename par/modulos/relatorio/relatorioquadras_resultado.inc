<?php

function quadra_monta_agp(){
	
	$agrupador = $_REQUEST['colunas'];//array('nomedaobra');
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array( "qtdescolas", "qtdmatriculas" )
				);
	
	foreach ( $agrupador as $val ){
		switch( $val ){
			case "ano":
				array_push($agp['agrupador'], array(
													"campo" => "ano",
											  		"label" => "Ano")										
									   				);
			break;
			case "codigo":
				array_push($agp['agrupador'], array(
													"campo" => "codigo",
											  		"label" => "Código INEP")										
									   				);
			break;
			case "escola":
				array_push($agp['agrupador'], array(
													"campo" => "escola",
											  		"label" => "Escola")										
									   				);
			break;
			case "uf":
				array_push($agp['agrupador'], array(
													"campo" => "uf",
											  		"label" => "UF")										
									   				);
			break;
			case "municipio":
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
											  		"label" => "Município")										
									   				);
			break;
			case "dependencia":
				array_push($agp['agrupador'], array(
													"campo" => "dependencia",
											  		"label" => "Dependência")										
									   				);
			break;
			case "situacao":
				array_push($agp['agrupador'], array(
													"campo" => "situacao",
											  		"label" => "Situação")										
									   				);
			break;
			case "quadra":
				array_push($agp['agrupador'], array(
													"campo" => "quadra",
											  		"label" => "Quadra")										
									   				);
			break;
			case "tpmdsc":
				array_push($agp['agrupador'], array(
													"campo" => "tpmdsc",
											  		"label" => "Grupo de Município")										
									   				);
			break;

		}	
	}
	
	return $agp;
	
}


function quadra_monta_sql(){
	
	global $db;
	
	$where = array();
	
	$filtro="";
	
	// Filtros
	if ($_REQUEST["estuf"][0]){
		$notestuf = $_REQUEST['estuf_campo_flag'] == 'true' ? ' NOT ' : '';
		$filtro[] = "mun.estuf ".$notestuf."IN ('".implode("','", $_REQUEST["estuf"])."') ";
	}
	
	if ( $_REQUEST["muncod"][0]){
		$notmuncod = $_REQUEST['muncod_campo_flag'] == 'true' ? ' NOT ' : '';
		$filtro[] = "mun.muncod ".$notmuncod."IN ('".implode("','", $_REQUEST["muncod"])."') ";
	}
	
	if ( $_REQUEST["tpmid"][0]){
		$nottpmid = $_REQUEST['tpmid_campo_flag'] == 'true' ? ' NOT ' : '';
		$filtro[] = "mtm.tpmid ".$nottpmid."IN ('".implode("','", $_REQUEST["tpmid"])."') ";
	}
	
	// monta o sql 
	$sql = "SELECT DISTINCT				
				ano as ano,
				codigo as inep,
				'<a style=cursor:pointer; onclick=verEscola(\''||codigo||'\');>'||escola||'</a>' as escola,
				'<a style=cursor:pointer; onclick=verEstado(\''||uf||'\');>'||uf||'</a>' as uf,
				'<a style=cursor:pointer; onclick=verMunicipio(\''||municipio||'\');>'||municipio||'</a>' as municipio,
				dependencia as dependencia,
				situacao as situacao,
				quadra as quadra,
				matriculas as qtdmatriculas,
				tpm.tpmdsc,
				1 as qtdescolas 
			FROM
				par.pac2 pc 
			LEFT JOIN 
				territorios.municipio 	     mun ON mun.muncod = pc.codigo_municipio
			INNER JOIN
				territorios.muntipomunicipio mtm ON mtm.muncod = pc.codigo_municipio
			INNER JOIN
				territorios.tipomunicipio    tpm ON tpm.tpmid  = mtm.tpmid AND gtmid = 7 			
			".(($filtro)?"WHERE ".implode(" AND ", $filtro):"")." ".(($_REQUEST['colunas'])?"ORDER BY ".implode(",",$_REQUEST['colunas']):"");
	return $sql;
	
}

function quadra_monta_coluna(){
	
	$coluna = array();
	
	array_push( $coluna, array("campo" 	  => "qtdescolas",
					   		   "label" 	  => "Quantidade de escolas",
					   		   "blockAgp" => "escola",
					   		   "type"	  => "numeric") );
	
	array_push( $coluna, array("campo" 	  => "qtdmatriculas",
					   		   "label" 	  => "Quantidade de matrículas",
					   		   "type"	  => "numeric") );
	
	
	return $coluna;
	
}


/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */


// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

// monta o sql, agrupador e coluna do relatório
$sql       = quadra_monta_sql(); 
$agrupador = quadra_monta_agp();
$coluna    = quadra_monta_coluna();
$dados 	   = $db->carregar( $sql );

$rel->setAgrupador($agrupador, $dados); 
$rel->setColuna($coluna);
$rel->setTolizadorLinha(true);
$rel->setEspandir(false);
$rel->setTotNivel(true);
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" language="javascript" src="../includes/agrupador.js"></script>
		 
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<body>
<script>
function verEstado(estuf) {
	window.open('http://painel.mec.gov.br/painel/detalhamentoIndicador/detalhes/estado/estuf/'+estuf,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}

function verMunicipio(muncod) {
	window.open('http://painel.mec.gov.br/painel/detalhamentoIndicador/detalhes/municipio/muncod/'+muncod,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}

function verEscola(esccodinep) {
	window.open('http://painel.mec.gov.br/painel/detalhamentoIndicador/detalhes/escola/esccodinep/'+esccodinep,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
</script>
<?
echo $rel->getRelatorio(); 
?>
</body>
