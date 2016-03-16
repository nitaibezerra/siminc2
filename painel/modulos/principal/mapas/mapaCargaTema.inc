<?php

function executarCarga($dados) {
	global $db;
	$_cr_municipios = $db->carregarColuna("SELECT muncod FROM territorios.municipio");
	$_cr_temas = $db->carregarColuna("SELECT tmaid FROM mapa.tema");

	
	if($_FILES["arquivo"]["tmp_name"]) {
	    $csvarray = file($_FILES["arquivo"]["tmp_name"]);
	} else {
		$erros[0][0] = "Arquivo não foi enviado corretamente";
	}
	
	if(is_numeric(substr($csvarray[0],0,1))) {
		$st=0;
	} else {
		$st=1;
	}
	
	$armazena = array();
	
	for($i=$st;$i<count($csvarray);$i++) {
		
		$colunas = explode(";",$csvarray[$i]);
		
		// validando se existe tema
		if(!in_array(trim($colunas[0]),$_cr_temas)) {
			$erro[$i][1] = "Tema não existe";
		}
		
		if(!in_array(trim($colunas[1]),$_cr_municipios)) {
			$erro[$i][2] = "Município não existe";
		}

		
		if(!$erro[$i][1]) {
			
			$tpd = $db->pegaLinha("SELECT tpdtipo, tpdformato, tpdcampotema FROM mapa.tipodado t 
								   INNER JOIN mapa.tema i ON i.tpdid = t.tpdid 
								   WHERE i.tmaid='".trim($colunas[0])."'");
			
			switch($tpd['tpdtipo']) {
				case 'numeric':
					$vlr = trim($colunas[2]);
					if(!is_numeric($vlr)) {
						$erro[$i][3] = "Valor não confere com o padrão (".$tpd['tpdformato'].")";
					}
					break;
				case 'money':
					$vlr = str_replace(array(".",","),array("","."),trim($colunas[2]));
					if(!is_numeric($vlr)) {
						$erro[$i][3] = "Valor não confere com o padrão (".$tpd['tpdformato'].")";
					}
					break;
				case 'boolean':
					$vlr = trim($colunas[2]);
					$op = array("TRUE","FALSE");
					if(!in_array(trim($colunas[2]),$op)) {
						$erro[$i][3] = "Valor não confere com o padrão (".$tpd['tpdformato'].")";
					}
					break;
			}
			
			$_EXECUTAR[trim($colunas[0])][trim($colunas[1])][$tpd['tpdcampotema']] = $vlr;
			
		}
		
	}
	
	if(count($erro)>0) {
		
		echo "Foram encontrados ".count($erro)." erros.<br>";
		
		foreach($erro as $lin => $arr) {
			foreach($arr as $col => $err) {
				echo "* LINHA : ".$lin.", COLUNA : ".$col." => ".$err."<br>";
			}
		}
	} else {
		
		if($_EXECUTAR) {
			foreach($_EXECUTAR as $tmaid => $arrD) {
				
				$db->executar("DELETE FROM mapa.temadado WHERE tmaid='".$tmaid."'");
				
				if($arrD) {
					foreach($arrD as $muncod => $arrT) {
						
						if($arrT) {
							$campo = key($arrT);
							$conteudo = current($arrT);
							$sql = "INSERT INTO mapa.temadado(tmaid, {$campo}, muncod)
	    							VALUES ('".$tmaid."', ".(($campo=="tmdboleano")?trim($conteudo):"'".trim($conteudo)."'").", '{$muncod}');";
							$db->executar($sql);
							
						}

					}
				}
			}
		}
		
		$db->commit();
		echo "CSV executado com sucesso.<br>";
	}

}

if($_REQUEST['requisicao']) {
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;

$pflcod = PAINEL_PERFIL_MAPAS;

if($db->testa_superuser()) {
	
	$sql = "SELECT 
				i.tmaid, 
				i.tmadsc,
				m.mapid,
				m.mapdsc,
				td.tpdformato
			FROM 
				mapa.tema i
			INNER JOIN 
				mapa.mapatema tm ON tm.tmaid = i.tmaid   
			INNER JOIN 
				mapa.mapa m ON m.mapid = tm.mapid  
			LEFT JOIN 
				mapa.tipodado td ON td.tpdid = i.tpdid  
			WHERE 
				m.mapstatus='A' 
			GROUP BY 
				i.tmaid, 
				i.tmadsc,
				m.mapid,
				m.mapdsc,
				td.tpdformato
			order by
				m.mapdsc";
	
} else {

	$sql = "SELECT 				i.tmaid, 
				i.tmadsc,
				m.mapid,
				m.mapdsc,
				td.tpdformato

			FROM 
				painel.usuarioresponsabilidade ur 
			INNER JOIN 
				mapa.mapa m ON ur.mapid = m.mapid
			INNER JOIN 
				mapa.mapatema tm ON tm.mapid = m.mapid 
			INNER JOIN 
				mapa.tema i ON i.tmaid = tm.tmaid 
			LEFT JOIN 
				mapa.tipodado td ON td.tpdid = i.tpdid  
			WHERE 
				ur.rpustatus = 'A' 
			AND 
				ur.usucpf = '$usucpf' 
			AND 
				ur.pflcod = $pflcod 
			AND 
				m.mapstatus = 'A' 
			GROUP BY 
				i.tmaid, 
				i.tmadsc,
				m.mapid,
				m.mapdsc,
				td.tpdformato
			order by
				m.mapdsc";

}

?>
<script>

function enviarCarga() {
	if(document.getElementById('arquivocsv').value == '') {
		alert('Selecione um arquivo para enviar');
		return false;
	}
	document.getElementById('formulario').submit();
}

</script>


<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
<tr>
	<td>
	<form method="post" name="formulario" id="formulario" enctype="multipart/form-data" action="" target="iframeUpload">
	<input type="hidden" name="requisicao" value="executarCarga">
	<p><input type="file" name="arquivo" id="arquivocsv"> <input type="button" value="Enviar" onclick="enviarCarga();"></p>
	<p align="center"><b>Log da carga</b></p>
	<iframe name="iframeUpload" id="iframeresp" style="width:100%;height:80px;border:0px solid #fff;"></iframe>
	</form>
	</td>
</tr>
<tr>
	<td>
	<p>A estrutura da carga dever seguir as seguintes variaveis:</p>
	<SPAN style="FONT-FAMILY:courier">( ID ) Tema; ( MUNCOD ) Município; Valor</SPAN>
	<p>Estas variaveis podem ser consultadas no manual de carga localizado abaixo. Segue um exemplo de arquivo CSV.</p>
	<p>
	<SPAN style="FONT-FAMILY:courier">( ID ) Tema; ( MUNCOD ) Município; Valor</SPAN><br>
	<SPAN style="FONT-FAMILY:courier">1 ; 4209409 ; 22120</SPAN><br>
	<SPAN style="FONT-FAMILY:courier">2 ; 4209409 ; 10632</SPAN><br>
	</p>
	<p>Este arquivo ira fazer uma carga do Tema <b>( 1 ) Atenção Domiciliar - População e ( 2 ) Atenção Domiciliar - Hospital de Referência</b>. A carga acontecera no municipio 
	de <b>(4209409)Laguna-SC</b>.</p>
	</td>
</tr>

</table>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
<tr>
	<td class="SubTituloCentro">( ID ) Mapa - Tema</td>
	<td class="SubTituloCentro">Formato</td>
</tr>
<?

$arrMapas = $db->carregar($sql);

if($arrMapas[0]) {
	foreach($arrMapas as $map) {
	?>
	<tr>
		<td style="border: 1px solid #000;"><?="( ".$map['tmaid']." ) ".$map['mapdsc']." - ".$map['tmadsc'] ?></td>
		<td style="border: 1px solid #000;"><?=$map['tpdformato'] ?></td>
	</tr>
	<?
	}
}
?>
</table>