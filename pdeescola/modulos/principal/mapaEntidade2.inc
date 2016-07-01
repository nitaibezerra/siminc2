<?

$sesi = array(0 => array('PE','Recife','SESI - Serviço Social da Indústria','NÃO','momo momoom','xx','Momomomo','9999999','08.02.45.S','34.54.26.W','sesi01'),
		      1 => array('PE','Recife','SESI - Serviço Social da Indústria','NÃO','Moommo ommo','xx','ommomo','9999999','08.02.36.S','34.52.26.W','sesi02'),
			  2 => array('PE','Recife','SESI - Serviço Social da Indústria de Ibura-Setor Educação','NÃO','Moomomomom','xx','Momommomomo om','9999999','08.06.37.S','34.56.13.W','sesi03'));
			  
$sesc = array(0 => array('PE','Recife','SESC - Serviço Social do Comércio em Casa Amarela','NÃO','momo momoom','xx','Momomomo','9999999','08.01.33.S','34.54.20.W','sesc01'),
		      1 => array('PE','Recife','SESC - Serviço Social do Comércio','NÃO','Moommo ommo','xx','ommomo','9999999','08.03.13.S','34.53.04.W','sesc02'),
			  2 => array('PE','Recife','SESC - Serviço Social do Comércio-Santa Rita-Clínica Odontológica','NÃO','Moomomomom','xx','Momommomomo om','9999999','08.04.07.S','34.52.37.W','sesc03'));
			  
$senai = array(0 => array('PE','Recife','ESCOLA TÉCNICA SENAI AGUA FRIA - ENGENHEIRO AUSTRICLÍNIO CORTE REAL','NÃO','Rua Japaranduba','98','Água Fria','52120150','08.01.11.S','34.53.36.W','senai01'),
		       1 => array('PE','Recife','ESCOLA TECNICA SENAI AREIAS - JOSEPH TURTON JÚNIOR','NÃO','Avenida Doutor José Rufino','1141','areias','50780000','08.05.19.S','34.56.01.W','senai02'),
			   2 => array('PE','Recife','ESCOLA TECNICA SENAI SANTO AMARO - MANOEL DE BRITO','NÃO','Avenida Norte','539','Santo Amaro','50100000','08.02.50.S','34.52.35.W','senai03'));
			  
$senac = array(0 => array('PE','Recife','Centro de Tecnologia do Varejo CTV - SENAC','SIM','Avenida Visconde de Albuquerque','280','Madalena','50610090','08.03.20.S','34.54.25.W','senac01'),
			   1 => array('PE','Recife','SENAC/PE - SERVIÇO NACIONAL DE APRENDIZAGEM COMERCIAL','NÃO','Avenida Visconde de Suassuna','500','Santo Amaro','50050540','08.03.09.S','34.53.09.W','senac02'));
			   

if($_REQUEST['pontos_sesi']) :

	header('content-type:text/xml; charset=ISO-8859-1');
				   
	if($sesi[0]):
				
			$conteudo .= "<markers> "; // inicia o XML
				
			foreach($sesi as $d):
					
					unset($arrLat,$arrLon);
					$arrLat = explode(".", $d[8]);
					$lat = $arrLat[0]+($arrLat[1]/60)+($arrLat[2]/3600);
					$arrLon = explode(".", $d[9]);
					$log = $arrLon[0]+($arrLon[1]/60)+($arrLon[2]/3600); 
					
					$conteudo .= "<marker "; //inicia um ponto no mapa
					$conteudo .= "nome=\"". simec_htmlspecialchars($d[2]) ."\" "; // adiciona o nome da instituição;
					$conteudo .= "municipio=\"". simec_htmlspecialchars($d[1]) ."\" "; // adiciona a descrição do município;
					$conteudo .= "id=\"". $d[10] ."\" "; // adiciona a descrição do município;
					$conteudo .= "uf=\"". $d[0] ."\" "; // adiciona UF;
					$conteudo .= "endereco=\"".$d[4].", numero ".$d[5].", Bairro ".$d[6]."\" "; 
					$conteudo .= "lat=\"".(($arrLat[3]=="S")?(-1*$lat):$lat)."\" "; // adiciona a latitude;
					$conteudo .= "lng=\"".(($arrLon[3]=="W")?(-1*$log):$log)."\" "; //adiciona a longitude;
					$conteudo .= "/> ";
				
			endforeach;
				
			$conteudo .= "</markers> ";
				
	endif;
	
	die($conteudo);
	
endif;




if($_REQUEST['verInformacoesEscolas']) :

	$sql = "SELECT entcodent, entnome, estuf, endbai, endlog, endnum, mundescricao, entid, lat, lng, distanciaPontosGPS({$_REQUEST['latitude']},{$_REQUEST['longitude']}, lat, lng) as distancia FROM (
			SELECT e.entnome, e.entid, ende.estuf, mun.mundescricao, ende.endbai, ende.endnum, ende.endlog, e.entcodent,
			CASE WHEN (medlatitude is not null AND medlatitude <> '...S' AND medlatitude <> '00000000' AND TRIM(medlatitude) <> 'A.A.A.N' AND TRIM(medlatitude) <> 'A.A.A.A') THEN (CASE WHEN split_part(medlatitude, '.', 4) <>'N' THEN CASE WHEN split_part(medlatitude, '.', 3) <> '' AND split_part(medlatitude, '.', 2) <> '' AND split_part(medlatitude, '.', 1) <> '' THEN (((split_part(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int)))*(-1) END ELSE ((SPLIT_PART(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int)) END) END as lat,
			CASE WHEN (medlongitude is not null AND medlongitude <> '...W' AND medlongitude <> '..' AND TRIM(medlongitude) <> 'A.A.A' AND TRIM(medlongitude) <> 'A.A.A.W') THEN CASE WHEN (SPLIT_PART(medlongitude, '.', 1) <>'' AND SPLIT_PART(medlongitude, '.', 2) <>'' AND split_part(medlongitude, '.', 3) <>'') THEN ((split_part(medlongitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlongitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlongitude, '.', 1)::int))*(-1)	END END as lng 
			FROM entidade.entidade e 
			INNER JOIN entidade.funcaoentidade f ON f.entid = e.entid 
			INNER JOIN entidade.endereco ende ON ende.entid = e.entid 
			INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
			WHERE f.funid = 3 AND medlatitude is not null AND medlongitude is not null) as foo 
			WHERE distanciaPontosGPS({$_REQUEST['latitude']},{$_REQUEST['longitude']}, lat, lng) < ".($_REQUEST['raio']*1000);
	
	$escolas = $db->carregar($sql);
	
	if($escolas[0]):
	
			foreach($escolas as $d):
					
				$_esc[$d['entcodent']] = $d;
			
			endforeach;
	
	endif;
	
	if($_esc) {
	
	$sql = "SELECT dshcod 
			FROM ( SELECT case when d.tdiid1=180 then tiddsc1 when d.tdiid2=180 then tiddsc2 end as tiddsc, 
						  case when d.tdiid1=180 then tidid1 when d.tdiid2=180 then tidid2 end as tidid, 
						  case when d.tdiid1=275 then tidid1 when d.tdiid2=275 then tidid2 end as tidid2,
						  case when d.indcumulativo='S' then sum(d.qtde) when d.indcumulativo='N' then case when d.sehstatus='A' then sum(d.qtde) else 0 end when d.indcumulativo='A' then case when d.dpeanoref='2009' then sum(d.qtde) else 0 end end as qtde, 
						  case when d.indcumulativovalor='S' then sum(d.valor) when d.indcumulativovalor='N' then case when d.sehstatus='A' then sum(d.valor) else 0 end when d.indcumulativovalor='A' then case when d.dpeanoref='2009' then sum(d.valor) else 0 end end as valor, 
						  d.indqtdevalor,
						  d.dshcod
						  from painel.v_detalheindicadorsh d 
						  INNER JOIN territorios.municipio mun ON mun.muncod=d.dshcodmunicipio 
						  INNER JOIN territorios.estado est ON est.estuf=d.dshuf 
						  INNER JOIN territorios.regiao reg ON reg.regcod=est.regcod 
						  INNER JOIN territorios.mesoregiao mes ON mes.mescod = mun.mescod 
						  INNER JOIN territorios.microregiao mic ON mic.miccod = mun.miccod 
						  WHERE d.dshcod IN('".implode("','", array_keys($_esc))."') AND d.indid=442 AND (d.tdiid1=180 or d.tdiid2=180) 
						  GROUP BY d.tiddsc1, d.tidid1, d.tidid2, d.tiddsc2, d.tdiid1, d.tdiid2, d.indcumulativo, d.indcumulativovalor,d.sehstatus, d.dpeanoref,d.indqtdevalor,d.dshcod ORDER BY 1 ) foo 
			WHERE tidid=1689 AND tidid2=1684
			GROUP BY dshcod";
	
	$_escolas = $db->carregarColuna($sql);
	
	}
	
	unset($escolas);

	if($_escolas[0]):
	
		foreach($_escolas as $d):
			if($_esc[$d]) {
				$escolas[] = $_esc[$d]; 
			}
		endforeach;
		
	endif;
	
	
	// Adaptação por não usar BD, gravado em arrays
	if($_REQUEST['id']) {
		$arrTodos = array_merge_recursive($senac, $senai);
		$arrTodos = array_merge_recursive($arrTodos, $sesc);
		$arrTodos = array_merge_recursive($arrTodos, $sesi);
		if($arrTodos) {
			foreach($arrTodos as $arr) {
				if(in_array($_REQUEST['id'],$arr)) {
					$intituicao = $arr[2];
				}
			}
		}
	}
	// fim
	
	echo "<table class=listagem bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center width=95%>";
	echo "<tr><td class=SubTituloDireita>Instituição</td><td>".$intituicao."</td></tr>";
	echo "<tr><td colspan=2 class=SubTituloCentro>Informações sobre escolas (Raio de ".$_REQUEST['raio']." KMs)</td></tr>"; 
	echo "<tr><td class=SubTituloDireita nowrap>Número de escolas:</td><td>".count($escolas)."</td></tr>";
	echo "<tr><td colspan=2>";
	
	if($_esc) {
	
	$sql = "SELECT tiddsc, to_char(sum(qtde),'99') as qtde 
			FROM ( SELECT case when d.tdiid1=180 then tiddsc1 when d.tdiid2=180 then tiddsc2 end as tiddsc, 
						  case when d.tdiid1=180 then tidid1 when d.tdiid2=180 then tidid2 end as tidid, 
						  case when d.tdiid1=275 then tidid1 when d.tdiid2=275 then tidid2 end as tidid2,
						  case when d.indcumulativo='S' then sum(d.qtde) when d.indcumulativo='N' then case when d.sehstatus='A' then sum(d.qtde) else 0 end when d.indcumulativo='A' then case when d.dpeanoref='2009' then sum(d.qtde) else 0 end end as qtde, 
						  case when d.indcumulativovalor='S' then sum(d.valor) when d.indcumulativovalor='N' then case when d.sehstatus='A' then sum(d.valor) else 0 end when d.indcumulativovalor='A' then case when d.dpeanoref='2009' then sum(d.valor) else 0 end end as valor, 
						  d.indqtdevalor 
						  from painel.v_detalheindicadorsh d 
						  INNER JOIN territorios.municipio mun ON mun.muncod=d.dshcodmunicipio 
						  INNER JOIN territorios.estado est ON est.estuf=d.dshuf 
						  INNER JOIN territorios.regiao reg ON reg.regcod=est.regcod 
						  INNER JOIN territorios.mesoregiao mes ON mes.mescod = mun.mescod 
						  INNER JOIN territorios.microregiao mic ON mic.miccod = mun.miccod 
						  WHERE d.dshcod IN('".implode("','", array_keys($_esc))."') AND d.indid=442 AND (d.tdiid1=180 or d.tdiid2=180) 
						  GROUP BY d.tiddsc1, d.tidid1, d.tidid2, d.tiddsc2, d.tdiid1, d.tdiid2, d.indcumulativo, d.indcumulativovalor,d.sehstatus, d.dpeanoref,d.indqtdevalor ORDER BY 1 ) foo 
			WHERE tidid=1689 AND tidid2=1684
			GROUP BY tiddsc, indqtdevalor, tidid";
	
	$cabecalho = array("Detalhes","Qtde");
	
	} else $sql = array();
	
	$db->monta_lista_simples($sql,$cabecalho,50,5,'S','100%',$par2);
	
	echo "</td></tr>";
	echo "</table>";
	
	
	exit;
	
endif;


if($_REQUEST['pontos_escolas']) :

	header('content-type:text/xml; charset=ISO-8859-1');
	
	$sql = "SELECT entcodent, entnome, estuf, endbai, endlog, endnum, mundescricao, entid, lat, lng, distanciaPontosGPS({$_REQUEST['latitude']},{$_REQUEST['longitude']}, lat, lng) as distancia FROM (
			SELECT e.entcodent, e.entnome, e.entid, ende.estuf, mun.mundescricao, ende.endbai, ende.endnum, ende.endlog,
			CASE WHEN (medlatitude is not null AND medlatitude <> '...S' AND medlatitude <> '00000000' AND TRIM(medlatitude) <> 'A.A.A.N' AND TRIM(medlatitude) <> 'A.A.A.A') THEN (CASE WHEN split_part(medlatitude, '.', 4) <>'N' THEN CASE WHEN split_part(medlatitude, '.', 3) <> '' AND split_part(medlatitude, '.', 2) <> '' AND split_part(medlatitude, '.', 1) <> '' THEN (((split_part(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int)))*(-1) END ELSE ((SPLIT_PART(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int)) END) END as lat,
			CASE WHEN (medlongitude is not null AND medlongitude <> '...W' AND medlongitude <> '..' AND TRIM(medlongitude) <> 'A.A.A' AND TRIM(medlongitude) <> 'A.A.A.W') THEN CASE WHEN (SPLIT_PART(medlongitude, '.', 1) <>'' AND SPLIT_PART(medlongitude, '.', 2) <>'' AND split_part(medlongitude, '.', 3) <>'') THEN ((split_part(medlongitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlongitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlongitude, '.', 1)::int))*(-1)	END END as lng 
			FROM entidade.entidade e 
			INNER JOIN entidade.funcaoentidade f ON f.entid = e.entid 
			INNER JOIN entidade.endereco ende ON ende.entid = e.entid 
			INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
			WHERE f.funid = 3 AND medlatitude is not null AND medlongitude is not null) as foo 
			WHERE distanciaPontosGPS({$_REQUEST['latitude']},{$_REQUEST['longitude']}, lat, lng) < ".($_REQUEST['raio']*1000);
	
	$escolas = $db->carregar($sql);
	
	if($escolas[0]):
	
			foreach($escolas as $d):
					
				$_esc[$d['entcodent']] = $d;
			
			endforeach;
	
	endif;
	
	if($_esc) {
	
	$sql = "SELECT dshcod 
			FROM ( SELECT case when d.tdiid1=180 then tiddsc1 when d.tdiid2=180 then tiddsc2 end as tiddsc, 
						  case when d.tdiid1=180 then tidid1 when d.tdiid2=180 then tidid2 end as tidid, 
						  case when d.tdiid1=275 then tidid1 when d.tdiid2=275 then tidid2 end as tidid2,
						  case when d.indcumulativo='S' then sum(d.qtde) when d.indcumulativo='N' then case when d.sehstatus='A' then sum(d.qtde) else 0 end when d.indcumulativo='A' then case when d.dpeanoref='2009' then sum(d.qtde) else 0 end end as qtde, 
						  case when d.indcumulativovalor='S' then sum(d.valor) when d.indcumulativovalor='N' then case when d.sehstatus='A' then sum(d.valor) else 0 end when d.indcumulativovalor='A' then case when d.dpeanoref='2009' then sum(d.valor) else 0 end end as valor, 
						  d.indqtdevalor,
						  d.dshcod
						  from painel.v_detalheindicadorsh d 
						  INNER JOIN territorios.municipio mun ON mun.muncod=d.dshcodmunicipio 
						  INNER JOIN territorios.estado est ON est.estuf=d.dshuf 
						  INNER JOIN territorios.regiao reg ON reg.regcod=est.regcod 
						  INNER JOIN territorios.mesoregiao mes ON mes.mescod = mun.mescod 
						  INNER JOIN territorios.microregiao mic ON mic.miccod = mun.miccod 
						  WHERE d.dshcod IN('".implode("','", array_keys($_esc))."') AND d.indid=442 AND (d.tdiid1=180 or d.tdiid2=180) 
						  GROUP BY d.tiddsc1, d.tidid1, d.tidid2, d.tiddsc2, d.tdiid1, d.tdiid2, d.indcumulativo, d.indcumulativovalor,d.sehstatus, d.dpeanoref,d.indqtdevalor,d.dshcod ORDER BY 1 ) foo 
			WHERE tidid=1689 AND tidid2=1684
			GROUP BY dshcod";
	
	$_escolas = $db->carregarColuna($sql);
	
	}
	
	unset($escolas);

	if($_escolas[0]):
	
		foreach($_escolas as $d):
			if($_esc[$d]) {
				$escolas[] = $_esc[$d]; 
			}
		endforeach;
		
	endif;
	
				   
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


if($_REQUEST['pontos_sesc']) :

	header('content-type:text/xml; charset=ISO-8859-1');

	if($sesc[0]):
				
			$conteudo .= "<markers> "; // inicia o XML
				
			foreach($sesc as $d):
					
					unset($arrLat,$arrLon);
					$arrLat = explode(".", $d[8]);
					$lat = $arrLat[0]+($arrLat[1]/60)+($arrLat[2]/3600);
					$arrLon = explode(".", $d[9]);
					$log = $arrLon[0]+($arrLon[1]/60)+($arrLon[2]/3600); 
					
					$conteudo .= "<marker "; //inicia um ponto no mapa
					$conteudo .= "nome=\"". simec_htmlspecialchars($d[2]) ."\" "; // adiciona o nome da instituição;
					$conteudo .= "municipio=\"". simec_htmlspecialchars($d[1]) ."\" "; // adiciona a descrição do município;
					$conteudo .= "uf=\"". $d[0] ."\" "; // adiciona UF;
					$conteudo .= "id=\"". $d[10] ."\" "; // adiciona a descrição do município;
					$conteudo .= "endereco=\"".$d[4].", numero ".$d[5].", Bairro ".$d[6]."\" "; 
					$conteudo .= "lat=\"".(($arrLat[3]=="S")?(-1*$lat):$lat)."\" "; // adiciona a latitude;
					$conteudo .= "lng=\"".(($arrLon[3]=="W")?(-1*$log):$log)."\" "; //adiciona a longitude;
					$conteudo .= "/> ";
				
			endforeach;
				
			$conteudo .= "</markers> ";
				
	endif;
	
	die($conteudo);
	
endif;



if($_REQUEST['pontos_senai']) :

	header('content-type:text/xml; charset=ISO-8859-1');
				   
	if($senai[0]):
				
			$conteudo .= "<markers> "; // inicia o XML
				
			foreach($senai as $d):
					
					unset($arrLat,$arrLon);
					$arrLat = explode(".", $d[8]);
					$lat = $arrLat[0]+($arrLat[1]/60)+($arrLat[2]/3600);
					$arrLon = explode(".", $d[9]);
					$log = $arrLon[0]+($arrLon[1]/60)+($arrLon[2]/3600); 
					
					$conteudo .= "<marker "; //inicia um ponto no mapa
					$conteudo .= "nome=\"". simec_htmlspecialchars($d[2]) ."\" "; // adiciona o nome da instituição;
					$conteudo .= "municipio=\"". simec_htmlspecialchars($d[1]) ."\" "; // adiciona a descrição do município;
					$conteudo .= "uf=\"". $d[0] ."\" "; // adiciona UF;
					$conteudo .= "id=\"". $d[10] ."\" "; // adiciona a descrição do município;
					$conteudo .= "endereco=\"".$d[4].", numero ".$d[5].", Bairro ".$d[6]."\" "; 
					$conteudo .= "lat=\"".(($arrLat[3]=="S")?(-1*$lat):$lat)."\" "; // adiciona a latitude;
					$conteudo .= "lng=\"".(($arrLon[3]=="W")?(-1*$log):$log)."\" "; //adiciona a longitude;
					$conteudo .= "/> ";
				
			endforeach;
				
			$conteudo .= "</markers> ";
				
	endif;
	
	die($conteudo);
	
endif;


if($_REQUEST['pontos_senac']) :

	header('content-type:text/xml; charset=ISO-8859-1');
				   
	if($senac[0]):
				
			$conteudo .= "<markers> "; // inicia o XML
				
			foreach($senac as $d):
					
					unset($arrLat,$arrLon);
					$arrLat = explode(".", $d[8]);
					$lat = $arrLat[0]+($arrLat[1]/60)+($arrLat[2]/3600);
					$arrLon = explode(".", $d[9]);
					$log = $arrLon[0]+($arrLon[1]/60)+($arrLon[2]/3600); 
					
					$conteudo .= "<marker "; //inicia um ponto no mapa
					$conteudo .= "nome=\"". simec_htmlspecialchars($d[2]) ."\" "; // adiciona o nome da instituição;
					$conteudo .= "municipio=\"". simec_htmlspecialchars($d[1]) ."\" "; // adiciona a descrição do município;
					$conteudo .= "uf=\"". $d[0] ."\" "; // adiciona UF;
					$conteudo .= "id=\"". $d[10] ."\" "; // adiciona a descrição do município;
					$conteudo .= "endereco=\"".$d[4].", numero ".$d[5].", Bairro ".$d[6]."\" "; 
					$conteudo .= "lat=\"".(($arrLat[3]=="S")?(-1*$lat):$lat)."\" "; // adiciona a latitude;
					$conteudo .= "lng=\"".(($arrLon[3]=="W")?(-1*$log):$log)."\" "; //adiciona a longitude;
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
var markersArray_ESCOLA = [];
var markersArray_SENAC = [];
var markersArray_SENAI = [];
var markersArray_SESC = [];
var markersArray_SESI = [];
var circles = [];
var infowindow;
var htmlInfo;


function initialize() {

//	var myLatLng = new google.maps.LatLng(-15.794819612941085, -47.882983261108386);
	var myLatLng = new google.maps.LatLng(-8.05555555556, -34.9069444444);

    var center = myLatLng;
    var myOptions = {
      zoom: 11,
      center: myLatLng,
      mapTypeId: google.maps.MapTypeId.TERRAIN
    };
    
    map = new google.maps.Map(document.getElementById("map_canvas"),
        myOptions);
        
   	criaPontosSENAC("pdeescola.php?modulo=principal/mapaEntidade2&acao=A&pontos_senac=todos");
   	criaPontosSENAI("pdeescola.php?modulo=principal/mapaEntidade2&acao=A&pontos_senai=todos");
   	criaPontosSESC("pdeescola.php?modulo=principal/mapaEntidade2&acao=A&pontos_sesc=todos");
   	criaPontosSESI("pdeescola.php?modulo=principal/mapaEntidade2&acao=A&pontos_sesi=todos");

}

function carregarPontos() {
	
	if(document.getElementById('raio').value == '') {
		alert('Digite um raio');
		return false;
	}
	
	divCarregando();
	
	
	document.getElementById('colunainfo').innerHTML = '';
	deleteOverlays(markersArray_ESCOLA);
	deleteOverlays(circles);

	
	var pt = document.getElementsByName('pontos[]');
	
	if(pt.length > 0) {
		for(var i=0;i<pt.length;i++) {
			if(pt[i].checked) {
				var coords = pt[i].value.split(",");
				verEscolasDosPontos(coords[0], coords[1], document.getElementById('raio').value, pt[i].id);
			}
		}
	}
	
	divCarregado();
}

function verEscolasDosPontos(lat, lng, raio, id) {

	criaPontosEscolas("pdeescola.php?modulo=principal/mapaEntidade2&acao=A&pontos_escolas=todos&latitude="+lat+"&longitude="+lng+"&raio="+raio);
	
    var pointcenter = new google.maps.LatLng(parseFloat(lat),
                                    		 parseFloat(lng));

	DrawCircle(raio, pointcenter);
	
	jQuery.ajax({
  		type: "POST",
   		url: "pdeescola.php?modulo=principal/mapaEntidade2&acao=A&verInformacoesEscolas=true&latitude="+lat+"&longitude="+lng+"&raio="+raio+"&id="+id,
   		async: false,
   		success: function(nhtml) {
   			document.getElementById('colunainfo').innerHTML += nhtml;
   		}
   	});
   	
   	

}



function verEscolasDoPonto(lat, lng, raio, id) {

	divCarregando();
	
	deleteOverlays(markersArray_ESCOLA);
	deleteOverlays(circles);
	
	var pt = document.getElementsByName('pontos[]');
	
	if(pt.length > 0) {
		for(var i=0;i<pt.length;i++) {
			pt[i].checked = false;
		}
	}
	
	document.getElementById(id).checked = true;
	document.getElementById('raio').value = raio;
	
	criaPontosEscolas("pdeescola.php?modulo=principal/mapaEntidade2&acao=A&pontos_escolas=todos&latitude="+lat+"&longitude="+lng+"&raio="+raio+'&id='+id);
	
    var pointcenter = new google.maps.LatLng(parseFloat(lat),
                                    		 parseFloat(lng));
    
	DrawCircle(raio, pointcenter);
	
	var nhtml;
	
	jQuery.ajax({
  		type: "POST",
   		url: "pdeescola.php?modulo=principal/mapaEntidade2&acao=A&verInformacoesEscolas=true&latitude="+lat+"&longitude="+lng+"&raio="+raio+'&id='+id,
   		async: false,
   		success: function(nhtml) {
   			document.getElementById('colunainfo').innerHTML = nhtml;
   		}
   	});

	divCarregado();
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
  if (markersArray_) {
    for (i in markersArray_) {
      markersArray_[i].setMap(null);
    }
  }
}

// Shows any overlays currently in the array
function showOverlays(markersArray_) {
  if (markersArray_) {
    for (i in markersArray_) {
      markersArray_[i].setMap(map);
    }
  }
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

/* Função para subustituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}

/*
 * Marca os pontos referente aos SENACs
 */	
function criaPontosSENAC(xml_filtro) {

	jQuery.ajax({
   		type: "POST",
   		url: xml_filtro,
   		async: false,
   		success: function(data) {
   		
		      jQuery(data).find("marker").each(function() {
		        var marker = jQuery(this);

		        var latlng = new google.maps.LatLng(parseFloat(marker.attr("lat")),
		                                    		parseFloat(marker.attr("lng")));
		                                    		
		                                    		
				var html_senac;
				
				html_senac = "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
				html_senac += "<tr><td class=SubTituloDireita><b>SENAC:</b></td><td>"+ marker.attr("nome") +"</td></tr>";
				html_senac += "<tr><td class=SubTituloDireita><b>Endereço:</b></td><td>"+ marker.attr("endereco") +"</td></tr>";
				html_senac += "<tr><td class=SubTituloDireita><b>Localização:</b></td><td>" + marker.attr("municipio") + "/" + marker.attr("uf") + "</td></tr>";
				html_senac += "<tr><td class=SubTituloDireita nowrap><b>Digite o raio:</b></td><td><input type=text size=4 maxlength=3 name=dist id=dist> KM(s) <input type=button value=Ok onclick=\"verEscolasDoPonto("+ parseFloat(marker.attr("lat")) +","+ parseFloat(marker.attr("lng")) +",document.getElementById('dist').value,'"+ marker.attr("id") +"');\"></td></tr>";
				html_senac += "</table>";
				
			    var marker = new google.maps.Marker({
			        position: latlng,
			        map: map,
			        icon: image2,
			        shape: shape
			    });
			    
			    markersArray_SENAC.push(marker);
				
			    
			    google.maps.event.addListener(marker, "click", function() {
			      if (infowindow) infowindow.close();
    		      htmlInfo = html_senac;
			      infowindow = new google.maps.InfoWindow({content: html_senac});
			      infowindow.open(map, marker);
			    });

			    

		     });
   		}
 		});

}


/*
 * Marca os pontos referente aos SENAIs
 */	
function criaPontosSENAI(xml_filtro) {

	jQuery.ajax({
   		type: "POST",
   		url: xml_filtro,
   		async: false,
   		success: function(data) {
   		
		      jQuery(data).find("marker").each(function() {
		        var marker = jQuery(this);

		        var latlng = new google.maps.LatLng(parseFloat(marker.attr("lat")),
		                                    		parseFloat(marker.attr("lng")));
		                                    		
		                                    		
				var html_senai;
				
				html_senai = "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
				html_senai += "<tr><td class=SubTituloDireita><b>SENAI:</b></td><td>"+ marker.attr("nome") +"</td></tr>";
				html_senai += "<tr><td class=SubTituloDireita><b>Endereço:</b></td><td>"+ marker.attr("endereco") +"</td></tr>";
				html_senai += "<tr><td class=SubTituloDireita><b>Localização:</b></td><td>" + marker.attr("municipio") + "/" + marker.attr("uf") + "</td></tr>";
				html_senai += "<tr><td class=SubTituloDireita nowrap><b>Digite o raio:</b></td><td><input type=text size=4 maxlength=3 name=dist id=dist> KM(s) <input type=button value=Ok onclick=\"verEscolasDoPonto("+ parseFloat(marker.attr("lat")) +","+ parseFloat(marker.attr("lng")) +",document.getElementById('dist').value,'"+ marker.attr("id") +"');\"></td></tr>";
				html_senai += "</table>";
				
			    var marker = new google.maps.Marker({
			        position: latlng,
			        map: map,
			        icon: image3,
			        shape: shape
			    });
			    
			    markersArray_SENAI.push(marker);
			    
			    google.maps.event.addListener(marker, "click", function() {
			      if (infowindow) infowindow.close();
			      htmlInfo = html_senai;
			      infowindow = new google.maps.InfoWindow({content: html_senai});
			      infowindow.open(map, marker);
			    });

			    

		     });
   		}
 		});

}

/*
 * Marca os pontos referente aos SENAIs
 */	
function criaPontosSESC(xml_filtro) {

	jQuery.ajax({
   		type: "POST",
   		url: xml_filtro,
   		async: false,
   		success: function(data) {
   		
		      jQuery(data).find("marker").each(function() {
		        var marker = jQuery(this);

		        var latlng = new google.maps.LatLng(parseFloat(marker.attr("lat")),
		                                    		parseFloat(marker.attr("lng")));
		                                    		
		                                    		
				var html_sesc;
				
				html_sesc = "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
				html_sesc += "<tr><td class=SubTituloDireita><b>SESC:</b></td><td>"+ marker.attr("nome") +"</td></tr>";
				html_sesc += "<tr><td class=SubTituloDireita><b>Endereço:</b></td><td>"+ marker.attr("endereco") +"</td></tr>";
				html_sesc += "<tr><td class=SubTituloDireita><b>Localização:</b></td><td>" + marker.attr("municipio") + "/" + marker.attr("uf") + "</td></tr>";
				html_sesc += "<tr><td class=SubTituloDireita nowrap><b>Digite o raio:</b></td><td><input type=text size=4 maxlength=3 name=dist id=dist> KM(s) <input type=button value=Ok onclick=\"verEscolasDoPonto("+ parseFloat(marker.attr("lat")) +","+ parseFloat(marker.attr("lng")) +",document.getElementById('dist').value,'"+ marker.attr("id") +"');\"></td></tr>";
				html_sesc += "</table>";
				
			    var marker = new google.maps.Marker({
			        position: latlng,
			        map: map,
			        icon: image4,
			        shape: shape
			    });
			    
			    markersArray_SESC.push(marker);
			    
			    google.maps.event.addListener(marker, "click", function() {
			      if (infowindow) infowindow.close();
			      htmlInfo = html_sesc;
			      infowindow = new google.maps.InfoWindow({content: html_sesc});
			      infowindow.open(map, marker);
			    });

		     });
   		}
 		});

}

/*
 * Marca os pontos referente aos SENAIs
 */	
function criaPontosSESI(xml_filtro) {

	jQuery.ajax({
   		type: "POST",
   		url: xml_filtro,
   		async: false,
   		success: function(data) {
   		
		      jQuery(data).find("marker").each(function() {
		        var marker = jQuery(this);

		        var latlng = new google.maps.LatLng(parseFloat(marker.attr("lat")),
		                                    		parseFloat(marker.attr("lng")));
		                                    		
		                                    		
				var html_sesi;
				
				html_sesi = "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
				html_sesi += "<tr><td class=SubTituloDireita><b>SESI:</b></td><td>"+ marker.attr("nome") +"</td></tr>";
				html_sesi += "<tr><td class=SubTituloDireita><b>Endereço:</b></td><td>"+ marker.attr("endereco") +"</td></tr>";
				html_sesi += "<tr><td class=SubTituloDireita><b>Localização:</b></td><td>" + marker.attr("municipio") + "/" + marker.attr("uf") + "</td></tr>";
				html_sesi += "<tr><td class=SubTituloDireita nowrap><b>Digite o raio:</b></td><td><input type=text size=4 maxlength=3 name=dist id=dist> KM(s) <input type=button value=Ok onclick=\"verEscolasDoPonto("+ parseFloat(marker.attr("lat")) +","+ parseFloat(marker.attr("lng")) +",document.getElementById('dist').value,'"+ marker.attr("id") +"');\"></td></tr>";
				html_sesi += "</table>";
				
			    var marker = new google.maps.Marker({
			        position: latlng,
			        map: map,
			        icon: image5,
			        shape: shape
			    });
			    
			    markersArray_SESI.push(marker);
			    
			    google.maps.event.addListener(marker, "click", function() {
			      if (infowindow) infowindow.close();
			      htmlInfo = html_sesi;
			      infowindow = new google.maps.InfoWindow({content: html_sesi});
			      infowindow.open(map, marker);
			    });

			    

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

</script>

<?php 
	include  APPRAIZ."includes/cabecalho.inc";
	echo '<br>';
	
	$titulo = "Google Maps";
	monta_titulo( $titulo, '&nbsp' );

?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td valign="top" style="width:265px;height:400px" >
			<table cellSpacing="0" cellPadding="3" align="left" class="tabela" width="260px">
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
					<td class="SubTituloEsquerda">Pontos</td>
				</tr>
				<tr>
					<td>
						<input type="checkbox" id="ponto_senac" name="ponto[senac]" checked="checked" value="senac" onclick="if(this.checked){showOverlays(markersArray_SENAC);}else{clearOverlays(markersArray_SENAC);}" /> <img src="/imagens/icone_capacete_pre2.png" /> SENAC <br />
						<table width=100% cellpadding=0 cellspacing=0 class=listagem>
						<?
						if($senac) {
							foreach($senac as $se) {
								unset($arrLat,$arrLon);
								$arrLat = explode(".", $se[8]);
								$lat = $arrLat[0]+($arrLat[1]/60)+($arrLat[2]/3600);
								$lat = (($arrLat[3]=="S")?(-1*$lat):$lat);
								$arrLon = explode(".", $se[9]);
								$log = $arrLon[0]+($arrLon[1]/60)+($arrLon[2]/3600);
								$log = (($arrLon[3]=="W")?(-1*$log):$log); 
								
								echo "<tr>";
								echo "<td valign=top width=98%><img src=../imagens/seta_filho.gif align=absmiddle><font size=1>".$se[2]."</font></td><td valign=top><input type=checkbox id=".$se[10]." name=pontos[] value=\"".$lat.",".$log."\"></td>";
								echo "</tr>";
							}
						}
						?>
						</table>
						<input type="checkbox" id="ponto_senai" name="ponto[senai]" checked="checked" value="senai" onclick="if(this.checked){showOverlays(markersArray_SENAI);}else{clearOverlays(markersArray_SENAI);}" /> <img src="/imagens/icone_capacete_pre3.png" /> SENAI <br />
						<table width=100% cellpadding=0 cellspacing=0 class=listagem>
						<?
						if($senai) {
							foreach($senai as $se) {
								unset($arrLat,$arrLon);
								$arrLat = explode(".", $se[8]);
								$lat = $arrLat[0]+($arrLat[1]/60)+($arrLat[2]/3600);
								$lat = (($arrLat[3]=="S")?(-1*$lat):$lat);
								$arrLon = explode(".", $se[9]);
								$log = $arrLon[0]+($arrLon[1]/60)+($arrLon[2]/3600);
								$log = (($arrLon[3]=="W")?(-1*$log):$log); 
								
								echo "<tr>";
								echo "<td valign=top width=98%><img src=../imagens/seta_filho.gif align=absmiddle><font size=1>".$se[2]."</font></td><td valign=top><input type=checkbox id=".$se[10]." name=pontos[] value=\"".$lat.",".$log."\"></td>";
								echo "</tr>";
							}
						}
						?>
						</table>
						<input type="checkbox" id="ponto_sesc" name="ponto[sesc]" checked="checked" value="sesc" onclick="if(this.checked){showOverlays(markersArray_SESC);}else{clearOverlays(markersArray_SESC);}" /> <img src="/imagens/icone_capacete_pre4.png" /> SESC <br />
						<table width=100% cellpadding=0 cellspacing=0 class=listagem>
						<?
						if($sesc) {
							foreach($sesc as $se) {
								unset($arrLat,$arrLon);
								$arrLat = explode(".", $se[8]);
								$lat = $arrLat[0]+($arrLat[1]/60)+($arrLat[2]/3600);
								$lat = (($arrLat[3]=="S")?(-1*$lat):$lat);
								$arrLon = explode(".", $se[9]);
								$log = $arrLon[0]+($arrLon[1]/60)+($arrLon[2]/3600);
								$log = (($arrLon[3]=="W")?(-1*$log):$log); 
								
								echo "<tr>";
								echo "<td valign=top width=98%><img src=../imagens/seta_filho.gif align=absmiddle><font size=1>".$se[2]."</font></td><td valign=top><input type=checkbox id=".$se[10]." name=pontos[] value=\"".$lat.",".$log."\"></td>";
								echo "</tr>";
							}
						}
						?>
						</table>
						<input type="checkbox" id="ponto_sesi" name="ponto[sesi]" checked="checked" value="sesi" onclick="if(this.checked){showOverlays(markersArray_SESI);}else{clearOverlays(markersArray_SESI);}" /> <img src="/imagens/icone_capacete_pre5.png" /> SESI
						<table width=100% cellpadding=0 cellspacing=0 class=listagem>
						<?
						if($sesi) {
							foreach($sesi as $se) {
								unset($arrLat,$arrLon);
								$arrLat = explode(".", $se[8]);
								$lat = $arrLat[0]+($arrLat[1]/60)+($arrLat[2]/3600);
								$lat = (($arrLat[3]=="S")?(-1*$lat):$lat);
								$arrLon = explode(".", $se[9]);
								$log = $arrLon[0]+($arrLon[1]/60)+($arrLon[2]/3600);
								$log = (($arrLon[3]=="W")?(-1*$log):$log); 
								
								echo "<tr>";
								echo "<td valign=top width=98%><img src=../imagens/seta_filho.gif align=absmiddle><font size=1>".$se[2]."</font></td><td valign=top><input type=checkbox id=".$se[10]." name=pontos[] value=\"".$lat.",".$log."\"></td>";
								echo "</tr>";
							}
						}
						?>
						</table>

					</td>
				</tr>
				<tr>
					<td class="SubTituloEsquerda">Raio: <input type=text id=raio name=raio size=4></td>
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
					<td class="SubTituloEsquerda">
						<img style="cursor: pointer" src="../imagens/mais.gif" id="img_grupo" onclick="mostrar_painel('grupo');" border=0> Grupo de Municípios
					</td>
				</tr>
				<tr>
					<td>
						<div id="grupo" style="display:none">
							<?php
							$sql = " 	SELECT	
											tpm.tpmid AS codigo,
											tpmdsc AS descricao
										FROM 
											territorios.tipomunicipio tpm
										ORDER BY
											tpmdsc";
				
							combo_popup( 'tpmid', $sql, 'Selecione os Grupos de Município', '400x400', 0, array(), '', 'S', false, false, 5, 240);							?>
						</div>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">
						<input type="button" value="Carregar" id="carregar" onclick="carregarPontos();" >
					</td>
				</tr>
			</table>
		</td>
		<td valign="top"><div id="map_canvas" style="width:100%;height:100%;position:relative;"></div></td>
		
		<td valign="top" style="width:300px;">
		
		<table width="100%">
		<tr>
			<td class="SubTituloCentro">Informações</td>
		</tr>
		<tr>
			<td id="colunainfo"></td>
		</tr>

		</table>
		
		</td>
		
	</tr>
</table>
<script>
	initialize();
	
</script>