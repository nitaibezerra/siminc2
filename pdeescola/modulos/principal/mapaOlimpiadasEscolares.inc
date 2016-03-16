<?php

if ( isset( $_REQUEST['pesquisa'] ) == true){
	
	include APPRAIZ."pdeescola/modulos/relatorio/relOlimpiadasEscolares_result.inc";
	die;	
}

if($_REQUEST['verInformacoesEscolas']) :

	$sql = "SELECT				
				(ST_Distance_Sphere(ST_Transform(eng.point,4291), ST_Transform(atl.estlatlong,4291))/1000) as distancia,
				atl.estid,
				atl.estnome as instituicao,
				ent.entcodent, 
				ent.entnome, 
				ede.estuf, 
				ede.endbai, 
				ede.endlog, 
				ede.endnum, 
				mun.mundescricao, 
				eng.entid, 
				eng.latitudedecimal as lat, 
				eng.longitudedecimal as lng,
				eng.entid,
				(select tesdesc from entidade.tipoestrutura tes where tes.tesid = atl.tesid limit 1) as dsctipo,
				atl.tesid
			FROM entidade.estrutura atl,  entidade.geo eng
			left join entidade.endereco ede on ede.entid = eng.entid
			left join territorios.municipio mun on mun.muncod = ede.muncod
			join entidade.entidade ent on ent.entid = eng.entid
			WHERE (ST_Distance_Sphere(ST_Transform(eng.point,4291), ST_Transform(atl.estlatlong,4291))/1000) <= {$_REQUEST['raio']}
			and atl.estid = '{$_REQUEST['id']}'";

	$escolas = $db->carregar($sql);

	if(isset($_REQUEST['html'])){
		echo '<table style="border: 1px solid black;" width="100%">
				<tr>
					<th>Tipo</th>
					<th align="center">Raio</th>
					<th align="center">Pontos</th>
					<th align="center">Escolas</th>
					<th align="center">Alunos</th>
				</tr>
				<tr>
					<td>'.$escolas[0]['dsctipo'].'</td>
					<td align="center">'.$_REQUEST['raio'].'km</td>
					<td align="center">1</td>
					<td align="center">'.count($escolas).'</td>
					<td align="center">&nbsp;</td>
				</tr>
			  </table>';
	}else{
		$arDados['tipo'] 		= $escolas[0]['dsctipo'];
		$arDados['idtipo'] 		= $escolas[0]['tesid'];
		$arDados['instituicao'] = $escolas[0]['instituicao'];
		$arDados['raio'] 		= $_REQUEST['raio'];
		$arDados['total'] 		= count($escolas);
		$arDados['id'] 			= $_REQUEST['id'];
		echo simec_json_encode($arDados);
	}
	
	exit;
	
endif;

if($_REQUEST['pontos_escolas']) :

//	header('content-type:text/xml; charset=ISO-8859-1');
	
	$sql = "SELECT				
				(ST_Distance_Sphere(ST_Transform(eng.point,4291), ST_Transform(atl.estlatlong,4291))/1000) as distancia,
				ent.entcodent, 
				ent.entnome, 
				ede.estuf, 
				ede.endbai, 
				ede.endlog, 
				ede.endnum, 
				mun.mundescricao, 
				eng.entid, 
				eng.latitudedecimal as lat, 
				eng.longitudedecimal as lng,
				eng.entid
			FROM entidade.estrutura atl, entidade.geo eng
			left join entidade.endereco ede on ede.entid = eng.entid
			left join territorios.municipio mun on mun.muncod = ede.muncod
			join entidade.entidade ent on ent.entid = eng.entid
			WHERE (ST_Distance_Sphere(ST_Transform(eng.point,4291), ST_Transform(atl.estlatlong,4291))/1000) <= {$_REQUEST['raio']}
			and atl.estid = '{$_REQUEST['id']}'";
	
	$escolas = $db->carregar($sql);
	
	if($escolas[0]):
				
			$conteudo .= "<markers> "; // inicia o XML
				
			foreach($escolas as $d):
					
					$conteudo .= "<marker "; //inicia um ponto no mapa
					$conteudo .= "nome=\"". simec_htmlspecialchars($d['entnome']) ."\" "; // adiciona o nome da instituição;
					$conteudo .= "municipio=\"". simec_htmlspecialchars($d['mundescricao']) ."\" "; // adiciona a descrição do município;
					$conteudo .= "uf=\"". $d['estuf'] ."\" "; // adiciona UF;
					$conteudo .= "distancia=\"". round($d['distancia']/1000,1) ."\" "; // adiciona UF;
					$conteudo .= "codescola=\"". $d['entcodent'] ."\" "; // adiciona UF;
					$conteudo .= "endereco=\"".$d['endlog'].", numero ".$d['endnum'].", Bairro ".$d['endbai']."\" "; 
					$conteudo .= "lat=\"".$d['lat']."\" "; // adiciona a latitude;
					$conteudo .= "lng=\"".$d['lng']."\" "; //adiciona a longitude;
					$conteudo .= "/> ";
				
			endforeach;
				
			$conteudo .= "</markers> ";
				
	endif;
	
	die($conteudo);
	
endif;

function recuperaEstruturas($post = 1, $limit = 0, $offset = 0)
{
	global $db;
	
	if(is_array($post)) extract($post);
	
	if($muncod)
		$arWhere[] = "muncod in ('".(implode("','",explode(',', $muncod)))."')";
	
	if($estuf)
		$arWhere[] = "estuf in ('".(implode("','",explode(',', $estuf)))."')";
		
	if($tipo)
		$arWhere[] = "tesid = {$tipo}";
	
	$sql = "select 
				estid,
				entid,
				case when estnome is null then lower(estlog) || ' - ' || estuf else estnome end as estnome,
				tesid,
				estcep,
				estlog,
				estcom,
				estbai,
				mundescricao,
				muncod,
				estuf,
				estnum,
				eststatus,
				estlat,
				estlon,
				estlatlong,
				estpistaatletismo,
				estidexterno,
				estpiscinasemiolimp,
				estpiscinaolimpica,
				estquadrapoliesportiva,
				estcoberturaquadpolies,
				estginasiocoberto 
			from 
				entidade.estrutura 
			where 
				eststatus = 'A'
			and 
				estlatlong is not null		
			
			".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '');
	
	if($limit>0){
		$sql = $sql.' limit '.$limit;
	}
	
	if($limit>0 && $offset>0){
		$sql = $sql.' offset '.$offset;
	}
	
	return $db->carregar($sql);
}

if($_REQUEST['recupera_pontos']) :
	
	ini_set("memory_limit", "2048M");
	header('content-type:text/xml; charset=ISO-8859-1');
	
	$rs = recuperaEstruturas($_REQUEST);
			   
	if($rs):
				
			$conteudo .= "<markers> "; // inicia o XML
				
			foreach($rs as $d):
					
					$conteudo .= "<marker "; //inicia um ponto no mapa
					$conteudo .= "nome=\"". simec_htmlspecialchars($d['estnome']) ."\" "; // adiciona o nome da instituição;
					$conteudo .= "municipio=\"". simec_htmlspecialchars($d['mundescricao']) ."\" "; // adiciona a descrição do município;
					$conteudo .= "uf=\"". $d['estuf'] ."\" "; // adiciona UF;
					$conteudo .= "id=\"". $d['estid'] ."\" "; // adiciona a descrição do município;
					$conteudo .= "endereco=\"".$d['estlog']."\" ";  
					$conteudo .= "lat=\"". $d['estlat'] ."\" "; // adiciona a latitude;
					$conteudo .= "lng=\"". $d['estlon'] ."\" "; // adiciona a longitude;
					$conteudo .= "tipo=\"". $d['tesid'] ."\" "; // tipo estrutura
					$conteudo .= "/> ";
				
			endforeach;
				
			$conteudo .= "</markers> ";
				
	endif;
	
	die($conteudo);
	
endif;

?>
<link href="http://code.google.com/apis/maps/documentation/javascript/examples/default.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="http://maps.google.com/maps/api/js?sensor=false"></script>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script type="text/javascript">

larguraJanela = parseInt(document.documentElement.clientWidth)-400;

jQuery(function(){
	jQuery('#map_canvas').css('width', larguraJanela);	
});

var map;
var draw_circle = null;  // object of google maps polygon for redrawing the circle

var shape = {
    coord: [1, 1, 1, 20, 18, 20, 18 , 1],
    type: 'poly'
};

var image1 = new google.maps.MarkerImage('/imagens/icone_capacete_pre1.png', new google.maps.Size(9, 14));
var image2 = new google.maps.MarkerImage('/imagens/icone_capacete_pre2.png', new google.maps.Size(9, 14));
var image3 = new google.maps.MarkerImage('/imagens/icone_capacete_pre3.png', new google.maps.Size(9, 14));
var image4 = new google.maps.MarkerImage('/imagens/icone_capacete_pre4.png', new google.maps.Size(9, 14));
var image5 = new google.maps.MarkerImage('/imagens/icone_capacete_pre5.png', new google.maps.Size(9, 14));
var image6 = new google.maps.MarkerImage('/imagens/icone_capacete_pre6.png', new google.maps.Size(9, 14));
var image7 = new google.maps.MarkerImage('/imagens/icone_capacete_pre7.png', new google.maps.Size(9, 14));
var image8 = new google.maps.MarkerImage('/imagens/icone_capacete_pre8.png', new google.maps.Size(9, 14));

var markersArray_ESCOLA = [];
var markersArray_ATLETISMO = [];
var markersArray_SESI = [];
var markersArray_AABB = [];
var markersArray_ABRIGOS = [];
var markersArray_INSTITUTOS = [];
var circles = [];
var infowindow;

var qtdPontos1 = 0
var qtdPontos2 = 0
var qtdPontos3 = 0
var qtdPontos4 = 0
var qtdPontos5 = 0

var qtdEscolas1 = 0;
var qtdEscolas2 = 0;
var qtdEscolas3 = 0;
var qtdEscolas4 = 0;
var qtdEscolas5 = 0;

function initialize() {

	var myLatLng = new google.maps.LatLng(-15.740710,-47.926712);

    var center = myLatLng;
    var myOptions = {
      zoom: 4,
      center: myLatLng,
      mapTypeId: google.maps.MapTypeId.TERRAIN
    };
    
    map = new google.maps.Map(document.getElementById("map_canvas"), myOptions);
        
}

function carregarPontos() {
	
	divCarregando();
	
	document.getElementById('colunainfo').innerHTML = '';
	
	deleteOverlays(markersArray_ESCOLA);
	deleteOverlays(markersArray_ATLETISMO);
	deleteOverlays(markersArray_SESI);
	deleteOverlays(markersArray_AABB);
	deleteOverlays(markersArray_ABRIGOS);
	deleteOverlays(markersArray_INSTITUTOS);	
	deleteOverlays(circles);

	for(x=1;x<6;x++){		
		eval('qtdPontos'+x+'=0;');
		eval('qtdEscolas'+x+'=0;');
	}
	
	var pt = jQuery("[name^='ponto[']:checked");
	jQuery.each(pt, function(i,v){
		
		tipo = parseInt(v.id.split('_')[1]);
		
		if(tipo>0){
			
			switch(tipo){
				case 1: pontos = markersArray_ATLETISMO; icone = image2; break;
				case 2: pontos = markersArray_SESI; icone = image3; break;
				case 3: pontos = markersArray_AABB; icone = image4; break;
				case 4: pontos = markersArray_ABRIGOS; icone = image5; break;
				case 5: pontos = markersArray_INSTITUTOS; icone = image6; break;
			}
			
			criaPontosPorTipo("pdeescola.php?modulo=principal/mapaOlimpiadasEscolares&acao=A&recupera_pontos=todos&tipo="+tipo, pontos, icone, tipo);
		}			
		
	});
	
	html = recuperaInfoMapa();
	document.getElementById('colunainfo').innerHTML = html;
	divCarregado();
}

// Marca os pontos referente as pistas de atletismo militares
function criaPontosPorTipo(xml_filtro, markersArray_, icone, tipo) {

	selectAllOptions( document.getElementById('estuf') );
	selectAllOptions( document.getElementById('muncod') );

	stParams = recuperaFiltros();
	
	jQuery.ajax({
   		type: "POST",
   		url: xml_filtro+stParams,   		
   		async: false,
   		success: function(data) {

			  raio = jQuery('#raio').val() ? parseInt(jQuery('#raio').val()) : '';
			  
		      jQuery(data).find("marker").each(function() {

		    	eval('qtdPontos'+tipo+' = qtdPontos'+tipo+'+1');
		    	
		        var marker = jQuery(this);    	        	

		    	marker.attr('qtdEscolas',eval('qtdEscolas'+tipo));
		    	marker.attr('raio',raio);      
					
		    	if(raio>0) verEscolasDosPontos(parseFloat(marker.attr("lat")),parseFloat(marker.attr("lng")), raio, parseFloat(marker.attr("id")));
		    	if(marker) recuperaInfoPonto(marker, markersArray_);
	    		
		     });
		     
   		}
 	});
}

function verEscolasDosPontos(lat, lng, raio, id) {
	
	criaPontosEscolas("pdeescola.php?modulo=principal/mapaOlimpiadasEscolares&acao=A&pontos_escolas=todos&latitude="+lat+"&longitude="+lng+"&raio="+raio+"&id="+id);
		
    var pointcenter = new google.maps.LatLng(parseFloat(lat),
                                    		 parseFloat(lng));
    
	DrawCircle(raio, pointcenter);
	
	jQuery.ajax({
  		type: "POST",
   		url: "pdeescola.php?modulo=principal/mapaOlimpiadasEscolares&acao=A&verInformacoesEscolas=true&latitude="+lat+"&longitude="+lng+"&raio="+raio+"&id="+id,
   		async: false,
   		dataType: 'json',
   		success: function(nhtml) {
   		
			eval('qtdEscolas'+nhtml.idtipo+' = qtdEscolas'+nhtml.idtipo+'+nhtml.total');
			
   		}
   	});
}

function verEscolasDoPonto(lat, lng, raio, id) {

	deleteOverlays(markersArray_ESCOLA);
	deleteOverlays(circles);

	document.getElementById('raio').value = raio;
	
	criaPontosEscolas("pdeescola.php?modulo=principal/mapaOlimpiadasEscolares&acao=A&pontos_escolas=todos&latitude="+lat+"&longitude="+lng+"&raio="+raio+'&id='+id);
	
    var pointcenter = new google.maps.LatLng(parseFloat(lat),
                                    		 parseFloat(lng));
    
	DrawCircle(raio, pointcenter);
	
	var nhtml;
	
	jQuery.ajax({
  		type: "POST",
   		url: "pdeescola.php?modulo=principal/mapaOlimpiadasEscolares&acao=A&verInformacoesEscolas=true&latitude="+lat+"&longitude="+lng+"&raio="+raio+'&id='+id+'&html=true',
   		async: false,   		
   		success: function(nhtml) {
   			document.getElementById('colunainfo').innerHTML = nhtml;
   		}
   	});
}

function criaPontosEscolas(xml_filtro) {

	jQuery.ajax({
   		type: "POST",
   		url: xml_filtro,
   		async: false,
   		success: function(data) {
   			
		      jQuery(data).find("marker").each(function() {
		        var marker = jQuery(this);

		        var latlng = new google.maps.LatLng(parseFloat(marker.attr("lat")),
		                                    		parseFloat(marker.attr("lng")));
		                                    		
				var html_escola;
					
				html_escola = "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
				html_escola += "<tr><td class=SubTituloDireita>Instituição:</td><td>"+ marker.attr("nome") +"</td></tr>";
				html_escola += "<tr><td class=SubTituloDireita>Endereço:</td><td>"+ marker.attr("endereco") +"</td></tr>";
				html_escola += "<tr><td class=SubTituloDireita>Localização:</td><td>" + marker.attr("municipio") + "/" + marker.attr("uf") + "</td></tr>";
				html_escola += "<tr><td class=SubTituloDireita nowrap>Distância do ponto:</td><td>" + marker.attr("distancia")  + " KM(s)</td></tr>";
				html_escola += "<tr><td class=SubTituloDireita nowrap>Painel Público:</td><td><a style=cursor:pointer; onclick=\"window.open('http://painel.mec.gov.br/painel/detalhamentoIndicador/detalhes/escola/esccodinep/"+ marker.attr("codescola") +"','Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');void(0);\">clique aqui</td></tr>";
				html_escola += "</table>";
				
			    var marker = new google.maps.Marker({
			        position: latlng,
			        map: map,
			        icon: image1,
			        shape: shape
			    });
			    
			    google.maps.event.addListener(marker, "click", function() {
			      if (infowindow) infowindow.close();
			      infowindow = new google.maps.InfoWindow({content: html_escola});
			      infowindow.open(map, marker);
			    });

			    markersArray_ESCOLA.push(marker);

		     });
   		}
 	});
}

function mostrar_painel(painel)
{
	if(document.getElementById(painel).style.display == "none") {
		document.getElementById("img_"+painel).src="../imagens/menos.gif";
		document.getElementById(painel).style.display = "";
	} else {
		document.getElementById("img_"+painel).src="../imagens/mais.gif";
		document.getElementById(painel).style.display = "none";
	}
}

function DrawCircle(rad, pointcenter) {
	 
   rad *= 1000; // convert to meters if in miles

   draw_circle = new google.maps.Circle({
      center: pointcenter,
      radius: rad,
      strokeColor: "#FF0000",
      strokeOpacity: 0.8,
      strokeWeight: 2,
      fillColor: "#FF0000",
      fillOpacity: 0.35,
      map: map
   });
   
   circles.push(draw_circle);   
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
  if (markersArray_.length > 0) {
    for (i in markersArray_) {
    	if(i <= markersArray_.length-1){
      		markersArray_[i].setMap(null);
    	}
    }
  }
}

// Shows any overlays currently in the array
function showOverlays(markersArray_) {
  if (markersArray_.length > 0) {
    for (i in markersArray_) {
       if(i <= markersArray_.length-1){
      		markersArray_[i].setMap(map);
       }      
    }
  }
}

// Função para subustituir todos
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}

function recuperaNomeTipo(tipo)
{	
	switch(parseInt(tipo))
	{
		case 1	: nome = 'Pistas de Atletismo Militares'; break;
		case 2	: nome = 'Instalações Esportivas do SESI'; break;
		case 3	: nome = 'AABB'; break;
		case 4	: nome = 'Abrigos'; break;
		case 5	: nome = 'Instalações Esportivas Institutos'; break;
		default	: nome = 'Não encontrado'; break;
	}
	return nome;
}

function recuperaIconeTipo(tipo)
{
	switch(parseInt(tipo))
	{
		case 1	: icone = image2; break;
		case 2	: icone = image3; break;
		case 3	: icone = image4; break;
		case 4	: icone = image5; break;
		case 5	: icone = image6; break;
		default	: icone = image1; break; 
	}
	return icone;
}

function recuperaInfoPonto(marker, markersArray_)
{

	var html_tipo;			

	html_tipo = '<table style="border: 1px solid black;" width="100%">';
	html_tipo += '<tr><th align="left">Nome</th><td colspan="3">'+ marker.attr("nome") +'</td></tr>';
	html_tipo += '<tr><th align="left">Endereço</th><td colspan="3">'+ marker.attr("endereco") +'</td></tr>';
	html_tipo += '<tr><th align="left">Localização</th><td colspan="3">'+ marker.attr("municipio") + "/" + marker.attr("uf") +'</td></tr>';
	html_tipo += '<tr><th align="left">Tipo</th><td colspan="3">'+recuperaNomeTipo(marker.attr('tipo'))+'</td></tr>';
	if(raio){
		html_tipo += '<tr><th align="left">Raio</th><td colspan="3">'+ marker.attr('raio') +'</td></tr>';
		html_tipo += '<tr><th align="left">Escolas</th><td colspan="3">'+ marker.attr('qtdEscolas') +'</td></tr>';
		html_tipo += '<tr><th align="left">Alunos</th><td colspan="3">&nbsp;</td></tr>';
	}
	html_tipo += '<tr><th>Informe o raio</th><td><input type=text size=4 maxlength=3 name=dist id=dist class="normal"> KM(s) <input type=button value=Ok onclick="verEscolasDoPonto('+ parseFloat(marker.attr("lat")) +','+ parseFloat(marker.attr("lng")) +',document.getElementById(\'dist\').value,\''+ marker.attr("id") +'\');"></td></tr>';
	html_tipo += '</table>';

	var latlng = new google.maps.LatLng(parseFloat(marker.attr("lat")),parseFloat(marker.attr("lng")));

	icone = recuperaIconeTipo(marker.attr('tipo'));

	var marker = new google.maps.Marker({
        position: latlng,
        map: map,
        icon: icone,
        shape: shape
    });
	
	markersArray_.push(marker);

    google.maps.event.addListener(marker, "click", function() {
      if (infowindow) infowindow.close();
      infowindow = new google.maps.InfoWindow({content: html_tipo});
      infowindow.open(map, marker);
    });

}

function recuperaInfoMapa()
{
	raio = jQuery('#raio').val() ? jQuery('#raio').val()+'km' : '';

	html = '<table style="border: 1px solid black;" width="100%">';
	html += '<tr>';
	html += '<th>Tipo</th>';
	html += '<th align="center">Raio</th>';
	html += '<th align="center">Pontos</th>';
	html += '<th align="center">Escolas</th>';
	html += '<th align="center">Alunos</th>';
	html += '</tr>';

	for(x=1;x<6;x++){
		if(eval('qtdPontos'+x+'>0')){			
			
			if(x%2) cor = '#f0f0f0'; else cor = 'white';
			
			html += '<tr style="background:'+cor+'">';
			html += '<td>'+recuperaNomeTipo(x)+'</td>';
			html += '<td align="center">'+raio+'</td>';
			html += '<td align="center">'+eval('qtdPontos'+x)+'</td>';
			html += '<td align="center">'+eval('qtdEscolas'+x)+'</td>';
			html += '<td align="center">&nbsp;</td>';
			html += '</tr>';
		}
	}
		
	html += '</table>';

	return html;
}

function recuperaFiltros()
{
	estuf 	= [];		
	jQuery('#estuf option').each(function(i,v){ estuf.push(v.value); });
	
	muncod 	= [];
	jQuery('#muncod option').each(function(i,v){ muncod.push(v.value); });

	stParams = '';
	if(estuf.length) stParams += '&estuf='+estuf;
	if(estuf.length) stParams += '&muncod='+muncod;

	return stParams;
}

function exibeRelatorio( )
{	
	var formulario = document.formulario;
	formulario.pesquisa.value='1';

	selectAllOptions( document.getElementById( 'agrupadores' ) );
//	selectAllOptions( document.getElementById( 'tesid' ) );
	selectAllOptions( document.getElementById( 'estuf' ) );
	selectAllOptions( document.getElementById( 'muncod' ) );

	agrupador = false;
	jQuery('#agrupadores').each(function(i,v){		
		if(v.value){
			agrupador = true;		
		}
	});
	
	if(agrupador==false){
		alert('Selecione pelo menos um agrupador!');
		return false;
	}

	if(jQuery('#raio').val() == ''){
		alert('O raio é obrigatório!');
		jQuery('#raio').focus();
		return false;
	}
	
	formulario.target = 'resultadoGeral';
	var janela = window.open( '?modulo=principal/mapaOlimpiadasEscolares&acao=A', 'resultadoGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	
	formulario.submit();
	janela.focus();		
}

</script>

<?php 
	include  APPRAIZ."includes/cabecalho.inc";
	echo '<br>';
	
	$titulo = "Google Maps";
	monta_titulo( $titulo, '&nbsp' );

?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td valign="top" style="width:270px;height:400px" >
			<form name="formulario" id="formulario" method="post">
				<input type="hidden" name="pesquisa" value="" />
				<table cellSpacing="0" cellPadding="3" align="left" class="tabela" width="100%">
					<tr>
						<td class="SubTituloEsquerda">Pontos</td>
					</tr>
					<tr>
						<td>					
	<!--						<input type="checkbox" id="ponto_atletismo" name="ponto[atletismo]" value="atletismo" onclick="if(this.checked){showOverlays(markersArray_ATLETISMO, 1);}else{clearOverlays(markersArray_ATLETISMO);}" /> <img src="/imagens/icone_capacete_pre2.png" /> Pistas de Atletismo Militares <br />-->
	<!--						<input type="checkbox" id="ponto_sesi" name="ponto[sesi]" value="sesi" onclick="if(this.checked){showOverlays(markersArray_SESI, 2);}else{clearOverlays(markersArray_SESI);}" /> <img src="/imagens/icone_capacete_pre3.png" /> Instalações Esportivas do SESI <br />-->
	<!--						<input type="checkbox" id="ponto_aabb" name="ponto[aabb]" value="aabb" onclick="if(this.checked){showOverlays(markersArray_AABB, 3);}else{clearOverlays(markersArray_AABB);}" /> <img src="/imagens/icone_capacete_pre4.png" /> AABB <br />-->
	<!--						<input type="checkbox" id="ponto_abrigos" name="ponto[abrigos]" value="abrigos" onclick="if(this.checked){showOverlays(markersArray_ABRIGOS, 4);}else{clearOverlays(markersArray_ABRIGOS);}" /> <img src="/imagens/icone_capacete_pre5.png" /> Abrigos <br />-->
	<!--						<input type="checkbox" id="ponto_institutos" name="ponto[institutos]" value="institutos" onclick="if(this.checked){showOverlays(markersArray_INSTITUTOS, 5);}else{clearOverlays(markersArray_INSTITUTOS);}" /> <img src="/imagens/icone_capacete_pre6.png" /> Instalações Esportivas Institutos <br />-->
							
							<input type="checkbox" id="tipo_1" name="ponto[atletismo]" value="atletismo" onclick="if(this.checked){showOverlays(markersArray_ATLETISMO, 1);}else{clearOverlays(markersArray_ATLETISMO);}" /> <img src="/imagens/icone_capacete_pre2.png" /> Pistas de Atletismo Militares <br />
							<input type="checkbox" id="tipo_2" name="ponto[sesi]" value="sesi" onclick="if(this.checked){showOverlays(markersArray_SESI, 2);}else{clearOverlays(markersArray_SESI);}" /> <img src="/imagens/icone_capacete_pre3.png" /> Instalações Esportivas do SESI <br />
							<input type="checkbox" id="tipo_3" name="ponto[aabb]" value="aabb" onclick="if(this.checked){showOverlays(markersArray_AABB, 3);}else{clearOverlays(markersArray_AABB);}" /> <img src="/imagens/icone_capacete_pre4.png" /> AABB <br />
							<input type="checkbox" id="tipo_4" name="ponto[abrigos]" value="abrigos" onclick="if(this.checked){showOverlays(markersArray_ABRIGOS, 4);}else{clearOverlays(markersArray_ABRIGOS);}" /> <img src="/imagens/icone_capacete_pre5.png" /> Abrigos <br />
							<input type="checkbox" id="tipo_5" name="ponto[institutos]" value="institutos" onclick="if(this.checked){showOverlays(markersArray_INSTITUTOS, 5);}else{clearOverlays(markersArray_INSTITUTOS);}" /> <img src="/imagens/icone_capacete_pre6.png" /> Instalações Esportivas Institutos <br />
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
												estuf || ' - ' || mundescricao AS descricao
											FROM 
												territorios.municipio
											ORDER BY
												estuf, mundescricao";
					
								combo_popup( 'muncod', $sql, 'Selecione os Municípios', '400x400', 0, array(), '', 'S', false, false, 5, 240);							
								?>
								
							</div>
						</td>
					</tr>
					
					<tr>
						<td class="SubTituloEsquerda">
							<img style="cursor: pointer" src="../imagens/mais.gif" id="img_agrupador" onclick="mostrar_painel('agrupador');" border=0> Agrupadores
						</td>
					</tr>
					<tr>
						<td>
							<div id="agrupador" style="display:none">
								<?php
								
								$sql = "
										select 'tipo' as codigo, 'Tipo' as descricao
										union all
										select 'estuf' as codigo, 'UF' as descricao
										union all
										select 'mundescricao' as codigo, 'Município' as descricao
										union all
										select 'instituicao' as codigo, 'Instituição' as descricao
									";
								
								combo_popup( 'agrupadores', $sql, 'Selecione o(s) Agrupador(es)', '400x400', 0, array(), '', 'S', false, false, 5, 240);							
								?>
							</div>
						</td>
					</tr>
					
					<tr>
						<td class="SubTituloEsquerda">Raio: <input type=text id=raio name=raio size=4></td>
					</tr>
	
					<tr>
						<td align="right">
							<input type="button" value="Carregar Mapa" id="carregar" onclick="carregarPontos();" >
							<input type="button" value="Carregar Relatório" id="carregar" onclick="exibeRelatorio();" >
						</td>
					</tr>
					<tr>
						<td class="SubTituloCentro">Informações</td>
					</tr>				
					<tr>
						<td id="colunainfo"></td>
					</tr>	
				</table>
			</form>
		</td>
		
		<td valign="top" align="center">
		
			<div id="map_canvas" style="width:600px;height:600px;position:relative;"></div>
			
		</td>		
		
	</tr>
</table>
<script>
	initialize();	
</script>