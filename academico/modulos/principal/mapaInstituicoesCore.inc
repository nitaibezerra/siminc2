<?php
function curPageURL() {
	$pageURL = 'http';
	if ($_SERVER["HTTPS"] == "on") {$pageURL .= "s";}
	$pageURL .= "://";
	if ($_SERVER["SERVER_PORT"] != "80") {
		$pageURL .= $_SERVER["SERVER_NAME"].":".$_SERVER["SERVER_PORT"].$_SERVER["REQUEST_URI"];
	}else{
		$pageURL .= $_SERVER["SERVER_NAME"].$_SERVER["REQUEST_URI"];
	}
	return $pageURL;
}

$local= explode("/",curPageURL());

if ($local[2]=="simec.mec.gov.br" ){ ?>
    	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAXKOUFW3PNxuPE30S17ocgBQhVwj8ALbvbyVgNcB-R-H_S2MIRxSRw07TtkPv50s-khCgCjw1KhcuSw" type="text/javascript"></script>
<? }
if ($local[2]=="simec-d.mec.gov.br"){ ?>
  		<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAXKOUFW3PNxuPE30S17ocgBRYtD8tuHxswJ_J7IRZlgTxP-EUtxRD3aBmpKp7QQhM-oKEpi_q_Z6nzQ" type="text/javascript"></script> 
	<? }
if ($local[2]=="simec" ){ ?>
    	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAXKOUFW3PNxuPE30S17ocgBTNzTBk8zukZFuO3BxF29LAEN1D1xRrpthpVw0AZ6npV05I8JLIRtHtyQ" type="text/javascript"></script>
  	<? }
if ($local[2]=="simec-d"){ ?>
  		<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAXKOUFW3PNxuPE30S17ocgBTFm3qU4CVFuo3gZaqihEzC-0jfaRSyt8kdLeiWuocejyXXgeTtRztdYQ" type="text/javascript"></script> 
	<? }
if ($local[2]=="simec-local"){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAXKOUFW3PNxuPE30S17ocgBRzjpIxsx3o6RYEdxEmCzeJMTc4zBRTBTWZ8zjwenwhN3CBw_oSZaFJjQ" type="text/javascript"></script> 	
<? } ?>
<script>

	var map; //variável do Google Maps
	
	var markerGroups = { '11': [], '12': [], '21': [] , '22': [] ,'0' :[],'121' :[],'122' :[],'123' :[],'124' :[],'221' :[],'222' :[],'223' :[],'224' :[] , '1212' :[] , '2212' :[]};
	
	var xml_antigo;
    
    function initialize() {
		if (GBrowserIsCompatible()) { // verifica se o navegador é compatível
				map = new GMap2(document.getElementById("mapa")); // inicila com a div mapa
				var zoom = 4;	var lat_i = -14.689881; var lng_i = -52.373047;	//Brasil	
				map.setCenter(new GLatLng(lat_i,lng_i), parseInt(zoom)); //Centraliza e aplica o zoom

				
				// Início Controles
				map.addControl(new GMapTypeControl());
				map.addControl(new GLargeMapControl3D());
		        map.addControl(new GOverviewMapControl());
		        map.enableScrollWheelZoom();
		        map.addMapType(G_PHYSICAL_MAP);
		        // Fim Controles
		        
		        // Criando o ícone do mapa
				var baseIcon = new GIcon();
				baseIcon.iconSize = new GSize(9, 14);
				baseIcon.iconAnchor = new GPoint(9, 14);
				baseIcon.infoWindowAnchor = new GPoint(9, 2);
		
		}
		
	}
	
	function validaBuscaTextual(msg){
		
		var busca = document.getElementById('inp_busca');
		
		if(!busca.value && msg){
			alert("Favor inserir dados para a busca!");
			busca.focus();
			return false;
		}if(!busca.value && !msg){
			return false;
		}else{
			return busca.value;
		}
	}
	
	function buscaMapaEnter(e){
		if (e.keyCode == 13)
	         buscaMapa(true);
	}
	
	function buscaMapa(msg){
		
		//Busca Textual
		var busca_textual = validaBuscaTextual(msg);
		
		if(msg && !busca_textual)
			return false;
		
		//Busca por UF
		var busca_uf = validaBuscaUF();
		
		//Busca por Ensino
		var orgid = validaOrgid();
		
		if(!orgid)
			return false;
		
		var xml_filtro="XMLmapaInstituicoes.php?1=1";
		
		if(orgid)
			xml_filtro += "&orgid="+orgid;
		if(busca_uf)
			xml_filtro += "&uf="+busca_uf;
		if(busca_textual)
			xml_filtro += "&buscaTextual="+busca_textual;
		
		if(xml_antigo != xml_filtro)
			marcaInstituicoes(xml_filtro);
			
	}
	
	function exibeUF(obj){
		
		if(obj.src.search("mais.gif") > 0 ){
			obj.src="/imagens/menos.gif";
			document.getElementById('td_uf').style.display='';
			document.getElementById('hdn_uf').value = 1;
		}else{
			obj.src="/imagens/mais.gif";
			document.getElementById('td_uf').style.display='none';
			document.getElementById('hdn_uf').value = 0;
		}
	
	}
	
	function validaBuscaUF(selObj){
		
		if(document.getElementById('hdn_uf').value == 1){
			selectAllOptions(document.getElementById('inp_estuf'));
			selObj = document.getElementById('inp_estuf');
			if(selObj.options[0].value){
				var estuf;
				for (var i=0; i<selObj.options.length; i++) {
					if (selObj.options[i].selected) {
				     	if(i != 0)
				     		estuf+= ",'"+selObj.options[i].value+"'";
				     	else
				     		estuf = "'"+selObj.options[i].value+"'";
				  	}
				}
				return estuf;
			}else{
				return false;
			}
		}else{
				return false;
			}
	}
	
	function validaOrgid(){
		
		var ens_sup = document.getElementById('ckc_orgid_sup');
		var ens_prof = document.getElementById('ckc_orgid_prof');
		
		if(ens_sup.checked == false && ens_prof.checked == false){
			alert('Favor informar o Tipo de Ensino!');
			return false;
		}else{
			if(ens_sup.checked == false && ens_prof.checked == true)
				return ens_prof.value;
			if(ens_sup.checked == true && ens_prof.checked == false)
				return ens_sup.value;
			else
				return ens_sup.value + "," + ens_prof.value ;
		}
				
	}
	
	function marcaInstituicoes(url){
		
		xml_antigo = url;
		
		exibeCarregando();
		
		map.clearOverlays();
		
		// Criando o ícone 1 do mapa
		var baseIcon1 = new GIcon();
		baseIcon1.iconSize = new GSize(9, 14);
		baseIcon1.iconAnchor = new GPoint(9, 14);
		baseIcon1.infoWindowAnchor = new GPoint(9, 2);
        baseIcon1.image='/imagens/icone_capacete_1.png';
        
        // Criando o ícone 1 do campus
		var baseIconCampus1 = new GIcon();
		baseIconCampus1.iconSize = new GSize(15, 17);
		baseIconCampus1.iconAnchor = new GPoint(11, 13);
		baseIconCampus1.infoWindowAnchor = new GPoint(9, 2);
        baseIconCampus1.image='/imagens/icones/lp.png';
        
        // Criando o ícone 2 do campus
		var baseIconCampus2 = new GIcon();
		baseIconCampus2.iconSize = new GSize(15, 17);
		baseIconCampus2.iconAnchor = new GPoint(11, 13);
		baseIconCampus2.infoWindowAnchor = new GPoint(9, 2);
        baseIconCampus2.image='/imagens/icones/lr.png';
        
        // Criando o ícone 2 do campus funcionando
		var baseIconCampus2Funcionando = new GIcon();
		baseIconCampus2Funcionando.iconSize = new GSize(15, 17);
		baseIconCampus2Funcionando.iconAnchor = new GPoint(11, 13);
		baseIconCampus2Funcionando.infoWindowAnchor = new GPoint(9, 2);
        baseIconCampus2Funcionando.image='/imagens/icones/lbr.png';
        
        // Criando o ícone 3 do campus
		var baseIconCampus3 = new GIcon();
		baseIconCampus3.iconSize = new GSize(15, 17);
		baseIconCampus3.iconAnchor = new GPoint(11, 13);
		baseIconCampus3.infoWindowAnchor = new GPoint(9, 2);
        baseIconCampus3.image='/imagens/icones/ly.png';
        
        // Criando o ícone 4 do campus
		var baseIconCampus4 = new GIcon();
		baseIconCampus4.iconSize = new GSize(15, 17);
		baseIconCampus4.iconAnchor = new GPoint(11, 13);
		baseIconCampus4.infoWindowAnchor = new GPoint(9, 2);
        baseIconCampus4.image='/imagens/icones/ly.png';
        
        // Criando o ícone 5 do campus
		var baseIconCampus5 = new GIcon();
		baseIconCampus5.iconSize = new GSize(15, 17);
		baseIconCampus5.iconAnchor = new GPoint(11, 13);
		baseIconCampus5.infoWindowAnchor = new GPoint(9, 2);
        baseIconCampus5.image='/imagens/icones/lb.png';

        
        // Criando o ícone 1 do campus
		var baseIcon2Campus1 = new GIcon();
		baseIcon2Campus1.iconSize = new GSize(15, 17);
		baseIcon2Campus1.iconAnchor = new GPoint(11, 13);
		baseIcon2Campus1.infoWindowAnchor = new GPoint(9, 2);
        baseIcon2Campus1.image='/imagens/icones/tp.png';
        
        // Criando o ícone 2 do campus
		var baseIcon2Campus2 = new GIcon();
		baseIcon2Campus2.iconSize = new GSize(15, 17);
		baseIcon2Campus2.iconAnchor = new GPoint(11, 13);
		baseIcon2Campus2.infoWindowAnchor = new GPoint(9, 2);
        baseIcon2Campus2.image='/imagens/icones/tr.png';
        
        // Criando o ícone 2 do campus funcionando
		var baseIcon2Campus2Funcionando = new GIcon();
		baseIcon2Campus2Funcionando.iconSize = new GSize(15, 17);
		baseIcon2Campus2Funcionando.iconAnchor = new GPoint(11, 13);
		baseIcon2Campus2Funcionando.infoWindowAnchor = new GPoint(9, 2);
        baseIcon2Campus2Funcionando.image='/imagens/icones/tbr.png';
        
        // Criando o ícone 3 do campus
		var baseIcon2Campus3 = new GIcon();
		baseIcon2Campus3.iconSize = new GSize(15, 17);
		baseIcon2Campus3.iconAnchor = new GPoint(11, 13);
		baseIcon2Campus3.infoWindowAnchor = new GPoint(9, 2);
        baseIcon2Campus3.image='/imagens/icones/ty.png';
        
        // Criando o ícone 4 do campus
		var baseIcon2Campus4 = new GIcon();
		baseIcon2Campus4.iconSize = new GSize(15, 17);
		baseIcon2Campus4.iconAnchor = new GPoint(11, 13);
		baseIcon2Campus4.infoWindowAnchor = new GPoint(9, 2);
        baseIcon2Campus4.image='/imagens/icones/ty.png';
        
        // Criando o ícone 5 do campus
		var baseIcon2Campus5 = new GIcon();
		baseIcon2Campus5.iconSize = new GSize(15, 17);
		baseIcon2Campus5.iconAnchor = new GPoint(11, 13);
		baseIcon2Campus5.infoWindowAnchor = new GPoint(9, 2);
        baseIcon2Campus5.image='/imagens/icones/tb.png';

        
        // Criando o ícone 1 do mapa
		var baseIcon2 = new GIcon();
		baseIcon2.iconSize = new GSize(9, 14);
		baseIcon2.iconAnchor = new GPoint(9, 14);
		baseIcon2.infoWindowAnchor = new GPoint(9, 2);
        baseIcon2.image='/imagens/icone_capacete_2.png';
         
		xml_filtro = url;
		
		// Criando os Marcadores com o resultado
			GDownloadUrl(xml_filtro, function(data) {
				var xml = GXml.parse(data);
				
				var markers = xml.documentElement.getElementsByTagName("marker");
				
				
				if(markers.length > 0) {
					var lat_ant=0;
					var lng_ant=0;
				
					for (var i = 0; i < markers.length; i++) {
						
						var instituicao = markers[i].getAttribute("instituicao");
						var entid = markers[i].getAttribute("entid");
						var mundsc = markers[i].getAttribute("mundsc");
						var estuf = markers[i].getAttribute("estuf");
						title = instituicao + " - " + mundsc + " / " + estuf + "."; 
						var tipoensino = markers[i].getAttribute("tipoensino");
						var tipo = markers[i].getAttribute("tipo");
						var subdivisao = markers[i].getAttribute("subdivisao");
						var situacao = markers[i].getAttribute("situacao");
						
						if(tipoensino.search("Ensino Superior") >= 0)
							var orgid = "1";
						else
							var orgid = "2";
						
						if(situacao.search("Instituição") >= 0)
							var balao = "instituicao";
						else
							var balao = "campus";
						
						
						if(tipo.search("21") >= 0 || tipo.search("22") >= 0 ){
							
							icon = baseIcon2;
							
							if(subdivisao.search("Novo") >= 0 ){
								
								if(situacao.search("Não Funcionando") >= 0 ){
								
									icon = baseIcon2Campus2;
									tipo = '221';
								}else{
									icon = baseIcon2Campus2Funcionando;
									tipo = '2212';
								}
								
							}
							if(subdivisao.search("Criadas em 2003/2010") >= 0){
								icon = baseIcon2Campus5;
								tipo = '224';
							}
							
							if(subdivisao.search("Preexistente") >= 0){
								icon = baseIcon2Campus1;
								tipo = '222';
							}
							if(subdivisao.search("Previsto") >= 0){
								icon = baseIcon2Campus3;
								tipo = '223';
							}
							if(subdivisao.search("Não Existente") >= 0){
								icon = baseIcon2Campus4;
								tipo = '224';
							}
						}
						//if(tipo.search("11") >= 0 || tipo.search("12") >= 0){
						else{
							
							icon = baseIcon1;
							
							if(subdivisao.search("Novo") >= 0 ){
								
								if(situacao.search("Não Funcionando") >= 0 ){
									icon = baseIconCampus2;
									tipo = '121';
								}else{
									icon = baseIconCampus2Funcionando;
									tipo = '1212';
								}
								
							}
							
							if(subdivisao.search("Criadas em 2003/2010") >= 0){
								icon = baseIconCampus5;
								tipo = '124';
							}
							
							if(subdivisao.search("Preexistente") >= 0){
								icon = baseIconCampus1;
								tipo = '122';
							}
							if(subdivisao.search("Previsto") >= 0){
								icon = baseIconCampus3;
								tipo = '123';
							}
							if(subdivisao.search("Não Existente") >= 0){
								icon = baseIconCampus4;
								tipo = '124';
							}
						}
						
												
						var lat = markers[i].getAttribute("lat");
						var lng = markers[i].getAttribute("lng");
						
						var html;
						
						html = "<div style=\"font-family:verdana;font-size:11px;\" >";
						html += "<b>Instituição:</b> " + instituicao + "<br />";
						html += "<b>Localização:</b> " + mundsc + "/" + estuf + "<br /><br />";
						html += "<span style=\"cursor:pointer;font-weight:bold\">Mais detalhes...</span>";
						html += "</div>";
						
						html = "<div style=\"padding:5px\" ><iframe src=\"academico.php?modulo=principal/mapaInstituicoes&acao=A&montaBalao=1&entid=" + entid + "&tipo=" + balao + "&orgid=" + orgid + "\" frameborder=0 scrolling=\"auto\" height=\"380px\" width=\"480px\" ></iframe></div>";
						
						//var html = '<div style=\"font-family:verdana;font-size:11px;\" ><b>Localização:</b> ' + mundsc + ' / <br><br><br><div style=\"cursor:pointer;width:100%;text-align:right\">Mais detalhes...</div></div>';
										
						// Verifica pontos em um mesmo lugar e move o seguinte para a direita
						if(lat_ant==markers[i].getAttribute("lat") && lng_ant==markers[i].getAttribute("lng"))
							var point = new GLatLng(markers[i].getAttribute("lat"),	markers[i].getAttribute("lng"));
						else
							var point = new GLatLng(markers[i].getAttribute("lat"),	parseFloat(markers[i].getAttribute("lng"))+0.0005);				
			
						lat_ant=markers[i].getAttribute("lat");
						lng_ant=markers[i].getAttribute("lng");
						
						// Cria o marcador na tela
						var marker = createMarker(point,title,icon,html);
						
						markerGroups[tipo].push(marker);
						
						map.addOverlay(marker);
						//marker.openInfoWindowHtml(html);
				
		        }
		        setTimeout("disponibilizaMapa()",5000);
				}else{
					alert("Não foi possível localizar Instituições com o(s) filtro(s) aplicado(s)!")
					disponibilizaMapa();
				}
	        });
	        
	}
	
	function createMarker(posn, title, icon, html,muncod) {
      var marker = new GMarker(posn, {title: title, icon: icon, draggable:false });
      if(html != false){
	      GEvent.addListener(marker, "click", function() {
	      	marker.openInfoWindowHtml(html);
	       });
       }
      return marker;
    }
    
    function exibeCarregando(){
    	
    	//Bloqueia a Busca Textual
    	document.getElementById('inp_busca').disabled = true;
    	
    	//Bloqueia o Botão de Busca Textual
    	document.getElementById('btn_busca_textual').disabled = true;
    	
    	//Bloqueia o Botão de carregar
    	document.getElementById('btn_buscar').disabled = true;
    	
    	document.getElementById('carregando_mapa').style.display = "";
    	document.getElementById('mapa_exibe_carregando').style.display = "";
    	
    }
    
    function disponibilizaMapa(){
    	
    	//Bloqueia a Busca Textual
    	document.getElementById('inp_busca').disabled = false;
    	
    	//Bloqueia o Botão de Busca Textual
    	document.getElementById('btn_busca_textual').disabled = false;
    	
    	//Bloqueia o Botão de carregar
    	document.getElementById('btn_buscar').disabled = false;
    	
    	document.getElementById('carregando_mapa').style.display = "none";
    	document.getElementById('mapa_exibe_carregando').style.display = "none";
    	
    	if(document.getElementById('dados_superior').style.display == "none" && document.getElementById('ckc_orgid_sup').checked == true)
    		document.getElementById('dados_superior').style.display = ""
    	if(document.getElementById('dados_superior').style.display == "" && document.getElementById('ckc_orgid_sup').checked == false)	
			document.getElementById('dados_superior').style.display = "none"    	
    	
    	if(document.getElementById('dados_profissional').style.display == "none" && document.getElementById('ckc_orgid_prof').checked == true)
    		document.getElementById('dados_profissional').style.display = ""
    	if(document.getElementById('dados_profissional').style.display == "" && document.getElementById('ckc_orgid_prof').checked == false)
    		document.getElementById('dados_profissional').style.display = "none"
    	
    	exibeEscondeMarcadores(document.getElementById('ckc_orgid_sup'));
    	exibeEscondeMarcadores(document.getElementById('ckc_orgid_prof'));
    	
    	
    }
    
    function exibeEscondeMarcadores(obj){
    	
    	if(xml_antigo != ""){
    		
			if( obj.value == 1 || obj.value == 2){
			
				if(obj.value == 1){
					if(obj.checked == true){
						document.getElementById('ckc_orgid_sup_inst').checked = true;
						document.getElementById('ckc_orgid_sup_campus').checked = true;
						document.getElementById('ckc_campus_novo').checked = true;
						document.getElementById('ckc_campus_novo_funcionando').checked = true;
						document.getElementById('ckc_campus_preexistente').checked = true;
						document.getElementById('ckc_campus_previsto').checked = true;
						
						if(markerGroups[obj.value + "1"]){
							for (var i = 0; i < markerGroups[obj.value + "1"].length; i++) {
						        var marker = markerGroups[obj.value + "1"][i];
						        marker.show();
							}
						}
						if(markerGroups[obj.value + "2"]){
							for (var i = 0; i < markerGroups[obj.value + "2"].length; i++) {
						        var marker = markerGroups[obj.value + "2"][i];
						        marker.show();
							}
						}
						if(markerGroups[obj.value + "21"]){
							for (var i = 0; i < markerGroups[obj.value + "21"].length; i++) {
						        var marker = markerGroups[obj.value + "21"][i];
						        marker.show();
							}
						}
						if(markerGroups[obj.value + "22"]){
							for (var i = 0; i < markerGroups[obj.value + "22"].length; i++) {
						        var marker = markerGroups[obj.value + "22"][i];
						        marker.show();
							}
						}
						if(markerGroups[obj.value + "23"]){
							for (var i = 0; i < markerGroups[obj.value + "23"].length; i++) {
						        var marker = markerGroups[obj.value + "23"][i];
						        marker.show();
							}
						}
						if(markerGroups[obj.value + "24"]){
							for (var i = 0; i < markerGroups[obj.value + "24"].length; i++) {
						        var marker = markerGroups[obj.value + "24"][i];
						        marker.show();
							}
						}
						if(markerGroups[obj.value + "212"]){
							for (var i = 0; i < markerGroups[obj.value + "212"].length; i++) {
						        var marker = markerGroups[obj.value + "212"][i];
						        marker.show();
							}
						} 
						
					}else{
						document.getElementById('ckc_orgid_sup_inst').checked = false;
						document.getElementById('ckc_orgid_sup_campus').checked = false;
						document.getElementById('ckc_campus_novo').checked = false;
						document.getElementById('ckc_campus_novo_funcionando').checked = false;
						document.getElementById('ckc_campus_preexistente').checked = false;
						document.getElementById('ckc_campus_previsto').checked = false;
						
						if(markerGroups[obj.value + "1"]){
							for (var i = 0; i < markerGroups[obj.value + "1"].length; i++) {
						        var marker = markerGroups[obj.value + "1"][i];
						        marker.hide();
							}
						}
						if(markerGroups[obj.value + "2"]){
							for (var i = 0; i < markerGroups[obj.value + "2"].length; i++) {
						        var marker = markerGroups[obj.value + "2"][i];
						        marker.hide();
							}
						}
						if(markerGroups[obj.value + "21"]){
							for (var i = 0; i < markerGroups[obj.value + "21"].length; i++) {
						        var marker = markerGroups[obj.value + "21"][i];
						        marker.hide();
							}
						}
						if(markerGroups[obj.value + "22"]){
							for (var i = 0; i < markerGroups[obj.value + "22"].length; i++) {
						        var marker = markerGroups[obj.value + "22"][i];
						        marker.hide();
							}
						}
						if(markerGroups[obj.value + "23"]){
							for (var i = 0; i < markerGroups[obj.value + "23"].length; i++) {
						        var marker = markerGroups[obj.value + "23"][i];
						        marker.hide();
							}
						}
						if(markerGroups[obj.value + "24"]){
							for (var i = 0; i < markerGroups[obj.value + "24"].length; i++) {
						        var marker = markerGroups[obj.value + "24"][i];
						        marker.hide();
							}
						}
						if(markerGroups[obj.value + "212"]){
							for (var i = 0; i < markerGroups[obj.value + "212"].length; i++) {
						        var marker = markerGroups[obj.value + "212"][i];
						        marker.hide();
							}
						}    
					}
				}
				
				if(obj.value == 2){
					if(obj.checked == true){
						document.getElementById('ckc_orgid_prof_inst').checked = true;
						document.getElementById('ckc_orgid_prof_campus').checked = true;
						document.getElementById('ckc_campus_novo2').checked = true;
						document.getElementById('ckc_campus_novo2_funcionando').checked = true;
						document.getElementById('ckc_campus_preexistente2').checked = true;
						document.getElementById('ckc_campus_previsto2').checked = true;
						
						if(markerGroups[obj.value + "1"]){
							for (var i = 0; i < markerGroups[obj.value + "1"].length; i++) {
						        var marker = markerGroups[obj.value + "1"][i];
						        marker.show();
							}
						}
						if(markerGroups[obj.value + "2"]){
							for (var i = 0; i < markerGroups[obj.value + "2"].length; i++) {
						        var marker = markerGroups[obj.value + "2"][i];
						        marker.show();
							}
						}
						if(markerGroups[obj.value + "21"]){
							for (var i = 0; i < markerGroups[obj.value + "21"].length; i++) {
						        var marker = markerGroups[obj.value + "21"][i];
						        marker.show();
							}
						}
						if(markerGroups[obj.value + "22"]){
							for (var i = 0; i < markerGroups[obj.value + "22"].length; i++) {
						        var marker = markerGroups[obj.value + "22"][i];
						        marker.show();
							}
						}
						if(markerGroups[obj.value + "23"]){
							for (var i = 0; i < markerGroups[obj.value + "23"].length; i++) {
						        var marker = markerGroups[obj.value + "23"][i];
						        marker.show();
							}
						}
						if(markerGroups[obj.value + "24"]){
							for (var i = 0; i < markerGroups[obj.value + "24"].length; i++) {
						        var marker = markerGroups[obj.value + "24"][i];
						        marker.show();
							}
						}
						if(markerGroups[obj.value + "212"]){
							for (var i = 0; i < markerGroups[obj.value + "212"].length; i++) {
						        var marker = markerGroups[obj.value + "212"][i];
						        marker.show();
							}
						}
						
					}else{
						document.getElementById('ckc_orgid_prof_inst').checked = false;
						document.getElementById('ckc_orgid_prof_campus').checked = false;
						document.getElementById('ckc_campus_novo2').checked = false;
						document.getElementById('ckc_campus_novo2_funcionando').checked = false;
						document.getElementById('ckc_campus_preexistente2').checked = false;
						document.getElementById('ckc_campus_previsto2').checked = false;
						
						if(markerGroups[obj.value + "1"]){
							for (var i = 0; i < markerGroups[obj.value + "1"].length; i++) {
						        var marker = markerGroups[obj.value + "1"][i];
						        marker.hide();
							}
						}
						if(markerGroups[obj.value + "2"]){
							for (var i = 0; i < markerGroups[obj.value + "2"].length; i++) {
						        var marker = markerGroups[obj.value + "2"][i];
						        marker.hide();
							}
						}
						if(markerGroups[obj.value + "21"]){
							for (var i = 0; i < markerGroups[obj.value + "21"].length; i++) {
						        var marker = markerGroups[obj.value + "21"][i];
						        marker.hide();
							}
						}
						if(markerGroups[obj.value + "22"]){
							for (var i = 0; i < markerGroups[obj.value + "22"].length; i++) {
						        var marker = markerGroups[obj.value + "22"][i];
						        marker.hide();
							}
						}
						if(markerGroups[obj.value + "23"]){
							for (var i = 0; i < markerGroups[obj.value + "23"].length; i++) {
						        var marker = markerGroups[obj.value + "23"][i];
						        marker.hide();
							}
						}
						if(markerGroups[obj.value + "24"]){
							for (var i = 0; i < markerGroups[obj.value + "24"].length; i++) {
						        var marker = markerGroups[obj.value + "24"][i];
						        marker.hide();
							}
						}
						if(markerGroups[obj.value + "212"]){
							for (var i = 0; i < markerGroups[obj.value + "212"].length; i++) {
						        var marker = markerGroups[obj.value + "212"][i];
						        marker.hide();
							}
						}
					}
				}
			
			}if( obj.value == 12 || obj.value == 22){
			
				if(obj.value == 22){
				
					if(obj.checked == true){
					
						document.getElementById('ckc_campus_novo2').checked = true;
						document.getElementById('ckc_campus_novo2_funcionando').checked = true;
						document.getElementById('ckc_campus_preexistente2').checked = true;
						document.getElementById('ckc_campus_previsto2').checked = true;
						if(markerGroups[obj.value]){
							for (var i = 0; i < markerGroups[obj.value].length; i++) {
					        	var marker = markerGroups[obj.value][i];
					        	marker.show();
					        }
					    }
					    if(markerGroups[obj.value + "1"]){
					        for (var i = 0; i < markerGroups[obj.value + "1"].length; i++) {
						        var marker = markerGroups[obj.value + "1"][i];
						        marker.show();
							}
						}
						if(markerGroups[obj.value + "2"]){
							for (var i = 0; i < markerGroups[obj.value + "2"].length; i++) {
						        var marker = markerGroups[obj.value + "2"][i];
						        marker.show();
							}
						}
						if(markerGroups[obj.value + "3"]){
							for (var i = 0; i < markerGroups[obj.value + "3"].length; i++) {
						        var marker = markerGroups[obj.value + "3"][i];
						        marker.show();
							}
						}
						if(markerGroups[obj.value + "4"]){
							for (var i = 0; i < markerGroups[obj.value + "4"].length; i++) {
						        var marker = markerGroups[obj.value + "4"][i];
						        marker.show();
							}
						}
						if(markerGroups[obj.value + "12"]){
							for (var i = 0; i < markerGroups[obj.value + "12"].length; i++) {
						        var marker = markerGroups[obj.value + "12"][i];
						        marker.show();
							}
						}
					
					}else{
						
						document.getElementById('ckc_campus_novo2').checked = false;
						document.getElementById('ckc_campus_novo2_funcionando').checked = false;
						document.getElementById('ckc_campus_preexistente2').checked = false;
						document.getElementById('ckc_campus_previsto2').checked = false;
						
						if(markerGroups[obj.value]){
							for (var i = 0; i < markerGroups[obj.value].length; i++) {
					        	var marker = markerGroups[obj.value][i];
					        	marker.hide();
					        }
					    }
					    if(markerGroups[obj.value + "1"]){
					        for (var i = 0; i < markerGroups[obj.value + "1"].length; i++) {
						        var marker = markerGroups[obj.value + "1"][i];
						        marker.hide();
							}
						}
						if(markerGroups[obj.value + "2"]){
							for (var i = 0; i < markerGroups[obj.value + "2"].length; i++) {
						        var marker = markerGroups[obj.value + "2"][i];
						        marker.hide();
							}
						}
						if(markerGroups[obj.value + "3"]){
							for (var i = 0; i < markerGroups[obj.value + "3"].length; i++) {
						        var marker = markerGroups[obj.value + "3"][i];
						        marker.hide();
							}
						}
						if(markerGroups[obj.value + "4"]){
							for (var i = 0; i < markerGroups[obj.value + "4"].length; i++) {
						        var marker = markerGroups[obj.value + "4"][i];
						        marker.hide();
							}
						}
						if(markerGroups[obj.value + "12"]){
							for (var i = 0; i < markerGroups[obj.value + "12"].length; i++) {
						        var marker = markerGroups[obj.value + "12"][i];
						        marker.hide();
							}
						}
						
					}
				
				}else{
				
					if(obj.checked == true){
					
						document.getElementById('ckc_campus_novo').checked = true;
						document.getElementById('ckc_campus_novo_funcionando').checked = true;
						document.getElementById('ckc_campus_preexistente').checked = true;
						document.getElementById('ckc_campus_previsto').checked = true;
						if(markerGroups[obj.value]){
							for (var i = 0; i < markerGroups[obj.value].length; i++) {
					        	var marker = markerGroups[obj.value][i];
					        	marker.show();
					        }
					    }
					    if(markerGroups[obj.value + "1"]){
					        for (var i = 0; i < markerGroups[obj.value + "1"].length; i++) {
						        var marker = markerGroups[obj.value + "1"][i];
						        marker.show();
							}
						}
						if(markerGroups[obj.value + "2"]){
							for (var i = 0; i < markerGroups[obj.value + "2"].length; i++) {
						        var marker = markerGroups[obj.value + "2"][i];
						        marker.show();
							}
						}
						if(markerGroups[obj.value + "3"]){
							for (var i = 0; i < markerGroups[obj.value + "3"].length; i++) {
						        var marker = markerGroups[obj.value + "3"][i];
						        marker.show();
							}
						}
						if(markerGroups[obj.value + "4"]){
							for (var i = 0; i < markerGroups[obj.value + "4"].length; i++) {
						        var marker = markerGroups[obj.value + "4"][i];
						        marker.show();
							}
						}
						if(markerGroups[obj.value + "12"]){
							for (var i = 0; i < markerGroups[obj.value + "12"].length; i++) {
						        var marker = markerGroups[obj.value + "12"][i];
						        marker.show();
							}
						}
					
					}else{
					
						document.getElementById('ckc_campus_novo').checked = false;
						document.getElementById('ckc_campus_novo_funcionando').checked = false;
						document.getElementById('ckc_campus_preexistente').checked = false;
						document.getElementById('ckc_campus_previsto').checked = false;
						if(markerGroups[obj.value]){
							for (var i = 0; i < markerGroups[obj.value].length; i++) {
					        	var marker = markerGroups[obj.value][i];
					        	marker.hide();
					        }
					    }
					    if(markerGroups[obj.value + "1"]){
					        for (var i = 0; i < markerGroups[obj.value + "1"].length; i++) {
						        var marker = markerGroups[obj.value + "1"][i];
						        marker.hide();
							}
						}
						if(markerGroups[obj.value + "2"]){
							for (var i = 0; i < markerGroups[obj.value + "2"].length; i++) {
						        var marker = markerGroups[obj.value + "2"][i];
						        marker.hide();
							}
						}
						if(markerGroups[obj.value + "3"]){
							for (var i = 0; i < markerGroups[obj.value + "3"].length; i++) {
						        var marker = markerGroups[obj.value + "3"][i];
						        marker.hide();
							}
						}
						if(markerGroups[obj.value + "4"]){
							for (var i = 0; i < markerGroups[obj.value + "4"].length; i++) {
						        var marker = markerGroups[obj.value + "4"][i];
						        marker.hide();
							}
						}
						if(markerGroups[obj.value + "12"]){
							for (var i = 0; i < markerGroups[obj.value + "12"].length; i++) {
						        var marker = markerGroups[obj.value + "12"][i];
						        marker.hide();
							}
						}
						
					}
				
				}
				
			
			}else{
			
				if(obj.checked == true){
					if(markerGroups[obj.value]){
						for (var i = 0; i < markerGroups[obj.value].length; i++) {
				        	var marker = markerGroups[obj.value][i];
				        	marker.show();
				        }
				    }
				
				}else{
					if(markerGroups[obj.value]){
						for (var i = 0; i < markerGroups[obj.value].length; i++) {
				        	var marker = markerGroups[obj.value][i];
				        	marker.hide();
				        }
				    }
					
				}
				
			}
    	
    	}
    }
    
    function centralizaBrasil(){
    	
    	var zoom = 4;	var lat_i = -14.689881; var lng_i = -52.373047;	//Brasil	
		map.setCenter(new GLatLng(lat_i,lng_i), parseInt(zoom)); //Centraliza e aplica o zoom
    	
    }
</script>

<table style="border-bottom:0px"  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td id="td_filtros" valign="top" width="18%" >
	
		<table style="border-bottom:0px"  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td class="SubTituloEsquerda" >Busca</td>
			</tr>
			<tr>
				<td bgcolor="#FFFFFF"  >
					<?=campo_texto("inp_busca","N","S","Busca","30","120","","","","","","id='inp_busca'","buscaMapaEnter(event)");?>
					<input type="button" id="btn_busca_textual" value="ok" onclick="buscaMapa(true)"  />
				</td>
			</tr>
			<tr>
				<td  class="SubTituloEsquerda" >Tipo de Ensino</td>
			</tr>
			<tr>
				<td bgcolor="#FFFFFF" >
					<img src="/imagens/icone_capacete_1.png"> <input onclick="exibeEscondeMarcadores(this)" type="checkbox" name="orgid[1]" checked="checked" title="Ensino Superior" id="ckc_orgid_sup" value="1"  />Educação Superior <br />
					<div id="dados_superior" style="padding-left:20px;display:none" >
						<img src="/imagens/icone_capacete_1.png"> <input onclick="exibeEscondeMarcadores(this)" type="checkbox" name="orgid[11]" checked="checked" title="Instituições Ensino Superior" id="ckc_orgid_sup_inst" value="11"  />Instituições<br />
						<input style="margin-left:16px" onclick="exibeEscondeMarcadores(this)" type="checkbox" name="orgid[12]" checked="checked" title="Campus Ensino Superior" id="ckc_orgid_sup_campus" value="12"  />Campus
						<div style="padding-left:20px" >
							<img src="/imagens/icones/lbr.png"> <input onclick="exibeEscondeMarcadores(this)" type="checkbox" name="orgid[1212]" checked="checked" title="Campus - Novo em Funcionamento" id="ckc_campus_novo_funcionando" value="1212"  />Novo Funcionando<br />
							<img src="/imagens/icones/lr.png"> <input onclick="exibeEscondeMarcadores(this)" type="checkbox" name="orgid[121]" checked="checked" title="Campus - Novo" id="ckc_campus_novo" value="121"  />Novo<br />
							<img src="/imagens/icones/lp.png"> <input onclick="exibeEscondeMarcadores(this)" type="checkbox" name="orgid[122]" checked="checked" title="Campus - Preexistente" id="ckc_campus_preexistente" value="122"  />Preexistente<br />
							<img src="/imagens/icones/ly.png"> <input onclick="exibeEscondeMarcadores(this)" type="checkbox" name="orgid[123]" checked="checked" title="Campus - Previsto" id="ckc_campus_previsto" value="123"  />Previsto<br />
							<img src="/imagens/icones/lb.png"> <input onclick="exibeEscondeMarcadores(this)" type="checkbox" name="orgid[124]" checked="checked" title="Campus - Criadas em 2003/2010" id="ckc_campus_criadas_2003_2010" value="124"  />Criadas em 2003/2010
					</div>
					</div>
					<img src="/imagens/icone_capacete_2.png"> <input onclick="exibeEscondeMarcadores(this)" type="checkbox" name="orgid[2]" id="ckc_orgid_prof" title="Ensino Profissional" value="2"  />Educação Profissional
					<div id="dados_profissional" style="padding-left:20px;display:none" >
						<img src="/imagens/icone_capacete_2.png"> <input onclick="exibeEscondeMarcadores(this)" type="checkbox" name="orgid[21]" checked="checked" title="Instituições Ensino Profissional" id="ckc_orgid_prof_inst" value="21"  />Instituições<br />
						<input style="margin-left:16px" onclick="exibeEscondeMarcadores(this)" type="checkbox" name="orgid[22]" checked="checked" title="Campus Ensino Profissionalr" id="ckc_orgid_prof_campus" value="22"  />Campus
						<div style="padding-left:20px" >
							<img src="/imagens/icones/tbr.png"> <input onclick="exibeEscondeMarcadores(this)" type="checkbox" name="orgid[221]" checked="checked" title="Campus - Novo em Funcionamento" id="ckc_campus_novo2_funcionando" value="2212"  />Novo Funcionando<br />
							<img src="/imagens/icones/tr.png"> <input onclick="exibeEscondeMarcadores(this)" type="checkbox" name="orgid[221]" checked="checked" title="Campus - Novo" id="ckc_campus_novo2" value="221"  />Novo<br />
							<img src="/imagens/icones/tp.png"> <input onclick="exibeEscondeMarcadores(this)" type="checkbox" name="orgid[222]" checked="checked" title="Campus - Preexistente" id="ckc_campus_preexistente2" value="222"  />Preexistente<br />
							<img src="/imagens/icones/ty.png"> <input onclick="exibeEscondeMarcadores(this)" type="checkbox" name="orgid[223]" checked="checked" title="Campus - Previsto" id="ckc_campus_previsto2" value="223"  />Previsto<br />
							<img src="/imagens/icones/tb.png"> <input onclick="exibeEscondeMarcadores(this)" type="checkbox" name="orgid[224]" checked="checked" title="Campus - Criadas em 2003/2010" id="ckc_campus_criadas_2003_2010" value="224"  />Criadas em 2003/2010
						</div>
					</div>
				</td>
			</tr>
			<tr>
				<td class="SubTituloEsquerda" ><img style="cursor:pointer" onclick="exibeUF(this)" src="/imagens/mais.gif"> UF</td>
			</tr>
			<tr>
				<td  bgcolor="#FFFFFF" style="display:none" id="td_uf" >
					<?php 
					$sql = " SELECT	estuf AS codigo,
										estdescricao AS descricao
									FROM 
										territorios.estado
									ORDER BY
										estdescricao ";
	
					combo_popup( 'inp_estuf', $sql, 'Selecione as Unidades Federativas', '400x400', 0, array(), '', 'S', false, false, 5, 210, '', '' );
					?>
					<input type="hidden" name="hdn_uf" id="hdn_uf" value="0" />
				</td>
			</tr>
			<tr>
				<td class="SubTituloEsquerda" ><input type="button" id="btn_buscar" name="btn_buscar" value="Carregar" onclick="buscaMapa(false)" /></td>
			</tr>
			<tr id="carregando_mapa" style="display:none" >
				<td bgcolor="#FFFFFF" align="center" > <img style="vertical-align: middle" src="../imagens/wait.gif" /> <b>Aguarde... Carregando.</b></td>
			</tr>
		</table>
		
	</td>
	<td id="td_mapa" valign="top" >
			<div style="position:relative;z-index:1" >
			<div id="mapa" style="width:100%;height:550px" ></div>
			</div>
			<div style="position:relative;z-index:3;top:-545;left:70px;text-align:center;padding:1px;width:60px;" ><input type="button" value="Brasil"  onclick="centralizaBrasil()" /></div>
			<div id="mapa_exibe_carregando" style="position:relative;z-index:2;top:-280;left:35%;background-color:#FFFFFF;text-align:center;padding:5px;border:solid 1px black;width:300px;display:none" ><img style="vertical-align: middle" src="../imagens/wait.gif" /> <b>Aguarde... Carregando.</b></div>
	</td>
</tr>
</table>
<script>
	initialize();
</script>