<?

function ept_monta_agp_relatorio(){
	
	$agrupador = $_REQUEST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array("regiao",
										  "uf",
										  "tipo",
										  "campus","populacao","municipio"),
				"agrupadorDetalhamento" => array(
													array(
															"campo" => "regiao",
															"label" => "Região"
														  ),
													array(
															"campo" => "uf",
															"label" => "UF"
														  ),
													array(
															"campo" => "tipo",
															"label" => "Tipo"
														  ),
													array(
															"campo" => "campus",
															"label" => "Campus"
														  ),
													array(
															"campo" => "municipio",
															"label" => "Município"
														  ),
													array(
															"campo" => "populacao",
															"label" => "População"
														  )				  
														  
												)	  
				);
	
	foreach ( $agrupador as $val ){
		switch( $val ){
			case "regiao":
				array_push($agp['agrupador'], array(
													"campo" => "regiao",
											  		"label" => "Região")										
									   				);
			break;
			case "municipio":
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
											  		"label" => "Município")										
									   				);
			break;
			case "uf":
				array_push($agp['agrupador'], array(
													"campo" => "uf",
											  		"label" => "UF")										
									   				);
			break;
			case "tipo":
				array_push($agp['agrupador'], array(
													"campo" => "tipo",
											  		"label" => "Tipo")										
									   				);
			break;
			case "campus":
				array_push($agp['agrupador'], array(
													"campo" => "campus",
											  		"label" => "Campus")										
									   				);
			break;
			case "populacao":
				array_push($agp['agrupador'], array(
													"campo" => "populacao",
											  		"label" => "População")										
									   				);
			break;
			
		}	
	}
	
	return $agp;
	
}

if($_REQUEST['relatorio_superior']) {
	
	ini_set("memory_limit", "1024M");
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	
	if($_REQUEST['chk']) {
		if(in_array("7", $_REQUEST['chk'])) $classif[] = "1 - Câmpus Preexistentes";			
		if(in_array("8", $_REQUEST['chk']))	$classif[] = "2 - Criadas (2003/2010)";			
		if(in_array("4", $_REQUEST['chk']))	$classif[] = "3 - Previstos (2011/2012)";
		if(in_array("3", $_REQUEST['chk']))	$classif[] = "4 - Propostos (2013/2014)";
		if(in_array("6", $_REQUEST['chk']))	$classif[] = "5 - Universidades Previstas (2013/2014)";
		
		if($classif) $filtro[] = "r.rfstipo IN ('".implode("','",$classif)."') ";
	}
	
	if( $_REQUEST['buscaTextual'] ){
		$filtro[] = "UPPER(r.rfsnome) like UPPER('%".$_REQUEST['buscaTextual']."%')";
	}
	if( $_REQUEST['estuf'][0] ){
		$filtro[] = "r.estuf in ('".implode("','",$_REQUEST['estuf'])."') ";
	}
	if( $_REQUEST['muncod'][0] ){
		$filtro[] = "mun.muncod in ('".implode("','",$_REQUEST['muncod'])."') ";
	}
	
	
	
	$sql = "SELECT 
			'<a style=cursor:pointer; onclick=\"abrebalao(\''||r.rfsid||'\');\">('||mun.mundescricao || ') ' || r.rfsnome||'</a>' as campus,
			r.rfstipo as tipo,
			trim(r.estuf) as uf,
			reg.regdescricao as regiao,
			mun.munpopulacao as populacao,
			'<a onmouseover=\"f_mouseover(\''||trim(r.estuf)||mun.muncod||'\',\'#d82b40\',\''||mun.mundescricao||'/'||trim(r.estuf)||'\');\" onmouseout=\"f_mouseout(\''||trim(r.estuf)||mun.muncod||'\',\'\');\" style=cursor:pointer; onclick=\"infmunicipio(\''||mun.muncod||'\');abreAcordion(\'infoMunicipio!\');\">'||mun.mundescricao||'</a>' as municipio
			FROM academico.redefederalsuperior r 
			LEFT JOIN territorios.estado est ON est.estuf = r.estuf::character(2)
			LEFT JOIN territorios.municipio mun ON r.muncod = mun.muncod::character varying(255)
			LEFT JOIN territorios.regiao reg ON reg.regcod = est.regcod 
			".(($filtro)?"WHERE ".implode(" AND ",$filtro):"")." ".(($_REQUEST['agrupador'])?"ORDER BY ".implode(",",$_REQUEST['agrupador']):"");
	
		
	if(!$_REQUEST['agrupador']) $_REQUEST['agrupador'] = array("uf","tipo","municipio","campus","regiao");
	$agrupador = ept_monta_agp_relatorio();
	$dados = $db->carregar( $sql );

	$rel = new montaRelatorio();
	$rel->setTolizadorLinha(false);
	$rel->setMonstrarTolizadorNivel(true);
	$rel->setTotalizador(true);
	$rel->setAgrupador($agrupador, $dados); 
	$rel->setTotNivel(true);
	echo $rel->getRelatorio();
	exit;
	
	
}
	   


?>
<link href="http://code.google.com/apis/maps/documentation/javascript/examples/default.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="http://maps.google.com/maps/api/js?sensor=false"></script>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script src="http://ajax.googleapis.com/ajax/libs/jqueryui/1.8/jquery-ui.min.js"></script>
<script type="text/javascript">
$(document).ready(function() {
    $("#accordion").accordion();
  });


var map;
var draw_circle = null;  // object of google maps polygon for redrawing the circle

var shape = {
    coord: [1, 1, 1, 20, 18, 20, 18 , 1],
    type: 'poly'
};

var baseIcon3 = new google.maps.MarkerImage('/imagens/icone_capacete_4.png', new google.maps.Size(9, 14));
var baseIcon4 = new google.maps.MarkerImage('/imagens/icone_capacete_2.png', new google.maps.Size(9, 14));
var baseIcon6 = new google.maps.MarkerImage('/imagens/icone_capacete_7.png', new google.maps.Size(9, 14));
var baseIcon8 = new google.maps.MarkerImage('/imagens/icone_capacete_3.png', new google.maps.Size(9, 14));
var baseIcon9 = new google.maps.MarkerImage('/imagens/icone_capacete_1.png', new google.maps.Size(9, 14));
var markersArray_3 = new Array();
var markersArray_4 = new Array();
var markersArray_6 = new Array();
var markersArray_7 = new Array();
var markersArray_8 = new Array();

var nomePoli = new Array();
var nomePoli2 = new Array();
var nomePont = new Array();
var corPoli = new Array();

var estadoPoligono = new Array();
var estadoPoligono2 = new Array();
var centroPoligono2 = new Array();
var estadoPonto = new Array();

var infowindow;
var htmlInfo;
var xml_antigo;
var params_antigo;

function initialize() {

	var myLatLng = new google.maps.LatLng(-14.689881, -52.373047);

    var center = myLatLng;
    var myOptions = {
      zoom: 4,
      center: myLatLng,
      mapTypeId: google.maps.MapTypeId.TERRAIN
    };
    
    map = new google.maps.Map(document.getElementById("map_canvas"),
        myOptions);

}

function centraliza(zoom, coords) {
  	zoom = parseInt(zoom);
  	map.setZoom(zoom);
	map.setCenter(new google.maps.LatLng(coords[1], coords[0])); //Centraliza e aplica o zoom
}

function centralizaBrasil(){

  	map.setZoom(4);
	map.setCenter(new google.maps.LatLng(-14.689881, -52.373047)); //Centraliza e aplica o zoom
    	
}

function carregarPontos() {

	var xml_filtro="XMLmapaInstituicoesSuperior.php?1=1";
	var params;
	
	divCarregando();
	
	document.getElementById('colunainfo').innerHTML = '';
	
	if(document.getElementById('texto_busca').value != "") {
		xml_filtro += "&texto_busca="+document.getElementById('texto_busca').value;
	}
	if(document.getElementById('chk_7').checked) {
		xml_filtro += "&chk[]=7";
		params     += "&chk[]=7";
	}
	if(document.getElementById('chk_8').checked) {
		xml_filtro += "&chk[]=8";
		params     += "&chk[]=8";
	}
	if(document.getElementById('chk_4').checked) {
		xml_filtro += "&chk[]=4";
		params     += "&chk[]=4";
	}
	if(document.getElementById('chk_3').checked) {
		xml_filtro += "&chk[]=3";
		params     += "&chk[]=3";
	}
	if(document.getElementById('chk_6').checked) {
		xml_filtro += "&chk[]=6";
		params     += "&chk[]=6";
	}
	
	selectAllOptions( document.getElementById( 'estuf' ) );
	selectAllOptions( document.getElementById( 'muncod' ) );		
	selectAllOptions( document.getElementById( 'agrupador' ) );

	xml_filtro += '&'+$('#estuf').serialize();
	xml_filtro += '&'+$('#muncod').serialize();
	
	params = params+'&'+$('#estuf').serialize();
	params = params+'&'+$('#muncod').serialize();
	params = params+'&'+$('#agrupador').serialize();
		
	if(xml_antigo != xml_filtro || params_antigo != params) {
		deleteOverlays(markersArray_3);
		deleteOverlays(markersArray_4);
		deleteOverlays(markersArray_6);
		deleteOverlays(markersArray_7);
		deleteOverlays(markersArray_8);

		criarPontos(xml_filtro);
		
		$.ajax({
	 		type: "POST",
	   		url: "academico.php?modulo=principal/mapSuperior&acao=A",
	   		data: "relatorio_superior=1"+params,
	   		async: false,
	   		success: function(msg){
	   			document.getElementById('info_td').style.display = '';
	   			extrairScript(msg);
	   			document.getElementById('colunainfo').innerHTML = msg;
	   		}
	 		});
		
		xml_antigo = xml_filtro;
		params_antigo = params;

	}
	
	divCarregado();
}
 


// Deletes all markers in the array by removing references to them
function deleteOverlays(markersArray_) {
  if (markersArray_.length > 0) {
    for (i=0;i<markersArray_.length;i++) {
      markersArray_[i].setMap(null);
    }
    markersArray_.length = 0;
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
  if(markersArray_.length>0) {
  	for(i=0;i<markersArray_.length;i++) {
      markersArray_[i].setMap(map);
  	}
  }
}



function criarPontos(xml_filtro) {

	jQuery.ajax({
   		type: "POST",
   		url: xml_filtro,
   		async: false,
   		success: function(data) {
		      jQuery(data).find("marker").each(function() {
		      
		        var marker = jQuery(this);
				
		        var latlng = new google.maps.LatLng(parseFloat(marker.attr("lat")),
		                                    		parseFloat(marker.attr("lng")));
		                                    		
				var html;
				
				if(marker.attr("entid")) {
					html = "<div style=\"font-family:verdana;font-size:11px;\" >";
					html += "<b>Campus:</b> " + marker.attr("nome") + "<br />";
					html += "<b>Localização:</b> " + marker.attr("mundsc") + "/" + marker.attr("estuf") + "<br /><br />";
					html += "<span style=\"cursor:pointer;font-weight:bold\">Mais detalhes...</span>";
					html += "</div>";
					html = "<div style=\"padding:5px\" ><iframe src=\"academico.php?modulo=principal/mapaInstituicoesProfissionais&acao=A&montaBalao=1&entid=" +  marker.attr("entid") + "&tipo=" + marker.attr("balao") + "&orgid=" + marker.attr("orgid") + "\" frameborder=0 scrolling=\"auto\" height=\"380px\" width=\"480px\" ></iframe></div>";
				} else {
					html='';
					var dadoscaract = marker.attr("rfscaracteristica").split(";");
					for(var j=0;j<dadoscaract.length;j++) {
						html += dadoscaract[j]+"<br/>";
					}
				}
				
				switch(marker.attr("tipo")) {
					case '3':
						icon = baseIcon3;
						if(nomePoli[marker.attr("estuf")+marker.attr("muncod")]) {
							f_mudacor(marker.attr("estuf")+marker.attr("muncod"),"#FF3333");
						}
						break;
					case '4':
						icon = baseIcon4;
						if(nomePoli[marker.attr("estuf")+marker.attr("muncod")]) {
							f_mudacor(marker.attr("estuf")+marker.attr("muncod"),"#00CED1");
						}
						break;
					case '6':
						icon = baseIcon6;
						if(nomePoli[marker.attr("estuf")+marker.attr("muncod")]) {
							f_mudacor(marker.attr("estuf")+marker.attr("muncod"),"#6A5ACD");
						}
						break;
					case '7':
						icon = baseIcon9;
						if(nomePoli[marker.attr("estuf")+marker.attr("muncod")]) {
							f_mudacor(marker.attr("estuf")+marker.attr("muncod"),"#FCD116");
						}
						break;
					case '8':
						icon = baseIcon8;
						if(nomePoli[marker.attr("estuf")+marker.attr("muncod")]) {
							f_mudacor(marker.attr("estuf")+marker.attr("muncod"),"#B3EE3A");
						}
						break;
				}

				
			    var ponto = new google.maps.Marker({
			        position: latlng,
			        map: map,
			        icon: icon,
			        shape: shape,
			        zIndex: 1
			    });
			    
			    if(!estadoPonto[marker.attr("estuf")]) {
			    	estadoPonto[marker.attr("estuf")]='';
			    }
			    
			    nomePont[marker.attr("rfsid")] = ponto;
			    estadoPonto[marker.attr("estuf")] += marker.attr("rfsid")+",";
			    
				switch(marker.attr("tipo")) {
					case '3':
						markersArray_3.push(ponto);
						break;
					case '4':
						markersArray_4.push(ponto);
						break;
					case '6':
						markersArray_6.push(ponto);
						break;
					case '7':
						markersArray_7.push(ponto);
						break;
					case '8':
						markersArray_8.push(ponto);
						break;
				}
			    
			    google.maps.event.addListener(ponto, "click", function() {
			      if (infowindow) infowindow.close();
			      infowindow = new google.maps.InfoWindow({content: html});
			      infowindow.open(map, ponto);
			    });
				

		     });
   		}
 		});

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


function mostrar_painel(painel) {
	if(document.getElementById(painel).style.display == "none") {
		document.getElementById("img_"+painel).src="../imagens/menos.gif";
		document.getElementById(painel).style.display = "";
	} else {
		document.getElementById("img_"+painel).src="../imagens/mais.gif";
		document.getElementById(painel).style.display = "none";
	}
}

function restaurarItens() {
	removeAllOptions(document.getElementById('agrupador'));
    addOption(document.getElementById('agrupador'),"UF","uf",false);
    addOption(document.getElementById('agrupador'),"Tipo","tipo",false);
    addOption(document.getElementById('agrupador'),"Município","municipio",false);
    addOption(document.getElementById('agrupador'),"Campus","campus",false);
    addOption(document.getElementById('agrupador'),"Região","regiao",false);
}

function carregarEstado(estuf, obj) {

	divCarregando();
	
	if(obj.style.backgroundColor=='') {
		obj.style.backgroundColor='#f6ead9';
		
		if(estadoPoligono[estuf] || estadoPonto[estuf]) {
		
			var poligon2 = estadoPoligono2[estuf].split(",");
			for(var i=0;i<(poligon2.length-1);i++) {
				nomePoli2[poligon2[i]].setMap(map);
			}
			var poligon = estadoPoligono[estuf].split(",");
			for(var i=0;i<(poligon.length-1);i++) {
				nomePoli[poligon[i]].setMap(map);
			}
			var point = estadoPonto[estuf].split(",");
			for(var i=0;i<(point.length-1);i++) {
				nomePont[point[i]].setMap(map);
			}
			
			
			var xml_filtro="XMLmapaInstituicoesSuperior.php?estuf[]="+estuf;
			var params="&estuf[]="+estuf;
			
			if(document.getElementById('chk_7').checked) {
				xml_filtro += "&chk[]=7";
				params     += "&chk[]=7";
			}
			if(document.getElementById('chk_8').checked) {
				xml_filtro += "&chk[]=8";
				params     += "&chk[]=8";
			}
			if(document.getElementById('chk_4').checked) {
				xml_filtro += "&chk[]=4";
				params     += "&chk[]=4";
			}
			if(document.getElementById('chk_3').checked) {
				xml_filtro += "&chk[]=3";
				params     += "&chk[]=3";
			}
			if(document.getElementById('chk_6').checked) {
				xml_filtro += "&chk[]=6";
				params     += "&chk[]=6";
			}
			
			params = params+'&'+$('#agrupador').serialize();
				
			if(xml_antigo != xml_filtro || params_antigo != params) {
		
				criarPontos(xml_filtro);
				
				$.ajax({
			 		type: "POST",
			   		url: "academico.php?modulo=principal/mapSuperior&acao=A",
			   		data: "relatorio_superior=1"+params,
			   		async: false,
			   		success: function(msg){
			   			document.getElementById('info_td').style.display = '';
			   			extrairScript(msg);
			   			document.getElementById('colunainfo').innerHTML = msg;
			   		}
			 		});
				
				xml_antigo = xml_filtro;
				params_antigo = params;
		
			}
			
			var arrCoords = centroPoligono2[estuf].split(",");
			var arr = new Array(arrCoords[0],arrCoords[1]);
			centraliza(arrCoords[2],arr);
			
		} else {

			carregauf2(estuf);		
			carregauf(estuf);

			
			var xml_filtro="XMLmapaInstituicoesSuperior.php?estuf[]="+estuf;
			var params="&estuf[]="+estuf;
			
			if(document.getElementById('chk_7').checked) {
				xml_filtro += "&chk[]=7";
				params     += "&chk[]=7";
			}
			if(document.getElementById('chk_8').checked) {
				xml_filtro += "&chk[]=8";
				params     += "&chk[]=8";
			}
			if(document.getElementById('chk_4').checked) {
				xml_filtro += "&chk[]=4";
				params     += "&chk[]=4";
			}
			if(document.getElementById('chk_3').checked) {
				xml_filtro += "&chk[]=3";
				params     += "&chk[]=3";
			}
			if(document.getElementById('chk_6').checked) {
				xml_filtro += "&chk[]=6";
				params     += "&chk[]=6";
			}
			
			params = params+'&'+$('#agrupador').serialize();
				
			if(xml_antigo != xml_filtro || params_antigo != params) {
		
				criarPontos(xml_filtro);
				
				$.ajax({
			 		type: "POST",
			   		url: "academico.php?modulo=principal/mapSuperior&acao=A",
			   		data: "relatorio_superior=1"+params,
			   		async: false,
			   		success: function(msg){
			   			document.getElementById('info_td').style.display = '';
			   			extrairScript(msg);
			   			document.getElementById('colunainfo').innerHTML = msg;
			   		}
			 		});
				
				xml_antigo = xml_filtro;
				params_antigo = params;
		
			}
		
		}
		
			
	} else {
		
		var poligon2 = estadoPoligono2[estuf].split(",");
		for(var i=0;i<(poligon2.length-1);i++) {
			nomePoli2[poligon2[i]].setMap(null);
		}
		var poligon = estadoPoligono[estuf].split(",");
		for(var i=0;i<(poligon.length-1);i++) {
			nomePoli[poligon[i]].setMap(null);
		}
		var point = estadoPonto[estuf].split(",");
		for(var i=0;i<(point.length-1);i++) {
			nomePont[point[i]].setMap(null);
		}
		obj.style.backgroundColor='';
		
	}
	


	
	divCarregado();
}

/////////////////////////////////////////////


function infmunicipio(muncod){
	$.ajax({
		type: "POST",
		url: "inf_municipio.php",
		data: "muncod="+muncod,
		async: false,
		success: function(response){
			mostrainfmunicipio(response);
		}
	});
}

function mostrainfmunicipio(dados) {
	document.getElementById('inf_mun').innerHTML = dados;
}


function carregauf(estuf){

	$.ajax({
		type: "POST",
		url: "carrega_poligonos.php",
		data: "uf="+estuf,
		async: false,
		dataType:'JSON',
		success: function(response){
			montarPoligonos(response);
		}
	});
}

function carregauf2(estuf){

	$.ajax({
		type: "POST",
		url: "carrega_poligonos.php",
		data: "uf2="+estuf,
		async: false,
		dataType:'JSON',
		success: function(response){
			montarPoligonos2(response);
		}
	});
}

function montarPoligonos2(response){
	response = jQuery.parseJSON(response);
	$.each(response,function(index,item){
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

	
	nomePoli2[item.estuf] = new google.maps.Polygon({
      paths: paths, 
      strokeColor: '#000',
      strokeOpacity: 1,
      strokeWeight: 1.5,
      fillOpacity: 0,
      zIndex: 0
    });
	nomePoli2[item.estuf].setMap(map);
	if(!estadoPoligono2[item.estuf]) {
		estadoPoligono2[item.estuf]='';
	}
	estadoPoligono2[item.estuf] = estadoPoligono2[item.estuf]+item.estuf+",";
	centroPoligono2[item.estuf] = arrCoord[0]+","+arrCoord[1]+",6";
	centraliza(6,arrCoord);
	});
	
}


function montarPoligonos(response){
	response = jQuery.parseJSON(response);
	$.each(response,function(index,item){
		var corpolyd = "#d82b40";
		var corpoly = "#f6ead9";
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

	corPoli[item.estuf+item.muncod] = corpoly;
	
	nomePoli[item.estuf+item.muncod] = new google.maps.Polygon({
      paths: paths, 
      strokeColor: '#000000',
      strokeOpacity: 0.6,
      strokeWeight: 0.5,
      fillColor: item.cor,
      fillOpacity: 0.5,
      zIndex: 0
    });
	nomePoli[item.estuf+item.muncod].setMap(map);
	if(!estadoPoligono[item.estuf]) {
		estadoPoligono[item.estuf]='';
	}
	estadoPoligono[item.estuf] = estadoPoligono[item.estuf]+item.estuf+item.muncod+",";
	
	 google.maps.event.addListener(nomePoli[item.estuf+item.muncod], 'mouseover', function(event){f_mouseover(item.estuf+item.muncod,corpolyd,item.mundescricao+'/'+item.estuf);});
	 google.maps.event.addListener(nomePoli[item.estuf+item.muncod], 'mouseout', function(event){f_mouseout(item.estuf+item.muncod,item.cor);});
	 google.maps.event.addListener(nomePoli[item.estuf+item.muncod], 'click', function(event){infmunicipio(item.muncod);abreAcordion('infoMunicipio!');});
	});

	

	
	
}

 function f_mouseover(obj,cor,mundescricao)
  {
	nomePoli[obj].setOptions( {fillColor: cor} );
	document.getElementById('nome_mun').innerHTML = mundescricao;
  }

 function f_mouseout(obj,cor)
  {
	f_mudacor(obj,corPoli[obj]); 
	document.getElementById('nome_mun').innerHTML = '&nbsp;';
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

function abreAcordion(id)
{
	$("#accordion").accordion( "activate" , 1 );
}


</script>

<style>

/*
 * jQuery UI Accordion 1.8.14
 *
 * Copyright 2011, AUTHORS.txt (http://jqueryui.com/about)
 * Dual licensed under the MIT or GPL Version 2 licenses.
 * http://jquery.org/license
 *
 * http://docs.jquery.com/UI/Accordion#theming
 */
/* IE/Win - Fix animation bug - #4615 */
.ui-accordion { width: 100%; }
.ui-accordion .ui-accordion-header { cursor: pointer; position: relative; margin-top: 1px; zoom: 1; }
.ui-accordion .ui-accordion-li-fix { display: inline; }
.ui-accordion .ui-accordion-header-active { border-bottom: 0 !important; }
.ui-accordion .ui-accordion-header a { display: block; font-size: 1em; padding: 0px; }
.ui-accordion-icons .ui-accordion-header a { padding-left:0px; }
.ui-accordion .ui-accordion-header .ui-icon { position: absolute; left: .5em; top: 50%; margin-top: -8px; }
.ui-accordion .ui-accordion-content { padding: 0px; border-top: 0; margin-top: -2px; position: relative; top: 1px; margin-bottom: 2px; overflow: auto; display: none; zoom: 1; }
.ui-accordion .ui-accordion-content-active { display: block; }
.tituloAcordion{}
</style>

<?php 
	include  APPRAIZ."includes/cabecalho.inc";
	echo '<br>';
	monta_titulo( $titulo_modulo, '&nbsp' );

?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
	<td colspan="2" align="center" bgcolor="#dedede">
	<TABLE width="100%" cellspacing="0" cellpadding="3" border ="1">
		<tr>
			<?
				$sql = "SELECT	estuf, estdescricao	FROM territorios.estado	ORDER BY estuf ";
				$arrDados = $db->carregar($sql);
				if($arrDados){
					for ( $i = 0; $i < count( $arrDados ); $i++ )
						{
			?>
			<td bgcolor="#dedede" align="center" onclick="carregarEstado('<?=$arrDados[$i]['estuf'];?>', this);"><div id="estuf<?=$arrDados[$i]['estuf'];?>"><?=$arrDados[$i]['estuf'];?></div></td>
			<?
						}

				}
			?>
		</tr>
	</TABLE>
	<div id="nome_mun" style="color: black;font-family: Arial;font-size: 12pt;font-weight: bolder;">Brasil</div>
	</td>
	</tr>
</table>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td valign="top" style="width:250px;height:400px">
			
			<div id="accordion">
				<a class="tituloAcordion" href="#">FILTROS<hr></a>
				<div>
			<table cellSpacing="0" cellPadding="3" align="left" class="tabela" width="240px">
				<tr>
					<td class="SubTituloEsquerda">Busca Textual</td>
				</tr>
				<tr>
					<td>
						<input type="text" id=texto_busca name=texto_busca size=16> 
						<input type=button value="Ok" onClick="javascript: buscar();">
					</td>
				</tr>
				<tr>
					<td class="SubTituloEsquerda">Marcadores</td>
				</tr>
				<tr>
					<td>
						<img src="/imagens/icone_capacete_1.png"> <input onclick="if(this.checked){showOverlays(markersArray_7);}else{clearOverlays(markersArray_7);}" type="checkbox" name="chk_7" checked="checked" title="Câmpus Preexistentes" id="chk_7" value="7"  /> <font size=1>1 - Câmpus Preexistentes</font><br/>
						<img src="/imagens/icone_capacete_3.png"> <input onclick="if(this.checked){showOverlays(markersArray_8);}else{clearOverlays(markersArray_8);}" type="checkbox" name="chk_8" checked="checked" title="Criadas em 2003/2010" id="chk_8" value="8"  /> <font size=1>2 - Criadas (2003/2010)</font><br/>
						<img src="/imagens/icone_capacete_2.png"> <input onclick="if(this.checked){showOverlays(markersArray_4);}else{clearOverlays(markersArray_4);}" type="checkbox" name="chk_4" checked="checked" title="Câmpus Novos" id="chk_4" value="4"  /> <font size=1>3 - Previstos (2011/2012)</font><br/>
						<img src="/imagens/icone_capacete_4.png"> <input onclick="if(this.checked){showOverlays(markersArray_3);}else{clearOverlays(markersArray_3);}" type="checkbox" name="chk_3" checked="checked" title="Câmpus Previstos" id="chk_3" value="3"  /> <font size=1>4 - Propostos (2013/2014)</font><br/>
						<img src="/imagens/icone_capacete_7.png"> <input onclick="if(this.checked){showOverlays(markersArray_6);}else{clearOverlays(markersArray_6);}" type="checkbox" name="chk_6" checked="checked" title="Universidades Previstas" id="chk_6" value="6"  /> <font size=1>5 - Universidades Previstas (2013/2014)</font><br/>
					</td>
				</tr>
				<tr>
					<td class="SubTituloEsquerda">
						<img style="cursor: pointer" src="../imagens/mais.gif" id="img_uf" onclick="mostrar_painel('uf');" border=0> UF
					</td>
				</tr>


				<tr>
					<td>
						<div id="uf" style="display:none">
							<?php
							$sql = "	SELECT
											estuf AS codigo,
											estdescricao AS descricao
										FROM 
											territorios.estado
										ORDER BY
											estdescricao ";
				
							combo_popup( 'estuf', $sql, 'Selecione as Unidades Federativas', '400x400', 0, array(), '', 'S', false, false, 5, 240, '', '' );
							?>
						</div>
					</td>
				</tr>
				<tr>
					<td class="SubTituloEsquerda">
						<img style="cursor: pointer" src="../imagens/mais.gif" id="img_municipio" onclick="mostrar_painel('municipio');" border=0> Município
					</td>
				</tr>
				<tr>
					<td>
						<div id="municipio" style="display:none">
							<?php
							$sql = " 	SELECT	
											muncod AS codigo,
											mundescricao AS descricao
										FROM 
											territorios.municipio
										ORDER BY
											mundescricao";
				
							combo_popup( 'muncod', $sql, 'Selecione os Municípios', '400x400', 0, array(), '', 'S', false, false, 5, 240);							?>
						</div>
					</td>
				</tr>

				
			<tr>
				<td class="SubTituloEsquerda" ><img style="cursor:pointer" id="img_agrup" onclick="mostrar_painel('agrup');" src="/imagens/mais.gif"> Agrupadores</td>
			</tr>
			<tr>
				<td>
				<div style="display:none" id="agrup">
				<table width="100%">
				<tr>
					<td>
						<select id="agrupador" name="agrupador[]" multiple="multiple" size="4" style="width: 160px; height: 70px;" class="combo campoEstilo">
						<option value="uf">UF</option>
						<option value="tipo">Tipo</option>
						<option value="municipio">Município</option>
						<option value="campus">Campus</option>
						<option value="regiao">Região</option>
						</select>
					</td>
					<td>
		                <img src="../imagens/uarrow.gif" style="padding: 5px" onClick="subir( document.getElementById( 'agrupador' ) );"/><br/>
		                <img src="../imagens/darrow.gif" style="padding: 5px" onClick="descer( document.getElementById( 'agrupador' ) );"/><br/>
					</td>
				</tr>
				<tr>
					<td colspan="2" align="right"><input type="button" name="removeritem" value="Remover" onclick="removeSelectedOptions(document.getElementById('agrupador'));"> <input type="button" name="restauraritens" value="Restaurar" onclick="restaurarItens();"></td>
				</tr>
				</table>
				<input type="hidden" name="hdn_agrup" id="hdn_agrup" value="0" />
				</div>
				
				</td>
			</tr>
				
				
				<tr>
					<td class="SubTituloDireita">
						<input type="button" value="Carregar" id="carregar" onclick="carregarPontos();" >
					</td>
				</tr>
			</table>
			
					</div>
					<a class="tituloAcordion" href="#">INFORMAÇÃO DO MUNICÍPIO<hr></a>
					<div>
						<div id="inf_mun" style="color: black;font-family: Arial;font-size: 14pt;font-weight: bolder;width:230px"></div>
					</div>
					<a class="tituloAcordion" href="#">MAIS INFORMAÇÕES<hr></a>
					<div>
						...
					</div>
			</div>
			
		</td>
		<td valign="top">
		<div style="position:absolute;z-index:3;text-align:center;padding:1px;width:60px;margin-left:80px;margin-top:10px;"><input type="button" value="Brasil"  onclick="centralizaBrasil()" /></div>
		<div id="map_canvas" style="width:100%;height:100%;position:relative;"></div>
		</td>
		
		<td valign="top" id="info_td" style="width:250px;display:none;">
		
		<table width="100%">
		<tr>
			<td>
			<div style="overflow:auto;height:398px" id="colunainfo"></div>
			</td>
		</tr>

		</table>
		
		</td>
		
	</tr>
</table>
<script>
	initialize();
	
</script>