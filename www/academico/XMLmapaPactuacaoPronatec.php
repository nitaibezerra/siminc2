<?php 
/* configurações */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações */


// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

if( $_REQUEST['estuf'][0] ){
	$filtro[] = "trim(sg_uf) in ('".implode("','",$_REQUEST['estuf'])."') ";
}

if($_REQUEST['chk_superior']) {
	$filtro[] = "\"Tipo\" IN('".implode("','",$_REQUEST['chk_superior'])."')";
}

$sql = "SELECT no_unidade_ensino, ds_coordenada, \"Tipo\" as tipo 
		FROM academico.relatorio_mapa_pactuacao 
		".(($filtro)?"WHERE ".implode(" AND ",$filtro):"")." 
		GROUP BY no_unidade_ensino, ds_coordenada, \"Tipo\"";

$dados = $db->carregar($sql);

ob_clean();
header('content-type: text/xml; charset=ISO-8859-1');

if($dados):
	
	$conteudo .= "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?><markers>"; // inicia o XML
	
	foreach($dados as $d):
	
		$coords = str_replace(array("(",")"),array(""),$d['ds_coordenada']);
		$coords = explode(",",$coords);
		
		$conteudo .= "<marker "; //inicia um ponto no mapa
		$conteudo .= "nome=\"".(($d['no_unidade_ensino'])?$d['no_unidade_ensino']:"Em branco")."\" "; // adiciona o nome da instituição;
		$conteudo .= "lat=\"".trim($coords[0])."\" "; // adiciona a latitude;
		$conteudo .= "lng=\"".trim($coords[1])."\" "; // adiciona a longitude;
		$conteudo .= "tipo=\"".$d['tipo']."\" "; // adiciona a longitude;
		$conteudo .= "/> ";
	
	endforeach;
	
	$conteudo .= "</markers> ";
	print $conteudo;
	
endif;
	
?>