<?php
// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

if(!$_SESSION['usucpf'])
	echo "<script>
			alert('Problemas com autenticação.');
			window.close();
		  </script>";


// abre conexão com o servidor de banco de dados
$db = new cls_banco();

if($_REQUEST['requisicao'] == "validarlatlongmunicipio") {
	$sql = "SELECT seguranca.distanciaPontosGPS(
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
	ob_clean();
	//echo $sql;
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
	<meta http-equiv="Cache-Control" content="no-cache">
	<meta http-equiv="Pragma" content="no-cache">
	<meta http-equiv="Expires" content="-1">


    <title>Mapa</title>
    <link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
	<script type="text/javascript" src="https://maps.googleapis.com/maps/api/js?sensor=false"></script>    
        <script type="text/javascript" src="/includes/prototype.js"></script>
    
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
    <div id="mapa" style="width: 530px; height: 550px"></div>
    <!-- 
    <table bgcolor=#c3c3c3 width=530>
    <tr>
    <td align="center">
    Latitude: <input type=text id=lat value="0" STYLE="border:none ; background-color:#c3c3c3" size=8 /> 
    Longitude: <input type=text id=lng value="0" STYLE="border:none; background-color:#c3c3c3" size=8 />
    <input type=text id=proximo value="" STYLE="border:none; background-color:#c3c3c3" size=40 />
    </td>
    </tr>
    <tr>
    <td align="center" style="color:000099;">
    	Se desejar, digite as coordenadas abaixo e clique em visualizar.
    </td>
    </tr>
    <tr>
    <td align="center">
    Latitude:  <input type="text" class="normal" size="3" maxlength="2" id="__graulatitude"> º <input type="text" class="normal" size="3" maxlength="2" id="__minlatitude"> ' <input type="text" class="normal" size="3" maxlength="2" id="__seglatitude"> " <select id="__pololatitude"><option value="S">S</option><option value="N">N</option></select>
    Longitude: <input type="text" class="normal" size="3" maxlength="2" id="__graulongitude"> º <input type="text" class="normal" size="3" maxlength="2" id="__minlongitude"> ' <input type="text" class="normal" size="3" maxlength="2" id="__seglongitude"> " <select id="__pololongitude"><option value="W">W</option><option value="E">E</option></select> 
    <input type="button" name="visualizar" value="Visualizar" onclick="visualizarPontosManuais();">
    </td>
    </tr>

    </table>
     -->
    </form>
  </body>
    <script type="text/javascript">	
    document.getElementById("mapa").innerHTML = "Carregando...";
    
    function marcarPontosManuais() {
	    var latitude;
    	var longitude;
    
	    if(document.getElementById("__seglatitude").value && 
	       document.getElementById("__minlatitude").value &&
	       document.getElementById("__graulatitude").value &&
	       document.getElementById("__seglongitude").value &&
	       document.getElementById("__seglongitude").value &&
	       document.getElementById("__seglongitude").value) {
	       
	    	var latitude  = ((( Number(document.getElementById("__seglatitude").value) / 60 ) + Number(document.getElementById("__minlatitude").value)) / 60 ) + Number(document.getElementById("__graulatitude").value);
			var longitude = ((( Number(document.getElementById("__seglongitude").value) / 60 ) + Number(document.getElementById("__minlongitude").value)) / 60 ) + Number(document.getElementById("__graulongitude").value);
		}
		
		if(document.getElementById("__pololatitude").value == "S") {
			latitude = latitude * -1;
		}
		
		if(document.getElementById("__pololongitude").value == "W") {
			longitude = longitude * -1;
		}
		
		
		if(!latitude || !longitude) {
			alert("Digite as coordenadas geograficas");
			return false;
		}
		
		var lat = latitude;
        var lng = longitude;

		copiar(lat,lng,window.opener.document.getElementById("endzoom<? echo $_REQUEST['tipoendereco']; ?>").value);

    }
    
    function visualizarPontosManuais() {
    
    var latitude;
    var longitude;
    
    if(document.getElementById("__seglatitude").value && 
       document.getElementById("__minlatitude").value &&
       document.getElementById("__graulatitude").value &&
       document.getElementById("__seglongitude").value &&
       document.getElementById("__seglongitude").value &&
       document.getElementById("__seglongitude").value) {
       
    	var latitude  = ((( Number(document.getElementById("__seglatitude").value) / 60 ) + Number(document.getElementById("__minlatitude").value)) / 60 ) + Number(document.getElementById("__graulatitude").value);
		var longitude = ((( Number(document.getElementById("__seglongitude").value) / 60 ) + Number(document.getElementById("__minlongitude").value)) / 60 ) + Number(document.getElementById("__graulongitude").value);
	}
	
	if(latitude=='' || longitude=='') {
		alert("Digite as coordenadas geograficas");
		return false;
	}
	
	if(document.getElementById("__pololatitude").value == "S") {
		latitude = latitude * -1;
	}
		
	if(document.getElementById("__pololongitude").value == "W") {
		longitude = longitude * -1;
	}
    
    if (GBrowserIsCompatible()) {
      
      
      	var zoom = window.opener.document.getElementById("endzoom<? echo $_REQUEST['tipoendereco']; ?>").value;
        //var map = new GMap2(document.getElementById("mapa"));
        var options = {showOnLoad : true, suppressInitialResultSelection : true};
        var map = new GMap2(document.getElementById("mapa"),{googleBarOptions:options}) 
      	map.enableGoogleBar();
		var lat = latitude;
        var lng = longitude;
		if (zoom=='') zoom = 14;
		map.setCenter(new GLatLng(lat,lng), parseInt(zoom));
		
	    // Criando o ícone do mapa
		var baseIcon = new GIcon();
		baseIcon.iconSize = new GSize(18, 28);
		baseIcon.shadowSize = new GSize(18, 28);
		baseIcon.iconAnchor = new GPoint(9, 34);
		baseIcon.infoWindowAnchor = new GPoint(9, 2);
		baseIcon.infoShadowAnchor = new GPoint(18, 25);
		
		// Criando os Marcadores com o resultado
        baseIcon.image='/imagens/tachinha_b.png';
		
		var posn = new GLatLng(lat, lng);
		var icon = baseIcon;
		var title = 'Clique para ver os detalhes';
		var entidTemp = '<? echo $_REQUEST['entid']; ?>';
		if( entidTemp == ""){
			var html = "Não implementado";
		} else {
			var html = "<iframe src='/apigoogle/php/mapa_balao.php?entid=<? echo $_REQUEST['entid']; ?>' width=350 frameborder=0 ></iframe>";
		}
		var marker = createMarker(posn,title,icon, html); 
		map.addOverlay(marker);
		
        map.addControl(new GSmallMapControl());
        map.addControl(new GMapTypeControl());
        map.enableScrollWheelZoom();
        map.setMapType(G_HYBRID_MAP);
        
        
        GEvent.addListener(map,"mousemove",function(latlng) {
	       	document.getElementById('lat').value=dec2grau(latlng.lat());
	        document.getElementById('lng').value=dec2grau(latlng.lng());
         });
         
        GEvent.addListener(map,"click", function(overlay,latlng) {
          var tileCoordinate = new GPoint();
          var tilePoint = new GPoint();
          var currentProjection = G_NORMAL_MAP.getProjection();
          tilePoint = currentProjection.fromLatLngToPixel(latlng, map.getZoom());
          var myHtml = "Latitude: " + dec2grau(latlng.lat()) +"<br/>Longitude: " + dec2grau(latlng.lng()) 
				+ "<br/> Zoom:  " + map.getZoom() + "<br> <a href=# onClick='javascript: copiar("+latlng.lat()+","+latlng.lng()+","+map.getZoom()+")'>[Definir local neste ponto]</a>";
          map.openInfoWindow(latlng, myHtml);
        });



	}

    }
    
    
    
function initialize() {

	<?php if ($_REQUEST["latitude"]!='0') : ?>

	var zoom = window.opener.document.getElementById("endzoom<? echo $_REQUEST['tipoendereco']; ?>").value;
	var lat = <? echo ((trim($_REQUEST["polo"])=="S")?"-".$_REQUEST["latitude"]:$_REQUEST["latitude"]); ?>;
	var lng = <? echo ((trim($_REQUEST["poloLong"])=="W")?"-".$_REQUEST["longitude"]:$_REQUEST["longitude"]); ?>;
	if (zoom=='') zoom = 14;
	
  	var mapOptions = {
    zoom: parseInt(zoom),

    panControl:true,
    zoomControl:true,
    mapTypeControl:true,
    scaleControl:true,
    streetViewControl:true,
    overviewMapControl:true,
    rotateControl:true,
    disableDefaultUI:false,
    
    center: new google.maps.LatLng(lat, lng),
    mapTypeId: google.maps.MapTypeId.ROADMAP
  	};

  	var map = new google.maps.Map(document.getElementById('mapa'),
    	  mapOptions);
    	  
    	  
	var entidTemp = '<? echo $_REQUEST['entid']; ?>';
	if( entidTemp == ""){
		var contentString = "Não implementado";
	} else {
		var contentString = "<iframe src='/apigoogle/php/mapa_balao.php?entid=<? echo $_REQUEST['entid']; ?>' width=350 frameborder=0 ></iframe>";
	}

  	var infowindow = new google.maps.InfoWindow({
      content: contentString
  	});

  	var marker = new google.maps.Marker({
      position: new google.maps.LatLng(lat, lng),
      map: map,
      title: 'Hospital'
  	});
  	
  	google.maps.event.addListener(marker, 'click', function() {
    	infowindow.open(map,marker);
  	});
  	
  	google.maps.event.addListener(map, 'click', function(e) {
    var myHtml = "Latitude: " + dec2grau(e.latLng.lat()) +"<br/>Longitude: " + dec2grau(e.latLng.lng()) 
					+ "<br/> Zoom:  " + map.getZoom() + "<br> <a href=# onClick='javascript: copiar("+e.latLng.lat()+","+e.latLng.lng()+","+map.getZoom()+")'>[Definir local neste ponto]</a>";
	
  	var infowindow2 = new google.maps.InfoWindow({
      content: myHtml,
      position: e.latLng
  	});

	infowindow2.open(map);

  	});

	
	<? else :?>
	
	geocoder = new google.maps.Geocoder();
	
	var cep = window.opener.document.getElementById("endcep<? echo $_REQUEST['tipoendereco']; ?>").value;
	var pais = '';
	if (window.opener.document.getElementById("paiid<? echo $_REQUEST['tipoendereco']; ?>")){
		pais = window.opener.document.getElementById("paiid<? echo $_REQUEST['tipoendereco']; ?>").options[window.opener.document.getElementById("paiid<? echo $_REQUEST['tipoendereco']; ?>").selectedIndex].text;
	}
	var cidade = window.opener.document.getElementById("mundescricao<? echo $_REQUEST['tipoendereco']; ?>").value;
	var estado = window.opener.document.getElementById("estuf<? echo $_REQUEST['tipoendereco']; ?>").value;
	var bairro=	window.opener.document.getElementById("endbai<? echo $_REQUEST['tipoendereco']; ?>").value;

	if (pais == ''){
		pais = 'Brasil';
	}
	console.log(pais);
	var novabusca = '<?php echo $_REQUEST['novabusca']; ?>';
	if(novabusca){
		endereco= cidade+", "+estado+", "+pais;
	} else {
		endereco= cep+", "+cidade+", "+estado+", "+pais;
	}

    geocoder.geocode( { 'address': endereco}, function(results, status) {
    if (status == google.maps.GeocoderStatus.OK) {
    
	  	var mapOptions = {
	    zoom: 10,
	    
	    panControl:true,
	    zoomControl:true,
	    mapTypeControl:true,
	    scaleControl:true,
	    streetViewControl:true,
	    overviewMapControl:true,
	    rotateControl:true,
	    disableDefaultUI:false,
	    
	    center: results[0].geometry.location,
	    mapTypeId: google.maps.MapTypeId.ROADMAP
	  	};
	
	  	var map = new google.maps.Map(document.getElementById('mapa'),
	    	  mapOptions);
	    	  
	  	google.maps.event.addListener(map, 'click', function(e) {
	    var myHtml = "Latitude: " + dec2grau(e.latLng.lat()) +"<br/>Longitude: " + dec2grau(e.latLng.lng()) 
						+ "<br/> Zoom:  " + map.getZoom() + "<br> <a href=# onClick='javascript: copiar("+e.latLng.lat()+","+e.latLng.lng()+","+map.getZoom()+")'>[Definir local neste ponto]</a>";
		
	  	var infowindow2 = new google.maps.InfoWindow({
	      content: myHtml,
	      position: e.latLng
	  	});
	
		infowindow2.open(map);
	
	  	});

	

    } else {
      alert('Geocode was not successful for the following reason: ' + status);
    }
  	});

	
	
	<? endif; ?>


}
    

    function copiar(lat, lng, z){
		var ddLat=lat+"";
		var ddLng=lng+"";
		var validadistancia = true;
		
		if(window.opener.document.getElementById("muncod<? echo $_REQUEST['tipoendereco']; ?>").value != "") {
			
			var myAjax = new Ajax.Request(
				window.location.href,
				{
					method: 'post',
					parameters: 'requisicao=validarlatlongmunicipio&lat='+lat+'&lng='+lng+'&muncod='+window.opener.document.getElementById("muncod<? echo $_REQUEST['tipoendereco']; ?>").value,
					asynchronous: false,
					onComplete: function(resp) {
						//alert(resp.responseText);
						//return false;
						if(resp.responseText == "FALSE") {
							validadistancia = false;
						}
					},
					onLoading: function(){}
				});
				//return false;
			
			if(!validadistancia) {
				alert("As coordenadas não correspondem aos limites do munícipio,\n verifique se você está marcando no município desejado.");
				return false;
			}
			
		}

		if (ddLat.substr(0,1) == "-") {
			ddLatVal = ddLat.substr(1,ddLat.length-1);
		} else {
			ddLatVal = ddLat;
		}
		
		// Graus Lat 
		var ddLatVals = ddLatVal.split(".");
		if(ddLatVals[0]) {
			var dmsLatDeg = ddLatVals[0];
			window.opener.document.getElementById("graulatitude<? echo $_REQUEST['tipoendereco']; ?>").value=dmsLatDeg;
			window.opener.document.getElementById("_graulatitude<? echo $_REQUEST['tipoendereco']; ?>").innerHTML=dmsLatDeg;
		}
		
		// * 60 = mins
		if(ddLatVals[1]) {
			var ddLatRemainder  = ("0." + ddLatVals[1]) * 60;
			var dmsLatMinVals   = ddLatRemainder.toString().split(".");
			var dmsLatMin = dmsLatMinVals[0];
			window.opener.document.getElementById("minlatitude<? echo $_REQUEST['tipoendereco']; ?>").value=dmsLatMin;
			window.opener.document.getElementById("_minlatitude<? echo $_REQUEST['tipoendereco']; ?>").innerHTML=dmsLatMin;
		}
			
		// * 60 novamente = secs
		if(dmsLatMinVals[1]) {
			var ddLatMinRemainder = ("0." + dmsLatMinVals[1]) * 60;
			var dmsLatSec  = Math.round(ddLatMinRemainder);
			window.opener.document.getElementById("seglatitude<? echo $_REQUEST['tipoendereco']; ?>").value=dmsLatSec;
			window.opener.document.getElementById("_seglatitude<? echo $_REQUEST['tipoendereco']; ?>").innerHTML=dmsLatSec;
		}
		
		if (ddLat.substr(0,1) == "-") {
			var ddLatVal = ddLat.substr(1,ddLat.length-1);
			window.opener.document.getElementById("pololatitude<? echo $_REQUEST['tipoendereco']; ?>").value = "S";
			window.opener.document.getElementById("_pololatitude<? echo $_REQUEST['tipoendereco']; ?>").innerHTML = "S";
		} else {
			var ddLatVal = ddLat;
			window.opener.document.getElementById("pololatitude<? echo $_REQUEST['tipoendereco']; ?>").value = "N";
			window.opener.document.getElementById("_pololatitude<? echo $_REQUEST['tipoendereco']; ?>").innerHTML = "N";
		}
		
		if (ddLng.substr(0,1) == "-") {
			ddLngVal = ddLng.substr(1,ddLng.length-1);
		} else {
			ddLngVal = ddLng;
		}

		// Graus Long 
		ddLngVals = ddLngVal.split(".");
		if(ddLngVals[0]) {
			var dmsLngDeg = ddLngVals[0];
			window.opener.document.getElementById("graulongitude<? echo $_REQUEST['tipoendereco']; ?>").value=dmsLngDeg;
			window.opener.document.getElementById("_graulongitude<? echo $_REQUEST['tipoendereco']; ?>").innerHTML=dmsLngDeg;
		}
		
		// * 60 = mins
		if(ddLngVals[1]) {
			ddLngRemainder  = ("0." + ddLngVals[1]) * 60;
			dmsLngMinVals   = ddLngRemainder.toString().split(".");
			dmsLngMin = dmsLngMinVals[0];
			window.opener.document.getElementById("minlongitude<? echo $_REQUEST['tipoendereco']; ?>").value=dmsLngMin;
			window.opener.document.getElementById("_minlongitude<? echo $_REQUEST['tipoendereco']; ?>").innerHTML=dmsLngMin;
		}
			
		// * 60 novamente = secs
		if(dmsLngMinVals[1]) {
			ddLngMinRemainder = ("0." + dmsLngMinVals[1]) * 60;
			dmsLngSec  = Math.round(ddLngMinRemainder);
			window.opener.document.getElementById("seglongitude<? echo $_REQUEST['tipoendereco']; ?>").value=dmsLngSec;
			window.opener.document.getElementById("_seglongitude<? echo $_REQUEST['tipoendereco']; ?>").innerHTML=dmsLngSec;
		}

		if (ddLng.substr(0,1) == "-") {
			ddLngVal = ddLat.substr(1,ddLng.length-1);
			window.opener.document.getElementById("pololongitude<? echo $_REQUEST['tipoendereco']; ?>").value = "W";
			window.opener.document.getElementById("_pololongitude<? echo $_REQUEST['tipoendereco']; ?>").innerHTML = "W";
		} else {
			ddLngVal = ddLng;
			window.opener.document.getElementById("pololongitude<? echo $_REQUEST['tipoendereco']; ?>").value = "E";
			window.opener.document.getElementById("_pololongitude<? echo $_REQUEST['tipoendereco']; ?>").innerHTML = "E";
		}
		
		window.opener.document.getElementById("endzoom<? echo $_REQUEST['tipoendereco']; ?>").value=z;
		
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
 	valor=((((Number(valor[2]) / 60 ) + Number(valor[1])) / 60 ) + Number(valor[0]))*-1;

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
		
		valor=dmsLatDeg+unescape('%B0')+" "+dmsLatMin+"' "+dmsLatSec+"''";

     	return valor;
     }
     initialize();
</script>
</html>
