<?php
/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */


// inicializa sistema
define( 'APPRAIZ', '' );
		$nome_bd     = '';
		$servidor_bd = '';
		$porta_bd    = '5432';
		$usuario_db  = '';
		$senha_bd    = '';
//require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

$sql = "SELECT ST_astext(the_geom) as poli, muncod, mundescricao, estuf 
from municipios_br m
inner join territorios.municipio mun on m.codigo_mun = mun.muncod
where mun.estuf in ('ES','AC','RR','MG')";



$dados = $db->carregar($sql); 



?>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Polígono</title>
<title>Google Maps JavaScript API v3 Example: Map Simple</title>
<link href="http://code.google.com/apis/maps/documentation/javascript/examples/default.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="http://maps.google.com/maps/api/js?sensor=false"></script>
<script type="text/javascript">
  function initialize() {

	var myLatLng = new google.maps.LatLng(-8.05555555556, -34.9069444444);

    var center = myLatLng;
    var myOptions = {
      zoom: 5,
      center: myLatLng,
      mapTypeId: google.maps.MapTypeId.TERRAIN
    };
    
    map = new google.maps.Map(document.getElementById("map"),
        myOptions);
	<? 
foreach ($dados as $dado){

$poli = str_replace(array('MULTIPOLYGON',')','('),'',$dado['poli']);
$pontos = explode(',',$poli);
$nome_poly = $dado['estuf'].$dado['muncod'];
	$js = " var ".$nome_poly." = [";
	unset($pontojs);
	foreach ($pontos as $k=>$ponto){
		$p = explode(' ',$ponto);
		$pontojs[] = " new google.maps.LatLng(".$p[1].", ".$p[0].")";

	}
	$js .= implode(",",$pontojs);
	$js .= "];";

	$js .= $nome_poly."_ = new google.maps.Polygon({
      paths: ".$nome_poly.",
      strokeColor: \"#000000\",
      strokeOpacity: 0.6,
      strokeWeight: 0.5,
      fillColor: \"#0000ff\",
      fillOpacity: 0.35
    });
     ".$nome_poly."_.setMap(map);";
	 $js .= "google.maps.event.addListener(".$nome_poly."_, 'mouseover', f_mouseover(this));";
	echo $js;
}
	
	?>

  }

  function f_mouseover(obj)
  {
	obj.fillColor = '#000000';
  }

 

</script>

 
</head> 
<body onload="initialize()"> 
<div id="map" style="width: 900px; height: 550px; position:relative; "></div> 
</body> 
</html> 
