<?php
function obras_monta_agp_painel_gerencial(){

	$colunas = str_replace("}{",",",$_REQUEST["colunas"]);
	$colunas = str_replace("}","",$colunas);
	$colunas = str_replace("{","",$colunas);
	$colunas = str_replace("\'","'",$colunas);
	$colunas = explode(",",$colunas);
	$agrupador = $colunas;//array('nomedaobra');

	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array( "qtd" )
				);
	
	foreach ( $agrupador as $val ){
		switch( $val ){
			case "ufusuario":
				array_push($agp['agrupador'], array(
													"campo" => "ufusuario",
											  		"label" => "Usuário")										
									   				);
			break;
			case "orgao":
				array_push($agp['agrupador'], array(
													"campo" => "orgao",
											  		"label" => "Orgão")										
									   				);
			break;
			case "nomeobra":
				array_push($agp['agrupador'], array(
													"campo" => "nomeobra",
											  		"label" => "Nome da Obra")										
									   				);
			break;
			case "situacao":
				array_push($agp['agrupador'], array(
													"campo" => "situacao",
											  		"label" => "Situação da Obra")										
									   				);
			break;
			case "municipioobra":
				array_push($agp['agrupador'], array(
													"campo" => "municipioobra",
											  		"label" => "Municipio")										
									   				);
			break;
			case "grupo":
				array_push($agp['agrupador'], array(
													"campo" => "grupo",
											  		"label" => "Grupo de Municipios")										
									   				);
			break;
			case "tipoobra":
				array_push($agp['agrupador'], array(
													"campo" => "tipoobra",
											  		"label" => "Pro-Infância/Quadra")										
									   				);
			break;
			case "extensao":
				array_push($agp['agrupador'], array(
													"campo" => "extensao",
											  		"label" => "Extensão do Arquivo")										
									   				);
			break;
			case "nomearquivo":
				array_push($agp['agrupador'], array(
													"campo" => "nomearquivo",
											  		"label" => "Dados do Arquivo")										
									   				);
				array_push($agp['agrupadoColuna'], 'nomearquivo' );
				array_push($agp['agrupadoColuna'], 'descricaoarquivo' );
				array_push($agp['agrupadoColuna'], 'tamanho' );
				array_push($agp['agrupadoColuna'], 'datainclusao' );
				array_push($agp['agrupadoColuna'], 'horainclusao' );
			break;
		}	
	}
	
	return $agp;
	
}

function obras_monta_sql_painel_gerencial(){
	
	global $db;
	
	$where = array();
	
	$filtro="";
	
	// Filtros
	if ( $_REQUEST["estuf"] ){
		$notestuf = $_REQUEST['notestuf'] == 'true' ? ' NOT ' : '';
		$estuf = str_replace("}{",",",$_REQUEST["estuf"]);
		$estuf = str_replace("}","",$estuf);
		$estuf = str_replace("{","",$estuf);
		$estuf = str_replace("\'","'",$estuf);
		$filtro .= " AND ufusuario ".$notestuf."IN (".$estuf.") ";
		$ordem  .= " ufusuario, ";
	}
	if ( $_REQUEST["muncod"] ){
		$notmuncod = $_REQUEST['notmuncod'] == 'true' ? ' NOT ' : '';
		$muncod = str_replace("}{",",",$_REQUEST["muncod"]);
		$muncod = str_replace("}","",$muncod);
		$muncod = str_replace("{","",$muncod);
		$muncod = str_replace("\'","'",$muncod);
		$filtro .= " AND codigomunicipioobra ".$notmuncod."IN (".$muncod.") ";
		$ordem  .= " ufusuario, ";
	}
	if ( $_REQUEST["tpmid"] ){
		$nottpmid = $_REQUEST['nottpmid'] == 'true' ? ' NOT ' : '';
		$tpmid = str_replace("}{",",",$_REQUEST["tpmid"]);
		$tpmid = str_replace("}","",$tpmid);
		$tpmid = str_replace("{","",$tpmid);
		$tpmid = str_replace("\'","",$tpmid);
		$filtro .= " AND codigogrupo ".$nottpmid."IN (".$tpmid.") ";
		$ordem  .= " grupo, ";
	}
	if ( $_REQUEST["esdid"] ){
		$notesdid = $_REQUEST['notesdid'] == 'true' ? ' NOT ' : '';
		$esdid = str_replace("}{",",",$_REQUEST["esdid"]);
		$esdid = str_replace("}","",$esdid);
		$esdid = str_replace("{","",$esdid);
		$esdid = str_replace("\'","",$esdid);
		$filtro .= " AND codigosituacao ".$notesdid."IN (".$esdid.") ";
		$ordem  .= " situacao, ";
	}
	if ( $_REQUEST["ptoid"] ){
		$notptoid = $_REQUEST['notptoid'] == 'true' ? ' NOT ' : '';
		$ptoid = str_replace("}{",",",$_REQUEST["ptoid"]);
		$ptoid = str_replace("}","",$ptoid);
		$ptoid = str_replace("{","",$ptoid);
		$ptoid = str_replace("\'","",$ptoid);
		$filtro .= " AND codigotipoobra ".$notptoid."IN (".$ptoid.") ";
		$ordem  .= " tipoobra, ";
	}
	
	// monta o sql 
	$sql = "SELECT 
				usuario || ' - ' || nomeusuario as nomeusuario,  
				nomeusuario, 
				ufusuario, 
				municipiousuario, 
				orgao, 
				nomeobra, 
				situacao, 
				municipioobra, 
				tipoobra, 
				nomearquivo, 
				descricaoarquivo, 
				extensao, 
				tamanho || ' bytes' as tamanho, 
				datainclusao, 
				horainclusao, 
				grupo,
				1 as qtd
			FROM
				obras.rel_arquivos
			WHERE
				1=1 {$filtro}
			ORDER BY
				$ordem qtd" ;
//	ver($sql,d);
	return $sql;
	
}

function obras_monta_coluna_painel_gerencial(){
	
	$coluna = array();

	if( strstr($_REQUEST['colunas'], 'nomearquivo') ){
		array_push( $coluna, array("campo" 	  => "nomearquivo",
						   		   "label" 	  => "Nome do Arquivo",
								   "type"	  => "string") );
		array_push( $coluna, array("campo" 	  => "descricaoarquivo",
						   		   "label" 	  => "Descrição do Arquivo",
								   "type"	  => "string") );
		array_push( $coluna, array("campo" 	  => "datainclusao",
						   		   "label" 	  => "Data da Inclusão do Arquivo",
								   "type"	  => "string") );
		array_push( $coluna, array("campo" 	  => "horainclusao",
						   		   "label" 	  => "Hora da Inclusão do Arquivo",
								   "type"	  => "string") );
		array_push( $coluna, array("campo" 	  => "tamanho",
						   		   "label" 	  => "Tamanho do Arquivo",
								   "type"	  => "string") );
	}
	array_push( $coluna, array("campo" 	  => "qtd",
					   		   "label" 	  => "Quantidade de Arquivos",
					   		   "blockAgp" => "nomearquivo",
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
$sql       = obras_monta_sql_painel_gerencial(); 
$agrupador = obras_monta_agp_painel_gerencial();
$coluna    = obras_monta_coluna_painel_gerencial();
$dados 	   = $db->carregar( $sql );

$rel->setAgrupador($agrupador, $dados); 
$rel->setColuna($coluna);
$rel->setTolizadorLinha(true);
$rel->setEspandir(false);
$rel->setTotNivel(true);

echo $rel->getRelatorio(); 
?>
