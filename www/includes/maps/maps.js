//Excuta após a página ser carregada
jQuery(document).ready(function() {
  jQuery("#accordion").accordion();
  jQuery('#color_filtro_avancado').colorPicker();
});

//Variáveis globais
var map;
var markerClusterer = null;
var arrMarkers = new Array();

//Área do clique do marcador
var shape = {
    coord: [1, 1, 1, 20, 18, 20, 18 , 1],
    type: 'poly'
};

//Array de poligonos
var nomePoli = new Array();

//Array de cores do poligono
var corPoli = new Array();

//Array do centro do poligonos de estado
var centroPoliEstado = new Array();

//Array do centro do poligonos de tipo de municipios
var centroPoliTipoMunicipio = new Array();

// Cor de destaque do municipio
var corDestaquePoliMunicipio = "#d82b40";

// Cor do municipio 
var corPoliMunicipio = "#f6ead9";

//Array de dados do município
var arrMuncod = new Array();

//Array de estados com seus municípios
var estadoMunicipio = new Array();

var estado = new Array();

// Informações dos pontos
var infowindow;

// Array de pontos dos icones
var pontoMarcadores = new Array();


//Variáveis para controle de rota
var directionDisplay;
var directionsService;
var stepDisplay;
var markerArray = [];

/*
 * Função de inicialização do google maps
 */
function initialize() {
	var myLatLng = new google.maps.LatLng(-14.689881, -52.373047);
   	var center = myLatLng;
   	var myOptions = {
     zoom: 4,
     center: myLatLng,
     mapTypeId: google.maps.MapTypeId.ROADMAP
   };
    
	map = new google.maps.Map(document.getElementById("map_canvas"),
        myOptions);

	/* Rotas (origem e destino) */
	var rendererOptions = {
      map: map,
      polylineOptions:{zIndex:5,strokeColor:'#0000CC',strokeOpacity:0.5},
      draggable: true
      
    }
    
    directionsDisplay = new google.maps.DirectionsRenderer(rendererOptions)
	
    directionsService = new google.maps.DirectionsService();
    
    directionsDisplay.setMap(map);
    directionsDisplay.setPanel(document.getElementById("directionsPanel"));
    
    // Instantiate an info window to hold step text.
    stepDisplay = new google.maps.InfoWindow();
    
    
	google.maps.event.addListener(map, "rightclick",function(event){showContextMenu(event.latLng);});

} //fim do initialize


/*
 * Centraliza o mapa em determianadas coordenadas e zoom 
 */
function centraliza(zoom, coords) {
  	zoom = parseInt(zoom);
  	map.setZoom(zoom);
	map.setCenter(new google.maps.LatLng(coords[1], coords[0])); //Centraliza e aplica o zoom
}//fim do centraliza

/*
 * Cria o menu de "Definir rotas a partir" 
 */
function showContextMenu(caurrentLatLng) {
    var LatLng = '' + caurrentLatLng + '';
    LatLng = LatLng.replace("(","");
    LatLng = LatLng.replace(")","");
    var arrLatLng = [];
    arrLatLng = LatLng.split(",");
    var projection;
    var contextmenuDir;
    projection = map.getProjection() ;
    jQuery('.contextmenu').remove();
     contextmenuDir = document.createElement("div");
      contextmenuDir.className  = 'contextmenu';
      contextmenuDir.innerHTML = '<div id="menu1" onclick="defineRotaStart(' + arrLatLng[0] + ',' + arrLatLng[1] + ')" class="context">Como chegar a partir daqui<\/div>'
                              + '<div id="menu2" onclick="defineRotaEnd(' + arrLatLng[0] + ',' + arrLatLng[1] + ')" class="context">Como chegar até aqui<\/div>';

    jQuery(map.getDiv()).append(contextmenuDir);
 
    setMenuXY(caurrentLatLng);

    contextmenuDir.style.visibility = "visible";
    
}//fim do showContextMenu

/*
 * Define as rotas iniciais e abre o Acordion
 */
function defineRotaStart(lat,lng)
{
	jQuery('.contextmenu').hide();
	abreAcordion(3);
	jQuery('#start').val("");
	jQuery('#start_lat').val(lat);
	jQuery('#start_lng').val(lng);
	if(jQuery('#end_lat').val() && jQuery('#end_lng').val()){
		calcRoute();
	}
}//fim defineRotaStart

/*
 * Define as rotas finais e abre o Acordion
 */
function defineRotaEnd(Lat,Lng)
{
	jQuery('.contextmenu').hide();
	jQuery('#end').val("");
	abreAcordion(3);
	jQuery('#end_lat').val(Lat);
	jQuery('#end_lng').val(Lng);
	if(jQuery('#start_lat').val() && jQuery('#start_lng').val()){
		calcRoute();
	}
}//fim defineRotaEnd

/*
 * Pega as coordenadas para gerar o menu de rotas
 */
function getCanvasXY(caurrentLatLng){
   var scale = Math.pow(2, map.getZoom());
   var nw = new google.maps.LatLng(
       map.getBounds().getNorthEast().lat(),
       map.getBounds().getSouthWest().lng()
   );
   var worldCoordinateNW = map.getProjection().fromLatLngToPoint(nw);
   var worldCoordinate = map.getProjection().fromLatLngToPoint(caurrentLatLng);
   var caurrentLatLngOffset = new google.maps.Point(
       Math.floor((worldCoordinate.x - worldCoordinateNW.x) * scale),
       Math.floor((worldCoordinate.y - worldCoordinateNW.y) * scale)
   );
   return caurrentLatLngOffset;
}//fim getCanvasXY

/*
 * Define o menu de rotas do mata 
 */
function setMenuXY(caurrentLatLng){
   var mapWidth = jQuery('#map_canvas').width();
   var mapHeight = jQuery('#map_canvas').height();
   var menuWidth = jQuery('.contextmenu').width();
   var menuHeight = jQuery('.contextmenu').height();
   var clickedPosition = getCanvasXY(caurrentLatLng);
   var x = clickedPosition.x ;
   var y = clickedPosition.y ;

    if((mapWidth - x ) < menuWidth)//if to close to the map border, decrease x position
        x = x - menuWidth;
   if((mapHeight - y ) < menuHeight)//if to close to the map border, decrease y position
       y = y - menuHeight;

   jQuery('.contextmenu').css('left',x  );
   jQuery('.contextmenu').css('top',y );

}//fim setMenuXY

/*
 * Remove a rota
 */
function removeRoute() {
  directionsDisplay.setMap(null);
}//fim removeRoute

/*
 * Calcula e cria a rota no mapa
 */
function calcRoute() {
	directionsDisplay.setMap(map);
	// First, remove any existing markers from the map.
	for (i = 0; i < markerArray.length; i++) {
		markerArray[i].setMap(null);
	}

	// Now, clear the array itself.
	markerArray = [];

	// Retrieve the start and end locations and create
	// a DirectionsRequest using WALKING directions.
	var start = document.getElementById("start").value;
	var end = document.getElementById("end").value;
   
	if(!start){
		var start = new google.maps.LatLng($('#start_lat').val(), $('#start_lng').val());
	}
  
	if(!end){
		var end = new google.maps.LatLng($('#end_lat').val(), $('#end_lng').val());
	}    
	
	var request = {
	    origin: start,
	    destination: end,
	    travelMode: google.maps.DirectionsTravelMode.WALKING
	};
	
	// Route the directions and pass the response to a
	var request = {
	    origin:start, 
	    destination:end,
	    travelMode: google.maps.DirectionsTravelMode.DRIVING
	};

	// function to create markers for each step.
	directionsService.route(request, function(response, status) {
		if (status == google.maps.DirectionsStatus.OK) {
			var warnings = document.getElementById("warnings_panel");
			warnings.innerHTML = "<b>" + response.routes[0].warnings + "</b>";
			directionsDisplay.setDirections(response);
			showSteps(response);
			jQuery('#start').val(response.routes[0].legs[0].start_address);
			jQuery('#end').val(response.routes[0].legs[0].end_address);
	     }
	});
}//fim calcRoute

/*
 * Exibe os steps no mapa
 */
function showSteps(directionResult) {
	// For each step, place a marker, and add the text to the marker's
	// info window. Also attach the marker to an array so we
	// can keep track of it and remove it when calculating new
	// routes.
	var myRoute = directionResult.routes[0].legs[0];

    for (var i = 0; i < myRoute.steps.length; i++) {
//      var marker = new google.maps.Marker({
//        position: myRoute.steps[i].start_point, 
//        map: map
//      });
      //attachInstructionText(marker, myRoute.steps[i].instructions);
      //markerArray[i] = marker;
    }
}//fim showSteps

/*
 * Inclui os textos dos steps
 */
function attachInstructionText(marker, text) {
	google.maps.event.addListener(marker, 'click', function() {
    // Open an info window when the marker is clicked on,
    // containing the text of the step.
    stepDisplay.setContent(text);
    stepDisplay.open(map, marker);
  });
}//fim attachInstructionText


/*
 * Centraliza o mapa no Brasil
 */
function centralizaBrasil(){

  	map.setZoom(4);
	map.setCenter(new google.maps.LatLng(-14.689881, -52.373047)); //Centraliza e aplica o zoom
    	
}


/*
 * Função que cria o HTML exibido ao clicar em algum ponto 
 */
function gerarHtmlPonto(marker) {
	return marker.attr("info");
}// fim gerarHtmlPonto



/*
 * Função que marca os pontos no mapa 
 */
function marcarPontos(filtros) {
 
	jQuery.ajax({
  		type: "POST",
  		url: filtros,
  		async: false,
  		success: function(data) {

  		  
      jQuery(data).find("marker").each(function() {
        var marker = jQuery(this);
        var latlng = new google.maps.LatLng(parseFloat(marker.attr("lat")),
                                    		parseFloat(marker.attr("lng")));
                                    		
        var html = marker.attr("info");
        
        html = replaceAll(html,"EE","&");
		
		var icon = new google.maps.MarkerImage(marker.attr("icon"));
		
	    var ponto = new google.maps.Marker({
	        position: latlng,
	        map: map,
	        icon: icon,
	        shape: shape,
	        zIndex: 3
	    });
	    
	    if(!pontoMarcadores[marker.attr("icon")]) {
	    	pontoMarcadores[marker.attr("icon")] = new Array();
	    }
	    	    
	    pontoMarcadores[marker.attr("icon")].push(ponto);
	    
	    arrMarkers.push(ponto);
	    
	    google.maps.event.addListener(ponto, "click", function() {if (infowindow) infowindow.close(); infowindow = new google.maps.InfoWindow({content: html}); infowindow.open(map, ponto); });

     });
     	if(jQuery("#agrupado_pontos_sim").attr("checked") == true){
     		//var mcOptions = {gridSize: 50, maxZoom: 15};
			markerClusterer = new MarkerClusterer(map,arrMarkers);
		}else{
			if(markerClusterer) {
				markerClusterer.clearMarkers();
	        }
		}
     	
  	}
	});

}// fim marcarPontos

/*
 * Função que monta o poligono dos estados
 */
function montarPoligonoEstado(estuf) {

	var url = "/maps/maps.php?requisicao=carregaEstado&estuf=" + estuf;

	jQuery.ajax({
		type: "POST",
		url: url,
		async: false,
		dataType:'JSON',
		success: function(response){

			response = jQuery.parseJSON(response);
	
			jQuery.each(response,function(index,item){
			    var centro = item.centro;
				var arrCoord = centro.split(" ");
				var GeoJSON = jQuery.parseJSON(item.poli);
				var coords = GeoJSON.coordinates;
		         var paths = [];
		            for (var i = 0; i < coords.length; i++) {
		                for (var j = 0; j < coords[i].length; j++) {
		                    var path = [];
		                    for (var k = 0; k < coords[i][j].length; k++) {
		                        var ll = new google.maps.LatLng(coords[i][j][k][1],coords[i][j][k][0]);
		                        path.push(ll);
		                    }
		                    paths.push(path);
		                }
		            } 
		
			
			nomePoli[item.estuf] = new google.maps.Polygon({
		      paths: paths, 
		      strokeColor: '#444',
		      strokeOpacity: 1,
		      strokeWeight: 1,
		      fillOpacity: 0,
		      zIndex: 0
		    });
		    
			nomePoli[item.estuf].setMap(map);
			
			
			centroPoliEstado[item.estuf] = arrCoord;
				
			//google.maps.event.addListener(nomePoli[item.estuf], 'click', function(event){alert(item.estdescricao)});
			
			});
			
			
		}
	});

	
}// fim montarPoligonoEstado

function carregarTipoMunicipio(tpmid) {
	
	var url = "/maps/maps.php?requisicao=carregaTipoMunicipio&tpmid=" + tpmid;
	var resp = '';
	jQuery.ajax({
			type: "POST",
			url: url,
			async: false,
			success: function(response){
				resp = response;
			}
		});
		
	montarPoligonoTipoMunicipio(resp);

}

function carregarTipoMunicipioPorGrupoMunicipio(gtmid) {
		var url = "/maps/maps.php?requisicao=carregaTipoMunicipio&gtmid=" + gtmid;
		var resp = '';
		jQuery.ajax({
				type: "POST",
				url: url,
				async: false,
				success: function(response){
					resp = response;
				}
			});
			montarPoligonoTipoMunicipio(resp);

}

function montarPoligonoTipoMunicipio(response) {
	response = jQuery.parseJSON(response);

	jQuery.each(response,function(index,item){
	    var centro = item.centro;
		var corpolyd = corDestaquePoliMunicipio;
		var corpoly = corPoliMunicipio;
	    if(centro){
	    	var arrCoord = centro.split(" ");
	    }
		if(item.poli){
			var GeoJSON = jQuery.parseJSON(item.poli);
			var coords = GeoJSON.coordinates;
	         var paths = [];
	            for (var i = 0; i < coords.length; i++) {
	                for (var j = 0; j < coords[i].length; j++) {
	                    var path = [];
	                    for (var k = 0; k < coords[i][j].length; k++) {
	                        var ll = new google.maps.LatLng(coords[i][j][k][1],coords[i][j][k][0]);
	                        path.push(ll);
	                    }
	                    paths.push(path);
	                }
	            }
	        corPoli[item.tpmid] = corpoly;
			
			nomePoli[item.tpmid] = new google.maps.Polygon({
		      paths: paths, 
		      strokeColor: '#000000',
		      strokeOpacity: 0.6,
		      strokeWeight: 0.5,
		      fillColor: item.cor,
		      fillOpacity: 0.7,
		      zIndex: 2
		    });
		    
			nomePoli[item.tpmid].setMap(map);
			if(arrCoord){
				centroPoliTipoMunicipio[item.tpmid] = arrCoord;
			}
			google.maps.event.addListener(nomePoli[item.tpmid], 'rightclick', function(event){showContextMenu(event.latLng);});
			google.maps.event.addListener(nomePoli[item.tpmid], 'mouseover', function(event){f_mouseover(item.tpmid,corpolyd,item.tpmdsc);});
			google.maps.event.addListener(nomePoli[item.tpmid], 'mouseout', function(event){f_mouseout(item.tpmid,item.cor);});
			google.maps.event.addListener(nomePoli[item.tpmid], 'click', function(event){infGrupoMunicipio(item.gtmid,item.tpmid,event.latLng,item.mapid);});
		}
	});
	
}// fim montarPoligonoEstado

/*
 * Função que monta o poligono dos municípios 
 */
function montarPoligonoMunicipio(response){
	response = jQuery.parseJSON(response);
	jQuery.each(response,function(index,item){
		var corpolyd = corDestaquePoliMunicipio;
		var corpoly = corPoliMunicipio;
		var GeoJSON = jQuery.parseJSON(item.poli);
		var coords = GeoJSON.coordinates;
         var paths = [];
            for (var i = 0; i < coords.length; i++) {
                for (var j = 0; j < coords[i].length; j++) {
                    var path = [];
                    for (var k = 0; k < coords[i][j].length; k++) {
                        var ll = new google.maps.LatLng(coords[i][j][k][1],coords[i][j][k][0]);
                        path.push(ll);
                    }
                    paths.push(path);
                }
            } 

	corPoli[item.muncod] = corpoly;
	
	if(!estadoMunicipio[item.estuf]){
		estadoMunicipio[item.estuf] = new Array();
	}
	
	estadoMunicipio[item.estuf][item.muncod] = true;
			
	if(nomePoli[item.muncod]){
		nomePoli[item.muncod].setMap(null);
	}
	
	nomePoli[item.muncod] = new google.maps.Polygon({
      paths: paths, 
      strokeColor: '#000000',
      strokeOpacity: 0.4,
      strokeWeight: 0.3,
      fillColor: item.cor,
      fillOpacity: 0.7,
      zIndex: 2
    });
    
	nomePoli[item.muncod].setMap(map);
			
	google.maps.event.addListener(nomePoli[item.muncod], 'rightclick', function(event){showContextMenu(event.latLng);});	
	google.maps.event.addListener(nomePoli[item.muncod], 'mouseover', function(event){f_mouseover(item.muncod,corpolyd,item.mundescricao+'/'+item.estuf);});
	google.maps.event.addListener(nomePoli[item.muncod], 'mouseout', function(event){f_mouseout(item.muncod,item.cor);});
	google.maps.event.addListener(nomePoli[item.muncod], 'click', function(event){infmunicipio(item.muncod); if(typeof(html_municipio) != "undefined") {infowindow.setContent(replaceAll(html_municipio,"{muncod}",item.muncod));infowindow.setPosition(event.latLng);infowindow.open(map);}abreAcordion(1);});});
	
	infowindow = new google.maps.InfoWindow();
	
	
}

function f_mouseover(obj,cor,mundescricao){
	if (nomePoli[obj]){
		nomePoli[obj].setOptions( {fillColor: cor} );
		document.getElementById('nome_mun').innerHTML = mundescricao;
	}
}

function f_mouseout(obj,cor){
	if (nomePoli[obj]){
		f_mudacor(obj,corPoli[obj]); 
		document.getElementById('nome_mun').innerHTML = '&nbsp;';
	}
}

function f_mudacor(obj,cor){
	corPoli[obj] = cor;
	nomePoli[obj].setOptions( {fillColor: cor} );
}

function f_mudacores(arr, obj){
	var cor;
	var codes = arr.split(",");
	for(var i=0;i<codes.length;i++) {
		if(nomePoli[codes[i]]) {
			if(obj.checked) {nomePoli[codes[i]].setOptions( {fillColor: '#00ffff'} );} else {nomePoli[codes[i]].setOptions( {fillColor: corPoli[codes[i]]} );}
		}	
	}
}

function abreAcordion(id) {
	jQuery("#accordion").accordion( "activate" , id );
}

function carregarEstado(estuf) {
	if(nomePoli[estuf]){ //se já existir, apenas exibe
		nomePoli[estuf].setMap(map);
	}else{ //se não existir, cria
		montarPoligonoEstado(estuf);
	}
}

function carregarMunicipio(muncod) {
	
	if(nomePoli[muncod]){ //se já existir, apenas exibe
		nomePoli[muncod].setMap(map);
	}else{ //se não existir, cria
		var url = "/maps/maps.php?requisicao=carregaMunicipio&muncod=" + muncod;
		var resp = '';
		jQuery.ajax({
				type: "POST",
				url: url,
				async: false,
				success: function(response){
					resp = response;
				}
			});
			montarPoligonoMunicipio(resp);
	}

}


function clicaEstado(estuf, obj) {
	divCarregando();
	if(obj.style.backgroundColor=='') {
		obj.style.backgroundColor='#f6ead9';
		carregarEstado(estuf);
		centraliza(6,centroPoliEstado[estuf]);
		carregaMunicipiosPorUF(estuf);
		atribuiFiltroEstado();
		divCarregado();
	} else {
		obj.style.backgroundColor='';
		nomePoli[estuf].setMap(null);
		escondeMunicipioPorUF(estuf);
		divCarregado();
		retiraFiltroEstado();
	}
	callBack();
}

function atribuiFiltroEstado()
{
	var arrUF = new Array();
	jQuery('#estuf').find('option').remove();
	jQuery.each(jQuery("[id=linha_uf]").children(),function(index,item){
	    if(jQuery(this).attr("style"))
		{
		    var id = jQuery(this).children().attr("id");
		    arrUF[i] = id;
		    jQuery('#estuf').append(new Option(jQuery(this).children().attr("title"),id.replace("estuf",""), true, true));
		}
	});
}

function retiraFiltroEstado()
{
	var arrUF = new Array();
	jQuery('#estuf').find('option').remove();
	jQuery.each(jQuery("[id=linha_uf]").children(),function(index,item){
	    if(jQuery(this).attr("style"))
		{
		    var id = jQuery(this).children().attr("id");
		    arrUF[i] = id;
		    jQuery('#estuf').append(new Option(jQuery(this).children().attr("title"),id.replace("estuf",""), true, true));
		}
	});
	if(arrUF.length == 0){
		jQuery('#estuf').append( new Option ('Duplo clique para selecioanr da lista','', true, true));
	}
}

function clicaTodos(estuf, obj) {
	if(obj.style.backgroundColor=='') {
		obj.style.backgroundColor='#f6ead9';
		carregarEstado(estuf);
		carregaMunicipiosPorUF(estuf);
		
	} else {
		obj.style.backgroundColor='';
		nomePoli[estuf].setMap(null);
		escondeMunicipioPorUF(estuf);
	}
	callBack();
}

function carregaMunicipiosPorUF(estuf)
{
	
	if(estado[estuf]){
		jQuery.each(estadoMunicipio[estuf], function(muncod,value) { 
		  	if(nomePoli[muncod]){
				nomePoli[muncod].setMap(map);
			}			
		});
	}else{
		var url = "/maps/maps.php?requisicao=carregaMunicipio&estuf=" + estuf;
		var resp = '';
		jQuery.ajax({
				type: "POST",
				url: url,
				async: false,
				success: function(response){
					// console.log(response);
					resp = response;
				}
			});
			montarPoligonoMunicipio(resp);
	}
	estado[estuf]=true;
}

function escondeMunicipioPorUF(estuf)
{
	if(estadoMunicipio[estuf]){
		jQuery.each(estadoMunicipio[estuf], function(muncod,value) { 
		  	if(nomePoli[muncod]){
				nomePoli[muncod].setMap(null);
			}
		});
	}
}

/*
 * Função que controla as exibição dos pontos marcados no Mapa
 */
function controlaPontoMarcadores(icone, exibe)
{
	if(pontoMarcadores[icone]){
		jQuery.each(pontoMarcadores[icone], function(cont,value) { 
		  	if(pontoMarcadores[icone][cont]){
		  		if(exibe) {
					pontoMarcadores[icone][cont].setMap(map);
		  		} else {
					pontoMarcadores[icone][cont].setMap(null);		  		
		  		}

			}
		});
	}
}

function BuscaMunicipioEnter2(e)
{
    if (e.keyCode == 13)
    {
        BuscaMunicipio2();
    }
}

function BuscaMunicipio2(){
	var mun = document.getElementById('busca_municipio');
	var camada = document.getElementById('camada').value;
	if(!mun.value){
		alert('Digite o texto para a busca.');
		return false;
	}

	if(mun.value){
		document.getElementById('resultado_pesquisa').innerHTML = "<center>Carregando...</center>";
		jQuery.ajax({
			type: "POST",
			url: '/painel/_funcoes_mapa_painel_controle.php?redefederal=1',
			data: 'BuscaMunicipioAjax=' + mun.value + '&camada='+camada,
			async: false,
			success: function(response){
				document.getElementById('resultado_pesquisa').innerHTML = response;
			}
		});

	}
}
function localizaMapa2(muncod,latitude,longitude){
			
	var myLatLng = new Array();
	myLatLng[0] = latitude;
	myLatLng[1] = longitude;
	centraliza(parseInt(8), myLatLng);
	infmunicipio(muncod);
	if(!nomePoli[muncod]){
		carregarMunicipio(muncod);
	}
	abreAcordion(1);
	nomePoli[muncod].setOptions( {fillColor: '#F0F'} );
}

function localizaTipoMunicipio(tpmid,latitude,longitude){
			
	var myLatLng = new Array();
	myLatLng[0] = latitude;
	myLatLng[1] = longitude;
	centraliza(parseInt(8), myLatLng);
	nomePoli[tpmid].setOptions( {fillColor: '#F0F'} );
}

function infmunicipio(muncod){
	jQuery.ajax({
		type: "POST",
		url: "/maps/maps.php?requisicao=infoMunicipio&muncod=" + muncod,
		async: false,
		success: function(response){
			mostrainfmunicipio(response);
		}
	});
}

function exibeRegisaoSaude(tpmid){
	var gtmid = 10;
	var mapid = 32;
	var html_tipo_municipio = "<div style=\"padding:5px\" ><iframe src=\""+base_url+"&requisicao=montaBalaoTipoMunicipio&gtmid=" + gtmid + "&tpmid=" + tpmid + "&mapid=" + mapid + "\" frameborder=0 scrolling=\"auto\" height=\"300px\" width=\"480px\" ></iframe></div>";
	infowindow.setContent(html_tipo_municipio);
}

function infGrupoMunicipio(gtmid,tpmid,LatLng,mapid){
	infowindow = new google.maps.InfoWindow();
	var html_tipo_municipio = "<div style=\"padding:5px\" ><iframe src=\""+base_url+"&requisicao=montaBalaoTipoMunicipio&gtmid=" + gtmid + "&tpmid=" + tpmid + "&mapid=" + mapid + "\" frameborder=0 scrolling=\"auto\" height=\"300px\" width=\"480px\" ></iframe></div>";
	infowindow.setContent(html_tipo_municipio);
	infowindow.setPosition(LatLng);
	infowindow.open(map);
}

function mostrainfmunicipio(dados) {
	document.getElementById('inf_mun').innerHTML = dados;
}

// Deletes all markers in the array by removing references to them
function deleteOverlays(markersArray_) {
  if(markersArray_) {
	  if (markersArray_.length > 0) {
	    for (i=0;i<markersArray_.length;i++) {
	      markersArray_[i].setMap(null);
	    }
	    markersArray_.length = 0;
	  }
  }
}

// Removes the overlays from the map, but keeps them in the array
function clearOverlays(markersArray_) {
  if(markersArray_.length>0) {
  	for(i=0;i<markersArray_.length;i++) {
      markersArray_[i].setMap(null);
  	}
  }
}

// Shows any overlays currently in the array
function showOverlays(markersArray_) {
  if(markersArray_) {
	  if(markersArray_.length>0) {
	  	for(i=0;i<markersArray_.length;i++) {
	      markersArray_[i].setMap(map);
	  	}
	  }
  }
}

/* Função para subustituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}

function preencherRelatorio(filtros) {

	jQuery.ajax({
 		type: "POST",
   		url: filtros,
   		async: false,
   		success: function(response){
   			abreAcordion(2);
   			extrairScript(response);
   			document.getElementById('colunainfo').innerHTML = response;
   		}
 		});
 		
}

function retornarCor(corInicio, corFim, escala, posicao ){
	if(!posicao)
	{
		return corInicio;
	}
	var cores = 0;
	for(i=0;i < escala; i++){
		cores = jQuery.xcolor.gradientlevel(corInicio, corFim, i, escala);
		if(i+1 == posicao ){
			return cores;
      }
    }
}

function removeTodosPontos()
{
	if(pontoMarcadores['/imagens/tachinha_b.png']) {
		deleteOverlays(pontoMarcadores['/imagens/tachinha_b.png']);
	}
}




function carregarRegiao(regcod) {

	if(nomePoli[regcod]){ //se já existir, apenas exibe
		nomePoli[regcod].setMap(map);
	}else{ //se não existir, cria
		var url = "/maps/maps.php?requisicao=carregaRegiao&regcod=" + regcod;
		var resp = '';
		jQuery.ajax({
				type: "POST",
				url: url,
				async: false,
				success: function(response){
					resp = response;
				}
			});
			montarPoligonoRegiao(resp);
	}

}

/*
 * Função que monta o poligono dos região 
 */
function montarPoligonoRegiao(response){
	response = jQuery.parseJSON(response);
	jQuery.each(response,function(index,item){
		var corpolyd = corDestaquePoliMunicipio;
		var corpoly = corPoliMunicipio;
		var GeoJSON = jQuery.parseJSON(item.poli);
		var coords = GeoJSON.coordinates;
         var paths = [];
            for (var i = 0; i < coords.length; i++) {
                for (var j = 0; j < coords[i].length; j++) {
                    var path = [];
                    for (var k = 0; k < coords[i][j].length; k++) {
                        var ll = new google.maps.LatLng(coords[i][j][k][1],coords[i][j][k][0]);
                        path.push(ll);
                    }
                    paths.push(path);
                }
            } 

	corPoli[item.regcod] = corpoly;
	
	if(nomePoli[item.regcod]){
		nomePoli[item.regcod].setMap(null);
	}
	
	nomePoli[item.regcod] = new google.maps.Polygon({
      paths: paths, 
      strokeColor: '#000000',
      strokeOpacity: 1,
      strokeWeight: 1,
      fillColor: item.cor,
      fillOpacity: 0,
      zIndex: 0
    });
    
	nomePoli[item.regcod].setMap(map);
			
	//google.maps.event.addListener(nomePoli[item.muncod], 'rightclick', function(event){showContextMenu(event.latLng);});	
	//google.maps.event.addListener(nomePoli[item.muncod], 'mouseover', function(event){f_mouseover(item.muncod,corpolyd,item.mundescricao+'/'+item.estuf);});
	//google.maps.event.addListener(nomePoli[item.muncod], 'mouseout', function(event){f_mouseout(item.muncod,item.cor);});
	//google.maps.event.addListener(nomePoli[item.muncod], 'click', function(event){infmunicipio(item.muncod); if(typeof(html_municipio) != "undefined") {infowindow.setContent(replaceAll(html_municipio,"{muncod}",item.muncod));infowindow.setPosition(event.latLng);infowindow.open(map);}abreAcordion(1);});
	});
	
	//infowindow = new google.maps.InfoWindow();
	
	
}

function carregarMicroRegiaoPorUF(estuf) {

	if(nomePoli[estuf]){ //se já existir, apenas exibe
		nomePoli[estuf].setMap(map);
	}else{ //se não existir, cria
		var url = "/maps/maps.php?requisicao=carregaMicroRegiao&estuf=" + estuf;
		var resp = '';
		jQuery.ajax({
				type: "POST",
				url: url,
				async: false,
				success: function(response){
					resp = response;
				}
			});
			montarPoligonoMicroRegiao(resp);
	}

}

/*
 * Função que monta o poligono dos região 
 */
function montarPoligonoMicroRegiao(response){
	response = jQuery.parseJSON(response);
	jQuery.each(response,function(index,item){
		var corpolyd = corDestaquePoliMunicipio;
		var corpoly = corPoliMunicipio;
		var GeoJSON = jQuery.parseJSON(item.poli);
		var coords = GeoJSON.coordinates;
         var paths = [];
            for (var i = 0; i < coords.length; i++) {
                for (var j = 0; j < coords[i].length; j++) {
                    var path = [];
                    for (var k = 0; k < coords[i][j].length; k++) {
                        var ll = new google.maps.LatLng(coords[i][j][k][1],coords[i][j][k][0]);
                        path.push(ll);
                    }
                    paths.push(path);
                }
            } 

	corPoli[item.miccod] = corpoly;
	
	if(nomePoli[item.miccod]){
		nomePoli[item.miccod].setMap(null);
	}
	
	nomePoli[item.miccod] = new google.maps.Polygon({
      paths: paths, 
      strokeColor: '#000000',
      strokeOpacity: 1,
      strokeWeight: 1,
      fillColor: item.cor,
      fillOpacity: 0,
      zIndex: 0
    });
    
	nomePoli[item.miccod].setMap(map);
			
	//google.maps.event.addListener(nomePoli[item.muncod], 'rightclick', function(event){showContextMenu(event.latLng);});	
	//google.maps.event.addListener(nomePoli[item.muncod], 'mouseover', function(event){f_mouseover(item.muncod,corpolyd,item.mundescricao+'/'+item.estuf);});
	//google.maps.event.addListener(nomePoli[item.muncod], 'mouseout', function(event){f_mouseout(item.muncod,item.cor);});
	//google.maps.event.addListener(nomePoli[item.muncod], 'click', function(event){infmunicipio(item.muncod); if(typeof(html_municipio) != "undefined") {infowindow.setContent(replaceAll(html_municipio,"{muncod}",item.muncod));infowindow.setPosition(event.latLng);infowindow.open(map);}abreAcordion(1);});
	});
	
	//infowindow = new google.maps.InfoWindow();
	
	
}


function carregarMesoRegiaoPorUF(estuf) {

	if(nomePoli[estuf]){ //se já existir, apenas exibe
		nomePoli[estuf].setMap(map);
	}else{ //se não existir, cria
		var url = "/maps/maps.php?requisicao=carregaMesoRegiao&estuf=" + estuf;
		var resp = '';
		jQuery.ajax({
				type: "POST",
				url: url,
				async: false,
				success: function(response){
					resp = response;
				}
			});
			montarPoligonoMesoRegiao(resp);
	}

}

/*
 * Função que monta o poligono dos região 
 */
function montarPoligonoMesoRegiao(response){
	response = jQuery.parseJSON(response);
	jQuery.each(response,function(index,item){
		var corpolyd = corDestaquePoliMunicipio;
		var corpoly = corPoliMunicipio;
		var GeoJSON = jQuery.parseJSON(item.poli);
		var coords = GeoJSON.coordinates;
         var paths = [];
            for (var i = 0; i < coords.length; i++) {
                for (var j = 0; j < coords[i].length; j++) {
                    var path = [];
                    for (var k = 0; k < coords[i][j].length; k++) {
                        var ll = new google.maps.LatLng(coords[i][j][k][1],coords[i][j][k][0]);
                        path.push(ll);
                    }
                    paths.push(path);
                }
            } 

	corPoli[item.mescod] = corpoly;
	
	if(nomePoli[item.mescod]){
		nomePoli[item.mescod].setMap(null);
	}
	
	nomePoli[item.mescod] = new google.maps.Polygon({
      paths: paths, 
      strokeColor: '#000000',
      strokeOpacity: 1,
      strokeWeight: 1,
      fillColor: item.cor,
      fillOpacity: 0,
      zIndex: 0
    });
    
	nomePoli[item.mescod].setMap(map);
			
	//google.maps.event.addListener(nomePoli[item.muncod], 'rightclick', function(event){showContextMenu(event.latLng);});	
	//google.maps.event.addListener(nomePoli[item.muncod], 'mouseover', function(event){f_mouseover(item.muncod,corpolyd,item.mundescricao+'/'+item.estuf);});
	//google.maps.event.addListener(nomePoli[item.muncod], 'mouseout', function(event){f_mouseout(item.muncod,item.cor);});
	//google.maps.event.addListener(nomePoli[item.muncod], 'click', function(event){infmunicipio(item.muncod); if(typeof(html_municipio) != "undefined") {infowindow.setContent(replaceAll(html_municipio,"{muncod}",item.muncod));infowindow.setPosition(event.latLng);infowindow.open(map);}abreAcordion(1);});
	});
	
	//infowindow = new google.maps.InfoWindow();
	
	
}
//alteravalorRange('fim','0','9',this)
function alteravalorRange(tipo,chave,parametro,obj)
{
	jQuery("#"+ obj.id).attr("size",obj.value.length + 1);
	valor = replaceAll(obj.value,"%","");
	valor = replaceAll(valor,".","");
	valor = replaceAll(valor,",",".");
	
	if(tipo == "inicio"){
		arrIncio[parametro][chave] = valor*1;
	}else{
		arrFim[parametro][chave] = valor*1;
	}
}

function selecionaTipoFiltro(obj)
{
	var nome = obj.name;
	nome = nome.replace("cmb_filtro","cmb_filtro_a");
	nome = nome.replace("][","_");
	nome = nome.replace("]","_");
	nome = nome.replace("[","_");
	var valor = obj.value;
	if(valor == "entre"){
		jQuery("[id='" + nome + "']").show();
	}else{
		jQuery("[id='" + nome + "']").hide();
	}
}

function fake()
{

}

function addTemaGrupo(grupo,mapid)
{
	var total = jQuery("[name^='cmb_tema[" + grupo + "]']").length;
	total = total + 1;
	var url = "painel.php?modulo=principal/mapas/mapaPadrao&acao=A&requisicao=addTemaGrupo&grupo=" + grupo + "&mapid=" + mapid + "&num=" + total;
	jQuery.ajax({
			type: "POST",
			url: url,
			async: false,
			success: function(response){
				jQuery("#grupo_avancado_" + grupo).append(response);
			}
		});
}

function addNovoGrupo(mapid)
{
	var total = jQuery("[id^='grupo_avancado_']").length;
	total = total + 1;
	var url = "painel.php?modulo=principal/mapas/mapaPadrao&acao=A&requisicao=addGrupo&mapid=" + mapid + "&num=" + total;
	jQuery.ajax({
			type: "POST",
			url: url,
			async: false,
			success: function(response){
				jQuery("#grupo_filtros_avancados").append(response);
			}
		});
}

function removeTemaGrupo(grupo)
{
	jQuery("[id='grupo_avancado_" + grupo + "']").remove();
	jQuery("[name='cmb_entregrupo[" + grupo + "]']").remove();
}

function exibeRelatorioFiltrosAvancados(mapid)
{
	var arrUF = new Array();
	var i = 0;
	jQuery.each(jQuery("[id=linha_uf]").children(),function(index,item){
	    if(jQuery(this).attr("style"))
		{
		    arrUF[i] = jQuery(this).children().attr("id");
			i++;
		}
	});
	
	if(!verificaExibicaoFiltroAvancado()){
		return false;
	}
	
	jQuery("#estuf_filtros_avancados").val(arrUF);
	jQuery("#camada_filtros_avancados").val(jQuery("#camada").val());
	
	jQuery("#requisicao_filtros_avancados").val("exibeRelatorioFiltrosAvancados");
	var formulario = jQuery("#form_busca_avancada");
    var janela = window.open( 'painel.php?modulo=principal/mapas/popUpMapaPadrao&acao=A&mapid=' + mapid, 'filtroavancadoresultado', 'width=800,height=600,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
    formulario.submit();
    janela.focus();
}

function exportarAvancadosExcel(mapid)
{
	var arrUF = new Array();
	var i = 0;
	jQuery.each(jQuery("[id=linha_uf]").children(),function(index,item){
	    if(jQuery(this).attr("style"))
		{
		    arrUF[i] = jQuery(this).children().attr("id");
			i++;
		}
	});
	
	if(!verificaExibicaoFiltroAvancado()){
		return false;
	}
	
	jQuery("#estuf_filtros_avancados").val(arrUF);
	
	jQuery("#requisicao_filtros_avancados").val("exportarAvancadosExcel");
	var formulario = jQuery("#form_busca_avancada");
    var janela = window.open( 'painel.php?modulo=principal/mapas/popUpMapaPadrao&acao=A&mapid=' + mapid, 'filtroavancadoresultado', 'width=800,height=600,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
    formulario.submit();
    janela.focus();
}

function salvarFiltrosAvancados(mapid)
{
	var fustitulo = jQuery("[name='fustitulo']").val();
	var fuspublico = jQuery("[name='fuspublico']:checked").val();
	
	if(!fustitulo){
		alert('Informe o título do filtro!');
		return false;
	}
	if(!fuspublico){
		alert('O filtro será público?');
		return false;
	}
	
	if(!verificaExibicaoFiltroAvancado()){
		return false;
	}
	
	var cor = jQuery("#color_filtro_avancado").val();
	jQuery("#requisicao_filtros_avancados").val("salvarFiltrosAvancados");
	var arrDados = jQuery("#form_busca_avancada").serialize();
	jQuery.ajax({
		type: "POST",
		url: 'painel.php?modulo=principal/mapas/mapaPadrao&acao=A',
		data: "requisicao=salvarFiltrosAvancados&mapid=" + mapid + "&" + arrDados + "&fustitulo=" + fustitulo + "&fuspublico=" + fuspublico,
		async: false,
		dataType:'JSON',
		success: function(response){
			jQuery('#div_salvar_filtros_avancados').hide();
			jQuery("#lista_salvar_filtros_avancados").html(response);
		}
	});
}

function excluirFiltroAvancado(fusid,mapid)
{
	if(confirm("Deseja realmente excluir o filtro?")){
		jQuery("#requisicao_filtros_avancados").val("excluirFiltroAvancado");
		var arrDados = jQuery("#form_busca_avancada").serialize();
		jQuery.ajax({
			type: "POST",
			url: 'painel.php?modulo=principal/mapas/mapaPadrao&acao=A',
			data: "requisicao=excluirFiltroAvancado&fusid=" + fusid + "&mapid=" + mapid,
			async: false,
			dataType:'JSON',
			success: function(response){
				jQuery("#lista_salvar_filtros_avancados").html(response);
			}
		});
	}
}

function excluirFiltroNormal(fusid,mapid)
{
	if(confirm("Deseja realmente excluir o filtro?")){
		jQuery.ajax({
			type: "POST",
			url: 'painel.php?modulo=principal/mapas/mapaPadrao&acao=A',
			data: "requisicao=excluirFiltroNormal&fusid=" + fusid + "&mapid=" + mapid,
			async: false,
			dataType:'JSON',
			success: function(response){
				jQuery("#div_filtros_normais").html(response);
			}
		});
	}
}

function visualizarFiltrosAvancadosMapa(mapid)
{
	var arrUF = new Array();
	var i = 0;
	jQuery.each(jQuery("[id=linha_uf]").children(),function(index,item){
	    if(jQuery(this).attr("style"))
		{
		    arrUF[i] = jQuery(this).children().attr("id");
			i++;
		}
	});
	
	jQuery("#estuf_filtros_avancados").val(arrUF);
	
	if(!verificaExibicaoFiltroAvancado()){
		return false;
	}
	
	
	divCarregando();
	var cor = jQuery("#color_filtro_avancado").val();
	jQuery("#camada_filtros_avancados").val(jQuery("#camada").val());
	jQuery("#requisicao_filtros_avancados").val("visualizarFiltrosAvancadosMapa");	
	var arrDados = jQuery("#form_busca_avancada").serialize();
	jQuery.ajax({
		type: "POST",
		url: 'painel.php?modulo=principal/mapas/mapaPadrao&acao=A',
		data: "requisicao=visualizarFiltrosAvancadosMapa&mapid=" + mapid + "&" + arrDados,
		async: false,
		dataType:'JSON',
		success: function(response){
			response = jQuery.parseJSON(response);
			jQuery.each(response,function(index,item){
			   	if(nomePoli[item.muncod]){
			   		nomePoli[item.muncod].setMap(map);
				   	f_mudacor(item.muncod,cor);
			   	}
			});
			divCarregado();
		}
	});
}

function limparMapa()
{
	var camada = jQuery("#camada").val();
	jQuery.ajax({
		type: "POST",
		url: 'painel.php?modulo=principal/mapas/mapaPadrao&acao=A',
		data: "requisicao=limparMapaCompleto&camada="+camada,
		async: false,
		dataType:'JSON',
		success: function(response){
			response = jQuery.parseJSON(response);
			jQuery.each(response,function(index,item){
			   	if(nomePoli[item.muncod]){
			   		f_mudacor(item.muncod,corPoliMunicipio);
			   	}
			});
		}
	});
}

function carregarFiltroAvancado(fusid,mapid)
{
	jQuery.ajax({
		type: "POST",
		url: 'painel.php?modulo=principal/mapas/mapaPadrao&acao=A',
		data: "requisicao=carregarFiltroAvancado&fusid=" + fusid + "&mapid=" + mapid,
		async: false,
		success: function(response){
			jQuery("#grupo_filtros_avancados").html(response)
		}
	});
}

function carregarFiltroNormal(fusid,mapid)
{
	jQuery.ajax({
		type: "POST",
		url: 'painel.php?modulo=principal/mapas/mapaPadrao&acao=A',
		data: "requisicao=carregarFiltroNormal&fusid=" + fusid + "&mapid=" + mapid,
		async: false,
		success: function(response){
			jQuery("#filtro_normal").html(response);
			extrairScript(response);
		}
	});
}

function alteraTipoFiltroAvancado(obj)
{
	var name = obj.id;
	name = name.replace('cmb_tema','span_tema');
	jQuery.ajax({
		type: "POST",
		url: 'painel.php?modulo=principal/mapas/mapaPadrao&acao=A',
		data: "requisicao=alteraTipoFiltroAvancado&tmaid=" + obj.value + "&campo=" + name,
		async: false,
		success: function(response){
			jQuery("#" + name).html(response)
		}
	});
}

function verificaExibicaoFiltroAvancado()
{
	var erro = 0;
	jQuery("[name^='cmb_filtro_inicio[']").each(function(index,obj) {
        	if(obj.value == ""){
        		erro = 1;
        	}
	    });
	 if(erro == 0){
	 	return true;
	 }else{
	 	alert('Favor informar todos os filtros!');
	 	return false;
	 }
}
