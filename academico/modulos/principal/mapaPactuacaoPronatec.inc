<?


function ept_monta_agp_relatorio(){
	
	$agrupador = $_REQUEST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array("regiao",
										  "estado",
										  "unidadeensino",
										  "eixo",
										  "curso",
										  "nu_vagas_2011",
										  "nu_vagas_2012"),
				"agrupadorDetalhamento" => array(
													array(
															"campo" => "regiao",
															"label" => "Região"
														  ),
													array(
															"campo" => "estado",
															"label" => "UF"
														  ),
													array(
															"campo" => "unidadeensino",
															"label" => "Ensino Unidade"
														  ),
													array(
															"campo" => "eixo",
															"label" => "Eixo"
														  ),
													array(
															"campo" => "municipio",
															"label" => "Município"
														  ),
													array(
															"campo" => "curso",
															"label" => "Curso"
														  ),
													array(
															"campo" => "tipocurso",
															"label" => "Tipo Curso"
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
			case "estado":
				array_push($agp['agrupador'], array(
													"campo" => "estado",
											  		"label" => "UF")										
									   				);
			break;

			case "unidadeensino":
				array_push($agp['agrupador'], array(
													"campo" => "unidadeensino",
											  		"label" => "Unidade Ensino")										
									   				);
			break;
			case "rede":
				array_push($agp['agrupador'], array(
													"campo" => "rede",
											  		"label" => "Rede")										
									   				);
			break;
			
			case "eixo":
				array_push($agp['agrupador'], array(
													"campo" => "eixo",
											  		"label" => "Eixo")										
									   				);
			break;
			case "tipocurso":
				array_push($agp['agrupador'], array(
													"campo" => "tipocurso",
											  		"label" => "Tipo Curso")										
									   				);
			break;
			
			case "curso":
				array_push($agp['agrupador'], array(
													"campo" => "curso",
											  		"label" => "Curso")										
									   				);
			break;
			
		}	
	}
	
	return $agp;
	
}

if($_REQUEST['relatorio_superior']) {
	
	ini_set("memory_limit", "1024M");
	set_time_limit(0);
	
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	
	if($_REQUEST['chk_superior']) {
		$filtro[] = "\"Tipo\" IN ('".implode("','",$_REQUEST['chk_superior'])."') ";
	}
	if( $_REQUEST['buscaTextual'] ){
		$filtro[] = "UPPER(m.no_unidade_ensino) like UPPER('%".$_REQUEST['buscaTextual']."%')";
	}
	if( $_REQUEST['estuf'][0] ){
		$filtro[] = "trim(est.estuf) in ('".implode("','",$_REQUEST['estuf'])."') ";
	}
	if( $_REQUEST['muncod'][0] ){
		$filtro[] = "mun.muncod in ('".implode("','",$_REQUEST['muncod'])."') ";
	}
	
	$sql = "SELECT 
			CASE WHEN m.ds_eixo_tecnologico='' THEN 'Em branco' ELSE m.ds_eixo_tecnologico END as eixo,
			mun.estuf as estado,
			mun.mundescricao as municipio,
			CASE WHEN m.no_unidade_ensino='' THEN 'Em branco' ELSE m.no_unidade_ensino END as unidadeensino,
			CASE WHEN m.no_curso='' THEN 'Em branco' ELSE m.no_curso END as curso,
			CASE WHEN m.ds_tipo_curso='' THEN 'Em branco' ELSE m.ds_tipo_curso END as tipocurso,
			reg.regdescricao as regiao,
			CASE WHEN trim(\"Tipo\")='1' THEN 'Rede Federal'
				 WHEN trim(\"Tipo\")='2' THEN 'SENAC' 
				 WHEN trim(\"Tipo\")='3' THEN 'SENAI' END as rede,
			COALESCE(nu_vagas_2011::numeric,0) as nu_vagas_2011,
			COALESCE(nu_vagas_2012::numeric,0) as nu_vagas_2012
			FROM academico.relatorio_mapa_pactuacao m 
			LEFT JOIN territorios.municipio mun ON mun.muncod = trim(m.co_municipio) 
			LEFT JOIN territorios.estado est ON est.estuf = mun.estuf 
			LEFT JOIN territorios.regiao reg ON reg.regcod = est.regcod 
			".(($filtro)?"WHERE ".implode(" AND ",$filtro):"");
	
	$agrupador = ept_monta_agp_relatorio();
	$dados = $db->carregar( $sql );
	
	$coluna = array();
	
	array_push( $coluna, array("campo" 	  => "nu_vagas_2011",
					   		   "label" 	  => "2011",
					   		   "type"	  => "numeric") );

	array_push( $coluna, array("campo" 	  => "nu_vagas_2012",
					   		   "label" 	  => "2012",
					   		   "type"	  => "numeric") );
	
	$rel = new montaRelatorio();
	$rel->setTolizadorLinha(true);
	$rel->setColuna($coluna);
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

var baseIcon_superior_1 = new google.maps.MarkerImage('/imagens/icone_capacete_1.png', new google.maps.Size(9, 14));
var baseIcon_superior_2 = new google.maps.MarkerImage('/imagens/icone_capacete_2.png', new google.maps.Size(9, 14));
var baseIcon_superior_3 = new google.maps.MarkerImage('/imagens/icone_capacete_3.png', new google.maps.Size(9, 14));

var markers_superior_1  = new Array();
var markers_superior_2  = new Array();
var markers_superior_3  = new Array();

var infowindow      = '';
var htmlInfo        = '';
var filtro_antigo   = '';

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
      mapTypeId: google.maps.MapTypeId.TERRAIN
    };
    
    map = new google.maps.Map(document.getElementById("map_canvas"),
        myOptions);
    
}

/*
 * Centraliza o mapa em determianadas coordenadas e zoom 
 */
function centraliza(zoom, coords) {
  	zoom = parseInt(zoom);
  	map.setZoom(zoom);
	map.setCenter(new google.maps.LatLng(coords[1], coords[0])); //Centraliza e aplica o zoom
}



function centralizaBrasil(){

  	map.setZoom(4);
	map.setCenter(new google.maps.LatLng(-14.689881, -52.373047)); //Centraliza e aplica o zoom
    	
}

/*
 * Função que cria o HTML exibido ao clicar em algum ponto 
 */
function gerarHtmlPonto(marker) {
	var html=marker.attr("nome");
	return html;
}

/*
 * Define qual o icone o ponto vai utilizar 
 */
function retornarIcone(tipo) {

	var icon;
		
	switch(tipo) {
		case '1':
			icon = baseIcon_superior_1;
			break;
		case '2':
			icon = baseIcon_superior_2;
			break;
		case '3':
			icon = baseIcon_superior_3;
			break;
	}
	
	return icon;

}

function armazenarPonto(tipo, ponto) {

	switch(tipo) {
		case '1':
			markers_superior_1.push(ponto);
			break;
		case '2':
			markers_superior_2.push(ponto);
			break;
		case '3':
			markers_superior_3.push(ponto);
			break;
	}

}


function marcarPontos(xml_filtro) {

	jQuery.ajax({
   		type: "POST",
   		url: xml_filtro,
   		async: false,
   		success: function(data) {
   		
		      jQuery(data).find("marker").each(function() {
		      
		        var marker = jQuery(this);
		        var latlng = new google.maps.LatLng(parseFloat(marker.attr("lat")),
		                                    		parseFloat(marker.attr("lng")));
		                                    		
		        var html = gerarHtmlPonto(marker);
				
				var icon = retornarIcone(marker.attr("tipo"));
				
			    var ponto = new google.maps.Marker({
			        position: latlng,
			        map: map,
			        icon: icon,
			        shape: shape,
			        zIndex: 1
			    });
			    
			    armazenarPonto(marker.attr("tipo"), ponto);
			    
			    google.maps.event.addListener(ponto, "click", function() {if (infowindow) infowindow.close(); infowindow = new google.maps.InfoWindow({content: html}); infowindow.open(map, ponto); });

		     });
   		}
 		});

}

function carregarFiltros() {

	var filtros=''; 
	
	if(document.getElementById('texto_busca').value != "") {
		filtros += "&texto_busca="+document.getElementById('texto_busca').value;
	}
	
	if(document.getElementById('chk_superior_1').checked) { filtros += "&chk_superior[]=1"; }
	if(document.getElementById('chk_superior_2').checked) { filtros += "&chk_superior[]=2"; }
	if(document.getElementById('chk_superior_3').checked) { filtros += "&chk_superior[]=3"; }
	
	selectAllOptions( document.getElementById( 'estuf' ) );
	selectAllOptions( document.getElementById( 'muncod' ) );		
	selectAllOptions( document.getElementById( 'agrupador' ) );

	filtros += '&'+$('#estuf').serialize();
	filtros += '&'+$('#muncod').serialize();
	filtros += '&'+$('#agrupador').serialize();
	
	return filtros;

}

function preencherRelatorio(filtros) {

	var relat = "relatorio_superior=1"; 

	$.ajax({
 		type: "POST",
   		url: "academico.php?modulo=principal/mapaPactuacaoPronatec&acao=A",
   		data: relat+filtros,
   		async: false,
   		success: function(msg){
   			abreAcordion(1);
   			extrairScript(msg);
   			document.getElementById('colunainfo').innerHTML = msg;
   		}
 		});
 		
}

function apagarPontos() {
	deleteOverlays(markers_superior_1);
	deleteOverlays(markers_superior_2);
	deleteOverlays(markers_superior_3);
}

function carregarPontos() {

	divCarregando();
	
	document.getElementById('colunainfo').innerHTML = 'Carregando...';

/*	
	var linha_uf = document.getElementById('linha_uf');
	for(var i=0;i<linha_uf.cells.length;i++) {
		linha_uf.cells[i].style.backgroundColor='';
	}
*/
	
	var filtros = carregarFiltros();
	
	apagarPontos();
	marcarPontos("XMLmapaPactuacaoPronatec.php?1=1"+filtros);
	
	preencherRelatorio(filtros);
		
	filtro_antigo = filtros;
	
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
    addOption(document.getElementById('agrupador'),"Região","regiao",false);
    addOption(document.getElementById('agrupador'),"UF","estado",false);
    addOption(document.getElementById('agrupador'),"Município","municipio",false);
    addOption(document.getElementById('agrupador'),"Rede","rede",false);
    addOption(document.getElementById('agrupador'),"Unidade Ensino","unidadeensino",false);
    addOption(document.getElementById('agrupador'),"Tipo Curso","tipocurso",false);
    addOption(document.getElementById('agrupador'),"Curso","curso",false);
}


function abreAcordion(id) {
	$("#accordion").accordion( "activate" , id );
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

.contextmenu{
  visibility:hidden;
  background:#ffffff;
  border:1px solid #8888FF;
  z-index: 10;
  position: relative;
  width: 140px;
  cursor:pointer;
}
.contextmenu div{
padding-left: 5px
}

</style>


<?php 
	include  APPRAIZ."includes/cabecalho.inc";
	echo '<br>';
	
	$titulo = "SIMEC Maps";
	monta_titulo( $titulo, '' );

?>
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
						<input type="text" id=texto_busca name=texto_busca size=25> 
					</td>
				</tr>
				<tr>
					<td class="SubTituloEsquerda">Marcadores</td>
				</tr>
				<tr>
					<td>
					<img src="/imagens/icone_capacete_1.png"> <input onclick="if(this.checked){showOverlays(markers_superior_1);}else{clearOverlays(markers_superior_1);}" type="checkbox" name="chk_superior[]" checked="checked" title="Câmpus Preexistentes"    id="chk_superior_1" value="1"  /> <font size=1>1 - Rede Federal</font><br/>
					<img src="/imagens/icone_capacete_2.png"> <input onclick="if(this.checked){showOverlays(markers_superior_2);}else{clearOverlays(markers_superior_2);}" type="checkbox" name="chk_superior[]" checked="checked" title="Criadas em 2003/2010"    id="chk_superior_2" value="2"  /> <font size=1>2 - SENAC</font><br/>
					<img src="/imagens/icone_capacete_3.png"> <input onclick="if(this.checked){showOverlays(markers_superior_3);}else{clearOverlays(markers_superior_3);}" type="checkbox" name="chk_superior[]" checked="checked" title="Câmpus Novos"            id="chk_superior_3" value="3"  /> <font size=1>3 - SENAI</font><br/>
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
						<option value="regiao">Região</option>
						<option value="estado">UF</option>
						<option value="municipio">Município</option>
						<option value="rede">Rede</option>
						<option value="unidadeensino">Unidade Ensino</option>
						<option value="eixo">Eixo</option>
						<option value="tipocurso">Tipo Curso</option>
						<option value="curso">Curso</option>

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
					<a class="tituloAcordion" href="#">RELATÓRIO QUANTITATIVO<hr></a>
					<div>
						<div style="overflow:auto;height:398px" id="colunainfo"></div>
					</div>
			</div>
		</td>
		<td valign="top">
		<div style="position:absolute;z-index:3;text-align:center;padding:1px;width:60px;margin-left:80px;margin-top:10px;"><input type="button" value="Brasil"  onclick="centralizaBrasil()" /></div>
		<div id="map_canvas" style="width:100%;height:600px;position:relative;"></div>
		</td>
	
		
	</tr>
</table>
<script>
	function BuscaMunicipioEnter2(e)
	{
	    if (e.keyCode == 13)
	    {
	        BuscaMunicipio2();
	    }
	}

	function BuscaMunicipio2(){
		var mun = document.getElementById('busca_municipio');

		if(!mun.value){
			alert('Digite o Município para busca.');
			return false;
		}

		if(mun.value){
			document.getElementById('resultado_pesquisa').innerHTML = "<center>Carregando...</center>";
			$.ajax({
				type: "POST",
				url: 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/painel/_funcoes_mapa_painel_controle.php?redefederal=1',
				data: 'BuscaMunicipioAjax=' + mun.value,
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
		if(!arrMuncod[muncod]){
			carregaMunicipio(muncod);
		}
		abreAcordion(1);
		nomePoli[arrMuncod[muncod]+muncod].setOptions( {fillColor: '#F0F'} );
	}
	
	initialize();
</script>