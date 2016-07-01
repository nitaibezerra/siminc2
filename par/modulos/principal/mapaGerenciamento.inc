<meta http-equiv="content-type" content="text/html; charset=iso-8859-1"/>
<?php

function carregaMunicipios( $estuf ){
	
	$sql = "SELECT DISTINCT
				muncod as codigo, 
				estuf || ' - ' || mundescricao as descricao
			FROM 
				territorios.municipio
			WHERE
				estuf = '$estuf'";
	
	combo_popup( 'muncod', $sql, 'Selecione os municípios', '500x500', 0, array(), '', 'S', false, false, 5, 155, '', '' );
}

if( $_REQUEST['req'] == 'carregaMunicipios' ){
	
	echo carregaMunicipios( $_REQUEST['estuf'] );
	die();
}
	
include APPRAIZ."includes/cabecalho.inc";
include APPRAIZ . 'includes/Agrupador.php';
echo'<br>';

$abasPar = array( 0 => array( "descricao" => "Painel Gerencial",
						  "link"	  => "par.php?modulo=principal/painelGerenciamento&acao=A" ),
			      1 => array( "descricao" => "Mapa Gerencial",
						  "link"	  => "par.php?modulo=principal/mapaGerenciamento&acao=A" )  
					 );

echo montarAbasArray( $abasPar, "par.php?modulo=principal/mapaGerenciamento&acao=A" );

monta_titulo( $titulo_modulo, '' );
	
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
<?if ($local[2]=="simec.mec.gov.br" ){ ?>
<script src="http://maps.google.com/maps?file=api&v=2&amp;key=ABQIAAAAXKOUFW3PNxuPE30S17ocgBQhVwj8ALbvbyVgNcB-R-H_S2MIRxSRw07TtkPv50s-khCgCjw1KhcuSw" type="text/javascript"></script>
<? } ?>
<?if ($local[2]=="simec-d.mec.gov.br"){ ?>
<script src="http://maps.google.com/maps?file=api&v=2&amp;key=ABQIAAAAXKOUFW3PNxuPE30S17ocgBRYtD8tuHxswJ_J7IRZlgTxP-EUtxRD3aBmpKp7QQhM-oKEpi_q_Z6nzQ" type="text/javascript"></script> 
<? } ?>
<?if ($local[2]=="simec" ){ ?>
<script src="http://maps.google.com/maps?file=api&v=2&amp;key=ABQIAAAAXKOUFW3PNxuPE30S17ocgBTNzTBk8zukZFuO3BxF29LAEN1D1xRrpthpVw0AZ6npV05I8JLIRtHtyQ" type="text/javascript"></script>
<? } ?>
<?if ($local[2]=="simec-d"){ ?>
<script src="http://maps.google.com/maps?file=api&v=2&amp;key=ABQIAAAAXKOUFW3PNxuPE30S17ocgBTFm3qU4CVFuo3gZaqihEzC-0jfaRSyt8kdLeiWuocejyXXgeTtRztdYQ" type="text/javascript"></script> 
<? } ?>

<?if ($local[2]=="simec-local"){ ?>
<script src="http://maps.google.com/maps?file=api&v=2&amp;key=ABQIAAAANS5-OhaJ-rD0gFdWBaQI8xRzjpIxsx3o6RYEdxEmCzeJMTc4zBSsYYW_8yZ5SPlLhcEEXDryN4-dAw" type="text/javascript"></script> 	
<? } ?>
<script type="text/javascript">
	document.write('<script type="text/javascript" src="../includes/extlargemapcontrol'+(document.location.search.indexOf('packed')>-1?'_packed':'')+'.js"><'+'/script>');
</script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/FusionCharts/FusionCharts.js"></script>
<script type="text/javascript">
    
    var map;
    
    function ajaxatualizar(params,iddestinatario) {
    	var myAjax = new Ajax.Request(
    		window.location.href,
    		{
    			method: 'post',
    			parameters: params,
    			asynchronous: false,
    			onComplete: function(resp) {
    				if(document.getElementById(iddestinatario)) {
    					extrairScript(resp.responseText);
    					document.getElementById(iddestinatario).innerHTML = resp.responseText;
    					
    					if(document.getElementById('temporizador'))
							document.getElementById('temporizador').style.display = 'none';
    					
    				} 
    			},
    			onLoading: function(){
    				if(document.getElementById(iddestinatario))
    					document.getElementById(iddestinatario).innerHTML = 'Carregando...';
    			}
    		});
    }
    
    function verificarCarregar() {
    	
    	document.getElementById('clickcarregar').value='1';
    	document.getElementById('carregar').disabled=true;
    	document.getElementById('carregando').innerHTML="Carregando...";
    	carregar();
    	document.getElementById('carregando').innerHTML="";
    	document.getElementById('carregar').disabled=false;
    }
    
	var marcadores = new Array();
	var markerGroups = new Array();
	
	function initialize() {
      if (GBrowserIsCompatible()) {
       var opts = {
		 googleBarOptions : {
		   style : 'new'
		 }
		}
		map = new GMap2(document.getElementById("mapa"), opts);
		// Controles      	
		map.addControl(new GLargeMapControl3D());
        map.addControl(new GOverviewMapControl());
        map.enableScrollWheelZoom();
        map.addMapType(G_PHYSICAL_MAP);

       	// Preenchendo o valor inicial do mapa
		var zoom = 4;	var lat_i = -14.689881; var lng_i = -52.373047;		
		map.setCenter(new GLatLng(lat_i,lng_i), parseInt(zoom));

		// Mostrar Lat e Lng MouseMove
		GEvent.addListener(map,"mousemove",function(latlng) {
	       	document.getElementById('lat').value=dec2grau(latlng.lat());
	        document.getElementById('lng').value=dec2grau(latlng.lng());
        });

		} 
	}
	
	// Criando o ícones do mapa
	var customIcons = [];
<?php 
	$sql = "SELECT 
				ptoid as codigo 
			FROM 
				obras.pretipoobra
			WHERE
				ptostatus = 'A'
			ORDER BY
				ptoid";
	
	$ptoids = $db->carregar($sql);
	
	if( is_array( $ptoids ) ){
		foreach( $ptoids as $dado ){
			$marcadores .= $dado['codigo'].':[],';
?>
			var marca = new GIcon();
			marca.iconSize = new GSize(9, 14);
			marca.iconAnchor = new GPoint(9, 15);
			marca.infoWindowAnchor = new GPoint(9, 2);
			marca.image = '../imagens/icone_capacete_pre<?=$dado['codigo'];?>.png';
			customIcons[<?=$dado['codigo'];?>] = marca;	
<?php 
		}
		$marcadores .= '104:[]';
	}
?>

	markerGroups = {<?=$marcadores ?>};
 
	function carregar( texto_busca ){
		// Limpar todos os marcadores
		map.clearOverlays();

		// Lendo os Filtros
		
		// Situação
		var esdid= "0";
		for(i = 0 ; i < document.getElementsByName("esdid[]").length ; i++){
			if(document.getElementsByName("esdid[]")[i].checked)
				esdid += "{"+document.getElementsByName("esdid[]")[i].value+"}";
		}

		// Tipo Obra
		var ptoid= "0";
		for(i = 0 ; i < document.getElementsByName("ptoid[]").length ; i++){
			if(document.getElementsByName("ptoid[]")[i].checked)
				ptoid += "{"+document.getElementsByName("ptoid[]")[i].value+"}";
		}

		// Estado 
		selectAllOptions(document.getElementById('estuf'));
		estuf="";
		var selObj = document.getElementById('estuf');
		var i;
		for (i=0; i<selObj.options.length; i++) {
			if (selObj.options[i].selected) {
		     	estuf +="{'"+selObj.options[i].value+"'}";
		  	}
		}

		// Municipios
		selectAllOptions(document.getElementById('muncod'));
		muncod = "";
		var selObj = document.getElementById('muncod');
		var i;
		for ( i=0; i < selObj.options.length; i++ ) {
			if ( selObj.options[i].selected ) {
		     	muncod += "{'"+selObj.options[i].value+"'}";
		  	}
		}
		
		if ( estuf != "{''}" ){
			if( muncod != "{''}{''}" && muncod != "{''}" ){
				xml_filtro="par.php?modulo=principal/mapaGerenciamento_geral_xml&acao=A&esdid="+esdid+"&ptoid="+ptoid+"&estuf="+estuf+"&muncod="+muncod;
			}else{
				xml_filtro="par.php?modulo=principal/mapaGerenciamento_geral_xml&acao=A&esdid="+esdid+"&ptoid="+ptoid+"&estuf="+estuf;
			}
		}else{
			xml_filtro="par.php?modulo=principal/mapaGerenciamento_geral_xml&acao=A&esdid="+esdid+"&ptoid="+ptoid;
		}

		// Criando os Marcadores com o resultado
		GDownloadUrl(xml_filtro, function( data ) {
			var xml = GXml.parse(data);
			var markers = xml.documentElement.getElementsByTagName("marker");
//			alert(xml_filtro);
//			alert(markers.length);
			if(markers.length > 0) {
				var lat_ant=0;
				var lng_ant=0;
				for (var i = 0; i < markers.length; i++) {
					var obrdesc = markers[i].getAttribute("obrdesc");
					var obrid = markers[i].getAttribute("idobra");
					type = parseInt(markers[i].getAttribute("ptoid"));
//					type = 1;
		
					// Verifica pontos em um mesmo lugar e move o seguinte para a direita
					if(lat_ant==markers[i].getAttribute("lat") && lng_ant==markers[i].getAttribute("lng"))
						var point = new GLatLng(markers[i].getAttribute("lat"),	markers[i].getAttribute("lng"));
					else
						var point = new GLatLng(markers[i].getAttribute("lat"),	parseFloat(markers[i].getAttribute("lng"))+0.0005);				
		
					lat_ant=markers[i].getAttribute("lat");
					lng_ant=markers[i].getAttribute("lng");
					
					// Cria o marcador na tela
					var marker=createMarker(point,obrdesc,type,obrid);
					markerGroups[type].push(marker);
					marcadores[obrid]=marker;
					map.addOverlay(marker);
				}
			} else {
				alert('Não existem obras');
			}
		});
	}

	
	function abrebalao(marcador){
		marcadores[marcador].openInfoWindowHtml('<iframe src=/par/par.php?modulo=principal/mapaGerenciamento_balao&acao=A&preid='+marcador+' width=350 frameborder=0 ></iframe>');
	}
	
	function createMarker(point,obrdesc,type,obrid) {
		var icone = customIcons[type];
		var marker = new GMarker(point,{title: obrdesc, icon: icone, draggable:false });
		GEvent.addListener(marker, 'click', function() {
			html='<iframe src=/par/par.php?modulo=principal/mapaGerenciamento_balao&acao=A&preid='+obrid+' width=350 frameborder=0 ></iframe>';
	        marker.openInfoWindowHtml(html);
	    	});
	 return marker;
	}
	
	function toggleGroup(type) {
		for (var i = 0; i < markerGroups[type].length; i++) {
			var marker = markerGroups[type][i];
			if (marker.isHidden()) {
          		marker.show();
	        } else {
	          	marker.hide();
	        }
      	} 
      	for (var i = 0; i < markerGroups[type+100].length; i++) {
        	var marker = markerGroups[type+100][i];
        	if (marker.isHidden()) {
          		marker.show();
        	} else {
         		 marker.hide();
        	}
 		}
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

	// Esconder e mostrar os paineis
	function mostrar_painel( painel ){
		switch( painel ) {
			case 'situacao':
				if(document.getElementById(painel).style.display == "none") {
					document.getElementById("img_"+painel).src="../imagens/menos.gif";
				} else {
					document.getElementById("img_"+painel).src="../imagens/mais.gif";
				}
			break;
			case 'tipo':
				if(document.getElementById(painel).style.display == "none") {
					document.getElementById("img_"+painel).src="../imagens/menos.gif";
				} else {
					document.getElementById("img_"+painel).src="../imagens/mais.gif";
				}
			break;
			case 'uf':
				if(document.getElementById(painel).style.display == "none") {
					document.getElementById("img_"+painel).src="../imagens/menos.gif";
				} else {
					document.getElementById("img_"+painel).src="../imagens/mais.gif";
				}
				
				status = document.getElementById('municipios').style.display;
				
				if ( status != "none" ) {
					document.getElementById('img_municipios').src="../imagens/mais.gif";
					document.getElementById('municipios').style.display="none";
				} 
				
			break;
			case 'municipios':
				if(document.getElementById( painel ).style.display == "none") {
					document.getElementById( "img_"+painel ).src="../imagens/menos.gif";
				} else {
					document.getElementById( "img_"+painel ).src="../imagens/mais.gif";
				}
			break;
		}
			
		status = document.getElementById( painel ).style.display;
		
		if ( status=="none" ) {
			document.getElementById( painel ).style.display="block";
		} else {
			document.getElementById( painel ).style.display="none";
		}
	}
	
	function visao(tipo){
		if (tipo=='mapa')
			map.setMapType(G_NORMAL_MAP);
		if (tipo=='satelite')
			map.setMapType(G_SATELLITE_MAP);
		if (tipo=='hibrido')
			map.setMapType(G_HYBRID_MAP);
		if (tipo=='terreno')
			map.setMapType(G_PHYSICAL_MAP);		
	}
	
	function mapa_original (){
		// Preenchendo o valor inicial do mapa
		var zoom = 4;	var lat_i = -14.689881; var lng_i = -52.373047;		
		map.setCenter(new GLatLng(lat_i,lng_i), parseInt(zoom));
	}

	function mostra_municipios(){
		
		var uf = $('estuf');
		var municipios = $('municipios');

		if( uf[0].value != '' ){
			var myAjax = new Ajax.Request(
		    		window.location.href,
		    		{
		    			method: 'post',
		    			parameters: '&req=carregaMunicipios&estuf='+uf[0].value,
		    			asynchronous: false,
		    			onComplete: function(resp) {
		    				municipios.innerHTML = resp.responseText;
		    			},
		    			onLoading: function(){
		    				if(document.getElementById(iddestinatario))
		    					document.getElementById(iddestinatario).innerHTML = 'Carregando...';
		    			}
		    		});
		}else{
			municipios.innerHTML = '<select style="width: 155px;" class="CampoEstilo" id="muncod" name="muncod[]" size="5" multiple="multiple" disabled="disabled" tipo="combo_popup" maximo="0"><option value="">Escolha pelo menos</option><option value=""> um estado.</option></select>';
		}
	}
	
</script>
<form name="formulario" id="pesquisar" method="POST" action="">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
		<tr>
			<td valign="top" bgcolor=#ffffff width="170px">
				<table cellSpacing="0" cellPadding="3" align="left" class="tabela" width="260px">
					<tr> 
						<td class="SubTituloEsquerda">
							Tipo da Obra
						</td>
					</tr>
					<tr> 
						<td >
							<div >
							<?php  
							$sql = "SELECT 
										' <img src=\"../imagens/icone_capacete_pre' || ptoid || '.png\">' as indice,
										ptoid as codigo, 
										ptodescricao  as descricao
									FROM 
										obras.pretipoobra
									WHERE
										ptostatus = 'A'
									ORDER BY
										ptoid";
							$db->monta_checkbox("ptoid[]", $sql, $ptoid, "<br>"); ?>
							</div>
						</td>
					</tr>
					<tr> 
						<td class="SubTituloEsquerda">
							<img style="cursor: pointer" src="../imagens/mais.gif" id="img_situacao" onclick="javascript: mostrar_painel('situacao');" border=0> 
							Situação
						</td>
					</tr>
					<tr> 
						<td>
							<div id=situacao style="display: none">
								<?php  
								$sql = "SELECT DISTINCT
											esdid as codigo,
											esddsc as descricao
										FROM 
											workflow.estadodocumento
										WHERE
											esdstatus = 'A' AND
											tpdid = 37
										ORDER BY
											descricao";
								$db->monta_checkbox("esdid[]", $sql, $stoid, "<br>"); 
								?>
							</div>
						</td>
					</tr>
					<tr> 
						<td class="SubTituloEsquerda">
							<img style="cursor: pointer" src="../imagens/mais.gif" id="img_uf" onclick="javascript: mostrar_painel('uf');" border=0> 
							UF
						</td>
					</tr>
					<tr> 
						<td >
							<div id=uf style="display: none">
							<?php
							$sql = " SELECT		
											estuf AS codigo,
											estdescricao AS descricao
										FROM 
											territorios.estado
										ORDER BY
											estdescricao ";
				
							combo_popup( 'estuf', $sql, 'Selecione as Unidades Federativas', '400x400', 0, array(), '', 'S', false, false, 5, 155, '', '' );
							?>
							</div>
						</td>
					</tr>
					<tr> 
						<td class="SubTituloEsquerda">
							<img style="cursor: pointer" src="../imagens/mais.gif" id="img_municipios" onclick="mostra_municipios();mostrar_painel('municipios');" border=0> 
							Municípios
						</td>
					</tr>
					<tr> 
						<td>
							<div id="municipios" style="display: none">
								<select style="width: 155px;" class="CampoEstilo" id="muncod" name="muncod[]" size="5" multiple="multiple" disabled="disabled" tipo="combo_popup" maximo="0">
									<option value="">Escolha pelo menos</option>
									<option value=""> um estado.</option>
								</select>
							</div>
						</td>
					</tr>
					<tr> 
						<td class="SubTituloDireita">
							<input type="button" value="Carregar"     id="carregar"      onClick="javascript: verificarCarregar();">
							<input type="hidden" name="clickcarregar" id="clickcarregar" value="0">
							<br>
							<div id="carregando"></div>
						</td>
					</tr>
				</table>
			</td>
			<td style="vertical-align: top;">
			    <table bgcolor=#c3c3c3 width=100%>
			    <tr><td align=left><input type="button" value="Mapa Brasil" onClick="javascript: mapa_original();"></td><td align=center></td><td align=center></td><td align="right"><input type="button" value="Mapa" onClick="javascript: visao('mapa');"><input type="button" value="Satélite" onClick="javascript: visao('satelite');"><input type="button" value="Híbrido" onClick="javascript: visao('hibrido');"><input type="button" value="Terreno" onClick="javascript: visao('terreno');"></td></tr>
			    </table>
			    <!-- Mapa -->
			    	<div id="mapa" style="width: 100%; height: 520px; position: relative"></div>
			    <!-- Mapa -->
			    
			    <table bgcolor=#c3c3c3 width=100%>
			    <tr>
			    <td>
			    Latitude: <input type=text id=lat name=lat value="0" STYLE="border:none ; background-color:#c3c3c3" size=8 /> 
			    Longitude: <input type=text id=lng name=lng value="0" STYLE="border:none; background-color:#c3c3c3" size=8 />
			    </td>
			    </tr>
			    </table>
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript">initialize(); </script>