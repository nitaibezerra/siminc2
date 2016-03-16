<html>
  <head>
    <meta http-equiv="content-type" content="text/html; charset=iso-8859-1"/>
    <title>Mapa</title>
    <?
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
		$local= explode("/",curPageURL());
		
		$idobra=$_REQUEST["obrid"];
		if(!$_REQUEST["obrid"])
			$idobra=$_SESSION["obra"]["obrid"];
	
    ?>
	<?if ( $local[2]==URL_BASE_RAIZ || $local[2]==URL_BASE_IP ){ ?>
    	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=<?= GOOGLE_MAPS_KEY ?>" type="text/javascript"></script>
  	<? } ?>
  	<?if ($local[2]=="iphan.sisgov.com.br"){ ?>
  		<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAXhZnWO_1DxrDAaxPju9PWxQE-xjEYOv_XgKQVMqE9KQIm6kWpxRFwqr_8JB_SrnNU0aLqxSnUP6GWA" type="text/javascript"></script> 
	<? } ?>
	<?if ($local[2]=="simec-iphan" ){ ?>
    	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAWbyMUakcN-USJ9UnwuCEyBSTw2qJ2MupfoANqXOEZxwM6K_UnRQneKmHr0NA6upzfyzGbZU-fmQGgQ" type="text/javascript"></script>
  	<? } ?>
	<?if ($local[2]=="iphan" ){ ?>
    	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAXhZnWO_1DxrDAaxPju9PWxSyQOI8iiCb7d-H89u4CrP8RWbpCBTbjHIMUHRuAbk4D1HCxvQINXl_Mw" type="text/javascript"></script>
  	<? } ?>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <script type="text/javascript">	
    function initialize() {
      if (GBrowserIsCompatible()) {
        //var map = new GMap2(document.getElementById("mapa"));
        var options = {showOnLoad : true, suppressInitialResultSelection : true};

        var map = new GMap2(document.getElementById("mapa"),{googleBarOptions:options}) 
       	 map.enableGoogleBar();
       	
       	// Preenchendo o mapa
		<?php if ($_REQUEST["latitude"]!='0'){ ?>
			var zoom = window.opener.document.getElementById("endzoom").value;
			var lat = -<? echo $_REQUEST["latitude"] ?>;
	        var lng = -<? echo $_REQUEST["longitude"] ?>;
			if (zoom=='') zoom = 14;
			map.setCenter(new GLatLng(lat,lng), parseInt(zoom));

		    // Criando o ícone do mapa
			var baseIcon = new GIcon();
			baseIcon.shadow = "http://www.google.com/mapfiles/shadow50.png";
			baseIcon.iconSize = new GSize(30, 30);
			baseIcon.shadowSize = new GSize(30, 30);
			baseIcon.iconAnchor = new GPoint(9, 34);
			baseIcon.infoWindowAnchor = new GPoint(9, 2);
			baseIcon.infoShadowAnchor = new GPoint(18, 25);
			
			// Criando os Marcadores com o resultado
	        baseIcon.image='/imagens/icone_capacete.png'
			
			var posn = new GLatLng(lat, lng);
			var icon = baseIcon;
			var title = 'Clique para ver os detalhes';
			var html = "Ponto informado";
			var marker = createMarker(posn,title,icon, html); 
			map.addOverlay(marker);
				
		<?php } else {?>
			
			var cidade = window.opener.document.getElementById("muncod").options[window.opener.document.getElementById("muncod").selectedIndex].text.substring(5);
			var estado = window.opener.document.getElementById("estuf").options[window.opener.document.getElementById("estuf").selectedIndex].text;
			if (!cidade)
				cidade = 'Brasília';
			if (!estado || estado == ' ')
				estado = 'DF';
			endereco= cidade+", "+estado+", Brasil";
			
		    document.getElementById("proximo").value=endereco;
		    // vai para o local apxoximado
		    try{
		    var address = endereco;
		      var geocoder = new GClientGeocoder();
		      if (geocoder) {
		        	geocoder.getLatLng(
		          	address,
		          	function(point) {
			            if (!point) {
		              	alert(address + " Não encontrado"); return false;
		            	} else {
			              	map.setCenter(point, 13);
			              	var marker = new GMarker(point);
			              	map.addOverlay(marker);
			              	marker.openInfoWindowHtml(address);
			              	start = point;
		            	}
		          	}
		        );
		      }
		     } catch(e){}
			
		<? } ?>  
        map.addControl(new GSmallMapControl());
        map.addControl(new GMapTypeControl());
        map.enableScrollWheelZoom();
        map.setMapType(G_HYBRID_MAP);
        GEvent.addListener(map,"mousemove",function(latlng) {
	       	document.getElementById('lat').value=dec2grau(latlng.lat());
	        document.getElementById('lng').value=dec2grau(latlng.lng());
         });
         <?php 
$habilitado = true;
if($habilitado){ ?> 
        GEvent.addListener(map,"click", function(overlay,latlng) {
          var tileCoordinate = new GPoint();
          var tilePoint = new GPoint();
          var currentProjection = G_NORMAL_MAP.getProjection();
          tilePoint = currentProjection.fromLatLngToPixel(latlng, map.getZoom());
          var myHtml = "Latitude: " + dec2grau(latlng.lat()) +"<br/>Longitude: " + dec2grau(latlng.lng()) 
				+ "<br/> Zoom:  " + map.getZoom() + "<br> <a href=# onClick='javascript: copiar("+latlng.lat()+","+latlng.lng()+","+map.getZoom()+")'>[Definir local da obra neste ponto]</a>";	
          map.openInfoWindow(latlng, myHtml);
        });
        <?php } ?>
      }
    }

    function copiar(lat, lng, z){

		ddLat=lat+"";

		if (ddLat.substr(0,1) == "-") {
			ddLatVal = ddLat.substr(1,ddLat.length-1);
			window.opener.document.getElementById("pololatitude").value = "S";
		} else {
			ddLatVal = ddLat;
			window.opener.document.getElementById("pololatitude").value = "N";
		}
		
		// Graus Lat 
		ddLatVals = ddLatVal.split(".");
		dmsLatDeg = ddLatVals[0];
		window.opener.document.getElementById("graulatitude").value=dmsLatDeg;
		
		// * 60 = mins
		ddLatRemainder  = ("0." + ddLatVals[1]) * 60;
		dmsLatMinVals   = ddLatRemainder.toString().split(".");
		dmsLatMin = dmsLatMinVals[0];
		window.opener.document.getElementById("minlatitude").value=dmsLatMin;
			
		// * 60 novamente = secs
		ddLatMinRemainder = ("0." + dmsLatMinVals[1]) * 60;
		dmsLatSec  = Math.round(ddLatMinRemainder);
		window.opener.document.getElementById("seglatitude").value=dmsLatSec;
		
		ddLat=lng+"";
		if (ddLat.substr(0,1) == "-") {
			ddLatVal = ddLat.substr(1,ddLat.length-1);
		} else {
			ddLatVal = ddLat;
		}
		// Graus Long 
		ddLatVals = ddLatVal.split(".");
		dmsLatDeg = ddLatVals[0];
		window.opener.document.getElementById("graulongitude").value=dmsLatDeg;
		
		// * 60 = mins
		ddLatRemainder  = ("0." + ddLatVals[1]) * 60;
		dmsLatMinVals   = ddLatRemainder.toString().split(".");
		dmsLatMin = dmsLatMinVals[0];
		window.opener.document.getElementById("minlongitude").value=dmsLatMin;
			
		// * 60 novamente = secs
		ddLatMinRemainder = ("0." + dmsLatMinVals[1]) * 60;
		dmsLatSec  = Math.round(ddLatMinRemainder);
		window.opener.document.getElementById("seglongitude").value=dmsLatSec;
		
		window.opener.document.getElementById("endzoom").value=z;
		
		window.opener.focus();
	   	window.close();
    	
    }
	
	 function createMarker(posn, title, icon, html) {
      var marker = new GMarker(posn, {title: title, icon: icon, draggable:false });
      GEvent.addListener(marker, "click", function() {
      	marker.openInfoWindowHtml(html);
       });

      return marker;
      
    }
 
 	// Converte para corrdenada Gmaps
 	function grau2dec(valor){
 	var valor=valor.split(".");	
 	valor=((((Number(valor[2]) / 60 ) + Number(valor[1])) / 60 ) + Number(valor[0]));
	if (valor[3]!="N")
		valor = valor *-1;
	return valor
 		
 	}
     // Converter em Graus
     function dec2grau(valor){
		ddLat=valor+"";

		if (ddLat.substr(0,1) == "-") {
			ddLatVal = ddLat.substr(1,ddLat.length-1);
		} else {
			ddLatVal = ddLat;
		}
		
		// Graus 
		ddLatVals = ddLatVal.split(".");
		dmsLatDeg = ddLatVals[0];
		
		// * 60 = mins
		ddLatRemainder  = ("0." + ddLatVals[1]) * 60;
		dmsLatMinVals   = ddLatRemainder.toString().split(".");
		dmsLatMin = dmsLatMinVals[0];
			
		// * 60 novamente = secs
		ddLatMinRemainder = ("0." + dmsLatMinVals[1]) * 60;
		dmsLatSec  = Math.round(ddLatMinRemainder);
		
		if (ddLat.substr(0,1) == "-") {
			valor="-"+dmsLatDeg+unescape('%B0')+" "+dmsLatMin+"' "+dmsLatSec+"''";
		} else {
			valor=dmsLatDeg+unescape('%B0')+" "+dmsLatMin+"' "+dmsLatSec+"''";
		}
		
     	return valor;
     }
    </script>
<style>
.body {
BORDER-BOTTOM: 0px; BORDER-LEFT: 0px; BORDER-RIGHT: 0px; BORDER-TOP: 0px}
</style>
  </head>
  <body onload="initialize()" onunload="GUnload()">
    <form action="#">
    <table bgcolor=#c3c3c3 width=530><tr><td align=center>
    <?php if($habilitado){ ?><font color=blue>Navegue pelo mapa e clique sobre o ponto desejado para definir as coordenadas.<? } ?></font>
    </td></tr></table>
    <div id="mapa" style="width: 530px; height: 550px">
    </div>
    <table bgcolor=#c3c3c3 width=530>
    <tr>
    <td>
    Latitude: <input type=text id=lat value="0" STYLE="border:none ; background-color:#c3c3c3" size=8 /> 
    Longitude: <input type=text id=lng value="0" STYLE="border:none; background-color:#c3c3c3" size=8 />
    <input type=text id=proximo value="" STYLE="border:none; background-color:#c3c3c3" size=40 />
    </td>
    </tr>
    </table>
    </form>
  </body>
</html>

