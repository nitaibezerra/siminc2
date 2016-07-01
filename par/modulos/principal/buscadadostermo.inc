<?php
ini_set("memory_limit", "2048M");
set_time_limit(0);

global $db;

$str = "";
if( $_REQUEST['dopid'] ){
	$str = " AND d.dopid = ".$_REQUEST['dopid']." ";
}

$sql = "SELECT 
			d.dopid,  
			d.mdoid
		FROM 
			par.documentopar d 
			INNER JOIN par.modelosdocumentos md ON md.mdoid = d.mdoid
		WHERE 
			md.tpdcod IN (102,21)
			AND d.dopvalortermo is null
			--1 = 1 
			{$str}
		--	AND d.dopid IN (42152,43453,42151,23782,43326,23350,43098,34441,41524,41654,41486,41665,26697,39982,41593) 
		--	AND d.mdoid IN (39, 32) 
		ORDER BY 
			d.mdoid, d.dopid --mdoid IN (3,16,19,20) ORDER BY mdoid, dopid";

$dados = $db->carregar($sql);

//ver($dados, d);

$sql = "";

/*
// Inicio da Vigencia
foreach( $dados as $documento ){

	$procurar   = array('&lt;br&gt;', '&lt;br/&gt;', '&lt;br /&gt;', '&lt;strong&gt;', '&lt;/strong&gt;', '&lt;b&gt;', '&lt;/b&gt;', '&lt;td&gt;', '&lt;/td&gt;', '&l');
	$texto = str_ireplace($procurar, '', $documento['doptexto']);
	
	if( $documento['mdoid'] == 3 || $documento['mdoid'] == 16 ){
		$texto = trim(substr($texto, stripos($texto, 'S INICIAL:') + 10, 11));
	} else {
		$texto = trim(substr($texto, stripos($texto, 'S INICIAL:') + 10, 7));
	}

	if (is_numeric(substr($texto, 0, 2))) {
		$sql .= "UPDATE par.documentopar SET dopdatainiciovigencia = '".$texto."' WHERE dopid = ".$documento['dopid']."; ";
	}
	
}

// Fim da Vigencia
foreach( $dados as $documento ){

	$procurar   = array('&lt;br&gt;', '&lt;br/&gt;', '&lt;br /&gt;', '&lt;strong&gt;', '&lt;/strong&gt;', '&lt;b&gt;', '&lt;/b&gt;', '&lt;td&gt;', '&lt;/td&gt;', '&l');
	$texto = str_ireplace($procurar, '', $documento['doptexto']);
	
	if( $documento['mdoid'] == 3 || $documento['mdoid'] == 16 ){
		$texto = trim(substr($texto, stripos($texto, 'S FINAL') + 8, 11));
	} else {
		$texto = trim(substr($texto, stripos($texto, 'S FINAL') + 8, 7));
	}
	
	if (is_numeric(substr($texto, 0, 2))) {
		$vig = "";
		$vig = $db->pegaUm("SELECT dopdatafimvigencia FROM par.documentopar WHERE dopid = ".$documento['dopid']);
		
		if( $vig != $texto ){
			$sql .= "UPDATE par.documentopar SET dopdatafimvigencia = '".$texto."' WHERE dopid = ".$documento['dopid']."; ";
		}
		
///		if(!$vig){
///			$sql .= "UPDATE par.documentopar SET dopdatafimvigencia = '".$texto."' WHERE dopid = ".$documento['dopid']."; ";
///		}
	}
	
}

*/
// Valor do termo
foreach( $dados as $documento ){
	$valor = "";
	
	$texto = pegaTermoCompromissoArquivo( $documento['dopid'], '');
//ver($documento['mdoid'], $documento['dopid']);
	if( $documento['mdoid'] == 28 || $documento['mdoid'] == 32 || $documento['mdoid'] == 30 ){
		// A partir de TOTAL GERAL
		$texto = trim(substr($texto, stripos($texto, 'TOTAL GERAL') + 4, 500));
	
		// Até a próxima </tr>
		$texto = trim(substr($texto, 0, stripos($texto, '/tr&gt;')));

		// Entre R$ e </tr>
		//$texto = trim(substr($texto, strripos($texto, '&lt;td')+4, -5));
		$texto = trim(substr($texto, stripos($texto, 'R$')+3, 20));
		
		$procurar   = array('&lt;br&gt;', '&lt;br/&gt;', '&lt;br /&gt;', '&lt;strong&gt;', '&lt;/strong&gt;', '&lt;b&gt;', '&lt;/b&gt;', '&lt;td&gt;', '&lt;/td&gt;', '&lt;tr&gt;', '&lt;/tr&gt;', '&lt;TD&gt;', '&lt;/TD&gt;', '&lt;TD', '/TD&gt;', 'TEXT-ALIGN: CENTER', 'TEXT-ALIGN: RIGHT', 'TEXT-ALIGN: LEFT', '&gt;', '&lt;', 'style=', '&quot;;&quot;', '&amp;nbsp;', 'td', 'R$', '/span', 'span', '/&', 'gt', '/', 'text-align: -webkit-right;','&quot;');
	
		$texto = trim(str_ireplace($procurar, '', $texto));

		$valor = str_replace('.','',$texto);
		$valor = str_replace(',','.',$valor);
		
	} else {
		// A partir de TOTAL GERAL
//		if( $documento['dopid'] == 40804 ){
//			ver($texto, d);
//		}
		$texto = trim(substr($texto, stripos($texto, 'TOTAL GERAL') + 4, 500));
		// Até a próxima </tr>
		$texto = trim(substr($texto, 0, stripos($texto, '/tr&gt;')));
		
		// Entre TOTAL GERAL e </tr>
		$texto = trim(substr($texto, strripos($texto, '&lt;td')+4, -5));
		
		$procurar   = array('&lt;br&gt;', '&lt;br/&gt;', '&lt;br /&gt;', '&lt;strong&gt;', '&lt;/strong&gt;', '&lt;b&gt;', '&lt;/b&gt;', '&lt;td&gt;', '&lt;/td&gt;', '&lt;tr&gt;', '&lt;/tr&gt;', '&lt;TD&gt;', '&lt;/TD&gt;', '&lt;TD', '/TD&gt;', 'TEXT-ALIGN: CENTER', 'TEXT-ALIGN: RIGHT', 'TEXT-ALIGN: LEFT', '&gt;', '&lt;', 'style=', '&quot;;&quot;', '&amp;nbsp;', 'td', 'R$', '/span', 'span', 'text-align: -webkit-right;','&quot;');
	
		$texto = trim(str_ireplace($procurar, '', $texto));
		
		$valor = str_replace('.','',$texto);
		$valor = str_replace(',','.',$valor);
	}	
	
	if( is_numeric($valor) ){
		$vl = "";
		$vl = $db->pegaUm("SELECT dopvalortermo FROM par.documentopar WHERE dopid = ".$documento['dopid']);
		
		if(!$vl){
			$sql .= "UPDATE par.documentopar SET dopvalortermo = ".$valor." WHERE dopid = ".$documento['dopid']."; ";
			//	ver($documento['dopid'], $valor);
		}
	}
}


if( $sql ){
	$db->executar($sql);
	$db->commit();
}

if( $_REQUEST['dopid'] ){
	return true;
} else {
	echo "FIM";
}
?>