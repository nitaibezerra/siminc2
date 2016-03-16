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


<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script>

	var map; //variável do Google Maps
	
	var markerGroups = { '1': [], '2': [], '3': [] , '4': [] ,'5' :[],'6' :[],'7' :[],'8' :[]};
	var marcadores     = new Array();
	var marcadoreshtml = new Array();
	
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
		
		var xml_filtro="XMLmapaInstituicoesProfissionais.php?1=1";
		var params="";
		
		if(document.getElementById('chk_3').checked) {
			xml_filtro += "&chk[]=3";
			params     += "&chk[]=3";
		}
		if(document.getElementById('chk_4').checked) {
			xml_filtro += "&chk[]=4";
			params     += "&chk[]=4";
		}
		if(document.getElementById('chk_5').checked) {
			xml_filtro += "&chk[]=5";
			params     += "&chk[]=5";
		}
		if(document.getElementById('chk_6').checked) {
			xml_filtro += "&chk[]=6";
			params     += "&chk[]=6";
		}

		
		if(orgid)
			xml_filtro += "&orgid="+orgid;
		if(busca_uf) {
			xml_filtro += "&uf="+busca_uf;
			params     += "&uf="+busca_uf;
		}
		if(busca_textual) {
			xml_filtro += "&buscaTextual="+busca_textual;
			params     += "&buscaTextual="+busca_textual;
		}
		
		document.getElementById('ept_relatorio_div').innerHTML = 'Carregando...';
		
		if(xml_antigo != xml_filtro) {
			marcaInstituicoes(xml_filtro);
		}
		
		selectAllOptions( document.getElementById( 'agrupador' ) );
		params = params+'&'+$('#agrupador').serialize();
		$.ajax({
	   		type: "POST",
	   		url: "academico.php?modulo=principal/mapaInstituicoesProfissionais&acao=A",
	   		data: "relatorio_obras=1"+params,
	   		async: false,
	   		success: function(msg){
	   			document.getElementById('ept_relatorio').style.display = '';
	   			extrairScript(msg);
	   			document.getElementById('ept_relatorio_div').innerHTML = msg;
	   		}
	 		});

			
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
	
	function exibeAgrup(obj){
		
		if(obj.src.search("mais.gif") > 0 ){
			obj.src="/imagens/menos.gif";
			document.getElementById('td_agrup').style.display='';
			document.getElementById('hdn_agrup').value = 1;
		}else{
			obj.src="/imagens/mais.gif";
			document.getElementById('td_agrup').style.display='none';
			document.getElementById('hdn_agrup').value = 0;
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

		var ens_prof = document.getElementById('ckc_orgid_prof');
		
		if(ens_prof.checked == false){
			alert('Favor informar o Tipo de Ensino!');
			return false;
		}else{
			return ens_prof.value;
		}
				
	}
	
	function abrebalao(rfeid) {
		marcadores[rfeid].openInfoWindowHtml(marcadoreshtml[rfeid]);
	}
	
	function marcaInstituicoes(url){
		
		xml_antigo = url;
		
		exibeCarregando();
		
		map.clearOverlays();
		
		// Criando o ícone 1 do mapa
		var baseIcon1 = new GIcon();
		baseIcon1.iconSize = new GSize(15, 17);
		baseIcon1.iconAnchor = new GPoint(11, 13);
		baseIcon1.infoWindowAnchor = new GPoint(9, 2);
        baseIcon1.image='/imagens/icones/lp.png';
        
        // Criando o ícone 1 do campus
		var baseIcon2 = new GIcon();
		baseIcon2.iconSize = new GSize(15, 17);
		baseIcon2.iconAnchor = new GPoint(11, 13);
		baseIcon2.infoWindowAnchor = new GPoint(9, 2);
        baseIcon2.image='/imagens/icones/lr.png';
        
        // Criando o ícone 1 do campus
		var baseIcon3 = new GIcon();
		baseIcon3.iconSize = new GSize(9, 14);
		baseIcon3.iconAnchor = new GPoint(9, 14);
		baseIcon3.infoWindowAnchor = new GPoint(9, 2);
        baseIcon3.image='/imagens/icone_capacete_3.png';
        
        // Criando o ícone 1 do campus
		var baseIcon4 = new GIcon();
		baseIcon4.iconSize = new GSize(9, 14);
		baseIcon4.iconAnchor = new GPoint(9, 14);
		baseIcon4.infoWindowAnchor = new GPoint(9, 2);
        baseIcon4.image='/imagens/icone_capacete_1.png';
        
        // Criando o ícone 1 do campus
		var baseIcon5 = new GIcon();
		baseIcon5.iconSize = new GSize(9, 14);
		baseIcon5.iconAnchor = new GPoint(9, 14);
		baseIcon5.infoWindowAnchor = new GPoint(9, 2);
        baseIcon5.image='/imagens/icone_capacete_2.png';
        
        // Criando o ícone 1 do campus
		var baseIcon6 = new GIcon();
		baseIcon6.iconSize = new GSize(9, 14);
		baseIcon6.iconAnchor = new GPoint(9, 14);
		baseIcon6.infoWindowAnchor = new GPoint(9, 2);
        baseIcon6.image='/imagens/icone_capacete_4.png';
        
        // Criando o ícone 1 do campus
		var baseIcon7 = new GIcon();
		baseIcon7.iconSize = new GSize(15, 17);
		baseIcon7.iconAnchor = new GPoint(11, 13);
		baseIcon7.infoWindowAnchor = new GPoint(9, 2);
        baseIcon7.image='/imagens/icones/qb.png';

        // Criando o ícone 1 do campus
		var baseIcon8 = new GIcon();
		baseIcon8.iconSize = new GSize(15, 17);
		baseIcon8.iconAnchor = new GPoint(11, 13);
		baseIcon8.infoWindowAnchor = new GPoint(9, 2);
        baseIcon8.image='/imagens/icones/qr.png';
         
		xml_filtro = url;
		
		// Criando os Marcadores com o resultado
			GDownloadUrl(xml_filtro, function(data) {
				var xml = GXml.parse(data);
				
				var markers = xml.documentElement.getElementsByTagName("marker");
				
				
				if(markers.length > 0) {
					var lat_ant=0;
					var lng_ant=0;
				
					for (var i = 0; i < markers.length; i++) {
						
						var nome = markers[i].getAttribute("nome");
						var entid = markers[i].getAttribute("entid");
						var rfeid = markers[i].getAttribute("rfeid");
						var mundsc = markers[i].getAttribute("mundsc");
						var estuf = markers[i].getAttribute("estuf");
						var rfecaracteristica = markers[i].getAttribute("rfecaracteristica");
						title = nome + " - " + mundsc + " / " + estuf + "."; 
						var tipo = markers[i].getAttribute("tipo");
						var orgid = "2";
						var balao = "campus";
						
						switch(tipo) {
							case '1':
								icon = baseIcon1;
								break;
							case '2':
								icon = baseIcon2;
								break;
							case '3':
								icon = baseIcon3;
								break;
							case '4':
								icon = baseIcon4;
								break;
							case '5':
								icon = baseIcon5;
								break;
							case '6':
								icon = baseIcon6;
								break;
							case '7':
								icon = baseIcon7;
								break;
							case '8':
								icon = baseIcon8;
								break;
						}
												
						var lat = markers[i].getAttribute("lat");
						var lng = markers[i].getAttribute("lng");
						
						var html;
						
						if(entid) {
						html = "<div style=\"font-family:verdana;font-size:11px;\" >";
						html += "<b>Campus:</b> " + nome + "<br />";
						html += "<b>Localização:</b> " + mundsc + "/" + estuf + "<br /><br />";
						html += "<span style=\"cursor:pointer;font-weight:bold\">Mais detalhes...</span>";
						html += "</div>";
						
						html = "<div style=\"padding:5px\" ><iframe src=\"academico.php?modulo=principal/mapaInstituicoesProfissionais&acao=A&montaBalao=1&entid=" + entid + "&tipo=" + balao + "&orgid=" + orgid + "\" frameborder=0 scrolling=\"auto\" height=\"380px\" width=\"480px\" ></iframe></div>";
						} else {
							html='';
							var dadoscaract = rfecaracteristica.split(";");
							for(var j=0;j<dadoscaract.length;j++) {
								html += dadoscaract[j]+"<br/>";
							}
							
						}
										
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
						
						if(rfeid) {
							marcadores[rfeid]     = marker;
							marcadoreshtml[rfeid] = html;
						}
						
						map.addOverlay(marker);
						//marker.openInfoWindowHtml(html);
				
		        }
		        
		        setTimeout("disponibilizaMapa()",5000);
		        
				} else {
					alert("Não foi possível localizar Campus com o(s) filtro(s) aplicado(s)!")
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
    	
    	exibeEscondeMarcadores(document.getElementById('ckc_orgid_prof'));
    	
    	
    }
    
    function exibeEscondeMarcadores(obj){
    	
    	if(xml_antigo != ""){
    		
			if( obj.value == 'orgid_2'){
				
				if(obj.checked == true) {
				
					//document.getElementById('chk_1').checked = true;
					//document.getElementById('chk_2').checked = true;
					//document.getElementById('chk_3').checked = true;
					//document.getElementById('chk_4').checked = true;
					//document.getElementById('chk_5').checked = true;
					//document.getElementById('chk_6').checked = true;
					//document.getElementById('chk_7').checked = true;
					//document.getElementById('chk_8').checked = true;
					
					if(markerGroups["1"]){
						for (var i = 0; i < markerGroups["1"].length; i++) {
					        var marker = markerGroups["1"][i];
					        marker.show();
						}
					}
					if(markerGroups["2"]){
						for (var i = 0; i < markerGroups["2"].length; i++) {
					        var marker = markerGroups["2"][i];
					        marker.show();
						}
					}
					if(markerGroups["3"]){
						for (var i = 0; i < markerGroups["3"].length; i++) {
					        var marker = markerGroups["3"][i];
					        marker.show();
						}
					}
					if(markerGroups["4"]){
						for (var i = 0; i < markerGroups["4"].length; i++) {
					        var marker = markerGroups["4"][i];
					        marker.show();
						}
					}
					if(markerGroups["5"]){
						for (var i = 0; i < markerGroups["5"].length; i++) {
					        var marker = markerGroups["5"][i];
					        marker.show();
						}
					}
					if(markerGroups["6"]){
						for (var i = 0; i < markerGroups["6"].length; i++) {
					        var marker = markerGroups["6"][i];
					        marker.show();
						}
					}
					if(markerGroups["7"]){
						for (var i = 0; i < markerGroups["7"].length; i++) {
					        var marker = markerGroups["7"][i];
					        marker.show();
						}
					}
					if(markerGroups["8"]){
						for (var i = 0; i < markerGroups["8"].length; i++) {
					        var marker = markerGroups["8"][i];
					        marker.show();
						}
					}
					
				} else {
				
					//document.getElementById('chk_1').checked = false;
					//document.getElementById('chk_2').checked = false;
					//document.getElementById('chk_3').checked = false;
					//document.getElementById('chk_4').checked = false;
					//document.getElementById('chk_5').checked = false;
					//document.getElementById('chk_6').checked = false;
					//document.getElementById('chk_7').checked = false;
					//document.getElementById('chk_8').checked = false;
					
					if(markerGroups["1"]){
						for (var i = 0; i < markerGroups["1"].length; i++) {
					        var marker = markerGroups["1"][i];
					        marker.hide();
						}
					}
					if(markerGroups["2"]){
						for (var i = 0; i < markerGroups["2"].length; i++) {
					        var marker = markerGroups["2"][i];
					        marker.hide();
						}
					}
					if(markerGroups["3"]){
						for (var i = 0; i < markerGroups["3"].length; i++) {
					        var marker = markerGroups["3"][i];
					        marker.hide();
						}
					}
					if(markerGroups["4"]){
						for (var i = 0; i < markerGroups["4"].length; i++) {
					        var marker = markerGroups["4"][i];
					        marker.hide();
						}
					}
					if(markerGroups["5"]){
						for (var i = 0; i < markerGroups["5"].length; i++) {
					        var marker = markerGroups["5"][i];
					        marker.hide();
						}
					}
					if(markerGroups["6"]){
						for (var i = 0; i < markerGroups["6"].length; i++) {
					        var marker = markerGroups["6"][i];
					        marker.hide();
						}
					}
					if(markerGroups["7"]){
						for (var i = 0; i < markerGroups["7"].length; i++) {
					        var marker = markerGroups["7"][i];
					        marker.hide();
						}
					}
					if(markerGroups["8"]){
						for (var i = 0; i < markerGroups["8"].length; i++) {
					        var marker = markerGroups["8"][i];
					        marker.hide();
						}
					}
				}

			
			}
			
			if( obj.value == '1' || 
				obj.value == '2' ||
				obj.value == '3' ||
				obj.value == '4' ||
				obj.value == '5' ||
				obj.value == '6' ||
				obj.value == '7' ||
				obj.value == '8') {
			
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
    
    function restaurarItens() {
    	removeAllOptions(document.getElementById('agrupador'));
    	addOption(document.getElementById('agrupador'),"Tipo","tipo",false);
    	addOption(document.getElementById('agrupador'),"Região","regiao",false);
    	addOption(document.getElementById('agrupador'),"UF","uf",false);
    	addOption(document.getElementById('agrupador'),"Município","municipio",false);
    	addOption(document.getElementById('agrupador'),"Campus","campus",false);
    }
</script>

<table style="border-bottom:0px"  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td id="td_filtros" valign="top" width="20%" >
	
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
					</div>
					<input onclick="exibeEscondeMarcadores(this)" type="checkbox" name="orgid[2]" id="ckc_orgid_prof" title="Ensino Profissional" value="orgid_2" checked  />Educação Profissional
					<div id="dados_profissional" style="padding-left:0px;" >
					<div style="padding-left:2px" >
						<!-- <img src="/imagens/icones/lp.png"> <input onclick="exibeEscondeMarcadores(this)" type="checkbox" name="chk_1" checked="checked" title="Vinculadas não registrada" id="chk_1" value="1"  /> <font size=1>Vinculadas não registrada</font><br/>  -->
						<!-- <img src="/imagens/icones/lr.png"> <input onclick="exibeEscondeMarcadores(this)" type="checkbox" name="chk_2" checked="checked" title="5 Emarcs/CEPLAC" id="chk_2" value="2"  /> <font size=1>5 Emarcs/CEPLAC</font><br/> -->
						<img src="/imagens/icone_capacete_1.png"> <input onclick="exibeEscondeMarcadores(this)" type="checkbox" name="chk_4" checked="checked" title="140 Pré existentes" id="chk_4" value="4"  /> <font size=1>1 - Câmpus Preexistentes</font><br/>
						<img src="/imagens/icone_capacete_3.png"> <input onclick="exibeEscondeMarcadores(this)" type="checkbox" name="chk_3" checked="checked" title="214 da expansão da rede EPT" id="chk_3" value="3"  /> <font size=1>2 - Criadas (2003/2010)</font><br/>
						<img src="/imagens/icone_capacete_2.png"> <input onclick="exibeEscondeMarcadores(this)" type="checkbox" name="chk_5" checked="checked" title="81 Programadas para 2011/2012" id="chk_5" value="5"  /> <font size=1>3 - Previstos (2011/2012)</font><br/>
						<img src="/imagens/icone_capacete_4.png"> <input onclick="exibeEscondeMarcadores(this)" type="checkbox" name="chk_6" checked="checked" title="120 da expansão fase III (2013/2014)" id="chk_6" value="6"  /> <font size=1>4 - Propostos (2013/2014)</font><br/>
						<!-- <img src="/imagens/icones/qb.png"> <input onclick="exibeEscondeMarcadores(this)" type="checkbox" name="chk_7" checked="checked" title="Não reconhecido" id="chk_7" value="7"  /> <font size=1>Não reconhecido</font><br/> -->
						<!-- <img src="/imagens/icones/qr.png"> <input onclick="exibeEscondeMarcadores(this)" type="checkbox" name="chk_8" checked="checked" title="Não pertence a rede EPT" id="chk_8" value="223"  /> <font size=1>Não pertence a rede EPT</font><br/> -->
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
				<td class="SubTituloEsquerda" ><img style="cursor:pointer" onclick="exibeAgrup(this)" src="/imagens/mais.gif"> Agrupadores</td>
			</tr>
			<tr>
				<td  bgcolor="#FFFFFF" style="display:none" id="td_agrup">
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
	<td valign="top" width="20%" id="ept_relatorio" style="display:none">
	<div style="overflow:auto;height:480px" id="ept_relatorio_div">
	&nbsp;
	</div>
	</td>
</tr>
</table>
<script>
	initialize();
</script>