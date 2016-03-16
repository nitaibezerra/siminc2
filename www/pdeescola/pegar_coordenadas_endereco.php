<?php 
$_REQUEST['baselogin'] = "simec_desenvolvimento";

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '';
$_SESSION['usucpf'] = '';


$db = new cls_banco();

if(isset($_POST['sql'])){
	$sql = $_POST['sql'];
	if(strstr($sql,"update")){
		$db->executar($sql);
		$db->commit();
	}else{
		die("atualizou tudo!");
	}
	echo "<script>window.location=window.location</script>";
}

$sql = "select 
\"CO_CNES\", \"Nome Estabelecimento\", \"Conectividade S/N\", 
\"Latitude1\", \"Longitude1\", \"Logradouro\", \"Endereço\", 
\"Número\", \"Bairro\", \"CEP\", \"MUNICIPIO_LOCADO\", \"UF_LOCADO\" 
from carga.\"UBS\" where \"CO_CNES\" != '' and \"Latitude1\" = '' and \"CEP\" != '';";

$tabela = "carga.\"UBS\"";
$coluna_primaria = "idserial";
$coluna_latitude = "Latitude1";
$coluna_longitude = "Longitude1";



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



?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="br" lang="br">
<head>
<title>Google Maps</title>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1" />
<script src="https://maps.googleapis.com/maps/api/js?v=3.exp&sensor=false"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
var geocoder;
var map;
geocoder = new google.maps.Geocoder();
function initialize() {
  var latlng = new google.maps.LatLng(-14.689881, -52.373047);
  var mapOptions = {
    zoom: 8,
    center: latlng,
    mapTypeId: google.maps.MapTypeId.ROADMAP
  }
  map = new google.maps.Map(document.getElementById("mapa"), mapOptions);
}

function getLatLong(address, pk) {
  geocoder.geocode( { 'address': address}, function(results, status) {
    if (status == google.maps.GeocoderStatus.OK) {
      map.setCenter(results[0].geometry.location);
      var marker = new google.maps.Marker({
          map: map,
          position: results[0].geometry.location
      });
      var lat = results[0].geometry.location.lat();
      var lng = results[0].geometry.location.lng();
      document.getElementById("update").innerHTML += "--Endereço passado: '"+address+"'<br/>update carga.\"UBS\" set \"<?php echo $coluna_latitude?>\" = '"+lat+"', \"<?php echo $coluna_longitude?>\" = '"+lng+"' where idserial = "+pk+";<br/>";
      document.getElementById("sql").value += "update carga.\"UBS\" set \"<?php echo $coluna_latitude?>\" = '"+lat+"', \"<?php echo $coluna_longitude?>\" = '"+lng+"' where idserial = "+pk+";";
    } else {
    	document.getElementById("update").innerHTML += "--Endereço passado: '"+address+"'<br/>--Latitude e Longitude não encontradas.<br/>";
    	//document.getElementById("sql").value += "--Endereço passado: '"+address+"'<br/>--Latitude e Longitude não encontradas.<br/>";
	    //alert("Geocode was not successful for the following reason: " + status);
    }
  });
}

google.maps.event.addDomListener(window, 'load', initialize);


$(function() {
	if($('#sql').val()){
		window.setTimeout( "$('#formulario').submit()" , '5000' );
	}
});

</script>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>

</head>
<body onload="initialize()" >
<div id="mapa" style="width:800px;height:600px" ></div>
<div id="update"></div>
<form name="formulario" id="formulario" method="post" action="">
<textarea rows="10" cols="200" name="sql" id="sql">

</textarea>
</form>
<?php 

$sql = "select $coluna_primaria, * from $tabela where \"CO_CNES\" != '' and \"Latitude1\" = '' and \"CEP\" != ''";

$instalacoes = $db->carregar($sql);

$t=1;

echo "<script>";

foreach($instalacoes as $ins){
	echo "getLatLong('".$ins['MUNICIPIO_LOCADO'].", ".$ins['UF_LOCADO'].", Brasil, ".$ins['CEP']."','{$ins["idserial"]}');";
	
	if($t==5) {
		sleep(20);
		$t=1;
	}

	//$t++;
	
}
echo "</script>";

?>
</body>
</html>