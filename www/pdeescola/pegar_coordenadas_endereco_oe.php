<?php 

$_REQUEST['baselogin'] = "simec_desenvolvimento";

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";


function mascaraglobal($value, $mask) {
	$casasdec = explode(",", $mask);
	// Se possui casas decimais
	if($casasdec[1])
		$value = sprintf("%01.".strlen($casasdec[1])."f", $value);

	$value = str_replace(array("."),array(""),$value);
	if(strlen($mask)>0) {
		$masklen = -1;
		$valuelen = -1;
		while($masklen>=-strlen($mask)) {
			if(-strlen($value)<=$valuelen) {
				if(substr($mask,$masklen,1) == "#") {
						$valueformatado = trim(substr($value,$valuelen,1)).$valueformatado;
						$valuelen--;
				} else {
					if(trim(substr($value,$valuelen,1)) != "") {
						$valueformatado = trim(substr($mask,$masklen,1)).$valueformatado;
					}
				}
			}
			$masklen--;
		}
	}
	return $valueformatado;
}



/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '';
$_SESSION['usucpf'] = '';


$db = new cls_banco();


?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="br" lang="br">
<head>
<title>Google Maps</title>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1" />
<?php

function curPageURL() {
	$pageURL = 'http';
	if ($_SERVER["HTTPS"] == "on") {$pageURL .= "s";}
		$pageURL .= "://";
	if ($_SERVER["SERVER_PORT"] != "80") {
		$pageURL .= $_SERVER["SERVER_NAME"].":".$_SERVER["SERVER_PORT"].$_SERVER["REQUEST_URI"];
	} else {
		$pageURL .= $_SERVER["SERVER_NAME"].$_SERVER["REQUEST_URI"];
	}
	return $pageURL;
}

$local= explode("/",curPageURL());?>
<?if ($local[2]=="simec.mec.gov.br" ){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxQhVwj8ALbvbyVgNcB-R-H_S2MIRxTIdhrqjcwTK3xxl_Nu_YMC5SdLWg" type="text/javascript"></script>
	<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxQhVwj8ALbvbyVgNcB-R-H_S2MIRxTIdhrqjcwTK3xxl_Nu_YMC5SdLWg"; ?>
<? } ?>
<?if ($local[2]=="simec-d.mec.gov.br"){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxRYtD8tuHxswJ_J7IRZlgTxP-EUtxT_Cz5IMSBe6d3M1dq-XAJNIvMcpg" type="text/javascript"></script>
	<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxRYtD8tuHxswJ_J7IRZlgTxP-EUtxT_Cz5IMSBe6d3M1dq-XAJNIvMcpg"; ?> 
<? } ?>
<?if ($local[2]=="simec" ){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxTNzTBk8zukZFuO3BxF29LAEN1D1xSIcGWxF7HCjMwks0HURg6MTfdk1A" type="text/javascript"></script>
	<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxTNzTBk8zukZFuO3BxF29LAEN1D1xSIcGWxF7HCjMwks0HURg6MTfdk1A"; ?>
<? } ?>
<?if ($local[2]=="simec-d"){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxTFm3qU4CVFuo3gZaqihEzC-0jfaRTY9Fe8UfzYeoYDxtThvI3nGbbZEw" type="text/javascript"></script>
	<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxTFm3qU4CVFuo3gZaqihEzC-0jfaRTY9Fe8UfzYeoYDxtThvI3nGbbZEw"; ?> 
<? } ?>
<?if ($local[2]=="simec-local"){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxRzjpIxsx3o6RYEdxEmCzeJMTc4zBSMifny_dJtMKLfrwCcYh5B01Pq_g" type="text/javascript"></script>
	<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxRzjpIxsx3o6RYEdxEmCzeJMTc4zBSMifny_dJtMKLfrwCcYh5B01Pq_g"; ?> 	
<? } ?>
<?if ($local[2]=="painel.mec.gov.br"){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxTPkFYZwQy2nvpGvFj08HQmPOt9ZBT2EJmQsTms0WQqU_5GvEj7bMZd7g" type="text/javascript"></script>
	<? $GKey = "ABQIAAAAwN0kvNsueYw8CBs704pusxTPkFYZwQy2nvpGvFj08HQmPOt9ZBT2EJmQsTms0WQqU_5GvEj7bMZd7g"; ?> 	
<? } ?>
<script type="text/javascript">

var markerGroups = { '1': [], '2': []};

function initialize() {
	if (GBrowserIsCompatible()) { // verifica se o navegador é compatível
			map = new GMap2(document.getElementById("google_map")); // inicila com a div mapa
			var zoom = 4;	var lat_i = -14.689881; var lng_i = -52.373047;	//Brasil	
			map.setCenter(new GLatLng(lat_i,lng_i), parseInt(zoom)); //Centraliza e aplica o zoom

			
			// Início Controles
			map.addControl(new GMapTypeControl());
			map.addControl(new GLargeMapControl3D());
	        map.addControl(new GOverviewMapControl());
	        map.enableScrollWheelZoom();
	        map.addMapType(G_PHYSICAL_MAP);
	        // Fim Controles
	
	}
}


function getLatLng(address,nu) {
	geocoder = new GClientGeocoder();
	if (geocoder) {
		geocoder.getLatLng(
		address,
		function(point) {
			if (point) {
//				document.getElementById("update").innerHTML += "UPDATE carga.instalacoes_esportivas set lat = '" + point.lat() + "', \"long\" = '" + point.lng() + "' where \"N°\" = '" + nu + "';<br>";
				document.getElementById("update").innerHTML += "UPDATE carga.atletismo set lat = '" + point.lat() + "', lon = '" + point.lng() + "' where \"CEP\" = '" + nu + "';<br>";
			} else {
				
			}
		}
		);
	}
}



</script>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>

</head>
<body>
<div id="update"></div>
<?php 

$sql = "select \"CEP\" as cep, \"CIDADE\" as cidade from carga.atletismo where lat is null";
//select * from carga.atletismo

$instalacoes = $db->carregar($sql);

$t=1;

echo "<script>";

foreach($instalacoes as $ins){
		
	echo "getLatLng(\"CEP ".mascaraglobal(str_replace(array(".","-"," "),"",$ins['cep']),"#####-###").",".$ins['cidade'].", Brasil\",'".$ins['cep']."');";
	
	if($t==5) {
		sleep(5);
		$t=1;
	}

	//$t++;
	
}

echo "</script>";

?>
</body>
</html>