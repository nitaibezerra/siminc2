<?php

function executarCarga($dados) {
	global $db;
	$_cr_municipios = $db->carregarColuna("SELECT muncod FROM territorios.municipio");
	
	$indicadores = $db->carregar("SELECT * FROM mapa.indicador WHERE indstatus='A'");
	if($indicadores[0]) {
		foreach($indicadores as $ind) {
			
			$indid = $ind['indid'];
			$mapid = $ind['mapid'];
			
			$_cr_indicadores[$indid]['indicador'] = true;
			
			$detalhes = $db->carregarColuna("SELECT dtiid FROM mapa.detalheindicador WHERE indid='".$indid."'");
			if($detalhes) {
				foreach($detalhes as $dtiid) {
					$_cr_indicadores[$indid]['detalheindicador'][$dtiid]=true;		
				}
			}
			
			$cenarios = $db->carregarColuna("SELECT mpcid FROM mapa.mapacenario WHERE mapid='".$mapid."'");
			if($cenarios) {
				foreach($cenarios as $mpcid) {
					$_cr_indicadores[$indid]['mapacenario'][$mpcid]=true;		
				}
			}
		}
	}
	
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
		
		// validando se existe indicador
		if(!$_cr_indicadores[trim($colunas[0])]['indicador']) {
			$erro[$i][1] = "Indicador não existe";
		}
		// validando se existe cenario
		if(is_numeric(trim($colunas[1]))) {
			if(!$_cr_indicadores[trim($colunas[0])]['mapacenario'][trim($colunas[1])]) {
				$erro[$i][2] = "Cenário não existe e/ou não esta vinculado com indicador";
			}
		}
		// validando se existe detalhes
		if(!$_cr_indicadores[trim($colunas[0])]['detalheindicador'][trim($colunas[2])]) {
			$erro[$i][3] = "Detalhe não existe e/ou não esta vinculado com indicador";
		}
		// validando municipio
		if(!in_array(trim($colunas[3]), $_cr_municipios)) {
			$erro[$i][4] = "Municipio não existe";
		}
		
		if(!$erro[$i][1]) {
			
			$tpd = $db->pegaLinha("SELECT tpdtipo, tpdformato, tpdcampo FROM mapa.tipodado t 
								   INNER JOIN mapa.detalheindicador i ON i.tpdid = t.tpdid 
								   WHERE i.dtiid='".trim($colunas[2])."'");

			switch($tpd['tpdtipo']) {
				case 'numeric':
					$vlr = trim($colunas[4]);
					if(!is_numeric($vlr)) {
						$erro[$i][5] = "Valor não confere com o padrão (".$tpd['tpdformato'].")";
					}
					break;
				case 'money':
					$vlr = str_replace(array(".",","),array("","."),trim($colunas[4]));
					if(!is_numeric($vlr)) {
						$erro[$i][5] = "Valor não confere com o padrão (".$tpd['tpdformato'].")";
					}
					break;
				case 'boolean':
					$vlr = trim($colunas[4]);
					$op = array("TRUE","FALSE");
					if(!in_array(trim($colunas[5]),$op)) {
						$erro[$i][5] = "Valor não confere com o padrão (".$tpd['tpdformato'].")";
					}
					break;
			}
			
			$_EXECUTAR[trim($colunas[0])][trim($colunas[2])][((trim($colunas[1]))?trim($colunas[1]):'*')][trim($colunas[3])][$tpd['tpdcampo']] = $vlr;
			
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
			foreach($_EXECUTAR as $indid => $arrD) {
				$dets = array_keys($arrD);
				if($arrD) {
					foreach($arrD as $dtiid => $arrC) {
						if($arrC) {
							foreach($arrC as $mpcid => $arrM) {
								if($mpcid=='*') $db->executar("DELETE FROM mapa.valorindicador WHERE dtiid='".$dtiid."'");
								else $db->executar("DELETE FROM mapa.valorindicador WHERE mpcid='".$mpcid."' AND dtiid='".$dtiid."'");
								if($arrM) {
									foreach($arrM as $muncod => $arrT) {
										$campo = key($arrT);
										$conteudo = current($arrT);
										$sql = "INSERT INTO mapa.valorindicador(
            									muncod, {$campo}, dtiid ".((is_numeric($mpcid))?", mpcid":"").")
    											VALUES ('".$muncod."', '".$conteudo."', '".$dtiid."' ".((is_numeric($mpcid))?", '".$mpcid."'":"").");";
										$db->executar($sql);
									}
								}
							}
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
				i.indid, 
				i.inddsc,
				m.mapid,
				m.mapdsc
			FROM 
				mapa.indicador i 
			INNER JOIN 
				mapa.mapa m ON m.mapid = i.mapid   
			WHERE 
				m.mapstatus='A' AND i.indstatus='A'
			order by
				m.mapdsc";
	
} else {

	$sql = "SELECT 				i.indid, 
				i.inddsc,
				m.mapid,
				m.mapdsc
	
			FROM 
				painel.usuarioresponsabilidade ur 
			INNER JOIN 
				mapa.mapa m ON ur.mapid = m.mapid 
			INNER JOIN 
				mapa.indicador i ON i.mapid = mapid 
			WHERE 
				ur.rpustatus = 'A' 
			AND 
				ur.usucpf = '$usucpf' 
			AND 
				ur.pflcod = $pflcod 
			AND 
				m.mapstatus = 'A'
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
	<SPAN style="FONT-FAMILY:courier">( ID ) Indicador; ( ID ) Cenário; ( ID ) Detalhe; ( MUNCOD ) Munícipio; Valor</SPAN>
	<p>Estas variaveis podem ser consultadas no manual de carga localizado abaixo. Segue um exemplo de arquivo CSV.</p>
	<p>
	<SPAN style="FONT-FAMILY:courier">( ID ) Indicador; ( ID ) Cenário; ( ID ) Detalhe; ( MUNCOD ) Munícipio; Valor</SPAN><br>
	<SPAN style="FONT-FAMILY:courier">22 ; * ; 26 ; 4209409 ; 10632</SPAN><br>
	<SPAN style="FONT-FAMILY:courier">22 ; * ; 27 ; 4209409 ; 99.634,32</SPAN>
	</p>
	<p>Este arquivo ira fazer uma carga no indicador <b>22 (EMAP - Equipe de Apoio)</b>, 
	o <b>*</b> significa que não é necessário incluir cenário. A carga acontecera nos detalhes <b>26 (Quantidade EMAP)</b> e <b>27 (Custeio EAP)</b> no municipio 
	de <b>(4209409)Laguna-SC</b>. Para o detalhe 26 o formato do valor deve ser inteiro e do 27 o formato é Monetário.</p>
	</td>
</tr>

</table>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
<tr>
	<td class="SubTituloCentro">( ID ) Mapa - Indicador</td>
	<td class="SubTituloCentro">( ID ) Cenários</td>
	<td class="SubTituloCentro">&nbsp;</td>
</tr>
<?

$arrMapas = $db->carregar($sql);

if($arrMapas[0]) {
	foreach($arrMapas as $map) {
	?>
	<tr>
		<td style="border: 1px solid #000;"><?="( ".$map['indid']." ) ".$map['mapdsc']." - ".$map['inddsc'] ?></td>
		<td style="border: 1px solid #000;"><?
		$sql = "SELECT '( '||mpcid||' ) '||trim(mpcdsc) as reg FROM mapa.mapacenario d
				WHERE d.mapid='".$map['mapid']."' AND mpcstatus='A'";
		$cabecalho = array("( ID ) Cenário");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
		?></td>
		<td style="border: 1px solid #000;">
		<?
		$sql = "SELECT '( '||dtiid||' ) '||trim(dtidsc) as reg, trim(tpddsc)||' ( '||tpdformato||' )' as tipo FROM mapa.detalheindicador d 
				LEFT JOIN mapa.tipodado t ON t.tpdid = d.tpdid  
				WHERE d.indid='".$map['indid']."' ORDER BY dtiordem";
		$cabecalho = array("( ID ) Detalhe","Valor");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2); 
		?>
		</td>
	</tr>
	<?
	}
}
?>
</table>