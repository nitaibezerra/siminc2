<?
$habilitado = true;

if($_REQUEST['requisicao'] == "validarlatlongmunicipio") {
	$sql = "SELECT distanciaPontosGPS(
										CASE WHEN (length (mun.munmedlat)=8) THEN 
											CASE WHEN length(REPLACE('0' || mun.munmedlat,'S','')) = 8 THEN
												((SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),1,2)::double precision))*(-1)
											ELSE
												(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),1,2)::double precision)
											END
										ELSE
											CASE WHEN length(REPLACE(mun.munmedlat,'S','')) = 8 THEN
												((SUBSTR(REPLACE(mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),1,2)::double precision))*(-1)
											ELSE
												0--((SUBSTR(REPLACE(mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'N',''),1,2)::double precision))
											END
										END,
										
										(SUBSTR(REPLACE(mun.munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(mun.munmedlog,'W',''),3,2)::double precision/60)) *(-1),
										'".$_REQUEST['lat']."',
										'".$_REQUEST['lng']."') as distancia,
										
				CAST(munmedraio as integer)*1000 as munmedraio
			FROM territorios.municipio mun
			WHERE mun.muncod='".$_REQUEST['muncod']."'";
	
	$dados = $db->pegaLinha($sql);
	
	if($dados['distancia'] > $dados['munmedraio']) {
		echo "FALSE";
	} else {
		echo "TRUE";
	}
	exit;
}

?>
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
		
    ?>
    <script type="text/javascript" src="https://maps.googleapis.com/maps/api/js?sensor=false"></script>

    <script type="text/javascript" src="/includes/prototype.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    
    <script type="text/javascript">
      var map;
      var infowindow;
      
      function initialize() {
      
		<? if ($_REQUEST["latitude"]!='0') : ?>
		
		var polo 	 = window.opener.document.getElementById("pololatitude_").innerHTML;
		
		if(polo=="N") {var latitude = <? echo $_REQUEST["latitude"] ?>;} else {var latitude = -<? echo $_REQUEST["latitude"] ?>;}
		
		var longitude = -<? echo $_REQUEST["longitude"] ?>;
		
		var myLatlng = new google.maps.LatLng(latitude, longitude);
		
        var myOptions = {
          zoom: parseInt(window.opener.document.getElementById("endzoom").value),
          center: myLatlng,
          mapTypeId: google.maps.MapTypeId.ROADMAP
        };
        
        map = new google.maps.Map(document.getElementById('mapa'),
            myOptions);

		var icon = new google.maps.MarkerImage("/imagens/icone_capacete_5.png");
        
        var marker = new google.maps.Marker({
        position: myLatlng, 
        icon: icon,
        map: map,
        title:"Clique para ver os detalhes"
    	});

		var html_icon = window.opener.document.getElementById("pdenome").innerHTML;    	
	    google.maps.event.addListener(marker, "click", function() {if (infowindow) infowindow.close(); infowindow = new google.maps.InfoWindow({content: html_icon}); infowindow.open(map, marker); });
    	
        <? if($habilitado) : ?>
		google.maps.event.addListener(map, 'click', function(event) {

          var html = "Latitude: " + dec2grau(event.latLng.lat()) +"<br/>Longitude: " + dec2grau(event.latLng.lng()) 
				+ "<br/> Zoom: "+map.getZoom()+" <br> <a href=# onClick='javascript: copiar("+event.latLng.lat()+","+event.latLng.lng()+","+map.getZoom()+")'>[Definir escola neste ponto]</a>";	

		  infowindow = new google.maps.InfoWindow({content: html});
		  infowindow.setPosition(event.latLng)
		  infowindow.open(map);
 
        });
        <? endif; ?>
        
        <? else : ?>
        
		var cep = window.opener.document.getElementById("pdecep").value;
		var cidade = window.opener.document.getElementById("mundescricao").value;
		var estado = window.opener.document.getElementById("estuf").value;
		var bairro=	window.opener.document.getElementById("pdebairro").value;
		
		var endereco= cep+", "+cidade+", "+estado+", Brasil";
		if (cep.substr(5,8) == '000');
			endereco= cidade+", "+estado+", Brasil";
        
	    geocoder = new google.maps.Geocoder();
	    
	    var latlng = new google.maps.LatLng(-14.689881, -52.373047);
	    var myOptions = {
	      zoom: 4,
	      center: latlng,
	      mapTypeId: google.maps.MapTypeId.ROADMAP
	    }
	    
	    map = new google.maps.Map(document.getElementById("mapa"), myOptions);
	    
        <? if($habilitado) : ?>
		google.maps.event.addListener(map, 'click', function(event) {

          var html = "Latitude: " + dec2grau(event.latLng.lat()) +"<br/>Longitude: " + dec2grau(event.latLng.lng()) 
				+ "<br/> Zoom: "+map.getZoom()+" <br> <a href=# onClick='javascript: copiar("+event.latLng.lat()+","+event.latLng.lng()+","+map.getZoom()+")'>[Definir escola neste ponto]</a>";	

		  infowindow = new google.maps.InfoWindow({content: html});
		  infowindow.setPosition(event.latLng)
		  infowindow.open(map);
 
        });
        <? endif; ?>

        
	    var address = endereco;
	    geocoder.geocode( { 'address': address}, function(results, status) {
	      if (status == google.maps.GeocoderStatus.OK) {
      		var icon = new google.maps.MarkerImage("/imagens/icone_capacete_5.png");
	        map.setCenter(results[0].geometry.location);
	        var marker = new google.maps.Marker({
        		icon: icon,
	            map: map,
	            position: results[0].geometry.location
	        });
	      } else {
	        alert("Geocode was not successful for the following reason: " + status);
	      }
	    });
        <? endif; ?>
        
      }

      google.maps.event.addDomListener(window, 'load', initialize);
    </script>
    
    
    <script type="text/javascript">	

    function copiar(lat, lng, z){

		ddLat=lat+"";
		var validadistancia = true;
		
		if(window.opener.document.getElementById("muncod").value != "") {
			
			var myAjax = new Ajax.Request(
				window.location.href,
				{
					method: 'post',
					parameters: 'requisicao=validarlatlongmunicipio&lat='+lat+'&lng='+lng+'&muncod='+window.opener.document.getElementById("muncod").value,
					asynchronous: false,
					onComplete: function(resp) {
						if(resp.responseText == "FALSE") {
							validadistancia = false;
						}
					},
					onLoading: function(){}
				});
			
			if(!validadistancia) {
				alert("As coordenadas não correspondem os limites do munícipio");
				return false;
			}
			
		}
		
		

		if (ddLat.substr(0,1) == "-") {
			ddLatVal = ddLat.substr(1,ddLat.length-1);
			window.opener.document.getElementById("pololatitude").value = "S";
			window.opener.document.getElementById("pololatitude_").innerHTML = "S";
		} else {
			ddLatVal = ddLat;
			window.opener.document.getElementById("pololatitude").value = "N";
			window.opener.document.getElementById("pololatitude_").innerHTML = "S";
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
  <body>
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

