<?php
/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */
// inicializa sistema
define( 'APPRAIZ', '' );
		$nome_bd     = '';
		$servidor_bd = '';
		$porta_bd    = '5432';
		$usuario_db  = '';
		$senha_bd    = '';

//require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();
?>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Exemplo de Polígono</title>
<link href="http://code.google.com/apis/maps/documentation/javascript/examples/default.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript" src="http://maps.google.com/maps/api/js?sensor=false"></script>
<script type="text/javascript">
  function initialize() {

	var myLatLng = new google.maps.LatLng(-13.44, -48.24);

    var center = myLatLng;
    var myOptions = {
      zoom: 4,
      center: myLatLng,
      mapTypeId: google.maps.MapTypeId.TERRAIN
    };
    
    map = new google.maps.Map(document.getElementById("map_canvas"),myOptions);
	
  }

</script>

 
</head> 
<body onload="initialize();"> 

<TABLE cellspacing="0" cellpadding="0">
<TR>
	<TD width="250" rowspan="2" valign="top">
		<?php 
			$sql = "SELECT	estuf AS codigo,
				estdescricao AS descricao
				FROM 
				territorios.estado
				ORDER BY
				estdescricao ";

			combo_popup( 'inp_estuf', $sql, 'Selecione as Unidades Federativas', '400x400', 0, array(), '', 'S', false, false, 5, 210, '', '' );
		?><br>
		<input type="button" id="btn_buscar" name="btn_buscar" value="Carregar" onclick="carregar()" />
	</TD>
	<TD width="100%" align="center" height="30" bgcolor="#dedede">
			<TABLE width="100%" cellspacing="0" cellpadding="3" border ="1">
				<tr>
					<?
						$sql = "SELECT	estuf, estdescricao	FROM territorios.estado	ORDER BY estuf ";
						$arrDados = $db->carregar($sql);
						if($arrDados){
							for ( $i = 0; $i < count( $arrDados ); $i++ )
								{
					?>
					<td bgcolor="#dedede" align="center" onclick="carregauf('<?=$arrDados[$i]['estuf'];?>');"><div id="estuf<?=$arrDados[$i]['estuf'];?>"><?=$arrDados[$i]['estuf'];?></div></td>
					<?
								}

						}
					?>
				</tr>
			</table>
			<div id="nome_mun" style="color: black;font-family: Arial;font-size: 14pt;font-weight: bolder;">Brasil</div>
			
	</TD>
	<TD width="250" rowspan="2" valign="top"><div id="inf_mun" style="color: black;font-family: Arial;font-size: 14pt;font-weight: bolder;width:250px"></div></TD>
</TR>
<TR>
	<TD valign="top"><div id="map_canvas" style="width: 100%; height: 550px; position:relative; "></div></TD>
</TR>
</TABLE>


<script type="text/javascript">

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

function mostrainfmunicipio(dados){
document.getElementById('inf_mun').innerHTML = dados;
}


function carregar(){
	$.ajax({
		type: "POST",
		url: "carrega_poligonos.php",
		data: "uf=MG",
		async: false,
		dataType:'JSON',
		success: function(response){
			montarPoligonos(response);
		}
	});
}

function carregauf(estuf){
	divCarregando();
	$.ajax({
		type: "POST",
		url: "carrega_poligonos.php",
		data: "uf="+estuf,
		async: false,
		dataType:'JSON',
		success: function(response){
			montarPoligonos(response);
			divCarregado();
		}
	});
}

var nomePoli = Array();
var corPoli = Array();
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
      fillOpacity: 0.8
    });
	 nomePoli[item.estuf+item.muncod].setMap(map);
	 google.maps.event.addListener(nomePoli[item.estuf+item.muncod], 'mouseover', function(event){f_mouseover(item.estuf+item.muncod,corpolyd,item.mundescricao+'/'+item.estuf);});
	 google.maps.event.addListener(nomePoli[item.estuf+item.muncod], 'mouseout', function(event){f_mouseout(item.estuf+item.muncod,item.cor);});
	 google.maps.event.addListener(nomePoli[item.estuf+item.muncod], 'click', function(event){infmunicipio(item.muncod);});
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
	if(obj.checked) {cor = '#00ffff';} else {cor = '#f6ead9';}
	var codes = arr.split(",");
	for(var i=0;i<codes.length;i++) {
		f_mudacor(codes[i],cor);
	}
}


</script>

</body> 
</html> 
